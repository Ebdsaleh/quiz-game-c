// question_model.c - by Nadir Saleh 04/03/2024
#include "question_model.h"
#include "quiz_brain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
struct Question create_question(const char *text, const char *answer){
    struct Question new_question;
    // Allocate memory for text and answer
    new_question.text = malloc(strlen(text) +1);
    new_question.answer = malloc(strlen(answer) + 1);
    // Check if memory allocation was successful
    if (new_question.text == NULL || new_question.answer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE); // Or handle the error appropriately
    }
    // copy string literals
    strcpy((char*)new_question.text, text);
    strcpy((char*)new_question.answer,answer);

    int available = find_available_index();
    // printf("index: %d is available.\n", available);
    question_list[available] = new_question;
    return new_question;
}

struct Question create_default_question(){
    struct Question default_question;

    // Allocate memory for text and answer
    default_question.text = malloc(strlen("default") + 1);
    default_question.answer = malloc(strlen("default answer") + 1);

    // Check if memory allocation was successful
    if (default_question.text == NULL || default_question.answer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE); // Or handle the error appropriately
    }

    // Copy string literals
    strcpy((char*)default_question.text, "default");
    strcpy((char*)default_question.answer, "default answer");
    return  default_question;

}

void clear_question(struct Question *question){
    free((char*)question->text);
    free((char*)question->answer);
    question->text = NULL;
    question->answer = NULL;

}

struct Question return_error_question(){
    struct Question error_question;
    error_question.text = "ERROR";
    error_question.answer = "ERROR";
    return error_question;
}


