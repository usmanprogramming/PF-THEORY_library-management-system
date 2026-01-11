#include <stdio.h>
#include <string.h>

// Book structure
struct Book {
    int id;
    char title[50];
    char author[50];
    float price;
    int quantity;
};

// Global data
struct Book books[100];
int bookCount = 0;

int cart[50];
int cartCount = 0;

// Admin credentials
char adminUser[] = "admin";
char adminPass[] = "1234";

// Function prototypes
void loadBooksFromFile();
void saveBooksToFile();

void visitorPanel();
void adminPanel();

void viewBooks();
void searchBook();
void addToCart();
void removeFromCart();
void viewCart();
void confirmPurchase();

void addBook();
void deleteBook();

int main() {
    int choice;

    // Load books from file
    loadBooksFromFile();

    while (1) {
    	printf("========================================\n");
        printf("\tLibrary Management System\n");
        printf("========================================\n");
        printf("1. Visitor\n");
        printf("2. Admin\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            visitorPanel();
        else if (choice == 2)
            adminPanel();
        else if (choice == 0)
            break;
        else
            printf("Invalid input!\n");
    }

    // Save before exit
    saveBooksToFile();
    return 0;
}

/* ================= FILE HANDLING ================= */

void loadBooksFromFile() {
    FILE *fp = fopen("books.txt", "r");

    if (fp == NULL) {
        printf("books.txt not found. Starting empty library.\n");
        return;
    }

    bookCount = 0;
    while (fscanf(fp, "%d %s %s %f %d",
                  &books[bookCount].id,
                  books[bookCount].title,
                  books[bookCount].author,
                  &books[bookCount].price,
                  &books[bookCount].quantity) != EOF) {
        bookCount++;
    }

    fclose(fp);
}

void saveBooksToFile() {
    FILE *fp = fopen("books.txt", "w");

    if (fp == NULL) {
        printf("Error saving file.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(fp, "%d %s %s %.2f %d\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].price,
                books[i].quantity);
    }

    fclose(fp);
}

/* ================= VISITOR ================= */

void visitorPanel() {
    int choice;

    while (1) {
        printf("\n==== Visitor Panel ====\n");
        printf("1. View books\n");
        printf("2. Search book\n");
        printf("3. Add to cart\n");
        printf("4. Remove from cart\n");
        printf("5. View cart\n");
        printf("6. Confirm purchase\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) viewBooks();
        else if (choice == 2) searchBook();
        else if (choice == 3) addToCart();
        else if (choice == 4) removeFromCart();
        else if (choice == 5) viewCart();
        else if (choice == 6) confirmPurchase();
        else if (choice == 0) break;
        else printf("Invalid choice!\n");
    }
}

void viewBooks() {
    printf("\nID   TITLE           AUTHOR        PRICE   QTY\n");
	printf("------------------------------------------------\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%d\t%s\t%s\t%.2f\t%d\n",
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].price,
               books[i].quantity);
    }
}

void searchBook() {
    char name[50];
    int found = 0;

    printf("Enter title or author: ");
    scanf("%s", name);

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].title, name) == 0 ||
            strcmp(books[i].author, name) == 0) {

            printf("Found: %s by %s (%.2f, Qty: %d)\n",
                   books[i].title,
                   books[i].author,
                   books[i].price,
                   books[i].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("Book not found.\n");
}

void addToCart() {
    int id;

    printf("Enter book ID: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id && books[i].quantity > 0) {
            cart[cartCount++] = id;
            books[i].quantity--;
            saveBooksToFile();
            printf("Added to cart.\n");
            return;
        }
    }
    printf("Book unavailable.\n");
}

void removeFromCart() {
    int id;

    printf("Enter book ID to remove: ");
    scanf("%d", &id);

    for (int i = 0; i < cartCount; i++) {
        if (cart[i] == id) {

            for (int j = 0; j < bookCount; j++)
                if (books[j].id == id)
                    books[j].quantity++;

            for (int j = i; j < cartCount - 1; j++)
                cart[j] = cart[j + 1];

            cartCount--;
            saveBooksToFile();
            printf("Removed from cart.\n");
            return;
        }
    }
    printf("Book not in cart.\n");
}

void viewCart() {
    float total = 0;

    for (int i = 0; i < cartCount; i++) {
        for (int j = 0; j < bookCount; j++) {
            if (books[j].id == cart[i]) {
                printf("%s - %.2f\n",
                       books[j].title,
                       books[j].price);
                total += books[j].price;
            }
        }
    }
    printf("Total: %.2f\n", total);
}

void confirmPurchase() {
    if (cartCount == 0) {
        printf("Cart is empty.\n");
        return;
    }

    viewCart();
    cartCount = 0;
    printf("Purchase confirmed.\n");
}

/* ================= ADMIN ================= */

void adminPanel() {
    char user[20], pass[20];
    int choice;

    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, adminUser) != 0 || strcmp(pass, adminPass) != 0) {
        printf("Wrong credentials.\n");
        return;
    }

    while (1) {
        printf("\n--- Admin Panel ---\n");
        printf("1. Add book\n");
        printf("2. Delete book\n");
        printf("3. View books\n");
        printf("0. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) addBook();
        else if (choice == 2) deleteBook();
        else if (choice == 3) viewBooks();
        else if (choice == 0) break;
    }
}

void addBook() {
    struct Book b;

    b.id = bookCount + 1;
    printf("Title: ");
    scanf("%s", b.title);
    printf("Author: ");
    scanf("%s", b.author);
    printf("Price: ");
    scanf("%f", &b.price);
    printf("Quantity: ");
    scanf("%d", &b.quantity);

    books[bookCount++] = b;
    saveBooksToFile();
    printf("Book added.\n");
}

void deleteBook() {
    int id;

    printf("Enter book ID: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            for (int j = i; j < bookCount - 1; j++)
                books[j] = books[j + 1];

            bookCount--;
            saveBooksToFile();
            printf("Book deleted.\n");
            return;
        }
    }
    printf("Book not found.\n");
}