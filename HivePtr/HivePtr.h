#include <iostream>
#include <typeinfo>

template<class T> class HivePtr;
template<class T> class Hive;

using std::cout;
using std::endl;

// canonically, "shared_ptr"
template<class T>
class HivePtr<T*>
{
public:
  // necessary for op=
  HivePtr() { cout << "# hiveptr @ " << raw_ << " empty ctor" << endl; }

  // upgrade: optionally can take deleter member func ptr
  HivePtr(T*& raw) : raw_(raw), hive_( new Hive<T*>(raw) ) { cout << "# hiveptr @ " << raw_ << " ctor" << endl; }
  ~HivePtr() {
    cout << "# hiveptr @ " << raw_ << " dtor" << endl;
    --(*hive_);
  }

  HivePtr( HivePtr& src ) : raw_( src.raw_ ), hive_( src.hive_ ) {
    cout << "# hiveptr @ " << raw_ << " cctor" << endl;
    ++(*hive_);
  }
  HivePtr& operator=( HivePtr& src ) {
    cout << "# hiveptr @ " << raw_ << " op=" << endl;
    raw_ = src.raw_;
    hive_ = src.hive_;
    ++(*hive_);
  }

  T* operator->() const { return raw_; }
  T& operator*() const { return *raw_; }

  T* get() const { return raw_; }

private:
  T* raw_;
  Hive<T*>* hive_;
};


// canonically, "control block"
template<class T>
class Hive<T*>
{
  Hive() = delete;
  Hive( Hive& ) = delete;
  Hive& operator=( Hive& ) = delete;

  Hive( T*& raw ) : raw_(raw), countr_(0) { cout << "# hive @ " << raw_ << " ctor" << endl; ++(*this); }
  ~Hive() { cout << "# hive @ " << raw_ << " dtor" << endl; }

  Hive& operator++() {
    ++countr_;
    cout << "# hive: counter = " << countr_ << endl;
  }
  Hive& operator--() {
    --countr_;
    cout << "# hive: counter = " << countr_ << endl;
    if ( countr_ <= 0 ) redeem();
  }

  void redeem() {
    cout << "# ptr @ " << raw_ << " redeem()" << endl;

    delete raw_;
    delete this;
  }

  int countr_;
  T* raw_;

  friend class HivePtr<T*>;
};