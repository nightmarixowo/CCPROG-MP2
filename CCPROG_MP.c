	// MANGAHAS && CARTAGO - CCPROG2 MP - SOLAR ENERGY INFORMATION SYSTEM!
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* CONSTANTS */
#define KEY 5 // this key can be changed

/* DATA STRUCTURES*/
typedef struct
{
	char username[31];
 	char password[31];
} User;

typedef struct 
{
	char username[31];
	char house[31];
	double monthlyBill;
	double monthlykWh;
	double roofsize;

}Records;


/* FUNCTION DECLRATIONS */


void encryptPassword(char password[]) //simple xor encryption
{
	int key = KEY;
	
	for(int i=0; i<strlen(password); i++)
	{
		password[i] = password[i] ^ key;
	}
}

void pauseScreen() //helper function lang, i didnt want to type it everytime
{
	printf("--- PRESS ENTER TO PROCEED ---\n");
	while(getchar() != '\n')
	{}
	getchar();
	
	system("cls"); //clears screen
}

void sortRecords() //sorts records alphabetically by overwriting the file
{
	
}

int registerUser() //this function still errors on my end, need to debug
{
	FILE *fp;
	User u;
	User temp; //variables to read the file
	int found; //variable to see if the username already exists . this avoids duplicates
	int result = 1;
	
	printf("+---------------------------------+\n");
	printf("| USER REGISTRATION               |\n");
	printf("+---------------------------------+\n");


		found = 0;
		printf("--- ENTER USERNAME ---\n");
		printf("-> ");
		scanf("%30s", u.username);
		
		fp = fopen("users.txt","r"); // text file for the list of usernames and encrypted passwords / read & append
		
		if(fp!=NULL)//checks if the file exists
		{
			while(fscanf(fp, "%s %s", temp.username, temp.password) == 2)//loops through list of all usernames
			{
				if(strcmp(temp.username, u.username)==0)//checks to see if the username is already registered
				{	
					printf("--- This username is already taken --- \n");//output for username
					found = 1;
				}	
			}
			fclose(fp);
		}
		else
		{
			found = 0; // if the username is unique it will proceed
		}

	if(found==1)
	{
		result=0; // returns to main UI if the username exists
	}	
	else
	{
		printf("--- ENTER PASSWORD ---\n");
		printf("-> ");
		scanf(" %30s", u.password);
		encryptPassword(u.password); //call encryption function
		
		fp = fopen("users.txt", "a");
		fprintf(fp,"%s %s\n", u.username, u.password); //prints the registered username and encrypted password in the text file
		fclose(fp);
		
	}
	
	return result; //returns 1 if successful, and 0 if failed
	             
}

int loginUser(char currentUser[])
{
	/* Logic used is almost the same as the registration function with some differences */
	FILE *fp;
	User u;
	User stored;
	int found = 0;
	int result = 0;
	int attempts=3;

	
	printf("+---------------------------------+\n");
	printf("| USER LOGIN                      |\n");
	printf("+---------------------------------+\n");

	found = 0;
	printf("--- ENTER USERNAME ---\n");
	printf("-> ");
	scanf("%30s", u.username); //takes the username input
	
	fp = fopen("users.txt", "r");

		if(fp == NULL) // i added this late, but this makes it so it says if the file doesn't exists
		{
			printf("--- Files not Found ---\n"); // tested by using a different file name and works
			result = 0;
		}	
		else
		{
			while((found == 0)&&(fscanf(fp, "%s %s", stored.username, stored.password) == 2))//loops through list of all usernames
			{
				if(strcmp(stored.username, u.username)==0)//checks to see if the username is registered
				{	
					found = 1;
				}	
			}
			fclose(fp);
		}
		
		if(found == 0)
		{
			printf("--- Username does not exists ---\n");
			result = 0; // if the username is not registered
		}	
		else
		{
			while((attempts > 0)&&(result == 0))//the loop ends if correct password or no more attempts
			{
				attempts--; //removes an attempt from password
				printf("--- ENTER PASSWORD ---\n");
				printf("-> ");
				scanf(" %30s", u.password);
				encryptPassword(u.password);
				
				if(strcmp(u.password, stored.password)==0)
				{
					strcpy(currentUser, u.username);
					result = 1;
				}
				else
				{
					if(attempts>0)
					{
						printf("--- Password Incorrect! %d attempts remaining ---\n", attempts); // password attempt loops
					}
					else
					{
						printf("--- Too many failed attempts! Try Password Recovery ---\n"); // redirects back to main UI
					}
				}
			}
		}
	
	return result;
}

void addHousehold(char currentUser[])
{
	FILE *fp;
	Records r;
	
	printf("+---------------------------------+\n");
	printf("| ADD HOUSEHOLD                   |\n");
	printf("+---------------------------------+\n");
	
	fp = fopen("records.txt", "a"); //ask miss on this if we need if(fp==NULL)
	
	strcpy(r.username, currentUser); //makes the created record for this user only
	
	/* each line is self-explanatory, just takes user input for each factor and records it*/
	printf("--- ENTER HOUSEHOLD NAME ---\n");
	printf("-> ");
	scanf("%30s", r.house);
	
	printf("--- ENTER MONTHLY ELECTRICITY BILL () ---\n");
	printf("-> ");
	scanf("%lf", &r.monthlyBill);
	
	printf("--- ENTER MONTHLY ENERGY CONSUMPTION (kWh) ---\n");
	printf("-> ");
	scanf("%lf", &r.monthlykWh);
	
	printf("--- ENTER ROOF SIZE (m^2) ---\n");
	printf("-> ");
	scanf("%lf", &r.roofsize);
	
	fprintf(fp, "%s %s %.2lf %.2lf %.2lf\n", r.username, r.house, r.monthlyBill, r.monthlykWh, r.roofsize); //copies records to records.txt
	fclose(fp);
	             
}

void editHousehold(char currentUser[])
{
	FILE *fp;
	Records r;
	
	
	
}

void viewRecords(char currentUser[])
{
	FILE *fp;
    Records r;

    printf("+-------------------------------+\n");
    printf("| HOUSEHOLD RECORDS             |\n");
    printf("+-------------------------------+\n");

    fp = fopen("records.txt", "r");

    if(fp == NULL)
    {
        printf("--- No records found. ---\n");

    }
	else
	{
		   while(fscanf(fp, "%s %s %lf %lf %lf",
	        r.username, r.house,
	        &r.monthlyBill,
	        &r.monthlykWh,
	        &r.roofsize) == 5)
	    {
	    	
	    	if(strcmp(r.username, currentUser)==0)
	    	{
	    		printf("<--------------------------------->\n");
		        printf("Household Name: %s\n", r.house);
		        printf("Monthly Bill: %.2f\n", r.monthlyBill);
		        printf("Monthly kWh: %.2f\n", r.monthlykWh);
		        printf("Roof Size: %.2f\n", r.roofsize);
		        printf("<--------------------------------->\n");
	    	}
	    }
	}

    fclose(fp);
    
}


void loggedinUI(char currentUser[])
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
			case 1: addHousehold(currentUser); //addHousehold
					pauseScreen();
			break;
			case 2: printf("function not added yet\n"); //editHousehold
			break;
			case 3: printf("function not added yet\n"); //deleteHousehold
			break;
			case 4: viewRecords(currentUser); //viewRecords
					pauseScreen();
			break;
			case 5: printf("function not added yet\n"); //viewSummary
			break;
			case 0: //loggedin = 0; omit this. i changed it to a do while loop
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

	int loginSelect;
	int loginStatus = 1; //initialize for loop
	int Rstatus; //registser status
	int Lstatus; //logged in status
	char currentUser[31];
	
//LOGIN UI SCREEN
//	printf("+---------------------------------+\n");
//	printf("| SOLAR ENERGY INFORMATION SYSTEM |\n");
//	printf("+---------------------------------+\n");
//	printf("|                                 |\n");
//	printf("| 1. LOGIN                        |\n");
//	printf("| 2. REGISTER                     |\n");
//	printf("| 3. ACCOUNT RECOVERY             |\n");
//	printf("|                                 |\n");
//	printf("| 0. EXIT PROGRAM                 |\n");		
//	printf("|                                 |\n");	
//	printf("+---------------------------------+\n");	
	
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
			case 1: Lstatus = loginUser(currentUser); //LOGIN FUNCTION - NOT CREATED YET
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
			case 3: printf("PASSWORD RECOVERY FUNCTION NOT YET INPUTED\n"); //PW RECOVERY FUNCTION - NOT CREATED YET
			break;
			case 0: printf("--- EXITING PROGRAM! ---\n");
					loginStatus = 0; //EXIT PROGRAM
			break;
			default:
				{
			printf("--- INVALID SELECTION! PLEASE TRY AGAIN! ---\n"); //incase of wrong input
			pauseScreen();
				}
			break;
			}
		
	}
	
	return 0;
}