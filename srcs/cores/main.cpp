#include <iostream>
#include <queue>
#include <resolver/KStar.hpp>
#include <parser/Parser.hpp>
#include <boost/program_options.hpp>

bool checkSolvable(GridContainer gridContainer) {
	std::map<int, int> m;
	std::cout << gridContainer << std::endl;
	int inversions = 0;

	for (int i = 0; i < gridContainer.size(); ++i) {
		for (int j= i + 1; j < gridContainer.size(); ++j){
			if (gridContainer[j] > gridContainer[i]) {
				inversions++;
			}
		}
	}

	return inversions % 2 == 1;
}

int main(int argc, char *argv[]) {


	int i[] = { 2 , 3 , 6, 21, 43 ,65 ,3};
	std::vector<int> v1 {i, i+7};
	std::vector<int> v2 {i, i+7};
	std::cout << "Equals : " << std::equal(v1.begin(), v1.end(), v2.begin(), v2.end()) << std::endl;
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
		std::cout << vm.count("file") << std::endl;
		Parser parser;
		std::cout << std::endl;
		std::ifstream file("f3");
		parser.parseFile(file);
		KStar kStar;
		kStar.setHeuristic(KStar::Heuristic::manhattan);
		KStar::Builder builder;
		builder.setSize(parser.getSize());
		builder.setArray(parser.getRawArray());
		std::cout << "Resolve Puzzle" << std::endl;
		KStar::Node node(builder.build());
		size_t n = 1;
		for (auto &item : node.grid) {
			item = n++;
		}
//		if (!checkSolvable(node.grid)) {
//			std::cout << "Non solvable"<< std::endl;
//			exit(1);
//		}
		node.grid(node.grid.getX() - 1, node.grid.getY() - 1) = 0;
		KStar::ResolverContainer resolverContainer = kStar.resolvePuzzle(
				builder.build(), node);
		std::priority_queue<int> e;
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