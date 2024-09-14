#include "cmmtype.h"

#ifndef _HASHSYMTABLE_H
#define _HASHSYMTABLE_H

typedef struct ParamList
{
    struct SymNode* symnode;
    Type argty;
    struct ParamList* next;
} ParamList;

typedef enum {Variable,FuncDef,StructDef,Undef} SymKind;
typedef struct SymNode
{
    char *sname;
    struct SymNode* next;
    SymKind symKind;
    union
    {
        struct
        {
            int isparam;
            Type vartype;
            char* irname;
            enum {BasicVar,ArrVar,StructVar} varkind;
        } varinfo;
        struct
        {
            Type rettype;
            int argnum;
            struct ParamList* paramlist;
            char* objName;
        } funcinfo;
        struct
        {
            FieldList fieldLt;
        }struinfo;
        struct
        {
            int reg_index;
            int inreg;
            int offset;
            int is_param;
            int instk;
        } irvarinfo;
    }val;
} SymNode;

typedef struct SymTable
{
    SymNode** table;
} SymTable;

SymNode* readfunc;
SymNode* writefunc;
SymTable* newSymTable();
void insertSymNode(SymTable*,SymNode*);
SymNode* searchTable(SymTable*,char*,SymKind);
void clearTable(SymTable*);

#endif