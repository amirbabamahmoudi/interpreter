#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000

char* converToPostfix(char* buffer);
int operatorPriorityMap(char operator);
int postfixCalculator(char* exp);
void errExit(char* e);

int main() {
    char buffer[MAX_LEN];
    printf("Enter your expression:\n");
    fgets(buffer, MAX_LEN, stdin);
    int result = postfixCalculator(converToPostfix(buffer));
    printf("%d\n", result);
    return 0;
}

int operatorPriorityMap(char token) {

    if (token == '^') {
        return 3;

    } else if (token == '*' || token == '/' || token == '%') {
        return 2;

    } else if (token == '+' || token == '-') {
        return 1;

    } else {
        errExit("unknown operator");
    }

}

int postfixCalculator(char* exp_queue) {
    int calc_stack[MAX_LEN];
    int stack_top = -1;
    int i = 0;
    while (exp_queue[i] != '\0' && i < MAX_LEN) {

        if (isdigit(exp_queue[i])) {
            int num = atoi(exp_queue+i);
            stack_top++;
            calc_stack[stack_top] = num;
            while (isdigit(exp_queue[i]))
                i++;
        
        } else if (exp_queue[i] == ' ') {
            i++;
        } else {
            int a = calc_stack[stack_top];
            stack_top--;
            int b;
            if (stack_top == -1) {
                if (exp_queue[i] == '-') {
                    b = 0;
                } else {
                    errExit("invalid exp");
                }
            } else {
                b = calc_stack[stack_top];
                stack_top--;
            }

            switch(exp_queue[i]) {
                case '*':
                    a = a*b;
                    stack_top++;
                    calc_stack[stack_top] = a;
                    break;
                case '/':
                    a = b/a;
                    stack_top++;
                    calc_stack[stack_top] = a;
                    break;
                case '^':
                    a = pow(b, a);
                    stack_top++;
                    calc_stack[stack_top] = a;
                    break;
                case '+':
                    a = a+b;
                    stack_top++;
                    calc_stack[stack_top] = a;
                    break;
                case '-':
                    a = b-a;
                    stack_top++;
                    calc_stack[stack_top] = a;
                    break;
                case '%':
                    a = (int)b % (int)a;
                    stack_top++;
                    calc_stack[stack_top] = a;
                    break;
                default:
                    errExit("unknown operator");
            }
            i++;
        }
    }
    return calc_stack[stack_top];
}

char* converToPostfix(char* exp) {
    
    char* output_queue = malloc(sizeof(char) * MAX_LEN);
    char operator_stack[MAX_LEN];
    int i = 0;
    int output_index = 0;
    int stack_top = -1;

    while (exp[i] != '\0') {

        if (i >= MAX_LEN)
            errExit("entered expression is too long");

        if (isblank(exp[i]) || exp[i] == '\n'){
            i++;

        } else if (isdigit(exp[i])) {
 
            while(isdigit(exp[i])) {
                output_queue[output_index] = exp[i];
                output_index++;
                i++;
            }
            output_queue[output_index] = ' ';
            output_index++;

        } else if (exp[i] == '('){
            stack_top++;
            operator_stack[stack_top] = exp[i];
            i++;

        } else if (exp[i] == ')') {
            if (stack_top == -1) {
                errExit("parantheses doesn't match");
            }
            char top = operator_stack[stack_top];
            stack_top--;
            while (top != '(') {
                if (stack_top == -1) {
                    errExit("parantheses doesn't match");
                }
                output_queue[output_index] = top;
                output_index ++;
                top = operator_stack[stack_top];
                stack_top--;
            }
            i++;

        } else {
            char top;
            while (stack_top != -1) {
                top = operator_stack[stack_top];
                if (top != '(' && operatorPriorityMap(top) >= operatorPriorityMap(exp[i])) {
                    output_queue[output_index] = top;
                    output_index++;
                    stack_top--;;
                } else 
                    break;
            }
            stack_top++;
            operator_stack[stack_top] = exp[i];
            i++;
        }
    }
    while (stack_top != -1) {
        char top = operator_stack[stack_top];
        stack_top--;
        if (top == '(' || top == ')')
            errExit("parantheses doesn't match");
        output_queue[output_index] = top;
        output_index ++;
    }
    output_queue[output_index] = '\0';

    return output_queue;
}

void errExit(char *e) {
    printf("Error: %s\n", e);
    exit(1);
}
