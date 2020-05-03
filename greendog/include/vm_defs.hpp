#if !defined(VM_DEFS_HPP)
#define VM_DEFS_HPP 1

const constexpr unsigned int MAX_TICKS = 102400;
const constexpr unsigned int MAX_PROGRAM_SIZE = 1024u;
const constexpr unsigned int MAX_REGISTERS    = 32u;
const constexpr unsigned int MAX_HEAP_SIZE    = 8192u;

using OPCODE = unsigned char;

const constexpr OPCODE LOAD = 1;
const constexpr OPCODE STORE = 2;

#endif
