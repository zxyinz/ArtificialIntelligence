#include"cHOP.h"
using namespace std;
using namespace San;
using namespace San::Device;
cHOP::cHOP(const uint32 BoardWidth, const uint32 BoardHeight)
	:m_BoardWidth(BoardWidth), m_BoardHeight(BoardHeight)
{
	for (uint32 seek = 0; seek < ::eHOPSTATE::BS_PLAYER_COUNT; seek = seek + 1)
	{
		this->m_PlayerFuncArray.push_back(make_pair(nullptr, nullptr));
	}
	this->m_bState = false;
}
cHOP::~cHOP()
{

	this->m_BoardHistory.clear();

	this->m_PlayerFuncArray.clear();

	this->m_BoardWidth = 0;
	this->m_BoardHeight = 0;
	this->m_bState = false;
}
const eHOPSTATE* cHOP::operator[](const uint32 Position) const
{
	return this->m_Board[Position];
}
void cHOP::_GeneratePieceAvaliableMoveArray(const BOARD2D &Board, const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const POINT2 &CurrentPosition, vector<POINT2> &VisitedPosition, const vector<POINT2> CurrentActionList, vector<vector<POINT2>> &ActionArray) const
{
	/*8 direction*/
	const SVECTOR3 DirectionArray[8] = { SVECTOR3(1, 0, 0), SVECTOR3(-1, 0, 0), SVECTOR3(0, 1, 0), SVECTOR3(0, -1, 0),
										SVECTOR3(-1, 1, 0), SVECTOR3(-1, -1, 0), SVECTOR3(1, -1, 0), SVECTOR3(1, 1, 0) };

	/*For 8 direction*/
	for (uint32 seek = 0; seek < 8; seek = seek + 1)
	{
		SPOINT3 CurrentPos(CurrentPosition.x, CurrentPosition.y, 0);

		while (true)
		{
			CurrentPos = CurrentPos + DirectionArray[seek];

			/*Check boundary*/
			if ((CurrentPos.x < 0) || (CurrentPos.x >= this->m_BoardWidth))
			{
				break;
			}

			if ((CurrentPos.y < 0) || (CurrentPos.y >= this->m_BoardHeight))
			{
				break;
			}

			vector<POINT2> ActionList = CurrentActionList;
			ActionList.push_back(POINT2(CurrentPos.x, CurrentPos.y));

			/*If valid, push to available action list, and call _GeneratePieceAvaliableMoveArray for next possible move*/
			if (this->iEvaluateMove(Board, PlayerID, PiecePosition, ActionList) != eHOPSTATE::BS_INVALID)
			{

				bool bFound = false;

				POINT2 Position(CurrentPos.x, CurrentPos.y);

				/*uint32 Size = ActionList.size() - 1;
				for (uint32 seek_visited = 0; seek_visited < Size; seek_visited = seek_visited + 1)
				{
					if (ActionList[seek_visited] == Position)
					{
						bFound = true;
						break;
					}
				}//*/

				uint32 Size = VisitedPosition.size() - 1;
				for (uint32 seek_visited = 0; seek_visited < Size; seek_visited = seek_visited + 1)
				{
					if (VisitedPosition[seek_visited] == Position)
					{
						bFound = true;
						break;
					}
				}//*/

				/*Check if not found in history (visited positions)*/
				if (!bFound)
				{
					VisitedPosition.push_back(Position);

					ActionArray.push_back(ActionList);

					this->_GeneratePieceAvaliableMoveArray(Board, PlayerID, PiecePosition, Position, VisitedPosition, ActionList, ActionArray);
				}
			}
		}
	}
}
void cHOP::iInitializeBoardState()
{
	this->iClearBoard();
	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		this->m_Board[seek][0] = eHOPSTATE::BS_PLAYER_ONE;
		this->m_Board[seek][this->m_BoardHeight - 1] = ::eHOPSTATE::BS_PLAYER_TWO;
	}
	this->m_CurrentPlayerID = ::eHOPSTATE::BS_PLAYER_ONE;
	this->m_bState = true;
}
void cHOP::iClearBoard()
{
	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		for (uint32 seek_pos = 0; seek_pos < this->m_BoardHeight; seek_pos = seek_pos + 1)
		{
			this->m_Board[seek][seek_pos] = BS_EMPTY;
		}
	}
	this->m_CurrentPlayerID = ::eHOPSTATE::BS_PLAYER_ONE;
	this->m_bState = false;
}
bool cHOP::iRegisteUserFunc(const eHOPSTATE PlayerID, PLAYERMOVEFUNC Func, SHANDLE UserData)
{
	if (PlayerID >= ::eHOPSTATE::BS_PLAYER_COUNT)
	{
		return false;
	}
	this->m_PlayerFuncArray[PlayerID] = make_pair(Func, UserData);
	return true;
}
bool cHOP::iReleaseUserFunc(const eHOPSTATE PlayerID)
{
	if (PlayerID >= ::eHOPSTATE::BS_PLAYER_COUNT)
	{
		return false;
	}
	this->m_PlayerFuncArray[PlayerID] = make_pair(nullptr, nullptr);
	return true;
}
uint32 cHOP::iGetBoardHeight() const
{
	return this->m_BoardHeight;
}
uint32 cHOP::iGetBoardWidth() const
{
	return this->m_BoardWidth;
}
BOARD2D cHOP::iGetBoard() const
{
	return this->m_Board;
}
eHOPSTATE cHOP::iGetCurrentPlayerID() const
{
	return this->m_CurrentPlayerID;
}
vector<POINT2> cHOP::iGetPiecesPositionArray(const eHOPSTATE PlayerID) const
{
	if (PlayerID >= eHOPSTATE::BS_PLAYER_COUNT)
	{
		return vector<POINT2>(0);
	}

	vector<POINT2> PiecesArray(this->m_BoardWidth);
	uint32 PiecesCount = 0;

	for (uint32 seek_width = 0; seek_width < this->m_BoardWidth; seek_width = seek_width + 1)
	{
		for (uint32 seek_height = 0; seek_height < this->m_BoardHeight; seek_height = seek_height + 1)
		{
			if (this->m_Board[seek_width][seek_height] == PlayerID)
			{
				PiecesArray[PiecesCount] = POINT2(seek_width, seek_height);
				PiecesCount = PiecesCount + 1;
			}
		}
	}

	return PiecesArray;
}
vector<vector<POINT2>> cHOP::iGetPieceAvaliableMoveArray(const eHOPSTATE PlayerID, const POINT2 &PiecePosition)
{
	return this->iGetPieceAvaliableMoveArray(this->m_Board, PlayerID, PiecePosition);
}
vector<vector<POINT2>> cHOP::iGetPieceAvaliableMoveArray(const BOARD2D &Board, const eHOPSTATE PlayerID, const POINT2 &PiecePosition) const
{
	if (Board[PiecePosition.x][PiecePosition.y] != PlayerID)
	{
		return vector<vector<POINT2>>(0);
	}

	vector<vector<POINT2>> ActionList;
	vector<POINT2> VisitedPosition;
	vector<POINT2> CurrentAction;

	VisitedPosition.push_back(PiecePosition);

	this->_GeneratePieceAvaliableMoveArray(this->m_Board, PlayerID, PiecePosition, PiecePosition, VisitedPosition, CurrentAction, ActionList);

	return ActionList;
}
eHOPSTATE cHOP::iGetBoardPositionState(const POINT2 &Position) const
{
	if ((Position.x >= this->m_BoardWidth) || (Position.y >= this->m_BoardHeight))
	{
		return eHOPSTATE::BS_INVALID;
	}
	return this->m_Board[Position.x][Position.y];
}
eHOPSTATE cHOP::iGetBoardHistoryPositionState(const uint32 HistoryBoardID, const POINT2 &Position) const
{
	if (HistoryBoardID >= this->m_BoardHistory.size())
	{
		return eHOPSTATE::BS_INVALID;
	}
	if ((Position.x >= this->m_BoardWidth) || (Position.y >= this->m_BoardHeight))
	{
		return eHOPSTATE::BS_INVALID;
	}
	return this->m_BoardHistory[HistoryBoardID].Board[Position.x][Position.y];
}
BOARDSTATE cHOP::iGetBoardHistoryState(const uint32 HistoryBoardID) const
{
	if (HistoryBoardID >= this->m_BoardHistory.size())
	{
		BOARDSTATE State;
		State.PlayerID = eHOPSTATE::BS_INVALID;
		return State;
	}
	return this->m_BoardHistory[HistoryBoardID];
}
uint32 cHOP::iPlayerMove(const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const vector<POINT2> &ActionList)
{
	if (!this->m_bState)
	{
		return eHOPSTATE::BS_INVALID;
	}

	if (PlayerID != this->m_CurrentPlayerID)
	{
		return eHOPSTATE::BS_INVALID;
	}

	if (this->iEvaluateMove(PlayerID, PiecePosition, ActionList) == eHOPSTATE::BS_INVALID)
	{
		return eHOPSTATE::BS_INVALID;
	}

	/*Push current board state to history*/

	BOARDSTATE State;
	State.Board = this->m_Board;
	State.PlayerID = this->m_CurrentPlayerID;
	State.Action = ActionList;
	State.Action.insert(State.Action.begin(), PiecePosition);

	this->m_BoardHistory.push_back(State);

	/*Make move*/

	uint32 Size = ActionList.size() - 1;

	this->m_Board[PiecePosition.x][PiecePosition.y] = eHOPSTATE::BS_EMPTY;

	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		this->m_Board[ActionList[seek].x][ActionList[seek].y] = ::eHOPSTATE::BS_EMPTY;
	}

	this->m_Board[ActionList[Size].x][ActionList[Size].y] = PlayerID;

	this->m_CurrentPlayerID = static_cast<eHOPSTATE>((this->m_CurrentPlayerID + 1) % (::eHOPSTATE::BS_PLAYER_COUNT));

	return this->iEvaluateBoard();
}
uint32 cHOP::iEvaluateMove(const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const vector<POINT2> &ActionList)
{
	/*Check is in the game*/
	if (!this->m_bState)
	{
		return eHOPSTATE::BS_INVALID;
	}

	/*Clear the evaluate pieces*/
	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		for (uint32 seek_pos = 0; seek_pos < this->m_BoardHeight; seek_pos = seek_pos + 1)
		{
			if (this->m_Board[seek][seek_pos] >= ::eHOPSTATE::BS_PLAYER_COUNT)
			{
				this->m_Board[seek][seek_pos] = BS_EMPTY;
			}
		}
	}

#pragma region Evaluate Move
	///*Check player ID*/
	//if (PlayerID >= ::eHOPSTATE::BS_PLAYER_COUNT)
	//{
	//	return eHOPSTATE::BS_INVALID;
	//}

	///*Check is on the board*/
	//if ((PiecePosition.x < 0) || (PiecePosition.x >= this->m_BoardWidth))
	//{
	//	return eHOPSTATE::BS_INVALID;
	//}

	//if ((PiecePosition.y < 0) || (PiecePosition.y >= this->m_BoardHeight))
	//{
	//	return eHOPSTATE::BS_INVALID;
	//}

	//if (this->m_Board[PiecePosition.x][PiecePosition.y] != PlayerID)
	//{
	//	return eHOPSTATE::BS_INVALID;
	//}

	///*Check action list*/
	//if (ActionList.empty())
	//{
	//	return eHOPSTATE::BS_INVALID;
	//}

	//SVECTOR3 CurrentPos = SVECTOR3(PiecePosition.x, PiecePosition.y, 0);

	//for (uint32 seek = 0; seek < ActionList.size(); seek = seek + 1)
	//{
	//	/*Check is on the board*/
	//	if ((ActionList[seek].x < 0) || (ActionList[seek].x >= this->m_BoardWidth))
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}

	//	if ((ActionList[seek].y < 0) || (ActionList[seek].y >= this->m_BoardHeight))
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}

	//	if (this->m_Board[ActionList[seek].x][ActionList[seek].y] < eHOPSTATE::BS_PLAYER_COUNT)
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}

	//	int32 OffsetX = ActionList[seek].x - CurrentPos.x;
	//	int32 OffsetY = ActionList[seek].y - CurrentPos.y;

	//	uint32 AbsOffsetX = abs(OffsetX);
	//	uint32 AbsOffsetY = abs(OffsetY);

	//	if ((AbsOffsetX*AbsOffsetY != 0) && (AbsOffsetX != AbsOffsetY))
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}

	//	if (((AbsOffsetX == 1) || (AbsOffsetY == 1)) && ((AbsOffsetX + AbsOffsetY) <= 2))
	//	{
	//		if (this->m_Board[ActionList[seek].x][ActionList[seek].y] <= eHOPSTATE::BS_PLAYER_COUNT)
	//		{
	//			return eHOPSTATE::BS_INVALID;
	//		}

	//		if (ActionList.size() > 1)
	//		{
	//			return eHOPSTATE::BS_INVALID;
	//		}

	//		break;
	//	}

	//	SVECTOR3 Dir(0, 0, 0);

	//	/*Check hop is valid*/
	//	if ((AbsOffsetX!= 0) && (AbsOffsetX % 2 == 1))
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}
	//	else
	//	{
	//		Dir.x = (OffsetX == 0) ? 0 : (OffsetX < 0) ? -1 : 1;
	//	}

	//	if ((AbsOffsetY != 0) && (AbsOffsetY % 2 == 1))
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}
	//	else
	//	{
	//		Dir.y = (OffsetY == 0) ? 0 : (OffsetY < 0) ? -1 : 1;
	//	}

	//	uint32 Count = 0;

	//	SPOINT3 SeekPos = CurrentPos + Dir;
	//	SPOINT3 TargetPos(ActionList[seek].x, ActionList[seek].y, 0);
	//	while (SeekPos != TargetPos)
	//	{
	//		if (this->m_Board[SeekPos.x][SeekPos.y] < eHOPSTATE::BS_PLAYER_COUNT)
	//		{
	//			Count = Count + 1;
	//		}
	//		SeekPos = SeekPos + Dir;
	//	}

	//	if (Count>1)
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}

	//	OffsetX = OffsetX / 2;// +DirX;
	//	OffsetY = OffsetY / 2;// +DirY;

	//	CurrentPos = CurrentPos + SVECTOR3(OffsetX, OffsetY, 0);

	//	if ((CurrentPos.x == PiecePosition.x) && (CurrentPos.y == PiecePosition.y))
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}

	//	if (this->m_Board[CurrentPos.x][CurrentPos.y] >= eHOPSTATE::BS_PLAYER_COUNT)
	//	{
	//		return eHOPSTATE::BS_INVALID;
	//	}
	//	
	//	CurrentPos = SVECTOR3(ActionList[seek].x, ActionList[seek].y, 0);
	//}
	//return eHOPSTATE::BS_EVALUATE;
#pragma endregion

	return this->iEvaluateMove(this->m_Board, PlayerID, PiecePosition, ActionList);
}
uint32 cHOP::iEvaluateMove(const BOARD2D &Board, const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const vector<POINT2> &ActionList) const
{
	/*Check player ID*/
	if (PlayerID >= ::eHOPSTATE::BS_PLAYER_COUNT)
	{
		return eHOPSTATE::BS_INVALID;
	}

	/*Check is on the board*/
	if ((PiecePosition.x < 0) || (PiecePosition.x >= this->m_BoardWidth))
	{
		return eHOPSTATE::BS_INVALID;
	}

	if ((PiecePosition.y < 0) || (PiecePosition.y >= this->m_BoardHeight))
	{
		return eHOPSTATE::BS_INVALID;
	}

	if (Board[PiecePosition.x][PiecePosition.y] != PlayerID)
	{
		return eHOPSTATE::BS_INVALID;
	}

	/*Check action list*/
	if (ActionList.empty())
	{
		return eHOPSTATE::BS_INVALID;
	}

	SVECTOR3 CurrentPos = SVECTOR3(PiecePosition.x, PiecePosition.y, 0);

	for (uint32 seek = 0; seek < ActionList.size(); seek = seek + 1)
	{
		/*Check is on the board*/
		if ((ActionList[seek].x < 0) || (ActionList[seek].x >= this->m_BoardWidth))
		{
			return eHOPSTATE::BS_INVALID;
		}

		if ((ActionList[seek].y < 0) || (ActionList[seek].y >= this->m_BoardHeight))
		{
			return eHOPSTATE::BS_INVALID;
		}

		if (Board[ActionList[seek].x][ActionList[seek].y] < eHOPSTATE::BS_PLAYER_COUNT)
		{
			return eHOPSTATE::BS_INVALID;
		}

		int32 OffsetX = ActionList[seek].x - CurrentPos.x;
		int32 OffsetY = ActionList[seek].y - CurrentPos.y;

		uint32 AbsOffsetX = abs(OffsetX);
		uint32 AbsOffsetY = abs(OffsetY);

		if ((AbsOffsetX*AbsOffsetY != 0) && (AbsOffsetX != AbsOffsetY))
		{
			return eHOPSTATE::BS_INVALID;
		}

		/*Check one move (not hop)*/
		if (((AbsOffsetX == 1) || (AbsOffsetY == 1)) && ((AbsOffsetX + AbsOffsetY) <= 2))
		{
			if (Board[ActionList[seek].x][ActionList[seek].y] <= eHOPSTATE::BS_PLAYER_COUNT)
			{
				return eHOPSTATE::BS_INVALID;
			}

			/*If have one move and have multi-action, invalid*/
			if (ActionList.size() > 1)
			{
				return eHOPSTATE::BS_INVALID;
			}

			break;
		}

		SVECTOR3 Dir(0, 0, 0);

		/*Check hop is valid*/
		if ((AbsOffsetX != 0) && (AbsOffsetX % 2 == 1))
		{
			return eHOPSTATE::BS_INVALID;
		}
		else
		{
			Dir.x = (OffsetX == 0) ? 0 : (OffsetX < 0) ? -1 : 1;
		}

		if ((AbsOffsetY != 0) && (AbsOffsetY % 2 == 1))
		{
			return eHOPSTATE::BS_INVALID;
		}
		else
		{
			Dir.y = (OffsetY == 0) ? 0 : (OffsetY < 0) ? -1 : 1;
		}

		uint32 Count = 0;

		/*Check if there have more than one piece on the hop road*/
		SPOINT3 SeekPos = CurrentPos + Dir;
		SPOINT3 TargetPos(ActionList[seek].x, ActionList[seek].y, 0);
		while (SeekPos != TargetPos)
		{
			if (Board[(int32)SeekPos.x][(int32)SeekPos.y] < eHOPSTATE::BS_PLAYER_COUNT)
			{
				Count = Count + 1;
			}
			SeekPos = SeekPos + Dir;
		}

		if (Count>1)
		{
			return eHOPSTATE::BS_INVALID;
		}

		OffsetX = OffsetX / 2;// +DirX;
		OffsetY = OffsetY / 2;// +DirY;

		CurrentPos = CurrentPos + SVECTOR3(OffsetX, OffsetY, 0);

		/*Check hop middle position is valid*/
		if ((CurrentPos.x == PiecePosition.x) && (CurrentPos.y == PiecePosition.y))
		{
			return eHOPSTATE::BS_INVALID;
		}

		if (Board[(int32)CurrentPos.x][(int32)CurrentPos.y] >= eHOPSTATE::BS_PLAYER_COUNT)
		{
			return eHOPSTATE::BS_INVALID;
		}

		CurrentPos = SVECTOR3(ActionList[seek].x, ActionList[seek].y, 0);
	}
	return eHOPSTATE::BS_EVALUATE;
}
uint32 cHOP::iEvaluateBoard() const
{
	/*uint32 PlayerPiecesCount[eHOPSTATE::BS_PLAYER_COUNT];

	for (uint32 seek = 0; seek < eHOPSTATE::BS_PLAYER_COUNT; seek = seek + 1)
	{
		PlayerPiecesCount[seek] = 0;
	}

	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		if (this->m_Board[seek][0] == eHOPSTATE::BS_PLAYER_TWO)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] + 1;
		}
		if (this->m_Board[seek][1] == eHOPSTATE::BS_PLAYER_TWO)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] + 1;
		}

		if (this->m_Board[seek][this->m_BoardHeight - 1] == eHOPSTATE::BS_PLAYER_ONE)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] + 1;
		}
		if (this->m_Board[seek][this->m_BoardHeight - 2] == eHOPSTATE::BS_PLAYER_ONE)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] + 1;
		}
	}

	for (uint32 seek = 0; seek < eHOPSTATE::BS_PLAYER_COUNT; seek = seek + 1)
	{
		if (PlayerPiecesCount[seek] == this->m_BoardWidth)
		{
			return eHOPSTATE::BS_WIN | seek;
		}
	}
	return eHOPSTATE::BS_EVALUATE;*/
	return this->iEvaluateBoard(this->m_Board);
}
uint32 cHOP::iEvaluateBoard(const BOARD2D &Board) const
{
	uint32 PlayerPiecesCount[eHOPSTATE::BS_PLAYER_COUNT];

	for (uint32 seek = 0; seek < eHOPSTATE::BS_PLAYER_COUNT; seek = seek + 1)
	{
		PlayerPiecesCount[seek] = 0;
	}

	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		if (Board[seek][0] == eHOPSTATE::BS_PLAYER_TWO)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] + 1;
		}
		if (Board[seek][1] == eHOPSTATE::BS_PLAYER_TWO)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] + 1;
		}

		if (Board[seek][this->m_BoardHeight - 1] == eHOPSTATE::BS_PLAYER_ONE)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] + 1;
		}
		if (Board[seek][this->m_BoardHeight - 2] == eHOPSTATE::BS_PLAYER_ONE)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] + 1;
		}
	}

	for (uint32 seek = 0; seek < eHOPSTATE::BS_PLAYER_COUNT; seek = seek + 1)
	{
		if (PlayerPiecesCount[seek] == this->m_BoardWidth)
		{
			return eHOPSTATE::BS_WIN | seek;
		}
	}
	return eHOPSTATE::BS_EVALUATE;
}
uint32 cHOP::iUpdateGame()
{
	if (!this->m_bState)
	{
		return eHOPSTATE::BS_INVALID;
	}

	if (this->m_PlayerFuncArray[this->m_CurrentPlayerID].first != nullptr)
	{
		return this->m_PlayerFuncArray[this->m_CurrentPlayerID].first(this, this->m_PlayerFuncArray[this->m_CurrentPlayerID].second);
	}
	return eHOPSTATE::BS_INVALID;
}
uint32 cHOP::iTerminateGame()
{
	vector<uint32> PlayerPiecesCount(eHOPSTATE::BS_PLAYER_COUNT);

	for (uint32 seek = 0; seek < PlayerPiecesCount.size(); seek = seek + 1)
	{
		PlayerPiecesCount[seek] = 0;
	}

	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		if (this->m_Board[seek][0] == eHOPSTATE::BS_PLAYER_TWO)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] + 1;
		}
		if (this->m_Board[seek][1] == eHOPSTATE::BS_PLAYER_TWO)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] + 1;
		}

		if (this->m_Board[seek][this->m_BoardHeight - 1] == eHOPSTATE::BS_PLAYER_ONE)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] + 1;
		}
		if (this->m_Board[seek][this->m_BoardHeight - 2] == eHOPSTATE::BS_PLAYER_ONE)
		{
			PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] = PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] + 1;
		}
	}

	this->m_bState = false;

	if (PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] == PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO])
	{
		return eHOPSTATE::BS_TIE;
	}
	return eHOPSTATE::BS_WIN|(PlayerPiecesCount[eHOPSTATE::BS_PLAYER_ONE] > PlayerPiecesCount[eHOPSTATE::BS_PLAYER_TWO] ? eHOPSTATE::BS_PLAYER_ONE : eHOPSTATE::BS_PLAYER_TWO);
}

bool cHOP::iPopBoardHistory()
{

	if (this->m_BoardHistory.empty())
	{
		return false;
	}

	this->m_Board = this->m_BoardHistory.back().Board;
	this->m_CurrentPlayerID = this->m_BoardHistory.back().PlayerID;

	this->m_BoardHistory.pop_back();

	return true;
}
void cHOP::iRenderBoard(cSanTerminalDevice &Terminal) const
{
	//char ColSymbolArray[] = "ABCDEFGHIJ";
	char ColSymbolArray[] = "0123456789";
	char RowSymbolArray[] = "0123456789";
	char PlayerSymbolArray[] = "OX*#";

	SPOINT3 TerminalCoord = Terminal.iGetCurrentCursorPosition();

	this->iRenderBoard(this->m_Board, Terminal);

	TerminalCoord = TerminalCoord + SPOINT3(35, 2, 0);

	char Buffer[32];
	Buffer[1] = '\0';

	for (uint32 seek = 0; seek < eHOPSTATE::BS_PLAYER_COUNT; seek = seek + 1)
	{
		Buffer[0] = PlayerSymbolArray[seek];
		if (seek == this->m_CurrentPlayerID)
		{
			Terminal.iOutputString(_SSTR("Player #") + ::gloIToS(seek + 1) + _SSTR(": ") + Buffer + _SSTR(" Current Player"), TerminalCoord, STC_GREY);
		}
		else
		{
			Terminal.iOutputString(_SSTR("Player #") + ::gloIToS(seek + 1) + _SSTR(": ") + Buffer, TerminalCoord, STC_GREY);
		}
		TerminalCoord.y = TerminalCoord.y + 1;
	}
}
void cHOP::iRenderBoard(const BOARD2D &Board, cSanTerminalDevice &Terminal) const
{
	//char ColSymbolArray[] = "ABCDEFGHIJ";
	char ColSymbolArray[] = "0123456789";
	char RowSymbolArray[] = "0123456789";
	char PlayerSymbolArray[] = "OX*#";

	SString strStartRowString;
	SString strMidRowString;
	SString strEndRowString;

	uint32 Size = this->m_BoardWidth;

	strStartRowString = strStartRowString + " ";
	for (uint32 seek = 0; seek < this->m_BoardWidth; seek = seek + 1)
	{
		strStartRowString = strStartRowString + "   " + ColSymbolArray[seek];
	}
	strStartRowString = strStartRowString + "\n";

	strStartRowString = strStartRowString + "  ¨X";
	strMidRowString = strMidRowString + "  ¨d";
	strEndRowString = strEndRowString + "  ¨^";

	for (uint32 seek = 1; seek < Size; seek = seek + 1)
	{
		strStartRowString = strStartRowString + "¨T¨j";
		strMidRowString = strMidRowString + "¨T¨p";
		strEndRowString = strEndRowString + "¨T¨m";
	}
	strStartRowString = strStartRowString + "¨T¨[\n";
	strMidRowString = strMidRowString + "¨T¨g\n";
	strEndRowString = strEndRowString + "¨T¨a\n";

	Terminal.iOutputString(strStartRowString, STC_GREY);

	char Buffer[32];

	for (uint32 seek_y = 0; seek_y < this->m_BoardHeight; seek_y = seek_y + 1)
	{
		Buffer[0] = RowSymbolArray[seek_y];
		Buffer[1] = '\0';

		Terminal.iOutputString(Buffer, STC_GREY);
		Terminal.iOutputString(" ¨U", STC_GREY);

		for (uint32 seek_x = 0; seek_x < this->m_BoardWidth; seek_x = seek_x + 1)
		{
			switch (Board[seek_x][seek_y])
			{
			case eHOPSTATE::BS_EMPTY:
				Buffer[0] = ' ';
				break;
			case  eHOPSTATE::BS_PLAYER_ONE:
				Buffer[0] = 'O';
				break;
			case  eHOPSTATE::BS_PLAYER_TWO:
				Buffer[0] = 'X';
				break;
			case  eHOPSTATE::BS_EVALUATE:
				Buffer[0] = '*';
				break;
			case eHOPSTATE::BS_INVALID:
				Buffer[0] = '#';
			default:
				Buffer[0] = ' ';
				break;
			}
			Terminal.iOutputString(Buffer, STC_GREY);
			Terminal.iOutputString(" ¨U", STC_GREY);
		}
		Terminal.iOutputString("\n", STC_GREY);

		(seek_y + 1) != this->m_BoardHeight ? Terminal.iOutputString(strMidRowString, STC_GREY) : Terminal.iOutputString(strEndRowString, STC_GREY);
	}
}