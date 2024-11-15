#include <stdio.h>
#include<string.h>


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
