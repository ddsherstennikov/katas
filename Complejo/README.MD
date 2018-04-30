Goal:

Create a working full class for complex numbers (complejo = complex number in spanish, #4fun)

Refs:

(1) - Stroustrup 3E, 11.3+: details
(2) - cctor nuances (Stroustrup, ch. 14)
(3) - Stroustrup 4E, 18.3+
(4) - In-Depth features (Meyers, Sutter, Alexandrescu)

Plan:

1) take technical details from Stroustrup w/o implementation or interface
2) object scheme - pseudocode - C++ inteface - impl
3) implement: ctor, dtor, cctor, defctor, op=, helper func conversion ops, exc policy
4) test: main creates and plays with this Complex Class objects
5) use this class in later ex-s
6) conclude - commit

Constraint:

1) use C++11 move semantics as if Complejo, re_ & im_ were very big objects, too big to copy if can be avoided