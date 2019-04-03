#include <iostream>
#include <queue>
#include <resolver/KStar.hpp>
#include <parser/Parser.hpp>
#include <boost/program_options.hpp>
#include <visualizer/DisplaySfml.hpp>
#include <boost/filesystem.hpp>
#include <visualizer/Visualizer.hpp>

//Make Min Max

int main(int argc, char *argv[]) {

	const std::map<std::string, KStar::eHeuristic> map {
			{ "hamming", KStar::kHamming },
			{ "manhattan", KStar::kManhattan },
			{ "linear conflict", KStar::kLinearConflict },
			{ "euclidiean", KStar::kEuclidean },
	};

	try {
		/*
		 * Set option arguments
		 */
		boost::program_options::options_description desc("Options");
		desc.add_options()
				("file,f", boost::program_options::value<std::string>()->required(), "File to be parse")
				("visualizer,v", "Enable visualizer")
				("hamming,h", "Set heuristic to hamming")
				("manhattan,m", "Set heuristic to manhattan")
				("euclidean,e", "Set heuristic to euclidean")
				("linear,l", "Set heuristic to linear conflict")
				("force", "Ignore the solvability of the puzzle")
				("greedy,g", "Make the research greedy")
				("uniform,u", "Make the research uniform")
				("help", "display this message");

		boost::program_options::variables_map vm;

		boost::program_options::store(
				boost::program_options::parse_command_line(argc, argv, desc),
				vm);
		/*
		 * Help management
		 */
		if (vm.count("help")) {
			std::cout << "Basic Command Line Parameter App" << std::endl
					  << desc << std::endl;
			return EXIT_SUCCESS;
		}

		boost::program_options::notify(vm);

		KStar kStar;
		/*
		 * Set the greed
		 */

		kStar.setGreedy(static_cast<bool>(vm.count("greed")));
		kStar.setUniform(static_cast<bool>(vm.count("uniform")));

		/*
		 * Option to heuristic management
		 */
		if (std::all_of(map.begin(), map.end(), [&vm](std::pair<std::string, KStar::eHeuristic> pair){
			return vm.count(pair.first) == 0;
		})) {
			kStar.setHeuristic(KStar::Heuristic::manhattan);
		} else {
			for (const auto &option : map) {
				if (vm.count(option.first))
					kStar.setHeuristic(option.second);
			}
		}

		/*
		 * File to parser
		 */

		Parser parser;
		std::ifstream file;
		file.open(vm["file"].as<std::string>(), std::ifstream::in);
		if (file.is_open())
			parser.parseFile(file);
		else {
			std::cerr << "n-puzzle error: " << vm["file"].as<std::string>() <<
			        " : can not be open" << std::endl;
			return EXIT_FAILURE;
		}

		/*
		 * Builder Grid
		 */

		KStar::Builder builder;
		builder.setSize(parser.getSize());
		builder.setArray(parser.getGridContainer());
		KStar::const_node_pointer startNode = builder.build();
		/*
		 * Final goal Grid builder
		 */

		KStar::const_node_pointer goalNode = builder.buildGoalGrid();

		/*
		 * Check if solvable
		 */

		if (!KStar::isSovablePuzzle(startNode) && !static_cast<bool>(vm.count("force"))) {
			std::cerr << "Non solvable" << std::endl;
			exit(1);
		}

		/*
		 * Resolver
		 */
		assert(goalNode != nullptr);
		KStar::ResolverData resolverData = kStar.resolvePuzzle(builder.build(), goalNode);

		/*
		 * Display solution
		 */
		std::cout << "Size resolver :" << resolverData.resolverContainer.size() << std::endl;
		for (const auto &grid : resolverData.resolverContainer) {
			std::cout << grid << std::endl;
			std::cout << std::endl;
		}
		std::cout << "Number of move to reach final state : " << resolverData.numberOfMove << std::endl;
		std::cout << "Complexity in size : " << resolverData.complexityInSize << std::endl;
		std::cout << "Complexity in time : " << resolverData.complexityInTime << std::endl;

		/*
		 * GG WP
		 */
		if (vm.count("visualizer")) {
			Visualizer::Core visu(parser.getSize());
			visu.loop(resolverData);
		}


	} catch (const std::exception &e) {
		std::cout << "n-puzzle error: "<< e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}