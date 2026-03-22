// MANGAHAS && CARTAGO - CCPROG2 MP - SOLAR ENERGY INFORMATION SYSTEM! - VERSION 2
#include <stdio.h>
#include <string.h>

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
	
	String user;
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
	
	printf("\n--- PRESS ENTER TO PROCEED ---\n");
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
	int found = 0;
	
	for(int i=0; i<userCount; i++)
	{
		if((strcmp(users[i].username, currentUser) == 0)&&(found == 0))//checks array of usernames
		{
			index = i;
			found = 1;
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
            if(strcmp(users[j].username, users[minIndex].username) < 0)
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
        for(j = i+1; j < householdCount[userIndex]; j++)
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
	
	if(fp == NULL) //checks if files exists or not
	{
		printf("--- FILES NOT FOUND ---\n");
	}
	else
	{
		while(fscanf(fp, "%30s %30s", users[count].username, users[count].password) == 2) //user counter
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
	
	fclose(fp);
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
		while(fscanf(fp, "%30s %30s %lf %lf %lf", username,  r.house, &r.monthlyBill, &r.monthlykWh, &r.roofsize )== 5) //scans file
		{
			int index = searchUser(users, userCount, username);
			
			if((index != -1)&&( householdCount[index] < MAX_HH)) // if the user exists and household has not exceeded the max
			{
				strcpy(r.user, username);
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
	
	if(fp == NULL) //checks if files exists or not
	{
		printf("--- FILES NOT FOUND ---\n");
	}
	else
	{	
		//for loops that writes on the file
		for(int i = 0; i < userCount; i++)
		{
			for(int j = 0; j < householdCount[i]; j++)
			{
				fprintf(fp, "%s %s %.2lf %.2lf %.2lf\n", users[i].username, households[i][j].house, households[i][j].monthlyBill, households[i][j].monthlykWh, households[i][j].roofsize);
				// username, house name, electricty bill, power kWh, roof
			}
		}
	}
	fclose(fp);
	
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
		printf("| REGISTRATION SUCCESSFUL         |\n");
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

void addHousehold(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	
	Record r;
	int userIndex = searchUser(users, userCount, currentUser);
	if(userIndex == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	
	
	if(householdCount[userIndex] >= MAX_HH)
	{
		printf("--- MAX HOUSEHOLD LIMIT REACHED ---\n");
	}
	else
	{
		printf("+---------------------------------+\n");
		printf("| ADD HOUSEHOLD                   |\n");
		printf("+---------------------------------+\n");
		
		strcpy(r.user, currentUser); //copies logged in user for file
		
		/* each line is self-explanatory, just takes user input for each factor and records it*/
		printf("--- ENTER HOUSEHOLD NAME ---\n");
		printf("-> ");
		scanf("%30s", r.house);
		
		printf("--- ENTER MONTHLY ELECTRICITY BILL (PHP) ---\n");
		printf("-> ");
		scanf("%lf", &r.monthlyBill);
		
		printf("--- ENTER MONTHLY ENERGY CONSUMPTION (kWh) ---\n");
		printf("-> ");
		scanf("%lf", &r.monthlykWh);
		
		printf("--- ENTER ROOF SIZE (m^2) ---\n");
		printf("-> ");
		scanf("%lf", &r.roofsize);
		
		households[userIndex][householdCount[userIndex]] = r;
		householdCount[userIndex]++; 
	
		sortHouseholds(households, householdCount, userIndex);
		saveHouseholds(users, userCount, households, householdCount);
	}
	
}

void editHousehold(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	
	int select;
	int userIndex = searchUser(users, userCount, currentUser);
	if(userIndex == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	
	if(householdCount[userIndex] == 0)
	{
		printf("\n--- NO HOUSEHOLDS TO EDIT ---\n");
	}
	else	
	{
		printf("+---------------------------------+\n");
		printf("| EDIT HOUSEHOLD                  |\n");
		printf("+---------------------------------+\n");
		printf("--- SELECT HOUSEHOLD TO EDIT ---\n");
		
		for(int i = 0; i < householdCount[userIndex]; i++)
		{
			printf("%d. %s\n", i+1, households[userIndex][i].house);
		}
		
		printf("+---------------------------------+\n");
		printf("-> ");
		scanf("%d", &select);
		select--; //to match the index
		
		if((select < 0)||(select >= householdCount[userIndex]))
		{
			printf("--- INVALID SELECTION ---\n");
		}
		else
		{
			Record *r = &households[userIndex][select];	
			
			printf("--- ENTER HOUSEHOLD NAME ---\n");
			printf("-> ");
			scanf("%30s", r->house);
			
			printf("--- ENTER MONTHLY ELECTRICITY BILL (PHP) ---\n");
			printf("-> ");
			scanf("%lf", &r->monthlyBill);
			
			printf("--- ENTER MONTHLY ENERGY CONSUMPTION (kWh) ---\n");
			printf("-> ");
			scanf("%lf", &r->monthlykWh);
			
			printf("--- ENTER ROOF SIZE (m^2) ---\n");
			printf("-> ");
			scanf("%lf", &r->roofsize);
			
		sortHouseholds(households, householdCount, userIndex);
		saveHouseholds(users, userCount, households, householdCount);
		
		printf("+---------------------------------+\n");
		printf("| HOUSEHOLD UPDATED!              |\n");
		printf("+---------------------------------+\n");
		
		}		
			
	}
}

void deleteHousehold(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	int select;
	int userIndex = searchUser(users, userCount, currentUser);
	if(userIndex == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
		
	if(householdCount[userIndex] == 0)
	{
		printf("\n--- NO HOUSEHOLDS TO DELETE ---\n");
	}
	else	
	{
		printf("+---------------------------------+\n");
		printf("| DELETE HOUSEHOLD                  |\n");
		printf("+---------------------------------+\n");
		printf("--- SELECT HOUSEHOLD TO DELETE ---\n");
		
		for(int i = 0; i < householdCount[userIndex]; i++)
		{
			printf("%d. %s\n", i+1, households[userIndex][i].house);
		}
		
		printf("+---------------------------------+\n");
		printf("-> ");
		scanf("%d", &select);
		select--; //to match the index
		
		if((select < 0)||(select >= householdCount[userIndex]))
		{
			printf("--- INVALID SELECTION ---\n");
		}
		else
		{
			char confirm;
			
			printf("--- YOU ARE DELETING: %s ---\n", households[userIndex][select].house); //indicates which record is being deleted
			printf("--- CONFIRM DELETE? ENTER (Y/N) ---\n"); //confirmation message
			
			do
			{
				printf("-> ");
				scanf("%c", &confirm);
				if((confirm == 'Y')||(confirm == 'y'))
				{
					
					for(int j = select; j < householdCount[userIndex]-1; j++)
					{
						households[userIndex][j] = households[userIndex][j+1]; //shifts down the records, overriding the selected
					}
					householdCount[userIndex]--; //decrements the amount of existing users
					
					sortHouseholds(households, householdCount, userIndex);
					saveHouseholds(users, userCount, households, householdCount);
					printf("+---------------------------------+\n");
					printf("| RECORD SUCCESSFULLY DELETED!    |\n");
					printf("+---------------------------------+\n");
				}
				else if((confirm == 'N')||(confirm == 'n'))
				{
					printf("+---------------------------------+\n");
					printf("| RECORD DELETION CANCELLED!      |\n");
					printf("+---------------------------------+\n");	
				}
				else
				{
					printf("--- INVALID SELECTION ---");
				}
			}while((confirm!='Y')&&(confirm!='y')&&(confirm!='N')&&(confirm!='n')); //loops until the user confirms whether to delete or not
		}		
			
	}	
}

void viewRecords(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	Record r;
	printf("+---------------------------------+\n");
	printf("| HOUSEHOLDS                      |\n");
	printf("+---------------------------------+\n");
	
	int userIndex = searchUser(users, userCount, currentUser); //
//	int userIndex = 2; it works
	if(userIndex == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	
	if(householdCount[userIndex] == 0)
	{
		printf("--- NO RECORDS FOUND ---");
	}
	else
	{
		printf("<--------------------------------->\n");
		
		for(int i = 0; i < householdCount[userIndex]; i++)
		{
		    printf("Household Name: %s\n", households[userIndex][i].house);
		    printf("Monthly Bill: %.2f\n", households[userIndex][i].monthlyBill);
		    printf("Monthly kWh: %.2f\n", households[userIndex][i].monthlykWh);
		    printf("Roof Size: %.2f\n", households[userIndex][i].roofsize);
		    printf("<--------------------------------->\n");
		}
	}
}

/* computes and shows summary of households */
void viewSummary(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	
	/* Average kWh/m^2 per month generated by each solar panel type */
	float monoRate = 18; //monocrystalline
	float polyRate = 15; //polycrystalline
	float thinRate = 12; //thin-film
	
	int userIndex = searchUser(users, userCount, currentUser);
	if(userIndex == -1)
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	
	printf("+---------------------------------+\n");
	printf("| SUMMARY                         |\n");
	printf("+---------------------------------+\n");
	
	if(householdCount[userIndex] == 0)
	{
		printf("--- NO RECORDS FOUND ---\n");
	}
	else
	{
		for(int i = 0; i < householdCount[userIndex]; i++)//loops through all households, for their summary
		{
		
		printf("\n <<< HOUSEHOLD %d: %s\n", i+1, households[userIndex][i].house);
		
		/* Variables for the most optimal panel type */
		String bestPanel;
		strcpy(bestPanel, "");
		double bestkwh = 0;
		double bestSave = 0;
		double coverageFactor = 0.4; //assuming that only 40% of the roof can be used for solar panels
		double usableRoof = (households[userIndex][i].roofsize * coverageFactor);
		double unitRate = (households[userIndex][i].monthlyBill / households[userIndex][i].monthlykWh);
		
		/* Estimated power generated based on roofsize */
		double monokwh = usableRoof * monoRate;
		double polykwh = usableRoof * polyRate;
		double thinkwh = usableRoof * thinRate;
		
		/* kWh x cost of kwh */
		double monoSave = monokwh * unitRate;
		double polySave = polykwh * unitRate;
		double thinSave = thinkwh * unitRate;
		

		if(usableRoof < 40) //small roofs
		{
			strcpy(bestPanel, "MONOCRYSTALLINE");
			bestSave = monoSave;
			bestkwh = monokwh;
		}
		else if((usableRoof >= 40)&&(usableRoof <= 70)) //medium roofs could be either mono or poly
		{
			if(polySave <= monoSave)
			{
				strcpy(bestPanel, "MONOCRYSTALLINE");
				bestSave = monoSave;
				bestkwh = monokwh;
			}
			else
			{
				strcpy(bestPanel, "POLYCRYSTALLINE");
				bestSave = polySave;
				bestkwh = polykwh;
			}
		}
		else if((usableRoof >= 70)&&(usableRoof <= 100))
		{
			strcpy(bestPanel, "POLYCRYSTALLINE");
			bestSave = polySave;
			bestkwh = polykwh;
		}
		else if(usableRoof>= 100) //considered large roofsize
		{
			strcpy(bestPanel, "THIN-FILM");
			bestSave = thinSave;
			bestkwh = thinkwh;
		}
		
		printf("RECCOMENDED PANEL TYPE : %s\n", bestPanel);
		printf("ESTIMATED MONTHLY SAVINGS : %.2lf P\n", bestSave);
		printf("ESTIMATED MONTHLY POWER GENERATED : %.2lf kWh\n", bestkwh);
		
		/* not sure if i should to include this in the final but just prints out everything*/
		printf("---------------------------------------------------------------\n");
		printf("| MONOCRYSTALLINE | SAVINGS: %.2lf PHP | POWER: %.2lf kWh |\n", monoSave, monokwh);
		printf("---------------------------------------------------------------\n");
		printf("| POLYCRYSTALLINE | SAVINGS: %.2lf PHP | POWER: %.2lf kWh |\n", polySave, polykwh);
		printf("---------------------------------------------------------------\n");
		printf("| THIN-FILM       | SAVINGS: %.2lf PHP | POWER: %.2lf kWh |\n", thinSave, thinkwh);
		printf("---------------------------------------------------------------\n");

		printf("\n<--------------------------------->\n");
			
		}
	}		
}
	
	


void loggedinUI(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{

	char select = -1;
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
	scanf("%c", &select);
	
		switch(select)
		{
			case '1': 
			{
				addHousehold(users, userCount, households, householdCount, currentUser); //addHousehold
				pauseScreen();
			}
			break;
			case '2':
			{
			 	editHousehold(users, userCount, households, householdCount, currentUser);
			 	pauseScreen();
			}
			break;
			case '3': 
			{
				deleteHousehold(users, userCount, households, householdCount, currentUser);
				pauseScreen();
			}
			break;
			case '4':
			{
				viewRecords(users, userCount, households, householdCount, currentUser);
				pauseScreen();
			}
			break;
			case '5': 
			{
				viewSummary(users, userCount, households, householdCount, currentUser);
				pauseScreen();
			}
			break;
			case '0': 
			{
				printf("+---------------------------------+\n");
				printf("| LOGGING OUT!                    |\n");
				printf("+---------------------------------+\n");
				pauseScreen();
			}
			break;	
			default:
			{
				printf("--- INVALID SELECTION! PLEASE TRY AGAIN! ---\n");
				pauseScreen();
			}
			break;
		}
	}
	while(select != '0');
	
}

int main()
{
	
    User users[MAX_USER];
    Record households[MAX_USER][MAX_HH];
    int householdCount[MAX_USER] = {0};
    int userCount = 0;
    String currentUser;
	
	char loginSelect;
	int loginStatus;
	
	userCount = loadUsers(users);
	loadHouseholds(users, userCount, households, householdCount);
	
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
		scanf("%c", &loginSelect);

		switch(loginSelect)
		{
			
			case '1': //user login
			{
				loginStatus = loginUser(users, userCount, currentUser);
				pauseScreen();
				
				if(loginStatus == 1) //login sucessful, proceed to home page UI
				{
					loggedinUI(users, userCount, households, householdCount, currentUser);
				}
			}
			break;
			
			case '2': //user registration
			{
				registerUser(users, &userCount);
				pauseScreen();
			}	
			break;
			
			case '3': //password recovery
			{
				passwordRecovery(users, userCount);
				pauseScreen();
			}
			break;
			
			case '0': //exit program
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
	while(loginSelect != '0'); //runs until logout

	
	return 0;
}