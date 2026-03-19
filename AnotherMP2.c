#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY 5 //encryption decryption key - do not change this or all encryptions will be ruined
#define MAX_STR 31 //max string length
#define MAX_USER 100 //max recorded users
#define MAX_HH 5 //max household

/* DATA STRUCTURES AND STRING TYPEDEF */
typedef char String[MAX_STR];

typedef struct {
    String username;
    String password;
} User;

typedef struct {
    String house;
    double monthlyBill;
    double monthlykWh;
    double roofsize;
} Record;

/* FUNCTION DECLARATIONS */

/* Helper functions */
void encryptPassword(String password) //simple xor encryption
{

	int n = strlen(password);
	for(int i=0; i<n; i++)
	{
		password[i] = password[i] ^ KEY;
	}
}

void pauseScreen() //pause and clear screen - i didn't want to type it everytime
{
	printf("--- PRESS ENTER TO PROCEED ---\n");
	while(getchar() != '\n')
	{}
	getchar();
	
	system("cls"); //clears screen - this is for windows only, search for ways to 
}

int findUserIndex(User users[], int userCount, String currentUser)
{
    int index = -1;
    int i = 0;
    while(i < userCount && index == -1) {
        if(strcmp(users[i].username, currentUser) == 0) {
            index = i;
        }
        i++;
    }
    return index;
}

/* File I/O */
int loadUsers(User users[]) {
    FILE *fp = fopen("users.txt", "r");
    int count = 0;
    if(fp != NULL) {
        while(fscanf(fp, "%30s %30s", users[count].username, users[count].password) == 2) {
            count++;
        }
        fclose(fp);
    }
    return count;
}

void saveUsers(User users[], int userCount) {
    FILE *fp = fopen("users.txt", "w");
    for(int i = 0; i < userCount; i++) {
        fprintf(fp, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(fp);
}

void loadHouseholds(User users[], int userCount, Record households[][MAX_HH], int householdCount[]) {
    FILE *fp = fopen("records.txt", "r");
    if(fp != NULL) {
        Record r;
        String uname;
        while(fscanf(fp, "%30s %30s %lf %lf %lf", uname, r.house, &r.monthlyBill, &r.monthlykWh, &r.roofsize) == 5) {
            int idx = findUserIndex(users, userCount, uname);
            if(idx != -1 && householdCount[idx] < MAX_HH) {
                households[idx][householdCount[idx]] = r;
                householdCount[idx]++;
            }
        }
        fclose(fp);
    }
}

void saveHouseholds(User users[], int userCount, Record households[][MAX_HH], int householdCount[]) {
    FILE *fp = fopen("records.txt", "w");
    for(int i = 0; i < userCount; i++) {
        for(int j = 0; j < householdCount[i]; j++) {
            fprintf(fp, "%s %s %.2lf %.2lf %.2lf\n",
                users[i].username,
                households[i][j].house,
                households[i][j].monthlyBill,
                households[i][j].monthlykWh,
                households[i][j].roofsize);
        }
    }
    fclose(fp);
}

/* Registration */
int registerUser(User users[], int *userCount) {
    User u;
    int found = 0;

    printf("--- ENTER USERNAME ---\n-> ");
    scanf("%30s", u.username);

    int i = 0;
    while(i < *userCount && found == 0) {
        if(strcmp(users[i].username, u.username) == 0) {
            printf("--- Username already taken ---\n");
            found = 1;
        }
        i++;
    }

    if(found == 1) return 0;

    printf("--- ENTER PASSWORD ---\n-> ");
    scanf("%30s", u.password);
    encryptPassword(u.password);

    users[*userCount] = u;
    (*userCount)++;

    printf("--- Registration successful ---\n");
    return 1;
}

/* Login */
int loginUser(User users[], int userCount, String currentUser) {
    String username, password;
    int foundIndex = -1;

    printf("--- ENTER USERNAME ---\n-> ");
    scanf("%30s", username);

    int i = 0;
    while(i < userCount && foundIndex == -1) {
        if(strcmp(users[i].username, username) == 0) {
            foundIndex = i;
        }
        i++;
    }

    if(foundIndex == -1) {
        printf("--- Username not found ---\n");
        return 0;
    }

    int attempts = 3;
    int success = 0;
    while(attempts > 0 && success == 0) {
        printf("--- ENTER PASSWORD ---\n-> ");
        scanf("%30s", password);
        encryptPassword(password);

        if(strcmp(password, users[foundIndex].password) == 0) {
            strcpy(currentUser, users[foundIndex].username);
            success = 1;
        } else {
            attempts--;
            if(attempts > 0) printf("--- Incorrect! %d attempts left ---\n", attempts);
            else printf("--- Too many failed attempts ---\n");
        }
    }
    return success;
}

/* Change Password */
void changePassword(User users[], int userCount, String currentUser) {
    int userIndex = findUserIndex(users, userCount, currentUser);
    if(userIndex != -1) {
        String oldPass, newPass;
        printf("--- ENTER OLD PASSWORD ---\n-> ");
        scanf("%30s", oldPass);
        encryptPassword(oldPass);
        if(strcmp(oldPass, users[userIndex].password) == 0) {
            printf("--- ENTER NEW PASSWORD ---\n-> ");
            scanf("%30s", newPass);
            encryptPassword(newPass);
            strcpy(users[userIndex].password, newPass);
            printf("--- Password changed successfully ---\n");
        } else {
            printf("--- Incorrect old password ---\n");
        }
    }
}

/* Household operations */
void addHousehold(User users[], Record households[][MAX_HH], int householdCount[], int userCount, String currentUser) {
    int userIndex = findUserIndex(users, userCount, currentUser);
    if(userIndex != -1 && householdCount[userIndex] < MAX_HH) {
        int h = householdCount[userIndex];
        printf("--- ENTER HOUSEHOLD NAME ---\n-> ");
        scanf("%30s", households[userIndex][h].house);
        printf("--- ENTER MONTHLY BILL ---\n-> ");
        scanf("%lf", &households[userIndex][h].monthlyBill);
        printf("--- ENTER MONTHLY kWh ---\n-> ");
        scanf("%lf", &households[userIndex][h].monthlykWh);
        printf("--- ENTER ROOF SIZE ---\n-> ");
        scanf("%lf", &households[userIndex][h].roofsize);

        householdCount[userIndex]++;
        sortHouseholds(households, householdCount, userIndex);
        printf("--- Household added ---\n");
    }
}

void viewRecords(User users[], Record households[][MAX_HH], int householdCount[], int userCount, String currentUser) {
    int userIndex = findUserIndex(users, userCount, currentUser);
    if(userIndex != -1) {
        printf("+-------------------------------+\n");
        printf("| HOUSEHOLD RECORDS             |\n");
        printf("+-------------------------------+\n");
        for(int j = 0; j < householdCount[userIndex]; j++) {
            printf("<--------------------------------->\n");
            printf("Household Name: %s\n", households[userIndex][j].house);
            printf("Monthly Bill: %.2f\n", households[userIndex][j].monthlyBill);
            printf("Monthly kWh: %.2f\n", households[userIndex][j].monthlykWh);
            printf("Roof Size: %.2f\n", households[userIndex][j].roofsize);
            printf("<--------------------------------->\n");
        }
    }
}



int main()
{
	User users[MAX_USER];
	Record households[MAX_USERS][MAX_HH];
    int householdCount[MAX_USERS] = {0};
    int userCount = 0;
    String currentUser;
	int loginSelect;
	int loginStatus = 1; //initialize for loop
	int Rstatus; //registser status
	int Lstatus; //logged in status
	
	
	while(loginStatus)
	{

	printf("+---------------------------------+\n");
	printf("| SOLAR ENERGY INFORMATION SYSTEM |\n");
	printf("+---------------------------------+\n");
	printf("|                                 |\n");
	printf("| 1. LOGIN                        |\n");
	printf("| 2. REGISTER                     |\n");
	printf("| 3. ACCOUNT RECOVERY             |\n");
	printf("|                                 |\n");
	printf("| 0. EXIT PROGRAM                 |\n");		
	printf("|                                 |\n");	
	printf("+---------------------------------+\n");
	printf("-> ");
		scanf("%d", &loginSelect);

		switch(loginSelect)
			{
			case 1: Lstatus = loginUser(users, userCount, currentUser); //LOGIN FUNCTION
					if(Lstatus==1)
					{	
						printf("+---------------------------------+\n");
						printf("| LOGIN SUCCESSFUL!               |\n");
						printf("+---------------------------------+\n");	
						pauseScreen();
						loggedinUI(currentUser); //then implement all the login functions
							
					}
			
			break;
			case 2:	Rstatus = registerUser();
					if(Rstatus==1) //successful return
					{	
						printf("+---------------------------------+\n");
						printf("| REGISTRATION SUCESSFUL!         |\n");
						printf("+---------------------------------+\n");
					}
					pauseScreen();
					break;
					
			case 3: 
				printf("PASSWORD RECOVERY FUNCTION NOT YET INPUTED\n"); //PW RECOVERY FUNCTION - NOT CREATED YET
				break;
				
			case 0: 
				printf("--- EXITING PROGRAM! ---\n");
				loginStatus = 0; //EXIT PROGRAM
				break;
				
			default:
				printf("--- INVALID SELECTION! PLEASE TRY AGAIN! ---\n"); //incase of wrong input
				pauseScreen();
				break;
			}
		
	}	
	
	return 0;
}
