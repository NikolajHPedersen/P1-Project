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
    check_fopen_success(fp);

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
char *find_and_read_patient_line_binary(char file_name[], unsigned int cpr){
    FILE* fp = fopen(file_name, "r");
    check_fopen_success(fp);


}

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

void retrieve_cpr_as_arr(char file_name[], int number_of_entries, unsigned int* output_arr){
    //Find CPR-Number string on a line & Converting CPR-Number string into an unsigned int & loading it into an array
    FILE* fp = fopen(file_name, "r");
    check_fopen_success(fp);

    for (int i = 0; i <= number_of_entries; ++i) {
        char buffer[200];
        fgets(buffer, 200, fp);
        sscanf(buffer, "id: %u,", &output_arr[i]);

        //TODO: Remove Debug printf when done
        printf("Parsed CPR-Number string: %u\n", output_arr[i]);
    }
    fclose(fp);
}

void sort_cpr_database(char file_name[]){
    //FASE 1: Importing CPR-Numbers into an array

    //Get total amount of entries in CPR DB
    int number_of_entries = get_number_of_entries_in_cpr(file_name);

    //TODO: Remove Debug printf when done
    printf("Number of entries: %d\n", number_of_entries);

    //Get all the CPR-Numbers as an array
    unsigned int cpr_arr[number_of_entries];
    retrieve_cpr_as_arr(file_name, number_of_entries, cpr_arr);

    //FASE 2: Sort the array using an implementation of Insertion Sort.
    //TODO: Remove Debug printf when done
    printf("\n\n");
    insertion_sort(cpr_arr, number_of_entries);

    for (int i = 0; i <= number_of_entries; ++i) {
        //TODO: Remove Debug printf when done
        printf("Sorted CPR-Number: %lld\n", cpr_arr[i]);
    }


    //FASE 3: Update CPR-DB
    //Goes through the entire list for every line in the list/DB
    for (int i = 0; i <= number_of_entries; ++i) {
        for (int j = 0; j <= number_of_entries; ++j) {
            unsigned int val;
            char* line_to_check = read_entry("test_db.txt", j);
            sscanf(line_to_check, "id: %u", &val);
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
    //uncorrupt_after_append("tmp_db.txt");

    remove(file_name);
    rename("tmp_db.txt", file_name);

}

void insertion_sort(unsigned int array[], int size_of_array) {
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
int get_number_of_entries_in_cpr(char file_name[]){
    //Determine how many patient entries there are in txt file, by checking the amount of '\n'
    FILE *db_fp = fopen(file_name, "r");
    check_fopen_success(db_fp);

    int number_of_entries = 0;
    while(!feof(db_fp)){
        //Also checking if the file ends after the '\n', so that it does'nt count
        if(fgetc(db_fp) == '\n' && fgetc(db_fp) != EOF){
            number_of_entries++;
        }
    }
    fclose(db_fp);

    return number_of_entries;
}

void copy_file_to_line(FILE *source,FILE *destination,int line){
    char current_line[100];
    for(int i = 0;i < line;i++){
        fgets(current_line,100,source);
        fputs(current_line,destination);
    }
}

void check_fopen_success(FILE* fp){
    if(fp == NULL){
        printf("Can't open or create files as required. Quiting program...\n");
        exit(EXIT_FAILURE);
    }
}

void index_swap_array(unsigned int* array, int swapper_index, int swappee_index){
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
    if(string == NULL){
        return -1;
    }

    for (int i = 0; i < strlen(string); ++i) {
        if(string[i] == '\n'){
            return i;
        }
    }
    return -1;
}

void uncorrupt_after_append(char* file_name){
    FILE* fp = fopen(file_name, "r+");
    check_fopen_success(fp);

    //Set file location pointer to end - 1
    if(fseek(fp, -1L, SEEK_END) != 0){
        printf("Could'nt uncorrupt after append....");
        exit(EXIT_FAILURE);
    }

    char c;
    if((c = fgetc(fp)) == '\n'){
        fseek(fp, -1L, SEEK_END);
        fputc('\0', fp);
    }

    fclose(fp);
}