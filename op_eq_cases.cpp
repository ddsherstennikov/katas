// op_eq_cases.cpp
// different cases of operator= definition
// (-> Meyers "Effective C++", Item 14)
//========================================

#include <algorithm>

// auxiliary class
class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}

private:
	Uncopyable(const Uncopyable& rhs);
	Uncopyable& operator=(const Uncopyable& rhs);
};

// monomorphic class
class Foo
{
	// owns content with unknown semantics
	char* content;
};

class SuperBoo
{
public:
	SuperBoo() 
	{  
		// ctor logic here...
	}
	virtual ~SuperBoo()
	{
		// dtor logic here...
	}
};

// CASE 0: no op=
/*
	compiler creates op= & copy ctor with	memberwise bitwise copy
	ok if Foo is 
		(1) globally defined (pointer is valid throughout whole program execution)
		(2) can be shared (one Foo object can be accessed from several Boo objects, e.g. - readonly usage)
*/
class Boo0 : public SuperBoo 
{
	Foo* fFoo1;
	Foo* fFoo2;
};

// CASE 1: prohibit op=
/*
	Foo object implements a resource that is not supposed to be copied
	e.g., Mutex
*/
class Boo1 : public SuperBoo, private Uncopyable
{
	Foo* fFoo1;
	Foo* fFoo2;
};

// CASE 2: count references
/*
	is not applicable for fFoo1 & fFoo2 are not shared_ptr-s
	would suit situation with shareable resource
*/

// CASE 3: deep copy
/*
	when we copy ptr, we copy pointee
	incomplete info^ who is responsible for Foo objects creation/deletion
*/
class Boo3 : public SuperBoo
{
	Foo* fFoo1;
	Foo* fFoo2;

public:

	Boo3()
	{
		// ctor logic
	}

	Boo3(const Boo3& rhs)
	{
		// copy ctor logic
	}

	~Boo3()
	{
		// dtor logic
	}

	Boo3& operator=(const Boo3& rhs)
	{
		if (this == &rhs)
			return *this;

		Foo* new_fFoo1;
		Foo* new_fFoo2;
		try
		{
			new_fFoo1 = new Foo(*rhs.fFoo1);
			new_fFoo2 = new Foo(*rhs.fFoo2);
		}
		catch (...)
		{
			// process & rethrow
			// ..
			throw;
		}

		// copy-initializtion case taken into account
		if ( fFoo1 != nullptr )
			delete fFoo1;
		if (fFoo2 != nullptr)
			delete fFoo2;

		// consistency guarantee
		std::swap(fFoo1, new_fFoo1);
		std::swap(fFoo2, new_fFoo2);
		
		// copy other fields
		// ...		

		return *this;
	}
};

// CASE 4: transfer ownership
/*
	is not applicable for fFoo1 & fFoo2 are not auto_ptr-s/unique_ptr-s
	would suit situation with transferrably owned resource
*/


// 1) Mock objects for all classes
// 2) test-code for default impl of op=
// 3) impl all possible op= semantics cases

int main(int argc, char** argv) 
{
  return 0;
}

