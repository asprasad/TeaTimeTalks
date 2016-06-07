// VirtualFunctions.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

class Base
{
	int i;
public:
	Base(int iVal)
		:i(iVal)
	{
		this->foo();
	}
	virtual void foo()
	{
		std::cout << "Base::foo()\n";
	}
	virtual void bar()
	{
		std::cout << "Base::bar()\n";
	}
};

class Derived : public Base
{
	int j;
public:
	Derived(int iVal, int jVal)
		:Base(iVal), j(jVal)
	{ }

	virtual void foo()
	{
		std::cout << "Derived::foo()\n";
	}
	virtual void bar()
	{
		std::cout << "Derived::bar()\n";
	}
};

typedef void (*VirtualFunc_t)(void*);

struct MyClassVTable
{
	VirtualFunc_t fooPtr;
	VirtualFunc_t barPtr; 
};

struct Base_C
{
	MyClassVTable *_vtbl;
	int i;
};

void foo_Base(void *myThis)
{
	std::cout << "Base::foo()\n";
}

void bar_Base(void *myThis)
{
	std::cout << "Base::bar()\n";
}

struct Derived_C
{
	// Base_C _base;
	MyClassVTable *_vtbl;
	int i;
	int j;
};

void foo_Derived(void *myThis)
{
	std::cout << "Derived::foo()\n";
}

void bar_Derived(void *myThis)
{
	std::cout << "Derived::bar()\n";
}

MyClassVTable baseVirtualTable = {foo_Base, bar_Base};
MyClassVTable derivedVirtualTable = {foo_Derived, bar_Derived};

void Base_Constructor(Base_C *myThis, int iVal)
{
	myThis->_vtbl = &baseVirtualTable;
	myThis->i = iVal;
}

void Derived_Constructor(Derived_C *myThis, int iVal, int jVal)
{
	Base_Constructor((Base_C*)myThis, iVal);
	myThis->_vtbl = &derivedVirtualTable;
	myThis->j = jVal;
}

void CallFoo(void *ptr)
{
	((Base_C*)ptr)->_vtbl->fooPtr((void*)ptr);
}

void CallBar(void *ptr)
{
	((Base_C*)ptr)->_vtbl->barPtr((void*)ptr);
}

void MyFunctionThatCallsFoo(void *obj)
{
	CallFoo(obj);
}

//class Base1
//{
//protected:
//	char b1;
//	char c2;
//public:
//	Base1() :b1(1), c2(42) { }
//	virtual void foo() { }
//};
//
//class Base2
//{
//public:
//	int b2;
//	Base2() :b2(2) { }
//	virtual void bar() { }
//};
//
//class Derived : public Base1, public Base2
//{
//public:
//	int b3;
//	Derived() :b3(3) { }
//	virtual void foo() { }
//	virtual void bar() { std::cout << this->b3 << std::endl; }
//	virtual void foobar() { }
//};
//
//void barCaller(Base2 *ptr)
//{
//	ptr->bar();
//}

//class Base
//{
//public:
//	Base() : c('A') {}
//private:
//	char c;
//};
//
//class Derived2 : public Base
//{
//public:
//	Derived2() : x('B'),z(9) {}
//private:
//	char x;
//	int z;
//};

int _tmain(int argc, _TCHAR* argv[])
{
	//Derived2 d2;
	//int x = 5;
	//std::cout << &d2 << x << std::endl;
	//std::cout << "Size of Base" << sizeof(Base) << std::endl;
	//std::cout << "Size of Derived" << sizeof(Derived2) << std::endl;
	//Derived d;
	//std::cout << &d << std::endl;
	
	//Base1 *b1Ptr = &d;
	//std::cout << "base 1 ptr : " << b1Ptr << std::endl;

	//Base2 *b2Ptr = &d;
	//std::cout << b2Ptr << std::endl;
	//int y = b2Ptr->b2;
	//b2Ptr->bar();
	//barCaller(b2Ptr);

	//Derived d(42, 42);
	//d.foo();

	//Base *ptr = &d;
	//ptr->foo();
	//ptr->bar();
	// (*ptr).foo();
	
	Base_C b;
	Base_Constructor(&b, 42);
	MyFunctionThatCallsFoo((void*)&b);

	Derived_C d;
	Derived_Constructor(&d, 42, 42);
	MyFunctionThatCallsFoo((void*)&d);

	return 0;
}

