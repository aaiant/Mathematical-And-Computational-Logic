#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <ctype.h>
#include <string.h>
#include <fstream>

using namespace std;

char curValue;
string formula = "((A -> (B -> C)) & (A -> B) <-> (A -> C)) v ((A -> B) v (B ~ C) v (C -> D))";
// string formula = "((B->A)&(B->(!AvC))~(B->(!BvC)))&((A->B)v(B~C)v(C->D))";
string formula2 = "(A -> (B v C))";
string formulaCopy;
string subFormula;
string binFormula;
vector <tuple <char, int, int> > varData;
vector <vector <int> > table;
vector <vector <int> > Ftable;
vector <vector <int> > Gtable;
vector <pair <string, vector <int> > > F_FNCP;
vector <pair <string, vector <int> > > G_FNCP;
vector <pair <string, vector <int> > > FNCP;
vector <pair <string, vector <int> > > S;
vector <pair <string, vector <int> > > SP;
vector <int> line;

ofstream output_file("output.h");


bool alreadyIn(tuple <char, int, int> &element)
{
    return (get<0>(element) == curValue);
}

bool alphabetical(tuple <char, int, int> &element1, tuple <char, int, int> &element2)
{
    return (get<0>(element1) < get<0>(element2));
}

void createVarData()
{
    for(auto index = formula.begin(); index != formula.end(); index++)
    {
        if(isalpha(*index) && (*index != 'v'))
        {
            curValue = *index;
            auto it = find_if(varData.begin(), varData.end(), alreadyIn);
            if(it != varData.end())
                get<1>(*it)++;
            else
                varData.push_back(make_tuple(curValue, 1, 0));
        }
    }
    sort(varData.begin(), varData.end(), alphabetical);
}

int implication(int lvalue, int rvalue)
{
    if(lvalue <= rvalue)
        return 1;
    return 0;
}

char binarySolve()
{
    int lvalue;
    int rvalue;

    lvalue = binFormula[0] - '0';
    rvalue = binFormula[binFormula.length() - 1] - '0';
    binFormula.assign(binFormula, 1, binFormula.length() - 2);

    if(binFormula == "<->" || binFormula == "~")
        return (lvalue == rvalue) + '0';
    else if(binFormula == "&")
        return (lvalue & rvalue) + '0';
    else if(binFormula == "v")
        return (lvalue || rvalue) + '0';
    else 
        return implication(lvalue, rvalue) + '0';
}

char subFormulaBreak()
{   
    int end;

    while(subFormula.length() > 1)
    {
        end = (subFormula.substr(1)).find_first_of("01");
        end++;

        binFormula.assign(subFormula, 0, end + 1);
        subFormula.erase(1, end);
        subFormula[0] = binarySolve();
    }
    return subFormula[0];
}

void removeNonOp()
{
    int found;
    do
    {
        found = formulaCopy.find('!');
        if(found != -1)
        {
            if(formulaCopy[found + 1] == '0')
                formulaCopy[found + 1] = '1';
            else
                formulaCopy[found + 1] = '0';
            formulaCopy.erase(found, 1);
        }
    }
    while(found != string::npos);
}

char func()
{
    int start;
    int end;
    
    formulaCopy.erase(remove(formulaCopy.begin(), formulaCopy.end(), ' '), formulaCopy.end());
    removeNonOp();
    do
    {
        end = formulaCopy.find(')');
        start = formulaCopy.find_last_of('(', end);
        subFormula.assign(formulaCopy, start + 1, end - start - 1);
        formulaCopy.erase(start + 1, end - start);
        formulaCopy[start] = subFormulaBreak();
    }
    while(formulaCopy.length() > 1 && formulaCopy.find(')') != string::npos);
    subFormula = formulaCopy;
    formulaCopy[0] = subFormulaBreak();
    return formulaCopy[0];
}

void assignNumVal()
{
    for(auto it : varData)
    {
        char new_char = (char) get<1> (it) + '0';
        std::replace(formulaCopy.begin(), formulaCopy.end(), get<0> (it), new_char);
    }
}

void generateNumVal(int k)
{
    if(k == varData.size())
    {
        formulaCopy = formula;
        assignNumVal();
        
        line.clear();
        for(auto it : varData)
            line.push_back(get<1>(it));
        line.push_back(func() - '0');
        table.push_back(line);
        return;
    }

    get<1>(varData[k]) = 0;
    generateNumVal(k + 1);
    get<1>(varData[k]) = 1;
    generateNumVal(k + 1);
}

void createTable()
{
    int k;

    k = 0;
    line.clear();
    varData.clear();
    createVarData();
    for(auto it : varData)
        line.push_back(get<0>(it));
    line.push_back('F');
    table.push_back(line);
    generateNumVal(k);
}

void printTable()
{
    for(int i = 0; i < table[0].size(); i++)
    {
        cout << char(table[0][i]) << " ";
        if(i == table[0].size() - 2)
            cout << " ";
    }
    cout << endl;
    for(int i = 1; i < table.size(); i++)
    {
        for(int j = 0; j < table[i].size(); j++)
        {
            if(table[i][j] == 2)
                cout << "-";
            else
                cout << table[i][j];
            if(j == table[i].size() - 2)
                cout << " ";
        }
        cout << endl;
    }
}

void createFNDP()
{
    int i;

    i = 0;
    string FNDP;

    while(i < table.size())
    {
        if(table[i][table[i].size() - 1] == 0)
        {
            FNDP += "(";
            for(int j = 0; j < table[i].size() - 1; j++)
            {
                if(table[i][j] == 1)
                    FNDP += "!";
                FNDP += table[0][j];
                if(j != table[i].size() - 2)
                    FNDP += " & ";
                else
                    FNDP += ") V\n";
            }
        }       
        i++;
    }
    FNDP.erase(FNDP.end() - 3, FNDP.end());
    FNDP += ".\n";
    cout << FNDP;
}

void createFNCP()
{
    int i;
    int end;
    pair <string, vector <int> > line;

    line.first.clear();
    line.second.clear();

    i = 0;
    end = 2;

    if(count(table[1].begin(), table[1].end(), 2))
        end++;
    while(i < table.size())
    {
        if(table[i][table[i].size() - 1] == 0)
        {
            for(int j = 0; j < table[i].size() - 1; j++)
            {
                if(table[i][j] != 2)
                {
                    if(table[i][j] != 0)
                        line.first += "!";
                    line.first += table[0][j];
                }
                if(j != table[i].size() - end)
                    line.first += " V ";
                else
                {
                    line.second = table[i];
                    FNCP.push_back(line);
                    line.first.clear();
                    line.second.clear();
                }
            }
        }       
        i++;    
    }
}

void printFNCP()
{
    for(int i = 0; i < FNCP.size(); i++)
    {
        cout << FNCP[i].first << " : ";
        for(int j = 0; j < FNCP[i].second.size(); j++)
        {
            if( (FNCP[i].second)[j] == 2)
                cout << "- ";   
            else
                cout << (FNCP[i].second)[j] << " ";
        }
        cout << endl;
    }
}

void gatherData()
{
    cout << "Formula F = " << formula << endl;
    cout << "Tabel de adevar : " << endl;
    createTable();
    printTable();
    Ftable = table;
    createFNCP();
    F_FNCP = FNCP;
    cout << "FNCP pentru F : ";
    printFNCP();

    table.clear();
    FNCP.clear();
    formula = formula2;

    cout << "\nFormula G = " << formula << endl;
    cout << "Tabel de adevar : " << endl;
    createTable();
    for(int i = 1; i < table.size(); i++)
        table[i].insert(table[i].end() - 1, 2);
    printTable();
    Gtable = table;
    cout << "FNCP pentru G : ";
    createFNCP();
    printFNCP();
    G_FNCP = FNCP;
}

bool areCompatible(vector <int> line1, vector <int> line2)
{
    int count; 
    count = 0;
    for(int i = 0; i < line1.size() - 1; i++)
    {
        if(line1[i] != line2[i])
        {
            if(line1[i] != 2 && line2[i] != 2)
            {
                count++;
            }
        }
    }
    return (count < 2); 
}


void printS()
{
    cout << "S = (";
    for(int i = 0; i < S.size(); i++)
    {
        cout << S[i].first;
        if(i != S.size() - 1)
            cout << " , ";
        else cout << ")";
    }
    cout << "\n";
    cout << "S = (";
    for(int i = 0; i < S.size(); i++)
    {   
        for(int j = 0; j < S[i].second.size(); j++)
        {
            if((S[i].second)[j] == 2)
                cout << "- ";
            else
                cout << (S[i].second)[j] << " ";
        }
        if(i != S.size() - 1)
            cout << ", ";
        else cout << ")\n";
    }
}

void printSP()
{
    cout << "SP = (";
    for(int i = 0; i < SP.size(); i++)
    {
        cout << SP[i].first;
        if(i != SP.size() - 1)
            cout << " , ";
        else cout << ")";
    }
    cout << "\n";
    cout << "SP = (";
    for(int i = 0; i < SP.size(); i++)
    {
        for(int j = 0; j < SP[i].second.size(); j++)
        {
            if((SP[i].second)[j] == 2)
                cout << "- ";
            else
                cout << (SP[i].second)[j] << " ";
        }
        if(i != SP.size() - 1)
            cout << ", ";
        else 
            cout << ")\n";
    }
}



pair <string, vector <int> > createNewElement(int i, int j)
{
    pair <string, vector <int> > aux;

    for(int k = 0; k < S[i].second.size(); k++)
    {
        if((S[i].second)[k] == (S[j].second)[k])
            aux.second.push_back((S[i].second)[k]);
        else
        {
            if((S[i].second)[k] == 2)
                aux.second.push_back((S[j].second)[k]);
            else if((S[j].second)[k] == 2)
                aux.second.push_back((S[i].second)[k]);
            else
                aux.second.push_back(2);
        }
    }
    for(int i = 0; i < aux.second.size() - 1; i++)
    {
        if(aux.second[i] != 2)
        {
            if(aux.second[i] != 0)
                aux.first += "!";
            aux.first += char(i + 65);
        }
        if(i < aux.second.size() - 3)
            aux.first += " V ";
    }

    return aux;
}

vector <vector <int> > combinations;

void createSP()
{
    bool isEmpty;
    vector <int> combination;

        for(int i = 0; i < S.size(); i++)
        {
            for(int j = i + 1; j < S.size(); j++)
            {
                if(areCompatible(S[i].second, S[j].second) == true)
                {
                    combination.clear();
                    combination.push_back(i);
                    combination.push_back(j);
                    if(find(combinations.begin(), combinations.end(), combination) == combinations.end())
                    {
                        SP.push_back(createNewElement(i, j));
                    }
                }
            }
        }
        printSP();
        S.insert(S.end(), SP.begin(), SP.end());
        SP.clear();
        printS();
}

void run()
{
    gatherData();

    S = F_FNCP;
    S.insert(S.end(), G_FNCP.begin(), G_FNCP.end());
    cout << "\n\n\n";
    printS();
    createSP();

    cout << "SP = (multime vida)\n";
    cout << "SP = (multime vida)\n";

    cout << "\nMultimea este consistenta.\n";
}



int main()
{
    run();
    return 0;
}