%option noyywrap
%option yylineno
%{
#include <config.h>
#include <string.h>
#include <statement.h>
#include <rayscript.tab.h>
#include <object.h>
#include <allocator.h>
#ifdef DEBUG_LEXER

YYSTYPE yylval;

#endif
extern int fileno(FILE*);
#include <editline/readline.h>
#define YY_INPUT(buf, result, max_size) result = mygetinput(buf, max_size);
extern int line_status;
static int mygetinput(char *buf, int size) {
    char *line;
    if (feof(yyin))  return YY_NULL;
    if(line_status == 0) {
        line = readline(">>>");
    } else {
        line = readline("...");
    }
    if(!line)        return YY_NULL;
    if(strlen(line) > size-2){
        fprintf(stderr,"input line too long\n"); return YY_NULL; }
    sprintf(buf,"%s\n",line);
    add_history(line);
    free(line);
    return strlen(buf);
}   
%}

%%
"true"                               { 
    yylval.b = p_bool_true;    
    return BOOL_TOKEN;
}
"false"                              {
    yylval.b = p_bool_false;
    return BOOL_TOKEN;
}
"nil"                                { return NIL_TOKEN;}
"for"                                { return FOR_TOKEN;}
"from"                               { return FROM_TOKEN;}
"to"                                 { return TO_TOKEN;}
"step"                               { return STEP_TOKEN;    }
"loop"                               { return LOOP_TOKEN;}
"if"                                 { return IF_TOKEN;}
"else"                               { return ELSE_TOKEN;}
"print"                              { return PRINT_TOKEN;}
"->"                                 { return ARROW_TOKEN;}
","                                  { return COMMA;     }
"and"                                { return AND_TOKEN; }
"or"                                 { return OR_TOKEN;  }
"=="                                 { yylval.op = OP_TYPE_EQ;return EQUALITY_TOKEN;}
"!="                                 { yylval.op = OP_TYPE_NE;return EQUALITY_TOKEN;}
">"                                  { yylval.op = OP_TYPE_GT; return RELATIONAL_TOKEN;}
"<"                                  { yylval.op = OP_TYPE_LT; return RELATIONAL_TOKEN;}
"<="                                  { yylval.op = OP_TYPE_LE; return RELATIONAL_TOKEN;}
">="                                  { yylval.op = OP_TYPE_GE; return RELATIONAL_TOKEN;}
"{"                                  { return LB;     }
"}"                                  { return RB;     }
";"                                  { return SC;     }

"+"                                  { yylval.op = OP_TYPE_ADD;return OPERATOR_1;    } 
"-"                                  { yylval.op = OP_TYPE_SUB; return OPERATOR_1;   } 
"*"                                  { yylval.op = OP_TYPE_MUL;return OPERATOR_2;    } 
"/"                                  { yylval.op = OP_TYPE_DIV;return OPERATOR_2;    } 
"("                                  { return OP;     }
")"                                  { return CP;     }
\n                                   {
        return EOL;
}
"="                                  { return ASSIGN_TOKEN; }
[ t]                                 { /* ignore */   } 

([a-zA-Z_][a-zA-Z0-9]*)                      {
    char *str = R_MALLOC_N(char, strlen(yytext));
    strcpy(str, yytext);
    yylval.str = str; 
    return IDENTIFIER;
}
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)     { 
    yylval.fnum = atof(yytext);
    return FLOAT_TOKEN;
} 
([1-9][0-9]*|[0-9]) {
    yylval.inum = atoi(yytext);
    return INT_TOKEN;
}
%%


#ifdef DEBUG_LEXER 
int main (int argc, char** argv) {
    int token;
    while ((token = yylex()) != 0) {
        printf("%d\n", token);
    }
    return 0;
}
#endif 
