/*
 * Pezzè Alessandro 158629
 * origin source by Tom Niemann at epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * edited by Alessandro Pezzè 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"
#include "y.tab.h"

extern program * prg;

void executeProgram(program * program, symrec * symTable){
    ex(program->commandList, symTable);
    freeNode(program->commandList);
    printf("\n");
}

float ex(nodeType *p, symrec * symTable) {
    if (!p) return 0;
    switch(p->type) {
        case typeCon:       {
                                return p->con.value;
                            }
        case typeId:        {
                                symrec * s= getsymTable(p->id.name, symTable);
                                if(s == NULL){
                                    printf("there is not such '%s' varibale in the symtable\n", p->id.name);
                                    exit(0);
                                }
                                return s->value;
                            }
        case typeOpr:
            switch(p->opr.oper) {
                case WHILE:
                    while(ex(p->opr.op[0], symTable))
                    {
                        ex(p->opr.op[1], symTable);
                    }
                    return 0;
                case FOR:
                    {   symrec * s= getsymTable(p->opr.op[0]->id.name, symTable);
                        s->value = ex(p->opr.op[1], symTable);
                        while(s->value - ex(p->opr.op[2], symTable)){
                            ex(p->opr.op[3], symTable);
                            s->value++;
                        }
                        return 0;
                    }
                case IF:
                    if (ex(p->opr.op[0], symTable)){
                        
                        ex(p->opr.op[1], symTable);
                    }
                    else if (p->opr.nops > 2)
                    {
                        ex(p->opr.op[2], symTable);
                    }
                    return 0;

                
                case PRINT:     {   
                                    int conType =  p->opr.op[0]->con.tipoInserito;                                    
                                    
                                    if (conType == BOOLTYPE || (conType > 250 &&conType< 280)){ //sono gli id dei token
                                        if (ex(p->opr.op[0], symTable) == 0){
                                            printf("false\n");
                                        }
                                        else{
                                            printf("true\n");
                                        }
                                    }
                                    else{
                                        symrec * s= getsymTable(p->opr.op[0]->id.name, symTable);
                                        

                                        if(s->tipo == INTTYPE)
                                            printf("%d\n", (int)ex(p->opr.op[0], symTable));
                                        else if (s->tipo == REALTYPE) 
                                            printf("%.3f\n", ex(p->opr.op[0], symTable));
                                        else if (s->tipo == BOOLTYPE)
                                            {
                                            if (ex(p->opr.op[0], symTable) == 0)
                                                printf("false\n");
                                            else
                                                printf("true\n");
                                            }
                                        }

                                    return 0; 
                                }
                case PRINTINT:  printf("%d\n", (int)ex(p->opr.op[0], symTable)); return 0; 
                case PRINTREAL: {
                                    printf("%.3f", ex(p->opr.op[0], symTable));
                                    return 0; //f print trailing zeros
                                }
                case ';':       ex(p->opr.op[0], symTable); return ex(p->opr.op[1], symTable);
                case '=':       {  //0 numero 1 espressione
                                    symrec * s= getsymTable(p->opr.op[0]->id.name, symTable);
                                    if(s == NULL){
                                        printf("there is not such '%s' varibale in the symtable\n", p->opr.op[0]->id.name);
                                        exit(0);
                                    }
                                    

                                    int tipoVariabile = s->tipo;
                                    int tipoEspressione = p->opr.op[1]->con.tipoInserito;

                                    if (tipoVariabile == tipoEspressione) //se combaciano continuo
                                        return s->value = ex(p->opr.op[1], symTable);
                                    else {
                                        if (tipoVariabile == INTTYPE && tipoEspressione == REALTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == INTTYPE && tipoEspressione == BOOLTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == BOOLTYPE && tipoEspressione == INTTYPE) {
                                            return s->value = ex(p->opr.op[1], symTable);
                                            }
                                        if (tipoVariabile == BOOLTYPE && tipoEspressione == REALTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == REALTYPE && tipoEspressione == INTTYPE) {
                                            return s->value = ex(p->opr.op[1], symTable);
                                            }
                                        if (tipoVariabile == REALTYPE && tipoEspressione == BOOLTYPE) {
                                            printf("errore\n");
                                            exit(0);
                                            }
                                        
                                        return s->value = ex(p->opr.op[1], symTable);                    
                                        }
                                    }

                case EQVAR:         {  //0 var 1 var
                                    symrec * s= getsymTable(p->opr.op[0]->id.name, symTable);
                                    symrec * s2= getsymTable(p->opr.op[1]->id.name, symTable);
                                    
                                    if(s == NULL || s2 == NULL){
                                        printf("there is not such '%s' varibale in the symtable\n", p->opr.op[0]->id.name);
                                        exit(0);
                                    }
                                    

                                    int tipoVariabile = s->tipo;
                                    int tipoEspressione = s2->tipo;

                                    if (tipoVariabile == tipoEspressione) //ok if equals
                                        return s->value = s2->value;
                                    else {
                                        if (tipoVariabile == INTTYPE && tipoEspressione == REALTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == INTTYPE && tipoEspressione == BOOLTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == BOOLTYPE && tipoEspressione == INTTYPE) {
                                            //printf("WARNING, espressione INT non potrebbe essere assegnata a tipo BOOL\n");
                                            return s->value = s2->value;
                                            }
                                        if (tipoVariabile == BOOLTYPE && tipoEspressione == REALTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == REALTYPE && tipoEspressione == INTTYPE) {
                                            //printf("WARNING, espressione INT non potrebbe essere assegnata a tipo REAL\n");
                                            return s->value = s2->value;
                                            }
                                        if (tipoVariabile == REALTYPE && tipoEspressione == BOOLTYPE) {
                                            printf("errore\n");
                                            exit(0);
                                            }
                                        
                                        return s->value = s2->value;                    
                                        }
                                    }

                case NEW:       {//new variable declared, no assignemnt, i assign it the default value '0' (see return)
                                    symrec * s= getsymTable(p->opr.op[1]->id.name, symTable);//in 1 c'è il nome, in 0 il tipo
                                    if(s == NULL){
                                        printf("there is not such '%s' varibale in the symtable\n", p->opr.op[0]->id.name);
                                        exit(0);
                                    }
                                    
                                    
                                    s->tipo = p->opr.op[0]; //update type
                                    
                                    return s->value = 0; 
                                    }
                case NEWASSIGNMENT:{
                                    symrec * s= getsymTable(p->opr.op[1]->id.name, symTable);
                                    if(s == NULL){
                                        printf("there is not such '%s' varibale in the symtable\n", p->opr.op[0]->id.name);
                                        exit(0);
                                    }
                                    

                                    int tipoVariabile = s->tipo = p->opr.op[0];
                                    int tipoEspressione = p->opr.op[2]->con.tipoInserito;

                                    if (tipoVariabile == tipoEspressione) //se combaciano continuo
                                        return s->value = ex(p->opr.op[2], symTable);
                                    else {
                                        if (tipoVariabile == INTTYPE && tipoEspressione == REALTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == INTTYPE && tipoEspressione == BOOLTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == BOOLTYPE && tipoEspressione == INTTYPE) {
                                            //printf("WARNING, espressione INT non potrebbe essere assegnata a tipo BOOL\n");
                                            return s->value = ex(p->opr.op[2], symTable);  
                                            }
                                        if (tipoVariabile == BOOLTYPE && tipoEspressione == REALTYPE) {
                                            printf("errore\n");
                                            exit(1);
                                            }
                                        if (tipoVariabile == REALTYPE && tipoEspressione == INTTYPE) {
                                            //printf("WARNING, espressione INT non potrebbe essere assegnata a tipo REAL\n");
                                            return s->value = ex(p->opr.op[2], symTable);
                                            }
                                        if (tipoVariabile == REALTYPE && tipoEspressione == BOOLTYPE) {
                                            printf("errore\n");
                                            exit(0);
                                            }
                                        
                                        return s->value = ex(p->opr.op[2], symTable);                   
                                        }
                                    }
                                    
                                
                case UMINUS:    {return -ex(p->opr.op[0], symTable);}
                case '+':       { 
                                /*checkSYM (p->opr.op[0], p->opr.op[1], symTable );*/ 
                                return ex(p->opr.op[0], symTable) + ex(p->opr.op[1], symTable);}
                case '-':       {return ex(p->opr.op[0], symTable) - ex(p->opr.op[1], symTable);}
                case '*':       {return ex(p->opr.op[0], symTable) * ex(p->opr.op[1], symTable);}
                case '/':       {return ex(p->opr.op[0], symTable) / ex(p->opr.op[1], symTable);}
                case '<':       {return ex(p->opr.op[0], symTable) < ex(p->opr.op[1], symTable);}
                case '>':       {return ex(p->opr.op[0], symTable) > ex(p->opr.op[1], symTable);}
                case GE:        {return ex(p->opr.op[0], symTable) >= ex(p->opr.op[1], symTable);}
                case LE:        {return ex(p->opr.op[0], symTable) <= ex(p->opr.op[1], symTable);}
                case NE:        {return ex(p->opr.op[0], symTable) != ex(p->opr.op[1], symTable);}
                case OR:        {return ex(p->opr.op[0], symTable) || ex(p->opr.op[1], symTable);}
                case AND:       {return ex(p->opr.op[0], symTable) && ex(p->opr.op[1], symTable);}
                case NOT:       {return !ex(p->opr.op[0], symTable);}
                case EQ:        {return ex(p->opr.op[0], symTable) == ex(p->opr.op[1], symTable);}
                    
                    
            }
        case typeRoutine:
                    {
                        /*look up routine in environment*/
                        routine * r = getRoutine(p->routine.name, prg->routineList);
                        if(r == NULL){
                            printf("No such %s in the environement\nProgram will quit now..", p->routine.name);
                            exit(0);
                        }
                        /*count number of parameters*/
                        float paramsCount = size(r->parameters);
                        /*count number of args*/
                        float argsCount = size(p->routine.args);
                        if(paramsCount!=argsCount){
                            printf("Parameter mismatch for %s call\nParams' list size is %f, provided args are %f\n"
                                   , p->routine.name, paramsCount, argsCount);
                            exit(0);
                        }
                        /*procede with params/args matching*/
                        symrec * environement = populateEnv(r->parameters, p->routine.args, symTable);
                        /*add the function name it self to the environment
                         we will be using it for evaluating the return type
                         */
                        symrec * fun = getsymTable(r->name, environement);
                        if (fun == NULL) {
                            putsymTable(r->name, &environement);
                        }
                        r->env = environement;
                        ex(r->commands, r->env);
                        if(r->type == PROCEDURE){
                            return 0;
                        }else{
                            /*look up variable in the environment*/
                            symrec * res = getsymTable(r->name, environement);
                            if(res == NULL){
                                printf("function %s does not return any type\n", r->name);
                            }
                            return res->value;
                        }

                    }

    }
    printf("%s\n", "Unable to evaluate node");
    return 0;
}

