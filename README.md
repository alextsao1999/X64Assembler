# x64Assembler
a tiny cpp x64 assembler

## Usage

```c++
#include <iostream>
#include <vector>
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
    // Find Instruction And Emit
    auto add_instr = GetInstruct("add", ParamReg64, ParamImm32);
    EmitInst(bytes, add_instr, Reg::rcx(), Imm(1, Size32));
    return 0;
}
```
## Output
```
85,72,139,236,72,139,69,240,72,199,192,188,1,0,0,72,139,224,255,208,201,194,4,0,
243,15,16,193,243,15,16,72,5,104,210,4,0,0,
```

