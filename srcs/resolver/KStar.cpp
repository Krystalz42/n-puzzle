#include <utility>

#include "KStar.hpp"
#include <boost/container/flat_set.hpp>
/*
 * KStar
 */

const std::map<KStar::eHeuristic, KStar::HeuristicFunction> KStar::heuristicArray = {
		{kManhattan,      Heuristic::manhattan},
		{kHamming,        Heuristic::hamming},
		{kLinearConflict, Heuristic::linearConflict}
};

void KStar::setHeuristic(const KStar::HeuristicFunction &heuristic) {
	heuristic_ = heuristic;

}


void KStar::setHeuristic(KStar::eHeuristic heuristic) {
	heuristic_ = heuristicArray.at(heuristic);
}

KStar::ResolverContainer
KStar::resolvePuzzle(node_pointer start, const_node_pointer goal) {
	std::cout << __PRETTY_FUNCTION__<< std::endl;

	/*
	 * Add start to open list
	 */
//	std::cout << heuristic_(start, goal) << std::endl;
	assert(goal != nullptr);
	resolveCost(start, goal);
	assert(goal != nullptr);
	nodeOpenList.push(start);

	/*
	 * Pre allocation memory
	 */

	nodeOpenList.reserve(start->size * 10);

	while (!nodeOpenList.empty()) { // While isn't empty
//		std::cout << "Current Node List : " << std::endl;
//		for (const auto &item : nodeOpenList) {
//			std::cout << "H : " <<item->H << " F : " << item->F << " G : " << item->G << std::endl;
//		}
		/*
		 * Take smallest node
		 */
		const_node_pointer currentNode = nodeOpenList.top();
		std::cout << "Current Node work : " << std::endl;
		std::cout << GRN << currentNode->grid << RST << std::endl;
		std::cout << "H : " << currentNode->H << " F : " << currentNode->F
				  << " G : " << currentNode->G << std::endl;
		/*
		 * Find goal node
		 */
		if (currentNode->grid == goal->grid)
			return buildSolution();
		else {
			/*
			 * Put node in close list
			 */
			nodeCloseList.emplace(currentNode);
			nodeOpenList.pop();
			Position blankTile = getPositionOf(currentNode, 0);

			for (const auto &dir : direction) {
				if (currentNode->grid.range(blankTile + dir)) {

					/*
					 * Build new node
					 */
					node_pointer node(std::make_shared<Node>(*currentNode));
					node->parent = currentNode;
					std::swap(node->grid(blankTile), node->grid(blankTile + dir));

					if (!isInClosedList(node)) {
						resolveCost(node, goal); // Calc cost if not in close list

						NodeContainer::const_iterator it;
						if ((it = std::find_if(nodeOpenList.cbegin(),
											   nodeOpenList.cend(),
											   [&node](const_node_pointer cmp) {
							return node->grid == cmp->grid;}
							)) != nodeOpenList.end()) {

							/*
							 * If find replace it if the node is close to the
							 * start node
							 */
							if (node->G < (*it)->G) {
								nodeOpenList.erase(it);
								nodeOpenList.push(node);
							}
						} else {
							/*
							 * Push to open list
							 */
							std::cout << "New node push : " << std::endl;
							std::cout << YEL << node->grid << RST << std::endl;
							std::cout << "H : " << node->H << " F : 	"
									  << node->F << " G : " << node->G
									  << std::endl;

							nodeOpenList.push(node);
						}
					}
				}
			}
		}
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
		std::cout << node->F << ' ' << node->H << std::endl;

		node = node->parent;
	}
	std::reverse(resolverContainer.begin(), resolverContainer.end());
	return resolverContainer;
}

void KStar::resolveCost(node_pointer node, const_node_pointer_reference goal) {
	node->H = heuristic_(node, goal);
	node->G = (node->parent ? node->parent->G + 1 : 0);
	node->F = node->H + node->G;
}


Position KStar::getPositionOf(const_node_pointer node, ValuePuzzle value) {
	GridContainer::const_iterator cit;
	if ((cit = std::find(node->grid.cbegin(), node->grid.cend(), value)) !=
		node->grid.cend()) {

		long index = std::distance(node->grid.cbegin(), cit);
		return Position(index % node->grid.getX(), index / node->grid.getY());
	}
	std::cout << node->grid << " Value : " << value << std::endl;
	assert(false);
	return Position();
}


bool KStar::isInClosedList(const_node_pointer node) const {
	return std::any_of(nodeCloseList.cbegin(), nodeCloseList.cend(),
					   [node](const_node_pointer cmp) {
						   return node->grid == cmp->grid;
					   });
}

/*
 * static heuristic
 */

size_t
KStar::Heuristic::hamming(const_node_pointer start, const_node_pointer goal) {
	/*
	 * Hamming - Misplaced tile
	 * 1 cost each tile is misplaced to the goal
	 */
	auto sit = start->grid.cbegin();
	auto git = goal->grid.cbegin();
	size_t n = 0;
	for (; sit != start->grid.cend();) {
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
		H += getTravelCost(getPositionOf(start, index),
						   getPositionOf(goal, index));
	}
	return H;
}

size_t
KStar::Heuristic::getTravelCost(const Position &source,
								const Position &target) {
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

std::shared_ptr<KStar::Node> KStar::Builder::buildGoalGrid() {
	GridContainer data_(size_);
	Direction direction(KStar::Direction::eDirection::kEast);
	size_t offset = 0;
	size_t steps = data_.getX() - offset;
	size_t x = 0;
	size_t y = 0;
	ValuePuzzle value = 1;
	int changeOffsetCount = 1; // usually change offset after 2 direction change, but at the beginning is after the first direction change
	data_.fill(0);
	while (value < data_.size()) {
		std::cout << " Value : "<< value << std::endl;
		data_( x, y ) = value;
		steps--;
		if (steps == 0) {
			++direction;
			changeOffsetCount--;
			if (changeOffsetCount == 0) {
				changeOffsetCount = 2;
				offset++;
			}
			steps = data_.getX() - offset;
		}
		//	int direction = 0; // 0 = right, 1 = down, 2 = left, 3 = up

		switch (direction.direction_) {
			case KStar::Direction::eDirection::kEast: ++x; break;
			case KStar::Direction::eDirection::kSouth: ++y; break;
			case KStar::Direction::eDirection::kWest: --x; break;
			case KStar::Direction::eDirection::kNorth: --y; break;
		}
		value++;
	}
	std::cout << data_(1, 0) << std::endl;
	return std::make_shared<KStar::Node>(data_, size_);
}

//->
//|
//v
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

KStar::Direction::Direction() : direction_(kEast) {

}

KStar::Direction::Direction(KStar::Direction::eDirection direction)
		: direction_(direction) {

}

KStar::Direction &KStar::Direction::operator++() {
	int temp = static_cast<int>(direction_);
	direction_ = static_cast<eDirection >(++temp);
	return *this;
}

std::ostream &operator<<(std::ostream &os, const KStar::Direction &direction) {
	switch (direction.direction_) {
		case KStar::Direction::eDirection::kNorth: os << "kNorth"; break;
		case KStar::Direction::eDirection::kSouth: os << "kSouth"; break;
		case KStar::Direction::eDirection::kEast: os << "kEast"; break;
		case KStar::Direction::eDirection::kWest: os << "kWest"; break;
	}
	return os;
}
