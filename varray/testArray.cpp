// testArray.cpp: 
//			- to test class template: class VArray<T,A> (in vArray.h)
//			  . esp. ranged for loop
//

#include <iostream>
#include <time.h>
#include <vector>
#include <memory>
#include <stdexcept>
using std::vector ;
using std::cout ;
using std::endl ;
using std::exception ;
using std::cerr ;
using std::allocator ;

#include "vArray.h"
#include "class_MError.h"

time_t show_lapse(const bool fl_start) ;

// using namespace OwnTrials ;

int
main()
{
	try {
							constexpr	size_t	num_elements = 10 ;

							bool				flOK{false} ;
							vector<int>			vi{1,2, 3, 4, 5, 6, 7} ;

							VArray<int>::iterator   it ;

							size_t					i {0} ;
							size_t					num = 0 ;
							

		cout << endl, show_lapse(true) ;

		// Place the TESTING BLOCK hereafter
		{
				cout << endl << "\n-- PREPARING var{1 - 10}:" ;
			VArray<int>		varr (VArray<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }) ;
			num = varr.size() ;

				// cout << endl << "\n---- result:" << varr ;
				cout << endl << "-- START:  reserve(+50) - ", show_lapse(false) ;
			flOK = varr.reserve(num_elements + 50) ;
				cout << endl << "---- result: " << (flOK ? "<OK>> " : "<ERROR>> ") ; // << varr ;

				cout << endl << "-- START:  resize(+40, 99) - ", show_lapse(false) ;
			flOK = varr.resize(varr.size() + 40, 99) ;
				cout << endl << "---- result: " << (flOK ? "<OK> " : "<ERROR> ") << varr ;

				cout << endl << endl << "\n--- Tests on ::iterator follow: " ;
				
				it = varr.begin() ;
				for (i = 0 , it = (varr.begin() + num) ; it != varr.end() ; i++, it += 1)   { // it++)   {   // ++it) {
					// if (i % ElementsPerRow == 0)    cout << '\n' ;
					// cout << std::setw(SpacePerElement) , cout << 
					(*it = varr.size() - i) ;   // *it = ... does not compile as well as *(ra_it = it) = ... does not compile
				}
				cout << endl << "--- end of *() = : setting [begin() + num(10), size) to (size - i). Result: " << endl ;
				for (it = varr.begin() ; it != varr.endlim() ; it += 1)   {   // ++it) {
					if (i++ % ElementsPerRow == 0)    cout << '\n' ;
					cout << std::setw(SpacePerElement) << (*it) ;
				}
				cout << endl << "--- and, now -- skip 'not-used 'num(10)'' via (endlim() - 1 - num) and, BACK through --:" ;
				for ((it = varr.endlim() - 1 - num) ; it >= varr.begin() ; --it)   {   // it -= 1)   {   // --it)   {    // it--) {
					if (i++ % ElementsPerRow == 0)    cout << '\n' ;
					cout << std::setw(SpacePerElement) << (*it) ;
				}

				cout << endl << endl << "-- varr is: " << varr << endl ;
				cout << endl << "-- show the last num(10) in reverse order" ;
				i = 0 ;
				for (it = varr.endlim() - (varr.capacity() - varr.size()) - 1 ; it >= varr.begin() + (varr.size() - num) ; --it) {
					if (i++ % ElementsPerRow == 0)    cout << '\n' ;
					cout << std::setw(SpacePerElement) << (*it) ;
				}
				cout << endl << endl << "-------------------- end of iterator tests";
		}
		// Limit of the TESTING Block

		cout << endl << endl << endl << endl << "-- end of try block - ", show_lapse(false) ;
	} catch (const exception& e) {
		cout << endl << "______________________________________________________ catch(exception&){}" ;
		cout << endl << endl , show_lapse(false) ;

		cerr << endl << endl
			<< endl << "___ Caught: " << e.what()
			<< endl << "___ Type  : " << typeid(e).name() << endl ;
	} catch (CM_error& e) {
		cout << endl << "______________________________________________________ catch(CM_error&){}" ;
		cout << endl << endl, show_lapse(false) ;
		cerr << endl << endl
			<< endl << "___ Caught: " << e.what()
			<< endl << "___ Type  : " << typeid(e).name() << endl ;
	} catch (...) {
		cout << endl << "===================================================___ catch(...){}" ;
	}

	cout << endl << endl << endl << "-- That's it ..." << endl << endl ;
	return(0) ;
} // main()

time_t
show_lapse(const bool fl_start)  // returns the lapse (in ...) since the start (fl_start)
{
						static time_t	start_lapse {0} ;
						static time_t	break_point {0} ;
						time_t			end_lapse {0} ;

	if (fl_start)	{
		time(&start_lapse) , break_point = start_lapse ;
		cout << "_____ start at: " << start_lapse ;
	} else if (start_lapse == 0)   {   // has never been started
		time(&end_lapse) ;
		cout << endl << "___ " << end_lapse << ": the stopwatch is not running ..." << endl ;
	} else {
		time(&end_lapse) ;
		cout << "_____ break -> lapse since: start(" << (end_lapse - start_lapse) << ')' ;
		cout << ", last break(" << (end_lapse - break_point) << ')' ;
		break_point = end_lapse , 
		end_lapse -= start_lapse ;
	}
	return(end_lapse) ;
} // show_lapse()


#ifdef TEST2_VECTOR_ETC
{ // Open TESTING Block Scope 
	vector<int>			vi ;
	for (size_t i = 0 ; i < 5 ; i++) vi.push_back(i) ;
	cout << endl << "-- vector is : " ; for (auto i : vi) cout << '(' << i << ", " << vi[i] << ')' ;

	cout << endl << endl << "-- test pop(): " ;
	// for (size_t i = 0 ; i < 6 ; i++)   cout << '(' << i << ", " << (vi.back()) << ')' , vi.pop_back() ; 
	// for (size_t i = 0 ; i < 6 ; i++)   cout << '(' << i << ", " << "--" << ')', vi.pop_back() ;
	for (size_t i = 0 ; i < vi.size() + 1 ; i++)   cout << '(' << i << ", " << vi.at(i) << ')' ;

	throw std::runtime_error("__End of Vector -> jump to catch()") ;
}   // CLose TESTING Block Scope
#endif

#ifdef TEST1_CONSTRUCTORS_ASSIGNMENTS

{ // Open TESTING Block Scope 
	// Testing Constructors and Assignments
	cout << endl << "\n-- preparing varr(n_el, 33):" ;
	VArray<int>					varr(num_elements, 33) ;
	cout << endl << "\n-- 'varr':" << varr ;

	cout << endl << "\n-- preparing va(varr):" ;
	VArray<int>		va((varr)) ;
	cout << endl << "\n-- va(varr):" << va ;
	cout << endl << "\n-- varr:" << varr ;

	cout << endl << "\n-- preparing varr(&&va):" ;
	VArray<int>		varr(std::move(va)) ;
	cout << endl << "\n-- varr(&&va):" << varr ;
	cout << endl << "\n--         va:" << va ;

	cout << endl << "\n-- preparing va1(..., 88):" ;
	VArray<int>     va1(num_elements * 2, 88) ;
	cout << endl << "\n-- preparing va = va1:" ;
	va = (va1) ;
	cout << endl << "\n-- va = va1:" << va ;
	cout << endl << "\n--      va1:" << va1 ;

	cout << endl << "\n-- preparing varr(&&..., 66):" ;
	varr = VArray<int>(num_elements * 2, 66) ;
	cout << endl << "\n--                   varr:" << varr ;
	cout << endl << "\n-- preparing va1 = &&varr:" ;
	va1 = std::move(varr) ;
	cout << endl << "\n-- va1 = &&varr:" << va1 ;
	cout << endl << "\n--         varr:" << varr ;
}   // CLose TESTING Block Scope

#endif

#ifdef TEST2_RESERVE_RESIZE_INITLIST_PUSH_POP_ETC
{	// Open TESTING Block Scope		::   VArray{}, reserve(), resize(), empty(), push_back(), back(), pop()		
	bool flOK = true ;

	cout << endl << "\n-- PREPARING var{}:" ;
	VArray<int>					varr (VArray<int>{ 5, 6, 7, 8, 9, 10 }) ;
	cout << endl << "\n---- result:" << varr ;

	cout << endl << endl << "-- START:  reserve(60) - ", show_lapse(false) ;
	flOK = varr.reserve(num_elements + 50) ;
	cout << endl << "\n---- result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(+40, 99) - ", show_lapse(false) ;
	flOK = varr.resize(varr.size() + 40, 99) ;
	cout << endl << "\n---- result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(-20, 77)", show_lapse(false) ;
	flOK = varr.resize(varr.size() - 20, 77) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(0)", show_lapse(false) ;
	flOK = varr.resize(0) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(40, 88)", show_lapse(false) ;
	flOK = varr.resize(40, 88) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  pushing (temp=55) (20 + 10) times", show_lapse(false) ;
	for (size_t i = 0 ; i < 30 ; i++) { if (!(flOK = varr.push_back(temp = 55))) { cout << endl << "--- failed push #" << i ; break ; } }
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  pushing (i) until size() < capacity()", show_lapse(false) ;
	for (size_t i = 0 ; varr.size() < varr.capacity() ; i++) { if (!(flOK = varr.push_back(i))) { cout << endl << "--- failed push #" << i ; break ; } }
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  pushing varr.back() into va1", show_lapse(false) ;
	{ // introduce 'va1'
		VArray<int>		va1(0, 0, varr.size()) ;
		for (size_t i = 0 ; !(varr.empty()) ; i++) {
			if (!(flOK = va1.push_back(varr.back()))) { cout << endl << "--- failed push #" << i ; break ; }
			// cout << "(" << i << ", " << varr.back() << "} " ; 
			varr.pop_back() ;
		}
		cout << endl << "\n----  result(varr): " << varr ;
		cout << endl << "\n----  result(va1): " << (flOK ? "<OK>> " : "<ERROR>> ") << va1 ;
		cout << endl << "\n-- START: clear() va1" ;
		va1.clear() ;
		cout << endl << "\n----  result(va1): " << va1 ;

		cout << endl << "\n-- START: va1-> shrink_to_fit()" ;
		va1.shrink_to_fit() ;
		cout << endl << "\n----  result(va1): " << va1 ;
	} // end 'va1'

	cout << endl << endl << "-- START:  'varr' -> reserve(50)", show_lapse(false) ;
	flOK = varr.reserve(50) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  pushing (i) until size() < capacity() / 2", show_lapse(false) ;
	for (size_t i = 0 ; varr.size() < (varr.capacity() / 2) ; i++) { if (!(flOK = varr.push_back(i))) { cout << endl << "--- failed push #" << i ; break ; } }
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(30, 88)", show_lapse(false) ;
	flOK = varr.resize(30, 88) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(50, 88)", show_lapse(false) ;
	flOK = varr.resize(50, 88) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  shrink_to_fit()", show_lapse(false) ;
	varr.shrink_to_fit() ;
	cout << endl << "\n----  result: " << varr ;

	{ // StOS 'va_const'
		cout << endl << endl << "\n-- START: va_const(varr)" ;
		const VArray<int>	va_const(varr) ;
		cout << endl << "\n----  result 'va_const': " << va_const ;

		cout << endl << "\n-- START: testing operator[] on va_const" ;
		for (size_t i = 0 ; i < va_const.size() ; i++) {
			if (i % ElementsPerRow == 0)			cout << endl ;
			cout << std::setw(SpacePerElement) << (va_const[i]) ;  // ERROR: va_const[i] = i 
			// if (i == 30)	cout << endl << "--- at(30) -->> va_const[size()]: " << va_const[va_const.size()] << endl ;
		}
	} // EOS 'va_const;

	{ // StOS 'va1'
		cout << endl << endl << "\n-- START: va1(varr)" ;
		VArray<int>	va1(varr) ;
		cout << endl << "\n----  result 'va1': " << va1 ;

		cout << endl << "\n-- START: testing operator[] on va1: change va1[i] to (size() - i)" ;
		for (size_t i = 0 ; i < va1.size() ; i++) {
			if (i % ElementsPerRow == 0)			cout << endl ;
			cout << std::setw(SpacePerElement) << (va1[i] = (va1.size() - i)) ;  // OK: va1[i] = i
			// if (i == 30)	cout << endl << "--- at(30) -->> va1[size()]: " << va1[va1.size()] << endl ;
		}
		cout << endl << "\n----  result 'va1': " << va1 ;
	} // EOS 'va1'
		}   // CLose TESTING Block Scope
#endif

#ifdef TEST3_TEST3_ITERATORS_COMPARISONS
{ // Comparisons for (it1 < rho)
cout << endl << "-- begin() " << (varr.begin() < varr.end() ? "<" : ">=") << " end()" ;
cout << endl << "-- begin() " << (varr.begin() > varr.end() ? ">" : "<=") << " end()" ;
cout << endl << "-- begin() " << (varr.begin() >= varr.end() ? ">=" : "<") << " end()" ;
cout << endl << "-- begin() " << (varr.begin() <= varr.end() ? "<=" : ">") << " end()" ;
				}
{ // Comparisons for (it1 = rho)
	cout << endl << "-- (it = begin(), so: it " << (it > varr.begin() ? ">" : "<=") << " begin()" ;
	cout << endl << "--                    it " << (it >= varr.begin() ? " >= " : "<") << " begin()" ;
	cout << endl << "--                    it " << (it < varr.begin() ? "<" : ">=") << " begin()" ;
	cout << endl << "--                    it " << (it <= varr.begin() ? "<=" : ">") << " begin()" ;
}
{ // Comparisons for (it > rho)
	cout << endl << "-- (it = end() > begin(), so: it " << (it > varr.begin() ? ">" : "<=") << " begin()" ;
	cout << endl << "--                            it " << (it >= varr.begin() ? ">=" : "<") << " begin()" ;
	cout << endl << "--                            it " << (it < varr.begin() ? "<" : ">=") << " begin()" ;
	cout << endl << "--                            it " << (it <= varr.begin() ? "<=" : ">") << " begin()" ;
}
#endif
