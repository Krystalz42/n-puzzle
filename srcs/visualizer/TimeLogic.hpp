#pragma once

#include <chrono>

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
	bool pause_;
};
