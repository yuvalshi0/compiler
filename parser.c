#include "parser.h"

void parse_prog()
{
    parse_global_vars();
    parse_func_predefs();
    parse_func_full_defs();
    parse_param_list();
    if (match_token(TOKEN_END_OF_FILE))
    {
        return;
    }
}
void parse_global_vars()
{
    print_grammer(PARSE_GLOBAL_VARS,PARSE_VAR_DEC);
    parse_var_dec();
    print_grammer(PARSE_GLOBAL_VARS,PARSE_GLOBAL_VARS2);
    parse_global_vars2();
}
void parse_global_vars2()
{
    parse_var_dec();
    if(match_token(TOKEN_TYPE)) {
        print_grammer(PARSE_GLOBAL_VARS2,PARSE_GLOBAL_VARS2);
        parse_global_vars2();
    } else {
        return;
    }
}
void parse_var_dec()
{
    parse_type();
    print_grammer(PARSE_VAR_DEC, PARSE_TYPE);
    Token *t = next_token();
    switch (t->kind)
    {
    case TOKEN_VAR:
        t = next_token();
        switch (*(t->lexeme))
        {
        case ';':
            break;
        case '[':
            print_grammer(PARSE_VAR_DEC, PARSE_DIM_SIZES);
            parse_dim_sizes();
            t = next_token();
            if (strcmp(t->lexeme, "]") != 0)
            {
                print_error(t, TOKEN_SEPERATION);
                break;
            }
            if (match_char(";") == 0)
            {
                print_error(next_token(), TOKEN_SEPERATION);
                break;
            }
            next_token();
            break;
        default:
            print_error(t, TOKEN_SEPERATION);
            break;
        }
        break;
    default:
        print_error(t, TOKEN_VAR);
        break;
    }
}
void parse_type()
{
    Token *t = next_token();
    if (t->kind != TOKEN_TYPE)
    {
        print_error(t, TOKEN_TYPE);
    }
}
void parse_dim_sizes()
{
    Token *t = next_token();
    if (t->kind == TOKEN_INTEGER)
    {
        if (match_char(","))
        {
            next_token();
            print_grammer(PARSE_DIM_SIZES, PARSE_DIM_SIZES);
            parse_dim_sizes();
        }
    }
    else
    {
        print_error(t, TOKEN_INTEGER);
    }
}
void parse_func_predefs()
{
}
void parse_func_predefs2()
{
}
void parse_func_prototype()
{
}
void parse_func_full_defs()
{
}
void parse_func_with_body()
{
}
void parse_returned_type()
{
}
void parse_params()
{
}
void parse_param_list()
{
    if (match_token(TOKEN_TYPE))
    {
        parse_param();
        print_grammer(PARSE_PARAM_LIST, PARSE_PARAM_LIST2);
        parse_param_list2();
    }
    else
    {
        return;
    }
}
void parse_param_list2()
{
    if (match_char(","))
    {
        next_token();
        print_grammer(PARSE_PARAM_LIST2, PARSE_PARAM_LIST2);
        parse_param();
        parse_param_list2();
    }
    else
    {
        return;
    }
}
void parse_param()
{
    parse_type();
    print_grammer(PARSE_PARAM, PARSE_TYPE);
    Token *t = next_token();
    switch (t->kind)
    {
    case TOKEN_VAR:
        if (match_char("["))
        {
            next_token();
            print_grammer(PARSE_PARAM, PARSE_DIM_SIZES);
            parse_dim_sizes();
            t = next_token();
            if (strcmp(t->lexeme, "]") != 0)
            {
                print_error(t, TOKEN_SEPERATION);
                break;
            }
        }
        break;
    default:
        print_error(t, TOKEN_VAR);
        break;
    }
}
void parse_comp_stmt()
{
}
void parse_var_dec_list()
{
}
void parse_var_dec_list2()
{
    if (match_token(TOKEN_TYPE))
    {
        print_grammer(PARSE_VAR_DEC_LIST2, PARSE_VAR_DEC);
        parse_var_dec();
        parse_var_dec_list2();
        
    }
    else
    {
        return;
    }
}

void parse_stmt_list()
{
    int x;
}
void parse_stmt_list2()
{
}
void parse_stmt()
{
}
void parse_if_stmt()
{
}
void parse_call()
{
}
void parse_args()
{
}
void parse_args_list()
{
}
void parse_args_list2()
{
}
void parse_return_stmt()
{
}
void parse_var()
{
}
void parse_expr_list()
{
}
void parse_expr_list2()
{
}
void parse_condition()
{
}
void parse_expr()
{
}
void parse_expr2()
{
}
void parse_term()
{
}
void parse_term2()
{
}
void parse_factor()
{
}
