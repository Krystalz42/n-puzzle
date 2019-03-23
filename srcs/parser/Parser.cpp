#include <zconf.h>
#include "Parser.hpp"

//
//
//
Parser::Parser() :
		size_(0),
		current_(0) {
	regex[kCommentary] = std::regex(R"(^\s*#.*$)");
	regex[kSize] = std::regex(R"(^\s*[0-9]+$)");
	regex[kArrayLine] = std::regex(R"(\s*[0-9]+\s*)");
	workFunction[kCommentary] = &Parser::commentaryWork;
	workFunction[kSize] = &Parser::sizeWork;
	workFunction[kArrayLine] = &Parser::arrayLineWork;
}

void Parser::parseFile(std::ifstream &file) {
	std::string buffer;
	while (!file.eof()) {
		std::getline(file, buffer);
		if (!buffer.empty())
			parseLine(buffer);
	}
}

void Parser::parseLine(const std::string &string) {
	std::cout << string << std::endl;
	for (int index = 0; index < kArrayLine + 1; ++index) {
		std::sregex_iterator it = std::sregex_iterator(string.begin(), string.end(), regex[index]);
		if (it != std::sregex_iterator()) {
			for (; it != std::sregex_iterator() ; ++it) {
				std::smatch m = *it;
				(this->*workFunction[index])(m[0]);
			}
			break;
		}
	}
}

size_t Parser::getSize() const {
	return size_;
}

GridContainer Parser::getRawArray() const {
	return rawArray;
}

void Parser::commentaryWork(const std::string &string) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Parser::sizeWork(const std::string &string) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << string << std::endl;
	size_ = std::stoul(string);
	rawArray.resize(size_, size_);
}

void Parser::arrayLineWork(const std::string &string) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	assert(size_ != 0);
	std::cout << "Fill : " << current_ / size_ << " "<< current_ % size_ << std::endl;
	rawArray(current_ % size_, current_ / size_) = std::stoul(string);
	current_++;
}
