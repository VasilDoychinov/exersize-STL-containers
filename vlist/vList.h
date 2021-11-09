// vList.h:	defines a double-linked list vlist<T,A>
//			Depends on: 
//				- nodes\nodes.h: representation of a node 
//				- class_VLI.h:  ... of related to vlist<> Bidirectional iterators
// 
// 
//			General notes:
//				- provides Strong or Basic guarantee, no exceptions thrown - as mentioned
//				- elements are of type <T>
//				- memory handling is through the template parameter A (allocator). 
//				  So, if any SPECIAL(other than std::allocator<T>) memory handling is needed, a proper allocator is to be provided
//				- follows the interface of STL list 
//				  
//				- destroyed from 'left' to 'right'
//				- all Constructors : leave a valid list constructed before an exception was thrown
//				- template parameters: T- type of the elements, Allocator = allocator<T>
//				- Member types:
//					- value_type = t
//					- size_type = size_t
//					- [const_]pointer = [const] T*
//					- [const_]reference = [const] T&
//					- [const_]iterator = _vl_iter<[const] T> ;
//			Interface:
//				- Member functions: public
//					- Constructors: as in <list> 
//					- Destructor
//					- operator = (const & / &&): as in <list>: provides Strong Guarantee
//					- assign() family: as in <list>. For behaviour - see Constructors//
//				- Element Access: public
//					- [const_]reference back() : as in <list> throws out_of_range on an empty list
//					- [const_]reference front() : ...               
//				- Bidirectional iterators vlist<T,A>::[const_]iterator :
//					- [c]begin/[c]end()
//					- operators: =, *, ++, --
//				- Capacity: public
//					- size(), empty(): as in <list>
//				- Modifiers: public
//					- clear():         ...
//					- insert(T&/&&): ...    ; Strong guarantee: return iterator
//						- insert(sequence): ... ; Strong guarantee: returns iterator
//						- insert(initializer_list): ... ; Strong guarantee: returns iterator	
//					- emplace() family: ...; Strong guarantee, rethrows any exception thrown; FORCES rval
//					- erase(iterator), erase(iterator f, iterator s): ... noexcept. NB: NO validity check possible 
// 					- push_[back/front](&/&&): ...
//					- pop_[back/front](): ...; do nothing on empty
//	 				- resize(): as in <list> : Strong Guarantee
//					- .swap() : as in <list> ... noexcept
//				- Operations: public
//					- merge(): as in <list> 
//					- remove(const_reference): ... but returns # of elements removed: noexcept. NB: is == is to be provided by T
//					- remove_if(cond): requires cond convertible to std::fucntion<bool(T&)>: returns # of elements removed: noexcept.
//					- sort(): ... : noexcept ;
//					- sort(std::function<bool(const_iterator,const_iterator)> comp): ... : noexcept
//					- splice family of (): as in <list>: noexcept
//					- unique(cond = operator ==) :
//				- Templates(friends):
//					- operator == (const vlist&, const vlist&)
//					- operator <<(const std::ostream&, const vlist<T,A>&)
//

#ifndef CLASS_VLIST_CLASS_VLIST_CLASS
#define CLASS_VLIST_CLASS_VLIST_CLASS

#define CLASS_VLIST_CLASS_VLIST_TEST // For testing stage

#include <iterator> // For std::bidirectional_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <iomanip>
#include <exception>

#include <iostream>
using std::cout ;
using std::cerr ;
#include <utility>
#include <memory>
using std::allocator ;
using std::forward ;
using std::move ;
#include <functional>


#include "..\nodes\nodes.h"						// Definitions for nodes
#include "..\iterators\class_VLI.h"					// ... vlist<> iterators


// class vlist<> defienitions, etc follow

template <typename T, typename A = allocator<T>>
class vlist {
	public:
		using value_type	= T ;						// a TYPE that represents the data type (T) stored in the list
		using size_type		= size_t ;					// ...         counts the # of elements ...
		using pointer		= T* ;						// ...         provides a pointer to an element in the list
		using const_pointer = const T* ;				// ...         ...                   a const element ...
		using reference		= T& ;						// ...         ...        reference to an element ...
		using const_reference = const T& ;				// ...         ...                     a const element ...

		using iterator = typename _vl_iter<T> ;
		using const_iterator = typename _vl_iter<const T> ;

	private:
		using node = typename node_VAL<T> ;
		using nodeB = typename CB_nodePN ;
		using VL_alloc = typename A::template rebind<node_VAL<T>>::other;

		VL_alloc	_all ;
		A			_tall ;

		nodeB		l_head ;		// the left limit (bumper): .prev is nullptr. Probably, they could be just two pointer instead of nodeB but...
		nodeB		l_tail ;		// the right ...            .next is ...

#ifdef CLASS_VLIST_CLASS_VLIST_TEST
		void  show_bumpers(vlist&) ;
		void  show_lim(nodeB * lim) ;
#endif

		size_type	m_size ;

		void   deall_node(node* wn) noexcept ;				// destroy and deallocate the node
		nodeB* era_seq(nodeB* pn, nodeB* lim) noexcept ;	// ... (= erases) all in [++pn,lim). works on [pn, end]
		nodeB* era_node(nodeB* pn) noexcept ;				// erases a node; returns node->next ;
		
		template <typename FTy> node* new_node(node* prev, FTy val) ;	// insert a NEW node between prev and next. Return node* or nullptr
		
		template <typename InpIt> void _seq_to_empty(InpIt first, InpIt last) ; // [first, last} into an empty list
		template <typename FTy> iterator _emplace(iterator wh, FTy val) ;       // place val into wh

		iterator _ins_vl(iterator wh, vlist<T, A>&& vl) noexcept ;   // engulf vl into wh
		iterator _move_vl_seq(nodeB* wh, nodeB* f, nodeB* l) noexcept ;   // transfer [f,l) into wh
		iterator _move_it_seq(const_iterator wh, const_iterator f, const_iterator l) noexcept ;   // transfer [f,l) into wh

		iterator _vli_next(const_iterator it) { return(static_cast<iterator>((it.p_node)->next)) ; }

	public:
		explicit   vlist(A a = A()) ;								// default Constructor
		virtual   ~vlist() ;										// Destructor
		explicit   vlist(size_type num, const_reference val = value_type(), A a = A()) ;
		
		vlist(std::initializer_list<value_type> ilist, A a = A()) ;				// from initializer_list
		template <typename InpIt> vlist(InpIt first, InpIt last, A a = A()) ;	// ... any type of input sequence presented by [first, last)

		vlist(const vlist& l) ;										// Copy C.
		vlist(vlist&& l) noexcept ;									// Move C.
		
		vlist& operator =(const vlist& vl) ;						// Copy A.
		vlist& operator =(vlist&& vl) noexcept ;					// Move A.

		void		assign(size_type count, const T& val) ;					// as in <list>
		void		assign(std::initializer_list<value_type> ilist) ;		// ...
		template <typename InpIt> void assign(InpIt first, InpIt last) ;	// ...

		reference	back() ;										// reference to the last element 
		const_reference back() const ;								// const_reference
		reference	front() ;										// ... 1st ...
		const_reference front() const ;								// ...

		void		emplace(iterator wh, value_type&& val) { _emplace(wh, move(val)) ; } // Inserts val into 'where'				
		void		emplace_back(value_type&& val)  { _emplace(end(), move(val)) ; }	 // adds at the end
		void		emplace_front(value_type&& val) { _emplace(begin(), move(val)) ; }	 // inserts in front

											// the INSERT family of ()
		template <typename FTy> iterator insert(iterator wh, FTy val) { return(_emplace(wh, forward<FTy>(val))) ; }
		iterator insert(iterator wh, size_type count, const T& val) ;
		iterator insert(iterator wh, std::initializer_list<value_type> il) ;
		template <typename InpIt> iterator insert(iterator wh, InpIt first, InpIt last) ;

		template <typename FTy> void push_back(FTy val) { _emplace(end(), forward<FTy>(val)) ; }
		template <typename FTy> void push_front(FTy val) { _emplace(begin(), forward<FTy>(val)) ; }
		
		void		pop_back() noexcept ;
		void		pop_front() noexcept ;

		size_type	size() const  { return(m_size) ; }
		bool		empty() const { return(m_size == 0) ; }			// if empty
		void		clear() noexcept ;								// Clear all to an empty list(initial state)
		iterator	erase(iterator where) noexcept ;				// removes the element. returns iterator to the next
		iterator	erase(iterator first, iterator last) noexcept ;	// ... [first, last)    ...

		size_type   remove(const_reference val) noexcept ;
		size_type   remove_if(std::function<bool(const_reference)> cond) noexcept ;

		void		resize(size_type count, const_reference val = value_type()) ;
		
		void		splice(const_iterator wh, vlist& oth) noexcept { _ins_vl(static_cast<iterator>(wh.p_node), move(oth)) ; }
		void		splice(const_iterator wh, vlist&& oth) noexcept { _ins_vl(static_cast<iterator>(wh.p_node), move(oth)) ; }
		void		splice(const_iterator wh, vlist& oth, const_iterator it) noexcept { splice(wh, move(oth), it, _vli_next(it)) ; }
		void		splice(const_iterator wh, vlist&& oth, const_iterator it) noexcept { splice(wh, move(oth), it, _vli_next(it)) ; }
		void		splice(const_iterator wh, vlist& oth, const_iterator first, const_iterator lim) noexcept 
																					{ splice(wh, move(oth), first, lim) ; }
		void		splice(const_iterator wh, vlist&& oth, const_iterator first, const_iterator lim) noexcept ;

		void		sort(std::function<bool(const_reference,const_reference)> comp= [](const T v1,const T v2)->bool{return(v1<v2);}) noexcept ;

		void		merge(vlist&& o, std::function<bool(const_reference, const_reference)> 
											comp = [](const T v1, const T v2)->bool{return(v1<v2);}) noexcept;
		void		merge(vlist& o, std::function<bool(const_reference, const_reference)> 
											comp = [](const T v1, const T v2)->bool{return(v1<v2);}) noexcept { merge(move(o), comp) ; }

		void		swap(vlist<T,A>& rvl) noexcept ;

		size_type	unique(std::function<bool(const_reference, const_reference)> comp = [](const T v1, const T v2)->bool{return(v1==v2);}) noexcept ;

		const_iterator cbegin() const { return(const_iterator(l_head.next)) ; }
		const_iterator cend()   const { return(const_iterator((l_tail.prev)->next)) ; }
		iterator   begin() const { return(iterator(l_head.next)) ; }
		iterator   end() const   { return(iterator((l_tail.prev)->next)) ; }

		// Friends ...
		template <class T, class A> friend void swap(vlist& lvl, vlist& rvl) noexcept ;
		template <class T, class A> friend bool operator ==(const vlist& rvl, const vlist& lvl) ;
		template <class T, class A> friend std::ostream& operator << (std::ostream& os, const vlist& vl) ;
}; // class vlist

template<typename T, typename A> inline
vlist<T, A>::vlist(A a) : _all{a}, _tall{VL_alloc(a)}, l_head{nullptr,&l_tail}, l_tail{&l_head,nullptr}, m_size{0}
{
																		// cout << "\n___ vlist ...";
} // vlist default


template <typename T, typename A> inline void
vlist<T, A>::deall_node(node* wn) noexcept
{
	(&(wn->m_val))-> ~T(),						// call T's destructor
		_all.deallocate(wn, 1) ;
} // vlist:: deall_node

template <typename T, typename A> typename vlist<T, A>::nodeB*
vlist<T, A>::era_seq(nodeB* pn, nodeB* lim) noexcept
{							// pn HAS to be in [begin, end); lim in [pn, end]
	// Start with pn->next, move lim, ... onto; destroy object meanwhile
	size_type count = 0 ;
	nodeB* const pPrev = (pn->prev) ;
																// cout << endl ;
																// cout << endl << endl << "- pPrev is " << size_t(pPrev) << " pn is " << size_t(pn) ;
	nodeB* wn = pn ;		// the one to be removed
	for (count = 0 ; wn != lim && wn != &l_tail ; wn = pn, count++) { // deconstruct elements and free memory
																// cout << endl << "_ destroying # " << (count) << ": {" << /*wn->m_val <<*/ '}' ;
		pn = (wn->next),
			deall_node(static_cast<node*>(wn)) ;
	}
	(pPrev)->next = wn, wn->prev = pPrev ;		// the list is linked 
																//cout << endl << endl ;
																// cout << "- pPrev is " << size_t(pPrev) << " ->next is " << size_t(pPrev->next) ;
																// cout << "- wn    is " << size_t(wn   ) << " ->prev is " << size_t(wn->prev) ;
	m_size -= count ;
	return(wn) ;
} // vlist:: era_seq()

template <typename T, typename A> typename vlist<T, A>::nodeB*
vlist<T, A>::era_node(nodeB* pn) noexcept
{							// pn HAS to be in [begin, end); lim in [pn, end]
	nodeB* pP = pn->prev ;
	nodeB* pN = pn->next ;

	deall_node(static_cast<node*>((pn))) ;
	pP->next = pN , pN->prev = pP ;           // completed: links
	return(pN) ;
} // vlist:: era_node()

template <typename T, typename A> inline void
vlist<T, A>::clear() noexcept
{
																// cout << endl << "- before clear: ", show_bumpers(*this) ;
	era_seq((l_head.next), (&l_tail)) ;
	l_head.next = &l_tail, l_tail.prev = &l_head ; // for an empty list
} // vlist clear()

template <typename T, typename A> inline
vlist<T, A>::~vlist()
{
																// cout << "\n___ ~ vlist of size " << m_size << " ..." ;
	clear() ;
} // vlist ~ ()


template <typename T, typename A> 
template <typename FTy> typename vlist<T, A>::node*
vlist<T, A>::new_node(node* prev, FTy val)		// does NOT set ->right; 
{
	node* wn{ nullptr } ;
	try {
		wn = _all.allocate(1) ;
		new (static_cast<void*>(&(wn->m_val))) T(forward<FTy>(val)) ;
		wn->prev = prev ;     // (wn->right = nullptr): Save time
	}
	catch (...) { throw ; }
	return(wn) ;
} // vlist:: new_node(FTy)


template <typename T, typename A>
vlist<T, A>::vlist(size_type num, const_reference val, A a) : _all{ a }, _tall{ VL_alloc(a) },
l_head{ nullptr,&l_tail }, l_tail{ &l_head,nullptr }, m_size{ 0 }
{															// It is a valid vlist
																// cout << "\n___ vlist of size " << num ;
	if (num > 0) {
		node* tail{ nullptr } ;
		try {
			l_head.next = tail = new_node(static_cast<node*>(&l_head), val) ;			// + itostr(m_size)) ; ???? TESTING
			// Now the 1st node is: (&left bumper, val, ???)
			for (m_size = 1 ; m_size != num ; m_size++) {
				tail->next = new_node(tail, val) ;       // it is (right, val, ???)     // + itostr(m_size) + val) ;			
				tail = static_cast<node*>(tail->next) ;
			}
			tail->next = &l_tail, l_tail.prev = tail ;   // to the right bumper as: no more nodes
		}
		catch (...)   {    cerr << "___ vlist(): exception thrown -> re-thrown" ;
			tail->next = &l_tail, l_tail.prev = tail ;	// To validate vlist of m_size elements as all before the exception are OK
			throw ;
		}
	}
	// NB: if an exception is thrown, a valid vlist, only shorter, has still been created
} // vlist (size_type, const T&)


template <typename T, typename A>
template <typename InpIt> void
vlist<T, A>::_seq_to_empty(InpIt first, InpIt last) // re-throws any exception generated during memory allocation of T()
{													// leaves a valid list: Basic G.
	if (first != last) {
		node* tail = nullptr ;
		try {
			l_head.next = tail = new_node(static_cast<node*>(&l_head), *first) ;
			for (++first, m_size = 1 ; first != last ; ++first, m_size++) {
				tail->next = new_node(tail, *first) ;       // it is (right, val, ???)     // + itostr(m_size) + val) ;			
				tail = static_cast<node*>(tail->next) ;
			}
			tail->next = &l_tail, l_tail.prev = tail ;   // to the right bumper as: no more nodes
		}
		catch (...) {
			cerr << "___ vlist():: _seq_to_empty(): exception thrown -> re-thrown" ;
			tail->next = &l_tail, l_tail.prev = tail ;	// To validate vlist of m_size elements as all before the exception are OK
			throw ;
		}
	}
} // vlist:: _seq_to_empty()

template <typename T, typename A>
template <typename InpIt> inline
vlist<T, A>::vlist(InpIt first, InpIt last, A a) : _all{ a }, _tall{ VL_alloc(a) },
												   l_head{ nullptr,&l_tail }, l_tail{ &l_head,nullptr }, m_size{ 0 }
{												 // It is a valid vlist
																// cout << "\n___ vlist [f, l)" ;
	_seq_to_empty(first, last) ;
} // vlist InpIt

template <typename T, typename A> inline
vlist<T,A>::vlist(std::initializer_list<value_type> ilist, A a) : _all{ a }, _tall{ VL_alloc(a) },
																  l_head{ nullptr,&l_tail }, l_tail{ &l_head,nullptr }, m_size{ 0 }
{												 // It is a valid vlist
																// cout << "\n___ vlist (initializer_list) of size " << ilist.size() ;
	_seq_to_empty(ilist.begin(), ilist.end()) ;
} // vlist initializer_list


template <typename T, typename A> inline
vlist<T, A>::vlist(const vlist<T,A>& vl) : _all{ vl._all }, _tall{ vl._tall },
										   l_head{ nullptr,&l_tail }, l_tail{ &l_head,nullptr }, m_size{ 0 }
{										// It is a valid vlist
																// cout << "\n___ vlist (& of size) " << vl.size() ;
	_seq_to_empty(vl.cbegin(), vl.cend()) ;
} // vlist &

template <typename T, typename A> inline
vlist<T, A>::vlist(vlist<T, A>&& vl) noexcept : _all{ vl._all }, _tall{ vl._tall },
												l_head{ nullptr,&l_tail }, l_tail{ &l_head,nullptr }, m_size{ 0 }
{										// It is a valid vlist
																// cout << "\n___ vlist (&&) of size " << vl.size() ;
													// cout << endl << "- initial this is ", show_bumpers(*this) ;
													// cout << endl << "-         temp is " , show_bumpers(vl) ;
	if (vl.m_size != 0)           {
		l_head.next = (vl.l_head).next ,				// head bumper "next"s to first of the input list
		(l_head.next)->prev = &l_head ;					// and vice versa 
		l_tail.prev = (vl.l_tail).prev ,				// tail bumper : "prev"s to the last of the input list
		(l_tail.prev)->next = &l_tail ;					// and vice versa
		m_size = vl.m_size ;

		(vl.l_head).next = &(vl.l_tail) , 
		(vl.l_tail).prev = &(vl.l_head) , vl.m_size = 0 ;
	}
																// cout << endl << "- final   this is ", show_bumpers(*this) ;
																// cout << endl << "-         temp is ", show_bumpers(vl) ;
} // vlist &&


template <typename T, typename A> typename vlist<T, A>&
vlist<T, A>::operator =(const vlist& vl)   // provides Strong guarantee
{
																// cout << "\n__ vlist operator = &" ;
	if (this != &vl) {
		try {
			vlist<T, A>    tmp(vl) ;
			swap(tmp) ;		// tmp should destroy this
		}
		catch (...) { cerr << "___ vlist operator = &: exception thrown -> re-thrown" ;	throw ; }
	}
	return(*this) ;
} // vlist operator =(&)

template <typename T, typename A> inline typename vlist<T, A>&
vlist<T, A>::operator =(vlist&& vl) noexcept
{
																// cout << "\n__ vlist operator = &&" ;
	if (this != &vl) {
		// a strong care about freeing resources:   vlist<T, A>  tmp(std::move(vl)) ; swap(tmp) ; // tmp should destroy this
		// However, supposedly, vl is to be destroyed being rval. SO,
		swap(vl) ; // has  to suffice. and, no need of vl.clear()
	}
	return(*this) ;
} // vlist operator =(&)


template <typename T, typename A> inline void
vlist<T, A>::assign(size_type count, const T& val)
{
	vlist<T,A> tmp(count, val) ;
	swap(tmp) ;
} // vlist assign()

template <typename T, typename A> inline void
vlist<T, A>::assign(std::initializer_list<T> ilist)
{
	vlist<T, A> tmp(ilist) ;
	swap(tmp) ;
} // vlist assign()

template <typename T, typename A> 
template <typename InpIt> inline void
vlist<T, A>::assign(InpIt first, InpIt last)
{
	vlist<T, A> tmp(first, last) ;
	swap(tmp) ;
} // vlist assign()


template <typename T, typename A> inline typename vlist<T, A>::reference
vlist<T, A>::back() // improvement: implement compiler warning levels 
{
	if (empty())		throw std::out_of_range("___ vlist::back() -> empty list") ;
	return(reference((static_cast<node*>(l_tail.prev))->m_val)) ;
} // vlist back()

template <typename T, typename A> inline typename vlist<T, A>::const_reference
vlist<T, A>::back() const
{
	if (empty())		throw std::out_of_range("___ vlist::back() -> empty list") ;
	return(reference((static_cast<node*>(l_tail.prev))->m_val)) ;
} // vlist back()

template <typename T, typename A> inline typename vlist<T, A>::reference
vlist<T, A>::front()
{
	if (empty())		throw std::out_of_range("___ vlist::front() -> empty list") ;
	return(reference((static_cast<node*>(l_head.next))->m_val)) ;
} // vlist front()

template <typename T, typename A> inline typename vlist<T, A>::const_reference
vlist<T, A>::front() const
{
	if (empty())		throw std::out_of_range("___ vlist::front() -> empty list") ;
	return(reference((static_cast<node*>(l_head.next))->m_val)) ;
} // vlist front()


template <typename T, typename A> 
template <typename FTy> typename vlist<T,A>::iterator
vlist<T, A>::_emplace(iterator wh, FTy val)    // std::forward<FTy>(val)
{
	nodeB* const pP = (wh.p_node)->prev ;
	nodeB* const pN = (pP)->next ;   // must insert between pW->prev and pN

	pP->next = new_node(static_cast<node*>(pP), forward<FTy>(val)) ;	// complete links with previous; if exception - nothing will change
	(pP->next)->next = pN, pN->prev = pP->next ;				// ... next
	m_size++ ;

	return(static_cast<iterator>(pP->next)) ;
} // vlist:: _emplace()

template <typename T, typename A> inline typename vlist<T,A>::iterator
vlist<T, A>::_move_it_seq(const_iterator wh, const_iterator f, const_iterator l) noexcept
{
	return(_move_vl_seq(wh.p_node, f.p_node, l.p_node)) ;
} // vlist:: _move_it_seq()

template <typename T, typename A> typename vlist<T,A>::iterator                                   // returns start of engulfed
vlist<T, A>::_move_vl_seq(nodeB* wh, nodeB* first, nodeB* lim) noexcept  // transfer [f,l) into wh; leaves size(s) unaltered
{															
	// first, last might be from a different list: No care of Allocators here
	// Cannot check if [f,l) size is meaningful here
	// first : [begin, --end) and last : [++begin, end) presumed here
	// Care of new size is to be taken of outside this function

	nodeB* const vlHead = (first)->prev ; nodeB* const vlTail = (lim) ;	// to be transferred

	nodeB* const pP = (wh)->prev ;		// the Host 
	nodeB* const pN = (pP)->next ;				// must insert between pW->prev and pN

	pP->next = vlHead->next , (pP->next)->prev = pP ;		// completed: links with Host's previous
	pN->prev = vlTail->prev , (pN->prev)->next = pN ;		// ...                          next
	// m_size += vl.m_size ;   // care of is to be taken outside

	vlHead->next = vlTail , vlTail->prev = vlHead ; // wrong ->next ;   // completed: links of source
	// source's size: care of out side as well // other.m_size = 0 ;   // for good order's sake

	return(static_cast<iterator>(pP->next)) ;  // to the start of endgulfed
} // vlist:: _move_vl_seq()

template <typename T, typename A> inline typename vlist<T, A>::iterator
vlist<T, A>::_ins_vl(iterator wh, vlist<T, A>&& vl) noexcept  // engulf vl into wh: returns start of engulfed
{
	if (vl.m_size == 0)     return(wh) ;
	wh = _move_vl_seq(wh.p_node, (vl.begin()).p_node, (vl.end()).p_node) , m_size += vl.m_size , vl.m_size = 0 ;
	return(wh) ;  // to the start of endgulfed
} // vlist:: _ins_vl()

template <typename T, typename A> void
vlist<T, A>::splice(const_iterator wh, vlist&& oth, const_iterator first, const_iterator lim) noexcept
{
	if (oth.m_size > 0) {
		vlist<T, A>::const_iterator   cit_e = oth.end() ;
		vlist<T, A>::const_iterator   cit = first ;
		size_type count = 0 ;
		for ( ; cit != lim && cit != cit_e ; count++, ++cit) ;   // count length [first, lim) 
		if (count > 0)   { _move_vl_seq(wh.p_node, first.p_node, lim.p_node) , m_size += count , oth.m_size -= count ; }
	}
} // vlist splice()

template <typename T, typename A>
template <typename InpIt> typename vlist<T,A>::iterator
vlist<T, A>::insert(iterator wh, InpIt first, InpIt last)
{
	try { 
		vlist<T, A> tmp(first, last) ;		// an exception might have occurred only here
		return(_ins_vl(wh, move(tmp))) ;
	} catch (...) { throw ; }				// 'this' is intact
} // vlist insert()

template <typename T, typename A> typename vlist<T, A>::iterator
vlist<T, A>::insert(iterator wh, std::initializer_list<value_type> il)
{
	if (il.size() <= 0)   return(wh) ;
	try {
		vlist<T, A> tmp(il) ;				// an exception might have occurred only here
		return(_ins_vl(wh, move(tmp))) ;
	}
	catch (...) { throw ; }					// 'this' is intact
} // vlist insert()

template <typename T, typename A> typename vlist<T,A>::iterator
vlist<T, A>::insert(iterator wh, size_type count, const T& val)
{
	if (count <= 0)   return(wh) ;
	try { 
		vlist<T, A> tmp(count, val) ;		// an exception might have occurred only here
		return(_ins_vl(wh, move(tmp))) ;
	} catch (...) { throw ; }				// 'this' is intact
} // vlist insert()


template <typename T, typename A> inline typename vlist<T, A>::iterator
vlist<T, A>::erase(iterator first, iterator last) noexcept
{
	return(static_cast<iterator>(era_seq(first.p_node, last.p_node))) ;
} // vlist erase()

template <typename T, typename A> inline typename vlist<T, A>::iterator
vlist<T, A>::erase(iterator what) noexcept
{
	return(static_cast<iterator>(era_seq(what.p_node, (what.p_node)->next))) ;
} // vlist erase()

template <typename T, typename A> inline void
vlist<T, A>::pop_back() noexcept
{
	if (m_size > 0)    era_seq(l_tail.prev, &l_tail) ;
} // vlist pop_back()

template <typename T, typename A> inline void
vlist<T, A>::pop_front() noexcept
{
	if (m_size > 0)    era_seq(l_head.next, (l_head.next)->next) ;
} // vlist pop_front()

template <typename T, typename A> inline typename vlist<T,A>::size_type
vlist<T, A>::remove(const_reference val) noexcept
{
	return(remove_if([&](const_reference nv)->bool{ return(nv == val) ; })) ;
} // vlist remove()

template <typename T, typename A> typename vlist<T, A>::size_type
vlist<T, A>::remove_if(std::function<bool(const_reference)> cond) noexcept
{
	vlist<T, A>::iterator it = begin() ;
	vlist<T, A>::iterator ite = end() ;
	size_type			  count = 0 ;

	for (count = 0 ; it != ite ; ) {
		if (cond(*it)) {
			it = static_cast<iterator>(era_node(it.p_node)) ;
			count++ ;
		} else  ++it ;
	}
	m_size -= count ;
	return(count) ;
} // vlist remove_if()

template <typename T, typename A> void
vlist<T, A>::resize(size_type count, const_reference val)
{
	if (count > m_size)  insert(end(), count - m_size, val) ;	// Append (count - m_size) elements val
	else if (count < m_size) {
		vlist<T,A>::iterator it = end() ;
		for (size_type i = m_size ; i > count ; i--, --it) ;
		era_seq((it.p_node), (end()).p_node) ;  // adjust m_size as well
	}
	// nothing to do
} // vlist resize()


template <typename T, typename A> void
vlist<T, A>::sort(std::function<bool(const_reference, const_reference)> comp) noexcept
{
	// Move all nodes in ordered (by comp) fashion into a new vlist.
	// swap lists at the end
	if (m_size > 1) {
		vlist tmp ;								// Target: tmp.end() should not change throughout - tmp.begin() might
		const_iterator tit ;
		const_iterator tit_end = tmp.end() ;

		const_iterator it = begin() ;			// Source: .begin() changes after every move
		_move_it_seq(tmp.begin(), it, _vli_next(it)) , m_size-- , (tmp.m_size)++ ;

		 while (m_size > 0)   {     
			it = begin() ;		// preferred to taking the next of the previous
			for (tit = tmp.begin() ; tit != tit_end ; ++tit) {
				if (comp(*it, *tit))		break ; 
			}
			_move_it_seq(tit, it, _vli_next(it)) , m_size-- , (tmp.m_size)++ ;
		}
		swap(tmp) ;
	}
} // vlist sort()


template <typename T, typename A> void
vlist<T,A>::merge(vlist&& oth, std::function<bool(const_reference, const_reference)> comp) noexcept
{
	if (this == &oth || oth.m_size == 0)      return ;
	else if (m_size == 0)	{ swap(oth) ; return ; }

	vlist::iterator oit = oth.begin() ; vlist::iterator oite = oth.end() ;
	vlist::iterator it = begin()      ; vlist::iterator ite = end() ;
	vlist::iterator ob = oit ;

	for (vlist::size_type num = 0 ; it != ite ; it = _vli_next(it))   {
		for (ob = oit, num = 0 ; oit != oite && comp(*oit, *it) ; ++oit, num++) ; // the order of comparisons is IMPORTANT
		if (num > 0) {   // there are smaller in oth - [ob, oit) has to be moved into it
			it = _move_it_seq(it, ob, oit) ; // its the one before the move actually
		}                 		
		// it must move to the next one
		// oit is the start of oth
	}
	// move all remaining in oth (the greater than all in this)
	_move_it_seq(ite, oit, oite) , m_size += oth.m_size , oth.m_size = 0 ;
} // vlist merge()


template <typename T, typename A> void
vlist<T, A>::swap(vlist<T, A>& vl) noexcept
{
	nodeB* vlh = &(vl.l_head) ;
	nodeB* vlt = &(vl.l_tail) ;

	std::swap(_all, vl._all), std::swap(_tall, vl._tall) ;							// allocators
	if (m_size == 0 && vl.m_size == 0)                return ;
		
	if (m_size == 0) {  // vl is not empty
		l_head.next = vlh->next, (l_head.next)->prev = &l_head ;  // Fix the head
		l_tail.prev = vlt->prev, (l_tail.prev)->next = &l_tail ;  //  ...    tail
		vlh->next = vlt , vlt->prev = vlh ; // now it is the empty one
	} else if (vl.m_size == 0) { // vl is empty
		vlh->next = l_head.next, (vlh->next)->prev = vlh ;        // ... head
		vlt->prev = l_tail.prev, (vlt->prev)->next = vlt ;		  // ... tail
		l_head.next = &l_tail, l_tail.prev = &l_head ;            // ... empty
	} else {
		std::swap(l_head.next, (vlh)->next), std::swap(l_tail.prev, (vlt)->prev) ;	// bumpers' active fields
		// now bumpers point to the "new" sequences. sequences still not aligned
		(l_head.next)->prev = &l_head, (l_tail.prev)->next = &l_tail ;				// Fix (this)
		(vlh->next)->prev = vlh, (vlt->prev)->next = vlt ;							// Fix rhs(vl)
	}
	std::swap(m_size, vl.m_size) ;                                                  // ... sizes

	return ;
} // vlist swap()


template <typename T, typename A> typename vlist<T,A>::size_type
vlist<T, A>::unique(std::function<bool(const_reference, const_reference)> comp) noexcept
{
	size_type o_size = m_size ;
	if (m_size > 1) {
		vlist::iterator ite = end() ;
		vlist::iterator it = begin() ;
		vlist::iterator itN ;

		while (it != ite) {
			itN = _vli_next(it) ;
			for ( ; itN != ite && comp(*it, *itN) ; ++itN) ;    // to the 1st different: oreder of comparisons is IMPORTANT
			it = static_cast<iterator>(era_seq((_vli_next(it)).p_node, itN.p_node)) ;  
			// erased if any to;  it and itN are at the next different
			// era_seq() has taken care of limits and m_size
		}
	}
	return(o_size - m_size) ;
} // vlist unique()


// private functions used for testing follow

#ifdef CLASS_VLIST_CLASS_VLIST_TEST
template <typename T, typename A> void
vlist<T, A>::show_lim(nodeB* lim)
{
	cout << '(' << size_t(lim) << "){" << size_t(lim->prev) << ", " << size_t(lim->next) << "}" ;
} // vlist::

template <typename T, typename A> void
vlist<T, A>::show_bumpers(vlist<T, A>& vl)
{
	cout << "(" << std::setw(2) << vl.size() << "): ",
		show_lim(&(vl.l_head)) ; cout << " x " ; show_lim(&(vl.l_tail)) ; cout << " --tail(" << size_t((vl.l_tail.prev)->next) << ')' ;
} // vlist::
#endif


// Friends follow ...

template <typename T, typename A> inline void
swap(vlist<T, A>& lvl, vlist<T, A>& rvl) noexcept
{
	lvl.swap(rvl) ;
} // vlist friend swap()

template <typename T, typename A> bool
operator ==(const vlist<T, A>& rvl, const vlist<T, A>& lvl)
{
	if (lvl.size() == rvl.size()) {
		typename vlist<T,A>::iterator lit = lvl.begin() ; 
		typename vlist<T,A>::iterator rit = rvl.begin() ;

		for (auto num = lvl.size() ; num > 0 ; num--, ++lit, ++rit) {
			if (!(*lit == *rit))   return(false) ;
		}
		return(true) ;
	}
	return(false) ;
} // vlist friend operator ==()

template <typename DTy>
auto cbegin(const vlist<DTy>& vl) -> decltype(vl.cbegin()) { return(vl.cbegin()) ; }   // To preserve the const qualifier (for awhile
template <typename DTy>
auto cend(const vlist<DTy>& vl) -> decltype(vl.cbegin()) { return(vl.cend()) ; }


template <class T, class A> std::ostream&
operator << (std::ostream& os, const vlist<T,A>& ar)
{
	constexpr int ElementsPerRow{ 7 } ;    // For the output: ostream:
	constexpr int SpacePerElement{ 10} ;    // ... # of ' '

	os << "vlist <" << /*typeid(Data_type<decltype(ar.cbegin())>).name() <<*/ ">:"
		<< endl << "{of size " << ar.size() << '}' << '\n' ;
	if (ar.size() == 0) { os << "<EMPTY>" ; }

	size_t i = 0 ;
	for (auto p = ar.cbegin() ; p != ar.cend() ; ++p) { 
		if (i++ % ElementsPerRow == 0)    os << '\n' ;
		if (isVT_pointer(p)) { auto ptr = (&*p) ; os << std::setw(SpacePerElement) << (ptr ? *ptr : "NULL") ; }
		else                                      os << std::setw(SpacePerElement) << (*p) ;
		os << ':' << std::setw(2) << i ;
	}

	os << endl << "-------------------------------" ;
	return(os) ;
} // friend vlist<T,A>::operator << ()

// eoc vlist<T,A>

#endif

// eof vList.h