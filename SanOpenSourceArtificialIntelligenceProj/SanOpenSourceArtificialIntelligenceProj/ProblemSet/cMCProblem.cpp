#include"cMCProblem.h"
using namespace San;
using namespace San::AI;
MC::MC()
	:Left_MNum(0), Left_CNum(0), Right_MNum(0), Right_CNum(0), Boat_MNum(0), Boat_CNum(0), bBoatOnLeft(true)
{
}
MC::MC(const int32 LMN, const int32 LCN, const int32 RMN, const int32 RCN, const int32 BMN, const int32 BCN, const bool bBoatOnLeft)
	:Left_MNum(LMN), Left_CNum(LCN), Right_MNum(RMN), Right_CNum(RCN), Boat_MNum(BMN), Boat_CNum(BCN), bBoatOnLeft(bBoatOnLeft)
{
}
MC::~MC()
{
}
bool MC::operator==(const MC &Data)
{
	if (this->Left_MNum != Data.Left_MNum){ return false; }
	if (this->Left_CNum != Data.Left_CNum){ return false; }
	if (this->Right_MNum != Data.Right_MNum){ return false; }
	if (this->Right_CNum != Data.Right_CNum){ return false; }
	if (this->Boat_MNum != Data.Boat_MNum){ return false; }
	if (this->Boat_CNum != Data.Boat_CNum){ return false; }
	if (this->bBoatOnLeft != Data.bBoatOnLeft){ return false; }
	return true;
}
bool MC::iCheckInvalid() const
{
	if (this->Left_MNum < 0)
	{
		return true;
	}
	if (this->Left_CNum < 0)
	{
		return true;
	}
	if (this->Right_MNum < 0)
	{
		return true;
	}
	if (this->Right_CNum < 0)
	{
		return true;
	}
	if (this->Boat_MNum < 0)
	{
		return true;
	}
	if (this->Boat_CNum < 0)
	{
		return true;
	}

	if ((this->Boat_MNum + this->Boat_CNum)>2)
	{
		return true;
	}

	if ((this->Left_MNum != 0) && (this->Left_MNum < this->Left_CNum))
	{
		return true;
	}
	if ((this->Right_MNum != 0) && (this->Right_MNum < this->Right_CNum))
	{
		return true;
	}
	if ((this->Boat_MNum != 0) && (this->Boat_MNum < this->Boat_CNum))
	{
		return true;
	}

	return false;
}
cMCProblem::cMCProblem()
	:m_CurrentSearchALgorithmType(SAT_BREADTH_FIRST_SEARCH)
{
}
cMCProblem::~cMCProblem()
{
}
vector<uint32> cMCProblem::_GeneratePathList(SEARCHTREE<MC, double>* pTargetTreeNode, SString* pstrOutput)
{
	vector<uint32> PathList;
	SEARCHTREE<MC, double>* pCurrentNode = pTargetTreeNode;
	while (!pCurrentNode->iIsRoot())
	{
		PathList.insert(PathList.begin(), pCurrentNode->iGetData().ID);
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
bool cMCProblem::_IsGoalState(const SEARCHTREE<MC, double>* pTreeNode)
{
	if ((pTreeNode->iGetData().Right_MNum == 3) && (pTreeNode->iGetData().Right_CNum == 3))
	{
		return true;
	}
	return false;
}
bool cMCProblem::_GenerateSuccessorNodes(SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID)
{
	MC PrevState = pTreeNode->iGetData();
	MC CurrentState;

	uint32 IDOffset = 0;
	double CurrentWeight;

	SEARCHTREE<MC, double>* pCurrentNode = nullptr;

	/*Move one M to Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum + 1;
		CurrentState.Left_MNum = CurrentState.Left_MNum - 1;
	}
	else
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum + 1;
		CurrentState.Right_MNum = CurrentState.Right_MNum - 1;
	}
	CurrentState.ID = 0;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move two M to Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum + 2;
		CurrentState.Left_MNum = CurrentState.Left_MNum - 2;
	}
	else
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum + 2;
		CurrentState.Right_MNum = CurrentState.Right_MNum - 2;
	}
	CurrentState.ID = 1;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move one C to Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum + 1;
		CurrentState.Left_CNum = CurrentState.Left_CNum - 1;
	}
	else
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum + 1;
		CurrentState.Right_CNum = CurrentState.Right_CNum - 1;
	}
	CurrentState.ID = 2;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move two C to Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum + 2;
		CurrentState.Left_CNum = CurrentState.Left_CNum - 2;
	}
	else
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum + 2;
		CurrentState.Right_CNum = CurrentState.Right_CNum - 2;
	}
	CurrentState.ID = 3;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	//--------------------------------------------------//

	/*Move one M off Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum - 1;
		CurrentState.Left_MNum = CurrentState.Left_MNum + 1;
	}
	else
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum - 1;
		CurrentState.Right_MNum = CurrentState.Right_MNum + 1;
	}
	CurrentState.ID = 4;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move two M off Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum - 2;
		CurrentState.Left_MNum = CurrentState.Left_MNum + 2;
	}
	else
	{
		CurrentState.Boat_MNum = CurrentState.Boat_MNum - 2;
		CurrentState.Right_MNum = CurrentState.Right_MNum + 2;
	}
	CurrentState.ID = 5;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move one C off Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum - 1;
		CurrentState.Left_CNum = CurrentState.Left_CNum + 1;
	}
	else
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum - 1;
		CurrentState.Right_CNum = CurrentState.Right_CNum + 1;
	}
	CurrentState.ID = 6;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move two C off Boat*/
	CurrentState = PrevState;
	if (CurrentState.bBoatOnLeft)
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum - 2;
		CurrentState.Left_CNum = CurrentState.Left_CNum + 2;
	}
	else
	{
		CurrentState.Boat_CNum = CurrentState.Boat_CNum - 2;
		CurrentState.Right_CNum = CurrentState.Right_CNum + 2;
	}
	CurrentState.ID = 7;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	/*Move boat to another side*/
	CurrentState = PrevState;
	CurrentState.bBoatOnLeft = !CurrentState.bBoatOnLeft;
	CurrentState.ID = 8;
	if (!CurrentState.iCheckInvalid())
	{
		pCurrentNode = pTreeNode->iCreateOffshootNode(CurrentState, 1);
		CurrentWeight = this->_CalcCurrentNodeCostVal(pCurrentNode, GlobalID + IDOffset) + this->_EvaluateHeuristicVal(pCurrentNode, GlobalID + IDOffset);
		pCurrentNode->iSetGlobalWeight(CurrentWeight);

		IDOffset = IDOffset + 1;
	}

	return true;
}
double cMCProblem::_CalcCurrentNodeCostVal(const SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID)
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
		switch (pTreeNode->iGetData().ID)
		{
		case 0:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 1;
			break;
		case 1:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 2;
			break;
		case 2:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 2;
			break;
		case 3:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 4;
			break;
		case 4:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 1;
			break;
		case 5:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 2;
			break;
		case 6:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 2;
			break;
		case 7:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 4;
			break;
		case 8:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 1;
		default:
			return pTreeNode->iGetTrunkPtr()->iGetGlobalWeight() + 1;
		}
		break;
	default:
		break;
	}
}
double cMCProblem::_EvaluateHeuristicVal(const SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID)
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
	case SAT_A_STAR_SEARCH:
		/*In this problem, H function is defined by the absolute value of (GoalState_Right_Number - CurrentState_Right_Number		*/
		/*For best situation, the right already have 3 - M and 2 - D, one D is on the boat, so in this state still need 1 operation	*/
		/*to reach the goal state, for other stuation, the H value only can greater or equal with the real cost. so this function	*/
		/*is admissible																												*/
		return ::abs((int32) pTreeNode->iGetData().Right_MNum - 3) + ::abs((int32) pTreeNode->iGetData().Right_CNum - 3);
		break;
	default:
		break;
	}
}
void cMCProblem::_PrintCurrentNode(const SEARCHTREE<MC, double>* pTreeNode, const uint32 GlobalID, SString* pstrOutput)
{
	SString strOutput;

	MC State = pTreeNode->iGetData();
	for (uint32 seek = 0; seek < pTreeNode->iGetDepth(); seek = seek + 1)
	{
		strOutput = strOutput + "\t";
	}//*/
	switch (pTreeNode->iGetData().ID)
	{
	case 0:
		strOutput = strOutput + "1MB ";
		break;
	case 1:
		strOutput = strOutput + "2MB ";
		break;
	case 2:
		strOutput = strOutput + "1CB ";
		break;
	case 3:
		strOutput = strOutput + "2CB ";
		break;
	case 4:
		strOutput = strOutput + "1MOFF ";
		break;
	case 5:
		strOutput = strOutput + "2MOFF ";
		break;
	case 6:
		strOutput = strOutput + "1COFF ";
		break;
	case 7:
		strOutput = strOutput + "2COFF ";
		break;
	case 8:
		strOutput = strOutput + "MOV ";
		break;
	default:
		break;
	}

	if (State.bBoatOnLeft)
	{
		strOutput = strOutput + "Left<" + ::gloIToS(State.Left_MNum) + ", " + ::gloIToS(State.Left_CNum) + ">";
		strOutput = strOutput + " Boat<" + ::gloIToS(State.Boat_MNum) + ", " + ::gloIToS(State.Boat_CNum) + ">   ";
		strOutput = strOutput + "Right<" + ::gloIToS(State.Right_MNum) + ", " + ::gloIToS(State.Right_CNum) + ">";
	}
	else
	{
		strOutput = strOutput + "Left<" + ::gloIToS(State.Left_MNum) + ", " + ::gloIToS(State.Left_CNum) + ">";
		strOutput = strOutput + "   Boat<" + ::gloIToS(State.Boat_MNum) + ", " + ::gloIToS(State.Boat_CNum) + "> ";
		strOutput = strOutput + "Right<" + ::gloIToS(State.Right_MNum) + ", " + ::gloIToS(State.Right_CNum) + ">";
	}

	::cout << strOutput.c_str();

	if (pstrOutput != nullptr)
	{
		*pstrOutput = *pstrOutput + strOutput;
	}
}
void cMCProblem::_PrintPathList(const vector<uint32> PathList, SString* pstrOutput)
{
	SString strOutput = "InitState - ";
	for (uint32 seek = 0; seek < PathList.size(); seek = seek + 1)
	{
		switch (PathList[seek])
		{
		case 0:
			strOutput = strOutput + "1MB - ";
			break;
		case 1:
			strOutput = strOutput + "2MB - ";
			break;
		case 2:
			strOutput = strOutput + "1CB - ";
			break;
		case 3:
			strOutput = strOutput + "2CB - ";
			break;
		case 4:
			strOutput = strOutput + "1MOFF - ";
			break;
		case 5:
			strOutput = strOutput + "2MOFF - ";
			break;
		case 6:
			strOutput = strOutput + "1COFF - ";
			break;
		case 7:
			strOutput = strOutput + "2COFF - ";
			break;
		case 8:
			strOutput = strOutput + "MOV - ";
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
vector<uint32> cMCProblem::iSearch(const MC &InitState, const uint32 MaxDepth, const eSEARCHALGORITHMTYPE Type, SString* pstrOutput)
{
	this->m_CurrentSearchALgorithmType = Type;
	return cGeneralSearchAlgorithm::iSearch(InitState, MaxDepth, pstrOutput);
}