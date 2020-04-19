#if !defined(VM_HPP)
#define VM_HPP

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

public:
    VM();

    void push(int val);
    void pop();
    void dup();
    void add();
    void sub();
    void mul();
    void div();

    void exec() const;

private:
    OPCODE program[MAX_PROGRAM_SIZE];
    unsigned int program_size;
    bool valid_program;

    bool maybe_add_op(OPCODE op);
    void program_too_big();
};

#endif
