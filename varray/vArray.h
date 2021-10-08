// vArray.h: 
//	
//			class VArray <class T, class A = std::allocator<T>>   : a resemblance of STL vector<>
//				General Notes:
//					- container for elements of type T, ...
//					- 'size': # of active elements ::= [0, 'capacity') ; 'capacity' current storage size ::= [0, ... 
//					- all elements in [size, capacity) are initialized with T() (CURRENTLY FOR TESTING PURPOSES)
//					  NB: T() should, preferably, NOT need memory releases as it will not be taken care of
//					- ...             [0, size) are initialized after an operation with user defined value<T>
//                    POSSIBLE Optimization for SPEED: FORGET ABOUT T() initialization except explicitly requested
// 
//				Interface:
//					- default Constructor: create VArray of size num and initialize its elements with a value, use allocator
//					  might also reserve additional storage space
//    ver 0.2       - initializer_list Constructor
//					- COPY operations
//					- MOVE operations
//					- size(): returns the size within the current storage capacity::=capacity()
//					- T* begin/end() family to support RFL
//					- reserve(new_capacity): reserve new storage capacity 
//					- resize(new_size, T filler): change the size (reallocate new storage capacity if needed), 
//							'new' elements are to be initialized with 'filler', redundant ones destroyed to T()
//					- push_back(): <as in vector> add new element (at the end). Increase capacity, if necessary as (empty() ? 8 : capacity() * 2)
//					- pop_back(): differs from <vector>: erases the last one, BUT returns again true if on empty. If an exception occured
//                                because of ~T() or initialization of not used storage, etc BUT AGAIN will just re-throw and return false
//                              AGAIN: possible optimization after Testing - skip the initialization of the NOT-USED storage
//					- back(): differs from vector<>: doesnot throw; returns T() if empty(), the last element's (MOVABLE) copy otherwise
//    ver 0.2		- front(): ...                                                              1st ...
//					- shrink_to_fit(): release the access storage
//					- [const] T& operator [i] const : throws out_of_range if (i >= size())
//    ver 0.2		- member types ::iterator -> Random Access Iterators (class iterator_RA<T> defined in "class_RAI.h")
//                                 ::const_iterator -> ...                type const_iterator_RA<T> ...
// 
//			struct VA_base: private member of VArray: designed for use within VArray:: only
//				General Notes:
//					- handle for the memory slots (base layer for memory related operation)
//					- no <T> related operations aiming for exception safety
//					- Only MOVE Operations defined
//
// 

#ifndef OWN_ARRAY_OWN_ARRAY_OWN
#define OWN_ARRAY_OWN_ARRAY_OWN

#include <iostream>
#include <iomanip>
#include <utility>
#include <memory>

using std::allocator ;
using std::cout ;
using std::endl ;
using std::move ;
using std::swap ;
using std::cerr ;


// struct VA_base

template <class T, class A = allocator<T>>
struct VA_base    {				// Acts as a memory handle for VArray

		A	_all ;
		T*	_elems  ;
		T*	_lim  ;
		T*	_spa ;

		explicit VA_base(A a = A(), typename A::size_type size = 0) : _all{a}, _elems{a.allocate(size)}, 
											 _lim{_elems + size}, _spa{_elems + size} { cout << endl << "___ VA_base for " << size ; }
		virtual  ~VA_base() noexcept { 
					cout << endl << "___  ~ VA_base ..." ;
					_all.deallocate(_elems, _lim - _elems); _elems = _lim = _spa = nullptr ; 
				 }

		VA_base(const VA_base& vb) = delete ;
		VA_base& operator =(const VA_base& vb) = delete ;

		VA_base  (VA_base&& vb) noexcept ;				// Move C.
		VA_base& operator =(VA_base&& vb) noexcept ;	// Move A.
}; // struct VA_base

// ---------- class VA_base definitions:

template <class T, class A>
VA_base<T, A>::VA_base(VA_base&& vb) noexcept : _all { vb._all }, _elems { vb._elems }, _lim { vb._lim }, _spa { vb._spa }
{
	cout << endl << "___ VA_base && ..." ;
	vb._elems = vb._lim = vb._spa = nullptr ;
} // VA_base::VA_base(&&)

template <class T, class A>
VA_base<T, A> &
VA_base<T, A>::operator =(VA_base&& vb) noexcept
{
	cout << endl << "___ VA_base = && ..." ;
	if (this != &vb) {
		// -> vb aka *this is to be destroyed at the end of the scope
		swap(_all, vb._all), swap(_elems, vb._elems), swap(_lim, vb._lim), swap(_spa, vb._spa) ;
	}
	return(*this) ;
} // VA_base::operator=(&&) 

// eoclass VA_base

#include "class_RAI.h"			// Defines RandomAccessIterator class

// ------------ class VArray
template <class T, class A = allocator<T>>
class VArray {
	private:
		struct VA_base<T,A>		m_base ;
		void destroy_elements() ;

	public:
		using	iterator = typename iterator_RA<T> ;
		using	const_iterator = typename iterator_RA<const T> ;

	public:
		explicit VArray (size_t num = 0, const T& val = T(), size_t cap = 0, A a = A()) ;	// default C.
		explicit VArray (std::initializer_list<T>, A a = A()) ;								// initializer_list C.
		virtual ~VArray () ;

		VArray(const VArray& va) ;               // Copy C.
		VArray& operator = (const VArray& va) ;  // Copy A.

		VArray(VArray&& va) noexcept : m_base {move(va.m_base)} { cout << endl << "___ VArray && ..." ; }	// Move C.
		VArray& operator =(VArray&& va) noexcept ;															// Move A.

		size_t size() const { return(m_base._spa - m_base._elems) ; }
		size_t capacity() const { return(m_base._lim - m_base._elems) ; }
		bool   empty() const { return(size() == 0) ; }

		bool reserve(size_t cap) noexcept ;  					// increase the capacity
		bool resize(size_t size, const T& val = T()) noexcept ; // Change the size, initialize with val, if needed, upto _lim
		void clear() { resize(0) ; } 
		void shrink_to_fit() ;
		
		bool push_back(const T& val) ;
		bool push_back(T&& val) ;

		T	 back() const noexcept ;
		T	 front() const noexcept ;
		bool pop_back() ;

		T&			operator [] (const size_t ind) ;
		const T&	operator [] (const size_t ind) const ;

		iterator		begin() const { return(static_cast<iterator>(m_base._elems)) ; }
		iterator		end() const { return(static_cast<iterator>(m_base._spa)) ; }
		iterator		endlim() const { return(static_cast<iterator>(m_base._lim)) ; }

		template <class T, class A> friend std::ostream& operator << (std::ostream& os, const VArray& ar) ;
}; // class VArray

// ---------- class VArray

template <class T, class A> void
VArray<T, A>::destroy_elements()
{
	cout << "___ VArray : destroying elements" ;
	for (T* p = m_base._elems ; p != m_base._spa ; ++p)     p->~T() ;    // Destroy the elements
	m_base._spa = m_base._elems ;
} // VArray::destroy_elements() 

template <class T, class A>
VArray<T, A>::~VArray()
{
	cout << endl << "___  ~ VArray - " ;
	destroy_elements() ;
} // VArray:: ~

template <class T, class A>
VArray<T, A>::VArray(size_t num, const T& val, size_t cap, A a) : m_base(a, (cap <= num ? (cap = num + 8) : cap))
{
	// in : if (capacity <= num)        cap = num + 8 ; // Magic # 8: size of space to reserve since the beginning
	cout << endl << "___ VArray (" << num << ", " << val << ")/cap(): " << cap ;
	try {
		std::uninitialized_fill(m_base._elems, m_base._elems + num, val) ;    // See VS docs
		std::uninitialized_fill(m_base._spa, m_base._lim, T()) ;  // Initialize it with the <empty>: For testing mostly
	} catch (...) { cerr << endl << "___ VArray default: exception thrown. Cleaning done" ; throw ; }
	m_base._spa = m_base._elems + num ;									      // _lim is just where it has to be: next spot after the end
} // VArray:: ()

template <class T, class A>
VArray<T, A>::VArray(std::initializer_list<T> ilist, A a) : m_base(a, ilist.size())
{
	m_base._spa = m_base._elems ;
	for (const auto& i : ilist)		push_back(i) ;
} // VArray (initializer_list)


template <typename In, typename T, typename Out> void
uini_cpy(In beg, T* end, Out ooo)			// Expected to provide Strong guarantee; (&* is for the intended use of iterators); T()- request using Constructors
{
	cout << endl << "__ copying: " ;
	In p = beg ; Out q = ooo ;    // for ERROR handling and Testing: "cout << i << ':'" as well
	try {
		for (; beg != end ; ++beg, ++ooo) {
			// if (beg - p == 4)   throw (CM_error(string("__ testing CM_error"))) ;
			cout << (beg - p) << ':',
				new (static_cast<void *>(&*ooo)) T ( *beg ) ;
		}
	} catch (...) {
		cerr << endl << "__ exception thrown -> cleaning: " ;
		for (size_t i = 0 ; q != ooo ; ++q, i++)	(&(*q))->~T(), cout << i << ':' ;
		throw ;
	}
} // uini_cpy()

template <typename In, typename T, typename Out> void
uini_move(In beg, T* end, Out ooo)			
{
	cout << endl << "__ moving: " ;
	In p = beg ; Out q = ooo ;    // for ERROR handling and Testing: "cout << i << ':'" as well
	try {
		for (; beg != end ; ++beg, ++ooo) {
			// if (beg - p == 4)   throw (CM_error(string("__ testing CM_error"))) ;
			cout << (beg - p) << ':',
				new (static_cast<void *>(&*ooo)) T ( move(*beg) ) ;
			beg->~T() ;
		}
	} catch (...) {
		cerr << endl << "__ exception thrown during moving: can not clean" ;
		throw ;
	}
} // uini_move()

template <typename Out, typename T> void
uini_chng(Out beg, Out end, const T& val)
{
	// cout << endl << "__ changing: " ;
	Out p = beg ;    // for ERROR handling and Testing: "cout << i << ':'" as well
	try {
		for (; beg != end ; ++beg) {
			// if (beg - p == 4)   throw (CM_error(string("__ testing CM_error"))) ;
			beg->~T(),
				// cout << (beg - p) << ':',
				new (static_cast<void *>(&*beg)) T ( val ) ;
		}
	} catch (...) {
		cerr << endl << "__ exception thrown during changing: can not clean" ;
		throw ;
	}
} // uini_chng()

template <typename Out, typename T> void
uini_mval(Out beg, T&& val)
{
	cout << endl << "__ changing with &&: " ;
	try {
		beg->~T() ;
		new (static_cast<void *>(&*beg)) T (move(val)) ;
	} catch (...) {
		cerr << endl << "__ exception thrown during moving value: can not clean" ;
		throw ;
	}
} // uini_mval()

template <class T, class A>
VArray<T, A>::VArray(const VArray& var) : m_base(var.m_base._all, var.capacity())
{
	cout << endl << "___ VArray & ..." ;
	m_base._spa = m_base._elems + var.size() ;
	uini_cpy(var.m_base._elems, var.m_base._lim, m_base._elems) ; // Copy all of them - up to the limit: fully identical
} // VArray (&)


template <class T, class A> VArray<T, A>&
VArray<T, A>:: operator =(const VArray& var)
{
	cout << endl << "___ VArray = & ..." ;
	if (this != &var) {
		VArray<T, A>  temp(var) ;            // 'var' should provide destruction of the old *this      
		m_base = move(temp.m_base) ;   // As per operator =() for m_base it is a swap, actually
	}
	return(*this) ;
} // VArray operator =(&)

template <class T, class A> VArray<T, A> &
VArray<T, A>::operator =(VArray&& var) noexcept
{
	cout << endl << "___ VArray = && ..." ;
	if (this != &var) {
		clear() ;					       // Caring about unproper use of std::move() somewhere
		m_base = move(var.m_base) ;   // As per operator =() for m_base it is a swap, actually
	}
	return(*this) ;
} // VArray operator =(&&)



template <class T, class A> bool
VArray<T, A>::reserve(size_t new_cap) noexcept
{
	if (new_cap <= capacity())               return(true) ; // BjarneS: do not decrease allocated space

	// PLAN A (very exception-safe):
	//  - create a new base and COPY elements from the old
	//  - if no exception thrown - destroy the old one
	// PLAN B (less exception safe): BjarneS: MOVE Operations shoud NOT throw
	//  - create a new base and if OK MOVE elements from the old
	//  - if no exception thrown - destroy the old one. Otherwise keep what's left
	// -> GO for PLAN B

	try {
		VA_base<T, A>		tb(m_base._all, new_cap) ;
		tb._spa = tb._elems + size() ;       // Adjust to match this->size(); Then initialize to _lim
		std::uninitialized_fill(tb._spa, tb._lim, T()) ; // In case of an eception, Nothing has been damaged so far
		uini_move(m_base._elems, m_base._spa, tb._elems) ; // Now, it is what it is
		// Only need to swap (The allocator has to be the same); 'tb' must be destroyed at EOS
		swap(m_base._elems, tb._elems) , swap(m_base._spa, tb._spa) , swap(m_base._lim, tb._lim) ;
	} catch (...) {
		cerr << endl << "___ (VArray): reserve() -> exception -> ..." ;
		return(false) ;
	}
	return(true) ;
} // VArray reserve()


template <class T, class A> bool
VArray<T, A>::resize (size_t ns, const T& val) noexcept
{
					size_t	arr_size = size() ;

	if (ns == arr_size)				return(true) ;  // Nothing to do
	if (!reserve(ns))				return(false) ; // Will attempt smthg only if (ns > capacity)

	// Now _lim and _elems are, hopefully, where they should be
	try {
		if (ns < arr_size) {   // need to destroy some and set them to T() - the rest are expected to be T()
			uini_chng(m_base._elems + ns, m_base._spa, T{}) ; 
			m_base._spa = m_base._elems + ns ;                
		} else if (ns > size()) {
			std::uninitialized_fill(m_base._spa, m_base._elems + ns, val) , m_base._spa = m_base._elems + ns ;
		}
	} catch (...) {
		cerr << endl << "___ (VArray): resize() -> exception -> ..." ;
		return(false) ;
	}

	return(true) ;
} // VArray resize()

template <class T, class A> void
VArray<T, A>::shrink_to_fit()
{
	VA_base<T,A>	tb(m_base._all, size()) ;
	try {
		uini_move(m_base._elems, m_base._spa, tb._elems) ;  // should take care of destruction of source elements
		// -> tb aka *this is to be destroyed at the end of the scope
		swap(m_base._elems, tb._elems), swap(m_base._lim, tb._lim), swap(m_base._spa, tb._spa) ;
	} catch (...) {
		cerr << endl << "___ (VArray): shrink_to_fit -> exception -> ..." ;
	}
} // VArray shrink_to_fit()


template <class T, class A> bool
VArray<T,A>::push_back(const T& val)
{
					size_t			arr_size = size() ;

	if (size() == capacity())		{ if (!reserve(arr_size ? arr_size * 2 : 8))    return(false) ; }

	try {
	uini_chng(m_base._spa, m_base._spa + 1, val) ;
	} catch (...)   {  
		cerr << endl << "___ (VArray): push_back(&) -> exception -> ..." ;
		// The only discrepancy is that *_spa might not be {}
		return(false) ;
	}  
	(m_base._spa) ++ ;
	return(true) ;
} // VArray push_back(&)

template <class T, class A> bool
VArray<T, A>::push_back(T&& val)
{
	size_t			arr_size = size() ;

	if (arr_size == capacity()) { if (!reserve(arr_size ? arr_size * 2 : 8))    return(false) ; }

	try {
		uini_mval(m_base._spa, move(val)) ;
	} catch (...) {
		cerr << endl << "___ (VArray): push_back(&&) -> exception -> ..." ;
		// The only discrepancy is that *_spa might not be {}
		return(false) ;
	}
	(m_base._spa)++ ;
	return(true) ;
} // VArray push_back(&&)

template <class T, class A> bool
VArray<T, A>::pop_back()
{
	if (empty())    return(true) ;    // Logic is: did nothing but there is no element anyway
	try {
		uini_mval(m_base._spa - 1, move(T {})) ;
	} catch (...) {
		cerr << endl << "__ (VArray): pop_back() -> exception -> ..." ;
		throw ;
	}
	(m_base._spa)-- ;
	return(true) ;
} // VArray pop_back()

template <class T, class A> T
VArray<T, A>::back() const noexcept
{
	return(empty() ? T{} : *(m_base._spa - 1)) ;	
} // Varray back()

template <class T, class A> T
VArray<T, A>::front() const noexcept
{
	return(empty() ? T {} : *(m_base._elems)) ;
} // Varray front()


template <class T, class A> inline T&
VArray<T,A>::operator [](const size_t i)
{
	if (i < size())      return(*(m_base._elems + i)) ;
	throw			std::out_of_range("___ (VArray): subscript out of range") ;
} // Varray operator[]()

template <class T, class A> inline const T&
VArray<T, A>::operator [](const size_t i) const
{
	if (i < size())      return(*(m_base._elems + i)) ;
	throw			std::out_of_range("___ (VArray): subscript out of range") ;
} // Varray operator[]()


// FRIENDS follow
constexpr int ElementsPerRow { 10 } ;     // For the output: ostream:
constexpr int SpacePerElement { 10 } ;    // ... # of ' '

template <class T, class A> std::ostream&
operator << (std::ostream& os, const VArray<T, A> & ar)
{
	os << "  {used " << ar.size() << " of " << ar.capacity() << '}' << '\n' ;
	if (ar.size() == 0) { os << "<EMPTY>\n" ; /* return(os) ; */ }

	size_t i = 0 ; // std::string str{} ;
	for (auto p = ar.begin() ; p != ar.endlim() ; ++p) {
		// str = typeid(p).name() ;
		if (i++ % ElementsPerRow == 0)    os << '\n' ;
		os << std::setw(SpacePerElement) << (*p) ; // = -1 ); // << (ar.m_base._elems)[i] ; // << '\t' ;
	}
	// cout << endl << "-- typeid iterator {" << str << "}" ;
	return(os) ;
} // friend VArray<T,A>::operator << ()

// eoclass VArray

#endif // OWN_ARRAY_OWN_ARRAY_OWN

// eof vArray.h