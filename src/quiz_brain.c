// quiz_brain.c - by Nadir Saleh 04/03/2024.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "quiz_brain.h"
#include "question_data.h"
#include "question_model.h"
#define MAX_MESSAGE_LENGTH 100
#define MAX_INPUT_LENGTH 255
#define MAX_ANSWER_LENGTH 6
#define MAX_FILE_SIZE 1000000
int score = 0;
int question_number = 0;
int question_count = 0;
// functions
struct Question question_list[MAX_QUESTION_LIMIT];
void initialise_question_list(){
    for (int i = 0; i< MAX_QUESTION_LIMIT; i++){
        struct Question new_question = create_default_question();
        question_list[i] = new_question;
        question_count++;
        
    }
    // printf("%d question count after initialisation.\n", question_count);
}

int find_available_index(){

    for (int i = 0; i < MAX_QUESTION_LIMIT; i++){
        if (strcmp(question_list[i].text, "default") == 0){
            return i;
        }
    }
    exit(EXIT_FAILURE);
}

void clear_question_list(){
    int i = 0;
    while (question_count > 0){
        if (question_list[i].text == NULL || question_list[i].answer == NULL){
            printf("NULL POINTER DETECTED! index: %d\n", i);
            return;
        }
        else{
            clear_question(&question_list[i]);
            i++;
            question_count--;
        }
    }
}
void str_to_lower(char * str){
    while(*str){
        *str = tolower((unsigned char)*str);
        str++;
    }
}
void str_to_title_case(char *str){
    bool capitalise_next = true;
    while (*str){
        if (capitalise_next){
            *str = toupper((unsigned char)*str);
            capitalise_next = false;
        }
        else{
            *str = tolower((unsigned char)*str);
        }
        if (*str == ' ' || *str == '-'){
            capitalise_next = true;
        }
        str++;        
    }
}
char *input(const char *prompt, ...){
    // handle the input buffer.
    char *buffer = malloc(MAX_ANSWER_LENGTH);
    if (buffer == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed.\n");
        exit(EXIT_FAILURE);
    }
    // handle args
    va_list args;
    va_start(args, prompt);
    vprintf(prompt, args);
    va_end(args);
    
    if (fgets(buffer, MAX_ANSWER_LENGTH, stdin) == NULL){
        fprintf(stderr, "Error: Unable to read input.\n");
        exit(EXIT_FAILURE);
    }
    // convert the input to title case.
    str_to_title_case(buffer);

    // Remove newline character if present.
    size_t length = strlen(buffer);
    if (length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')){
        buffer[length - 1] = '\0';
    } else {
        int c;
        // clear input buffer
        while((c = getchar()) != '\n' && c != EOF);
    }
    // end of handling the input buffer.
    return buffer;
}
void next_question(){
    struct Question q = question_list[question_number];
    question_number++;
    char *user_input = malloc(MAX_ANSWER_LENGTH);
    if (user_input == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(user_input, input("Q%d: %s, (True/False)?: ", question_number, q.text));

    if (strcmp(user_input, "") == 0){
        strcpy(user_input, "pass");
    }
    check_answer(user_input, q.answer);
    free(user_input);
}
bool still_has_questions(){
    return question_number < question_count;
}

void check_answer(const char *u_answer, const char *q_answer){
    // check for NULL pointers
    if (u_answer == NULL || q_answer == NULL){
        printf("Error: Invalid input.\n");
        return;
    }
    // create local variables to use in the comparison of the first characters
    // in the values, only.
    char u_first = tolower(*u_answer);
    char q_first = tolower(*q_answer);
    // compare user's answer with answer to the question
    // (only comparing the first characters)
    bool is_correct = u_first == q_first;
    if (is_correct){
        printf("You got it right!\n");
        score++;
    }
    else {
        if (!strcmp(u_answer, "pass")){
            printf("You've decided to pass on the question.\n");
        }
        else{
            printf("That's incorrect!\n");
        }
        printf("The answer was: %s.\n", q_answer);
        printf("You're current score is: %d/%d\n", score, question_number);
    }
}

void print_final_score(){
    float percentage_score = ((float)score / question_count) * 100;
    char *message = malloc(MAX_MESSAGE_LENGTH);
    if (message == NULL){
        fprintf(stderr, "Error: Memory Allocation Failed.\n");
        exit(EXIT_FAILURE);
    }
    if (percentage_score <= 50){
        // 50% or less
        strcpy(message, "Good effort, keep trying!");
    }
    else if (percentage_score < 70){
        // between 50% and 70%
        strcpy(message, "Almost there, keep practicing!");
    }
    else if (percentage_score < 90){
        // between 70& and 90%
        strcpy(message, "Great work, you're making progress!");
    }
    else {
        // above 90%
        strcpy(message, "You're Amazing! Keep up the good work!");
    }
    printf("You're final score is: %d/%d\n", score, question_count);
    printf("%s\n", message);
    free(message);
}

 void read_data_file(const char *filename){
    if (filename == NULL){
        printf("Error: No filename given.\n");
        return;
    }
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error: failed to open file %s. \n", filename);
        return;
    }
    // determine file size.
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    if(file_size > MAX_FILE_SIZE){
        printf("Error: file sized exceeds maximum allowed size.\n");
        fclose(file);
        return;
    }

    // allocate memory for file contents.
    char *file_contents = malloc(file_size + 1); // +1 for null terminator.
    if (file_contents == NULL){
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return;
    }
    // read the file contents.
    size_t bytes_read = fread(file_contents, 1, file_size, file);
    file_contents[bytes_read] = '\0'; // null terminated string.

    // print the file contents.
    printf("%s", file_contents);

    // clean up.
    free(file_contents);
    fclose(file);
 }
         
bool is_duplicate(QuestionData question){
bool duplicate = false;
for (int i = 0; i < MAX_QUESTIONS; i ++){
    if (question_list[i].text == NULL){
        return duplicate;
    }
    if (strcmp(question.question, question_list[i].text) == 0 && strcmp(question.correct_answer, question_list[i].answer) == 0){
        duplicate = true;
        break;
    }
}
return duplicate;
}
