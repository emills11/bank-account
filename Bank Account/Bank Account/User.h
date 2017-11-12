#pragma once
#include <string>
#include <vector>

class User
{
private:
	std::string m_accountNum = "";
	std::string m_accountPin = "";
	std::string m_accountBalanceStr{ "0" };

	std::vector< std::string > m_accountInfo;

	double m_accountBalanceDbl{ 0.00 };
public:
	enum class AccountOption
	{
		NO_CHOICE,
		SUMMARY,
		WITHDRAWAL,
		DEPOSIT,
		LOGOUT
	};

	User(const std::string &accountNum = "");
	~User();

	void getInitAccountInfo(const std::string &accountNum);
	void displayAccountSummary();
	void displayAccountOptions();
	void makeWithdrawal();
	void makeDeposit();
	
	bool getAccountOption();
	
	std::string getAccountNum() const { return m_accountInfo[0]; }
	void setAccountNum(std::string accountNum) { m_accountInfo[0] = accountNum; }

	std::string getAccountPin() const { return m_accountInfo[1]; }
	void setAccountPin(std::string accountPin) { m_accountInfo[1] = accountPin; }

	std::string getAccountBalance() const { return m_accountInfo[2]; }
	void setAccountBalance(std::string accountBalance) { m_accountInfo[2] = accountBalance; }

	double getAccountBalanceDbl() const { return m_accountBalanceDbl; }
	void setAccountBalanceDbl(double accountBalanceDbl) { m_accountBalanceDbl = accountBalanceDbl; }
};