#ifndef TOKEN_H
#define TOKEN_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	TOKEN_FUNCTION,
	TOKEN_IF,
	TOKEN_RETURN,
	TOKEN_MINUS,
	TOKEN_SEPERATION,
	TOKEN_INTEGER,
	TOKEN_FLOAT,
	TOKEN_WHILE,
	TOKEN_OP_EQUAL,
	TOKEN_VAR,
	TOKEN_TYPE,
	TOKEN_COMPARISON,
	TOKEN_END_OF_FILE,
	TOKEN_SEPARATION_COMMA,
	TOKEN_SEPARATION_COLON,
	TOKEN_SEPERATION_SEMICOLON,
	TOKEN_SEPARATION_BRACKET_OPEN,
	TOKEN_SEPARATION_BRACKET_CLOSE,
	TOKEN_SEPARATION_SQUARE_BRACKET_OPEN,
	TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE,
	TOKEN_SEPARATION_CURLY_BRACKET_OPEN,
	TOKEN_SEPARATION_CURLY_BRACKET_CLOSE,
	TOKEN_COMMAND,
	TOKEN_ADD,
	TOKEN_MUL
} eTOKENS;

typedef enum eRULE
{
	PARSE_PROG,
	PARSE_GLOBAL_VARS,
	PARSE_GLOBAL_VARS2,
	PARSE_VAR_DEC,
	PARSE_TYPE,
	PARSE_DIM_SIZES,
	PARSE_FUNC_PREDEFS,
	PARSE_FUNC_PREDEFS2,
	PARSE_FUNC_PROTOTYPE,
	PARSE_FUNC_FULL_DEFS,
	PARSE_FUNC_WITH_BODY,
	PARSE_RETURNED_TYPE,
	PARSE_PARAMS,
	PARSE_PARAM_LIST,
	PARSE_PARAM_LIST2,
	PARSE_PARAM,
	PARSE_COMP_STMT,
	PARSE_VAR_DEC_LIST,
	PARSE_VAR_DEC_LIST2,
	PARSE_STMT_LIST,
	PARSE_STMT_LIST2,
	PARSE_STMT,
	PARSE_IF_STMT,
	PARSE_CALL,
	PARSE_ARGS,
	PARSE_ARGS_LIST,
	PARSE_ARGS_LIST2,
	PARSE_RETURN_STMT,
	PARSE_VAR,
	PARSE_EXPR_LIST,
	PARSE_EXPR_LIST2,
	PARSE_CONDITION,
	PARSE_EXPR,
	PARSE_EXPR2,
	PARSE_TERM,
	PARSE_TERM2,
	PARSE_FACTOR,
	PARSE_END_OF_FILE,
	PARSE_DIM_SIZES2,
	PARSE_FUNC_FULL_DEFS2,
	PARSE_PARAM2,
	PARSE_RETURN_STMT2,
	PARSE_VAR2,
	PARSE_VAR_DEC2
} eRULE;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
} Token;

typedef struct NodeFollow
{
	eTOKENS token;
	struct NodeFollow *next;
} NodeFollow;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
	int numberOfElements;
} Node;

void create_and_store_token(eTOKENS kind, char *lexeme, int numOfLine);
Token *next_token();
Token *back_token();
NodeFollow* create_node_follow(eTOKENS token);
int match_char(char *ch);
int match_token(eTOKENS token);
int match_k_char(char *ch, int k);
int match_k_token(eTOKENS token, int k);
void print_tokens();
void print_error(Token *source_token, eTOKENS target_token_type);
void print_grammer(eRULE root, eRULE leaf);
NodeFollow** init_follow_rules();

#endif