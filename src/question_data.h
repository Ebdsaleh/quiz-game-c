// question_data.h - by Nadir Saleh 04/03/2024
#ifndef QUESTION_DATA_H
#define QUESTION_DATA_H
#include <stdbool.h>
#include "parser.h"
#define MAX_QUESTIONS 100
#define MAX_BUFFER_SIZE 10000000

typedef struct {
    char *category;
    char *type;
    char *difficulty;
    char *question;
    char *correct_answer;
    char *incorrect_answers[3];  // Assuming at most 3 incorrect answers
} QuestionData;

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

extern QuestionData questions[];
extern int num_questions;
// function prototypes
//void initalise_question_data();
char *read_and_get_question_data_file(const char *filename);

void parse_test_data(char *data);
void parse_question_file_data(const char *filename);
int find_total_questions(char* data);
void parse_question_data(char* data);
int search_for(const char *token, char *data, int position, const char* flag, bool verbose);
char **extract_questions(int amount, char* data);
QuestionData parse_question_block(const char *block); 
void print_parsing_report(int entry_points, int exit_points, int keys, int values, int arrays, int element_terminators, int data_objects);
const char *extract_value(const char *data);
#endif // QUESTION_DATA_H
