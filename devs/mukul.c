#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "ambar.h"
#include "dhvani.h"
#include <menu.h>

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


void add_book(WINDOW *right_win){
    char book_name[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter book name : ", book_name, 1, 2, 0);
    if (is_book_present(book_name, "database/books.txt")) {
        mvwprintw(right_win, 4, 2, "Book already exists.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
        return;

    } else {
        char book_number[MAX_INPUT_LENGTH];
        take_input_assign_value(right_win, "Enter book number : ", book_number, 4, 2, 0);
        strcat(book_name, "," );
        strcat(book_name, book_number);
        strcat(book_name, "\n");
        simple_append("database/books.txt", book_name); 
        mvwprintw(right_win, 7 , 2, "Book added successfully.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);

    }   
}

void search_book(WINDOW *right_win){
    char book_name[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter book name : ", book_name, 1, 2, 0);
    if (is_book_present(book_name, "database/books.txt")) {
        mvwprintw(right_win, 4, 2, "Book found.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
    } else {
        mvwprintw(right_win, 4, 2, "Book not found.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
    }
}

void add_customer(WINDOW *right_win){
    char customer_name[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter customer name : ", customer_name, 1, 2, 0);
    char book_name[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter book name : ", book_name, 4, 2, 0);
    char issue_date[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter issue date : ", issue_date, 7, 2, 0);
    strcat(customer_name, "," );
    strcat(customer_name, book_name);
    strcat(customer_name, "," );
    strcat(customer_name, issue_date);
    strcat(customer_name, "\n");
    simple_append("database/customers.txt", customer_name); 
    mvwprintw(right_win, 10  , 2, "Customer added successfully.");
    wrefresh(right_win);
    napms(1500);
    werase(right_win);
    box(right_win, 0, 0);
    wrefresh(right_win);
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

void search_customer(WINDOW *right_win) {        
    char customer_name[MAX_INPUT_LENGTH];
    int lines = count_lines_in_file("database/customers.txt");
    take_input_assign_value(right_win, "Enter customer name : ", customer_name, 1, 2, 0);
    
    if (is_customer_present(customer_name, "database/customers.txt")) {
        char *input[lines];
        get_all_custo("database/customers.txt", customer_name, input);
        
        int item_count = 0; 
        while (input[item_count] != NULL) { 
            item_count++; 
        }

        if (item_count == 0) {
            mvwprintw(right_win, 4, 2, "No matching records found.");
            wrefresh(right_win);
            napms(1500);
            werase(right_win);
            box(right_win, 0, 0);
            wrefresh(right_win);
            return;
        }

        ITEM *itemss[item_count + 1];
        for (int i = 0; i < item_count; ++i) {
            itemss[i] = new_item(input[i], NULL);
        }
        itemss[item_count] = NULL;

        MENU *menuu = new_menu(itemss);
        set_menu_win(menuu, right_win);
        set_menu_sub(menuu, derwin(right_win, 20, 40, 13, 30));
        set_menu_mark(menuu, " -> ");
        box(right_win, 0, 0);
        post_menu(menuu);
        wrefresh(right_win);

        int ch;
        while ((ch = getch()) != 'q') { 
            switch (ch) {
                case KEY_DOWN:
                    menu_driver(menuu, REQ_DOWN_ITEM);
                    break;
                case KEY_UP:
                    menu_driver(menuu, REQ_UP_ITEM);
                    break;
            }
            wrefresh(right_win);
        }

        unpost_menu(menuu);
        free_menu(menuu);
        for (int i = 0; i < item_count; ++i) {
            free_item(itemss[i]);
            free(input[i]); // Free memory for each string
        }
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
    } else {
        mvwprintw(right_win, 4, 2, "Customer not found.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
    }
}

void delete_customer(WINDOW *right_win){
    char customer_name[MAX_INPUT_LENGTH];
    take_input_assign_value(right_win, "Enter customer name : ", customer_name, 1, 2, 0);
    if (is_customer_present(customer_name, "database/customers.txt")) {
        deleterow("database/customers.txt", customer_name);
        mvwprintw(right_win, 4, 2, "Customer deleted successfully.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
    } else {
        mvwprintw(right_win, 4, 2, "Customer not found.");
        wrefresh(right_win);
        napms(1500);
        werase(right_win);
        box(right_win, 0, 0);
        wrefresh(right_win);
    }
}
    
void display_all_customers(WINDOW *right_win){
    int lines = count_lines_in_file("database/customers.txt");
    char *input[lines];
    pass_list_from_file("database/customers.txt", input);
    ITEM *  itemss[lines+1];
    for (int i = 0; i < lines; ++i) {
        itemss[i] = new_item(input[i], NULL);
    }
    itemss[lines] = NULL;
    // Create menu and associate it with the left window
    MENU *menuu = new_menu(itemss);
    set_menu_win(menuu, right_win);
    set_menu_sub(menuu, derwin(right_win, 20, 40, 13, 30));
    set_menu_mark(menuu, " -> ");
    box(right_win, 0, 0);              // Draw the box with colored border
    post_menu(menuu);
    wrefresh(right_win);
    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to exit
        switch (ch) {
            case KEY_DOWN:
                menu_driver(menuu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menuu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(menuu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(menuu, REQ_SCR_UPAGE);
                break;
        }
        wrefresh(right_win);
    }
    werase(right_win);
    box(right_win, 0, 0);
    wrefresh(right_win);
    endwin();
}

void display_all_books(WINDOW *right_win) {
    int lines = count_lines_in_file("database/books.txt");
    char *input[lines];
    char *input2[lines];
    pass_list_from_file("database/books.txt", input);
    // tabulate_each_element(input2, input);
    ITEM *  itemss[lines+1];
    for (int i = 0; i < lines; ++i) {
        itemss[i] = new_item(input[i], NULL);
    }
    itemss[lines] = NULL;
    // Create menu and associate it with the left window
    MENU *menuu = new_menu(itemss);
    set_menu_win(menuu, right_win);
    set_menu_sub(menuu, derwin(right_win, 20, 40, 13, 30));
    set_menu_mark(menuu, " -> ");
    box(right_win, 0, 0);              // Draw the box with colored border
    post_menu(menuu);
    wrefresh(right_win);
    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to exit
        switch (ch) {
            case KEY_DOWN: // Switch focus with enter key
                menu_driver(menuu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menuu, REQ_UP_ITEM);
                break;
        }
        wrefresh(right_win);
    }
    werase(right_win);
    box(right_win, 0, 0);
    wrefresh(right_win);
    endwin();

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
            display_all_customers(win);
            break;
        case 7:
            reset_password(win);
            break;
        case 8:
            display_all_books(win);
            break;
    }
}
