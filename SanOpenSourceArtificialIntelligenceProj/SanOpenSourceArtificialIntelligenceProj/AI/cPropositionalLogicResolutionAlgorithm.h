#include"PropositionalLogicRuleDef.h"
using namespace std;
using namespace San;
#pragma once
namespace San
{
	namespace AI
	{
#ifndef __CPROPOSITIONALLOGICRESOLUTIONALGORITHM_H__
#define __CPROPOSITIONALLOGICRESOLUTIONALGORITHM_H__
		enum eLOGICRESULT
		{
			LR_TRUE,
			LR_FALSE,
			LR_UNKNOWN,
		};

		class cPropositionalLogicResolutionAlgorithm
		{
		private:
			vector<LOGICCLAUSES> m_KnowledgeBase;
			vector<pair<SString, SVECTOR3>> m_ElementTable; // ElementName, (Positive Count, Negative Count)
		protected:
			/*Resolution Core Algorithm*/
			bool _Resolve(vector<LOGICCLAUSES> InverseAlpha);

			/*Resolution Core Algorithm (optimized version)*/
			bool _ResolveOptimize(vector<LOGICCLAUSES> InverseAlpha);

			/*New Clauses generate function*/
			bool _GenerateNewClausesList(const LOGICCLAUSES &SrcClauses, const LOGICCLAUSES &DestClauses, const vector<LOGICCLAUSES> &ClausesList, vector<LOGICCLAUSES> &NewClausesList);

			/*Insert new clause into clauses list (min - max order)*/
			void _InsertClause(const LOGICCLAUSES &Clause, vector<LOGICCLAUSES> &ClausesList);

			/*Find the clause in caluses list*/
			int32 _FindClause(const LOGICCLAUSES &Clause, const vector<LOGICCLAUSES> &ClausesList);

			/*Print the clauses list, for debug*/
			SString _PrintClausesList(const vector<LOGICCLAUSES> &ClausesList);

			/*Element table operator, Add*/
			void _AddElementToTable(const RULEELEMENT &Clause, vector<pair<SString, SVECTOR3>> &ElementTable);

			/*Element table operator, Delete*/
			void _DeleteElementFromTable(const RULEELEMENT &Clause, vector<pair<SString, SVECTOR3>> &ElementTable);

			/*Element table operator, Find*/
			int32 _GetElementIndex(const RULEELEMENT &Clausee, vector<pair<SString, SVECTOR3>> &ElementTable);
		public:
			cPropositionalLogicResolutionAlgorithm();
			~cPropositionalLogicResolutionAlgorithm();

			bool iGenerateKnowedgeBase(const vector<SString> &strRuleStringList);

			void iAddClauses(const vector<LOGICCLAUSES> &ClausesList);
			bool iAddClause(const LOGICCLAUSES &Clause);

			void iDeleteClauses(const vector<LOGICCLAUSES> &ClausesList);
			bool iDeleteClause(const LOGICCLAUSES &Clause);

			void iClearClauses();

			/*Resolution Core Algorithm Interface*/
			eLOGICRESULT iResolve(const SString &strTargetString);
			eLOGICRESULT iResolve(const vector<LOGICCLAUSES> &TargetClauses);

			/*Resolution Core Algorithm Interface (optimized version)*/
			eLOGICRESULT iResolveOptimize(const SString &strTargetString);
			eLOGICRESULT iResolveOptimize(const vector<LOGICCLAUSES> &TargetClauses);

			SString iPrintKnowledgeBase() const;
		};
#endif
	}
}