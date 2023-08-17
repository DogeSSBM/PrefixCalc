#ifndef TOKEN_H
#define TOKEN_H

typedef enum{TOK_ERR, TOK_SYM, TOK_NUM, TOK_LP, TOK_RP, TOK_END, TOK_TOKS}TokenType;
char *TokenTypeStr[TOK_TOKS] = {"TOK_ERR", "TOK_SYM", "TOK_NUM", "TOK_LP", "TOK_RP", "TOK_END"};
typedef struct Token{
    TokenType type;
    const char *token;
    st len;
    struct Token *next;
}Token;

void tokenPrint(Token *token)
{
    assertExpr(token != NULL);
    printf("token->type: %s\n", TokenTypeStr[token->type]);
    printf("token->len: %zu\n", token->len);
    printf("token->token: ");
    for(st i = 0; i < token->len; i++)
        printf("%c", token->token[i]);
    printf("\n\n");
}

void tokensPrint(Token *token)
{
    while(token){
        tokenPrint(token);
        token = token->next;
    }
}

Token* tokenFree(Token *tokens)
{
    if(!tokens)
        return NULL;
    Token *next = tokens->next;
    free(tokens);
    return next;
}

void tokensFree(Token *tokens)
{
    while(tokens)
        tokens = tokenFree(tokens);
}

bool issym(const char c)
{
    return isalpha(c) || c == '+' || c == '-' || c == '*' || c == '/';
}

Token* tokenParse(const char *source)
{
    Token *token = calloc(1, sizeof(Token));
    token->token = source;
    if(*source == '\0'){
        token->type = TOK_END;
        return token;
    }
    if(*source == '('){
        token->type = TOK_LP;
        token->len = 1;
        return token;
    }
    if(*source == ')'){
        token->type = TOK_RP;
        token->len = 1;
        return token;
    }
    if(issym(*source)){
        token->type = TOK_SYM;
        do{
            token->len++;
        }while(issym(*++source));
        return token;
    }
    if(isdigit(*source)){
        token->type = TOK_NUM;
        do{
            token->len++;
        }while(isdigit(*++source));
        return token;
    }
    return token;
}

Token* tokenAppend(Token *head, Token *tail)
{
    if(!head)
        return tail;
    Token *cur = head;
    while(cur->next)
        cur = cur->next;
    cur->next = tail;
    return head;
}

void tokensCheckParens(Token *token)
{
    int bal = 0;
    Token *parentLeft = NULL;
    while(token->type != TOK_END){
        if(token->type == TOK_LP){
            if(bal == 0)
                parentLeft = token;
            bal++;
        }else if(token->type == TOK_RP){
            if(bal < 1){
                printf("Invalid ')'.\n");
                printf("At: %s\n", token->token);
                exit(EXIT_FAILURE);
            }
            bal--;
        }
        token = token->next;
    }
    if(bal != 0){
        printf("Unclosed '('\n");
        printf("At: %s\n", parentLeft->token);
        exit(EXIT_FAILURE);
    }
}

Token* tokenize(const char *source)
{
    Token *tokens = NULL;
    Token *token = NULL;
    do{
        while(isspace(*source))
            source++;
        token = tokenParse(source);
        tokens = tokenAppend(tokens, token);
        assertExprMsg(token->type != TOK_ERR, source);
        source += token->len;
    }while(token->type != TOK_END);
    return tokens;
}

#endif /* end of include guard: TOKEN_H */
