#include"..\RelyOn\Core\SanString.h"
#include"..\RelyOn\Core\SanTypes.h"
#include"..\RelyOn\Core\SanMathematics.h"
#include"..\RelyOn\Device\cSanTerminalDeviceWin.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
using namespace San::Device;
#pragma once
#ifndef __CHOP_H__
#define __CHOP_H__
enum eHOPSTATE
{
	BS_PLAYER_ONE	= 0x00,
	BS_PLAYER_TWO	= 0x01,
	BS_PLAYER_COUNT	= 0x02,
	BS_EMPTY		= 0x03,
	BS_EVALUATE		= 0x04,
	BS_INVALID		= 0x10,
	BS_WIN			= 0x20,
	BS_TIE			= 0x30,
};

class cHOP;

struct POINT2
{
public:
	int32 x;
	int32 y;
public:
	POINT2()
		:x(0), y(0)
	{};
	POINT2(const int32 x, const int32 y)
		:x(x), y(y)
	{};
	~POINT2(){};

	bool operator==(const POINT2 &Point) const
	{
		return (this->x == Point.x) && (this->y == Point.y);
	}
};

/*Board data structure*/
struct BOARD2D
{
public:
	eHOPSTATE B[6][6];
public:
	BOARD2D()
	{
		B[0][0] = BS_EMPTY; B[0][1] = BS_EMPTY; B[0][2] = BS_EMPTY; B[0][3] = BS_EMPTY; B[0][4] = BS_EMPTY; B[0][5] = BS_EMPTY;
		B[1][0] = BS_EMPTY; B[1][1] = BS_EMPTY; B[1][2] = BS_EMPTY; B[1][3] = BS_EMPTY; B[1][4] = BS_EMPTY; B[1][5] = BS_EMPTY;
		B[2][0] = BS_EMPTY; B[2][1] = BS_EMPTY; B[2][2] = BS_EMPTY; B[2][3] = BS_EMPTY; B[2][4] = BS_EMPTY; B[2][5] = BS_EMPTY;
		B[3][0] = BS_EMPTY; B[3][1] = BS_EMPTY; B[3][2] = BS_EMPTY; B[3][3] = BS_EMPTY; B[3][4] = BS_EMPTY; B[3][5] = BS_EMPTY;
		B[4][0] = BS_EMPTY; B[4][1] = BS_EMPTY; B[4][2] = BS_EMPTY; B[4][3] = BS_EMPTY; B[4][4] = BS_EMPTY; B[4][5] = BS_EMPTY;
		B[5][0] = BS_EMPTY; B[5][1] = BS_EMPTY; B[5][2] = BS_EMPTY; B[5][3] = BS_EMPTY; B[5][4] = BS_EMPTY; B[5][5] = BS_EMPTY;
	};
	BOARD2D(const BOARD2D &Board)
	{
		B[0][0] = Board.B[0][0]; B[0][1] = Board.B[0][1]; B[0][2] = Board.B[0][2]; B[0][3] = Board.B[0][3]; B[0][4] = Board.B[0][4]; B[0][5] = Board.B[0][4];
		B[1][0] = Board.B[1][0]; B[1][1] = Board.B[1][1]; B[1][2] = Board.B[1][2]; B[1][3] = Board.B[1][3]; B[1][4] = Board.B[1][4]; B[1][5] = Board.B[1][4];
		B[2][0] = Board.B[2][0]; B[2][1] = Board.B[2][1]; B[2][2] = Board.B[2][2]; B[2][3] = Board.B[2][3]; B[2][4] = Board.B[2][4]; B[2][5] = Board.B[2][4];
		B[3][0] = Board.B[3][0]; B[3][1] = Board.B[3][1]; B[3][2] = Board.B[3][2]; B[3][3] = Board.B[3][3]; B[3][4] = Board.B[3][4]; B[3][5] = Board.B[3][4];
		B[4][0] = Board.B[4][0]; B[4][1] = Board.B[4][1]; B[4][2] = Board.B[4][2]; B[4][3] = Board.B[4][3]; B[4][4] = Board.B[4][4]; B[4][5] = Board.B[4][4];
		B[5][0] = Board.B[5][0]; B[5][1] = Board.B[5][1]; B[5][2] = Board.B[5][2]; B[5][3] = Board.B[5][3]; B[5][4] = Board.B[5][4]; B[5][5] = Board.B[5][4];
	}
	~BOARD2D(){};

	BOARD2D& operator=(const BOARD2D &Board)
	{
		B[0][0] = Board.B[0][0]; B[0][1] = Board.B[0][1]; B[0][2] = Board.B[0][2]; B[0][3] = Board.B[0][3]; B[0][4] = Board.B[0][4]; B[0][5] = Board.B[0][4];
		B[1][0] = Board.B[1][0]; B[1][1] = Board.B[1][1]; B[1][2] = Board.B[1][2]; B[1][3] = Board.B[1][3]; B[1][4] = Board.B[1][4]; B[1][5] = Board.B[1][4];
		B[2][0] = Board.B[2][0]; B[2][1] = Board.B[2][1]; B[2][2] = Board.B[2][2]; B[2][3] = Board.B[2][3]; B[2][4] = Board.B[2][4]; B[2][5] = Board.B[2][4];
		B[3][0] = Board.B[3][0]; B[3][1] = Board.B[3][1]; B[3][2] = Board.B[3][2]; B[3][3] = Board.B[3][3]; B[3][4] = Board.B[3][4]; B[3][5] = Board.B[3][4];
		B[4][0] = Board.B[4][0]; B[4][1] = Board.B[4][1]; B[4][2] = Board.B[4][2]; B[4][3] = Board.B[4][3]; B[4][4] = Board.B[4][4]; B[4][5] = Board.B[4][4];
		B[5][0] = Board.B[5][0]; B[5][1] = Board.B[5][1]; B[5][2] = Board.B[5][2]; B[5][3] = Board.B[5][3]; B[5][4] = Board.B[5][4]; B[5][5] = Board.B[5][4];
		return *this;
	}

	const eHOPSTATE* operator[](const uint32 Position) const
	{
		return B[Position];
	};

	eHOPSTATE* operator[](const uint32 Position)
	{
		return B[Position];
	};
};

typedef uint32(*PLAYERMOVEFUNC)(cHOP*, SHANDLE UserData);

struct BOARDSTATE
{
	BOARD2D Board;
	eHOPSTATE PlayerID;
	vector<POINT2> Action;
};

class cHOP
{
private:
	BOARD2D				m_Board;
	uint32				m_BoardWidth;
	uint32				m_BoardHeight;
	vector<BOARDSTATE>	m_BoardHistory;
	eHOPSTATE			m_CurrentPlayerID;
	vector<pair<PLAYERMOVEFUNC, SHANDLE>> m_PlayerFuncArray;	// iUpdate() will call this list to automaticlly update user move, if function pointer is nullptr, waiting
	bool				m_bState;
public:
	const eHOPSTATE* operator[](const uint32 Position) const;
protected:
	/*Core function to generate all avaliable move*/
	void _GeneratePieceAvaliableMoveArray(const BOARD2D &Board, const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const POINT2 &CurrentPosition, vector<POINT2> &VisitedPosition, const vector<POINT2> CurrentActionList, vector<vector<POINT2>> &ActionArray) const;
public:
	cHOP(const uint32 BoardWidth = 6, const uint32 BoardHeight = 6);
	~cHOP();

	void iInitializeBoardState();
	void iClearBoard();

	bool iRegisteUserFunc(const eHOPSTATE PlayerID, PLAYERMOVEFUNC Func, SHANDLE UserData);
	bool iReleaseUserFunc(const eHOPSTATE PlayerID);

	uint32 iGetBoardHeight() const;
	uint32 iGetBoardWidth() const;
	BOARD2D iGetBoard() const;

	eHOPSTATE iGetCurrentPlayerID() const;

	/*Get all pieces coordinate by PlayerID*/
	vector<POINT2> iGetPiecesPositionArray(const eHOPSTATE PlayerID) const;

	/*Get all available move list by piece coordinate*/
	vector<vector<POINT2>> iGetPieceAvaliableMoveArray(const eHOPSTATE PlayerID, const POINT2 &PiecePosition);

	/*Get all available move list by piece coordinate*/
	vector<vector<POINT2>> iGetPieceAvaliableMoveArray(const BOARD2D &Board, const eHOPSTATE PlayerID, const POINT2 &PiecePosition) const;

	/*Get position state (Player or Empty) by piece coordinate*/
	eHOPSTATE iGetBoardPositionState(const POINT2 &Position) const;

	/*Get history board position state (Player or Empty) by piece coordinate*/
	eHOPSTATE iGetBoardHistoryPositionState(const uint32 HistoryBoardID, const POINT2 &Position) const;

	/*Get history board*/
	BOARDSTATE iGetBoardHistoryState(const uint32 HistoryBoardID) const;

	/*Player move function*/
	uint32 iPlayerMove(const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const vector<POINT2> &ActionList);

	/*Evaluate one move*/
	uint32 iEvaluateMove(const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const vector<POINT2> &ActionList);

	/*Evaluate one move by given board*/
	uint32 iEvaluateMove(const BOARD2D &Board, const eHOPSTATE PlayerID, const POINT2 &PiecePosition, const vector<POINT2> &ActionList) const;

	/*Evaluate the winner*/
	uint32 iEvaluateBoard() const;

	/*Evaluate the board winner*/
	uint32 iEvaluateBoard(const BOARD2D &Board) const;

	/*Update game by one move*/
	uint32 iUpdateGame();

	/*Terminate game*/
	uint32 iTerminateGame();

	/*Pop current board state*/
	bool iPopBoardHistory();

	/*Render board, print*/
	void iRenderBoard(cSanTerminalDevice &Terminal) const;

	/*Render given board, print*/
	void iRenderBoard(const BOARD2D &Board, cSanTerminalDevice &Terminal) const;
};
#endif