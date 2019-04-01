#include <utility>

#include "KStar.hpp"
#include <Grid.hpp>
#include <cassert>
/*
 * KStar
 */

const std::map<KStar::eHeuristic, KStar::HeuristicFunction> KStar::heuristicArray = {
		{kManhattan,		Heuristic::manhattan},
		{kHamming,			Heuristic::hamming},
		{kLinearConflict,	Heuristic::linearConflict},
		{kEuclidean,		Heuristic::euclidean},
		{kPatternDatabase,	Heuristic::patternDatabase}
};

void KStar::setHeuristic(const KStar::HeuristicFunction &heuristic) {
	heuristic_ = heuristic;

}


void KStar::setHeuristic(KStar::eHeuristic heuristic) {
	heuristic_ = heuristicArray.at(heuristic);
}

KStar::ResolverData
KStar::resolvePuzzle(node_pointer start, const_node_pointer goal) {

	if (!isSovablePuzzle(start)) {
		std::cout << "Not solvable" << std::endl;
		exit(1);
	}
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
//		std::cout << "Current Node List : " << std::endl;
//		for (const auto &item : nodeOpenList) {
//			std::cout << "H : " <<item->H << " F : " << item->F << " G : " << item->G << std::endl;
//		}
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
			std::cout << "Current node :\n" << currentNode->grid << std::endl << std::endl;
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
	std::reverse(resolverData.resolverContainer.begin(), resolverData.resolverContainer.end());
	resolverData.complexityInTime = nodeOpenList.size();
	resolverData.complexityInSize = nodeCloseList.size();
	resolverData.numberOfMove = resolverData.resolverContainer.size();
	return resolverData;
}

inline void
KStar::resolveCost(node_pointer_reference start, const_node_pointer_reference goal) {
	start->H = heuristic_(start, goal);
	std::cout << "H : " << start->H  << std::endl;
	start->G = (start->parent ? start->parent->G + 1 : 0);
	start->F = start->H + start->G;
}


inline Position
KStar::getPositionOf(const_node_pointer_reference node, ValuePuzzle value) {
	GridContainer::const_iterator cit;
	if ((cit = std::find(node->grid.cbegin(), node->grid.cend(), value)) !=
		node->grid.cend()) {

		long index = std::distance(node->grid.cbegin(), cit);
		return Position(index % node->grid.getX(), index / node->grid.getY());
	}
//	std::cout << node->grid << " Value : " << value << std::endl;
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

// 1  2  3  4
//12 13 14  5
//11  0 15  6
//10  9  8  7
const KStar::Heuristic::PatternDatabase KStar::Heuristic::patternDatabasePuzzle4[3] = {
				{ //Vector [0]
						{1, {0, 0}},
						{2, {1, 0}},
						{3, {2, 0}},
						{4, {3, 0}},

				},
				{ //vector [1]
						{12, {0, 1}},
						{11, {0, 2}},
						{10, {0, 3}},
						{9, {1, 3}},
						{13, {1, 1}},
						{14, {2, 1}},
						{5, {3, 1}},

				}, //vector[2]
				{
						{15, {2, 2}},
						{6, {3, 2}},
						{8, {2, 3}},
						{7, {3, 3}},

				}

};
inline size_t
KStar::Heuristic::patternDatabase(
		KStar::const_node_pointer start, KStar::const_node_pointer goal) {

	std::cout << "patternDatabase : \n" << start->grid << std::endl;
	for (const auto &databasePuzzle4 : patternDatabasePuzzle4) {
		if (!std::all_of(databasePuzzle4.begin(), databasePuzzle4.end(), [start](const PairValuePosition &pair){
			return start->grid(pair.second) == pair.first;
		}))	{
			std::cout << "On " << databasePuzzle4.size() << "pattern" << std::endl;
			if (databasePuzzle4.size() != 4)
				sleep(2);
			Cost H = 0;
			for (const auto &puzzle4 : databasePuzzle4) {
				H += manhattan_(getPositionOf(start, puzzle4.first), getPositionOf(goal, puzzle4.first));
			}
			return H;
		}
	}
	return 0;
}

inline size_t
KStar::Heuristic::manhattan(const_node_pointer start, const_node_pointer goal) {
	/*
	 * Manhattan
	 * Cost for travel distance between all tile
	 */
	Cost H = 0;
	for (size_t index = 1; index < start->grid.size(); ++index) {
//		std::cout << "Start : " << start.grid << std::endl;
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
//		std::cout << "Start : " << start.grid << std::endl;
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

size_t KStar::Heuristic::euclidean(KStar::const_node_pointer start,
								   KStar::const_node_pointer goal) {
	Cost H = 0;
	for (size_t n = 1; n < start->grid.size(); ++n) {
		H += euclidean_(getPositionOf(start, n), getPositionOf(goal, n));
	}
	return H;
}

inline size_t
KStar::Heuristic::hamming_(const Position &&start,const  Position &&goal) {
	return start != goal;
}

inline size_t
KStar::Heuristic::manhattan_(const Position &&start,const  Position &&goal) {
	return getTravelCost(start, goal);
}


inline size_t
KStar::Heuristic::euclidean_(const Position &&start,const  Position &&goal) {
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
		data( x, y ) = value;
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
			case KStar::Direction::eDirection::kEast: ++x; break;
			case KStar::Direction::eDirection::kSouth: ++y; break;
			case KStar::Direction::eDirection::kWest: --x; break;
			case KStar::Direction::eDirection::kNorth: --y; break;
		}
		value++;
	}
	return std::make_shared<KStar::Node>(data);
}

//0 1 2 3
//11     4
//10     5
//9 8 7 6
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

std::ostream &operator<<(std::ostream &os, const KStar::Direction &direction) {
	switch (direction.direction_) {
		case KStar::Direction::eDirection::kNorth: os << "kNorth"; break;
		case KStar::Direction::eDirection::kSouth: os << "kSouth"; break;
		case KStar::Direction::eDirection::kEast: os << "kEast"; break;
		case KStar::Direction::eDirection::kWest: os << "kWest"; break;
	}
	return os;
}

bool KStar::isSovablePuzzle(const_node_pointer node) const {
	size_t inversionCount = countInversionInPuzzle(node);
	if (node->grid.getX() & 1) {
		/*
		 * Odd part
		 * If N is odd, then puzzle instance is solvable
		 * if number of inversions is even in the input state.
		 */
		return !(inversionCount & 1);
	} else {
		size_t positionOfEmptyFromBottom = node->grid.getY() - getPositionOf(node, 0).y;
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

		return (positionOfEmptyFromBottom & 1) != 0 == !(inversionCount & 1);
	}
}

size_t
KStar::countInversionInPuzzle(const_node_pointer_reference node) const {
	Builder builder;
	builder.setSize(node->grid.getX());
	node_pointer nodePointer = builder.buildGoalGrid();
	size_t invCount = 0;
	/*
	 * Have to count
	 */
	for (int idx = 0; idx < node->grid.size(); ++idx) {
		for (int idx1 = idx + 1; idx1 < node->grid.size(); ++idx1) {
			invCount += isBetween(node->grid[idx1], node->grid[idx], nodePointer);
		}
	}
	return invCount;
}

bool KStar::isBetween(
		ValuePuzzle value,
		ValuePuzzle limit,
		node_pointer node) const {


	for (const auto &n : node->grid) {
		if (value == n)
			return true;
		if (limit == n)
			return false;
	}
	return false;
}

KStar::const_node_pointer
KStar::buildDatabase(KStar::const_node_pointer node, int step) {
	if (node->grid.getX() - step > 3) {
		GridContainer grid(node->grid);
		for (int y = 0; y < grid.getY(); ++y) {
			for (int x = 0; x < grid.getX(); ++x) {
				if (!(y == step || x == step))
					grid(x, y) = -1;
			}
		}
		return std::make_shared<const Node>(grid);
	}
	return node;
}

// 7 1 4 2 6 8 3 0 5
// 1 2 3 8 0 4 7 6 5



// v
//> 5 11  2  3 <
//  0  6 12  8
// 15  4  1 10
//  9 14 13  7
//  ^

// v
//> 1  2  3  4 <
// 12  6 12  8
// 11  4  1 10
// 10 14 13  7
//  ^











