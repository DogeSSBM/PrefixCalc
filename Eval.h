#ifndef EVAL_H
#define EVAL_H

int sum(const int a, const int b)
{
    return a + b;
}

int sub(const int a, const int b)
{
    return a - b;
}

int mul(const int a, const int b)
{
    return a * b;
}

int qot(const int a, const int b)
{
    if(b == 0)
        return 0;
    return a / b;
}

typedef int(*OprFun)(int, int);
OprFun oprFun[OPR_OPRS] = {sum, sub, mul, qot};

int eval(Expr *expr)
{
    if(expr->type == EXP_NUM)
        return expr->num;
    assertExpr(expr->type == EXP_OPR);
    Expr *oprand = expr->ops;
    int result = eval(oprand);
    while((oprand = oprand->next) != NULL)
        result = oprFun[expr->opr](result, eval(oprand));
    return result;
}

#endif /* end of include guard: EVAL_H */
