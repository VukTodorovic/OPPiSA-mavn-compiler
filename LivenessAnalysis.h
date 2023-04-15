#ifndef __LIVENESS_ANALYSIS__
#define __LIVENESS_ANALYSIS__
#include "SyntaxAnalysis.h"
/* Autor: Vuk Todorovic Datum: 17.06.2022. */
//out[n] = unija in[s] (svih s pripada succ[n]) (iteriram succ[n])
	//in[n] = use[n] U (out[n] - def[n])
	//iteracije dok stara iteracija nije jednaka novoj


static void fillSucc()
{
	int size = g_instructions.size();


	std::list<Instruction*>::iterator it = g_instructions.begin();
	std::list<Instruction*>::iterator last = std::prev(g_instructions.end());
	

	for(; it != last; it++)//bez poslednjeg
	{
		if ((*it)->getType() == I_B)//bezuslovni skok dodaj samo gde se skocilo
		{
			Label* lab = (*it)->getLabel();//za labelu iz trenutne instrukcije
			Instruction* instr = getLabelPosition(lab);//nadjemo instrukciju u kojoj je ta labela
			((*it)->m_succ).push_back(instr);
		}
		else if ((*it)->getType() == I_BLTZ || (*it)->getType() == I_BLT)//uslovni skok dodaj i sledeci i labelu gde se skace
		{
			Label* lab = (*it)->getLabel();//za labelu iz trenutne instrukcije
			Instruction* instr = getLabelPosition(lab);//nadjemo instrukciju u kojoj je ta labela
			((*it)->m_succ).push_back(instr);

			((*it)->m_succ).push_back(*(std::next(it)));//dodaj sledecu naredbu
		}
		else
		{
			((*it)->m_succ).push_back(*(std::next(it)));//dodaj sledecu naredbu
		}
	}

	(*g_instructions.rbegin())->m_succ.clear();//poslednji nema succ u programu
}

static void fillUse()//m_use = m_src, al samo registri
{
	for each (Instruction* instr in g_instructions)
	{
		for each (Variable* var in instr->m_src)
		{
			if (var->getType() == Variable::REG_VAR)
			{
				(instr->m_use).push_back(var);
			}
		}
	}
}

static void printLiveness()
{
	list<Instruction*>::reverse_iterator it;
	for (it = g_instructions.rbegin(); it != g_instructions.rend(); it++)
		{
			Instruction* instr = *it;
			cout << "_________________________________________" << endl;
			cout <<	instr->toStringLiveness();
		}
	//for each (Instruction * instr in g_instructions)
	//{
		//cout << "_________________________________________" << endl;
		//cout << instr->toStringLiveness();
	//}
}

static void livenessAnalysis()
{
	fillSucc();
	fillUse();
	bool ponovi = false;

	cout << "\n\n"  << "||||||||||||||||||||||||||||"<< "Instructions before liveness" << "||||||||||||||||||||||||||||" << endl;
	printLiveness();

	int i = 1;
	do
	{
		//cout << "liveness\n";
		ponovi = false;
		std::list<Instruction*>::reverse_iterator it = g_instructions.rbegin();
		for (; it != g_instructions.rend(); it++)
		{
			//cout << "liveness 1. for\n";
			Variables& in = (*it)->m_in;
			Variables& out = (*it)->m_out;

			Variables in_prim;
			for each (Variable * var in in)//Variables in_prim = in;//kopiranje liste
				in_prim.push_back(var);


			//Variables out_prim = out;
			Variables out_prim;
			for each (Variable * var in out)//Variables in_prim = in;//kopiranje liste
				out_prim.push_back(var);

			//out[n] = unija in[s] svih s pripada succ[n]
			std::list<Instruction*>::iterator itSucc = (*it)->m_succ.begin();
			//out[n]=
			for (; itSucc != (*it)->m_succ.end(); itSucc++)//za svako succ[]
			{
				//cout << "liveness 2. for\n";
				out.insert(out.end(), (*itSucc)->m_in.begin(), (*itSucc)->m_in.end());//in[n]
			}
			out.sort();
			out.unique();//ciscenje duplikata

						 //std::list<Instruction*>::iterator itSucc = (*it)->succ.begin();
						 //Variables outMinusDef;

			std::list<Variable*>::iterator itOut = out.begin();
			for (; itOut != out.end(); itOut++)
			{
				//cout << "liveness 3. for\n";
				if (!variableExists((*itOut)->getName(), (*it)->m_dst))
						in.push_back(*itOut);//
			}
			in.sort();
			in.unique();//ciscenje duplikata

			in.insert(in.end(), (*it)->m_use.begin(), (*it)->m_use.end());//unija use
			in.sort();
			in.unique();//ciscenje duplikata
			//provera ucinjena
			if (in_prim != in)
				ponovi = true;
			if (out_prim != out)
				ponovi = true;

			
		}
		cout << "||||||||||||||||||||||||||||" << i << ". iteration of liveness analysis ||||||||||||||||||||||||||||" << endl;
		printLiveness();
		i++;
	} while (ponovi);
}
#endif