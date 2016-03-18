#ifndef MAGLIB_INVENTORY_INVENTORY_H
#define MAGLIB_INVENTORY_INVENTORY_H

#include "inv_cell.h"
#include "t_iterator.h"
#include <ostream>
#include <algorithm>

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
    bool is_empty() const;

    const cell_type& show_cell(const int row, const int col) const;

    push_results push(int row, int col, cell_type& pushed);
    cell_type pop(int row, int col, int count);

    typedef mag_detail::t_iterator<cell_type*, fixed_size_inv> iterator;
    typedef mag_detail::t_iterator<const cell_type*, fixed_size_inv> c_iterator;

    iterator begin() { return iterator(container, get_end()); }
    iterator end()   { return iterator(get_end(), get_end()); }

    c_iterator cbegin() const { return c_iterator(container, get_cend()); }
    c_iterator cend()   const { return c_iterator(get_cend(), get_cend()); }

private:

    static int index_at(const int r, const int c) { return r * cols + c; }

    cell_type& show_cell(const int row, const int col)
        { return container[index_at(row, col)]; }

    const cell_type* get_cend() const { return container + size; }
    cell_type* get_end() { return container + size; }

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

template <typename c_t, int R, int C, int m>
bool fixed_size_inv<c_t, R, C, m>::is_empty() const {
    return std::all_of( cbegin(), cend()
                      , [](const auto& cell) { return cell.is_empty(); } );
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
