#include"cPropositionalLogicResolutionAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::AI;
cPropositionalLogicResolutionAlgorithm::cPropositionalLogicResolutionAlgorithm()
{
	this->m_KnowledgeBase.clear();
}
cPropositionalLogicResolutionAlgorithm::~cPropositionalLogicResolutionAlgorithm()
{
	this->m_KnowledgeBase.clear();
}
bool cPropositionalLogicResolutionAlgorithm::_Resolve(vector<LOGICCLAUSES> InverseAlpha)
{
	vector<LOGICCLAUSES> Clauses = this->m_KnowledgeBase;

	/*Push inverse alpha to clauses*/
	for (uint32 seek = 0; seek < InverseAlpha.size(); seek = seek + 1)
	{
		Clauses.push_back(InverseAlpha[seek]);
	}

	vector<LOGICCLAUSES> NewClauses;

	while (true)
	{
		for (uint32 seek_src = 0; seek_src < Clauses.size(); seek_src = seek_src + 1)
		{
			for (uint32 seek_dest = seek_src + 1; seek_dest < Clauses.size(); seek_dest = seek_dest + 1)
			{
				if (this->_GenerateNewClausesList(Clauses[seek_src], Clauses[seek_dest], Clauses, NewClauses))
				{
					return true;
				}
			}
		}

		if (NewClauses.empty()){ return false; }

		for (uint32 seek = 0; seek < NewClauses.size(); seek = seek + 1)
		{
			Clauses.push_back(NewClauses[seek]);
		}

		NewClauses.clear();
	}

	return false;
}
bool cPropositionalLogicResolutionAlgorithm::_ResolveOptimize(vector<LOGICCLAUSES> InverseAlpha)
{
	//Optimize: Sorted clauses list, base on the clause size
	vector<LOGICCLAUSES> Clauses = this->m_KnowledgeBase;

	/*Push inverse alpha to clauses*/
	for (uint32 seek = 0; seek < InverseAlpha.size(); seek = seek + 1)
	{
		if (this->_FindClause(InverseAlpha[seek], Clauses) == -1)
		{
			this->_InsertClause(InverseAlpha[seek], Clauses);
		}
	}

	vector<LOGICCLAUSES> SrcClauses = InverseAlpha;
	vector<LOGICCLAUSES> VisitedClauses = SrcClauses;

	while (true)
	{
		vector<LOGICCLAUSES> CandidateClausesList;
		vector<pair<SString, SVECTOR3>> ElementTable;

		/*Generate dest clauses list*/
		for (uint32 seek = 0; seek < SrcClauses.size(); seek = seek + 1)
		{
			for (uint32 seek_element = 0; seek_element < SrcClauses[seek].size(); seek_element = seek_element + 1)
			{
				if (this->_GetElementIndex(SrcClauses[seek][seek_element], ElementTable) != -1)
				{
					continue;
				}
				this->_AddElementToTable(SrcClauses[seek][seek_element], ElementTable);

				/*Find suit candidate clause*/
				for (uint32 seek_candidate = 0; seek_candidate < Clauses.size(); seek_candidate = seek_candidate + 1)
				{
					for (uint32 seek_celement = 0; seek_celement < Clauses[seek_candidate].size(); seek_celement = seek_celement + 1)
					{
						if (SrcClauses[seek][seek_element].strValue == Clauses[seek_candidate][seek_celement].strValue)
						{
							if (SrcClauses[seek][seek_element].bNegative != Clauses[seek_candidate][seek_celement].bNegative)
							{
								/*Add to candidate list*/
								if (this->_FindClause(Clauses[seek_candidate], CandidateClausesList) == -1)
								{
									CandidateClausesList.push_back(Clauses[seek_candidate]);
								}
							}
						}
					}
				}
			}
		}

		/*Generate new clauses from candidate list*/
		vector<LOGICCLAUSES> NewClauses;

		for (uint32 seek = 0; seek < SrcClauses.size(); seek = seek + 1)
		{
			for (uint32 seek_candidate = 0; seek_candidate < CandidateClausesList.size(); seek_candidate = seek_candidate + 1)
			{
				if (this->_GenerateNewClausesList(SrcClauses[seek], CandidateClausesList[seek_candidate], Clauses, NewClauses))
				{
					return true;
				}
			}
		}

		/*Delete visited node*/
		vector<LOGICCLAUSES>::iterator pItem = NewClauses.begin();
		while (pItem != NewClauses.end())
		{
			if (this->_FindClause(*pItem, VisitedClauses) != -1)
			{
				pItem = NewClauses.erase(pItem);
				continue;
			}
			pItem++;
		}

		if (NewClauses.empty()){ return false; }

		/*Add to history*/
		for (uint32 seek_visited = 0; seek_visited < NewClauses.size(); seek_visited = seek_visited + 1)
		{
			VisitedClauses.push_back(NewClauses[seek_visited]);
		}

		SrcClauses = NewClauses;

		/*Add new clauses to clauses list*/
		for (uint32 seek = 0; seek < NewClauses.size(); seek = seek + 1)
		{
			this->_InsertClause(NewClauses[seek], Clauses);
		}

		NewClauses.clear();
	}

	return false;
}
bool cPropositionalLogicResolutionAlgorithm::_GenerateNewClausesList(const LOGICCLAUSES &SrcClauses, const LOGICCLAUSES &DestClauses, const vector<LOGICCLAUSES> &ClausesList, vector<LOGICCLAUSES> &NewClausesList)
{
	const uint32 SrcSize = SrcClauses.size();
	const uint32 DestSize = DestClauses.size();

	for (uint32 seek_element = 0; seek_element < SrcSize; seek_element = seek_element + 1)
	{
		/*Src: (a | b), Dest: (!a | b), find a and !a*/
		for (uint32 seek_inv = 0; seek_inv < DestSize; seek_inv = seek_inv + 1)
		{
			if (SrcClauses[seek_element].strValue == DestClauses[seek_inv].strValue)
			{
				if (SrcClauses[seek_element].bNegative != DestClauses[seek_inv].bNegative)
				{
					/*Generate new clause*/
					LOGICCLAUSES CurrentClause;

					for (uint32 seek_item = 0; seek_item < SrcSize; seek_item = seek_item + 1)
					{
						if (seek_item != seek_element)
						{
							CurrentClause.push_back(SrcClauses[seek_item]);
						}
					}

					for (uint32 seek_item = 0; seek_item < DestSize; seek_item = seek_item + 1)
					{
						if (seek_item != seek_inv)
						{
							bool bFound = false;

							for (uint32 seek_subitem = 0; seek_subitem < CurrentClause.size(); seek_subitem = seek_subitem + 1)
							{
								if (CurrentClause[seek_subitem] == DestClauses[seek_item])
								{
									bFound = true;
								}
							}

							if (!bFound)
							{
								CurrentClause.push_back(DestClauses[seek_item]);
							}
						}
					}

					if (CurrentClause.empty()){ return true; }

					/*Check if already generated*/
					bool bFound = false;

					for (uint32 seek_item = 0; seek_item < ClausesList.size(); seek_item = seek_item + 1)
					{
						if (ClausesList[seek_item] == CurrentClause)
						{
							bFound = true;
							break;
						}
					}

					if (bFound){ continue; }

					for (uint32 seek_item = 0; seek_item < NewClausesList.size(); seek_item = seek_item + 1)
					{
						if (NewClausesList[seek_item] == CurrentClause)
						{
							bFound = true;
							break;
						}
					}

					if (!bFound){ NewClausesList.push_back(CurrentClause); }

				}
			}
		}
	}
	return false;
}
void cPropositionalLogicResolutionAlgorithm::_InsertClause(const LOGICCLAUSES &Clause, vector<LOGICCLAUSES> &ClausesList)
{
	vector<LOGICCLAUSES>::iterator pItem = ClausesList.begin();
	while (pItem != ClausesList.end())
	{
		if ((*pItem) > Clause){ break; }
		pItem++;
	}
	ClausesList.insert(pItem, Clause);

}
int32 cPropositionalLogicResolutionAlgorithm::_FindClause(const LOGICCLAUSES &Clause, const vector<LOGICCLAUSES> &ClausesList)
{
	uint32 Size = ClausesList.size();
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		if (Clause == ClausesList[seek])
		{
			return seek;
		}
	}
	return -1;
}
SString cPropositionalLogicResolutionAlgorithm::_PrintClausesList(const vector<LOGICCLAUSES> &ClausesList)
{
	SString strOutput;
	for (uint32 seek = 0; seek < ClausesList.size(); seek = seek + 1)
	{
		for (uint32 seek_item = 0; seek_item < ClausesList[seek].size(); seek_item = seek_item + 1)
		{
			strOutput = strOutput + _SSTR(" ");
			if (ClausesList[seek][seek_item].bNegative)
			{
				strOutput = strOutput + _SSTR("!");
			}

			strOutput = strOutput + ClausesList[seek][seek_item].strValue + _SSTR(" ");
		}
		strOutput = strOutput + _SSTR("\r\n");
	}

	return strOutput;
}
void cPropositionalLogicResolutionAlgorithm::_AddElementToTable(const RULEELEMENT &Clause, vector<pair<SString, SVECTOR3>> &ElementTable)
{
	uint32 Size = ElementTable.size();
	for (uint32 seek = 0; seek < Size; seek = seek + 1)
	{
		if (ElementTable[seek].first == Clause.strValue)
		{
			ElementTable[seek].second = ElementTable[seek].second + (!Clause.bNegative ? SVECTOR3(1.0, 0.0, 0.0) : SVECTOR3(0.0, 1.0, 0.0));
			return;
		}
	}
	ElementTable.push_back(::make_pair(Clause.strValue, Clause.bNegative ? SVECTOR3(1.0, 0.0, 0.0) : SVECTOR3(0.0, 1.0, 0.0)));
}
void cPropositionalLogicResolutionAlgorithm::_DeleteElementFromTable(const RULEELEMENT &Clause, vector<pair<SString, SVECTOR3>> &ElementTable)
{
	vector<pair<SString, SVECTOR3>>::iterator pItem = ElementTable.begin();
	while (pItem != ElementTable.end())
	{
		if (pItem->first == Clause.strValue)
		{
			if (!Clause.bNegative)
			{
				if (pItem->second.x != 1)
				{
					pItem->second.x = pItem->second.x - 1;
				}
				else
				{
					this->m_ElementTable.erase(pItem);
				}
			}
			else
			{
				if (pItem->second.y != 1)
				{
					pItem->second.y = pItem->second.y - 1;
				}
				else
				{
					this->m_ElementTable.erase(pItem);
				}
			}
			return;
		}
		pItem++;
	}
}
int32 cPropositionalLogicResolutionAlgorithm::_GetElementIndex(const RULEELEMENT &Clause, vector<pair<SString, SVECTOR3>> &ElementTable)
{
	uint32 Size = ElementTable.size();
	for (uint32 seek = 0; seek < Size;seek=seek+1)
	{
		if (ElementTable[seek].first == Clause.strValue)
		{
			int32 Count = !Clause.bNegative ? ElementTable[seek].second.x : ElementTable[seek].second.y;
			return Count >= 1 ? seek : -1;
		}
	}
	return -1;
}
bool cPropositionalLogicResolutionAlgorithm::iGenerateKnowedgeBase(const vector<SString> &strRuleStringList)
{
	for (uint32 seek = 0; seek < strRuleStringList.size(); seek = seek + 1)
	{
		PROPOSITIONALLOGICRULE RuleTree;

		RuleTree.iGenerateTree(strRuleStringList[seek]);
		RuleTree.iConvertToCNF();
		//::cout << RuleTree.iPrintTree().c_str() << "\n";
		RuleTree.iGenerateClausesList();

		this->iAddClauses(RuleTree.iGetCNFClausesList());
	}
	return true;
}
void cPropositionalLogicResolutionAlgorithm::iAddClauses(const vector<LOGICCLAUSES> &ClausesList)
{
	for (uint32 seek = 0; seek < ClausesList.size(); seek = seek + 1)
	{
		this->iAddClause(ClausesList[seek]);
	}
}
bool cPropositionalLogicResolutionAlgorithm::iAddClause(const LOGICCLAUSES &Clause)
{
	/*Check collision*/
	for (uint32 seek = 0; seek < this->m_KnowledgeBase.size(); seek = seek + 1)
	{
		if (this->m_KnowledgeBase[seek].size() == Clause.size())
		{
			bool bEqual = true;
			for (uint32 seek_clause = 0; seek_clause < this->m_KnowledgeBase[seek].size(); seek_clause = seek_clause + 1)
			{
				if (this->m_KnowledgeBase[seek][seek_clause] != Clause[seek_clause])
				{
					bEqual = false;
					break;
				}
			}
			if (bEqual)
			{
				return false;
			}
		}
	}

	/*Add each element to knowledge base*/
	for (uint32 seek = 0; seek < Clause.size(); seek = seek + 1)
	{
		this->_AddElementToTable(Clause[seek], this->m_ElementTable);
	}

	this->_InsertClause(Clause, this->m_KnowledgeBase);
	return true;
}
void cPropositionalLogicResolutionAlgorithm::iDeleteClauses(const vector<LOGICCLAUSES> &ClausesList)
{
	for (uint32 seek = 0; seek < ClausesList.size(); seek = seek + 1)
	{
		this->iDeleteClause(ClausesList[seek]);
	}
}
bool cPropositionalLogicResolutionAlgorithm::iDeleteClause(const LOGICCLAUSES &Clause)
{
	vector<LOGICCLAUSES>::iterator pClause = this->m_KnowledgeBase.begin();
	for (uint32 seek = 0; seek < this->m_KnowledgeBase.size(); seek = seek + 1)
	{
		if (this->m_KnowledgeBase[seek].size() == Clause.size())
		{
			bool bEqual = true;
			for (uint32 seek_clause = 0; seek_clause < this->m_KnowledgeBase[seek].size(); seek = seek + 1)
			{
				if (this->m_KnowledgeBase[seek][seek_clause] != Clause[seek_clause])
				{
					bEqual = false;
					break;
				}
			}
			if (bEqual)
			{
				/*Delete each element to knowledge base*/
				for (uint32 seek = 0; seek < Clause.size(); seek = seek + 1)
				{
					this->_DeleteElementFromTable(Clause[seek], this->m_ElementTable);
				}

				this->m_KnowledgeBase.erase(pClause);
				return true;
			}
		}
		pClause++;
	}
	return false;
}
void cPropositionalLogicResolutionAlgorithm::iClearClauses()
{
	this->m_KnowledgeBase.clear();
	this->m_ElementTable.clear();
}
eLOGICRESULT cPropositionalLogicResolutionAlgorithm::iResolve(const SString &strTargetString)
{
	PROPOSITIONALLOGICRULE RuleTree;

	RuleTree.iGenerateTree(strTargetString);
	RuleTree.iConvertToCNF();

	/*Resolve for true*/
	RuleTree.iInverse();
	RuleTree.iGenerateClausesList();

	if (this->_Resolve(RuleTree.iGetCNFClausesList()))
	{
		return LR_TRUE;
	}

	/*Resolve for false*/
	RuleTree.iInverse();
	RuleTree.iGenerateClausesList();

	if (this->_Resolve(RuleTree.iGetCNFClausesList()))
	{
		return LR_FALSE;
	}

	return LR_UNKNOWN;
}
eLOGICRESULT cPropositionalLogicResolutionAlgorithm::iResolve(const vector<LOGICCLAUSES> &TargetClauses)
{
	SString strClauses;

	for (uint32 seek = 0; seek < TargetClauses.size(); seek = seek + 1)
	{
		SString strCurrentClauses = seek == 0 ? _SSTR("(") : _SSTR("&(");
		for (uint32 seek_element = 0; seek_element < TargetClauses[seek].size(); seek_element = seek_element + 1)
		{
			if (seek_element != 0)
			{
				strCurrentClauses = strCurrentClauses + _SSTR("|");
			}
			if (TargetClauses[seek][seek_element].bNegative)
			{
				strCurrentClauses = strCurrentClauses + _SSTR("!");
			}
			strCurrentClauses = strCurrentClauses + TargetClauses[seek][seek_element].strValue;
		}
		strCurrentClauses = strCurrentClauses + _SSTR(")");
		strClauses = strClauses+ strCurrentClauses;
	}
	return this->iResolve(strClauses);
}
eLOGICRESULT cPropositionalLogicResolutionAlgorithm::iResolveOptimize(const SString &strTargetString)
{
	PROPOSITIONALLOGICRULE RuleTree;

	RuleTree.iGenerateTree(strTargetString);
	RuleTree.iConvertToCNF();

	/*Resolve for true*/
	RuleTree.iInverse();
	RuleTree.iGenerateClausesList();

	if (this->_ResolveOptimize(RuleTree.iGetCNFClausesList()))
	{
		return LR_TRUE;
	}

	/*Resolve for false*/
	RuleTree.iInverse();
	RuleTree.iGenerateClausesList();

	if (this->_ResolveOptimize(RuleTree.iGetCNFClausesList()))
	{
		return LR_FALSE;
	}

	return LR_UNKNOWN;
}
eLOGICRESULT cPropositionalLogicResolutionAlgorithm::iResolveOptimize(const vector<LOGICCLAUSES> &TargetClauses)
{
	SString strClauses;

	for (uint32 seek = 0; seek < TargetClauses.size(); seek = seek + 1)
	{
		SString strCurrentClauses = seek == 0 ? _SSTR("(") : _SSTR("&(");
		for (uint32 seek_element = 0; seek_element < TargetClauses[seek].size(); seek_element = seek_element + 1)
		{
			if (seek_element != 0)
			{
				strCurrentClauses = strCurrentClauses + _SSTR("|");
			}
			if (TargetClauses[seek][seek_element].bNegative)
			{
				strCurrentClauses = strCurrentClauses + _SSTR("!");
			}
			strCurrentClauses = strCurrentClauses + TargetClauses[seek][seek_element].strValue;
		}
		strCurrentClauses = strCurrentClauses + _SSTR(")");
		strClauses = strClauses + strCurrentClauses;
	}
	return this->iResolveOptimize(strClauses);
}
SString cPropositionalLogicResolutionAlgorithm::iPrintKnowledgeBase() const
{
	SString strOutput;
	for (uint32 seek = 0; seek < this->m_KnowledgeBase.size(); seek = seek + 1)
	{
		for (uint32 seek_clause = 0; seek_clause < this->m_KnowledgeBase[seek].size(); seek_clause = seek_clause + 1)
		{
			if (this->m_KnowledgeBase[seek][seek_clause].bNegative)
			{
				strOutput = strOutput + _SSTR("!");
			}
			strOutput = strOutput + this->m_KnowledgeBase[seek][seek_clause].strValue + _SSTR(" ");
		}
		strOutput = strOutput + _SSTR("\r\n");
	}
	return strOutput;
}