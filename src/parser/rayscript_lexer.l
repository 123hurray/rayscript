%option noyywrap
%option yylineno
%option noyyalloc noyyfree
%{
#include <config.h>
#include <string.h>
#include <statement.h>
#include <rayscript.tab.h>
#include <object.h>
#include <allocator.h>

#define yyalloc(s) _R_MALLOC(s)
#define yyfree(p) _R_FREE(p)
    
#ifdef DEBUG_LEXER

YYSTYPE yylval;

#endif
extern int fileno(FILE*);
#ifdef HAVE_EDITLINE
#include <editline/readline.h>
#define YY_INPUT(buf, result, max_size) result = mygetinput(buf, max_size);
extern int line_status;
static int mygetinput(char *buf, int max_size) {
    if(is_interactive) {
        char *line;
        if (feof(yyin)) {
            return YY_NULL;
        }
        if(line_status == 0) {
            line = readline(">>>");
        } else {
            line = readline("...");
        }
        if(!line) {
            return YY_NULL;
        }
        if(strlen(line) > max_size - 2) {
            R_ERROR("input line too long\n");
            return YY_NULL; 
        }
        sprintf(buf,"%s\n",line);
        add_history(line);
        free(line);
        return strlen(buf);
    } else {
        // Copy from default YY_INPUT
        if(YY_CURRENT_BUFFER_LVALUE->yy_is_interactive) {
            int c = '*';
            size_t n;
            for (n = 0; n < max_size &&
                    (c = getc( yyin )) != EOF && c != '\n'; ++n) {
                buf[n] = (char) c;
            }
            if(c == '\n') {
                buf[n++] = (char) c;
            }
            if(c == EOF && ferror( yyin )) {
                R_FATAL("input in flex scanner failed");
            }
            return n;
        }
        else {
            errno=0;
            int result;
            while ((result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin)) {
                if(errno != EINTR) {
                    R_FATAL( "input in flex scanner failed" );
                    break;
                }
                errno=0;
                clearerr(yyin);
            }
            return result;
        }
    }
}   
#endif
%}

%%
"true"                               { 
    INC_REF(p_bool_true);
    yylval.b = p_bool_true;    
    return BOOL_TOKEN;
}
"false"                              {
    INC_REF(p_bool_false);
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
    char *str = R_MALLOC_N(char, strlen(yytext) + 1);
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
