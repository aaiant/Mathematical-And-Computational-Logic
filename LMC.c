#include <stdio.h>
#include <string.h>

struct frequency
{
    char    value;
    char    numValue;
    int     frequency; 
};


void update(struct frequency varStatus[], char value)
{
    int i = 0;

    while(varStatus[i].value != '\0')
    {
        if(varStatus[i].value == value)
        {
            varStatus[i].frequency++;
            return;
        }
        i++;
    }
    varStatus[i].value = value;
    varStatus[i].frequency++;
}

void sortVariables(struct frequency var[10], int n)
{
    struct frequency aux;
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = i; j < n; j++)
        {
            if(var[i].value> var[j].value)
            {
                aux = var[i];
                var[i] = var[j];
                var[j] = aux;
            }
        }
    }

}




int main()
{
    int i;
    int disVar;
    int operatorCount;
    int subformulasCount;
    struct frequency varStatus[10] = {0};
    
    char formula[] = "(B -> (B -> C)) & (A -> B) <-> (A -> C)\0";
    char operators[] = "-!v&~\0";

    operatorCount = 0;
    subformulasCount = 1;
    for(i = 0; i < strlen(formula); i++)
    {
        if(strchr(operators, formula[i]) == 0 && strchr("()<> ", formula[i]) == 0)
        {
            update(varStatus, formula[i]);
            subformulasCount++;
        }
        else if(strchr(operators, formula[i]) != 0)
            operatorCount++;
        else if(strchr("(&<", formula[i]) != 0)
            subformulasCount++;
    }

    i = 0;
    while(varStatus[i].value != '\0')
    {
        printf("%c : %d\n", varStatus[i].value, varStatus[i].frequency);
        i++;
    }
    sortVariables(varStatus, i);
    printf("i = %d\n", i);
        i = 0;
    while(varStatus[i].value != '\0')
    {
        printf("%c : %d\n", varStatus[i].value, varStatus[i].frequency);
        i++;
    }
    printf("Formula order : %d\n", operatorCount);
    printf("Subformulas : %d\n", subformulasCount);
    return 0;
}