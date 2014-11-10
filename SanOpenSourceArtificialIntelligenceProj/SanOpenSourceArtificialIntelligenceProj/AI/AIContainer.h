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
#endif
	}
}