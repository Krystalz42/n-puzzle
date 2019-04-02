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
			{ "pattern database", KStar::kPatternDatabase }
	};

	try {
		/*
		 * Set option arguments
		 */
		boost::program_options::options_description desc("Options");
		desc.add_options()
				("file,f", boost::program_options::value<std::string>()->required(), "File to be parse")
				("hamming,h", "set heuristic to hamming")
				("visualizer,v", "Enable visualizer")
				("manhattan,m", "set heuristic to manhattan")
				("euclidean,e", "set heuristic to euclidean")
				("linear conflict,l", "set heuristic to linear conflict")
				("pattern database,p", "set pattern databse (only work with size > 3)")
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
		std::ifstream file(vm["file"].as<std::string>());
		if (file.is_open())
			parser.parseFile(file);
		else
			; //todo error file

		/*
		 * Builder Grid
		 */

		std::cout << "Hello" << std::endl;
		KStar::Builder builder;
		builder.setSize(parser.getSize());
		builder.setArray(parser.getGridContainer());

		/*
		 * Final goal Grid builder
		 */

		KStar::const_node_pointer node = builder.buildGoalGrid();

		/*
		 * Check if solvable
		 */

//		if (isSolvable(node->grid)) {
//			std::cout << "Non solvable" << std::endl;
//			exit(1);
//		}
		/*
		 * Resolver
		 */
		assert(node != nullptr);
		KStar::ResolverData resolverData = kStar.resolvePuzzle(builder.build(), node);

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
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}