#include <stdio.h>
#include<string.h>
struct library
{
    char str[100];
    int bookno;
};
int is_book_present(char *book_name, FILE *fptr)
{
    struct library book;
    char ch;
    while (ch != EOF)
    {
        int i = 0;
        fscanf(fptr, "%c", &book.str[i]);
        while (book.str[i] != ',')
        {
            i++;
            fscanf(fptr, "%c", &book.str[i]);
        }
        book.str[i] = '\0';
        fscanf(fptr, "%d", &book.bookno);
        int length = 0;
        for (int j = 0; book.str[j] != '\0'; j++)
        {
            length++;
        }
        int count = 0;
        for (int j = 0; book_name[j] != '\0'; j++)
        {
            if (book.str[j] == book_name[j])
            {
                count++;
            }
        }
        int booklength = 0;
        for (int j = 0; book_name[j] != '\0'; j++)
        {
            booklength++;
        }
        ch = fgetc(fptr);
        strcpy(book.str, "");
        if (length == count && length == booklength)
        {
            fclose(fptr);
            return 1;
        }
        else
        {
            continue;
        }
    }
    fclose(fptr);
    return 0;
}
// int main()
// {
//     FILE *fptr;
//     fptr = fopen("books.txt", "r");
//     printf("%d", is_book_present("The Great Adventure", fptr));
//     fptr = fopen("books.txt", "r");
//     printf("%d", is_book_present("Echoes of the Past", fptr));
//     return 0;
// }