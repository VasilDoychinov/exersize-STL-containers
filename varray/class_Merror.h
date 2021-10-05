// classM_error.h:	describes class CM_error - for custom error handling via throw-catch
//					- base : runtime_error
//

#ifndef OWNOWN_ERRORS_CAT
#define OWNOWN_ERRORS_CAT


#include <string>
#include <stdexcept>
using std::string ;
using std::ostream ;
using std::runtime_error ;
using std::bad_alloc ;
using std::out_of_range ;


class CM_error	{   // : public exception { // runtime_error, bad_alloc, out_of_range	{
	private:
		string	m_msg ;
		int		m_id ;	
	public:
		explicit CM_error(const string& msg = "__ : of my errors", const int id = 0) : m_msg{msg}, m_id{id} {}
		virtual ~CM_error() {}

		const string what()     { return(m_msg) ; }
		const int	 id() const { return(m_id) ; }
		// const string name() const { return("CM_error") ; }
}; // class CM_error

#endif
// eof classM_error.h