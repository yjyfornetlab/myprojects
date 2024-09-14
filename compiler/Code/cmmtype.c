#include <stdio.h>
#include "cmmtype.h"

void printBasic(Type bas)
{
    if (bas->kind == BASIC)
    {
        if (bas->u.basic == 0)
        {
            printf("int");
        }
        else if (bas->u.basic == 1)
        {
            printf("float");
        }
    }
}

void printArray(Type arr)
{
    if (arr->kind == ARRAY)
    {
        Type temp = arr;
        while (temp->kind == ARRAY)
        {
            printf("array(%d,", temp->u.array.size);
            temp = temp->u.array.elem;
        }
        if (temp->kind == BASIC)
        {
            printBasic(temp);
        }
        if (temp->kind == STRUCTURE)
        {
            printStruct(temp);
        }
        printf(")\n");
    }
}

void printStruct(Type stru)
{
    if (stru->kind == STRUCTURE)
    {
        printf("struct{");
        FieldList temp = stru->u.structure;
        while (temp != NULL)
        {
            printf("%s:", temp->name);
            if (temp->type->kind == BASIC)
            {
                printBasic(temp->type);
            }
            if (temp->type->kind == ARRAY)
            {
                printArray(temp->type);
            }
            if (temp->type->kind == STRUCTURE)
            {
                printStruct(temp->type);
            }
            printf(";");
            temp = temp->tail;
        }
        printf("}\n");
    }
}

void printType(Type ty)
{
    if(ty==NULL)
    {
        printf("null");
        return;
    }
    if (ty->kind == BASIC)
    {
        printBasic(ty);
    }
    if (ty->kind == ARRAY)
    {
        printArray(ty);
    }
    if (ty->kind == STRUCTURE)
    {
        printStruct(ty);
    }
}

int matchType(Type ty1, Type ty2)
{
    if(ty1==NULL || ty2==NULL)
    {
        return 1;
    }

    if (ty1->kind != ty2->kind)
    {
        return 0;
    }
    if (ty1->kind == BASIC && ty1->u.basic != ty2->u.basic)
    {
        return 0;
    }
    if (ty1->kind == ARRAY)
    {
        return matchType(ty1->u.array.elem, ty2->u.array.elem);
    }
    if (ty1->kind == STRUCTURE)
    {
        FieldList temp1 = ty1->u.structure;
        FieldList temp2 = ty2->u.structure;
        while (temp1 != NULL && temp2 != NULL)
        {
            if (matchType(temp1->type, temp2->type))
            {
                temp1 = temp1->tail;
                temp2 = temp2->tail;
            }
            else
            {
                return 0;
            }
        }
        if ((temp1 == NULL && temp2 != NULL) || (temp1 != NULL && temp2 == NULL))
        {
            return 0;
        }
    }
    return 1;
}