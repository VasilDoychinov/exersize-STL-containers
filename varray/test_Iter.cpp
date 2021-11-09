// test_Iter.cpp: a sample of test for Iterators and varray<T,A> and some ...

#include <iostream>
#include <string>

#include <iostream>
#include <iomanip>
#include <time.h>
#include <vector>
#include <memory>
#include <sstream>
#include <stdexcept>

using std::vector ;
using std::cout ;
using std::endl ;
using std::exception ;
using std::cerr ;
using std::allocator ;
using std::string ;
using std::setw ;


string
itostr(int i)
{
	std::stringstream	wwss ;
	wwss << i ;
	string	str ;
	wwss >> str ;
	return(str) ;
} // itostr()

using std::ostream ;

template <typename Iter>
ostream& operator <<(ostream& os, string * ptr) { os << *ptr ; return(os) ; }

constexpr size_t NUM_PER_ROW = 10 ;

template <typename DTy>
ostream& operator <<(ostream& os, vector<DTy>& v)
{
	os << "{used " << v.size() << " of " << v.capacity() << '}' ;
	for (size_t i = 0 ; i < v.size() ; i++) {
		if (i % NUM_PER_ROW == 0)   cout << endl ;
		cout << ' ' << std::setw(10) << *(v[i]) ;
	}
	return(os) ;
}

#include <iostream>
#include <vector>
#include <list>
#include <iterator>

#include "..\iterators\class_RAI.h"
#include "vArray.h"

using String = string * ;

template <typename Iter>
using Diff_type = typename std::iterator_traits<Iter>::difference_type ;

template <typename Iter>
using Data_type = typename std::iterator_traits<Iter>::value_type ;

namespace test_implementation
{
	template <class InpIt, class OutIt> void
	unini_cpy(InpIt beg, InpIt end, OutIt ooo)	
	{				             					
		auto q = ooo ;	
		try {
			for (; beg != end ; ++beg, ++ooo) {
				new (static_cast<void *>(&*ooo)) (Data_type<OutIt>) (*beg) ;
			}
		} catch (...) {
			cerr << endl << "__ exception thrown -> cleaning: " ;
			// for (size_t i = 0 ; q != ooo ; ++q, i++)	(&(*q))->~T(), cout << i << ':' ;
			throw ;
		}
	} // unini_cpy()
	
	template <typename Iter> bool
	show_is_pointer(Iter it)
	{
		bool   fl = false ;
		cout << endl << "--- is <" << typeid(Data_type<Iter>).name()
			<< "> a pointer: " << std::boolalpha << (fl = isVT_pointer(it)) ;
		return(fl) ;
	}
} // namespace test_implementation


template <typename DTy> inline bool
is_valVoid(DTy val)
{
	return(val == DTy()) ;
}


int main()
{
	cout << endl ;

	String				aType[40] ;
	String				wPtr ;
	auto					arr_size = (sizeof(aType) / sizeof(aType[0])) ;
	decltype(arr_size)		count = 0 ;
	auto    b_it = begin(aType) ; auto e_it = b_it + static_cast<size_t>(arr_size / 2) ;
	Diff_type<decltype(b_it)>    steps = (e_it - b_it) ;
	cout << endl << endl << ">>>> " << typeid(steps).name() << "   steps(p2 - p1) is: " << steps ;
	test_implementation::show_is_pointer(b_it) ;

	for (auto p = b_it ; p != e_it ; ++p, count++) {
		wPtr = new string('{' + itostr(count) + '}') , *p = wPtr ;
	}
	decltype(arr_size)		q_size = arr_size / 4 ;
	for (auto p = e_it ; p != (e_it + q_size) ; ++p) {    // set to DTy()
		wPtr = new string(), *p = wPtr ;
	}
	test_implementation::unini_cpy(e_it, e_it + q_size, e_it + q_size) ;   // initialize to the end with T()
	{
		varray<string *>		vs ;
		test_implementation::show_is_pointer(begin(vs)) ;
		cout << endl << endl << "--- to start with vs<string *> is:" << endl << vs ;
		vs.insert_at(0, b_it, b_it + arr_size) ;   // Or, the one below
		// for (auto ptr = b_it ; ptr != b_it + arr_size ; ++ptr) vs.insert(begin(vs), (*ptr)) ;   // with vs<string> -> *(*ptr)
		cout << endl << endl << "--- after insert(s):" << endl << vs ;

		vs.erase(begin(vs), begin(vs) + 20), vs.push_back(new string("GEPPI!")) ;				// with vs<string> -> remove 'new'
		cout << endl << endl << "--- after erase[0:20) and push_back(&& GEPPI!):" << endl << vs ;

		vs.shrink_to_fit() ;
		cout << endl << endl << "--- after shrink_to_fit():" << endl << vs ;

		vs.insert(begin(vs), b_it, e_it) ;
		cout << endl << endl << "--- after insert():" << endl << vs ;
	}

	cout << endl << endl << "--- show aType[" << arr_size << "]: " << endl ;
	for (count = 0 ; count < arr_size ; count++ ) {
		if (count % NUM_PER_ROW == 0)   cout << endl ;
		if (is_valVoid(*(aType[count])))     cout << std::setw(5) << "{-}" ;
		else                                 cout << std::setw(5) << *(aType[count]) ;
		cout << ':' << std::setw(2) << count ;
	}

	cout << endl << endl << "--- That's it ..." << endl << endl ;
	return(0) ;
}

/*  varray<string *, A>
	varray<string  *>         v ;

	string *				ptr = nullptr ;
	size_t					num_elements = 20 ;
	for (size_t i = 0 ; i < num_elements ; i++) {
		ptr = new string ('{' + itostr(i) + '}') ;
		v.push_back(std::move(ptr)) ;
	}
	cout << endl << "--- after pushing v is: " << v ;

	varray<string>		vStr { "-1-", "-2-", "-3-", "-4-", "-5-" } ;

	auto out = begin(v) + 5 ;
	for (auto it = begin(vStr) ; it != end(vStr) ; ++it) {
		out = v.insert(out, &(*it)) ;
	}

	string				str("{-}") ;
	varray<string *>	vPtrStr (5, &str) ;
	v.insert(begin(v) + 15, begin(vPtrStr), end(vPtrStr)) ;
	cout << endl << "--- after inserting v is: " << v ;

	cout << endl << endl << "--- clearing v ..." ;
	v.clear() ;
	cout << endl << endl << "--- result in v: " << v ;
*/