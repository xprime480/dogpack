## Yellow Dog Virtual Machine

This is the Yellow Dog Virtual Machine.  It is a stack machine that supports the following basic 
arithmetical operations: `+`, `-`, `*`, and `/`.  In addition, is supports value testing and both
conditional and unconditional program jumps.

It is defined as follows:

#### Data

The data available to the program consists entirely of a single stack which
can hold integers.  The stack is initialized with no elements.


#### Instructions

The machine supports the following instruction.  In the diagrams below, the
stack grows to the left.  `S` represents 0 or more values on the stack that
are unaffected by the operation.

| Instruction | Before | After | Notes |
| ----------- | ------ | ----- | ----- |
| `PUSH val` | `S` | `val S` | |
| `POP`  | `val S` | `S` | |
| `DUP`  | `val S` | `val val S` | |
| `SWAP` | `x y S` | `y x S` | |
| `ADD` | `x y S` | `y+x S` | |
| `SUB` | `x y S` | `y-x S` | |
| `MUL` | `x y S` | `y*x S` | |
| `DIV` | `x y S` | `y/x S` | |
| `CMP` | `x y S` | `val S` | `val` is -1, 0, 1 according to y <, ==, > x |
| `JMP label` | `S` | `S` | program counter set to `label` | 
| `JEQ label` | `x S` | `S` | program counter set to `label` if `x == 0` | 
| `JLE label` | `x S` | `S` | program counter set to `label` if `x <= 0` | 
| `JLT label` | `x S` | `S` | program counter set to `label` if `x < 0` | 
| `JGT label` | `x S` | `S` | program counter set to `label` if `x > 0` | 
| `JGE label` | `x S` | `S` | program counter set to `label` if `x >= 0` | 
| `JNE label` | `x S` | `S` | program counter set to `label` if `x <> 0` | 
| `label` | `S` | `S` | the next program counter is aliased to `label`|

#### Control Flow

Control Starts at the beginning of a program and executes one instruction at a time until either an error is
encountered, a jump instruction is evaluated, or all instructions have been executed.  If an error is
encountered, a relevant error message is printed and execution stops.  If execution finishes normally, the top 
of the stack is printed.  It is an error if there is no value on the stack at the end.

#### Error Conditions

The following conditions are reported errors:

- `POP`, `DUP`, conditional `Jxx` instruction while stack is empty
- `ADD`, `SUB`, `MUL`, `DIV`, `SWAP`, `CMP` instruction with fewer than two values on the stack.
- `DIV` with `x` equal to 0
- `Jxx` instruction where `label` has not been defined.
- Program termination with empty stack.
- Program executes for more than 64K instructions.

In addition, over- and under-flow of arithmatic operations is silently ignored.

