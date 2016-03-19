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
    int get_j() const { return j; }
    static const int max_stack = 37;
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
    void (*test)();
} tests[] = {

{ "direct_inv_cell", []() {
    typedef inv_cell<int, 2> two_int_cell;
    two_int_cell item;
    assert(item.get_max_stack_size() == 2);
    assert(item.get_count() == 0);
    cout << "two_int_cell item before: " << item << endl;

    {
        two_int_cell item2(5, 1);
        const fits result = item.push(item2);
        assert(item.get_count() == 1);
        assert(result == fits::full);
    } {
        two_int_cell item3(5, 1);
        const fits result_two = item.push(item3);
        assert(item.get_count() == 2);
        assert(item3.get_count() == 0);
        assert(result_two == fits::full);
    } {
        two_int_cell item_popped = item.pop(1);
        assert(item_popped.show_content() == 5);
        assert(item.get_count() == 1);
    } {
        two_int_cell item_push(5, 2);
        const fits result_push = item.push(item_push);
        assert(result_push == fits::partial);
    } {
        two_int_cell item_no_fit(3, 1);
        const fits result_no_fit = item.push(item_no_fit);
        assert(result_no_fit == fits::none);
    }
    cout << "two_int_cell item after: " << item << endl;
}},

{ "managed_pointer", []() {
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

{ "pointer_cell", []() {
    typedef inv_cell<m_pointer> cell;
    cell c1(m_pointer(new memory_test(40)), 20);
    cout << "pointer cell before: " << c1 << endl;
    assert(c1.get_max_stack_size() == memory_test::max_stack);
    {
        cell empty;
        assert(empty.get_max_stack_size() == numeric_limits<int>::max());
    } {
        cell c2(m_pointer(new memory_test(40)), 20);
        const int count1 = c1.get_count();
        const int count2 = c2.get_count();
        const fits result = c1.push(c2);
        assert(c1.get_count() + c2.get_count() == count1 + count2);
        assert(result == fits::partial);
    } {
        cell c3 = c1.pop(10);
        assert(c1.show_content().get() != nullptr);
        assert(c3.get_count() == 10);
    }
    cout << "pointer cell after: " << c1 << endl;
}},

{ "fixed inv", []() {
    typedef fixed_size_inv<m_pointer, 2, 4> inv_type;
    typedef inv_type::cell_type cell;
    inv_type inv;
    assert(inv.is_empty(0, 0));
    assert(inv.is_empty());
    {
        cell c(m_pointer(new memory_test(7)), 3);
        inv.push(1, 2, c);
        assert(inv.get_count(1, 2) == 3);
        cell cn(m_pointer(new memory_test(1)), 9);
        inv.push(1, 3, cn);
    } {
        cell c2(m_pointer(new memory_test(5)), 4);
        inv.push(1, 2, c2);
        cell c3(m_pointer(new memory_test(9)), 1);
        inv.push(c3);
        cout << "Push inventory:" << endl << inv;
    } {
        cell c3 = inv.pop(1, 2, 1);
        assert(inv.get_count(1, 2) == 2);
    } {
        for (const cell& c: inv) {
            cout << c << endl;
        }
    }
}},

{ "dynamic inv", []() {
    typedef dynamic_inv<m_pointer, 7> inv_type;
    typedef inv_type::cell_type cell;
    //typedef inv_type::cell_type cell;
    inv_type inv(5);
    assert(inv.is_empty(0));
    assert(inv.is_empty());
    assert(inv.get_size() == 5);
    { // push test
        cell c(m_pointer(new memory_test(13)), 2);
        assert(inv.push(3, c) == fits::full);
        assert(inv.get_count(3) == 2);
    } { // pop test
        cell p = inv.pop(3, 1);
        assert(p.get_count() == 1);
        assert(inv.get_count(3) == 1);
        inv.pop(3, 1);
        assert(inv.get_count(3) == 0);
    } { // show test
        const m_pointer& m = inv.show_at(3);
        assert(m.get()->get_j() == 13);
    } { // unlimited push test
        // fill inventory
        for (size_t i = 0; i < inv.get_size(); ++i) {
            cell c(m_pointer(new memory_test(i)), 1);
            inv.push(i, c);
        }
        const size_t old_size = inv.get_size();
        cell more(m_pointer(new memory_test(12)), 1);
        assert(inv.push(more) == fits::none);
        inv.push_expand(more);
        assert(inv.get_size() == old_size + 1);
    } { // resize test
        inv.resize(10);
        assert(inv.get_size() == 10);
        assert(inv.get_count(3) == 1);
    }
}},

};

int main() {
    for (const auto& test : tests) {
        cout << test.test_name << " test started." << endl;
        test.test();
        cout << test.test_name << " test passed." << endl;
    }
    return 0;
}

