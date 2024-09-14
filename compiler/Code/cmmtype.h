#ifndef _CMMTYPE_H
#define _CMMTYPE_H

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

typedef struct Type_
{
    enum {BASIC,ARRAY,STRUCTURE} kind;
    union
    {
        int basic;
        struct {Type elem; int size;} array;
        FieldList structure;
    } u;
    int width;
} Type_;

typedef struct FieldList_
{
    char* name;
    Type type;
    FieldList tail;
} FieldList_;

void printBasic(Type);
void printArray(Type);
void printStruct(Type);
void printType(Type);
int matchType(Type,Type);

#endif
