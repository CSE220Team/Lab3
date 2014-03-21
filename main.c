//
//  main.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

line_count = MAX_LINES_PER_PAGE;

FILE *init_lister(const char *name, char source_file_name[], char dte[]);
//void quit_scanner(FILE *src_file, Token *list);
//void add_token_to_list(Token *list, Token *new_token);

int main(int argc, const char * argv[])
{
    char token;
    char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file = init_lister(argv[1], source_name, date);
    init_scanner(source_file, source_name, date);
    
    do
    {
        token = get_token();
    }
    while (token != '.'); //loop ends when a period is found returning the char 0
    
    return 0;
}


FILE *init_lister(const char *name, char source_file_name[], char dte[])
{
    time_t timer;
    FILE *file;
    
    strcpy(source_file_name, name);
    file = fopen(source_file_name, "r");
    time(&timer);
    strcpy(dte, asctime(localtime(&timer)));
    return file;
}

