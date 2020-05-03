
#include "vm.hpp"

VM::VM()
    : program_size(0u), valid_program(true)
{
}

VM_exec_status VM::exec(bool verbose) const
{
    return VM_exec_status("Empty program");
}