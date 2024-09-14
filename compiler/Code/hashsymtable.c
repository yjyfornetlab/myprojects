#include <stdlib.h>
#include <string.h>
#include "hashsymtable.h"
#define table_size 0x3fff
unsigned int hash_pjw(char* name)
{
    unsigned int val = 0,i;
    for(;*name;name++)
    {
        val = (val << 2)+*name;
        if(i = val & ~table_size) val = (val^(i>>12)) & table_size;
    }
    return val;
}

void clearTable(SymTable* symTable)
{
    for(int i = 0;i<table_size;i++)
    {
        symTable->table[i] = NULL;
    }
}

SymTable* newSymTable()
{
    SymTable* symTable = (SymTable*)malloc(sizeof(SymTable));
    symTable->table = (SymNode**)malloc(sizeof(SymNode*)*table_size);
    clearTable(symTable);
    return symTable;
}

void insertSymNode(SymTable* symTable,SymNode* symNode)
{
    if(symNode==NULL)
    {
        return;
    }
    int index = hash_pjw(symNode->sname);
    symNode->next = NULL;

    if(symTable->table[index]==NULL)
    {
        symTable->table[index] = symNode;
    }
    else
    {
        symNode->next = symTable->table[index];
        symTable->table[index] = symNode;
    }
}

SymNode* searchTable(SymTable* symTable,char* name,SymKind symk)
{
    if(symTable==NULL)
    {
        return NULL;
    }
    unsigned int index = hash_pjw(name);
    SymNode* curr = symTable->table[index];
    while(curr!=NULL)
    {
        if(strcmp(curr->sname,name)==0 && (symk==Undef || curr->symKind==symk))
        {
            break;
        }
        curr = curr->next;
    }
    return curr;
}