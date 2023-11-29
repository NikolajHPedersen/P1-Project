#include <stdlib.h>
#include <stdio.h>


//WRITE/REMOVE functionality

//adds a string to the end of text-file
void append_entry(char file_name[], char message[]);

//adds a string at the line number specified (No error handling)
void insert_entry(char file_name[], char message[], int line_number);

//changes entry at the line number specified to the message parameter (No error handling)
void change_entry(char file_name[], char message[], int line_number);
void change_entry_v2(char file_name[], char message[], int line_number);

//Remove entry from line number (No error handling)
void remove_entry(char file_name[], int line);


//READ functionality

//Returns the string on the line number given. Returns null if string is out-of-bounds
char *read_entry(char file_name[], int line);

//Returns the string with containing a certain cpr
char *read_entry_cpr(char file_name[], long cpr);

//MISC functionality

//Returns line-number of entry containing cpr
int find_entry_cpr(char file_name[], long cpr);

//HELPER FUNCTIONS

//Copies the contents of the source file to the destination file (No error handling)
void copy_file(FILE *source,FILE *destination);

//Copies everything before the line parameter to the destination file (No error handling)
void copy_file_to_line(FILE *source,FILE *destination, int line);

