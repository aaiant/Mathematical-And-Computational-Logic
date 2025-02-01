#include <iostream>
#include <string.h>

using namespace std;

void removeWhitespaces(char *str)
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

void removeNonOp(char *str)
{
    int j;
    char copy[60];

    j = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        if(str[i] == '!')
        {
            if(str[i + 1] == '0')
                copy[j++] = '1';
            else
                copy[j++] = '0';
            i++;
        }
        else
        {
            copy[j++] = str[i];
        }
    }
    copy[j] = 0;
    strcpy(str, copy);
}

int implicatie(int lvalue, int rvalue)
{
    if(lvalue <= rvalue)
        return 1;
    return 0;
}

char binarySolve(char *str)
{
    int lvalue;
    int rvalue;

    lvalue = str[0] - '0';
    rvalue = str[strlen(str) - 1] - '0';

    if(strchr(str, '<') || strchr(str, '~'))
        return (lvalue == rvalue) + '0';
    else if (strchr(str, '&'))
        return (lvalue & rvalue) + '0';
    else if (strchr(str, 'v'))
        return (lvalue || rvalue) + '0';
    else 
        return implicatie(lvalue, rvalue) + '0';
}

char formulaBreak(char *str)
{
    int start;
    int end;
    char subformula[60];

    do
    {
        start = 0;
        end = 1;
        while(!strchr("01", str[end]) && str[end] != 0)
            end++;
        strncpy(subformula, str, end - start + 1);
        subformula[end - start + 1] = 0;
        str[end] = binarySolve(subformula);

        for(int i = start; i < end; i++)
            str[i] = ' ';
        removeWhitespaces(str);
    } 
    while (strlen(str) > 1);
    return str[0];
}


void mainSolve(char *str)
{
    int start;
    int end;
    char subformula[60];

    if(strchr(str, '!'))
        removeNonOp(str);
    
    while(strlen(str) > 1)
    {
        start = 0;
        end = 0;
        while(str[end] != ')' && str[end] != 0)
        {
            if(str[end] == '(')
                start = end;
            end++;
        }
        if(!(start == 0 && end == strlen(str)))
        {
            strncpy(subformula, str + start + 1, end - start - 1);
            subformula[end - start - 1] = 0;
            str[end] = formulaBreak(subformula);

            for(int i = start; i < end; i++)
                str[i] = ' ';

            removeWhitespaces(str);
        }
        else
            formulaBreak(str); 
            
    cout << str << endl;
    }
}

int main()  
{
    char formula[] = "(((0->0) & (0->(!0 v 0))) ~ (0->(!0 v 0)))\0";
    char operators[] = "<>!v&~\0";
    char str[] = "0&0<->1\0";
    
    cout << formula << endl;
    mainSolve(formula);
    cout << endl;
    // cout << implicatie(1,1);
    // cout << implicatie(1,0);
    
    // cout << implicatie(0,1);
    // cout << implicatie(0,0);
    
}