#include <iostream>
#include "NonCopyable.hpp"

using namespace std;
using namespace spero;

class Foo : public NonCopyable
{
public:
    Foo(int a, int b) : a_(a), b_(b)
    {
        cout << "Foo: a[" << a_ << "], b[" << b_ << "]" << endl;
    }
private:
    int a_;
    int b_;
};

int main(int argc, char const *argv[])
{
    Foo f1(1, 2);
    Foo f2(3, 4);
    // Foo is non-copyable, can't do the following operation:
    // Foo f3(f1);
    // Foo f4(std::move(f1));
    // f1 = f2;
    return 0;
}