%{
#include "statement.h"
#include "rayscript.tab.h"
%}

%%
"+"                                  { return ADD;    } 
"-"                                  { return SUB;    } 
"*"                                  { return MUL;    } 
"/"                                  { return DIV;    } 
"("                                  { return OP;     }
")"                                  { return CP;     }
\n                                   { return EOL;    } 
[ t]                                 { /* ignore */   } 
([0-9]*\.?[0-9]+|[0-9]+\.[0-9]*)     { 
    double *v = (double *)malloc(sizeof(double));
    *v  =atof(yytext);
    factor_node *f = (factor_node*)malloc(sizeof(factor_node));
    f->type = NUM;
    f->val = v;
    syn_node *s = (syn_node*)malloc(sizeof(syn_node));
    s->type = FACTOR;
    s->factor = f; 
    yylval = s;
    
return NUMBER; } 


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
