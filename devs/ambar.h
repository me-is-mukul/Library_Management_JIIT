#ifndef AMBAR_H
#define AMBAR_H

void change_password(char *filename, char *new_password);
void simple_append(const char *file_name, const char *data);
int count_lines_in_file(const char *filename);
void get_all_custo(const char *filename, const char *customer_name, char *arr[]);
char* draw_input_box(WINDOW *win, int width, const char *prompt, int max_len);
#endif
