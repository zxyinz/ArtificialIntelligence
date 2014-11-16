#include"queue"
#include"..\RelyON\Core\SanTypes.h"
#include"..\RelyOn\Core\SanContainer.h"
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __STDAICONTAINER_H__
#define __STDAICONTAINER_H__

		/*Search tree defination*/
		template<class DType, class WType>
		struct SEARCHTREE
		{
		private:
			uint32								ID;						// Local ID
			DType								Data;					// Data, for AI stand for current node state
			uint32								Depth;					// Current depth
			SEARCHTREE<DType, WType>*			pTrunk;					// Trunk pointer, if null, root node
			vector<SEARCHTREE<DType, WType>*>	OffshootPtrArray;		// Offshoot pointer array, if empty, leaf node
			vector<WType>						OffshootWeightArray;	// Offshoot weight array
			WType								GlobalWeight;			// Glboal weight, for build the heap
			bool								bCopy;					// For release
		public:
			SEARCHTREE(const uint32 ID = 0, SEARCHTREE<DType, WType>* pTrunk = nullptr)
				:ID(ID), Depth(0), pTrunk(pTrunk), GlobalWeight(0), bCopy(false)
			{
				this->OffshootPtrArray.clear();
			};

			SEARCHTREE(const uint32 ID, const DType &Data, const uint32 Depth, SEARCHTREE<DType, WType>* pTrunk)
				:ID(ID), Data(Data), Depth(Depth), pTrunk(pTrunk), GlobalWeight(0), bCopy(false)
			{
				this->OffshootPtrArray.clear();
			};

			SEARCHTREE(const SEARCHTREE<DType, WType> &Tree)
				:ID(Tree.ID), Data(Tree.Data), Depth(Tree.Depth), pTrunk(Tree.pTrunk), GlobalWeight(Tree.GlobalWeight), bCopy(true)
			{
				this->OffshootPtrArray = Tree.OffshootPtrArray;
				this->OffshootWeightArray = Tree.OffshootWeightArray;
			};

			~SEARCHTREE()
			{
				if (this->bCopy)
				{
					return;
				}
				if (this->pTrunk != nullptr)
				{
					this->pTrunk->OffshootPtrArray[this->ID] = nullptr;
					//this->pTrunk->iDeleteOffshootNode(this->ID);
				}
				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						this->OffshootPtrArray[seek] = nullptr;
					}
				}
				this->ID = 0;
				this->OffshootPtrArray.clear();
				this->OffshootWeightArray.clear();
				this->pTrunk = nullptr;
			};

			/*Operator Ovarload*/
			SEARCHTREE<DType, WType>& operator=(const SEARCHTREE<DType, WType> Tree)
			{
				this->ID = Tree.ID;
				this->Data = Tree.Data;
				this->Depth = Tree.Depth;
				this->pTrunk = Tree->pTrunk;
				this->OffshootPtrArray = Tree->OffshootPtrArray;
				this->OffshootWeightArray = Tree->OffshootWeightArray;
				this->GlobalWeight = Tree.GlobalWeight;
				this->bCopy = true;

				return *this;
			};

			const SEARCHTREE<DType, WType>& operator[](const  size_t Position) const
			{
				return *this->OffshootPtrArray[Position];
			};

			SEARCHTREE<DType, WType>& operator[](const  size_t Position)
			{
				return *this->OffshootPtrArray[Position];
			};

			/*For heap use*/
			bool operator()(const SEARCHTREE<DType, WType> &Tree1, const SEARCHTREE<DType, WType> &Tree2)
			{
				return Tree1.GlobalWeight < Tree2.GlobalWeight;
			};

			bool operator<(const SEARCHTREE<DType, WType> &Tree)
			{
				return this->GlobalWeight < Tree.GlobalWeight;
			};

			/*Basic tree functions*/
			bool iSetTreeID(const uint32 ID)
			{
				this->ID = ID;
				return true;
			};
			bool iSetData(const DType &Data)
			{
				this->Data = Data;
				return true;
			};
			bool iSetDepth(const uint32 &Depth)
			{
				this->Depth = Depth;
				return true;
			};
			bool iSetTrunkPtr(SEARCHTREE<DType, WType>* pTrunk)
			{
				if (this->bCopy)
				{
					return false;
				}
				this->pTrunk = pTrunk;
				return true;
			};
			bool iReSizeOffshootListSize(const uint32 Size, const DType &DefaultData, const WType &DefaultWeight)
			{
				if (this->bCopy)
				{
					return false;
				}
				if (this->OffshootPtrArray.size() == Size)
				{
					return true;
				}

				if (this->OffshootPtrArray.size() < Size)
				{
					this->OffshootPtrArray.resize(Size);
					this->OffshootWeightArray.resize(Size);

					for (uint32 seek = this->OffshootPtrArray.size(); seek < Size; seek = seek + 1)
					{
						this->OffshootPtrArray[seek] = new SEARCHTREE<DType, WType>(seek, DefaultData, this->Depth + 1, this);
						this->OffshootWeightArray[seek] = DefaultWeight;
					}
					return true;
				}
				else
				{
					for (uint32 seek = (Size - 1) > 0 ? (Size - 1) : 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
					{
						if (this->OffshootPtrArray[seek] != nullptr)
						{
							delete this->OffshootPtrArray[seek];
							this->OffshootPtrArray[seek] = nullptr;
						}
					}
					this->OffshootPtrArray.resize(Size);
					this->OffshootWeightArray.resize(Size);
					return true;
				}
			};
			SEARCHTREE<DType, WType>* iCreateOffshootNode(const DType &Data, const WType &Weight)
			{
				if (this->bCopy)
				{
					return nullptr;
				}

				SEARCHTREE<DType, WType>* pNode = new  SEARCHTREE<DType, WType>(this->OffshootPtrArray.size(), Data, this->Depth + 1, this);
				this->OffshootPtrArray.push_back(pNode);
				this->OffshootWeightArray.push_back(Weight);
				return pNode;
			};
			SEARCHTREE<DType, WType>* iInsertOffshootNode(const uint32 ID, const DType &Data, const WType &Weight)
			{
				if (this->bCopy)
				{
					return nullptr;
				}

				return this->iInsertOffshootNode(ID, new SEARCHTREE<DType, WType>(ID, Data, this->Depth + 1, this), Weight);
			}
			SEARCHTREE<DType, WType>* iInsertOffshootNode(const uint32 ID, SEARCHTREE<DType, WType>* pTreeNode, const WType &Weight)
			{
				if (this->bCopy)
				{
					return nullptr;
				}

				if (this->OffshootPtrArray.size() < ID)
				{
					return nullptr;
				}

				/*Search for the target node*/
				vector<SEARCHTREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
				vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
				for (uint32 seek = 0; seek < ID; seek = seek + 1)
				{
					pItem++;
					pWeightItem++;
				}

				/*Insert*/
				this->OffshootPtrArray.insert(pItem, pTreeNode);
				this->OffshootWeightArray.insert(pWeightItem, Weight);

				/*Adjust the local ID for rest offshoot nodes*/
				for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					this->OffshootPtrArray[seek]->ID = seek;
				}
				return this->OffshootPtrArray[ID];
			};
			bool iDeleteOffshootNode(const uint32 ID)
			{
				if (this->bCopy)
				{
					return false;
				}

				if (this->OffshootPtrArray.size() <= ID)
				{
					return false;
				}

				/*If target node exist, delete the target node*/
				if (this->OffshootPtrArray[ID].pPtr != nullptr)
				{
					delete this->OffshootPtrArray[ID].pPtr;
					this->OffshootPtrArray[ID] = nullptr;
				}

				/*Erase the target node from the array*/
				vector<SEARCHTREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
				vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
				for (uint32 seek = 0; seek < ID; seek = seek + 1)
				{
					pItem++;
					pWeightItem++;
				}
				this->OffshootPtrArray.erase(pItem);
				this->OffshootWeightArray.erase(pWeightItem);

				/*Adjust the local ID for rest offshoot nodes*/
				for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					this->OffshootPtrArray[seek].pPtr->ID = seek;
				}
			};
			void iClear()
			{
				if (this->bCopy)
				{
					return;
				}

				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						this->OffshootPtrArray[seek] = nullptr;
					}
				}
				this->OffshootPtrArray.clear();
				this->OffshootWeightArray.clear();
			}
			bool iSetOffshootWeight(const uint32 ID, const WType Weight)
			{
				if (ID >= this->OffshootWeightArray.size())
				{
					return false;
				}
				this->OffshootWeightArray[ID] = Weight;
				return true;
			}
			WType iSetGlobalWeight(WType GlobalWeight)
			{
				this->GlobalWeight = GlobalWeight;
				return this->GlobalWeight;
			}
			const uint32 iGetTreeID() const
			{
				return this->ID;
			};
			const DType iGetDataConst() const
			{
				return Data;
			};
			DType iGetData() const
			{
				return Data;
			};
			const DType* iGetDataPtrConst() const
			{
				return &Data;
			};
			DType* iGetDataPtr()
			{
				return &Data;
			};
			uint32 iGetDepth() const
			{
				return Depth;
			}
			const SEARCHTREE<DType, WType>* iGetTrunkPtrConst() const
			{
				return this->pTrunk;
			};
			SEARCHTREE<DType, WType>* iGetTrunkPtr() const
			{
				return this->pTrunk;
			};
			const uint32 iGetOffshootListSize() const
			{
				return this->OffshootPtrArray.size();
			};
			const SEARCHTREE<DType, WType>* iGetOffshootPtrConst(const uint32 ID) const
			{
				if (ID >= this->OffshootPtrArray.size())
				{
					return nullptr;
				}
				return this->OffshootPtrArray[ID];
			};
			SEARCHTREE<DType, WType>* iGetOffshootPtr(const uint32 ID) const
			{
				if (ID >= this->OffshootPtrArray.size())
				{
					return nullptr;
				}
				return this->OffshootPtrArray[ID];
			};
			WType iGetOffshootWeight(const uint32 ID) const
			{
				if (ID >= this->OffshootWeightArray.size())
				{
					return 0.0;
				}
				return this->OffshootWeightArray[ID];
			}
			WType iGetGlobalWeight() const
			{
				return this->GlobalWeight;
			}
			bool iIsRoot() const
			{
				return (this->pTrunk == nullptr);
			}
			bool iIsLeaf() const
			{
				return this->OffshootPtrArray.empty();
			}
		};

		template<class DType, class WType>
		struct TREEPTR
		{
		public:
			SEARCHTREE<DType, WType>* pPtr;
		public:
			TREEPTR(SEARCHTREE<DType, WType>* pPtr)
				:pPtr(pPtr)
			{};
			~TREEPTR(){};
		};

		template<class DType, class WType>
		class TreePtrCompare
		{
		public:
			bool operator()(TREEPTR<DType, WType> LPtr, TREEPTR<DType, WType> RPtr)
			{
				return LPtr.pPtr->iGetGlobalWeight()>RPtr.pPtr->iGetGlobalWeight();
			}
		};


		/*General tree node structure defination*/
		template<class DType>
		struct TREENODE
		{
		public:
			TREENODE(){};
			~TREENODE(){};

			virtual SString iPrintNode(const TREENODE &Node) const { return SString(); };
		};

		/*General tree structure defination*/
		template<class DType, class WType = sfloat>
		struct TREE
		{
		private:
			uint32						ID;						// Local ID
			DType						Data;					// Data, for AI stand for current node state
			uint32						Depth;					// Current depth
			TREE<DType, WType>*			pTrunk;					// Trunk pointer, if null, root node
			vector<TREE<DType, WType>*>	OffshootPtrArray;		// Offshoot pointer array, if empty, leaf node
			vector<WType>				OffshootWeightArray;	// Offshoot weight array
			WType						GlobalWeight;			// Glboal weight, for build the heap
		protected:
			TREE<DType, WType>& operator=(const TREE<DType, WType> Tree);
			TREE(const TREE<DType, WType> &Tree);

			void _Reshape(const uint32 ID, const uint32 Depth)
			{
				this->ID = ID;
				this->Depth = Depth;

				vector<TREE<DType, WType>*>::iterator pOffshoot = this->OffshootPtrArray.begin();
				vector<WType>::iterator pWeight = this->OffshootWeightArray.begin();
				while (pOffshoot != this->OffshootPtrArray.end())
				{
					if ((*pOffshoot) == nullptr)
					{
						pOffshoot = this->OffshootPtrArray.erase(pOffshoot);
						pWeight = this->OffshootWeightArray.erase(pWeight);
					}
					else
					{
						pOffshoot++;
						pWeight++;
					}
				}
				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					this->OffshootPtrArray[seek]->_Reshape(seek, this->Depth + 1);
				}
			}
		public:
			TREE(const uint32 ID = 0, TREE<DType, WType>* pTrunk = nullptr)
				:ID(ID), Depth(0), pTrunk(pTrunk), GlobalWeight(0)
			{
			};

			TREE(const uint32 ID, const DType &Data, const uint32 Depth, TREE<DType, WType>* pTrunk)
				:ID(ID), Data(Data), Depth(Depth), pTrunk(pTrunk), GlobalWeight(0)
			{
			};

			~TREE()
			{
				if (this->pTrunk != nullptr)
				{
					this->pTrunk->OffshootPtrArray[this->ID] = nullptr;
				}

				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						this->OffshootPtrArray[seek] = nullptr;
					}
				}
				this->ID = 0;
				this->OffshootPtrArray.clear();
				this->OffshootWeightArray.clear();

				this->pTrunk = nullptr;
			};

			/*Operator Ovarload*/
			const TREE<DType, WType>& operator[](const  size_t Position) const
			{
				if (this->OffshootPtrArray[Position] == nullptr){ ::cout << "Error: Tree access invalid node\n"; }
				return *this->OffshootPtrArray[Position];
			};

			TREE<DType, WType>& operator[](const  size_t Position)
			{
				if (this->OffshootPtrArray[Position] == nullptr){ ::cout << "Error: Tree access invalid node\n"; }
				return *this->OffshootPtrArray[Position];
			};

			/*For heap use*/
			bool operator()(const TREE<DType, WType> &Tree1, const TREE<DType, WType> &Tree2)
			{
				return Tree1.GlobalWeight < Tree2.GlobalWeight;
			};

			bool operator<(const TREE<DType, WType> &Tree)
			{
				return this->GlobalWeight < Tree.GlobalWeight;
			};

			bool operator==(const TREE<DType, WType> &Tree)
			{
				if (this->Data != Tree.Data){ return false; }
				if (this->GlobalWeight != Tree.GlobalWeight){ return false; }
				if (this->OffshootPtrArray.size() != Tree.OffshootPtrArray.size()){ return false; }
				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootWeightArray[seek] != Tree.OffshootWeightArray[seek]){ return false; }
					if ((*this->OffshootPtrArray[seek]) != (*Tree.OffshootPtrArray[seek])){ return false; }
				}
				return true;
			};

			bool operator!=(const TREE<DType, WType> &Tree)
			{
				return !(*this == Tree);
			};

			/*Basic tree functions*/
			void iSetID(const uint32 ID)
			{
				this->ID = ID;
			};

			void iSetData(const DType &Data)
			{
				this->Data = Data;
			};

			bool iResize(const uint32 Size, const DType &DefaultData, const WType &DefaultWeight)
			{
				if (this->OffshootPtrArray.size() == Size)
				{
					return true;
				}

				if (this->OffshootPtrArray.size() < Size)
				{
					this->OffshootPtrArray.resize(Size);
					this->OffshootWeightArray.resize(Size);

					for (uint32 seek = this->OffshootPtrArray.size(); seek < Size; seek = seek + 1)
					{
						this->OffshootPtrArray[seek] = new TREE<DType, WType>(seek, DefaultData, this->Depth + 1, this);
						this->OffshootWeightArray[seek] = DefaultWeight;
					}
					return true;
				}
				else
				{
					for (uint32 seek = (Size - 1) > 0 ? (Size - 1) : 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
					{
						if (this->OffshootPtrArray[seek] != nullptr)
						{
							delete this->OffshootPtrArray[seek];
							//this->OffshootPtrArray[seek] = nullptr;
						}
					}
					this->OffshootPtrArray.resize(Size);
					this->OffshootWeightArray.resize(Size);
					return true;
				}
			};

			TREE<DType, WType>* iCreate(const DType &Data, const WType &Weight)
			{
				TREE<DType, WType>* pNode = new  TREE<DType, WType>(this->OffshootPtrArray.size(), Data, this->Depth + 1, this);
				this->OffshootPtrArray.push_back(pNode);
				this->OffshootWeightArray.push_back(Weight);
				return pNode;
			};

			TREE<DType, WType>* iCreate(const TREE<DType, WType> &Tree)
			{
				TREE<DType, WType>* pNode = Tree.iCopy();

				pNode->pTrunk = this;

				pNode->_Reshape(this->OffshootPtrArray.size(), this->Depth + 1);

				this->OffshootPtrArray.push_back(pNode);
				this->OffshootWeightArray.push_back(Weight);
				return pNode;
			};

			TREE<DType, WType>* iInsert(const uint32 ID, const DType &Data, const WType &Weight)
			{
				if (this->OffshootPtrArray.size() < ID)
				{
					return nullptr;
				}

				/*Search for the target node*/
				vector<TREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
				vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
				for (uint32 seek = 0; seek < ID; seek = seek + 1)
				{
					pItem++;
					pWeightItem++;
				}

				/*Insert*/
				TREE<DType, WType>* pTreeNode = new TREE<DType, WType>(ID, Data, this->Depth, this);

				this->OffshootPtrArray.insert(pItem, pTreeNode);
				this->OffshootWeightArray.insert(pWeightItem, Weight);

				/*Adjust the local ID for rest offshoot nodes*/
				for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					this->OffshootPtrArray[seek]->ID = seek;
				}

				this->OffshootPtrArray[ID]->_Reshape(ID, this->Depth + 1);

				return this->OffshootPtrArray[ID];
			};

			TREE<DType, WType>* iInsert(const uint32 ID, const TREE<DType, WType> &TreeNode, const WType &Weight)
			{
				if (this->OffshootPtrArray.size() < ID)
				{
					return nullptr;
				}

				/*Search for the target node*/
				vector<TREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
				vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
				for (uint32 seek = 0; seek < ID; seek = seek + 1)
				{
					pItem++;
					pWeightItem++;
				}

				/*Insert*/
				TREE<DType, WType>* pTreeNode = TreeNode.iCopy();

				pTreeNode->pTrunk = this;

				this->OffshootPtrArray.insert(pItem, pTreeNode);
				this->OffshootWeightArray.insert(pWeightItem, Weight);

				/*Adjust the local ID for rest offshoot nodes*/
				for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					this->OffshootPtrArray[seek]->ID = seek;
				}

				this->OffshootPtrArray[ID]->_Reshape(ID, this->Depth + 1);

				return this->OffshootPtrArray[ID];
			};

			bool iDelete(const uint32 ID)
			{

				if (this->OffshootPtrArray.size() <= ID)
				{
					return false;
				}

				/*If target node exist, delete the target node*/
				if (this->OffshootPtrArray[ID] != nullptr)
				{
					delete this->OffshootPtrArray[ID];
					this->OffshootPtrArray[ID] = nullptr;
				}

				/*Erase the target node from the array*/
				vector<TREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
				vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
				for (uint32 seek = 0; seek < ID; seek = seek + 1)
				{
					pItem++;
					pWeightItem++;
				}
				this->OffshootPtrArray.erase(pItem);
				this->OffshootWeightArray.erase(pWeightItem);

				/*Adjust the local ID for rest offshoot nodes*/
				for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					this->OffshootPtrArray[seek]->ID = seek;
				}
			};

			void iCopy(TREE<DType, WType> &Tree) const
			{
				Tree.Data = this->Data;
				Tree.pTrunk = nullptr;// this->pTrunk;

				pTree->OffshootPtrArray.resize(this->OffshootPtrArray.size());
				for (uint32 seek = 0; seek < pTree->OffshootPtrArray.size(); seek = seek + 1)
				{
					pTree->OffshootPtrArray[seek] = this->OffshootPtrArray[seek]->iCopy();
				}
				pTree->OffshootWeightArray = this->OffshootWeightArray;
				pTree->GlobalWeight = this->GlobalWeight;

				Tree._Reshape(0, 0);
			};

			TREE<DType, WType>* iCopy() const
			{
				TREE<DType, WType>* pTree = new TREE<DType, WType>();

				pTree->Data = this->Data;
				pTree->Depth = this->Depth;

				pTree->OffshootPtrArray.resize(this->OffshootPtrArray.size());
				for (uint32 seek = 0; seek < pTree->OffshootPtrArray.size(); seek = seek + 1)
				{
					pTree->OffshootPtrArray[seek] = this->OffshootPtrArray[seek]->iCopy();
				}
				pTree->OffshootWeightArray = this->OffshootWeightArray;
				pTree->GlobalWeight = this->GlobalWeight;

				pTree->_Reshape(0, 0);
				return pTree;
			};

			void iClear(bool bDeleteObject = true)
			{
				if (bDeleteObject)
				{
					for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
					{
						if (this->OffshootPtrArray[seek] != nullptr)
						{
							delete this->OffshootPtrArray[seek];
							this->OffshootPtrArray[seek] = nullptr;
						}
					}
				}
				this->OffshootPtrArray.clear();
				this->OffshootWeightArray.clear();
			};

			bool iSetOffshootWeight(const uint32 ID, const WType Weight)
			{
				if (ID >= this->OffshootWeightArray.size())
				{
					return false;
				}
				this->OffshootWeightArray[ID] = Weight;
				return true;
			};

			WType iSetGlobalWeight(WType GlobalWeight)
			{
				this->GlobalWeight = GlobalWeight;
				return this->GlobalWeight;
			};

			const uint32 iGetID() const
			{
				return this->ID;
			};

			DType iGetData() const
			{
				return Data;
			};

			const DType* iGetDataPtrConst() const
			{
				return &Data;
			};

			DType* iGetDataPtr()
			{
				return &Data;
			};

			uint32 iGetDepth() const
			{
				return Depth;
			};

			const TREE<DType, WType>* iGetTrunkPtrConst() const
			{
				return this->pTrunk;
			};

			TREE<DType, WType>* iGetTrunkPtr() const
			{
				return this->pTrunk;
			};

			const uint32 iGetSize() const
			{
				return this->OffshootPtrArray.size();
			};

			const TREE<DType, WType>* iGetOffshootPtrConst(const uint32 ID) const
			{
				if (ID >= this->OffshootPtrArray.size())
				{
					return nullptr;
				}
				return this->OffshootPtrArray[ID];
			};

			TREE<DType, WType>* iGetOffshootPtr(const uint32 ID) const
			{
				if (ID >= this->OffshootPtrArray.size())
				{
					return nullptr;
				}
				return this->OffshootPtrArray[ID];
			};

			WType iGetOffshootWeight(const uint32 ID) const
			{
				if (ID >= this->OffshootWeightArray.size())
				{
					return 0.0;
				}
				return this->OffshootWeightArray[ID];
			};

			WType iGetGlobalWeight() const
			{
				return this->GlobalWeight;
			};

			uint32 iGetRelativeMaxDepth() const
			{
				//this->_Reshape(this->ID, this->Depth);

				uint32 MaxDepth = 0;

				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					uint32 CurrentDepth = this->OffshootPtrArray[seek]->iGetRelativeMaxDepth() + 1;
					if (CurrentDepth>MaxDepth)
					{
						MaxDepth = CurrentDepth;
					}
				}

				return MaxDepth;// -this->Depth + 1;
			};

			SString iPrintTree(const SString &strBaseString = _SSTR("   ")) const
			{
				SString strOutput;

				for (uint32 seek = 0; seek < this->Depth; seek = seek + 1)
				{
					strOutput = strOutput + strBaseString;
				}

				strOutput = strOutput + this->Data.iPrintNode() + _SSTR("\r\n");

				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					strOutput = strOutput + this->OffshootPtrArray[seek]->iPrintTree();
				}

				return strOutput;
			}

			bool iIsRoot() const
			{
				return (this->pTrunk == nullptr);
			};

			bool iIsLeaf() const
			{
				return this->OffshootPtrArray.empty();
			};
		};
#endif
	}
}