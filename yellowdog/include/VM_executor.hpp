#if !defined(VM_EXECUTOR_HPP)
#define VM_EXECUTOR_HPP 1

#include <functional>
#include <string>

#include "VM_defs.hpp"
#include "VM_labels.hpp"
#include "VM_exec_status.hpp"

class VM_executor
{
private:
    static constexpr unsigned int MAX_STACK_SIZE = 1024;
    static constexpr unsigned int MAX_TICKS = 102400;

public:
    VM_executor(OPCODE const *program, unsigned int length, VM_labels const &labels);
    VM_exec_status exec(bool verbose);

private:
    OPCODE const *program;
    unsigned int program_size;
    VM_labels const &labels;

    int stack[MAX_STACK_SIZE];
    unsigned int sp;
    unsigned int pc;
    unsigned ticks;

    std::string status;

    void reset();

    void is_stack_available(const char *name);
    void is_arg_available(size_t count, const char *name);
    int get_jump_target();

    void do_instructions(std::function<void(void)> instr, size_t argcount, size_t stackneeded, const char * name);
    void do_jump(std::function<void(int)> jump, size_t argcount, const char * name);

    void trace(unsigned int pc, int *stack, unsigned int sp) const;
    void trace_jmp(std::string const &op, unsigned int pc) const;
};

#endif