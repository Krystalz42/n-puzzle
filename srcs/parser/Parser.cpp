#include <zconf.h>
#include "Parser.hpp"
#include <cassert>
#include <unordered_set>

//
//
//
Parser::Parser() :
		size_(0),
		current_(0) {
	regex[kCommentary] = std::regex(R"(^#.*$)");
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
	std::unordered_set<int> s;
	std::copy(gridContainer.begin(), gridContainer.end(), std::inserter(s, s.begin()));

	if (gridContainer.size() != s.size()) {
		std::string error("n-puzzle error: there is two same value on puzzle");
		throw std::runtime_error(error);
	}
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
			std::regex r(R"([0-9 ]+)");
			if (!std::regex_match(string.begin(), string.end(), r)) {
				std::string buffer("line have wrong format : ");
				buffer += '\'';
				buffer += string;
				buffer += '\'';
				throw std::runtime_error(buffer);
			}
			if (current_ % size_) {
				std::string buffer("missing args on line : ");
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
	if (size_)
		throw std::runtime_error("size already set");
	size_ = std::stoul(string);
	if (size_ < 3)
		throw std::runtime_error("size can't be less than 3");
	gridContainer.resize(size_, size_);
}

void Parser::arrayLineWork(const std::string &string) {
	if (size_ == 0)
		throw std::runtime_error(std::string("missing size"));
	if (!gridContainer.range(current_ % size_, current_ / size_))
		throw std::runtime_error(std::string("too much args"));
	gridContainer(current_ % size_, current_ / size_) = std::stoul(string);
	current_++;
}

/*
 * Parser Error
 */
