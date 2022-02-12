#include "iostream"
using namespace std;

int main() {
    int a = 0;
    __sync_lock_test_and_set(&a, 3);
}