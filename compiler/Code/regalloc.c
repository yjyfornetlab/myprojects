#include <stdlib.h>
#include <string.h>
#include "regalloc.h"
#include "objgen.h"

int already_used(int ind)
{
    for (int i = 0; i < alloced_num; i++)
    {
        if (ind == instr_alloced[i])
        {
            return 1;
        }
    }
    return 0;
}

int get_freereg()
{
    for (int i = 0; i < t_regs; i++)
    {
        if (var_regs[i].alloced != 1 && already_used(i) != 1 && !var_regs[i].has_arg)
        {
            instr_alloced[alloced_num++] = i;
            return i;
        }
    }

    return -1;
}

char *allocate(char *irvar, int is_arg)
{
    if (irvar == NULL)
    {
        int temp_ind = get_freereg();
        return var_regs[temp_ind].rname;
    }
    if (strcmp(irvar, "#0") == 0)
    {
        return zero_reg;
    }

    SymNode *objNode = searchTable(objSymTable, irvar, Undef);
    if (objNode != NULL)
    {
        int rindex = objNode->val.irvarinfo.reg_index;
        if (objNode->val.irvarinfo.is_param)
        {
            return arg_regs[rindex].rname;
        }
        if (objNode->val.irvarinfo.inreg)
        {
            return var_regs[rindex].rname;
        }
    }
    int ind = get_freereg();

    if (objNode == NULL)
    {
        objNode = (SymNode *)malloc(sizeof(SymNode));
        objNode->sname = irvar;
        insertSymNode(objSymTable, objNode);
        objNode->val.irvarinfo.inreg = 1;
        objNode->val.irvarinfo.instk = 0;
        objNode->val.irvarinfo.reg_index = ind;
        var_regs[ind].alloced = 1;
    }

    var_regs[ind].has_arg = is_arg;
    return var_regs[ind].rname;
}

char *ensure(char *irvar, int is_arg)
{
    if (strcmp(irvar, "#0") == 0)
    {
        return zero_reg;
    }
    SymNode *objNode = searchTable(objSymTable, irvar, Undef);
    if (!objNode->val.irvarinfo.instk)
    {
        int rindex = objNode->val.irvarinfo.reg_index;
        if (objNode->val.irvarinfo.is_param)
        {
            return arg_regs[rindex].rname;
        }
        instr_alloced[alloced_num++] = rindex;
        var_regs[rindex].alloced = 0;
        objNode->val.irvarinfo.inreg = 0;

        var_regs[rindex].has_arg = is_arg;
        return var_regs[rindex].rname;
    }
    else
    {
        char *alloc_reg = allocate(irvar, is_arg);
        load_reg(alloc_reg, objNode->val.irvarinfo.offset, fra_p);
        return alloc_reg;
    }
}

void get_reg(char **irtoks, char **regx, char **regy, char **regz)
{
    alloced_num = 0;
    if (strcmp(irtoks[0], "READ") == 0)
    {
        *regx = allocate(irtoks[1], 0);
    }
    else if (strcmp(irtoks[0], "WRITE") == 0)
    {
        *regx = ensure(irtoks[1], 0);
    }
    else if (strcmp(irtoks[0], "ARG") == 0)
    {
        *regx = ensure(irtoks[1], 1);
    }
    else if (strcmp(irtoks[0], "IF") == 0)
    {
        *regx = ensure(irtoks[1], 0);
        *regy = ensure(irtoks[3], 0);
    }
    else if (strcmp(irtoks[0], "RETURN") == 0)
    {
        *regx = ensure(irtoks[1], 0);
    }
    else
    {
        if (ir_tok_num <= 3)
        {
            if (irtoks[0][0] == '*')
            {
                char *arr_var = suffix_str(irtoks[0]);
                *regx = ensure(arr_var, 0);
                *regy = ensure(irtoks[2], 0);
                free(arr_var);
            }
            else if (irtoks[2][0] == '*')
            {
                char *arr_var = suffix_str(irtoks[2]);
                *regx = allocate(irtoks[0], 0);
                *regy = ensure(arr_var, 0);
                free(arr_var);
            }
            else if (irtoks[2][0] == '#')
            {
                *regx = allocate(irtoks[0], 0);
            }
            else
            {
                *regx = allocate(irtoks[0], 0);
                *regy = ensure(irtoks[2], 0);
            }
        }
        else
        {
            if (strcmp(irtoks[2], "CALL") == 0)
            {
                *regx = allocate(irtoks[0], 0);
            }
            else if (irtoks[2][0] == '&')
            {
                *regx = allocate(irtoks[0], 0);
                char *arr_var = suffix_str(irtoks[2]);
                *regy = allocate(arr_var, 0);
                *regz = ensure(irtoks[4], 0);
                SymNode *objNode = searchTable(objSymTable, arr_var, Undef);
                char a_off[32];
                sprintf(a_off, "%d", objNode->val.irvarinfo.offset);
                addcode(addi_instr(*regy, fra_p, a_off));
                free(arr_var);
            }
            else
            {
                *regx = allocate(irtoks[0], 0);
                if (strcmp(irtoks[3], "*") == 0 && irtoks[4][0] == '#')
                {
                    *regz = allocate(NULL, 0);
                    *regy = ensure(irtoks[2], 0);
                    char *const_num = suffix_str(irtoks[4]);
                    addcode(li_instr(*regz, const_num));
                    free(const_num);
                }
                else
                {
                    *regy = ensure(irtoks[2], 0);
                    if (irtoks[4][0] != '#')
                    {
                        *regz = ensure(irtoks[4], 0);
                    }
                }
            }
        }
    }
}