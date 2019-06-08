#include <iostream>
#include "TypeCast.hpp"

using namespace std;
using namespace spero;

class Top { };
class MiddleA : public Top { };
class MiddleB : public Top { };
class Bottom : public MiddleA, public MiddleB { };

void Function(const MiddleA& A)
{
    cout << "MiddleA Function" << endl;
}
void Function(const MiddleB& B)
{
    cout << "MiddleB Function" << endl;
}
void Function(const Bottom&)
{
    cout << "Bottom Function" << endl;
}


// Polymorphic Inheritance
class Parent
{
public:
    virtual ~Parent() { }
    virtual void print()
    {
        cout << "I am Parent" << endl;
    }
};

class Son : public Parent
{
public:
    virtual ~Son() { }
    virtual void print()
    {
        cout << "I am Son" << endl;
    }
};

void Function(const Parent&)
{
    cout << "Parent Function" << endl;
}

void Function(const Son&)
{
    cout << "Son Function" << endl;
}

int main()
{
    Bottom bottom;
    Top top;

 
    /*  following call of is ambiguous:
            Function(bottom);
        we could use static_cast:*/
    Function(static_cast<MiddleA&>(bottom));

  
    /*  but static_cast won't check down_cast,
        it can pass the compilation,
        but may crash when running if we do this:*/
    Function(static_cast<MiddleB&>(top));


    /*  instead, implicit_cast can check this typecast,
        it won't pass the compilation:*/
    // Function(implicit_cast<MiddleB>(top));


    /*  down_cast can replace dynamic_cast,
        but it will failed if we do this:*/
    // MiddleA* pMid = new Bottom;
    // Function(*down_cast<Bottom*>(pMid));

  
    /*  cause MiddleA is not polymorphic,
        it work when using class Parent and Son:*/
    Parent* pParent = new Son;
    Function(*pParent);
    Function(*down_cast<Son*>(pParent));


    /*  dynamic_cast will check in Debug mode,
        it will coredump if we do this:*/
    pParent = new Parent;
    Function(*down_cast<Son*>(pParent));


    return 0;
}