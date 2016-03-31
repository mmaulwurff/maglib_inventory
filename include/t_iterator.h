#ifndef MAGLIB_INVENTORY_T_ITERATOR_H
#define MAGLIB_INVENTORY_T_ITERATOR_H

#include <iterator>
#include <type_traits>

namespace mag {
namespace detail {

template <typename iterated_type, typename friend_type>
class t_iterator
    : public std::iterator<std::random_access_iterator_tag,
        typename std::remove_reference<decltype(*iterated_type())>::type>
{
public:

    typedef decltype(*iterated_type()) content_type;

    t_iterator& operator++();

    bool operator!=(const t_iterator& o) const;
    bool operator==(const t_iterator& o) const;
    int  operator- (const t_iterator& o) const;

    content_type& operator*() { return *i; }

private:

    t_iterator(const iterated_type& begin, const iterated_type& end);

    bool skip() const;

    iterated_type i;
    const iterated_type e;

    friend typename friend_type::const_iterator friend_type::begin() const;
    friend typename friend_type::const_iterator friend_type::end() const;
    friend typename friend_type::iterator friend_type::begin();
    friend typename friend_type::iterator friend_type::end();
};

#define T template <typename i_t, typename f_t>
#define C t_iterator<i_t, f_t>

T C& C::operator++() {
    do ++i;
    while (skip());
    return *this;
}

T C::t_iterator(const i_t& begin, const i_t& end)
    : i(begin)
    , e(end)
{
    while (skip()) ++i;
}

T bool C::skip() const {
    return (i != e && i->get_count() == 0);
}

T bool C::operator!=(const t_iterator& o) const { return i != o.i; }
T bool C::operator==(const t_iterator& o) const { return i == o.i; }
T int  C::operator- (const t_iterator& o) const { return (i - o.i); }

#undef T
#undef C

}
}

#endif //MAGLIB_INVENTORY_T_ITERATOR_H
