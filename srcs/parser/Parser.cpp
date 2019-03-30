#include <zconf.h>
#include "Parser.hpp"
#include <cassert>

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
	GridContainer temp(gridContainer);
	std::sort(temp.begin(), temp.end());
	GridContainer::const_iterator iterator = std::adjacent_find(temp.cbegin(), temp.cend());
//	if (iterator != gridContainer.cend()) {
//		std::string error("n-puzzle error: there is two same value on puzzle");
//		throw std::runtime_error(error);
//	}
}

void Parser::parseLine(const std::string &string) {
	for (int index = kCommentary; index < kArrayLine + 1; ++index) {
		std::sregex_iterator it = std::sregex_iterator(string.begin(), string.end(), regex[index]);
		if (it != std::sregex_iterator()) {
			for (; it != std::sregex_iterator() ; ++it) {
				std::smatch m = *it;
				(this->*workFunction[index])(m[0]);
			}
			break;
		}
		if (index == kArrayLine) {
			if (current_ % size_) {
				std::string buffer("n-puzzle error: missing args on line : ");
				buffer += string;
				throw std::runtime_error(buffer);
			}
		}
	}

}

size_t Parser::getSize() const {
	return size_;
}

GridContainer Parser::getGridContainer() const {
	return gridContainer;
}

void Parser::commentaryWork(const std::string &string) {

}

void Parser::sizeWork(const std::string &string) {
	size_ = std::stoul(string);
	gridContainer.resize(size_, size_);
}

void Parser::arrayLineWork(const std::string &string) {
	if (size_ == 0) {
		throw std::runtime_error(std::string("n-puzzle error: missing size"));
	}
	gridContainer(current_ % size_, current_ / size_) = std::stoul(string);
	current_++;
}

/*
 * Parser Error
 */
