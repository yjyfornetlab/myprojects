#include <stdio.h>
#include "tree.h"

#ifndef _TRANSLATE_H
#define _TRANSLATE_H

typedef struct ArgList
{
    char* aname;
    struct ArgList* next;
} ArgList;

int lno;
int tno;
int vno;
int vlen;
int tlen;
FILE* outfp;
char** irs;
int IR_MAX_LEN;
int ir_len;
// int func_num;

char* conc(char*,char*);
char* new_var();
void translate(char*,TreeNode*);
void transtoir(TreeNode*);
void translate_ExtDef(TreeNode*);
void translate_FunDec(TreeNode*);
void translate_Exp(TreeNode*,char*,int);
void translate_Cond(TreeNode*,char*,char*);
void translate_Stmt(TreeNode*);
void translate_CompSt(TreeNode*);
void translate_Args(TreeNode*,ArgList**);
void translate_Dec(TreeNode*);

#endif