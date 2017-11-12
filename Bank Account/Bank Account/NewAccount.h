#pragma once
#include <string>

namespace NewAccount
{
	void generateNewAccount();
	std::string generateAccountNum();
	std::string generateAccountPin();
	bool isUniqueAccountNum(const std::string &accountNum);

}