/** calculator calc.l
 * Pezzè Alessandro 158629
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * edited by Alessandro Pezzè 
 */

%{
    #include <stdlib.h>
    #include <strings.h>
    #include <string.h>
    #include "calc.h" 
    #include "y.tab.h"


    void yyerror(char *);
%}

%%
"true"          return TRUE; 
"false"         return FALSE;
">="            return GE;
"<="            return LE;
"=="            return EQ;
"==="           return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"printInt"      return PRINTINT;
"printReal"     return PRINTREAL;
"for"           return FOR;
"to"            return TO;
"not"           return NOT;
"or"            return OR;
"and"           return AND;
int             return INT; 
float           return REAL;
real            return REAL;
bool            return BOOL; 
main            return MAIN;
\|\|            return OR;
\&\&            return AND;
[a-zA-Z][a-zA-Z_]*   {
                        symrec * s;
                        char * variableName = malloc(strlen(yytext));
                        strcpy(variableName,yytext);
                        s = getsym(variableName);
                        if (s == 0){
                          s =(symrec *) malloc (sizeof (symrec));
                          s->name = (char *) malloc(strlen(yytext));
                          strcpy (s->name,yytext);
                         }
                         *((symrec**)&yylval) = s;
                         return VARIABLE;
                    }
0               {
                    yylval.iValue = atoi(yytext);
                    return INTEGER;
                }
[1-9][0-9]*     {
                    yylval.iValue = atoi(yytext);
                    return INTEGER;
                }
([0-9]+\,)[0-9]+   {  int i;
                      for (i = 0; i < strlen(yytext); i += 1)
                        {
                        if (yytext[i] == ',')
                          yytext[i] = '.';
                        }
                        
                      yylval.iValue = atof(yytext);
                      return REALNUMBER;
                    }

[-+<>=*/.]   { return *yytext; }




[ \t\n]         ;/* skip whitespace */
\;              return SEMICOLON;
\,              return COMMA;
\(              return LP;
\)              return RP;
\{              return LCURLY;
\}              return RCURLY;
.               {ECHO;
                yyerror("Previous char is unknown to lexer");}

%%

int yywrap(void) {
    return 1;
}