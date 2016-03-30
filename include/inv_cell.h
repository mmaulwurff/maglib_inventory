#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H

#include "has_method.h"

#include <ostream>
#include <cassert>

HAS_METHOD_INIT(get_max_stack_size)

namespace mag_detail {
template <typename T, int max, int has_max> struct get_max_stack_size_inner
    { static int get(const T&) { return 0; } };

template <typename T, int max> struct get_max_stack_size_inner<T, max, 0>
    { static int get(const T&) { return max; } };

template <typename T, int max> struct get_max_stack_size_inner<T, max, 1>
    { static int get(const T& c) { return c.get_max_stack_size(); } };
}

namespace mag {
    enum class fits { none, part, full };

template <typename content_type, int max_stack_size = 1>
class inv_cell {
    static_assert(max_stack_size >= 1, "minimal stack size is 1");

public:

    inv_cell() : count(0), content() {}

    inv_cell(const content_type& init, int count = 1);

    /** Shows how much of this item can fit in a stack (one cell in inventory).
     * @returns maximum number of items in stack. */
    int get_max_stack_size() const;

    fits can_push(const inv_cell& other) const;

    /** Pushes other cell contents into this cell, updates counts.
     * @pre can_push(other) should return fits::full or fits::part. */
    void push(inv_cell& other);

    inv_cell pop(int count);

    int get_count() const { return count; }
    bool is_empty() const { return count == 0; }
    const content_type& show_content() const { return content; }

    bool operator<(const inv_cell& other) const;

private:

    int count;
    content_type content;
};

template <typename c_t, int max>
fits inv_cell<c_t, max>::can_push(const inv_cell& cell) const {
    assert(cell.count > 0);

    if (count == 0) return fits::full;
    if (cell.content != content) return fits::none;

    const int max_stack_size = get_max_stack_size();
    if (count == max_stack_size) return fits::none;

    return ( (cell.count + count <= max_stack_size)
        ? fits::full
        : fits::part );
}

template <typename c_t, int max>
void inv_cell<c_t, max>::push(inv_cell<c_t, max>& pushed) {
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

template <typename c_t, int max>
inv_cell<c_t, max> inv_cell<c_t, max>::pop(const int pop_count) {
    assert(count >= pop_count);
    count -= pop_count;
    if (count == 0) {
        c_t result_content(std::move(content));
        content = c_t();
        return inv_cell<c_t, max>(result_content, pop_count);
    }
    return inv_cell<c_t, max>(content, pop_count);
}

template <typename c_t, int max>
inv_cell<c_t, max>::inv_cell(const c_t& init, const int _count)
    : count(_count)
    , content(init)
{
    assert(0 < count);
    assert(    count <= get_max_stack_size());
}

template <typename c_t, int max>
int inv_cell<c_t, max>::get_max_stack_size() const {
    return mag_detail::get_max_stack_size_inner<c_t, max,
        HAS_METHOD(c_t, get_max_stack_size, int())>::get(content);
}

template <typename c_t, int max>
bool inv_cell<c_t, max>::operator<(const inv_cell<c_t, max>& other) const {
    return content < other.content;
}

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
