// class_Iter.h: general conscepts, definitions and templates for iterators
//

#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

#ifndef CLASS_VITER_CLASS_VITER_CLASS
#define CLASS_VITER_CLASS_VITER_CLASS

template <typename Iter>
using Data_type = typename std::iterator_traits<Iter>::value_type ;

template <typename Iter> inline bool
isVT_pointer(Iter it)
{
	return(std::is_pointer<Data_type<Iter>>::value) ;
}

#include <iostream>
using std::cout ;
using std::endl ;

template <typename Iter> void
show_it_traits(Iter it)
{
	cout << endl << endl << "-- iterator_traits:" << endl
		<< " tag: " << typeid(std::iterator_traits<decltype(it)>::iterator_category()).name() << endl
		<< " DTy: " << typeid(std::iterator_traits<decltype(it)>::value_type()).name() << endl
		<< " it : " << typeid(decltype(it)).name() << endl
		<< " is pointer: " << std::boolalpha << (isVT_pointer(it)) ;
}


#endif

// eof class_Iter

