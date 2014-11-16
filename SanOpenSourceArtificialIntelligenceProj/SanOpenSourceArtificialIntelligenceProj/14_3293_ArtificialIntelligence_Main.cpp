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

	Terminal.iOutputString(_SSTR("Please select search problem set: \r\n"));
	Terminal.iOutputString(_SSTR("\tJ                   for Jugs\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tM                   for MC\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tH                   for HOP\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tCNF                 for CNF\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tResolutionAlgorithm for Propositional Logic Resolution Algorithm\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("Commander> "));
	::cin.getline(Buffer, 1024);

	SString strCommand = Buffer;


	if (strCommand == _SSTR("J")){ JugsProblem(&Terminal, &strOutput); }
	if (strCommand == _SSTR("H")){ HOPProblem(&Terminal, &strOutput); }
	if (strCommand == _SSTR("M")){ MCProblem(&Terminal, &strOutput); }
	if (strCommand == _SSTR("CNF")){ CNFProblem(&Terminal, &strOutput); }
	if (strCommand == _SSTR("ResolutionAlgorithm")){ ResolutionAlgorithmProblem(&Terminal, &strOutput); }

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");

	::system("pause");
	return 0;
}