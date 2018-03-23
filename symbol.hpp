#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "key.hpp"

class Symbol {
	std::vector<Key> T;
	void construct_table(const std::vector<Key> &, int, int);
	void helper(int, std::string&, const Key&);
public:
	Symbol(const std::string&);
	void decrypt(const std::string&);
};

#endif