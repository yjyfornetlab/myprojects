#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashsymtable.h"
#include "semanalysis.h"
#include "cmmtype.h"
#include "translate.h"

void semerror(int typeno, int nline)
{
    fprintf(stdout, "Error type %d at Line %d: semantic error\n", typeno, nline);
}

int isleftValue(TreeNode *expNode)
{
    return expNode->expType == idExp || expNode->expType == ArrExp || expNode->expType == StruExp;
}

int isAri(ToKind tkind)
{
    return tkind == RelK || tkind == PlK || tkind == MiK || tkind == StK || tkind == DiK;
}

int isLogic(ToKind tkind)
{
    return tkind == AnK || tkind == OrK || tkind == NoK;
}

int check(ParamList *paramlist, TreeNode *arglist, TreeNode *expNode)
{
    if ((paramlist == NULL && arglist != NULL) || (paramlist != NULL && arglist == NULL))
    {
        semerror(9, expNode->lineno);
        return 0;
    }
    if (paramlist == NULL && arglist == NULL)
    {
        return 1;
    }

    TreeNode *fchd = arglist->firstChild;
    analyseExp(fchd);
    if (!matchType(paramlist->argty, fchd->synty))
    {
        semerror(9, fchd->lineno);
        return 0;
    }
    TreeNode *sib = NULL;
    if (fchd->nextSibling != NULL)
    {
        sib = fchd->nextSibling->nextSibling;
    }
    return check(paramlist->next, sib, expNode);
}

int calc_width(Type ty)
{
    if(ty->kind!=ARRAY)
    {
        return ty->width;
    }
    ty->width = ty->u.array.size*calc_width(ty->u.array.elem);
    return ty->width;
}

SymNode *insertVar(TreeNode *node, int isflist)
{
    if (!isflist)
    {
        SymNode *searchNode1 = searchTable(symTable, node->attr.id, Variable);
        SymNode *searchNode2 = searchTable(symTable, node->attr.id, StructDef);
        if (searchNode1 != NULL || searchNode2 != NULL)
        {
            semerror(3, node->lineno);
        }
    }

    SymNode *symNode = (SymNode *)malloc(sizeof(SymNode));
    symNode->sname = node->attr.id;
    symNode->symKind = Variable;
    insertSymNode(symTable, symNode);
    symNode->val.varinfo.vartype = node->synty;
    symNode->val.varinfo.irname = new_var();
    symNode->val.varinfo.isparam = 0;
    if (node->synty != NULL)
    {
        switch (node->synty->kind)
        {
        case BASIC:
            symNode->val.varinfo.varkind = BasicVar;
            break;
        case ARRAY:
            symNode->val.varinfo.varkind = ArrVar;
            calc_width(symNode->val.varinfo.vartype);
            break;
        case STRUCTURE:
            symNode->val.varinfo.varkind = StructVar;
            break;
        }
    }
    return symNode;
}

void analyseExp(TreeNode *ExpNode)
{
    ExpNode->visited = 1;
    if (ExpNode->expType == idExp)
    {
        TreeNode *chd = ExpNode->firstChild;
        SymNode *symNode = searchTable(symTable, chd->attr.id, Variable);
        if (symNode == NULL)
        {
            semerror(1, ExpNode->lineno);
            return;
        }
        ExpNode->synty = symNode->val.varinfo.vartype;
        ExpNode->attr.id = symNode->val.varinfo.irname;
        ExpNode->ispar = symNode->val.varinfo.isparam;
    }
    if (ExpNode->expType == IntExp)
    {
        ExpNode->synty = intTy;
    }
    if (ExpNode->expType == FloatExp)
    {
        ExpNode->synty = floatTy;
    }
    if (ExpNode->expType == FuncExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        SymNode *symNode = searchTable(symTable, fchd->attr.id, Undef);
        if (symNode == NULL)
        {
            semerror(2, ExpNode->lineno);
            return;
        }
        symNode = searchTable(symTable, fchd->attr.id, FuncDef);
        if (symNode == NULL)
        {
            semerror(11, ExpNode->lineno);
            return;
        }
        TreeNode *sib = fchd->nextSibling->nextSibling;
        TreeNode *argList = NULL;
        if (sib->nodeKind == Exps && sib->subkind.eKind == Ags)
        {
            argList = sib;
        }
        if (check(symNode->val.funcinfo.paramlist, argList, ExpNode))
        {
            ExpNode->synty = symNode->val.funcinfo.rettype;
        }
    }

    if (ExpNode->expType == StruExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        analyseExp(fchd);
        if (fchd->synty == NULL)
        {
            return;
        }
        if (fchd->synty == NULL || fchd->synty->kind != STRUCTURE)
        {
            semerror(13, ExpNode->lineno);
            return;
        }
        TreeNode *sib = fchd->nextSibling->nextSibling;
        FieldList temp = fchd->synty->u.structure;
        while (temp != NULL)
        {
            if (strcmp(temp->name, sib->attr.id) == 0)
            {
                ExpNode->synty = temp->type;
                return;
            }
            temp = temp->tail;
        }
        semerror(14, ExpNode->lineno);
    }

    if (ExpNode->expType == BiExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        ToKind tkind = fchd->nextSibling->subkind.tKind;
        TreeNode *nsib = fchd->nextSibling->nextSibling;
        analyseExp(fchd);
        analyseExp(nsib);
        Type ty1 = fchd->synty;
        Type ty2 = nsib->synty;
        if (tkind == AssignK)
        {
            if (!isleftValue(fchd))
            {
                semerror(6, ExpNode->lineno);
                return;
            }
            if (!matchType(ty1, ty2))
            {
                semerror(5, ExpNode->lineno);
                return;
            }
            ExpNode->synty = ty1;
            return;
        }
        if (ty1 != NULL && ty2 != NULL)
        {
            if (ty1->kind == BASIC && ty2->kind == BASIC && ty1->u.basic == ty2->u.basic)
            {
                if (isAri(tkind) || (isLogic(tkind) && ty1->u.basic == 0))
                {
                    if (tkind == RelK)
                    {
                        ExpNode->synty = intTy;
                    }
                    else
                    {
                        ExpNode->synty = ty1;
                    }
                    return;
                }
            }
            semerror(7, ExpNode->lineno);
        }
    }
    
    if (ExpNode->expType == UnaExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        ToKind tkind = fchd->subkind.tKind;
        TreeNode *sib = fchd->nextSibling;
        analyseExp(sib);
        Type ty = sib->synty;
        if (ty != NULL)
        {
            if (tkind == LpK || (ty->kind == BASIC && (isAri(tkind) || (isLogic(tkind) && ty->u.basic == 0))))
            {
                if (tkind == RelK)
                {
                    ExpNode->synty = intTy;
                }
                else
                {
                    ExpNode->synty = ty;
                }
                return;
            }
            semerror(7, ExpNode->lineno);
        }
    }

    if (ExpNode->expType == ArrExp)
    {
        TreeNode *fchd = ExpNode->firstChild;
        TreeNode *sib = fchd->nextSibling->nextSibling;
        analyseExp(fchd);
        analyseExp(sib);
        if (fchd->synty == NULL)
        {
            return;
        }
        if (fchd->synty == NULL || fchd->synty->kind != ARRAY)
        {
            semerror(10, ExpNode->lineno);
            return;
        }
        if ((sib->synty != NULL) && (sib->synty->kind != BASIC || sib->synty->u.basic != 0))
        {
            semerror(12, ExpNode->lineno);
        }
        ExpNode->synty = fchd->synty->u.array.elem;
        ExpNode->attr.id = fchd->attr.id;
        ExpNode->ispar = fchd->ispar;
    }
}

void analyseDec(TreeNode *DecNode, int isflist)
{
    TreeNode *varDecNode = DecNode->firstChild;
    varDecNode->inhty = DecNode->inhty;
    analyseVarDec(varDecNode);
    if (varDecNode->nextSibling != NULL)
    {
        if (isflist)
        {
            semerror(15, DecNode->lineno);
        }
        TreeNode *expNode = varDecNode->nextSibling->nextSibling;
        analyseExp(expNode);
        if (!matchType(expNode->synty, varDecNode->synty))
        {
            semerror(5, DecNode->lineno);
        }
    }
    DecNode->synty = varDecNode->synty;
    DecNode->attr.id = varDecNode->attr.id;
}

FieldList analyseDecLt(TreeNode *DecLtNode, int isflist, SymTable *struTable)
{
    TreeNode *DecNode = DecLtNode->firstChild;
    DecNode->inhty = DecLtNode->inhty;
    analyseDec(DecNode, isflist);

    if (isflist)
    {
        SymNode *searchNode = searchTable(struTable, DecNode->attr.id, Variable);
        if (searchNode != NULL)
        {
            semerror(15, DecNode->lineno);
        }
        SymNode *symNode = insertVar(DecNode, isflist);
        insertSymNode(struTable, symNode);
        FieldList nfield = (FieldList)malloc(sizeof(FieldList_));
        nfield->name = DecNode->attr.id;
        nfield->type = DecNode->synty;
        nfield->tail = NULL;
        if (DecNode->nextSibling != NULL)
        {
            TreeNode *sib = DecNode->nextSibling->nextSibling;
            sib->inhty = DecLtNode->inhty;
            FieldList post = analyseDecLt(sib, isflist, struTable);
            nfield->tail = post;
        }
        return nfield;
    }
    else
    {
        SymNode* symNode = insertVar(DecNode, isflist);
        if (DecNode->nextSibling != NULL)
        {
            TreeNode *sib = DecNode->nextSibling->nextSibling;
            sib->inhty = DecLtNode->inhty;
            analyseDecLt(sib, isflist, struTable);
        }
        return NULL;
    }
}

FieldList analyseDef(TreeNode *DefNode, int isflist, SymTable *struTable)
{
    DefNode->visited = 1;
    TreeNode *fchd = DefNode->firstChild;
    analyseSpec(fchd);
    TreeNode *sib = fchd->nextSibling;
    sib->inhty = fchd->synty;
    FieldList fieldLt = analyseDecLt(sib, isflist, struTable);
    return fieldLt;
}

FieldList analyseDefLt(TreeNode *DefLtNode, int isflist, SymTable *struTable)
{
    TreeNode *fchd = DefLtNode->firstChild;
    if (fchd->nodeKind == NullK)
    {
        return NULL;
    }
    else
    {
        FieldList prev = analyseDef(fchd, isflist, struTable);
        TreeNode *sib = fchd->nextSibling;
        FieldList post = analyseDefLt(sib, isflist, struTable);
        if (prev != NULL)
        {
            FieldList temp = prev;
            while (temp->tail != NULL)
            {
                temp = temp->tail;
            }
            temp->tail = post;
        }
        return prev;
    }
}

void analyseSpec(TreeNode *SpecNode)
{
    TreeNode *chd = SpecNode->firstChild;
    if (chd->nodeKind == TypeTok)
    {
        switch (chd->subkind.tyKind)
        {
        case IntK:
            SpecNode->synty = intTy;
            break;
        case FloatK:
            SpecNode->synty = floatTy;
            break;
        }
        return;
    }
    else
    {
        istransable = 0;
        Type ty = (Type)malloc(sizeof(Type_));
        TreeNode *sibf = chd->firstChild->nextSibling;
        if (sibf->nextSibling != NULL)
        {
            TreeNode *sibs = sibf->nextSibling->nextSibling;
            SymTable *struTable = newSymTable();
            ty->kind = STRUCTURE;
            ty->u.structure = analyseDefLt(sibs, 1, struTable);
            ty->width = 0;
            free(struTable->table);
            free(struTable);
            SpecNode->synty = ty;
            if (sibf->firstChild->nodeKind == IdTok)
            {
                SymNode *searchNode1 = searchTable(symTable, sibf->firstChild->attr.id, StructDef);
                SymNode *searchNode2 = searchTable(symTable, sibf->firstChild->attr.id, Variable);
                if (searchNode1 != NULL || searchNode2 != NULL)
                {
                    semerror(16, sibf->lineno);
                }
                SymNode *symNode = (SymNode *)malloc(sizeof(SymNode));
                symNode->sname = sibf->firstChild->attr.id;
                symNode->symKind = StructDef;
                symNode->val.struinfo.fieldLt = ty->u.structure;
                insertSymNode(symTable, symNode);
            }
            return;
        }
        else
        {
            SymNode *searchNode = searchTable(symTable, sibf->firstChild->attr.id, StructDef);
            if (searchNode == NULL)
            {
                semerror(17, sibf->lineno);
                return;
            }
            ty->kind = STRUCTURE;
            ty->u.structure = searchNode->val.struinfo.fieldLt;
            ty->width = 0;
            SpecNode->synty = ty;
        }
        
    }
}

void analyseVarDec(TreeNode *VarDecNode)
{
    TreeNode *fchd = VarDecNode->firstChild;
    if (fchd->nodeKind == IdTok)
    {
        VarDecNode->attr.id = fchd->attr.id;
        VarDecNode->synty = VarDecNode->inhty;
        return;
    }
    else
    {
        int arrsize = fchd->nextSibling->nextSibling->attr.val;
        fchd->inhty = VarDecNode->inhty;
        analyseVarDec(fchd);
        VarDecNode->attr.id = fchd->attr.id;
        if (fchd->synty == NULL)
        {
            return;
        }
        Type arrty = (Type)malloc(sizeof(Type_));
        arrty->kind = ARRAY;
        arrty->u.array.size = arrsize;
        Type temp = fchd->synty;

        if (temp->kind != ARRAY)
        {
            arrty->u.array.elem = temp;
            VarDecNode->synty = arrty;
        }
        else
        {
            while (temp->u.array.elem->kind == ARRAY)
            {
                temp = temp->u.array.elem;
            }
            arrty->u.array.elem = temp->u.array.elem;
            temp->u.array.elem = arrty;
            VarDecNode->synty = fchd->synty;
        }
    }
}

SymNode* analyseParamDec(TreeNode *ParamDecNode)
{
    TreeNode *fchd = ParamDecNode->firstChild;
    analyseSpec(fchd);
    TreeNode *sib = fchd->nextSibling;
    sib->inhty = fchd->synty;
    analyseVarDec(sib);
    ParamDecNode->synty = sib->synty;
    ParamDecNode->attr.id = sib->attr.id;
    SymNode* varnode = insertVar(ParamDecNode, 0);
    varnode->val.varinfo.isparam = 1;
    return varnode;
}

void analyseVarList(TreeNode *VarListNode, SymNode *symNode)
{
    TreeNode *fchd = VarListNode->firstChild;
    SymNode* varnode = analyseParamDec(fchd);

    ParamList *argth = (ParamList *)malloc(sizeof(ParamList));
    argth->symnode = varnode;
    argth->argty = fchd->synty;
    argth->next = NULL;
    if (symNode->val.funcinfo.paramlist == NULL)
    {
        symNode->val.funcinfo.paramlist = argth;
    }
    else
    {
        ParamList *temp = symNode->val.funcinfo.paramlist;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = argth;
    }
    symNode->val.funcinfo.argnum++;
    if (fchd->nextSibling != NULL)
    {
        TreeNode *sib = fchd->nextSibling->nextSibling;
        sib->inhty = VarListNode->inhty;
        analyseVarList(sib, symNode);
    }
}

void analyseStmt(TreeNode *StmtNode)
{
    if (StmtNode->stmtType == ExpStmt)
    {
        analyseExp(StmtNode->firstChild);
    }
    if (StmtNode->stmtType == CompStStmt)
    {
        TreeNode *fchd = StmtNode->firstChild;
        fchd->retty = StmtNode->retty;
        analyseCompSt(fchd);
    }
    if (StmtNode->stmtType == RetStmt)
    {
        TreeNode *expNode = StmtNode->firstChild->nextSibling;
        analyseExp(expNode);
        if (!matchType(expNode->synty, StmtNode->retty))
        {
            semerror(8, StmtNode->lineno);
        }
    }
    if (StmtNode->stmtType == IfStmt || StmtNode->stmtType == WhileStmt)
    {
        TreeNode *sib = StmtNode->firstChild->nextSibling->nextSibling;
        analyseExp(sib);
        TreeNode *nsib = sib->nextSibling->nextSibling;
        nsib->retty = StmtNode->retty;
        analyseStmt(nsib);
        if (nsib->nextSibling != NULL)
        {
            TreeNode *nnsib = nsib->nextSibling->nextSibling;
            nnsib->retty = StmtNode->retty;
            analyseStmt(nnsib);
        }
    }
}

void analyseStmtLt(TreeNode *StmtLtNode)
{
    TreeNode *fchd = StmtLtNode->firstChild;
    if (fchd->nodeKind != NullK)
    {
        fchd->retty = StmtLtNode->retty;
        analyseStmt(fchd);
        TreeNode *sib = fchd->nextSibling;
        sib->retty = StmtLtNode->retty;
        analyseStmtLt(sib);
    }
}

void analyseCompSt(TreeNode *CompStNode)
{
    TreeNode *defLtNode = CompStNode->firstChild->nextSibling;
    analyseDefLt(defLtNode, 0, NULL);
    TreeNode *sib = defLtNode->nextSibling;
    sib->retty = CompStNode->retty;
    analyseStmtLt(sib);
}

void analyseFunDec(TreeNode *FunDecNode)
{
    TreeNode *fchd = FunDecNode->firstChild;
    SymNode *searchNode = searchTable(symTable, fchd->attr.id, FuncDef);
    if (searchNode != NULL)
    {
        semerror(4, FunDecNode->lineno);
    }
    SymNode *symNode = (SymNode *)malloc(sizeof(SymNode));
    symNode->sname = fchd->attr.id;
    insertSymNode(symTable, symNode);
    symNode->symKind = FuncDef;
    symNode->val.funcinfo.rettype = FunDecNode->inhty;
    symNode->val.funcinfo.argnum = 0;
    symNode->val.funcinfo.paramlist = NULL;
    TreeNode *sib = fchd->nextSibling->nextSibling;
    if (sib->nodeKind == Decl && sib->subkind.dKind == VList)
    {
        analyseVarList(sib, symNode);
    }
}

void analyseExtDecLt(TreeNode *ExtDecLtNode)
{
    TreeNode *varDecNode = ExtDecLtNode->firstChild;
    varDecNode->inhty = ExtDecLtNode->inhty;
    analyseVarDec(varDecNode);

    insertVar(varDecNode, 0);
    if (varDecNode->nextSibling != NULL)
    {
        TreeNode *sib = varDecNode->nextSibling->nextSibling;
        sib->inhty = ExtDecLtNode->inhty;
        analyseExtDecLt(sib);
    }
}

void analyseExtDef(TreeNode *ExtDefNode)
{
    ExtDefNode->visited = 1;
    TreeNode *specNode = ExtDefNode->firstChild;
    analyseSpec(specNode);
    TreeNode *sib = specNode->nextSibling;
    if (sib->nodeKind != StrTok)
    {
        sib->inhty = specNode->synty;
        if (sib->nodeKind == HlDef && sib->subkind.hKind == ExtDecLt)
        {
            analyseExtDecLt(sib);
        }
        else
        {
            analyseFunDec(sib);
            TreeNode *nsib = sib->nextSibling;
            nsib->retty = specNode->synty;
            analyseCompSt(nsib);
        }
    }
}

void analyse(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->nodeKind == HlDef && root->subkind.hKind == ExtD && root->visited == 0)
    {
        analyseExtDef(root);
    }

    if (root->nodeKind == LcDef && root->subkind.lKind == Df && root->visited == 0)
    {
        analyseDef(root, 0, NULL);
    }

    if (root->nodeKind == Exps && root->subkind.eKind == Exp && root->visited == 0)
    {
        analyseExp(root);
    }

    analyse(root->firstChild);
    analyse(root->nextSibling);
}