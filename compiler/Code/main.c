#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "semanalysis.h"
#include "cmmtype.h"
#include "translate.h"
#include "objgen.h"

TreeNode *parseTree;
extern int corr;
int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 0;
    }
    yyrestart(f);
    yyparse();

    if (corr)
    {
        // printTree(parseTree, 0);
        symTable = newSymTable();
        intTy = (Type)malloc(sizeof(Type_));
        intTy->kind = BASIC;
        intTy->u.basic = 0;
        intTy->width = 4;

        floatTy = (Type)malloc(sizeof(Type_));
        floatTy->kind = BASIC;
        floatTy->u.basic = 1;
        floatTy->width = 8;

        readfunc = (SymNode *)malloc(sizeof(SymNode));
        readfunc->sname = "read";
        readfunc->symKind = FuncDef;
        readfunc->val.funcinfo.rettype = intTy;
        readfunc->val.funcinfo.argnum = 0;
        readfunc->val.funcinfo.paramlist = NULL;

        writefunc = (SymNode *)malloc(sizeof(SymNode));
        writefunc->sname = "write";
        writefunc->symKind = FuncDef;
        writefunc->val.funcinfo.rettype = intTy;
        writefunc->val.funcinfo.argnum = 1;
        ParamList *param = (ParamList *)malloc(sizeof(ParamList));
        param->argty = intTy;
        param->next = NULL;
        writefunc->val.funcinfo.paramlist = param;
        insertSymNode(symTable, readfunc);
        insertSymNode(symTable, writefunc);

        lno = 0;
        tno = 0;
        vno = 0;
        
        vlen = 0;
        tlen = 0;
        istransable = 1;

        analyse(parseTree);

        if (istransable)
        {
            translate(NULL, parseTree);
            if (argc <= 2)
            {
                generate(NULL);
            }
            else
            {
                generate(argv[2]);
            }
        }
    }
    return 0;
}