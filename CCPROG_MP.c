	// MANGAHAS && CARTAGO - CCPROG2 MP - SOLAR ENERGY INFORMATION SYSTEM!
	#include <stdio.h>
	#include <string.h>
	
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
		int key = 5;
		
		for(int i=0; i<strlen(password); i++)
		{
			password[i] = password[i] ^ key;
		}
	}
	
	int registerUser() //this function still errors on my end, need to debug
	{
		FILE *fp;
		User u;
		User temp; //variables to read the file
		int found; //variable to see if the username already exists . this avoids duplicates
		int result = 1;
		
		printf("+---------------------------------+\n");
		printf("| USER RESGISTRATION              |\n");
		printf("+---------------------------------+\n");
	
	
			found = 0;
			printf("--- ENTER USERNAME ---\n");
			printf("-> ");
			scanf("%30s", u.username);
			
			fp = fopen("users.txt","r"); // text file for the list of usernames and encrypted passwords / read & append
			
			if(fp!=NULL)
			{
				while(fscanf(fp, "%s %s", temp.username, temp.password) == 2)
				{
					if(strcmp(temp.username, u.username)==0)
					{	
						printf("--- This username is taken --- \n");
						found = 1; //loops so user can enter a new username
					}
				}
				fclose(fp);
			}
			else
			{
				found = 0;
			}
			
		
		if(found==1)
		{
			result=0;
		}
		
		else
		{
			printf("--- ENTER PASSWORD ---\n");
			printf("-> ");
			scanf(" %30s", u.password);
			encryptPassword(u.password); //call encryption function
			
			fp = fopen("users.txt", "a");
			fprintf(fp,"%s %s\n", u.username, u.password);
			fclose(fp);
			
		}
		
		return result;
		
	}
	
	int main()
	{
	
		int loginSelect;
		int loginStatus = 1; //initialize for loop
		int status;
		
//		//LOGIN UI SCREEN
//		printf("+---------------------------------+\n");
//		printf("| SOLAR ENERGY INFORMATION SYSTEM |\n");
//		printf("+---------------------------------+\n");
//		printf("|                                 |\n");
//		printf("| 1. LOGIN                        |\n");
//		printf("| 2. REGISTER                     |\n");
//		printf("| 3. ACCOUNT RECOVERY             |\n");
//		printf("|                                 |\n");
//		printf("| 0. EXIT PROGRAM                 |\n");		
//		printf("|                                 |\n");	
//		printf("+---------------------------------+\n");	
		
		while(loginStatus)
		{
			//LOGIN UI SCREEN
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
				case 1: printf("LOGIN FUNCTION NOT YET INPUTED\n"); //LOGIN FUNCTION - NOT CREATED YET
				break;
				case 2:	status = registerUser();
						if(status==1)
						{
							printf("REGISTRATION SUCESSFUL!");
						printf("Press Enter to proceed...\n");
						while(getchar() != '\n');
						{
							getchar();
							printf("insert user UI"); //then implement all the login functions
						}
						}
				
				break;
				case 3: printf("PASSWORD RECOVERY FUNCTION NOT YET INPUTED\n"); //PW RECOVERY FUNCTION - NOT CREATED YET
				break;
				case 0: loginStatus = 0; //EXIT PROGRAM
				default: printf("INVALID SELECTION! PLEASE TRY AGAIN!\n"); //incase of wrong input
				break;
				}
			
		}
		
		return 0;
	}
