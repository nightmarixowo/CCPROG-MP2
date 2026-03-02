// MANGAHAS && CARTAGO - CCPROG2 MP - SOLAR ENERGY INFORMATION SYSTEM!
#include <stdio.h>
#include <string.h>

/* FUNCTION DECLRATIONS */


int main()
{

	int loginSelect;
	int loginStatus = 1;
	
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
	
	while(loginStatus)
	{

		printf("-> ");
		scanf("%d", &loginSelect);

		switch(loginSelect)
			{
			case 1: printf("LOGIN FUNCTION NOT YET INPUTED\n"); //LOGIN FUNCTION - NOT CREATED YET
			break;
			case 2: printf("REGISTER FUNCTION NOT YET INPUTED\n"); //REGISTER FUNCTION - NOT CREATED YET
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
