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

int registerUser()
{
	FILE *fp;
	User u;
	User temp;
	
	int found = 0; //variable to see if the username already exists . this avoids duplicates
	
	printf("+---------------------------------+\n");
	printf("| USER RESGISTRATION              |\n");
	printf("+---------------------------------+\n");
	printf("+=================================+\n");
	printf("|ENTER USERNAME                   |\n");
	printf("+=================================+\n");
	printf("-> ");
	scanf("%s", u.username);
	printf("+=================================+\n");
	printf("|ENTER PASSWORD                   |\n");
	printf("+=================================+\n");
	printf("-> ");
	scanf("%s", u.password);
	
	fp = fopen("users.txt","a");
	
	
	
}


//void loginUI()
//{
//	
//}


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
