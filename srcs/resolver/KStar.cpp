#include <utility>

#include "KStar.hpp"
#include <Grid.hpp>
#include <cassert>
#include <array>

/*
 * KStar
 */

const std::map<KStar::eHeuristic, KStar::HeuristicFunction> KStar::heuristicArray = {
		{kManhattan,       Heuristic::manhattan},
		{kHamming,         Heuristic::hamming},
		{kLinearConflict,  Heuristic::linearConflict},
		{kEuclidean,       Heuristic::euclidean},
};

const std::map<KStar::eHeuristic, std::string> KStar::heuristicNameArray = {
		{kManhattan,		"Manhattan"},
		{kHamming,			"Hamming"},
		{kLinearConflict,	"LinearConflict"},
		{kEuclidean,		"Euclidean"}
};

void KStar::setHeuristic(const KStar::HeuristicFunction &heuristic) {
	heuristic_ = heuristic;

}


void KStar::setHeuristic(KStar::eHeuristic heuristic) {
	heuristic_ = heuristicArray.at(heuristic);
}

KStar::ResolverData
KStar::resolvePuzzle(node_pointer start, const_node_pointer goal) {

	std ::cout << "Trying to resolve ... " << std::endl;
	/*
	 * Add start to open list
	 */

	resolveCost(start, goal);
	nodeOpenList.push(start);

	/*
	 * Pre allocation memory
	 */

	nodeOpenList.reserve(start->grid.size() * 10);

	while (!nodeOpenList.empty()) { // While isn't empty

		/*
		 * Take smallest node
		 */
		const_node_pointer currentNode = nodeOpenList.top();
		/*
		 * If find goal node
		 */
		if (currentNode->H == 0)
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
					std::swap(node->grid(blankTile),
							  node->grid(blankTile + dir));

					if (!isInClosedList(node)) {
						resolveCost(node,
									goal); // Calc cost if not in close list

						NodeContainer::const_iterator it;
						if ((it = std::find_if(nodeOpenList.cbegin(),
											   nodeOpenList.cend(),
											   [&node](const_node_pointer cmp) {
												   return node->grid ==
														  cmp->grid;
											   }
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
							nodeOpenList.push(node);
						}
					}
				}
			}
		}
	}
	return KStar::ResolverData();
}


KStar::ResolverData KStar::buildSolution() {
	const_node_pointer node(nodeOpenList.top());
	ResolverData resolverData;
	std::memset(&resolverData, 0, sizeof(ResolverData));
	resolverData.resolverContainer.push_back(node->grid);
	while (node->parent) {
		resolverData.resolverContainer.push_back(node->parent->grid);
		node = node->parent;
	}
	std::reverse(resolverData.resolverContainer.begin(),
				 resolverData.resolverContainer.end());
	resolverData.complexityInTime = nodeOpenList.size();
	resolverData.complexityInSize = nodeCloseList.size();
	resolverData.numberOfMove = resolverData.resolverContainer.size();
	for (auto &pair : heuristicArray) {
		if (pair.second == heuristic_) {
			resolverData.nameHeuristic = heuristicNameArray.at(pair.first);
			break;
		}
	}
	return resolverData;
}

inline void
KStar::resolveCost(node_pointer_reference start,
				   const_node_pointer_reference goal) {
	start->H = heuristic_(start, goal);
	start->G = (start->parent ? start->parent->G + 1 : 0);
	start->F = (isUniform() ? 0 : start->H) + (isGreedy() ? 0 : start->G);
}


inline Position
KStar::getPositionOf(const_node_pointer_reference node, ValuePuzzle value) {
	GridContainer::const_iterator cit;
	if ((cit = std::find(node->grid.cbegin(), node->grid.cend(), value)) !=
		node->grid.cend()) {

		long index = std::distance(node->grid.cbegin(), cit);
		return Position(index % node->grid.getX(), index / node->grid.getY());
	}
	assert(false);
	return Position();
}


inline bool
KStar::isInClosedList(const_node_pointer node) const {
	return std::any_of(nodeCloseList.cbegin(), nodeCloseList.cend(),
					   [node](const_node_pointer cmp) {
						   return node->grid == cmp->grid;
					   });
}

/*
 * static heuristic
 */

inline size_t
KStar::Heuristic::manhattan(const_node_pointer start, const_node_pointer goal) {
	/*
	 * Manhattan
	 * Cost for travel distance between all tile
	 */
	Cost H = 0;
	for (size_t index = 1; index < start->grid.size(); ++index) {
		H += manhattan_(getPositionOf(start, index),
						getPositionOf(goal, index));
	}
	return H;
}

inline size_t
KStar::Heuristic::hamming(const_node_pointer start, const_node_pointer goal) {
	/*
	 * Hamming - Misplaced tile
	 * 1 cost each tile is misplaced to the goal
	 */
	Cost H = 0;
	for (size_t index = 1; index < start->grid.size(); ++index) {
		H += hamming_(getPositionOf(start, index),
					  getPositionOf(goal, index));
	}
	return H;
}

inline size_t
KStar::Heuristic::getTravelCost(const Position &source,
								const Position &target) {
	return std::abs(target.x - source.x) + std::abs(target.y - source.y);
}

inline size_t
KStar::Heuristic::linearConflict(const_node_pointer start,
								 const_node_pointer goal) {
	/*
	 * Linear Conflict = Manhattan
	 * Cost for travel distance between all tile &&
	 * 2 cost if 2 tile have their own position inverse
	 */
	static const size_t COST = 2;
	Cost H = 0;
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

inline size_t
KStar::Heuristic::euclidean(KStar::const_node_pointer start,
								   KStar::const_node_pointer goal) {
	Cost H = 0;
	for (size_t n = 1; n < start->grid.size(); ++n) {
		H += euclidean_(getPositionOf(start, n), getPositionOf(goal, n));
	}
	return H;
}

inline size_t
KStar::Heuristic::hamming_(const Position &&start, const Position &&goal) {
	return start != goal;
}

inline size_t
KStar::Heuristic::manhattan_(const Position &&start, const Position &&goal) {
	return getTravelCost(start, goal);
}


inline size_t
KStar::Heuristic::euclidean_(const Position &&start, const Position &&goal) {
	size_t dx = std::abs(start.x - goal.x);
	size_t dy = std::abs(start.y - goal.y);
	return std::sqrt(dx * dx + dy * dy);
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
	for (const auto &item : data_) {
		if (item < 0 || item >= data_.size())
			throw std::runtime_error("Size too much");
	}
	return std::make_shared<Node>(data_);
}

std::shared_ptr<KStar::Node> KStar::Builder::buildGoalGrid() {
	GridContainer data(size_);
	Direction direction_(KStar::Direction::eDirection::kEast);
	size_t offset = 0;
	size_t steps = data.getX();
	size_t x = 0;
	size_t y = 0;
	ValuePuzzle value = 1;
	int changeOffsetCount = 1; // usually change offset after 2 direction change, but at the beginning is after the first direction change
	data.fill(0);
	while (value < data.size()) {
		data(x, y) = value;
		steps--;
		if (steps == 0) {
			++direction_;
			changeOffsetCount--;
			if (changeOffsetCount == 0) {
				changeOffsetCount = 2;
				offset++;
			}
			steps = data.getX() - offset;
		}
		//	int direction = 0; // 0 = right, 1 = down, 2 = left, 3 = up

		switch (direction_.direction_) {
			case KStar::Direction::eDirection::kEast:
				++x;
				break;
			case KStar::Direction::eDirection::kSouth:
				++y;
				break;
			case KStar::Direction::eDirection::kWest:
				--x;
				break;
			case KStar::Direction::eDirection::kNorth:
				--y;
				break;
		}
		value++;
	}
	return std::make_shared<KStar::Node>(data);
}

/*
 * Node
 */

KStar::Node::Node(const GridContainer &grid) :
		grid(grid),
		H(0),
		G(0),
		F(0),
		parent(nullptr) {

}

std::ostream &operator<<(std::ostream &os, const KStar::Node &node) {
	os << " size_: " << node.grid.size() << std::endl;
	for (size_t index = 0; index < node.grid.size(); ++index) {
		for (size_t idx = 0; idx < node.grid.size(); ++idx) {
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
	if (direction_ == kNorth) {
		direction_ = kEast;
	} else {
		int temp = static_cast<int>(direction_);
		direction_ = static_cast<eDirection >(++temp);
	}
	return *this;
}

bool KStar::isSovablePuzzle(const_node_pointer node) {
	size_t inversionCount = countInversionInPuzzle(node);
	if (node->grid.getX() & 1) {
		/*
		 * Odd part
		 * If N is odd, then puzzle instance is solvable
		 * if number of inversions is even in the input state.
		 */

		return !(inversionCount & 1);
	} else {
		size_t positionOfEmptyFromBottom =
				node->grid.getY() - getPositionOf(node, 0).y;
		/*
		 * Even part
		 *
		 * the blank is on an even row counting from the bottom (second-last, fourth-last, etc.)
		 * and number of inversions is odd.
		 *
		 * the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.)
		 * and number of inversions is even.
		 *
		 */
		return (positionOfEmptyFromBottom & 1) ? (inversionCount & 1) : !(inversionCount & 1);
	}
}
//1 2 3 8 4 7 6 5
//8 5 6 1 7 2 4 3
//3 6 5 0 3 0 1 0 = 18
size_t
KStar::countInversionInPuzzle(const_node_pointer_reference node) {
	Builder builder;
	builder.setSize(node->grid.getX());
	node_pointer nodePointer = builder.buildGoalGrid();
	size_t invCount = 0;
	/*
	 * Have to count
	 */
	for (int idx = 0; idx < node->grid.size(); ++idx) {
		for (int idx1 = idx + 1; idx1 < node->grid.size(); ++idx1) {
			if (node->grid[idx1] != 0 && node->grid[idx] != 0)
				invCount += isBetween(node->grid[idx1], node->grid[idx],
								  nodePointer);
		}
	}
	return invCount;
}

bool KStar::isBetween(
		ValuePuzzle value,
		ValuePuzzle limit,
		node_pointer node) {


	for (const auto &n : node->grid) {
		if (n == 0)
			continue;
		if (value == n)
			return true;
		if (limit == n)
			return false;
	}
	return false;
}

void KStar::setGreedy(bool greedy) {
	greedy_ = greedy;
}

bool KStar::isGreedy() const {
	return greedy_;
}

void KStar::setUniform(bool uniform) {
	uniform_ = uniform;
}

bool KStar::isUniform() const {
	return uniform_;
}

