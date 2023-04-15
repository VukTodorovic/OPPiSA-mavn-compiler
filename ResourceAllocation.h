#ifndef __RESOURCE_ALLOCATION__
#define __RESOURCE_ALLOCATION__
/* Autor: Vuk Todorovic Datum: 17.06.2022. */

#include "LivenessAnalysis.h"
#include <stack>
#include <algorithm>

class InterferenceGraph
{
public:
		vector<Variable*> variables;	///< all variables from instructions
		char** values;			///< matrix value
		int size;				///< size of square matrix

		Variables remainingNodes;//lista nedodatih,radimo sa kopijom da ne unistimo citljivost grafa

		InterferenceGraph()
		{
			int position = 0;
			for each (Variable * var in g_variables) //dodamo sve reg variable u graf
			{
				if (var->getType() == Variable::REG_VAR)
				{
					variables.push_back(var);
					var->m_position = position;
					position++;
					remainingNodes.push_back(var);
				}
			}
			size = variables.size();
			values = new char* [size];
		}

		void fillInterferenceGraph()
		{
			for (int i = 0; i < size; ++i)//values = new char[size][size];
			{
				values[i] = new char[size];
				memset(values[i], __EMPTY__, size);//podesava sve interferencije na nulu
			}
			//punjenje smetnjama //ako nije move
			for each (Instruction * instr in g_instructions)
			{
				if (!(instr->m_dst).empty())//za svaku definiciju (ako je ima)
				{
					Variable* dst = (instr->m_dst).front();
					for each (Variable * var in instr->m_out)//dodaj interferenciju sa promenljivama koje su u out
					{
						int pozicijaPrvog = var->m_position;
						int pozicijaDrugog = dst->m_position;
						values[pozicijaPrvog][pozicijaDrugog] = __INTERFERENCE__;
						values[pozicijaDrugog][pozicijaPrvog] = __INTERFERENCE__;
					}
				}
			}

			//cistimo smetnje sa samim sobom
			for (int j = 0; j < size; ++j)//ocisti veze u drugim promenljivim
			{
				values[j][j] = __EMPTY__;
			}
		}

		void print()
		{

			cout << "___________Interference graph:____________" << endl;
			cout << "List of nodes" << endl;
			for each (Variable * var in variables)
			{
				cout << var->toStringLiveness() << " pos in table: " << var->m_position << endl;
			}
			cout << "_________________________________________" << endl;

			if (remainingNodes.empty())
			{
				cout << "All nodes moved to stack" << endl;
			}
			else
			{
				cout << "Nodes to be moved to stack" << endl;
				for each (Variable * var in remainingNodes)
				{
					cout << var->toStringLiveness() << " pos in table: " << var->m_position << endl;
				}
			}
			printTableView();
		}

		void printTableView()
		{
			cout << "Table view" << endl << "\\\\";
			for (int i = 0; i < size; ++i)
			{
				cout << variables[i]->getName() << " ";
			}
			cout << endl;
			for (int i = 0; i < size; ++i)
			{
				cout << variables[i]->getName();
				for (int j = 0; j < size; ++j)
				{
					if (values[i][j] == __INTERFERENCE__)
					{
						cout << " * ";
					}
					if (values[i][j] == __EMPTY__)
					{
						cout << " _ ";
					}
				}
				cout << endl;
			}
		}
};

int K = __REG_NUMBER__;

Variable* getNode(InterferenceGraph& ig)
{
	Variable* varRes = nullptr;
	int bestK = -1;
	int iPos = -1;
	for (int i = 0; i < ig.size; i++)
	{
		if (variableExists((ig.variables[i])->getName(), ig.remainingNodes))
		{
			int currentK = 0;
			for (int j = 0; j < ig.size; j++)
			{
				if (ig.values[i][j] == __INTERFERENCE__)
				{
					currentK++;
				}
			}
			if (currentK > bestK && currentK < K)//uspesno nadjena prva promenljiva sa odgovarajuce veza
			{
				varRes = ig.variables[i];
				bestK = currentK;
				iPos = i;
			}
		}
	}

	if (varRes != nullptr)
	{
		for (int j = 0; j < ig.size; ++j)//ocisti veze u drugim promenljivim
		{
			ig.values[iPos][j] = __EMPTY__;
			ig.values[j][iPos] = __EMPTY__;
		}
		return varRes;
	}
	else
	{
		return nullptr;//nije nadjena
	}
}

static bool simplify(InterferenceGraph& ig, stack<Variable*>& ss)
{
	//uzmemo maksimalan broj mogucih konekcija, spill detektujemo kasnije
	while(!((ig.remainingNodes).empty()))//svaku varijablu treba da prebacimo na stek
	{
		Variable* varMaxK = getNode(ig);
		if (varMaxK != nullptr)//uspesno
		{
			ss.push(varMaxK);//dodaj
			(ig.remainingNodes).remove(varMaxK);
			cout << "Removed " << varMaxK->getName() << endl;
			ig.print();
			cout << endl << endl;
		}
		else//smanji broj konekcija i idi ponovo
		{
			cout << "SPILL!\n";
			return false;
		}
	}
	cout << "Made the simplification stack succcessfuly!";
	return true;
}

bool doResourceAllocation(InterferenceGraph& ig, stack<Variable*>& ss)
{
	std::list<Variable*> varList;//obradjen deo
		std::list<Variable*>::iterator itV;

	std::list<Regs> regList;
	ig.fillInterferenceGraph();//popravljamo ga jer smo ga unistili praveci ss

	while (!ss.empty())
	{
		Variable* top = ss.top();
		//napuni reglist
		for (int i = 1; i <= __REG_NUMBER__; ++i)//__REG_NUMBER__ = 4
			regList.push_back((Regs)i);

		for (itV = varList.begin(); itV != varList.end(); itV++) {
			if (ig.values[ss.top()->m_position][(*itV)->m_position] == __INTERFERENCE__)
			{
				regList.remove((*itV)->getAssignment());
			}
		}
		//regListpush_back(reg0);

		if (regList.empty())//nismo uspeli
			return false;

		top->setAssignment(regList.front());

		cout << "Alocirana varijabla " << top->getName() << " registru: " << top->getAssignment()-1 << endl;
		varList.push_back(top);
		ss.pop();
		regList.clear();
	}
	return true;
}


#endif