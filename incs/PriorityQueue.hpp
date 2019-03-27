#ifndef N_PUZZLE_PRIORITY_QUEUE_HPP
#define N_PUZZLE_PRIORITY_QUEUE_HPP

#include <vector>
#include <boost/concept_check.hpp>

template<class _Tp, class _Container = std::vector<_Tp>,
		class _Compare = std::less<typename _Container::value_type>>
class PriorityQueue {
public:
	typedef typename _Container::iterator iterator;
	typedef typename _Container::const_iterator const_iterator;
	typedef typename _Container::const_reference const_reference;
	typedef typename _Container::reference reference;
	typedef typename _Container::value_type value_type;

	void push(value_type &&__x);

	void push(const_reference __x);

	const_reference top() const;

	reference top();

	void pop();

	void erase(const_iterator __position);

	void erase(const_iterator __first, const_iterator __last);

	iterator begin();

	const_iterator cbegin() const;

	iterator end();

	const_iterator cend() const;

	size_t size() const;

	bool empty() const;

private:
	_Container c;
	_Compare comp;
};

template<class _Tp, class _Container, class _Compare>
void PriorityQueue<_Tp, _Container, _Compare>::push(const_reference __x) {
	c.push_back(__x);
	std::push_heap(c.begin(), c.end(), comp);
}

template<class _Tp, class _Container, class _Compare>
void PriorityQueue<_Tp, _Container, _Compare>::push(value_type &&__x) {
	c.push_back(__x);
	std::push_heap(c.begin(), c.end(), comp);
}

template<class _Tp, class _Container, class _Compare>
typename PriorityQueue<_Tp, _Container, _Compare>::const_reference
PriorityQueue<_Tp, _Container, _Compare>::top() const {
	return c.front();
}

template<class _Tp, class _Container, class _Compare>
typename PriorityQueue<_Tp, _Container, _Compare>::reference
PriorityQueue<_Tp, _Container, _Compare>::top() {
	return c.front();
}

template<class _Tp, class _Container, class _Compare>
void PriorityQueue<_Tp, _Container, _Compare>::pop() {

	std::pop_heap(c.begin(), c.end(), comp);
	c.pop_back();
}

template<class _Tp, class _Container, class _Compare>
void PriorityQueue<_Tp, _Container, _Compare>::erase(const_iterator __first,
													 const_iterator __last) {
	c.erase(__first, __last);
	std::sort_heap(c.begin(), c.end(), comp);
}

template<class _Tp, class _Container, class _Compare>
void
PriorityQueue<_Tp, _Container, _Compare>::erase(const_iterator __position) {
	c.erase(__position);
	std::sort_heap(c.begin(), c.end(), comp);
}

template<class _Tp, class _Container, class _Compare>
typename PriorityQueue<_Tp, _Container, _Compare>::iterator
PriorityQueue<_Tp, _Container, _Compare>::begin() {
	return c.begin();
}

template<class _Tp, class _Container, class _Compare>
typename PriorityQueue<_Tp, _Container, _Compare>::const_iterator
PriorityQueue<_Tp, _Container, _Compare>::cbegin() const {
	return c.cbegin();
}

template<class _Tp, class _Container, class _Compare>
typename PriorityQueue<_Tp, _Container, _Compare>::iterator
PriorityQueue<_Tp, _Container, _Compare>::end() {
	return c.end();
}

template<class _Tp, class _Container, class _Compare>
typename PriorityQueue<_Tp, _Container, _Compare>::const_iterator
PriorityQueue<_Tp, _Container, _Compare>::cend() const {
	return c.cend();
}

template<class _Tp, class _Container, class _Compare>
size_t PriorityQueue<_Tp, _Container, _Compare>::size() const {
	return c.size();
}

template<class _Tp, class _Container, class _Compare>
bool PriorityQueue<_Tp, _Container, _Compare>::empty() const {
	return c.empty();
}

#endif //N_PUZZLE_PRIORITY_QUEUE_HPP
