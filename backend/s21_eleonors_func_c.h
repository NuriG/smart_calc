#ifndef S21_ELEONORS_FUNC_C
#define S21_ELEONORS_FUNC_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define STACK_EMPTY 1
#define STACK_NOT_EMPTY 0

typedef struct Stack {
    double digit;
    char operator_name[5];
    int priority;
    struct Stack *nearby_node;
} stack;

/*функция создает новый узел стека и заполняет его данными*/
void push(stack **pointer_to_top_element, double digit, const char *operator_name, int priority);
/*функция удаляет верхний узел в стеке*/
int pop(stack **pointer_to_top_element);
/*функция которая возвращает 1 из переменных верхнего узла*/
double peek_digit(stack *pointer_to_top_element);
/*функция которая возвращает 1 из переменных верхнего узла*/
const char* peek_operator(stack *pointer_to_top_element);
/*функция которая возвращает 1 из переменных верхнего узла*/
int peek_priority(stack *pointer_to_top_element);
/*функция для проверки содержимого стэка*/
void check_stack(stack **pointer_to_top_element);
/*функция для проверки содержимого стэка*/
void check_stack_operator(stack **pointer_to_top_element);


bool is_unary_minus_candidate(char ch);
int count_unary_minus(char *string);
void unary_minus(char *string);

// удали меня!
void check_stack_2(stack *top_element);
void print_output_stack_2(stack *top_element);

// функция алгоритма сортировочной станции
bool yard_algorithm(char *expression, stack **operator_stac, stack **output);

void help_yard_for_operator(stack** operator_stack, stack** output, int* i, char* string);
void help_yard_for_digit(stack** output, int* i, char* string, bool* flag);
void help_yard_for_isalpha(stack** operator_stack, int* i, char* string, bool* flag);
void help_yard_for_end_bracket(stack** operator_stack, stack** output, int* i, bool* flag);
void help_calculation_result_rpn_operator(stack **temp, stack **result_stack, double* result, double* digit_bottom, double* digit_top);
void help_calculation_result_rpn_functions(stack **temp, stack **result_stack, double* result, double* digit_bottom, double* digit_top);
void help_unary_minus_digits(char *string, char *new_string, int *i, int *j, int *new_index);
void help_unary_minus_brakes(char *string, char *new_string, int *i, int *j, int *new_index);
void help_unary_minus_functions(char *string, char *new_string, int *i, int *j, int *new_index);

// функция которая считывает с консоли выражение и записывает его в память
char *read_string_from_console();

bool is_operator(char ch);
int operator_priority(char ch);
bool is_digit(char ch);
bool check_dots(char* string);
bool check_string_func_name(char* string);
bool checking_empty_brackets(char *string);

/*функция для подсчета результат*/
void calculation_result_rpn(stack *top_element, double* result);

#endif 