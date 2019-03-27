#include "KStar.hpp"
#include <iostream>
/*
 * KStar
 */

void KStar::setHeuristic(const KStar::HeuristicFunction &heuristic) {
	heuristic_ = heuristic;
}

KStar::ResolverContainer
KStar::resolvePuzzle(node_pointer start, const_node_pointer goal) {
//	std::cout << __PRETTY_FUNCTION__<< std::endl;

	/*
	 * Add start to open list
	 */
//	std::cout << heuristic_(start, goal) << std::endl;
	node_pointer temp = start;
	resolveCost(temp, goal);

	nodeOpenList.push(temp);

	std::cout << "Start node " << std::endl;
	std::cout << start->grid << std::endl;
	std::cout << "Goal node " << std::endl;
	std::cout << goal->grid << std::endl;
	while (!nodeOpenList.empty()) { // While isn't empty
		std::cout << nodeOpenList.size() << "\n\n";
//		std::cout << "Current Node List : " << std::endl;
//		for (const auto &item : nodeOpenList) {
//			std::cout << "H : " <<item.H << " F : " << item.F << " G : " << item.G << std::endl;

//		}
		const_node_pointer currentNode = nodeOpenList.top(); // Take smallest one
		std::cout << "Current Node work : " << std::endl;
		std::cout << GRN << currentNode->grid << RST <<  std::endl;
		std::cout << "H : " <<currentNode->H << " F : " << currentNode->F << " G : " << currentNode->G << std::endl;
		if (currentNode->grid == goal->grid)
			return buildSolution();
		else {
			nodeCloseList.emplace(currentNode);
			nodeOpenList.pop();

			Position blankTile = getPositionOf(currentNode, 0);
			for (const auto &dir : direction) {
				if (currentNode->grid.range(blankTile + dir)) {
					node_pointer node(std::make_shared<Node>(*currentNode));

					node->parent = currentNode;
					std::swap(node->grid(blankTile), node->grid(blankTile + dir));
					std::cout << node->grid << std::endl;
					resolveCost(node, goal);

					if (!isInClosedList(node)) {

						NodeContainer::const_iterator it;
						if ((it = std::find_if(nodeOpenList.cbegin(), nodeOpenList.cend(), [&node](const_node_pointer cmp){ return node->grid == cmp->grid; })) != nodeOpenList.end()) {
//							std::cout << "find in open" << std::endl;
							if (node->G < (*it)->G) {
//								std::cout << "swap H < H" << std::endl;
								nodeOpenList.erase(it);
								nodeOpenList.push(node);
							}
						} else {
							std::cout << "New node push : " << std::endl;
							std::cout << YEL << node->grid << RST <<  std::endl;
							std::cout << "H : " <<node->H << " F : 	" << node->F << " G : " << node->G << std::endl;
							nodeOpenList.push(node);
						}
					}
				}
			}
		}
//		std::cout <<std::endl;
//		std::cout <<std::endl;
//		std::string s;
//		std::getline(std::cin, s);
	}
	return KStar::ResolverContainer();
}


KStar::ResolverContainer KStar::buildSolution() {
	const_node_pointer node(nodeOpenList.top());
	ResolverContainer resolverContainer;
	resolverContainer.push_back(node->grid);
	while (node->parent) {
		resolverContainer.push_back(node->parent->grid);
		std::cout << node->F << ' ' << node->H  << std::endl;

		node = node->parent;
	}
	std::reverse(resolverContainer.begin(), resolverContainer.end());
	return resolverContainer;
}
void KStar::resolveCost(node_pointer node, const_node_pointer goal) {
	node->H = heuristic_(node, goal);
	node->G = (node->parent ? node->parent->G + 1 : 0);
	node->F = node->H + node->G;
}


Position KStar::getPositionOf(const_node_pointer node, ValuePuzzle value) {
	GridContainer::const_iterator cit;
	if ((cit = std::find(node->grid.cbegin(), node->grid.cend(), value)) != node->grid.cend()) {

		long index = std::distance(node->grid.cbegin(), cit);
		return Position(index % node->grid.getX(), index / node->grid.getY());
	}
	std::cout << node->grid << " Value : " << value << std::endl;
	assert(false);
	return Position();
}


bool KStar::isInClosedList(const_node_pointer node) const {
	return std::any_of(nodeCloseList.cbegin(), nodeCloseList.cend(), [node](const_node_pointer cmp){
		return node->grid == cmp->grid;
	});}

/*
 * static heuristic
 */

size_t
KStar::Heuristic::hamming(const_node_pointer start, const_node_pointer goal) {
	/*
	 * Hamming - Misplaced tile
	 * 1 cost each tile is misplaced to the goal
	 */
	GridContainer::const_iterator sit = start->grid.cbegin();
	GridContainer::const_iterator git = goal->grid.cbegin();
	size_t n = 0;
	for (; sit != start->grid.cend() ; ) {
		if (*sit && *sit != *git) ++n;
		++sit;
		++git;
	}
	return n;
}

size_t
KStar::Heuristic::manhattan(const_node_pointer start, const_node_pointer goal) {

	size_t H = 0;
	for (size_t index = 1; index < start->grid.size(); ++index) {
//		std::cout << "Start : " << start.grid << std::endl;
		H += getTravelCost(getPositionOf(start, index), getPositionOf(goal, index));
	}
	return H;
}

size_t
KStar::Heuristic::getTravelCost(const Position &source, const Position &target) {
//	std::cout << source << " to " << target << " equals to : " << std::abs(target.x - source.x) + std::abs(target.y - source.y) << std::endl;
	return std::abs(target.x - source.x) + std::abs(target.y - source.y);
}

size_t KStar::Heuristic::linearConflict(const_node_pointer start,
										const_node_pointer goal) {
	static const size_t COST = 2;
	size_t H = 0;
	for (size_t n = 1; n < start->grid.size(); ++n) {
		Position positionGoal = getPositionOf(goal, n);
		Position positionStart = getPositionOf(start, n);
		//n == 1
		if (positionGoal != positionStart &&
				start->grid(positionGoal) == goal->grid(positionStart) &&
				manhattan(start, goal) == 1) {
			H += COST;
		}
	}
	return H + manhattan(start, goal);
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

std::shared_ptr<KStar::Node> KStar::Builder::build() {
	assert(data_.size() > 0);
	return std::make_shared<Node>(data_, size_);
}

/*
 * Node
 */

KStar::Node::Node(const GridContainer &grid, const size_t &size) :
	grid(grid),
	size(size),
	H(0),
	G(0),
	F(0),
	parent(nullptr) {

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
	return grid == rhs.grid;
}

bool KStar::Node::operator!=(const KStar::Node &rhs) const {
	return !(rhs == *this);
}


bool KStar::Node::operator<(const KStar::Node &rhs) const {
	return F < rhs.F;
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
