#include "Token.h"

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

#define TOKEN_ARRAY_SIZE 1000
const char *enumDisplayName[] = {
	[TOKEN_FUNCTION] = "FUNCTION",
	[TOKEN_IF] = "IF or WHILE",
	[TOKEN_RETURN] = "RETURN",
	[TOKEN_OPERATION] = "OPERATION",
	[TOKEN_SEPERATION] = "SEPERATION",
	[TOKEN_INTEGER] = "INTEGER",
	[TOKEN_FLOAT] = "FLOAT",
	[TOKEN_OP_EQUAL] = "OP_EQUAL",
	[TOKEN_VAR] = "VARIABLE",
	[TOKEN_TYPE] = "VAR TYPE",
	[TOKEN_COMPARISON] = "COMPARISON",
	[TOKEN_END_OF_FILE] = "END_OF_FILE"};

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