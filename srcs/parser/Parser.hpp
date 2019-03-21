#ifndef __N_PUZZLE_PARSER__
#define __N_PUZZLE_PARSER__

#include "Parser.hpp"
#include <regex>
#include <iostream>
#include <fstream>
#include <regex>
#include <n_puzzle.hpp>

class Parser {
	using WorkFunction = void (Parser::*)(const std::string &string);
	enum Regex : int {
		kCommentary = 0,
		kSize,
		kArrayLine
	};
public:
	Parser();
	void parseFile(std::ifstream &file);
	size_t getSize() const;
	RawArray getRawArray() const;

private:
	void parseLine(const std::string &string);
	void commentaryWork(const std::string &string);
	void sizeWork(const std::string &string);
	void arrayLineWork(const std::string &string);
	std::smatch smatch;
	WorkFunction workFunction[3];
	std::regex regex[3];

	/*
	 * Data Variable
	 */
	size_t size_;
	size_t current_;
	RawArray rawArray;

};

#endif //__N_PUZZLE_PARSER__