#include "Includes.h"

char *getSrc(int argc, char **argv)
{
    assertExprMsg(argc == 2 || (argc == 3 && !strcmp(argv[1], "-f")), "Usage:\n\t%s -f <SourceFile>\n\t%s <Expr>", argv[0], argv[0]);
    char *source = NULL;
    if(argc == 3){
        source = fileReadText(argv[2]);
    }else{
        source = calloc(strlen(argv[1])+1, sizeof(char));
        memcpy(source, argv[1], strlen(argv[1]));
    }
    return source;
}

int main(int argc, char **argv)
{
    char *source = getSrc(argc, argv);
    printf("source: %s\n", source);

    Token *tokens = tokenize(source);
    tokensCheckParens(tokens);
    Expr *expr = exprParse(tokens);
    tokensPrint(tokens);
    free(source);
    tokensFree(tokens);

    exprPrint(expr);
    int result = eval(expr);
    exprFree(expr);
    printf("result: %i\n", result);

    return 0;
}
