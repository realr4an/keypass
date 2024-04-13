#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "password.h"

// Function prototypes
void setPassword(Password *p, const char *desc, const char *username, const char *password);
void savePassword(Password *p);
void decryptPassword(Password *password, const char *key);
void encryptPassword(Password *password, const char *key);

// Function to set password details
void setPassword(Password *p, const char *desc, const char *username, const char *password)
{
    strncpy(p->description, desc, MAX_LENGTH);
    strncpy(p->username, username, MAX_LENGTH);
    strncpy(p->password, password, MAX_LENGTH);
}

// Function to save password to file
void savePassword(Password *p)
{
    FILE *file = fopen("passwords.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error creating temp file!\n");
        fclose(file);
        exit(1);
    }

    char line[300];
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        char desc[100];
        sscanf(line, "%[^:]", desc);

        if (strcmp(desc, p->description) == 0)
        {
            found = 1;
            fprintf(tempFile, "%s:%s:%s\n", p->description, p->username, p->password);
        }
        else
        {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("passwords.txt");
    rename("temp.txt", "passwords.txt");

    if (found)
    {
        printf("Logindetails for %s got updated.\n", p->description);
    }
    else
    {
        file = fopen("passwords.txt", "a");
        if (file == NULL)
        {
            printf("Error reopening file!\n");
            exit(1);
        }
        fprintf(file, "%s:%s:%s\n", p->description, p->username, p->password);
        fclose(file);
        printf("Logindetails for %s got saved.\n", p->description);
    }
}

// Function to decrypt password
void decryptPassword(Password *password, const char *key)
{
    int keyLength = strlen(key);
    int usernameLength = strlen(password->username);
    int passwordLength = strlen(password->password);

    char encryptedUsername[50];
    char encryptedPassword[50];

    for (int i = 0; i < usernameLength; i++)
    {
        int decryptedChar = (int)(password->username[i]) - (int)(key[i % keyLength]);
        if (decryptedChar < 32)
            decryptedChar += 95;
        encryptedUsername[i] = (char)decryptedChar;
    }

    for (int i = 0; i < passwordLength; i++)
    {
        int decryptedChar = (int)(password->password[i]) - (int)(key[i % keyLength]);
        if (decryptedChar < 32)
            decryptedChar += 95;
        encryptedPassword[i] = (char)decryptedChar;
    }

    strncpy(password->username, encryptedUsername, usernameLength);
    strncpy(password->password, encryptedPassword, passwordLength);

    printf("Logindetails for %s got decrypted.\n", password->description);
}

// Function to encrypt password
void encryptPassword(Password *password, const char *key)
{
    int keyLength = strlen(key);
    int usernameLength = strlen(password->username);
    int passwordLength = strlen(password->password);

    char encryptedUsername[50];
    char encryptedPassword[50];

    for (int i = 0; i < usernameLength; i++)
    {
        int encryptedChar = (int)(password->username[i]) + (int)(key[i % keyLength]);
        if (encryptedChar > 126)
            encryptedChar -= 95;
        encryptedUsername[i] = (char)encryptedChar;
    }

    for (int i = 0; i < passwordLength; i++)
    {
        int encryptedChar = (int)(password->password[i]) + (int)(key[i % keyLength]);
        if (encryptedChar > 126)
            encryptedChar -= 95;
        encryptedPassword[i] = (char)encryptedChar;
    }

    strncpy(password->username, encryptedUsername, usernameLength);
    strncpy(password->password, encryptedPassword, passwordLength);

    printf("Logindetails for %s got encrypted.\n", password->description);
}
