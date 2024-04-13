#ifndef PASSWORD_H
#define PASSWORD_H

// Maximum length for description, username, and password
#define MAX_LENGTH 50

// Structure for storing password information
typedef struct
{
    char description[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} Password;

// Function prototypes

// Set password details
void setPassword(Password *p, const char *desc, const char *username, const char *password);

// Print password details
void printPassword(const Password *p);

// Save password to file
void savePassword(Password *p);

// Decrypt password using the given key
void decryptPassword(Password *password, const char *key);

// Encrypt password using the given key
void encryptPassword(Password *password, const char *key);

#endif /* PASSWORD_H */
