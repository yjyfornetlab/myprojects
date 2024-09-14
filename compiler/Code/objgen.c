#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "regalloc.h"
#include "objgen.h"
#include "translate.h"

char *read_data = "_prompt: .asciiz \"Enter an integer:\"";
char *write_data = "_ret: .asciiz \"\\n\"";
char *pre_code = ".globl main\n.text";
char *read_func[10] = {"read:", "li $v0, 4", "la $a0, _prompt", "syscall", "li $v0, 5", "syscall", "jr $ra"};
char *write_func[10] = {"write:", "li $v0, 1", "syscall", "li $v0, 4", "la $a0, _ret", "syscall", "move $v0, $0", "jr $ra"};
int used_var[3000];
int used_spe[3000];
int max_vid = 0;
int used_vnum = 0;
int use_read = 0;
int use_write = 0;
int arg_num = 0;
int param_num = 0;
char *irarg_reg[500];
SymNode *instk_par[500];
int next_isparam = 1;
int in_main = 0;

int is_sign(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ':')
        {
            return 1;
        }
    }
    return 0;
}

char *new_func()
{
    func_num++;
    char numstr[32];
    sprintf(numstr, "%d", func_num);
    return conc("func", numstr);
}

char *add_func(char *func_name)
{
    SymNode *objNode = searchTable(objSymTable, func_name, Undef);
    if (objNode == NULL)
    {
        objNode = (SymNode *)malloc(sizeof(SymNode));
        objNode->sname = func_name;
        insertSymNode(objSymTable, objNode);
        if (strcmp(func_name, "main") == 0)
        {
            objNode->val.funcinfo.objName = "main";
        }
        else
        {
            objNode->val.funcinfo.objName = new_func();
        }
    }
    return objNode->val.funcinfo.objName;
}

char *suffix_str(char *s)
{
    char *suffixs = (char *)malloc(sizeof(char) * (strlen(s) + 5));
    strcpy(suffixs, s + 1);
    return suffixs;
}

char *addr_from_reg(char *regname, int offset)
{
    char off[32];
    sprintf(off, "%d", offset);
    char *str = (char *)malloc(sizeof(char) * (strlen(regname) + strlen(off) + 5));
    strcpy(str, off);
    strcat(str, "(");
    strcat(str, regname);
    strcat(str, ")");
    return str;
}

char *una_instr(char *instr_ty, char *s)
{
    char *str = (char *)malloc(sizeof(char) * (strlen(instr_ty) + strlen(s) + 5));
    strcpy(str, instr_ty);
    strcat(str, " ");
    strcat(str, s);
    return str;
}

char *bi_instr(char *instr_ty, char *s1, char *s2)
{
    char *str1 = una_instr(instr_ty, s1);
    char *str2 = (char *)malloc(sizeof(char) * (strlen(str1) + strlen(s2) + 5));
    strcpy(str2, str1);
    strcat(str2, ", ");
    strcat(str2, s2);
    free(str1);
    return str2;
}

char *tri_instr(char *instr_ty, char *s1, char *s2, char *s3)
{
    char *str1 = bi_instr(instr_ty, s1, s2);
    char *str2 = (char *)malloc(sizeof(char) * (strlen(str1) + strlen(s3) + 5));
    strcpy(str2, str1);
    strcat(str2, ", ");
    strcat(str2, s3);
    free(str1);
    return str2;
}

char *func(char *s)
{
    return conc(s, ":");
}

char *label_instr(char *s)
{
    return conc(s, ":");
}

char *li_instr(char *s1, char *s2)
{
    return bi_instr("li", s1, s2);
}

char *move_instr(char *s1, char *s2)
{
    if (strcmp(s1, s2) == 0)
    {
        return NULL;
    }
    return bi_instr("move", s1, s2);
}

char *addi_instr(char *s1, char *s2, char *s3)
{
    return tri_instr("addi", s1, s2, s3);
}

char *add_instr(char *s1, char *s2, char *s3)
{
    return tri_instr("add", s1, s2, s3);
}

char *sub_instr(char *s1, char *s2, char *s3)
{
    return tri_instr("sub", s1, s2, s3);
}

char *mul_instr(char *s1, char *s2, char *s3)
{
    return tri_instr("mul", s1, s2, s3);
}

char *div_instr(char *s1, char *s2)
{
    return bi_instr("div", s1, s2);
}

char *mflo_instr(char *s)
{
    return una_instr("mflo", s);
}

char *lw_instr(char *s1, char *s2)
{
    return bi_instr("lw", s1, s2);
}

char *sw_instr(char *s1, char *s2)
{
    return bi_instr("sw", s1, s2);
}

char *jump_instr(char *suffix, char *dest)
{
    char *str1 = conc("j", suffix);
    char *str2 = una_instr(str1, dest);
    free(str1);
    return str2;
}

char *branch_instr(char *suffix, char *s1, char *s2, char *dest)
{
    char *str1 = conc("b", suffix);
    char *str2 = tri_instr(str1, s1, s2, dest);
    free(str1);
    return str2;
}

void move_sp(int offset)
{
    char imm_s[32];
    sprintf(imm_s, "%d", offset);
    addcode(addi_instr(stk_p, stk_p, imm_s));
}

void load_reg(char *r, int offset, char *ptr)
{
    char *addr = addr_from_reg(ptr, offset);
    addcode(lw_instr(r, addr));
    free(addr);
}

void store_reg(char *r, int offset, char *ptr)
{
    char *addr = addr_from_reg(ptr, offset);
    addcode(sw_instr(r, addr));
    free(addr);
}

void addcode(char *obj_code)
{
    if (obj_code == NULL)
    {
        return;
    }
    if (obj_len == OBJ_MAX_LEN)
    {
        OBJ_MAX_LEN += 1000;
        char **temp = (char **)malloc(sizeof(char *) * OBJ_MAX_LEN);
        for (int i = 0; i < obj_len; i++)
        {
            temp[i] = objcodes[i];
        }
        free(objcodes);
        objcodes = temp;
    }
    objcodes[obj_len++] = obj_code;
}

void outputobj(char *str)
{
    if (str[strlen(str) - 1] != ':')
    {
        fprintf(objout, "    ");
    }
    fprintf(objout, "%s\n", str);
}

void change_stk_val(char *irvar, char *r)
{
    SymNode *objNode = searchTable(objSymTable, irvar, Undef);
    if (objNode != NULL && objNode->val.irvarinfo.instk)
    {
        store_reg(r, objNode->val.irvarinfo.offset, fra_p);
    }
}

void push_var(char *irvar)
{
    if (irvar == NULL)
    {
        return;
    }
    if (irvar[0] == 'v')
    {
        char *suffix_ind = suffix_str(irvar);
        int vind = atoi(suffix_ind);
        if ((!used_spe[vind]) && (!used_var[vind]))
        {
            used_var[vind] = 1;
            max_vid = (max_vid > vind) ? max_vid : vind;
        }
    }
}

void add_spe(char *irvar)
{
    char *suffix_ind = suffix_str(irvar);
    int vind = atoi(suffix_ind);
    used_spe[vind] = 1;
}

int preprocess_stmt(char *ir)
{
    char *temp = (char *)malloc(sizeof(char) * (strlen(ir) + 1));
    strcpy(temp, ir);
    char *tokens[10];
    ir_tok_num = 0;
    char *str = strtok(temp, " ");
    while (str != NULL)
    {
        tokens[ir_tok_num++] = str;
        str = strtok(NULL, " ");
    }
    if (strcmp(tokens[0], "FUNCTION") == 0)
    {
        return 0;
    }
    else if (strcmp(tokens[0], "ARG") == 0)
    {
        push_var(tokens[1]);
    }
    else if (strcmp(tokens[0], "PARAM") == 0)
    {
        add_spe(tokens[1]);
    }
    else if (strcmp(tokens[0], "READ") == 0 || strcmp(tokens[0], "WRITE") == 0)
    {
        push_var(tokens[1]);
    }
    else if (strcmp(tokens[0], "IF") == 0)
    {
        push_var(tokens[1]);
        push_var(tokens[3]);
    }
    else if (strcmp(tokens[0], "DEC") == 0)
    {
        add_spe(tokens[1]);
    }
    else if (strcmp(tokens[0], "RETURN") == 0)
    {
        push_var(tokens[1]);
    }
    else
    {
        if (ir_tok_num <= 3)
        {
            push_var(tokens[0]);
            push_var(tokens[2]);
        }
        else
        {
            push_var(tokens[0]);
            if (strcmp(tokens[2], "CALL") != 0)
            {
                push_var(tokens[2]);
                push_var(tokens[4]);
            }
        }
    }
    return 1;
}

void preprocess_func(int start)
{
    memset(used_var, 0, sizeof(used_var));
    memset(used_spe, 0, sizeof(used_spe));
    max_vid = 0;
    used_vnum = 0;
    for (int i = start + 1; i < ir_len; i++)
    {
        if (!preprocess_stmt(irs[i]))
        {
            break;
        }
    }
}

void generate_objcode(char *ir, int ir_ind)
{
    char *tokens[10];
    char *temp = (char *)malloc(sizeof(char) * (strlen(ir) + 1));
    strcpy(temp, ir);
    ir_tok_num = 0;
    char *str = strtok(temp, " ");
    while (str != NULL)
    {
        tokens[ir_tok_num++] = str;
        str = strtok(NULL, " ");
    }
    char *reg_x = NULL;
    char *reg_y = NULL;
    char *reg_z = NULL;
    if (strcmp(tokens[0], "FUNCTION") == 0)
    {
        preprocess_func(ir_ind);

        addcode(func(add_func(tokens[1])));
        in_main = 0;
        move_sp(-4);
        store_reg(fra_p, 0, stk_p);
        addcode(move_instr(fra_p, stk_p));
        for (int i = 0; i < t_regs; i++)
        {
            var_regs[i].alloced = 0;
        }
        param_num = 0;
        var_off = 0;

        for (int i = 0; i <= max_vid; i++)
        {
            if (used_var[i] && !(used_spe[i]))
            {
                char num_str[32];
                sprintf(num_str, "%d", i);
                char *varn = conc("v", num_str);
                SymNode *objNode = (SymNode *)malloc(sizeof(SymNode));
                objNode->sname = varn;
                insertSymNode(objSymTable, objNode);
                objNode->val.irvarinfo.is_param = 0;
                objNode->val.irvarinfo.instk = 1;
                var_off -= 4;
                objNode->val.irvarinfo.offset = var_off;
                used_vnum++;
            }
        }
        if (used_vnum != 0)
        {
            move_sp(-(used_vnum * 4));
        }
    }
    else if (strcmp(tokens[0], "READ") == 0)
    {
        use_read = 1;
        move_sp(-4);
        store_reg(ret_addr, 0, stk_p);
        addcode(jump_instr("al", "read"));
        load_reg(ret_addr, 0, stk_p);
        move_sp(4);
        get_reg(tokens, &reg_x, NULL, NULL);
        addcode(move_instr(reg_x, ret_reg));
        SymNode *objNode = searchTable(objSymTable, tokens[1], Undef);
        if (objNode == NULL)
        {
            return;
        }
        if (objNode->val.irvarinfo.instk)
        {
            store_reg(reg_x, objNode->val.irvarinfo.offset, fra_p);
        }
    }
    else if (strcmp(tokens[0], "WRITE") == 0)
    {
        use_write = 1;
        move_sp(-8);
        store_reg(arg_regs[0].rname, 4, stk_p);
        get_reg(tokens, &reg_x, NULL, NULL);
        addcode(move_instr(arg_regs[0].rname, reg_x));
        store_reg(ret_addr, 0, stk_p);
        addcode(jump_instr("al", "write"));
        load_reg(ret_addr, 0, stk_p);
        load_reg(arg_regs[0].rname, 4, stk_p);
        move_sp(8);
    }
    else if (strcmp(tokens[0], "ARG") == 0)
    {
        get_reg(tokens, &reg_x, NULL, NULL);
        irarg_reg[arg_num++] = reg_x;
    }
    else if (strcmp(tokens[0], "PARAM") == 0)
    {
        SymNode *objNode = searchTable(objSymTable, tokens[1], Undef);
        if (objNode == NULL)
        {
            objNode = (SymNode *)malloc(sizeof(SymNode));
            objNode->sname = tokens[1];
            insertSymNode(objSymTable, objNode);
        }
        if (param_num < a_regs)
        {
            objNode->val.irvarinfo.inreg = 1;
            objNode->val.irvarinfo.is_param = 1;
            objNode->val.irvarinfo.reg_index = param_num;
            objNode->val.irvarinfo.instk = 0;
        }
        else
        {
            objNode->val.irvarinfo.inreg = 0;
            objNode->val.irvarinfo.is_param = 1;
            objNode->val.irvarinfo.instk = 1;
            instk_par[param_num - a_regs] = objNode;
        }
        param_num++;

        if (param_num > a_regs && next_isparam != 1)
        {
            int instk_num = param_num - a_regs;
            for (int i = 0; i < instk_num; i++)
            {
                instk_par[i]->val.irvarinfo.offset = (instk_num + 1 - i) * 4;
            }
        }
    }
    else if (strcmp(tokens[0], "LABEL") == 0)
    {
        addcode(label_instr(tokens[1]));
    }
    else if (strcmp(tokens[0], "GOTO") == 0)
    {
        addcode(jump_instr("", tokens[1]));
    }
    else if (strcmp(tokens[0], "RETURN") == 0)
    {
        get_reg(tokens, &reg_x, NULL, NULL);
        addcode(move_instr(ret_reg, reg_x));
        if (var_off != 0)
        {
            move_sp(-var_off);
        }
        addcode(move_instr(stk_p, fra_p));
        load_reg(fra_p, 0, stk_p);
        move_sp(4);
        addcode(jump_instr("r", ret_addr));
    }
    else if (strcmp(tokens[0], "IF") == 0)
    {
        char *cond = NULL;
        char *relsym = tokens[2];
        if (strcmp(relsym, "==") == 0)
        {
            cond = "eq";
        }
        if (strcmp(relsym, "!=") == 0)
        {
            cond = "ne";
        }
        if (strcmp(relsym, ">") == 0)
        {
            cond = "gt";
        }
        if (strcmp(relsym, "<") == 0)
        {
            cond = "lt";
        }
        if (strcmp(relsym, ">=") == 0)
        {
            cond = "ge";
        }
        if (strcmp(relsym, "<=") == 0)
        {
            cond = "le";
        }
        get_reg(tokens, &reg_x, &reg_y, NULL);
        addcode(branch_instr(cond, reg_x, reg_y, tokens[5]));
    }
    else
    {
        if (ir_tok_num <= 3)
        {
            if (strcmp(tokens[0], "DEC") == 0)
            {
                SymNode *objNode = (SymNode *)malloc(sizeof(SymNode));
                objNode->sname = tokens[1];
                insertSymNode(objSymTable, objNode);
                objNode->val.irvarinfo.inreg = 0;
                objNode->val.irvarinfo.instk = 1;
                objNode->val.irvarinfo.is_param = 0;
                int arr_off = atoi(tokens[2]);
                move_sp(-arr_off);
                var_off -= arr_off;
                objNode->val.irvarinfo.offset = var_off;
            }
            else if (tokens[0][0] == '*')
            {
                get_reg(tokens, &reg_x, &reg_y, NULL);
                store_reg(reg_y, 0, reg_x);
            }
            else if (tokens[2][0] == '*')
            {
                get_reg(tokens, &reg_x, &reg_y, NULL);
                load_reg(reg_x, 0, reg_y);
            }
            else
            {
                if (tokens[2][0] == '#')
                {
                    get_reg(tokens, &reg_x, NULL, NULL);
                    char *const_num = suffix_str(tokens[2]);
                    addcode(li_instr(reg_x, const_num));
                    free(const_num);
                }
                else
                {
                    get_reg(tokens, &reg_x, &reg_y, NULL);
                    addcode(move_instr(reg_x, reg_y));
                }
                change_stk_val(tokens[0], reg_x);
            }
        }
        else
        {
            if (strcmp(tokens[2], "CALL") == 0)
            {
                for (int i = 0; i < t_regs; i++)
                {
                    var_regs[i].has_arg = 0;
                }
                int treg_num = 0;
                char *regtosave[15];
                for (int i = 0; i < t_regs; i++)
                {
                    if (var_regs[i].alloced)
                    {
                        regtosave[treg_num++] = var_regs[i].rname;
                    }
                }
                if (treg_num)
                {
                    move_sp(-(treg_num * 4));
                    for (int i = 0; i < treg_num; i++)
                    {
                        store_reg(regtosave[i], (treg_num - 1 - i) * 4, stk_p);
                    }
                }

                int inreg_arg = 0;
                int inmem_arg = 0;
                if (arg_num > a_regs)
                {
                    inreg_arg = a_regs;
                    inmem_arg = arg_num - a_regs;
                }
                else
                {
                    inreg_arg = arg_num;
                }

                if (inreg_arg)
                {
                    for (int i = 0; i < a_regs; i++)
                    {
                        arg_regs[i].changed = 0;
                    }
                    move_sp(-(inreg_arg * 4));
                    for (int i = 0; i < inreg_arg; i++)
                    {
                        store_reg(arg_regs[i].rname, (inreg_arg - 1 - i) * 4, stk_p);
                    }
                    for (int i = 0; i < inreg_arg; i++)
                    {
                        int cflag = 0;
                        if (irarg_reg[(arg_num - 1) - i][1] == 'a')
                        {
                            char *argr_i = suffix_str(irarg_reg[(arg_num - 1) - i] + 1);
                            int r_ind = atoi(argr_i);
                            if (arg_regs[r_ind].changed)
                            {
                                load_reg(arg_regs[i].rname, (inreg_arg - 1 - r_ind) * 4, stk_p);
                                cflag = 1;
                            }
                        }
                        if (cflag != 1)
                        {
                            addcode(move_instr(arg_regs[i].rname, irarg_reg[(arg_num - 1) - i]));
                        }
                        arg_regs[i].changed = 1;
                    }
                }

                if (inmem_arg)
                {
                    move_sp(-(inmem_arg * 4));
                    for (int i = inmem_arg - 1; i >= 0; i--)
                    {
                        store_reg(irarg_reg[i], i * 4, stk_p);
                    }
                }

                move_sp(-4);
                store_reg(ret_addr, 0, stk_p);
                addcode(jump_instr("al", add_func(tokens[3])));
                if (tokens[0][0] != 'u')
                {
                    get_reg(tokens, &reg_x, NULL, NULL);
                    addcode(move_instr(reg_x, ret_reg));
                    change_stk_val(tokens[0], reg_x);
                }
                load_reg(ret_addr, 0, stk_p);
                move_sp(4);

                if (inmem_arg)
                {
                    for (int i = inmem_arg - 1; i >= 0; i--)
                    {
                        load_reg(irarg_reg[i], i * 4, stk_p);
                    }
                    move_sp(inmem_arg * 4);
                }

                if (inreg_arg)
                {
                    for (int i = 0; i < inreg_arg; i++)
                    {
                        load_reg(arg_regs[i].rname, (inreg_arg - 1 - i) * 4, stk_p);
                    }
                    move_sp(inreg_arg * 4);
                }

                if (treg_num)
                {
                    for (int i = 0; i < treg_num; i++)
                    {
                        load_reg(regtosave[i], (treg_num - 1 - i) * 4, stk_p);
                    }
                    move_sp(treg_num * 4);
                }
                arg_num = 0;
            }
            else
            {
                if (tokens[2][0] == '&')
                {
                    get_reg(tokens, &reg_x, &reg_y, &reg_z);
                    addcode(add_instr(reg_x, reg_y, reg_z));
                }
                else
                {
                    if (strcmp(tokens[3], "+") == 0)
                    {
                        if (tokens[4][0] == '#')
                        {
                            get_reg(tokens, &reg_x, &reg_y, NULL);
                            char *const_num = suffix_str(tokens[4]);
                            addcode(addi_instr(reg_x, reg_y, const_num));
                            free(const_num);
                        }
                        else
                        {
                            get_reg(tokens, &reg_x, &reg_y, &reg_z);
                            addcode(add_instr(reg_x, reg_y, reg_z));
                        }
                    }
                    if (strcmp(tokens[3], "-") == 0)
                    {
                        if (tokens[4][0] == '#')
                        {
                            char *const_num = suffix_str(tokens[4]);
                            char *neg_num = conc("-", const_num);
                            get_reg(tokens, &reg_x, &reg_y, NULL);
                            addcode(addi_instr(reg_x, reg_y, neg_num));
                            free(const_num);
                            free(neg_num);
                        }
                        else
                        {
                            get_reg(tokens, &reg_x, &reg_y, &reg_z);
                            addcode(sub_instr(reg_x, reg_y, reg_z));
                        }
                    }
                    if (strcmp(tokens[3], "*") == 0)
                    {
                        get_reg(tokens, &reg_x, &reg_y, &reg_z);
                        addcode(mul_instr(reg_x, reg_y, reg_z));
                    }
                    if (strcmp(tokens[3], "/") == 0)
                    {
                        get_reg(tokens, &reg_x, &reg_y, &reg_z);
                        addcode(div_instr(reg_y, reg_z));
                        addcode(mflo_instr(reg_x));
                    }
                }
                change_stk_val(tokens[0], reg_x);
            }
        }
    }

}

void generate(char *outfile)
{
    if (outfile != NULL)
    {
        objout = fopen(outfile, "w");

        if (!objout)
        {
            perror(outfile);
            return;
        }
    }
    else
    {
        objout = NULL;
    }

    OBJ_MAX_LEN = 1000;
    obj_len = 0;
    objcodes = (char **)malloc(sizeof(char *) * OBJ_MAX_LEN);
    func_num = 0;

    stk_p = "$sp";
    fra_p = "$fp";
    ret_addr = "$ra";
    ret_reg = "$v0";
    zero_reg = "$0";
    regs_num = 18;
    t_regs = 10;
    s_regs = 8;
    a_regs = 4;

    for (int i = 0; i < 18; i++)
    {
        char regno[5];
        if (i < t_regs)
        {
            sprintf(regno, "%d", i);
            var_regs[i].rname = conc("$t", regno);
            if (i < a_regs)
            {
                arg_regs[i].rname = conc("$a", regno);
                arg_regs[i].alloced = 0;
                arg_regs[i].changed = 0;
                arg_regs[i].has_arg = 0;
            }
        }
        else
        {
            sprintf(regno, "%d", i - 10);
            var_regs[i].rname = conc("$s", regno);
        }
        var_regs[i].alloced = 0;
        var_regs[i].changed = 0;
        var_regs[i].has_arg = 0;
    }
    objSymTable = newSymTable();

    for (int i = 0; i < ir_len; i++)
    {
        next_isparam = 1;
        if (i != ir_len - 1 && strcmp(irs[i + 1], "PARAM") != 0)
        {
            next_isparam = 0;
        }
        generate_objcode(irs[i], i);
    }

    if (objout == NULL)
    {
        objout = stdout;
    }

    fprintf(objout, "%s\n", ".data");
    if (use_read)
    {
        fprintf(objout, "%s\n", read_data);
    }
    if (use_write)
    {
        fprintf(objout, "%s\n", write_data);
    }
    fprintf(objout, "%s\n", pre_code);
    if (use_read)
    {
        for (int i = 0; i < 7; i++)
        {
            outputobj(read_func[i]);
        }
    }
    if (use_write)
    {
        for (int i = 0; i < 8; i++)
        {
            outputobj(write_func[i]);
        }
    }
    for (int i = 0; i < obj_len; i++)
    {
        outputobj(objcodes[i]);
    }
}