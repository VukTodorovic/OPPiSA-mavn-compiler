#ifndef __SYNTAX_ANALYSIS__
#define __SYNTAX_ANALYSIS__
/* Autor: Vuk Todorovic Datum: 17.06.2022. */
#include <iostream>
#include <iomanip>
#include "LexicalAnalysis.h"
#include "Instruction.h"
using namespace std;
class SyntaxAnalysis
{
public:

	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex);

	/**
	* Method which performs lexical analysis
	*/
	bool Do();


	///TEMP
	void printVarList();

	void printAll();

	void printSectionData();

	void liveness();

private:
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);

	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();

	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& lexicalAnalysis;

	/**
	* Syntax error indicator
	*/
	bool errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token currentToken;
};

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin()) {}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	Q();
	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left;
	token.printTokenInfo();
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			cout << currentToken.getValue() << endl;
			currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	if (errorFound == false)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}


void SyntaxAnalysis::S()
{
	if (errorFound == false)
	{
		Variable* var = NULL;
		Label* lab = NULL;
		switch (currentToken.getType())
		{
		case T_MEM:
		{
			eat(T_MEM);
			var = new Variable(currentToken.getValue(), Variable::MEM_VAR);
			eat(T_M_ID);
			var->setValue(stoi(currentToken.getValue()));
			if (variableExists(var->getName()))
				throw runtime_error("Error: variable already exists");
			//cout << "napravljena variabla: mem id ";

			eat(T_NUM);
			break;
		}
		case T_REG:
		{
			eat(T_REG);
			var = new Variable(currentToken.getValue(), Variable::REG_VAR);
			if (variableExists(var->getName()))
				throw runtime_error("Error: variable already exists");
			//cout << "napravljena variabla: reg r_id, alociran registar ";

			eat(T_R_ID);
			break;
		}
		case T_FUNC:
		{
			Instruction* instr = new Instruction(I_NO_TYPE);
			g_instructions.push_back(instr);
			eat(T_FUNC);

			lab = new Label(currentToken.getValue());
			if (labelExists(lab->getName()))
				throw runtime_error("Error: label already exists");
			instr->addLabel(lab);
			//cout << "napravljena labela: label t_id ";
			eat(T_ID);
			break;
		}
		case T_ID:
		{
			Instruction* instr = new Instruction(I_NO_TYPE);
			g_instructions.push_back(instr);
			lab = new Label(currentToken.getValue());
			if (labelExists(lab->getName()))
				throw runtime_error("Error: label already exists");
			instr->addLabel(lab);
			eat(T_ID);
			eat(T_COL);
			E();
			break;
		}
		default:
		{
			E();
			// error
			//printSyntaxError(currentToken);
			//errorFound = true;
			break;
		}
		}
		if (var != NULL)
			g_variables.push_back(var);
		if (lab != NULL)
			g_labels.push_back(lab);
	}
}


void SyntaxAnalysis::L()
{
	if (errorFound == false)
	{
		switch (currentToken.getType())
		{
		case T_END_OF_FILE:
		{
			//eat(T_END_OF_FILE);///PITANJE
			break;
		}
		default:
		{
			Q();
			break;
		}
		}
	}
}


void SyntaxAnalysis::E()
{
	if (errorFound == false)
	{
		Instruction* instr = NULL;
		Variables srcs;
		Variables dsts;
		switch (currentToken.getType())
		{
		case T_ADD:
		{
			eat(T_ADD);
			instr = new Instruction(I_ADD);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			break;
		}
		case T_ADDU:
		{
			eat(T_ADDU);
			instr = new Instruction(I_ADDU);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			break;
		}
		case T_ADDI:
		{
			eat(T_ADDI);
			instr = new Instruction(I_ADDI);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			Variable* number = new Variable();
			number->setValue(stoi(currentToken.getValue()));
			srcs.push_back(number);
			eat(T_NUM);
			break;
		}
		case T_SUB:
		{
			eat(T_SUB);
			instr = new Instruction(I_SUB);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			break;
		}
		case T_NOR:
		{
			eat(T_NOR);
			instr = new Instruction(I_NOR);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			break;
		}
		case T_LA:
		{
			eat(T_LA);
			instr = new Instruction(I_LA);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_M_ID);
			break;
		}
		case T_LW:
		{
			eat(T_LW);
			instr = new Instruction(I_LW);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			Variable* number = new Variable();
			number->setValue(stoi(currentToken.getValue()));
			srcs.push_back(number);//PITANJE
			eat(T_NUM);
			eat(T_L_PARENT);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_R_PARENT);
			break;
		}
		case T_LI:
		{
			eat(T_LI);
			instr = new Instruction(I_LI);

			dsts.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			Variable* number = new Variable();
			number->setValue(stoi(currentToken.getValue()));
			srcs.push_back(number);
			eat(T_NUM);
			break;
		}
		case T_SW:
		{
			eat(T_SW);
			instr = new Instruction(I_SW);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			Variable* number = new Variable();
			number->setValue(stoi(currentToken.getValue()));
			srcs.push_back(number);
			eat(T_NUM);
			eat(T_L_PARENT);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_R_PARENT);
			break;
		}
		case T_B:
		{
			eat(T_B);
			instr = new Instruction(I_B);

			instr->addLabel(getLabel(currentToken.getValue()));
			eat(T_ID);
			break;
		}
		case T_BLTZ:
		{
			eat(T_BLTZ);
			instr = new Instruction(I_BLTZ);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);


			instr->addLabel(getLabel(currentToken.getValue()));
			eat(T_ID);
			break;
		}
		case T_BLT://////////////////////////////////////////////////////////////////////////
		{
			cout << "BLT";
			eat(T_BLT);
			instr = new Instruction(I_BLT);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			srcs.push_back(getVariable(currentToken.getValue()));
			eat(T_R_ID);
			eat(T_COMMA);

			instr->addLabel(getLabel(currentToken.getValue()));
			eat(T_ID);
			break;
		}
		case T_NOP:
		{
			eat(T_NOP);
			instr = new Instruction(I_NOP);
			break;
		}
		default:
		{
			// error
			printSyntaxError(currentToken);
			errorFound = true;
			break;
		}
		}
		if (instr != NULL)
		{
			if (!srcs.empty())
				instr->addSrcs(srcs);
			if (!dsts.empty())
				instr->addDsts(dsts);
			g_instructions.push_back(instr);
		}
	}
}



///TEMP RESENJE
void SyntaxAnalysis::printVarList()
{
	cout << "Variable list after syntax analysis:" << endl;

	for each (Variable * var in g_variables)
	{
		cout << var->toStringVarList() << endl;
	}
}

void SyntaxAnalysis::printAll()
{
	ofstream fout("result.s");
	//fout << "****************OUTPUT*************************" << endl;
	fout << ".globl " << (*g_labels.begin())->getName() << endl << endl;//.globl main
	fout << ".data" << endl;
	for each (Variable * var in g_variables)
	{
		if (var->getType() == Variable::MEM_VAR)
			fout << var->toStringSectionData() << endl;
	}
	fout << endl << ".text" << endl;


	//int iLabel = 0;//indeks sledece labele po redu za ispis

	Labels::iterator itLabel = g_labels.begin();
	for each (Instruction * instr in g_instructions)
	{
		if (instr->getType() != I_NO_TYPE)//ako nema operatora onda je labela, za sta pravimo nov
		{
			fout << instr->toString() << endl;
		}
		else
		{
			fout << (*itLabel)->getName() << ":" << endl;
			itLabel++;
		}
	}

	cout << "result.s done!" << endl;
}

#endif // !__SYNTAX__