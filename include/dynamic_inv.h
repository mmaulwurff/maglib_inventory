#ifndef MAGLIB_INVENTORY_DYNAMIC_INV_H
#define MAGLIB_INVENTORY_DYNAMIC_INV_H

#include "inv_cell.h"
#include "t_iterator.h"

#include <vector>

namespace mag {

/** dynamic_inv is dynamically-sized inventory with linear structure.
 *  It can contain >= 1 inventory cells of cell_type.
 *  Each cell can contain several items up to max stack size.
 *  If content_type is mag::managed_pointer, max stack size is determined by
 *  underlying pointed type. Otherwise, max stack size is a template
 *  parameter max. */
template <typename content_type, int max = 1>
class dynamic_inv {
public:

    typedef inv_cell <content_type, max> cell_type;
    typedef std::vector<cell_type> container_t;
    typedef detail::t_iterator<typename container_t::iterator,
        dynamic_inv> iterator;
    typedef detail::t_iterator<typename container_t::const_iterator,
        dynamic_inv> const_iterator;

    dynamic_inv(size_t size);

    /** @name Information section */ ///@{
    size_t get_size() const;
    bool is_empty() const;

    int get_count(int i) const;
    bool is_empty(int i) const;

    fits can_push(int i, const cell_type& pushed) const;
    fits can_push(const cell_type& pushed) const;
    ///@}

    /** @name cell operations */ ///@{
    const cell_type& cell_at(int i) const;
    const content_type& content_at(int i) const;

    void push(int i, cell_type& pushed);
    void push(cell_type& pushed);
    void push_expand(cell_type& pushed);

    cell_type pop(int i, int count);
    ///@}

    void resize(size_t size);

    /** @name iterators section */ ///@{
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
    ///@}

protected:

    cell_type& cell_at_m(int i);

private:

    container_t inv;

};

#define T template <typename content_type, int max>
#define C dynamic_inv<content_type, max>

T C::dynamic_inv(const size_t size)
    : inv(size)
{}

T typename C::iterator C::begin() {
    return C::iterator(inv.begin(), inv.end());
}

T typename C::iterator C::end() {
    return C::iterator(inv.end(), inv.end());
}

T typename C::const_iterator C::begin() const {
    return C::const_iterator(inv.begin(), inv.end());
}

T typename C::const_iterator C::end() const {
    return C::const_iterator(inv.end(), inv.end());
}

T const typename C::cell_type& C::cell_at(const int i) const {
    return inv[i];
}

T typename C::cell_type& C::cell_at_m(const int i) {
    return inv[i];
}

T bool C::is_empty(const int i) const {
    return (get_count(i) == 0);
}

T int C::get_count(const int i) const {
    return inv[i].get_count();
}

T bool C::is_empty() const {
    for (const cell_type& cell : inv) if (not cell.is_empty()) return false;
    return true;
}

T size_t C::get_size() const {
    return inv.size();
}

T fits C::can_push(const int i, const cell_type& pushed) const {
    return cell_at(i).can_push(pushed);
}

T fits C::can_push(const cell_type& pushed) const {
    int count = pushed.get_count();
    for (const C::cell_type& cell : inv) {
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

T void C::push(const int i, cell_type& pushed) {
    cell_at_m(i).push(pushed);
}

T void C::push(cell_type& pushed) {
    for (C::cell_type& cell : inv) {
        if (cell.can_push(pushed) != fits::none) cell.push(pushed);
        if (pushed.is_empty()) return;
    }
}

T void C::push_expand(cell_type& pushed) {
    switch (can_push(pushed)) {
    case fits::none:
        resize(get_size() + 1);
        inv.back().push(pushed);
        break;
    case fits::part:
        push(pushed);
        resize(get_size() + 1);
        inv.back().push(pushed);
        break;
    case fits::full:
        push(pushed);
        break;
    }
}

T typename C::cell_type C::pop(const int i, const int count) {
    return cell_at_m(i).pop(count);
}

T const content_type& C::content_at(const int i) const {
    return inv[i].show_content();
}

T void C::resize(const size_t size) {
    inv.resize(size);
}

#undef T
#undef C

} // namespace mag;

template <typename c_t, int m>
std::ostream& operator<<( std::ostream& stream
                        , const mag::dynamic_inv<c_t, m>& inv)
{
    for (const auto& cell : inv) {
        stream << cell << std::endl;
    }
    return stream;
}

#endif //MAGLIB_INVENTORY_DYNAMIC_INV_H
