#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100
#define NAME_SIZE 50

typedef struct Symbol
{
    char name[NAME_SIZE];
    int value;
    struct Symbol *next;
} Symbol;

typedef struct Scope
{
    int level;
    Symbol *symbols[TABLE_SIZE];
    struct Scope *parent;
} Scope;

unsigned long hash(char str[])
{
    int index, sum = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        sum = sum + str[i];
        i++;
    }
    index = sum % TABLE_SIZE;
    return index;
}

Scope *create_scope(int level, Scope *parent) // done
{
    Scope *scope = (Scope *)malloc(sizeof(Scope));
    scope->level = level;
    scope->parent = parent;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        scope->symbols[i] = NULL;
    }
    return scope;
}

Scope *enter_scope(Scope *sptr) // done
{
    sptr = create_scope(sptr->level + 1, sptr);
    return sptr;
}

Scope *end(Scope *sptr)
{

    sptr = sptr->parent;
    return sptr;
}

Symbol *lookup_symbol(char name[], Scope *sptr)
{
    unsigned long index = hash(name);
    while (sptr != NULL)
    {
        Symbol *symbol = sptr->symbols[index];
        while (symbol != NULL)
        {
            if (strcmp(symbol->name, name) == 0)
            {
                return symbol;
            }
            symbol = symbol->next;
        }
        sptr = sptr->parent;
    }
    return NULL;
}

void print(char *name, Scope *sptr)
{
    Symbol *symbol = lookup_symbol(name, sptr);
    if (symbol == NULL)
    {
        printf("Variable %s not found\n", name);
    }
    else
    {
        printf("Variable %s has value %d\n", name, symbol->value);
    }
}

void assign(char name[], int value, Scope *sptr)
{
    unsigned long index = hash(name);
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    strcpy(symbol->name, name); // Store a copy of the name
    symbol->value = value;
    symbol->next = sptr->symbols[index];
    sptr->symbols[index] = symbol;
}

int main()
{
    // Initialize the symbol table
    Scope *current_scope = create_scope(0, NULL);
    FILE *file = fopen("input.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "begin") == 0)
        {
            current_scope = enter_scope(current_scope);
        }
        else if (strcmp(line, "end") == 0)
        {
            current_scope = end(current_scope);
        }
        else if (strncmp(line, "assign", 6) == 0)
        {
            char name[20];
            int value;
            if (sscanf(line, "assign %s %d", name, &value) == 2)
            {
                assign(name, value, current_scope);
            }
            else
            {
                printf("Invalid assign command: %s\n", line);
            }
        }
        else if (strncmp(line, "print", 5) == 0)
        {
            char name[20];
            if (sscanf(line + 5, "%s", name) == 1)
            {
                print(name, current_scope);
            }
            else
            {
                printf("Invalid print command: %s\n", line);
            }
        }
        else
        {
            printf("Invalid command: %s\n", line);
        }
    }
    fclose(file);
    return 0;
}