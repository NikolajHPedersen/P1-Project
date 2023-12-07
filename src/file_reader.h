#include <stdlib.h>
#include <stdio.h>

struct patient{
    unsigned int patient_id;
    char first_name[32];
    char last_name[32];
    int age;
    char factors[][32];

}; typedef struct patient patient;
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

//Find and read the patient line of a specific patient, found by CPR-Number, using binary search
char *find_and_read_patient_line_binary(char file_name[], unsigned int cpr);

//Returns the string on the line number given. Returns null if string is out-of-bounds
char *read_entry(char file_name[], int line);

//Returns the string with containing a certain cpr
//Works only with VALID CPR numbers
char *read_entry_cpr(char file_name[], long cpr);

//MISC functionality

//Returns line-number of entry containing cpr
//Works only with a VALID CPR number
int find_entry_cpr(char file_name[], long cpr);

//Retrieves all the CPR-Numbers in the CPR-DB and returns them to an array
void retrieve_cpr_as_arr(char file_name[], int number_of_entries, unsigned int* output_arr);

//Implements a "insertion sort"-style algorithm to sort cpr-numbers from lowest to highest
void sort_cpr_database(char file_name[]);

//The implementation of the insertion sort algorithm
void insertion_sort(unsigned int array[], int size_of_array);

//The implementation of the binary search algorithm
int binary_search(unsigned int* arr_to_search, unsigned int target_num, int lower_bound_of_arr, int highest_bound_of_arr);

//HELPER FUNCTIONS

//Gets total number of entries in CPR DB
int get_number_of_entries_in_cpr(char file_name[]);

//Copies the contents of the source file to the destination file (No error handling)
void copy_file(FILE *source,FILE *destination);

//Copies everything before the line parameter to the destination file (No error handling)
void copy_file_to_line(FILE *source,FILE *destination, int line);

//Checks if a file was opened correctly
void check_fopen_success(FILE* fp);

//Swaps values in an array.
void index_swap_array(unsigned int array[], int swapper_index, int swappee_index);

//Finds the '\n' in a string and returns the index of it
int find_newline_index(char* string);

//Removes the unnecessary '\n' at the end
void uncorrupt_after_append(char* file_name);