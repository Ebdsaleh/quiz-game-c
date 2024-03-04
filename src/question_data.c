// question_data.c - by Nadir Saleh 04/03/2024
#include "question_data.h"
#include "parser.h"
// #include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



QuestionData questions[MAX_QUESTIONS];
int num_questions = 0;
// functions.
void parse_test_data( char *data){
    Parser parser = initialise_default_parser();
    // Allocate memory to copy the data.
    char * data_copy = malloc(strlen(data) + 1);
    // check for memory errors.
    if (data_copy == NULL){
        fprintf(stderr, "Error during memory allocation for data_copy");
    }
    // Make a copy of the data.
    printf("Checking Data integrity...\n");
    strcpy(data_copy, data);
    if (check_data_integrity(data_copy, parser, true)){
        printf("Data structure is valid with selected parser.\n");
    } else {
        fprintf(stderr, "Error: Malformed detected.\n");
    }

    // Free the data copy when finshed.
    free(data_copy);    
}
//.............................................................................
char* read_and_get_question_data_file(const char* filename) {
    char *buffer = malloc(MAX_BUFFER_SIZE);
    buffer = read_file(filename, true, false);
    return buffer;
}

// extract questions from file buffer in memory.
char **extract_questions(int amount, char* data){
    // This function will store the question_data items in memory
    // and prepared for parsing to be turned into a collection of
    // QuestionData data types.
    char **extracted = (char**) malloc(amount * sizeof(char*));
    if (extracted == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    char *file_entry_token = " = {";
    char *object_entry_token =  "{";
    char *object_exit_token = "},";
    int q_data_start = 0;
    int q_data_end = 0;
    int q_data_length = 0;
    int extracted_questions = 0; 
    
    int object_search_start = search_for(file_entry_token, data, 0, "end", false);
    // loop through data and extract the questions.
    for (int i = 0; i < amount; i++)
    {
        q_data_start = search_for(object_entry_token, data, object_search_start, "start",false);
        q_data_end = search_for(object_exit_token, data, q_data_start , "end",false);
        q_data_length = q_data_end - q_data_start;
        char *data_str = (char*)malloc(q_data_length + 1);
        if (data_str == NULL){
            fprintf(stderr, "Error: Memory allocation failed for data_str.\n");
            exit(EXIT_FAILURE);
        }
        // copy the question string.
        strncpy(data_str, data + q_data_start, q_data_length);
        // null terminate the string.
        data_str[q_data_length] = '\0';
        //printf("Question data extracted: %s\n", data_str);
        // add the data_str to the extracted list.
        extracted[i] = strdup(data_str);
        object_search_start = q_data_end;


        if (q_data_start && q_data_end != -1){
            extracted_questions++;
        }else {
            printf("No more questions to extract.\n");
        }
        // free allocated memory for data_str
        free(data_str);
    }

    return extracted;
}
int search_for(const char *token, char *data, int position, const char *flag, bool verbose) {
    // Returns the start or end positions of the token found
    // in data depending on the flag that is passed.
    
    const char *result = strstr(data + position, token);
    if (result != NULL) {
        int start_at = result - data;
        int end_at = start_at + strlen(token);
        if (verbose){
        printf("Searching for \"%s\": starting at position: %d\n", token, position);
        printf("Found at: %d position.\n", start_at);
        printf("Ends at: %d position.\n", end_at);
        }
        if (strcmp(flag, "start") == 0){
            return start_at;
        }else if (strcmp(flag,"end") == 0) {
            return end_at;
        }
        else {
            return start_at;
        }
    } else {
        if(verbose){
            printf("'%s' not found.\n", token);
        }
        return -1;
    }
}
int find_total_questions(char *data){
    // identifies how many questions in there are and returns the amount.
    // find the entry point of the question_data file.
    char *file_entry_token = " = {";
    char *object_entry_token =  "{";
    char *object_exit_token = "},";
    int q_data_start = 0;
    int q_data_end = 0;
    bool verbosity = false;
    int q_data_length = 0;

    int questions_found = 0;
    int object_search_start = search_for(file_entry_token, data, 0, "end", false);
    if (object_search_start == -1){
        printf("Malformed file data: Cannot find file entry token ('var_name%s')", file_entry_token);
    }
    while (q_data_start != -1){
        q_data_start = search_for(object_entry_token, data, object_search_start, "start", false);
        if (q_data_start == -1){
            break;
        }
        q_data_end = search_for(object_exit_token, data, q_data_start, "end", false);
        if (q_data_end == -1){
            break;
        }
        questions_found++;
        q_data_length = q_data_end - q_data_start;
        if (verbosity){
            printf("Question length found: %d\n", q_data_length);
        }
        
        // update the start and end positions.
        object_search_start = q_data_end;
        
    }
    return questions_found; // placeholder value
}
// getting seg fault, comment out and try new approach.

void parse_question_data(char *data){
    // Now we have the number of questions to iterate over.
    // printf("*data received, length of characters received: %zu\n", strlen(data));
        //int num_questions = 0; // Initialize num_questions
    // 
    int total_questions_found_in_data = find_total_questions(data);
    char **extracted_data = extract_questions(total_questions_found_in_data, data);
    for (int i = 0; i < total_questions_found_in_data; i++){

        questions[i] = parse_question_block(extracted_data[i]);
        num_questions++;
        // printf("Question:\n");
        // printf("%s\n", questions[i].question);
    }
    
}
void parse_question_file_data(const char *filename) {
    // Step 1: Read and retrieve question data
    char *data = read_and_get_question_data_file(filename);
    // Loop through the data to extract key-value pairs
    // Prepare the question array for usage.
    memset(questions, 0, sizeof(questions));
    parse_question_data(data);
    // parse_question_data will be repsonsible for freeing the char* data.
    if (num_questions > 0){
        for (int i = 0; i < num_questions; i++){
            //printf("Question: %d %s\n", i, questions[i].question);
        } 
    } else {
        printf("QuestionData questions array is null.\nparse_question_block was unsuccessful!\n");
    }
    
    free(data);
}


QuestionData parse_question_block(const char *block) {
    QuestionData question;
    memset(&question, 0, sizeof(QuestionData));
    question.category = NULL;
    question.type = NULL;
    question.difficulty = NULL;
    question.question = NULL;
    question.correct_answer = NULL;

    // Initialize incorrect_answers array
    for (int i = 0; i < 3; i++) {
        question.incorrect_answers[i] = NULL; 
    }
    //printf("Question init OK, now on to parsing...\n");
    // printf("Check block content.\n");
    //printf("Block content: %s\n", block);

    // Allocate Memory to copy the block
    char *block_copy = (char *)malloc(strlen(block) + 1); // Allocate memory for the copy
    if (block_copy == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for block_copy.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(block_copy, block); // Copy the block using strcpy
    // printf("%s\n", block_copy);
    //printf("got the block copy OK!\n");
        // Find key-value pairs
    const char *token = strdup(block_copy);
    int pos = 0;
    bool all_pairs_processed = false;
    while (!all_pairs_processed){
        // Extract key
        if (strstr(token, "category")) {
            pos = search_for("category", block_copy, 0, "end", false);
            pos = search_for(":", block_copy, pos, "end", false);
            token = strdup(block_copy + pos);
            question.category = strdup(extract_value(token));
        } else if (strstr(token, "type")) {
            pos = search_for("type", block_copy, 0, "end", false);
            pos = search_for(":", block_copy, pos, "end", false);
            token = strdup(block_copy + pos);
            question.type = strdup(extract_value(token));
        } else if (strstr(token, "difficulty")) {
            pos = search_for("difficulty", block_copy, 0, "end", false);
            pos = search_for(":", block_copy, pos, "end", false);
            token = strdup(block_copy + pos);
            question.difficulty = strdup(extract_value(token));
        } else if (strstr(token, "question")) {
            pos = search_for("question", block_copy, 0, "end", false);
            pos = search_for(":", block_copy, pos, "end", false);
            token = strdup(block_copy + pos);
            question.question = strdup(extract_value(token));
        } else if (strstr(token, "correct_answer") && question.correct_answer == NULL) {
            pos = search_for("correct_answer", block_copy, 0, "end", false);
            pos = search_for(":", block_copy, pos, "end", false);
            token = strdup(block_copy + pos);
            question.correct_answer = strdup(extract_value(token));
        } else if (strstr(token, "incorrect_answers")) {
            pos = search_for("correct_answer", block_copy, 0, "end", false);
            pos = search_for("[", block_copy, pos, "end", false);
            token = strdup(block_copy + pos);
            question.incorrect_answers[0] = strdup(token);
        } else {
            all_pairs_processed = true;
        }
         
    }

    free(block_copy);
    return question;
}

//*****************************************************************
const char *extract_value(const char *data){
    char* value = malloc(strlen(data) + 1);
    if (value == NULL){
        fprintf(stderr,"Error: Memory allocation failed, for 'value' inside extract_value function.\n");
        exit(EXIT_FAILURE);
    }
    Parser parser = initialise_default_parser();
    value[0] = '\0';
    // copy the data
    const char* data_copy = data;
    data_copy = strstr(data_copy, parser.value_token_start);
    if (data_copy == NULL){
        fprintf(stderr, "Error: Start of value not found in 'data_copy' in extract_value function.\n");
    }
    // search between value token start and end
       // extract the value
    bool inside_value = false;
    while(!inside_value && *data_copy != parser.element_terminator){
        if (strncmp(data_copy, parser.value_token_start, strlen(parser.value_token_start)) == 0){
            data_copy++;
            inside_value = true;
            while (strncmp(data_copy, parser.value_token_end, strlen(parser.value_token_end)) != 0){
                strncat(value,data_copy, 1);
                data_copy++;
            }
            data_copy++; // skip over the value end token.
            inside_value = false;
        }
        else if (strncmp(data_copy, parser.concatenation_token, strlen(parser.concatenation_token)) == 0){
            data_copy++; 
            
        } else {
            data_copy++;
        }
    }
    // return the value
    const char *return_value = strdup(value);
    free(value);
    return  return_value;
}
