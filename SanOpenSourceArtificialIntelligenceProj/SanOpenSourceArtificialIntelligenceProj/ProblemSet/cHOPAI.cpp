#include"cHOPAI.h"
using namespace San;
using namespace San::AI;
cHOPAI::cHOPAI(cHOP* pHOP)
	:m_pHOP(pHOP)
{
}
cHOPAI::~cHOPAI()
{
}
uint32 cHOPAI::_GenerateStates(SEARCHTREE<HOPSTATE, sfloat>* pTreeNode, const eMINMAXSEARCHTYPE Type)
{
	eHOPSTATE CurrentPlayerID = (eHOPSTATE) ((this->m_pHOP->iGetCurrentPlayerID() + pTreeNode->iGetDepth()) % eHOPSTATE::BS_PLAYER_COUNT);

	/*Get all pieces position*/
	vector<POINT2> PiecesArray = this->m_pHOP->iGetPiecesPositionArray(CurrentPlayerID);

	uint32 PieceSize = PiecesArray.size();

	HOPSTATE State;

	if (this->m_pHOP->iEvaluateBoard(State.Board) & 0xf0 == eHOPSTATE::BS_WIN)
	{
		return 0;
	}

	/*For each piece, generate all possible actions*/
	for (uint32 seek = 0; seek < PiecesArray.size(); seek = seek + 1)
	{
		vector<vector<POINT2>> ActionArray = this->m_pHOP->iGetPieceAvaliableMoveArray(CurrentPlayerID, PiecesArray[seek]);

		State.PiecePosition = PiecesArray[seek];
		State.Board = this->m_pHOP->iGetBoard();

		State.Board[State.PiecePosition.x][State.PiecePosition.y] = eHOPSTATE::BS_EMPTY;

		uint32 ArraySize = ActionArray.size();

		SEARCHTREE<HOPSTATE, sfloat>* pCurrentNode = nullptr;
		POINT2 TargetPosition;

		for (uint32 seek_item = 0; seek_item < ArraySize; seek_item = seek_item + 1)
		{
			State.ActionList = ActionArray[seek_item];
			//State.Board[State.ActionList.back().x][State.ActionList.back().y] = AIPlayerID;

			TargetPosition = ActionArray[seek_item].back();

			State.Board[TargetPosition.x][TargetPosition.y] = CurrentPlayerID;

			/*Add to tree*/
			pCurrentNode = pTreeNode->iCreateOffshootNode(State, 0.0);

			if (Type == MMST_BASIC)
			{
				pCurrentNode->iSetGlobalWeight(0.0);
			}
			else
			{
				/*For sort, a-b pruning*/
				pCurrentNode->iSetGlobalWeight(this->_EvaluateState(pCurrentNode));
			}

			State.Board[TargetPosition.x][TargetPosition.y] = eHOPSTATE::BS_EMPTY;
		}
	}

	return pTreeNode->iGetOffshootListSize();
}
sfloat cHOPAI::_EvaluateState(SEARCHTREE<HOPSTATE, sfloat>* pTreeNode)
{
	/*Calculate the current player distance, opponet distance, current player goal pieces number opponet goal pieces number*/

	if (this->m_pHOP == nullptr)
	{
		return 0;
	}

	const uint32 BoardWidth = this->m_pHOP->iGetBoardWidth();
	const uint32 BoardHeight = this->m_pHOP->iGetBoardHeight();

	const eHOPSTATE CurrentPlayerID = this->m_pHOP->iGetCurrentPlayerID();
	const eHOPSTATE CurrentOpPlayerID = (eHOPSTATE) ((CurrentPlayerID + 1) % eHOPSTATE::BS_PLAYER_COUNT);

	sfloat PlayerTotalDis = 0.0;
	sfloat OpTotalDis = 0.0;

	sfloat PlayerArrivalCount = 0.0;
	sfloat OpArrivalCount = 0.0;

	const HOPSTATE* pState = pTreeNode->iGetDataPtrConst();

	for (int32 seek = 0; seek < BoardWidth; seek = seek + 1)
	{
		for (int32 seek_pos = 0; seek_pos <BoardHeight; seek_pos = seek_pos + 1)
		{
			sfloat CurrentDis = 0.0;
			if (pState->Board[seek][seek_pos] == CurrentPlayerID)
			{
				if (CurrentPlayerID == eHOPSTATE::BS_PLAYER_ONE)
				{
					CurrentDis = seek_pos;
				}
				else
				{
					CurrentDis = BoardHeight - 1 - seek_pos;
				}
				/*4 and 5 are similer*/
				CurrentDis = CurrentDis > 4 ? 4.1 : CurrentDis;

				PlayerTotalDis = PlayerTotalDis + CurrentDis;
				PlayerArrivalCount = PlayerArrivalCount + CurrentDis >= 4 ? 1 : 0;
			}
			if (pState->Board[seek][seek_pos] == CurrentOpPlayerID)
			{
				if (CurrentOpPlayerID == eHOPSTATE::BS_PLAYER_ONE)
				{
					CurrentDis = seek_pos;
				}
				else
				{
					CurrentDis = BoardHeight - 1 - seek_pos;
				}
				CurrentDis = CurrentDis > 4 ? 4.1 : CurrentDis;

				OpTotalDis = OpTotalDis + CurrentDis;
				OpArrivalCount = OpArrivalCount + CurrentDis >= 4 ? 1 : 0;
			}
		}
	}

	/*If one is win, set a large value*/
	PlayerArrivalCount = PlayerArrivalCount == 6 ? 100 : PlayerArrivalCount;
	OpArrivalCount = OpArrivalCount == 6 ? 100 : OpArrivalCount;

	/*Evaluate function*/
	sfloat StateScore = PlayerTotalDis*PlayerTotalDis + PlayerArrivalCount - 0.9*(OpTotalDis*OpTotalDis + OpArrivalCount);

	return StateScore;
}
sfloat cHOPAI::_QuickEvaluateState(SEARCHTREE<HOPSTATE, sfloat>* pTreeNode)
{
	if (this->m_pHOP == nullptr)
	{
		return 0;
	}

	/*Only calculate the distance*/
	const uint32 BoardWidth = this->m_pHOP->iGetBoardWidth();
	const uint32 BoardHeight = this->m_pHOP->iGetBoardHeight();

	const eHOPSTATE CurrentPlayerID = this->m_pHOP->iGetCurrentPlayerID();

	const sfloat Boundary = (CurrentPlayerID == eHOPSTATE::BS_PLAYER_ONE) ? 0.0 : 5.0;

	sfloat Value = 0.0;

	const HOPSTATE* pState = pTreeNode->iGetDataPtrConst();

	for (int32 seek = 0; seek < BoardWidth; seek = seek + 1)
	{
		for (int32 seek_pos = 0; seek_pos <BoardHeight; seek_pos = seek_pos + 1)
		{
			if (pState->Board[seek][seek_pos] == CurrentPlayerID)
			{
				sfloat CurrentVal = abs(Boundary - (sfloat) seek_pos);
				Value = Value + CurrentVal>5.0 ? 5.0 : CurrentVal;
			}
		}
	}

	return Value;
}
bool cHOPAI::_BoundaryCheck(const time_t &Value)
{
	if (::clock() > Value)
	{
		return true;
	}
	return false;
}
void cHOPAI::iSetHOPPointer(cHOP* pHOP)
{
	this->m_pHOP = pHOP;
}
cHOP* cHOPAI::iGetHOPPointer() const
{
	return this->m_pHOP;
}
HOPSTATE cHOPAI::iSearch(const BOARD2D &InitState, const uint32 MaxSearchDepth, const time_t &RestrictionValue, cSanTerminalDevice* pTerminal)
{
	HOPSTATE State;
	State.Board = InitState;
	State.PiecePosition = POINT2(0, 0);
	State.ActionList.clear();
	State.CurrentPlayer = this->m_pHOP->iGetCurrentPlayerID();
	State.Quality = 0.0;

	//this->m_pHOP->iRenderBoard(State.Board, *pTerminal);

	//return this->iMinMaxSearch(State, MaxSearchDepth, RestrictionValue, pTerminal);
	return this->iAlphaBetaPruningSearch(State, MaxSearchDepth, RestrictionValue, pTerminal);
}