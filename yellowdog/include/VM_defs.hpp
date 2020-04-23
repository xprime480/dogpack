#if !defined(VM_DEFS_HPP)
#define VM_DEFS_HPP 1

using OPCODE = unsigned char;

constexpr OPCODE PUSH = 1;
constexpr OPCODE POP = 2;
constexpr OPCODE DUP = 3;
constexpr OPCODE SWAP = 4;
constexpr OPCODE ADD = 5;
constexpr OPCODE SUB = 6;
constexpr OPCODE MUL = 7;
constexpr OPCODE DIV = 8;
constexpr OPCODE CMP = 9;
constexpr OPCODE JMP = 10;
constexpr OPCODE JEQ = 11;
constexpr OPCODE JNE = 12;
constexpr OPCODE JLT = 13;
constexpr OPCODE JLE = 14;
constexpr OPCODE JGT = 15;
constexpr OPCODE JGE = 16;

#endif