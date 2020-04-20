#include <cstring>
#include <iostream>

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

VM_labels::VM_labels()
{
    labels.reserve(20);
}

size_t VM_labels::size() const
{
    return labels.size();
}

int VM_labels::find(std::string const &name) const
{
    for (int i = 0; i < size(); ++i)
    {
        if (name == labels[i].first)
        {
            return i;
        }
    }

    return -1;
}

int VM_labels::new_label(std::string const &name, int location)
{
    //cerr << "new_label: " << name << " " << location << "\n";
    if (find(name) > 0)
    {
        //cerr << "\talready exists\n";
        return -1;
    }
    return add_or_update(name, location);
}

int VM_labels::add_or_update(std::string const &name, int location)
{
    //cerr << "add_or_update: " << name << " " << location << "\n";
    int index = find(name);
    if (index < 0)
    {
        index = size();
        labels.push_back(pair<string, int>(name, location));
        //cerr << "\tbrand new at index " << index << "\n";
        return index;
    }

    if (labels[index].second >= 0)
    {
        //cerr << "\tduplicate\n";
        return -1;
    }

    //cerr << "updated\n";
    labels[index].second = location;
    return index;
}

int VM_labels::pc_at(int index) const
{
    if (index < size())
    {
        return labels[index].second;
    }
    return -1;
}

std::string const &VM_labels::name_at(int index) const
{
    if (index < size())
    {
        return labels[index].first;
    }
    static const string empty("");
    return empty;
}

void VM_labels::dump() const
{
    cerr << "Labels [" << size() << "]\n";
    for (size_t i = 0; i < size(); ++i)
    {
        cerr << "\t" << labels[i].first << ": " << labels[i].second << "\n";
    }
}

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
    if (maybe_add_op(JMP))
    {
        int index = labels.add_or_update(target, -1);
        maybe_add_arg(index);
    }
}

void VM::label(const std::string &target)
{
    if (!valid_program)
    {
        return;
    }

    if (!labels.add_or_update(target, program_size))
    {
        valid_program = false;
    }
}

VM_exec_status VM::exec(bool verbose) const
{
    if (verbose)
    {
        cerr << "Starting program execution\n";
        labels.dump();
    }

    if (!valid_program)
    {
        return VM_exec_status("Cannot execute invalid program");
    }

    static const unsigned int MAX_STACK_SIZE = 1024;
    static const unsigned int MAX_TICKS = 102400;

    int stack[MAX_STACK_SIZE];
    unsigned int sp = 0;
    unsigned int pc = 0;
    unsigned ticks = 0;

    while (pc < program_size)
    {
        if (++ticks > MAX_TICKS)
        {
            return VM_exec_status("Max Runtime Exceeded");
        }

        if (verbose)
        {
            trace(pc, stack, sp);
        }

        OPCODE op = program[pc++];
        switch (op)
        {
        case PUSH:
        {
            if (MAX_STACK_SIZE == sp)
            {
                return VM_exec_status("Stack overflow on PUSH");
            }

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
            if (MAX_STACK_SIZE == sp)
            {
                return VM_exec_status("Stack overflow on DUP");
            }
            if (0 == sp)
            {
                return VM_exec_status("Cannot DUP empty stack");
            }
            stack[sp] = stack[sp - 1];
            ++sp;
        }
        break;

        case SWAP:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to SWAP");
            }

            std::swap(stack[sp - 2], stack[sp - 1]);
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

        case CMP:
        {
            if (sp < 2)
            {
                return VM_exec_status("Too few items on stack to CMP");
            }

            int lhs = stack[sp - 2];
            int rhs = stack[sp - 1];

            stack[sp - 2] = (lhs < rhs) ? -1 : ((lhs > rhs) ? +1 : 0);
            --sp;
        }
        break;

        case JMP:
        {
            int index;
            memcpy((void *)&index, (void *)&program[pc], sizeof(int));
            int next_pc = labels.pc_at(index);
            if (next_pc < 0)
            {
                return VM_exec_status("Label was never defined");
            }

            pc = (unsigned int)next_pc;
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

void VM::trace(unsigned int pc, int *stack, unsigned int sp) const
{
    cerr << "STACK[" << sp << "]: ";
    for (unsigned int i = 3; i > 0 && sp > 0; --i)
    {
        cerr << stack[--sp] << " ";
    }
    cerr << "\n";

    cerr << "PC: " << pc << "\n";

    OPCODE op = program[pc++];
    switch (op)
    {
    case PUSH:
    {
        int val;
        memcpy((void *)&val, (void *)&program[pc], sizeof(int));
        cerr << "PUSH " << val << "\n";
    }
    break;

    case POP:
        cerr << "POP\n";
        break;

    case DUP:
        cerr << "DUP\n";
        break;

    case ADD:
        cerr << "ADD\n";
        break;

    case SUB:
        cerr << "SUB\n";
        break;

    case MUL:
        cerr << "MUL\n";
        break;

    case DIV:
        cerr << "DIV\n";
        break;

    case CMP:
        cerr << "CMP\n";
        break;

    case JMP:
    {
        int index;
        memcpy((void *)&index, (void *)&program[pc], sizeof(int));

        string const &name = labels.name_at(index);
        if (name.empty())
        {
            cerr << "JMP ???";
        }
        else
        {
            cerr << "JMP " << name;
            int next_pc = labels.pc_at(index);
            if (next_pc < 0)
            {
                cerr << " (never defined)";
                cerr << "\n";
            }
        }
    }
    break;

    default:
        cerr << "unknown op code: " << op << "\n";
    }
}
