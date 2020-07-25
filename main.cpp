#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "tiny_asm.h"
int main() {
    std::vector<uint8_t> bytes;
    Emit(bytes, "push", Register::rbp());
    Emit(bytes, "mov", Register::rbp(), Register::rsp());
    Emit(bytes, "mov", Register::rax(), Address::QWord(-16, Register::rbp()));
    Emit(bytes, "leave");
    Emit(bytes, "ret", Immdiate::Word(4));
    for (auto &byte : bytes) {
        std::cout << (int) byte << ",";
    }
    std::cout << std::endl;
    return 0;
}

