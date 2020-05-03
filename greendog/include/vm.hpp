#if !defined(VM_HPP)
#define VM_HPP 1

#include "VM_exec_status.hpp"
#include "VM_defs.hpp"

class VM
{
public:
    VM();

    void load(unsigned int reg, unsigned int addr);
    void store(unsigned int reg, unsigned int addr);

    VM_exec_status exec(bool verbose = false);

    void set_heap(unsigned int addr, int value);
    int get_heap(unsigned int addr);

private:
    unsigned int program_size;
    bool valid_program;
    unsigned int program[MAX_PROGRAM_SIZE];
    int heap[MAX_HEAP_SIZE];

    bool check_program_size();
    bool check_register(unsigned int reg);
    bool check_address(unsigned int addr);
    void maybe_add_op_RA(OPCODE op, unsigned int reg, unsigned int addr);
};

#endif
