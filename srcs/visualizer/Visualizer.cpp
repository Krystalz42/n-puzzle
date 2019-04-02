#include "Visualizer.hpp"
#include <exception>
#include <sstream>

namespace Visualizer {

	Core::Core(unsigned int tileSize) :
	pathRoot_(N_PUZZLE_ROOT),
	tileSize_(tileSize),
	restart_(false),
	pause_(false),
	help_(false),
	resolverContainer_(nullptr) {

		if (!texturePuzzle_.loadFromFile((pathRoot_ / "ressources" / "test.jpg").generic_string()))
			throw(std::runtime_error("N_Puzzle image Visualizer cannot be load"));
		if (!font_.loadFromFile((pathRoot_ / "ressources" / "OpenSans-Regular.ttf").generic_string()))
			throw(std::runtime_error("N_Puzzle image Visualizer cannot be load"));
		display_ = std::make_unique<DisplaySfml>(texturePuzzle_.getSize().x + pixelBorder * (tileSize - 1), texturePuzzle_.getSize().y + pixelBorder * (tileSize - 1) + sizeInfo * 3, "Test");
		display_->win_.setActive(false);
		gs_ = std::make_unique<GridSpriteManager>(texturePuzzle_, sf::Vector2u(tileSize, tileSize));

		complexityInTime_ = std::make_unique<Text>(font_, sf::Vector2f(display_->win_.getSize().x / 2.f / 2.f, display_->win_.getSize().y - sizeInfo / 2.f));
		complexityInSize_ = std::make_unique<Text>(font_, sf::Vector2f(complexityInTime_->getPosition().x + display_->win_.getSize().x / 2.f, complexityInTime_->getPosition().y));
		speed_ = std::make_unique<Text>(font_, sf::Vector2f(display_->win_.getSize().x * 0.9f, sizeInfo / 2.f));
		nameHeuristique_ = std::make_unique<Text>(font_, sf::Vector2f(display_->win_.getSize().x * 0.4f, sizeInfo / 2.f));
		numberOfMove_ = std::make_unique<Text>(font_, sf::Vector2f(display_->win_.getSize().x * 0.5f, complexityInTime_->getPosition().y - sizeInfo));
		helpText_ = std::make_unique<Text>(font_, sf::Vector2f(display_->win_.getSize().x * 0.5f, complexityInTime_->getPosition().y * 0.5f));
	}


	void Core::initText_(KStar::ResolverData &resolver) {
		std::ostringstream os;
		os << "Complexity in time [" << resolver.complexityInTime << "]";
		complexityInTime_->setText(os.str(), sizeInfo * 0.7f);
		os.str(std::string());
		os.clear();
		os << "Complexity in size [" << resolver.complexityInSize << "]";
		complexityInSize_->setText(os.str(), sizeInfo * 0.7f);
		nameHeuristique_->setText(resolver.nameHeuristic, sizeInfo * 0.7f);
		helpText_->setText("Pause: Space\nStep: Left/Right Arrow\nSpeed: +/-\n", sizeInfo * 0.7f);
	}

	void Core::loop(KStar::ResolverData &resolver) {
		display_->win_.setActive(true);


		initText_(resolver);
		resolverContainer_ = &resolver.resolverContainer;

		iterCurrentState_ = resolverContainer_->begin();
		iterPreviousState_ = resolverContainer_->begin();


		display_->setCallbackEvent(std::bind(&Core::callbackEvent_, this, std::placeholders::_1));

		while (!display_->exit()) {
			display_->updateInput();
			timeLogic_.update();

			while (timeLogic_.needLogicUpdate()) {
				if (restart_) {
					iterCurrentState_ = resolverContainer_->begin();
					restart_ = false;
				} else if (iterCurrentState_ != resolverContainer_->end()) {
					iterPreviousState_ = iterCurrentState_;
					++iterCurrentState_;
				}
			}

			display_->win_.clear();
			if (iterCurrentState_ != resolverContainer_->end())
				gs_->updateSpritePositionFromGridContainers(*iterPreviousState_, *iterCurrentState_, timeLogic_.getRatio());
			else if (iterPreviousState_ != resolverContainer_->end())
				gs_->updateSpritePositionFromGridContainers(*iterPreviousState_);

			gs_->renderTarget(display_->win_, sf::Vector2f(1.f, 1.0f), sf::Vector2f(0.f, sizeInfo));


			complexityInTime_->render(display_->win_);
			complexityInSize_->render(display_->win_);
			nameHeuristique_->render(display_->win_);

			numberOfMove_->setText(std::to_string(resolverContainer_->size() - std::distance(iterCurrentState_, resolverContainer_->end())) + "/" + std::to_string(resolverContainer_->size()), sizeInfo * 0.7f);
			numberOfMove_->render(display_->win_);
			speed_->setText(std::to_string(timeLogic_.getStep().count()) + "ms", sizeInfo * 0.7f);
			speed_->render(display_->win_);

			if (help_)
				helpText_->render(display_->win_);

			display_->render();
		}
	}

	void Core::callbackEvent_(sf::Event &ev) {
		if (ev.type == sf::Event::KeyPressed) {
			switch (ev.key.code) {
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
				case sf::Keyboard::Tab:
					help_ = !help_;
					break;
				case sf::Keyboard::Left:
					if (iterCurrentState_ != resolverContainer_->begin()) {
						if (iterCurrentState_ - 1 != resolverContainer_->end())
							--iterCurrentState_;
						iterPreviousState_ = iterCurrentState_;
						if (iterPreviousState_ != resolverContainer_->begin())
							--iterPreviousState_;
					}
					break;
				case sf::Keyboard::Right:
					if (iterCurrentState_ != resolverContainer_->end()) {
						iterPreviousState_ = iterCurrentState_;
						if (iterCurrentState_ + 1 != resolverContainer_->end())
							++iterCurrentState_;
					}
					break;
				default :
					break;
			}
		}
	}

}