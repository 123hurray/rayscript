%option noyywrap
%option yylineno
%{
#include <config.h>
#include <string.h>
#include <statement.h>
#include <rayscript.tab.h>
#include <object.h>
extern int fileno(FILE*);
%}

%%
"if"                                 { return IF_TOKEN;}
"else"                               { return ELSE_TOKEN;}
"print"                              { return PRINT_TOKEN;}
"and"                                { return AND_TOKEN; }
"or"                                 { return OR_TOKEN;  }
"=="                                 { return EQUALS_TOKEN;}
"{"                                  { return LB;     }
"}"                                  { return RB;     }
";"                                  { return SC;     }

"+"                                  { yylval.op = OP_ADD;return OPERATOR_1;    } 
"-"                                  { yylval.op = OP_SUB; return OPERATOR_1;   } 
"*"                                  { yylval.op = OP_MUL;return OPERATOR_2;    } 
"/"                                  { yylval.op = OP_DIV;return OPERATOR_2;    } 
"("                                  { return OP;     }
")"                                  { return CP;     }
\n                                   {
        return EOL;
}
"="                                  { return ASSIGN; }
[ t]                                 { /* ignore */   } 

([a-zA-Z_][a-zA-Z0-9]*)                      {
    char *str = (char *)malloc(strlen(yytext));
    strcpy(str, yytext);
    yylval.str = str; 
    return IDENTIFIER;
}
([0-9]*\.?[0-9]+|[0-9]+\.[0-9]*)     { 
    yylval.fnum = atof(yytext);
    return FLOAT_TOKEN;
} 
([0-9]?) {
    yylval.inum = atoi(yytext);
    return FLOAT_TOKEN;
}
%%


#ifdef CALC_LEX  
int main (int argc, char** argv) {
    int token;


    while (token = yylex()) {
        printf("%d\n", token);
    }


    return 0;
}
#endif 
