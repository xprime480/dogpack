#if !defined(VM_HPP)
#define VM_HPP

#include <string>
#include <vector>

#include "VM_defs.hpp"
#include "VM_exec_status.hpp"
#include "VM_labels.hpp"

class VM
{
private:
    static constexpr unsigned int MAX_PROGRAM_SIZE = 1024u;

public:
    VM();

    void push(int val);
    void pop();
    void dup();
    void swap();
    void add();
    void sub();
    void mul();
    void div();
    void cmp();
    void jmp(const std::string &target);
    void jeq(const std::string &target);
    void jne(const std::string &target);
    void jlt(const std::string &target);
    void jle(const std::string &target);
    void jgt(const std::string &target);
    void jge(const std::string &target);
    void label(const std::string &target);

    VM_exec_status exec(bool verbose = false) const;

private:
    OPCODE program[MAX_PROGRAM_SIZE];
    unsigned int program_size;
    bool valid_program;

    VM_labels labels;

    void maybe_add_jmp(OPCODE op, std::string const & target);
    bool maybe_add_op(OPCODE op);
    bool maybe_add_arg(int arg);
    void program_too_big();
};

#endif
