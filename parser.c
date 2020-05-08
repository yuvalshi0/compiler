#include "parser.h"

void parse_prog()
{
    print_grammer(PARSE_PROG, PARSE_GLOBAL_VARS);
    parse_global_vars();
    print_grammer(PARSE_PROG, PARSE_FUNC_PREDEFS);
    parse_func_predefs();
    print_grammer(PARSE_PROG, PARSE_FUNC_FULL_DEFS);
    parse_func_full_defs();
    print_grammer(PARSE_PROG, PARSE_END_OF_FILE);
    match_token(TOKEN_END_OF_FILE);
}
void parse_global_vars()
{
    if (match_k_char("(", 3))
    {
        return;
        print_grammer(PARSE_GLOBAL_VARS, PARSE_VAR_DEC);
        parse_var_dec();
    }
    else
    {
        print_grammer(PARSE_GLOBAL_VARS, PARSE_GLOBAL_VARS2);
        parse_global_vars2();
    }
}
void parse_global_vars2()
{
    if (match_token(TOKEN_TYPE) && (match_k_char("(", 3) == 0))
    {
        parse_var_dec();
        print_grammer(PARSE_GLOBAL_VARS2, PARSE_GLOBAL_VARS2);
        parse_global_vars2();
    }
    else
    {
        return;
    }
}
void parse_var_dec()
{
    parse_type();
    print_grammer(PARSE_VAR_DEC, PARSE_TYPE);
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_VAR:
        t = next_token(__LINE__);
        switch (*(t->lexeme))
        {
        case ';':
            break;
        case '[':
            print_grammer(PARSE_VAR_DEC, PARSE_DIM_SIZES);
            parse_dim_sizes();
            t = next_token(__LINE__);
            if (strcmp(t->lexeme, "]") != 0)
            {
                print_error(t, TOKEN_SEPERATION);
                break;
            }
            if (match_char(";") == 0)
            {
                print_error(next_token(__LINE__), TOKEN_SEPERATION);
                break;
            }
            next_token(__LINE__);
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
    Token *t = next_token(__LINE__);
    if (t->kind != TOKEN_TYPE)
    {
        print_error(t, TOKEN_TYPE);
    }
}
void parse_dim_sizes()
{
    Token *t = next_token(__LINE__);
    if (t->kind == TOKEN_INTEGER)
    {
        if (match_char(","))
        {
            next_token(__LINE__);
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
    print_grammer(PARSE_FUNC_PREDEFS, PARSE_FUNC_PROTOTYPE);
    parse_func_prototype();
    if (match_char(";"))
    {
        next_token(__LINE__);
        if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
        {
            print_grammer(PARSE_FUNC_PREDEFS, PARSE_FUNC_PREDEFS2);
            parse_func_predefs2();
        }
    }
    else
    {
        if (match_char("{"))
        {
            print_grammer(PARSE_FUNC_PROTOTYPE, PARSE_COMP_STMT);
            parse_comp_stmt();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPERATION);
        }
    }
}
void parse_func_predefs2()
{
    print_grammer(PARSE_FUNC_PREDEFS2, PARSE_FUNC_PROTOTYPE);
    parse_func_prototype();
    if (match_char(";"))
    {
        next_token(__LINE__);
        if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
        {
            print_grammer(PARSE_FUNC_PREDEFS2, PARSE_FUNC_PREDEFS2);
            parse_func_predefs2();
        }
    }
    else
    {
        if (match_char("{"))
        {
            print_grammer(PARSE_FUNC_PROTOTYPE, PARSE_COMP_STMT);
            parse_comp_stmt();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPERATION);
        }
    }
}
void parse_func_prototype()
{
    parse_returned_type();
    Token *t = next_token(__LINE__);
    if (t->kind == TOKEN_VAR)
    {
        t = next_token(__LINE__);
        if (strcmp(t->lexeme, "(") != 0)
        {
            print_error(t, TOKEN_SEPERATION);
        }
        parse_params();
        t = next_token(__LINE__);
        if (strcmp(t->lexeme, ")") != 0)
        {
            print_error(t, TOKEN_SEPERATION);
        }
    }
    else
    {
        print_error(t, TOKEN_VAR);
    }
}
void parse_func_full_defs()
{
    if (match_token(TOKEN_END_OF_FILE))
    {
        return;
    }
    print_grammer(PARSE_FUNC_FULL_DEFS, PARSE_FUNC_WITH_BODY);
    parse_func_with_body();
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        print_grammer(PARSE_FUNC_FULL_DEFS, PARSE_FUNC_FULL_DEFS);
        parse_func_full_defs();
    }
}
void parse_func_with_body()
{
    print_grammer(PARSE_FUNC_WITH_BODY, PARSE_FUNC_PROTOTYPE);
    parse_func_prototype();
    print_grammer(PARSE_FUNC_WITH_BODY, PARSE_COMP_STMT);
    parse_comp_stmt();
}
void parse_returned_type()
{
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_TYPE:
        return;
        break;
    case TOKEN_FUNCTION:
        return;
        break;
    default:
        print_error(t, TOKEN_FUNCTION);
    }
}

void parse_params()
{
    if (match_char(")"))
    {
        return;
    }
    else
    {
        parse_param_list();
    }
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
        next_token(__LINE__);
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
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_VAR:
        if (match_char("["))
        {
            next_token(__LINE__);
            print_grammer(PARSE_PARAM, PARSE_DIM_SIZES);
            parse_dim_sizes();
            t = next_token(__LINE__);
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
    if (match_char("{"))
    {
        next_token(__LINE__);
        print_grammer(PARSE_COMP_STMT, PARSE_VAR_DEC_LIST);
        parse_var_dec_list();
        print_grammer(PARSE_COMP_STMT, PARSE_STMT_LIST);
        parse_stmt_list();
        if (match_char("}"))
        {
            next_token(__LINE__);
            return;
        }
        else
        {
            print_error(next_token(), TOKEN_SEPERATION);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_SEPERATION);
    }
}
void parse_var_dec_list()
{
    print_grammer(PARSE_VAR_DEC_LIST, PARSE_VAR_DEC);
    parse_var_dec();
    if (match_token(TOKEN_TYPE))
    {
        parse_var_dec_list2();
    }
}
void parse_var_dec_list2()
{
    print_grammer(PARSE_VAR_DEC_LIST2, PARSE_VAR_DEC);
    parse_var_dec();
    if (match_token(TOKEN_TYPE))
    {
        print_grammer(PARSE_VAR_DEC_LIST2, PARSE_VAR_DEC_LIST2);
        parse_var_dec_list2();
    }
    else
    {
        return;
    }
}

void parse_stmt_list()
{
    print_grammer(PARSE_STMT_LIST, PARSE_STMT);
    parse_stmt();
    if (match_char(";"))
    {
        next_token(__LINE__);
        if (match_token(TOKEN_VAR) || match_token(TOKEN_IF) || match_token(TOKEN_RETURN) || match_char("{"))
        {
            parse_stmt_list2();
        }
    }
    else
    {
        print_error(next_token(), TOKEN_SEPERATION);
    }
}
void parse_stmt_list2()
{
    print_grammer(PARSE_STMT_LIST2, PARSE_STMT);
    parse_stmt();
    if (match_char(";"))
    {
        next_token(__LINE__);
        if (match_token(TOKEN_VAR) || match_token(TOKEN_IF) || match_token(TOKEN_RETURN) || match_char("{"))
            parse_stmt_list2();
    }
}
void parse_stmt()
{
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_VAR:
        back_token(__LINE__);
        if (match_k_char("(", 2))
        {
            print_grammer(PARSE_STMT, PARSE_CALL);
            parse_call();
        }
        else
        {
            print_grammer(PARSE_STMT, PARSE_VAR);
            parse_var();
            if (match_char("="))
            {
                next_token(__LINE__);
                parse_expr();
            }
            else
            {
                print_error(t, TOKEN_OPERATION);
            }
        }
        break;
    case TOKEN_IF:
        print_grammer(PARSE_STMT, PARSE_IF_STMT);
        back_token(__LINE__);
        parse_if_stmt();
        break;
    case TOKEN_RETURN:
        print_grammer(PARSE_STMT, PARSE_RETURN_STMT);
        back_token(__LINE__);
        parse_return_stmt();
        break;
    case TOKEN_SEPERATION:
        back_token();
        if (match_char("{"))
        {
            print_grammer(PARSE_STMT, PARSE_COMP_STMT);
            parse_comp_stmt();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPERATION);
        }

        break;
    default:
        //PRINT WHAT ERROR?
        break;
    }
}
void parse_if_stmt()
{
    if (match_token(TOKEN_IF))
    {
        next_token();
        if (match_char("("))
        {
            next_token(__LINE__);
            print_grammer(PARSE_IF_STMT, PARSE_CONDITION);
            parse_condition();
            if (match_char(")"))
            {
                next_token(__LINE__);
                print_grammer(PARSE_IF_STMT, PARSE_STMT);
                parse_stmt();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPERATION);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPERATION);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_IF);
    }
}
void parse_call()
{
    Token *t = next_token(__LINE__);
    if (t->kind == TOKEN_VAR)
    {
        if (match_char("("))
        {
            next_token(__LINE__);
            print_grammer(PARSE_CALL, PARSE_ARGS);
            parse_args();
            if (match_char(")"))
            {
                next_token(__LINE__);
            }
            else
            {
                print_error(t, TOKEN_SEPERATION);
            }
        }
        else
        {
            print_error(t, TOKEN_SEPERATION);
        }
    }
    else
    {
        print_error(t, TOKEN_VAR);
    }
}
void parse_args()
{
    if (match_char(")"))
    {
        return;
    }
    else
    {
        print_grammer(PARSE_ARGS, PARSE_ARGS_LIST);
        parse_args_list();
    }
}
void parse_args_list()
{
    print_grammer(PARSE_ARGS_LIST, PARSE_EXPR);
    parse_expr();
    print_grammer(PARSE_ARGS_LIST, PARSE_ARGS_LIST2);
    parse_args_list2();
}
void parse_args_list2()
{
    if (match_char(","))
    {
        print_grammer(PARSE_ARGS_LIST2, PARSE_EXPR);
        next_token(__LINE__);
        parse_expr();
        print_grammer(PARSE_ARGS_LIST2, PARSE_ARGS_LIST2);
        parse_args_list2();
    }
}
void parse_return_stmt()
{
    Token *t = next_token(__LINE__);
    if (t->kind == TOKEN_RETURN)
    {
        if (match_token(TOKEN_VAR) || match_token(TOKEN_INTEGER) || match_token(TOKEN_FLOAT) || match_char("("))
        {
            print_grammer(PARSE_RETURNED_TYPE, PARSE_EXPR);
            parse_expr();
        }
    }
    else
    {
        print_error(t, TOKEN_RETURN);
    }
}
void parse_var()
{
    Token *t = next_token(__LINE__);
    if (t->kind == TOKEN_VAR)
    {
        if (match_char("["))
        {
            next_token(__LINE__);
            print_grammer(PARSE_VAR, PARSE_EXPR_LIST);
            parse_expr_list();
            if (match_char("]"))
            {
                next_token(__LINE__);
                return;
            }
            else
            {
                print_error(t, TOKEN_SEPERATION);
            }
        }
    }
    else
    {
        print_error(t, TOKEN_VAR);
    }
}
void parse_expr_list()
{
    print_grammer(PARSE_EXPR_LIST, PARSE_EXPR);
    parse_expr();
    if (match_char(","))
    {
        next_token(__LINE__);
        print_grammer(PARSE_EXPR_LIST, PARSE_EXPR_LIST2);
        parse_expr_list2();
    }
    else
    {
        return;
    }
}
void parse_expr_list2()
{
    print_grammer(PARSE_EXPR_LIST2, PARSE_EXPR_LIST);
    parse_expr();
    if (match_char(","))
    {
        next_token(__LINE__);
        print_grammer(PARSE_EXPR_LIST2, PARSE_EXPR_LIST2);
        parse_expr_list2();
    }
}
void parse_condition()
{
    print_grammer(PARSE_CONDITION, PARSE_EXPR);
    parse_expr();
    if (match_token(TOKEN_COMPARISON))
    {
        next_token(__LINE__);
    }
    else
    {
        print_error(next_token(), TOKEN_COMPARISON);
    }
    print_grammer(PARSE_CONDITION, PARSE_EXPR);
    parse_expr();
}
void parse_expr()
{
    print_grammer(PARSE_EXPR, PARSE_TERM);
    parse_term();
    if (match_char("+"))
    {
        print_grammer(PARSE_EXPR, PARSE_EXPR2);
        parse_expr2();
    }
}
void parse_expr2()
{
    if (match_char("+"))
    {
        print_grammer(PARSE_EXPR2, PARSE_TERM2);
        next_token(__LINE__);
        parse_term();
        print_grammer(PARSE_EXPR2, PARSE_EXPR2);
        parse_expr2();
    }
}
void parse_term()
{
    print_grammer(PARSE_TERM, PARSE_FACTOR);
    parse_factor();
    if (match_char("*"))
    {
        next_token(__LINE__);
        print_grammer(PARSE_TERM, PARSE_TERM2);
        parse_term2();
    }
}
void parse_term2()
{
    print_grammer(PARSE_TERM2, PARSE_FACTOR);
    parse_factor();
    if (match_char("*"))
    {
        next_token(__LINE__);
        print_grammer(PARSE_TERM2, PARSE_TERM2);
        parse_term2();
    }
}
void parse_factor()
{
    Token *t = next_token(__LINE__);

    switch (t->kind)
    {
    case TOKEN_INTEGER:
        return;
        break;
    case TOKEN_FLOAT:
        return;
    case TOKEN_VAR:
        if (match_char("("))
        {
            print_grammer(PARSE_FACTOR, PARSE_CALL);
            back_token();
            parse_call();
        }
        else
        {
            print_grammer(PARSE_FACTOR, PARSE_VAR);
            back_token();
            parse_var();
        }
        break;
    case TOKEN_SEPERATION:
        back_token();
        if (match_char("("))
        {
            next_token(__LINE__);
            print_grammer(PARSE_FACTOR, PARSE_EXPR);
            parse_expr();
            if (match_char(")"))
            {
                next_token(__LINE__);
            }
            else
            {
                print_error(t, TOKEN_SEPERATION);
            }
        }
        else
        {
            print_error(t, TOKEN_SEPERATION);
        }
        break;
    default:
        //PRINTERROR??
        break;
    }
}
