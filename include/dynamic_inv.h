#ifndef MAGLIB_INVENTORY_DYNAMIC_INV_H
#define MAGLIB_INVENTORY_DYNAMIC_INV_H

#include "inv_cell.h"
#include "t_iterator.h"
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

    typedef std::vector<cell_type> container_type;
    typedef mag_detail::t_iterator<typename container_type::iterator,
        dynamic_inv> iterator;
    typedef mag_detail::t_iterator<typename container_type::const_iterator,
        dynamic_inv> c_iterator;

    iterator begin() const { return iterator(inv.begin(), inv.end()); }
    iterator end()   const { return iterator(inv.end(),   inv.end()); }

    c_iterator cbegin() const { return c_iterator(inv.begin(), inv.end()); }
    c_iterator cend()   const { return c_iterator(inv.end(),   inv.end()); }

private:

    container_type inv;

};

template <typename cell_type>
dynamic_inv<cell_type>::dynamic_inv(const size_t size)
    : inv(size)
{}

}

#endif //MAGLIB_INVENTORY_DYNAMIC_INV_H
