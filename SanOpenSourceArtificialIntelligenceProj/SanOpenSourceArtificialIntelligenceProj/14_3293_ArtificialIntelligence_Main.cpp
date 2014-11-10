#include"iostream"
#include"process.h"
#include"ProblemUIFunc.h"
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

	char Buffer[1024];

	::cout << "Please select search problem set \n\t(J for Jugs, M for MC, H for HOP)\nCommander>";
	::cin >> Buffer;

	switch (Buffer[0])
	{
	case 'J':
		JugsProblem(&Terminal, &strOutput);
		break;
	case 'H':
		::cin.getline(Buffer, 1024);
		HOPProblem(&Terminal, &strOutput);
		break;
	case 'M':
		MCProblem(&Terminal, &strOutput);
		break;
	default:
		break;
	}

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");

	::system("pause");
	return 0;
}