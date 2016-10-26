%{
#include <string.h>
#include <statement.h>
#include <rayscript.tab.h>
#include <object.h>
%}

%%
"if"                                 { return IF_TOKEN;}
"else"                               { return ELSE_TOKEN;}
"print"                              { return PRINT_TOKEN;}
"{"                                  { return LB;     }
"}"                                  { return RB;     }
";"                                  { return SC;     }

"+"                                  { return ADD;    } 
"-"                                  { return SUB;    } 
"*"                                  { return MUL;    } 
"/"                                  { return DIV;    } 
"("                                  { return OP;     }
")"                                  { return CP;     }
\n                                   { /* ignore */    } 
"="                                  { return ASSIGN; }
[ t]                                 { /* ignore */   } 
([0-9]*\.?[0-9]+|[0-9]+\.[0-9]*)     { 
    factor_node *f = (factor_node*)malloc(sizeof(factor_node));
    f->ast_type = FACTOR;
    f->type = NUM;
    f->val = (ray_object*)new_number_object(atof(yytext));
    yylval = (ast_node *)f;
    return NUMBER; 
} 

([a-zA-Z_][a-zA-Z0-9]*)                      {
    char *str = (char *)malloc(strlen(yytext));
    strcpy(str, yytext);
    identifier_node* e = (identifier_node *)malloc(sizeof(identifier_node));
    e->ast_type = ID;
    e->val = str;
    yylval = (ast_node *)e;
    return IDENTIFIER;
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
