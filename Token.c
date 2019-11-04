#define _CRT_SECURE_NO_WARNINGS

#include "Token.h"

/* This package describes the storage of tokens identified in the input text.

* The storage is a bi-directional list of nodes.

* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.

* Such data structure supports an efficient way to manipulate tokens.



There are three functions providing an external access to the storage:

- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.

- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)

*/

int currentIndex = 0;
extern FILE *yyin, *yyout;
extern yylex();
Node* currentNode = NULL;

#define TOKEN_ARRAY_SIZE 1000

/*

* This function creates a token and stores it in the storage.

*/

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine) {
	int length = strlen(lexeme) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL) {
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL) {
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL) {
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}
	// case 2: at least one token exists in the storage.
	else {
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1) {
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));
			if (currentNode == NULL) {
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
			if (currentNode->tokensArray == NULL) {
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}
		// the array (the current node) is not full
		else {
			currentIndex++;
		}
	}
	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif		
}

/*

* This function returns the token in the storage that is stored immediately before the current token (if exsits).

*/

Token* back_token() {

	if (currentNode == NULL)
	{
		printf("there is no tokens in the storage");
		exit(1);
	}
	currentIndex--;
	return &(currentNode->tokensArray[currentIndex]);
}

/*

* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):

*  this function returns the next stored token.

* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);

*  returns the token that was created.

*/

Token* next_token() {

	if (currentNode == NULL) {
		yylex();
		return &(currentNode->tokensArray[currentIndex]);
	}
	else {
		if (currentNode->tokensArray[currentIndex + 1].lexeme == NULL) {
			yylex();
			return &(currentNode->tokensArray[currentIndex]);
		}
		currentIndex++;
		return &(currentNode->tokensArray[currentIndex]);
	}

}

char *tokenToChar(eTOKENS tok) {
	switch (tok) {
	case TOKEN_INTEGER:
		return "INTEGER";
		break;

	case TOKEN_REAL:
		return "REAL";
		break;

	case TOKEN_INTEGER_NUMBER:
		return "INTEGER_NUMBER";
		break;

	case TOKEN_REAL_NUMBER:
		return "REAL_NUMBER";
		break;

	case TOKEN_IF:
		return "IF";
		break;

	case TOKEN_WHILE:
		return "WHILE";
		break;

	case TOKEN_MUL_OP:
		return "MUL_OP";
		break;

	case TOKEN_DIV_OP:
		return "DIV_OP";
		break;

	case TOKEN_ASSIGN_OP:
		return "ASSIGN_OP";
		break;

	case TOKEN_PROGRAM:
		return "PROGRAM";
		break;

	case TOKEN_END:
		return "END";
		break;

	case TOKEN_RETURN:
		return "RETURN";
		break;

	case TOKEN_VOID:
		return "VOID";
		break;


	case TOKEN_LSB:
		return "LSB";
		break;

	case TOKEN_RSB:
		return "RSB";
		break;

	case TOKEN_COMMA:
		return "COMMA";
		break;

	case TOKEN_SEMICOLON:
		return "SEMICOLON";
		break;

	case TOKEN_RRB:
		return "RRB";
		break;

	case TOKEN_LRB:
		return "LRB";
		break;

	case TOKEN_LCB:
		return "LCB";
		break;

	case TOKEN_RCB:
		return "RCB";
		break;

	case TOKEN_ID:
		return "ID";
		break;


	case TOKEN_EOF:
		return "EOF";
		break;
	default:
		exit(1);
		break;
	}
}
void match(eTOKENS tok) {
	Token* token = next_token();
	if (token->kind != tok)
	{
		fprintf(yyout1, "Expected: token %s at line %d, Actual token: %s, lexeme %s\n", tokenToChar(tok), token->lineNumber, tokenToChar(token->kind), token->lexeme);
	}

}

eTOKENS handleToken(eTOKENS kind, char* lexeme, int numLine) {
	create_and_store_token(kind, lexeme, numLine);
	fprintf(yyout, "Token of kind: %s \t found at line: %d \t lexeme: %s\n", tokenToChar(kind), numLine, lexeme);
	return kind;
}

Token* matchReturnNextToken(eTOKENS tok) {
	Token* token = next_token();
	if (token->kind != tok)
	{
		fprintf(yyout1, "Expected: token %s at line %d, Actual token: %s, lexeme %s\n", tokenToChar(tok), token->lineNumber, tokenToChar(token->kind), token->lexeme);
	}
	return token;
}