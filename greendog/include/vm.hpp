#if !defined(VM_HPP)
#define VM_HPP 1

#include "VM_exec_status.hpp"

class VM
{
private:
    static constexpr unsigned int MAX_PROGRAM_SIZE = 1024u;

public:
    VM();

    VM_exec_status exec(bool verbose = false) const;

private:
    unsigned int program_size;
    bool valid_program;
};

#endif
