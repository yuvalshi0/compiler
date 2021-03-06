#include "token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

//Pointer for token collection
int currentIndex = 0;

//Pointer for token matching
//Start from -1 because we call next_token at first
int startIndex = -1;

Node *currentNode = NULL;
const int RULES_SIZE = 43;
NodeFollow **rulesArray = NULL;
int init = 0;

#define TOKEN_ARRAY_SIZE 1000
const char *enumDisplayName[] = {
	[TOKEN_FUNCTION] = "FUNCTION",
	[TOKEN_IF] = "IF",
	[TOKEN_RETURN] = "RETURN",
	[TOKEN_MINUS] = "OPERATION",
	[TOKEN_INTEGER] = "INTEGER",
	[TOKEN_FLOAT] = "FLOAT",
	[TOKEN_OP_EQUAL] = "OP_EQUAL",
	[TOKEN_VAR] = "VARIABLE",
	[TOKEN_TYPE] = "VAR TYPE",
	[TOKEN_COMPARISON] = "COMPARISON",
	[TOKEN_END_OF_FILE] = "END_OF_FILE",
	[TOKEN_SEPARATION_COMMA] = "SEPARATION_COMMA",
	[TOKEN_SEPARATION_COLON] = "SEPARATION_COLON",
	[TOKEN_SEPERATION_SEMICOLON] = "SEPERATION_SEMICOLON",
	[TOKEN_SEPARATION_BRACKET_OPEN] = "SEPARATION_BRACKET_OPEN",
	[TOKEN_SEPARATION_BRACKET_CLOSE] = "SEPARATION_BRACKET_CLOSE",
	[TOKEN_SEPARATION_SQUARE_BRACKET_OPEN] = "SEPARATION_SQUARE_BRACKET_OPEN",
	[TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE] = "SEPARATION_SQUARE_BRACKET_CLOSE",
	[TOKEN_SEPARATION_CURLY_BRACKET_OPEN] = "SEPARATION_CURLY_BRACKET_OPEN",
	[TOKEN_SEPARATION_CURLY_BRACKET_CLOSE] = "TOKEN_SEPARATION_CURLY_BRACKET_CLOSE",
	[TOKEN_COMMAND] = "COMMAND",
	[TOKEN_ADD] = "ADD",
	[TOKEN_MUL] = "MUL"

};

const char *enumDisplayRuleName[] = {
	[PARSE_PROG] = "PROG -> GLOBAL_VARS FUNC_PREDFS FUNC_FULL_DEFS",
	[PARSE_GLOBAL_VARS] = "GLOBAL_VARS' -> VAR_DEC GLOBAL_VARS' | ϵ",
	[PARSE_GLOBAL_VARS2] = "VAR_DEC GLOBAL_VARS' | ϵ",
	[PARSE_VAR_DEC] = "VAR_DEC -> TYPE id VAR_DEC'",
	[PARSE_TYPE] = "TYPE -> int | float",
	[PARSE_DIM_SIZES] = "DIM_SIZES -> int_num DIM_SIZES'",
	[PARSE_FUNC_PREDEFS] = "FUNC_PREDFS -> FUNC_PROTOTYPE ; FUNC_PREDFS'",
	[PARSE_FUNC_PREDEFS2] = "FUNC_PREDFS' -> FUNC_PROTOTYPE ; FUNC_PREDFS' | ϵ",
	[PARSE_FUNC_PROTOTYPE] = "FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )",
	[PARSE_FUNC_FULL_DEFS] = "FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS'",
	[PARSE_FUNC_WITH_BODY] = "FUNC_WITH_BODY -> RETURNED_TYPE id ( PARAMS ) COMP_STMT",
	[PARSE_RETURNED_TYPE] = "RETURNED_TYPE -> int | float | void",
	[PARSE_PARAMS] = "PARAMS -> PARAMS_LIST | ε",
	[PARSE_PARAM_LIST] = "PARAMS_LIST -> PARAM PARAMS_LIST'",
	[PARSE_PARAM_LIST2] = "PARAMS_LIST' -> , PARAM PARAMS_LIST' | ϵ",
	[PARSE_PARAM] = "PARAM -> int id PARAM' | float id PARAM'",
	[PARSE_COMP_STMT] = "COMP_STMT -> { VAR_DEC_LIST STMT_LIST }",
	[PARSE_VAR_DEC_LIST] = "VAR_DEC_LIST -> VAR_DEC_LIST'",
	[PARSE_VAR_DEC_LIST2] = "VAR_DEC VAR_DEC_LIST' | ϵ",
	[PARSE_STMT_LIST] = " STMT_LIST -> STMT STMT_LIST'",
	[PARSE_STMT_LIST2] = "STMT_LIST' -> ; STMT STMT_LIST' | ϵ",
	[PARSE_STMT] = "STMT -> VAR = EXPR | { VAR_DEC_LIST STMT_LIST } | IF_STMT | CALL | RETURN_STMT",
	[PARSE_IF_STMT] = "IF_STMT -> if ( CONDITION ) STMT",
	[PARSE_CALL] = "CALL -> id ( ARGS )",
	[PARSE_ARGS] = "ARGS -> ARGS_LIST | ε",
	[PARSE_ARGS_LIST] = "ARGS_LIST -> EXPR ARGS_LIST'",
	[PARSE_ARGS_LIST2] = "ARGS_LIST' -> , EXPR ARGS_LIST' | ϵ",
	[PARSE_RETURN_STMT] = "RETURN_STMT -> return RETURN_STMT'",
	[PARSE_VAR] = "VAR -> id VAR''",
	[PARSE_EXPR_LIST] = "EXPR_LIST -> EXPR EXPR_LIST'",
	[PARSE_EXPR_LIST2] = "EXPR_LIST' -> , EXPR EXPR_LIST' | ϵ",
	[PARSE_CONDITION] = "CONDITION -> EXPR rel_op EXPR",
	[PARSE_EXPR] = "EXPR -> TERM EXPR'",
	[PARSE_EXPR2] = "EXPR' -> + TERM EXPR' | ϵ",
	[PARSE_TERM] = "TERM -> FACTOR TERM'",
	[PARSE_TERM2] = "TERM' -> * FACTOR TERM' | ϵ",
	[PARSE_FACTOR] = "TERM -> FACTOR TERM' | int_num | float_num | ( EXPR )",
	[PARSE_END_OF_FILE] = "PRASE_END_OF_FILE",
	[PARSE_DIM_SIZES2] = "DIM_SIZES' -> ϵ | , DIM_SIZES",
	[PARSE_FUNC_FULL_DEFS2] = "FUNC_FULL_DEFS' -> int id ( PARAMS ) COMP_STMT FUNC_FULL_DEFS' | float id ( PARAMS ) COMP_STMT FUNC_FULL_DEFS' | void id ( PARAMS ) COMP_STMT FUNC_FULL_DEFS' | ϵ",
	[PARSE_PARAM2] = "PARAM' -> ϵ | [ DIM_SIZES ]",
	[PARSE_RETURN_STMT2] = "RETURN_STMT' -> ϵ | id VAR' TERM' EXPR' | id ( ARGS ) TERM' EXPR' | int_num TERM' EXPR' | float_num TERM' EXPR' | ( EXPR ) TERM' EXPR'",
	[PARSE_VAR2] = "VAR' -> ϵ | [ EXPR_LIST ]",
	[PARSE_VAR_DEC2] = "VAR_DEC' -> ; | [ DIM_SIZES ] ;"};

const char *followRules[] = {
	[PARSE_PROG] = "PRASE_PROG",
	[PARSE_GLOBAL_VARS] = "PARSE_GLOBAL_VARS",
	[PARSE_GLOBAL_VARS2] = "PARSE_GLOBAL_VARS2",
	[PARSE_VAR_DEC] = "PARSE_VAR_DEC",
	[PARSE_TYPE] = "PARSE_TYPE",
	[PARSE_DIM_SIZES] = "PARSE_DIM_SIZES",
	[PARSE_FUNC_PREDEFS] = "PARSE_FUNC_PREDEFS",
	[PARSE_FUNC_PREDEFS2] = "PARSE_FUNC_PREDEFS2",
	[PARSE_FUNC_PROTOTYPE] = "PARSE_FUNC_PROTOTYPE",
	[PARSE_FUNC_FULL_DEFS] = "PARSE_FUNC_FULL_DEFS",
	[PARSE_FUNC_WITH_BODY] = "PARSE_FUNC_WITH_BODY",
	[PARSE_RETURNED_TYPE] = "PARSE_RETURNED_TYPE",
	[PARSE_PARAMS] = "PARSE_PARAMS",
	[PARSE_PARAM_LIST] = "PARSE_PARAM_LIST",
	[PARSE_PARAM_LIST2] = "PARSE_PARAM_LIST2",
	[PARSE_PARAM] = "PARSE_PARAM",
	[PARSE_COMP_STMT] = "PARSE_COMP_STMT",
	[PARSE_VAR_DEC_LIST] = "PARSE_VAR_DEC_LIST",
	[PARSE_VAR_DEC_LIST2] = "PARSE_VAR_DEC_LIST2",
	[PARSE_STMT_LIST] = "PARSE_STMT_LIST",
	[PARSE_STMT_LIST2] = "PARSE_STMT_LIST2",
	[PARSE_STMT] = "PARSE_STMT",
	[PARSE_IF_STMT] = "PARSE_IF_STMT",
	[PARSE_CALL] = "PARSE_CALL",
	[PARSE_ARGS] = "PARSE_ARGS",
	[PARSE_ARGS_LIST] = "PARSE_ARGS_LIST",
	[PARSE_ARGS_LIST2] = "PARSE_ARGS_LIST2",
	[PARSE_RETURN_STMT] = "PARSE_RETURN_STMT",
	[PARSE_VAR] = "PARSE_VAR",
	[PARSE_EXPR_LIST] = "PARSE_EXPR_LIST",
	[PARSE_EXPR_LIST2] = "PARSE_EXPR_LIST2",
	[PARSE_CONDITION] = "PARSE_CONDITION",
	[PARSE_EXPR] = "PARSE_EXPR",
	[PARSE_EXPR2] = "PARSE_EXPR2",
	[PARSE_TERM] = "PARSE_TERM",
	[PARSE_TERM2] = "PARSE_TERM2",
	[PARSE_FACTOR] = "PARSE_FACTOR",
	[PARSE_END_OF_FILE] = "PRASE_END_OF_FILE"};
/*
* This function creates a token and stores it in the storage.
*/

void create_and_store_token(eTOKENS kind, char *lexeme, int numOfLine)
{
	//printf("Adding new token to the list...\t %s\n", lexeme);

	int length = strlen(lexeme) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node *)malloc(sizeof(Node));
		currentNode->numberOfElements = 0;

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray =
			(Token *)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node *)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token *)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	currentNode->numberOfElements++;
	currentNode->tokensArray[currentIndex].lexeme = (char *)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token()
{
	//If we are in the edge of the list
	if (startIndex == 0 && currentNode->prev != NULL)
	{
		currentNode = currentNode->prev;
	}
	startIndex = startIndex - 1;
	return &currentNode->tokensArray[startIndex];
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token *next_token(int line)
{
	//if we are in the end of the array
	if (startIndex == TOKEN_ARRAY_SIZE - 1)
	{
		currentNode = currentNode->next;
		startIndex = 0;
	}

	startIndex++;
	Token *temp = &currentNode->tokensArray[startIndex];
	// printf("\033[0;34mNEXT TOKEN CALLED: (line: %d) \033[1;36m%s (%d)\033[22;0m  |  ", line, temp->lexeme, startIndex);
	// printf("\033[0;34mPOINTER IS NOT ON: \033[1;36m%s (%d)\033[22;0m\n", currentNode->tokensArray[startIndex].lexeme, startIndex);
	// printf("-----------------------------------------------\n");
	return temp;
}

void print_tokens()
{
	int i = 0, count = 0;
	Token *t = NULL;
	//fprintf(yyout,"\n");
	Node *temp = currentNode;
	while (temp->prev != NULL)
		temp = temp->prev;

	////printf("Got to the fist elend ini the node list!\nstarting to print all tokens....\n");

	while (temp != NULL)
	{
		for (i = 0; i < temp->numberOfElements; i++)
		{
			count++;
			fprintf(yyout, "Token of type %s, lexeme: %s, found in line :%d\n",
					enumDisplayName[temp->tokensArray[i].kind],
					temp->tokensArray[i].lexeme,
					temp->tokensArray[i].lineNumber);

			//fprintf(yyout, "#%d kind: %s\tlexema: %s\t lineNumber: %d\t\n",
			//	i,
			//	enumDisplayName[temp->tokensArray[i].kind],
			//	temp->tokensArray[i].lexeme,
			//	temp->tokensArray[i].lineNumber);
			//
			//fprintf(yyout,"#%d kind: %s\tlexema: %s\t lineNumber: %d\t\n",
			//	   i,
			//	   enumDisplayName[temp->tokensArray[i].kind],
			//	   temp->tokensArray[i].lexeme,
			//	   temp->tokensArray[i].lineNumber);
		}
		temp = temp->next;
	}
	int currentIndex = 0;
	int startIndex = -1;
	Node *currentNode = NULL;
	// printf("Total numbers of tokens:%d\n", count);
}

int match_token(eTOKENS token)
{
	int index = startIndex + 1;
	int temp = currentNode->tokensArray[index].kind;
	int x = temp == token ? 1 : 0;
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";

	// printf("\033[0;35mMatching token: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  \033", enumDisplayName[temp], index, enumDisplayName[token]);
	// printf("%s\033[22;0m\n", result);
	// printf("-----------------------------------------------\n");
	return x;
}

int match_char(char *ch)
{

	char *lexema = currentNode->tokensArray[startIndex + 1].lexeme;
	int x = strcmp(lexema, ch) == 0 ? 1 : 0;
	// printf("\033[0;36mMatching chars: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  ", lexema, startIndex, ch);
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";
	// printf("%s\033[22;0m\n", result);
	// printf("-----------------------------------------------\n");
	return x;
}

int match_k_char(char *ch, int k)
{

	char *lexema = currentNode->tokensArray[startIndex + k].lexeme;
	int x = strcmp(lexema, ch) == 0 ? 1 : 0;
	// DEBUG PARSER
	// printf("\033[0;33mMatching %d chars: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  ", k, lexema, startIndex, ch);
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";
	// printf("%s\033[22;0m\n", result);
	// //printf("Maching indexs (current:%d, matching: %d\n", startIndex, matchIndex);
	// printf("-----------------------------------------------\n");
	return x;
}

int match_k_token(eTOKENS token, int k)
{
	int index = startIndex == 0 ? 0 : startIndex;
	int temp = currentNode->tokensArray[index + k].kind;
	int x = temp == token ? 1 : 0;
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";

	// DEBUG PARSER
	// printf("\033[0;33mMatching %d token: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  \033", k, enumDisplayName[temp], index, enumDisplayName[token]);
	// printf("%s\033[22;0m\n", result);
	// //printf("Maching indexs (current:%d, matching: %d\n", startIndex, matchIndex);
	// printf("-----------------------------------------------\n");
	return x;
}

void print_grammer(eRULE rule)
{
	fprintf(yyout, "Rule (%s)\n", enumDisplayRuleName[rule]);
	// DEBUG PARSER
	// printf("\033[1;33mRule (%s -> %s)\033[22;0m\n", enumDisplayRuleName[root],
	// 	   enumDisplayRuleName[leaf]);
	// printf("-----------------------------------------------\n");
}

void print_error(Token *source_token, eTOKENS target_token_type)
{
	fprintf(yyout, "Expected token of type '%s' at line: %d, Actual token of type '%s', lexeme: '%s'.\n",
			enumDisplayName[target_token_type],
			source_token->lineNumber,
			enumDisplayName[source_token->kind],
			source_token->lexeme);
	// DEBUG PARSER
	// printf("\033[0;31mExpected token of type '%s' at line: %d, Actual token of type '%s', lexeme: '%s'.\n",
	// 	   enumDisplayName[target_token_type],
	// 	   source_token->lineNumber,
	// 	   enumDisplayName[source_token->kind],
	// 	   source_token->lexeme);
	// printf("\033[22;0m-----------------------------------------------\n");
}
NodeFollow *create_node_follow(eTOKENS token)
{
	NodeFollow *n = (NodeFollow *)malloc(sizeof(NodeFollow));
	if (n == NULL)
	{
		fprintf(yyout, "\nUnable to allocate memory! \n");
		exit(0);
	}
	n->token = token;
	n->next = NULL;
	return n;
}

NodeFollow **init_follow_rules()
{

	if (init != 0)
	{
		return rulesArray;
	}
	rulesArray = (NodeFollow **)calloc(sizeof(NodeFollow *), RULES_SIZE);
	//PARSE_PROG
	rulesArray[0] = create_node_follow(TOKEN_END_OF_FILE);
	//PARSE_GLOBAL_VARS
	NodeFollow *temp = create_node_follow(TOKEN_TYPE);
	NodeFollow *head = temp;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	temp = temp->next;

	rulesArray[1] = head;
	//PARSE_GLOBAL_VARS2
	rulesArray[2] = head;
	//PARSE_VAR_DEC
	temp = create_node_follow(TOKEN_TYPE);
	head = temp;
	temp->next = create_node_follow(TOKEN_VAR);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_IF);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_RETURN);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_OPEN);
	rulesArray[3] = head;

	//PARSE_TYPE
	rulesArray[4] = create_node_follow(TOKEN_VAR);
	//PARSE_DIM_SIZES
	rulesArray[5] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
	//PARSE_FUNC_PREDEFS
	temp = create_node_follow(TOKEN_TYPE);
	head = temp;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	temp = temp->next;

	rulesArray[6] = head;
	// PARSE_FUNC_PREDEFS2,
	rulesArray[7] = head;
	// PARSE_FUNC_PROTOTYPE,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp->next = create_node_follow(TOKEN_TYPE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[8] = head;
	// PARSE_FUNC_FULL_DEFS,
	rulesArray[9] = create_node_follow(TOKEN_END_OF_FILE);
	// PARSE_FUNC_WITH_BODY,
	temp = create_node_follow(TOKEN_END_OF_FILE);
	head = temp;
	temp->next = create_node_follow(TOKEN_TYPE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[10] = head;
	// PARSE_RETURNED_TYPE,
	rulesArray[11] = create_node_follow(TOKEN_VAR);
	// PARSE_PARAMS,
	rulesArray[12] = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	// PARSE_PARAM_LIST,
	rulesArray[13] = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	// PARSE_PARAM_LIST2,
	rulesArray[14] = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	// PARSE_PARAM,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	rulesArray[15] = head;
	// PARSE_COMP_STMT,
	temp = create_node_follow(TOKEN_END_OF_FILE);
	head = temp;
	temp->next = create_node_follow(TOKEN_TYPE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[16] = head;
	// PARSE_VAR_DEC_LIST,
	temp = create_node_follow(TOKEN_VAR);
	head = temp;
	temp->next = create_node_follow(TOKEN_IF);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_RETURN);
	rulesArray[17] = head;
	// PARSE_VAR_DEC_LIST2,
	rulesArray[18] = head;
	// PARSE_STMT_LIST,
	rulesArray[19] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	// PARSE_STMT_LIST2,
	rulesArray[20] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	// PARSE_STMT,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	rulesArray[21] = head;

	// PARSE_IF_STMT,
	rulesArray[22] = head;

	// PARSE_CALL,
	rulesArray[23] = head;

	// PARSE_ARGS,
	rulesArray[24] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_ARGS_LIST,
	rulesArray[25] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_ARGS_LIST2,
	rulesArray[26] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_RETURN_STMT,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[27] = head;

	// PARSE_VAR,
	rulesArray[28] = create_node_follow(TOKEN_COMMAND);

	// PARSE_EXPR_LIST,
	rulesArray[29] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_EXPR_LIST2,
	rulesArray[30] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_CONDITION,
	rulesArray[31] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	// PARSE_EXPR,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_COMPARISON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
	rulesArray[32] = head;

	// PARSE_EXPR2,
	rulesArray[33] = head;

	// PARSE_TERM,
	temp = create_node_follow(TOKEN_ADD);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_COMPARISON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[34] = head;

	// PARSE_TERM2,
	rulesArray[35] = head;

	// PARSE_FACTOR,
	temp = create_node_follow(TOKEN_MUL);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_ADD);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_COMPARISON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[36] = head;
	// PARSE_DIM_SIZE2,
	rulesArray[37] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	// PARSE_FUNC_FULL_DEFS2,
	rulesArray[38] = create_node_follow(TOKEN_END_OF_FILE);
	// PARSE_PARAM2,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	rulesArray[39] = head;
	// PARSE_RETURN_STMT2,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[40] = head;
	// PARSE_VAR2,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp->next = create_node_follow(TOKEN_MUL);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_ADD);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_COMMAND);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_COMPARISON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[41] = head;
	// PARSE_VAR_DEC2
	temp = create_node_follow(TOKEN_TYPE);
	head = temp;
	temp->next = create_node_follow(TOKEN_VAR);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_IF);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_RETURN);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[42] = head;
	rulesArray[43] = create_node_follow(TOKEN_END_OF_FILE);
	init++;
	return rulesArray;
}
