#ifndef MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
#define MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H

#include "has_method.h"
#include <cassert>
#include <iostream>
#include <stdlib.h>

HAS_METHOD_INIT(get_max_stack_size)

namespace mag {
    enum push_results { fit_none, fit_partial, fit_full };

template <typename T, int max, int has_max> struct get_max_stack_size_inner
    { static int get(const T&) {} };

template <typename T, int max> struct get_max_stack_size_inner<T, max, 0>
    { static int get(const T&) { return max; } };

template <typename T, int max> struct get_max_stack_size_inner<T, max, 1>
    { static int get(const T& c) { return c.get_max_stack_size(); } };

template <typename content_type, int max_stack_size = 1>
class inv_cell {
    typedef inv_cell<content_type, max_stack_size> this_type;

public:

    inv_cell() : count(0), content() {}

    explicit inv_cell(const content_type& init, const int _count = 1)
        : count(_count), content(init)
    {
        assert(count <= get_max_stack_size());
    }

    /** Shows how much of this item can fit in a stack (one cell in inventory).
     * @returns maximum number of items in stack. */
    int get_max_stack_size() const {
        return get_max_stack_size_inner< content_type
                                       , max_stack_size
                                       , HAS_METHOD( content_type
                                                   , get_max_stack_size
                                                   , int() )>::get(content);
    }

    /** Tries to push some other item to this item.
     *  It is possible for other item to not fit.
     *  @returns remainder of other item that doesn't fit. */
    mag::push_results push(this_type& other);
    this_type pop(int count);

    int get_count() const { return count; }
    const content_type& showContent() const { return content; }

private:

    int count;
    content_type content;
};

template <typename c_t, int max>
push_results inv_cell<c_t, max>::push(inv_cell<c_t, max>& pushed) {
    if (pushed.count == 0) return mag::fit_full;

    if (count == 0) {
        content = pushed.content;
        count   = pushed.count;
        pushed.count = 0;
        return mag::fit_full;
    }

    if (pushed.content != content) return mag::fit_none;

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
             ? mag::fit_full
             : mag::fit_partial );
}

template <typename c_t, int max>
inv_cell<c_t, max> inv_cell<c_t, max>::pop(const int pop_count) {
    count -= pop_count;
    assert(count >= 0);
    return inv_cell<c_t, max>(content, pop_count);
}

} // stop using namespace mag;

#endif //MAGLIB_INVENTORY_INVENTORY_ITEM_INTERFACE_H
