#include <iostream>
#include <queue>
#include <resolver/KStar.hpp>
#include <parser/Parser.hpp>
#include <boost/program_options.hpp>

// A utility function to count inversions in given
// array 'arr[]'. Note that this function can be
// optimized to work in O(n Log n) time. The idea
// here is to keep code small and simple.
int getInvCount(GridContainer gridContainer)
{
	int inv_count = 0;

	for (size_t index = 0; index < gridContainer.size(); ++index)
	{
		for (int j = index + 1; j < gridContainer.size(); j++)
		{
			// count pairs(i, j) such that i appears
			// before j, but i > j.
			if (gridContainer[j] && gridContainer[index] && gridContainer[index] > gridContainer[j])
				inv_count++;
		}
	}
	return inv_count;
}

// find Position of blank from bottom
int findXPosition(GridContainer gridContainer)
{
	// start from bottom-right corner of matrix
	for (int y = gridContainer.getY() - 1; y >= 0; --y)
		for (int x = gridContainer.getX() - 1; x >= 0; --x)
			if (gridContainer(x, y) == 0)
				return gridContainer.getY() - y;
}

// This function returns true if given
// instance of N*N - 1 puzzle is solvable
bool isSolvable(GridContainer gridContainer)
{
	// Count inversions in given puzzle
	int invCount = getInvCount(gridContainer);

	// If grid is odd, return true if inversion
	// count is even.
	if (gridContainer.getY() & 1)
		return !(invCount & 1);
	else	 // grid is even
		return (findXPosition(gridContainer) & 1) != 0 == !(invCount & 1);
}

int main(int argc, char *argv[]) {


	try {
		boost::program_options::options_description desc("Options");
		desc.add_options()
				("file", "File to be parse")
				("hamming", "set heuristic to hamming")
				("help", "display this message");

		boost::program_options::variables_map vm;
		boost::program_options::store(
				boost::program_options::parse_command_line(argc, argv, desc),
				vm);
		if (vm.count("help")) {
			std::cout << "Basic Command Line Parameter App" << std::endl
					  << desc << std::endl;
		}
		Parser parser;
		std::ifstream file("f3");
		parser.parseFile(file);

		KStar kStar;
		kStar.setHeuristic(KStar::Heuristic::manhattan);

		KStar::Builder builder;
		builder.setSize(parser.getSize());
		builder.setArray(parser.getRawArray());
		KStar::node_pointer node(builder.build());
		node->grid = Grid<ValuePuzzle>({1,2,3,8,0,4,7,6,5});
		if (isSolvable(node->grid)) {
			std::cout << "Non solvable"<< std::endl;
			exit(1);
		}

		std::cout << "Resolve Puzzle" << std::endl;
		KStar::ResolverContainer resolverContainer = kStar.resolvePuzzle(
				builder.build(), node);
		for (const auto &grid : resolverContainer) {
			std::cout << grid << std::endl;
			std::cout << std::endl;
		}
		std::cout << "Size resolver :" << resolverContainer.size() << std::endl;
		boost::program_options::notify(vm);
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}