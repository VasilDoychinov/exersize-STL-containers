// class_VLI.h:  definitions of a node and of iterators for vlist<>
//


#ifndef CLASS_NODE_VLI_CLASS_NODE_VLI_CLASS
#define CLASS_NODE_VLI_CLASS_NODE_VLI_CLASS

#include "..\iterators\class_Iter.h"
#include "..\nodes\nodes.h"


template <typename T, typename A> class vlist ;

template <typename DTy>
class _vl_iter {
	public:
		using iterator_category = std::bidirectional_iterator_tag ;
		using difference_type = std::ptrdiff_t ;
		using value_type = typename node_VAL<DTy>::value_type ;
		using size_type = typename size_t ;

		using pointer = typename CB_nodePN* ;
		using reference = typename node_VAL<DTy>::reference ;

		using _vl_const_iter = typename _vl_iter<const DTy> ;

	private:
		CB_nodePN* p_node ;
		template <typename DTy, typename A> friend class vlist  ;

	public:
		explicit _vl_iter(CB_nodePN* pn = nullptr) : p_node{ pn } { }						// default C.
		_vl_iter(const _vl_iter& it) : p_node{it.p_node} { /*cout << endl << "_ _vl_iter: Copy ..." ;*/ }	// Copy C.
		_vl_iter& operator =(const _vl_iter& it) ;															// Copy A.

		operator _vl_iter<const DTy>() { _vl_iter<const DTy>  tmp(p_node) ; return(tmp) ; }
		operator size_t () { return(size_t(p_node)) ; }

		_vl_iter& operator ++() ;
		_vl_iter& operator --() ;
		const _vl_iter operator ++(int) { _vl_iter<DTy> tmp(*this) ; ++(*this) ; return(tmp) ; }
		const _vl_iter operator --(int) { _vl_iter<DTy> tmp(*this) ; --(*this) ; return(tmp) ; }

		
		reference	operator *() ;

		// pointer		operator ->() { return(pointer(p_node)) ; } // &((static_cast<node_VAL<DTy>*>(p_node))->m_val)) ; }

		bool operator !=(_vl_iter& ri) { return(p_node != ri.p_node) ; }
		bool operator ==(_vl_iter& ri) { return(p_node == ri.p_node) ; }

		// Friends ...

}; // class _vl_iter

template <typename DTy> inline typename _vl_iter<DTy>::reference
_vl_iter<DTy>::operator *()
{
	// node_VAL<DTy>* pVal ;
	// if (p_node == &l_tail) throw std::out_of_range("vlist: cannot dereference end iterator") ;
	// the checks, and the above, might be just too much: cannot check everything anyway
	if (p_node == nullptr || p_node->prev == nullptr || p_node->next == nullptr)    
											throw std::out_of_range("vlist: cannot dereference out-of-range iterator") ;
	return((static_cast<node_VAL<DTy>*>(p_node))->m_val) ;
} // _vl_iter operator *

template <typename DTy> inline _vl_iter<DTy>&
_vl_iter<DTy>::operator =(const _vl_iter& it)
{
	p_node = it.p_node ;
	return(*this) ;
} // _vl_iter operator =

template <typename DTy> inline _vl_iter<DTy>&
_vl_iter<DTy>::operator ++()
{
	if (p_node == nullptr || p_node->next == nullptr) throw std::out_of_range("vlist: cannot increment end-list iterator") ;
	p_node = p_node->next ;
	return(*this) ;
} // _vl_iter operator ++()

template <typename DTy> inline _vl_iter<DTy>&
_vl_iter<DTy>::operator --()
{
	if (p_node == nullptr || ((p_node->prev)->prev) == nullptr) throw std::out_of_range("vlist: cannot decrement begin-list iterator") ;
	p_node = p_node->prev ;
	return(*this) ;
} // _vl_iter operator --()

#endif

// eof [const]_vlist_iter
