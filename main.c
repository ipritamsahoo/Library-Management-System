#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 100
#define MAX_PHONE_LENGTH 100 
#define MAX_GENRE_LENGTH 100

// Admin credentials
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin@123"

// Structure to store book details
typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_TITLE_LENGTH];
    char genre[MAX_GENRE_LENGTH];  // New field to store the genre of the book
    int isIssued; // 1 if issued, 0 otherwise
    float issueCost;
} Book;

// Structure to store user details
typedef struct {
    int id;
    char name[MAX_TITLE_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH]; // New field for phone number
    int issuedBookId; // -1 if no book issued
} User;

// Dynamic arrays for books and users
Book* books = NULL;
User* users = NULL;
int totalBooks = 0;
int totalUsers = 0;
int bookCapacity = 10; // Initial capacity for books
int userCapacity = 10; // Initial capacity for users

// Function prototypes
int adminLogin();
void loadBooks();
void loadUsers();
void saveBooks();
void saveUsers();
void addBook();
void viewBooks();
void issueBook();
void returnBook();
void addUser();
void viewUsers();
void removeUserById(int id);
void removeBookById(int id);
int findBookById(int id);
int findUserById(int id);
void editBookById(int id);
void editUserById(int id);
void increaseBookCapacity();
void increaseUserCapacity();

int main() {
    int choice, id;

    // Initialize dynamic arrays
    books = (Book*) malloc(bookCapacity * sizeof(Book));
    users = (User*) malloc(userCapacity * sizeof(User));

    // Call the admin login function
    if (!adminLogin()) {
        printf("Incorrect login credentials. Access denied.\n");
        return 0; // Terminate the program if login fails
    }

    // Load existing data from files
    loadBooks();
    loadUsers();

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Add User\n");
        printf("6. View Users\n");
        printf("7. Remove User\n");
        printf("8. Remove Book\n");
        printf("9. Edit User\n");
        printf("10. Edit Book\n");
        printf("11. Exit\n");
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
            printf("Enter User ID to remove: ");
            scanf("%d", &id);
            removeUserById(id);
            break;
        case 8:
            printf("Enter Book ID to remove: ");
            scanf("%d", &id);
            removeBookById(id);
            break;
        case 9:
                printf("Enter User ID to edit: ");
                scanf("%d", &id);
                editUserById(id);
                break;
            case 10:
                printf("Enter Book ID to edit: ");
                scanf("%d", &id);
                editBookById(id);
                break;
            case 11: 
                saveBooks();
                saveUsers();
                 // Free allocated memory
                free(books);
                free(users);
                exit(0); 
            default: printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Admin login function
int adminLogin() {
    char username[MAX_TITLE_LENGTH];
    char password[MAX_TITLE_LENGTH];

    printf("Admin Login\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Login successful. Welcome, Admin!\n");
        return 1; // Login successful
    } else {
        return 0; // Login failed
    }
}

// Increase capacity for books if needed
void increaseBookCapacity() {
    bookCapacity *= 2;
    books = (Book*) realloc(books, bookCapacity * sizeof(Book));
    if (!books) {
        printf("Memory allocation failed for books.\n");
        exit(1); // Exit if memory allocation fails
    }
}

// Increase capacity for users if needed
void increaseUserCapacity() {
    userCapacity *= 2;
    users = (User*) realloc(users, userCapacity * sizeof(User));
    if (!users) {
        printf("Memory allocation failed for users.\n");
        exit(1); // Exit if memory allocation fails
    }
}

// Load books from file
void loadBooks() {
    FILE *file = fopen("books.txt", "rb");
    if (file == NULL) {
        printf("No book data found. Starting fresh.\n");
        return;
    }

    fread(&totalBooks, sizeof(int), 1, file);
    fread(books, sizeof(Book), totalBooks, file);
    fclose(file);
}

// Load users from file
void loadUsers() {
    FILE *file = fopen("users.txt", "rb");
    if (file == NULL) {
        printf("No user data found. Starting fresh.\n");
        return;
    }

    fread(&totalUsers, sizeof(int), 1, file);
    fread(users, sizeof(User), totalUsers, file);
    fclose(file);
}

// Save books to file
void saveBooks() {
    FILE *file = fopen("books.txt", "wb");
    if (file == NULL) {
        printf("Error saving book data.\n");
        return;
    }

    fwrite(&totalBooks, sizeof(int), 1, file);
    fwrite(books, sizeof(Book), totalBooks, file);
    fclose(file);
}

// Save users to file
void saveUsers() {
    FILE *file = fopen("users.txt", "wb");
    if (file == NULL) {
        printf("Error saving user data.\n");
        return;
    }

    fwrite(&totalUsers, sizeof(int), 1, file);
    fwrite(users, sizeof(User), totalUsers, file);
    fclose(file);
}

// Add a new book to the library
void addBook() {
    if (totalBooks >= bookCapacity) {
        increaseBookCapacity(); // Increase capacity if limit is reached
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

    printf("Enter book genre: ");
    fgets(newBook.genre, MAX_GENRE_LENGTH, stdin);
    newBook.genre[strlen(newBook.genre) - 1] = '\0'; // Remove newline character

    printf("Enter issue cost for the book: ");
    scanf("%f", &newBook.issueCost);

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

    printf("\nID\tTitle\t\tAuthor\t\tGenre\t\tCost\t\tStatus\n");
    for (int i = 0; i < totalBooks; i++) {
        printf("%d\t%s\t\t%s\t%s\t%.2f\t\t%s\n", books[i].id, books[i].title, books[i].author, books[i].genre, books[i].issueCost,
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
    if (totalUsers >= userCapacity) {
        increaseUserCapacity(); // Increase capacity if limit is reached
    }

    User newUser;
    newUser.id = totalUsers + 1;
    newUser.issuedBookId = -1;

    printf("Enter user name: ");
    getchar(); // Clear newline character
    fgets(newUser.name, MAX_TITLE_LENGTH, stdin);
    newUser.name[strlen(newUser.name) - 1] = '\0'; // Remove newline character

    printf("Enter user phone number: ");
    fgets(newUser.phoneNumber, MAX_PHONE_LENGTH, stdin);
    newUser.phoneNumber[strlen(newUser.phoneNumber) - 1] = '\0'; // Remove newline character

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

    printf("\nID\tName\t\tPhone Number\t\tIssued Book\n");
    for (int i = 0; i < totalUsers; i++) {
        printf("%d\t%s\t\t%s\t\t%d\n", users[i].id, users[i].name, users[i].phoneNumber, users[i].issuedBookId);
    }
}

// Remove user by ID
void removeUserById(int id)
{
    int index = findUserById(id);

    if (index == -1)
    {
        printf("User not found.\n");
        return;
    }

    for (int i = index; i < totalUsers - 1; i++)
    {
        users[i] = users[i + 1]; // Shift all users after the one to remove
    }

    totalUsers--;
    printf("User removed successfully.\n");
}

// Remove book by ID
void removeBookById(int id)
{
    int index = findBookById(id);

    if (index == -1)
    {
        printf("Book not found.\n");
        return;
    }

    for (int i = index; i < totalBooks - 1; i++)
    {
        books[i] = books[i + 1]; // Shift all books after the one to remove
    }

    totalBooks--;
    printf("Book removed successfully.\n");
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

// Edit book by ID
void editBookById(int id) {
    int index = findBookById(id);

    if (index == -1) {
        printf("Book not found.\n");
        return;
    }

    printf("Enter new book title: ");
    getchar(); // Clear newline character
    fgets(books[index].title, MAX_TITLE_LENGTH, stdin);
    books[index].title[strlen(books[index].title) - 1] = '\0'; // Remove newline character

    printf("Enter new author name: ");
    fgets(books[index].author, MAX_TITLE_LENGTH, stdin);
    books[index].author[strlen(books[index].author) - 1] = '\0'; // Remove newline character

    printf("Enter new book genre: ");
    fgets(books[index].genre, MAX_TITLE_LENGTH, stdin);
    books[index].genre[strlen(books[index].genre) - 1] = '\0'; // Remove newline character

    printf("Enter issue cost for the book: ");
    scanf("%f", &books[index].issueCost);

    printf("Book details updated successfully.\n");
}

// Edit user by ID
void editUserById(int id) {
    int index = findUserById(id);

    if (index == -1) {
        printf("User not found.\n");
        return;
    }

    printf("Enter new user name: ");
    getchar(); // Clear newline character
    fgets(users[index].name, MAX_TITLE_LENGTH, stdin);
    users[index].name[strlen(users[index].name) - 1] = '\0'; // Remove newline character

    printf("Enter new phone number: ");
    fgets(users[index].phoneNumber, MAX_PHONE_LENGTH, stdin);
    users[index].phoneNumber[strlen(users[index].phoneNumber) - 1] = '\0'; // Remove newline character
    
    printf("User details updated successfully.\n");
}
