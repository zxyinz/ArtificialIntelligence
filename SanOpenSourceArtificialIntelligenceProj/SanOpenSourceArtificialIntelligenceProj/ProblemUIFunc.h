#include"RelyOn\Device\cSanTerminalDeviceWin.h"
#include"RelyOn\FileIO\SanFileIO.h"
#include"AI\AIContainer.h"
#include"ProblemSet\cJugsProblem.h"
#include"ProblemSet\cMCProblem.h"
#include"ProblemSet\cHOPAI.h"
#include"AI\cPropositionalLogicResolutionAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
using namespace San::AI;
#pragma once
#ifndef __PROBLEMUIFUNC_H__
#define __PROBLEMUIFUNC_H__

void	JugsProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

void	MCProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

void	HOPProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);
uint32	HOPProblemUserMoveFunc(cHOP* pHop, SHANDLE UserData);
uint32	HOPProblemAIMoveFunc(cHOP* pHop, SHANDLE UserData);
void	HOPProblemPrintTime(cSanTerminalDevice* pTerminal, const SPOINT3 &Coord, const time_t Time, const eSANTERMINALCOLOR TextColor = STC_DEFAULT, const eSANTERMINALCOLOR BackgroundColor = STC_DEFAULT);

void CNFProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);
void ResolutionAlgorithmProblem(cSanTerminalDevice* pTerminal, SString* pstrOutputString);

#endif