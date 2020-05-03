## Green Dog Virtual Machine

This is the Green Dog Virtual Machine.  It is a register machine.  This
version has 32 32-bit registers.   See below for supported operators.
There are two types of data:  The program area which is read-only and
the heap which is read-write and accessible to both the VM and the
caller.

It is defined as follows:

#### Data

The machine has the following kinds of data:  First, there is a program
area which consists of 32-bit instructions (see below).  A program may
contain up to 1024 instructions.  Second, there is a heap of 32-bit
words.  The heap is up to 8192 words and is initialized by the calling
program, is available to it after the VM finishes.  Finally, there are
32 registers named r00 through r31.

#### Instructions

The machine supports the following instructions.  The general format of
a 32 bit instruction is 8 bits for a tag (operator) and 3 eight bit
fields for operands.  Unused operands are ignored.  Instructions that
reference the heap use the first operand for the register involved and
combine the next two for the heap address.

In the following, `rNN` refers to a register numbered in `[0..31]`.
`addr` refers to a heap address between 0 and 8191.  `loc` refers to a 
program counter value between 0 and 1023.  The table describes the
supported instructions.

| Instruction | Notes |
| ----------- | ----- |
| `LOAD rNN addr` | Contents of  `addr` copied to `rNN` |
| `STORE rNN addr` | Contents of `rNN` copied to `addr` |
| `ADD rN1 rN2 rN3` | `rN3` assigned value of `rN1 + rn2` |
| `SUB rN1 rN2 rN3` | `rN3` assigned value of `rN1 - rN2` |
| `MUL rN1 rN2 rN3` | `rN3` assigned value of `rN1 * rN2` |
| `DIV rN1 rN2 rN3` | `rN3` assigned value of `rN1 / rN2` |
| `CMP rN1 rN2 rN3` | `rN3` assigned value of -1, 0, 1 according to `rN1` <, ==, > `rN2` |
| `JMP loc` | program counter set to `loc` | 
| `JEQ rNN loc` | program counter set to `loc` if `rNN == 0` | 
| `JLE rNN loc` | program counter set to `loc` if `rNN <= 0` | 
| `JLT rNN loc` | program counter set to `loc` if `rNN < 0` | 
| `JGT rNN loc` | program counter set to `loc` if `rNN > 0` | 
| `JGE rNN loc` | program counter set to `loc` if `rNN >= 0` | 
| `JNE rNN loc` | program counter set to `loc` if `rNN <> 0` | 

#### Control Flow

Control Starts at the beginning of a program (pc = 0) and executes one
instruction at a time until either an error is encountered, a jump
instruction is evaluated, or all instructions have been executed.  If
an error is encountered, a relevant error message is returned to the
caller and execution stops.  If execution finishes normally, the
program returns an empty message.  The heap will be available for 
inspection in case the program needs to retrieve any computed values.

#### Error Conditions

The following conditions are reported errors:

- Any instruction that references a register number outside the range `r00 <= rNN <= r31`
- `LOAD` or `STORE` with `addr` greater than 8191
- `DIV` with `rN2` equal to 0
- `Jxx` instruction where `loc` is greater than 1023.
- Program executes for more than 64K instructions.

In addition, over- and under-flow of arithmatic operations is silently ignored.
