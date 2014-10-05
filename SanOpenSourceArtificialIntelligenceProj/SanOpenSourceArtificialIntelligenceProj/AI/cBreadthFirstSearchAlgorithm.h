#include"cGeneralSearchAlgorithm.h"
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __CBREADTHFIRSTSEARCHALGORITHM_H__
#define __CBREADTHFIRSTSEARCHALGORITHM_H__
		template<class DType>
		class cBreadthFirstSearchAlgorithm : public cGeneralSearchAlgorithm<DType, uint32>
		{
		public:
		protected:
			/*Node cost function*/
			uint32 _CalcCurrentNodeCostVal(const SEARCHTREE<DType, uint32>* pTreeNode, const uint32 GlobalID)
			{
				return GlobalID;
			}

			/*H evaluate function*/
			uint32 _EvaluateHeuristicVal(const SEARCHTREE<DType, uint32>* pTreeNode, const uint32 GlobalID)
			{
				return 0;
			};

		public:
			cBreadthFirstSearchAlgorithm(){};
			~cBreadthFirstSearchAlgorithm(){};

			vector<uint32> iSearch(const DType &InitState, const uint32 MaxDepth = 0, SString* pstrOutput = nullptr)
			{
				return this->_SearchCore(InitState, MaxDepth, pstrOutput);
			};
		};
#endif
	}
}