/*
Student Names: Chad Wilson, Rohan Burke, Jordan Parague
School: Jamaica College
Project Title: JC Canteen Catering System
Project Synopsis: Implementation of an school canteen ordering system
Date Completed:
SKills Used: Read/write to file, search, add/remove/update records
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1600
int currSize = 0, choice;
int monCount = 0, tuesCount = 0, wedCount = 0;
int thurCount = 0, friCount = 0;

FILE *ptrIn, *ptrIn2, *ptrOut, *ptrOut2;
FILE *ptrIn3, *ptrIn4, *ptrIn5, *ptrIn6;

/// @brief The structure to be used with student database file.
struct database{
    unsigned int ID;
    char lName[25];
    char fName[25];
    unsigned int grade;
    char payMethod[16];
    float balance;
}s[MAX_SIZE], studentUser, temp;

/// @brief The structure storing the students' credentials
struct credentials{
    unsigned int ID;
    char StuPWords[17];
    int accNum;
    int PIN;
}stuCreds[MAX_SIZE], userCreds, temp2;

/// @brief The structure that will hold the information on food items
struct foodItem{
    char name[20];
    float price;
}lunches[5], patties[5], pastries[5], drinks[5];

/// @brief The structure that will hold the total price for the receipt
struct receipt{
    float totalPrice;
}receiptInfo;

//Prototyping Functions

/// @brief Prompts user to sign in as a student or staff
/// @returns 1 to verify student, 2 to verify staff, 0 for failure
int SignIn();

/// @brief Verifies the credentials entered to sign in user as student
/// @param ID 
/// @param password 
/// @returns 1 for success, 0 for failure
int SignInStudent(int ID, char password[]);

/// @brief Verifies the credentials entered to sign in user as staff
/// @param password 
/// @returns 2 for success, 0 for failure
int SignInStaff(char password[]);

/// @brief Reads from student records file and stores record to studentUser structure
/// @param ID 
/// @returns 1 for success, 0 for failure
int UpdateStuInfo(int ID);

/// @brief Displays the menu for the user if they are a student
void Student();

/// @brief Displays information about the student from a record to the screen
void ViewOwnInfo();

/// @brief Changes the payment information of the user
void EditOwnInfo();

/// @brief Increases the balance of the user via online payment
void TopUpBalance();

/// @brief Displays the canteen menu and prompts the user to select items
void PlaceOrder();

/// @brief Displays the total price of the items selected and prompts the user
/// @brief to finish the transaction
void Payment();

/// @brief Prints the receipt to a file
void PrintCheck();

/// @brief Displays the menu for the user if they are a staff member
void Staff();

/// @brief Displays the entire student database to the screen
void ViewStuDatabase();

/// @brief Searches the database structure for a record with the corresponding ID and dlisplays the information
/// @param ID 
/// @returns The structure array index for success, -1 if the record is not found
int SearchRecord(int ID);

/// @brief Adds a record to the student database
void AddRecord();

/// @brief Changes the information of a record with the corresponding ID
/// @param ID
void EditRecord(int ID);

/// @brief Removes a record with the corresponding ID from the structure array
/// @param ID
void RemoveRecord(int ID);

/// @brief Displays the amount of orders made for each day
void OrderCount();

/// @brief Overwrites the student records file with the updated structure array
void UpdateDatabase();

/// @brief Reads the student records file and stores into the structure array
void ReadDatabase();

/// @brief Navigates the user through the different menus
/// @return 0 to exit the program
int main()
{
    int p = 0;

    printf("\n** Welcome to the JC Canteen Catering System **\n\n");
    
    p = SignIn();

    if (p == 1) Student();
    else if (p == 2) Staff();

   return 0;
}

//Function working 04/10/23, DON'T TOUCH
int SignIn()
{    
    int ID = 0, x = 0, r = -1;
    char password[17];

    // While loop used to repeat the menu and keep the program running
    do{
    printf("1. Student Sign In\n2. Staff Sign In\n0. Exit\n\n");
    printf("Choose an option: ");
    scanf("%d", &choice);

        switch (choice){
        case 1:
            printf("\nEnter ID: ");
            scanf("%d", &ID);

            printf("Enter password (16 character limit): ");
            scanf("%s", password);

            if (strlen(password) > 16){
                printf("Error: Password exceeds character limit\n\n");
                Student();
            }

            x = SignInStudent(ID, password);

            if (x == 0) printf("Error: Invalid credentials\n\n");
            else if (x == 1){
                printf("\nSigned In Successfully!!\n\n");

                printf("Retreiving user info...\n\n");

                ReadDatabase();
                r = UpdateStuInfo(ID);

                if (r == 1)
                    printf("User info retrieved!!\n\n");
                else
                    printf("Error: User info not found. Try again");
            }
            break;
        case 2:
            printf("\nEnter password: ");
            scanf("%s", password);

            x = SignInStaff(password);

            if (x == 0) printf("\nError: Invalid credentials\n\n");
            else if (x == 2){
                printf("\nSigned In Successfully!!\n\n");
            }
            break;
        case 0:
            printf("Exiting application...");
            exit(0);
            break;
        default:
            printf("Invalid input: Enter the corresponding number only\n\n");
            break;
        }        
    } while (choice != 0 && x == 0);
    return x;
}

//Function working 04/10/23, DON'T TOUCH
int SignInStudent(int ID, char password[])
{
    int i = 0, success = 0;

    ptrIn = fopen("studcreds.txt", "r");
    if(ptrIn == NULL){
        printf("Error: file unavailable\n");
        exit(1);
    }

    while(!feof(ptrIn)){
        fscanf(ptrIn, "%d", &stuCreds[i].ID);
        fscanf(ptrIn, "%s", stuCreds[i].StuPWords);
        
        if(ID == stuCreds[i].ID && strcmp(password, stuCreds[i].StuPWords) == 0){
            success = 1;
            break;
        }
        i++;
    }

    fclose(ptrIn);
    return success;
}

//Function working 04/10/23, DON'T TOUCH
int SignInStaff(char password[])
{
    char StaffPWord[] = {"TrueBlueSt@Ff189"};
    int success = 0;

    if (strcmp(password, StaffPWord) == 0) success = 2;

    return success;
}

//Function working 04/11/23, DON'T TOUCH
int UpdateStuInfo(int ID)
{
    int success = 0, i = 0;

    ptrIn = fopen("sturecords.txt", "r");
    ptrIn2 = fopen("studcreds.txt", "r");
    
    if (ptrIn == NULL || ptrIn2 == NULL){
       printf("Error: file(s) unavailable\n");
       exit(1);
    }

    while(!feof(ptrIn)){
        fscanf(ptrIn, "%d %s", &s[i].ID, s[i].lName);
        fscanf(ptrIn, "%s %d", s[i].fName, &s[i].grade);
        fscanf(ptrIn, "%s %f", s[i].payMethod, &s[i].balance);

        fscanf(ptrIn2, "%d", &stuCreds[i].ID);
        fscanf(ptrIn2, "%s", stuCreds[i].StuPWords);
        fscanf(ptrIn2, "%d", &stuCreds[i].accNum);
        fscanf(ptrIn2, "%d", &stuCreds[i].PIN);

        if (ID == s[i].ID){
            studentUser = s[i];
            userCreds = stuCreds[i];
            success = 1;
            break;
        }
        i++;
    }
    fclose(ptrIn);   

    return success;
}

//Function working 04/11/23, DON'T TOUCH
void Student()
{
    // While loop used to repeat the menu and keep the program running
    do{
        printf("\n\n**Student Menu**\n\n");

        printf("1. View Student Info\n2. Edit Payment Info\n");
        printf("3. Place Order\n4. TopUp Balance\n5. View Receipt\n");
        printf("0. Return to Main Menu\n\n");

        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                ViewOwnInfo();
                break;
            case 2:
                EditOwnInfo();
                break;
            case 3:
                PlaceOrder();
                break;
            case 4:
                TopUpBalance();
                break;
            case 5:
                PrintCheck();
                break;
            case 0:
                printf("Returning to Main Menu...\n\n");

                UpdateDatabase();
                main();
                break;

            default:
                printf("Invalid input: Enter the corresponding number only\n\n");
                break;
        }
    } while (choice != 0);
}

//Function working 04/11/23, DON'T TOUCH
void ViewOwnInfo()
{
    printf("\nStudent Information:\n");

    printf("\nID: %d\n", studentUser.ID);
    printf("LASTNAME: %s\n", studentUser.lName);
    printf("FIRSTNAME: %s\n", studentUser.fName);
    printf("GRADE: %d\n", studentUser.grade);
    printf("PAY METHOD: %s\n", studentUser.payMethod);
    printf("BALANCE: $%0.2f\n", studentUser.balance);
    printf("A/C NUMBER: %d\n\n", userCreds.accNum);
}

//Function working 04/11/23, DON'T TOUCH
void EditOwnInfo()
{
    printf("Choose Payment Method:\n\n");

    printf("1. Visa\n2. NCB Quisk\n3. Paymaster\n");
    printf("4. Paypal\n5. Payoneer\n6. Cash (Pay at Canteen/Bursary)\n");
    printf("7. Remove Info\n0. Return to Student Menu\n\n");

    scanf("%d", &choice);

    switch (choice){
        case 1:
            printf("Enter account number: ");
            scanf("%d", &userCreds.accNum);

            printf("Enter PIN number: ");
            scanf("%d", &userCreds.PIN);            

            strcpy(studentUser.payMethod, "Visa");
            printf("Payment Method Successfully Changed to %s!!!\n\n", studentUser.payMethod);
            break;
        case 2:
            printf("Enter account number: ");
            scanf("%d", &userCreds.accNum);

            printf("Enter PIN number: ");
            scanf("%d", &userCreds.PIN);

            strcpy(studentUser.payMethod, "Quisk");
            printf("Payment Method Successfully Changed to %s!!!\n\n", studentUser.payMethod);
            break;
        case 3:
            printf("Enter account number: ");
            scanf("%d", &userCreds.accNum);

            printf("Enter PIN number: ");
            scanf("%d", &userCreds.PIN);

            strcpy(studentUser.payMethod, "Paymaster");
            printf("Payment Method Successfully Changed to %s!!!\n\n", studentUser.payMethod);
            break;
        case 4:
            printf("Enter account number: ");
            scanf("%d", &userCreds.accNum);

            printf("Enter PIN number: ");
            scanf("%d", &userCreds.PIN);

            strcpy(studentUser.payMethod, "PayPal");
            printf("Payment Method Successfully Changed to %s!!!\n\n", studentUser.payMethod);
            break;
        case 5:
            printf("Enter account number: ");
            scanf("%d", &userCreds.accNum);

            printf("Enter PIN number: ");
            scanf("%d", &userCreds.PIN);

            strcpy(studentUser.payMethod, "Payoneer");
            printf("Payment Method Successfully Changed to %s!!!\n\n", studentUser.payMethod);
            break;
        case 6:
            strcpy(studentUser.payMethod, "Cash");
            printf("Payment Method Successfully Changed to %s!!!\n\n", studentUser.payMethod);
            break;
        case 7:
            strcpy(studentUser.payMethod, "");
            printf("Payment Method Successfully Removed!!!\n\n");
            break;                                            
        case 0:
            printf("Returning to Student Menu...\n\n");
            Student();
            break;      
        default:
            printf("Invalid input: Enter the corresponding number only\n\n");
            break;
    }
}

//Function working 04/16/23, DON'T TOUCH
void PlaceOrder()
{
    char day[10], c;
    int success = 0;

    ptrIn = fopen("Lunch Menu.txt", "r");
    ptrOut = fopen("receipt.txt", "w");
    if(ptrIn == NULL){
        printf("Error: file unavailable\n\n");
        exit(1);
    }

    while ((c = fgetc(ptrIn)) != EOF){
        printf("%c", c);
    }

    fprintf (ptrOut, "ID: %d\n", studentUser.ID);
    fprintf (ptrOut, "Name: %s %s", studentUser.fName, studentUser.lName);

    
    do{
        printf("\n\nWhich day would you like to order for?\n");
        printf("\n1. Monday\n2. Tuesday\n3. Wednesday\n");
        printf("4. Thursday\n5. Friday\n6. Finish Order\n");
        printf("0. Exit\n");

        scanf("%d", &choice);

        switch (choice){
            case 1:
                strcpy(day, "Monday");
                monCount++;
                break;
            case 2:
                strcpy(day, "Tuesday");
                tuesCount++;
                break;
            case 3:
                strcpy(day, "Wednesday");
                wedCount++;
                break;
            case 4:
                strcpy(day, "Thursday");
                thurCount++;
                break;
            case 5:
                strcpy(day, "Friday");
                friCount++;
                break;
            case 6:
                fprintf(ptrOut, "Total Amount: $%0.2f", receiptInfo.totalPrice);
                success = 1;
                break;
            case 0:
                Student();
                break;                                                                              
            default:
                printf("Invalid input: Enter corresponding digits only\n\n");
                break;
        }        

        switch(choice){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                fprintf(ptrOut, "\n\n%s\n\n", day);
                do{
                    printf("\nChoose an item: \n");
                    printf("1. Box Lunches\n2. Patties\n3. Pastries\n");
                    printf("4. Beverages\n0. Return to Cafeteria Menu\n");

                    scanf("%d", &choice);

                    switch(choice){
                        case 1:
                            printf("Box Lunch Menu\n\n");

                            for (int i = 0, j = 1; i < 5; i++, j++){
                                printf("%d. %s - $%0.2f\n", j, lunches[i].name, lunches[i].price);
                            }
                            printf("\nChoose lunch: ");

                            scanf("%d", &choice);

                            switch(choice){
                                case 1:
                                    receiptInfo.totalPrice += lunches[0].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", lunches[0].name, lunches[0].price);
                                    break;
                                case 2:
                                    receiptInfo.totalPrice += lunches[1].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", lunches[1].name, lunches[1].price);
                                    break;
                                case 3:
                                    receiptInfo.totalPrice += lunches[2].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", lunches[2].name, lunches[2].price);
                                    break;
                                case 4:
                                    receiptInfo.totalPrice += lunches[3].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", lunches[3].name, lunches[3].price);
                                    break;
                                case 5:
                                    receiptInfo.totalPrice += lunches[4].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", lunches[4].name, lunches[4].price);
                                    break;
                                default:
                                    printf("Invalid input: Enter corresponding digits only\n\n");
                                    break;      
                            }

                            break;
                        case 2:
                           printf("Patties Menu\n\n");

                            for (int i = 0, j = 1; i < 5; i++, j++){
                                printf("%d. %s - $%0.2f\n", j, patties[i].name, patties[i].price);
                            }
                            printf("\nChoose lunch: ");

                            scanf("%d", &choice);

                            switch(choice){
                                case 1:
                                    receiptInfo.totalPrice += patties[0].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", patties[0].name, patties[0].price);
                                    break;
                                case 2:
                                    receiptInfo.totalPrice += patties[1].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", patties[1].name, patties[1].price);
                                    break;
                                case 3:
                                    receiptInfo.totalPrice += patties[2].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", patties[2].name, patties[2].price);
                                    break;
                                case 4:
                                    receiptInfo.totalPrice += patties[3].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", patties[3].name, patties[3].price);
                                    break;
                                case 5:
                                    receiptInfo.totalPrice += patties[4].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", patties[4].name, patties[4].price);
                                    break;
                                default:
                                    printf("Invalid input: Enter corresponding digits only\n\n");
                                    break;      
                            }                            

                            break;
                        case 3:
                           printf("Pastries Menu\n\n");

                            for (int i = 0, j = 1; i < 5; i++, j++){
                                printf("%d. %s - $%0.2f\n", j, pastries[i].name, pastries[i].price);
                            }
                            printf("\nChoose lunch: ");

                            scanf("%d", &choice);

                            switch(choice){
                                case 1:
                                    receiptInfo.totalPrice += pastries[0].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", pastries[0].name, pastries[0].price);
                                    break;
                                case 2:
                                    receiptInfo.totalPrice += pastries[1].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", pastries[1].name, pastries[1].price);
                                    break;
                                case 3:
                                    receiptInfo.totalPrice += pastries[2].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", pastries[2].name, pastries[2].price);
                                    break;
                                case 4:
                                    receiptInfo.totalPrice += pastries[3].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", pastries[3].name, pastries[3].price);
                                    break;
                                case 5:
                                    receiptInfo.totalPrice += pastries[4].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", pastries[4].name, pastries[4].price);
                                    break;
                                default:
                                    printf("Invalid input: Enter corresponding digits only\n\n");
                                    break;      
                            }                            

                            break;
                        case 4:
                           printf("Beverages Menu\n\n");

                            for (int i = 0, j = 1; i < 5; i++, j++){
                                printf("%d. %s - $%0.2f\n", j, drinks[i].name, drinks[i].price);
                            }
                            printf("\nChoose lunch: ");

                            scanf("%d", &choice);

                            switch(choice){
                                case 1:
                                    receiptInfo.totalPrice += drinks[0].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", drinks[0].name, drinks[0].price);
                                    break;
                                case 2:
                                    receiptInfo.totalPrice += drinks[1].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", drinks[1].name, drinks[1].price);
                                    break;
                                case 3:
                                    receiptInfo.totalPrice += drinks[2].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", drinks[2].name, drinks[2].price);
                                    break;
                                case 4:
                                    receiptInfo.totalPrice += drinks[3].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", drinks[3].name, drinks[3].price);
                                    break;
                                case 5:
                                    receiptInfo.totalPrice += drinks[4].price;
                                    fprintf(ptrOut, "%-10s - $%0.2f\n", drinks[4].name, drinks[4].price);
                                    break;
                                default:
                                    printf("Invalid input: Enter corresponding digits only\n\n");
                                    break;      
                            }                            

                            break;
                        case 0:
                            printf("Returning to Order Menu...\n\n");
                            break;
                        default:
                            printf("Invalid input: Enter corresponding digits only\n\n");
                            break;
                    }

                }while (choice != 0);

                break;
        }
    } while (success == 0);

    fclose(ptrIn);
    fclose(ptrOut);

    printf("Initiating Payment Process...\n\n");
    Payment();

}

//Function working 04/16/23, DON'T TOUCH
void Payment()
{
    char c;
    int success = 0;
    ptrIn = fopen("receipt.txt", "r");
    ptrOut = fopen("receipt.txt", "a");

    printf("        **Receipt**\n\n");

    while ((c = fgetc(ptrIn)) != EOF){
        printf("%c", c);
    }

    fseek(ptrOut, 0, SEEK_END);
    do{
        printf("\n\nCurrent balance: $%0.2f\n", studentUser.balance);
        printf("Choose Method of Payment: \n");
        printf("\n1. Pay via Balance\n2. Cash (Pay at Canteen or Bursary Office at a later date)\n");
        printf("0. Exit\n");

        scanf("%d", &choice);

        switch (choice){
            case 1:
                if (studentUser.balance < receiptInfo.totalPrice)
                    printf("\nInsufficient Funds: Please top up balance at Student Menu.\n\n");
                else{
                    fprintf(ptrOut, "\nAmount Tendered: $%0.2f\n", receiptInfo.totalPrice);
                    fprintf(ptrOut, "Pay Method: %s", studentUser.payMethod);
                    studentUser.balance -= receiptInfo.totalPrice;

                    printf("Transaction Successful. Printing Receipt...\n\n");
                    success = 1;
                }

                break;
            case 2:
                fprintf(ptrOut, "\nAmount Tendered: $%0.2f\n", receiptInfo.totalPrice);
                fprintf(ptrOut, "Pay Method: Cash");
                studentUser.balance -= receiptInfo.totalPrice;

                printf("Transaction Successful. Printing Receipt...\n\n");            
                success = 1;
                break;
            case 0:
                Student();
            default:
                printf("Invalid input: Enter corresponding digits only\n\n");
                break;
        }        
    } while (success == 0);

    fclose(ptrIn);
    fclose(ptrOut);

    PrintCheck();


}

//Function working 04/16/23, DON'T TOUCH
void PrintCheck()
{
    char c;
    ptrIn = fopen("receipt.txt", "r");
    if (ptrIn == NULL){
        printf("Error: file unavailable");
        exit(1);
    }

    printf("        **Receipt**\n\n");

    while ((c = fgetc(ptrIn)) != EOF){
        printf("%c", c);
    }

    fclose(ptrIn);

    Student();
}

//Function working 04/11/23, DON'T TOUCH
void TopUpBalance()
{
    int accNum, PIN;
    float amount;
    int success = 0;

    if (strcmp(studentUser.payMethod, "Cash") == 0){
        printf("Warning: Cash users may not utilize the online balance.\nSwitch to online payment.\n\n");
        Student();
    }

    printf("\nSelect the amount you want to top up: \n\n");
    printf("1. $500\n2. $1000\n3. 2000\n4. $5000");
    printf("\n0. Return to Student Menu\n\n");

    scanf("%d", &choice);

    switch (choice){
        case 1:
            amount = 500.00;
            break;
        case 2:
            amount = 1000.00;
            break;
        case 3:
            amount = 2000.00;
            break;
        case 4:
            amount = 5000.00;
            break;
        case 0:
            printf("Returning to Student Menu...\n\n");
            Student();
            break;                                
        default:
            printf("Invalid input: Enter the corresponding number only\n\n");
            TopUpBalance();
            break;
    }
    printf("\nEnter account number: ");
    scanf("%d", &accNum);

    printf("Enter PIN number: ");
    scanf("%d", &PIN);

    if (accNum == userCreds.accNum && PIN == userCreds.PIN){
        studentUser.balance += amount;

        printf("\nTransaction Successful!!!\n");
        printf("Current Balance: $%0.2f\n\n", studentUser.balance);        

    }
    else
        printf("Error: Invalid credentials\n\n");
}

//Function working 04/11/23, DON'T TOUCH
void Staff()
{
    int index = 0, ID;

    ReadDatabase();

    do{
        printf("**Staff Menu**\n\n");

        printf("1. View Student Database\n2. Search Student Record\n");
        printf("3. Add Student Record\n4. Edit Student Record\n");
        printf("5. Remove Student Record\n6. View Lunch Orders\n0. Return to Main Menu\n\n");

        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                ViewStuDatabase();
                break;
            case 2:
                printf("Enter Student ID: ");
                scanf("%d", &ID);

                index = SearchRecord(ID);

                if (index < 0)
                    printf("Error: Student not found. Enter a valid ID\n\n");
                else printf("Student found at Record: %d!!\n\n", index);
                break;
            case 3:
                AddRecord();
                break;
            case 4:
                printf("Enter Student ID: ");
                scanf("%d", &ID);
                
                EditRecord(ID);
                break;
            case 5:
                printf("Enter Student ID you want to remove: ");
                scanf("%d", &ID);

                RemoveRecord(ID);
                break;
            case 6:
                OrderCount();     
            case 0:
                printf("Returning to Main Menu...\n\n");
                UpdateDatabase();
                main();
                break;

            default:
                printf("Invalid input: Enter the corresponding number only\n\n");
                break;
        }
    }while (choice != 0);


}

//Function working 04/11/23, DON'T TOUCH
void ViewStuDatabase()
{
    int i;

    printf("\n                                  ** STUDENT DATABASE **\n\n");
    printf("ID     Lastname                  Firstname                 Grade  Pay Method   Balance\n\n");
    for (i = 0; i < currSize; i++){
        if(s[i].ID == 0){
            break;
        }
        else{
            printf("%-6d %-25s %-25s %-6d %-12s $%0.2f\n", s[i].ID, s[i].lName, s[i].fName, s[i].grade, s[i].payMethod, s[i].balance);
        }

    }
    printf("\n");
}

//Function working 04/11/23, DON'T TOUCH
int SearchRecord(int ID)
{
    for(int i = 0; i <= currSize; i++){
        if (ID == s[i].ID){            
            printf("\nID: %d\n", s[i].ID);
            printf("LASTNAME %s\n", s[i].lName);
            printf("FIRSTNAME: %s\n", s[i].fName);
            printf("GRADE: %d\n", s[i].grade);
            printf("PAY METHOD: %s\n", s[i].payMethod);
            printf("BALANCE: $%0.2f\n\n", s[i].balance);

            return i;
        }
    }
    return -1;
}

//Function working 04/11/23, DON'T TOUCH
void AddRecord()
{
    printf("Please enter the new student information\n\n");

    printf("ID: ");
    scanf("%d", &s[currSize].ID);

    for(int i = 0; i < currSize; i++){
        if (s[currSize].ID == s[i].ID){
            printf("Error: Cannot create record with an existing ID\n\n");
            Staff();
        }

    }

    printf("Last Name: ");
    scanf("%s", s[currSize].lName);

    printf("First Name: ");
    scanf("%s", s[currSize].fName);

    printf("Grade: ");
    scanf("%d", &s[currSize].grade);

    printf("Password (16 character limit): ");
    scanf("%s", stuCreds[currSize].StuPWords);

    strcpy(s[currSize].payMethod, "N/A");
    stuCreds[currSize].ID = s[currSize].ID;
    s[currSize].balance = 0.00;

    currSize++;

    printf("Record added successfully!!\n");
}

//Function working 04/11/23, DON'T TOUCH
void EditRecord(int ID)
{
    int index = SearchRecord(ID);
    
    if (index < 0){
        printf("Error: Student not found. Enter a valid ID\n\n");
        Staff();
    }

    printf("Which changes would you like to make?\n");
    printf("1. Surname\n2. First Name\n");
    printf("3. Grade\n");
    scanf("%d", &choice);

    switch (choice){
        case 1:
            printf("Surname: ");
            scanf("%s", s[index].lName);
            break;
        case 2:
            printf("First Name: ");
            scanf("%s", s[index].fName);
            break;
        case 3:
            printf("Grade ");
            scanf("%d", &s[index].grade);
            break;
        default:
            printf("Invalid input: Enter the corresponding number only\n\n");
            break;
    }

    printf("Record Updated Successfully!!\n");

    SearchRecord(ID);
}

//Function working 04/11/23, DON'T TOUCH
void RemoveRecord(int ID)
{
    int index = SearchRecord(ID);

    printf("Are you sure you want to remove this record?\n");
    printf("1. Yes\n0. No\n");
    scanf("%d", &choice);

    switch (choice){
        case 1:
            s[index].ID = 0;
            strcpy(s[index].fName, "");
            strcpy(s[index].lName, "");
            s[index].grade = 0;
            strcpy(s[index].payMethod, "");
            s[index].balance = 0;

            stuCreds[index].ID = 0;
            strcpy(stuCreds[index].StuPWords, "");
            stuCreds[index].accNum = 0;
            stuCreds[index].PIN = 0;

            for(int i = index; i < currSize; i++){
                temp = s[i];
                s[i] = s[i+1];
                s[i+1] = temp;

                temp2 = stuCreds[i];
                stuCreds[i] = stuCreds[i+1];
                stuCreds[i+1] = temp2;
            }

            currSize--;

            printf("Record removed successfully!!\n\n");
            break;
        case 0:
            Staff();
            break;
        default:
            printf("Invalid input: Enter the corresponding number only\n\n");
            break;
    }
}

//Function working 04/17/23, DON'T TOUCH
void OrderCount()
{
    ptrIn = fopen("ordercount.txt", "r");

    if(ptrIn == NULL){
        printf("Note: 0 orders were made recently\n");
    } else{
        fscanf(ptrIn, "%d", &monCount);
        fscanf(ptrIn, "%d", &tuesCount);
        fscanf(ptrIn, "%d", &wedCount);
        fscanf(ptrIn, "%d", &thurCount);
        fscanf(ptrIn, "%d", &friCount);
    }

    printf("Monday Orders: %d\n", monCount);
    printf("Tuesday Orders: %d\n", tuesCount);
    printf("Wednesday Orders: %d\n", wedCount);
    printf("Thursday Orders: %d\n", thurCount);
    printf("Friday Orders: %d\n\n", friCount);

    fclose(ptrIn);

    Staff();
}

//Function working 04/11/23, DON'T TOUCH
void ReadDatabase()
{
    int i = 0;

    ptrIn = fopen("sturecords.txt", "r");
    ptrIn2 = fopen("studcreds.txt", "r");
    if(ptrIn == NULL || ptrIn2 == NULL){
        printf("Error: file unavailable\n\n");
        exit(1);
    }

    ptrIn3 = fopen("Weekly Menu.txt", "r");
    ptrIn4 = fopen("Patties Menu.txt", "r");
    if(ptrIn3 == NULL || ptrIn4 == NULL){
        printf("Error: file unavailable\n\n");
        exit(1);
    }

    ptrIn5 = fopen("Pastries Menu.txt", "r");
    ptrIn6 = fopen("Beverage Menu.txt", "r");
    if(ptrIn5 == NULL || ptrIn6 == NULL){
        printf("Error: file unavailable\n\n");
        exit(1);
    }

    while (!feof(ptrIn) && !feof(ptrIn2)){
        fscanf(ptrIn, "%d", &s[i].ID);
        fscanf(ptrIn, "%s", s[i].lName);
        fscanf(ptrIn, "%s", s[i].fName);
        fscanf(ptrIn, "%d", &s[i].grade);
        fscanf(ptrIn, "%s", s[i].payMethod);
        fscanf(ptrIn, "%f", &s[i].balance);

        fscanf(ptrIn2, "%d", &stuCreds[i].ID);
        fscanf(ptrIn2, "%s", stuCreds[i].StuPWords);
        fscanf(ptrIn2, "%d", &stuCreds[i].accNum);
        fscanf(ptrIn2, "%d", &stuCreds[i].PIN);

        i++;
    }
    currSize = i;

    i = 0;

    while (!feof(ptrIn3)){
        
        fscanf(ptrIn3, "%s %f", lunches[i].name, &lunches[i].price);

        i++;
    }

    i = 0;
    while (!feof(ptrIn4)){

        fscanf(ptrIn4, "%s %f", patties[i].name, &patties[i].price);

        i++;
    }

    i = 0;

    while (!feof(ptrIn5)){
        
        fscanf(ptrIn5, "%s %f", pastries[i].name, &pastries[i].price);

        i++;
    }

    i = 0;
    while (!feof(ptrIn6)){

        fscanf(ptrIn6, "%s %f", drinks[i].name, &drinks[i].price);

        i++;
    }

    fclose(ptrIn);
    fclose(ptrIn2);
    fclose(ptrIn3);
    fclose(ptrIn4);
    fclose(ptrIn5);
    fclose(ptrIn6);
}

//Function working 04/11/23, DON'T TOUCH
void UpdateDatabase()
{
    int i = 0;
    ptrOut = fopen("sturecords.txt", "w");
    ptrOut2 = fopen("studcreds.txt", "w");

    while(i < currSize){
        
        if (studentUser.ID == s[i].ID){
            s[i] = studentUser;
            stuCreds[i] = userCreds;
        }
        if (s[i].ID == 0)
            break;
        else{
            fprintf(ptrOut, "\n%d ", s[i].ID);
            fprintf(ptrOut, "%s ", s[i].lName);
            fprintf(ptrOut, "%s ", s[i].fName);
            fprintf(ptrOut, "%d ", s[i].grade);
            fprintf(ptrOut, "%s ", s[i].payMethod);
            fprintf(ptrOut, "%0.0f", s[i].balance);

            fprintf(ptrOut2, "\n%d ", stuCreds[i].ID);
            fprintf(ptrOut2, "%s ", stuCreds[i].StuPWords);
            fprintf(ptrOut2, "%d ", stuCreds[i].accNum);
            fprintf(ptrOut2, "%d", stuCreds[i].PIN);
        }
        i++;
    }

    fclose(ptrOut);
    fclose(ptrOut2);

    ptrOut = fopen("ordercount.txt", "w");

    fprintf(ptrOut, "%d\n%d\n%d\n%d\n%d", monCount, tuesCount, wedCount, thurCount, friCount);

    fclose(ptrOut);

}
