#include"cGeneralSearchAlgorithm.h"
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __CUNIFORMSEARCHALGORITHM_H__
#define __CUNIFORMSEARCHALGORITHM_H__
		template<class DType,class WType>
		class cUniformSearchAlgorithm : public cGeneralSearchAlgorithm<DType, WType>
		{
		public:
		protected:
			/*H evaluate function*/
			WType _EvaluateHeuristicVal(const SEARCHTREE<DType, WType>* pTreeNode, const uint32 GlobalID)
			{
				return 0;
			};

		public:
			cUniformSearchAlgorithm(){};
			~cUniformSearchAlgorithm(){};

			vector<uint32> iSearch(const DType &InitState, const uint32 MaxDepth = 0, SString* pstrOutput = nullptr)
			{
				return this->_SearchCore(InitState, MaxDepth, pstrOutput);
			};
		};
#endif
	}
}