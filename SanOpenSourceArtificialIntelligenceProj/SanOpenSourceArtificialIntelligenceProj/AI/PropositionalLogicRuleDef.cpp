#include"PropositionalLogicRuleDef.h"
using namespace std;
using namespace San;
using namespace San::AI;
RULEELEMENT::RULEELEMENT()
	:Type(RET_OPERATOR_NULL), strValue(""), bNegative(false)
{
}
RULEELEMENT::RULEELEMENT(const RULEELEMENT &Element)
	:Type(Element.Type), strValue(Element.strValue), bNegative(Element.bNegative)
{
}
RULEELEMENT::~RULEELEMENT()
{
}
bool RULEELEMENT::operator==(const RULEELEMENT& Element) const
{
	if (Type != Element.Type)
	{
		return false;
	}
	if ((Type == RET_CONSTANT) || (Type == RET_VARIABLE))
	{
		if (bNegative != Element.bNegative)
		{
			return false;
		}
		if (strValue != Element.strValue)
		{
			return false;
		}
	}
	return true;
}
bool RULEELEMENT::operator!=(const RULEELEMENT& Element) const
{
	if (Type != Element.Type)
	{
		return true;
	}
	if ((Type == RET_CONSTANT) || (Type == RET_VARIABLE))
	{
		if (bNegative != Element.bNegative)
		{
			return true;
		}
		if (strValue != Element.strValue)
		{
			return true;
		}
	}
	return false;
}
SString RULEELEMENT::iPrintNode() const
{
	SString strOutput;
	switch (this->Type)
	{
	case RET_OPERATOR_NULL:
		strOutput = strOutput + _SSTR("(_ NULL)");
		break;
	case RET_OPERATOR_AND:
		strOutput = strOutput + _SSTR("(& AND)");
		break;
	case RET_OPERATOR_OR:
		strOutput = strOutput + _SSTR("(| OR)");
		break;
	case RET_OPERATOR_NOT:
		strOutput = strOutput + _SSTR("(! NOT)");
		break;
	case RET_OPERATOR_IMPLY:
		strOutput = strOutput + _SSTR("(=> IMPLY)");
		break;
	case RET_OPERATOR_BICOND:
		strOutput = strOutput + _SSTR("(<=> BICOND)");
		break;
	case RET_CONSTANT:
	case RET_VARIABLE:
		strOutput = strOutput + _SSTR("(");
		if (this->bNegative)
		{
			strOutput = strOutput + _SSTR("!");
		}
		strOutput = strOutput + this->strValue + _SSTR(" VAR)");
		break;
	default:
		break;
	}
	return strOutput;
}
bool San::AI::operator==(const LOGICCLAUSES &Clauses1, const LOGICCLAUSES &Clauses2)
{
	if (Clauses1.size() != Clauses2.size())
	{
		return false;
	}

	uint32 Size = Clauses1.size();
	for (uint32 seek_src = 0; seek_src < Size; seek_src = seek_src + 1)
	{
		bool bFound = false;

		for (uint32 seek_dest = 0; seek_dest < Size; seek_dest = seek_dest + 1)
		{
			if (Clauses1[seek_src] == Clauses2[seek_dest])
			{
				bFound = true;
				continue;
			}
		}

		if (bFound == false)
		{ 
			return false; 
		}
	}
	return true;
}
bool San::AI::operator<(const LOGICCLAUSES &LClauses, const LOGICCLAUSES &RClauses)
{
	return LClauses.size() > RClauses.size();
}
bool LogicClausesCompare::operator()(const LOGICCLAUSES &LClauses, const LOGICCLAUSES &RClauses) const
{
	return LClauses.size() > RClauses.size();
}
PROPOSITIONALLOGICRULE::PROPOSITIONALLOGICRULE(const SString &strRule)
	:strLogicString(strRule)
{
	this->iGenerateTree(this->strLogicString);
}
PROPOSITIONALLOGICRULE::PROPOSITIONALLOGICRULE(const PROPOSITIONALLOGICRULE &LogicRule)
	:strLogicString(LogicRule.strLogicString)
{
	this->iGenerateTree(this->strLogicString);
}
PROPOSITIONALLOGICRULE::~PROPOSITIONALLOGICRULE()
{
	this->Tree.iClear();
}
/* INPUT STRING: !( a & b ) => ( c & ( d | !e ) )	*/
/* OUTPUT TREE (NOT CNF TREE):						*/
/*						=>							*/
/*					   /  \							*/
/*					  !    &						*/
/*					 /    / \						*/
/*				    &    c   |						*/
/*				   / \      / \						*/
/*				  a   b    d   !					*/
/*							  /						*/
/*							 e						*/
bool PROPOSITIONALLOGICRULE::_GenerateTree(TREE<RULEELEMENT>* pTreeNode, SString strString)
{
	if (pTreeNode == nullptr)
	{
		return false;
	}

	if (strString.empty())
	{
		return false;
	}

	uint32 Start = 0;
	int32 Count = 0;
	bool bElement = true;

	vector<SString> strItems;
	for (uint32 seek = 0; seek < strString.size(); seek = seek + 1)
	{
		switch (strString[seek])
		{
		case _SSTR('('):
			if (Count == 0)
			{
				SString strSubString = strString.substr(Start, seek - Start);
				if (!strSubString.empty())
				{
					strItems.push_back(strSubString);
				}
				Start = seek;
			}
			Count = Count + 1;
			break;
		case _SSTR(')'):
			Count = Count - 1;
			if (Count < 0)
			{
				return false;
			}
			if (Count == 0)
			{
				SString strSubString = strString.substr(Start, seek - Start + 1);
				if (!strSubString.empty())
				{
					strItems.push_back(strSubString);
				}
				Start = seek + 1;
			}
			break;
		case _SSTR('&'):case _SSTR('|'):case _SSTR('!'):case _SSTR('='):case _SSTR('<'):
			if (Count == 0)
			{
				SString strSubString = strString.substr(Start, seek - Start);
				if (!strSubString.empty())
				{
					strItems.push_back(strSubString);
				}

				if (strString[seek] == _SSTR('&'))
				{
					strSubString = _SSTR("&");
					strItems.push_back(strSubString);
					Start = seek + 1;
				}

				if (strString[seek] == _SSTR('|'))
				{
					strSubString = _SSTR("|");
					strItems.push_back(strSubString);
					Start = seek + 1;
				}

				if (strString[seek] == _SSTR('!'))
				{
					strSubString = _SSTR("!");
					strItems.push_back(strSubString);
					Start = seek + 1;
				}

				if (strString[seek] == _SSTR('='))
				{
					if ((seek + 1) >= strString.size())
					{
						return false;
					}
					strSubString = _SSTR("=>");
					strItems.push_back(strSubString);
					Start = seek + 2;
					seek = seek + 1;
				}

				if (strString[seek] == _SSTR('<'))
				{
					if ((seek + 2) >= strString.size())
					{
						return false;
					}
					strSubString = _SSTR("<=>");
					strItems.push_back(strSubString);
					Start = seek + 3;
					seek = seek + 2;
				}
			}
			break;
		default:
			break;
		}
	}

	SString strSubString = strString.substr(Start, (strString.size() - Start)< 0 ? 0 : (strString.size() - Start));
	if ((!strSubString.empty()) && (strSubString[0] != '\0'))
	{
		strItems.push_back(strSubString);
	}

	vector<SString>::iterator pSubItem = strItems.end();
	pSubItem--;
	while (pSubItem != strItems.begin())
	{
		if ((*pSubItem) == _SSTR("!"))
		{
			pSubItem = strItems.erase(pSubItem);
			if (pSubItem == strItems.end())
			{
				return false;
			}
			(*pSubItem) = _SSTR("!") + (*pSubItem);
		}
		pSubItem--;
	}
	if ((*pSubItem) == _SSTR("!"))
	{
		pSubItem = strItems.erase(pSubItem);
		if (pSubItem == strItems.end())
		{
			return false;
		}
		(*pSubItem) = _SSTR("!") + (*pSubItem);
	}

	eRULEELEMENTTYPE Type = RET_OPERATOR_NULL;
	for (uint32 seek = 0; seek < strItems.size(); seek = seek + 1)
	{
		if (strItems[seek] == _SSTR("&"))
		{
			if ((Type != RET_OPERATOR_NULL) && (Type != RET_OPERATOR_AND))
			{
				return false;
			}
			Type = RET_OPERATOR_AND;
			continue;
		}
		if (strItems[seek] == _SSTR("|"))
		{
			if ((Type != RET_OPERATOR_NULL) && (Type != RET_OPERATOR_OR))
			{
				return false;
			}
			Type = RET_OPERATOR_OR;
			continue;
		}
		if (strItems[seek] == _SSTR("=>"))
		{
			if ((Type != RET_OPERATOR_NULL) && (Type != RET_OPERATOR_IMPLY))
			{
				return false;
			}
			Type = RET_OPERATOR_IMPLY;
			continue;
		}
		if (strItems[seek] == _SSTR("<=>"))
		{
			if ((Type != RET_OPERATOR_NULL) && (Type != RET_OPERATOR_BICOND))
			{
				return false;
			}
			Type = RET_OPERATOR_BICOND;
			continue;
		}
		if (!this->_CreateTreeNode(pTreeNode, strItems[seek]))
		{
			return false;
		}
	}

	if (Type == RET_OPERATOR_NULL)
	{
		return false;
	}
	RULEELEMENT Element = pTreeNode->iGetData();
	Element.Type = Type;
	pTreeNode->iSetData(Element);

	return true;
}
bool PROPOSITIONALLOGICRULE::_CreateTreeNode(TREE<RULEELEMENT>* pTreeNode, SString strString)
{
	if (pTreeNode == nullptr)
	{
		return false;
	}
	if (strString.empty())
	{
		return true;
	}

	uint32 Count = 0;
	for (uint32 seek = 0; seek < strString.size(); seek = seek + 1)
	{
		if (strString[seek] != '!')
		{
			break;
		}
		Count = Count + 1;
	}

	SString strSubString = strString.substr(Count, (strString.size() - Count) < 0 ? 0 : (strString.size() - Count));

	if (strSubString.empty())
	{
		return false;
	}

	/*!(A n B)*/
	TREE<RULEELEMENT>* pNode;
	if ((Count % 2) != 0)
	{
		RULEELEMENT Element;
		Element.Type = RET_OPERATOR_NOT;
		pNode = pTreeNode->iCreate(Element, 0.0);
	}
	else
	{
		pNode = pTreeNode;
	}

	/* (((a))) to a */
	uint32 LeftCount = 0;
	uint32 RightCount = 0;

	for (uint32 seek = 0; seek < strSubString.size(); seek = seek + 1)
	{
		if (strSubString[seek] != '(')
		{
			break;
		}
		LeftCount = LeftCount + 1;
	}
	for (uint32 seek = strSubString.size() - 1; seek >= 0; seek = seek - 1)
	{
		if (strSubString[seek] != ')')
		{
			break;
		}
		RightCount = RightCount + 1;
	}

	Count = LeftCount < RightCount ? LeftCount : RightCount;
	strSubString = strSubString.substr(Count, strSubString.size() - Count - Count);

	if (this->_IsElement(strSubString))
	{
		RULEELEMENT Element;
		Element.Type = RET_CONSTANT;
		Element.bNegative = false;

		Count = 0;
		for (uint32 seek = 0; seek < strSubString.size(); seek = seek + 1)
		{
			if (strSubString[seek] != '!')
			{
				break;
			}
			Count = Count + 1;
		}

		if ((Count % 2) == 1)
		{
			Element.bNegative = true;
		}

		Element.strValue = strSubString.substr(Count, (strSubString.size() - Count) < 0 ? 0 : (strSubString.size() - Count));

		if (Element.strValue.empty())
		{
			return false;
		}

		pNode->iCreate(Element, 0.0);

		return true;
	}

	return this->_GenerateTree(pNode->iCreate(RULEELEMENT(), 0.0), strSubString);
}
/*OUTPUT TREE (CNF TREE): see CNF.jpg*/
void PROPOSITIONALLOGICRULE::_ConvertToCNF(TREE<RULEELEMENT>* pTreeNode, const uint32 State)
{
	if (pTreeNode == nullptr)
	{
		return;
	}

	RULEELEMENT Element = pTreeNode->iGetData();
	switch (Element.Type)
	{
	case RET_OPERATOR_NULL:
	{
		if (pTreeNode->iGetSize() > 1)
		{
			::cout << "Error: Invalid tree format";
			return;
		}
		for (uint32 seek = 0; seek < (*pTreeNode)[0].iGetSize(); seek = seek + 1)
		{
			pTreeNode->iInsert(1 + seek, (*pTreeNode)[0][seek], 0.0);//, pTreeNode[0].iGetOffshootWeight(seek));
		}
		pTreeNode->iSetData((*pTreeNode)[0].iGetData());
		pTreeNode->iSetGlobalWeight((*pTreeNode)[0].iGetGlobalWeight());
		pTreeNode->iDelete(0);
		this->_ConvertToCNF(pTreeNode, State);
		break;
	}
	case RET_OPERATOR_AND:
		if ((State == 0) || (State == 1))
		{
			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				this->_ConvertToCNF(pTreeNode->iGetOffshootPtr(seek), State);
			}
		}
		break;
	case RET_OPERATOR_OR:
		if (State == 0)
		{
			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				this->_ConvertToCNF(pTreeNode->iGetOffshootPtr(seek), State);
			}
		}
		if (State == 1)
		{
			// (A & B & C) | D = (A | D) & (B | D) & (C | D)
			// (A & B) | C | D = ( (A | C) & (B | C) ) | D = (A | C | D) & (B | C | C) 
			int32 Index = -1;
			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				if ((*pTreeNode)[seek].iGetData().Type == RET_OPERATOR_AND)
				{
					Index = seek;
					break;
				}
			}

			if (Index == -1)
			{
				return;
			}

			uint32 TargetIndex = Index == 0 ? 1 : 0;

			switch (pTreeNode->iGetSize())
			{
			case 0:
			case 1:
				::cout << "Error: Invalid tree format";
				return;
				break;
			case 2:
			{
				RULEELEMENT Element = pTreeNode->iGetData();

				uint32 Size = (*pTreeNode)[Index].iGetSize();

				for (uint32 seek = 0; seek < (*pTreeNode)[Index].iGetSize(); seek = seek + 1)
				{
					TREE<RULEELEMENT>* pNode = pTreeNode->iCreate(Element, 0.0);

					pNode->iInsert(0, (*pTreeNode)[Index][seek], 0.0);
					pNode->iInsert(1, (*pTreeNode)[TargetIndex], 0.0);
				}

				Element.Type = RET_OPERATOR_AND;
				pTreeNode->iSetData(Element);

				pTreeNode->iDelete(0);
				pTreeNode->iDelete(0);

				for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
				{
					this->_ConvertToCNF(pTreeNode->iGetOffshootPtr(seek), State);
				}

				break;
			}
			default:
			{
				RULEELEMENT Element = pTreeNode->iGetData();

				TREE<RULEELEMENT>* pNode = pTreeNode->iCreate(Element, 0.0);

				pNode->iInsert(0, (*pTreeNode)[Index], 0.0);
				pNode->iInsert(1, (*pTreeNode)[TargetIndex], 0.0);

				pTreeNode->iDelete(Index > TargetIndex ? Index : TargetIndex);
				pTreeNode->iDelete(Index < TargetIndex ? Index : TargetIndex);

				this->_ConvertToCNF(pNode, State);

				break;
			}
			}
		}
		break;
	case RET_OPERATOR_NOT:
		if (State == 0)
		{
			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				this->_ConvertToCNF(pTreeNode->iGetOffshootPtr(seek), State);
			}
		}
		if (State == 1)
		{
			this->_Inverse(pTreeNode);
		}
		break;
	case RET_OPERATOR_IMPLY:
		if (State == 0)
		{
			if (pTreeNode->iGetSize() != 2)
			{
				::cout << "Error: Invalid tree format\n";
				return;
			}

			Element.Type = RET_OPERATOR_OR;

			pTreeNode->iSetData(Element);

			this->_Inverse(pTreeNode->iGetOffshootPtr(0));

			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				this->_ConvertToCNF(pTreeNode->iGetOffshootPtr(seek), State);
			}
		}
		break;
	case RET_OPERATOR_BICOND:
		if (State == 0)
		{
			if (pTreeNode->iGetSize() != 2)
			{
				::cout << "Error: Invalid tree format\n";
				return;
			}
			Element.Type = RET_OPERATOR_AND;
			pTreeNode->iSetData(Element);

			Element.Type = RET_OPERATOR_IMPLY;

			TREE<RULEELEMENT>* pLeft = pTreeNode->iCreate(Element, 0.0);
			TREE<RULEELEMENT>* pRight = pTreeNode->iCreate(Element, 0.0);

			pLeft->iInsert(0, (*pTreeNode)[0], 0.0);
			pLeft->iInsert(1, (*pTreeNode)[1], 0.0);

			pRight->iInsert(0, (*pTreeNode)[1], 0.0);
			pRight->iInsert(1, (*pTreeNode)[0], 0.0);

			pTreeNode->iDelete(0);
			pTreeNode->iDelete(0);

			this->_ConvertToCNF(pLeft, State);
			this->_ConvertToCNF(pRight, State);
		}
		break;
	case RET_CONSTANT:
	case RET_VARIABLE:
		break;
	default:
		break;
	}
}
int32 PROPOSITIONALLOGICRULE::_PruningTree(TREE<RULEELEMENT>* pTreeNode)
{
	if (pTreeNode == nullptr)
	{
		return 0;
	}
	//::cout << pTreeNode->iGetData().iPrintNode().c_str() << "\n";
	RULEELEMENT Element = pTreeNode->iGetData();
	switch (Element.Type)
	{
	case RET_OPERATOR_NULL:
		if (pTreeNode->iGetSize() != 1)
		{
			::cout << "Error: Invalid tree format\n";
			return 0;
		}
		if (pTreeNode->iIsLeaf())
		{
			::cout << "Error: Invalid tree format\n";
			if (pTreeNode->iIsRoot())
			{
				return 0;
			}
			TREE<RULEELEMENT>* pTrunk = pTreeNode->iGetTrunkPtr();
			pTrunk->iDelete(pTreeNode->iGetID());
			return -1;
		}
		else
		{
			for (uint32 seek = 0; seek < (*pTreeNode)[0].iGetSize(); seek = seek + 1)
			{
				pTreeNode->iInsert(1 + seek, (*pTreeNode)[0][seek], 0.0);
			}

			pTreeNode->iSetData((*pTreeNode)[0].iGetData());
			pTreeNode->iSetGlobalWeight((*pTreeNode)[0].iGetGlobalWeight());

			pTreeNode->iDelete(0);

			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				//::cout << seek << " " << pTreeNode->iGetSize() << "\n";
				seek = seek + this->_PruningTree(pTreeNode->iGetOffshootPtr(seek));
			}

			return 0;
		}
		break;
	case RET_OPERATOR_NOT:
		if (pTreeNode->iGetSize() != 1)
		{
			::cout << "Error: Invalid tree format\n";
			return 0;
		}
		if (pTreeNode->iIsLeaf())
		{
			::cout << "Error: Invalid tree format\n";
			if (pTreeNode->iIsRoot())
			{
				Element.Type = RET_OPERATOR_NULL;
				pTreeNode->iSetData(Element);
				return 0;
			}
			TREE<RULEELEMENT>* pTrunk = pTreeNode->iGetTrunkPtr();
			pTrunk->iDelete(pTreeNode->iGetID());
			return -1;
		}
		else
		{
			this->_Inverse(pTreeNode->iGetOffshootPtr(0));

			for (uint32 seek = 0; seek < (*pTreeNode)[0].iGetSize(); seek = seek + 1)
			{
				pTreeNode->iInsert(1 + seek, (*pTreeNode)[0][seek], 0.0);
			}

			pTreeNode->iSetData((*pTreeNode)[0].iGetData());
			pTreeNode->iSetGlobalWeight((*pTreeNode)[0].iGetGlobalWeight());

			pTreeNode->iDelete(0);

			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				//::cout << seek << " " << pTreeNode->iGetSize() << "\n";
				seek = seek + this->_PruningTree(pTreeNode->iGetOffshootPtr(seek));
			}

			return 0;
		}
		break;
	case RET_OPERATOR_AND:
	case RET_OPERATOR_OR:
	case RET_OPERATOR_IMPLY:
	case RET_OPERATOR_BICOND:
		if (pTreeNode->iIsLeaf())
		{
			if (pTreeNode->iIsRoot())
			{
				Element.Type = RET_OPERATOR_NULL;
				pTreeNode->iSetData(Element);
				return 0;
			}
			TREE<RULEELEMENT>* pTrunk = pTreeNode->iGetTrunkPtr();
			pTrunk->iDelete(pTreeNode->iGetID());
			return -1;
		}
		else
		{
			for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
			{
				//::cout << seek << " " << pTreeNode->iGetSize() << "\n";
				seek = seek + this->_PruningTree(pTreeNode->iGetOffshootPtr(seek));
			}
			return 0;
		}
		break;
	case RET_CONSTANT:
	case RET_VARIABLE:
		return 0;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
void PROPOSITIONALLOGICRULE::_OptimizeTree(TREE<RULEELEMENT>* pTreeNode)
{
	if (pTreeNode == nullptr)
	{
		return;
	}

	for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
	{
		this->_OptimizeTree(pTreeNode->iGetOffshootPtr(seek));
	}

	switch (pTreeNode->iGetData().Type)
	{
	case RET_OPERATOR_NULL:
		this->_OptimizeTree(pTreeNode);
		break;
	case RET_OPERATOR_AND:
	case RET_OPERATOR_OR:
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			if ((*pTreeNode)[seek].iGetData().Type == pTreeNode->iGetData().Type)
			{
				for (uint32 seek_subnode = 0; seek_subnode < (*pTreeNode)[seek].iGetSize(); seek_subnode = seek_subnode + 1)
				{
					pTreeNode->iInsert(pTreeNode->iGetSize(), (*pTreeNode)[seek][seek_subnode], 0.0);
				}
				pTreeNode->iDelete(seek);
				seek = seek - 1;
			}
		}
		break;
	case RET_OPERATOR_NOT:
		this->_OptimizeTree(pTreeNode);
		break;
	case RET_OPERATOR_IMPLY:
	case RET_OPERATOR_BICOND:
	case RET_CONSTANT:
	case RET_VARIABLE:
		break;
	default:
		break;
	}
}
void PROPOSITIONALLOGICRULE::_GenerateClausesList(TREE<RULEELEMENT>* pTreeNode)
{
	if (pTreeNode == nullptr)
	{
		return;
	}

	switch (pTreeNode->iGetData().Type)
	{
	case RET_OPERATOR_AND:
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			this->_GenerateClausesList(pTreeNode->iGetOffshootPtr(seek));
		}
		break;
	case RET_OPERATOR_OR:
	{
		LOGICCLAUSES Clauses;
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			RULEELEMENT Element = (*pTreeNode)[seek].iGetData();
			Element.strValue = Element.strValue.c_str();
			Clauses.push_back(Element);
		}
		this->ClausesList.push_back(Clauses);
		break;
	}
	case RET_CONSTANT:
	case RET_VARIABLE:
	{
		LOGICCLAUSES Clauses;

		RULEELEMENT Element = pTreeNode->iGetData();
		Element.strValue = Element.strValue.c_str();

		Clauses.push_back(Element);
		this->ClausesList.push_back(Clauses);
		break;
	}
	default:
		break;
	}
}
void PROPOSITIONALLOGICRULE::_GenerateLogicString(TREE<RULEELEMENT>* pTreeNode)
{
	if (pTreeNode == nullptr)
	{
		return;
	}

	SString strOperator;
	switch (pTreeNode->iGetData().Type)
	{
	case RET_OPERATOR_NULL:
		break;
	case RET_OPERATOR_AND:
		strOperator = _SSTR("&");
		break;
	case RET_OPERATOR_OR:
		strOperator = _SSTR("|");
		break;
	case RET_OPERATOR_NOT:
		strOperator = _SSTR("!");
		break;
	case RET_OPERATOR_IMPLY:
		strOperator = _SSTR("=>");
		break;
	case RET_OPERATOR_BICOND:
		strOperator = _SSTR("<=>");
		break;
	case RET_CONSTANT:
		break;
	case RET_VARIABLE:
		break;
	default:
		break;
	}

	if (!pTreeNode->iIsLeaf())
	{
		this->strLogicString = this->strLogicString + _SSTR("(");
	}

	switch (pTreeNode->iGetData().Type)
	{
	case RET_OPERATOR_NULL:
	case RET_OPERATOR_AND:
	case RET_OPERATOR_OR:
	case RET_OPERATOR_IMPLY:
	case RET_OPERATOR_BICOND:
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			this->_GenerateLogicString(pTreeNode->iGetOffshootPtr(seek));
			if (seek != (pTreeNode->iGetSize() - 1))
			{
				this->strLogicString = this->strLogicString + _SSTR(" ") + strOperator;
			}
		}
		break;
	case RET_OPERATOR_NOT:
		this->strLogicString = this->strLogicString + strOperator;
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			this->_GenerateLogicString(pTreeNode->iGetOffshootPtr(seek));
		}
		break;
	case RET_CONSTANT:
	case RET_VARIABLE:
		this->strLogicString = this->strLogicString + _SSTR(" ");
		if (pTreeNode->iGetData().bNegative)
		{
			this->strLogicString = this->strLogicString + _SSTR("!");
		}
		this->strLogicString = this->strLogicString + pTreeNode->iGetData().strValue;
		break;
	default:
		break;
	}

	if (!pTreeNode->iIsLeaf())
	{
		this->strLogicString = this->strLogicString + _SSTR(" )");
	}
}
void PROPOSITIONALLOGICRULE::_Inverse(TREE<RULEELEMENT>* pTreeNode)
{
	if (pTreeNode == nullptr)
	{
		return;
	}
	RULEELEMENT Element = pTreeNode->iGetData();
	switch (Element.Type)
	{
	case RET_OPERATOR_NULL:
	{
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			this->_Inverse(pTreeNode->iGetOffshootPtr(seek));
		}
		break;
	}
	case RET_OPERATOR_AND:
		Element.Type = RET_OPERATOR_OR;
		pTreeNode->iSetData(Element);
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			this->_Inverse(pTreeNode->iGetOffshootPtr(seek));
		}
		break;
	case RET_OPERATOR_OR:
		Element.Type = RET_OPERATOR_AND;
		pTreeNode->iSetData(Element);
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			this->_Inverse(pTreeNode->iGetOffshootPtr(seek));
		}
		break;
	case RET_OPERATOR_NOT:
	{
		Element.Type = RET_OPERATOR_NULL;
		pTreeNode->iSetData(Element);
		//this->_Inverse(pTreeNode);
		break;
	}
	case RET_OPERATOR_IMPLY:
	case RET_OPERATOR_BICOND:
		Element.Type = RET_OPERATOR_NOT;
		pTreeNode->iInsert(0, *pTreeNode, 0.0);
		while (pTreeNode->iGetSize() != 1)
		{
			pTreeNode->iDelete(1);
		}
		pTreeNode->iSetData(Element);
		break;
	case RET_CONSTANT:
	case RET_VARIABLE:
		Element.bNegative = !Element.bNegative;
		pTreeNode->iSetData(Element);
		break;
	default:
		break;
	}
}
bool PROPOSITIONALLOGICRULE::_IsCNFTree(const TREE<RULEELEMENT>* pTreeNode) const
{
	if (pTreeNode == nullptr)
	{
		return true;
	}

	bool bIsCNF = true;
	switch (pTreeNode->iGetData().Type)
	{
	case RET_OPERATOR_NULL:
	case RET_OPERATOR_AND:
		bIsCNF = true;
		for (uint32 seek = 0; seek < pTreeNode->iGetSize(); seek = seek + 1)
		{
			bIsCNF = bIsCNF && this->_IsCNFTree(pTreeNode->iGetOffshootPtr(seek));
		}
		return bIsCNF;
		break;
	case RET_OPERATOR_OR:
		if (pTreeNode->iGetRelativeMaxDepth() == 1)
		{
			return true;
		}
		else
		{
			uint32 Depth = pTreeNode->iGetRelativeMaxDepth();
			return false;
		}
	case RET_OPERATOR_NOT:case RET_OPERATOR_IMPLY:case RET_OPERATOR_BICOND:
		return false;
		break;
	case RET_CONSTANT:
	case RET_VARIABLE:
	default:
		return true;
		break;
	}
	return false;
}
bool PROPOSITIONALLOGICRULE::_IsElement(const SString &strString) const
{
	for (uint32 seek = 0; seek < strString.size(); seek = seek + 1)
	{
		switch (strString[seek])
		{
		case _SSTR('('):case _SSTR(')'):case _SSTR('&'):case _SSTR('|'):case _SSTR('='):case _SSTR('<'):
			return false;
			break;
		default:
			break;
		}
	}
	return true;
}
PROPOSITIONALLOGICRULE& PROPOSITIONALLOGICRULE::operator=(const PROPOSITIONALLOGICRULE &LogicRule)
{
	this->strLogicString = LogicRule.strLogicString;
	this->iGenerateTree(this->strLogicString);
	return *this;
}
bool PROPOSITIONALLOGICRULE::operator==(const PROPOSITIONALLOGICRULE &LogicRule)
{
	return this->Tree == LogicRule.Tree;
}
bool PROPOSITIONALLOGICRULE::operator!=(const PROPOSITIONALLOGICRULE &LogicRule)
{
	return this->Tree != LogicRule.Tree;
}
void PROPOSITIONALLOGICRULE::iGenerateTree(const SString &strRule)
{
	this->strLogicString = strRule;

	SString strString;

	strString.resize(strRule.size());

	/*Erase the space*/
	uint32 Index = 0;
	for (uint32 seek = 0; seek < strRule.size(); seek = seek + 1)
	{
		if (strRule[seek] != _SSTR(' '))
		{
			strString[Index] = strRule[seek];
			Index = Index + 1;
		}
	}
	this->_GenerateTree(&(this->Tree), strString);
}
void PROPOSITIONALLOGICRULE::iGenerateClausesList()
{
	if (!this->iIsCNFFormat())
	{
		this->iConvertToCNF();
	}
	this->ClausesList.clear();

	this->_GenerateClausesList(&(this->Tree));
}
void PROPOSITIONALLOGICRULE::iGenerateLogicString()
{
	this->strLogicString.clear();
	this->_GenerateLogicString(&(this->Tree));
}
void PROPOSITIONALLOGICRULE::iConvertToCNF()
{
	while (!this->iIsCNFFormat())
	{
		this->_ConvertToCNF(&this->Tree, 0);
		this->_PruningTree(&this->Tree);
		this->_ConvertToCNF(&this->Tree, 1);
		this->_PruningTree(&this->Tree);
		this->_OptimizeTree(&this->Tree);
	}
}
void PROPOSITIONALLOGICRULE::iInverse()
{
	this->_Inverse(&(this->Tree));
	this->_PruningTree(&(this->Tree));
}
vector<LOGICCLAUSES> PROPOSITIONALLOGICRULE::iGetCNFClausesList() const
{
	return this->ClausesList;
}
SString PROPOSITIONALLOGICRULE::iGetLogicString() const
{
	return this->strLogicString;
}
bool PROPOSITIONALLOGICRULE::iIsCNFFormat() const
{
	return this->_IsCNFTree(&(this->Tree));
}
SString PROPOSITIONALLOGICRULE::iPrintTree() const
{
	return this->Tree.iPrintTree().c_str();
}