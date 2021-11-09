// nodes.h:	presents some possible nodes
//		General notes:
//			- base classes to establish links
//			- classes that hold data
//			  NB: allocation and data operations - left to the User
//


#ifndef CLASS_NODES_CLASS_NODES_CLASS
#define CLASS_NODES_CLASS_NODES_CLASS

class CB_nodePN {			// Base for nodes of a Prev-Next functionality
	public:
		CB_nodePN* prev ;			// to the previous node
		CB_nodePN* next ;			// ...    next
}; // class CNodeL_base

template <typename DTy>
class node_VAL : public CB_nodePN {			// the value (DATA) held in
	public:				// types ..
		using value_type = DTy ;
		using pointer = DTy* ;
		using const_pointer = const DTy* ;
		using reference = DTy& ;
		using const_reference = const DTy& ;

	public: 
		DTy				m_val ;
}; // class node_VAL 

#endif

// eof nodes.h
