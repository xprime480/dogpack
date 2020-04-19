## White Dog Virtual Machine

This is the White Dog Virtual Machine.  It is close to the simplest VM
I can think of.  It is in essence a reverse polish notation calculator
for fixed expressions, with the four basic arithmetical operations `+`,
`-`, `*`, `/`.

It is defined as follows:

#### Data

The data available to the program consists entirely of a single stack which
can hold integers.  The stack is initialized with no elements.


#### Instructions

The machine supports the following instruction.  In the diagrams below, the
stack grows to the left.  `S` represents 0 or more values on the stack that
are unaffected by the operation.

| Instruction | Before | After |
| ----------- | ------ | ----- |
| `PUSH val` | `S` | `val S` |
| `POP`  | `val S` | `S` |
| `DUP`  | `val S` | `val val S` |
| `ADD` | `x y S` | `y+x S` |
| `SUB` | `x y S` | `y-x S` |
| `MUL` | `x y S` | `y*x S` |
| `DIV` | `x y S` | `y/x S` |

#### Control Flow

Control Starts at the beginning of a program and executes one instruction at
a time until either an error is encountered or all instructions have been
executed.  If an error is encountered, a relevant error message is printed
and execution stops.  If execution finishes normally, the top of the stack is
printed.  It is an error if there is no value on the stack at the end.

#### Error Conditions

The following conditions are reported errors:

- `POP` or `DUP` instruction while stack is empty
- `ADD`, `SUB`, `MUL`, `DIV` instruction with fewer than two values on the stack.
- `DIV` with `x` equal to 0
- Program termination with empty stack.

In addition, over- and under-flow of arithmatic operations is silently ignored.

