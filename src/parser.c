// parser.c - by Nadir Saleh 2024
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Parser initialise_default_parser(){
    Parser parser;
    parser.key_token_start = "\"";        // Initialize key_token_start member
    parser.key_token_end = "\"";       // Initialize key_token_end member
    parser.key_value_separator = ':';        // Initialize key_value_seperator member
    parser.value_token_start = "\"";     // Initialize value_token_start member
    parser.value_token_end = "\"";     // Initialize value_token_end member
    parser.element_terminator = ',';           // Initialize element_terminator member
    parser.entry_point = '{';          // Initialize entry_point member
    parser.exit_point = "},";        // Initialize exit_point member with a comma at the end
    parser.concatenation_token = "+"; // Initalize concatenation token member
    parser.array_token_start = '['; // Initalize array_token_start member
    parser.array_token_end = ']'; // Initalize array_token_end member
    return parser;
}

void print_parser_definition(Parser parser){
    // print out what we are looking for.
    printf("Parser's identifying Tokens: \n");
    printf("Entry point = \t\t\t%c\n", parser.entry_point);
    printf("Numeric Value = \t\t\t%d\n", parser.entry_point);
    printf("Exit point = \t\t\t%s\n", parser.exit_point);
    printf("Numeric Value = \t\t\t%d\n", *parser.exit_point);
    printf("Key Token Start = \t\t%s\n", parser.key_token_start);
    printf("Numeric Value = \t\t\t%d\n", *parser.key_token_start);
    printf("Key Token End = \t\t%s\n", parser.key_token_end);
    printf("Numeric Value = \t\t\t%d\n", *parser.key_token_end);
    printf("Key_Value Separator = \t\t%c\n", parser.key_value_separator);
    printf("Numeric Value = \t\t\t%d\n", parser.key_value_separator);
    printf("Value Token Start = \t\t%s\n",  parser.value_token_start);
    printf("Numeric Value = \t\t\t%d\n", *parser.value_token_start);
    printf("Value Token End = \t\t%s\n", parser.value_token_end);
    printf("Numeric Value = \t\t\t%d\n", *parser.value_token_end);
    printf("Element Terminator = \t\t%c\n", parser.element_terminator);
    printf("Numeric Value = \t\t\t%d\n", parser.element_terminator);
    printf("Concatenation Token = \t\t%s\n", parser.concatenation_token);
    printf("Numeric Value = \t\t\t%d\n", *parser.concatenation_token);
    printf("Array Token Start = \t\t%c\n", parser.array_token_start);
    printf("Numeric Value = \t\t\t%d\n", parser.array_token_start);
    printf("Array Token End = \t\t%c\n", parser.array_token_end);
    printf("Numeric Value = \t\t\t%d\n", parser.array_token_end);
}

char* read_file(const char* filename, bool return_it, bool verbose) {
    if (verbose){
        printf("READING DATA FILE: %s\n", filename);
    }

    // Open the file
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    size_t size = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer
    char* buffer = (char*)malloc(size + 1); // Allocate exact size
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Read the file contents into the buffer
    size_t bytesRead = fread(buffer, 1, size, file);
    if (bytesRead != size) {
        fclose(file);
        free(buffer);
        fprintf(stderr, "Error: Failed to read file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the buffer
    buffer[bytesRead] = '\0';

    // Close the file
    fclose(file);
    if (verbose){
        printf("BUFFER STORED OK AND FILE CLOSED!\n");
        printf("Length of the string in the buffer: %zu\n", bytesRead);
        // print out the buffer if verbose is true
        printf("File contents:\n %s", buffer);
    }
    // Return the buffer if requested
    if (return_it) {
        return buffer;
    } else {
        free(buffer); // Free the buffer if not returned
        return NULL;
    }
}
bool check_key_formatting(const char *chr_data, Parser parser){ 
    bool format_ok = false;
    bool found_key_end = false;
    bool found_key_value_separator = false;
    bool found_value_token_start = false;
    bool found_array_start = false;

    const char *chr_copy = chr_data;
   // printf("\nCHECK_KEY_FORMATTING: CHECK chr_data: %s\n\n", chr_data);
    //printf("\nCHECK_KEY_FORMATING: CHECK chr_copy: %s\n\n", chr_copy);
    printf("CHECK KEY FORMATTING: BEFORE THE LOOP!\n");
    // read through the data and check the key
    while (*chr_copy != '\0') {
        // Look for the key token end
        // Required.
        if (strncmp(chr_copy, parser.key_token_end, strlen(parser.key_token_end)) == 0) {
            // Move to the next character after the key token end
            found_key_end = true;
            chr_copy += strlen(parser.key_token_end);
            //printf("%c", *chr_copy);
            // Skip leading whitespace
            while (isspace(*chr_copy)) {
                chr_copy++;
            }
        }
        // Check for valid key value separator
        // Required
        if (*chr_copy == parser.key_value_separator) {
            // Move to the next character after the key value separator
            found_key_value_separator = true;
            chr_copy++;
            // Skip leading whitespace after separator
            while (isspace(*chr_copy)) {
                chr_copy++;
            }
        }
        // Check for valid array start token.
        // Required for array values.
        if (*chr_copy == parser.array_token_start){
            found_array_start = true;
            chr_copy++;
        }
        // Check for valid value start token.
        // Required for array values.
        if (found_array_start){
            while (isspace(*chr_copy)) {
                chr_copy++;
            }
            if(strncmp(chr_copy, parser.value_token_start,
                    strlen(parser.value_token_start)) == 0){
                chr_copy++;
                found_value_token_start = true;
                format_ok = true;
                break;
            }
        }
        // Check for valid value start token.
        // Required.
        if (!found_array_start && strncmp(chr_copy, parser.value_token_start, strlen(parser.value_token_start)) == 0){
            chr_copy++;
            found_value_token_start = true;
            format_ok = true;
            break;
        }
                    
        chr_copy++;
    }
    // check all flagged requirements.
    if (found_key_end && found_key_value_separator){
        if (found_array_start){
            if (found_value_token_start){
                format_ok = true;
            } else {
                format_ok = false;
            }
        } else {
            if(found_value_token_start){
                format_ok = true;
            } else {
                format_ok = false;
            }
        }
    }
    // If key requirements not found, key is malformed
    return format_ok;
}
//.................... ............ ............... ................... .......
bool check_value_formatting(const char *chr_data, Parser parser) {
    // value rules function is called after a key_value_seperator
    // : function called. format:
    // value_token_end -> element_terminator -> key_token_start  = valid
    // e.g. "value", "key" 
    //
    // value_token_end -> concatenation_token -> value_token_start -> restart loop
    //   until an -> element_terminator or an exit_point is found after a 
    //   value_token_end = valid
    // "value" + "value" loop restart -> , or exit_point.
    //
    //
    // value_token_end -> element_terminator -> exit_point = valid
    // e.g. "value", exit_point. 
    //
    // value_token_end -> exit_point. = valid
    // e.g. "value" exit_point.
    //
    // 1, Enter value start token - this position is already met when the
    // function is called.
    // 2, Enter the value end token.
    // 3, check for element terminator, this is only optional if an exit point
    // or a concatenation_token follows the value end token. Otherwise it is
    // required and must be followed by a key_token_start.
    // 4, In the case that goes "value" -> + -> "value"  loop through until
    // either an element terminator or an exit point is found.
    //     // 
    bool format_ok = false; // Assume format is incorrect by default.
    const char *chr_copy = chr_data;
    // Loop through the data
    while (*chr_copy != '\0') {

        if (strncmp(chr_copy, parser.value_token_end, strlen(parser.value_token_end)) == 0) {
            chr_copy++;

            // Skip leading whitespace
            while (isspace(*chr_copy)) {
                chr_copy++;
            }
            // concatenation rules
            if (strncmp(chr_copy, parser.concatenation_token, strlen(parser.concatenation_token)) == 0) {
                // value_token_end -> concatenation_token -> value_token_start -> restart loop
                //   until an -> element_terminator or an exit_point is found after a 
                //   value_token_end = valid
                // "value" + "value" loop restart -> , or exit_point.
                chr_copy++;
                // Skip leading whitespace
                while (isspace(*chr_copy)){
                    chr_copy++;
                }
                // check for value token start
                if(strncmp(chr_copy, parser.value_token_start, strlen(parser.value_token_start)) == 0) {
                    // value continues
                    continue;
                }
            }
            if (*chr_copy == parser.element_terminator){      
                chr_copy++;
                while(isspace(*chr_copy)){
                    chr_copy++;
                }
                if (strncmp(chr_copy, parser.key_token_start, strlen(parser.key_token_start)) == 0){
                    // value_token_end -> element_terminator -> key_token_start  = valid
                    // e.g. "value", "key"
                    // can leave the loop.
                    format_ok = true;
                    break;
                }
                // value ->element_terminator -> array_end -> element_terminator = valid
                if (*chr_copy == parser.array_token_end){
                    format_ok = true;
                    break;
                    // How to make this work?
                    
                }
                // value -> element_terminator -> exit_point
                if (strncmp(chr_copy, parser.exit_point, strlen(parser.exit_point)) == 0){
                    // value -> element_terminator -> exit_point = valid
                    // can leave loop.
                    format_ok = true;
                    break;
                }
            } // element terminator done
            if (strncmp(chr_copy, parser.exit_point, strlen(parser.exit_point)) == 0) {
                // value_token_end -> exit_point. = valid
                // e.g. "value" exit_point.
                // can leave loop.
                format_ok = true;
                break;
            } // exit point done
            
        }

        chr_copy++; // Move to the next character
    }

    return format_ok;
}

//.................... ............ ............... ................... .......
bool check_array_formatting(const char *chr_data, Parser parser) { 
    // array rules [ "value1", "value2", "value2", ],
    // element terminators before array_end_token are optional.
    // element terminators after the array_end_token are only required
    // if there is key_start_token following it.
    // 1, Enter the array start token - this is done already as we 
    // are in this function.
    // 2, Enter the value start and end tokens.
    // 3, check for element terminator, only required if there is a following
    // value token, if no value token then.
    // 4, Enter the array_end_token.
    // 5, check for element terminator, only required if there is a following
    // key token. Not required if exit_point is found after the
    // element terminator.

    bool format_ok = false;
    bool found_value_start = false;
    bool found_value_end = false;
    bool found_element_terminator = false;
    bool found_array_end = false;
    const char *chr_copy = chr_data;
    // Loop through the data now we're inside the array.
    while (*chr_copy != '\0') {
        //printf("chr_copy:\t\t%c\n", *chr_copy);
        // Check for value_token_start 
        if (!found_value_start && strncmp(chr_copy, parser.value_token_start, 
                    strlen(parser.value_token_start)) == 0) {
            // Value start found in array
            found_value_start = true;
            chr_copy++;
        }
        // Check for value_token_end
        if (!found_value_end && strncmp(chr_copy, parser.value_token_end,
                    strlen(parser.value_token_end)) == 0) {
            found_value_end = true;
            // Move through whitespace
            while (isspace(*chr_copy)) {
                chr_copy++;
            }
        }
        // Check for end of array first.
        if ((found_value_end && found_element_terminator) && *chr_copy == parser.array_token_end) {
            // Check if there is data following the array end token
            found_array_end = true;
            
            while (isspace(*chr_copy)) {
                chr_copy++;
            }
        }
        else if ((found_value_end && !found_element_terminator) && *chr_copy == parser.array_token_end){
            found_array_end = true;
            while(isspace(*chr_copy)){
                chr_copy++;
            }
        }
        // Check for element terminator (optional, unless followed by a value_token_start or array_token_end)
        if (found_value_end && found_array_end){
            if(*chr_copy == parser.element_terminator) {
                found_element_terminator = true;
                break;
            }
        }
        if (found_value_end && *chr_copy == parser.element_terminator){
            found_element_terminator = true;
            //chr_copy++;
            while (isspace(*chr_copy)) {
                    chr_copy++;
                }
            if(strncmp(chr_copy, parser.value_token_start, strlen(parser.value_token_start)) == 0){
                // Another value inside the array, restart the loop
                found_value_start = false;
                found_value_end = false;
                found_element_terminator = false;
                chr_copy--;
                printf("reset\n");
            }
        }
                       // Move to the next character
        chr_copy++;
    }
    // Checking requirements.
    if (found_array_end){
        if (found_value_start && found_value_end && found_element_terminator){
            format_ok = true;
            printf("CHECK_ARRAY_FORMATTING: ARRAY FORMAT OK!\n");
        } else if (found_value_start && found_value_end){
            format_ok = true;
            printf("CHECK_ARRAY_FORMATTING: ARRAY FORMAT OK!\n");
        }
        else {
            format_ok = false;
        }
    }
    // Return the formatting result
    return format_ok;
}
/*............... ................. ........................................*/
 /*"comment": "This is a comment.",
    "before array": "THIS IS A TEST FOR THE 3RD VALUE!",
    "cars": ["Alfa Romeo", "Nissan Skyline", "BMW M3",],
    "test": "Testing Data following an Array.",
*/
bool check_rules(const char *chr_data, Parser parser, TOKEN_MARKER token_marker){
    // Pass in the data, pass in the Parser, and ... will be the token rules
    // to be checked.
    bool conditions_met = false;
     
    const char *chr_copy = chr_data;
    const char *c = chr_copy;
    char *token = malloc(strlen("token maker type") +1);
    if (token_marker == KEY_START){
        strcpy(token, "KEY");
    }
    else if (token_marker == VALUE_START) {
        strcpy(token, "VALUE");
    }
    else if (token_marker == ARRAY_START) {
        strcpy(token, "ARRAY");
    } else {
        strcpy(token, "INVALID TOKEN");
        fprintf(stderr, "TOKEN_MARKER ERROR: %s\n", token);
        free(token);
        return false;
    }
    printf("\nCHECK_RULES: CHECKING %s\n", token);
    free(token);
    // scan for next appropriate token.
    // Rule for parser.key_token_start.
    if (strncmp(c, parser.key_token_start, strlen(parser.key_token_start)) == 0 && token_marker == KEY_START){
        // rule: next token found must be the key_token_end
        // keep moving through the data.
        if(check_key_formatting(c, parser)){
            conditions_met = true;
        } else {
            // Key formatting Failiure.
            printf("CHECK_RULES: Error: Key not formatted correctly.\n");
            return conditions_met;
        }
    }
    // value check goes here
    if (strncmp(c, parser.value_token_start, strlen(parser.value_token_start)) == 0 && token_marker == VALUE_START){
        // rule for parser.value_token_start
        if (check_value_formatting(c, parser)){
        conditions_met = true;
        }else {
            printf("CHECK_RULES: Error: Value not formatted correctly.\n");
            return conditions_met;
        }
    }
    // Array checking goes here
    if (*c == parser.array_token_start && token_marker == ARRAY_START){
        if (check_array_formatting(c, parser)){
            conditions_met = true;
        }else {
            printf("CHECK_RULES: Error: Array not formatted correctly.\n");
        }
    }
    return conditions_met;
}

//.............................................................................
// BROKEN! NEEDS FIXING PERHAPS RE-WRITING... CHECK AFTER GETTING SOME SLEEP!!!
bool check_data_integrity(char *data, Parser parser, bool verbose){
    // These are used to check the integrity of that data.

    // True when parser.entry_point is found. e.g. {
    bool entry_point_identified = false; 

    // True when parser.exit_point is found. e.g. },
    bool exit_point_identified = false;

    // True when parser.key_token_start and parser.key_token_end followed by the
    // parser.key_value_seperator is found. e.g. "key":
    bool key_identified = false;

    // True when parser.value_token_start parser.value_token_end followed by a
    // parser.element_terminator is found. e.g. "value",
    bool value_identified = false;

    // True when parser.key_value_separator is found. e.g : 
    // when directly after a parser.token_end
    bool key_value_separator_identified = false;

    // True when parser.element_terminator is found. e.g. ,
    // when directly after a parser.value_token_end
    bool element_terminator_identified = false;
    
    // True when parser.concatenation_token is found e.g +
    //bool concatenation_token_identified;

    // if all these above are true, set to true.
    bool is_valid = false;

    // use inside while loop when checking the data.
    bool checking_data = true;
    
    //bool inside_key = false;
    //bool inside_value = false;
    bool inside_array = false;
    if (verbose){
        print_parser_definition(parser);
        }
    // These are for counting how many keys, values, key_value pairs and other
    // tokens are found.
    int keys = 0;
    int values = 0;
    int key_value_pairs = 0;
    int entry_points = 0;
    int exit_points = 0;
    int element_terminators = 0;
    int arrays = 0;
    int data_objects = 0;
    int position = 0;
    if (verbose){
        printf("\nChecking data against parser\n");
    }
    char *chr_scan =  data + position;
    // loop through the data.
    while (checking_data){
        while (*chr_scan != '\0'){
        // printf("%c\n", *chr_scan);
        // for testing each token
        // printf("%d", !strncmp(parser.key_token_end, chr_scan, 1) );
        // check for entry point.
        
        if (*chr_scan == parser.entry_point){
            if (!entry_point_identified){
                entry_point_identified = true;
                entry_points++;
                if (verbose){
                    printf("\nEntry point discovered, %d found in total\n", entry_points);
                }
            }
        }
        // check key token start and end.
        if (!key_identified && !key_value_separator_identified && !inside_array){
            if (strncmp(chr_scan, parser.key_token_start,
                        strlen(parser.key_token_start)) == 0){
                if(verbose){
                    printf("\nKey token start found: %s\n", parser.key_token_start);
                }
                const char *c = chr_scan;
                if (check_rules(c, parser, KEY_START)){
                    while(*chr_scan != '\0'){
                        printf("%c", *chr_scan);
                        chr_scan++;
                        position++;
                        if (strncmp(chr_scan, parser.key_token_end, 
                                    strlen(parser.key_token_end)) == 0){
                            printf("%c", *chr_scan);
                            key_identified = true;
                            keys++;
                            break;
                        }
                    // key formatting OK.
                    }
                } else {
                    fprintf(stderr, "KEY ERROR: Key not formed correctly!\n");
                    return false;
                }
            }
            if (key_identified && !key_value_separator_identified){
                //key_end_pos = position;
            }
        } // found the key.
//...........................................................................//
        // check the key_value_seperator
        if (!key_value_separator_identified){
            if (*chr_scan == parser.key_value_separator){
                //printf("\nkey value separator found: %c\n", parser.key_value_separator);
                key_value_separator_identified = true;
            }
        }
        if (key_value_separator_identified && !value_identified){
            // check for arrays first.
            if (*chr_scan == parser.array_token_start){
                const char *c = chr_scan;
                if (check_rules(c, parser, ARRAY_START)){
                    chr_scan++;
                    position++;
                    inside_array = true;
                }else {
                    fprintf(stderr, "ARRAY ERROR: Array not formed properly.\n");
                    return false;
                }
            } 
            if (inside_array){
                while (*chr_scan != parser.array_token_end){
                    printf("%c", *chr_scan);
                    chr_scan++;
                    position++;
                    }
                while(*chr_scan != parser.element_terminator){
                    // Skip leading whitespace
                    while(isspace(*chr_scan)){
                        chr_scan++;
                        position++;
                    }
                    chr_scan++;
                    position++;
                }
                // found end of array reset values and start the loop.
                arrays++;
                values++;
                inside_array = false;
                value_identified = true;
                element_terminator_identified = true;
                key_value_pairs++;
                continue;
                }
            if (!inside_array && !value_identified){
                if (strncmp(chr_scan, parser.value_token_start, strlen(parser.value_token_start)) == 0){
                    const char *c = chr_scan;
                    if (check_rules(c, parser, VALUE_START)){
                        while (*chr_scan != '\0'){
                                chr_scan++;
                                position++;
                            if (strncmp(chr_scan, parser.value_token_end, strlen(parser.value_token_end)) == 0) {
                                // end of value string, need to check for element terminator or exit point.
                                    chr_scan++;
                                    position++;
                                    // check for concatenation token
                                    if (strncmp(chr_scan, parser.concatenation_token, strlen(parser.concatenation_token)) == 0){
                                        // found concatenation token.
                                        // restart the loop and keep looking for the value start and end tokens.
                                        break;
                                    }
                                }
                                if (*chr_scan == parser.element_terminator){
                                    // element terminator found.
                                    // if inside an array, loop without identifying value.
                                    value_identified = true;
                                    values ++;
                                    key_value_pairs++;
                                    element_terminator_identified = true;
                                    break;
                                }
                                if (strncmp(chr_scan, parser.exit_point, strlen(parser.exit_point)) == 0){
                                    // exit point found, value identified, end of data_object.
                                    value_identified  = true;
                                    values++;
                                    exit_point_identified = true;
                                    exit_points++;
                                    key_value_pairs++;
                                    entry_point_identified = false;
                                    break;
                                }
                            } // <- value token end check.
                        } // <- while loop inside value token start 
                    } // <- if check_value_formatting is true.
            } else {
                    fprintf(stderr, "VALUE ERROR: Value not formed correctly.\n");
                    return false;
                    }  
                } // <- if value token start check.
             // <- if key_value_seperator is true and value_identified is false.
        
//..................................................................
        // now that we have found the element terminator after the value.
        // we reset the key, key value pair, value, and element terminator.
        if (element_terminator_identified){
            element_terminators++;
            key_identified = false;
            key_value_separator_identified = false;
            value_identified = false;
            element_terminator_identified = false;
        }
        // TESTING CODE: RESET THE POSITION
        // position -= position;

        
        // check exit point.
        if (strncmp(chr_scan, parser.exit_point, strlen(parser.exit_point)) == 0){
            if (!exit_point_identified){
                exit_point_identified = true;
                exit_points++;
                if (verbose){
                    printf("\nExit point discovered, found %d in total.\n", exit_points);
                }
            }
        }
        if (*chr_scan == '\0'){
            checking_data = false;
            break;
        }
        printf("%c", *chr_scan);
        // move to next character.
        chr_scan++;
        position++;
        }

    checking_data = false;
    }
    if (keys && values > 0){
        if (key_value_pairs == ((keys + values) /2 )){
            key_identified = true;
            value_identified = true;
            if(element_terminators >0){
                element_terminator_identified = true;
            }else{
                printf("Error: Inconsistent element terminators: %c", parser.element_terminator);
            }
        }else {
            printf("Error: key value pair ratio inaccuate.: %c", parser.key_value_separator);
        }

    }else {
        printf("Error: Keys and or Values not found.\n");
    }
    if (entry_points && exit_points > 0){
        data_objects = (entry_points + exit_points) / 2;
    }else {
        printf("Error: entry and exit points mismatch: %c %s\n", parser.entry_point, parser.exit_point);
    }

    

    if (entry_point_identified && exit_point_identified && key_identified &&
            value_identified && element_terminator_identified){
        is_valid = true;
        // Print the report.
        print_parsing_report(entry_points,exit_points, keys, values, arrays, element_terminators, data_objects);
    }
    // testing 
    return is_valid;

    }

void print_parsing_report(int entry_points, int exit_points, int keys, int values, int arrays, int element_terminators, int data_objects){
        // Print the report.
        printf("\nDiscovery Report:\n");
        printf("Entry Points Discoverd: %d\n", entry_points);
        printf("Exit Points Discovered: %d\n", exit_points);
        printf("Keys Discovered: %d\n", keys);
        printf("Values Discovered: %d\n", values);
        printf("Arrays Discovered: %d\n", arrays);
        printf("Element Terminators Discovered: %d\n", element_terminators);
        printf("Data Objects Discovered: %d\n", data_objects);

}
