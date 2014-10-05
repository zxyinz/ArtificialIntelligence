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
	}
}