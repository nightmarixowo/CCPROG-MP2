// MANGAHAS && CARTAGO - CCPROG2 MP - SOLAR ENERGY INFORMATION SYSTEM!
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

/* HELPER FUNCTIONS */

/* simple xor encryption/decryption*/
void encryptPassword(String password) //done
{

	int n = strlen(password);
	for(int i=0; i<n; i++)
	{
		password[i] = password[i] ^ KEY;
	}
}

/* pause and clear screen - i didn't want to type it in everytime */
void pauseScreen() //done
{
	
	printf("--- PRESS ENTER TO PROCEED ---");
	while(getchar() != '\n')
	{}
	getchar();
	
	// system("cls"); //clears screen - this is for windows only, search for ways to 
	
	printf("\n==========================================\n\n"); //border to separate
}

/* search username function - linear search*/
int searchUser(User users[], int userCount, String currentUser) //done
{
	
	int index = -1; //flag variable
	
	for(int i=0; i<userCount; i++)
	{
		if(strcmp(users[i].username, currentUser) == 0)//checks array of usernames
		{
			index = i;
		}
	}
	return index; //returns index of found user, and -1 if unsuccessful
}

/* sort users - selection sort */
void sortUsers(User users[], int userCount) //done - i dont know if we need this? linear search naman kse
{
	int i, j, minIndex;
	
    for(i=0; i < userCount - 1; i++)
	{
    	minIndex = i;
        for(j = i + 1; j < userCount; j++)
		{
            if(strcmp(users[i].username, users[minIndex].username) < 0)
			{
                minIndex = j;
            }
        }
        if(minIndex != i)
		{
            User temp = users[i];
            users[i] = users[minIndex];
            users[minIndex] = temp;
        }
    }	
}

/* sort household function - selection sort*/
void sortHouseholds(Record households[][MAX_HH], int householdCount[], int userIndex) //done
{
    int i, j, minIndex;
    
    for(i=0; i < householdCount[userIndex] - 1; i++)
	{
    	minIndex = i;
        for(j = 0; j < householdCount[userIndex]; j++)
		{
            if(strcmp(households[userIndex][j].house, households[userIndex][minIndex].house) < 0)
			{
                minIndex = j;
            }
        }
        if(minIndex != i)
		{
            Record temp = households[userIndex][i];
            households[userIndex][i] = households[userIndex][minIndex];
            households[userIndex][minIndex] = temp;
        }
    }
}

/* FILE HANDLING FUNCTIONS */
int loadUsers(User users[]) //done
{
	int count = 0;
	FILE *fp;
	fp = fopen("users.txt", "r");
	
	if(fp == NULL)
	{
		printf("--- FILES NOT FOUND ---\n");
	}
	else
	{
		while(fscanf(fp, "%30s %30s", users[count].username, users[count].password) == 2)
		{
			count++;
		}
	}
	
	fclose(fp);
	
	return count;
}

void saveUsers(User users[], int userCount)
{
	FILE *fp;
	fp = fopen("users.txt", "w");
	
	for(int i = 0; i < userCount; i++) //loops through all users
	{
		fprintf(fp, "%s %s\n", users[i].username, users[i].password);
		// username password(encrypted)
	}
}

void loadHouseholds(User users[], int userCount, Record households[][MAX_HH], int householdCount[])
{
	
	FILE *fp;
	fp = fopen("records.txt", "r");
	String username;
	Record r;
	
	if(fp == NULL) //checks if the files exists
	{
		printf("--- FILES NOT FOUND ---\n");
	}
	else
	{
		while(fscanf(fp, "%30s %30s %.2lf %.2lf %.2lf", username,  r.house, r.monthlyBill, r.monthlykWh, r.roofsize) == 5) //scans file
		{
			int index = searchUser(users, userCount, username);
			
			if((index != -1)&&( householdCount[index] < MAX_HH)) // if the user exists and household has not exceeded the max
			{
				households[index][householdCount[index]] = r; //saves record data onto the array of structs
                householdCount[index]++; //increments the householdCount of a user
			}
		}
	}
	
	fclose(fp);
	
}

void saveHouseholds(User users[], int userCount, Record households[][MAX_HH], int householdCount[])
{
	FILE *fp;
	fp = fopen("records.txt","w");
	
	//for loops that writes on the file
	for(int i = 0; i < userCount; i++)
	{
		for(int j = 0; j < householdCount[i]; j++)
		{
			fprintf(fp, "%30s %30s %.2lf %.2lf %.2lf\n", users[i].username, households[i][j].house, households[i][j].monthlyBill, households[i][j].monthlykWh, households[i][j].roofsize);
			// username, house name, electricty bill, power kWh, roof
		}
	}
}

/* DATA FUNCTIONS - LOGIN SCREEN */

int registerUser(User users[], int * userCount)
{
	User u;
	int result = 1; //flag
	
	printf("+---------------------------------+\n");
	printf("| USER REGISTRATION               |\n");
	printf("+---------------------------------+\n");
	printf("--- ENTER USERNAME ---\n");
	printf("-> ");
	scanf("%30s", u.username);
	
	for(int i = 0; i < *userCount; i++) //loop that checks for duplicates
	{
		if(strcmp(u.username, users[i].username)==0)
		{
			result = 0;
		}
	}
	
	if(result == 0)
	{
		printf("--- THIS USERNAME ALREADY EXISTS ---\n");
	}
	else
	{
		printf("--- ENTER PASSWORD ---\n");
		printf("-> ");
		scanf(" %30s", u.password);
		encryptPassword(u.password); //call encryption function
		
		users[*userCount] = u;
		(*userCount)++;
		sortUsers(users, *userCount); //sorts it to keep it organized
		
	}
	
	if(result == 1)
	{
		printf("+---------------------------------+\n");
		printf("| REGISTRATION SUCESSFUL          |\n");
		printf("+---------------------------------+\n");
		
		saveUsers(users, *userCount);
	}
	
	return result;
	
}

int loginUser(User users[], int userCount, String currentUser)
{
	User u; //for user input (username & password)
	int result = 0; //returns 1 on success
	int attempts = 3;
	
	printf("+---------------------------------+\n");
	printf("| USER LOGIN                      |\n");
	printf("+---------------------------------+\n");
	printf("--- ENTER USERNAME ---\n");
	printf("-> ");
	scanf("%30s", u.username); //takes the username input
	
	int index = searchUser(users, userCount, u.username);
	if(index == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	else
	{
		while((attempts > 0)&&(result == 0))
		{
			printf("--- ENTER PASSWORD ---\n");
			printf("-> ");
			scanf("%30s", u.password);
			encryptPassword(u.password); //call encryption function
			
			if(strcmp(u.password, users[index].password) == 0)
			{
				strcpy(currentUser, users[index].username); //"logs-in" the user
				result = 1;
			}
			else
			{
				attempts--; //user gets 3 attempts
				printf("--- INCORRECT PASSWORD! %d ATTEMPTS REMAINING ---\n", attempts);
			}
			if(attempts == 0)
			{
				printf("--- TOO MANY FAILED ATTEMPTS! TRY PASSWORD RECOVERY ---\n");
			}
		}
		
	}
	
	if(result == 1)
	{
		printf("+---------------------------------+\n");
		printf("| LOGIN SUCESSFUL!                |\n");
		printf("+---------------------------------+\n");
	}
	
	return result;	
}

void passwordRecovery(User users[], int userCount)
{
	
	User u;
	
	printf("+---------------------------------+\n");
	printf("| USER LOGIN                      |\n");
	printf("+---------------------------------+\n");
	printf("--- ENTER USERNAME ---\n");
	printf("-> ");
	scanf("%30s", u.username); //takes the username input
	
	int index = searchUser(users, userCount, u.username);
	if(index == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	else
	{
		printf("--- ENTER NEW PASSWORD ---\n");
		printf("-> ");
		scanf("%30s", u.password);
		encryptPassword(u.password); //call encryption function
		
		strcpy(users[index].password, u.password);
		printf("+---------------------------------+\n");
		printf("| PASSWORD SUCESSFUL CHANGED!     |\n");
		printf("+---------------------------------+\n");
	}
	
	
	
}

/* DATA FUNCTIONS AND COMPUTATIONS - HOME PAGE */

void addHousehold()
{
	
}

void editHousehold()
{
	
}

void deleteHousehold()
{
	
}

void viewRecords()
{
	
}

/* computes and shows summary of households */
void viewSummary()
{
	
}

void loggedinUI()
{

	int select;
	int loggedin = 1;
	do
	{
	printf("+---------------------------------+\n");
	printf("| SOLAR ENERGY INFORMATION SYSTEM |\n");
	printf("+---------------------------------+\n");
	printf("|                                 |\n");
	printf("| 1. ADD HOUSEHOLD                |\n");
	printf("| 2. EDIT HOUSEHOLD RECORD        |\n");
	printf("| 3. DELETE HOUSEHOLD RECORD      |\n");
	printf("| 4. VIEW RECORDS                 |\n");
	printf("| 5. VIEW SUMMARY                 |\n");
	printf("|                                 |\n");
	printf("| 0. LOGOUT                       |\n");		
	printf("|                                 |\n");	
	printf("+---------------------------------+\n");
	printf ("-> ");
	scanf("%d", &select);
	
		switch(select)
		{
			case 1: //addHousehold(currentUser); //addHousehold
					pauseScreen();
			break;
			case 2: //printf("function not added yet\n"); //editHousehold
			break;
			case 3: //printf("function not added yet\n"); //deleteHousehold
			break;
			case 4: //viewRecords(currentUser); //viewRecords
					pauseScreen();
			break;
			case 5: printf("function not added yet\n"); //viewSummary
			break;
			case 0: 
						printf("+---------------------------------+\n");
						printf("| LOGGING OUT!                    |\n");
						printf("+---------------------------------+\n");
						pauseScreen();
			break;	
			default: printf("--- INVALID SELECTION! PLEASE TRY AGAIN! ---\n");
					pauseScreen();
			break;
		}
	}
	while(select != 0);
	
}

int main()
{
	
    User users[MAX_USER];
    Record households[MAX_USER][MAX_HH];
    int householdCount[MAX_USER] = {0};
    int userCount = 0;
    String currentUser;
	
	int loginSelect;
	int Rstatus, Lstatus;
	
	userCount = loadUsers(users);
	
	do
	{
		printf("+---------------------------------+\n");
		printf("| SOLAR ENERGY INFORMATION SYSTEM |\n");
		printf("+---------------------------------+\n");
		printf("|                                 |\n");
		printf("| 1. LOGIN                        |\n");
		printf("| 2. REGISTER                     |\n");
		printf("| 3. PASSWORD RECOVERY            |\n");
		printf("|                                 |\n");
		printf("| 0. EXIT PROGRAM                 |\n");		
		printf("|                                 |\n");	
		printf("+---------------------------------+\n");
		printf("-> ");
		scanf("%d", &loginSelect);

		switch(loginSelect)
		{
			
			case 1: //user login
			{
				loginUser(users, userCount, currentUser);
				pauseScreen();
			}
			break;
			
			case 2: //user registration
			{
				registerUser(users, &userCount);
				pauseScreen();
			}	
			break;
			
			case 3: //password recovery
			{
				passwordRecovery(users, userCount);
				pauseScreen();
			}
			break;
			
			case 0: //exit program
			{
				/* saves all file changes just incase */
				saveHouseholds(users, userCount, households, householdCount);
				saveUsers(users, userCount);
				
				printf("+---------------------------------+\n");
				printf("| EXITING PROGRAM!                |\n");
				printf("+---------------------------------+\n");
			}
			break;
			
			default: //invalid integer input
			{
				printf("--- INVALID SELECTION! PLEASE TRY AGAIN! ---\n");
				pauseScreen();
			}
			break;
		}
	}
	while(loginSelect != 0); //runs until logout

	
	return 0;
}