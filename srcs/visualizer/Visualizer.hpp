#pragma once

#include <visualizer/GridSpriteManager.hpp>
#include <resolver/KStar.hpp>
#include <boost/filesystem.hpp>
#include "Text.hpp"
#include "TimeLogic.hpp"

namespace Visualizer {

	constexpr unsigned int pixelBorder = 3;
	constexpr unsigned int sizeInfo = 30;
	class Core {
	public:
		Core(unsigned int tileSize);
		void loop(KStar::ResolverData &resolver);

	private:
		TimeLogic timeLogic_;
		boost::filesystem::path pathRoot_;
		std::unique_ptr<DisplaySfml> display_;
		sf::Texture texturePuzzle_;
		sf::Font font_;
		std::unique_ptr<GridSpriteManager> gs_;

		KStar::ResolverContainer *resolverContainer_;
		KStar::ResolverContainer::iterator iterCurrentState_;
		KStar::ResolverContainer::iterator iterPreviousState_;

		unsigned int tileSize_;
		bool restart_;
		bool pause_;
		bool help_;

		std::unique_ptr<Text> complexityInTime_;
		std::unique_ptr<Text> complexityInSize_;
		std::unique_ptr<Text> numberOfMove_;
		std::unique_ptr<Text> speed_;
		std::unique_ptr<Text> nameHeuristique_;
		std::unique_ptr<Text> helpText_;

		void initText_(KStar::ResolverData &resolver);
		void callbackEvent_(sf::Event &ev);
	};
}
