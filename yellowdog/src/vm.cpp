#include <cstring>
#include <iostream>

#include "vm.hpp"
#include "VM_executor.hpp"

using namespace std;

VM::VM()
    : program_size(0u), valid_program(true)
{
}

void VM::push(int val)
{
    if (maybe_add_op(PUSH))
    {
        maybe_add_arg(val);
    };
}

void VM::pop()
{
    maybe_add_op(POP);
}

void VM::dup()
{
    maybe_add_op(DUP);
}

void VM::dupn(int which)
{
    if (maybe_add_op(DUPN))
    {
        maybe_add_arg(which);
    }
}

void VM::swap()
{
    maybe_add_op(SWAP);
}

void VM::add()
{
    maybe_add_op(ADD);
}

void VM::sub()
{
    maybe_add_op(SUB);
}

void VM::mul()
{
    maybe_add_op(MUL);
}

void VM::div()
{
    maybe_add_op(DIV);
}

void VM::cmp()
{
    maybe_add_op(CMP);
}

void VM::jmp(const std::string &target)
{
    maybe_add_jmp(JMP, target);
}

void VM::jeq(const std::string &target)
{
    maybe_add_jmp(JEQ, target);
}

void VM::jne(const std::string &target)
{
    maybe_add_jmp(JNE, target);
}

void VM::jlt(const std::string &target)
{
    maybe_add_jmp(JLT, target);
}

void VM::jle(const std::string &target)
{
    maybe_add_jmp(JLE, target);
}

void VM::jgt(const std::string &target)
{
    maybe_add_jmp(JGT, target);
}

void VM::jge(const std::string &target)
{
    maybe_add_jmp(JGE, target);
}

void VM::label(const std::string &target)
{
    if (!valid_program)
    {
        // cerr << "Program already invalid\n";
        return;
    }

    if (labels.add_or_update(target, program_size) < 0)
    {
        // cerr << "becoming invalid because of bad return code from labels.add_or_update\n";
        valid_program = false;
    }
}

VM_exec_status VM::exec(bool verbose) const
{
    if (!valid_program)
    {
        return VM_exec_status("Cannot execute invalid program");
    }

    if (verbose)
    {
        cerr << "Starting program execution\n";
        labels.dump();
        cerr << "program_size = " << program_size << "\n";
    }

    VM_executor executor(program, program_size, labels);
    return executor.exec(verbose);
}

void VM::maybe_add_jmp(OPCODE op, string const &target)
{
    if (maybe_add_op(op))
    {
        int index = labels.add_or_update(target, -1);
        maybe_add_arg(index);
    }
}

bool VM::maybe_add_op(OPCODE op)
{
    if (valid_program)
    {
        if (program_size < MAX_PROGRAM_SIZE)
        {
            program[program_size++] = op;
        }
        else
        {
            program_too_big();
        }
    }

    return valid_program;
}

bool VM::maybe_add_arg(int arg)
{
    if ((program_size + sizeof(int)) <= MAX_PROGRAM_SIZE)
    {
        memcpy((void *)&program[program_size], (void *)&arg, sizeof(int));
        program_size += sizeof(int);
    }
    else
    {
        program_too_big();
    }

    return valid_program;
}

void VM::program_too_big()
{
    valid_program = false;
}
