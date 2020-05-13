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
int startIndex = -1;

//Pointer for mathcing
int matchIndex = 0;
Node *currentNode = NULL;
const int RULES_SIZE = 41;
NodeFollow **rulesArray;

#define TOKEN_ARRAY_SIZE 1000
const char *enumDisplayName[] = {
	[TOKEN_FUNCTION] = "FUNCTION",
	[TOKEN_IF] = "IF or WHILE",
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
	[PARSE_PROG] = "PARSE_PROG",
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
	[PARSE_END_OF_FILE] = "PRASE_END_OF_FILE",
	[PARSE_DIM_SIZES2] = "PARSE_DIM_SIZE2",
	[PARSE_FUNC_FULL_DEFS2] = "PARSE_FUNC_FULL_DEFS2",
	[PARSE_PARAM2] = "PARSE_PARAM2",
	[PARSE_RETURN_STMT2] = "PARSE_RETURN_STMT2",
	[PARSE_VAR2] = "PARSE_VAR_2",
	[PARSE_VAR_DEC2] = "PARSE_DEC2"};

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
	printf("\033[0;34mNEXT TOKEN CALLED: (line: %d) \033[1;36m%s (%d)\033[22;0m  |  ", line, temp->lexeme, startIndex);
	printf("\033[0;34mPOINTER IS NOT ON: \033[1;36m%s (%d)\033[22;0m\n", currentNode->tokensArray[startIndex].lexeme, startIndex);
	printf("-----------------------------------------------\n");
	matchIndex = startIndex;

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
	//printf("Total numbers of tokens:%d\n", count);
}

int match_token(eTOKENS token)
{
	int index = startIndex + 1;
	int temp = currentNode->tokensArray[index].kind;
	int x = temp == token ? 1 : 0;
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";

	matchIndex++;
	printf("\033[0;35mMatching token: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  \033", enumDisplayName[temp], index, enumDisplayName[token]);
	printf("%s\033[22;0m\n", result);
	//printf("Maching indexs (current:%d, matching: %d\n", startIndex, matchIndex);
	printf("-----------------------------------------------\n");
	return x;
}

int match_char(char *ch)
{

	char *lexema = currentNode->tokensArray[startIndex + 1].lexeme;
	int x = strcmp(lexema, ch) == 0 ? 1 : 0;
	printf("\033[0;36mMatching chars: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  ", lexema, startIndex, ch);
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";
	printf("%s\033[22;0m\n", result);
	//printf("Maching indexs (current:%d, matching: %d\n", startIndex, matchIndex);
	printf("-----------------------------------------------\n");
	return x;
}

int match_k_char(char *ch, int k)
{

	char *lexema = currentNode->tokensArray[startIndex + k].lexeme;
	int x = strcmp(lexema, ch) == 0 ? 1 : 0;
	printf("\033[0;33mMatching %d chars: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  ", k, lexema, startIndex, ch);
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";
	printf("%s\033[22;0m\n", result);
	//printf("Maching indexs (current:%d, matching: %d\n", startIndex, matchIndex);
	printf("-----------------------------------------------\n");
	return x;
}

int match_k_token(eTOKENS token, int k)
{
	int index = startIndex == 0 ? 0 : startIndex;
	int temp = currentNode->tokensArray[index + k].kind;
	int x = temp == token ? 1 : 0;
	char *result = x == 1 ? "\033[1;32m✓ match" : "\033[1;31m✗ no match";

	matchIndex++;
	printf("\033[0;33mMatching %d token: \033[1;31m%s (%d)\033[22;0m  |  \033[1;32m%s  \033", k, enumDisplayName[temp], index, enumDisplayName[token]);
	printf("%s\033[22;0m\n", result);
	//printf("Maching indexs (current:%d, matching: %d\n", startIndex, matchIndex);
	printf("-----------------------------------------------\n");
	return x;
}

void print_grammer(eRULE root, eRULE leaf)
{
	fprintf(yyout, "Rule (%s -> %s)\n", enumDisplayRuleName[root],
			enumDisplayRuleName[leaf]);
	printf("\033[1;33mRule (%s -> %s)\033[22;0m\n", enumDisplayRuleName[root],
		   enumDisplayRuleName[leaf]);
	printf("-----------------------------------------------\n");
}

void print_error(Token *source_token, eTOKENS target_token_type)
{
	fprintf(yyout, "Expected token of type '%s' at line: %d, Actual token of type '%s', lexeme: '%s'.\n",
			enumDisplayName[target_token_type],
			source_token->lineNumber,
			enumDisplayName[source_token->kind],
			source_token->lexeme);
	printf("\033[0;31mExpected token of type '%s' at line: %d, Actual token of type '%s', lexeme: '%s'.\n",
		   enumDisplayName[target_token_type],
		   source_token->lineNumber,
		   enumDisplayName[source_token->kind],
		   source_token->lexeme);
	printf("\033[22;0m-----------------------------------------------\n");
}

NodeFollow **init_follow_rules()
{

	if (rulesArray != NULL)
	{
		return rulesArray;
	}
	rulesArray = (NodeFollow *)calloc(sizeof(NodeFollow *), RULES_SIZE);
	//PARSE_PROG
	rulesArray[0] = create_node_follow(TOKEN_END_OF_FILE);
	//PARSE_GLOBAL_VARS
	NodeFollow *temp = create_node_follow(TOKEN_TYPE);
	NodeFollow *head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);

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
	rulesArray[3] = create_node_follow(TOKEN_VAR);
	//PARSE_DIM_SIZES
	rulesArray[4] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
	//PARSE_FUNC_PREDEFS
	temp = create_node_follow(TOKEN_TYPE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[5] = head;
	// PARSE_FUNC_PREDEFS2,
	rulesArray[6] = head;
	// PARSE_FUNC_PROTOTYPE,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_TYPE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[7] = head;
	// PARSE_FUNC_FULL_DEFS,
	rulesArray[8] = create_node_follow(TOKEN_END_OF_FILE);
	// PARSE_FUNC_WITH_BODY,
	temp = create_node_follow(TOKEN_END_OF_FILE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_TYPE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[9] = head;
	// PARSE_RETURNED_TYPE,
	rulesArray[10] = create_node_follow(TOKEN_VAR);
	// PARSE_PARAMS,
	rulesArray[11] = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	// PARSE_PARAM_LIST,
	rulesArray[12] = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	// PARSE_PARAM_LIST2,
	rulesArray[13] = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	// PARSE_PARAM,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	rulesArray[14] = head;
	// PARSE_COMP_STMT,
	temp = create_node_follow(TOKEN_END_OF_FILE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_TYPE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[15] = head;
	// PARSE_VAR_DEC_LIST,
	temp = create_node_follow(TOKEN_VAR);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_IF);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_RETURN);
	rulesArray[16] = head;
	// PARSE_VAR_DEC_LIST2,
	rulesArray[17] = head;
	// PARSE_STMT_LIST,
	rulesArray[18] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	// PARSE_STMT_LIST2,
	rulesArray[19] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	// PARSE_STMT,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	rulesArray[20] = head;

	// PARSE_IF_STMT,
	rulesArray[21] = head;

	// PARSE_CALL,
	rulesArray[22] = head;

	// PARSE_ARGS,
	rulesArray[23] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_ARGS_LIST,
	rulesArray[24] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_ARGS_LIST2,
	rulesArray[25] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_RETURN_STMT,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[26] = head;

	// PARSE_VAR,
	rulesArray[27] = create_node_follow(TOKEN_COMMAND);

	// PARSE_EXPR_LIST,
	rulesArray[28] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_EXPR_LIST2,
	rulesArray[29] = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);

	// PARSE_CONDITION,
	rulesArray[30] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);

	// PARSE_EXPR,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_COMPARISON);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
	rulesArray[31] = head;

	// PARSE_EXPR2,
	rulesArray[32] = head;

	// PARSE_TERM,
	temp = create_node_follow(TOKEN_ADD);
	head = temp;
	temp = temp->next;
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
	rulesArray[33] = head;

	// PARSE_TERM2,
	rulesArray[34] = head;

	// PARSE_FACTOR,
	temp = create_node_follow(TOKEN_MUL);
	head = temp;
	temp = temp->next;
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
	rulesArray[35] = head;
	// PARSE_DIM_SIZE2,
	rulesArray[36] = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	// PARSE_FUNC_FULL_DEFS2,
	rulesArray[37] = create_node_follow(TOKEN_END_OF_FILE);
	// PARSE_PARAM2,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_COMMA);
	rulesArray[38] = head;
	// PARSE_RETURN_STMT2,
	temp = create_node_follow(TOKEN_SEPERATION_SEMICOLON);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
	rulesArray[39] = head;
	// PARSE_VAR2,
	temp = create_node_follow(TOKEN_SEPARATION_BRACKET_CLOSE);
	head = temp;
	temp = temp->next;
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
	rulesArray[40] = head;
	// PARSE_VAR_DEC2
	temp = create_node_follow(TOKEN_TYPE);
	head = temp;
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_VAR);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_IF);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_RETURN);
	temp = temp->next;
	temp->next = create_node_follow(TOKEN_FUNCTION);
	rulesArray[41] = head;
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
