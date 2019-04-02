#include "TimeLogic.hpp"

TimeLogic::TimeLogic() :
		lag_(std::chrono::nanoseconds(0)),
		update_(std::chrono::high_resolution_clock::now()),
		step_(std::chrono::milliseconds(400)),
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
	lag_ -= step_;
	return true;
}

float TimeLogic::getRatio() const {
	float ratio = static_cast<float>(lag_.count()) / static_cast<float>(step_.count());
	if (pause_ && ratio > 0.9999f)
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
	pause_ = b;
	if (!pause_)
		lag_ = step_;
}