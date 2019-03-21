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

	class Node;

	/*
	 * Typedef
	 */

	typedef double Cost;

	typedef uint16_t value;

	typedef double (*HeuristicFunction)(Node const &start, Node const &goal);

	typedef std::vector<Container> ResolverContainer;

	typedef PriorityQueueIter<Node, std::vector<Node>> NodeContainer;

	/*
	 * KStar function
	 */

	void setHeuristic(const HeuristicFunction &heuristic);

	ResolverContainer resolvePuzzle(Node const &start, Node const &goal);

private:
	HeuristicFunction heuristic_;

	NodeContainer nodeOpenList;

	NodeContainer nodeCloseList;

private:

	void addNeighbour(const Node &node);

	bool isInClosedList(const Node &node) const;

	bool isInOpenedList(const Node &node) const;

public:
	/*
	 * Node
	 */

	class Node {
	public:
		Node() = delete;

		Node(const Node &node);

		Node(const Container &grid, const size_t &size);

		Node &operator=(const Node &rhs);

		bool operator==(const Node &rhs) const;

		bool operator!=(const Node &rhs) const;

		bool operator<(const Node &rhs) const;

		bool operator>(const Node &rhs) const;

		bool operator<=(const Node &rhs) const;

		bool operator>=(const Node &rhs) const;

		friend std::ostream &operator<<(std::ostream &os, const Node &node);

		Container grid;

		size_t size;

		Cost H;
		Cost J;
		Cost G;
	};

	/*
	 * Heuristic
	 */

	class Heuristic {
	public:
		static double hamming(Node const &start, Node const &goal);

		static double manhattan(Node const &start, Node const &goal);
	};

	/*
	 * Builder
	 */

	class Builder {
	public:
		Builder() = default;

		Builder &setSize(const size_t size);

		Builder &setArray(const Container &container);

		Node build();

	private:
		size_t size_;
		Container data_;
	};

};

#endif
