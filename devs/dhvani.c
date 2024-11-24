#include <stdio.h>
#include<string.h>
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


// int main()
// {
//     printf("%d", is_book_present("The Great Advewwnture", "../database/books.txt"));
//     return 0;
// }