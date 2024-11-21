#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "ambar.h"
#include "dhvani.h"

#define MAX_INPUT_LENGTH 100

struct library
{
    char str[100];
    int bookno;
};

void take_input_assign_value(WINDOW *win, const char *prompt, char *input, int y, int x, int on_off){
    mvwprintw(win, y, x, prompt);
    wrefresh(win);
    int ch, i = 0;
    while ((ch = wgetch(win)) != '\n') {
        if (ch == KEY_BACKSPACE || ch == 127) {  // Handle backspace
            if (i > 0) {
                i--;
                mvwdelch(win, y, x+strlen(prompt) + i); // Delete the last character
            }
        } else if (i < MAX_INPUT_LENGTH - 1) {  // Add character to input
            input[i++] = ch;
            mvwaddch(win, y, x + strlen(prompt) + i - 1, (on_off==0)?(ch):('*')); // Display for each character
        }
        wrefresh(win);
    }
    input[i] = '\0';
}

int read_password_from_file_for_reset(char *password, size_t max_len) {
    FILE *file = fopen("passwords/password.txt", "r");
    if (!file) {
        perror("Could not open password file");
        return 1;
    }
    if (fgets(password, max_len, file) == NULL) {
        fclose(file);
        return 1;
    }
    // Remove newline character from the end of the password if it exists
    password[strcspn(password, "\n")] = '\0';
    fclose(file);
    return 0;
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


void add_book(WINDOW *right_win){
    printf("into add book");
}

void search_book(WINDOW *right_win){
    printf("into search book");

}

void add_customer(WINDOW *right_win){
    printf("into add customer");
}

void search_customer(WINDOW *right_win){
    printf("search customer");
}

void delete_customer(WINDOW *right_win){
    printf("delete search customer");
}

void delete_book(WINDOW *right_win){
    printf("delete book");
}

void reset_password(WINDOW *right_win){
    char current_password[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter current password : ", current_password, 1, 2, 1);

    char stored_password[MAX_INPUT_LENGTH];
    if (read_password_from_file_for_reset(stored_password, sizeof(stored_password)) != 0) {
        endwin();
        fprintf(stderr, "Error: Could not read password from file.\n");
    }

    if (strcmp(current_password, stored_password) == 0) {        // resetting password
        char new_password[MAX_INPUT_LENGTH];
        take_input_assign_value(right_win, "Enter new password : ", new_password, 4, 2, 0);

        change_password("passwords/password.txt", new_password);    

        mvwprintw(right_win, 6, 2, "Password changed Successfully!!!");
        wrefresh(right_win);

        napms(1500);  // Pause to show message

    } else {         // wrong password
        mvwprintw(right_win, 4, 2, "Wrong Password Bhosdike!");
        wrefresh(right_win);
        napms(1500);  // Pause to show message
    }   
    werase(right_win);
    box(right_win, 0, 0);
    wrefresh(right_win);
}

void display_all_books(WINDOW *right_win) {
    printf("displaying all books");
}




void main_loop(int men, WINDOW *win){
    switch (men){
        case 1:
            add_book(win);
            break;
        case 2:
            search_book(win);
            break;
        case 3:
            add_customer(win);
            break;
        case 4:
            search_customer(win);
            break;
        case 5:
            delete_customer(win);
            break;
        case 6:
            delete_book(win);
            break;
        case 7:
            reset_password(win);
            break;
        case 8:
            display_all_books(win);
            break;
    }
}