#include"iostream"
#include"process.h"
#include"RelyOn\Device\cSanTerminalDeviceWin.h"
#include"RelyOn\FileIO\SanFileIO.h"
#include"AI\AIContainer.h"
#include"ProblemSet\cJugsProblem.h"
#include"ProblemSet\cMCProblem.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
using namespace San::AI;

int main()
{
	cSanTerminalDevice Terminal;
	Terminal.iSetTerminalTittle(_SSTR("San Artificial Intelligence Terminal"));

	SString strOutput;

	eSEARCHALGORITHMTYPE Type;

	char Buffer[1024];

	::cout << "Please select search problem set (J for Jugs, M for MC):";
	::cin >> Buffer;

	if (Buffer[0] == 'J')
	{
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
		Searcher.iSearch(JUGS(SmallVal, LargeVal), 0, Type, &strOutput);
	}

	if (Buffer[0] == 'M')
	{
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
		Searcher.iSearch(MC(LeftMNum, LeftCNum, RightMNum, RightCNum, BoatMNum, BoatCNum, IsBoatOnLeftSide), 0, Type, &strOutput);
	}

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");

	::system("pause");
	return 0;
}