#pragma once

#include "Semantic.h"

Table* current_tale;

Parameters* searchDecl(Table* curr,Token* tok)
{
	Parameters* param = find(curr, tok->lexeme);
	if (param == NULL)
	{
		fprintf(yyout2, "Semantic error - declaration: %s is not found in line %d\n", tok->lexeme, tok->lineNumber);
	}
	used(param);
	return param;
}

int varDuplicateDecl(Parameters* param, Token* tok)
{
	if (param == NULL)
	{
		fprintf(yyout2, "Semantic error - this variable: %s has double declaration\n", tok->lexeme);
		return 0;
	}
	return 1;
}


int funcDuplicateDecl(Parameters* param, Token* tok)
{
	if (param == NULL)
	{
		fprintf(yyout2, "Semantic error - this function: %s has double declaration \n", tok->lexeme);
		return 0;
	}
	return 1;
}

void expressionCheck(Parameters* param, Token* tok, elm_type type)
{
	if (param != NULL && param->entryInfo.varType == TYPE_INTEGER && type == TYPE_REAL)
		fprintf(yyout2, "Semantic error - false assignment type %s in line: %d\n", param->name, tok->lineNumber);

	if (type == TYPE_INTEGER_ARRAY || type == TYPE_REAL_ARRAY)
		fprintf(yyout2, "Semantic error - impossible to assign array of %s in line %d\n", param->name, tok->lineNumber);

	if (type == TYPE_ERROR)
		fprintf(yyout2, "Semantic error - invalid type %s in line: %d\n", enumConvert(type), tok->lineNumber);
}


void assignCheck(Parameters* param, int isIndexArray, Token* tok)
{
	if (param == NULL)
		return;
	if (!isIndexArray && param->entryInfo.arraySize != -1)
		fprintf(yyout2, "Semantic error - impossible to assign value to array %s in line %d\n", param->name, tok->lineNumber);
	if (param->entryInfo.numOfParam != -1)
		fprintf(yyout2, "Semantic error - impossible to assign value to function %s in line %d\n", param->name, tok->lineNumber);
}

void arrayCheck(Parameters* param, Token* tok, elm_role role)
{
	if (role == DEFINITON)
		set_arr_size(param, tok->lexeme);

	else if (param != NULL && role == STATEMENT && param->entryInfo.arraySize>0 && param->entryInfo.arraySize<atoi(tok->lexeme))
		fprintf(yyout2, "Semantic error - impossible to access index of array:%s in line %d\n", param->name, tok->lineNumber);

	else if (param != NULL && role == STATEMENT && param->entryInfo.arraySize == -1)
		fprintf(yyout2, "Semantic error - impossible to access variable :%s as array in line %d\n", param->name, tok->lineNumber);

}

void returnType(elm_type type)
{
	if (type != TYPE_VOID)
		fprintf(yyout2, "Semantic error - program shouldn't return statement\n");
}

void numberOfParam(Parameters* param, int numOfParam, Token* tok)
{
	if (param != NULL && param->entryInfo.numOfParam != numOfParam)
		fprintf(yyout2, "Semantic error - the number of parameters not match to function %s definiton in line %d\n", param->name, tok->lineNumber);
}

void returnTypeLikeFunc(elm_type typeFunc, elm_type returnType, Token* tok)
{
	if (returnType == TYPE_VOID && typeFunc != TYPE_VOID)
		fprintf(yyout2, "Semantic error -  the function %s should return type %s in line %d\n", tok->lexeme, enumConvert(typeFunc), tok->lineNumber);
	else if (returnType != typeFunc)
		fprintf(yyout2, "Semantic error - the function %s should return type %s but return: %s\n", tok->lexeme, enumConvert(typeFunc), enumConvert(returnType));


}