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

class VM
{
private:
    using OPCODE = unsigned char;

    static const unsigned int MAX_PROGRAM_SIZE = 1024u;

    static const OPCODE PUSH = 1;
    static const OPCODE POP = 2;
    static const OPCODE DUP = 3;
    static const OPCODE ADD = 4;
    static const OPCODE SUB = 5;
    static const OPCODE MUL = 6;
    static const OPCODE DIV = 7;

    static const OPCODE JMP = 9;

public:
    VM();

    void push(int val);
    void pop();
    void dup();
    void add();
    void sub();
    void mul();
    void div();

    void jmp(const std::string & target);
    void label(const std::string & target);

    VM_exec_status exec(bool verbose = false) const;

private:
    OPCODE program[MAX_PROGRAM_SIZE];
    unsigned int program_size;
    bool valid_program;

    std::vector<std::pair<std::string, int>> labels;

    bool maybe_add_op(OPCODE op);
    bool maybe_add_arg(int arg);
    int find_label_index(std::string const & target);
    void program_too_big();

    void trace(unsigned int pc, int * stack, unsigned int sp) const;
};

#endif
