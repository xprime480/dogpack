#if !defined(VM_EXECUTOR_HPP)
#define VM_EXECUTOR_HPP 1

#include <functional>

#include "vm_defs.hpp"
#include "VM_exec_status.hpp"

class VM_executor
{
private:

public:
    VM_executor(unsigned int const *program, unsigned int length, int * heap);
    VM_exec_status exec(bool verbose);

private:
    unsigned int const *program;
    unsigned int program_size;
    int * heap;
    int registers[MAX_REGISTERS];

    unsigned int pc;
    unsigned ticks;

    std::string status;

    void reset();

    struct Instruction
    {
        Instruction(unsigned int code, bool verbose = false);

        OPCODE op;
        unsigned int r1;
        unsigned int r2;
        unsigned int r3;
        unsigned int addr;
        unsigned int loc;

    private:
    
        void decode_RA(unsigned int code);
        void decode_RRR(unsigned int code);
        void decode_L(unsigned int code);
        void decode_RL(unsigned int code);
    };

    void do_instructions(std::function<void(void)> instr, const char *name);
    void do_jump(Instruction const & instr, std::function<bool()> test, const char *name);

    void trace(Instruction const & instr) const;
};

#endif
