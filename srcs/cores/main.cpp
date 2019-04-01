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
#include <visualizer/Visualizer.hpp>

//Make Min Max

void temp_visualizer(KStar::ResolverData resolver, unsigned int tileSize) {
	KStar::ResolverContainer &resolverContainer = resolver.resolverContainer;
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

	sf::Font font;
	if (!font.loadFromFile((pathRoot / "ressources" / "OpenSans-Regular.ttf").generic_string()))
		exit(0);


	GridSprite gs(tileset_, sf::Vector2u(tileSize, tileSize));
	float zoom = 100.f;
	sf::Vector2i mapPosition(0.f, 0.f);

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
					case sf::Keyboard::O:
						zoom += 2.f;
						break;
					case sf::Keyboard::L:
						zoom -= 2.f;
						break;
					default :
						break;
				}
			}
		}

		static bool firstPress = true;
		static sf::Vector2i firstPosition;
		static sf::Vector2i lastPosition;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (firstPress) {
				firstPosition = sf::Mouse::getPosition();
				firstPress = false;
				lastPosition = sf::Mouse::getPosition();
			}
			mapPosition += (sf::Mouse::getPosition() - lastPosition);
			lastPosition = sf::Mouse::getPosition();
		} else {
			firstPress = true;
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

		float sizeRender = 0.7f * zoom;
		float sizeRenderInfo = 0.3f * zoom;
		sf::Vertex line[] =
				{
						sf::Vertex(sf::Vector2f(mapPosition.x + 10, mapPosition.y + 10)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10, mapPosition.y + 10 + sizeRender + sizeRenderInfo)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10 + sizeRender, mapPosition.y + 10 + sizeRender + sizeRenderInfo)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10 + sizeRender, mapPosition.y + 10)),

						sf::Vertex(sf::Vector2f(mapPosition.x + 10, mapPosition.y + 10)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10 + sizeRender, mapPosition.y + 10)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10, mapPosition.y + 10 + sizeRender + sizeRenderInfo)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10 + sizeRender, mapPosition.y + 10 + sizeRender + sizeRenderInfo)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10, mapPosition.y + 10 + sizeRender)),
						sf::Vertex(sf::Vector2f(mapPosition.x + 10 + sizeRender, mapPosition.y + 10 + sizeRender))
				};
		display.win_.draw(line, 10, sf::Lines);
		sf::RenderTexture renderPuzzle;
		if (!(renderPuzzle.create(display.win_.getSize().x, display.win_.getSize().y)))
			exit(0);
		renderPuzzle.clear();
		gs.renderTarget(renderPuzzle, sf::Vector2f(1.f, 1.0f));
		renderPuzzle.display();

		sf::Sprite sprite(renderPuzzle.getTexture());
		sprite.setPosition(mapPosition.x + 10, mapPosition.y + 10);
		float ratioRender = display.win_.getSize().y;
		if (display.win_.getSize().x > display.win_.getSize().y)
			ratioRender = display.win_.getSize().x;
		sprite.setScale(sizeRender / ratioRender, sizeRender / ratioRender);
		display.win_.draw(sprite);

		sf::Text text1;
		text1.setFont(font);
		text1.setCharacterSize(sizeRenderInfo * 0.8);
		text1.setString("OOOKE");
		text1.setStyle(sf::Text::Regular);

		sf::FloatRect textRect = text1.getLocalBounds();
		text1.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top  + textRect.height / 2.0f);
		text1.setPosition(mapPosition.x + 10 + sizeRender / 2, mapPosition.y + 10 + sizeRender + sizeRenderInfo / 2);
		display.win_.draw(text1);
		display.render();
	}
}

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
		if (vm.count("visualizer"))
			temp_visualizer(resolverData, parser.getSize());


	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}