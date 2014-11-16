#include"ProblemUIFunc.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
using namespace San::AI;
void JugsProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	if ((pTerminal == nullptr) || (pstrOutputString == nullptr))
	{
		return;
	}

	eSEARCHALGORITHMTYPE Type;

	char Buffer[1024];

	uint32 SmallVal, LargeVal;
	::cout << "Please enter state (SmallVal LargeVal):";
	::cin >> SmallVal >> LargeVal;

	::cout << "Please select search algorithm (BFS, DFS, US, GS, AS):";
	::cin >> Buffer;

	SString strName = Buffer;
	if (strName == "BFS")
	{
		Type = SAT_BREADTH_FIRST_SEARCH;
	}
	if (strName == "DFS")
	{
		Type = SAT_DEPTH_FIRST_SEARCH;
	}
	if (strName == "US")
	{
		Type = SAT_UNIFORM_COST_SEARCH;
	}
	if (strName == "GS")
	{
		Type = SAT_GREEDY_SEARCH;
	}
	if (strName == "AS")
	{
		Type = SAT_A_STAR_SEARCH;
	}

	cJugsProblem Searcher;
	/*JUGS( Small - Init - Value, Large - Init - Value)*/
	Searcher.iSearch(JUGS(SmallVal, LargeVal), 0, Type, pstrOutputString);
}
void MCProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	if ((pTerminal == nullptr) || (pstrOutputString == nullptr))
	{
		return;
	}

	eSEARCHALGORITHMTYPE Type;

	char Buffer[1024];

	uint32 LeftMNum, LeftCNum, RightMNum, RightCNum, BoatMNum, BoatCNum, IsBoatOnLeftSide;
	::cout << "Please enter state (LeftMNum, LeftCNum, RightMNum, RightCNum, BoatMNum, BoatCNum, IsBoatOnLeftSide):";
	::cin >> LeftMNum >> LeftCNum >> RightMNum >> RightCNum >> BoatMNum >> BoatCNum >> IsBoatOnLeftSide;

	::cout << "Please select search algorithm (BFS, DFS, US, GS, AS):";
	::cin >> Buffer;

	SString strName = Buffer;
	if (strName == "BFS")
	{
		Type = SAT_BREADTH_FIRST_SEARCH;
	}
	if (strName == "DFS")
	{
		Type = SAT_DEPTH_FIRST_SEARCH;
	}
	if (strName == "US")
	{
		Type = SAT_UNIFORM_COST_SEARCH;
	}
	if (strName == "GS")
	{
		Type = SAT_GREEDY_SEARCH;
	}
	if (strName == "AS")
	{
		Type = SAT_A_STAR_SEARCH;
	}

	cMCProblem Searcher;
	/*MC(Left-M-Num, Left-C-Num, Right-M-Num, Right-C-Num, Boat-M-Num, Boat-C-Num, Is-Boat-On-Left-Side)*/
	Searcher.iSearch(MC(LeftMNum, LeftCNum, RightMNum, RightCNum, BoatMNum, BoatCNum, IsBoatOnLeftSide), 0, Type, pstrOutputString);
}
void HOPProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	if ((pTerminal == nullptr) || (pstrOutputString == nullptr))
	{
		return;
	}

	std::ios::sync_with_stdio(false);

	SString strTitle;
	strTitle = strTitle + "/*--------------- CSE5290 Artificial Intelligence ---------------*/\n";
	strTitle = strTitle + "/*Project: Alpha-Beta Pruning Algorithm                          */\n";
	strTitle = strTitle + "/*Author:  Wang Kai                                              */\n";
	strTitle = strTitle + "/*---------------------------------------------------------------*/\n";

	pTerminal->iSetTerminalTittle(_SSTR("San Artificial Intelligence Terminal"));

	/*Create HOP Game Object*/
	cHOP HOPGame(6, 6);

	uint32 GameCount = 0;

	while (true)
	{
		pTerminal->iClearScreen();

		char Buffer[1024];

		int32 UserID = 0;

		pTerminal->iOutputString(strTitle);
		pTerminal->iOutputString("Please choose first player (User - 1, AI - 2):");

		::cin.getline(Buffer, 1024);

		switch (::gloSToI(Buffer))
		{
		case 1:
			/*Registe Move Function*/
			HOPGame.iRegisteUserFunc(eHOPSTATE::BS_PLAYER_ONE, HOPProblemUserMoveFunc, pTerminal);
			HOPGame.iRegisteUserFunc(eHOPSTATE::BS_PLAYER_TWO, HOPProblemAIMoveFunc, pTerminal);
			break;
		case 2:
			/*Registe Move Function*/
			HOPGame.iRegisteUserFunc(eHOPSTATE::BS_PLAYER_TWO, HOPProblemUserMoveFunc, pTerminal);
			HOPGame.iRegisteUserFunc(eHOPSTATE::BS_PLAYER_ONE, HOPProblemAIMoveFunc, pTerminal);
			break;
		default:
			pTerminal->iOutputString("Error: Invalid value, user id out of range 1 - 2\n", STC_WHITE, STC_RED);
			::system("pause");
			UserID = -1;
			break;
		}

		if (UserID < 0)
		{
			continue;
		}

		GameCount = GameCount + 1;
		HOPGame.iInitializeBoardState();

		uint32 MoveCount = 1;

		time_t GameStartTime = ::clock();

		/*Move Loop*/
		while (true)
		{
			pTerminal->iClearScreen();

			pTerminal->iOutputString(strTitle);
			pTerminal->iOutputString(SString("Current Game ID: ") + ::gloIToS(GameCount) + _SSTR(", Move: ") + ::gloIToS(MoveCount) + _SSTR("\n\n"));
			HOPGame.iRenderBoard(*pTerminal);

			/*Update Move*/
			uint32 State = HOPGame.iUpdateGame();

			/*Check Result*/
			if (State == eHOPSTATE::BS_INVALID)
			{
				pTerminal->iOutputString(_SSTR("Error: Invalid game state\n"), STC_WHITE, STC_RED);
				::system("pause");
				break;
			}

			/*Win*/
			if ((State & 0xf0) == ::eHOPSTATE::BS_WIN)
			{
				uint32 PlayerID = State & 0x0f;

				if (PlayerID >= eHOPSTATE::BS_PLAYER_COUNT)
				{
					pTerminal->iOutputString(_SSTR("Error: Invalid player ID\n"), STC_WHITE, STC_RED);
				}
				else
				{
					pTerminal->iOutputString(_SSTR("*** Current Game Result: Player #" + ::gloIToS(PlayerID + 1) + " WIN, press any key to start a new game ***\n"));
				}
				::system("pause");
				break;
			}

			/*Tie, only occur when user teminate game*/
			if ((State & 0xf0) == ::eHOPSTATE::BS_TIE)
			{
				pTerminal->iOutputString(_SSTR("*** Current Game Result: TIE, press any key to start a new game ***\n"));
				::system("pause");
				break;
			}

			MoveCount = MoveCount + 1;
			//::system("pause");
		}

	}
}
/*User move function*/
uint32 HOPProblemUserMoveFunc(cHOP* pHop, SHANDLE UserData)
{
	cSanTerminalDevice* pTerminal = (cSanTerminalDevice*) UserData;

	if (pTerminal == nullptr)
	{
		return eHOPSTATE::BS_INVALID;
	}

	if (pHop == nullptr)
	{
		pTerminal->iOutputString("Error: Invalid user func input, pHop == nullptr\n", STC_WHITE, STC_RED);
		::system("pause");
		return eHOPSTATE::BS_INVALID;
	}

	pTerminal->iOutputString(_SSTR("User #") + ::gloIToS(pHop->iGetCurrentPlayerID() + 1) + _SSTR(" Move. Waiting for command...\n"));

	char Buffer[1024];
	//::cin.getline(Buffer, 1024);

	while (true)
	{
		pTerminal->iOutputString(_SSTR("Commander>"), STC_GREY);
		::cin.getline(Buffer, 1024);

		SString strCommand = Buffer;

		if (strCommand.empty())
		{
			//pTerminal->iClearLine();
			continue;
		}

		if (strCommand == _SSTR("Help"))
		{
			pTerminal->iOutputString(_SSTR("Command:          Description:                    \n"));
			pTerminal->iOutputString(_SSTR("Help              Get help information            \n"));
			pTerminal->iOutputString(_SSTR("Terminate         Terminate current game by fource\n"));


			continue;
		}

		/*Terminate game*/
		if (strCommand == _SSTR("Terminate"))
		{
			return pHop->iTerminateGame();
		}

		uint32 bIsActionList = true;
		vector<POINT2> ActionList;

		vector<SString> SubItems = ::gloGetStringItems(strCommand);

		/*Calculate and execute one piece all possible moves*/
		if ((SubItems[0] == _SSTR("Evaluate")) || (SubItems[0] == _SSTR("Exec")))
		{
			if (SubItems.size() != (SubItems[0] == _SSTR("Evaluate") ? 2 : 3))
			{
				pTerminal->iOutputString("Error: Invalid command, wrong parameter format\n", STC_WHITE, STC_RED);
				continue;
			}

			vector<SString> CoordString = ::gloGetStringItems(SubItems[1], "(),");

			if (CoordString.size() != 2)
			{
				pTerminal->iOutputString("Error: Invalid command, wrong parameter format\n", STC_WHITE, STC_RED);
				continue;
			}

			if (CoordString[0].size() > 1)
			{
				pTerminal->iOutputString("Error: Invalid command, wrong parameter format\n", STC_WHITE, STC_RED);
				continue;
			}

			if (CoordString[1].size() > 1)
			{
				pTerminal->iOutputString("Error: Invalid command, wrong parameter format\n", STC_WHITE, STC_RED);
				continue;
			}

			POINT2 Coord(CoordString[1][0] >= 'A' ? CoordString[1][0] - 'A' : CoordString[1][0] - '0', CoordString[0][0] - '0');

			if ((Coord.x < 0) || (Coord.x >= pHop->iGetBoardWidth()) || (Coord.y < 0) || (Coord.y >= pHop->iGetBoardHeight()))
			{
				pTerminal->iOutputString("Error: Invalid command, wrong parameter format\n", STC_WHITE, STC_RED);
				continue;
			}

			vector<vector<POINT2>> ActionList = pHop->iGetPieceAvaliableMoveArray(pHop->iGetCurrentPlayerID(), Coord);

			if (SubItems[0] == _SSTR("Exec"))
			{
				uint32 Index = ::gloSToI(SubItems[2]);

				/*pTerminal->iOutputString(_SSTR("Piece:\t(") + ::gloIToS(Coord.y) + _SSTR(", ") + ::gloIToS(Coord.x) + _SSTR(")\nAction:\t"));

				for (uint32 seek_action = 0; seek_action < ActionList[Index].size(); seek_action = seek_action + 1)
				{
				pTerminal->iOutputString(_SSTR("(") + ::gloIToS(ActionList[Index][seek_action].y) + _SSTR(", ") + ::gloIToS(ActionList[Index][seek_action].x) + _SSTR(") "));
				}

				pTerminal->iOutputString("\n");*/

				if (pHop->iEvaluateMove(pHop->iGetCurrentPlayerID(), Coord, ActionList[Index]) != eHOPSTATE::BS_INVALID)
				{
					//pTerminal->iOutputString(_SSTR("Move evaluate successful, press any key to execute this move\n"));
					//::system("pause");
					return pHop->iPlayerMove(pHop->iGetCurrentPlayerID(), Coord, ActionList[Index]);
				}

				continue;
			}

			pTerminal->iOutputString(_SSTR("Piece:\t(") + ::gloIToS(Coord.y) + _SSTR(", ") + ::gloIToS(Coord.x) + _SSTR(")\nAction Sequence List:\n"));

			for (uint32 seek = 0; seek < ActionList.size(); seek = seek + 1)
			{
				pTerminal->iOutputString(_SSTR("#") + ::gloIToS(seek) + _SSTR("\t"));
				for (uint32 seek_action = 0; seek_action < ActionList[seek].size(); seek_action = seek_action + 1)
				{
					pTerminal->iOutputString(_SSTR("(") + ::gloIToS(ActionList[seek][seek_action].y) + _SSTR(", ") + ::gloIToS(ActionList[seek][seek_action].x) + _SSTR(") "));
				}
				pTerminal->iOutputString(_SSTR("\n"));
			}

			continue;
		}

		if (SubItems.size() < 2)
		{
			pTerminal->iOutputString("Error: Invalid command, undefined command type\n", STC_WHITE, STC_RED);
			continue;
		}

		/*Evaluate user input list, if valid, make move*/
		for (uint32 seek = 0; seek < SubItems.size(); seek = seek + 1)
		{
			vector<SString> CoordString = ::gloGetStringItems(SubItems[seek], "(),");

			if (CoordString.size() != 2)
			{
				bIsActionList = false;
				break;
			}

			if (CoordString[0].size() > 1)
			{
				bIsActionList = false;
				break;
			}

			if (CoordString[1].size() > 1)
			{
				bIsActionList = false;
				break;
			}

			POINT2 Coord(CoordString[1][0] >= 'A' ? CoordString[1][0] - 'A' : CoordString[1][0] - '0', CoordString[0][0] - '0');

			if ((Coord.x < 0) || (Coord.x >= pHop->iGetBoardWidth()) || (Coord.y < 0) || (Coord.y >= pHop->iGetBoardHeight()))
			{
				bIsActionList = false;
				break;
			}

			ActionList.push_back(Coord);
		}

		if (bIsActionList)
		{
			POINT2 Coord = ActionList[0];

			ActionList.erase(ActionList.begin());

			pTerminal->iOutputString(_SSTR("Piece:\t(") + ::gloIToS(Coord.y) + _SSTR(", ") + ::gloIToS(Coord.x) + _SSTR(")\nAction:\t"));

			for (uint32 seek_action = 0; seek_action < ActionList.size(); seek_action = seek_action + 1)
			{
				pTerminal->iOutputString(_SSTR("(") + ::gloIToS(ActionList[seek_action].y) + _SSTR(", ") + ::gloIToS(ActionList[seek_action].x) + _SSTR(") "));
			}

			pTerminal->iOutputString("\n");

			if (pHop->iEvaluateMove(pHop->iGetCurrentPlayerID(), Coord, ActionList) != eHOPSTATE::BS_INVALID)
			{
				pTerminal->iOutputString(_SSTR("Move evaluate successful, press any key to execute this move\n"));
				::system("pause");
				return pHop->iPlayerMove(pHop->iGetCurrentPlayerID(), Coord, ActionList);
			}
		}

		pTerminal->iOutputString("Error: Invalid command, illgeal move\n", STC_WHITE, STC_RED);
	}

	return eHOPSTATE::BS_EVALUATE;
}

/*AI move function*/
uint32 HOPProblemAIMoveFunc(cHOP* pHop, SHANDLE UserData)
{
	cSanTerminalDevice* pTerminal = (cSanTerminalDevice*) UserData;

	if (pTerminal == nullptr)
	{
		return eHOPSTATE::BS_INVALID;
	}

	/*Create AI*/
	cHOPAI AI(pHop);

	SPOINT3 Pos = pTerminal->iGetCurrentCursorPosition();

	pTerminal->iOutputString(_SSTR("AI Move, Evaluation..."));

	/*Get start time*/
	time_t StartTime = ::clock();

	/*Search, no depth limit, time limit in 59 sec*/
	HOPSTATE State = AI.iSearch(pHop->iGetBoard(), 0, StartTime + (1000 * 10), pTerminal);

	/*Print result*/
	pTerminal->iOutputString(_SSTR("AI Move, Evaluation done.\t\t\t\n"), Pos);

	HOPProblemPrintTime(pTerminal, Pos + SVECTOR3(27, 0, 0), ::clock() - StartTime);

	pTerminal->iOutputString(_SSTR("\nPiece:\t(") + ::gloIToS(State.PiecePosition.y) + _SSTR(", ") + ::gloIToS(State.PiecePosition.x) + _SSTR(")\nAction:\t"));

	for (uint32 seek_action = 0; seek_action < State.ActionList.size(); seek_action = seek_action + 1)
	{
		pTerminal->iOutputString(_SSTR("(") + ::gloIToS(State.ActionList[seek_action].y) + _SSTR(", ") + ::gloIToS(State.ActionList[seek_action].x) + _SSTR(") "));
	}

	pTerminal->iOutputString("\n");

	/*Evaluate and make move*/
	if (pHop->iEvaluateMove(pHop->iGetCurrentPlayerID(), State.PiecePosition, State.ActionList) != eHOPSTATE::BS_INVALID)
	{
		pTerminal->iOutputString(_SSTR("Move evaluate successful, press any key to execute this move\n"));
		::system("pause");
		return pHop->iPlayerMove(pHop->iGetCurrentPlayerID(), State.PiecePosition, State.ActionList);
	}
	::system("pause");

	return eHOPSTATE::BS_EVALUATE;
}
void HOPProblemPrintTime(cSanTerminalDevice* pTerminal, const SPOINT3 &Coord, const time_t Time, const eSANTERMINALCOLOR TextColor, const eSANTERMINALCOLOR BackgroundColor)
{

	uint32 Hour = Time / (1000 * 60 * 60);
	uint32 Min = (Time % (1000 * 60 * 60)) / (1000 * 60);
	uint32 Sec = (Time % (1000 * 60)) / 1000;
	uint32 MS = Time % 1000;

	pTerminal->iOutputString(::gloIToS(Hour) + _SSTR(" hour ") + ::gloIToS(Min) + _SSTR(" min ") + ::gloIToS(Sec) + _SSTR(" sec ") + ::gloIToS(MS) + _SSTR(" ms"), Coord, TextColor, BackgroundColor);
}
void CNFProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	std::ios::sync_with_stdio(false);

	SString strTitle;
	strTitle = strTitle + "/*--------------- CSE5290 Artificial Intelligence ---------------*/\n";
	strTitle = strTitle + "/*Project: Propositional Logic Resolution Algorithm              */\n";
	strTitle = strTitle + "/*Author:  Wang Kai                                              */\n";
	strTitle = strTitle + "/*---------------------------------------------------------------*/\n";

	SString strLine = _SSTR("--------------------------------------------------\r\n");

	cSanTerminalDevice Terminal;
	Terminal.iSetTerminalTittle(_SSTR("San Artificial Intelligence Terminal"));

	SString strOutput;

	char Buffer[1024];

	Terminal.iOutputString(strTitle);

	SString strCommand = Buffer;

	while (true)
	{
		Terminal.iClearScreen();

		Terminal.iOutputString(strTitle);

		Terminal.iOutputString(_SSTR("INPUT: Propositional Logic Rule String\r\n"), STC_GREY);
		Terminal.iOutputString(_SSTR("EXAMPLE INPUT: !( !a & b ) => ( c & ( d | !e ) )\r\n"), STC_GREY);
		Terminal.iOutputString(_SSTR("EXAMPLE OUTPUT: (a | c) & (a | d | !e) & (b | c) & (b | d | !e)\r\n"), STC_GREY);

		Terminal.iOutputString(_SSTR("Command>"));
		::cin.getline(Buffer, 1024);

		strCommand = Buffer;

		PROPOSITIONALLOGICRULE Rule(strCommand);

		Terminal.iOutputString(_SSTR("\r\nINPUT TREE:\r\n"), STC_GREY);
		Terminal.iOutputString(strLine, STC_GREY);

		Terminal.iOutputString(Rule.iPrintTree() + _SSTR("\r\n"));

		Terminal.iOutputString(_SSTR("OUTPUT TREE (CNF TREE):\r\n"), STC_GREY);
		Terminal.iOutputString(strLine, STC_GREY);

		Rule.iConvertToCNF();

		Terminal.iOutputString(Rule.iPrintTree() + _SSTR("\r\n"));

		::system("pause");
	}
}
void ResolutionAlgorithmProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString)
{
	std::ios::sync_with_stdio(false);

	SString strTitle;
	strTitle = strTitle + "/*--------------- CSE5290 Artificial Intelligence ---------------*/\n";
	strTitle = strTitle + "/*Project: Propositional Logic Resolution Algorithm              */\n";
	strTitle = strTitle + "/*Author:  Wang Kai                                              */\n";
	strTitle = strTitle + "/*---------------------------------------------------------------*/\n";

	SString strLine = _SSTR("--------------------------------------------------\r\n");

	cSanTerminalDevice Terminal;
	Terminal.iSetTerminalTittle(_SSTR("San Artificial Intelligence Terminal"));

	SString strOutput;

	char Buffer[1024];

	Terminal.iOutputString(strTitle);

	SString strCommand = Buffer;

	Terminal.iClearScreen();

	Terminal.iOutputString(strTitle);

	Terminal.iOutputString(_SSTR("COMMAND: Load [knowledge database path]     Load the knowledge database\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("COMMAND: Solve [propositional logic rule]   Solve the logic question\r\n\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("DEFAULT KNOWLEDGE DATABASE PATH:\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\twumpus.txt\tWumpus Knowledge Database\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tunicorn.txt\tUnicorn Knowledge Database\r\n\r\n"), STC_GREY);

	cPropositionalLogicResolutionAlgorithm ResolutionAlgorithm;

	while (true)
	{
		Terminal.iOutputString(_SSTR("Command>"));

		::cin.getline(Buffer, 1024);

		strCommand = Buffer;

		if (strCommand.empty())
		{
			continue;
		}

		vector<SString> strItems = ::gloGetStringItems(strCommand, _SSTR(" "));

		if (strItems.size() < 2)
		{
			Terminal.iOutputString("Error: Invalid command\n", STC_WHITE, STC_RED);
			::system("pause");
			continue;
		}

		if (strItems[0] == _SSTR("Load"))
		{
			SStringA strKnowledgeDataBase;

			if (!gloLoadFile(strItems[1], strKnowledgeDataBase))
			{
				Terminal.iOutputString("Error: Load knowledge database file failed\n", STC_WHITE, STC_RED);
				::system("pause");
				continue;
			}
			vector<SString> strSubString = ::gloGetStringItems(::gloAToT(strKnowledgeDataBase), _SSTR("\r\n"));

			Terminal.iOutputString(_SSTR("\r\nKNOWLEDGE DATABASE LOGIC RULE LIST:\r\n"), STC_GREY);
			Terminal.iOutputString(strLine, STC_GREY);

			for (uint32 seek = 0; seek < strSubString.size(); seek = seek + 1)
			{
				Terminal.iOutputString(strSubString[seek] + _SSTR("\r\n"));
			}

			ResolutionAlgorithm.iClearClauses();

			ResolutionAlgorithm.iGenerateKnowedgeBase(strSubString);

			Terminal.iOutputString(_SSTR("CLAUSES LIST:\r\n"), STC_GREY);
			Terminal.iOutputString(strLine, STC_GREY);

			Terminal.iOutputString(ResolutionAlgorithm.iPrintKnowledgeBase() + _SSTR("\r\n"));

			//::system("pause");

			continue;
		}

		if (strItems[0] == _SSTR("Solve"))
		{
			strCommand = strCommand.substr(6, strCommand.size() - 6);

			Terminal.iOutputString(_SSTR("\r\nInput question:\r\n"), STC_GREY);
			Terminal.iOutputString(strLine, STC_GREY);

			Terminal.iOutputString(strCommand + _SSTR("\r\n\r\n"));

			Terminal.iOutputString(_SSTR("Input question (CNF):\r\n"), STC_GREY);
			Terminal.iOutputString(strLine, STC_GREY);

			PROPOSITIONALLOGICRULE Rule(strCommand);
			Rule.iConvertToCNF();
			Rule.iGenerateLogicString();

			Terminal.iOutputString(Rule.iGetLogicString() + _SSTR("\r\n\r\n"));

			switch (ResolutionAlgorithm.iResolve(strCommand))
			{
			case LR_TRUE:
				Terminal.iOutputString(_SSTR("General Resolve Algorithm Result: TRUE\r\n"));
				break;
			case LR_FALSE:
				Terminal.iOutputString(_SSTR("General Resolve Algorithm Result: FALSE\r\n"));
				break;
			case LR_UNKNOWN:
				Terminal.iOutputString(_SSTR("General Resolve Algorithm Result: UNKNOWN\r\n"));
				break;
			default:
				Terminal.iOutputString(_SSTR("Error: Unknown result\r\n"), STC_WHITE, STC_RED);
				break;
			}

			switch (ResolutionAlgorithm.iResolveOptimize(strCommand))
			{
			case LR_TRUE:
				Terminal.iOutputString(_SSTR("Optimized Resolve Algorithm Result: TRUE\r\n"));
				break;
			case LR_FALSE:
				Terminal.iOutputString(_SSTR("Optimized Resolve Algorithm Result: FALSE\r\n"));
				break;
			case LR_UNKNOWN:
				Terminal.iOutputString(_SSTR("Optimized Resolve Algorithm Result: UNKNOWN\r\n"));
				break;
			default:
				Terminal.iOutputString(_SSTR("Error: Unknown result\r\n"), STC_WHITE, STC_RED);
				break;
			}
			::system("pause");
			continue;
		}

		Terminal.iOutputString("Error: Invalid command\n", STC_WHITE, STC_RED);
		::system("pause");
	}
}