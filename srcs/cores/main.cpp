#include <iostream>
#include <queue>
#include <resolver/KStar.hpp>
#include <parser/Parser.hpp>
#include <boost/program_options.hpp>
#include <visualizer/DisplaySfml.hpp>
#include <chrono>
#include <boost/filesystem.hpp>
#include <visualizer/GridVisualizer.hpp>
#include <visualizer/SfVectorInterpolate.hpp>
#include <visualizer/GridSprite.hpp>

//Make Min Max

void temp_visualizer(KStar::ResolverContainer resolverContainer, unsigned int tileSize) {
	bool restart = false;

	sf::Texture tileset_;

	boost::filesystem::path pathRoot(N_PUZZLE_ROOT);
	if (!tileset_.loadFromFile((pathRoot / "ressources" / "test.jpg").generic_string()))
		exit(0);

	DisplaySfml display(tileset_.getSize().x + pixelBorder * (tileSize - 1), tileset_.getSize().y + pixelBorder * (tileSize - 1), "Test");
	std::chrono::nanoseconds lag(std::chrono::nanoseconds(0));
	auto time_start = std::chrono::high_resolution_clock::now();

	constexpr std::chrono::nanoseconds timestep(std::chrono::milliseconds(400));

	auto resolvedIterCurrentState = resolverContainer.begin();
	auto resolvedIterPreviousState = resolverContainer.begin();

	GridSprite gs(tileset_, sf::Vector2u(tileSize, tileSize));

	while (!display.exit()) {
		//display.updateInput();
		std::cout << "updateInput" << std::endl;

		while (display.win_.pollEvent(display.ev_)) {
			if (display.ev_.type == sf::Event::Closed)
				display.exit_ = true;
			if (display.ev_.type == sf::Event::KeyPressed) {

				switch (display.ev_.key.code) {
					case sf::Keyboard::Escape:
						display.exit_ = true;
						break;
					case sf::Keyboard::N:
						restart = true;
						break;
					default :
						break;
				}
			}
		}

		auto delta_time = std::chrono::high_resolution_clock::now() - time_start;
		time_start = std::chrono::high_resolution_clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);
		std::cout << "lag" << std::endl;

		while(lag >= timestep) {
			std::cout << "timestep" << std::endl;

			lag -= timestep;
			if (restart) {
				resolvedIterCurrentState = resolverContainer.begin();
				restart = false;
			}
			else if (resolvedIterCurrentState != resolverContainer.end()) {
				resolvedIterPreviousState = resolvedIterCurrentState;
				++resolvedIterCurrentState;
				std::cout << "update" << std::endl;
			}
		}
		float ratio = static_cast<float>(lag.count()) / static_cast<float>(timestep.count());

		display.win_.clear();
		if (resolvedIterCurrentState != resolverContainer.end())
			gs.updateSpritePositionFromGridContainers(*resolvedIterPreviousState, *resolvedIterCurrentState, ratio);
		else if (resolvedIterPreviousState != resolverContainer.end())
			gs.updateSpritePositionFromGridContainers(*resolvedIterPreviousState);
		gs.renderTarget(display.win_);
		display.render();
	}
}

int main(int argc, char *argv[]) {

	const std::map<std::string, KStar::eHeuristic> map {
			{ "hamming", KStar::kHamming },
			{ "manhattan", KStar::kManhattan },
			{ "linear conflict", KStar::kLinearConflict },
			{ "euclidiean", KStar::kEuclidean }
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

		KStar::const_node_pointer node = nullptr;
		if (parser.getSize() == 3)
			node = std::make_shared<const KStar::Node>(Grid<ValuePuzzle>({1,2,3,8,0,4,7,6,5}), parser.getSize());
		else {
			node = std::make_shared<const KStar::Node>(Grid<ValuePuzzle>({
																				 1,2,3,4,
																				 12,13,14,5,
																				 11,0,15,6,
																				 10,9,8,7}), parser.getSize());
		}


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

		for (const auto &grid : resolverData.resolverContainer) {
			std::cout << grid << std::endl;
			std::cout << std::endl;
		}
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
		if (vm.count("visualizer"))
			temp_visualizer(resolverData.resolverContainer, parser.getSize());


	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}