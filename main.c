#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "password.h"

#define CHUNK 1
#define MAX_PASSWORDS 100

// Function prototypes
char *getln();
void printMenu();
void printMainMenu();
void clrscr();
void manageMenu(int choice);
void clearInputBuffer();
void printLogo();
void printPasswordList();
void setKey();
void addPassword();

// Global variable for the encryption key
char *key;

int main(int argc, char const *argv[])
{
    // Set encryption key
    setKey();
    return 0;
}

// Function to set encryption key
void setKey()
{
    printf("Enter key: ");
    key = getln();
    printMainMenu();
}

// Function to print main menu
void printMainMenu()
{
    printLogo();
    printf("             Password-Manager by realr4an\n");
    printf("\n");
    printMenu();
}

// Function to print logo
void printLogo()
{
    printf("\n");
    printf(" ██╗  ██╗███████╗██╗   ██╗██████╗  █████╗ ███████╗███████╗ \n");
    printf(" ██║ ██╔╝██╔════╝╚██╗ ██╔╝██╔══██╗██╔══██╗██╔════╝██╔════╝ \n");
    printf(" █████╔╝ █████╗   ╚████╔╝ ██████╔╝███████║███████╗███████╗ \n");
    printf(" ██╔═██╗ ██╔══╝    ╚██╔╝  ██╔═══╝ ██╔══██║╚════██║╚════██║ \n");
    printf(" ██║  ██╗███████╗   ██║   ██║     ██║  ██║███████║███████║ \n");
    printf(" ╚═╝  ╚═╝╚══════╝   ╚═╝   ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝ \n");
    printf("\n");
}

// Function to print password list
void printPasswordList()
{
    Password passwordArray[MAX_PASSWORDS];
    FILE *file = fopen("passwords.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    char line[256];
    int numPasswords = 0;
    while (numPasswords < MAX_PASSWORDS && fgets(line, sizeof(line), file))
    {
        char desc[50], user[50], pas[50];

        if (sscanf(line, "%[^:]:%[^:]:%s", desc, user, pas) == 3)
        {
            strncpy(passwordArray[numPasswords].description, desc, sizeof(desc));
            strncpy(passwordArray[numPasswords].username, user, sizeof(user));
            strncpy(passwordArray[numPasswords].password, pas, sizeof(pas));
            numPasswords++;
            printf("%d. %s\n", numPasswords, desc);
        }
        else
        {
            printf("Error parsing line: %s\n", line);
        }
    }
    fclose(file);
    printf("Which password you want to display: ");
    int choice;
    if (scanf("%d", &choice) != 1)
    {
        clearInputBuffer();
        clrscr();
        printPasswordList();
    }
    else
    {
        clearInputBuffer();
        encryptPassword(&passwordArray[choice - 1], key);
        clrscr();
        printLogo();
        printf("Discription: %s\nUsername: %s\nPassword: %s\n", passwordArray[choice - 1].description, passwordArray[choice - 1].username, passwordArray[choice - 1].password);
        printf("Press Enter to continue...");
        getchar();
        decryptPassword(&passwordArray[choice - 1], key);
        clrscr();
    }
}

// Function to print menu
void printMenu()
{
    printf("1. Add new password\n");
    printf("2. Show all passwords\n");
    printf("3. Exit\n");
    printf("\nSelection: ");
    int choice;

    if (scanf("%d", &choice) != 1)
    {
        clearInputBuffer();
        clrscr();
        printMainMenu();
    }
    else
    {
        clearInputBuffer();
        manageMenu(choice);
    }
}

// Function to add new password
void addPassword()
{
    printf("Enter description: ");
    char *desc = getln();
    printf("Enter username: ");
    char *user = getln();
    printf("Enter password: ");
    char *pass = getln();
    Password password;
    setPassword(&password, desc, user, pass);
    decryptPassword(&password, key);
    savePassword(&password);
    printf("Press Enter to continue...");
    getchar();
    clrscr();
}

// Function to manage menu choices
void manageMenu(int choice)
{
    switch (choice)
    {
    case 1:
        clrscr();
        printLogo();
        addPassword();
        printMainMenu();
        break;
    case 2:
        clrscr();
        printLogo();
        printPasswordList();
        printMainMenu();
        break;
    case 3:
        printf("Exit\n");
        break;
    default:
        clrscr();
        printMainMenu();
        break;
    }
}

// Function to clear the screen
void clrscr()
{
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Unix/Linux/Mac
#endif
}

// Function to clear the input buffer
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// Function to read a line of input
char *getln()
{
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    int ch = EOF;

    while (ch)
    {
        ch = getc(stdin);
        if (ch == EOF || ch == '\n')
            ch = 0;

        if (size <= index)
        {
            size += CHUNK;
            tmp = realloc(line, size);
            if (!tmp)
            {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }

        line[index++] = ch;
    }

    return line;
}