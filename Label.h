#ifndef __LABEL__
#define __LABEL__
/* Autor: Vuk Todorovic Datum: 17.06.2022. */

using namespace std;
class Label {
private:
	static int nextPosition;
	int position;//mesto u g_variables
	std::string name;

public:
	Label() : position(0), name("") {}
	Label(std::string n) : name(n)
	{
		position = nextPosition;
		nextPosition++;
	}

	void setPosition(int pos) { position = pos; }
	void setName(std::string n) { name = n; }
	std::string getName() { return name; }

};
int Label::nextPosition = 0;
typedef std::list<Label*> Labels;
Labels g_labels;

Label* getLabel(std::string name)
{
	for each (Label* var in g_labels)
	{
		if (var->getName() == name)
		{
			return var;
		}
	}
	throw std::invalid_argument("Error: wanted label not found in the list!");
	//return nullptr;
}

bool labelExists(string name, Labels labelList = g_labels)
{
	for each (Label* var in labelList)
	{
		if (var->getName() == name)
		{
			return true;
		}
	}
	return false;
}

#endif // !__LABEL__
