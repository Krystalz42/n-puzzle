#include "KStar.hpp"
#include <iostream>
/*
 * KStar
 */

void KStar::setHeuristic(const KStar::HeuristicFunction &heuristic) {
	heuristic_ = heuristic;
}

KStar::ResolverContainer
KStar::resolvePuzzle(const KStar::Node &start, const KStar::Node &goal) {
	std::cout << __PRETTY_FUNCTION__<< std::endl;

	/*
	 * Add start to open list
	 */
	Node temp(start);
	resolveCost(temp, goal);

	nodeOpenList.push(temp);

	std::cout << "Start node " << std::endl;
	start.grid.print();
	std::cout << "Goal node " << std::endl;
	goal.grid.print();
	while (!nodeOpenList.empty()) { // While isn't empty
		std::cout << "Current Node work : " << std::endl;
		Node currentNode = nodeOpenList.top(); // Take smallest one
		currentNode.grid.print();
		std::cout << "H : " <<currentNode.H << " F : " << currentNode.F << " G : " << currentNode.G << std::endl;
		if (currentNode == goal)
			return KStar::ResolverContainer(); //todo finish
		else {
			nodeCloseList.push(currentNode);
			nodeOpenList.pop();
			Position blankTile = getPositionOf(currentNode, 0);
			for (const auto &dir : direction) {
				if (currentNode.grid.range(blankTile + dir)) {
					Node node(currentNode);
					node.parent = &currentNode;
					std::cout << "With : " << dir << " go to : " << (blankTile + dir) << std::endl;
					std::swap(node.grid(blankTile), node.grid(blankTile + dir));
					resolveCost(node, goal);

					if (!isInClosedList(node)) {

						NodeContainer::iterator it;
						if ((it = std::find(nodeOpenList.begin(), nodeOpenList.end(), node)) != nodeOpenList.end()) {
//							std::cout << "find in open" << std::endl;
//							if (node.H < it->H) {
//								std::cout << "swap H < H" << std::endl;
//								std::swap(node, *it);
//							}
						} else {
							nodeOpenList.push(node);
							std::cout << "Current node open list : " << std::endl;
							for (auto &item : nodeOpenList) {
								std::cout << item.F << ' ';
							}
							std::cout << std::endl;
						}
					}
				}
			}
		}
	}
	return KStar::ResolverContainer();
}


void KStar::resolveCost(KStar::Node &node, const Node &goal) {
	node.H = heuristic_(node, goal);
	node.G = (node.parent ? node.parent->G + 1 : 0);
	node.F = node.H + node.G;
}


Position KStar::getPositionOf(const KStar::Node &node, ValuePuzzle value) {
	GridContainer::const_iterator cit;
	if ((cit = std::find(node.grid.cbegin(), node.grid.cend(), value)) != node.grid.cend()) {

		long index = std::distance(node.grid.cbegin(), cit);
		Position p(index % node.grid.getX(), index / node.grid.getY());
		return p;
	}
	node.grid.print();
	assert(false);
	return Position();
}


bool KStar::isInClosedList(const KStar::Node &node) const {
	return std::find(nodeCloseList.begin(), nodeCloseList.end(), node) != nodeCloseList.end();
}

bool KStar::isInOpenedList(const KStar::Node &node) const {
	return std::any_of(nodeOpenList.begin(),nodeOpenList.end(), [node](const KStar::Node &cmp){
		return node == cmp;
	});
}

/*
 * static heuristic
 */

size_t
KStar::Heuristic::hamming(const KStar::Node &start, const KStar::Node &goal) {
	/*
	 * Hamming - Misplaced tile
	 * 1 cost each tile is misplaced to the goal
	 */
	auto sit = start.grid.cbegin();
	auto git = goal.grid.cbegin();
	size_t n = 0;
	for (; sit != start.grid.cend() ; ) {
		if (*sit && *git && *sit != *git) ++n;
		++sit;
		++git;
	}
	return n;
}

size_t
KStar::Heuristic::manhattan(const KStar::Node &start, const KStar::Node &goal) {

	size_t n = 0;
	for (size_t index = 1; index < start.grid.size(); ++index) {
		n += getTravelCost(getPositionOf(start, index), getPositionOf(goal, index));
	}
	std::cout << "Manhattan : " << n << std::endl;
	return n;
}

size_t
KStar::Heuristic::getTravelCost(const Position &source, const Position &target) {
//	std::cout << source << " to " << target << " equals to : " << std::abs(target.x - source.x) + std::abs(target.y - source.y) << std::endl;
	return std::abs(target.x - source.x) + std::abs(target.y - source.y);
}

/*
 * Builder, create node
 * */

KStar::Builder &KStar::Builder::setSize(const size_t size) {
	size_ = size;
	return *this;
}


KStar::Builder &KStar::Builder::setArray(const GridContainer &container) {
	data_ = container;
	return *this;
}

KStar::Node KStar::Builder::build() {
	assert(data_.size() > 0);
	return KStar::Node(data_, size_);
}

/*
 * Node
 */

KStar::Node::Node(const GridContainer &grid, const size_t &size) :
	grid(grid),
	size(size),
	H(0),
	G(0),
	F(0) {

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
		parent = rhs.parent;
		H = rhs.H;
		G = rhs.G;
		F = rhs.F;
	}
	return *this;
}

