#include "NewAccount.h"
#include "Database.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

long long getRandomNumber(long long min, long long max) //generates a random number between min and max (inclusive)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  //static used for efficiency, so we only calculate this value once
																				 //evenly distribute the random number across our range
	return min + static_cast<long long>((max - min + 1) * (rand() * fraction));
}

void NewAccount::generateNewAccount() //sets up a new bank account (number/PIN/balance)
{
	std::cout << "Generating a new account...\n";

	std::string accountNum{ generateAccountNum() };
	std::string accountPin{ generateAccountPin() };

	sqlite3 *db;
	sqlite3_stmt *st = nullptr;

	std::string dbStatement = "INSERT INTO accounts VALUES (?, ?, 10);";

	if (sqlite3_open("Accounts.db", &db) == SQLITE_OK)
	{
		sqlite3_prepare_v2(db, dbStatement.c_str(), -1, &st, NULL);
		sqlite3_bind_text(st, 1, accountNum.c_str(), accountNum.length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(st, 2, accountPin.c_str(), accountPin.length(), SQLITE_TRANSIENT);
		sqlite3_step(st);
	}

	sqlite3_finalize(st);
	sqlite3_close(db);

	std::cout << "Your account number is: " << accountNum << "\n";
	std::cout << "Your account PIN is:    " << accountPin << "\n";
}

std::string NewAccount::generateAccountNum() //generates a random 12 digit account number that is insured to be unique
{
	while (true)
	{
		long long accountNum{ getRandomNumber(100'000'000'000, 999'999'999'999)};
		std::string accountNumStr{ std::to_string(accountNum) };
		
		if (NewAccount::isUniqueAccountNum(accountNumStr))
		{	
			return accountNumStr;
		}
	}
}

std::string NewAccount::generateAccountPin() //asks the user for a desired account PIN
{
	std::string accountPin{ "" };

	do
	{
		std::cout << "What would you like your PIN to be? ";
		std::cin >> accountPin;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Invalid PIN number.\n";
		}
	} while (accountPin.size() != 4 && isdigit(accountPin[4]));
	
	std::cin.ignore(32767, '\n');
	
	return accountPin;
}

bool NewAccount::isUniqueAccountNum(const std::string &accountNum) //checks to make sure that the generated account number does not already exist
{
	Database *db = new Database("Accounts.db");

	vector<vector<string> > result = db->query("SELECT num FROM accounts;");
	for (vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		if (row.at(0) == accountNum)
		{
			db->close();
			return false;
		}
	}

	db->close();
	return true;
}