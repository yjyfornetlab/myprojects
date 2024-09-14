%{
    #include <stdio.h>
    #include "tree.h"
    int corr = 1;
    int last_error = 0;
    extern TreeNode* parseTree;
%}
%locations

%union{
    int num;
    float fnum;
    char* id;
    char* ty;
    char* rel;
    struct TreeNode* treeNode;
}

%token <num> INT
%token <fnum> FLOAT
%token <id> ID
%token SEMI
%token COMMA
%token ASSIGNOP
%token <rel> RELOP
%token PLUS MINUS STAR DIV
%token AND OR
%token DOT
%token NOT
%token <ty> TYPE
%token LP RP
%token LB RB
%token LC RC
%token STRUCT
%token RETURN
%token IF ELSE
%token WHILE

%type <treeNode> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec
%type <treeNode> FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT
%left LP RP LB RB DOT
%%
Program : ExtDefList {$$ = createProgNode($1); parseTree = $$;}
;
ExtDefList : ExtDef ExtDefList {$$ = createExtDefLtNode($1,$2);}
| {$$ = createExtDefLtNode(NULL,NULL);}
;
ExtDef : Specifier ExtDecList SEMI {$$ = createExtDNode($1,$2,NULL);}
| Specifier SEMI {$$ = createExtDNode($1,NULL,NULL);}
| Specifier FunDec CompSt {$$ = createExtDNode($1,$2,$3);}
| error SEMI {$$ = createErrorNode();}
;
ExtDecList : VarDec {$$ = createExtDecLtNode($1,NULL);}
| VarDec COMMA ExtDecList {$$ = createExtDecLtNode($1,$3);}
;
Specifier : TYPE {$$ = createSpecifNode(NULL,$1,@1.first_line);}
| StructSpecifier {$$ = createSpecifNode($1,NULL,$1->lineno);}
;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = createStrSpecNode($2,$4,@1.first_line);}
| STRUCT Tag {$$ = createStrSpecNode($2,NULL,@1.first_line);}
;
OptTag : ID {$$ = createOptNode($1,@1.first_line);}
| {$$ = createOptNode(NULL,0);}
;
Tag : ID {$$ = createTagNode($1,@1.first_line);}
;
VarDec : ID {$$ = createVDecNode(NULL,0,$1,@1.first_line);}
| VarDec LB INT RB {$$ = createVDecNode($1,$3,NULL,$1->lineno);}
;
FunDec : ID LP VarList RP {$$ = createFDecNode($3,$1,@1.first_line);}
| ID LP RP {$$ = createFDecNode(NULL,$1,@1.first_line);}
| error RP {$$ = createErrorNode();}
;
VarList : ParamDec COMMA VarList {$$ = createVListNode($1,$3);}
| ParamDec {$$ = createVListNode($1,NULL);}
;
ParamDec : Specifier VarDec {$$ = createParDNode($1,$2);}
;
CompSt : LC DefList StmtList RC {$$ = createCpStNode($2,$3,@1.first_line);}
| error RC {$$ = createErrorNode();}
| error SEMI {$$ = createErrorNode();}
;
StmtList : Stmt StmtList {$$ = createStmtLtNode($1,$2);}
| {$$ = createStmtLtNode(NULL,NULL);}
;
Stmt : Exp SEMI {$$ = createStmtNode(ExpStmt,$1,NULL,NULL,$1->lineno);}
| CompSt {$$ = createStmtNode(CompStStmt,$1,NULL,NULL,$1->lineno);}
| RETURN Exp SEMI {$$ = createStmtNode(RetStmt,$2,NULL,NULL,@1.first_line);}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = createStmtNode(IfStmt,$3,$5,NULL,@1.first_line);}
| IF LP Exp RP Stmt ELSE Stmt {$$ = createStmtNode(IfStmt,$3,$5,$7,@1.first_line);}
| WHILE LP Exp RP Stmt {$$ = createStmtNode(WhileStmt,$3,$5,NULL,@1.first_line);}
| error SEMI {$$ = createErrorNode();}
;
DefList : Def DefList {$$ = createDefLtNode($1,$2);}
| {$$ = createDefLtNode(NULL,NULL);}
;
Def : Specifier DecList SEMI {$$ = createDfNode($1,$2);}
| error SEMI {$$ = createErrorNode();}
;
DecList : Dec {$$ = createDecLtNode($1,NULL);}
| Dec COMMA DecList {$$ = createDecLtNode($1,$3);}
;
Dec : VarDec {$$ = createDcNode($1,NULL);}
| VarDec ASSIGNOP Exp {$$ = createDcNode($1,$3);}
;
Exp : Exp ASSIGNOP Exp {$$ = createBiExpNode($1,$3,AssignK,NULL);}
| Exp AND Exp {$$ = createBiExpNode($1,$3,AnK,NULL);}
| Exp OR Exp {$$ = createBiExpNode($1,$3,OrK,NULL);}
| Exp RELOP Exp {$$ = createBiExpNode($1,$3,RelK,$2);}
| Exp PLUS Exp {$$ = createBiExpNode($1,$3,PlK,NULL);}
| Exp MINUS Exp {$$ = createBiExpNode($1,$3,MiK,NULL);}
| Exp STAR Exp {$$ = createBiExpNode($1,$3,StK,NULL);}
| Exp DIV Exp {$$ = createBiExpNode($1,$3,DiK,NULL);}
| LP Exp RP {$$ = createUnaExpNode($2,LpK,@1.first_line);}
| MINUS Exp %prec UMINUS {$$ = createUnaExpNode($2,MiK,@1.first_line);}
| NOT Exp {$$ = createUnaExpNode($2,NoK,@1.first_line);}
| ID LP Args RP {$$ = createFuncExpNode($3,$1,@1.first_line);}
| ID LP RP {$$ = createFuncExpNode(NULL,$1,@1.first_line);}
| Exp LB Exp RB {$$ = createArrExpNode($1,$3);}
| Exp DOT ID {$$ = createStruExpNode($1,$3);}
| ID {$$ = createIdExpNode($1,@1.first_line);}
| INT {$$ = createIntExpNode($1,@1.first_line);}
| FLOAT {$$ = createFloExpNode($1,@1.first_line);}
;
Args : Exp COMMA Args {$$ = createArgsNode($1,$3);}
| Exp {$$ = createArgsNode($1,NULL);}
;
%%
#include "lex.yy.c"
yyerror(char* msg)
{
    corr = 0;
    if(last_error==0 || last_error!=yylloc.first_line)
    {
        fprintf(stdout,"Error type B at Line %d: Syntax error\n",yylloc.first_line);
        last_error = yylloc.first_line;
    }
}