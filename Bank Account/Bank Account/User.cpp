#include "User.h"
#include "Database.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

//constructor for User object
User::User(const std::string &accountNum)
{
	//add cells to account info vector
	for (int i = 0; i < 3; i++)
		m_accountInfo.push_back("");

	getInitAccountInfo(accountNum);
}

//destructor for User object
User::~User()
{
}

//retrieve initial account info from database to be stored in vector
void User::getInitAccountInfo(const std::string &accountNum)
{
	//create new Database object pointing to 'Accounts.db'
	Database *db = new Database("Accounts.db");

	//iterate through accounts table until current user's account num is found
	vector<vector<string> > result = db->query("SELECT * FROM accounts;");
	for (vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		
		//if account num is found, fill vector with num, pin, and balance
		if (row.at(0) == accountNum)
		{
			m_accountInfo[0] = row.at(0);
			m_accountInfo[1] = row.at(1);
			m_accountInfo[2] = row.at(2);
			db->close();

			//set account balance double variable
			m_accountBalanceDbl = stod(m_accountInfo[2]);
			return;
		}
	}
	
	db->close();
}

//display info of current user's account
void User::displayAccountSummary()
{
	std::cout << "\n----------Account Summary----------\n";
	std::cout << " Account Number:   " << m_accountInfo[0] << "\n";
	std::cout << " Account PIN:      " << m_accountInfo[1] << "\n\n";
	std::cout << " Account Balance: $" << m_accountInfo[2] << "\n";
	std::cout << "-----------------------------------\n\n";
}

//display possible options of current user's account
void User::displayAccountOptions()
{
	std::cout << "Would you like to:\n";
	std::cout << "\t(1) Display Account Summary\n";
	std::cout << "\t(2) Make a Withdrawal\n";
	std::cout << "\t(3) Make a Deposit\n";
	std::cout << "\t(4) Logout of Account\n";
	
}

//retrieve chosen option
bool User::getAccountOption()
{
	User::AccountOption choice{ User::AccountOption::NO_CHOICE };

	//retrieve user's choice until it is valid
	do
	{
		std::cout << "Option: ";
		int choiceInt;
		std::cin >> choiceInt;
		choice = static_cast<User::AccountOption>(choiceInt);

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
		}
		else
		{
			std::cin.ignore(32767, '\n');
		}
	} while (choice != User::AccountOption::SUMMARY && choice != User::AccountOption::WITHDRAWAL
				&& choice != User::AccountOption::DEPOSIT && choice != User::AccountOption::LOGOUT);

	//handle different choices
	switch (choice)
	{
	case User::AccountOption::SUMMARY:
		User::displayAccountSummary();
		return true;
	case User::AccountOption::WITHDRAWAL:
		User::makeWithdrawal();
		return true;
	case User::AccountOption::DEPOSIT:
		User::makeDeposit();
		return true;
	case User::AccountOption::LOGOUT:
		return false;
	default:
		return true;
	}
}

//function for making withdrawal
void User::makeWithdrawal()
{
	while (true)
	{
		std::cout << "Withdrawal Amount: ";
		double withdrawalAmount;
		std::cin >> withdrawalAmount;

		if (std::cin.fail() || withdrawalAmount < 0.00 || withdrawalAmount > m_accountBalanceDbl)
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
		}
		else
		{
			std::cin.ignore(32767, '\n');

			//subtract withdrawal amount from balance double, then set string value in vector using stringstream
			m_accountBalanceDbl -= withdrawalAmount;
			stringstream stream;
			stream << fixed << setprecision(2) << m_accountBalanceDbl;
			m_accountInfo[2] = stream.str();

			sqlite3 *db;
			sqlite3_stmt *st = nullptr;

			//change balance value in database
			std::string dbStatement = "UPDATE accounts SET balance = ? WHERE num = ?;";

			if (sqlite3_open("Accounts.db", &db) == SQLITE_OK)
			{
				sqlite3_prepare_v2(db, dbStatement.c_str(), -1, &st, NULL);
				sqlite3_bind_text(st, 1, m_accountInfo[2].c_str(), m_accountInfo[2].length(), SQLITE_TRANSIENT);
				sqlite3_bind_text(st, 2, m_accountInfo[0].c_str(), m_accountInfo[0].length(), SQLITE_TRANSIENT);
				sqlite3_step(st);
			}

			sqlite3_finalize(st);
			sqlite3_close(db);

			return;
		}
	}
}

//function for making deposit
void User::makeDeposit()
{
	while (true)
	{
		std::cout << "Deposit Amount: ";
		double depositAmount;
		std::cin >> depositAmount;

		if (std::cin.fail() || depositAmount < 0)
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
		}
		else
		{
			std::cin.ignore(32767, '\n');

			//add deposit amount to balance double, then set string value in vector using stringstream
			m_accountBalanceDbl += depositAmount;
			stringstream stream;
			stream << fixed << setprecision(2) << m_accountBalanceDbl;
			m_accountInfo[2] = stream.str();

			sqlite3 *db;
			sqlite3_stmt *st = nullptr;

			//change balance value in database
			std::string dbStatement = "UPDATE accounts SET balance = ? WHERE num = ?;";

			if (sqlite3_open("Accounts.db", &db) == SQLITE_OK)
			{
				sqlite3_prepare_v2(db, dbStatement.c_str(), -1, &st, NULL);
				sqlite3_bind_text(st, 1, m_accountInfo[2].c_str(), m_accountInfo[2].length(), SQLITE_TRANSIENT);
				sqlite3_bind_text(st, 2, m_accountInfo[0].c_str(), m_accountInfo[0].length(), SQLITE_TRANSIENT);
				sqlite3_step(st);
			}

			sqlite3_finalize(st);
			sqlite3_close(db);

			return;
		}
	}
}