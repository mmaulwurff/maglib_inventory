#ifndef MAGLIB_INVENTORY_DYNAMIC_INV_H
#define MAGLIB_INVENTORY_DYNAMIC_INV_H

#include "inv_cell.h"
#include "t_iterator.h"
#include <vector>
#include <algorithm>

namespace mag {

template <typename content_type, int max = 1>
class dynamic_inv {
public:

    typedef inv_cell<content_type, max> cell_type;

    dynamic_inv(size_t size);
    size_t get_size() const;

    void resize(size_t size);

    const content_type& show_at(int i) const;
    const cell_type& show_cell(const int i) const { return inv[i]; }

    int get_count(int i) const;
    bool is_empty(int i) const;
    bool is_empty() const;

    fits push(int i, cell_type& pushed);
    fits push(cell_type& pushed);
    void push_expand(cell_type& pushed);

    cell_type pop(int i, int count);

    void sort();

    typedef std::vector<cell_type> container_type;
    typedef mag_detail::t_iterator<typename container_type::iterator,
        dynamic_inv> iterator;
    typedef mag_detail::t_iterator<typename container_type::const_iterator,
        dynamic_inv> c_iterator;

    iterator begin() { return iterator(inv.begin(), inv.end()); }
    iterator end()   { return iterator(inv.end(),   inv.end()); }

    c_iterator cbegin() const { return c_iterator(inv.begin(), inv.end()); }
    c_iterator cend()   const { return c_iterator(inv.end(),   inv.end()); }

private:

    cell_type& show_cell(const int i) { return inv[i]; }

    container_type inv;

};

template <typename cell_type, int max>
dynamic_inv<cell_type, max>::dynamic_inv(const size_t size)
    : inv(size)
{}

template <typename cell_type, int max>
bool dynamic_inv<cell_type, max>::is_empty(const int i) const {
    return (get_count(i) == 0);
}

template <typename cell_type, int max>
int dynamic_inv<cell_type, max>::get_count(const int i) const {
    return inv[i].get_count();
}

template <typename cell_type, int max>
bool dynamic_inv<cell_type, max>::is_empty() const {
    return std::all_of( cbegin(), cend()
                      , [](const cell_type& c) { return c.is_empty(); });
}

template <typename cell_type, int max>
size_t dynamic_inv<cell_type, max>::get_size() const {
    return inv.size();
}

template <typename c_t, int m>
fits dynamic_inv<c_t, m>::push(const int i, inv_cell<c_t, m>& pushed) {
    return show_cell(i).push(pushed);
}

template <typename c_t, int m>
fits dynamic_inv<c_t, m>::push(inv_cell<c_t, m>& pushed) {
    const int old_count = pushed.get_count();
    for (dynamic_inv<c_t, m>::cell_type& c : inv) {
        if (c.push(pushed) == fits::full) return fits::full;
    }
    return (old_count == pushed.get_count())
           ? fits::none
           : fits::partial;
}

template <typename c_t, int m>
void dynamic_inv<c_t, m>::push_expand(cell_type& pushed) {
    for (dynamic_inv<c_t, m>::cell_type& c : inv) {
        if (c.push(pushed) == fits::full) return;
    }
    resize(get_size() + 1);
    inv.back().push(pushed);
}

template <typename c_t, int m>
inv_cell<c_t, m> dynamic_inv<c_t, m>::pop(const int i, const int count) {
    return show_cell(i).pop(count);
}

template <typename c_t, int m>
const c_t& dynamic_inv<c_t, m>::show_at(const int i) const {
    return inv[i].show_content();
}

template <typename c_t, int m>
void dynamic_inv<c_t, m>::resize(const size_t size) {
    inv.resize(size);
}

template <typename c_t, int m>
void dynamic_inv<c_t, m>::sort() {
    std::sort(inv.begin(), inv.end());
}

}

template <typename c_t, int m>
std::ostream& operator<<( std::ostream& stream
                        , const mag::dynamic_inv<c_t, m>& inv)
{
    for (size_t i = 0; i < inv.get_size(); ++i) {
        stream << inv.show_cell(i) << std::endl;
    }
    return stream;
}

#endif //MAGLIB_INVENTORY_DYNAMIC_INV_H