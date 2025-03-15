#include <iostream>
#include <cassert>

int add(int a, int b) {
    return a + b;
}

void test_add() {
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);
    assert(add(0, 0) == 0);
    assert(add(-5, -3) == -8);
    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_add();
    return 0;
}

