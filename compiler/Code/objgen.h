#include <stdio.h>
#ifndef _OBJGEN_H
#define _OBJGEN_H

int OBJ_MAX_LEN;
int obj_len;
char** objcodes;
int ir_tok_num;
FILE* objout;
int var_off;
int func_num;

void generate(char*);
void generate_objcode(char*,int);
void addcode(char*);
char* lw_instr(char*,char*);
char* sw_instr(char*,char*);
char* addr_from_reg(char*,int);
void move_sp(int);
void store_reg(char*,int,char*);
void load_reg(char*,int,char*);
char* suffix_str(char*);
char* addi_instr(char*,char*,char*);
char* li_instr(char*,char*);

#endif