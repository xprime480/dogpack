#include <iostream>

#include "../include/vm.hpp"

using namespace std;

namespace
{
class Runner
{
    public:
    Runner()
    : count(0u), passed(0u)
    {
    }

    void operator()(bool (*fn)())
    {
        ++count;
        if ( fn() )
        {
            ++passed;
        }
    }

    int report() const
    {
        cout << "\n" << passed << " passed out of " << count << " tests\n";
        return passed == count ? 0 : 1;
    }

    private:
    size_t count;
    size_t passed;
};

bool EXPECT_ERROR(VM &vm, const char *msg, bool verbose=false)
{
    VM_exec_status status = vm.exec(verbose);
    if (status.is_status_ok())
    {
        cerr << "[FAIL] " << msg << ", expected Error, got " << status.get_program_value() << "\n";
        return false;
    }
    else
    {
        cerr << "[PASS] " << msg << "\n";
        if (verbose)
        {
            cerr << "\tactual error message: " << status.get_message() << "\n";
        }
        return true;
    }
}

bool EXPECT_VALUE(VM &vm, const char *msg, int value, bool verbose = false)
{
    VM_exec_status status = vm.exec(verbose);
    if (status.is_status_ok())
    {
        int program_value = status.get_program_value();
        if (value == program_value)
        {
            cerr << "[PASS] " << msg << "\n";
            return true;
        }
        else
        {
            cerr << "[FAIL] " << msg << ", expected " << value << ", got " << program_value << "\n";
            return false;
        }
    }
    else
    {
        cerr << "[FAIL] " << msg << ", expected value " << value << ", got error" << status.get_message() << "\n";
        return false;
    }
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

bool add_too_few()
{
    VM vm;
    vm.push(1);
    vm.add();
    return EXPECT_ERROR(vm, "Add Too Few");
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
    VM vm;
    vm.push(1);
    vm.sub();
    return EXPECT_ERROR(vm, "Sub Too Few");
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
    VM vm;
    vm.push(1);
    vm.mul();
    return EXPECT_ERROR(vm, "Mul Too Few");
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
    VM vm;
    vm.push(1);
    vm.div();
    return EXPECT_ERROR(vm, "Div Too Few");
}

bool div_by_zero()
{
    VM vm;
    vm.push(1);
    vm.push(0);
    vm.div();
    return EXPECT_ERROR(vm, "Div by Zero");
}

bool div()
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
    for ( int i = 0 ; i < 1009 ; ++i )
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

bool unknown_label()
{
    VM vm;

    vm.push(1);
    vm.jmp("X");

    return EXPECT_ERROR(vm, "Unknown Label");
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
    VM vm;
    vm.push(1);
    vm.cmp();
    return EXPECT_ERROR(vm, "Cmp Too Few");
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
    VM vm;
    vm.push(1);
    vm.swap();
    return EXPECT_ERROR(vm, "Swap Too Few");
}

bool swap()
{
    VM vm;
    vm.push(1);
    vm.push(2);
    vm.swap();
    return EXPECT_VALUE(vm, "Swap", 1);
}

bool jeq_too_few()
{
    VM vm;
    vm.jeq("L00");
    vm.label("L00");
    vm.push(0);
    return EXPECT_ERROR(vm, "JEQ Too Few");
}

bool jeq_unknown_label()
{
    VM vm;
    vm.push(0);
    vm.jeq("L00");
    return EXPECT_ERROR(vm, "JEQ Too Few");
}

bool jeq_eq()
{
    VM vm;
    vm.push(0);
    vm.jeq("L00");
    vm.push(1);
    vm.jmp("L02");
    vm.label("L00");
    vm.push(2);
    vm.label("L02");
    return EXPECT_VALUE(vm, "JEQ EQ", 2);
}

bool jeq_ne()
{
    VM vm;
    vm.push(1);
    vm.jeq("L00");
    vm.push(1);
    vm.jmp("L02");
    vm.label("L00");
    vm.push(2);
    vm.label("L02");
   return EXPECT_VALUE(vm, "JEQ NE", 1);
}

} // namespace

int main(void)
{
    Runner runner;

    runner(empty_program);
    runner(push);
    runner(pop_to_empty);
    runner(pop_from_empty);
    runner(pop);
    runner(dup_from_empty);
    runner(dup);
    runner(add_too_few);
    runner(add);
    runner(sub_too_few);
    runner(sub);
    runner(mul_too_few);
    runner(mul);
    runner(div_too_few);
    runner(div_by_zero);
    runner(div);
    runner(longer_prog);
    runner(program_too_long);

    runner(overflow_push);
    runner(overflow_dup);

    runner(unknown_label);
    runner(duplicate_label);
    runner(run_too_long);

    runner(cmp_too_few);
    runner(cmp_lt);
    runner(cmp_eq);
    runner(cmp_gt);

    runner(swap_too_few);
    runner(swap);

    runner(jeq_too_few);
    runner(jeq_unknown_label);
    runner(jeq_eq);
    runner(jeq_ne);

    return runner.report();
}
