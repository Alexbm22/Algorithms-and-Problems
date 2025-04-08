#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100
#define MAX_NUM_SIZE 32

typedef struct {
    char v[MAX_SIZE][MAX_NUM_SIZE];
    int LastElIndex;
} Stack;

typedef struct {
    char op;
    int priority;
} Operator;

Operator operators[] = {
    {'+', 1}, {'-', 1},
    {'*', 2}, {'/', 2},
    {'(', 0}, {')', 0} 
};

int GetOpPriority(char op) {
    int size = sizeof(operators) / sizeof(operators[0]);
    for (int i = 0; i < size; i++) {
        if (operators[i].op == op) {
            return operators[i].priority;
        }
    }

    return -1;
}

void initStack(Stack* stack) {
    stack->LastElIndex = -1;
}

int isFull(Stack* stack) {
    return stack->LastElIndex >= MAX_SIZE - 1;
}

int isEmpty(Stack* stack) {
    return stack->LastElIndex == -1;
}

void Push(Stack* stack, const char* val) {
    if (isFull(stack)) {
        fprintf(stderr, "Error occured pushing element: %s, stack is full! \n", val);
        exit(1);
    }

    strncpy(stack->v[++stack->LastElIndex], val, MAX_NUM_SIZE - 1);
    stack->v[stack->LastElIndex][MAX_NUM_SIZE - 1] = '\0';
}

void PushChar(Stack* stack, char val) {
    char temp[2] = {val, '\0'};
    Push(stack, temp);
}

char* Pop(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Error occured poping, stack is empty! \n");
        exit(1);
    }
    return stack->v[stack->LastElIndex--];
}

char* Peek(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->v[stack->LastElIndex];
}

int  calculateResult(int val1, int val2, char operator) {
    switch (operator){
    case '+': 
        return val1 + val2;
    case '*':
        return val1 * val2;
    case '-':
        return val1 - val2;
    case '/':
        if (val2 == 0) {
            fprintf(stderr, "Error: Division by zero\n");
            exit(1);
        }
        return val1 / val2;
    default:
        fprintf(stderr, "Error occured calculating the posfixed equation, not an  operator element\n");
        exit(1);
    }
}

Stack ConvertToPostfix(const char* expression) {
    Stack result;
    Stack operators;

    initStack(&result);
    initStack(&operators);

    for (int i = 0; i < strlen(expression); i++) {
        char value = expression[i];

        if (isspace(value)) continue;

        if (isdigit(value)) {
            char number[MAX_NUM_SIZE] = { 0 };
            int numIndex = 0;

            while (i < strlen(expression) && isdigit(expression[i])) {
                number[numIndex++] = expression[i++];
                if (numIndex >= MAX_NUM_SIZE - 1) break;
            }

            number[numIndex] = '\0';
            Push(&result, number);
            --i;
            continue;
        }

        if (value == '(') {
            PushChar(&operators, value);
            continue;
        }

        if (value == ')') {
            while (!isEmpty(&operators)) {
                char* poppedOp = Pop(&operators);
                if (poppedOp[0] == '(') {
                    break;
                } else {
                    Push(&result, poppedOp);
                }
            }
            continue;
        }

        int valPriority = GetOpPriority(value);
        if (valPriority < 0) {
            fprintf(stderr, "Error occured. Wrong expression!! \n");
            exit(1);
        }

        while (!isEmpty(&operators) &&
            Peek(&operators)[0] != '(' &&
            GetOpPriority(Peek(&operators)[0]) >= valPriority) {
            Push(&result, Pop(&operators));
        }

        PushChar(&operators, value);
    }

    //pop remaining operators
    while (!isEmpty(&operators)) {
        Push(&result, Pop(&operators));
    }

    return result;
}

int CalcPostfixedEquation(Stack* postfixedStack) {
    Stack operands;
    initStack(&operands);

    for (int i = 0; i <= postfixedStack->LastElIndex; i++) {

        const char* element = postfixedStack->v[i];

        if (isdigit(element[0])) {
            Push(&operands, element);
            continue;
        }
        
        char* op2 = Pop(&operands);
        char* op1 = Pop(&operands);

        int operationResult = calculateResult(atoi(op1), atoi(op2), element[0]);
        char result[MAX_NUM_SIZE];
        snprintf(result, MAX_NUM_SIZE, "%d", operationResult);

        Push(&operands, result);
    }

    if (operands.LastElIndex != 0) {
        fprintf(stderr, "Error occured calculating the posfixed equation\n");
        exit(1);
    }

    return atoi(operands.v[0]);
}

void PrintPostfixedEq(Stack* stack) {
    printf("Postfixed Equation: ");
    for (int i = 0; i <= stack->LastElIndex; i++) {
        printf("%s ", stack->v[i]);
    }

    printf("\n");
}

int main() {
    
    char expression[] = "33*(6+23)/5";
    
    printf("Postfixed polish equation form!: ");
    Stack postfixed = ConvertToPostfix(expression);
    PrintPostfixedEq(&postfixed);

    int equationResult = CalcPostfixedEquation(&postfixed);
    printf("\nExpression Result: %d", equationResult);

	return 0;
}