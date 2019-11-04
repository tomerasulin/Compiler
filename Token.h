#define _CRT_SECURE_NO_WARNINGS
#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout,*yyout1,*yyout2;

typedef enum eTOKENS {
	TOKEN_INTEGER,
	TOKEN_REAL,
	TOKEN_INTEGER_NUMBER,
	TOKEN_REAL_NUMBER,
	TOKEN_IF,
	TOKEN_WHILE,
	TOKEN_DIV_OP,
	TOKEN_MUL_OP,
	TOKEN_ASSIGN_OP,
	TOKEN_PROGRAM,
	TOKEN_END,
	TOKEN_RETURN,
	TOKEN_VOID,
	TOKEN_LSB,	// (
	TOKEN_RSB,	// )
	TOKEN_COMMA,	// ,
	TOKEN_SEMICOLON,	// ;
	TOKEN_RRB,	// ]
	TOKEN_LRB,	// [
	TOKEN_LCB,	// {
	TOKEN_RCB,	// }
	TOKEN_ID,
	TOKEN_EOF,
	/*
	Examples of tokens, add/change according to your needs.
	Alternative way to define tokens:
	#define TOKEN_INTEGER 1
	#define TOKEN_IF 2
	...........
	*/
}eTOKENS;


typedef struct Token {
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node {
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

//functions
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token* next_token();
Token* back_token();
char *tokenToChar(eTOKENS tok);
void match(eTOKENS tok);
eTOKENS handleToken(eTOKENS kind, char* lexeme, int numLine);
Token* matchReturnNextToken(eTOKENS tok);

#endif