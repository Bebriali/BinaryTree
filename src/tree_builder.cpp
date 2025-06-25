#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#include "color.h"
#include "debug_info.h"
#include "differentiator.h"
#include "tree_builder.h"

#define ASSERT_BUILD(arg1, arg2, arg3) assert(arg1); \
                                       assert(arg2); \
                                       assert(arg3);

void SyntaxError(size_t p, Tokens* command, const char* error_text)
{
    printf(RED("error in syntax\n\t"));

    printf("current ptr = %d\n\t", p);

    DumpToken(command);

    for (size_t i = 0; i < p; i++)
    {
        printf("%9s", "");
    }
    printf(RED("^\n"));
    for (size_t i = 0; i < p; i++)
    {
        printf("%9s", "");
    }
    printf(RED("|\n"));

    printf(RED("%s\n"), error_text);

    assert(0);
}

Node_t* GetOperator(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetOperator : p = %d\n", *p);)

    Node_t* Operator = GetConditional(tree, p, command);

    if (Operator == NULL)
    {
        ON_DEBUG(printf("no condition such as 'if' 'while'\n");)
        Operator = GetAssignment(tree, p, command);
    }

    while (command->tokens[*p]->type != OP && command->tokens[*p]->data.op != EOT)
    {
        Node_t* Operator2 = GetConditional(tree, p, command);
        if (Operator2 == NULL)
        {
            Operator2 = GetAssignment(tree, p, command);
        }

        Operator = _SEM(Operator, Operator2);
    }

    return Operator;
}

Node_t* GetConditional(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetConditional : p = %d\n", *p);)

    if ((command->tokens[*p]->type == OP && command->tokens[*p]->data.op != IF && command->tokens[*p]->data.op != WHILE) || command->tokens[*p]->type != OP)
    {
        return NULL;
    }
    size_t if_token = *p;
    (*p)++;

    if (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != L_PR_EXP)
    {
        SyntaxError(*p, command, "expected left primary expression ('(')");
    }
    (*p)++;

    Node_t* condition = GetLogicalExpr(tree, p, command);
    ON_DEBUG(printf("after GetLogicalExpr : p = %d\n", *p);)
    if (condition == NULL)
    {
        SyntaxError(*p, command, "expected condition");
    }

    if (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != R_PR_EXP)
    {
        SyntaxError(*p, command, "expected right primary expression (')')");
    }
    (*p)++;
    if (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != L_CURL)
    {
        SyntaxError(*p, command, "expected left curly brace ('{')");
    }
    (*p)++;

    Node_t* directive = GetOperator(tree, p, command);

    if (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != R_CURL)
    {
        SyntaxError(*p, command, "expected right curly brace ('}')");
    }
    (*p)++;

    command->tokens[if_token]->left  = condition;
    command->tokens[if_token]->right = directive;

    return command->tokens[if_token];
}


Node_t* GetAssignment(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetAssignment : p = %d\n", *p);)

    Node_t* val1 = GetVariable(tree, p, command);
    if (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != AST)
        SyntaxError(*p, command, "expected assignment ('=')");

    size_t assign_ptr = *p;
    (*p)++;

    Node_t* val2 = GetAddSub(tree, p, command);

    command->tokens[assign_ptr]->left = val1;
    command->tokens[assign_ptr]->right = val2;
    ON_DEBUG(printf("val2 = %p\n", val2);)

    if (val2 == command->tokens[assign_ptr])
    {
        printf(RED("ADDR_PTR_FAULT\n"));
        return NULL;
    }

    if (command->tokens[*p]->type != OP || (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != SEM))
    {
        SyntaxError(*p, command, "expected semicolon (';')");
    }
    (*p)++;

    return command->tokens[assign_ptr];
}

Node_t* GetLogicalExpr(Tree_t* tree, size_t* p, Tokens* command)
{
    Node_t* expr_one = GetAddSub(tree, p, command);
    ON_DEBUG(printf("after first GetAddSub : p = %d\n", *p);)

    if (command->tokens[*p]->type != OP)
    {
        SyntaxError(*p, command, "expected comparative operator / right primary expression");
    }

    if (command->tokens[*p]->data.op == R_PR_EXP)
    {
        return expr_one;
    }

    // if (command->tokens[*p]->data.op == LESS || command->tokens[*p]->data.op == MORE || command->tokens[*p]->data.op == EQ || command->tokens[*p]->data.op == UN_EQ)
    if (ComparativeOperation(command->tokens[*p]->data.op))
    {
        size_t comparative_ptr = *p;
        (*p)++;

        Node_t* expr_sec = GetAddSub(tree, p, command);
        ON_DEBUG(printf("after _ GetAddSub : p = %d\n", *p);)

        command->tokens[comparative_ptr]->right = expr_sec;
        command->tokens[comparative_ptr]->left  = expr_one;

        return command->tokens[comparative_ptr];
    }

    SyntaxError(*p, command, "");

    return NULL;
}

Node_t* GetG(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetG : p = %d\n", *p);)


    Node_t* val = GetOperator(tree, p, command);
    while (command->tokens[*p]->type == OP && command->tokens[*p]->data.op == SEM && command->tokens[*p + 1]->data.op != EOT)
    {
        if (command->tokens[*p]->data.op != SEM)
        {
            SyntaxError(*p, command, "expected semicolon (';')");
        }
        else
        {
            (*p)++;
            ON_DEBUG(printf("getting new assignment\n");)
            Node_t* val2 = GetOperator(tree, p, command);
            if (val2 == NULL)
            {
                return NULL;
            }
            val = _SEM(val, val2);
        }
    }
    (*p)++;

    ON_DEBUG(printf("all assignments have gotten\n");)

    if (command->tokens[*p]->data.op != EOT)
        SyntaxError(*p, command, "expected End Of Translation ('$')");
    (*p)++;

    ON_DEBUG(printf("val addr = %p\n", val);)

    return val;
}

Node_t* GetAddSub(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetAddSub : p = %d\n", *p);)


    Node_t* val = GetMulDiv(tree, p, command);
    ON_DEBUG(printf("after GetMulDiv: p = %d\n", *p);)

    while (command->tokens[*p]->type == OP && (command->tokens[*p]->data.op == ADD || command->tokens[*p]->data.op == SUB))
    {
        Operation op = command->tokens[*p]->data.op;
        (*p)++;

        Node_t* val_extra = GetMulDiv(tree, p, command);

        if (op == ADD) val = _ADD(val, val_extra);
        if (op == SUB) val = _SUB(val, val_extra);
    }

    return val;
}

Node_t* GetMulDiv(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetMulDiv : p = %d\n", *p);)


    Node_t* val = GetPow(tree, p, command);
    ON_DEBUG(printf("after GetPow : p = %d\n", *p););

    while (command->tokens[*p]->type == OP && (command->tokens[*p]->data.op == DIV || command->tokens[*p]->data.op == MUL))
    {
        Operation op = command->tokens[*p]->data.op;

        (*p)++;

        Node_t* val_extra = GetPow(tree, p, command);

        if (op == MUL) val = _MUL(val, val_extra);
        if (op == DIV) val = _DIV(val, val_extra);
    }

    return val;
}

Node_t* GetPrExp(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetPrExp : p = %d\n", *p);)


    if (command->tokens[*p]->type == OP && command->tokens[*p]->data.op == L_PR_EXP)
    {
        (*p)++;
        Node_t* val = GetAddSub(tree, p, command);
        ON_DEBUG(printf("\tp after GetAddSub = %d\n", *p);)
        if (command->tokens[*p]->type != OP || (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != R_PR_EXP))
            SyntaxError(*p, command, "expected operation (')')");
        (*p)++;

        return val;
    }
    else
    {
        if (command->tokens[*p]->type == NUM)
        {
            return GetNumber(tree, p, command);
        }
        else
        {
            return GetVariable(tree, p, command);
        }
    }
}

Node_t* GetOperation(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetOperation : p = %d\n", *p);)

    if (command->tokens[*p]->type == OP)
    {
        if (command->tokens[*p]->data.op == L_PR_EXP)
        {
            ON_DEBUG(printf("\tp in GetOperation = %d\n", *p);)
            return GetPrExp(tree, p, command);
        }
        else
        {
            ON_DEBUG(printf(GREEN("detected operation : %d = %s\n"), command->tokens[*p]->data.op, DecryptOperation(command->tokens[*p]->data.op));)

            (*p)++;
            size_t cur_op_ptr = *p - 1;
            command->tokens[cur_op_ptr]->left = GetPrExp(tree, p, command);
            return command->tokens[cur_op_ptr];
        }
    }
    else
    {
        return GetPrExp(tree, p, command);
    }
}

Node_t* GetPow(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetPow : p = %d\n", *p);)


    Node_t* base = GetOperation(tree, p, command);
    ON_DEBUG(printf("p after GetOperation = %d\n", *p);)

    if (command->tokens[*p]->type != OP || (command->tokens[*p]->type == OP && command->tokens[*p]->data.op != POW))
    {
        ON_DEBUG(printf("returning base\n");)
        return base;
    }
    size_t pow_ptr = *p;
    (*p)++;

    Node_t* degree = GetOperation(tree, p, command);
    command->tokens[pow_ptr]->left = base;
    command->tokens[pow_ptr]->right = degree;

    if (degree == NULL)
    {
        SyntaxError(*p, command, "pow operator has no right operand ('smth^nullptr')");
        return NULL;
    }
    else
    {
        return command->tokens[pow_ptr];
    }
}

Node_t* GetNumber(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetNumber : p = %d\n", *p);)

    if (command->tokens[*p]->type == NUM)
    {
        return command->tokens[(*p)++];
    }
    else
    {
        SyntaxError(*p, command, "token is not a number");
        return NULL;
    }
}

Node_t* GetVariable(Tree_t* tree, size_t* p, Tokens* command)
{
    ASSERT_BUILD(tree, p, command);

    ON_DEBUG(printf("GetVariable : p = %d\n", *p);)

    if (command->tokens[*p]->type == VAR)
    {
        (*p)++;
        return command->tokens[*p - 1];
    }
    else
    {
        SyntaxError(*p, command, "token is not a variable");
    }

    return 0;
}
