#include "parser.h"
#include "token.h"

void parse_prog()
{
    print_grammer(PARSE_PROG);
    parse_global_vars();
    parse_func_predefs();
    parse_func_full_defs();
}
void parse_global_vars()
{
    print_grammer(PARSE_GLOBAL_VARS);
    parse_var_dec();
    parse_global_vars2();
}

void parse_global_vars2()
{
    print_grammer(PARSE_GLOBAL_VARS2);
    if (match_token(TOKEN_TYPE) && (match_k_token(TOKEN_SEPARATION_BRACKET_OPEN, 3) == 0))
    {
        parse_var_dec();
        parse_global_vars2();
    }
}
void parse_var_dec()
{
    print_grammer(PARSE_VAR_DEC);
    parse_type();
    if (match_token(TOKEN_VAR))
    {
        next_token();
        parse_var_dec2();
    }
    else
    {
        print_error(next_token(), TOKEN_TYPE);
        error_recovery(PARSE_VAR_DEC);
    }
}
void parse_type()
{
    print_grammer(PARSE_TYPE);
    if (match_token(TOKEN_TYPE))
    {
        next_token();
    }
    else
    {
        print_error(next_token(), TOKEN_TYPE);
        error_recovery(PARSE_TYPE);
    }
}
void parse_dim_sizes()
{
    print_grammer(PARSE_DIM_SIZES);
    if (match_token(TOKEN_INTEGER))
    {
        next_token();
        parse_dim_sizes2();
    }
    else
    {
        print_error(next_token(), TOKEN_INTEGER);
        error_recovery(PARSE_DIM_SIZES);
    }
}
void parse_func_predefs()
{
    print_grammer(PARSE_FUNC_PREDEFS);
    parse_func_prototype();
    if (match_token(TOKEN_SEPERATION_SEMICOLON))
    {
        next_token();
        parse_func_predefs2();
    }
    else
    {
        print_error(next_token(), TOKEN_SEPERATION_SEMICOLON);
        error_recovery(PARSE_FUNC_PREDEFS);
    }
}
void parse_func_predefs2()
{
    print_grammer(PARSE_FUNC_PREDEFS2);
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        parse_func_prototype();
        if (match_token(TOKEN_SEPERATION_SEMICOLON))
        {
            next_token();
            parse_func_predefs2();
        }
        else
        {
            parse_comp_stmt();
        }
    }
}
void parse_func_prototype()
{
    print_grammer(PARSE_FUNC_PROTOTYPE);
    parse_returned_type();
    if (match_token(TOKEN_VAR))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token();
            parse_params();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
                error_recovery(PARSE_FUNC_PROTOTYPE);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_OPEN);
            error_recovery(PARSE_FUNC_PROTOTYPE);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
        error_recovery(PARSE_FUNC_PROTOTYPE);
    }
}
void parse_func_full_defs()
{
    print_grammer(PARSE_FUNC_FULL_DEFS);
    if (match_token(TOKEN_END_OF_FILE) == 0)
    {
        parse_func_with_body();
        parse_func_full_defs2();
    }
}
void parse_func_with_body()
{
    print_grammer(PARSE_FUNC_WITH_BODY);
    parse_func_prototype();
    parse_comp_stmt();
}
void parse_returned_type()
{
    print_grammer(PARSE_RETURNED_TYPE);
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        next_token();
    }
    else
    {
        print_error(next_token(), TOKEN_TYPE);
        error_recovery(PARSE_RETURNED_TYPE);
    }
}

void parse_params()
{
    print_grammer(PARSE_PARAMS);
    if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE) == 0)
    {
        parse_param_list();
    }
}
void parse_param_list()
{
    print_grammer(PARSE_PARAM_LIST);
    parse_param();
    parse_param_list2();
}
void parse_param_list2()
{
    print_grammer(PARSE_PARAM_LIST2);
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token(__LINE__);
        parse_param();
        parse_param_list2();
    }
}
void parse_param()
{
    print_grammer(PARSE_PARAM);
    parse_type();
    if (match_token(TOKEN_VAR))
    {
        next_token();
        parse_param2();
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
        error_recovery(PARSE_PARAM);
    }
}
void parse_comp_stmt()
{
    print_grammer(PARSE_COMP_STMT);
    if (match_token(TOKEN_SEPARATION_CURLY_BRACKET_OPEN))
    {
        next_token(__LINE__);
        parse_var_dec_list();
        parse_stmt_list();
        if (match_token(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE))
        {
            next_token(__LINE__);
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
            error_recovery(PARSE_COMP_STMT);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_SEPARATION_CURLY_BRACKET_OPEN);
        error_recovery(PARSE_COMP_STMT);
    }
}
void parse_var_dec_list()
{
    print_grammer(PARSE_VAR_DEC_LIST);
    if (match_token(TOKEN_TYPE))
    {
        parse_var_dec_list2();
    }
}
void parse_var_dec_list2()
{
    print_grammer(PARSE_VAR_DEC_LIST2);
    parse_var_dec();
    if (match_token(TOKEN_TYPE))
    {
        parse_var_dec_list2();
    }
}

void parse_stmt_list()
{
    print_grammer(PARSE_STMT_LIST);
    parse_stmt();
    parse_stmt_list2();
}

void parse_stmt_list2()
{
    print_grammer(PARSE_STMT_LIST2);
    if (match_token(TOKEN_SEPERATION_SEMICOLON))
    {
        next_token();
        parse_stmt();
        parse_stmt_list2();
    }
}
void parse_stmt()
{
    print_grammer(PARSE_STMT);
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_VAR:
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            back_token(__LINE__);
            parse_call();
        }
        else
        {
            back_token();
            parse_var();
            if (match_token(TOKEN_COMMAND))
            {
                next_token(__LINE__);
                parse_expr();
            }
            else
            {
                print_error(t, TOKEN_COMMAND);
                error_recovery(PARSE_STMT);
            }
        }
        break;
    case TOKEN_IF:
        back_token();
        parse_if_stmt();
        break;
    case TOKEN_RETURN:
        back_token();
        parse_return_stmt();
        break;
    case TOKEN_SEPARATION_CURLY_BRACKET_OPEN:
        back_token();
        parse_comp_stmt();
        break;
    default:
        print_error(t, TOKEN_VAR);
        print_error(t, TOKEN_IF);
        print_error(t, TOKEN_RETURN);
        print_error(t, TOKEN_SEPARATION_CURLY_BRACKET_OPEN);
        error_recovery(PARSE_STMT);
        break;
    }
}
void parse_if_stmt()
{
    print_grammer(PARSE_IF_STMT);
    if (match_token(TOKEN_IF))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token(__LINE__);
            parse_condition();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token(__LINE__);
                parse_stmt();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
                error_recovery(PARSE_IF_STMT);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_OPEN);
            error_recovery(PARSE_IF_STMT);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_IF);
        error_recovery(PARSE_IF_STMT);
    }
}
void parse_call()
{
    print_grammer(PARSE_CALL);
    if (match_token(TOKEN_VAR))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token(__LINE__);
            parse_args();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token(__LINE__);
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
                error_recovery(PARSE_CALL);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_OPEN);
            error_recovery(PARSE_CALL);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
        error_recovery(PARSE_CALL);
    }
}
void parse_args()
{
    print_grammer(PARSE_ARGS);
    if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE) == 0)
    {
        parse_args_list();
    }
}
void parse_args_list()
{
    print_grammer(PARSE_ARGS_LIST);
    parse_expr();
    parse_args_list2();
}
void parse_args_list2()
{
    print_grammer(PARSE_ARGS_LIST2);
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token(__LINE__);
        parse_expr();
        parse_args_list2();
    }
}
void parse_return_stmt()
{
    print_grammer(PARSE_RETURN_STMT);
    if (match_token(TOKEN_RETURN))
    {
        next_token();
        parse_return_stmt2();
    }
    else
    {
        print_error(next_token(), TOKEN_RETURN);
        error_recovery(PARSE_RETURN_STMT);
    }
}
void parse_var()
{
    print_grammer(PARSE_VAR);
    if (match_token(TOKEN_VAR))
    {
        next_token();
        parse_var2();
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
        error_recovery(PARSE_VAR);
    }
}
void parse_expr_list()
{
    print_grammer(PARSE_EXPR_LIST);
    parse_expr();
    parse_expr_list2();
}
void parse_expr_list2()
{
    print_grammer(PARSE_EXPR_LIST2);
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token();
        parse_expr();
        parse_expr_list2();
    }
}
void parse_condition()
{
    print_grammer(PARSE_CONDITION);
    parse_expr();
    if (match_token(TOKEN_COMPARISON))
    {
        next_token(__LINE__);
        parse_expr();
    }
    else
    {
        print_error(next_token(), TOKEN_COMPARISON);
        error_recovery(PARSE_CONDITION);
    }
}
void parse_expr()
{
    print_grammer(PARSE_EXPR);
    parse_term();
    parse_expr2();
}
void parse_expr2()
{
    print_grammer(PARSE_EXPR2);
    if (match_token(TOKEN_ADD))
    {
        next_token();
        parse_term();
        parse_expr2();
    }
}
void parse_term()
{
    print_grammer(PARSE_TERM);
    parse_factor();
    parse_term2();
}
void parse_term2()
{
    print_grammer(PARSE_TERM2);
    if (match_token(TOKEN_MUL))
    {
        next_token(__LINE__);
        parse_factor();
        parse_term2();
    }
}
void parse_factor()
{
    print_grammer(PARSE_FACTOR);
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_INTEGER:
        return;
        break;
    case TOKEN_FLOAT:
        return;
        break;
    case TOKEN_VAR:
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            back_token();
            parse_call();
        }
        else
        {
            back_token();
            parse_var();
        }
        break;
    case TOKEN_SEPARATION_BRACKET_OPEN:
        parse_expr();
        if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
        {
            next_token(__LINE__);
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
            error_recovery(PARSE_FACTOR);
        }
        break;
    default:
        print_error(t, TOKEN_INTEGER);
        print_error(t, TOKEN_FLOAT);
        print_error(t, TOKEN_VAR);
        print_error(t, TOKEN_SEPARATION_BRACKET_OPEN);
        error_recovery(PARSE_FACTOR);
        break;
    }
}

void parse_var_dec2()
{
    print_grammer(PARSE_VAR_DEC2);
    if (match_token(TOKEN_SEPERATION_SEMICOLON))
    {
        next_token();
    }
    else if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_OPEN))
    {
        next_token();
        parse_dim_sizes();
        if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE))
        {
            next_token();
            if (match_token(TOKEN_SEPERATION_SEMICOLON))
            {
                next_token();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPERATION_SEMICOLON);
                error_recovery(PARSE_VAR_DEC2);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
            error_recovery(PARSE_VAR_DEC2);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_SEPERATION_SEMICOLON);
        error_recovery(PARSE_VAR_DEC2);
    }
}

void parse_dim_sizes2()
{
    print_grammer(PARSE_DIM_SIZES2);
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token();
        if (match_token(TOKEN_INTEGER))
        {
            next_token();
            parse_dim_sizes2();
        }
        else
        {
            print_error(next_token(), TOKEN_INTEGER);
            error_recovery(PARSE_DIM_SIZES2);
        }
    }
}

void parse_func_full_defs2()
{
    print_grammer(PARSE_FUNC_FULL_DEFS2);
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        parse_func_with_body();
        parse_func_full_defs2();
    }
}

void parse_param2()
{
    print_grammer(PARSE_PARAM2);
    if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_OPEN))
    {
        next_token();
        parse_dim_sizes();
        if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE))
        {
            next_token();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_SQUARE_BRACKET_OPEN);
            error_recovery(PARSE_PARAM2);
        }
    }
}

void parse_return_stmt2()
{
    print_grammer(PARSE_RETURN_STMT2);
    if (match_token(TOKEN_VAR))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token();
            parse_args();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token();
                parse_term2();
                parse_expr2();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
                error_recovery(PARSE_RETURN_STMT2);
            }
        }
        else
        {
            parse_term2();
            parse_expr2();
        }
    }
    else if (match_token(TOKEN_INTEGER) || match_token(TOKEN_FLOAT))
    {
        next_token();
        parse_term2();
        parse_expr2();
    }
    else if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
    {
        next_token();
        parse_expr();
        if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
        {
            next_token();
            parse_term2();
            parse_expr2();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
            error_recovery(PARSE_RETURN_STMT2);
        }
    }
}

void parse_var2()
{
    print_grammer(PARSE_VAR2);
    if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_OPEN))
    {
        next_token();
        parse_expr_list();
        if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE))
        {
            next_token();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
            error_recovery(PARSE_VAR2);
        }
    }
}

int found_follow(Token *t, eRULE rule)
{
    NodeFollow **n = init_follow_rules();
    NodeFollow *followGroup = NULL;
    followGroup = n[rule];
    while (followGroup != NULL)
    {
        if (t->kind == followGroup->token)
        {
            return 1;
        }
        followGroup = followGroup->next;
    }

    return 0;
}

void error_recovery(eRULE rule)
{
    Token *t;
    int follow = 0;

    while (follow == 0)
    {
        t = next_token();
        if (t->kind == TOKEN_END_OF_FILE || found_follow(t, rule) == 1)
        {
            follow = 1;
            back_token();
        }
    }
}