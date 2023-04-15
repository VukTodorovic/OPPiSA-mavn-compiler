#ifndef __VAR__
#define __VAR__
/* Autor: Vuk Todorovic Datum: 17.06.2022. */


using namespace std;
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE //konstanta
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign), m_value(0) {}
	//pravljenje mem i reg variabli
	Variable(std::string name, VariableType type) : m_type(type), m_name(name), m_assignment(no_assign), m_value(0){}


	void setValue(int val) { m_value = val; }
	int getValue() { return m_value; }
	void setAssignment(Regs reg) { m_assignment = reg; }
	const Regs getAssignment() { return m_assignment; }

	const std::string getName() { return m_name; }
	const VariableType getType() { return m_type; }

	const void spill()
	{

	}
	const std::string toStringVarList()
	{
		std::string output;
		if (m_type == MEM_VAR)
		{
			output = "MEM: " + this->m_name +  " " + std::to_string(this->m_value);
		}
		else if (m_type == REG_VAR)
		{
			output = "REG: ";
			output += this->m_name;
		}
		else //number
		{
			output = "NUM: " + std::to_string(this->m_value) + " ";
		}
		output += "\0";
		return output;
	}

	const std::string toStringLiveness()
	{
		std::string output = "";
		if (m_type == MEM_VAR)
		{
			output = this->m_name;
		}
		else if (m_type == REG_VAR)
		{
			output += this->m_name;
		}
		else //number
		{
			output = std::to_string(this->m_value);
		}
		output += "\0";
		return output;
	}

	const std::string toStringOutput()
	{
		std::string output = "";
		if (m_type == MEM_VAR)
		{
			output = this->m_name;
		}
		else if (m_type == REG_VAR)
		{
			output = "$t";
			output += std::to_string(this->m_assignment-1);
		}
		else //number
		{
			output = std::to_string(this->m_value);
		}
		output += "\0";
		return output;
	}

	const std::string toStringSectionData()
	{
		return m_name + ":\t.word " + std::to_string(m_value) + "\0";
	}

	
	int m_position;//mesto u grafu interferencije, konstante nece uticati na poziciju varijabli grafa
private:
	VariableType m_type;
	std::string m_name;
	int m_value;//polje korisceno za const variable
	Regs m_assignment;
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;
static Variables g_variables;

Variable* getVariable(std::string name)
{
	for each (Variable* var in g_variables)
	{
		if (var->getName() == name)
		{
			return var;
		}
	}
	throw std::invalid_argument("Error: wanted variable not found in the list!");
}

bool variableExists(string name, Variables varList = g_variables)
{
	for each (Variable* var in varList)
	{
		if (var->getName() == name)
		{
			return true;
		}
	}
	return false;
}

#endif // !__VAR__