#include"..\RelyOn\Core\SanMathematics.h"
#include"cGeneralSearchAlgorithm.h"
using namespace std;
using namespace San::Mathematics;
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __CMINMAXALGORITHM_H__
#define __cMINMAXALGORITHM_H__

		enum eMINMAXSEARCHTYPE
		{
			MMST_BASIC,
			MMST_ALPHA_BETA_PRUNING,
			MMST_ALPHA_BETA_PRUNING_PARALLEL, // not finish
		};

		template<class DType, class RType, class UType>
		class cMinMaxAlgorithm
		{
		protected:
			SEARCHTREE<DType, sfloat>	m_SearchTree;	// The whole tree which generated by search function
			pair<DType, sfloat>			m_OptimalState;	// The optimal state get by search
			UType						m_UserData;
		protected:
#pragma region Basic Min Max Search Algorithm
			sfloat _MinSearch(SEARCHTREE<DType, sfloat>* pTreeNode, const uint32 CurrentDepth, const uint32 MaxSearchDepth, const RType RestrictionValue)
			{
				/*Maxdepth*/
				if (CurrentDepth >= (MaxSearchDepth == 0 ? CurrentDepth + 1 : MaxSearchDepth))
				{
					return this->_EvaluateState(pTreeNode);
				}

				/*Out of user def restriction value, for this project, time*/
				if (this->_BoundaryCheck(RestrictionValue))
				{
					return this->_EvaluateState(pTreeNode);
				}

				/*Get all possible offshoot*/
				uint32 Size = this->_GenerateStates(pTreeNode, MMST_BASIC);

				sfloat MinVal = MAX_FLOAT_VALUE;

				/*Call _MaxSearch, select min to return*/
				for (uint32 seek = 0; seek < Size; seek = seek + 1)
				{
					SEARCHTREE<DType, sfloat>* pCurrentNode = pTreeNode->iGetOffshootPtr(seek);

					sfloat CurrentWeight = pCurrentNode->iSetGlobalWeight(this->_MaxSearch(pCurrentNode, CurrentDepth + 1, MaxSearchDepth, RestrictionValue));

					if (MinVal > CurrentWeight)
					{
						MinVal = CurrentWeight;
					}
				}

				return MinVal;
			};
			sfloat _MaxSearch(SEARCHTREE<DType, sfloat>* pTreeNode, const uint32 CurrentDepth, const uint32 MaxSearchDepth, const RType RestrictionValue)
			{
				if (CurrentDepth >= (MaxSearchDepth == 0 ? CurrentDepth + 1 : MaxSearchDepth))
				{
					return this->_EvaluateState(pTreeNode);
				}
				if (this->_BoundaryCheck(RestrictionValue))
				{
					return this->_EvaluateState(pTreeNode);
				}

				uint32 Size = this->_GenerateStates(pTreeNode, MMST_BASIC);

				sfloat MaxVal = -MAX_FLOAT_VALUE;

				/*Call _MinSearch, select min to return*/
				for (uint32 seek = 0; seek < Size; seek = seek + 1)
				{
					SEARCHTREE<DType, sfloat>* pCurrentNode = pTreeNode->iGetOffshootPtr(seek);

					sfloat CurrentWeight = pCurrentNode->iSetGlobalWeight(this->_MinSearch(pCurrentNode, CurrentDepth + 1, MaxSearchDepth, RestrictionValue));

					if (MaxVal < CurrentWeight)
					{
						MaxVal = CurrentWeight;
					}
				}

				return MaxVal;
			};
#pragma endregion

#pragma region Alpha Beta Pruning Search Algorithm
			sfloat _MinAlphaBetaPruningSearch(SEARCHTREE<DType, sfloat>* pTreeNode, sfloat Alpha, sfloat Beta, const uint32 CurrentDepth, const uint32 MaxSearchDepth, const RType RestrictionValue)
			{
				if (CurrentDepth >= (MaxSearchDepth == 0 ? CurrentDepth + 1 : MaxSearchDepth))
				{
					return this->_EvaluateState(pTreeNode);
				}
				if (this->_BoundaryCheck(RestrictionValue))
				{
					return this->_EvaluateState(pTreeNode);
				}

				uint32 Size = this->_GenerateStates(pTreeNode, MMST_ALPHA_BETA_PRUNING);

				sfloat MinVal = MAX_FLOAT_VALUE;

				for (uint32 seek = 0; seek < Size; seek = seek + 1)
				{
					SEARCHTREE<DType, sfloat>* pCurrentNode = pTreeNode->iGetOffshootPtr(seek);

					sfloat CurrentWeight = pCurrentNode->iSetGlobalWeight(this->_MaxAlphaBetaPruningSearch(pCurrentNode, Alpha, Beta, CurrentDepth + 1, MaxSearchDepth, RestrictionValue));

					/*Update MinVal*/
					if (MinVal > CurrentWeight)
					{
						MinVal = CurrentWeight;
						Beta = MinVal;
					}

					/*If current min value is smaller or equal than alpha, pruning*/
					if (MinVal <= Alpha)
					{
						//::cout << "Pruning\n";
						return MinVal;
					}
				}

				return MinVal;
			};
			sfloat _MaxAlphaBetaPruningSearch(SEARCHTREE<DType, sfloat>* pTreeNode, sfloat Alpha, sfloat Beta, const uint32 CurrentDepth, const uint32 MaxSearchDepth, const RType RestrictionValue)
			{
				if (CurrentDepth >= (MaxSearchDepth == 0 ? CurrentDepth + 1 : MaxSearchDepth))
				{
					return this->_EvaluateState(pTreeNode);
				}
				if (this->_BoundaryCheck(RestrictionValue))
				{
					return this->_EvaluateState(pTreeNode);
				}

				uint32 Size = this->_GenerateStates(pTreeNode, MMST_ALPHA_BETA_PRUNING);

				sfloat MaxVal = -MAX_FLOAT_VALUE;

				for (uint32 seek = 0; seek < Size; seek = seek + 1)
				{
					SEARCHTREE<DType, sfloat>* pCurrentNode = pTreeNode->iGetOffshootPtr(seek);

					sfloat CurrentWeight = pCurrentNode->iSetGlobalWeight(this->_MinAlphaBetaPruningSearch(pCurrentNode, Alpha, Beta, CurrentDepth + 1, MaxSearchDepth, RestrictionValue));

					/*Update MaxVal*/
					if (MaxVal < CurrentWeight)
					{
						MaxVal = CurrentWeight;
						Alpha = MaxVal;
					}

					/*If current max value is greater or equal than beta, pruning*/
					if (MaxVal >= Beta)
					{
						//::cout << "Pruning\n";
						return MaxVal;
					}
					//::cout << CurrentDepth << "\t" << pTreeNode->iGetOffshootWeight(seek) << "\n";
				}

				return MaxVal;
			};
#pragma endregion
		protected:
			/*Generate the avaliable state*/
			virtual uint32 _GenerateStates(SEARCHTREE<DType, sfloat>* pTreeNode, const eMINMAXSEARCHTYPE Type) = 0;

			/*Evaluate current state*/
			virtual sfloat _EvaluateState(SEARCHTREE<DType, sfloat>* pTreeNode) = 0;

			/*Quick evaluate current state, for alpha - beta pruning to sort*/
			virtual sfloat _QuickEvaluateState(SEARCHTREE<DType, sfloat>* pTreeNode) = 0;

			/*Check the restriction boundary*/
			virtual bool _BoundaryCheck(const RType &Value) = 0;
		public:
			cMinMaxAlgorithm()
			{
				this->m_SearchTree.iSetTreeID(0);
				this->m_SearchTree.iSetDepth(0);
				this->m_SearchTree.iSetTrunkPtr(nullptr);
				this->m_SearchTree.iSetGlobalWeight(0);
			};
			~cMinMaxAlgorithm()
			{
				this->m_SearchTree.iClear();
			};
			DType iMinMaxSearch(const DType &InitState, const uint32 MaxSearchDepth, const RType &RestrictionValue, UType UserData)
			{
				this->m_UserData = UserData;

				this->m_OptimalState.first = InitState;
				this->m_OptimalState.second = -MAX_FLOAT_VALUE;

				/*Use Limit - Depth to search optimal solution*/
				for (uint32 seek = 1; seek <= (MaxSearchDepth == 0 ? seek : MaxSearchDepth); seek = seek + 1)
				{
					::cout << "\nLayer #" << seek;

					if ((MaxSearchDepth == 0) && (this->_BoundaryCheck(RestrictionValue)))
					{
						return this->m_OptimalState.first;
					}

					this->m_SearchTree.iClear();
					this->m_SearchTree.iSetData(InitState);

					/*For seek, search seek depth use Min-Max Algorithm (For first iteration, only search one layer)*/
					sfloat MaxVal = this->_MaxSearch(&this->m_SearchTree, 0, seek, RestrictionValue);

					::cout << "\tdone";

					/*Not better than optimal, continue*/
					if (MaxVal <= this->m_OptimalState.second)
					{
						continue;
					}

					uint32 Size = this->m_SearchTree.iGetOffshootListSize();

					/*Find optimal move*/
					for (uint32 seek_optimal = 0; seek_optimal < Size; seek_optimal = seek_optimal + 1)
					{
						SEARCHTREE<DType, sfloat>* pCurrentNode = this->m_SearchTree.iGetOffshootPtr(seek_optimal);

						if (::gloIsFloatEqual(pCurrentNode->iGetGlobalWeight(), MaxVal))
						{
							this->m_OptimalState.first = pCurrentNode->iGetDataConst();
							this->m_OptimalState.second = MaxVal;
						}
					}
				}

				return this->m_OptimalState.first;
			};
			DType iAlphaBetaPruningSearch(const DType &InitState, const uint32 MaxSearchDepth, const RType &RestrictionValue, UType UserData)
			{
				this->m_UserData = UserData;

				this->m_OptimalState.first = InitState;
				this->m_OptimalState.second = -MAX_FLOAT_VALUE;

				for (uint32 seek = 1; seek <= (MaxSearchDepth == 0 ? seek : MaxSearchDepth); seek = seek + 1)
				{
					::cout << "\nLayer #" << seek;

					if ((MaxSearchDepth == 0) && (this->_BoundaryCheck(RestrictionValue)))
					{
						return this->m_OptimalState.first;
					}

					this->m_SearchTree.iClear();
					this->m_SearchTree.iSetData(InitState);

					/*For seek, search seek depth use Alpha - Beta Pruning Algorithm (For first iteration, only search one layer)*/
					sfloat MaxVal = this->_MaxAlphaBetaPruningSearch(&this->m_SearchTree, -MAX_FLOAT_VALUE, MAX_FLOAT_VALUE, 0, seek, RestrictionValue);

					::cout << "\tdone";

					if (MaxVal <= this->m_OptimalState.second)
					{
						continue;
					}

					uint32 Size = this->m_SearchTree.iGetOffshootListSize();

					for (uint32 seek_optimal = 0; seek_optimal < Size; seek_optimal = seek_optimal + 1)
					{
						SEARCHTREE<DType, sfloat>* pCurrentNode = this->m_SearchTree.iGetOffshootPtr(seek_optimal);

						if (::gloIsFloatEqual(pCurrentNode->iGetGlobalWeight(), MaxVal))
						{
							this->m_OptimalState.first = pCurrentNode->iGetDataConst();
							this->m_OptimalState.second = MaxVal;
						}
					}
				}

				return this->m_OptimalState.first;
			};
		};
#endif
	}
}