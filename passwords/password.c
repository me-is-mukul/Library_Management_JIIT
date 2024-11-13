#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_INPUT_LENGTH 100

// Function to read the password from password.txt
int read_password_from_file(char *password, size_t max_len) {
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

int passing(int *validity) {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    // Create a window with size LINES/3 by COLS/3
    int height = LINES / 3;
    int width = COLS / 3;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;
    WINDOW *win = newwin(height, width, starty, startx);

    // Draw a white box around the window
    wattron(win, COLOR_PAIR(1));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(1));
    mvwprintw(win, 1, 2, "Enter Password:");
    wrefresh(win);

    // Read the password from the file
    char stored_password[MAX_INPUT_LENGTH];
    if (read_password_from_file(stored_password, sizeof(stored_password)) != 0) {
        endwin();
        fprintf(stderr, "Error: Could not read password from file.\n");
        return 1;
    }

    // Take user input and match with the stored password
    char input[MAX_INPUT_LENGTH];
    int ch, i = 0;
    mvwprintw(win, 2, 2, "");  // Move cursor to start position for input
    wrefresh(win);

    while ((ch = wgetch(win)) != '\n') {
        if (ch == KEY_BACKSPACE || ch == 127) {  // Handle backspace
            if (i > 0) {
                i--;
                mvwdelch(win, 2, 2 + i); // Delete the last character
            }
        } else if (i < MAX_INPUT_LENGTH - 1) {  // Add character to input
            input[i++] = ch;
            mvwaddch(win, 2, 2 + i - 1, '*'); // Display '*' for each character
        }
        wrefresh(win);
    }
    input[i] = '\0';  // Null-terminate the input string

    // Check if input matches the stored password
    if (strcmp(input, stored_password) == 0) {
        *validity = 1;
        mvwprintw(win, 4, 2, "Access Granted! Exiting...");
        wrefresh(win);
        napms(1500);  // Pause to show message
    } else {
        *validity = 0;
        mvwprintw(win, 4, 2, "Access Denied!");
        wrefresh(win);
        napms(1500);  // Pause to show message
    }

    // Clean up
    delwin(win);
    endwin();
    return 0;
}
