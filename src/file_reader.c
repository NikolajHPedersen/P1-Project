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
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }

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
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }

    //Copy everything from the temp file to the main file
    copy_file(temp,fp);

    //Close both files and remove the temp file
    fclose(temp);
    remove("temp.txt");
    fclose(fp);
}

void change_entry_v2(char file_name[], char message[], int line_number){
    FILE *fp = fopen(file_name,"r+");
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }

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
    if(fp == NULL){
        printf("Something went wrong");
        exit(EXIT_FAILURE);
    }

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

    for(int i = 1;i < line;i++){
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

//Helper functions
void copy_file_to_line(FILE *source,FILE *destination,int line){
    char current_line[100];
    for(int i = 0;i < line;i++){
        fgets(current_line,100,source);
        fputs(current_line,destination);
    }
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