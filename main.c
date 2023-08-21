#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

typedef struct{
    char arr[MAX_SIZE];
    int top;
} Stack;

void push(Stack* stack, char item){
    if(stack->top == MAX_SIZE - 1){
        printf("Stack Overflow\n");
        return;
    }
    stack->arr[++stack->top]=item;
}

char pop(Stack* stack){
    if(stack->top == -1){
        printf("Stack underflow");
        return '\0';
    }
    return stack->arr[stack->top--];
}

bool isOperator(char c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

int precedence(char operator){
    switch (operator){
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
        case '%':
            return 3;
        default:
            return 0;
    }
}

char* infixToPostfix(char* infix){
    Stack operatorStack;
    operatorStack.top = -1;
    
    char* postfix = (char*)malloc(strlen(infix) * sizeof(char) + 1);
    int j=0;

    for(int i=0; infix[i]; i++){
        if(infix[i]>='0' && infix[i]<='9' || infix[i]=='.' || (infix[i] == '-' || infix[i] == '+') && (i == 0 || infix[i-1] == '(' || isOperator(infix[i-1]))){
            char tempNumber[MAX_SIZE];
            int k=0;

            if(infix[i] == '-' && (i == 0 || infix[i-1] == '(' || isOperator(infix[i-1]))){
                tempNumber[k++]='-';
                i++;
            }

            while(infix[i]>='0' && infix[i]<='9' || infix[i]=='.')
                tempNumber[k++]=infix[i++];
            tempNumber[k]='\0';

            for(int l=0;tempNumber[l];l++)
                postfix[j++]=tempNumber[l];
            postfix[j++] = ' ';
            i--;
        }

        else if(infix[i] == '(')
            push(&operatorStack,infix[i]);
        else if(infix[i]== ')'){
            while(operatorStack.top!=-1 && operatorStack.arr[operatorStack.top]!='('){
                postfix[j++]=pop(&operatorStack);    
            }
            pop(&operatorStack);
        }
        else if(isOperator(infix[i])){
            while(operatorStack.top!=-1 && precedence(infix[i]) <= precedence(operatorStack.arr[operatorStack.top]))
                postfix[j++]=pop(&operatorStack);
            push(&operatorStack,infix[i]);
        }
    }

    while(operatorStack.top!=-1)
        postfix[j++]=pop(&operatorStack);
    
    postfix[j]='\0';
    return postfix;
}

//переходим к вычислению постфикса
typedef struct{
    float arr[MAX_SIZE];
    int top;
} FloatStack;

void FloatPush(FloatStack* stack, float item){
    if(stack->top == MAX_SIZE-1){
        printf("Stack overflow\n");
        exit(1);
    }
    stack->arr[++stack->top] = item;
}

float floatPop(FloatStack* stack){
    if(stack->top == -1){
        printf("Stack underflow\n");
        exit(1);
    }
    return stack->arr[stack->top--];
}

float evaluatePostfix(char* postfix){
    FloatStack stack;
    stack.top = -1;

    for(int i=0; postfix[i];i++){
        if (postfix[i] == '-' && (postfix[i+1]>='0' && postfix[i+1]<='9' || postfix[i+1] == '.') || postfix[i]>='0' && postfix[i]<='9' || postfix[i]== '.'){
            char tempNumber[MAX_SIZE];
            int k = 0;

            while(postfix[i] != ' ' && postfix[i] != '\0')
                tempNumber[k++] = postfix[i++];
            tempNumber[k]='\0';
            FloatPush(&stack, atof(tempNumber));
        }
        else if(isOperator(postfix[i])){
            float operand2 = floatPop(&stack);
            float operand1 = floatPop(&stack);
            switch(postfix[i]){
                case '+':
                    FloatPush(&stack, operand1 + operand2);
                    break;
                case '-':
                    FloatPush(&stack, operand1 - operand2);
                    break;
                case '*':
                    FloatPush(&stack, operand1 * operand2);
                    break;
                case '/':
                    if(operand2 == 0){
                        printf("Division by zero\n");
                        exit(1);
                    }
                    FloatPush(&stack, operand1/operand2);
                    break;
                case '^':
                    FloatPush(&stack,pow(operand1,operand2));
                    break;
                case '%':
                    if(operand2 != 2){
                        printf("Поддерживается только квадратный корень.\n");
                        exit(1);
                    }
                    FloatPush(&stack,sqrt(operand1));
                    break;
            }
        }

    }
    return floatPop(&stack);
}

int hasBalancedParentheses(char* str){
    int count = 0;
    for (int i = 0; str[i]; i++){
        if(str[i] == '(') count++;
        else if(str[i] == ')') count --;
    }
    if (count>0) return 1;
    else if (count<0) return -1;
    return 0;
}

void interactiveMode() {
    char infix[MAX_SIZE];

    printf("Интерактивный калькулятор.\n");
    printf("Поддерживается сложение(+), вычитание(-), умножение(*), деление(/), возведение в степень(^), извлечение квадратного корня(%%).\n");
    printf("Введите выражение или 'q' для выхода.\n");

    while (1){
        printf("> ");
        if (!fgets(infix, MAX_SIZE, stdin)) {
            printf("Ошибка чтения ввода.\n");
            continue;
        }    

        size_t len = strlen(infix);
        if(len>0 && infix[len-1]=='\n')
            infix[len-1] = '\0';

        if (strcmp(infix,"q") == 0) 
            break;   

        if(hasBalancedParentheses(infix) == -1) {
            printf("Отсутствует открывающая скобка\n"); 
            continue;
        }  
        else if(hasBalancedParentheses(infix) == 1) {
            printf("Отсутствует закрывающая скобка\n");
            continue;
        }

        char* postfix = infixToPostfix(infix);
        printf("Postfix: %s\n", postfix);
        float result = evaluatePostfix(postfix);
        printf("Result: %g\n",result);
        free(postfix);
    }
}

int main(void){
    interactiveMode();
    return 0;
}