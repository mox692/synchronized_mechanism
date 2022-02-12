#include "iostream"
using namespace std;

int main() {
    int num = 0;

    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __sync_fetch_and_add(&num, 1);
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    cout << num << endl;
}