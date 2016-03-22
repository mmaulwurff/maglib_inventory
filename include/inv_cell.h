#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H

#include "has_method.h"
#include "mag_assert.h"

#include <ostream>
#include <stdlib.h>

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

    std::ostream& operator<<(std::ostream& str, const fits fit) {
        static const char* const names[] = { "none", "part", "full" };
        return str << names[static_cast<int>(fit)];
    }

template <typename content_type, int max_stack_size = 1>
class inv_cell {
    static_assert(max_stack_size >= 1, "minimal stack size is 1");

public:

    inv_cell() : count(0), content() {}

    inv_cell(const content_type& init, int count = 1);

    /** Shows how much of this item can fit in a stack (one cell in inventory).
     * @returns maximum number of items in stack. */
    int get_max_stack_size() const;

    /** Tries to push some other item to this item.
     *  It is possible for other item to not fit.
     *  @returns remainder of other item that doesn't fit. */
    fits push(inv_cell& other);
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
fits inv_cell<c_t, max>::push(inv_cell<c_t, max>& pushed) {
    if (pushed.count == 0) return mag::fits::full;

    if (count == 0) {
        content = pushed.content;
        count   = pushed.count;
        pushed.count = 0;
        return mag::fits::full;
    }

    if (pushed.content != content) return mag::fits::none;

    const int max_stack_size = get_max_stack_size();
    const int count_sum = pushed.count + count;
    const div_t d = div(count_sum, max_stack_size);
    if (d.quot > 0) {
        pushed.count = d.rem;
        count        = max_stack_size;
    } else {
        pushed.count = 0;
        count        = d.rem;
    }
    return ( (pushed.count == 0)
             ? mag::fits::full
             : mag::fits::part );
}

template <typename c_t, int max>
inv_cell<c_t, max> inv_cell<c_t, max>::pop(const int pop_count) {
    mag_assert(count >= pop_count);
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
    mag_assert(0 < count);
    mag_assert(    count <= get_max_stack_size());
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

#endif //MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
