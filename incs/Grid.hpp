#ifndef N_PUZZLE_GRID_HPP
#define N_PUZZLE_GRID_HPP

#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Position.hpp"

template<class _Tp, class _Container = std::vector<_Tp>>
class Grid {
public:
	typedef typename _Container::iterator iterator;
	typedef typename _Container::const_iterator const_iterator;

	Grid();

	Grid(const Grid &rhs);

	explicit Grid(size_t size);

	Grid(size_t x, size_t y);

	void resize(size_t size);

	void resize(size_t x, size_t y);

	_Tp &operator()(size_t x, size_t y);

	const _Tp &operator()(size_t x, size_t y) const;

	_Tp &operator()(const Position &position);

	const _Tp &operator()(const Position &position) const;

	bool operator==(const Grid &rhs) const;

	bool operator!=(const Grid &rhs) const;

	Grid &operator=(const Grid &rhs);

	bool range(const Position &position) const;

	bool range(int x, int y) const;

	_Tp &operator[](int n);

	_Tp &operator[](int n) const;

	size_t getX() const;

	size_t getY() const;

	size_t size() const;

	iterator begin();

	friend std::ostream &operator<<(std::ostream &os, const Grid &grid);

	const_iterator cbegin() const;

	iterator end();

	const_iterator cend() const;

private:
	size_t x_;
	size_t y_;
	_Container data;
};

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid() : x_(0), y_(0), data(0) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(const Grid &rhs)  :
		x_(rhs.x_), y_(rhs.y_), data(rhs.data) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(size_t x, size_t y)
		: x_(x), y_(y), data(x * y) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(size_t size)
		: x_(size), y_(size), data(size * size) {

}

template<class _Tp, class _Container>
void Grid<_Tp, _Container>::resize(size_t size) {
	x_ = y_ = size;
	data.resize(size * size);
}

template<class _Tp, class _Container>
void Grid<_Tp, _Container>::resize(size_t x, size_t y) {
	x_ = x;
	y_ = y;
	data.resize(x * y);
}

template<class _Tp, class _Container>
_Tp &Grid<_Tp, _Container>::operator()(size_t x, size_t y) {
	return data[y * y_ + x];
}

template<class _Tp, class _Container>
const _Tp &Grid<_Tp, _Container>::operator()(const Position &position) const {
	return data[position.y * y_ + position.x];
}

template<class _Tp, class _Container>
const _Tp &Grid<_Tp, _Container>::operator()(size_t x, size_t y) const {
	return data[y * y_ + x];
}

template<class _Tp, class _Container>
_Tp &Grid<_Tp, _Container>::operator()(const Position &position) {
	return data[position.y * y_ + position.x];
}


template<class _Tp, class _Container>
Grid<_Tp, _Container> &Grid<_Tp, _Container>::operator=(const Grid &rhs) {
	if (this != &rhs) {
		x_ = rhs.x_;
		y_ = rhs.y_;
		data = rhs.data;
	}
	return *this;
}

template<class _Tp, class _Container>
size_t Grid<_Tp, _Container>::size() const {
	return y_ * x_;
}

template<class _Tp, class _Container>
bool Grid<_Tp, _Container>::operator==(const Grid &rhs) const {
	return x_ == rhs.x_ && y_ == rhs.y_ &&
		   std::equal(cbegin(), cend(), rhs.cbegin(), rhs.cend());
}

template<class _Tp, class _Container>
bool Grid<_Tp, _Container>::operator!=(const Grid &rhs) const {
	return !(*this == rhs);
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::iterator Grid<_Tp, _Container>::begin() {
	return data.begin();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator
Grid<_Tp, _Container>::cbegin() const {
	return data.cbegin();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator
Grid<_Tp, _Container>::cend() const {
	return data.end();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::iterator Grid<_Tp, _Container>::end() {
	return data.end();

}

template<class _Tp, class _Container>
size_t Grid<_Tp, _Container>::getX() const {
	return x_;
}

template<class _Tp, class _Container>
size_t Grid<_Tp, _Container>::getY() const {
	return y_;
}

template<class _Tp, class _Container>
bool Grid<_Tp, _Container>::range(int x, int y) const {
	return y * y_ + x < size();
}

template<class _Tp, class _Container>
bool Grid<_Tp, _Container>::range(const Position &position) const {
	return position.x >= 0 && position.x < x_ && position.y >= 0 &&
		   position.y < y_;
}



template<class _Tp, class _Container>
_Tp &Grid<_Tp, _Container>::operator[](int n) const {
	return data.at(n);
}

template<class _Tp, class _Container>
_Tp &Grid<_Tp, _Container>::operator[](int n) {
	return data[n];
}



#endif //N_PUZZLE_GRID_HPP
