#ifndef MAGLIB_INVENTORY_DYNAMIC_INV_H
#define MAGLIB_INVENTORY_DYNAMIC_INV_H

#include "inv_cell.h"
#include <vector>

namespace mag {

template <typename content_type>
class dynamic_inv {
public:

    typedef inv_cell<content_type> cell_type;

    dynamic_inv(size_t size);

    const content_type& show_at(int i) const;
    int get_count(int i) const;
    bool is_empty(int i) const;

    push_results push(int i, cell_type& pushed);
    cell_type pop(int i, int count);

    class iterator {
    public:
        iterator& operator++() { do ++i; while (skip()); return *this; }
        bool operator==(const iterator& o) const { return i == o.i; }
        bool operator!=(const iterator& o) const { return i != o.i; }
        cell_type& operator*() { return *i; }
    private:
        typedef std::vector<cell_type>::iterator v_it;
        iterator(const v_it& begin, const v_it& end)
            : i(begin), e(end) { while (skip()) ++i; }
        bool skip() const { return (i->get_count() == 0 && i != e); }
        v_it i;
        const v_it e;
    };

private:

    std::vector<cell_type> inventory;

};

template <typename cell_type>
dynamic_inv<cell_type>::dynamic_inv(const size_t size)
    : inventory(size)
{}

};

#endif //MAGLIB_INVENTORY_DYNAMIC_INV_H
