#include <iostream>
#include <queue>
#include <resolver/KStar.hpp>
#include <parser/Parser.hpp>
#include <boost/program_options.hpp>
#include <PriorityQueueIter.hpp>

int main(int argc, char *argv[]) {

	try {
		boost::program_options::options_description desc("Options");
		desc.add_options()
				("file", "File to be parse")
				("hamming", "set heuristic to hamming")
				("help", "display this message");

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
		if (vm.count("help")) {
			std::cout << "Basic Command Line Parameter App" << std::endl
					  << desc << std::endl;
		}
		std::cout << vm.count("file") << std::endl;
		Parser parser;
		std::cout  << std::endl;
		std::ifstream file("../f2");
		parser.parseFile(file);
		KStar kStar;
		kStar.setHeuristic(KStar::Heuristic::hamming);
		KStar::Builder builder;
		builder.setSize(parser.getSize());
		builder.setArray(parser.getRawArray());
		std::cout << "Resolve Puzzle" << std::endl;
		KStar::Node node(builder.build());
		size_t n = 1;
		for (auto &item : node.grid) {
			item = n++;
		}
		node.grid(node.grid.getX() - 1, node.grid.getY() - 1) = 0;
		kStar.resolvePuzzle(builder.build(), node);

		boost::program_options::notify(vm);
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}