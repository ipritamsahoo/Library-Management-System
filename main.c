#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_TITLE_LENGTH 100

// Structure to store book details
typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_TITLE_LENGTH];
    int isIssued; // 1 if issued, 0 otherwise
} Book;

// Structure to store user details
typedef struct {
    int id;
    char name[MAX_TITLE_LENGTH];
    int issuedBookId; // -1 if no book issued
} User;

Book books[MAX_BOOKS];
User users[MAX_USERS];
int totalBooks = 0;
int totalUsers = 0;

// Function prototypes
void loadBooks();
void saveBooks();
void addBook();
void viewBooks();
void issueBook();
void returnBook();
void addUser();
void viewUsers();
int findBookById(int id);
int findUserById(int id);

int main() {
    int choice;

    // Load existing data from files
    loadBooks();

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Add User\n");
        printf("6. View Users\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: addUser(); break;
            case 6: viewUsers(); break;
            case 7: 
                saveBooks();
                exit(0); 
            default: printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Load books from file
void loadBooks() {
    FILE *file = fopen("books.dat", "rb");
    if (file == NULL) {
        printf("No book data found. Starting fresh.\n");
        return;
    }

    fread(&totalBooks, sizeof(int), 1, file);
    fread(books, sizeof(Book), totalBooks, file);
    fclose(file);
}

// Save books to file
void saveBooks() {
    FILE *file = fopen("books.dat", "wb");
    if (file == NULL) {
        printf("Error saving book data.\n");
        return;
    }

    fwrite(&totalBooks, sizeof(int), 1, file);
    fwrite(books, sizeof(Book), totalBooks, file);
    fclose(file);
}

// Add a new book to the library
void addBook() {
    if (totalBooks >= MAX_BOOKS) {
        printf("Library is full, cannot add more books.\n");
        return;
    }

    Book newBook;
    newBook.id = totalBooks + 1;
    newBook.isIssued = 0;

    printf("Enter book title: ");
    getchar(); // Clear newline character
    fgets(newBook.title, MAX_TITLE_LENGTH, stdin);
    newBook.title[strlen(newBook.title) - 1] = '\0'; // Remove newline character

    printf("Enter author name: ");
    fgets(newBook.author, MAX_TITLE_LENGTH, stdin);
    newBook.author[strlen(newBook.author) - 1] = '\0'; // Remove newline character

    books[totalBooks] = newBook;
    totalBooks++;

    printf("Book added successfully.\n");
}

// View all books in the library
void viewBooks() {
    if (totalBooks == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\nID\tTitle\t\tAuthor\t\tStatus\n");
    for (int i = 0; i < totalBooks; i++) {
        printf("%d\t%s\t\t%s\t\t%s\n", books[i].id, books[i].title, books[i].author,
               books[i].isIssued ? "Issued" : "Available");
    }
}

// Issue a book to a user
void issueBook() {
    int bookId, userId;
    printf("Enter user ID: ");
    scanf("%d", &userId);
    printf("Enter book ID to issue: ");
    scanf("%d", &bookId);

    int bookIndex = findBookById(bookId);
    int userIndex = findUserById(userId);

    if (bookIndex == -1) {
        printf("Book not found.\n");
        return;
    }

    if (userIndex == -1) {
        printf("User not found.\n");
        return;
    }

    if (books[bookIndex].isIssued) {
        printf("Book is already issued.\n");
        return;
    }

    books[bookIndex].isIssued = 1;
    users[userIndex].issuedBookId = bookId;
    printf("Book issued successfully.\n");
}

// Return a book
void returnBook() {
    int bookId, userId;
    printf("Enter user ID: ");
    scanf("%d", &userId);
    printf("Enter book ID to return: ");
    scanf("%d", &bookId);

    int bookIndex = findBookById(bookId);
    int userIndex = findUserById(userId);

    if (bookIndex == -1) {
        printf("Book not found.\n");
        return;
    }

    if (userIndex == -1) {
        printf("User not found.\n");
        return;
    }

    if (users[userIndex].issuedBookId != bookId) {
        printf("This user did not issue this book.\n");
        return;
    }

    books[bookIndex].isIssued = 0;
    users[userIndex].issuedBookId = -1;
    printf("Book returned successfully.\n");
}

// Add a new user to the system
void addUser() {
    if (totalUsers >= MAX_USERS) {
        printf("Maximum users reached. Cannot add more users.\n");
        return;
    }

    User newUser;
    newUser.id = totalUsers + 1;
    newUser.issuedBookId = -1;

    printf("Enter user name: ");
    getchar(); // Clear newline character
    fgets(newUser.name, MAX_TITLE_LENGTH, stdin);
    newUser.name[strlen(newUser.name) - 1] = '\0'; // Remove newline character

    users[totalUsers] = newUser;
    totalUsers++;

    printf("User added successfully.\n");
}

// View all users
void viewUsers() {
    if (totalUsers == 0) {
        printf("No users registered.\n");
        return;
    }

    printf("\nID\tName\t\tIssued Book\n");
    for (int i = 0; i < totalUsers; i++) {
        printf("%d\t%s\t\t%d\n", users[i].id, users[i].name, users[i].issuedBookId);
    }
}

// Find book by ID
int findBookById(int id) {
    for (int i = 0; i < totalBooks; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1; // Book not found
}

// Find user by ID
int findUserById(int id) {
    for (int i = 0; i < totalUsers; i++) {
        if (users[i].id == id) {
            return i;
        }
    }
    return -1; // User not found
}