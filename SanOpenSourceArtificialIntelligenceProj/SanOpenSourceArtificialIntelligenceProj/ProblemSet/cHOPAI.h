#include"time.h"
#include"..\AI\cMinMaxAlgorithm.h"
#include"cHOP.h"
using namespace std;
using namespace San;
using namespace San::AI;
#pragma once
#ifndef __CHOPAI_H__
#define __CHOPAI_H__

struct HOPSTATE
{
	BOARD2D Board;
	POINT2 PiecePosition;
	vector<POINT2> ActionList;
	eHOPSTATE CurrentPlayer;
	sfloat Quality;
	//POINT2 TargetPosition;
};

class cHOPAI : public cMinMaxAlgorithm<HOPSTATE, time_t, cSanTerminalDevice*>
{
private:
	cHOP* m_pHOP;
protected:
	uint32 _GenerateStates(SEARCHTREE<HOPSTATE, sfloat>* pTreeNode, const eMINMAXSEARCHTYPE Type);
	sfloat _EvaluateState(SEARCHTREE<HOPSTATE, sfloat>* pTreeNode);

	/*Qucik evaluate is the fucntion for alpha-beta pruning algorithm to sort each middle layers*/
	sfloat _QuickEvaluateState(SEARCHTREE<HOPSTATE, sfloat>* pTreeNode);

	bool cHOPAI::_BoundaryCheck(const time_t &Value);
public:
	cHOPAI(cHOP* pHOP);
	~cHOPAI();

	void iSetHOPPointer(cHOP* pHOP);

	cHOP* iGetHOPPointer() const;

	HOPSTATE iSearch(const BOARD2D &InitState, const uint32 MaxSearchDepth, const time_t &RestrictionValue, cSanTerminalDevice* pTerminal);
};
#endif