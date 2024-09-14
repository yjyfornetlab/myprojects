#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void insertChild(TreeNode *parent, TreeNode *child)
{
    if (parent == NULL || child == NULL)
    {
        return;
    }
    if (parent->firstChild == NULL)
    {
        parent->firstChild = child;
    }
    else
    {
        TreeNode *sibling = parent->firstChild;
        while (sibling->nextSibling != NULL)
        {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = child;
    }
}

TreeNode *newNode(int nline)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->firstChild = NULL;
    node->nextSibling = NULL;
    node->lineno = nline;
    node->synty = NULL;
    node->inhty = NULL;
    node->retty = NULL;
    node->visited = 0;
    node->translated = 0;
    return node;
}

TreeNode *newHlDefNode(HlDefKind hlk, int nline)
{
    TreeNode *hlNode = newNode(nline);
    hlNode->nodeKind = HlDef;
    hlNode->subkind.hKind = hlk;
    return hlNode;
}

TreeNode *newNull()
{
    TreeNode *nullNode = newNode(0);
    nullNode->nodeKind = NullK;
}

TreeNode *newSTokNode(ToKind tok)
{
    TreeNode *tokNode = newNode(0);
    tokNode->nodeKind = StrTok;
    tokNode->subkind.tKind = tok;
    return tokNode;
}

TreeNode *newIdTokNode(char *name)
{
    TreeNode *idtokNode = newNode(0);
    idtokNode->nodeKind = IdTok;
    // tokNode->attr.tokname = name;
    idtokNode->attr.id = name;
    return idtokNode;
}

TreeNode *newTyTokNode(TypeKind tyk)
{
    TreeNode *tyNode = newNode(0);
    tyNode->nodeKind = TypeTok;
    tyNode->subkind.tyKind = tyk;
    return tyNode;
}

TreeNode *newSpecNode(SpecKind spk, int nline)
{
    TreeNode *spNode = newNode(nline);
    spNode->nodeKind = Spec;
    spNode->subkind.sKind = spk;
    return spNode;
}

TreeNode *newDecNode(DeclKind dek, int nline)
{
    TreeNode *decNode = newNode(nline);
    decNode->nodeKind = Decl;
    decNode->subkind.dKind = dek;
    return decNode;
}

TreeNode *newStNode(StmtsKind stk, int nline)
{
    TreeNode *stNode = newNode(nline);
    stNode->nodeKind = Stmts;
    stNode->subkind.stKind = stk;
    return stNode;
}

TreeNode *newLcDNode(LcDefKind lcDk, int nline)
{
    TreeNode *lcNode = newNode(nline);
    lcNode->nodeKind = LcDef;
    lcNode->subkind.lKind = lcDk;
    return lcNode;
}

TreeNode *newExpNode(ExpsKind expk, int nline)
{
    TreeNode *exNode = newNode(nline);
    exNode->nodeKind = Exps;
    exNode->subkind.eKind = expk;
    return exNode;
}

TreeNode *newIntNode(int num)
{
    TreeNode *IntNode = newNode(0);
    IntNode->nodeKind = IntTok;
    IntNode->attr.val = num;
    return IntNode;
}

TreeNode *newFloatNode(float fnum)
{
    TreeNode *FloatNode = newNode(0);
    FloatNode->nodeKind = FloTok;
    FloatNode->attr.fval = fnum;
    return FloatNode;
}

TreeNode *createProgNode(TreeNode *chd)
{
    TreeNode *prog = newHlDefNode(Prog, chd->lineno);
    insertChild(prog, chd);
    return prog;
}

TreeNode *createExtDefLtNode(TreeNode *chd1, TreeNode *chd2)
{
    int nline = 0;
    if (chd1 != NULL)
    {
        nline = chd1->lineno;
    }
    TreeNode *ExtDefLtNode = newHlDefNode(ExtDefLt, nline);
    if (chd1 == NULL && chd2 == NULL)
    {
        TreeNode *nullNode = newNull();
        insertChild(ExtDefLtNode, nullNode);
    }
    else
    {
        insertChild(ExtDefLtNode, chd1);
        insertChild(ExtDefLtNode, chd2);
    }
    return ExtDefLtNode;
}

TreeNode *createExtDNode(TreeNode *chd1, TreeNode *chd2, TreeNode *chd3)
{
    TreeNode *ExtDefNode = newHlDefNode(ExtD, chd1->lineno);
    insertChild(ExtDefNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(ExtDefNode, chd2);
    }
    if (chd3 == NULL)
    {
        insertChild(ExtDefNode, newSTokNode(SemiK));
    }
    else
    {
        insertChild(ExtDefNode, chd3);
    }
    return ExtDefNode;
}

TreeNode *createExtDecLtNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *EdlNode = newHlDefNode(ExtDecLt, chd1->lineno);
    insertChild(EdlNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(EdlNode, newSTokNode(ComK));
        insertChild(EdlNode, chd2);
    }
    return EdlNode;
}

TreeNode *createSpecifNode(TreeNode *chd1, char *ty, int nline)
{
    TreeNode *spcNode = newSpecNode(Specif, nline);
    if (chd1 != NULL)
    {
        insertChild(spcNode, chd1);
    }
    else
    {
        TypeKind typeK;
        if (strcmp(ty, "int") == 0)
        {
            typeK = IntK;
        }
        else if (strcmp(ty, "float") == 0)
        {
            typeK = FloatK;
        }
        insertChild(spcNode, newTyTokNode(typeK));
    }
    return spcNode;
}

TreeNode *createStrSpecNode(TreeNode *chd1, TreeNode *chd2, int nline)
{
    TreeNode *StrSpNode = newSpecNode(StrSpec, nline);
    insertChild(StrSpNode, newSTokNode(StruK));
    insertChild(StrSpNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(StrSpNode, newSTokNode(LcK));
        insertChild(StrSpNode, chd2);
        insertChild(StrSpNode, newSTokNode(RcK));
    }
    return StrSpNode;
}

TreeNode *createOptNode(char *name, int nline)
{
    TreeNode *OptNode = newSpecNode(Opt, nline);
    if (name != NULL)
    {
        insertChild(OptNode, newIdTokNode(name));
    }
    else
    {
        insertChild(OptNode, newNull());
    }
    return OptNode;
}

TreeNode *createTagNode(char *name, int nline)
{
    TreeNode *TagNode = newSpecNode(TagK, nline);
    insertChild(TagNode, newIdTokNode(name));
    return TagNode;
}

TreeNode *createVDecNode(TreeNode *chd1, int num, char *name, int nline)
{
    TreeNode *VDecNode = newDecNode(VDec, nline);
    if (chd1 == NULL)
    {
        insertChild(VDecNode, newIdTokNode(name));
    }
    else
    {
        insertChild(VDecNode, chd1);
        insertChild(VDecNode, newSTokNode(LbK));
        insertChild(VDecNode, newIntNode(num));
        insertChild(VDecNode, newSTokNode(RbK));
    }
    return VDecNode;
}

TreeNode *createFDecNode(TreeNode *chd1, char *name, int nline)
{
    TreeNode *FDecNode = newDecNode(FDec, nline);
    insertChild(FDecNode, newIdTokNode(name));
    insertChild(FDecNode, newSTokNode(LpK));
    if (chd1 != NULL)
    {
        insertChild(FDecNode, chd1);
    }
    insertChild(FDecNode, newSTokNode(RpK));
    return FDecNode;
}

TreeNode *createVListNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *VListNode = newDecNode(VList, chd1->lineno);
    insertChild(VListNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(VListNode, newSTokNode(ComK));
        insertChild(VListNode, chd2);
    }
    return VListNode;
}

TreeNode *createParDNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *ParDNode = newDecNode(ParD, chd1->lineno);
    insertChild(ParDNode, chd1);
    insertChild(ParDNode, chd2);
    return ParDNode;
}

TreeNode *createCpStNode(TreeNode *chd1, TreeNode *chd2, int nline)
{
    TreeNode *CpStNode = newStNode(CpSt, nline);
    insertChild(CpStNode, newSTokNode(LcK));
    insertChild(CpStNode, chd1);
    insertChild(CpStNode, chd2);
    insertChild(CpStNode, newSTokNode(RcK));
    return CpStNode;
}

TreeNode *createStmtLtNode(TreeNode *chd1, TreeNode *chd2)
{
    int nline = 0;
    if (chd1 != NULL)
    {
        nline = chd1->lineno;
    }
    TreeNode *StmtLtNode = newStNode(StmtLt, nline);
    if (chd1 == NULL)
    {
        insertChild(StmtLtNode, newNull());
    }
    else
    {
        insertChild(StmtLtNode, chd1);
        insertChild(StmtLtNode, chd2);
    }
    return StmtLtNode;
}

TreeNode *createStmtNode(StmtType stTy, TreeNode *chd1, TreeNode *chd2, TreeNode *chd3, int nline)
{
    TreeNode *StmtNode = newStNode(Stmt, nline);
    StmtNode->stmtType = stTy;
    if (stTy == ExpStmt)
    {
        insertChild(StmtNode, chd1);
        insertChild(StmtNode, newSTokNode(SemiK));
    }
    else if (stTy == CompStStmt)
    {
        insertChild(StmtNode, chd1);
    }
    else if (stTy == RetStmt)
    {
        insertChild(StmtNode, newSTokNode(RetK));
        insertChild(StmtNode, chd1);
        insertChild(StmtNode, newSTokNode(SemiK));
    }
    else
    {
        if (stTy == IfStmt)
        {
            insertChild(StmtNode, newSTokNode(IfK));
        }
        else
        {
            insertChild(StmtNode, newSTokNode(WhK));
        }
        insertChild(StmtNode, newSTokNode(LpK));
        insertChild(StmtNode, chd1);
        insertChild(StmtNode, newSTokNode(RpK));
        insertChild(StmtNode, chd2);
        if (chd3 != NULL)
        {
            insertChild(StmtNode, newSTokNode(ElK));
            insertChild(StmtNode, chd3);
        }
    }
    return StmtNode;
}

TreeNode *createDefLtNode(TreeNode *chd1, TreeNode *chd2)
{
    int nline = 0;
    if (chd1 != NULL)
    {
        nline = chd1->lineno;
    }
    TreeNode *DefLtNode = newLcDNode(DefLt, nline);
    if (chd1 == NULL)
    {
        insertChild(DefLtNode, newNull());
    }
    else
    {
        insertChild(DefLtNode, chd1);
        insertChild(DefLtNode, chd2);
    }
    return DefLtNode;
}

TreeNode *createDfNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *DfNode = newLcDNode(Df, chd1->lineno);
    insertChild(DfNode, chd1);
    insertChild(DfNode, chd2);
    insertChild(DfNode, newSTokNode(SemiK));
    return DfNode;
}

TreeNode *createDecLtNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *DecLtNode = newLcDNode(DecLt, chd1->lineno);
    insertChild(DecLtNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(DecLtNode, newSTokNode(ComK));
        insertChild(DecLtNode, chd2);
    }
    return DecLtNode;
}

TreeNode *createDcNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *DcNode = newLcDNode(Dc, chd1->lineno);
    insertChild(DcNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(DcNode, newSTokNode(AssignK));
        insertChild(DcNode, chd2);
    }
    return DcNode;
}

TreeNode *createBiExpNode(TreeNode *chd1, TreeNode *chd2, ToKind tokind,char* relname)
{
    TreeNode *ExpNode = newExpNode(Exp, chd1->lineno);

    ExpNode->expType = BiExp;
    insertChild(ExpNode, chd1);
    TreeNode* stokNode = newSTokNode(tokind);
    stokNode->attr.rel = relname;
    // if(stokNode->attr.rel!=NULL){printf("%s\n",stokNode->attr.rel);}
    // insertChild(ExpNode, newSTokNode(tokind));
    insertChild(ExpNode, stokNode);
    insertChild(ExpNode, chd2);

    return ExpNode;
}

TreeNode *createUnaExpNode(TreeNode *chd, ToKind tokind, int nline)
{
    TreeNode *ExpNode = newExpNode(Exp, nline);

    ExpNode->expType = UnaExp;
    insertChild(ExpNode, newSTokNode(tokind));
    insertChild(ExpNode, chd);
    if (tokind == LpK)
    {
        insertChild(ExpNode, newSTokNode(RpK));
    }
    return ExpNode;
}

TreeNode *createFuncExpNode(TreeNode *chd, char *name, int nline)
{
    TreeNode *ExpNode = newExpNode(Exp, nline);

    ExpNode->expType = FuncExp;
    insertChild(ExpNode, newIdTokNode(name));
    insertChild(ExpNode, newSTokNode(LpK));
    if (chd != NULL)
    {
        insertChild(ExpNode, chd);
    }
    insertChild(ExpNode, newSTokNode(RpK));

    return ExpNode;
}

TreeNode *createArrExpNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *ExpNode = newExpNode(Exp, chd1->lineno);

    ExpNode->expType = ArrExp;
    insertChild(ExpNode, chd1);
    insertChild(ExpNode, newSTokNode(LbK));
    insertChild(ExpNode, chd2);
    insertChild(ExpNode, newSTokNode(RbK));
    return ExpNode;
}

TreeNode *createStruExpNode(TreeNode *chd, char *name)
{
    TreeNode *ExpNode = newExpNode(Exp, chd->lineno);

    ExpNode->expType = StruExp;
    insertChild(ExpNode, chd);
    insertChild(ExpNode, newSTokNode(DoK));
    insertChild(ExpNode, newIdTokNode(name));
    return ExpNode;
}

TreeNode *createIdExpNode(char *name, int nline)
{
    TreeNode *ExpNode = newExpNode(Exp, nline);

    ExpNode->expType = idExp;
    insertChild(ExpNode, newIdTokNode(name));

    return ExpNode;
}

TreeNode *createIntExpNode(int num, int nline)
{
    TreeNode *ExpNode = newExpNode(Exp, nline);

    ExpNode->expType = IntExp;
    insertChild(ExpNode, newIntNode(num));
    return ExpNode;
}

TreeNode *createFloExpNode(float fnum, int nline)
{
    TreeNode *ExpNode = newExpNode(Exp, nline);

    ExpNode->expType = FloatExp;
    insertChild(ExpNode, newFloatNode(fnum));
    return ExpNode;
}

TreeNode *createArgsNode(TreeNode *chd1, TreeNode *chd2)
{
    TreeNode *ArgsNode = newExpNode(Ags, chd1->lineno);
    insertChild(ArgsNode, chd1);
    if (chd2 != NULL)
    {
        insertChild(ArgsNode, newSTokNode(ComK));
        insertChild(ArgsNode, chd2);
    }
    return ArgsNode;
}

TreeNode *createErrorNode()
{
    return newNode(0);
}

void printTree(TreeNode *root, int indent)
{
    if (root == NULL || root->nodeKind == NullK)
    {
        return;
    }
    if (root->firstChild == NULL || root->firstChild->nodeKind != NullK)
    {
        for (int i = 0; i < indent; i++)
        {
            fprintf(stdout, " ");
        }
        if (root->nodeKind == HlDef)
        {
            switch (root->subkind.hKind)
            {
            case Prog:
                fprintf(stdout, "Program ");
                break;
            case ExtDefLt:
                fprintf(stdout, "ExtDefList ");
                break;
            case ExtD:
                fprintf(stdout, "ExtDef ");
                break;
            case ExtDecLt:
                fprintf(stdout, "ExtDecList ");
                break;
            }
            fprintf(stdout, "(%d)\n", root->lineno);
        }
        if (root->nodeKind == Spec)
        {
            switch (root->subkind.sKind)
            {
            case Specif:
                fprintf(stdout, "Specifier ");
                break;
            case StrSpec:
                fprintf(stdout, "StructSpecifier ");
                break;
            case Opt:
                fprintf(stdout, "OptTag ");
                break;
            case TagK:
                fprintf(stdout, "Tag ");
                break;
            }
            fprintf(stdout, "(%d)\n", root->lineno);
        }
        if (root->nodeKind == Decl)
        {
            switch (root->subkind.dKind)
            {
            case VDec:
                fprintf(stdout, "VarDec ");
                break;
            case FDec:
                fprintf(stdout, "FunDec ");
                break;
            case VList:
                fprintf(stdout, "VarList ");
                break;
            case ParD:
                fprintf(stdout, "ParamDec ");
                break;
            }
            fprintf(stdout, "(%d)\n", root->lineno);
        }
        if (root->nodeKind == Stmts)
        {
            switch (root->subkind.stKind)
            {
            case CpSt:
                fprintf(stdout, "CompSt ");
                break;
            case StmtLt:
                fprintf(stdout, "StmtList ");
                break;
            case Stmt:
                fprintf(stdout, "Stmt ");
                break;
            }
            fprintf(stdout, "(%d)\n", root->lineno);
        }
        if (root->nodeKind == LcDef)
        {
            switch (root->subkind.lKind)
            {
            case DefLt:
                fprintf(stdout, "DefList ");
                break;
            case Df:
                fprintf(stdout, "Def ");
                break;
            case DecLt:
                fprintf(stdout, "DecList ");
                break;
            case Dc:
                fprintf(stdout, "Dec ");
                break;
            }
            fprintf(stdout, "(%d)\n", root->lineno);
        }
        if (root->nodeKind == Exps)
        {
            switch (root->subkind.eKind)
            {
            case Exp:
                fprintf(stdout, "Exp ");
                break;
            case Ags:
                fprintf(stdout, "Args ");
                break;
            }
            fprintf(stdout, "(%d)\n", root->lineno);
        }
        if (root->nodeKind == IntTok)
        {
            fprintf(stdout, "INT: %d\n", root->attr.val);
        }
        if (root->nodeKind == FloTok)
        {
            fprintf(stdout, "FLOAT: %f\n", root->attr.fval);
        }
        if (root->nodeKind == IdTok)
        {
            fprintf(stdout, "ID: %s\n", root->attr.id);
        }
        if (root->nodeKind == TypeTok)
        {
            switch (root->subkind.tyKind)
            {
            case IntK:
                fprintf(stdout, "TYPE: int\n");
                break;
            case FloatK:
                fprintf(stdout, "TYPE: float\n");
                break;
            }
        }
        if (root->nodeKind == StrTok)
        {
            switch (root->subkind.tKind)
            {
            case SemiK:
                fprintf(stdout, "SEMI\n");
                break;
            case ComK:
                fprintf(stdout, "COMMA\n");
                break;
            case AssignK:
                fprintf(stdout, "ASSIGNOP\n");
                break;
            case RelK:
                fprintf(stdout, "RELOP\n");
                break;
            case PlK:
                fprintf(stdout, "PLUS\n");
                break;
            case MiK:
                fprintf(stdout, "MINUS\n");
                break;
            case StK:
                fprintf(stdout, "STAR\n");
                break;
            case DiK:
                fprintf(stdout, "DIV\n");
                break;
            case AnK:
                fprintf(stdout, "AND\n");
                break;
            case OrK:
                fprintf(stdout, "OR\n");
                break;
            case DoK:
                fprintf(stdout, "DOT\n");
                break;
            case NoK:
                fprintf(stdout, "NOT\n");
                break;
            case LpK:
                fprintf(stdout, "LP\n");
                break;
            case RpK:
                fprintf(stdout, "RP\n");
                break;
            case LbK:
                fprintf(stdout, "LB\n");
                break;
            case RbK:
                fprintf(stdout, "RB\n");
                break;
            case LcK:
                fprintf(stdout, "LC\n");
                break;
            case RcK:
                fprintf(stdout, "RC\n");
                break;
            case StruK:
                fprintf(stdout, "STRUCT\n");
                break;
            case RetK:
                fprintf(stdout, "RETURN\n");
                break;
            case IfK:
                fprintf(stdout, "IF\n");
                break;
            case ElK:
                fprintf(stdout, "ELSE\n");
                break;
            case WhK:
                fprintf(stdout, "WHILE\n");
                break;
            }
        }
    }
    printTree(root->firstChild, indent + 2);
    printTree(root->nextSibling, indent);
}