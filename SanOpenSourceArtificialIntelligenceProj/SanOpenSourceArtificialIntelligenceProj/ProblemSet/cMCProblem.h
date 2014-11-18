#include"..\AI\cGeneralSearchAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::AI;
#ifndef __CMCPROBLEM_H__
#define __CMCPROBLEM_H__

struct MC
{
public:
	int32 ID;
	int32 Left_MNum;
	int32 Left_CNum;
	int32 Right_MNum;
	int32 Right_CNum;
	int32 Boat_MNum;
	int32 Boat_CNum;
	bool bBoatOnLeft;
public:
	MC();
	MC(const int32 LMN, const int32 LCN, const int32 RMN, const int32 RCN, const int32 BMN, const int32 BCN, const bool bBoatOnLeft);
	~MC();

	bool operator==(const MC &Data);
	bool iCheckInvalid() const;
};

class cMCProblem : public cGeneralSearchAlgorithm<MC, double>
{
private:
	eSEARCHALGORITHMTYPE m_CurrentSearchALgorithmType;
private:
	vector<uint32> _GeneratePathList(SEARCHTREE<MC, double>* pTargetTreeNode, SString* pstrOutput = nullptr);
protected:
	/*Goal state test funciton*/
	bool _IsGoalState(const SEARCHTREE<MC, double>* pTreeNode);

	/*Node generate function															*/
	/*Action List																		*/
	/*All the actions if generate invalid state like Right_M_Number == -1 will delete	*/
	/*Move one M to boat																*/
	/*Move two M to boat																*/
	/*Move one C to boat																*/
	/*Move two C to boat																*/
	/*Move one M off boat																*/
	/*Move two M off boat																*/
	/*Move one C off boat																*/
	/*Move two C off boat																*/
	/*Move boat to another side															*/
	bool _GenerateSuccessorNodes(SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID);

	/*Node cost function*/
	double _CalcCurrentNodeCostVal(const SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID);

	/*H evaluate function*/
	double _EvaluateHeuristicVal(const SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID);

	/*Print function*/
	void _PrintCurrentNode(const SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID, SString* pstrOutput = nullptr);
	/*Print the action list*/
	void _PrintPathList(const vector<uint32> &PathList, SString* pstrOutput = nullptr);
public:
	cMCProblem();
	~cMCProblem();

	/*Search fucntion interface, for user call*/
	vector<uint32> iSearch(const MC &InitState, const uint32 MaxDepth, const eSEARCHALGORITHMTYPE Type, SString* pstrOutput = nullptr);
};
#endif