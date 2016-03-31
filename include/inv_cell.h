#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H

#include "has_method.h"

#include <ostream>
#include <cassert>

namespace mag {

enum class fits { none, part, full };

template <typename content_type, int max_stack_size = 1>
class inv_cell {
public:

    inv_cell();

    inv_cell(const content_type& init, int count = 1);

    /** Shows how much of this item can fit in a stack (one cell in inventory).
     * @returns maximum number of items in stack. */
    int get_max_stack_size() const;

    fits can_push(const inv_cell& other) const;

    /** Pushes other cell contents into this cell, updates counts.
     * @pre can_push(other) should return fits::full or fits::part. */
    void push(inv_cell& other);

    inv_cell pop(int count);

    int get_count() const;
    bool is_empty() const;
    const content_type& show_content() const;

    bool operator<(const inv_cell& other) const;

private:

    static_assert(max_stack_size >= 1, "minimal stack size is 1");

    int count;
    content_type content;
};

namespace detail {

HAS_METHOD_INIT(get_max_stack_size)

template <typename T, int max, int has_max> struct get_max_stack_size_inner
{ static int get(const T&) { return 0; } };

template <typename T, int max> struct get_max_stack_size_inner<T, max, 0>
{ static int get(const T&) { return max; } };

template <typename T, int max> struct get_max_stack_size_inner<T, max, 1>
{ static int get(const T& c) { return c.get_max_stack_size(); } };

} // namespace detail

#define T template <typename c_t, int max>
#define C inv_cell<c_t, max>

T C::inv_cell()
    : count(0)
    , content()
{}

T fits C::can_push(const inv_cell& cell) const {
    assert(cell.count > 0);

    if (count == 0) return fits::full;
    if (cell.content != content) return fits::none;

    const int max_stack_size = get_max_stack_size();
    if (count == max_stack_size) return fits::none;

    return ( (cell.count + count <= max_stack_size)
        ? fits::full
        : fits::part );
}

T void C::push(inv_cell<c_t, max>& pushed) {
    if (count == 0) {
        content = pushed.content;
        count   = pushed.count;
        pushed.count = 0;
    } else {
        const int max_stack_size = get_max_stack_size();
        if (count + pushed.count <= max_stack_size) {
            count       += pushed.count;
            pushed.count = 0;
        } else {
            pushed.count -= max_stack_size - count;
            count         = max_stack_size;
        }
    }
}

T C C::pop(const int pop_count) {
    assert(count >= pop_count);
    count -= pop_count;
    if (count == 0) {
        c_t result_content(std::move(content));
        content = c_t();
        return inv_cell<c_t, max>(result_content, pop_count);
    }
    return inv_cell<c_t, max>(content, pop_count);
}

T C::inv_cell(const c_t& init, const int _count)
    : count(_count)
    , content(init)
{
    assert(0 < count);
    assert(    count <= get_max_stack_size());
}

T int C::get_max_stack_size() const {
    return detail::get_max_stack_size_inner<c_t, max,
        detail::HAS_METHOD(c_t, get_max_stack_size, int())>::get(content);
}

T bool C::operator<(const inv_cell<c_t, max>& other) const {
    return content < other.content;
}

T int C::get_count() const {
    return count;
}

T bool C::is_empty() const {
    return count == 0;
}

T const c_t& C::show_content() const {
    return content;
}

#undef T
#undef C

} // namespace mag;

template <typename c_t, int max>
std::ostream& operator<<(std::ostream& str, const mag::inv_cell<c_t, max>& c) {
    return (str << c.show_content() << "x" << c.get_count());
}

std::ostream& operator<<(std::ostream& str, const mag::fits fit) {
    static const char* const names[] = { "none", "part", "full" };
    return str << names[static_cast<int>(fit)];
}

#endif //MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
