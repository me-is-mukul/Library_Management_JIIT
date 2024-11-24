#include <stdio.h>
#include<string.h>
#include <stdlib.h>
struct library
{
    char str[100];
    int bookno;
};

struct custo {
    char customer_name[100];  // Customer's name
    char book_name[100];      // Book name
    char issue_date[20];      // Date of issue in "YYYY-MM-DD" format
};

int is_customer_present(char *customer_name, const char *filename) {
    FILE *fptr = fopen(filename, "r");  // Open the file in read mode
    if (fptr == NULL) {
        perror("Error opening file");
        return 0;  // Error opening file
    }

    struct custo entry;  // Use custo struct to store customer data
    char ch;

    // Read through each line of the file
    while ((ch = fgetc(fptr)) != EOF) {
        // Put back the character and read customer_name (up to the first comma)
        ungetc(ch, fptr);  // Put back the character into the stream
        fscanf(fptr, "%99[^,],", entry.customer_name);  // Read customer name into entry.customer_name

        // Read book name (after the first comma)
        fscanf(fptr, "%99[^,],", entry.book_name);  // Read book name into entry.book_name

        // Read the date of issue (after the second comma)
        fscanf(fptr, "%19[^\n]", entry.issue_date);  // Read the issue date into entry.issue_date

        // Skip any newline character
        fgetc(fptr);  // Consume the newline character

        // Check if the customer name matches the given customer_name
        if (strcmp(entry.customer_name, customer_name) == 0) {
            fclose(fptr);  // Close the file after use
            return 1;  // Customer found
        }
    }

    fclose(fptr);  // Close the file after reading all lines
    return 0;  // Customer not found
}

int is_book_present(char *book_name, const char *filename) {
    FILE *fptr = fopen(filename, "r");  // Open the file
    if (fptr == NULL) {
        perror("Error opening file");
        return 0;  // Error opening file
    }

    struct library book;
    char ch;

    while ((ch = fgetc(fptr)) != EOF) {
        int i = 0;
        
        // Read the book name until comma
        ungetc(ch, fptr);  // Put back the character into the stream
        fscanf(fptr, "%99[^,],", book.str);  // Read book name into book.str
        
        // Read the book number (book ID)
        fscanf(fptr, "%d", &book.bookno);

        // Check if the book name matches the given book_name
        if (strcmp(book.str, book_name) == 0) {
            fclose(fptr);  // Close the file after use
            return 1;  // Book found
        }

        // Skip any remaining characters (after the book name and book number)
        fscanf(fptr, "%*[^\n]");  // Skip the rest of the line
        fgetc(fptr);  // Consume the newline character
    }

    fclose(fptr);  // Close the file after reading all lines
    return 0;  // Book not found
}

void pass_list_from_file(const char *file_name, char *array[])
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    char ch = '\0';
    int length = 0;
    int i = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        length = 0;
        while (ch != '\n' && ch != EOF)
        {
            length++;
            ch = fgetc(file);
        }
        if (length > 0)
        {
            array[i] = (char *)malloc((length + 1) * sizeof(char));
            if (array[i] == NULL)
            {
                printf("Memory allocation failed\n");
                fclose(file);
                return;
            }
            i++;
        }
    }
    fclose(file);
    file = fopen(file_name, "r");
    i = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        int j = 0;
        while (ch != '\n' && ch != EOF)
        {
            array[i][j++] = ch;
            ch = fgetc(file);
        }
        array[i][j] = '\0';
        i++;
    }
    array[i] = NULL;

    fclose(file);
}

void tabulate_each_element(char *array1[], char *array2[])
{
    int lines = 0;
    for (int i = 0; array1[i] != NULL; i++)
    {
        lines++;
    }
    int name1_max[lines];
    int name2_max[lines];
    int number_max[lines];
    for (int i = 0; i < lines; i++)
    {
        name1_max[i] = 0;
        name2_max[i] = 0;
        number_max[i] = 0;
    }
    for (int i = 0; array1[i] != NULL; i++)
    {
        for (int j = 0; *(array1[i] + j) != ','; j++)
        {
            name1_max[i]++;
        }
        for (int j = 0; *(array1[i] + j + name1_max[i] + 1) != ','; j++)
        {
            number_max[i]++;
        }
        for (int j = 0; *(array1[i] + j + name1_max[i] + number_max[i] + 2) != '\0'; j++)
        {
            name2_max[i]++;
        }
    }
    int temp;
    for (int i = 0; i < lines - 1; i++)
    {
        for (int j = 0; j < lines - 1 - i; j++)
        {
            if (name1_max[j] > name1_max[j + 1])
            {
                temp = name1_max[j];
                name1_max[j] = name1_max[j + 1];
                name1_max[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < lines - 1; i++)
    {
        for (int j = 0; j < lines - 1 - i; j++)
        {
            if (name2_max[j] > name2_max[j + 1])
            {
                temp = name2_max[j];
                name2_max[j] = name2_max[j + 1];
                name2_max[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < lines - 1; i++)
    {
        for (int j = 0; j < lines - 1 - i; j++)
        {
            if (number_max[j] > number_max[j + 1])
            {
                temp = number_max[j];
                number_max[j] = number_max[j + 1];
                number_max[j + 1] = temp;
            }
        }
    }
    int length1 = 0, length2 = 0, length3 = 0;
    int m, n;
    for (int i = 0; array1[i] != NULL; i++)
    {
        array2[i] = (char *)malloc(2 * (name1_max[lines - 1] + name2_max[lines - 1] + number_max[lines - 1]) + 10);
        *(array2[i]) = '|';
        *(array2[i] + 1) = '\t';
        n = 2;
        for (int j = 0; *(array1[i] + j) != ','; j++)
        {
            length1++;
        }
        for (m = 0; m < length1; m++, n++)
        {
            *(array2[i] + n) = *(array1[i] + m);
        }
        for (int j = 0; j < (name1_max[lines - 1] - length1); j++, n++)
        {
            *(array2[i] + n) = ' ';
        }
        *(array2[i] + n) = '\t';
        *(array2[i] + n + 1) = '|';
        *(array2[i] + n + 2) = '\t';
        n += 3;
        for (int j = 0; *(array1[i] + length1 + 1 + j) != ','; j++)
        {
            length2++;
        }
        m += 1;
        for (int j = 0; j < length2; j++, m++, n++)
        {
            *(array2[i] + n) = *(array1[i] + m);
        }
        for (int j = 0; j < (number_max[lines - 1] - length2); j++, n++)
        {
            *(array2[i] + n) = ' ';
        }
        *(array2[i] + n) = '\t';
        *(array2[i] + n + 1) = '|';
        *(array2[i] + n + 2) = '\t';
        n += 3;
        for (int j = 0; *(array1[i] + length2 + length1 + 2 + j) != '\0'; j++)
        {
            length3++;
        }
        m += 1;
        for (int j = 0; j < length3; j++, m++, n++)
        {
            *(array2[i] + n) = *(array1[i] + m);
        }
        for (int j = 0; j < (name2_max[lines - 1] - length3); j++, n++)
        {
            *(array2[i] + n) = ' ';
        }
        *(array2[i] + n) = '\t';
        *(array2[i] + n + 1) = '|';
        *(array2[i] + n + 2) = '\0';
        length1 = 0;
        length2 = 0;
        length3 = 0;
    }
}

void deleterow(const char *filename, const char *name) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char line[1024];
    char *token;
    const char delimiter[2] = ",";

    while (fgets(line, sizeof(line), file)) {
        char line_copy[1024];
        strcpy(line_copy, line);

        token = strtok(line_copy, delimiter);
        if (token == NULL || strcmp(token, name) != 0) {
            fputs(line, temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);

    if (remove(filename) != 0) {
        perror("Error deleting original file");
        return;
    }

    if (rename("temp.csv", filename) != 0) {
        perror("Error renaming temporary file");
    }
}
