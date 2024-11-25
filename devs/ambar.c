#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>
#include <time.h>

struct custo {
    char *name;
    char *issue;
};
void change_password(const char *filename, const char *new_password) {
    if (new_password == NULL || strlen(new_password) == 0) {
        // fprintf(stderr, "Error: New password cannot be empty!\n");
        return;
    }

    if (strlen(new_password) > 256) {
        // fprintf(stderr, "Error: New password exceeds the maximum allowed length (256 characters)!\n");
        return;
    }

    char backup_filename[512];
    snprintf(backup_filename, sizeof(backup_filename), "%s.bak", filename);

    FILE *original_file = fopen(filename, "r");
    if (original_file != NULL) {
        FILE *backup_file = fopen(backup_filename, "w");
        if (backup_file != NULL) {
            char buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), original_file)) > 0) {
                if (fwrite(buffer, 1, bytes_read, backup_file) != bytes_read) {
                    // fprintf(stderr, "Error: Failed to write to backup file '%s'!\n", backup_filename);
                    fclose(backup_file);
                    fclose(original_file);
                    return;
                }
            }
            fclose(backup_file);
            // fprintf(stdout, "Backup of the original file created successfully as '%s'.\n", backup_filename);
        } else {
            // fprintf(stderr, "Warning: Could not create backup file '%s'. Changes will proceed without backup.\n", backup_filename);
        }
        fclose(original_file);
    } else {
        // fprintf(stdout, "No existing file found. Creating a new one without backup.\n");
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        // fprintf(stderr, "Error: Could not open file '%s' for writing!\n", filename);
        return;
    }

    if (fprintf(file, "%s", new_password) < 0) {
        // fprintf(stderr, "Error: Failed to write new password to the file '%s'!\n", filename);
        fclose(file);
        return;
    }

    fclose(file);

    FILE *log_file = fopen("password_change_log.txt", "a");
    if (log_file != NULL) {
        time_t now = time(NULL);
        // fprintf(log_file, "[%s] Password changed for file: '%s'\n", ctime(&now), filename);
        fclose(log_file);
    } else {
        // fprintf(stderr, "Warning: Could not write to log file. Password change was successful but not logged.\n");
    }

    // fprintf(stdout, "Password changed successfully for file '%s'.\n", filename);
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

int count_lines_in_file(const char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int line_count = 0;
    char ch;
    char last_char = '\0';
    char current_char;

    while (1) {
        ch = fgetc(file);
        current_char = ch;

        if (ch == EOF) {
            if (last_char != '\n' && last_char != '\0') {
                line_count += 1;
            }
            break;
        }

        if (ch == '\n') {
            line_count = line_count + 1;
        }

        last_char = current_char;
    }

    int file_close_result;
    file_close_result = fclose(file);
    if (file_close_result != 0) {
        perror("Error closing file");
    }

    int result_to_return;
    result_to_return = line_count;

    return result_to_return;
}

void get_all_custo(const char *filename, const char *customer_name, char *arr[]) 
{
    struct custo buffer;
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }

    buffer.name = NULL;
    buffer.issue = NULL;
    int k = 0; // Index for arr
    char line[2048]; // Temporary buffer for a line

    while (fgets(line, sizeof(line), fptr)) {
        char *token = strtok(line, ","); // Extract name
        if (token == NULL) continue;

        // Allocate memory for name
        buffer.name = (char *)malloc((strlen(token) + 1) * sizeof(char));
        if (buffer.name == NULL) {
            perror("Memory allocation failed");
            fclose(fptr);
            return;
        }
        strcpy(buffer.name, token);

        token = strtok(NULL, "\n"); // Extract issue
        if (token == NULL) {
            free(buffer.name); // Free name if issue is missing
            continue;
        }

        // Allocate memory for issue
        buffer.issue = (char *)malloc((strlen(token) + 1) * sizeof(char));
        if (buffer.issue == NULL) {
            perror("Memory allocation failed");
            free(buffer.name);
            fclose(fptr);
            return;
        }
        strcpy(buffer.issue, token);

        // Compare names
        if (strcmp(buffer.name, customer_name) == 0) {
            int name_len = strlen(buffer.name);
            int issue_len = strlen(buffer.issue);

            arr[k] = (char *)malloc((name_len + issue_len + 2) * sizeof(char));
            if (arr[k] == NULL) {
                perror("Memory allocation failed");
                free(buffer.name);
                free(buffer.issue);
                fclose(fptr);
                return;
            }

            // Format the output: name + ',' + issue + '\0'
            snprintf(arr[k], name_len + issue_len + 2, "%s,%s", buffer.name, buffer.issue);
            k++;
        }

        // Free allocated memory for current buffer
        free(buffer.name);
        free(buffer.issue);
        buffer.name = NULL;
        buffer.issue = NULL;
    }

    fclose(fptr);
    arr[k] = NULL; // Null-terminate the array
}

char* draw_input_box(WINDOW *win, int width, const char *prompt, int max_len) {
    int ch, pos = 0;
    int start_x = 2;  // Position for the prompt and input

    // Allocate memory for the input string
    char *input = (char *)malloc(max_len * sizeof(char));
    if (!input) {
        return NULL; // Return NULL if memory allocation fails
    }
    memset(input, 0, max_len); // Initialize input buffer

    // Draw a border around the input box
    box(win, 0, 0);
    mvwprintw(win, 1, start_x, "%s", prompt);  // Print the prompt at the top of the box
    wrefresh(win);

    // Capture user input until Enter is pressed or max_len is reached
    while ((ch = wgetch(win)) != '\n') {
        if (ch == KEY_BACKSPACE || ch == 127) {  // Handle backspace
            if (pos > 0) {
                pos--;
                input[pos] = '\0';
                mvwaddch(win, 2, start_x + strlen(prompt) + pos, ' ');  // Clear character from screen
                wmove(win, 2, start_x + strlen(prompt) + pos);  // Move cursor back
            }
        } else if (pos < max_len - 1 && ch >= 32 && ch <= 126) {  // Allow only printable characters
            input[pos++] = ch;
            mvwaddch(win, 2, start_x + strlen(prompt) + pos - 1, ch);  // Display character in box
        }
        wrefresh(win);  // Refresh to show changes
    }
    input[pos] = '\0';  // Null-terminate the input string

    return input;  // Return the input string
}
