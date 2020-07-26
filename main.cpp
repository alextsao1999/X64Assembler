#include <iostream>
#include <vector>
#include "tiny_asm.h"
int main() {
    std::vector<uint8_t> bytes;
    Emit(bytes, "mov", Reg::rax(), Addr::QWord(0, Reg::rax()));
    Emit(bytes, "push", Reg::rbp());
    Emit(bytes, "mov", Reg::rsp(), Reg::r(12));
    Emit(bytes, "movss", XMMReg(0), XMMReg(1));
    Emit(bytes, "movss", XMMReg(1), Addr::QWord(5, Reg::rax()));
    Emit(bytes, "push", Imm::DWord(1234));
    Emit(bytes, "leave");
    Emit(bytes, "ret");

    // Find Instruction And Emit
    auto add_instr = GetInstruct("add", ParamReg64, ParamImm32);
    add_instr.emit(bytes, Reg::rcx(), Imm(1, Size32));

    //Emit(bytes, "push", Imm(1234));

    for (auto &byte : bytes) {
        std::cout << (int) byte << ",";
    }
    return 0;
}
