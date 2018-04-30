#include <utility>
#include <functional>
#include <iostream>
#include <string>
#include <cmath>
#include <tuple>


// forward declarations
template<typename T> class Complejo;
template<typename T> Complejo<T> operator+ ( Complejo<T>&, Complejo<T>& );
template<typename T> Complejo<T> operator+ ( Complejo<T>&, std::pair<T,T>& );
template<typename T> Complejo<T> operator+ ( std::pair<T,T>&, Complejo<T>& );
template<typename T> Complejo<T> operator+ ( Complejo<T>&, std::tuple<T,T>& );
template<typename T> Complejo<T> operator+ ( std::tuple<T,T>&, Complejo<T>& );


template<typename T> class Complejo {
public:
  // default ctor, move ctor from 2 parts and it's copying version
  Complejo() : re_( static_cast<T>(0) ), im_( static_cast<T>(0) ) { std::cout << "# default constructed!\n"; }
  Complejo( T&& re, T&& im ) : re_(std::move(re)), im_(std::move(im)) { std::cout << "# move parts-constructed!\n"; }
  Complejo( const T& re, const T& im ) : re_(re), im_(im) { std::cout << "# copy parts-constructed!\n"; }

  // copy ctor and it's copying version
  Complejo( Complejo&& src ) : re_( std::move(src.re_) ), im_( std::move(src.im_) ) { std::cout << "# move constructed!\n"; }
  Complejo( const Complejo& src ) : re_(src.re_), im_(src.im_) { std::cout << "# copy constructed!\n"; }

  // ctors from pair<T,T>, from tuple<T,T> and their copying versions
  Complejo<T>( std::pair<T,T>&& p ) : re_( std::move(p.first) ), im_( std::move(p.second) ) { std::cout << "# move pair-constructed!\n"; }
  Complejo<T>( const std::pair<T,T>& p ) : re_( p.first ), im_( p.second ) { std::cout << "# copy pair-constructed!\n"; }
  Complejo<T>( std::tuple<T,T>&& t ) : re_(std::move(std::get<0>(t))), im_(std::move(std::get<1>(t))) { std::cout << "# move tuple-constructed!\n"; }
  Complejo<T>( const std::tuple<T,T>& t ) : re_(std::get<0>(t)), im_(std::get<1>(t)) { std::cout << "# copy tuple-constructed!\n"; }

  // move-assignment op= and it's copying version
  Complejo& operator=( Complejo&& src ) {
    re_ = std::move(src.re_); im_ = std::move(src.im_); std::cout << "# move assigned!\n"; return *this; }
  Complejo& operator=( const Complejo& src ) {
    re_ = src.re_; im_ = src.im_; std::cout << "# copy assigned!\n"; return *this; }

  // explicit conversion to string
  // unfortunately, too much work is needed for formatted output of different types (e.g., setting double's precision)
  std::string to_string() const { return ( std::move( std::to_string(re_) + " + " + std::to_string(im_) + "i" ) ); }

  // friend operators
  friend Complejo<T> operator+<T> ( Complejo<T>&, Complejo<T>& );
  friend Complejo<T> operator+<T> ( Complejo<T>&, std::pair<T,T>& );
  friend Complejo<T> operator+<T> ( std::pair<T,T>&, Complejo<T>& );
  friend Complejo<T> operator+<T> ( Complejo<T>&, std::tuple<T,T>& );
  friend Complejo<T> operator+<T> ( std::tuple<T,T>&, Complejo<T>& );

  // tiny logic
  T mod() const {
    return std::move( std::hypot( re_, im_ ) );
  }
  T arg() const {
    return std::move( std::atan( im_ / re_ ) );
  }

private:
  T re_;
  T im_;
};


// helper functions
// impl in header to avoid linker problems
// another option was to enforce compilation of some cases, that seemed brute

// Complejo + Complejo
template<typename T> Complejo<T> operator+ (Complejo<T>& left, Complejo<T>& right) {
  return std::move( Complejo<T>( left.re_ + right.re_, left.im_ + right.im_ ) );
}

// Complejo + pair and vice versa
template<typename T> Complejo<T> operator+(Complejo<T>& left, std::pair<T,T>& right ) {
  return std::move( Complejo<T>( left.re_ + right.first, left.im_ + right.second ) );
}
template<typename T> Complejo<T> operator+(std::pair<T,T>& left, Complejo<T>& right ) {
  return std::move( Complejo<T>( left.first + right.re_, left.second + right.im_ ) );
}

// Complejo + tuple and vice versa
template<typename T> Complejo<T> operator+( Complejo<T>& left, std::tuple<T,T>& right ) {
  return std::move( Complejo<T>( left.re_ + std::get<0>(right), left.im_ + std::get<1>(right) ) );
}
template<typename T> Complejo<T> operator+( std::tuple<T,T>& left, Complejo<T>& right ){
  return std::move( Complejo<T>( std::get<0>(left) + right.re_, std::get<2>(left) + right.im_ ) );
}
