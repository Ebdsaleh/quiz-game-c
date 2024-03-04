// quiz_brain.h - by Nadir Saleh 04/03/2024
#ifndef QUIZ_BRAIN_H
#define QUIZ_BRAIN_H
#include <stdbit.h>
#include <stdio.h>
#include "question_model.h"
#include "question_data.h"
#define MAX_QUESTION_LIMIT 10
extern int question_count;
extern int question_number;
extern int score;
extern struct Question question_list[MAX_QUESTION_LIMIT];

// function prototypes
void initialise_question_list();
void str_to_lower(char *str);
void str_to_title_case(char *str);
char* input(const char *prompt, ...);
void next_question();
bool still_has_questions();
void check_answer(const char *u_answer, const char *q_answer);
void print_final_score();
void add_question(struct Question);
void read_data_file(const char* filename);
void clear_question_list();
int find_available_index();
bool is_duplicate(QuestionData question);
#endif // QUIZ_BRAIN_H
