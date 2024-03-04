// main.c
// Quiz Game Project - written by Nadir Saleh
// 23/02/2024 - 04/03/2024
#include "question_model.h"
#include "quiz_brain.h"
#include "question_data.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
    srand(time(NULL)); // Generate random seed based on current time.
    initialise_question_list();
    parse_question_file_data("question_data");
    // select 10 random questions for the quiz.
    // Implemention of duplicate selection prevention.
    for (int i = 0; i < MAX_QUESTION_LIMIT; i++){
        int r_num = rand() % 20;
        while(is_duplicate(questions[r_num])){
            r_num = rand() % 20;
        }
        create_question(questions[r_num].question, questions[r_num].correct_answer);
    }
    printf("Welcome to the Quiz Game.\n");
    while (still_has_questions()) {
        next_question();
    }
    print_final_score();
    clear_question_list();
    return 0;
}
