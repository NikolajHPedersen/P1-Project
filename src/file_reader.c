#include "file_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This file (poorly) implements the ability to read and write to text files
//These are all massively inefficient but there might not be better ways to do it with text files
//(If there are any ideas for improvements let me know [Nikolaj])

//TODO
// - rewrite change_entry to just overwrite the line instead of using a temp file
// - discuss the use of a sorted list so it is possible to use binary search

//WRITE & REMOVE
void append_entry(char file_name[], char message[]){
    FILE *fp = fopen(file_name,"a");
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }

    fprintf(fp,"%s\n",message);
    fclose(fp);
}

void change_entry(char file_name[], char message[], int line_number){
    //Open the file in which the entry should be inserted and a temp file
    FILE *fp = fopen(file_name,"r");
    FILE *temp = fopen("temp.txt","w");

    //Copy the contents of the main file to the temp file until the line
    //Where the entry is to be changed
    copy_file_to_line(fp,temp,line_number);

    //Skip the line to be changed
    char temp_buffer[100];
    fgets(temp_buffer,100,fp);

    //Add the line to the temp file
    fprintf(temp,"%s\n",message);

    //Copy the rest of the main file to the temp file
    copy_file(fp,temp);

    //Close both files so restrict mode of fopen can be changed
    //This is done so that the contents of the temp file can be
    //transferred back into main file
    fclose(fp);
    fclose(temp);

    fp = fopen(file_name,"w");
    temp = fopen("temp.txt","r");

    //Copy everything from the temp file to the main file
    copy_file(temp,fp);

    //Close both files and remove the temp file
    fclose(temp);
    remove("temp.txt");
    fclose(fp);
}

void change_entry_v2(char file_name[], char message[], int line_number){
    FILE *fp = fopen(file_name,"r+");

    char current_line[50];

    for(int i = 1;i < line_number;i++){
        fgets(current_line,50,fp);
    }

    fputs(message,fp);

    fclose(fp);
}

void insert_entry(char file_name[], char message[], int line_number){
    //Open the file in which the entry should be inserted and a temp file
    FILE *fp = fopen(file_name,"r");
    FILE *temp = fopen("temp.txt","w");

    //Copy the contents of the main file to the temp file until the line
    //Where the entry is to be inserted
    copy_file_to_line(fp,temp,line_number);

    //Add the line to the temp file
    fprintf(temp,"%s\n",message);

    //Copy the rest of the main file to the temp file
    copy_file(fp,temp);

    //Close both files so restrict mode of fopen can be changed
    //This is done so that the contents of the temp file can be
    //transferred back into main file
    fclose(fp);
    fclose(temp);

    fp = fopen(file_name,"w");
    temp = fopen("temp.txt","r");

    //Copy everything from the temp file to the main file
    copy_file(temp,fp);

    //Close both files and remove the temp file
    fclose(temp);
    remove("temp.txt");
    fclose(fp);

}

void remove_entry(char file_name[], int line){
    //Open the file in which the entry should be inserted and a temp file
    FILE *fp = fopen(file_name,"r");
    FILE *temp = fopen("temp.txt","w");

    //Copy the contents of the main file to the temp file until the line
    //Where the entry is to be removed
    copy_file_to_line(fp,temp,line);

    //Call fgets skipping the line to be removed from being copied to the temp-file
    char temp_buffer[100];
    fgets(temp_buffer,100,fp);


    //Copy the rest of the main file to the temp file
    copy_file(fp,temp);

    //Close both files so restrict mode of fopen can be changed
    //This is done so that the contents of the temp file can be
    //transferred back into main file
    fclose(fp);
    fclose(temp);

    fp = fopen(file_name,"w");
    temp = fopen("temp.txt","r");

    //Copy everything from the temp file to the main file
    copy_file(temp,fp);

    //Close both files and remove the temp file
    fclose(temp);
    remove("temp.txt");
    fclose(fp);

}

//READ

char *read_entry(char file_name[], int line){
    FILE *fp = fopen(file_name,"r");
    char buffer[100];

    char *check_ptr;

    for(int i = 1;i <= line;i++){
        check_ptr = fgets(buffer,100,fp);
        if(check_ptr == NULL){
            return NULL;
        }

    }
    check_ptr = fgets(buffer,100,fp);
    if(check_ptr == NULL){
        return NULL;
    }
    fclose(fp);
    return strdup(buffer);
}


char *read_entry_cpr(char file_name[], long cpr){
    FILE *fp = fopen(file_name,"r");
    char cpr_str[10];
    sprintf(cpr_str,"%ld",cpr);

    char buffer[100] = "";

    char *check_ptr;

    for(;;){
        check_ptr = fgets(buffer,100,fp);
        if(check_ptr == NULL){
            return NULL;
        }
        if(strstr(buffer, cpr_str)){
            break;
        }

    }
    fclose(fp);
    return strdup(buffer);
}

//MISC
//TODO: Fungerer ikke lige helt efter forventning, returnerer forkeret linje.
int find_entry_cpr(char file_name[], long cpr){
    FILE *fp = fopen(file_name,"r");
    char cpr_str[10];
    sprintf(cpr_str,"%ld",cpr);

    char buffer[100] = "";

    char *check_ptr;

    int count = 1;

    for(;;count++){
        check_ptr = fgets(buffer,100,fp);
        if(check_ptr == NULL){
            return -1;
        }
        if(strstr(buffer, cpr_str)){
            break;
        }
    }

    return count;
}

void sort_cpr_database(char file_name[]){
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("Can't open or create files as required. Quiting program...\n");
        exit(EXIT_FAILURE);
    }

    //FASE 1: Importing CPR-Numbers into an array

    //Determine how many patient entries there are in txt file, by checking the amount of '\n'
    int number_of_entries = 0;
    while(!feof(fp)){
        if(fgetc(fp) == '\n'){
            number_of_entries++;
        }
    }

    //TODO: Remove Debug printf when done
    printf("Number of entries: %d\n", number_of_entries);

    //Rewind file pointer to starting position
    /*Using fseek, one can set the location of the file position indicator to a specific location,
     *and check if any errors occured. fseek returns a non-zero value if there is an error.
     * If its succesful it returns zero */
    if(fseek(fp, 0L, SEEK_SET) != 0){
        printf("An error has occured during database loading. Quitting program...\n");
        exit(EXIT_FAILURE);
    }

    //Find CPR-Number string on a line & Converting CPR-Number string into a long long & loading it into an array
    long long cpr_arr[number_of_entries];
    for (int i = 0; i <= number_of_entries; ++i) {
        char buffer[200];
        fgets(buffer, 200, fp);
        sscanf(buffer, "id: %lld,", &cpr_arr[i]);

        //TODO: Remove Debug printf when done
        printf("Parsed CPR-Number string: %lld\n", cpr_arr[i]);
    }
    fclose(fp);

    //FASE 2: Sort the array using an implementation of Insertion Sort.
    //TODO: Remove Debug printf when done
    printf("\n\n");
    insertion_sort(cpr_arr, number_of_entries);

    for (int i = 0; i <= number_of_entries; ++i) {
        //TODO: Remove Debug printf when done
        printf("Sorted CPR-Number: %lld\n", cpr_arr[i]);
    }

    printf("%s", read_entry_cpr("test_db.txt", 5));
    //FASE 3: Opdatere CPR-DB
    //Går gennem hele listen for hver enkelt linje i listen/DB
    for (int i = 0; i <= number_of_entries; ++i) {
        for (int j = 0; j <= number_of_entries; ++j) {
            long long val;
            char* line_to_check = read_entry("test_db.txt", j);
            sscanf(line_to_check, "id: %lld", &val);
            if(val == cpr_arr[i]){
                //Removing '\n' from the string
                int newline_index = find_newline_index(line_to_check);
                if(newline_index != -1){
                    line_to_check[newline_index] = '\0';
                }

                append_entry("tmp_db.txt", line_to_check);
            }
        }
    }
    remove(file_name);
    rename("tmp_db.txt", file_name);

}

void insertion_sort(long long array[], int size_of_array) {
    //i has a value of 1, so that we can start by comparing the second element in the array to the first
    for (int i = 1; i <= size_of_array; ++i) {
        int pointer = i;

        //If the value of the variable that the pointer points to, is less than its predecessor, then we swap them
        while(array[pointer - 1] > array[pointer] && pointer != 0){
            index_swap_array(array, pointer, pointer - 1);
            pointer--;
        }

    }
}

//Helper functions
void copy_file_to_line(FILE *source,FILE *destination,int line){
    char current_line[100];
    for(int i = 0;i < line;i++){
        fgets(current_line,100,source);
        fputs(current_line,destination);
    }
}

void index_swap_array(long long* array, int swapper_index, int swappee_index){
    int swapper_value = array[swapper_index];
    array[swapper_index] = array[swappee_index];
    array[swappee_index] = swapper_value;
}

void copy_file(FILE *source,FILE *destination){
    char *check_ptr;
    char current_line[100];

    for(;;){
        check_ptr = fgets(current_line,100,source);

        if(check_ptr == NULL)
            break;

        fputs(current_line,destination);
    }

}

int find_newline_index(char* string){
    for (int i = 0; i < strlen(string); ++i) {
        if(string[i] == '\n'){
            return i;
        }
    }
    return -1;
}