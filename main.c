#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "passwords/password.h"

#define esc 27 

void activate_window(WINDOW *win, int pair){
    wattron(win, COLOR_PAIR(pair)); // Turn on color pair 1
    box(win, 0, 0);              // Draw the box with colored border
    wattroff(win, COLOR_PAIR(pair)); // Turn off color pair 1  
    wrefresh(win);
}   

int main() {
    int val;
    // Initialize ncurses       
    passing(&val);

    if (val == 0){
        goto hi;
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK); 
  
    

    // Create left, right, and top windows          
    WINDOW *left_win = newwin(LINES - 10, COLS / 4, 8, 0);
    WINDOW *right_win = newwin(LINES - 10, COLS * 3 / 4, 8, COLS / 4);
    WINDOW *top_win = newwin(8, COLS, 0, 0);
    refresh();

    // Create menu items
    ITEM *items[] = {
        new_item("Add Books In Database", NULL),
        new_item("Search Book From Database", NULL),
        new_item("Add Customer In Database", NULL),
        new_item("Search Customer From Database", NULL),
        new_item("Delete Customer From Database", NULL),
        new_item("Delete Book From Database", NULL),
        new_item("Set New Password", NULL),
        NULL
    };

    // Create menu and associate it with the left window
    MENU *menu = new_menu(items);
    set_menu_win(menu, left_win);
    set_menu_sub(menu, derwin(left_win, 8, 40, 1, 1));
    set_menu_mark(menu, " -> ");
    wattron(left_win, COLOR_PAIR(1)); // Turn on color pair 1
    box(left_win, 0, 0);              // Draw the box with colored border
    wattroff(left_win, COLOR_PAIR(1)); // Turn off color pair 1  
    post_menu(menu);
    wrefresh(left_win);

    // Display centered text in top window
    const char *text = "<< LIBRARY MANAGEMENT SHYSHTUM >>";
    int text_x = (COLS - strlen(text)) / 2;
    mvwprintw(top_win, 4, text_x, "%s", text);
    box(top_win, 0, 0);
    wrefresh(top_win);

    // Make box around right window
    box(right_win, 0, 0);
    wrefresh(right_win);

    // Track active window: 1 for left, 2 for right
    int active_window = 1;

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to exit
        switch (ch) {
            case '\n': // Switch focus with enter    key
                if (active_window == 1) {
                    active_window = 2;
                    activate_window(right_win, 1);
                    activate_window(left_win, 2);
                    mvwprintw(right_win, 1, 2, "");
                    wrefresh(right_win);    
                }
                break;
            
            case esc:
                if (active_window == 2){
                    active_window = 1;
                    activate_window(left_win, 1);
                    activate_window(right_win, 2);

                    mvwprintw(left_win, 1, 2, "");
                    wrefresh(left_win);  
                }       
                break;  

            case KEY_DOWN:
                if (active_window == 1) {
                    menu_driver(menu, REQ_DOWN_ITEM);
                }
                break;

            case KEY_UP:
                if (active_window == 1) {       
                    menu_driver(menu, REQ_UP_ITEM);
                }
                break;
            
            case KEY_RIGHT:
                if (active_window == 1) {
                    active_window = 2;
                    activate_window(right_win, 1);
                    activate_window(left_win, 2);

                    mvwprintw(right_win, 3, 2, "");
                    wrefresh(right_win);
                }
                break;
            
            case KEY_LEFT:
                if (active_window == 2){
                    active_window = 1;

                    activate_window(left_win, 1);
                    activate_window(right_win, 2);

                    mvwprintw(left_win, 1, 2, "");
                    wrefresh(left_win);  
                }       
                break; 

                
        }

        // Refresh windows after any inp        ut
        wrefresh(left_win);
        wrefresh(right_win);
    }

    // Cleanup
    unpost_menu(menu);
    free_menu(menu);
        for (int i = 0; i < 7; ++i) free_item(items[i]);
    delwin(left_win);
    delwin(right_win);
    delwin(top_win);
    endwin();
    hi: 
    return 0;
}
