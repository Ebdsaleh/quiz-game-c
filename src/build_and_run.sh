#!/bin/bash
clear
gcc -g -Wall -Wextra -pedantic-errors main.c question_model.c quiz_brain.c question_data.c parser.c -o ../build/quiz

if [ $? -eq 0 ]; then
    echo "Build successful. Executable created: quiz"
    # Copy question data to build directory
    echo "Copying files..."
    cp question_data ../build/
    echo "OK!"
    echo "program start..."
    cd ../build/
    ./quiz
else
    echo "Build failed."
fi

