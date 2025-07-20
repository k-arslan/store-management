#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Structures
struct Product {
    int id;
    string name;
    string category;
    double price;
};

struct User {
    string username;
    string password;
};

// Globals
const int MAX_PRODUCTS = 100;
Product products[MAX_PRODUCTS];
int productCount = 0;

// Function declarations
void loadProducts();
void saveProducts();
void addProduct();
void viewProducts();
void deleteProduct();
void registerUser();
bool loginUser();
void shoppingCart();
void searchProducts();

int main() {
    int choice;
    loadProducts();
    //this is the E-commerce store 

    cout << "\nWelcome to E-Commerce Store!\n";
    cout << "1. Register\n2. Login\nChoose an option: ";
    cin >> choice;

    if (choice == 1) {
        registerUser();
    }

    if (loginUser()) {
        do {
            cout << "\n--- Main Menu ---\n";
            cout << "1. Add Product\n";
            cout << "2. View Products\n";
            cout << "3. Delete Product\n";
            cout << "4. Shopping Cart\n";
            cout << "5. Search Products\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addProduct(); break;
                case 2: viewProducts(); break;
                case 3: deleteProduct(); break;
                case 4: shoppingCart(); break;
                case 5: searchProducts(); break;
                case 0: 
                    cout << "Exiting...\n"; 
                    saveProducts(); 
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 0);
    }

    return 0;
}

void loadProducts() {
    ifstream file("products.txt");
    productCount = 0;

    while (file >> products[productCount].id) {
        file.ignore();
        getline(file, products[productCount].name);
        getline(file, products[productCount].category);
        file >> products[productCount].price;
        file.ignore();
        productCount++;
        if(productCount >= MAX_PRODUCTS) break; // avoid overflow
    }
    file.close();
}

void saveProducts() {
    ofstream file("products.txt");
    for (int i = 0; i < productCount; i++) {
        file << products[i].id << "\n";
        file << products[i].name << "\n";
        file << products[i].category << "\n";
        file << products[i].price << "\n";
    }
    file.close();
}

void addProduct() {
    if(productCount >= MAX_PRODUCTS) {
        cout << "Cannot add more products, storage full.\n";
        return;
    }
    cout << "\nEnter Product ID: ";
    cin >> products[productCount].id;
    cin.ignore();
    cout << "Enter Product Name: ";
    getline(cin, products[productCount].name);
    cout << "Enter Product Category: ";
    getline(cin, products[productCount].category);
    cout << "Enter Product Price: ";
    cin >> products[productCount].price;
    productCount++;
    saveProducts();
    cout << "Product Added Successfully!\n";
}

void viewProducts() {
    cout << "\n--- Product List ---\n";
    if(productCount == 0) {
        cout << "No products available.\n";
        return;
    }
    for (int i = 0; i < productCount; i++) {
        cout << "ID: " << products[i].id
             << ", Name: " << products[i].name
             << ", Category: " << products[i].category
             << ", Price: " << products[i].price << "\n";
    }
}

void deleteProduct() {
    int id;
    cout << "\nEnter Product ID to Delete: ";
    cin >> id;
    bool found = false;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            for (int j = i; j < productCount - 1; j++) {
                products[j] = products[j + 1];
            }
            productCount--;
            saveProducts();
            cout << "Product Deleted!\n";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Product Not Found!\n";
    }
}

void registerUser() {
    User newUser;
    cout << "\n--- User Registration ---\n";
    cout << "Enter Username: ";
    cin >> newUser.username;
    cout << "Enter Password: ";
    cin >> newUser.password;

    ofstream file("users.txt", ios::app);
    file << newUser.username << "\n" << newUser.password << "\n";
    file.close();

    cout << "Registration Successful!\n";
}

bool loginUser() {
    User login;
    cout << "\n--- User Login ---\n";
    cout << "Enter Username: ";
    cin >> login.username;
    cout << "Enter Password: ";
    cin >> login.password;

    ifstream file("users.txt");
    User temp;
    while (getline(file, temp.username)) {
        getline(file, temp.password);
        if (temp.username == login.username && temp.password == login.password) {
            cout << "Login Successful!\n";
            file.close();
            return true;
        }
    }
    file.close();
    cout << "Login Failed!\n";
    return false;
}

void shoppingCart() {
    int cart[10], cartCount = 0, id;
    char choice = 'y';

    while ((choice == 'y' || choice == 'Y') && cartCount < 10) {
        viewProducts();
        cout << "\nEnter Product ID to add to cart: ";
        cin >> id;

        bool found = false;
        for(int i = 0; i < productCount; i++) {
            if(products[i].id == id) {
                found = true;
                break;
            }
        }

        if(!found) {
            cout << "Product ID not found.\n";
            continue;
        }

        cart[cartCount++] = id;
        if(cartCount == 10) {
            cout << "Cart full.\n";
            break;
        }

        cout << "Add more? (y/n): ";
        cin >> choice;
    }

    double total = 0;
    ofstream file("orders.txt", ios::app);
    file << "Order:\n";
    for (int i = 0; i < cartCount; i++) {
        for (int j = 0; j < productCount; j++) {
            if (products[j].id == cart[i]) {
                cout << "Added: " << products[j].name << " - " << products[j].price << "\n";
                file << products[j].name << " - " << products[j].price << "\n";
                total += products[j].price;
            }
        }
    }
    cout << "Total Cost: " << total << "\n";
    file << "Total: " << total << "\n\n";
    file.close();
}

void searchProducts() {
    string searchTerm;
    cout << "\nEnter Product Name or Category to Search: ";
    cin.ignore();
    getline(cin, searchTerm);

    bool foundAny = false;
    for (int i = 0; i < productCount; i++) {
        // Case insensitive search would be better, but for simplicity, just substring match:
        if (products[i].name.find(searchTerm) != string::npos ||
            products[i].category.find(searchTerm) != string::npos) {
            cout << "ID: " << products[i].id
                 << ", Name: " << products[i].name
                 << ", Category: " << products[i].category
                 << ", Price: " << products[i].price << "\n";
            foundAny = true;
        }
    }
    if(!foundAny) {
        cout << "No products found matching '" << searchTerm << "'.\n";
    }
}
