#pragma once

#include <SFML/Graphics.hpp>
#include <visualizer/GridSpriteManager.hpp>
#include <resolver/KStar.hpp>
#include <chrono>
#include <boost/filesystem.hpp>

namespace Visualizer {

	class TimeLogic {
	public:
		TimeLogic();
		void update();
		bool needLogicUpdate();
		float getRatio() const;

		void setStep(std::chrono::milliseconds milli);
		void setPause(bool b);
		std::chrono::milliseconds getStep() const;
	private:
		std::chrono::nanoseconds lag_;
		std::chrono::steady_clock::time_point update_;
		std::chrono::nanoseconds step_;
		bool wantPause_;
		bool pause_;
	};

	constexpr unsigned int pixelBorder = 3;

	class Vizualizer {
	public:
		Vizualizer(unsigned int tileSize);
		void loop(KStar::ResolverData &resolver);
	private:
		TimeLogic timeLogic_;
		boost::filesystem::path pathRoot_;
		std::unique_ptr<DisplaySfml> display_;
		sf::Texture texturePuzzle_;
		sf::Font  font_;
		std::unique_ptr<GridSpriteManager> gs_;

		bool restart_;
		bool pause_;
	};
}
