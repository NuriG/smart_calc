#include "s21_eleonors_func_c.h"

// ((-(1+2)*(3/(-4)))+(-5))^(-6)
// 3*-cos(-sin(((-(1+2)*(3/(-4)))+(-5))^(-6)))
// 5*-sin(3)

int main() {
    char* user_input_expression_2 = read_string_from_console();

    // это делаем чтобы удобно потом делать операции над статическим массивом в котором есть запас места для преобразования
    char user_input_expression[500];
    strcpy(user_input_expression, user_input_expression_2);
    free(user_input_expression_2);

    // создаем два стека для алгоритма сортировочной станции
    stack *operator_stack = NULL;
    stack *output = NULL;

    
    if (!checking_empty_brackets(user_input_expression)) {
        printf("\nнайден () или (-) в строке поэтому выход\n");
        return 1;
    }


    int count = count_unary_minus(user_input_expression); // функция считает кол-во унарных минусов
    for (int i = 0; i < count; i++)
        unary_minus(user_input_expression);

    printf("\nстрока после функции unary_minus = %s \n", user_input_expression); // печатаем результат замены унарных минусов в консоль
    //

    // вызываем теперь сортировочную станцию
    yard_algorithm(user_input_expression, &operator_stack, &output);

    print_output_stack_2(output); // печатаем в консоль результат сортировочной станции

    double result;
    calculation_result_rpn(output, &result);
    printf("Результат вычислений = %.12f\n\n", result);

    printf("i definitely can!");
    return 0;
}

void push(stack **pointer_to_top_element, double digit, const char *operator, int priority) {
    // сначала создаем новый узел посредством выделения памяти
    stack *new_node = (stack*)malloc(sizeof(stack));
    // проверка на выделение памяти
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // заполняем данными новый узел
    new_node->digit = digit;
    strcpy(new_node->operator_name, operator);
    new_node->priority = priority;
    // присваиваем узлу адрес бывшего верхнего узла
    new_node->nearby_node = *pointer_to_top_element;
    // а после этого в укзатель на верхний элемент помещаем адрес этого узла
    *pointer_to_top_element = new_node;
}

int pop(stack **pointer_to_top_element) {
    int flag = STACK_NOT_EMPTY;
    if (*pointer_to_top_element == NULL) {
        flag = STACK_EMPTY;
        fprintf(stderr, "Stack is empty, you can't pop\n");
        exit(EXIT_FAILURE);
    } else {
        stack *temp = *pointer_to_top_element;
        *pointer_to_top_element = (*pointer_to_top_element)->nearby_node;
        free(temp);
    }
    return flag;
}

double peek_digit(stack *pointer_to_top_element) {
    if (pointer_to_top_element == NULL) {
        fprintf(stderr, "Stack is empty, you can't peek_digit\n");
        exit(EXIT_FAILURE);
    }
    return pointer_to_top_element->digit;
}

const char* peek_operator(stack *pointer_to_top_element) {
    if (pointer_to_top_element == NULL) {
        fprintf(stderr, "Stack is empty, you can't peek_operator\n");
        exit(EXIT_FAILURE);
    }
    return pointer_to_top_element->operator_name;
}

void check_stack_operator(stack **pointer_to_top_element) {
    printf("\n------ вверх стека ------\n\n");
    for (int i = 0; i < 100 && *pointer_to_top_element != NULL; i++) {
        printf("stack[%d].operator = %s\n", i, peek_operator(*pointer_to_top_element));
        pop(pointer_to_top_element);
    }
    printf("\n------  низ стека  ------\n\n");
}

int peek_priority(stack *pointer_to_top_element) {
    if (pointer_to_top_element == NULL) {
        fprintf(stderr, "Stack is empty, you can't peek_priority\n");
        exit(EXIT_FAILURE);
    }
    return pointer_to_top_element->priority;
}

void check_stack(stack **pointer_to_top_element) {
    printf("\n------ вверх стека ------\n\n");
    for (int i = 0; i < 100 && *pointer_to_top_element != NULL; i++) {
        printf("stack[%d].digit = %f\n", i, peek_digit(*pointer_to_top_element));
        pop(pointer_to_top_element);
    }
    printf("\n------  низ стека  ------\n\n");
}

bool yard_algorithm(char *string, stack **operator_stack, stack **output) {
    bool flag = true;
    int i = 0;
     while (string[i] != '\0') {
         if (is_operator(string[i]) && flag != false) {
             help_yard_for_operator(operator_stack, output, &i, string);
         } else if (is_digit(string[i])) {
             help_yard_for_digit(output, &i, string, &flag);
         } else if (isalpha(string[i])) {
             help_yard_for_isalpha(operator_stack, &i, string, &flag);
         } else if (string[i] == '(') {
            push(operator_stack, 0, "(", 5);
            i++;
         } else if (string[i] == ')') {
            help_yard_for_end_bracket(operator_stack, output, &i, &flag);
         } else if (string[i] == '.'){
             int temp_i = i + 1;
             if (is_digit(string[temp_i])) {
                 help_yard_for_digit(output, &i, string, &flag);
             } else {
                 flag = false;
                 i++;
             }
         } else {
             i++;
         }
     }
     if (string[i] == '\0' && flag != false) {
         while (*operator_stack != NULL) {
             if (strcmp(peek_operator(*operator_stack), "(") == 0) {
                 flag = false;
             }
             push(output, 0, peek_operator(*operator_stack), peek_priority(*operator_stack));
             pop(operator_stack);
         }
     }
     return flag;
}

void help_yard_for_operator(stack** operator_stack, stack** output, int* i, char* string) {
    char operator[2] = { string[*i], '\0' };
    int priority_o1;
    priority_o1 = operator_priority(string[*i]);
    while (*operator_stack != NULL) {
        const char *top_operator = peek_operator(*operator_stack);
        int priority_o2 = peek_priority(*operator_stack);
        if (is_operator(*top_operator) && priority_o2 >= priority_o1) {
            push(output, 0, top_operator, priority_o2);
            pop(operator_stack);
        } else {
            break;
        }
    }
    push(operator_stack, 0, operator, priority_o1);
    (*i)++;
}

void help_yard_for_digit(stack** output, int* i, char* string, bool* flag) {
    char number[20] = "";
             int length = strlen(&string[*i]);
             for (int j = 0; j < length; j++) {
                 if(is_digit(string[*i]) || string[*i] == '.') {
                     number[j] = string[*i];
                     (*i)++;
                 } else {
                     number[j] = '\0';
                     j = length;
                 }
             }
             if (!check_dots(number)) {
                 *flag = false;
                 printf("\nfalse1\n");
             }
             double value = atof(number);
             push(output, value, "", 0);
}

void help_yard_for_isalpha(stack** operator_stack, int* i, char* string, bool* flag) {
    char word[20] = "";
             int length = strlen(&string[*i]);
             for (int j = 0; j < length; j++) {
                 if(isalpha(string[*i])) {
                     word[j] = string[*i];
                     (*i)++;
                 } else {
                     word[j] = '\0';
                     j = length;
                 }
             }
             if (check_string_func_name(word)) {
                 push(operator_stack, 0, word, 4);
             } else {
                 *flag = false;
                 printf("\nfalse2\n");
             }
}

void help_yard_for_end_bracket(stack** operator_stack, stack** output, int* i, bool* flag) {
    while (strcmp(peek_operator(*operator_stack), "(") != 0 && *operator_stack != NULL) {
              push(output, 0, peek_operator(*operator_stack), peek_priority(*operator_stack));
              pop(operator_stack);
              if (*operator_stack == NULL) {
                 *flag = false;
                 printf("\nfalse3\n");
              }
            }
            if (*operator_stack != NULL) {
                if (strcmp(peek_operator(*operator_stack), "(") == 0) pop(operator_stack);
            }
            if (*operator_stack != NULL) {
            if (check_string_func_name((*operator_stack)->operator_name)) {
                 push(output, 0, peek_operator(*operator_stack), peek_priority(*operator_stack));
                 pop(operator_stack);
            }
            }
            (*i)++;
}

// удали меня
void check_stack_2(stack *top_element) {
    printf("------ Stack contents: ------\n\n");
    while (top_element != NULL) {
        printf("Digit: %.6f, Operator: %s, Priority: %d\n",
               top_element->digit, top_element->operator_name, top_element->priority);
        top_element = top_element->nearby_node;
        printf("\n");
    }
    printf("------  End of stack  ------\n\n");
}

// удали меня
void print_output_stack_2(stack *top_element) {
    stack *temp = NULL;
    while (top_element != NULL) {
        push(&temp, top_element->digit, top_element->operator_name, top_element->priority);
        top_element = top_element->nearby_node;
    }

    printf("\nReverse Polish Notation (RPN):\n\n");
    while (temp != NULL) {
        if (temp->operator_name[0] == '\0') {
            printf("%.2f ", temp->digit);
        } else {
            printf("%s ", temp->operator_name);
        }
        pop(&temp);
    }
    printf("\n\n");
}

/*ищет в строке название функции*/
bool check_string_func_name(char* string) {
    bool flag = true;
    if (strlen(string) > 4) {
        flag = false;
    } else if (strcmp(string, "cos") == 0 || strcmp(string, "sin") == 0 || 
               strcmp(string, "tan") == 0 || strcmp(string, "acos") == 0 || 
               strcmp(string, "asin") == 0 || strcmp(string, "atan") == 0 || 
               strcmp(string, "sqrt") == 0 || strcmp(string, "ln") == 0 || 
               strcmp(string, "log") == 0) {
         flag = true;
    } else {
        flag = false;
    }
    return flag;
}

/*проверяет строку числа на содержание не более 1 запятой*/
bool check_dots(char* string) {
    int length = strlen(string);
    int dot_count = 0;
    for(int i = 0; i < length; i++) {
        if (string[i] == '.') {
            dot_count++;
        }
    }
    return (dot_count <= 1);
}

bool is_operator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%');
}

int operator_priority(char ch) {
    int priority;
    if (ch == '+' || ch == '-') priority = 1;
    if (ch == '*' || ch == '/' || ch == '%') priority = 2;
    if (ch == '^') priority = 3;
    return priority;
}

bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

char *read_string_from_console() {
    char *input = NULL;
    size_t len = 0;
    printf("Enter a expression: ");
    ssize_t read = getline(&input, &len, stdin);
    if (read == -1) {
        perror("getline");
        exit(EXIT_FAILURE);
    }
    // Удалить символ новой строки (\n) из конца строки
    if (input[read - 1] == '\n') {
        input[read - 1] = '\0';
    }
    return input;
} 

bool is_unary_minus_candidate(char ch) {
    return ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch == '(';
}

int count_unary_minus(char *string) {
    int count = 0, i = 0;
    
    while (string[i] != '\0') {
        if (string[i] == '-' && (i == 0 || is_unary_minus_candidate(string[i - 1]))) {
            count++;
        }
        i++;
    }
    printf("\ncount unary minus = %d\n", count);
    return count;
}

/*функция заменяет унарные минусы в строке*/
void unary_minus(char *string) {
    int i = 0;
    char new_string[400];
    int new_index = 0;
    while (string[i] != '\0') {
        if (string[i] == '-' && (i == 0 || is_unary_minus_candidate(string[i - 1]))) {
            strcpy(&new_string[new_index], "(0-");
            new_index += 3;
            int j = i + 1;
            if ((string[j] >= '0' && string[j] <= '9') || string[j] == '.') {
                help_unary_minus_digits(string, new_string, &i, &j, &new_index);
            } else if (string[j] == '(') {
                help_unary_minus_brakes(string, new_string, &i, &j, &new_index);
            } else if (isalpha(string[j])) {
                help_unary_minus_functions(string, new_string, &i, &j, &new_index);
            }
        } else {
            new_string[new_index] = string[i];
            new_index++;
            i++;
        }
    }
    new_string[new_index] = '\0';
    strcpy(string, new_string);
}

void help_unary_minus_digits(char *string, char *new_string, int *i, int *j, int *new_index) {
    while ((string[*j] >= '0' && string[*j] <= '9') || string[*j] == '.') {
        new_string[*new_index] = string[*j];
        (*new_index)++;
        (*j)++;
    }
    new_string[*new_index] = ')';
    (*new_index)++;
    *i = *j;
}

void help_unary_minus_brakes(char *string, char *new_string, int *i, int *j, int *new_index) {
    int count_open_brakes = 1;
    while (count_open_brakes != 0) {
        new_string[*new_index] = string[*j];
        (*new_index)++;
        (*j)++;
        if (string[*j] == '(') {
            count_open_brakes++;
        }
        if (string[*j] == ')') {
            count_open_brakes--;
        }
    }
    new_string[*new_index] = string[*j];
    (*new_index)++;
    (*j)++;
    new_string[*new_index] = ')';
    (*new_index)++;
    *i = *j;
}

void help_unary_minus_functions(char *string, char *new_string, int *i, int *j, int *new_index) {
    while (isalpha(string[*j])) {
        new_string[*new_index] = string[*j];
        (*new_index)++;
        (*j)++;
    }
    int count_open_brakes = 1;
    while (count_open_brakes != 0) {
        new_string[*new_index] = string[*j];
        (*new_index)++;
        (*j)++;
        if (string[*j] == '(') count_open_brakes++;
        if (string[*j] == ')') count_open_brakes--;
    }
    new_string[*new_index] = string[*j];
    (*new_index)++;
    (*j)++;
    new_string[*new_index] = ')';
    (*new_index)++;
    *i = *j;
}

void calculation_result_rpn(stack *top_element, double* result) {
    *result = 0.0;
    double digit_top = 0, digit_bottom = 0;
    stack *temp = NULL;
    stack *result_stack = NULL;
    while (top_element != NULL) {
        push(&temp, top_element->digit, top_element->operator_name, top_element->priority);
        top_element = top_element->nearby_node;
    }
    while (temp != NULL) {
        if (temp->priority == 0) {
            digit_bottom = digit_top;
            digit_top = temp->digit;
            push(&result_stack, digit_top, "", 0);
        } else {
            if (temp->priority > 0 && temp->priority < 4) {
                help_calculation_result_rpn_operator(&temp, &result_stack, result, &digit_bottom, &digit_top);
            } else if (temp->priority == 4) {
                help_calculation_result_rpn_functions(&temp, &result_stack, result, &digit_bottom, &digit_top);
            }
        }
        pop(&temp);
    }   
}

// help_calculation_result_rpn_operator(temp, result_stack, result, &digit_bottom, &digit_top);
/*Функция производит вычесление когда встречает в стеке ОПН оператор*/
void help_calculation_result_rpn_operator(stack **temp, stack **result_stack, double* result, double* digit_bottom, double* digit_top) {
    if (strcmp((*temp)->operator_name, "-") == 0) *result = *digit_bottom - *digit_top;
    if (strcmp((*temp)->operator_name, "+") == 0) *result = *digit_bottom + *digit_top;
    if (strcmp((*temp)->operator_name, "*") == 0) *result = *digit_bottom * *digit_top;
    if (strcmp((*temp)->operator_name, "/") == 0) *result = *digit_bottom / *digit_top;
    if (strcmp((*temp)->operator_name, "^") == 0) *result = pow(*digit_bottom, *digit_top);
    if (strcmp((*temp)->operator_name, "%") == 0) *result = fmod(*digit_bottom, *digit_top); // жалуется на спец. символ
    if (*result_stack != NULL) {
        if (*result_stack != NULL) pop(result_stack);
        if (*result_stack != NULL) pop(result_stack);
        if (*result_stack != NULL) *digit_bottom = peek_digit(*result_stack);
    }
    push(result_stack, *result, "", 0);
    *digit_top = *result;
}

// help_calculation_result_rpn_functions(&temp, &result_stack, result, &digit_bottom, &digit_top);
/*Функция производит вычесление когда встречает в стеке ОПН функцию*/
void help_calculation_result_rpn_functions(stack **temp, stack **result_stack, double* result, double* digit_bottom, double* digit_top) {
    if (strcmp((*temp)->operator_name, "sin") == 0) *result = sin(*digit_top);
    if (strcmp((*temp)->operator_name, "asin") == 0) *result = asin(*digit_top);
    if (strcmp((*temp)->operator_name, "cos") == 0) *result = cos(*digit_top);
    if (strcmp((*temp)->operator_name, "acos") == 0) *result = acos(*digit_top);
    if (strcmp((*temp)->operator_name, "tan") == 0) *result = tan(*digit_top);
    if (strcmp((*temp)->operator_name, "atan") == 0) *result = atan(*digit_top);
    if (strcmp((*temp)->operator_name, "sqrt") == 0) *result = sqrt(*digit_top);
    if (strcmp((*temp)->operator_name, "ln") == 0) *result = log(*digit_top);
    if (strcmp((*temp)->operator_name, "log") == 0) *result = log10(*digit_top);
    if (result_stack != NULL) {
        if (*result_stack != NULL) pop(result_stack);
        if (*result_stack != NULL) *digit_bottom = peek_digit(*result_stack);
    }
    push(result_stack, *result, "", 0);
    *digit_top = *result;
}

/*функция вернет ошибку если найдет () или (-) в строке*/
bool checking_empty_brackets(char *string) {
    bool flag = true;
    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == '(') {
            if (string[i+1] == ')') {
                flag = false;
            } else if (string[i+1] == '-') {
                if (string[i+2] == ')') flag = false;
            }
        }
        if (!flag) break;
        i++;
    }
    return flag;
}
