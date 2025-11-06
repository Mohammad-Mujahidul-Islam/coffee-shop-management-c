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
    char itemname[50];
    float itemprice;
    int stock;
};

//change password
void passchange() {
    FILE *file = fopen(PASSWORD_FILE, "w");

    if (file == NULL) {
        perror("Error opening admin password file");
        exit(0);
    }

    char pass[50];
    printf("Write new pass: ");

    fflush(stdin);//to fix an undefined behavior
    gets(pass);

    int i=0;
    while(pass[i]!='\0')
    {
    if(pass[i]==' ')
    {
        printf("Error, your password cannot contain space.\nTry again,\n");
        passchange();
    }
    else
        fprintf(file, "%c", pass[i]);
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
    char enteredpass[10];
    char correctpass[10];

    // Read the correct admin password from the file
    readpass(correctpass);

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
            enteredpass[i] = ch;
            i++;
            printf("*");
        }
    }

    enteredpass[i] = '\0';

    // Compare the entered password with the correct password
    if (strcmp(enteredpass, correctpass) == 0) {
        printf("\nAdmin login successful.\n");
        return 1;
    } else {
        printf("\nInvalid admin password. Access denied.\n");
        printf("1.Try again\n2.Back\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d",&choice);
        while(1)
   {
       if(choice==1 || choice==2)
       {
       break;
       }
       printf("Invalid choice. Please enter correct choice\n");
       printf("Please  Enter your choice again:");
       scanf("%d",&choice);

   }
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

// Function to swap two MenuItem structures
void swap(struct MenuItem *a, struct MenuItem *b) {
    struct MenuItem temp = *a;
    *a = *b;
    *b = temp;
}
void bubble(){
    FILE *file = fopen(MENU_FILE, "r+");

    if (file == NULL) {
        perror("Error opening menu file");
        exit(EXIT_FAILURE);
    }

    struct MenuItem menu_items[100];
    int count = 0;

    while (fscanf(file, "%s %f %d", menu_items[count].itemname, &menu_items[count].itemprice, &menu_items[count].stock) == 3) {
        count++;
    }

    // Sort the items
for (int i = 0; i < count - 1; i++)
{
        for (int j = 0; j < count - i - 1; j++)
        {
            if (menu_items[j].stock < menu_items[j + 1].stock)
            {
                swap(&menu_items[j], &menu_items[j + 1]);
            }
        }
}
    fseek(file, 0, SEEK_SET);

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %f %d\n", menu_items[i].itemname, menu_items[i].itemprice, menu_items[i].stock);
    }

    fclose(file);

}
// Function to read menu items from file
void readmenu(struct MenuItem menu[], int *itemcount) {
    bubble();
    FILE *file = fopen(MENU_FILE, "r");

    if (file == NULL) {
        perror("Error opening menu file");
        exit(EXIT_FAILURE);
    }

    *itemcount = 0;

    while (fscanf(file, "%s %f %d", menu[*itemcount].itemname, &menu[*itemcount].itemprice, &menu[*itemcount].stock) != EOF)
        {
        (*itemcount)++;
        }

    fclose(file);
}


void writemenu(const struct MenuItem menu[], int itemcount) {
    FILE *file = fopen(MENU_FILE, "w");

    if (file == NULL) {
        perror("Error opening menu file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < itemcount; i++) {
        fprintf(file, "%s %.2f %d\n", menu[i].itemname, menu[i].itemprice, menu[i].stock);
    }

    fclose(file);
}

// Function to update the menu
void updatemenu() {
    int itemcount;
    struct MenuItem menu[MAX_MENU_ITEMS];

    readmenu(menu, &itemcount);

    /*printf("Current Menu:\n");
    for (int i = 0; i < itemcount; i++) {
        printf("%d. %s - %.2f Taka - Stock: %d\n", i + 1, menu[i].itemname, menu[i].itemprice, menu[i].stock);
    }*/

    int choice;
    printf("\nMenu Update Options:\n");

    printf("1. Add Item\n");
    printf("2. Delete Item\n");
    printf("3. Change Item Price\n");
    printf("4. Add Stock\n");
    printf("5. Back to Admin option\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    printf("\n");
    switch (choice) {

        case 1:
            // Add Item
            if (itemcount < MAX_MENU_ITEMS) {
                displaystock();
                printf("\nEnter the name of the new item: ");
                scanf("%s", menu[itemcount].itemname);

                printf("Enter the price of the new item: ");
                scanf("%f", &menu[itemcount].itemprice);

                printf("Enter the initial stock of the new item: ");
                scanf("%d", &menu[itemcount].stock);

                itemcount++;
                writemenu(menu, itemcount);

                printf("Item added to the menu.\n");

            } else {
                printf("Cannot add more items. Menu is full.\n");
            }
            updatemenu();
            break;
        case 2:
            // Delete Item
            if (itemcount > 0) {
                    displaystock();
                    printf("%d. Return back.\n",itemcount+1);
            while(1){
                printf("Enter the number of the item to delete: ");
                int item_number_delete;
                scanf("%d", &item_number_delete);

                if (item_number_delete >= 1 && item_number_delete <= itemcount) {
                    for (int i = item_number_delete-1 ; i < itemcount-1 ; i++) {
                        menu[i] = menu[i + 1];
                    }
                    itemcount--;
                    writemenu(menu, itemcount);

                    printf("Item deleted from the menu.\n");
                    updatemenu();
                }
                else if(item_number_delete == itemcount+ 1){
                    updatemenu();
                } else {
                    printf("Invalid item number. Please enter a valid item number.\n");
                }
            }
            }else {
                printf("Cannot delete items. Menu is empty.\n");
            }
            updatemenu();
            break;
        case 3:
            // Change Item Price
            printf("Enter the number of the item to change the price: ");
            int item_number_price;
            scanf("%d", &item_number_price);

            if (item_number_price >= 1 && item_number_price <= itemcount) {
                printf("Enter the new price for %s: ", menu[item_number_price - 1].itemname);
                float newPrice;
                scanf("%f", &newPrice);

                if (newPrice >= 0) {
                    menu[item_number_price - 1].itemprice = newPrice;
                    writemenu(menu, itemcount);

                    printf("Price changed successfully.\n");

                } else {
                    printf("Invalid price. Please enter a non-negative price.\n");
                }
            } else {
                printf("Invalid item number. Please enter a valid item number.\n");
            }
            updatemenu();
            break;
        case 4:
            // Add Stock
            printf("Enter the number of the item to add stock: ");
            int item_number;
            scanf("%d", &item_number);

            if (item_number >= 1 && item_number <= itemcount) {
                printf("Enter the quantity to add to the stock for %s: ", menu[item_number - 1].itemname);
                int quantity;
                scanf("%d", &quantity);

                if (quantity > 0) {
                    menu[item_number - 1].stock += quantity;
                    writemenu(menu, itemcount);

                    printf("Stock added successfully.\n");

                } else {
                    printf("Invalid quantity. Please enter a quantity greater than 0.\n");
                }
            } else {
                printf("Invalid item number. Please enter a valid item number.\n");
            }
            updatemenu();
            break;
        case 5:
            return 0;
        default:
            printf("Invalid choice.\n");
    }
}

//display stock
void displaystock() {
    int itemcount;
    struct MenuItem menu[MAX_MENU_ITEMS];

    readmenu(menu, &itemcount);

    printf("Current Stock:\n");
    for (int i = 0; i < itemcount; i++) {
        printf("%d. %s - Price: %0.2f Taka - Stock: %d\n",i + 1, menu[i].itemname,menu[i].itemprice, menu[i].stock);
    }
}

// Function to take order
void takeorder() {
    int itemcount;
    struct MenuItem menu[MAX_MENU_ITEMS];

    readmenu(menu, &itemcount);

    printf("Available Menu:\n");
    int num_serial =0;
    int i;
    for (i = 0; i < itemcount; i++) {
        if (menu[i].stock > 0) {
            num_serial++;
            printf("%d. %s - %.2f Taka - Stock: %d\n", num_serial, menu[i].itemname, menu[i].itemprice, menu[i].stock);

        }
    }
    printf("Enter %d for return to main menu.", num_serial+1);
    printf("\n\nHow many types of item do you want to buy?\n=>");
    int items_to_buy;
    scanf("%d", &items_to_buy);

    int order_count = 0;
    float total_bill = 0;

    time_t current_time;
    struct tm *local_time;

    time(&current_time);
    local_time = localtime(&current_time);

    FILE *order_file = fopen(ORDER_FILE, "a");

    if (order_file == NULL)
    {
        perror("Error opening order file");
        exit(EXIT_FAILURE);
    }

    fprintf(order_file, "-----\n");
    fprintf(order_file, "Order Time: %04d-%02d-%02d %02d:%02d:%02d\n",
            local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

if(items_to_buy<=num_serial){
    for (int i = 0; i < items_to_buy; i++) {
        printf("Enter the number of the item you want to order: ");
        int item_number;
        scanf("%d", &item_number);

        if (item_number >= 1 && item_number <=num_serial+1 ) {
            printf("Enter the quantity: ");
            int quantity;
            scanf("%d", &quantity);


                if (quantity <= menu[item_number- 1].stock) {
                    float subtotal = menu[item_number- 1].itemprice * quantity;

                    fprintf(order_file, "%s - Quantity: %d - Subtotal: %.2f Taka\n",
                            menu[item_number- 1].itemname, quantity, subtotal);

                    menu[item_number- 1].stock -= quantity;
                    writemenu(menu, itemcount);

                    total_bill += subtotal;
                    order_count++;

                    printf("%s Price in subtotal : %.2f Taka\n", menu[item_number- 1].itemname, subtotal);
                } else {
                    printf("Insufficient stock. Please choose a quantity less than or equal to %d.\n", menu[item_number- 1].stock);
                    i--;
                }
        }
        else {
              printf("Invalid item number. Please enter a valid and available item number.\n");
              i--;
        }
    }
}
else if(items_to_buy==num_serial+1){
    return 0;
}
else{
    printf("Invalid item type. Please choose a items quantity less than or equal to %d.\n",num_serial);
    takeorder();
}
    fprintf(order_file," Total Bill of this customer: %0.2f Taka \n",total_bill);
    fprintf(order_file, "=========================================\n");

    fclose(order_file);

    printf("Order Summary:\n");
    printf("Number of types of item ordered: %d\n", order_count);
    printf("Total Bill: %.2f Taka\n", total_bill);
}


// Function to generate SalesReport
void generate_sales_report() {
    FILE *order_file = fopen(ORDER_FILE, "r");
    FILE *sales_report_file = fopen(SALES_REPORT_FILE, "a");

    if (order_file == NULL || sales_report_file == NULL) {
        perror("Error opening order or sales report file");
        exit(EXIT_FAILURE);
    }

    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), order_file) != NULL) {
        fputs(buffer, sales_report_file);
    }

    fprintf(sales_report_file, "=========================================\n");

    fclose(order_file);
    fclose(sales_report_file);

    printf("Sales report generated.\n");
}
// Function to view SalesReport
void viewsale() {
    FILE *sales_report_file = fopen(SALES_REPORT_FILE, "r");

    if (sales_report_file == NULL) {
        perror("Error opening sales report file");
        exit(EXIT_FAILURE);
    }

    printf("\nSales Report:\n");
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), sales_report_file) != NULL) {
        // Check if the line contains the "Order Time" string
        if (strstr(buffer, "Order Time") != NULL) {
            printf("%s", buffer); // Print the date and time line
        } else {
            printf("%s", buffer); // Print the rest of the lines
        }
    }

    fclose(sales_report_file);
}

// Function to clear SalesReport
void clearsalesreport() {
    FILE *sales_report_file = fopen(SALES_REPORT_FILE, "w");
    FILE *order_report_file=fopen(ORDER_FILE,"w");
    if (sales_report_file == NULL || order_report_file == NULL   ) {
        perror("Error opening sales report file");
        exit(EXIT_FAILURE);
    }
    fclose(order_report_file);
    fclose(sales_report_file);
    printf("Sales report cleared successfully.");
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
                        printf("Return to main system.\n");
                    }
                    break;
                case 2:
                        takeorder();

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
                    updatemenu();
                    break;
                case 2:
                    displaystock();
                    break;
                case 3:
                    generate_sales_report();
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
