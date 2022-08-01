#include "extensionFoodBank/srcFoodBank/foodBankSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define readLine(ptr) { \
if(!fgets(ptr,40, stdin)) { \
perror("IO error");     \
exit(EXIT_FAILURE);     \
}                       \
input[strlen(input) - 1] = '\0';};

typedef struct recipients {
    user_t* listOfRecipients[NUM_OF_RECIPIENTS];
    int currentPos;
    int maxPos;
} recipients_t;

bool staffUi(inventory_t *inv, user_t *user) {
    user->userType = STAFF;
    char *input = malloc(sizeof(char) * 40);
    while(strcmp(input, "exit")!= 0) {
        printf("What would you like to do?\n");
        printf("Enter 1 to view the stock held\n");
        printf("Enter 2 to view the donations on hold\n");
        printf("Enter 3 to close store\n");
        printf("Enter exit to exit\n");
        readLine(input);
        if (strcmp("1", input) == 0) {
            printInventory(inv);
            continue;
        } else if (strcmp("2", input) == 0) {
            printDonationsOnHold(inv);
            continue;
        } else if (strcmp("3", input) == 0) {
            return true;
        } else {
            continue;
        }
    }
    return false;
}

void recipientUi(inventory_t *inv, user_t *user) {
    user->userType = RECIPIENT;
    giveBoxOfFood(inv, user);
}

void donorUi(inventory_t *inv, user_t *user) {
    user->userType = DONOR;
    char *input = malloc(sizeof(char) * 40);
    printf("Thank you for donating! What is your name?\n");
    readLine(input);
    strcpy(user->name,input);

    printf("What is the name of the food item would you like to donate, %s?\n", user->name);
    readLine(input);
    char *foodName = malloc(sizeof(char) * MAX_FOOD_NAME_LENGTH);
    strcpy(foodName, input);

    date_t expiryDate;
    printf("What is the year of the expiry date of the food?\n");
    readLine(input);
    expiryDate.year = atoi(input);

    printf("What is the month of the expiry date of the food? (e.g. 7 for July or 11 for November)\n");
    readLine(input);
    expiryDate.month = atoi(input);

    printf("What is the day of the expiry date of the food? (e.g. 3 for the 3rd of the month\n");
    readLine(input);
    expiryDate.day = atoi(input);

    printf("What is the mass in grams of protein in your food item?\n");
    readLine(input);
    int protein = atoi(input);

    printf("What is the mass in grams of carbohydrates in your food item?\n");
    readLine(input);
    int carbs = atoi(input);

    printf("What is the type of your food item? (fruit, carbs, protein)\n");
    readLine(input);
    foodCategory category;
    if (strcmp("fruit", input) == 0) {
        category = FRUIT;
    } else if (strcmp("carbs", input) == 0) {
        category = CARBS;
    } else if(strcmp("protein", input) == 0) {
        category = PROTEIN;
    } else {
        printf("invalid category\n");
        return;
    }

    individualItem_t *newFoodItem = initialiseIndividualItem(inv, foodName, expiryDate, protein, carbs, category);
    user->typeData.donor.food = newFoodItem;
    donate(inv, *user);
    free(foodName);
}

void registerRecipientAccount(recipients_t *recipients) {
    char *input = malloc(sizeof(char) * 40);
    printf("What is your first name?\n");
    readLine(input);

    char *firstName = malloc(sizeof(char) * MAX_NAME_LENGTH);
    strcpy(firstName, input);

    printf("What is your last name?\n");
    readLine(input);

    char *lastName = malloc(sizeof(char) * MAX_NAME_LENGTH);
    strcpy(lastName, input);

    char *fullName = malloc(sizeof(char) * MAX_NAME_LENGTH);
    int i;
    for (i = 0; i < strlen(firstName); i++){
        fullName[i] = firstName[i];
    }
    fullName[i] = ' ';
    for (int j = i; j < strlen(lastName) + i; j++){
        fullName[j + 1] = lastName[j - i];
    }

    int id = hash(fullName);

    user_t *newRecipient = initialiseRecipient(id, fullName, -1);
    printf("You have now registered with us. your recipient ID is %d\n", id);
    if (recipients->currentPos == recipients->maxPos) {
        printf("We are not currently taking any more recipients.\n");
        return;
    }
    recipients->listOfRecipients[recipients->currentPos] = newRecipient;
    recipients->currentPos++;
}

bool isNotNum(char *string) {
    for (int i = 0; string[i]; i++) {
        if (string[i] < '1' || string[i] > '9') {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {

    inventory_t *inv = initialiseInventory();
    /* This is code copied from the test suite used to initialise the inventory with some items as opposed to empty
     * This was used purely for presentation purposes
    char apple[] = "apple";
    char banana[] ="banana";
    char pear[] = "pear";
    date_t appleDate = initialiseDate(2022, 8, 30);
    date_t bananaDate = initialiseDate(2022, 9, 4);
    date_t pearDate = initialiseDate(2022, 9, 4);
    individualItem_t *appleDonate1 = initialiseIndividualItem(inv, apple, appleDate, 0, 3, FRUIT);
    individualItem_t *appleDonate2 = initialiseIndividualItem(inv, apple, appleDate, 0, 3, FRUIT);
    individualItem_t *appleDonate3 = initialiseIndividualItem(inv, apple, appleDate, 0, 3, FRUIT);
    individualItem_t *bananaDonate1 = initialiseIndividualItem(inv, banana, bananaDate, 0, 2, FRUIT);
    individualItem_t *bananaDonate2 = initialiseIndividualItem(inv, banana, bananaDate, 0, 2, FRUIT);
    individualItem_t *pearDonate1 = initialiseIndividualItem(inv, pear, pearDate, 0, 2, FRUIT);
    char johnDon[] = "john";
    char tonyDon[] = "tony";
    char danDon[] = "dan";
    char martinDon[] = "martin";
    char garyDon[] = "gary";
    char barryDon[] = "barry";
    user_t *johnApple1 = createDonor(appleDonate1, johnDon);
    user_t *tonyApple2 = createDonor(appleDonate2, tonyDon);
    user_t *danApple3 = createDonor(appleDonate3, danDon);
    user_t *martinBanana1 = createDonor(bananaDonate1, martinDon);
    user_t *garyBanana2 = createDonor(bananaDonate2, garyDon);
    user_t *barryPear1 = createDonor(pearDonate1, barryDon);
    donate(inv, *johnApple1);
    donate(inv, *tonyApple2);
    donate(inv, *danApple3);
    donate(inv, *martinBanana1);
    donate(inv, *garyBanana2);
    donate(inv, *barryPear1);
    donate(inv, *barryPear1);

    char bread[] = "bread";
    date_t breadDate = initialiseDate(2022, 9, 1);
    individualItem_t *breadDonate = initialiseIndividualItem(inv, bread, breadDate, 8, 1000, CARBS);
    user_t *johnbread = createDonor(breadDonate, johnDon);
    donate(inv, *johnbread);
    char pasta[] = "pasta";
    date_t pastaDate = initialiseDate(2022, 6, 29);
    individualItem_t *pastaDonate = initialiseIndividualItem(inv, pasta, pastaDate, 5, 800, CARBS);
    user_t *johnpasta = createDonor(pastaDonate, johnDon);
    donate(inv, *johnpasta);
    char steak[] = "steak";
    date_t steakDate = initialiseDate(2022, 7, 1);
    individualItem_t *steakDonate = initialiseIndividualItem(inv, steak, steakDate, 250, 2, PROTEIN);
    user_t *johnsteak = createDonor(steakDonate, johnDon);
    donate(inv, *johnsteak);
    date_t steakDate2 = initialiseDate(2022, 7, 3);
    individualItem_t *steakDonate2 = initialiseIndividualItem(inv, steak, steakDate2, 350, 2, PROTEIN);
    user_t *johnsteak2 = createDonor(steakDonate2, johnDon);
    donate(inv, *johnsteak2);
     */





    user_t staff[NUM_OF_STAFF] = {
            {STAFF, "Dominic Qu", {{"dcq21"}}},
            {STAFF,"Darius Nilforooshan", {{"dn121"}}},
            {STAFF,"Sachin Wadhwani",{ {"ssw21"}}},
            {STAFF,"Sam Tackaberry", {{"st1621"}}}
    };

    recipients_t recipients;
    recipients.listOfRecipients[0] = initialiseRecipient(hash("John Smith"), "John Smith", 6);
    recipients.listOfRecipients[1] = initialiseRecipient(hash("Radhika Patel"), "Radhika Patel", 9);
    recipients.listOfRecipients[2] = initialiseRecipient(hash("Bob He"), "Bob He", 2);
    recipients.listOfRecipients[3] = initialiseRecipient(hash("Allison Matthews"), "Allison Matthews", 4);
    recipients.currentPos = 4;
    recipients.maxPos = NUM_OF_RECIPIENTS;


//    User interface
    printf("Welcome to the food bank!\n");
    char *input = calloc(40, sizeof(char));
    while(strcmp(input, "exit") != 0) {
        int verified = 0;

        //NEW USER
        user_t *user = malloc(sizeof(user_t));

        printf("Are you (s)taff, a (d)onor or a (r)ecipient?\n");
        readLine(input);

        if (strcmp(input,"s") == 0) {
            printf("Please enter your cardId\n");
            readLine(input);

            for (int i=0; i<NUM_OF_STAFF; i++) {
                if (strcmp(staff[i].typeData.staff.cardId, input) == 0) {
                    strcpy(user->typeData.staff.cardId, staff[i].typeData.staff.cardId);
                    verified=1;
                    break;
                }
            }

            if (verified == 0) {
                printf("You are not authorised.\n");
                continue;
            }
            if (staffUi(inv, user)) {
                break;
            }
        } else if (strcmp(input,"d") == 0) {
            donorUi(inv, user);
        } else if (strcmp(input,"r") == 0) {
            printf("Have you registered with us? (y/n)\n");
            readLine(input);
            if (strcmp("y", input) == 0) {
                printf("Please enter your recipient Id\n");
                readLine(input);

                for (int i=0; i<recipients.currentPos; i++) {
                    if (recipients.listOfRecipients[i]->typeData.recipient.recipientId == atoi(input) && (!isNotNum(input))) {
                        user = recipients.listOfRecipients[i];
                        verified=1;
                        break;
                    }
                }

                if (verified == 0) {
                    printf("You are not authorised.\n");
                    continue;
                }
                recipientUi(inv, user);
            } else if (strcmp("n", input) == 0) {
                registerRecipientAccount(&recipients);
                continue;
            } else {
                continue;
            }
        } else if (strcmp(input,"exit") == 0) {
            continue;
        }
        free(user);
    }

    printf("Thank you for visiting the food bank.\n");
    printf("We are now closed.\n");
    return 0;
}



