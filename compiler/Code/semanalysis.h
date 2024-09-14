#include "tree.h"
#include "hashsymtable.h"

#ifndef _SEMANALYSIS_H
#define _SEMANALYSIS_H

SymTable* symTable;
Type intTy;
Type floatTy;
int istransable;

void analyse(TreeNode*);
void analyseExtDef(TreeNode*);
void analyseExtDecLt(TreeNode*);
void analyseVarDec(TreeNode*);
void analyseSpec(TreeNode*);
FieldList analyseDefLt(TreeNode*,int,SymTable*);
FieldList analyseDef(TreeNode*,int,SymTable*);
FieldList analyseDecLt(TreeNode*,int,SymTable*);
void analyseDec(TreeNode*,int);
void analyseFunDec(TreeNode*);
void analyseCompSt(TreeNode*);
void analyseStmtLt(TreeNode*);
void analyseStmt(TreeNode*);
void analyseVarList(TreeNode*,SymNode*);
SymNode* analyseParamDec(TreeNode*);
void analyseExp(TreeNode*);

#endif
