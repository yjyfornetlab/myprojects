%{
    #include "syntax.tab.h"
    #include <stdlib.h>
    #include <string.h>
    int yycolumn = 1;
    extern int corr;
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;
%}
%option yylineno

digit [0-9]
letter [_a-zA-Z]

%%
([1-9]+{digit}*)|0 {yylval.num = atoi(yytext); return INT;}
{digit}+\.{digit}+ {yylval.fnum = atof(yytext); return FLOAT;}
(({digit}*\.{digit}+)|({digit}+\.))[eE][+-]?{digit}+ {yylval.fnum = atof(yytext); return FLOAT;}
";" {return SEMI;}
"," {return COMMA;}
"=" {return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" {yylval.rel = (char*)malloc(sizeof(char)*(strlen(yytext)+1)); strcpy(yylval.rel,yytext);return RELOP;}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return STAR;}
"/" {return DIV;}
"&&" {return AND;}
"||" {return OR;}
"." {return DOT;}
"!" {return NOT;}
"int"|"float" {yylval.ty = (char*)malloc(sizeof(char)*(strlen(yytext)+1)); strcpy(yylval.ty,yytext); return TYPE;}
"(" {return LP;}
")" {return RP;}
"[" {return LB;}
"]" {return RB;}
"{" {return LC;}
"}" {return RC;}
"struct" {return STRUCT;}
"return" {return RETURN;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;}
{letter}[a-zA-Z0-9_]* {yylval.id = (char*)malloc(sizeof(char)*(strlen(yytext)+1)); strcpy(yylval.id,yytext); return ID;}
" " {}
\n {}
\t {}
. {
    corr = 0;
    fprintf(stdout,"Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);
}
%%