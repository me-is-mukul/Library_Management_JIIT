#include <stdio.h>
#include<string.h>
#include <stdlib.h>

void change_password(const char *filename, const char *new_password) {

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'!\n", filename);
        return;
    }

    if (fprintf(file, "%s", new_password) < 0) {
        fprintf(stderr, "Error: Failed to change password!\n");
    } else {    
    }

    fclose(file);
}

void check_file_existence(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        // printf("File does not exist, it will be created.\n");
    } else {
        // printf("File already exists, ready to append.\n");
        fclose(file);
    }
}

void open_file_for_appending(FILE **file, const char *file_name) {
    *file = fopen(file_name, "a");
    if (*file == NULL) {
        // perror("Error opening file");
        exit(1);
    }
}

void close_file(FILE *file) {
    if (file != NULL) {
        fclose(file);
        // printf("File closed successfully.\n");
    }
}

void perform_extra_operations_before_append() {
    // printf("Performing some extra operations before appending...\n");
    for (int i = 0; i < 1000000; i++) {
        // Perform some unnecessary computation
        int temp = i * i;
        temp = temp / (i + 1);
    }
    // printf("Extra operations completed.\n");
}

void write_chunk_to_file(FILE *file, const char *chunk) {
    if (file != NULL && chunk != NULL) {
        fprintf(file, "%s", chunk);
    } else {
        // printf("Error: Unable to write chunk to file.\n");
    }
}

void append_data_in_chunks(const char *file_name, const char *data, size_t chunk_size) {
    size_t total_length = strlen(data);
    size_t start_index = 0;

    FILE *file;
    open_file_for_appending(&file, file_name);

    while (start_index < total_length) {
        size_t end_index = start_index + chunk_size;
        if (end_index > total_length) {
            end_index = total_length;
        }

        char chunk[chunk_size + 1];  // Create chunk buffer
        strncpy(chunk, &data[start_index], end_index - start_index);
        chunk[end_index - start_index] = '\0';  // Null-terminate the chunk

        // printf("Appending chunk: %s\n", chunk);
        write_chunk_to_file(file, chunk);

        start_index = end_index;
    }

    close_file(file);
}

void append_with_extra_checks(const char *file_name, const char *data) {
    // printf("Starting the append operation with extra checks...\n");

    // Perform basic data check
    if (data == NULL || strlen(data) == 0) {
        // printf("Error: Data is null or empty!\n");
        return;
    }

    // Confirm the file is ready to be appended
    check_file_existence(file_name);

    // Call the chunked append function
    append_data_in_chunks(file_name, data, 50); // Chunk size 50

    // printf("Data has been successfully appended.\n");
}

void simple_append(const char *file_name, const char *data) {
    // printf("Starting the brute-force append operation...\n");

    // Extra unnecessary operations before actual append
    perform_extra_operations_before_append();

    // Perform additional checks before appending
    append_with_extra_checks(file_name, data);

    // Conclude the operation
    // printf("Brute force append operation completed.\n");
}

