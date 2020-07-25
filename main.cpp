#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "tiny_asm.h"
int main() {
    std::vector<uint8_t> bytes;
    Emit(bytes, "push", Reg::rbp());
    Emit(bytes, "mov", Reg::rbp(), Reg::rsp());
    Emit(bytes, "mov", Reg::rax(), Addr::QWord(-16, Reg::rbp()));
    Emit(bytes, "mov", Reg::rax(), Imm::DWord(444));
    Emit(bytes, "mov", Reg::rsp(), Reg::rax());
    Emit(bytes, "call", Reg::rax());
    Emit(bytes, "leave");
    Emit(bytes, "ret", Imm::Word(4));
    for (auto &byte : bytes) {
        std::cout << (int) byte << ",";
    }
    bytes.clear();
    std::cout << std::endl;
    Emit(bytes, "movss", XMMReg(0), XMMReg(1));
    Emit(bytes, "movss", XMMReg(1), Addr::QWord(5, Reg::rax()));
    Emit(bytes, "push", Imm::DWord(1234));

    for (auto &byte : bytes) {
        std::cout << (int) byte << ",";
    }
    bytes.clear();
    std::cout << std::endl;
    return 0;
}

