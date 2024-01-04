#ifndef EXPR_H
#define EXPR_H

typedef enum{OPR_SUM, OPR_SUB, OPR_MUL, OPR_DIV, OPR_OPRS}OprType;
char *OprTypeStr[OPR_OPRS] = {"OPR_SUM", "OPR_SUB", "OPR_MUL", "OPR_DIV"};
char *OprTypeSym[OPR_OPRS] = {"+", "-", "*", "/"};

typedef enum{EXP_OPR, EXP_NUM, EXP_EXPS}ExprType;
char *ExprTypeStr[EXP_EXPS] = {"EXP_OPR", "EXP_NUM"};
typedef struct Expr{
    ExprType type;
    Token *token;
    union{
        int num;
        struct{
            OprType opr;
            struct Expr *ops;
        };
    };
    struct Expr *next;
}Expr;

OprType tokenOprType(Token *token)
{
    if(token->token[0] == '+')
        return OPR_SUM;
    if(token->token[0] == '-')
        return OPR_SUB;
    if(token->token[0] == '*')
        return OPR_MUL;
    if(token->token[0] == '/')
        return OPR_DIV;
    panic("uhoh");
    return 0;
}

Expr* exprAppend(Expr *head, Expr *tail)
{
    if(!head)
        return tail;
    Expr *cur = head;
    while(cur->next)
        cur = cur->next;
    cur->next = tail;
    return head;
}

void exprFree(Expr *expr)
{
    while(expr){
        if(expr->type == EXP_OPR)
            exprFree(expr->ops);
        Expr *next = expr->next;
        free(expr);
        expr = next;
    }
}

Expr* exprParseSub(Token **tokens)
{
    Expr *head = NULL;
    while((*tokens)->type != TOK_RP && (*tokens)->type != TOK_END){
        Expr *expr = calloc(1, sizeof(Expr));
        switch((*tokens)->type){
            case TOK_NUM:;
                char *end = NULL;
                const ul nat = strtoul((*tokens)->token, &end, 10);
                assertExpr(nat < 8*sizeof(int));
                expr->token = *tokens;
                expr->type = EXP_NUM;
                expr->num = (int)nat;
                break;
            case TOK_LP:
                (*tokens) = (*tokens)->next;
                assertExpr((*tokens) != NULL);
                expr->type = EXP_OPR;
                expr->token = *tokens;
                expr->opr = tokenOprType(*tokens);
                (*tokens) = (*tokens)->next;
                expr->ops = exprParseSub(tokens);
                assertExpr((*tokens)->type == TOK_RP);
                break;
            default:
                panic("uhoh :(");
                break;
        }
        head = exprAppend(head, expr);
        *tokens = (*tokens)->next;
        assertExpr((*tokens) != NULL);
    }
    assertExpr((*tokens) != NULL);
    return head;
}

Expr* exprParse(Token *tokens)
{
    Token *head = tokens;
    return exprParseSub(&head);
}

void ind(const uint ind, const uint indSize)
{
    for(uint i = 0; i < ind * indSize; i++)
        printf(" ");
}

void exprPrintSub(Expr *expr, uint lvl)
{
    while(expr){
        ind(lvl, 4);
        printf("type: %s, ", ExprTypeStr[expr->type]);
        if(expr->type == EXP_OPR){
            printf("%s (\n", OprTypeSym[expr->opr]);
            exprPrintSub(expr->ops, lvl + 1);
            ind(lvl, 4);
            printf(")\n");
        }else{
            printf("%i\n", expr->num);
        }
        expr = expr->next;
    }
}

void exprPrint(Expr *expr)
{
    exprPrintSub(expr, 0);
}

#endif /* end of include guard: EXPR_H */
