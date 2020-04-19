#include <cstring>

#include "../include/vm.hpp"

using namespace std;

VM_exec_status::VM_exec_status(int value)
    : is_ok(true), value(value), msg("Execution OK")
{
}

VM_exec_status::VM_exec_status(const char *msg)
    : is_ok(false), value(-1), msg(msg)
{
}

bool VM_exec_status::is_status_ok() const
{
    return is_ok;
}

int VM_exec_status::get_program_value() const
{
    return value;
}

const std::string &VM_exec_status::get_message() const
{
    return msg;
}

VM::VM()
    : program_size(0u), valid_program(true)
{
}

void VM::push(int val)
{
    if (maybe_add_op(PUSH))
    {
        if ((program_size + sizeof(int)) <= MAX_PROGRAM_SIZE)
        {
            memcpy((void *)&program[program_size], (void *)&val, sizeof(int));
            program_size += sizeof(int);
        }
        else
        {
            program_too_big();
        }
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

VM_exec_status VM::exec() const
{
    if (!valid_program)
    {
        return VM_exec_status("Cannot execute invalid program");
    }

    int stack[1024];
    unsigned int sp = 0;
    unsigned int pc = 0;

    while (pc < program_size)
    {
        OPCODE op = program[pc++];
        switch (op)
        {
        case PUSH:
        {
            int val;
            memcpy((void *)&val, (void *)&program[pc], sizeof(int));
            pc += sizeof(int);
            stack[sp++] = val;
        }
        break;

        case POP:
        {
            if (0 == sp)
            {
                return VM_exec_status("Cannot POP empty stack");
            }
            --sp;
        }
        break;

        case DUP:
        {
            if (0 == sp)
            {
                return VM_exec_status("Cannot DUP empty stack");
            }
            stack[sp] = stack[sp - 1];
            ++sp;
        }
        break;

        case ADD:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to ADD");
            }

            stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
            --sp;
        }
        break;

        case SUB:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to SUB");
            }

            stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
            --sp;
        }
        break;

        case MUL:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to MUL");
            }

            stack[sp - 2] = stack[sp - 2] * stack[sp - 1];
            --sp;
        }
        break;

        case DIV:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to DIV");
            }
            if (0 == stack[sp - 1])
            {
                return VM_exec_status("Division by zero not allowed");
            }

            stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
            --sp;
        }
        break;

        default:
            return VM_exec_status("Internal Error: Invalid OPCODE detected");
        }
    }

    if (0 == sp)
    {
        return VM_exec_status("Program produced no value");
    }

    return VM_exec_status(int(stack[sp - 1]));
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

void VM::program_too_big()
{
    valid_program = false;
}
