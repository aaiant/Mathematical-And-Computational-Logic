#include <iostream>
#include <string.h>

using namespace std;

int xTable;
int yTable;
int varCount;
char formula[] = "((A -> (B -> C)) & (A -> B) <-> (A -> C)) v((A -> B) v (B ~ C) v (C -> D))";
char formulaCopy[200];
int truthTable[20][20];
struct frequency
{
    char value;
    char numValue;
    int frequency;
} varStatus[10];

void printIntro() /* Printeaza antetul programului. */
{
    cout << "Student : Antohi Andi-Ionel\n";
    cout << "Specializarea: Informatica, anul 1, grupa 1\n";
    cout << "Disciplina: Logica matematica si computationala\n";
    cout << "Lab : 01-02" << endl;
    cout << endl
         << char(34) << formula << char(34) << endl
         << endl;
    cout << "Date initiale:\n---\n";
    cout << "Formula = " << formula << endl
         << endl;
    cout << "unde :\n";
    cout << "\t->: implicatie\n\t& : conjunctie\n";
    cout << "\tv : disjunctie\n\t! : negatie\n";
    cout << "\t~ sau <-> : echivalenta\n---\n\n";
    cout << "Solutii:\n---\n";
}

void update(struct frequency varStatus[], char value) /*Daca o variabila nu a fost gasita in formula, o adauga in array. Daca a fost gasita*/
{                                                     /*anterior, ii incrementeaza frecventa*/
    int i = 0;

    while (varStatus[i].value != '\0')
    {
        if (varStatus[i].value == value)
        {
            varStatus[i].frequency++;
            return;
        }
        i++;
    }
    varStatus[i].value = value;
    varStatus[i].frequency++;
}

void sortVariables(struct frequency var[10], int n) /* Sorteaza variabilele gasite in formula in ordine lexicografica. */
{
    struct frequency aux;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i; j < n; j++)
        {
            if (var[i].value > var[j].value)
            {
                aux = var[i];
                var[i] = var[j];
                var[j] = aux;
            }
        }
    }
}

void printFormulaStatus() /*Calculeaza si printeaza orice este legat de formula, in afara de tabelul logic.*/
{
    int i;
    int disVar;
    int operatorCount;
    int subformulasCount;

    char operators[] = "-!v&~\0";
    operatorCount = 0;
    subformulasCount = 1;
    for (i = 0; i < strlen(formula); i++)
    {
        if (strchr(operators, formula[i]) == 0 && strchr("()<>~ ", formula[i]) == 0)
        {
            update(varStatus, formula[i]);
            subformulasCount++;
        }
        else if (strchr(operators, formula[i]) != 0)
            operatorCount++;
        else if (strchr("(&<~", formula[i]) != 0)
            subformulasCount++;
    }

    varCount = 0;
    while (varStatus[varCount].value != '\0')
        varCount++;
    sortVariables(varStatus, varCount);
    cout << "a. # subformule = " << subformulasCount << endl;
    cout << "b. # de varaiabile distincte = " << varCount << endl;
    cout << "c. aparitii variabile : ";
    for (i = 0; i < varCount - 1; i++)
        cout << varStatus[i].value << "-" << varStatus[i].frequency << ", ";
    cout << varStatus[i].value << "-" << varStatus[i].frequency << endl;
    cout << "d. ordinul formulei (# conectori logici) =  " << operatorCount << endl;
    cout << "\ne. Tabelul de adevar\n\n";
}

void removeWhitespaces() /* Functia care se foloseste strict pentru eliminarea spatiilor din char array-ul declarat global, cu numele*/
{                        /* formulaCopy. */
    int count;

    count = 0;
    for (int i = 0; formulaCopy[i]; i++)
        if (formulaCopy[i] != ' ')
        {
            formulaCopy[count] = formulaCopy[i]; /* Nu exista conflict intre cele doua functii cu numele removeWhitespaces(), fiindca am facut function overloading. */
            count++;
        }

    formulaCopy[count] = 0;
}

void removeWhitespaces(char *str) /* Functia care elimina spatiile dintr-un char array care NU este declarat global, ci transmis prin parametru.*/
{
    int count;

    count = 0;
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
        {
            str[count] = str[i];
            count++;
        }

    str[count] = 0;
}

void removeNonOp() /* Functia removeNonOp() cauta semnul "!" si modifica stringul corespunzator: daca gaseste un "!"*/
{                  /* si valoarea de dupa "!" este 0, sterge "!" si schimba valoarea in 1. In caz contrar, o schimba in 0.*/
    int j;
    char copy[200];

    j = 0;
    for (int i = 0; i < strlen(formulaCopy); i++)
    {
        if (formulaCopy[i] == '!')
        {
            if (formulaCopy[i + 1] == '0')
            {
                copy[j] = '1';
            }
            else
            {
                copy[j] = '0';
            }
            i++;
        }
        else
        {
            copy[j] = formulaCopy[i];
        }
        j++;
    }
    copy[j] = 0;
    strcpy(formulaCopy, copy);
}

int implication(int lvalue, int rvalue) /* Functia implication() returneaza rezultatul implicatiei a doua valori. */
{
    if (lvalue <= rvalue)
        return 1;
    return 0;
}

char binarySolve(char *str) /* Functia binarySolve() are ca parametru un string ce contine valoare la stanga, operator */
{                           /* binar si valoare la dreapta. Verifica ce valori si ce operator a primit si returneaza */
    int lvalue;             /* rezultatul corespunzator. */
    int rvalue;

    lvalue = str[0] - '0';
    rvalue = str[strlen(str) - 1] - '0';

    if (strchr(str, '<') || strchr(str, '~'))
        return (lvalue == rvalue) + '0';
    else if (strchr(str, '&'))
        return (lvalue & rvalue) + '0';
    else if (strchr(str, 'v'))
        return (lvalue || rvalue) + '0';
    else
        return implication(lvalue, rvalue) + '0';
}

char formulaBreak(char *str) /* Functia formulaBreak() "sparge" din nou ceea ce a primit de la mainSolve(), pentru a putea imparti*/
{                            /* formula in stringuri care contin valoare la stanga, operator binar si valoare la dreapta, pentru a trimite*/
    int start;               /* fiecare string la functia binarySolve(). Intreg stringul care a fost trimis va fi inlocuit doar de valoarea*/
    int end;                 /* returnata de binarySolve(). Functia se repeta pana cand nu mai exista stringuri si ramane doar rezultatul.*/
    char subformula[200];

    do
    {
        start = 0;
        end = 1;
        while (!strchr("01", str[end]) && str[end] != 0)
            end++;
        strncpy(subformula, str, end - start + 1);
        subformula[end - start + 1] = 0;
        str[end] = binarySolve(subformula);

        for (int i = start; i < end; i++)
            str[i] = ' ';
        removeWhitespaces(str);
    } while (strlen(str) > 1);
    return str[0];
}

void mainSolve() /* Functia mainSolve() in primul rand face "curat" in formula, modificand-o astfel incat sa nu mai */
{                /* existe opeartorul unar '!' si sterge spatiile. Apoi cauta parti din formula care sa nu contina*/
    int start;   /* paranteze inauntrul lor, iar aceste parti din formula le trimite mai departe la functia formulaBreak() */
    int end;     /* Functia se repeta pana cand ramane doar rezultatul intregii formule. */
    char subformula[200];

    if (strchr(formulaCopy, '!'))
        removeNonOp();
    while (strlen(formulaCopy) > 1)
    {
        start = 0;
        end = 0;
        while (formulaCopy[end] != ')' && formulaCopy[end] != 0)
        {
            if (formulaCopy[end] == '(')
                start = end;
            end++;
        }
        if (!(start == 0 && end == strlen(formulaCopy)))
        {
            strncpy(subformula, formulaCopy + start + 1, end - start - 1);
            subformula[end - start - 1] = 0;
            formulaCopy[end] = formulaBreak(subformula);

            for (int i = start; i < end; i++)
                formulaCopy[i] = ' ';

            removeWhitespaces();
        }
        else
            formulaBreak(formulaCopy);
    }
}

void inputValues() /* Preia valorile curente pe care le-a generat functia back() si inlocuieste parametrii din formula*/
{
    char *p;
    for (int i = 0; i < varCount; i++)
    {
        do
        {
            p = strchr(formulaCopy, varStatus[i].value);
            if (p != NULL)
                formulaCopy[p - formulaCopy] = varStatus[i].numValue + '0';
        } while (p != NULL);
    }
}

void back(int k, int n) /* De la backtrack; Genereaza toate array-urile binare de lungime n*/
{                       /* am folosit functia pentru a genera toate valorile posibile ale variabilelor*/
    if (k == n)
    {
        for (int i = 0; i < n; i++)
        {
            printf("%d\t", varStatus[i].numValue);
            truthTable[xTable][yTable] = varStatus[i].numValue;
            yTable++;
        }

        strcpy(formulaCopy, formula);
        inputValues();
        mainSolve();
        cout << formulaCopy << endl;
        truthTable[xTable][yTable] = formulaCopy[0] - '0';
        xTable++;
        yTable = 0;
        return;
    }

    varStatus[k].numValue = 0;
    back(k + 1, n);
    varStatus[k].numValue = 1;
    back(k + 1, n);
}

void printTable() /*Afiseaza antetul tabelului logic*/
{
    for (int i = 0; i < varCount; i++)
        cout << varStatus[i].value << "\t";
    cout << "F" << endl;
    for (int i = 0; i < 35; i++)
        cout << "-";
    cout << endl;
    // cout << "varcount = " << varCount << endl;
    back(0, varCount);
    for (int i = 0; i < 35; i++)
        cout << "-";
}

void startTable()
{
    int i;
    for (i = 0; i < varCount; i++)
        truthTable[0][i] = varStatus[i].value;
    truthTable[0][i] = 'F';
    xTable++;
}

void printTruthTable()
{
    for (int i = 0; i < xTable; i++)
    {
        for (int j = 0; j <= varCount; j++)
        {
            if (i == 0)
                cout << char(truthTable[i][j]) << " ";
            else
                cout << truthTable[i][j] << " ";
        }
        cout << endl;
    }
}
int main()
{
    // printIntro();
    printFormulaStatus();
    printTable();

    cout << "xTable = " << xTable << endl;
    cout << "yTable = " << yTable << endl;
    cout << "\n\n\n";
    startTable();
    cout << "truth table : " << endl;
    printTruthTable();
    return 0;
}