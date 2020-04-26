#if !defined(VM_DEFS_HPP)
#define VM_DEFS_HPP 1

using OPCODE = unsigned char;

constexpr OPCODE PUSH = 1;
constexpr OPCODE POP = 2;
constexpr OPCODE DUP = 3;
constexpr OPCODE DUPN = 4;
constexpr OPCODE SWAP = 5;
constexpr OPCODE ADD = 6;
constexpr OPCODE SUB = 7;
constexpr OPCODE MUL = 8;
constexpr OPCODE DIV = 9;
constexpr OPCODE CMP = 10;
constexpr OPCODE JMP = 11;
constexpr OPCODE JEQ = 12;
constexpr OPCODE JNE = 13;
constexpr OPCODE JLT = 14;
constexpr OPCODE JLE = 15;
constexpr OPCODE JGT = 16;
constexpr OPCODE JGE = 17;

#endif