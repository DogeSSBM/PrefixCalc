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
        panic("Can't divide %i by 0!", a);
    return a / b;
}

int srt(const int a)
{
    return sqrt(a < 0 ? -a : a);
}

int eval(Expr *expr)
{
    // typedef int(*OprFun)(const int, const int);
    // OprFun oprFun[OPR_OPRS] = {sum, sub, mul, qot};
    int(*oprFun[OPR_OPRS])(const int, const int) = {sum, sub, mul, qot};
    if(expr->type == EXP_NUM)
        return expr->num;
    assertExpr(expr->type == EXP_OPR);
    Expr *oprand = expr->ops;
    int result = eval(oprand);
    if(expr->opr == OPR_SRT)
        return srt(eval(oprand));
    while((oprand = oprand->next) != NULL)
        result = oprFun[expr->opr](result, eval(oprand));
    return result;
}

#endif /* end of include guard: EVAL_H */
