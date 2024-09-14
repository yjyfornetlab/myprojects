#include <stdlib.h>
#include <string.h>
#include "translate.h"
#include "semanalysis.h"

void free_ArgList(ArgList *arg_list)
{
    ArgList *curr = arg_list;
    while (curr != NULL)
    {
        ArgList *temp = curr->next;
        free(curr->aname);
        free(curr);
        curr = temp;
    }
    arg_list = NULL;
}

int is_cond(ToKind tkind)
{
    return tkind == RelK || tkind == NoK || tkind == AnK || tkind == OrK;
}

int is_condExp(TreeNode *enode)
{
    ToKind tkind;
    if (enode->expType == BiExp)
    {
        tkind = enode->firstChild->nextSibling->subkind.tKind;
        return tkind == RelK || tkind == AnK || tkind == OrK;
    }
    else if (enode->expType == UnaExp)
    {
        tkind = enode->firstChild->subkind.tKind;
        return tkind == NoK;
    }
    return 0;
}

void outtofile(char *str)
{
    if (ir_len == IR_MAX_LEN)
    {
        IR_MAX_LEN += 1000;
        char **temp = (char **)malloc(sizeof(char *) * IR_MAX_LEN);
        for (int i = 0; i < ir_len; i++)
        {
            temp[i] = irs[i];
        }
        free(irs);
        irs = temp;
    }
    irs[ir_len++] = str;
}

char *sdup(char *str)
{
    char *s = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(s, str);
    return s;
}

char *conc(char *s1, char *s2)
{
    char *str = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(str, s1);
    strcat(str, s2);
    return str;
}

char *const_str(int num)
{
    char numstr[35];
    sprintf(numstr, "%d", num);
    return conc("#", numstr);
}

char *label_str(char *label)
{
    char *str1 = conc("LABEL ", label);
    char *str2 = conc(str1, " :");
    free(str1);
    return str2;
}

char *func_str(char *f)
{
    char *str1 = conc("FUNCTION ", f);
    char *str2 = conc(str1, " :");
    free(str1);
    return str2;
}

char *tri_str(char *left, char *right, char *op)
{
    char *op1 = conc(" ", op);
    char *op2 = conc(op1, " ");
    char *str1 = conc(left, op2);
    char *str2 = conc(str1, right);
    free(left);
    free(right);
    free(op1);
    free(op2);
    free(str1);
    return str2;
}

char *assign_str(char *left, char *right)
{
    return tri_str(left, right, ":=");
}

char *plus_str(char *left, char *right)
{
    return tri_str(left, right, "+");
}

char *minus_str(char *left, char *right)
{
    return tri_str(left, right, "-");
}

char *mul_str(char *left, char *right)
{
    return tri_str(left, right, "*");
}

char *div_str(char *left, char *right)
{
    return tri_str(left, right, "/");
}

char *ref_str(char *var)
{
    return conc("&", var);
}

char *val_str(char *pointer)
{
    return conc("*", pointer);
}

char *goto_str(char *des)
{
    return conc("GOTO ", des);
}

char *if_str(char *left, char *right, char *relname, char *des)
{
    char *tempstr1 = tri_str(left, right, relname);
    char *tempstr2 = goto_str(des);
    char *str1 = conc("IF ", tempstr1);
    char *str2 = conc(str1, " ");
    char *str3 = conc(str2, tempstr2);
    free(tempstr1);
    free(tempstr2);
    free(str1);
    free(str2);
    return str3;
}

char *ret_str(char *des)
{
    return conc("RETURN ", des);
}

char *dec_str(char *var, int size)
{
    char numstr[35];
    sprintf(numstr, "%d", size);
    char *str1 = conc("DEC ", var);
    char *str2 = conc(str1, " ");
    char *str3 = conc(str2, numstr);
    free(str1);
    free(str2);
    return str3;
}

char *arg_str(char *arg)
{
    return conc("ARG ", arg);
}

char *call_str(char *f)
{
    return conc("CALL ", f);
}

char *param_str(char *par)
{
    return conc("PARAM ", par);
}

char *read_str(char *var)
{
    return conc("READ ", var);
}

char *write_str(char *var)
{
    return conc("WRITE ", var);
}

char *new_label()
{
    lno++;
    char numstr[35];
    sprintf(numstr, "%d", lno);
    return conc("label", numstr);
}

char *new_temp(TreeNode *ExpNode)
{
    if (ExpNode != NULL && ExpNode->expType == idExp)
    {
        return (char *)malloc(sizeof(vlen + 10));
    }
    tno++;
    char numstr[35];
    sprintf(numstr, "%d", tno);
    int len = strlen(numstr) + 1;
    tlen = (len > tlen) ? len : tlen;
    char *str1 = conc("t", numstr);
    char *str2 = (char *)malloc(sizeof(tlen + 10));
    strcpy(str2, str1);
    free(str1);
    return str2;
}

char *new_var()
{
    vno++;
    char numstr[35];
    sprintf(numstr, "%d", vno);
    int len = strlen(numstr) + 1;
    vlen = (len > vlen) ? len : vlen;
    return conc("v", numstr);
}

void calc_offset(TreeNode *ExpNode, char *place)
{
    TreeNode *fchd = ExpNode->firstChild;
    TreeNode *sib = fchd->nextSibling->nextSibling;
    char *t1 = NULL;
    if (fchd->expType != idExp)
    {
        t1 = new_temp(NULL);
        calc_offset(fchd, t1);
    }
    char *t2 = new_temp(sib);
    translate_Exp(sib, t2, 0);
    if (fchd->expType != idExp)
    {
        char *t3 = new_temp(NULL);
        outtofile(assign_str(sdup(t3), mul_str(sdup(t2), const_str(fchd->synty->u.array.elem->width))));
        outtofile(assign_str(sdup(place), plus_str(sdup(t1), sdup(t3))));
        free(t1);
        free(t3);
    }
    else
    {
        outtofile(assign_str(sdup(place), mul_str(sdup(t2), const_str(fchd->synty->u.array.elem->width))));
    }
    free(t2);
}

void translate_Cond(TreeNode *ExpNode, char *label_t, char *label_f)
{
    if (is_condExp(ExpNode))
    {
        ToKind tkind;
        TreeNode *fchd = ExpNode->firstChild;
        TreeNode *sib = fchd->nextSibling;
        if (ExpNode->expType == BiExp)
        {
            tkind = sib->subkind.tKind;
            TreeNode *nsib = fchd->nextSibling->nextSibling;
            if (tkind == RelK)
            {
                char *t1 = new_temp(fchd);
                char *t2 = new_temp(nsib);
                translate_Exp(fchd, t1, 0);
                translate_Exp(nsib, t2, 0);
                outtofile(if_str(sdup(t1), sdup(t2), sib->attr.rel, label_t));
                outtofile(goto_str(label_f));
                free(t1);
                free(t2);
            }
            if (tkind == AnK)
            {
                char *label1 = new_label();
                translate_Cond(fchd, label1, label_f);
                outtofile(label_str(label1));
                translate_Cond(nsib, label_t, label_f);
                free(label1);
            }
            if (tkind == OrK)
            {
                char *label1 = new_label();
                translate_Cond(fchd, label_t, label1);
                outtofile(label_str(label1));
                translate_Cond(nsib, label_t, label_f);
                free(label1);
            }
        }
        else
        {
            translate_Cond(sib, label_f, label_t);
        }
    }
    else
    {
        char *t1 = new_temp(ExpNode);
        translate_Exp(ExpNode, t1, 0);
        outtofile(if_str(sdup(t1), const_str(0), "!=", label_t));
        outtofile(goto_str(label_f));
        free(t1);
    }
}

void translate_Args(TreeNode *ArgsNode, ArgList **parg_list)
{
    TreeNode *fchd = ArgsNode->firstChild;
    char *t1 = new_temp(fchd);
    int isref = 0;
    if (fchd->synty->kind == ARRAY)
    {
        isref = 1;
    }
    translate_Exp(fchd, t1, isref);
    ArgList *narg = (ArgList *)malloc(sizeof(ArgList));
    if (fchd->synty->kind == ARRAY && fchd->expType == idExp && (!fchd->ispar))
    {
        narg->aname = ref_str(t1);
        free(t1);
    }
    else
    {
        narg->aname = t1;
    }
    narg->next = *parg_list;
    *parg_list = narg;
    if (fchd->nextSibling != NULL)
    {
        translate_Args(fchd->nextSibling->nextSibling, parg_list);
    }
}

void translate_Exp(TreeNode *ExpNode, char *place, int isref)
{
    if (ExpNode->expType == IntExp)
    {
        if (place != NULL)
        {
            outtofile(assign_str(sdup(place), const_str(ExpNode->firstChild->attr.val)));
        }
    }
    if (ExpNode->expType == idExp)
    {
        SymNode *snode = searchTable(symTable, ExpNode->firstChild->attr.id, Variable);
        if (place != NULL)
        {
            strcpy(place, snode->val.varinfo.irname);
        }
    }
    if (ExpNode->expType == BiExp || ExpNode->expType == UnaExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        ToKind tkind;
        if (ExpNode->expType == BiExp)
        {
            tkind = fchd->nextSibling->subkind.tKind;
        }
        else
        {
            tkind = fchd->subkind.tKind;
        }

        if (is_cond(tkind))
        {
            char *label1 = new_label();
            char *label2 = new_label();
            if (place != NULL)
            {
                outtofile(assign_str(sdup(place), const_str(0)));
            }
            translate_Cond(ExpNode, label1, label2);
            outtofile(label_str(label1));
            if (place != NULL)
            {
                outtofile(assign_str(sdup(place), const_str(1)));
            }
            outtofile(label_str(label2));
            free(label1);
            free(label2);
        }
        else
        {
            if (ExpNode->expType == BiExp)
            {
                TreeNode *nsib = fchd->nextSibling->nextSibling;
                if (tkind == AssignK)
                {
                    char *t1 = new_temp(fchd);
                    char *t2 = new_temp(nsib);
                    translate_Exp(fchd, t1, 1);
                    if (fchd->expType == ArrExp)
                    {
                        char *tempstr = t1;
                        t1 = val_str(tempstr);
                        free(tempstr);
                    }
                    translate_Exp(nsib, t2, 0);
                    outtofile(assign_str(sdup(t1), sdup(t2)));
                    if (place != NULL)
                    {
                        if (fchd->expType == ArrExp)
                        {
                            char* t3 = new_temp(NULL);
                            translate_Exp(fchd,t3,0);
                            outtofile(assign_str(sdup(place),sdup(t3)));
                        }
                        else
                        {
                            outtofile(assign_str(sdup(place), sdup(t1)));
                        }
                    }
                    free(t1);
                    free(t2);
                }
                else
                {
                    char *t1 = new_temp(fchd);
                    char *t2 = new_temp(nsib);
                    translate_Exp(fchd, t1, 0);

                    translate_Exp(nsib, t2, 0);
                    char *right = NULL;
                    if (tkind == PlK)
                    {
                        right = plus_str(sdup(t1), sdup(t2));
                    }
                    if (tkind == MiK)
                    {
                        right = minus_str(sdup(t1), sdup(t2));
                    }
                    if (tkind == StK)
                    {
                        right = mul_str(sdup(t1), sdup(t2));
                    }
                    if (tkind == DiK)
                    {
                        right = div_str(sdup(t1), sdup(t2));
                    }
                    if (place != NULL)
                    {
                        outtofile(assign_str(sdup(place), right));
                    }
                    free(t1);
                    free(t2);
                }
            }
            else
            {

                if (tkind == LpK)
                {
                    translate_Exp(fchd->nextSibling, place, 0);
                }
                else
                {
                    char *t1 = new_temp(fchd->nextSibling);
                    translate_Exp(fchd->nextSibling, t1, 0);
                    if (place != NULL)
                    {
                        outtofile(assign_str(sdup(place), minus_str(const_str(0), sdup(t1))));
                    }
                    free(t1);
                }
            }
        }
    }
    if (ExpNode->expType == FuncExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        TreeNode *sib = fchd->nextSibling->nextSibling;
        SymNode *snode = searchTable(symTable, fchd->attr.id, FuncDef);
        if (sib->nextSibling == NULL)
        {
            if (strcmp(snode->sname, "read") == 0)
            {
                if (place != NULL)
                {
                    outtofile(read_str(place));
                }
                return;
            }
        }
        else
        {
            ArgList *arg_list = NULL;
            translate_Args(sib, &arg_list);
            if (strcmp(snode->sname, "write") == 0)
            {
                outtofile(write_str(arg_list->aname));
                if (place != NULL)
                {
                    outtofile(assign_str(sdup(place), const_str(0)));
                }
                return;
            }
            else
            {
                ArgList *temp = arg_list;
                char *str = NULL;
                while (temp != NULL)
                {
                    char *s = arg_str(temp->aname);
                    if (str == NULL)
                    {
                        str = s;
                    }
                    else
                    {
                        char *s1 = conc(str, "\n");
                        char *s2 = conc(s1, s);
                        free(str);
                        free(s1);
                        str = s2;
                    }
                    temp = temp->next;
                }
                if (str != NULL)
                {
                    char *chd_str = strtok(str, "\n");
                    while (chd_str != NULL)
                    {
                        outtofile(chd_str);
                        chd_str = strtok(NULL, "\n");
                    }
                }
                free_ArgList(arg_list);
            }
        }
        if (place != NULL)
        {
            outtofile(assign_str(sdup(place), call_str(snode->sname)));
        }
        else
        {
            char *t1 = "u0";
            outtofile(assign_str(sdup(t1), call_str(snode->sname)));
        }
    }

    if (ExpNode->expType == ArrExp)
    {
        char *t1 = new_temp(NULL);
        calc_offset(ExpNode, t1);
        if (place != NULL)
        {
            if (ExpNode->ispar)
            {
                outtofile(assign_str(sdup(place), plus_str(sdup(ExpNode->attr.id), sdup(t1))));
            }
            else
            {
                outtofile(assign_str(sdup(place), plus_str(ref_str(ExpNode->attr.id), sdup(t1))));
            }
            if (!isref)
            {
                char *t2 = new_temp(NULL);
                outtofile(assign_str(sdup(t2), val_str(place)));
                strcpy(place, t2);
                free(t1);
                free(t2);
            }
        }
    }
}

void translate_CompSt(TreeNode *CompStNode)
{
    transtoir(CompStNode);
}

void translate_Stmt(TreeNode *StmtNode)
{
    TreeNode *fchd = StmtNode->firstChild;
    if (StmtNode->stmtType == ExpStmt)
    {
        translate_Exp(fchd, NULL, 0);
    }
    if (StmtNode->stmtType == CompStStmt)
    {
        translate_CompSt(fchd);
    }
    if (StmtNode->stmtType == RetStmt)
    {
        char *t1 = new_temp(fchd->nextSibling);
        translate_Exp(fchd->nextSibling, t1, 0);
        outtofile(ret_str(t1));
        free(t1);
    }
    if (StmtNode->stmtType == IfStmt)
    {
        TreeNode *sib = fchd->nextSibling->nextSibling;
        TreeNode *nsib = sib->nextSibling->nextSibling;
        if (nsib->nextSibling == NULL)
        {
            char *label1 = new_label();
            char *label2 = new_label();
            translate_Cond(sib, label1, label2);
            outtofile(label_str(label1));
            translate_Stmt(nsib);
            outtofile(label_str(label2));
            free(label1);
            free(label2);
        }
        else
        {
            TreeNode *nnsib = nsib->nextSibling->nextSibling;
            char *label1 = new_label();
            char *label2 = new_label();
            char *label3 = new_label();
            translate_Cond(sib, label1, label2);
            outtofile(label_str(label1));
            translate_Stmt(nsib);
            outtofile(goto_str(label3));
            outtofile(label_str(label2));
            translate_Stmt(nnsib);
            outtofile(label_str(label3));
            free(label1);
            free(label2);
            free(label3);
        }
    }
    if (StmtNode->stmtType == WhileStmt)
    {
        TreeNode *sib = fchd->nextSibling->nextSibling;
        TreeNode *nsib = sib->nextSibling->nextSibling;
        char *label1 = new_label();
        char *label2 = new_label();
        char *label3 = new_label();
        outtofile(label_str(label1));
        translate_Cond(sib, label2, label3);
        outtofile(label_str(label2));
        translate_Stmt(nsib);
        outtofile(goto_str(label1));
        outtofile(label_str(label3));
        free(label1);
        free(label2);
        free(label3);
    }
    StmtNode->translated = 1;
}

void translate_Dec(TreeNode *DecNode)
{
    SymNode *snode = searchTable(symTable, DecNode->attr.id, Variable);
    if (snode->val.varinfo.varkind == ArrVar)
    {
        outtofile(dec_str(snode->val.varinfo.irname, snode->val.varinfo.vartype->width));
    }
    TreeNode *fchd = DecNode->firstChild;
    if (fchd->nextSibling != NULL)
    {
        TreeNode *sib = fchd->nextSibling->nextSibling;
        if (sib->expType == idExp || sib->expType == ArrExp)
        {
            char *t1 = new_temp(sib);
            translate_Exp(sib, t1, 0);
            outtofile(assign_str(sdup(snode->val.varinfo.irname), sdup(t1)));
            free(t1);
        }
        else
        {
            translate_Exp(sib, snode->val.varinfo.irname, 0);
        }
    }
    DecNode->translated = 1;
}

void translate_FunDec(TreeNode *FunDecNode)
{
    TreeNode *fchd = FunDecNode->firstChild;
    SymNode *snode = searchTable(symTable, fchd->attr.id, FuncDef);
    outtofile(func_str(snode->sname));
    ParamList *param_list = snode->val.funcinfo.paramlist;
    while (param_list != NULL)
    {
        outtofile(param_str(param_list->symnode->val.varinfo.irname));
        param_list = param_list->next;
    }
}

void translate_ExtDef(TreeNode *ExtDefNode)
{
    TreeNode *sib = ExtDefNode->firstChild->nextSibling;
    if (sib->nodeKind == Decl && sib->subkind.dKind == FDec)
    {
        translate_FunDec(sib);
        translate_CompSt(sib->nextSibling);
    }
    ExtDefNode->translated = 1;
}

void transtoir(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->nodeKind == HlDef && root->subkind.hKind == ExtD && root->translated == 0)
    {
        translate_ExtDef(root);
    }

    if (root->nodeKind == Stmts && root->subkind.stKind == Stmt && root->translated == 0)
    {
        translate_Stmt(root);
    }
    if (root->nodeKind == LcDef && root->subkind.lKind == Dc && root->translated == 0)
    {
        translate_Dec(root);
    }

    transtoir(root->firstChild);
    transtoir(root->nextSibling);
}

void translate(char *outfile, TreeNode *root)
{
    if (outfile != NULL)
    {
        outfp = fopen(outfile, "w");

        if (!outfp)
        {
            perror(outfile);
            return;
        }
    }
    else
    {
        outfp = NULL;
    }
    IR_MAX_LEN = 1000;
    ir_len = 0;
    irs = (char **)malloc(sizeof(char *) * IR_MAX_LEN);
    transtoir(root);
}