#include "iostream"
using namespace std;

int a = 0;
int main() {
    __sync_bool_compare_and_swap(&a, 42, 24);
}