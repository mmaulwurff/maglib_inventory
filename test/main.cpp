#include "fixed_size_inv.h"
#include "managed_pointer.h"
#include "dynamic_inv.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace mag;

class memory_test {
public:
    memory_test()      : j(i++) { out(); }
    memory_test(int s) : j(s)   { out(); }
    memory_test(const memory_test& other) : j(other.j) { out(); }
    ~memory_test() { --c; if (o) cout << "MT with i - " << j << endl; }
    int get_max_stack_size() const { return max_stack; }
    bool operator==(const memory_test& o) const { return j == o.j; }
    bool operator<(const memory_test& o) const { return j < o.j; }
    int get_j() const { return j; }
    static const int max_stack = 10;
    static int c;
private:
    void out() { ++c; if (o) cout << "MT with i + " << j << endl; }
    static const bool o = false;
    static int i;
    const  int j;
}; int memory_test::i = 0; int memory_test::c = 0;

ostream& operator<<(ostream& str, const memory_test& m) {
    return (str << m.get_j());
}

typedef managed_pointer<memory_test> m_pointer;

const struct {
    string test_name;
    int enabled;
    void (*test)();
} tests[] = {

{ "direct_inv_cell", 1, []() {
    typedef inv_cell<int, 2> two_int_cell;
    two_int_cell item;
    assert(item.get_count() == 0);
    cout << "two_int_cell item before: " << item << endl;

    {
        two_int_cell item2(5, 1);
        const fits result = item.can_push(item2);
        assert(result == fits::full);
        item.push(item2);
        assert(item.get_count() == 1);
    } {
        two_int_cell item3(5, 1);
        const fits result_two = item.can_push(item3);
        assert(result_two == fits::full);
        item.push(item3);
        assert(item.get_count() == 2);
        assert(item3.get_count() == 0);
    } {
        two_int_cell item_popped = item.pop(1);
        assert(item_popped.show_content() == 5);
        assert(item.get_count() == 1);
    } {
        two_int_cell item_push(5, 2);
        const fits result_push = item.can_push(item_push);
        assert(result_push == fits::part);
        item.push(item_push);
    } {
        const two_int_cell item_no_fit(3, 1);
        const fits result_no_fit = item.can_push(item_no_fit);
        assert(result_no_fit == fits::none);
    }
    cout << "two_int_cell item after: " << item << endl;
    { // push into full cell test
        const two_int_cell full(3, 2), other(3, 1);
        assert(full.can_push(other) == fits::none);
    }
}},

{ "managed_pointer", 1, []() {

    { // basic constructor/destructor test
        m_pointer one(new memory_test);
        m_pointer two(new memory_test);
        assert(one != two); // different pointers

    } { // assignment test
        assert(memory_test::c == 0);
        m_pointer one(new memory_test);
        m_pointer two(new memory_test);
        one = two;
        assert(one != two);
        assert(two.get() == nullptr);

    } { // copy constructor test
        assert(memory_test::c == 0);
        m_pointer one(new memory_test);
        m_pointer two(one);
        assert(one == two);

    } { // move constructor test
        assert(memory_test::c == 0);
        m_pointer source(new memory_test);
        m_pointer dest(move(source));
        assert(source.get() == nullptr);
        assert(source != dest);

    } { // release test
        assert(memory_test::c == 0);
        m_pointer one(new memory_test);
        delete one.release();
        assert(one.get() == nullptr);

    } { // same pointer
        assert(memory_test::c == 0);
        memory_test* const p_one = new memory_test;
        m_pointer one(p_one);
        m_pointer two(p_one);
        one = two;
        assert(one.get() == p_one);
        assert(two.get() == nullptr);
    }

    assert(memory_test::c == 0);
}},

{ "max stack size", 1, []() {

    { // default, implicit
        inv_cell<int> cell1;
        assert(cell1.get_max_stack_size() == 1);

    } { // explicit
        inv_cell<int, 2> cell2;
        assert(cell2.get_max_stack_size() == 2);

    } { // empty cell doesn't have object to determine max stack size:
        inv_cell<m_pointer, 3> cell3;
        assert(cell3.get_max_stack_size() == m_pointer::null_max_stack_size);

    } { // now cell know max stack size from object
        inv_cell<m_pointer, 4> cell4(new memory_test());
        assert(cell4.get_max_stack_size() == memory_test::max_stack);
    }
}},

{ "pointer_cell", 1, []() {
    typedef inv_cell<m_pointer> cell;
    static const int max_stack = memory_test::max_stack;
    cell c1(new memory_test(40), max_stack / 2);

    { // full fit
        cell c2(new memory_test(40), max_stack / 2 - 1);
        assert(c1.can_push(c2) == fits::full);
        c1.push(c2);

    } { // partial fit
        cell c3(new memory_test(40), max_stack / 2);
        assert(c1.can_push(c3) == fits::part);
        c1.push(c3);
        assert(c1.get_count() == c1.get_max_stack_size());
        assert(c3.get_count() == max_stack / 2 - 1);

    } { // pop
        cell c4 = c1.pop(1);
        assert(c4.get_count() == 1);
        assert(c1.get_count() == max_stack - 1);
    }
}},

{ "fixed inv", 1, []() {
    typedef fixed_size_inv<m_pointer, 2, 4> inv_type;
    typedef inv_type::cell_type cell;
    inv_type inv;
    assert(inv.is_empty(0, 0));
    assert(inv.is_empty());

    { // push test
        cell c(new memory_test(7), 3);
        inv.push(1, 2, c);
        assert(inv.get_count(1, 2) == 3);
        cell c2(new memory_test(5), 4);
        assert(inv.can_push(1, 2, c2) == fits::none);

    } { // pop test
        cell c3 = inv.pop(1, 2, 1);
        assert(inv.get_count(1, 2) == 2);

    } { // iterator test
        for (const cell& c: inv) {
            cout << c << endl;
        }
    }
}},

{ "dynamic inv", 1, []() {
    typedef dynamic_inv<m_pointer> inv_type;
    typedef inv_type::cell_type cell;
    typedef dynamic_inv<int> int_inv_type;
    typedef int_inv_type::cell_type int_cell;

    { // construction test
        inv_type inv(5);
        assert(inv.is_empty());
        assert(inv.is_empty(0));
        assert(inv.get_count(0) == 0);
        assert(inv.get_size() == 5);

    } { // push test
        const int size = 5;
        inv_type inv(size);
        // fill inventory
        for (int i = 0; i < size; ++i) {
            cell c(new memory_test(i));
            assert(inv.can_push(c) == fits::full);
            inv.push(c);
        }
        { // object is equal to one of the cells
            cell equals(new memory_test(1));
            assert(inv.can_push(equals) == fits::full);
            inv.push(equals);
        } { // object is not equal to any of the cells.
            cell not_equals(new memory_test(-1));
            assert(inv.can_push(not_equals) == fits::none);
        } { // push to exact cell, object is equal:
            cell equals(new memory_test(0));
            assert(inv.can_push(0, equals) == fits::full);
            inv.push(equals);
        } { // push to exact cell, object is not equal:
            cell not_equal(new memory_test(0));
            assert(inv.can_push(1, not_equal) == fits::none);
        }

    } { // expand push test
        inv_type inv(3);
        for (int i = 0; i < 3; ++i) {
            cell p(new memory_test);
            inv.push(p);
        }
        cell expand(new memory_test);
        inv.push_expand(expand);
        assert(inv.get_size() == 4);

    } { // pop test
        inv_type inv(5);
        cell c(new memory_test);
        inv.push(c);
        cell p = inv.pop(0, 1);
        assert(p.get_count() == 1);
        assert(inv.is_empty());
        const m_pointer& remainder = inv.content_at(0);
        assert(remainder.get() == nullptr);

    } { // int reset test
        int_inv_type inv(5);
        int_cell c(3);
        inv.push(c);
        int_cell p = inv.pop(0, 1);
        assert(p.show_content() == 3);
        assert(inv.content_at(0) == 0);

    } { // show test
        dynamic_inv<int, 10> inv(5);
        decltype(inv)::cell_type cell(50, 3);
        inv.push(cell);
        assert(inv.content_at(0) == 50);
        assert(inv.cell_at(0).get_count() == 3);
        assert(inv.cell_at(0).show_content() == 50);

    } { // resize test
        int_inv_type inv(2);
        for (size_t i = 0; i < inv.get_size(); ++i) {
            int_cell cell(i);
            inv.push(cell);
        }
        inv.resize(3);
        assert(inv.get_size() == 3);
        assert(inv.content_at(0) == 0);
        assert(inv.content_at(1) == 1);
    }
}},

};

int main() {
    for (const auto& test : tests) {
        if (test.enabled == 0) continue;
        cout << test.test_name << " test started." << endl;
        test.test();
        cout << test.test_name << " test passed." << endl;
    }
    return 0;
}

