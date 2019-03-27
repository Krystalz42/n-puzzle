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
		kManhattan,
		kHamming,
		kLinearConflict
	};

	/*
	 * Typedef
	 */

	typedef std::vector<GridContainer> ResolverContainer;

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

		Node(const GridContainer &grid, const size_t &size);

		Node &operator=(const Node &rhs);

		bool operator==(const Node &rhs) const;

		bool operator!=(const Node &rhs) const;

		bool operator<(const Node &rhs) const;

		friend std::ostream &operator<<(std::ostream &os, const Node &node);

		GridContainer grid;
		size_t size;

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

	ResolverContainer
	resolvePuzzle(node_pointer start, const_node_pointer goal);

	ResolverContainer buildSolution();

	void resolveCost(node_pointer node, const_node_pointer_reference goal);

private:
	HeuristicFunction heuristic_;

	NodeContainer nodeOpenList;

	std::set<const_node_pointer> nodeCloseList;

	const Position direction[4] = {
			{-1, 0}, //North
			{1,  0}, // South
			{0,  -1}, // East
			{0,  1}, // West
	};
private:


	static Position getPositionOf(const_node_pointer node, ValuePuzzle value);

	bool isInClosedList(const_node_pointer node) const;

	/*
	 * Heuristic
	 */
public:
	class Heuristic {
	public:
		static size_t
		hamming(const_node_pointer start, const_node_pointer goal);

		static size_t
		manhattan(const_node_pointer start, const_node_pointer goal);

		static size_t
		linearConflict(const_node_pointer start, const_node_pointer goal);

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
