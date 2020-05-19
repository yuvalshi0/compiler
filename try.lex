%option noyywrap
%{
#pragma warning (disable : 4996)
#include "token.h"
#include "parser.h"

int counter=1;
%}
FUNCTION "void"
CONDITION "if"|"while"
RETURN_VALUE "return"
TYPE "int"|"float"
NUMS [0-9] 
NUMS_WITHOUT_ZERO [1-9]
LOWER_CASE_ALPHA [a-z] 
UPPER_CASE_ALPHA [A-Z] 
UNDERSCORE [_] 
NEW_LINE "\n" 
WHITE_T "\t"
WHITE_SPACE " "
OPERATION_MUL [*]
OPERATION_ADD [+]
OPERATION_MINUS [-]
OPERATION_COMPARISON_G_E "<="
OPERATION_COMPARISON_L_E ">="
OPERATION_COMPARISON_E "=="
OPERATION_COMPARISON_D "!="
OPERATION_COMPARISON_GREATER [<]
OPERATION_COMPARISON_LOWER [>]
OPERATION_COMMAND [=]
COMMENTS "/*"([^*]|\*+[^*/])*\*+"/"
COMMENTS_MULTY_LINE "/*"(.[\r\n])*"*/"
SEPARATION_COMMA ","
SEPARATION_COLON ":"
SEPERATION_SEMICOLON ";"
SEPARATION_BRACKET_OPEN "("
SEPARATION_BRACKET_CLOSE ")"
SEPARATION_SQUARE_BRACKET_OPEN "["
SEPARATION_SQUARE_BRACKET_CLOSE "]"
SEPARATION_CURLY_BRACKET_OPEN "{"
SEPARATION_CURLY_BRACKET_CLOSE "}"
OTHER .

%%

{COMMENTS_MULTY_LINE}|{COMMENTS} {   for(int i= 0 ; i < yyleng; i++) {
                if(yytext[i]=='\n')  counter++; };
                }


{FUNCTION}       {  create_and_store_token(TOKEN_FUNCTION, yytext, counter); } 
{CONDITION}       {  create_and_store_token(TOKEN_IF, yytext, counter); }  
{RETURN_VALUE}       {  create_and_store_token(TOKEN_RETURN, yytext, counter); }  
{TYPE}         { create_and_store_token(TOKEN_TYPE, yytext, counter); }  
{SEPARATION_COMMA} { create_and_store_token(TOKEN_SEPARATION_COMMA, yytext, counter);}    
{SEPARATION_COLON} { create_and_store_token(TOKEN_SEPARATION_COLON, yytext, counter);}
{SEPERATION_SEMICOLON} { create_and_store_token(TOKEN_SEPERATION_SEMICOLON, yytext, counter);}
{SEPARATION_BRACKET_OPEN} { create_and_store_token(TOKEN_SEPARATION_BRACKET_OPEN, yytext, counter);}
{SEPARATION_BRACKET_CLOSE} { create_and_store_token(TOKEN_SEPARATION_BRACKET_CLOSE, yytext, counter);}
{SEPARATION_SQUARE_BRACKET_OPEN} { create_and_store_token(TOKEN_SEPARATION_SQUARE_BRACKET_OPEN, yytext, counter);}
{SEPARATION_SQUARE_BRACKET_CLOSE} { create_and_store_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE, yytext, counter);}
{SEPARATION_CURLY_BRACKET_OPEN} { create_and_store_token(TOKEN_SEPARATION_CURLY_BRACKET_OPEN, yytext, counter);}
{SEPARATION_CURLY_BRACKET_CLOSE} { create_and_store_token(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE, yytext, counter);}
{OPERATION_ADD} { create_and_store_token(TOKEN_ADD, yytext, counter);  }

{OPERATION_ADD}?({NUMS_WITHOUT_ZERO}*{NUMS})((("."{NUMS}+)+("e"("-")?("+")?{NUMS_WITHOUT_ZERO}+{NUMS}*))|(("."{NUMS}+)+("e"("-")?("+")?"0")))   {create_and_store_token(TOKEN_FLOAT, yytext, counter); }
{OPERATION_ADD}?({NUMS_WITHOUT_ZERO}{NUMS}*|{NUMS})   {create_and_store_token(TOKEN_INTEGER, yytext, counter); } 
{LOWER_CASE_ALPHA}({LOWER_CASE_ALPHA}|{UPPER_CASE_ALPHA}|{NUMS})*?({UNDERSCORE}({LOWER_CASE_ALPHA}|{UPPER_CASE_ALPHA}|{NUMS})+)*?  {  create_and_store_token(TOKEN_VAR, yytext, counter);                 }
{OPERATION_MINUS}		 {  create_and_store_token(TOKEN_MINUS, yytext, counter); }  
{OPERATION_MUL}       {  create_and_store_token(TOKEN_MUL, yytext, counter); }  
{OPERATION_COMPARISON_G_E}|{OPERATION_COMPARISON_L_E}|{OPERATION_COMPARISON_E}|{OPERATION_COMPARISON_D} {create_and_store_token(TOKEN_COMPARISON, yytext, counter); }
{OPERATION_COMPARISON_GREATER}|{OPERATION_COMPARISON_LOWER} {create_and_store_token(TOKEN_COMPARISON, yytext, counter); }
{OPERATION_COMMAND} { create_and_store_token(TOKEN_COMMAND, yytext, counter);}
{NEW_LINE}			{ counter++;}
{WHITE_SPACE}|{WHITE_T}	{ }
{OTHER} {
   
	 fprintf(yyout,"Character '%s' in line: %d does not begin any legal token in the language\n",yytext,counter);


}
<<EOF>>        { create_and_store_token(TOKEN_END_OF_FILE, yytext, counter); yyterminate(); }

%%
void main(){	
	yyin = fopen("./input.txt","r");
	yyout= fopen("./output.txt","w");
	yylex();
	print_tokens();
	parse_prog();
	fclose(yyin);
	fclose(yyout);	

	

}