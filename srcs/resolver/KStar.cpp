#include "KStar.hpp"
#include <iostream>
/*
 * KStar
 */

const int DIM = 2;

void KStar::setHeuristic(const KStar::HeuristicFunction &heuristic) {
	heuristic_ = heuristic;
}

KStar::ResolverContainer
KStar::resolvePuzzle(const KStar::Node &start, const KStar::Node &goal) {
	nodeOpenList.push(start);
	while (!nodeOpenList.empty()) {
		Node currentNode = nodeOpenList.top();
		if (currentNode == goal)
			return KStar::ResolverContainer(); //todo finish
		else
			;
	}
	return KStar::ResolverContainer();
}

void KStar::addLESVOISINS(const KStar::Node &node) {
}

bool KStar::isInClosedList(const KStar::Node &node) const {
	return std::any_of(nodeCloseList.begin(),nodeCloseList.end(), [node](const KStar::Node &cmp){
		return node == cmp;
	});
}

bool KStar::isInOpenedList(const KStar::Node &node) const {
	return std::any_of(nodeOpenList.begin(),nodeOpenList.end(), [node](const KStar::Node &cmp){
		return node == cmp;
	});
}

/*
 * static heuristic
 */

double
KStar::Heuristic::hamming(const KStar::Node &start, const KStar::Node &goal) {
	size_t n = 0;
	return n;
}

double
KStar::Heuristic::manhattan(const KStar::Node &start, const KStar::Node &goal) {
	return 0;
}

/*
 * Builder, create node
 * */

KStar::Builder &KStar::Builder::setSize(const size_t size) {
	size_ = size;
	return *this;
}

KStar::Builder &KStar::Builder::setArray(const RawArray rawArray) {
	data_.resize(size_, size_);
	for (size_t index_array = 0; index_array < size_; ++index_array) {
		for (size_t index = 0; index < size_; ++index) {
			data_(index_array,index) = rawArray[index_array][index];
		}
	}
	return *this;
}


KStar::Node KStar::Builder::build() {
	assert(data_.size() > 0);
	return KStar::Node(data_, size_);
}

/*
 * Node
 */

KStar::Node::Node(const KStar::Container &grid, const size_t &size) :
	grid(grid),
	size(size) {

}

std::ostream &operator<<(std::ostream &os, const KStar::Node &node) {
	os << " size_: " << node.size << std::endl;
	for (size_t index = 0; index < node.size; ++index) {
		for (size_t idx = 0; idx < node.size; ++idx) {
			os << node.grid(index, idx) << ' ';
		}
		os << std::endl;
	}
	return os;
}


bool KStar::Node::operator==(const KStar::Node &rhs) const {
	return grid == rhs.grid &&
		   size == rhs.size;
}

bool KStar::Node::operator!=(const KStar::Node &rhs) const {
	return !(rhs == *this);
}

KStar::Node::Node(const KStar::Node &node) {
	*this = node;
}

KStar::Node &KStar::Node::operator=(const KStar::Node &rhs) {
	if (this != &rhs) {
		size = rhs.size;
		grid = rhs.grid;
	}
	return *this;
}

bool KStar::Node::operator<(const KStar::Node &rhs) const {
	return H < rhs.H;
}

bool KStar::Node::operator>(const KStar::Node &rhs) const {
	return rhs < *this;
}

bool KStar::Node::operator<=(const KStar::Node &rhs) const {
	return !(rhs < *this);
}

bool KStar::Node::operator>=(const KStar::Node &rhs) const {
	return !(*this < rhs);
}
