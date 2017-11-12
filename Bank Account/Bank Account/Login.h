#pragma once
#include <string>

namespace Login
{
	void getLoginInfo(std::string &accountNumOut);
	std::string getAccountNum();
	std::string getAccountPin();
	bool isValidAccountNum(const std::string &accountNum);
	bool isValidAccountPin(const std::string &accountPin);
}