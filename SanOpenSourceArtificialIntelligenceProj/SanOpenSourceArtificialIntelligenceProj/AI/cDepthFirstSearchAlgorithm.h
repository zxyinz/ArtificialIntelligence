#include"cGeneralSearchAlgorithm.h"
#pragma once
namespace San
{
	namespace AI
	{
		template<class DType>
		class cDepthFirstSearchAlgorithm : public cGeneralSearchAlgorithm<DType, COMPARABLEFLOAT<uint8>>
		{
		public:
		protected:
			/*Node cost function*/
			COMPARABLEFLOAT<uint8> _CalcCurrentNodeCostVal(const SEARCHTREE<DType, COMPARABLEFLOAT<uint8>>* pTreeNode, const uint32 GlobalID)
			{
				COMPARABLEFLOAT<uint8> BaseVal = pTreeNode->iGetTrunkPtr()->iGetGlobalWeight();
				BaseVal.Float.push_back(pTreeNode->iGetTreeID());
				return BaseVal;
			}

			/*H evaluate function*/
			COMPARABLEFLOAT<uint8> _EvaluateHeuristicVal(const SEARCHTREE<DType, COMPARABLEFLOAT<uint8>>* pTreeNode, const uint32 GlobalID)
			{
				return COMPARABLEFLOAT<uint8>();
			};

		public:
			cDepthFirstSearchAlgorithm(){};
			~cDepthFirstSearchAlgorithm(){};

			vector<uint32> iSearch(const DType &InitState, const uint32 MaxDepth = 0, SString* pstrOutput = nullptr)
			{
				return this->_SearchCore(InitState, MaxDepth, pstrOutput);
			};
		};

		template<class DType, class _SuccessorGenerateFunc, class _GoalStateTestFunc>
		class _cDepthFirstSearchAlgorithm : public cDepthFirstSearchAlgorithm<DType>
		{
		public:
		protected:
			bool _IsGoalState(const SEARCHTREE<DType, WType>* pTreeNode)
			{ 
				return _GoalStateTestFunc(pTreeNode); 
			};

			bool _GenerateSuccessorNodes(SEARCHTREE<DType, WType>* pTreeNode, const uint32 GlobalID)
			{ 
				return _SuccessorGenerateFunc(pTreeNode, GlobalID); 
			};
		public:
			_cDepthFirstSearchAlgorithm(){};
			~_cDepthFirstSearchAlgorithm(){};

			vector<uint32> iSearch(const DType &InitState, const uint32 MaxDepth = 0, SString* pstrOutput = nullptr)
			{
				return this->_SearchCore(InitState, MaxDepth, pstrOutput);
			};
		};
	}
}