#include"..\AI\cGeneralSearchAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::AI;
#ifndef __CJUGSPROBLEM_H__
#define __CJUGSPROBLEM_H__
struct JUGS
{
public:
	uint32 Small;
	uint32 Large;
public:
	JUGS();

	JUGS(const uint32 Small, const uint32 Large);
	~JUGS();

	bool operator==(const JUGS &Jugs);
};

class cJugsProblem : public cGeneralSearchAlgorithm<JUGS, double>
{
private:
	eSEARCHALGORITHMTYPE m_CurrentSearchALgorithmType;
protected:
	/*Goal state test funciton*/
	bool _IsGoalState(const SEARCHTREE<JUGS, double>* pTreeNode);

	/*Node generate function*/
	bool _GenerateSuccessorNodes(SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID);

	/*Node cost function*/
	double _CalcCurrentNodeCostVal(const SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID);

	/*H evaluate function*/
	double _EvaluateHeuristicVal(const SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID);

	/*Print function*/
	void _PrintCurrentNode(const SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID, SString* pstrOutput = nullptr);
	/*Print the action list*/
	void _PrintPathList(const vector<uint32> &PathList, SString* pstrOutput = nullptr);
public:
	cJugsProblem();
	~cJugsProblem();

	/*Search fucntion interface, for user call*/
	vector<uint32> iSearch(const JUGS &InitState, const uint32 MaxDepth, const eSEARCHALGORITHMTYPE Type, SString* pstrOutput = nullptr);
};
#endif