#ifndef N_PUZZLE_GRID_HPP
#define N_PUZZLE_GRID_HPP

#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Position.hpp"

template<class _Tp, class _Container = std::vector<_Tp>>
class Grid {
public:
	typedef typename _Container::iterator iterator;
	typedef typename _Container::const_iterator const_iterator;
	typedef typename _Container::pointer pointer;
	typedef typename _Container::value_type value_type;
	typedef typename _Container::reference reference;
	typedef typename _Container::const_reference const_reference;
	Grid();

	Grid(const Grid &rhs);

	explicit Grid(size_t size);

	Grid(size_t x, size_t y);

	explicit Grid(std::vector<_Tp> vector_);

	void fill(value_type value);

	void resize(size_t size);

	void resize(size_t x, size_t y);

	reference operator()(size_t x, size_t y);

	const_reference operator()(size_t x, size_t y) const;

	reference operator()(const Position &position);

	const_reference operator()(const Position &position) const;

	bool operator==(const Grid &rhs) const;

	bool operator!=(const Grid &rhs) const;

	Grid &operator=(const Grid &rhs);

	bool range(const Position &position) const;

	bool range(int x, int y) const;

	reference operator[](int n);

	const_reference operator[](int n) const;

	size_t getX() const;

	size_t getY() const;

	size_t size() const;

	iterator begin();

	const_iterator begin() const ;

	friend std::ostream &operator<<(std::ostream &os, const Grid &grid);

	const_iterator cbegin() const;

	iterator end();

	const_iterator end() const;

	const_iterator cend() const;

	pointer data();

private:
	size_t x_;
	size_t y_;
	_Container data_;
};


template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(std::vector<_Tp> vector_)
		: data_(vector_),
		  x_(std::sqrt(vector_.size())),
		  y_(std::sqrt(vector_.size()))
		{
}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid() : x_(0), y_(0), data_(0) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(const Grid &rhs)  :
		x_(rhs.x_), y_(rhs.y_), data_(rhs.data_) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(size_t x, size_t y)
		: x_(x), y_(y), data_(x * y) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(size_t size)
		: x_(size), y_(size), data_(size * size) {

}

template<class _Tp, class _Container>
void Grid<_Tp, _Container>::resize(size_t size) {
	x_ = y_ = size;
	data_.resize(size * size);
}

template<class _Tp, class _Container>
void Grid<_Tp, _Container>::resize(size_t x, size_t y) {
	x_ = x;
	y_ = y;
	data_.resize(x * y);
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::reference Grid<_Tp, _Container>::operator()(size_t x, size_t y) {
	return data_[y * y_ + x];
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_reference  Grid<_Tp, _Container>::operator()(const Position &position) const {
	return data_[position.y * y_ + position.x];
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_reference Grid<_Tp, _Container>::operator()(size_t x, size_t y) const {
	return data_[y * y_ + x];
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::reference Grid<_Tp, _Container>::operator()(const Position &position) {
	return data_[position.y * y_ + position.x];
}


template<class _Tp, class _Container>
Grid<_Tp, _Container> &Grid<_Tp, _Container>::operator=(const Grid &rhs) {
	if (this != &rhs) {
		x_ = rhs.x_;
		y_ = rhs.y_;
		data_ = rhs.data_;
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
	return data_.begin();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator
Grid<_Tp, _Container>::cbegin() const {
	return data_.cbegin();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator
Grid<_Tp, _Container>::cend() const {
	return data_.end();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::iterator Grid<_Tp, _Container>::end() {
	return data_.end();

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
typename Grid<_Tp, _Container>::const_reference
Grid<_Tp, _Container>::operator[](int n) const {
	return data_.at(n);
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::reference
Grid<_Tp, _Container>::operator[](int n) {
	return data_[n];
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::pointer
Grid<_Tp, _Container>::data() {
	return data_.data();
}

template<class _Tp, class _Container>
void Grid<_Tp, _Container>::fill(value_type value) {
	std::fill(begin(), end(), value);
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator
Grid<_Tp, _Container>::begin() const {
	return data_.begin();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator
Grid<_Tp, _Container>::end() const {
	return data_.end();
}


#endif //N_PUZZLE_GRID_HPP
