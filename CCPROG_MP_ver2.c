// MANGAHAS && CARTAGO - CCPROG2 MP - SOLAR ENERGY INFORMATION SYSTEM! - VERSION 2

/*

This is to certify that this project is my own work, based on my personal
efforts in studying and applying the concepts learned. I have constructed the
functions and their respective algorithms and corresponding code by myself. The
program was run, tested, and debugged by my own efforts. I further certify that
I have not copied in part or whole or otherwise plagiarized the work of other
students and/or persons, nor did I employ the use of AI in any part of the
deliverable.

Members:
CARTAGO, Aki Nicole - ID: 12547298
MANGAHAS, Paul Nathan - ID: 12506974

Section: S19A

PROJECT TITLE: SOLAR ENERGY INFORMATION SYSTEM
SDG Integration: SDG 7 - Affordable and Clean Energy

PROGRAM DESCRIPTION:
This is a menu-driven C program that helps users understand their household electricity usage and 
evaluate whether switching to solar energy is practical and cost-effective for them. With the collected 
data from the user, such as electricity bills and power consumption, the system will analyze the data, 
recommend suitable solar panel options and costs, and calculate the estimated electricity savings.

*/
#include <stdio.h>
#include <string.h>

#define KEY 5 //encryption decryption key - do not change this or all encryptions will be ruined
#define MAX_STR 31 //max string length
#define MAX_USER 100 //max recorded users
#define MAX_HH 5 //max household

/* DATA STRUCTURES AND STRING TYPEDEF */

/* typedef: string variable wtih a max length of 30 characters */
typedef char String[MAX_STR];

/* structure: for user info */
typedef struct {
    String username;
    String password;
} User;

/* structure: for household record info */
typedef struct {
	
	String user;
    String house;
    double monthlyBill;
    double monthlykWh;
    double roofsize;
} Record;

/* FUNCTION DECLARATIONS */

/* HELPER FUNCTIONS */

/* ENCRYPTION FUNCTION
	- This function uses a simple xor operator to each character of a string
	@param password : the string that will be encrypted
	@return : none
*/
void encryptPassword(String password) //done
{

	int n = strlen(password);
	for(int i=0; i<n; i++)
	{
		password[i] = password[i] ^ KEY;
	}
}

/* PAUSE SCREEN
	- This function pauses the program and waits for user input
	- This will prompt the user to press Enter before proceeding, and prints a separator
	@param : none
	@return : none
*/
void pauseScreen() //done
{
	
	printf("\n--- PRESS ENTER TO PROCEED ---\n");
	while(getchar() != '\n')
	{}
	getchar();
	
	// system("cls"); //clears screen - this is for windows only, search for ways to 
	
	printf("\n=================================================\n\n"); //border to separate
}

/* SEARCH USER FUNCTION
	- This function uses linear search to find the index of a user inside a user array
	@param users : array of user structures
	@param userCount : number of existing users
	@param currentUser : username to search
	@return userIndex: returns the index of the found user in the array, if not found return -1
*/
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

/* SORT USER FUNCTION
	- This function uses selection sort, arranging the usernames alphabetically
	@param users : array of user structures
	@param userCount : number of existing users
	@return : none
*/
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

/* SEARCH HOUSEHOLD FUNCTION
	- This function uses selection sort, arranging the household names of a specific user alphabetically
	@param households : 2D array of household records
	@param householdCount: array for the number of households per user
	@param currentUser : username to search
	@return : none
*/
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

/* LOAD USERS FUNCTION
	- This function reads the usernames and passwords from "users.txt" into the users array
	@param users : array to store loaded users
	@return count : number of existing users from the file
*/
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
	
	return count; //returns number of existing users in the file
}

/* SAVE USERS FUNCTION
	- This function saves user data onto the "users.txt" file
	@param users : array of user structures
	@param userCount: number of users
	@return : none
*/
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

/* LOAD HOUSEHOLDS FUNCTION
	- This function reads the data from the "records.txt" file and assigns them to their corresponding users
	@param users : array of user structures
	@param userCount: number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@return : none
*/
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

/* SAVE HOUSEHOLDS FUNCTION
	- This function saves the household data onto the "records.txt" file
	@param users : array of user structures
	@param userCount: number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@return : none
*/
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

/* USER REGISTRATION
	- This function registers a new user. It prompts for a username and password, encrypts the password, and saves the user onto the array
	@param users : array of user structures
	@param *usercount : pointer to number of users
	@return result : 1 if successful, 0 if the user already exists
*/
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

/* USER LOGIN
	- This function logs-in a user. It prompts for a username and password and verifies if crendentials are valid
	@param users : array of user structures
	@param *usercount : pointer to number of users
	@param currentUser : stores the logged in username
	@return result : 1 if successful, 0 if the user already exists
*/
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

/* PASSWORD RECOVERY
	- This function changes a user's password, prompts for username and a new password then updates the password of that user
	@param users : array of user structures
	@param usercount : number of users
	@return : none
*/
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
	
	saveUsers(users, userCount);
	
}

/* DATA FUNCTIONS AND COMPUTATIONS - HOME PAGE */

/* ADD HOUSEHOLD RECORD
	- This function adds a new household, prompts for the different household data and stores that record under the logged-in user
	@param users : array of user structures
	@param usercount : number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@param currentUser : logged-in user
	@return : none
*/
void addHousehold(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	
	Record r;
	int userIndex = searchUser(users, userCount, currentUser);
	if(userIndex == -1) //checks if user exists
	{
		printf("--- USERNAME NOT FOUND ---\n");
	}
	else if(householdCount[userIndex] >= MAX_HH) //checks if max household limit is reached
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

/* EDIT HOUSEHOLD RECORD
	- This function edits 
	@param users : array of user structures
	@param usercount : number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@param currentUser : logged-in user
	@return : none
*/
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
		printf("--- SELECT HOUSEHOLD NUMBER TO EDIT ---\n");
		
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

/* DELETE HOUSEHOLD RECORD
	- This function deletes a household record of the logged in user, prompts to select which household to delete and confirmation
	@param users : array of user structures
	@param usercount : number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@param currentUser : logged-in user
	@return : none
*/
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
		printf("--- SELECT HOUSEHOLD NUMBER TO DELETE ---\n");
		
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
				scanf(" %c", &confirm);
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
					printf("\n--- INVALID SELECTION ---\n");
				}
			}while((confirm!='Y')&&(confirm!='y')&&(confirm!='N')&&(confirm!='n')); //loops until the user confirms whether to delete or not
		}		
			
	}	
}

/* VIEW RECORD
	- This function prints all household records of a specific user
	@param users : array of user structures
	@param usercount : number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@param currentUser : logged-in user
	@return : none
*/
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
		printf("--- NO RECORDS FOUND ---\n");
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

/* COMPUTATION / SUMMARY
	- This function calculates the estimated savings, costs of installation, energy output, and ROI on each for each household
	- It will reccomend a panel type best for energy output and another for savings
	- It will display the summary for all 3 panel types after the reccomendation
	@param users : array of user structures
	@param usercount : number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@param currentUser : logged-in user
	@return : none
*/
void viewSummary(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{
	
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
		
			printf("\n <<< HOUSEHOLD %d: %s >>>\n", i+1, households[userIndex][i].house);
			if((households[userIndex][i].monthlykWh <= 0)||(households[userIndex][i].roofsize <= 0)||(households[userIndex][i].monthlyBill <= 0))
			{
				printf("--- INVALID VALUE IN RECORDS ---\n");
			}
			else
			{
				/* Average kWh/m^2 per month generated by each solar panel type */
				float monoRate = 18; //monocrystalline
				float polyRate = 15; //polycrystalline
				float thinRate = 12; //thin-film
				
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
				
				/* estimated cost of solar panels per m^2 */
				double monoCost = 25000;
				double polyCost = 15000;
				double thinCost = 9000;
				
				/* cost of total installation */
				double monoInstall = usableRoof * monoCost;
				double polyInstall = usableRoof * polyCost;
				double thinInstall = usableRoof * thinCost;
				
				/* payback based on intallation cost and savings (months) */
				double monoPayback = monoInstall / monoSave;
				double polyPayback = polyInstall / polySave;
				double thinPayback = thinInstall / thinSave;
			
				
				/* Variables for the most optimal panel type */
				/* it is set to monocrystalline by default because it is the most optimal for the most part */
				String bestPanel;
				strcpy(bestPanel, "MONOCRYSTALLINE");
				double bestSave = monoSave;
				double bestInstall = monoInstall;
				
				if(polyInstall < bestInstall) //checks if poly is better than mono
				{
				    strcpy(bestPanel, "POLYCRYSTALLINE");
				    bestSave = polySave;
				    bestInstall = polyInstall;
				}	
				if(thinInstall < bestInstall) // checks if thin film is better than previous
				{
				    strcpy(bestPanel, "THIN-FILM");
				    bestSave = thinSave;
				    bestInstall = thinInstall;
				}
				
				/* best panel energy wise*/
				String energyPanel;
				strcpy(energyPanel, "MONOCRYSTALLINE");
				double maxEnergy = monokwh;
				
				if(polykwh > maxEnergy)
				{
				    maxEnergy = polykwh;
				    strcpy(energyPanel, "POLYCRYSTALLINE");
				}
				
				else if(thinkwh > maxEnergy)
				{
				    maxEnergy = thinkwh;
				    strcpy(energyPanel, "THIN-FILM");
				}
				
				printf("---------------------------------------------------------------------------\n");
				printf("| MOST COST EFFICIENT : %-49s |\n", bestPanel);
				printf("| MOST ENERGY GENERATED : %-47s |\n", energyPanel);
				printf("---------------------------------------------------------------------------\n");
				printf("| %-15s | %13s | %8s | %13s | %10s |\n", "PANEL TYPE", "COST (PHP)", "kWh", "SAVE (PHP)", "ROI(MOS)");
				printf("---------------------------------------------------------------------------\n");
				printf("| %-15s | %13.2lf | %8.2lf | %13.2lf | %10.2lf |\n", "MONOCRYSTALLINE", monoInstall, monokwh, monoSave, monoPayback);
				printf("| %-15s | %13.2lf | %8.2lf | %13.2lf | %10.2lf |\n", "POLYCRYSTALLINE", polyInstall, polykwh, polySave, polyPayback);
				printf("| %-15s | %13.2lf | %8.2lf | %13.2lf | %10.2lf |\n", "THIN-FILM", thinInstall, thinkwh, thinSave, thinPayback);
				printf("---------------------------------------------------------------------------\n");
			}
/*
//this decision algorithm is based on roof size, installation cost and roi wasnt included

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
*/		

/*
//i dont like having this one best because if we base it off a price, thin is always the best, but for energy, mono is the best.
		printf("-------------------------------------------------------------------------------------\n");
		printf("| RECOMMENDED PANEL : %s\n", bestPanel);
		printf("| INSTALLATION COST : %.2lf PHP\n", bestInstall);
		printf("| MONTHLY SAVINGS   : %.2lf PHP\n", bestSave);
		printf("| POWER GENERATED   : %.2lf kWh\n", bestkwh);
		printf("| PAYBACK PERIOD    : %.2lf months (%.2lf years)\n", bestPayback, bestPayback / 12); 
*/
		}
			
	}
		
}
	
	

/* LOGIN UI
	- This function dispalys the user interface after a sucessful login.
	- The user can select a number to perform the corresponding function
	@param users : array of user structures
	@param usercount : number of users
	@param households : 2D array of household records
	@param hosueholdCount: array for the number of households per user
	@param currentUser : logged-in user
	@return : none
*/
void loggedinUI(User users[], int userCount, Record households[][MAX_HH], int householdCount[], String currentUser)
{

	char select = -1;
	int loggedin = 1;
	do
	{
	printf("                        |\n");
	printf("                    .   |\n");
	printf("                        |\n");
	printf("          \\    *        |     *    .  /\n");
	printf("            \\        *  |  .        /\n");
	printf("         .    \\     ___---___     /    .\n");
	printf("                \\.--         --./\n");
	printf("     ~-_    *  ./               \\.   *   _-~\n");
	printf("        ~-_   /                   \\   _-~     *\n");
	printf("   *       ~-/                     \\-~\n");
	printf("     .      |                       |      .\n");
	printf("         * |                         | *\n");
	printf("+-----------------------------------------------+\n");
	printf("|         SOLAR ENERGY INFORMATION SYSTEM       |\n");
	printf("|-----------------------------------------------|\n");
	printf("|                                               |\n");
	printf("| 1. ADD HOUSEHOLD                              |\n");
	printf("| 2. EDIT HOUSEHOLD RECORD                      |\n");
	printf("| 3. DELETE HOUSEHOLD RECORD                    |\n");
	printf("| 4. VIEW RECORDS                               |\n");
	printf("| 5. VIEW SUMMARY                               |\n");
	printf("|                                               |\n");
	printf("| 0. LOGOUT                                     |\n");		
	printf("|                                               |\n");	
	printf("+-----------------------------------------------+\n");
	printf ("-> ");
	scanf(" %c", &select);
	
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

/* MAIN 
	- initializes data, loads files, and displays the log-in UI
	@param : none
	@return 0 : Terminate Program

*/

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
		printf("                        |\n");
	    printf("                    .   |\n");
	    printf("                        |\n");
	    printf("          \\    *        |     *    .  /\n");
	    printf("            \\        *  |  .        /\n");
	    printf("         .    \\     ___---___     /    .\n");
	    printf("                \\.--         --./\n");
	    printf("     ~-_    *  ./               \\.   *   _-~\n");
	    printf("        ~-_   /                   \\   _-~     *\n");
	    printf("   *       ~-/                     \\-~\n");
	    printf("     .      |                       |      .\n");
	    printf("         * |                         | *\n");
	    printf("+-----------------------------------------------+\n");
		printf("|         SOLAR ENERGY INFORMATION SYSTEM       |\n");
		printf("|-----------------------------------------------|\n");
		printf("|                                               |\n");
		printf("| 1. LOGIN                                      |\n");
		printf("| 2. REGISTER                                   |\n");
		printf("| 3. PASSWORD RECOVERY                          |\n");
		printf("|                                               |\n");
		printf("| 0. EXIT PROGRAM                               |\n");		
		printf("|                                               |\n");	
		printf("+-----------------------------------------------+\n");
		printf("-> ");
		scanf(" %c", &loginSelect);

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