#ifndef __INSTRUCTION__
#define __INSTRUCTION__
/* Autor: Vuk Todorovic Datum: 17.06.2022. */

//#include "LexicalAnalysis.h"
#include "Types.h"
#include "Label.h"
#include "Variable.h"

using namespace std;
/**
 * This class represents one instruction in program code.
 */

int totalPos = 0;
class Instruction
{
public:
	Instruction() : m_position(totalPos), m_type(I_NO_TYPE), m_label(NULL) { totalPos++; }

	//zarad prosirenja stabla medjukoda
	Instruction(InstructionType type) : m_position(totalPos), m_type(type), m_label(NULL) { totalPos++; }

	Variables getDst() { return m_dst; }
	const Variables getDst() const { return m_dst; }
	Variables getSrc() { return m_src; }
	const Variables getSrc() const { return m_src; }

	const InstructionType getType()
	{
		return m_type;
	}

	void addSrc(Variable* src)
	{
		m_src.push_back(src);
	}
	void addSrcs(Variables srcs)
	{
		for each (Variable * var in srcs)
		{
			addSrc(var);
		}
	}

	void addDst(Variable* dst)
	{
		m_dst.push_back(dst);
	}
	void addDsts(Variables dsts)
	{
		for each (Variable * var in dsts)
		{
			addDst(var);
		}
	}

	void addLabel(Label* label)
	{
		m_label = label;
	}

	Label* getLabel() { return m_label; }

	std::string toString()
	{
		std::string result;
		bool commaOutput = true;//automatizovan ispis sa zarezima
		switch (m_type)
		{
		case I_NO_TYPE:
		{
			result = "no_type\t";
			break;
		}
		case I_ADD:
		{
			result = "add\t";
			break;
		}
		case I_ADDU:
		{
			result = "addu\t";
			break;
		}
		case I_ADDI:
		{
			result = "addi\t";
			break;
		}
		case I_SUB:
		{
			result = "sub\t";
			break;
		}
		case I_NOR:
		{
			result = "nor\t";
			break;
		}
		case I_LA:
		{
			result = "la\t" + (m_dst.front())->toStringOutput() + ", " + (m_src.front())->toStringOutput();
			commaOutput = false;
			break;
		}
		case I_LI:
		{
			result = "li \t" + (m_dst.front())->toStringOutput() + ", " + (m_src.front())->toStringOutput();
			commaOutput = false;
			break;
		}
		case I_LW:
		{
			result = "lw\t" + (m_dst.front())->toStringOutput() + ", " + (m_src.front())->toStringOutput();
			list<Variable*>::iterator src = m_src.begin();
			src++;
			result += "(" + (*src)->toStringOutput() + ")";
			commaOutput = false;
			break;
		}
		case I_SW:
		{
			result = "sw\t" + (m_src.front())->toStringOutput() + ", ";
			list<Variable*>::iterator src = m_src.begin();
			src++;
			result +=(m_src.front())->toStringOutput();
			commaOutput = false;
			break;
		}
		case I_BLTZ:
		{
			result = "bltz\t " + (m_src.front())->toStringOutput() + ", " + m_label->getName();
			commaOutput = false;
			break;
		}
		case I_BLT:
		{
			result = "blt\t" + (m_src.front())->toStringOutput() + ", ";
			list<Variable*>::iterator src = m_src.begin();
			src++;
			result += (m_src.front())->toStringOutput(); ", " + m_label->getName();
			commaOutput = false;
			break;
		}
		case I_B:
		{
			result = "b\t" + m_label->getName();
			break;
		}
		case I_NOP:
		{
			result = "nop\t";
			break;
		}
		default:
		{
			throw std::runtime_error("Instruction not found");
		}
		}
		if (commaOutput)
		{
			for each (Variable * dst in m_dst)
			{
				result += dst->toStringOutput();
			}
			for each (Variable * src in m_src)
			{
				result += ", " + src->toStringOutput();
			}
		}
		/*else//lw sw
		{
			for each (Variable * dst in m_dst)
			{
				result += dst->toStringOutput();
			}
			
			Variables::iterator it = m_src.begin();
			result += ", " + (*it)->toStringOutput();
			it++;
			result += "(" + (*it)->toStringOutput() + ")";
		}*/
		result += "\0";
		return result;
	}

	std::string toStringLiveness()
	{
		string result = "TYPE: ";
		switch (m_type)
		{
		case I_NO_TYPE:
		{
			result += "no_type\t";
			break;
		}
		case I_ADD:
		{
			result += "add\t";
			break;
		}
		case I_ADDI:
		{
			result += "addi\t";
			break;
		}
		case I_SUB:
		{
			result += "sub\t";
			break;
		}
		case I_LA:
		{
			result += "la\t";
			break;
		}
		case I_LI:
		{
			result += "li\t";
			break;
		}
		case I_LW:
		{
			result += "lw\t";
			break;
		}
		case I_SW:
		{
			result += "sw\t";
			break;
		}
		case I_BLTZ:
		{
			result += "bltz\t";
			break;
		}
		case I_B:
		{
			result += "add\t";
			break;
		}
		case I_NOP:
		{
			result += "nop\t";
			break;
		}
		case I_NOR:
		{
			result = "nor\t";
			break;
		}
		case I_BLT:
		{
			result = "blt";
			break;
		}
		case I_ADDU:
		{
			result = "addu\t";
			break;
		}
		default:
		{
			throw std::runtime_error("Instruction not found");
		}
		}
		result += "\nPOSITION: " + to_string(m_position);

		result += "\nSUCC: ";
		for each (Instruction * var in m_succ)
			result += " " + to_string(var->m_position);

		result += "\nUSE:";
		for each (Variable * var in m_use)
		{
			result += " " + var->toStringLiveness();
		}

		result += "\nDEF:";
		for each (Variable * var in m_dst)
		{
			result += " " + var->toStringLiveness();
		}

		result += "\nIN: ";
		for each (Variable * var in m_in)
		{
			result += " " + var->toStringLiveness();
		}

		result += "\nOUT: ";
		for each (Variable * var in m_out)
		{
			result += " " + var->toStringLiveness();
		}

		result += "\n\0";
		return result;
	}
private:
	int m_position;
	InstructionType m_type;

public:
	Variables m_dst;
	Variables m_src;
	Variables m_use;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;

private:
	//prosirenje za instrukcije sa labelama
	Label* m_label;

};

/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;
Instructions g_instructions;

Instruction* getLabelPosition(Label* lab)//nadje u kojoj je instrukciji definisana prosledjena labela
{
	Labels::iterator itLabel = g_labels.begin();
	for each (Instruction* instr in g_instructions)
	{
		if (instr->getType() == I_NO_TYPE)//naisli na labelu
		{
			Label* instrLab = instr->getLabel();
			//cout << "*****************SENTLABEL:" << lab->getName();
			//cout << "****************GETLABEL:" << instrLab->getName();
			if (instrLab->getName() == lab->getName())
			{
				//cout << "SUCCESS";
				return instr;
			}
		}

	}
		throw std::invalid_argument("Error: label not found");
		return NULL;
}


///temp
int usedRegs = 0;
Regs getNewReg()
{
	++usedRegs;
	return Regs(usedRegs);
}
#endif