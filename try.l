%option noyywrap
%{
#pragma warning (disable : 4996)
#include "Token.h"
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
OPERATION_COMMAND [=]
OPERATION_ADD [+]
OPERATION_MINUS [-]
OPERATION_COMPARISON "<"|"<="|"=="|">="|">"|"!=" 
COMMENTS "/*"([^*]|\*+[^*/])*\*+"/"
COMMENTS_MULTY_LINE "/*"(.[\r\n])*"*/"
SEPARATION_SIGNS ","|":"|";"|"("|")"|"]"|"["|"{"|"}"
OTHER .

%%

{COMMENTS_MULTY_LINE}|{COMMENTS} {   for(int i= 0 ; i < yyleng; i++) {
                if(yytext[i]=='\n')  counter++; };
                }


{FUNCTION}       {  create_and_store_token(TOKEN_FUNCTION, yytext, counter); } 
{CONDITION}       {  create_and_store_token(TOKEN_IF, yytext, counter); }  
{RETURN_VALUE}       {  create_and_store_token(TOKEN_RETURN, yytext, counter); }  
{TYPE}         { create_and_store_token(TOKEN_TYPE, yytext, counter); }  
{SEPARATION_SIGNS} { create_and_store_token(TOKEN_SEPERATION, yytext, counter);}    
{OPERATION_ADD}?({NUMS_WITHOUT_ZERO}*{NUMS})((("."{NUMS}+)+("e"("-")?("+")?{NUMS_WITHOUT_ZERO}+{NUMS}*))|(("."{NUMS}+)+("e"("-")?("+")?"0")))   {create_and_store_token(TOKEN_FLOAT, yytext, counter); }
{OPERATION_ADD}?({NUMS_WITHOUT_ZERO}{NUMS}*|{NUMS})   {create_and_store_token(TOKEN_INTEGER, yytext, counter); } 
{LOWER_CASE_ALPHA}({LOWER_CASE_ALPHA}|{UPPER_CASE_ALPHA}|{NUMS})*?({UNDERSCORE}({LOWER_CASE_ALPHA}|{UPPER_CASE_ALPHA}|{NUMS})+)*?  {  create_and_store_token(TOKEN_VAR, yytext, counter);                 }
{OPERATION_COMMAND}|{OPERATION_MINUS}|{OPERATION_ADD}|{OPERATION_MUL}       {  create_and_store_token(TOKEN_OPERATION, yytext, counter); }  
{OPERATION_COMPARISON} {create_and_store_token(TOKEN_COMPARISON, yytext, counter); }
{NEW_LINE}			{ counter++;}
{WHITE_SPACE}|{WHITE_T}	{ }
{OTHER} {
   
	 fprintf(yyout,"Character '%s' in line: %d does not begin any legal token in the language\n",yytext,counter);


}
<<EOF>>        { create_and_store_token(TOKEN_END_OF_FILE, yytext, counter); yyterminate(); }

%%
void main(){	
	// yyin = fopen("C:\\temp\\test1.txt","r");
	// yyout= fopen("C:\\temp\\test1_314076571_313257750_321866477_lex.txt","w");
	yyin = fopen("./input.txt","r");
	yyout= fopen("./output.txt","w");
	yylex();
	//print_tokens();
	parse_prog();
	fclose(yyin);
	fclose(yyout);	

	counter=1;
	yyin= NULL;
	// yyrestart(yyin);
	// yyin = fopen("C:\\temp\\test2.txt","r");
	// yyout= fopen("C:\\temp\\test2_314076571_313257750_321866477_lex.txt","w"); 	

	// yylex();
	// print_tokens();
	// fclose(yyin);
	// fclose(yyout);	

}