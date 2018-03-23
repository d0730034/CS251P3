#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "key.hpp"
#include "symbol.hpp"
#include "timer.hpp"

std::string me;
std::string encrypted;
std::string table_filename;
std::unordered_map<Key, int> symbol_t;
bool verbose = false;
int P;

Symbol::Symbol(const std::string& filename) {
	T.resize(N);
	std::string buffer;
    std::fstream input(filename.c_str(), std::ios::in);
    for (int i = 0; i < N; i++) {
        std::getline(input, buffer);
        T[i].set_string(buffer);
    }
    P = N/2;
    input.close();
	construct_table(T, 0, P);
	//std::cout<<"Table creation finished"<<std::endl;
}

void Symbol::decrypt(const std::string& encrypted){
	// insert your code here
	Key encrypted_key(encrypted);
	int start = P;
	int end = N;
	for (int i = 0; i < pow(2, end-start); i ++) {
		//std::cout<<i<<std::endl;
		Key k;
		for (int j = 0; j < end-start; j ++) {
			if ((i >> j) & 1) {
				k += T[end -1 - j];
			}
		}
		Key look = encrypted_key - k;
		if (symbol_t.count(look) != 0) {
			int c = 0;
			std::string s;
			for (int j = 0; j < N; j ++) {
				if (j < (N-P)) {
					if ((i >> j) & 1) {
						c += 1 << (j % 5);
					}
				}
				else {
					if ((symbol_t[look] >> (j - (N-P))) & 1) {
						c += 1 << (j % 5);
					}
				}
				if (j % 5 == 4) {
					s = ALPHABET[c] + s;
					c = 0;
				}
			}
			std::cout << s << std::endl;
		}
	}
}

void Symbol::construct_table(const std::vector<Key> & T, int start, int end) {
	for (int i = 0; i < pow(2, end-start); i ++) {
		//std::cout << i << std::endl;
		Key k;
		for (int j = 0; j < end-start; j ++) {
			if ((i >> j) & 1) {
				k += T[end -1 - j];
			}
		}
		symbol_t[k] = i;
	}
}

void usage(const std::string& error_msg="") {
	if (!error_msg.empty()) std::cout << "ERROR: " << error_msg << '\n';
	std::cout << me << ": Symbol table-based cracking of Subset-sum password"
		<< " with " << B << " bits precision\n"
	    << "USAGE: " << me << " <encrypted> <table file> [options]\n"
		<< "\nArguments:\n"
		<< " <encrypted>:   encrypted password to crack\n"
		<< " <table file>:  name of file containing the table to use\n"
		<< "\nOptions:\n"
		<< " -h|--help:     print this message\n"
		<< " -v|--verbose:  select verbose mode\n\n";
	exit(0);
}

void initialize(int argc, char* argv[]) {
	me = argv[0];
	if (argc < 3) usage("Missing arguments");
	encrypted = argv[1];
	table_filename = argv[2];
	for (int i=3; i<argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-h" || arg == "--help") usage();
		else if (arg == "-v" || arg == "--verbose") verbose = true;
		else usage("Unrecognized argument: " + arg);
	}
}


int main(int argc, char *argv[]){
	
	initialize(argc, argv);
	
	// insert your code here
	Symbol s(table_filename);
	s.decrypt(encrypted);
	return 0;
}
