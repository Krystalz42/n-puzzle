#ifndef N_PUZZLE_PRIORITYQUEUEITER_HPP
#define N_PUZZLE_PRIORITYQUEUEITER_HPP

#include <queue>

template <class _Tp, class _Container = std::vector<_Tp>,
		class _Compare = std::less<typename _Container::value_type> >
class PriorityQueueIter : public std::priority_queue<_Tp, _Container, _Compare> {

public:
	typedef typename _Container::iterator iterator;
	typedef typename _Container::const_iterator const_iterator;

	typedef std::priority_queue<_Tp, _Container, _Compare> _MainContainer;
	iterator begin() {
		return _MainContainer::c.begin();
	}

	iterator end() {

		return _MainContainer::c.end();
	}

	const_iterator begin() const {
		return _MainContainer::c.begin();
	}

	const_iterator end() const {
		return _MainContainer::c.end();
	}
};

#endif //N_PUZZLE_PRIORITYQUEUEITER_HPP
