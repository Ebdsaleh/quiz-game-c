// parser.h by Nadir Saleh - 04/03/2024
#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct{
    char *key_token_start;
    char *key_token_end;
    char key_value_separator;
    char *value_token_start;
    char *value_token_end;
    char element_terminator;
    char entry_point;
    char *exit_point;
    char *concatenation_token;
    char array_token_start;
    char array_token_end;
    
}Parser;
typedef enum {
    KEY_START = 0,
    VALUE_START = 1,
    ARRAY_START = 2,
} TOKEN_MARKER;
// function prototypes
char *read_file(const char* filename, bool return_it, bool verbose);
Parser initialise_default_parser();
bool check_rules(const char *data_scanner, Parser parser, TOKEN_MARKER token_marker);
bool check_key_formatting(const char *chr_data, Parser parser);
bool check_value_formatting(const char *chr_data, Parser parser);
bool check_array_formatting(const char *chr_data, Parser parser);
bool check_data_integrity(char *data, Parser parser, bool verbose);
void print_parser_definition(Parser parser);
void print_parsing_report(int entry_points, int exit_points, int keys, int values, int arrays, int element_terminators, int data_objects);








#endif // PARSER_H

