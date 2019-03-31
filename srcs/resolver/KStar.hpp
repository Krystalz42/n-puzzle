#ifndef __N_PUZZLE_KSTAR__
#define __N_PUZZLE_KSTAR__

#include "KStar.hpp"
#include <ostream>
#include <n_puzzle.hpp>
#include <queue>
#include <PriorityQueue.hpp>
#include <Grid.hpp>
#include <deque>
#include <set>
#include <map>

class KStar {
public:

	class Node;

	struct Compare;

	enum eHeuristic {
		kManhattan = 0,    //0
		kHamming,        //1
		kLinearConflict,//2
		kEuclidean,        //3
		kPatternDatabase//4
	};

	/*
	 * Typedef
	 */

	typedef std::vector<GridContainer> ResolverContainer;

	struct ResolverData {
		//Path
		ResolverContainer resolverContainer;
		//Total number of states ever selected in the "opened" set (complexity in time)
		size_t complexityInTime;
		//Maximum number of states ever represented in memory at the same time during the search (complexity in size)
		size_t complexityInSize;
		//Number of moves required to transition from the initial state to the final state,according to the search
		size_t numberOfMove;
	};

	typedef std::shared_ptr<Node> node_pointer;

	typedef std::shared_ptr<const Node> const_node_pointer;

	typedef std::shared_ptr<const Node> &const_node_pointer_reference;

	typedef std::shared_ptr<Node> &node_pointer_reference;

	typedef size_t (*HeuristicFunction)(const_node_pointer start,
										const_node_pointer goal);

	typedef PriorityQueue<const_node_pointer, std::vector<const_node_pointer>, Compare> NodeContainer;

	typedef size_t Cost;

	/*
	 * Node
	 */

	class Node {
	public:
		Node() = delete;

		Node(const Node &node);

		Node(const GridContainer &grid);

		Node &operator=(const Node &rhs);

		bool operator==(const Node &rhs) const;

		bool operator!=(const Node &rhs) const;

		bool operator<(const Node &rhs) const;

		friend std::ostream &operator<<(std::ostream &os, const Node &node);

		GridContainer grid;

		const_node_pointer parent;
		Cost H;
		Cost F;
		Cost G;
	};

	struct Compare {
		bool operator()(const_node_pointer lhs, const_node_pointer rhs) const {
			return lhs->F < rhs->F;
		}
	};

	/*
	 * KStar function
	 */

	void setHeuristic(const HeuristicFunction &heuristic);

	void setHeuristic(eHeuristic heuristic);

	ResolverData
	resolvePuzzle(node_pointer start, const_node_pointer goal);

	ResolverData buildSolution();

	void
	resolveCost(node_pointer_reference start,
				const_node_pointer_reference goal);

	bool isSovablePuzzle(const_node_pointer node) const;

	size_t countInversionInPuzzle(const_node_pointer_reference node) const;

	bool
	isBetween(ValuePuzzle value, ValuePuzzle limit, node_pointer node) const;

private:
	HeuristicFunction heuristic_;

	NodeContainer nodeOpenList;

	std::set<const_node_pointer> nodeCloseList;

private:
	const Position direction[4] = {
			{-1, 0}, //North
			{1,  0}, // South
			{0,  -1}, // East
			{0,  1}, // West
	};

private:

	const_node_pointer buildDatabase(const_node_pointer node, int step);

	static Position
	getPositionOf(const_node_pointer_reference node, ValuePuzzle value);

	bool isInClosedList(const_node_pointer node) const;

	/*
	 * Heuristic
	 */
public:
	class Heuristic {
	public:
		typedef std::pair<ValuePuzzle, Position> PairValuePosition;
		typedef std::vector<PairValuePosition> PatternDatabase;

		static const PatternDatabase patternDatabasePuzzle4[3];

		static size_t
		hamming(const_node_pointer start, const_node_pointer goal);

		static size_t
		manhattan(const_node_pointer start, const_node_pointer goal);

		static size_t
		linearConflict(const_node_pointer start, const_node_pointer goal);

		static size_t
		euclidean(const_node_pointer start, const_node_pointer goal);

		static size_t
		patternDatabase(const_node_pointer start, const_node_pointer goal);

	private:

		static size_t
		hamming_(const Position &&start, const Position &&goal);

		static size_t
		manhattan_(const Position &&start, const Position &&goal);

		static size_t
		euclidean_(const Position &&start, const Position &&goal);

		static size_t
		getTravelCost(const Position &source, const Position &target);
	};

	static const std::map<eHeuristic, HeuristicFunction> heuristicArray;

	/*
	 * Builder
	 */

	class Direction {
	public:
		enum eDirection {
			kEast,
			kSouth,
			kWest,
			kNorth
		};

		Direction();

		Direction(eDirection direction);

		Direction &operator++();

		friend std::ostream &
		operator<<(std::ostream &os, const Direction &direction);

		eDirection direction_;
	};

	class Builder {
	public:
		Builder() = default;

		Builder &setSize(const size_t size);

		Builder &setArray(const GridContainer &container);

		std::shared_ptr<Node> buildGoalGrid();

		std::shared_ptr<Node> build();

	private:
		size_t size_;
		GridContainer data_;
	};

};

#endif
