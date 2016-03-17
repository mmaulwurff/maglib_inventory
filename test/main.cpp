#include "fixed_size_inv.h"
#include "managed_pointer.h"

#include <cassert>
#include <iostream>
#include <inv_cell.h>

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
    static const int max_stack = 37;
    static int c;
private:
    void out() { ++c; if (o) cout << "MT with i + " << j << endl; }
    static const bool o = false;
    static int i;
    const  int j;
}; int memory_test::i = 0; int memory_test::c = 0;

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
        const mag::push_results result = item.push(item2);
        assert(item.get_count() == 1);
        assert(result == mag::fit_full);
    } {
        two_int_cell item3(5, 1);
        const mag::push_results result_two = item.push(item3);
        assert(item.get_count() == 2);
        assert(item3.get_count() == 0);
        assert(result_two == mag::fit_full);
    } {
        two_int_cell item_popped = item.pop(1);
        assert(item_popped.showContent() == 5);
        assert(item.get_count() == 1);
    } {
        two_int_cell item_push(5, 2);
        const mag::push_results result_push = item.push(item_push);
        assert(result_push == mag::fit_partial);
    } {
        two_int_cell item_no_fit(3, 1);
        const mag::push_results result_no_fit = item.push(item_no_fit);
        assert(result_no_fit == mag::fit_none);
    }
    cout << "two_int_cell item after: " << item << endl;
}},

{ "managed_pointer", []() {
    typedef managed_pointer<memory_test> wrapped;
    { // basic constructor/destructor test
        wrapped one(new memory_test);
        wrapped two(new memory_test);
        assert(one != two); // different pointers
    } { // assignment test
        assert(memory_test::c == 0);
        wrapped one(new memory_test);
        wrapped two(new memory_test);
        one = two;
        assert(one != two);
        assert(two.get() == nullptr);
    } { // copy constructor test
        assert(memory_test::c == 0);
        wrapped one(new memory_test);
        wrapped two(one);
        assert(one == two);
    } { // move constructor test
        assert(memory_test::c == 0);
        wrapped source(new memory_test);
        wrapped dest(move(source));
        assert(source.get() == nullptr);
        assert(source != dest);
    } { // release test
        assert(memory_test::c == 0);
        wrapped one(new memory_test);
        delete one.release();
        assert(one.get() == nullptr);
    } { // same pointer
        assert(memory_test::c == 0);
        memory_test* const p_one = new memory_test;
        wrapped one(p_one);
        wrapped two(p_one);
        one = two;
        assert(one.get() == p_one);
        assert(two.get() == nullptr);
    }
    assert(memory_test::c == 0);
}},

{ "pointer_cell", []() {
    typedef managed_pointer<memory_test> adapter;
    typedef inv_cell<adapter> cell;
    cell c1(adapter(new memory_test(40)), 20);
    cout << "pointer cell before: " << c1 << endl;
    assert(c1.get_max_stack_size() == memory_test::max_stack);
    {
        cell empty;
        assert(empty.get_max_stack_size() == numeric_limits<int>::max());
    } {
        cell c2(adapter(new memory_test(40)), 20);
        const int count1 = c1.get_count();
        const int count2 = c2.get_count();
        const mag::push_results result = c1.push(c2);
        assert(c1.get_count() + c2.get_count() == count1 + count2);
        assert(result == mag::fit_partial);
    } {
        cell c3 = c1.pop(10);
        assert(c1.showContent().get() != nullptr);
        assert(c3.get_count() == 10);
    }
    cout << "pointer cell after: " << c1 << endl;
}},

{ "fixed inv", []() {
    typedef inv_cell<managed_pointer<memory_test>, 10> cell;
    typedef managed_pointer<memory_test> m_pointer;
    fixed_size_inv<cell, 2, 4> inv;
    cout << "Empty inventory:" << endl << inv;
    {
        cell c(m_pointer(new memory_test(7)), 3);
        inv.push(1, 2, c);
        cell cn(m_pointer(new memory_test(1)), 9);
        inv.push(1, 3, cn);
    } {
        cell c2(m_pointer(new memory_test(5)), 4);
        inv.push(1, 2, c2);
        cout << "Push inventory:" << endl << inv;
    } {
        cell c3 = inv.pop(1, 2, 1);
        cout << "Pop inventory:" << endl << inv;
    } {
        for (const cell& c: inv) {
            cout << c << endl;
        }
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

