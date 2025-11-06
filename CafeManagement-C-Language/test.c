#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PASSWORD_FILE "admin_password.txt"
#define MENU_FILE "menu.txt"
#define ORDER_FILE "orders.txt"
#define SALES_REPORT_FILE "sales_report.txt"
#define MAX_MENU_ITEMS 100

struct MenuItem {
    char itemName[50];
    float itemPrice;
    int stock;
};

void passchange() {
    FILE *file = fopen(PASSWORD_FILE, "w");

    if (file == NULL) {
        perror("Error opening admin password file");
        exit(0);
    }

    char array[50];
    printf("Write new pass: ");

    fflush(stdin);//to fix an undefined behavior
    gets(array);

    int i=0;
    while(array[i]!='\0')
    {
    if(array[i]==' ')
    {
        printf("Error, your password cannot contain space.\nTry again,\n");
        passchange();
    }
    else
        fprintf(file, "%c", array[i]);
    i++;
    }
    fclose(file);

    printf("Thank you. Your new password set successfully.\n");
}

// Function to read admin password from file
void readpass(char password[]) {
    FILE *file = fopen(PASSWORD_FILE, "r");

    if (file == NULL) {
        perror("Error opening admin password file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%s", password);

    fclose(file);
}
// Function to log in as an admin
int adminlog() {
    char enteredPassword[10];
    char correctPassword[10];

    // Read the correct admin password from the file
    readpass(correctPassword);

    printf("Admin Password: ");

    int i = 0;
    while (1) {
        char ch = getch();

        if (ch == '\r' || ch == '\n') {
            break;  // Enter key = terminate the loop
        } else if (ch == '\b' && i >=0) {
            if(i==0){
                    printf("*");
              printf("\b \b");
            }
            else{
                printf("\b \b");
            i--;}
        } else {
            enteredPassword[i] = ch;
            i++;
            printf("*");
        }
    }

    enteredPassword[i] = '\0';

    // Compare the entered password with the correct password
    if (strcmp(enteredPassword, correctPassword) == 0) {
        printf("\nAdmin login successful.\n");
        return 1;
    } else {
        printf("\nInvalid admin password. Access denied.\n");
        printf("1.Try again\n2.Back\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
    case 1:
        adminlog();
        break;
    case 2:
        return 0;
        break;
    default:
        printf("Invalid choice.\n");

        }

    }
}

// Function to read menu items from file
void readmenu(struct MenuItem menu[], int *itemCount) {
    FILE *file = fopen(MENU_FILE, "r");

    if (file == NULL) {
        perror("Error opening menu file");
        exit(EXIT_FAILURE);
    }

    *itemCount = 0;

    while (fscanf(file, "%s %f %d", menu[*itemCount].itemName, &menu[*itemCount].itemPrice, &menu[*itemCount].stock) != EOF)
        {
        (*itemCount)++;
        }

    fclose(file);
}

void writeMenu(const struct MenuItem menu[], int itemCount) {
    FILE *file = fopen(MENU_FILE, "w");

    if (file == NULL) {
        perror("Error opening menu file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%s %.2f %d\n", menu[i].itemName, menu[i].itemPrice, menu[i].stock);
    }

    fclose(file);
}

// Function to update the menu
void updateMenu() {
    int itemCount;
    struct MenuItem menu[MAX_MENU_ITEMS];

    readmenu(menu, &itemCount);

    printf("Current Menu:\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%d. %s - %.2f Taka - Stock: %d\n", i + 1, menu[i].itemName, menu[i].itemPrice, menu[i].stock);
    }

    int choice;
    printf("\nMenu Update Options:\n");

    printf("1. Add Item\n");
    printf("2. Delete Item\n");
    printf("3. Change Item Price\n");
    printf("4. Add Stock\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {

        case 1:
            // Add Item
            if (itemCount < MAX_MENU_ITEMS) {
                printf("Enter the name of the new item: ");
                scanf("%s", menu[itemCount].itemName);

                printf("Enter the price of the new item: ");
                scanf("%f", &menu[itemCount].itemPrice);

                printf("Enter the initial stock of the new item: ");
                scanf("%d", &menu[itemCount].stock);

                itemCount++;
                writeMenu(menu, itemCount);

                printf("Item added to the menu.\n");
            } else {
                printf("Cannot add more items. Menu is full.\n");
            }
            break;
        case 2:
            // Delete Item
            if (itemCount > 0) {
                printf("Enter the number of the item to delete: ");
                int itemNumberDelete;
                scanf("%d", &itemNumberDelete);

                if (itemNumberDelete >= 1 && itemNumberDelete <= itemCount) {
                    for (int i = itemNumberDelete - 1; i < itemCount - 1; i++) {
                        menu[i] = menu[i + 1];
                    }
                    itemCount--;
                    writeMenu(menu, itemCount);

                    printf("Item deleted from the menu.\n");
                } else {
                    printf("Invalid item number. Please enter a valid item number.\n");
                }
            } else {
                printf("Cannot delete items. Menu is empty.\n");
            }
            break;
        case 3:
            // Change Item Price
            printf("Enter the number of the item to change the price: ");
            int itemNumberPrice;
            scanf("%d", &itemNumberPrice);

            if (itemNumberPrice >= 1 && itemNumberPrice <= itemCount) {
                printf("Enter the new price for %s: ", menu[itemNumberPrice - 1].itemName);
                float newPrice;
                scanf("%f", &newPrice);

                if (newPrice >= 0) {
                    menu[itemNumberPrice - 1].itemPrice = newPrice;
                    writeMenu(menu, itemCount);

                    printf("Price changed successfully.\n");
                } else {
                    printf("Invalid price. Please enter a non-negative price.\n");
                }
            } else {
                printf("Invalid item number. Please enter a valid item number.\n");
            }
            break;
        case 4:
            // Add Stock
            printf("Enter the number of the item to add stock: ");
            int itemNumber;
            scanf("%d", &itemNumber);

            if (itemNumber >= 1 && itemNumber <= itemCount) {
                printf("Enter the quantity to add to the stock for %s: ", menu[itemNumber - 1].itemName);
                int quantity;
                scanf("%d", &quantity);

                if (quantity > 0) {
                    menu[itemNumber - 1].stock += quantity;
                    writeMenu(menu, itemCount);

                    printf("Stock added successfully.\n");
                } else {
                    printf("Invalid quantity. Please enter a quantity greater than 0.\n");
                }
            } else {
                printf("Invalid item number. Please enter a valid item number.\n");
            }
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void displayStock() {
    int itemCount;
    struct MenuItem menu[MAX_MENU_ITEMS];

    readmenu(menu, &itemCount);

    printf("Current Stock:\n");
    for (int i = 0; i < itemCount; i++) {
        printf("%s - Stock: %d\n", menu[i].itemName, menu[i].stock);
    }
}

// Function to take order
void takeOrder() {
    int itemCount;
    struct MenuItem menu[MAX_MENU_ITEMS];

    readmenu(menu, &itemCount);

    printf("Available Menu:\n");
    int s=0;
    int i;
    for (i = 0; i < itemCount; i++) {
        if (menu[i].stock > 0) {
            s++;
            printf("%d. %s - %.2f Taka - Stock: %d\n", s, menu[i].itemName, menu[i].itemPrice, menu[i].stock);

        }
    }
    int temp=i-s;
    printf("\n\nHow many items do you want to buy?\n=>");
    int itemsToBuy;
    scanf("%d", &itemsToBuy);

    int orderCount = 0;
    float totalBill = 0;

    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    FILE *orderFile = fopen(ORDER_FILE, "a");

    if (orderFile == NULL) {
        perror("Error opening order file");
        exit(EXIT_FAILURE);
    }

    fprintf(orderFile, "-----\n");
    fprintf(orderFile, "Order Time: %04d-%02d-%02d %02d:%02d:%02d\n",
            localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    for (int i = 0; i < itemsToBuy; i++) {
        printf("Enter the number of the item you want to order: ");
        int itemNumber;
        scanf("%d", &itemNumber);

        if (itemNumber >= 1 && itemNumber <=s ) {
            printf("Enter the quantity: ");
            int quantity;
            scanf("%d", &quantity);

            if (quantity > 0) {
                if (quantity <= menu[itemNumber+temp - 1].stock) {
                    float subtotal = menu[itemNumber+temp - 1].itemPrice * quantity;

                    fprintf(orderFile, "%s - Quantity: %d - Subtotal: %.2f Taka\n",
                            menu[itemNumber+temp - 1].itemName, quantity, subtotal);

                    menu[itemNumber+temp - 1].stock -= quantity;
                    writeMenu(menu, itemCount);

                    totalBill += subtotal;
                    orderCount++;

                    printf("%s Price in subtotal : %.2f Taka\n", menu[itemNumber+temp - 1].itemName, subtotal);
                } else {
                    printf("Insufficient stock. Please choose a quantity less than or equal to %d.\n", menu[itemNumber+temp - 1].stock);
                    i--;
                }
            } else {
                printf("Invalid quantity. Please enter a quantity greater than 0.\n");
                i--;
            }
        } else {
            printf("Invalid item number or item out of stock. Please enter a valid and available item number.\n");
            i--;
        }
    }

    fprintf(orderFile, "=========================================\n");

    fclose(orderFile);

    printf("Order Summary:\n");
    printf("Number of items ordered: %d\n", orderCount);
    printf("Total Bill: %.2f Taka\n", totalBill);
}

// Function to generate SalesReport
void generateSalesReport() {
    FILE *orderFile = fopen(ORDER_FILE, "r");
    FILE *salesReportFile = fopen(SALES_REPORT_FILE, "a");

    if (orderFile == NULL || salesReportFile == NULL) {
        perror("Error opening order or sales report file");
        exit(EXIT_FAILURE);
    }

    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), orderFile) != NULL) {
        fputs(buffer, salesReportFile);
    }

    fprintf(salesReportFile, "=========================================\n");

    fclose(orderFile);
    fclose(salesReportFile);

    printf("Sales report generated.\n");
}
// Function to view SalesReport
void viewsale() {
    FILE *salesReportFile = fopen(SALES_REPORT_FILE, "r");

    if (salesReportFile == NULL) {
        perror("Error opening sales report file");
        exit(EXIT_FAILURE);
    }

    printf("\nSales Report:\n");
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), salesReportFile) != NULL) {
        // Check if the line contains the "Order Time" string
        if (strstr(buffer, "Order Time") != NULL) {
            printf("%s", buffer); // Print the date and time line
        } else {
            printf("%s", buffer); // Print the rest of the lines
        }
    }

    fclose(salesReportFile);
}

// Function to clear SalesReport
void clearsalesreport() {
    FILE *salesReportFile = fopen(SALES_REPORT_FILE, "w");

    if (salesReportFile == NULL) {
        perror("Error opening sales report file");
        exit(EXIT_FAILURE);
    }

    fclose(salesReportFile);
}

int main() {
    int choice;
    int login = 0;

    while (1) {
        if (login==0) {
            printf("\nWelcome to SMS Coffee Shop\n");
            printf("1. Log in as Admin\n");
            printf("2. Log in as Customer\n");
            printf("3. Exit\n");

            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    if (adminlog()) {
                        login = 1;
                        printf("Welcome to the system.\n");
                    } else {
                        login = 0;
                        printf("Return to main menu.\n");
                    }
                    break;
                case 2:
                    /*if (login==1) {
                        printf("Please log out as admin to log in as a customer.\n");
                    } else {*/
                        takeOrder();

                    break;
                case 3:
                    printf("Exiting the program. Goodbye!\n");
                    exit(0);
                default:
                    printf("Invalid choice. Please try again.\n"); }
        } else {
            printf("\nAdmin Options:\n");

            printf("1. Update Menu\n");
            printf("2. Display Stock\n");
            printf("3. Generate Sales Report\n");
            printf("4. View Sales Report\n");
            printf("5. Clear Sales Report\n");
            printf("6. Change password\n");
            printf("7. Log out\n");
            printf("8. Exit\n");

            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {

                case 1:
                    updateMenu();
                    break;
                case 2:
                    displayStock();
                    break;
                case 3:
                    generateSalesReport();
                    break;
                case 4:
                    viewsale();
                    break;
                case 5:
                    clearsalesreport();
                    break;
                case 6:
                    passchange();
                    break;
                case 7:
                    login = 0;
                    printf("Admin logged out.\n");
                    break;
                case 8:
                    printf("Exiting the program. Goodbye!\n");
                    exit(1);
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");


            }
        }
    }

    return 0;
}
