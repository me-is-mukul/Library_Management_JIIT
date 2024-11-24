    #include <ncurses.h>
    #include <menu.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include "passwords/password.h"
    #include "devs/mukul.h"

    #define esc 27  

    void activate_window(WINDOW *win, int pair){
        wattron(win, COLOR_PAIR(pair)); // Turn on color pair 1
        box(win, 0, 0);              // Draw the box with colored border
        wattroff(win, COLOR_PAIR(pair)); // Turn off color pair 1  
        wrefresh(win);
    }   

    int main() {
        int menu_place = 1;
        int val;
        passing(&val);

        if (val == 0){
            goto hi;
        }
        // Initialize ncurses
        initscr();
        cbreak();
        noecho();   
        keypad(stdscr, TRUE);
        start_color();

        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); 
    
        attron(COLOR_PAIR(3)); // Turn on yellow color pair
        mvprintw(LINES - 1, 2, "Press 'q' to exit menu mode\t\t\tuse arrow keys to toggle left right window\t\t\t use arrow keys to toggle up down menu\t\t\tCtrl+Z to force exit program"); // Position at bottom-left
        attroff(COLOR_PAIR(3)); // Turn off ye  llow color pair

        // Create left, right, and top windows          
        WINDOW *left_win = newwin(LINES - 10, COLS / 4, 8, 0);
        WINDOW *right_win = newwin(LINES - 10, COLS * 3 / 4, 8, COLS / 4);
        WINDOW *top_win = newwin(8, COLS, 0, 0);
        refresh();

        // Create menu items dynamically    
        char *menu_options[] = {
            "Add Books In Database",
            "Search Book From Database",
            "Add Customer In Database",
            "Search Customer From Database",
            "Delete Customer From Database",
            "Display CUstomers From Database",
            "Set New Password",
            "Display All Books",
        };
        ITEM *  items[sizeof(menu_options)/sizeof(menu_options[0])];   
        for (int i = 0; i < sizeof(menu_options) / sizeof(menu_options[0]); ++i) {
            items[i] = new_item(menu_options[i], NULL);
        }

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
                case '\n': // Switch focus with enter key
                    if (active_window == 1) {
                        active_window = 2;
                        activate_window(right_win, 1);
                        activate_window(left_win, 2);
                        mvwprintw(right_win, 1, 2, "");
                        main_loop(menu_place, right_win);
                        wrefresh(right_win);

                    }
                    break;

                case KEY_RIGHT:
                    if (active_window == 1) {
                        active_window = 2;
                        activate_window(right_win, 1);
                        activate_window(left_win, 2);
                        mvwprintw(right_win, 3, 2, "");
                        main_loop(menu_place, right_win );
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
                        (menu_place<8)?(menu_place++):(menu_place=8);
                        menu_driver(menu, REQ_DOWN_ITEM);
                    }
                    break;

                case KEY_UP:
                    if (active_window == 1) {  
                        (menu_place>1)?(menu_place--):(menu_place=1);
                        menu_driver(menu, REQ_UP_ITEM);
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

            // Refresh windows after any input
            wrefresh(left_win);
            wrefresh(right_win);
        }

        // Cleanup
        unpost_menu(menu);
        free_menu(menu);
            for (int i = 0; i < 8; ++i) free_item(items[i]);
        delwin(left_win);
        delwin(right_win);
        delwin(top_win);
        endwin();
        hi: 
        return 0;
    }