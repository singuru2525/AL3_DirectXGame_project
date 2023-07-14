#pragma once
#include <functional>

class TimedCall {
public:
	TimedCall(std::function<void(void)> f, uint32_t time);

	void Update();

	bool IsFinished() { return isFinished_; }

private:

	std::function<void(void)> f_;

	uint32_t time_;

	int isFinished_ = false;
};