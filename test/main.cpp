#include "fixed_inventory.h"
#include "inv_pointer_adapter.h"

#include <cassert>
#include <iostream>
#include <inv_cell.h>

using namespace std;
using namespace mag;

class memory_test {
public:
    memory_test() : j(i++)    {++c; if (o) cout << "MT with i + " << j << endl;}
    memory_test(int s) : j(s) {++c; if (o) cout << "MT with i + " << j << endl;}
    ~memory_test()            {--c; if (o) cout << "MT with i - " << j << endl;}
    int get_max_stack_size() const { return max_stack; }
    bool operator==(const memory_test& o) const { return j == o.j; }
    static const int max_stack = 37;
    static int c;
private:
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

    {
        two_int_cell item2(5);
        const mag::push_results result = item.push(item2);
        assert(item.get_count() == 1);
        assert(result == mag::fit_full);
    } {
        two_int_cell item3(5);
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
        two_int_cell item_no_fit(3);
        const mag::push_results result_no_fit = item.push(item_no_fit);
        assert(result_no_fit == mag::fit_none);
    }
}},

{ "inv_pointer_adapter", []() {
    typedef inv_pointer_adapter<memory_test> wrapped;
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
        assert(one != two);
        assert(one.get() == nullptr);
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
    typedef inv_pointer_adapter<memory_test> adapter;
    typedef inv_cell<adapter> cell;
    cell c1(adapter(new memory_test(40)), 20);
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
        assert(c3.get_count() == 10);
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

