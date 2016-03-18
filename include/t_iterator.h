#ifndef MAGLIB_INVENTORY_T_ITERATOR_H
#define MAGLIB_INVENTORY_T_ITERATOR_H

#include <iterator>
#include <type_traits>

namespace mag_detail {

template <typename iterated_type, typename friend_type>
class t_iterator
    : public std::iterator<std::random_access_iterator_tag,
        typename std::remove_reference<decltype(*iterated_type())>::type>
{
    typedef t_iterator<iterated_type, friend_type> iterator;
    typedef decltype(*iterated_type()) content_type;

public:

    t_iterator& operator++() { do ++i; while (skip()); return *this; }

    bool operator!=(const iterator& o) const { return i != o.i; }
    bool operator==(const iterator& o) const { return i == o.i; }

    int operator-(const iterator& o) const { return (i - o.i); }

    content_type& operator*() { return *i; }

private:

    t_iterator(const iterated_type& begin, const iterated_type& end)
        : i(begin), e(end) { while (skip()) ++i; }

    bool skip() const { return (i->get_count() == 0 && i != e); }

    iterated_type i;
    const iterated_type e;

    friend friend_type;
};

}

#endif //MAGLIB_INVENTORY_T_ITERATOR_H
