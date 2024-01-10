#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_MAP_LEN 30

/*

# ============================================
# PROJECT: Interprinter
# ============================================
#
# Currently Avabile:
#   - Storing varible
#   - Printing variable
#
# To do:
#   - Function
#   - Add types
#   - Working operators

# Data types

Types        | Supported
--------------------------
0 = int      |     O
1 = char     |     O
2 = float    |     X
3 = double   |     X
4 = array    |     X
5 = function |     X
6 = action   |     X




# Operators

Types        | Supported
-------------------------
+ Add        |     O
- Substract  |     O
* Multiply   |     O
/ Divide     |     O
= Equal      |     O (Not with another operator)
< Than       |     X
> Than       |     X
() Priority  |     X


4 (type) (length) (values)



Data saving

(type) (values)

Synatx

var a = 2;

fun a(a, b) {

}

*/

typedef struct {
    char key[127];
    int* value;
} entry;

int map_len = 0;
entry map[MAX_MAP_LEN];

int findNextIndex() {
    int i;
    for (i = 0; i < MAX_MAP_LEN; i++) {
        if (strcmp(map[i].key, "") == 0) return i;
    }
    return -1;
}

int put(const char* key, const int* val) {
    if (map_len < MAX_MAP_LEN && key!=NULL&& val!=NULL) {
        int index = findNextIndex();
        if (index == -1) {
            return -1;
        }
        else {
            strcpy(map[index].key, key);
            map[index].value = val;
            map_len++;
            return 0;
        }

    }
    else return -1;
}

int* get(const char* key) {
    int i;
    for (i = 0; i < 20; i++) {
        if (strcmp(key, map[i].key) == 0) {
            return map[i].value;
        }
    }
    return NULL;
}

int map_remove(const char* key) {
    int i;
    for (i = 0; i < MAX_MAP_LEN; i++) {
        if (strcmp(key, map[i].key) == 0) {
            free(map[i].key);
            free(map[i].value);
        }
    }
    map_len--;
}

void printByType(int datatype, int data) {
    switch (datatype) {
    case 0:
        printf("%d", data);
        break;
    case 1:
        printf("%c", data);
        break;
    }
}

void print(const int* data) {
    if (data != NULL) {
        if (data[0] == 4) {
            int i;
            for (i = 0; i < data[2]; i++) {
                printByType(data[1], data[3 + i]);
            }
        }
        else printByType(data[0], data[1]);
    }
    else {
        printf("data == NULL");
    }
    printf("\n");
}

int hasVariable(const char key[127]) {
    int i;
    for (i = 0; i < 20; i++) {
        if (strcmp(key, map[i].key) == 0) {
            return 1;
        }
    }
    return 0;
}

int hasVariableWithTypeOf(const char key[127], const int type) {
    int i;
    for (i = 0; i < 20; i++) {
        if (strcmp(key, map[i].key) == 0 && map[i].value[0] == type) {
            return 1;
        }
    }
    return 0;
}

int* toVariable(const int type, const int* variable) {
    int* val = (int*) malloc(sizeof(variable) + sizeof(int));
    val[0] = type;
    int i;
    for (i = 0; i < sizeof(variable) / sizeof(int); i++) {
        val[1 + i] = variable[i];
    }
    return val;
}

int* toArrayVariable(const int type, const int* variable, int size) {
    int* val = (int*)malloc(size * sizeof(variable[0]) + sizeof(int) * 3);
    val[0] = 4;
    val[1] = type;
    int i;
    for (i = 0; i < size; i++) {
        val[3 + i] = variable[i];
    }
    val[2] = size;
    return val;
}

void saveVariable(const char* key, const int type, const int* variable) {
    if (4 > type) {
        int* v = toVariable(type, variable);
        put(key, v);
    }
}

void saveArrayVariable(const char* key, const int type, const int* variable, int size) {
    put(key, toArrayVariable(type, variable, size));
}

void callFunction(char name[127], int* argumnets[]) {

}

void callNativeFunction(char name[20], int*** arguments) {
    if (strcmp(name, "print") == 0) {
        print(arguments[0]);
    }
    else if (strcmp(name, "exit") == 0) {
        printf("Exiting the Interprinter.\n");
        exit(0);
    }
    else {
        printf("Error: unknown native function '%s'.\n", name);
    }
}

char* removeSpacing(char buf[], int* size) {
    int i, rs = *size;
    for (i = 0; i < *size; i++) {
        if (buf[*size - (i + 1)] == ' ' || buf[*size - (i + 1)] < 0 || buf[*size - (i + 1)] == NULL) {
            rs--;
        }
        else break;
    }

    char* cc = malloc((rs) * sizeof(char));
    for (i = 0; i < rs; i++) {
        cc[i] = buf[i];
    }
    cc[i] = '\0';
    *size = rs;
    return cc;
}

int calculate(char c[], int size, int* status) {
    int i, j = 0, result = 0, isFirst = 1;
    char buf[sizeof(c) / sizeof(c[0])];
    *status = 1;
    char lastsign = '+';
    for (i = 0; i < size + 1; i++) {
        if (i > size || isExpressionSign(c[i]) || c[i] == '\0') {
            buf[i] = '\0';
            if (isExpressionSign(c[i])) lastsign = c[i];
            int s = i;
            char* buff = removeSpacing(buf, &s);
            int num;
            if (s == 0) {
                continue;
            }
            else if (isNumber(buff, j)) {
                num = toInteger(buff, s);
            }
            else if (hasVariableWithTypeOf(buff, 0)) {
                num = get(buff)[1];
            }
            else {
                *status = 0;
                return 0;
            }

            if (isFirst) {
                result = num;
                isFirst = 0;
            }
            else {
                result = calcuateWithModifer(lastsign, result, num);
            }
            memset(buf, 0, j);
            j = 0;
        }
        else buf[j++] = c[i];
    }
    return result;
}

int isExpressionSign(char c) {
    return (39 < c && c < 44) || c == 45 || c == 47;
}

int isAsciiNumber(char c) {
    return 47 < c && c < 58;
}

int tenthpowerof(int n) {
    int i, result = 1;
    for (i = 0; i < n; i++) {
        result *= 10;
    }
    return result;
}

int toInteger(char c[], int size) {
    int i, result = 0;
    for (i = 0; i < size; i++) {
        if (c[i] != '\0')result += tenthpowerof(size - i - 1) * (c[i] - 48);
    }
    return result;
}


int isNumber(char c[], int size) {
    int j;
    for (j = 0; j < size && c[j] != '\0'; j++) {
        if (!isAsciiNumber(c[j])) return 0;
    }
    return 1;
}

int calcuateWithModifer(char c, int a, int b) {
    if (c == '+') {
        return a + b;
    }
    else if (c == '-') {
        return a - b;
    }
    else if (c == '*') {
        return a * b;
    }
    else if (c == '/') {
        return a / b;
    }
    else if (c == '^') {
        return a ^ b;
    }
    else {
        printf("Error: Unknown modifer %c\n", c);
    }
}

int** toArgs(char c[], int size) {
    if (size <= 0) {
        //printf("Error: Array size cannot be lower than 1\n");
        return NULL;
    }
    int** args = (int**)malloc(getCommaCount(c) * sizeof(int*));
    char buf[sizeof c / sizeof c[0]] = { '\0' };
    int i = 0, j = 0, ai = 0, status=0;
    char tmp;
    while (i <= size) {
        tmp = c[i++];
        if (tmp == ',' || tmp == '\0') {
            int result = calculate(buf, i, &status);
            if (status) {
                args[ai] = (int*)malloc(sizeof(int) * 2);
                args[ai][0] = 0;
                args[ai++][1] = result;
            }
            else {
                printf("Invalid argument '%d'\n", buf);
            }
            memset(buf, 0, sizeof buf);
            j = 0;
        }
        else buf[j++] = tmp;
    }
    return args;
}

int getCommaCount(char c[]) {
    int cnt = 1, i;
    for (i = 0; i < (sizeof c / sizeof c[0]); i++) {
        if (c[i] == ',') cnt++;
    }
    return cnt;
}

void interprint() {
    printf(">> ");
    char c;
    char buf[32];
    char lastbuf[32];
    //  Input Mode
    //  0 = Normal
    //  1 = variable name
    //  2 = value (expression)
    //  3 = function (arguments)
    int i = 0, mode = 0, chm = 0, j;
    while (c = getchar()) {
        mode = chm;
        if (c == ' ' || c == ';' || c == '=' || c == '(' || c == ')') {
            buf[i] = '\0';
            if (mode == 1) {
                put(buf, NULL);
                strcpy(lastbuf, buf);
                mode = 0;
                chm = 0;
            }

            if (mode == 0) {
                if (strcmp(buf, "var") == 0) {
                    chm = 1;
                }
                else if (c == '=') {
                    chm = 2;
                }
                else if (c == '(') {
                    chm = 3;
                    strcpy(lastbuf, buf);
                }
            }
            else if (mode == 2 && c == ';') {
                int status;
                int result = calculate(buf, i, &status);
                if (status) {
                    int v[1] = { result };
                    saveVariable(lastbuf, 0, v);
                    chm == 0;
                }
                else {
                    printf("ERROR");
                    return;
                }
            }
            else if (mode == 3 && c == ')') {
                callNativeFunction(lastbuf, toArgs(buf, i));
            }
            memset(buf, 0, sizeof buf);
            i = 0;
        }
        else if (c == '\n') {
            return;
        }
        else buf[i++] = c;
    }
}

int main() {
    printf("PROJECT: INTERPRINTER\nWritten by @Yellowstrawberrys (Github)\n");
    printf("=================================\n\n");
    while (1) {
        interprint();
    }
}
