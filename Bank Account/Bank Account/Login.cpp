#include "Login.h"
#include "Database.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

void Login::getLoginInfo(std::string &accountNumOut) //return account number and account pin by reference
{
	accountNumOut = Login::getAccountNum();
	Login::getAccountPin();
}

std::string Login::getAccountNum() //retrieve account number from user
{
	while (true)
	{
		std::cout << "Please enter your account number: ";
		std::string accountNum;
		std::cin >> accountNum;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Invalid account number.\n";
		}
		else if (!isValidAccountNum(accountNum))
		{
			std::cout << "Invalid account number.\n";
		}
		else
		{
			std::cin.ignore(32767, '\n');
			return accountNum;
		}
	}
}

std::string Login::getAccountPin() //retrieve account pin from user
{
	while (true)
	{
		std::cout << "Please enter your account PIN: ";
		std::string accountPin;
		std::cin >> accountPin;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Invalid PIN.\n";
		}
		else if (!isValidAccountPin(accountPin))
		{
			std::cout << "Invalid PIN.\n";
		}
		else
		{
			std::cin.ignore(32767, '\n');
			return accountPin;
		}
	}
}

bool Login::isValidAccountNum(const std::string &accountNum) //checks to make sure the user entered an account number that's in 'Accounts.db'
{
	Database *db = new Database("Accounts.db");

	vector<vector<string> > result = db->query("SELECT num FROM accounts;");
	for (vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		if (row.at(0) == accountNum)
		{
			db->close();
			return true;
		}
	}

	db->close();
	return false;
}

bool Login::isValidAccountPin(const std::string &accountPin) //checks to see if entered PIN matches with account number
{
	Database *db = new Database("Accounts.db");

	vector<vector<string> > result = db->query("SELECT pin FROM accounts;");
	for (vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		if (row.at(0) == accountPin)
		{
			db->close();
			return true;
		}
	}

	db->close();
	return false;
}