#include "cmmtype.h"

#ifndef _TREE_H
#define _TREE_H
typedef enum
{
    HlDef,
    Spec,
    Decl,
    Stmts,
    LcDef,
    Exps,
    NullK,
    IntTok,
    FloTok,
    StrTok,
    IdTok,
    TypeTok
} NodeKind;
typedef enum
{
    Prog,
    ExtDefLt,
    ExtD,
    ExtDecLt
} HlDefKind;
typedef enum
{
    Specif,
    StrSpec,
    Opt,
    TagK
} SpecKind;
typedef enum
{
    VDec,
    FDec,
    VList,
    ParD
} DeclKind;
typedef enum
{
    CpSt,
    StmtLt,
    Stmt
} StmtsKind;
typedef enum
{
    DefLt,
    Df,
    DecLt,
    Dc
} LcDefKind;
typedef enum
{
    Exp,
    Ags
} ExpsKind;
typedef enum
{
    SemiK,
    ComK,
    AssignK,
    RelK,
    PlK,
    MiK,
    StK,
    DiK,
    AnK,
    OrK,
    DoK,
    NoK,
    // TyK,
    LpK,
    RpK,
    LbK,
    RbK,
    LcK,
    RcK,
    StruK,
    RetK,
    IfK,
    ElK,
    WhK,
    OtherK
} ToKind;
typedef enum
{
    IntK,
    FloatK
} TypeKind;

typedef enum
{
    ExpStmt,
    CompStStmt,
    RetStmt,
    IfStmt,
    WhileStmt
} StmtType;
typedef enum
{
    BiExp,
    UnaExp,
    FuncExp,
    ArrExp,
    StruExp,
    IntExp,
    FloatExp,
    idExp
} ExpType;

typedef struct TreeNode
{
    NodeKind nodeKind;
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
    int lineno;
    ExpType expType;
    StmtType stmtType;
    Type synty;
    Type inhty;
    Type retty;
    int visited;
    int translated;
    int ispar;
    union
    {
        HlDefKind hKind;
        SpecKind sKind;
        DeclKind dKind;
        StmtsKind stKind;
        LcDefKind lKind;
        ExpsKind eKind;
        ToKind tKind;
        TypeKind tyKind;
    } subkind;

    union
    {
        int val;
        float fval;
        char *id;
        char* rel;
    } attr;
} TreeNode;

TreeNode *createProgNode(TreeNode *);
TreeNode *createExtDefLtNode(TreeNode *, TreeNode *);
TreeNode *createExtDNode(TreeNode *, TreeNode *, TreeNode *);
TreeNode *createExtDecLtNode(TreeNode *, TreeNode *);

TreeNode *createSpecifNode(TreeNode *, char *, int);
TreeNode *createStrSpecNode(TreeNode *, TreeNode *, int);
TreeNode *createOptNode(char *, int);
TreeNode *createTagNode(char *, int);

TreeNode *createVDecNode(TreeNode *, int, char *, int);
TreeNode *createFDecNode(TreeNode *, char *, int);
TreeNode *createVListNode(TreeNode *, TreeNode *);
TreeNode *createParDNode(TreeNode *, TreeNode *);

TreeNode *createCpStNode(TreeNode *, TreeNode *, int);
TreeNode *createStmtLtNode(TreeNode *, TreeNode *);
TreeNode *createStmtNode(StmtType, TreeNode *, TreeNode *, TreeNode *, int);

TreeNode *createDefLtNode(TreeNode *, TreeNode *);
TreeNode *createDfNode(TreeNode *, TreeNode *);
TreeNode *createDecLtNode(TreeNode *, TreeNode *);
TreeNode *createDcNode(TreeNode *, TreeNode *);

TreeNode *createBiExpNode(TreeNode *, TreeNode *, ToKind,char*);
TreeNode *createUnaExpNode(TreeNode *, ToKind, int);
TreeNode *createFuncExpNode(TreeNode *, char *, int);
TreeNode *createArrExpNode(TreeNode *, TreeNode *);
TreeNode *createStruExpNode(TreeNode *, char *);
TreeNode *createIdExpNode(char *, int);
TreeNode *createIntExpNode(int, int);
TreeNode *createFloExpNode(float, int);
TreeNode *createArgsNode(TreeNode *, TreeNode *);
TreeNode *createErrorNode();

void printTree(TreeNode *, int);

#endif
