# OPPiSA-mavn-compiler

## Description on english

Project for university featuring MAVN compiler written in C/C++ with following steps:

```
● lexical analysis
● syntax analysis
● generating instructions
● intermediate code generation
● liveness analysis
● resource allocation
● final code generation
```

## Documentation

### Labele (Label.h)

Fajl _Label.h_ definiše klasu Label kao i propratnepromenljive i metode koje se odnose na
labele programa. Neki od značajnijih delova fajla _Label.h_ su:

#### Klasa Label

```
● odgovarajući konstruktori klase Label, ge t i set metodenjenih polja
● int position , označava mesto varijable u globalnojlisti varijabli
● string name , koja označava ime varijable
```
#### Lista labela

```
● typedef list<Label*> Labels, imenuje Llabels kao listulabela
● Labels g_labels, pravi globalnu listu labela kojućemo koristiti
tokom celog zadatka
● Label* getLabel(string), dobavlja labelu iz g_labelssa
prosleđenim imenom, u slučaju da ne uspe, javlja grešku
● bool labelExists(name, Labels), proverava da li
labela (prosleđeno njeno ime) postoji u prosleđenoj listi
```

### Varijable (Variable.h)

Fajl _Variable.h_ definiše klasu Variable kao i propratnepromenljive i metode koje se
odnose na labele programa. Neki značajniji delovi fajla _Variable.h_ su:

#### Klasa Variable

```
● odgovarajući konstruktori klase Variable, get i set metode njenih polja
● enum VariableType {
MEM_VAR, //varijabla memorijskog tipa
REG_VAR, //varijabla koja se skladišti u registar
NO_TYPE //konstanta
};
VariableType imenuje sve moguće tipove varijable
● VariableType m_type , označava tip varijable
● string m_name , označava ime varijable
● int m_value , označava vrednost koju varijabla nosiu slučaju da
je MEM_VAR, ili konstanta
● Regs m_assignment , označava registar kome je varijabladodeljena
● implementacije raznih string toString() metoda kojese kasnije koriste
zarad ispisa varijable u različitim formatima tokom izvršavanja programa
```
#### Lista varijabli

```
● typedef list<Variable*> Variables, imenuje Variableskao
listu labela
● Variables g_variables, pravi globalnu listu varijablikoju
ćemo koristiti tokom celog zadatka
● Label* getVariable(string), dobavlja varijablu izg_variables
sa prosleđenim imenom, u slučaju da ne uspe, javlja grešku
● bool variableExists(name, Variables), proverava dali
labela (prosleđeno njeno ime) postoji u prosleđenoj listi
```

### Instruction (Instruction.h)

Fajl _Instruction.h_ definiše klasu Instruction kaoi propratne promenljive i metode koje
se odnose na labele programa. Neki značajniji delovi fajla _Instruction.h_ :

#### Klasa Instruction

```
● odgovarajući konstruktori klase Instruction, get i set metode njenih polja
● InstructionType m_type , označava tip instrukcije
● string m_name , označava ime varijable
● int m_position , označava mesto varijable u globalnojlisti varijabli
● Variables m_dst, m_src , liste unutar svake pojedinačneinstrukcije
koje popunjavamo tokom pravljenja instrukcije unutar sintaksne
analize
● Variables m_use, m_in, m_out, Instructions m_succ ,liste unutar svake
pojedinačne instrukcije koje koristimo tokom analize životnog veka
● Regs m_assignment , označava registar kome je varijabladodeljena
● Label* m_label , koje se koristi ukoliko instrukcijakoristi ili kreira labelu
● implementacije raznih string toString() metoda kojese kasnije koriste
zarad ispisa intrukcije u različitim formatima tokom izvršavanja programa
```

#### Lista instrukcija

```
● typedef list<Instruction*> Instructions,
imenuje Instructions kao listu instrukcija
● Instructions g_instructions, pravi globalnu listuvarijabli
koju ćemo koristiti tokom celog zadatka
● Instruction* getLabelPosition(Label*), vrati instrukcijuiz
globale liste instrkcija u kojoj je definisana prosledjena labela, u slučaju da
ne uspe, javlja grešku
● bool variableExists(name, Variables), proverava dali
labela (prosleđeno njeno ime) postoji u prosleđenoj listi
```
## Sintaksna analiza (SyntaxAnalysis.h)

Ako ulaz u vidu _.mavn_ fajla leksički ispravan posleleksičke analize bi trebalo da
posedujemo listu tokena kojom u ovom slučaju dalje procesuiramo u _SyntaxAnalysis.h_. U
ovoj verziji on pored standardne sintaksne analize obavlja i prepoznavanje šablona liste
tokena, ispis u mips32 formatu, kao i javljanje grešaka
Novoimplemetnirani tipovi instrukcija suI_NOP,I_BLT,ADD_U.


#### Standardna sintaksna analiza i izbor instrukcija:

Redom prolazimo kroz listu tokena metodomvoid eat(TokenType).Token koji
trenutno analiziramo skladištimo u promenljivoj _TokencurrentToken_. Kako prolazimo
kroz listu tokena i prepoznajemo određene šablone, tako pravimo nove varijable, labele i
instrukcije i ubacujemo ih u odgovarajuće globalne liste:Variables g_variables,
Labels g_labels,Instructions g_instructions.

#### Ispis u fajl u mips32 formatu:

U _SyntaxAnalysis.h_ se takođe nalaze i potrebne metodeza upis rezultata u fajl u _mips_
formatu. One se obimno služe pozivima raznih metoda za ispis definisanih u _Label.h_ ,
_Variables.h_ i _Instruction.h_

#### Javljanje grešaka:

U slučaju da se pojavio neočekivan redosled tokena u listi tokena iliti da je _void
eat(TokenType)_ metodi prosleđen neočekivan TokenType,metoda javlja grešku.

U slučaju da je prepoznata i kreirana nova varijabla ili labela sa već postojećim imenom,
program javlja grešku odmah nakon kreiranja te sporne varijable ili labele, a pre dodavanja
spornog sadržaja u odgovarajuću listu.
U slučaju da instrukcija sadrži prethodno nedeklarisanu labelu ili varijablu javiće se greška.


### Analiza životnog veka promenljive (LivenessAnalysis.h)

Vršenje analize životnog veka promenljive metodom _void livenessAnalysis()_ jeste
adekvatno dodavanje u varijabli u liste _Variablesm_in_ i _Variables m_out_ svake instrukcije u
listi _g_instructions_.
● Metoda _fillSucc()_ popunjava listu m_succ svake pojedinačneinstrukcije iz
liste globalnih instrukcija
● Metoda _fillUse()_ popunjava listu m_use svake pojedinačneinstrukcije iz
liste globalnih instrukcija
● Metoda _printLiveness()_ ispisuje instrukcije iz globalneliste instrukcija na
konzolu, kao i njihova relevatna polja tokom analize životnog veka
● Metoda _livenessAnalysis()_ vrši analizu životnog vekapromenljive po već
uspostavljenom algoritmu. Ona poziva _fillSucc()_ i _fillUse()_ pre iizvršavanja
algoritma, i _printLiveness()_ posle svake iteracije

### Alokacija resursa (ResourceAnalysis.h)

Prolazeći kroz listu instrukcija, tj. gledajući _m_dst_ (varijable koje instrukcija definiše) i
_m_out_ formiramo graf interferencije između registarskihvarijabli. Pomoću formiranog grafa
kreiramo i punimo stek varijabli. Broj boja _K_ je jednakbroju dostupnih registara, u našem
slučaju 4.
Biramo čvor sa najvećim brojem interferencija, a da je taj broj manji od _K_. Izabrani čvor
uklanjamo sa grafika, i prebacujemo u stek. Potrebno je izvršavati ovaj algoritam dok
svi
čvorovi grafa nisu obrađeni. U slučaju da u bilo kom momentu ne postoji čvor kome je broj
interferencija manji od _K_ , memoriju je nemoguće dodeliti( _spill)_. U ovom ograničenom
okviru zadatka program će javiti grešku, memorija neće biti alocirana, samim tim _output_ fajl
neće biti ispunjen.
Graf interferencije treba ponovo napuniti analogno prvom putu, i zatim na osnovuu njega,
steka, i broja registara, alocirati svakoj promenljivoj registar i ukloniti je sa steka. Detaljnije o
ovom procesu u funkciji
bool doResourceAllocation(InterferenceGraph&, stack<Variable*>&)
Nakon uspešne alokacije memorije ispis će u _mips32_ formatu biti upisan u fajl.




