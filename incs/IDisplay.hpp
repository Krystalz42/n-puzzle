#pragma once
class IDisplay {
public:

	virtual ~IDisplay() = default;

	virtual bool exit() const = 0;

	virtual void updateInput() = 0;

	virtual void render() = 0;
};