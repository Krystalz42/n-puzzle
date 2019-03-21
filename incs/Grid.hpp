#ifndef N_PUZZLE_GRID_HPP
#define N_PUZZLE_GRID_HPP

#include <cstdlib>
#include <vector>

template <class _Tp, class _Container = std::vector<_Tp>>
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
	bool operator==(const Grid &rhs) const;
	bool operator!=(const Grid &rhs) const;
	Grid &operator=(const Grid &rhs);

	size_t size() const;

	iterator begin();

	const_iterator begin() const;

	iterator end() {
		return data.end();
	}

	const_iterator end() const {
		return data.end();
	}

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
	x_(rhs.x_), y_(rhs.y_), data(rhs.data)  {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(size_t x, size_t y)
	: x_(x), y_(y), data(x * y) {

}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::Grid(size_t size)
		: x_(size), y_(size), data(size * size){

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
const _Tp &Grid<_Tp, _Container>::operator()(size_t x, size_t y) const {
	return data[y * y_ + x];
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
	return x_ == rhs.x_ && y_ == rhs.y_ && std::equal(begin(), end(), rhs.begin());
}

template<class _Tp, class _Container>
bool Grid<_Tp, _Container>::operator!=(const Grid &rhs) const {
	return !(*this == rhs);
}

template<class _Tp, class _Container>
Grid<_Tp, _Container>::iterator Grid<_Tp, _Container>::begin() {
	return data.begin();
}

template<class _Tp, class _Container>
typename Grid<_Tp, _Container>::const_iterator Grid<_Tp, _Container>::begin() const {
	return data.begin();
}

#endif //N_PUZZLE_GRID_HPP
