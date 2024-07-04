#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_TITLE_LENGTH 50
#define MAX_AUTHOR_LENGTH 30
#define MAX_USERS 50
#define MAX_BORROWED_BOOKS 5

// Structure to represent a book
struct Book {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int available;
};

// Structure to represent a user
struct User {
    char username[20];
    int borrowedBooks[MAX_BORROWED_BOOKS];
    int numBorrowed;
};

struct Book library[MAX_BOOKS];
struct User users[MAX_USERS];

int numBooks = 0;
int numUsers = 0;

// Function to add a book to the library
void addBook(char title[], char author[]) {
    if (numBooks < MAX_BOOKS) {
        strcpy(library[numBooks].title, title);
        strcpy(library[numBooks].author, author);
        library[numBooks].available = 1;
        numBooks++;
        printf("Book added successfully.\n");
    } else {
        printf("Library is full. Cannot add more books.\n");
    }
}

// Function to display all books in the library
void displayBooks() {
    if (numBooks == 0) {
        printf("No books in the library.\n");
        return;
    }

    printf("Books in the library:\n");
    for (int i = 0; i < numBooks; i++) {
        printf("%d. Title: %s, Author: %s, Available: %s\n", i + 1, library[i].title, library[i].author,
               library[i].available ? "Yes" : "No");
    }
}

// Function to search for a book by title
void searchBook(char title[]) {
    int found = 0;
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(library[i].title, title) == 0) {
            printf("Book found:\n");
            printf("Title: %s, Author: %s, Available: %s\n", library[i].title, library[i].author,
                   library[i].available ? "Yes" : "No");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Book not found.\n");
    }
}

// Function to add a new user
void addUser(char username[]) {
    if (numUsers < MAX_USERS) {
        strcpy(users[numUsers].username, username);
        users[numUsers].numBorrowed = 0;
        numUsers++;
        printf("User added successfully.\n");
    } else {
        printf("Maximum number of users reached. Cannot add more users.\n");
    }
}

// Function to display all users
void displayUsers() {
    if (numUsers == 0) {
        printf("No users in the system.\n");
        return;
    }

    printf("Users in the system:\n");
    for (int i = 0; i < numUsers; i++) {
        printf("%d. Username: %s, Borrowed Books: %d\n", i + 1, users[i].username, users[i].numBorrowed);
    }
}

// Function to borrow a book
void borrowBook(int userId, int bookIndex) {
    if (userId < 0 || userId >= numUsers) {
        printf("Invalid user ID.\n");
        return;
    }

    if (bookIndex < 0 || bookIndex >= numBooks) {
        printf("Invalid book index.\n");
        return;
    }

    if (users[userId].numBorrowed < MAX_BORROWED_BOOKS && library[bookIndex].available) {
        users[userId].borrowedBooks[users[userId].numBorrowed] = bookIndex;
        users[userId].numBorrowed++;
        library[bookIndex].available = 0;
        printf("Book borrowed successfully.\n");
    } else {
        printf("Cannot borrow the book. Either user reached the maximum borrowed books limit or the book is not available.\n");
    }
}

// Function to return a book
void returnBook(int userId, int bookIndex) {
    if (userId < 0 || userId >= numUsers) {
        printf("Invalid user ID.\n");
        return;
    }

    if (bookIndex < 0 || bookIndex >= users[userId].numBorrowed) {
        printf("Invalid borrowed book index.\n");
        return;
    }

    int bookToReturn = users[userId].borrowedBooks[bookIndex];

    if (bookToReturn < 0 || bookToReturn >= numBooks) {
        printf("Invalid book index to return.\n");
        return;
    }

    users[userId].numBorrowed--;

    library[bookToReturn].available = 1;

    for (int i = bookIndex; i < users[userId].numBorrowed; i++) {
        users[userId].borrowedBooks[i] = users[userId].borrowedBooks[i + 1];
    }

    printf("Book returned successfully.\n");
}

int main() {
    int choice, userId, bookIndex;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char username[20];

    do {
        printf("\nLibrary Management System Menu:\n");
        printf("1. Add a book\n");
        printf("2. Display all books\n");
        printf("3. Search for a book\n");
        printf("4. Add a user\n");
        printf("5. Display all users\n");
        printf("6. Borrow a book\n");
        printf("7. Return a book\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter book title: ");
                scanf("%s", title);
                printf("Enter book author: ");
                scanf("%s", author);
                addBook(title, author);
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                printf("Enter book title to search: ");
                scanf("%s", title);
                searchBook(title);
                break;
            case 4:
                printf("Enter username: ");
                scanf("%s", username);
                addUser(username);
                break;
            case 5:
                displayUsers();
                break;
            case 6:
                printf("Enter user ID (1-%d): ", numUsers);
                scanf("%d", &userId);
                printf("Enter book index to borrow (1-%d): ", numBooks);
                scanf("%d", &bookIndex);
                borrowBook(userId - 1, bookIndex - 1);
                break;
            case 7:
                printf("Enter user ID (1-%d): ", numUsers);
                scanf("%d", &userId);
                printf("Enter borrowed book index to return (1-%d): ", users[userId - 1].numBorrowed);
                scanf("%d", &bookIndex);
                returnBook(userId - 1, bookIndex - 1);
                break;
            case 8:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 8);

    // Save data to a file before exiting
    FILE *file = fopen("library_data.txt", "w");
    if (file != NULL) {
        // Save book data
        fprintf(file, "Books:\n");
        for (int i = 0; i < numBooks; i++) {
            fprintf(file, "%s|%s|%d\n", library[i].title, library[i].author, library[i].available);
        }

        // Save user data
        fprintf(file, "Users:\n");
        for (int i = 0; i < numUsers; i++) {
            fprintf(file, "%s", users[i].username);
            for (int j = 0; j < users[i].numBorrowed; j++) {
                fprintf(file, "|%d", users[i].borrowedBooks[j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        printf("Data saved to library_data.txt.\n");
    } else {
        printf("Error saving data to file.\n");
    }

    return 0;
}

