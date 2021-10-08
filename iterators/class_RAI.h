// class_RAI.h: an approach to RandomAccessIterators via class template <DTy> class iterator_RA
//			Support:
//				- Constructors: default (pointer to DTy), Copy
//				- Assignement:  Copy
//				- comparison:   ==, != , <=, >=, <, >. NB: Meaningful, if only 'assigned' to the same container
//				- increment/de: ++, --, +=, -=, -, +
//				- dereference: *()
// 
// ver 0.0:     - iterator_RA<> 
// ver 0.1:		- const_iterator_RA<>, also convertible from iterator_RA<>
//


#ifndef CLASS_RAI_CLASS_RAI_CLASS
#define CLASS_RAI_CLASS_RAI_CLASS

template <typename DTy> 
class iterator_RA {
	private:    // private:
		DTy * ptr {nullptr} ;
		using const_iterator_RA = typename iterator_RA<const DTy> ;
		
	public:
		iterator_RA(DTy * ip = nullptr) : ptr{ip} {}
		iterator_RA(const iterator_RA& it) : ptr{it.ptr} {}

		// iterator_RA(iterator_RA&&) = delete ;
		// iterator_RA& operator =(iterator_RA&&) = delete ;

		operator const_iterator_RA () { return(ptr) ; }

		virtual iterator_RA& operator =(const iterator_RA& it) { ptr = it.ptr ; return(*this) ; }

		bool operator ==(const iterator_RA& rho) const { return(ptr == rho.ptr) ; }
		bool operator !=(const iterator_RA& rho) const { return(ptr != rho.ptr) ; }
		bool operator <=(const iterator_RA& rho) const { return(ptr <= rho.ptr) ; }
		bool operator >=(const iterator_RA& rho) const { return(ptr >= rho.ptr) ; }
		bool operator < (const iterator_RA& rho) const { return(ptr < rho.ptr) ; }
		bool operator > (const iterator_RA& rho) const { return(ptr > rho.ptr) ; }

		iterator_RA&		operator ++()	 { ptr++ ; return(*this) ; }
		const iterator_RA	operator ++(int) { iterator_RA tmp(*this) ; ptr++ ; return(tmp) ; }
		iterator_RA&		operator --()    { ptr-- ; return(*this) ; }
		const iterator_RA	operator --(int) { iterator_RA tmp(*this) ; ptr-- ; return(tmp) ; }
		iterator_RA&        operator +=(const size_t incr) { ptr += incr ; return(*this) ; }
		iterator_RA&        operator -=(const size_t decr) { ptr -= decr ; return(*this) ; }
		iterator_RA         operator + (const size_t incr) const { iterator_RA tmp(*this) ; tmp.ptr += incr ; return(tmp) ; }
		iterator_RA         operator - (const size_t decr) const { iterator_RA tmp(*this) ; tmp.ptr -= decr ; return(tmp) ; }

		DTy&	operator *() const { return(static_cast<DTy&>(*ptr)) ; }

} ; // class iterator_RA


#endif

// eof class_RAI.h
