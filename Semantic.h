#pragma once

#include "Token.h"
#include "SymTab.h"

Parameters* searchDecl(Table* curr, Token* tok);
int varDuplicateDecl(Parameters* param, Token* tok);
int funcDuplicateDecl(Parameters* param, Token* tok);
void expressionCheck(Parameters* param, Token* tok, elm_type type);
void assignCheck(Parameters* param, int isIndexArray, Token* tok);
void arrayCheck(Parameters* param, Token* tok, elm_role role);
void returnType(elm_type type);
void numberOfParam(Parameters* param, int numOfParam, Token* tok);
void returnTypeLikeFunc(elm_type typeFunc, elm_type returnType, Token* tok);