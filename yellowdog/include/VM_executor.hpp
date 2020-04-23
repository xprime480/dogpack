#if !defined(VM_EXECUTOR_HPP)
#define VM_EXECUTOR_HPP 1

#include "VM_defs.hpp"
#include "VM_labels.hpp"
#include "VM_exec_status.hpp"

class VM_executor
{
private:
    static constexpr unsigned int MAX_STACK_SIZE = 1024;
    static constexpr unsigned int MAX_TICKS = 102400;

public:
    VM_executor(OPCODE const * program, unsigned int length, VM_labels const & labels);
    VM_exec_status exec(bool verbose);

private:
    OPCODE const * program;
    unsigned int program_size;
    VM_labels const & labels;

    int stack[MAX_STACK_SIZE];
    unsigned int sp;
    unsigned int pc;
    unsigned ticks;

    void reset();
    void trace(unsigned int pc, int *stack, unsigned int sp) const;
    void trace_jmp(std::string const & op, unsigned int pc) const;
};

#endif