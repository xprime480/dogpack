
#include <functional>
#include <iostream>

#include "../include/vm.hpp"
#include "Runner.hpp"

using namespace std;

bool one_arg_wants_two(void (VM::*op)(), const char *name)
{
    VM vm;
    vm.push(1);
    (vm.*op)();
    return EXPECT_ERROR(vm, name);
}

bool empty_program()
{
    VM vm;
    return EXPECT_ERROR(vm, "Empty program");
}

bool push()
{
    VM vm;
    vm.push(1);
    return EXPECT_VALUE(vm, "Just Push", 1);
}

bool pop_to_empty()
{
    VM vm;
    vm.push(1);
    vm.pop();
    return EXPECT_ERROR(vm, "Pop to Empty");
}

bool pop_from_empty()
{
    VM vm;
    vm.pop();
    return EXPECT_ERROR(vm, "Pop from Empty");
}

bool pop()
{
    VM vm;
    vm.push(1);
    vm.push(2);
    vm.pop();
    return EXPECT_VALUE(vm, "Pop", 1);
}

bool dup_from_empty()
{
    VM vm;
    vm.dup();
    return EXPECT_ERROR(vm, "Dup from Empty");
}

bool dup()
{
    VM vm;
    vm.push(1);
    vm.dup();
    vm.add();
    return EXPECT_VALUE(vm, "Dup", 2);
}

bool dupn_from_empty()
{
    VM vm;
    vm.dupn(55);
    return EXPECT_ERROR(vm, "DupN from Empty");
}

bool dupn_negative()
{
    VM vm;
    vm.push(0);
    vm.dupn(-1);
    return EXPECT_ERROR(vm, "DupN Negative");
}

bool dupn_zero()
{
    VM vm;
    vm.push(0);
    vm.dupn(0);
    return EXPECT_ERROR(vm, "DupN Zero");
}

bool dupn_too_few()
{
    VM vm;
    vm.push(0);
    vm.dupn(5);
    return EXPECT_ERROR(vm, "DupN Too Few");
}

bool dupn_first()
{
    VM vm;
    vm.push(10);
    vm.push(20);
    vm.push(30);
    vm.dupn(1);
    return EXPECT_VALUE(vm, "DupN First", 30);
}

bool dupn_middle()
{
    VM vm;
    vm.push(10);
    vm.push(20);
    vm.push(30);
    vm.dupn(2);
    return EXPECT_VALUE(vm, "DupN Middle", 20);
}

bool dupn_last()
{
    VM vm;
    vm.push(10);
    vm.push(20);
    vm.push(30);
    vm.dupn(3);
    return EXPECT_VALUE(vm, "DupN Last", 10);
}

bool dropn_from_empty()
{
    VM vm;
    vm.dropn(55);
    return EXPECT_ERROR(vm, "DropN from Empty");
}

bool dropn_negative()
{
    VM vm;
    vm.push(0);
    vm.dropn(-1);
    return EXPECT_ERROR(vm, "DropN Negative");
}

bool dropn_zero()
{
    VM vm;
    vm.push(0);
    vm.dropn(0);
    return EXPECT_ERROR(vm, "DropN Zero");
}

bool dropn_too_few()
{
    VM vm;
    vm.push(0);
    vm.dropn(5);
    return EXPECT_ERROR(vm, "DropN Too Few");
}

bool dropn_first()
{
    VM vm;
    vm.push(10);
    vm.push(20);
    vm.push(30);
    vm.dropn(1);
    vm.add();
    return EXPECT_VALUE(vm, "DropN First", 30);
}

bool dropn_middle()
{
    VM vm;
    vm.push(10);
    vm.push(20);
    vm.push(30);
    vm.dropn(2);
    vm.add();
    return EXPECT_VALUE(vm, "DropN Middle", 40);
}

bool dropn_last()
{
    VM vm;
    vm.push(10);
    vm.push(20);
    vm.push(30);
    vm.dropn(3);
    vm.add();
    return EXPECT_VALUE(vm, "DropN Last", 50);
}

void stack_suite(Runner &runner)
{
    runner(push);
    runner(pop_to_empty);
    runner(pop_from_empty);
    runner(pop);
    runner(dup_from_empty);
    runner(dup);

    runner(dupn_from_empty);
    runner(dupn_negative);
    runner(dupn_zero);
    runner(dupn_too_few);
    runner(dupn_first);
    runner(dupn_middle);
    runner(dupn_last);

    runner(dropn_from_empty);
    runner(dropn_negative);
    runner(dropn_zero);
    runner(dropn_too_few);
    runner(dropn_first);
    runner(dropn_middle);
    runner(dropn_last);
}

bool add_too_few()
{
    return one_arg_wants_two(&VM::add, "Add Too Few");
}

bool add()
{
    VM vm;
    vm.push(1);
    vm.push(33);
    vm.add();
    return EXPECT_VALUE(vm, "Add", 34);
}

bool sub_too_few()
{
    return one_arg_wants_two(&VM::sub, "Sub Too Few");
}

bool sub()
{
    VM vm;
    vm.push(1);
    vm.push(33);
    vm.sub();
    return EXPECT_VALUE(vm, "Sub", -32);
}

bool mul_too_few()
{
    return one_arg_wants_two(&VM::mul, "Mul Too Few");
}

bool mul()
{
    VM vm;
    vm.push(2);
    vm.push(3);
    vm.mul();
    return EXPECT_VALUE(vm, "Mul", 6);
}

bool div_too_few()
{
    return one_arg_wants_two(&VM::div, "Div Too Few");
}

bool div_by_zero()
{
    VM vm;
    vm.push(1);
    vm.push(0);
    vm.div();
    return EXPECT_ERROR(vm, "Div by Zero");
}

bool idiv()
{
    VM vm;
    vm.push(12);
    vm.push(3);
    vm.div();
    return EXPECT_VALUE(vm, "Div", 4);
}

bool longer_prog()
{
    VM vm;
    vm.push(2);
    vm.dup();
    vm.mul();
    vm.push(5);
    vm.push(2);
    vm.add();
    vm.sub();

    return EXPECT_VALUE(vm, "Longer Prog", -3);
}

bool program_too_long()
{
    VM vm;
    for (int i = 0; i < 1009; ++i)
    {
        vm.push(i);
    }

    return EXPECT_ERROR(vm, "Program Too Long");
}

bool overflow_push()
{
    VM vm;
    vm.label("Start");
    vm.push(1);
    vm.jmp("Start");

    return EXPECT_ERROR(vm, "Overflow Push");
}

bool overflow_dup()
{
    VM vm;
    vm.push(1);
    vm.label("Start");
    vm.dup();
    vm.jmp("Start");

    return EXPECT_ERROR(vm, "Overflow Dup");
}

bool duplicate_label()
{
    VM vm;
    vm.push(1);
    vm.label("X");
    vm.push(2);
    vm.label("X");
    vm.push(2);

    return EXPECT_ERROR(vm, "Duplicate Label");
}

bool run_too_long()
{
    VM vm;
    vm.push(0);
    vm.label("X");
    vm.push(1);
    vm.add();
    vm.jmp("X");

    return EXPECT_ERROR(vm, "Run Too Long");
}

bool cmp_too_few()
{
    return one_arg_wants_two(&VM::cmp, "CMP Too Few");
}

bool cmp_lt()
{
    VM vm;
    vm.push(1);
    vm.push(2);
    vm.cmp();
    return EXPECT_VALUE(vm, "Cmp LT", -1);
}

bool cmp_eq()
{
    VM vm;
    vm.push(2);
    vm.push(2);
    vm.cmp();
    return EXPECT_VALUE(vm, "Cmp EQ", 0);
}

bool cmp_gt()
{
    VM vm;
    vm.push(2);
    vm.push(1);
    vm.cmp();
    return EXPECT_VALUE(vm, "Cmp GT", 1);
}

bool swap_too_few()
{
    return one_arg_wants_two(&VM::swap, "Swap Too Few");
}

bool xswap()
{
    VM vm;
    vm.push(1);
    vm.push(2);
    vm.swap();
    return EXPECT_VALUE(vm, "Swap", 1);
}

bool jxx_without_test(void (VM::*op)(const std::string &), string const &label)
{
    VM vm;
    (vm.*op)("L00");
    vm.label("L00");
    return EXPECT_ERROR(vm, label);
}

bool jxx_to_missing_label(void (VM::*op)(const std::string &), string const &name)
{
    VM vm;
    vm.push(0);
    (vm.*op)("L00");
    return EXPECT_ERROR(vm, name);
}

bool jxx_when_test(void (VM::*op)(const std::string &), string const &label, int exp, int test_value)
{
    VM vm;
    vm.push(test_value);
    (vm.*op)("L00");
    vm.push(0);
    vm.jmp("L02");
    vm.label("L00");
    vm.push(1);
    vm.label("L02");
    return EXPECT_VALUE(vm, label, exp);
}

bool jxx_when_lt(void (VM::*op)(const std::string &), string const &label, int exp)
{
    return jxx_when_test(op, label, exp, -1);
}

bool jxx_when_eq(void (VM::*op)(const std::string &), string const &label, int exp)
{
    return jxx_when_test(op, label, exp, 0);
}

bool jxx_when_gt(void (VM::*op)(const std::string &), string const &label, int exp)
{
    return jxx_when_test(op, label, exp, 1);
}

void jmp_suite(Runner &runner, void (VM::*op)(const std::string &), const char *name, bool lt, bool eq, bool gt, bool args)
{
    const string base(name);

    if (args)
    {
        runner([&]() -> bool {
            return jxx_without_test(op, base + " Too Few Args");
        });
    }
    runner([&]() -> bool {
        return jxx_to_missing_label(op, base + " to unknown label");
    });
    runner([&]() -> bool {
        return jxx_when_lt(op, base + " when LT", lt ? 1 : 0);
    });
    runner([&]() -> bool {
        return jxx_when_eq(op, base + " when EQ", eq ? 1 : 0);
    });
    runner([&]() -> bool {
        return jxx_when_gt(op, base + " when GT", gt ? 1 : 0);
    });
}

bool factorial_test(int arg, string const &label, int res)
{
    VM vm;
    vm.push(arg);
    vm.dup();
    vm.jlt("ERROR_CASE");

    vm.push(1); // init the accumulator

    vm.label("LOOP");
    vm.dupn(2);
    vm.jle("LOOP_EXIT");

    vm.dupn(2);
    vm.mul();
    vm.swap();
    vm.push(1);
    vm.sub();
    vm.swap();
    vm.jmp("LOOP");

    vm.label("LOOP_EXIT");
    vm.swap();
    vm.pop();
    vm.jmp("EXIT");

    vm.label("ERROR_CASE");
    vm.push(-1);
    vm.jmp("EXIT");

    vm.label("EXIT");

    return EXPECT_VALUE(vm, label, res);
}

void factorial_suite(Runner &runner)
{
    runner([&]() -> bool {
        return factorial_test(-1, "factorial -1", -1);
    });
    runner([&]() -> bool {
        return factorial_test(0, "factorial 0", 1);
    });
    runner([&]() -> bool {
        return factorial_test(1, "factorial 1", 1);
    });
    runner([&]() -> bool {
        return factorial_test(5, "factorial 5", 120);
    });
}

bool fibonacci_test(int arg, string const &label, int res)
{
    /* TODO:  This program does not leave the stack clean.  VM help is needed */
    
    VM vm;

    // init the stack
    vm.push(arg);

    // test for bad argument (<= 0)
    vm.dup();
    vm.push(0);
    vm.cmp();
    vm.jle("ERROR");

    // init local variables
    vm.push(1);  // b
    vm.push(1);  // a

    // go to loop test
    vm.jmp("LOOP_TEST");

    // start the loop
    vm.label("TOP_OF_LOOP");

    // update local variables and test

    vm.dupn(3);
    vm.dupn(3);
    vm.dupn(2);
    vm.add();
    vm.swap();
    vm.dropn(5);
    vm.dropn(4);

    // decrement arg and break when 0;
    vm.label("LOOP_TEST");
    vm.dupn(3);
    vm.push(1);
    vm.sub();
    vm.dup();
    vm.jeq("DONE");
    vm.dropn(4);
    vm.jmp("TOP_OF_LOOP");

    // handle the error case
    vm.label("ERROR");
    vm.push(-1);
    vm.jmp("EXIT");

    // clean up the stack for normal return
    vm.label("DONE");
    vm.pop();

    vm.label("EXIT");

    return EXPECT_VALUE(vm, label, res);
}

void fibonacci_suite(Runner &runner)
{
    runner([&]() -> bool {
        return fibonacci_test(0, "Fibonacci 0", -1);
    });
    runner([&]() -> bool {
        return fibonacci_test(1, "fibonacci 1", 1);
    });
    runner([&]() -> bool {
        return fibonacci_test(8, "fibonacci 8", 21);
    });
}

int main(void)
{
    Runner runner;

    runner(empty_program);

    stack_suite(runner);

    runner(add_too_few);
    runner(add);
    runner(sub_too_few);
    runner(sub);
    runner(mul_too_few);
    runner(mul);
    runner(div_too_few);
    runner(div_by_zero);
    runner(idiv);
    runner(longer_prog);
    runner(program_too_long);

    runner(overflow_push);
    runner(overflow_dup);

    runner(run_too_long);

    runner(cmp_too_few);
    runner(cmp_lt);
    runner(cmp_eq);
    runner(cmp_gt);

    runner(swap_too_few);
    runner(xswap);

    runner(duplicate_label);

    jmp_suite(runner, &VM::jmp, "JMP", true, true, true, false);
    jmp_suite(runner, &VM::jeq, "JEQ", false, true, false, true);
    jmp_suite(runner, &VM::jne, "JNE", true, false, true, true);
    jmp_suite(runner, &VM::jlt, "JLT", true, false, false, true);
    jmp_suite(runner, &VM::jle, "JLE", true, true, false, true);
    jmp_suite(runner, &VM::jgt, "JGT", false, false, true, true);
    jmp_suite(runner, &VM::jge, "JGE", false, true, true, true);

    factorial_suite(runner);
    fibonacci_suite(runner);

    return runner.report();
}
