#ifndef FILE_H
#define FILE_H

st fileLen(const char *filePath)
{
    assertExprMsg(filePath, "Cannot open NULL file path");
    File *f = fopen(filePath, "r");
    assertExprMsg(f != NULL, "Could not open file: \"%s\"", filePath);
    int c = ' ';
    st len = 0;
    while((c = fgetc(f)) != EOF)
        len++;
    fclose(f);
    return len;
}

char* fileReadText(const char *filePath)
{
    const st len = fileLen(filePath);
    char *buf = calloc(len+1, sizeof(char));
    assertExprMsg(filePath != NULL, "Cannot open NULL file path");
    File *f = fopen(filePath, "r");
    assertExprMsg(f != NULL, "Could not open file: \"%s\"", filePath);
    int c = ' ';
    st pos = 0;
    while((c = fgetc(f)) != EOF){
        buf[pos] = c;
        pos++;
    }
    fclose(f);
    assertExprMsg(c == EOF && pos == len, "Size mismatch");
    return buf;
}

#endif /* end of include guard: FILE_H */
