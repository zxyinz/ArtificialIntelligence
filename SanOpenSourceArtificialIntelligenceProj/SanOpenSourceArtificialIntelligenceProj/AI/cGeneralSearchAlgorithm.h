#include"algorithm"
#include"functional"
#include"AIContainer.h"
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __cGeneralSearchAlgorithm_H__
#define __cGeneralSearchAlgorithm_H__

		enum eSEARCHALGORITHMTYPE
		{
			SAT_BREADTH_FIRST_SEARCH,
			SAT_DEPTH_FIRST_SEARCH,
			SAT_UNIFORM_COST_SEARCH,
			SAT_GREEDY_SEARCH,
			SAT_A_STAR_SEARCH,
		};

		template<class DType, class WType>
		class cGeneralSearchAlgorithm
		{
		protected:
			SEARCHTREE<DType, WType>	m_SearchTree;	// The whole tree which generated by search function
		protected:
			/*Path generate funciont*/
			virtual vector<uint32> _GeneratePathList(SEARCHTREE<DType, WType>* pTargetTreeNode, SString* pstrOutput = nullptr)
			{
				vector<uint32> PathList;
				SEARCHTREE<DType, WType>* pCurrentNode = pTargetTreeNode;
				while (!pCurrentNode->iIsRoot())
				{
					PathList.insert(PathList.begin(), pCurrentNode->iGetTreeID());
					pCurrentNode = pCurrentNode->iGetTrunkPtr();
				}
				::cout << "\r\n";
				if (pstrOutput != nullptr)
				{
					*pstrOutput = *pstrOutput + "\r\n\r\nEntire Tree:\r\n--------------------------------------------------\r\n";
				}
				this->_PrintTree(&(this->m_SearchTree), pstrOutput);
				if (pstrOutput != nullptr)
				{
					*pstrOutput = *pstrOutput + "\r\n\r\nPath:\r\n--------------------------------------------------\r\n";
				}
				this->_PrintPathList(PathList, pstrOutput);

				return PathList;
			}
			/*Actual search func, input is initial state, output is the path*/
			vector<uint32> _SearchCore(const DType &InitState, const uint32 MaxDepth, SString* pstrOutput = nullptr)
			{
				uint32 GlobalID = 1;
				uint32 Depth = 0;

				this->m_SearchTree.iClear();
				this->m_SearchTree.iSetData(InitState);
				this->m_SearchTree.iSetGlobalWeight(0);

				/*Min heap*/
				priority_queue < TREEPTR<DType, WType>, vector<TREEPTR<DType, WType>>, TreePtrCompare<DType, WType>> MinHeap;
				//priority_queue<TREEPTR<DType, WType>> MinHeap;

				/*Successful Path*/
				vector<uint32> Path;

				/*State History*/
				vector<DType> VisitedStateList;

				/*Generate Heap*/
				MinHeap.push(TREEPTR<DType, WType>(&this->m_SearchTree));

				/*Keep earch when there is no solution found or the heap is not empty*/
				while (!MinHeap.empty())
				{
					SEARCHTREE<DType, WType>* pCurrentNode = MinHeap.top().pPtr;

					MinHeap.pop();

					if (pCurrentNode == nullptr)
					{
						::cout << "Error: Invalid Pointer, nullptr\r\n";
						return  vector<uint32>(0);
					}

					/*Print visited node*/
					this->_PrintCurrentNode(pCurrentNode, GlobalID, pstrOutput);

					/*If current state is visited, stop generate*/
					bool bVisited = false;

					/* Lambda expression						*/
					//[pCurrentNode, &bVisited](DType &Data)
					//{
					//	if (Data == pCurrentNode->iGetData())
					//	{ 
					//		bVisited = true;
					//		return;
					//	}
					//}
					::_For_each(VisitedStateList.begin(), VisitedStateList.end(), [pCurrentNode, &bVisited](DType &Data){if (Data == pCurrentNode->iGetData()){ bVisited = true; return; }});
					if (bVisited)
					{
						if (pstrOutput != nullptr)
						{
							*pstrOutput = *pstrOutput + "   Visited\r\n";
						}
						::cout << "   Visited\r\n";
						continue;
					}
					if (pstrOutput != nullptr)
					{
						*pstrOutput = *pstrOutput + "\r\n";
					}
					::cout << "\r\n";

					/*Push current state to visited node list*/
					VisitedStateList.push_back(pCurrentNode->iGetData());

					/*Check if current state is goal state*/
					if (this->_IsGoalState(pCurrentNode))
					{
						/*Find solution, return the path*/
						return this->_GeneratePathList(pCurrentNode, pstrOutput);
					}

					/*If current depth is greater or equal with max depth, stop explore*/
					if ((MaxDepth != 0) && (pCurrentNode->iGetDepth() >= MaxDepth))
					{
						continue;
					}

					this->_GenerateSuccessorNodes(pCurrentNode, GlobalID);

					for (uint32 seek = 0; seek < pCurrentNode->iGetOffshootListSize(); seek = seek + 1)
					{
						MinHeap.push(TREEPTR<DType, WType>(pCurrentNode->iGetOffshootPtr(seek)));
					}

					GlobalID = GlobalID + pCurrentNode->iGetOffshootListSize();
				}

				/*No solution found, return empty path*/
				return vector<uint32>(0);
			};
			void _PrintTree(SEARCHTREE<DType, WType>* pTree, SString* pstrString = nullptr)
			{
				SString strOutput;

				if (pTree == nullptr)
				{
					return;
				}
				for (uint32 seek = 0; seek < pTree->iGetDepth(); seek = seek + 1)
				{
					strOutput = strOutput + "   ";
				}

				::cout << strOutput.c_str();

				if (pstrString != nullptr)
				{
					*pstrString = *pstrString + strOutput;
				}

				this->_PrintCurrentNode(pTree, 0, pstrString);

				::cout << "\r\n";

				if (pstrString != nullptr)
				{
					*pstrString = *pstrString + "\r\n";
				}

				for (uint32 seek = 0; seek < pTree->iGetOffshootListSize(); seek = seek + 1)
				{
					this->_PrintTree(pTree->iGetOffshootPtr(seek), pstrString);
				}
			};
		protected:

			/*Goal state test funciton*/
			virtual bool _IsGoalState(const SEARCHTREE<DType, WType>* pTreeNode) = 0;

			/*Node generate function*/
			virtual bool _GenerateSuccessorNodes(SEARCHTREE<DType, WType>* pTreeNode, const uint32 GlobalID) = 0;

			/*Node cost function*/
			virtual WType _CalcCurrentNodeCostVal(const SEARCHTREE<DType, WType>* pTreeNode, const uint32 GlobalID) = 0;

			/*H evaluate function*/
			virtual WType _EvaluateHeuristicVal(const SEARCHTREE<DType, WType>* pTreeNode, const uint32 GlobalID) = 0;

			/*Print function*/
			virtual void _PrintCurrentNode(const SEARCHTREE<DType, WType>* pTreeNode, const uint32 GlobalID, SString* pstrOutput = nullptr) = 0;

			virtual void _PrintPathList(const vector<uint32> PathList, SString* pstrOutput = nullptr) = 0;
		public:
			cGeneralSearchAlgorithm()
			{
			};
			~cGeneralSearchAlgorithm()
			{
			};

			/*Search interface fucntion, for user call				*/
			/*MaxDepth == 0, keep searching until find the solution	*/
			vector<uint32> iSearch(const DType &InitState, const uint32 MaxDepth = 0, SString* pstrOutput = nullptr)
			{
				return this->_SearchCore(InitState, MaxDepth, pstrOutput);
			};
		};
#endif
	}
}