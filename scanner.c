//
//  scanner.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#include "common.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and 
 return types for functions with ???.
 ******************/

static void skip_comment();
static void skip_blanks(char arrayWithBlanks[]);
static void get_word();
static void get_number();
static void get_string();
static void get_special();
static void transfer(char dst_array[], char src_array[]);
char token_string[256];
char skipArray[256];

typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE,
}
CharCode;
/*********************
 Static Variables for Scanner
 Must be initialized in the init_scanner function.
 *********************/
static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];  // The character table

//
int debugCount = 0;


const char *reserved[35] =
{   "do","if","in","of","or","to",
    "and","div","end","for","mod","nil","not","set","var",
    "case","else","file","goto",
    "then","type","with",
    "array","begin","const","label","until","while",
    "downto", "packed","record","repeat",
    "program","function","procedure"
};



void init_scanner(FILE *source_file, char source_name[], char date[])
{
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);
    
    
}
BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
//char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that?
    static int line_number = 0;
    
    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL)
    {
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

char get_token()
{
   // char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
    get_source_line(token_string);


    while(token_string[0] != '\0')
    {
    	//printf("I went through the while loop: %d times\n", debugCount); //debug line
    	transfer(skipArray,token_string);
        skip_blanks(skipArray);  //1.  Skip past all of the blanks
        transfer(token_string,skipArray);
        //printf("This is token_string after skip_blanks :2%s2\n", token_string); //debug
        //printf("ldksaf");
        if(token_string[0] == '\n')
        {
            return 'a';
        }
        if(token_string[0] == '.')
        {
            printf("\t>> .\t.\n");
            return '.';
        }
        if(isalpha(token_string[0]))
        {
        	get_word();
        }
        else if(isdigit(token_string[0]))
        {
            //token_string = get_number(token_string);
            get_number();
        }
        else if(token_string[0] == '\'')
        {
            //token_string = get_string(token_string);
            get_string();
        }
        else if(token_string[0] == '{'){
        	skip_comment(token_string);
        }
        else
        {
            if(token_string[0] == '.')
            {
                return '.';
            }
            get_special();
        }

    }
    return 'a'; //What should be returned here?
}

static void  skip_blanks(char arrayWithBlanks[])
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */
	char temp[MAX_TOKEN_STRING_LENGTH]; //create temp
	/*while(token_string[0] == ' '){ // while space exists in the first
		for(i = 1; i<strlen(token_string); i++){
			temp[i-1] = token_string[i];

		}
		transfer(token_string, temp);
	}
		printf("i went through skip blank\n");
    //return token_string;     */
    //printf("This is testArray before:1%s1\n", arrayWithBlanks);

	while(arrayWithBlanks[0] == ' '){ // while space exists in the first
		for(int i = 1; i<strlen(arrayWithBlanks); i++){
			temp[i-1] = arrayWithBlanks[i];

		}
		temp[strlen(temp)+1] = '\0';
		for(int i = 0; i< strlen(temp); i++)
		{
			skipArray[i] = temp[i];
		}
	}
		//printf("i went through skip blank\n");
		transfer(skipArray, arrayWithBlanks);
		//printf("This is testArray:1%s1\n", arrayWithBlanks);
    //return token_string;

}

static void  skip_comment()
{
    /*
     Write some code to Extract the string
     */
	char temp[MAX_TOKEN_STRING_LENGTH];
	char temp2[MAX_TOKEN_STRING_LENGTH];
	int count = 0;
    
    for(int i = 0; i < strlen(token_string); i++)
    {
        token_string[i] = token_string[i+1];
    }
    token_string[strlen(token_string)+1] = '\0';
    
    while(token_string[0] != '}'){ //AS LONG AS THERE EXISTS A CHARACTER
		temp[count] = token_string[0]; //ASSIGN CHARACTER AT THE CURRENT COUNT OF TEMP TO FIRST CHARACTER OF TOKEN STRING
        
		for(int i = 1; i<strlen(token_string); i++){ //START PAST FIRST BLANK CHARACTER (
			temp2[i-1] = token_string[i];
			temp2[i+1] = '\0';
		}
        
		transfer(token_string, temp2);
        
		count++;
        
		temp[count+1] = '\0';
	}//End of while
    token_string[0] = ' ';
    
}

static void get_word()
{
    /*
     Write some code to Extract the word
     */
	char temp[MAX_TOKEN_STRING_LENGTH];
	char uptemp[MAX_TOKEN_STRING_LENGTH];
	char temp2[MAX_TOKEN_STRING_LENGTH];
	int count = 0;
    
    for(int i = 0; i < strlen(uptemp); i++)
    {
        uptemp[i] = ' ';
    }

        
	while(isalpha(token_string[0])){ //AS LONG AS THERE EXISTS A CHARACTER
		temp[count] = token_string[0]; //ASSIGN CHARACTER AT THE CURRENT COUNT OF TEMP TO FIRST CHARACTER OF TOKEN STRING

		for(int i = 1; i<strlen(token_string); i++){ //START PAST FIRST BLANK CHARACTER (
			temp2[i-1] = token_string[i];
			temp2[i+1] = '\0';
		}

		transfer(token_string, temp2);

		count++;

		temp[count+1] = '\0';
	}//End of while

    transfer(uptemp, temp);
    
    //Downshift the word, to make it lower case
	for(int i = 0; i<strlen(temp); i++){
				temp[i] = tolower(temp[i]);
			}

    int flag = 0;
    //checks for reserved word
	for(int i=0; i<35; i++){
		if(strcmp(temp, reserved[i]) == 0){
				printf( "\t>> %s\t%s\n", uptemp, temp);
                line_count++;
				flag = 1;
            break;
		}
	}
    
	if(flag == 0)
    {
		printf( "\t>> <INDENTIFIER>\t%s\n", temp);
        line_count++;
	}



}

static void  get_number()
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
	char temp[256];
	char temp2[MAX_TOKEN_STRING_LENGTH];
	int count = 0;

    while(isdigit(token_string[0]) || (token_string[0] == '.') || (token_string[0] == 'e') || (token_string[0] == '-')){ //AS LONG AS THERE EXISTS A CHARACTER
		temp[count] = token_string[0]; //ASSIGN CHARACTER AT THE CURRENT COUNT OF TEMP TO FIRST CHARACTER OF TOKEN STRING
        
		for(int i = 1; i<strlen(token_string); i++){ //START PAST FIRST BLANK CHARACTER (
			temp2[i-1] = token_string[i];
			temp2[i+1] = '\0';
		}
        
        //printf("Temp 1: %s\n", temp);  //Debugging Lines
        //printf("Temp 2: %s\n", temp2); //Debugged
		transfer(token_string, temp2);
        
		count++;
        
		temp[count+1] = '\0';
	}//End of while
    printf("\t>> <NUMBER>\t%s\n", temp);
    line_count++;

}
static void  get_string()
{
    /*
     Write some code to Extract the string
     */
	char temp[MAX_TOKEN_STRING_LENGTH];
	char temp2[MAX_TOKEN_STRING_LENGTH];
	int count = 0;

    for(int i = 0; i < strlen(token_string); i++)
    {
        token_string[i] = token_string[i+1];
    }
    token_string[strlen(token_string)+1] = '\0';
    
    while(token_string[0] != '\''){ //AS LONG AS THERE EXISTS A CHARACTER
		temp[count] = token_string[0]; //ASSIGN CHARACTER AT THE CURRENT COUNT OF TEMP TO FIRST CHARACTER OF TOKEN STRING
        
		for(int i = 1; i<strlen(token_string); i++){ //START PAST FIRST BLANK CHARACTER (
			temp2[i-1] = token_string[i];
			temp2[i+1] = '\0';
		}
        
        //printf("Temp 1: %s\n", temp);  //Debugging Lines
        //printf("Temp 2: %s\n", temp2); //Debugged
		transfer(token_string, temp2);
        
		count++;
        
		temp[count+1] = '\0';
	}//End of while
    token_string[0] = ' ';
    

    printf("\t>> <STRING>\t%s\n", temp);
    line_count++;
}
static void  get_special()
{
    printf("\t>> %c\t%c\n", token_string[0], token_string[0]);
    line_count++;
    token_string[0] = ' ';
}
static void transfer(char dst_array[], char src_array[]) //TRANSFERS ELEMENTS OF SRC_ARRAY TO DST_ARRAY
{
    for(int j = 0; j < strlen(src_array); j++)
    {
        dst_array[j] = src_array[j];
    }
}

