#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "SymTab.h"
#include "Token.h"
#include "Parsers.h"
#include "Semantic.h"

Token *token = NULL;
Token *lookAhead = NULL;
Table *current_table;

void Parser()
{
	current_table = make_table(NULL);
	Parse_PROGRAM();
	match(TOKEN_EOF);
}

void Parse_PROGRAM()
{
	elm_type return_type;
	fprintf(yyout1, "RULE (PROGRAM -> program VAR_DEFINITIONS;STATEMENTS end FUNC_DEFINITIONS)\n");
	match(TOKEN_PROGRAM);
	current_table = make_table(current_table);
	Parse_VAR_DEFINITIONS(DEFINITON);
	match(TOKEN_SEMICOLON);
	return_type = Parse_STATEMENTS(STATEMENT);
	match(TOKEN_END);
	returnType(return_type);
	Parse_FUNC_DEFINITIONS(DEFINITON);
}

int Parse_VAR_DEFINITIONS(elm_role role)
{
	int numOfVars;
	fprintf(yyout1, "RULE (VAR_DAFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS1)\n");
	numOfVars = Parse_VAR_DEFINITION(DEFINITON);
	return Parse_VAR_DEFINITIONS1(DEFINITON) + numOfVars;
}

int Parse_VAR_DEFINITIONS1(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_SEMICOLON:
		lookAhead = next_token();
		switch (lookAhead->kind)
		{
		case TOKEN_REAL:
		case TOKEN_INTEGER:
			fprintf(yyout1, "RULE (VAR_DAFINITIONS1 -> ;VAR_DEFINITIONS)\n");
			lookAhead = back_token();
			return Parse_VAR_DEFINITIONS(role);
		default:
			fprintf(yyout1, "RULE (VAR_DEFINITIONS1 -> epsilon)\n");
			token = back_token();
			token = back_token();
			return 0;
		}
		break;
	case TOKEN_RCB:
		fprintf(yyout1, "RULE (VAR_DEFINITIONS1 -> epsilon)\n");
		token = back_token();
		return 0;
	default:
		fprintf(yyout1, "Expected token of type SEMICOLON at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON && token->kind != TOKEN_RSB) {
			token = next_token();
		}
		token = back_token();
		return 0;
	}
}

int Parse_VAR_DEFINITION(elm_role role)
{
	elm_type type;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		fprintf(yyout1, "RULE (VAR_DEFINITION -> TYPE VARIABLES_LIST)\n");
		token = back_token();
		type = Parse_TYPE();
		return Parse_VARIABLES_LIST(type,role);
	default:
		fprintf(yyout1, "Expected token of type REAL,INTEGER at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON && token->kind != TOKEN_RSB) {
			token = next_token();
		}
		token = back_token();
		return 0;
	}

}


elm_type Parse_TYPE()
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_REAL:
		fprintf(yyout1, "RULE (TYPE -> real)\n");
		return TYPE_REAL;
	case TOKEN_INTEGER:
		fprintf(yyout1, "RULE (TYPE -> integer)\n");
		return TYPE_INTEGER;
	default:
		fprintf(yyout1, "Expected token of type REAL, INTEGER at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF && token->kind != TOKEN_ID) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}
}

int Parse_VARIABLES_LIST(elm_type return_type, elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_ID:
		fprintf(yyout1, "RULE (VARIABLES_LIST -> VARIABLE VARIABLES_LIST1)\n");
		token = back_token();
		Parse_VARIABLE(return_type, role);
		return Parse_VARIABLES_LIST1(return_type, role) + 1;
	default:
		fprintf(yyout1, "Expected token of type ID at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON && token->kind != TOKEN_RSB) {
			token = next_token();
		}
		token = back_token();
		return 0;
	}
}

int Parse_VARIABLES_LIST1(elm_type return_type, elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_COMMA:
		fprintf(yyout1, "RULE (VARIABLES_LIST1 -> ,VARIABLE VARIABLES_LIST1)\n");
		Parse_VARIABLE(return_type, role);
		return Parse_VARIABLES_LIST1(return_type, role) + 1;
	case TOKEN_SEMICOLON:
	case TOKEN_RSB:
		fprintf(yyout1, "RULE (VARIABLES_LIST1 -> epsilon)\n");
		token = back_token();
		return 0;
	default:
		fprintf(yyout1, "Expected token of type SEMICOLON, RSB at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON && token->kind != TOKEN_RSB) {
			token = next_token();
		}
		token = back_token();
		return 0;
	}
}

void Parse_VARIABLE(elm_type return_type, elm_role role)
{
	Parameters *param;
	int indexAccess;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_ID:
		fprintf(yyout1, "RULE (VARIABLE -> id VARIABLE1)\n");
		param = insert(current_table, token->lexeme);
		if (varDuplicateDecl(param, token))
		{
			set_id_type(param, return_type);
			decleration_line(param, token->lineNumber);
		}
		indexAccess = Parse_VARIABLE1(param, role);
		break;
	default:
		fprintf(yyout1, "Expected token of type ID at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON && token->kind != TOKEN_RSB && token->kind != TOKEN_COMMA && token->kind != TOKEN_ASSIGN_OP) {
			token = next_token();
		}
		token = back_token();
		break;
	}
}


int Parse_VARIABLE1(Parameters *param, elm_role role)
{
	Token *sizeOfArr;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_LRB:
		fprintf(yyout1, "RULE (VARIABLE1 -> [int_number])\n");

		sizeOfArr = matchReturnNextToken(TOKEN_INTEGER_NUMBER);
		match(TOKEN_RRB);
		arrayCheck(param, sizeOfArr, role);
		return 1;
	case TOKEN_COMMA:
	case TOKEN_SEMICOLON:
	case TOKEN_ASSIGN_OP:
	case TOKEN_RSB:
		fprintf(yyout1, "RULE (VARIABLE1 -> epsilon)\n");
		token = back_token();
		return 0;
	default:
		fprintf(yyout1, "Expected token of type COMMA, SEMICOLON, RSB, ASSIGN_OP, LRB at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF && token->kind != TOKEN_SEMICOLON && token->kind != TOKEN_RSB && token->kind != TOKEN_ASSIGN_OP && token->kind != TOKEN_COMMA) {
			token = next_token();
		}
		token = back_token();
		return 0;
	}
}

void Parse_FUNC_DEFINITIONS(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout1, "RULE (FUNC_DEFININTIONS -> FUNC_DEFININTION FUNC_DEFININTIONS1)\n");
		token = back_token();
		Parse_FUNC_DEFINITION(role);
		Parse_FUNC_DEFINITIONS1(role);
	default:
		fprintf(yyout1, "Expected token of type VOID, REAL, INTEGER at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF) {
			token = next_token();
		}
		token = back_token();
		break;
	}

}

void Parse_FUNC_DEFINITIONS1(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout1, "RULE (FUNC_DEFININTIONS1 -> FUNC_DEFINITION FUNC_DEFINITIONS1)\n");
		token = back_token();
		Parse_FUNC_DEFINITION(role);
		Parse_FUNC_DEFINITIONS1(role);
		break;
	case TOKEN_EOF:
		fprintf(yyout1, "RULE (FUNC_DEFININTIONS1 -> epsilon)\n");
		token = back_token();
		break;
	default:
		fprintf(yyout1, "Expected token of type VOID, REAL, INTEGER, EOF at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF) {
			token = next_token();
		}
		token = back_token();
		break;
	}
}

void Parse_FUNC_DEFINITION(elm_role role)
{
	elm_type type, type1;
	Token *id;
	Parameters* param, *entry;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout1, "RULE (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK)\n");
		token = back_token();
		type = Parse_RETURNED_TYPE(role);
		id = matchReturnNextToken(TOKEN_ID);
		param = insert(current_table, id->lexeme);
		if (funcDuplicateDecl(param, id))
		{
			set_id_type(param, type);
			decleration_line(param, token->lineNumber);
		}
		match(TOKEN_LSB);
		Parse_PARAM_DEFINITIONS(param, role);
		match(TOKEN_RSB);
		type1 = Parse_BLOCK(role);
		returnTypeLikeFunc(type, type1, id);
		if (param != NULL) {
			entry = duplicateParam(param);
			param = insertEntry(current_table, entry);
		}	
		break;
	default:
		fprintf(yyout1, "Expected token of type VOID, REAL, INTEGER, EOF at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF && token->kind != TOKEN_VOID && token->kind != TOKEN_INTEGER && token->kind != TOKEN_REAL) {
			token = next_token();
		}
		token = back_token();
		break;
	}

}

elm_type Parse_RETURNED_TYPE(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_VOID:
		fprintf(yyout1, "RULE (RETURNED_TYPE -> void)\n");
		return TYPE_VOID;
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout1, "RULE (RETURNED_TYPE ->TYPE)\n");
		token = back_token();
		return Parse_TYPE();
	default:
		fprintf(yyout1, "Expected token of type VOID, REAL, INTEGER at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF && token->kind != TOKEN_ID) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}
}

void Parse_PARAM_DEFINITIONS(Parameters *param, elm_role role)
{
	int numOfParams;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout1, "RULE (PARAM_DEFINITIONS -> VAR_DEFINITIONS)\n");
		token = back_token();
		numOfParams = Parse_VAR_DEFINITIONS(role);
		set_param_size(param, numberOfParam);
		break;
	case TOKEN_RSB:
		fprintf(yyout1, "RULE (PARAM_DEFINITIONS -> epsilon)\n");
		token = back_token();
		set_param_size(param, 0);
		break;
	default:
		fprintf(yyout1, "Expected token of type REAL, INTEGER, RSB at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_EOF && token->kind != TOKEN_RSB) {
			token = next_token();
		}
		token = back_token();
		break;
	}
}

elm_type Parse_STATEMENTS(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_ID:
	case TOKEN_LCB:
	case TOKEN_RETURN:
		fprintf(yyout1, "RULE (STATEMENTS -> STATEMENT STATEMENTS1)\n");
		token = back_token();
		Parse_STATEMENT(STATEMENT);
		return Parse_STATEMENTS1(STATEMENT);
	default:
		fprintf(yyout1, "Expected token of type ID, LCB, RETURN at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_END && token->kind != TOKEN_RCB) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}

}

elm_type Parse_STATEMENTS1(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_SEMICOLON:
		fprintf(yyout1, "RULE (STATEMENTS1 -> ;STATEMENTS2)\n");
		return Parse_STATEMENTS2(role);
	default:
		fprintf(yyout1, "Expected token of type SEMICOLON at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_END && token->kind != TOKEN_RCB) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}
}

elm_type Parse_STATEMENTS2(elm_role role) {
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_LCB:
	case TOKEN_RETURN:
	case TOKEN_ID:
		fprintf(yyout1, "RULE (STATEMENTS2 -> STATEMENTS)\n");
		token = back_token();
		return Parse_STATEMENTS(role);	
	case TOKEN_RCB:
	case TOKEN_END:
		fprintf(yyout1, "RULE (STATEMENTS2 -> epsilon)\n");
		token = back_token();
		return TYPE_VOID;
	default:
		fprintf(yyout1, "Expected token of type ID, LCB, RETURN at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_END && token->kind != TOKEN_RCB) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}
}
elm_type Parse_STATEMENT(elm_role role)
{
	Parameters *param;
	elm_type type,type1 = TYPE_VOID;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_ID:
		param = searchDecl(current_table, token);
		lookAhead = next_token();
		token = back_token();
		switch (lookAhead->kind)
		{
		case TOKEN_LRB:
		case TOKEN_ASSIGN_OP:
			fprintf(yyout1, "RULE (STATEMENT -> VARIABLE = EXPRESSION)\n");
			token = back_token();
			Parse_VARIABLE(type1,role);
			match(TOKEN_ASSIGN_OP);
			type = Parse_EXPRESSION(role);
			expressionCheck(param, token, type);
			return type;
		case TOKEN_LSB:
			fprintf(yyout1, "RULE (STATEMENT -> FUNCTION_CALL)\n");
			lookAhead = back_token();
			Parse_FUNCTION_CALL(param,role);
			if (param) {
				return param->entryInfo.varType;
			}
			else {
				return TYPE_ERROR;
			}	
		default:
			fprintf(yyout1, "Expected token of type LRB, LSB, ASSIGN_OP at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
			while (lookAhead->kind != TOKEN_SEMICOLON && lookAhead->kind != TOKEN_COMMA && lookAhead->kind != TOKEN_ASSIGN_OP && lookAhead->kind != TOKEN_RSB) {
				lookAhead = next_token();
			}
			lookAhead = back_token();
			return TYPE_VOID;
		}
	case TOKEN_LCB:
		fprintf(yyout1, "RULE (STATEMENT -> BLOCK)\n");
		token = back_token();
		current_table = make_table(current_table);
		return Parse_BLOCK(role);
	case TOKEN_RETURN:
		fprintf(yyout1, "RULE (STATEMENT -> return STATEMENT1)\n");
		return Parse_STATEMENT1(role);
	default:
		fprintf(yyout1, "Expected token of type LSB, RETURN, ID at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_COMMA) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}

}

elm_type Parse_STATEMENT1(elm_role role)
{
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_INTEGER_NUMBER:
	case TOKEN_REAL_NUMBER:
	case TOKEN_ID:
		fprintf(yyout1, "RULE (STATEMENT1 -> EXPRESSION)\n");
		token = back_token();
		return Parse_EXPRESSION(role);	
	case TOKEN_SEMICOLON:
		fprintf(yyout1, "RULE (STATEMENT1 -> epsilon)\n");
		token = back_token();
		return TYPE_VOID;
	default:
		fprintf(yyout1, "Expected token of type INTEGER_NUMBER, REAL_NUMBER, ID at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}
}

elm_type Parse_BLOCK(elm_role role)
{
	elm_type return_type;
	fprintf(yyout1, "RULE (BLOCK -> {VAR_DEFINITIONS;STATEMENTS})\n");
	match(TOKEN_LCB);
	Parse_VAR_DEFINITIONS(role);
	match(TOKEN_SEMICOLON);
	return_type = Parse_STATEMENTS(role);
	match(TOKEN_RCB);
	return return_type;
}

void Parse_FUNCTION_CALL(Parameters* param,elm_role role)
{
	fprintf(yyout1, "RULE (FUNCTION_CALL -> id (PARAMETERS_LIST))\n");
	match(TOKEN_ID);
	match(TOKEN_LSB);
	Parse_PARAMETERS_LIST(param,role);
	match(TOKEN_RSB);
}

void Parse_PARAMETERS_LIST(Parameters* param, elm_role role)
{
	int numOfParams;
	token = next_token();
	switch (token->kind)
	{
	case TOKEN_ID:
		fprintf(yyout1, "RULE (PARAMETERS_LIST -> VARIABLES_LIST)\n");
		token = back_token();
		//Parameters *param = searchDecl(token);
		//Parse_VARIABLE1(param, DEFINITON);
		numOfParams = Parse_VARIABLES_LIST1(get_id_type(param), DEFINITON);
		numberOfParam(param, numberOfParam, token);
		break;

	case TOKEN_RSB:
		fprintf(yyout1, "RULE (PARAMETERS_LIST -> epsilon)\n");
		token = back_token();
		break;
	default:
		fprintf(yyout1, "Expected token of type ID at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_RSB) {
			token = next_token();
		}
		token = back_token();
		break;
	}

}

elm_type Parse_EXPRESSION(elm_role role)
{
	elm_type type = TYPE_VOID;
	Parameters *param = NULL;

	token = next_token();
	switch (token->kind)
	{
	case TOKEN_INTEGER_NUMBER:
		fprintf(yyout1, "RULE (EXPRESSION -> int_number)\n");
		return TYPE_INTEGER;
	case TOKEN_REAL_NUMBER:
		fprintf(yyout1, "RULE (EXPRESSION -> real_number)\n");
		return TYPE_REAL;
	case TOKEN_ID:
		param = searchDecl(current_table, token);
		lookAhead = next_token();
		switch (lookAhead->kind)
		{
		case TOKEN_MUL_OP:
		case TOKEN_DIV_OP:
			fprintf(yyout1, "RULE (EXPRESSION -> id ar_op EXPRESSION)\n");
			return Parse_EXPRESSION(role);
		case TOKEN_LRB:
			lookAhead = back_token();
			Parse_VARIABLE1(param, role);
			if (param != NULL)
				return param->entryInfo.varType;
			else
				return TYPE_ERROR;	
		case TOKEN_COMMA:
		case TOKEN_SEMICOLON:
		case TOKEN_ASSIGN_OP:
		case TOKEN_RSB:
			fprintf(yyout1, "RULE (EXPRESSION -> VARIABLE)\n");
			lookAhead = back_token();
			Parse_VARIABLE(type,role);
			if (param == NULL)
				return TYPE_ERROR;

			if (param->entryInfo.arraySize > -1)
			{
				if (param->entryInfo.varType == TYPE_INTEGER)
					return TYPE_INTEGER_ARRAY;
				else if (param->entryInfo.varType == TYPE_REAL)
					return TYPE_REAL_ARRAY;
				return TYPE_ERROR;
			}
			return param->entryInfo.varType;
		default:
			fprintf(yyout1, "Expected token of type AR_OP, LRB, COMMA, SEMICOLON, ASSIGN_OP, RSB at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
			while (lookAhead->kind != TOKEN_SEMICOLON) {
				lookAhead = next_token();
			}
			lookAhead = back_token();
			return TYPE_ERROR;
		}
		break;
	default:
		fprintf(yyout1, "Expected token of type ID, REAL_NUMBER, INTEGER_NUMBER at line: %d, Actual token of type %s, lexeme: %s\n", token->lineNumber, tokenToChar(token->kind), token->lexeme);
		while (token->kind != TOKEN_SEMICOLON) {
			token = next_token();
		}
		token = back_token();
		return TYPE_ERROR;
	}
}


