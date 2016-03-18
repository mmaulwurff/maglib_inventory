#ifndef MAGLIB_INVENTORY_INVENTORY_H
#define MAGLIB_INVENTORY_INVENTORY_H

#include "inv_cell.h"
#include <ostream>

namespace mag {

template <typename content_type, int rows, int cols, int max = 1>
class fixed_size_inv {
    static_assert(rows >= 1 && cols >=1, "minimal inventory sizes");

public:

    typedef inv_cell<content_type, max> cell_type;

    fixed_size_inv();

    const content_type& show(int row, int col) const;
    int get_count(int row, int col) const;
    bool is_empty(int row, int col) const;

    const cell_type& show_cell(const int row, const int col) const;

    push_results push(int row, int col, cell_type& pushed);
    cell_type pop(int row, int col, int count);

    class iterator {
    public:
        iterator& operator++() { do ++i; while (skip()); return *this; }
        bool operator==(const iterator& o) const { return i == o.i; }
        bool operator!=(const iterator& o) const { return i != o.i; }
        cell_type& operator*() { return *i; }
    private:
        iterator(cell_type* const begin, const cell_type* const end)
            : i(begin), e(end) { while (skip()) ++i; }
        bool skip() const { return (i->get_count() == 0 && i != e); }
        cell_type* i;
        const cell_type* const e;
        friend iterator fixed_size_inv<content_type, rows, cols, max>::begin();
        friend iterator fixed_size_inv<content_type, rows, cols, max>::end();
    };

    iterator begin() { return iterator(container, container + size); }
    iterator end()   { return iterator(container + size, container + size); }

private:

    static int index_at(const int r, const int c) { return r * cols + c; }

    cell_type& show_cell(const int row, const int col)
        { return container[index_at(row, col)]; }

    static const int size = rows * cols;

    cell_type container[size];
};

template <typename c_t, int R, int C, int m>
fixed_size_inv<c_t, R, C, m>::fixed_size_inv() : container() {}

template <typename c_t, int R, int C, int m>
const inv_cell<c_t, m>& fixed_size_inv<c_t, R, C, m>::
show_cell(const int row, const int col) const {
    return container[index_at(row, col)];
}

template <typename c_t, int R, int C, int m>
const c_t& fixed_size_inv<c_t, R, C, m>::show(const int r, const int c) const {
    return show_cell(r, c).showContent();
}

template <typename c_t, int R, int C, int m>
push_results fixed_size_inv<c_t, R, C, m>::
push(const int row, const int col, inv_cell<c_t, m>& pushed) {
    return show_cell(row, col).push(pushed);
}

template <typename c_t, int R, int C, int m>
inv_cell<c_t, m> fixed_size_inv<c_t, R, C, m>::
pop(const int r, const int c, const int count) {
    return show_cell(r, c).pop(count);
}

template <typename c_t, int R, int C, int m>
int fixed_size_inv<c_t, R, C, m>::get_count(const int r, const int c) const {
    return show_cell(r, c).get_count();
}

template <typename c_t, int R, int C, int m>
bool fixed_size_inv<c_t, R, C, m>::is_empty(const int r, const int c) const {
    return (get_count(r, c) == 0);
}

} // namespace mag;

template <typename c_t, int rows, int cols, int m>
std::ostream& operator<<( std::ostream& stream
                        , const mag::fixed_size_inv<c_t, rows, cols, m>& inv)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) stream << inv.show_cell(i, j) << '\t';
        stream << std::endl;
    }
    return stream;
}

#endif //MAGLIB_INVENTORY_INVENTORY_H
