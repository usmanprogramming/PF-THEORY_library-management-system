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
    loadBooksFromFile(); // Load once at startup

    while (1) {
        printf("\n========================================\n");
        printf("\tLibrary Management System\n");
        printf("========================================\n");
        printf("1. Visitor\n");
        printf("2. Admin\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Please enter a valid number.\n");
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        if (choice == 1) visitorPanel();
        else if (choice == 2) adminPanel();
        else if (choice == 0) break;
        else printf("Invalid input!\n");
    }

    saveBooksToFile(); // Final save
    return 0;
}

/* ================= FILE HANDLING ================= */

void loadBooksFromFile() {
    FILE *fp = fopen("books.txt", "r");
    if (fp == NULL) return;

    bookCount = 0;
    // Uses pipe delimiter to handle spaces in titles
    while (fscanf(fp, "%d|%[^|]|%[^|]|%f|%d\n",
                  &books[bookCount].id,
                  books[bookCount].title,
                  books[bookCount].author,
                  &books[bookCount].price,
                  &books[bookCount].quantity) == 5) {
        bookCount++;
        if (bookCount >= 100) break;
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
        fprintf(fp, "%d|%s|%s|%.2f|%d\n",
                books[i].id, books[i].title, books[i].author,
                books[i].price, books[i].quantity);
    }
    fclose(fp);
}

/* ================= VISITOR PANEL ================= */

void visitorPanel() {
    int choice;
    while (1) {
        printf("\n==== Visitor Panel ====\n");
        printf("1. View books\n2. Search book\n3. Add to cart\n4. Remove from cart\n5. View cart\n6. Confirm purchase\n0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) viewBooks();
        else if (choice == 2) searchBook();
        else if (choice == 3) addToCart();
        else if (choice == 4) removeFromCart();
        else if (choice == 5) viewCart();
        else if (choice == 6) confirmPurchase();
        else if (choice == 0) break;
    }
}

void viewBooks() {
    if (bookCount == 0) {
        printf("\nLibrary is empty.\n");
        return;
    }
    printf("\nID\tTITLE\t\t\tAUTHOR\t\t\tPRICE\tQTY\n");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%d\t%-20s\t%-20s\t%.2f\t%d\n",
               books[i].id, books[i].title, books[i].author,
               books[i].price, books[i].quantity);
    }
}

void searchBook() {
    char name[50];
    int found = 0;
    printf("Enter title or author: ");
    getchar(); // Clear buffer
    scanf("%[^\n]", name);

    for (int i = 0; i < bookCount; i++) {
        if (strstr(books[i].title, name) || strstr(books[i].author, name)) {
            printf("Found: ID %d | %s by %s ($%.2f, Qty: %d)\n",
                   books[i].id, books[i].title, books[i].author, books[i].price, books[i].quantity);
            found = 1;
        }
    }
    if (!found) printf("Book not found.\n");
}

void addToCart() {
    if (cartCount >= 50) {
        printf("Your cart is full (Limit: 50).\n");
        return;
    }
    // ... rest of your existing logic
}

void removeFromCart() {
    int id;
    printf("Enter book ID to remove: ");
    scanf("%d", &id);
    for (int i = 0; i < cartCount; i++) {
        if (cart[i] == id) {
            for (int j = 0; j < bookCount; j++)
                if (books[j].id == id) books[j].quantity++;
            for (int j = i; j < cartCount - 1; j++) cart[j] = cart[j + 1];
            cartCount--;
            printf("Removed from cart.\n");
            return;
        }
    }
    printf("Book not in cart.\n");
}

void viewCart() {
    float total = 0;
    if (cartCount == 0) { printf("Cart is empty.\n"); return; }
    for (int i = 0; i < cartCount; i++) {
        for (int j = 0; j < bookCount; j++) {
            if (books[j].id == cart[i]) {
                printf("%s - $%.2f\n", books[j].title, books[j].price);
                total += books[j].price;
            }
        }
    }
    printf("Total: $%.2f\n", total);
}

void confirmPurchase() {
    if (cartCount == 0) { printf("Cart is empty.\n"); return; }
    viewCart();
    cartCount = 0;
    saveBooksToFile(); // Save changes to stock
    printf("Purchase confirmed! Thank you.\n");
}

/* ================= ADMIN PANEL ================= */

void adminPanel() {
    char user[20], pass[20];
    printf("Username: "); scanf("%s", user);
    printf("Password: "); scanf("%s", pass);

    if (strcmp(user, adminUser) != 0 || strcmp(pass, adminPass) != 0) {
        printf("Wrong credentials.\n");
        return;
    }

    // --- Low Stock Alert ---
    int lowStockFound = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].quantity < 3) {
            if (!lowStockFound) printf("\n[!] LOW STOCK ALERT:\n");
            printf(" - %s (Only %d left!)\n", books[i].title, books[i].quantity);
            lowStockFound = 1;
        }
    }
    // -----------------------

    int choice;
    while (1) {
        printf("\n--- Admin Panel ---\n1. Add book\n2. Delete book\n3. View books\n0. Logout\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) addBook();
        else if (choice == 2) deleteBook();
        else if (choice == 3) viewBooks();
        else if (choice == 0) break;
    }
}

void addBook() {
    struct Book b;
    b.id = (bookCount == 0) ? 1 : books[bookCount - 1].id + 1;
    printf("Title: "); getchar(); scanf("%[^\n]", b.title);
    printf("Author: "); getchar(); scanf("%[^\n]", b.author);
    printf("Price: "); scanf("%f", &b.price);
    printf("Quantity: "); scanf("%d", &b.quantity);
    books[bookCount++] = b;
    saveBooksToFile();
    printf("Book added successfully!\n");
}

void deleteBook() {
    int id;
    printf("Enter book ID: ");
    scanf("%d", &id);
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            for (int j = i; j < bookCount - 1; j++) books[j] = books[j + 1];
            bookCount--;
            saveBooksToFile();
            printf("Book deleted.\n");
            return;
        }
    }
    printf("Book not found.\n");
}