#include"cJugsProblem.h"
using namespace San;
using namespace San::AI;
JUGS::JUGS()
	:Small(0), Large(0)
{
}
JUGS::JUGS(const uint32 Small, const uint32 Large)
	:Small(Small), Large(Large)
{
}
JUGS::~JUGS()
{
}

bool JUGS::operator==(const JUGS &Jugs)
{
	return (this->Small == Jugs.Small) && (this->Large == Jugs.Large);
}
cJugsProblem::cJugsProblem()
	:m_CurrentSearchALgorithmType(SAT_BREADTH_FIRST_SEARCH)
{
}
cJugsProblem::~cJugsProblem()
{
}
bool cJugsProblem::_IsGoalState(const SEARCHTREE<JUGS, double>* pTreeNode)
{
	if (pTreeNode->iGetData().Large == 4)
	{
		return true;
	}
	return false;
}
bool cJugsProblem::_GenerateSuccessorNodes(SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID)
{
	JUGS PrevState = pTreeNode->iGetData();
	JUGS CurrentState;
	double CurrentWeight;

	SEARCHTREE<JUGS, double>* pCurrentNode = nullptr;

	/*Fill Small*/
	CurrentState = PrevState;
	CurrentState.Small = 3;

	pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);

	CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID);

	pCurrentNode->iSetGlobalWeight(CurrentWeight);



	/*Fill Large*/
	CurrentState = PrevState;
	CurrentState.Large = 5;

	pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);

	CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + 1) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + 1);

	pCurrentNode->iSetGlobalWeight(CurrentWeight);

	/*Empty Small*/
	CurrentState = PrevState;
	CurrentState.Small = 0;

	pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);

	CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + 2) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + 2);

	pCurrentNode->iSetGlobalWeight(CurrentWeight);

	/*Empty Large*/
	CurrentState = PrevState;
	CurrentState.Large = 0;

	pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);

	CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + 3) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + 3);

	pCurrentNode->iSetGlobalWeight(CurrentWeight);

	/*M - S - L*/
	CurrentState.Small = (PrevState.Large + PrevState.Small) > 5 ? (PrevState.Large + PrevState.Small) - 5 : 0;
	CurrentState.Large = (PrevState.Large + PrevState.Small) > 5 ? 5 : PrevState.Large + PrevState.Small;

	pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);

	CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + 4) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + 4);

	pCurrentNode->iSetGlobalWeight(CurrentWeight);

	/*M - L - S*/
	CurrentState.Small = (PrevState.Large + PrevState.Small) > 3 ? 3 : PrevState.Large + PrevState.Small;
	CurrentState.Large = (PrevState.Large + PrevState.Small) > 3 ? (PrevState.Large + PrevState.Small) - 3 : 0;

	pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);

	CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + 5) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + 5);

	pCurrentNode->iSetGlobalWeight(CurrentWeight);

	return true;
}
double cJugsProblem::_CalcCurrentNodeCostVal(const SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID)
{
	switch (this->m_CurrentSearchALgorithmType)
	{
	case SAT_BREADTH_FIRST_SEARCH:
		return GlobalID;
		break;
	case SAT_DEPTH_FIRST_SEARCH:
	{
								   double BaseVal = pTreeNode->iGetTrunkPtr()->iGetGlobalWeight();
								   BaseVal = BaseVal + (double) (pTreeNode->iGetTreeID() + 1.0) * ::pow((double)0.1, (double) pTreeNode->iGetDepth());
								   return BaseVal;
								   break;
	}
	case SAT_GREEDY_SEARCH:
		return 0;// pTreeNode->iGetTrunkPtr()->iGetGlobalWeight();
		break;
	case SAT_UNIFORM_COST_SEARCH:
	case SAT_A_STAR_SEARCH:
		switch (pTreeNode->iGetTreeID())
		{
		case 0:
		case 1:
		case 4:
		case 5:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 2;
			break;
		case 2:
		case 3:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 3;
			break;
		default:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 2;
		}
		break;
	default:
		break;
	}
}
double cJugsProblem::_EvaluateHeuristicVal(const SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID)
{
	switch (this->m_CurrentSearchALgorithmType)
	{
	case SAT_BREADTH_FIRST_SEARCH:
		return 0;
		break;
	case SAT_DEPTH_FIRST_SEARCH:
		return 0;
		break;
	case SAT_UNIFORM_COST_SEARCH:
		return 0;
		break;
	case SAT_GREEDY_SEARCH:
		return ::abs((int32) pTreeNode->iGetData().Large - 4);
		break;
	case SAT_A_STAR_SEARCH:
		return ::abs((int32) pTreeNode->iGetData().Large - 4);
		break;
	default:
		break;
	}
}
void cJugsProblem::_PrintCurrentNode(const SEARCHTREE<JUGS, double>* pTreeNode, const uint32 GlobalID, SString* pstrOutput)
{
	SString strOutput;
	for (uint32 seek = 0; seek < pTreeNode->iGetDepth(); seek = seek + 1)
	{
		strOutput = strOutput + "   ";
	}//*/
	switch (pTreeNode->iGetTreeID())
	{
	case 0:
		strOutput = strOutput + "FS ";
		break;
	case 1:
		strOutput = strOutput + "FL ";
		break;
	case 2:
		strOutput = strOutput + "ES ";
		break;
	case 3:
		strOutput = strOutput + "EL ";
		break;
	case 4:
		strOutput = strOutput + "PSL";
		break;
	case 5:
		strOutput = strOutput + "PLS";
		break;
	default:
		break;
	}
	::cout << strOutput.c_str();
	::cout << " <" << pTreeNode->iGetData().Small << ", " << pTreeNode->iGetData().Large << "> " << pTreeNode->iGetGlobalWeight() << " ";

	if (pstrOutput != nullptr)
	{
		strOutput = strOutput + " <" + ::gloIToS(pTreeNode->iGetData().Small) + ", " + ::gloIToS(pTreeNode->iGetData().Large) + "> " + ::gloFToS(pTreeNode->iGetGlobalWeight()) + " ";
		*pstrOutput = *pstrOutput + strOutput;
	}
}
void cJugsProblem::_PrintPathList(const vector<uint32> PathList, SString* pstrOutput)
{
	SString strOutput = "InitState - ";
	for (uint32 seek = 0; seek < PathList.size(); seek = seek + 1)
	{
		switch (PathList[seek])
		{
		case 0:
			strOutput = strOutput + "FS - ";
			break;
		case 1:
			strOutput = strOutput + "FL - ";
			break;
		case 2:
			strOutput = strOutput + "ES - ";
			break;
		case 3:
			strOutput = strOutput + "EL - ";
			break;
		case 4:
			strOutput = strOutput + "PSL - ";
			break;
		case 5:
			strOutput = strOutput + "PLS - ";
			break;
		default:
			break;
		}
	}
	strOutput = strOutput + "GoalState\r\n";

	::cout << strOutput.c_str();

	if (pstrOutput != nullptr)
	{
		*pstrOutput = *pstrOutput + strOutput;
	}
}
vector<uint32> cJugsProblem::iSearch(const JUGS &InitState, const uint32 MaxDepth, const eSEARCHALGORITHMTYPE Type, SString* pstrOutput)
{
	this->m_CurrentSearchALgorithmType = Type;
	return cGeneralSearchAlgorithm::iSearch(InitState, MaxDepth, pstrOutput);
}