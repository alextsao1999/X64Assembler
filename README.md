# x64Assembler
a tiny cpp x64 assembler

```c++
int main() {
    std::vector<uint8_t> bytes;
    auto ins = GetInstruct("add", ParamReg | Size64, ParamMem | Size64);
    if (ins.need_REX_W()) {
        bytes.push_back(REXPrefix(ins.need_REX_W()));
    }
    if (ins.need_Reg()) {
        ins = ins + RegID::ax;
    }
    bytes.insert(bytes.end(), ins.begin(), ins.end());
    if (ins.need_ModRM()) {
        bytes.push_back(ModRM(Mode::Mode_Mem_Disp8, RegID::ax, RegID::ax));
    }
    //bytes.push_back(SIB(RegID::ax));
    bytes.push_back(8); // Disp8
    for (auto &byte : bytes) {
        std::cout << (int) byte << " ";
    }
    std::cout << std::endl;
    return 0;
}

```

