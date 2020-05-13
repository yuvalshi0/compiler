#include "parser.h"
#include "token.h"

void parse_prog()
{
    print_grammer(PARSE_PROG, PARSE_GLOBAL_VARS);
    parse_global_vars();
    print_grammer(PARSE_PROG, PARSE_FUNC_PREDEFS);
    parse_func_predefs();
    print_grammer(PARSE_PROG, PARSE_FUNC_FULL_DEFS);
    parse_func_full_defs();
    print_grammer(PARSE_PROG, PARSE_END_OF_FILE);
}
void parse_global_vars()
{
    if (match_k_token(TOKEN_SEPARATION_BRACKET_OPEN, 3))
    {
        return;
    }
    else
    {
        print_grammer(PARSE_GLOBAL_VARS, PARSE_GLOBAL_VARS2);
        parse_global_vars2();
    }
}
void parse_global_vars2()
{
    if (match_token(TOKEN_TYPE) && (match_k_token(TOKEN_SEPARATION_BRACKET_OPEN, 3) == 0))
    {
        parse_var_dec();
        print_grammer(PARSE_GLOBAL_VARS2, PARSE_GLOBAL_VARS2);
        parse_global_vars2();
    }
}
void parse_var_dec()
{
    parse_type();
    if (match_token(TOKEN_VAR))
    {
        next_token();
        parse_var_dec2();
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
    }
}
void parse_type()
{
    if (match_token(TOKEN_TYPE))
    {
        next_token();
    }
    else
    {
        print_error(next_token(), TOKEN_TYPE);
    }
}
void parse_dim_sizes()
{
    if (match_token(TOKEN_INTEGER))
    {
        next_token();
        print_grammer(PARSE_DIM_SIZES, PARSE_DIM_SIZES2);
        parse_dim_sizes2();
    }
    else
    {
        print_error(next_token(), TOKEN_INTEGER);
    }
}
void parse_func_predefs()
{
    print_grammer(PARSE_FUNC_PREDEFS, PARSE_FUNC_PROTOTYPE);
    parse_func_prototype();
    if (match_token(TOKEN_SEPERATION_SEMICOLON))
    {
        next_token();
        print_grammer(PARSE_FUNC_PREDEFS, PARSE_FUNC_PREDEFS2);
        parse_func_predefs2();
    }
}
void parse_func_predefs2()
{
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        print_grammer(PARSE_FUNC_PREDEFS2, PARSE_FUNC_PROTOTYPE);
        parse_func_prototype();
        if (match_token(TOKEN_SEPERATION_SEMICOLON))
        {
            next_token();
            print_grammer(PARSE_FUNC_PREDEFS2, PARSE_FUNC_PREDEFS2);
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
    print_grammer(PARSE_FUNC_PROTOTYPE, PARSE_RETURNED_TYPE);
    parse_returned_type();
    if (match_token(TOKEN_VAR))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token();
            print_grammer(PARSE_FUNC_PROTOTYPE, PARSE_PARAMS);
            parse_params();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_OPEN);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
    }
}
void parse_func_full_defs()
{
    if (match_token(TOKEN_END_OF_FILE) == 0)
    {
        print_grammer(PARSE_FUNC_FULL_DEFS, PARSE_FUNC_WITH_BODY);
        parse_func_with_body();
        print_grammer(PARSE_FUNC_FULL_DEFS, PARSE_FUNC_FULL_DEFS2);
        parse_func_full_defs2();
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
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        next_token();
    }
    else
    {
        print_error(next_token(), TOKEN_TYPE);
    }
}

void parse_params()
{
    if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE) == 0)
    {
        parse_param_list();
    }
}
void parse_param_list()
{
    parse_param();
    print_grammer(PARSE_PARAM_LIST, PARSE_PARAM_LIST2);
    parse_param_list2();
}
void parse_param_list2()
{
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token(__LINE__);
        print_grammer(PARSE_PARAM_LIST2, PARSE_PARAM);
        parse_param();
        print_grammer(PARSE_PARAM_LIST2, PARSE_PARAM_LIST2);
        parse_param_list2();
    }
}
void parse_param()
{
    print_grammer(PARSE_PARAM, PARSE_TYPE);
    parse_type();
    if (match_token(TOKEN_VAR))
    {
        next_token();
        print_grammer(PARSE_PARAM, PARSE_PARAM2);
        parse_param2();
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
    }
}
void parse_comp_stmt()
{
    if (match_token(TOKEN_SEPARATION_CURLY_BRACKET_OPEN))
    {
        next_token(__LINE__);
        print_grammer(PARSE_COMP_STMT, PARSE_VAR_DEC_LIST);
        parse_var_dec_list();
        print_grammer(PARSE_COMP_STMT, PARSE_STMT_LIST);
        parse_stmt_list();
        if (match_token(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE))
        {
            next_token(__LINE__);
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_SEPARATION_CURLY_BRACKET_OPEN);
    }
}
void parse_var_dec_list()
{
    if (match_token(TOKEN_TYPE))
    {
        print_grammer(PARSE_VAR_DEC_LIST, PARSE_VAR_DEC_LIST2);
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
}

void parse_stmt_list()
{
    print_grammer(PARSE_STMT_LIST, PARSE_STMT);
    parse_stmt();
    print_grammer(PARSE_STMT_LIST, PARSE_STMT_LIST2);
    parse_stmt_list2();
}
void parse_stmt_list2()
{
    if (match_token(TOKEN_SEPERATION_SEMICOLON))
    {
        next_token();
        print_grammer(PARSE_STMT_LIST2, PARSE_STMT);
        parse_stmt();
        parse_stmt_list2();
    }
}
void parse_stmt()
{
    Token *t = next_token(__LINE__);
    switch (t->kind)
    {
    case TOKEN_VAR:
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            back_token(__LINE__);
            print_grammer(PARSE_STMT, PARSE_CALL);
            parse_call();
        }
        else
        {
            print_grammer(PARSE_STMT, PARSE_VAR);
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
    case TOKEN_SEPARATION_CURLY_BRACKET_OPEN:
        back_token();
        print_grammer(PARSE_STMT, PARSE_COMP_STMT);
        parse_comp_stmt();
        break;
    default:
        print_error(next_token(), TOKEN_VAR);
        break;
    }
}
void parse_if_stmt()
{
    if (match_token(TOKEN_IF))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token(__LINE__);
            print_grammer(PARSE_IF_STMT, PARSE_CONDITION);
            parse_condition();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token(__LINE__);
                print_grammer(PARSE_IF_STMT, PARSE_STMT);
                parse_stmt();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_OPEN);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_IF);
    }
}
void parse_call()
{
    if (match_token(TOKEN_VAR))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token(__LINE__);
            print_grammer(PARSE_CALL, PARSE_ARGS);
            parse_args();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token(__LINE__);
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_OPEN);
        }
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
    }
}
void parse_args()
{
    if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE) == 0)
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
    if (match_token(TOKEN_SEPARATION_COMMA))
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
    if (match_token(TOKEN_RETURN))
    {
        next_token();
        print_grammer(PARSE_RETURN_STMT, PARSE_RETURN_STMT2);
        parse_return_stmt2();
    }
    else
    {
        print_error(next_token(), TOKEN_RETURN);
    }
}
void parse_var()
{
    if (match_token(TOKEN_VAR))
    {
        next_token();
        print_grammer(PARSE_VAR, PARSE_VAR2);
        parse_var2();
    }
    else
    {
        print_error(next_token(), TOKEN_VAR);
    }
}
void parse_expr_list()
{
    print_grammer(PARSE_EXPR_LIST, PARSE_EXPR);
    parse_expr();
    print_grammer(PARSE_EXPR_LIST, PARSE_EXPR_LIST2);
    parse_expr_list2();
}
void parse_expr_list2()
{
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token();
        print_grammer(PARSE_EXPR_LIST2, PARSE_EXPR_LIST);
        parse_expr();
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
        print_grammer(PARSE_CONDITION, PARSE_EXPR);
        parse_expr();
    }
    else
    {
        print_error(next_token(), TOKEN_COMPARISON);
    }
}
void parse_expr()
{
    print_grammer(PARSE_EXPR, PARSE_TERM);
    parse_term();
    print_grammer(PARSE_EXPR, PARSE_EXPR2);
    parse_expr2();
}
void parse_expr2()
{
    if (match_token(TOKEN_ADD))
    {
        next_token();
        print_grammer(PARSE_EXPR2, PARSE_TERM2);
        parse_term();
        print_grammer(PARSE_EXPR2, PARSE_EXPR2);
        parse_expr2();
    }
}
void parse_term()
{
    print_grammer(PARSE_TERM, PARSE_FACTOR);
    parse_factor();
    print_grammer(PARSE_TERM, PARSE_TERM2);
    parse_term2();
}
void parse_term2()
{
    if (match_token(TOKEN_MUL))
    {
        next_token(__LINE__);
        print_grammer(PARSE_TERM2, PARSE_FACTOR);
        parse_factor();
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
        break;
    case TOKEN_VAR:
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
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
    case TOKEN_SEPARATION_BRACKET_OPEN:
        print_grammer(PARSE_FACTOR, PARSE_EXPR);
        parse_expr();
        if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
        {
            next_token(__LINE__);
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
        }
        break;
    default:
        //??????????????????????????? WHAT IS THE RRORR ???
        print_error(t, TOKEN_VAR);
        break;
    }
}

void parse_var_dec2()
{
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
            }
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
        }
    }
    else
    {
        //????????????????? ERROR????
        print_error(next_token(), TOKEN_SEPERATION_SEMICOLON);
    }
}

void parse_dim_sizes2()
{
    if (match_token(TOKEN_SEPARATION_COMMA))
    {
        next_token();
        if (match_token(TOKEN_INTEGER))
        {
            next_token();
            print_grammer(PARSE_DIM_SIZES, PARSE_DIM_SIZES2);
            parse_dim_sizes2();
        }
        else
        {
            print_error(next_token(), TOKEN_INTEGER);
        }
    }
}

void parse_func_full_defs2()
{
    if (match_token(TOKEN_TYPE) || match_token(TOKEN_FUNCTION))
    {
        print_grammer(PARSE_FUNC_FULL_DEFS2, PARSE_FUNC_WITH_BODY);
        parse_func_with_body();
        print_grammer(PARSE_FUNC_FULL_DEFS2, PARSE_FUNC_FULL_DEFS2);
        parse_func_full_defs2();
    }
}

void parse_param2()
{
    if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_OPEN))
    {
        next_token();
        print_grammer(PARSE_PARAM2, PARSE_DIM_SIZES);
        parse_dim_sizes();
        if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE))
        {
            next_token();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_SQUARE_BRACKET_OPEN);
        }
    }
}

void parse_return_stmt2()
{
    if (match_token(TOKEN_VAR))
    {
        next_token();
        if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
        {
            next_token();
            print_grammer(PARSE_RETURN_STMT2, PARSE_ARGS);
            parse_args();
            if (match_token(TOKEN_SEPARATION_BRACKET_CLOSE))
            {
                next_token();
                print_grammer(PARSE_RETURN_STMT2, PARSE_TERM2);
                parse_term2();
                print_grammer(PARSE_RETURN_STMT2, PARSE_EXPR2);
                parse_expr2();
            }
            else
            {
                print_error(next_token(), TOKEN_SEPARATION_BRACKET_CLOSE);
            }
        }
        else
        {
            print_grammer(PARSE_RETURN_STMT2, PARSE_TERM2);
            parse_term2();
            print_grammer(PARSE_RETURN_STMT2, PARSE_EXPR2);
            parse_expr2();
        }
    }
    else if (match_token(TOKEN_INTEGER) || match_token(TOKEN_FLOAT))
    {
        next_token();
        print_grammer(PARSE_RETURN_STMT2, PARSE_TERM2);
        parse_term2();
        print_grammer(PARSE_RETURN_STMT2, PARSE_EXPR2);
        parse_expr2();
    }
    else if (match_token(TOKEN_SEPARATION_BRACKET_OPEN))
    {
        next_token();
        print_grammer(PARSE_RETURN_STMT2, PARSE_EXPR);
        parse_expr();
        if (match_token(TOKEN_SEPARATION_CURLY_BRACKET_CLOSE))
        {
            next_token();
            print_grammer(PARSE_RETURN_STMT2, PARSE_TERM2);
            parse_term2();
            print_grammer(PARSE_RETURN_STMT2, PARSE_EXPR2);
            parse_expr2();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_CURLY_BRACKET_CLOSE);
        }
    }
}

void parse_var2()
{
    if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE))
    {
        next_token();
        print_grammer(PARSE_VAR2, PARSE_EXPR_LIST);
        parse_expr_list();
        if (match_token(TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE))
        {
            next_token();
        }
        else
        {
            print_error(next_token(), TOKEN_SEPARATION_SQUARE_BRACKET_CLOSE);
        }
    }
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

int found_follow(Token *t, eRULE rule)
{
    NodeFollow **n = init_follow_rules();
    NodeFollow *followGroup = n[rule];
    while (followGroup != NULL)
    {
        if (t->kind == followGroup->token)
        {
            back_token();
            return 1;
        }
        followGroup = followGroup->next;
    }

    return 0;
}
