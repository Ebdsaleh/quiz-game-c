// question_model.h - by Nadir Saleh 04/03/2024
#ifndef QUESTION_MODEL_H
#define QUESTION_MODEL_H
struct Question {
    const char *text;
    const char *answer;
};

struct Question create_question(const char *text, const char *answer);
struct Question create_default_question();
void clear_question(struct Question *question);
struct Question return_error_question();
#endif // QUESTION_MODEL_H

