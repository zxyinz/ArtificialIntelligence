#include"../RelyOn/Core/SanMathematics.h"
#include"AIContainer.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __PROPOSITIONALLOGICRULEDEF_H__
#define __PROPOSITIONALLOGICRULEDEF_H__
		enum eRULEELEMENTTYPE
		{
			RET_OPERATOR_NULL,
			RET_OPERATOR_AND,
			RET_OPERATOR_OR,
			RET_OPERATOR_NOT,
			RET_OPERATOR_IMPLY,
			RET_OPERATOR_BICOND,
			RET_CONSTANT,
			RET_VARIABLE,			//First order rule, not use
		};

		struct RULEELEMENT : public TREENODE<RULEELEMENT>
		{
		public:
			eRULEELEMENTTYPE Type;
			SString strValue;
			bool bNegative;
		public:
			RULEELEMENT();
			RULEELEMENT(const RULEELEMENT& ELement);
			~RULEELEMENT();

			bool operator==(const RULEELEMENT& Element) const;
			bool operator!=(const RULEELEMENT& Element) const;

			SString iPrintNode() const;
		};

		typedef vector<RULEELEMENT> LOGICCLAUSES;

		bool operator==(const LOGICCLAUSES &Clauses1, const LOGICCLAUSES &Clauses2);
		bool operator<(const LOGICCLAUSES &LClauses, const LOGICCLAUSES &RClauses);

		class LogicClausesCompare
		{
		public:
			bool operator()(const LOGICCLAUSES &LClauses, const LOGICCLAUSES &RClauses) const;
		};


		struct PROPOSITIONALLOGICRULE
		{
		private:
			TREE<RULEELEMENT> Tree;
			vector<LOGICCLAUSES> ClausesList;
			SString strLogicString;
		protected:
			/*Tree generate core function*/
			bool _GenerateTree(TREE<RULEELEMENT>* pTreeNode, SString strString);
			bool _CreateTreeNode(TREE<RULEELEMENT>* pTreeNode, SString strString);

			/*Core function to convert a tree to CNF tree, have 3 stage	*/
			/*State 0: Convert IMPLY and BICOND operator				*/
			/*State 1: Convert AND, OR and NOT operator					*/
			void _ConvertToCNF(TREE<RULEELEMENT>* pTreeNode, const uint32 State);

			/*Pruning the RET_OPERATOR_NULL Node*/
			int32 _PruningTree(TREE<RULEELEMENT>* pTreeNode);

			/*Optimize the rule tree*/
			void _OptimizeTree(TREE<RULEELEMENT>* pTreeNode);

			/*Core function to generate the clauses list*/
			void _GenerateClausesList(TREE<RULEELEMENT>* pTreeNode);

			/*Core function to generate the logic string*/
			void _GenerateLogicString(TREE<RULEELEMENT>* pTreeNode);

			/*Core function to apply logic inverse to tree*/
			void _Inverse(TREE<RULEELEMENT>* pTreeNode);

			/*Core function to judge if current tree is a CNF tree*/
			bool _IsCNFTree(const TREE<RULEELEMENT>* pTreeNode) const;

			/*Resurn if current string is an element (!A is an element, (A and B) is not)*/
			bool _IsElement(const SString &strString) const;
		public:
			PROPOSITIONALLOGICRULE(const SString &strRule = _SSTR(""));
			PROPOSITIONALLOGICRULE(const PROPOSITIONALLOGICRULE &LogicRule);
			~PROPOSITIONALLOGICRULE();

			PROPOSITIONALLOGICRULE& operator=(const PROPOSITIONALLOGICRULE &LogicRule);

			/*Operator overload*/
			bool operator==(const PROPOSITIONALLOGICRULE &LogicRule);
			bool operator!=(const PROPOSITIONALLOGICRULE &LogicRule);

			/*Generate logic tree, may not be CNF tree*/
			void iGenerateTree(const SString &strRule);

			/*Generate the clauses list, must be CNF tree*/
			void iGenerateClausesList();

			/*Generate the logic string base on current tree*/
			void iGenerateLogicString();

			/*Convert current tree to CNF tree*/
			void iConvertToCNF();

			/*Inverse current tree (Logical Inverse)*/
			void iInverse();

			vector<LOGICCLAUSES> iGetCNFClausesList() const;
			SString iGetLogicString() const;

			/*Return if current tree is a CNF tree*/
			bool iIsCNFFormat() const;

			/*For debug, print the whole tree*/
			SString iPrintTree() const;
		};
#endif
	}
}