#ifndef MAGLIB_INVENTORY_INVENTORY_H
#define MAGLIB_INVENTORY_INVENTORY_H

#include "inv_cell.h"
#include "t_iterator.h"
#include <ostream>
#include <algorithm>

namespace mag {

template <typename content_type, int rows, int cols, int max = 1>
class fixed_size_inv {
public:

    typedef inv_cell<content_type, max> cell_type;
    typedef mag_detail::t_iterator<cell_type*, fixed_size_inv> iterator;
    typedef mag_detail::t_iterator<const cell_type*, fixed_size_inv> c_iterator;

    fixed_size_inv();

    const cell_type&    show_cell   (int row, int col) const;
    const content_type& show_content(int row, int col) const;

    int get_count(int row, int col) const;
    bool is_empty(int row, int col) const;
    bool is_empty() const;

    fits can_push(int row, int col, const cell_type& pushed) const;
    fits can_push(const cell_type& pushed) const;

    void push(int row, int col, cell_type& pushed);
    void push(cell_type& pushed);

    cell_type pop(int row, int col, int count);

    iterator begin() { return iterator(container, get_end()); }
    iterator end()   { return iterator(get_end(), get_end()); }

    c_iterator begin() const { return c_iterator(container, get_cend()); }
    c_iterator end()   const { return c_iterator(get_cend(), get_cend()); }

private:

    static_assert(rows >= 1 && cols >=1, "minimal inventory sizes");

    static int index_at(const int r, const int c) { return r * cols + c; }

    cell_type& show_cell(const int row, const int col);

    const cell_type* get_cend() const { return container + size; }
    cell_type* get_end() { return container + size; }

    static const int size = rows * cols;

    cell_type container[size];
};

#define T template <typename c_t, int rows, int cols, int m>
#define C fixed_size_inv<c_t, rows, cols, m>

T C::fixed_size_inv() : container() {}

T const typename C::cell_type& C::show_cell(const int row, const int col) const
    { return container[index_at(row, col)]; }
T       typename C::cell_type& C::show_cell(const int row, const int col)
    { return container[index_at(row, col)]; }

T const c_t& C::show_content(const int r, const int c) const {
    return show_cell(r, c).show_content();
}

T fits C::can_push(const int r, const int c, const cell_type& pushed) const {
    return show_cell(r, c).can_push(pushed);
}

T fits C::can_push(const cell_type& pushed) const {
    int count = pushed.get_count();
    for (const C::cell_type& cell : container) {
        switch (cell.can_push(pushed)) {
        case fits::none: break;
        case fits::part:
            count -= cell.get_max_stack_size() - cell.get_count();
            if (count <= 0) return fits::full;
            break;
        case fits::full: return fits::full;
        }
    }
    return ( (count == pushed.get_count())
             ? fits::none
             : fits::part );
}

T void C::push(const int row, const int col, C::cell_type& pushed) {
    show_cell(row, col).push(pushed);
}

T void C::push(inv_cell<c_t, m>& pushed) {
    for (C::cell_type& cell : container) {
        if (cell.can_push(pushed) != fits::none) cell.push(pushed);
        if (pushed.is_empty()) return;
    }
}

T inv_cell<c_t, m> C::pop(const int r, const int c, const int count) {
    return show_cell(r, c).pop(count);
}

T int C::get_count(const int r, const int c) const {
    return show_cell(r, c).get_count();
}

T bool C::is_empty(const int r, const int c) const {
    return (get_count(r, c) == 0);
}

T bool C::is_empty() const {
    return std::all_of( begin(), end()
                      , [](const auto& cell) { return cell.is_empty(); } );
}

#undef T
#undef C

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
