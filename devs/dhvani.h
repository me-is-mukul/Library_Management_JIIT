#ifndef DHVANI_H
#define DHVANI_H

int is_book_present(char *book_name, const char *filename);
int is_customer_present(char *customer_name, const char *filename);
void pass_list_from_file(const char *file_name, char *array[]);
void tabulate_each_element(char *array1[], char *array2[]);
void deleterow(const char *filename, const char *name);
#endif