#include "hashsymtable.h"
#ifndef _REGALLOC_H
#define _REGALLOC_H

typedef struct GenReg
{
    char* rname;
    SymNode* var_node;
    int alloced;
    int changed;
    int has_arg;
} GenReg;
char* stk_p;
char* fra_p;
char* ret_addr;
char* ret_reg;
char* zero_reg;
GenReg var_regs[18];
GenReg arg_regs[4];
int regs_num;
int instr_alloced[5];
int alloced_num;
int t_regs;
int s_regs;
int a_regs;

SymTable* objSymTable;

void get_reg(char**,char**,char**,char**);

#endif