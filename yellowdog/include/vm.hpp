#if !defined(VM_HPP)
#define VM_HPP

#include <string>
#include <vector>

class VM_exec_status
{
public:
    explicit VM_exec_status(int value);
    explicit VM_exec_status(const char *msg);

    bool is_status_ok() const;
    int get_program_value() const;
    const std::string &get_message() const;

private:
    const bool is_ok;
    const int value;
    const std::string msg;
};

class VM_labels
{
public:
    VM_labels();

    size_t size() const;
    int find(std::string const& name) const;
    int new_label(std::string const & name, int location);
    int add_or_update(std::string const & name, int location);
    int pc_at(int index) const;
    std::string const & name_at(int index) const;

    void dump() const;

private:
    std::vector<std::pair<std::string, int>> labels;
};

class VM
{
private:
    using OPCODE = unsigned char;

    static const unsigned int MAX_PROGRAM_SIZE = 1024u;

    static const OPCODE PUSH = 1;
    static const OPCODE POP = 2;
    static const OPCODE DUP = 3;
    static const OPCODE SWAP = 4;
    static const OPCODE ADD = 5;
    static const OPCODE SUB = 6;
    static const OPCODE MUL = 7;
    static const OPCODE DIV = 8;
    static const OPCODE CMP = 9;
    static const OPCODE JMP = 10;

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
    void label(const std::string &target);

    VM_exec_status exec(bool verbose = false) const;

private:
    OPCODE program[MAX_PROGRAM_SIZE];
    unsigned int program_size;
    bool valid_program;

    VM_labels labels;

    bool maybe_add_op(OPCODE op);
    bool maybe_add_arg(int arg);
    int find_label_index(std::string const &target);
    void program_too_big();

    void trace(unsigned int pc, int *stack, unsigned int sp) const;
};

#endif
