#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    // Create left, right, and top windows
    WINDOW *left_win = newwin(LINES - 10, COLS / 4, 8, 0);          // Left window
    WINDOW *right_win = newwin(LINES - 10, COLS * 3 / 4, 8, COLS / 4); // Right window
    WINDOW *top_win = newwin(8, COLS, 0, 0);                         // Top window

    // Refresh main screen to ensure windows are visible
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
        NULL // Null-terminate the array
    };

    // Create menu and associate it with the left window
    MENU *menu = new_menu(items);
    set_menu_win(menu, left_win);                       // Set menu window
    set_menu_sub(menu, derwin(left_win, 8, 40, 1, 1));  // Subwindow dimensions for menu
    set_menu_mark(menu, " -> ");                        // Set menu item marker

    // Post the menu and draw box around left window
    box(left_win, 0, 0);
    post_menu(menu);
    wrefresh(left_win);



    // Display centered text in top window
    const char *text = "<< LIBRARY MANAGEMENT SHYSHTUM >>";
    int text_x = (COLS - strlen(text)) / 2;             // Center the text
    mvwprintw(top_win, 4, text_x, "%s", text);          // Place in the middle
    box(top_win, 0, 0);
    wrefresh(top_win);


    // make box around right window
    box(right_win, 0, 0);
    wrefresh(right_win);



    int ch;
    while ((ch = getch()) != 'q') {                     // Press 'q' to exit
        switch (ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
        }
        wrefresh(left_win);
    }

    // Cleanup
    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < 7; ++i) free_item(items[i]); // Free all items
    delwin(left_win);       
    delwin(right_win);
    delwin(top_win);
    endwin();

    return 0;
}
