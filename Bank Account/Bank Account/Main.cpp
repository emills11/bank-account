#include "Login.h"
#include "NewAccount.h"
#include "User.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <sys/stat.h>
#include "sqlite3.h"
#include "Database.h"

inline bool dbExists(const std::string &name) //checks if the database exists
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void runBankAccount() //handles banking system events and options
{
	while (true)
	{
		int choice{ 0 };

		//get user option of logging in or creating new account
		do
		{
			std::cout << "Would you like to login to an account (1) or create a new account (2)? ";
			std::cin >> choice;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(32767, '\n');
			}
			else
			{
				std::cin.ignore(32767, '\n');
			}

		} while (!(choice == 1 || choice == 2));

		//if user chooses to login, get login info then create User object
		if (choice == 1)
		{
			std::string accountNum{ "" };
			std::string accountPin{ "" };

			Login::getLoginInfo(accountNum);

			User user{ accountNum };
			user.displayAccountSummary();
			user.displayAccountOptions();

			//get account options while user is logged in
			while (user.getAccountOption())
			{
				user.displayAccountOptions();
			}
		}
		else
		{
			//creates a new account to  be inserted into database
			NewAccount::generateNewAccount();
		}
	}
}

int main()
{
	//initial random number seed (for account num)
	srand(static_cast<unsigned int>(time(0)));
	rand();

	//check if database exists
	if (!dbExists("Accounts.db"))
	{
		//if database is not found, create a new one
		Database *db = new Database("Accounts.db");
		db->query("CREATE TABLE accounts (num TEXT, pin TEXT, balance TEXT);");
		db->close();
	}

	runBankAccount();

	return 0;
}