#ifndef MAGLIB_INVENTORY_INVENTORY_H
#define MAGLIB_INVENTORY_INVENTORY_H

#include <ostream>
#include "inv_cell.h"

namespace mag {

template <typename cell_type, int rows, int cols>
class fixed_size_inv {
public:

    fixed_size_inv() : container() {}

    const cell_type& show_at(int row, int col) const;

    push_results push(int row, int col, cell_type& pushed);
    cell_type pop(int row, int col, int count);

    class iterator {
    public:
        iterator(cell_type* const begin, cell_type* const end)
            : i(begin), e(end) { while (i->get_count() == 0 && i != e) ++i; }
        void operator++() { do { ++i; } while (i->get_count() == 0 && i != e); }
        bool operator==(const iterator& o) const { return i == o.i; }
        bool operator!=(const iterator& o) const { return i != o.i; }
        cell_type& operator*() { return *i; }
    private:
        cell_type* i;
        cell_type* const e;
    };

    iterator begin() { return iterator(container, container + size); }
    iterator end()   { return iterator(container + size, container + size); }

private:

    static int index_at(const int row, const int col) {
        return row * cols + col;
    }

    cell_type& show_at(const int row, const int col) {
        return container[index_at(row, col)];
    }

    static const int size = rows * cols;

    cell_type container[size];
};

template <typename c_t, int R, int C>
const c_t& fixed_size_inv<c_t, R, C>::show_at(const int r, const int c) const {
    return container[index_at(r, c)];
}

template <typename c_t, int R, int C>
push_results fixed_size_inv<c_t, R, C>::
push(const int row, const int col, c_t& pushed) {
    return show_at(row, col).push(pushed);
}

template <typename c_t, int R, int C>
c_t fixed_size_inv<c_t, R, C>::pop(const int r, const int c, const int count) {
    return show_at(r, c).pop(count);
}

} // namespace mag;

template <typename c_t, int rows, int cols>
std::ostream& operator<<( std::ostream& stream
                        , const mag::fixed_size_inv<c_t, rows, cols>& inventory)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            stream << inventory.show_at(i, j) << '\t';
        }
        stream << std::endl;
    }
    return stream;
}

#endif //MAGLIB_INVENTORY_INVENTORY_H
