#include "Visualizer.hpp"
#include <exception>

namespace Visualizer {

	Vizualizer::Vizualizer(unsigned int tileSize) :
	pathRoot_(N_PUZZLE_ROOT),
	restart_(false),
	pause_(false) {

		if (!texturePuzzle_.loadFromFile((pathRoot_ / "ressources" / "test.jpg").generic_string()))
			throw(std::runtime_error("N_Puzzle image Visualizer cannot be load"));
		if (!font_.loadFromFile((pathRoot_ / "ressources" / "OpenSans-Regular.ttf").generic_string()))
			throw(std::runtime_error("N_Puzzle image Visualizer cannot be load"));
		display_ = std::make_unique<DisplaySfml>(texturePuzzle_.getSize().x + pixelBorder * (tileSize - 1), texturePuzzle_.getSize().y + pixelBorder * (tileSize - 1) + sizeInfo, "Test");
		display_->win_.setActive(false);
		gs_ = std::make_unique<GridSpriteManager>(texturePuzzle_, sf::Vector2u(tileSize, tileSize));
	}

	void Vizualizer::loop(KStar::ResolverData &resolver) {
		display_->win_.setActive(true);

		KStar::ResolverContainer &resolverContainer = resolver.resolverContainer;

		auto resolvedIterCurrentState = resolverContainer.begin();
		auto resolvedIterPreviousState = resolverContainer.begin();


		while (!display_->exit()) {
			//display_->updateInput();
			timeLogic_.update();

			while (display_->win_.pollEvent(display_->ev_)) {
				if (display_->ev_.type == sf::Event::Closed)
					display_->exit_ = true;
				if (display_->ev_.type == sf::Event::KeyPressed) {

					switch (display_->ev_.key.code) {
						case sf::Keyboard::Escape:
							display_->exit_ = true;
							break;
						case sf::Keyboard::N:
							restart_ = true;
							break;
						case sf::Keyboard::Add:
							timeLogic_.setStep(timeLogic_.getStep() + std::chrono::milliseconds(100));
							break;
						case sf::Keyboard::Subtract:
							timeLogic_.setStep(timeLogic_.getStep() - std::chrono::milliseconds(100));
							break;
						case sf::Keyboard::Space:
							pause_ = !pause_;
							timeLogic_.setPause(pause_);
							break;
						case sf::Keyboard::Left:
							if (resolvedIterCurrentState != resolverContainer.begin()) {
								if (resolvedIterCurrentState - 1 != resolverContainer.end())
									--resolvedIterCurrentState;
								resolvedIterPreviousState = resolvedIterCurrentState;
								if (resolvedIterPreviousState != resolverContainer.begin())
									--resolvedIterPreviousState;
							}
							break;
						case sf::Keyboard::Right:
							if (resolvedIterCurrentState != resolverContainer.end()) {
								resolvedIterPreviousState = resolvedIterCurrentState;
								if (resolvedIterCurrentState + 1 != resolverContainer.end())
									++resolvedIterCurrentState;
							}
							break;
						default :
							break;
					}
				}
			}


			while (timeLogic_.needLogicUpdate()) {
				if (restart_) {
					resolvedIterCurrentState = resolverContainer.begin();
					restart_ = false;
				} else if (resolvedIterCurrentState != resolverContainer.end()) {
					resolvedIterPreviousState = resolvedIterCurrentState;
					++resolvedIterCurrentState;
				}
			}

			display_->win_.clear();
			if (resolvedIterCurrentState != resolverContainer.end())
				gs_->updateSpritePositionFromGridContainers(*resolvedIterPreviousState, *resolvedIterCurrentState, timeLogic_.getRatio());
			else if (resolvedIterPreviousState != resolverContainer.end())
				gs_->updateSpritePositionFromGridContainers(*resolvedIterPreviousState);

			gs_->renderTarget(display_->win_, sf::Vector2f(1.f, 1.0f));
			display_->render();
		}
	}


	TimeLogic::TimeLogic() :
	lag_(std::chrono::nanoseconds(0)),
	update_(std::chrono::high_resolution_clock::now()),
	step_(std::chrono::milliseconds(400)),
	wantPause_(false),
	pause_(false)
	{}

	void TimeLogic::update() {
		auto deltaTime = std::chrono::high_resolution_clock::now() - update_;
		update_ = std::chrono::high_resolution_clock::now();
		lag_ += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);
	}

	bool TimeLogic::needLogicUpdate() {
		if (lag_ < step_ || pause_)
			return false;
		if (wantPause_)
			pause_ = true;
		lag_ -= step_;
		return true;
	}

	float TimeLogic::getRatio() const {
		float ratio = static_cast<float>(lag_.count()) / static_cast<float>(step_.count());
		if (wantPause_ && ratio > 0.9999f)
			return 1.f;
		return ratio;
	}

	void TimeLogic::setStep(std::chrono::milliseconds milli) {
		step_ = milli;
		if (step_ < std::chrono::milliseconds(100))
			step_ = std::chrono::milliseconds(100);
		if (step_ > std::chrono::milliseconds(1000))
			step_ = std::chrono::milliseconds(1000);
	}

	std::chrono::milliseconds TimeLogic::getStep() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(step_);
	}

	void TimeLogic::setPause(bool b) {
		wantPause_ = b;
		if (!wantPause_) {
			lag_ = step_;
			pause_ = false;
		}
	}
}