#ifndef __N_PUZZLE_KSTAR__
#define __N_PUZZLE_KSTAR__

#include "KStar.hpp"
#include <boost/multi_array.hpp>
#include <ostream>
#include <n_puzzle.hpp>
#include <queue>
#include <PriorityQueueIter.hpp>
#include <Grid.hpp>

class KStar {
public:

	class Heuristic;

public:
	/*
	 * Node
	 */
	typedef size_t Cost;

	class Node {
	public:
		Node() = delete;

		Node(const Node &node);

		Node(const GridContainer &grid, const size_t &size);

		Node &operator=(const Node &rhs);

		bool operator==(const Node &rhs) const;

		bool operator!=(const Node &rhs) const;

		friend std::ostream &operator<<(std::ostream &os, const Node &node);

		GridContainer grid;
		size_t size;

		Node *parent;
		Cost H;
		Cost F;
		Cost G;
	};
	struct Compare {
		bool operator()(const Node &lhs, const Node &rhs) {
			return lhs.F > rhs.F;
		}
	};

	/*
	 * Typedef
	 */


	typedef size_t (*HeuristicFunction)(Node const &start, Node const &goal);

	typedef std::vector<GridContainer> ResolverContainer;

	typedef PriorityQueueIter<Node, std::vector<Node>, Compare> NodeContainer;

	/*
	 * KStar function
	 */

	void setHeuristic(const HeuristicFunction &heuristic);

	ResolverContainer resolvePuzzle(Node const &start, Node const &goal);

	void resolveCost(Node &node, const Node &goal);
private:
	HeuristicFunction heuristic_;

	NodeContainer nodeOpenList;

	NodeContainer nodeCloseList;

	const Position direction[4] = {
			{-1, 0}, //y, x
			{ 1, 0},
			{ 0, -1},
			{ 0, 1},
	};
private:


	static Position getPositionOf(const Node &node, ValuePuzzle value);
	bool isInClosedList(const Node &node) const;

	bool isInOpenedList(const Node &node) const;

	/*
	 * Heuristic
	 */
public:
	class Heuristic {
	public:
		static size_t hamming(const Node &start, const Node &goal);
		static size_t manhattan(const Node &start, const Node &goal);
		static size_t getTravelCost(const Position &source,
									const Position &target);
	};

	/*
	 * Builder
	 */

	class Builder {
	public:
		Builder() = default;

		Builder &setSize(const size_t size);

		Builder &setArray(const GridContainer &container);

		Node build();

	private:
		size_t size_;
		GridContainer data_;
	};

};

#endif
