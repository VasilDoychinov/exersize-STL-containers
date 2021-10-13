// testArray.cpp: 
//			- to test class template: class VArray<T,A> (in vArray.h)
//			  . esp. ranged for loop
//

#include <iostream>
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

#include "vArray.h"
#include "class_MError.h"

time_t show_lapse(const bool fl_start) ;

// using namespace OwnTrials ;

string
itostr(int i)
{
		std::stringstream	wwss ;
	wwss << i ; 
	string	str ;
	wwss >> str ;
	return(str) ;
} // itostr()

int
main()
{
	try {
							constexpr	size_t	num_elements = 20 ;
							bool				flOK{false} ;
							// vector<int>			vi{1,2, 3, 4, 5, 6, 7} ;
							// vector<int>			vi2{8, 8, 8, 8, 8, 8, 8, 8} ;
							// vi.insert(begin(vi) + 5, begin(vi2), end(vi2)) ;

							size_t				num = 0 ;
						

		cout << endl, show_lapse(true) ;

		// Place the TESTING BLOCK hereafter
		{ // insert(pos, from, to), insert_at(), insert(pos, &/&&), insert(pos, count, &val) 

			VArray<string>::const_iterator   cit_b ;
			VArray<string>::const_iterator   cit_e ;
			VArray<string>::iterator		 it_b ;
			VArray<string>::iterator		 it_e ;

			std::initializer_list<string> ilist1 = { "1st", "2nd", "3d", "4th", "5th", "6th", "7th", "8th", "9th", "10th" } ;
			std::initializer_list<string> ilist2 = { "-1-", "-2-", "-3-", "-4-", "-5-" } ;

			VArray<string>		va1(ilist1) ;
			cout << endl << endl << "--- to start with va1 is: " << va1 ;
			VArray<string>		va2(ilist2) ;
			cit_b = (it_b = begin(va2)), cit_e = (it_e = end(va2)) ;
			cout << endl << "--- sizeof va1 is " << (end(va1) - begin(va1)) << ", sizeof(va2) is " << (it_e - cit_b) ;

			// Insert at the end() 
			va1.insert(end(va1), cit_b, cit_e) ;         // _at(va1.size(), cit_b, cit_e) ;
			cout << endl << endl << "--- append va2 to va1 -> result in va1 is: " << va1 ;

			// Insert at []
			va1.insert(begin(va1) + 5, cit_b, cit_e) ;   // _at(5, cit_b, cit_e) ;
			cout << endl << endl << "--- insert va2 to va1 at [5]-> result in va1 is: " << va1 ;

			// Increase capacity
			va1.reserve(2 * (va1.capacity())) ;
			va1.insert(begin(va1), cit_b, cit_e) ;       // _at(0, cit_b, cit_e) ;
			cout << endl << endl << "--- capacity of va1 doubled;" << endl
				<< "    insert va2 to va1 at[0]->result in va1 is: " << va1 ;

			cout << endl << endl << "--- Testing insert(s) of values (one at a time) --------" << endl << endl ;

			string str { "=NC" } ; size_t i = va1.capacity() - va1.size() + 5 ;
			for (it_b = end(va1) ; i > 0 ; i--)    it_b = va1.insert(it_b, str) ;
			cout << endl << endl << "--- result of appending(const)): " << va1 ;

			va1.erase(end(va1) - size_t(20), end(va1)), va1.shrink_to_fit(), va1.reserve(2 * (va1.capacity()) - 5) ;
			cout << endl << endl << "--- insertions cleared; capacity doubled - 5. va1 is" << va1 ;

			i = va1.capacity() - va1.size() + 5 ;
			size_t  ni = i ;
			for (it_b = begin(va1) ; i > 0 ; i--)    it_b = va1.insert(it_b, str) ;
			cout << endl << endl << "--- result of inserting (const)): " << va1 ;

			va1.erase(begin(va1), begin(va1) + (ni)), va1.shrink_to_fit(), va1.reserve(2 * (va1.capacity()) - 5) ;
			cout << endl << endl << "--- insertions cleared; capacity doubled - 5. va1 is" << va1 ;

			i = va1.capacity() - va1.size() + 5 ;
			ni = i ;
			for (it_b = begin(va1) + 20 ; i > 0 ; i--)    it_b = va1.insert(it_b, itostr(ni - i)) ;
			cout << endl << endl << "--- result of inserting (move)): " << va1 ;

			va1.erase(begin(va1) + 20, begin(va1) + 20 + (ni)), va1.shrink_to_fit(), va1.reserve(2 * (va1.capacity()) - 5) ;
			cout << endl << endl << "--- insertions cleared; capacity doubled - 5. va1 is" << va1 ;
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
	cout << endl << "\n-- preparing varr(n_el, {33}):" ;
	VArray<string>					varr(num_elements, string("33")) ;
	cout << endl << "\n-- 'varr':" << varr ;

	cout << endl << "\n-- preparing va(varr):" ;
	VArray<string>		va((varr)) ;
	cout << endl << "\n-- va(varr):" << va ;
	cout << endl << "\n-- varr:" << varr ;

	cout << endl << "\n-- preparing va2(&&va):" ;
	VArray<string>		va2(std::move(va)) ;
	cout << endl << "\n-- va2(&&va):" << va2 ;
	cout << endl << "\n--         va:" << va ;

	cout << endl << "\n-- preparing va1(20, {88}):" ;
	VArray<string>     va1(num_elements * 2, "88") ;
	cout << endl << "\n-- preparing va = va1:" ;
	va = (va1) ;
	cout << endl << "\n-- va = va1:" << va ;
	cout << endl << "\n--      va1:" << va1 ;

	cout << endl << "\n-- preparing varr = &&(30, {66}):" ;
	varr = VArray<string>(num_elements * 3, "66") ;
	cout << endl << "\n--                   varr:" << varr ;
	cout << endl << "\n-- preparing va = &&varr:" ;
	va = std::move(varr) ;
	cout << endl << "\n-- va1 = &&varr:" << va ;
	cout << endl << "\n--         varr:" << varr ;
}   // CLose TESTING Block Scope	
#endif

#ifdef TEST2_RESERVE_RESIZE_INITLIST_PUSH_POP_INDEXING_ETC
{	// Open TESTING Block Scope		::   VArray{}, reserve(), resize(), empty(), push_back(), back(), pop(), operators []		
	bool flOK = true ;

	cout << endl << "\n-- PREPARING var{}:" ;
	VArray<string>					varr (VArray<string>{ "5", "6", "7", "8", "9", "10" }) ;
	cout << endl << "\n---- result:" << varr ;

	cout << endl << endl << "-- START:  reserve(60) - ", show_lapse(false) ;
	flOK = varr.reserve(num_elements + 50) ;
	cout << endl << "\n---- result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(+40, {99}) - ", show_lapse(false) ;
	flOK = varr.resize(varr.size() + 40, "99") ;
	cout << endl << "\n---- result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(-20, {77})", show_lapse(false) ;
	flOK = varr.resize(varr.size() - 20, "77") ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(0)", show_lapse(false) ;
	flOK = varr.resize(0) ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  resize(40, {88})", show_lapse(false) ;
	flOK = varr.resize(40, "88") ;
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  pushing (temp=55) (20 + 10) times", show_lapse(false) ;
	for (size_t i = 0 ; i < 30 ; i++) { if (!(flOK = varr.push_back("55"))) { cout << endl << "--- failed push #" << i ; break ; } }
	cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

	cout << endl << endl << "-- START:  pushing (i) until size() < capacity()", show_lapse(false) ;
	for (size_t i = 0 ; varr.size() < varr.capacity() ; i++) {
		if (!(flOK = varr.push_back(std::move(itostr(i))))) 			{
 cout << endl << "--- failed push #" << i ; break ; } }
				cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

				cout << endl << endl << "-- START:  pushing varr.back() into va1", show_lapse(false) ;
				{ // introduce 'va1'
					VArray<string>		va1 ;
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

				cout << endl << endl << endl << "-------------------------------------------------------" ;
				cout << endl << endl << "-- START Step 3:  'varr' -> reserve(50)", show_lapse(false) ;
				flOK = varr.reserve(50) ;
				cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

				cout << endl << endl << "-- START:  pushing (i) until size() < capacity() / 2", show_lapse(false) ;
				for (size_t i = 0 ; varr.size() < (varr.capacity() / 2) ; i++) {
					if (!(flOK = varr.push_back(itostr(i)))) 						{
 cout << endl << "--- failed push #" << i ; break ; } }
							cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

							cout << endl << endl << "-- START:  resize(30, 88)", show_lapse(false) ;
							flOK = varr.resize(30, "88") ;
							cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

							cout << endl << endl << "-- START:  resize(50, 99)", show_lapse(false) ;
							flOK = varr.resize(50, "99") ;
							cout << endl << "\n----  result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

							cout << endl << endl << "-- START:  shrink_to_fit()", show_lapse(false) ;
							varr.shrink_to_fit() ;
							cout << endl << "\n----  result: " << varr ;

							cout << endl << endl << endl << "-------------------------------------------------------" ;
							cout << endl << endl << "-- START Step 4 operators []:", show_lapse(false) ;

							{ // StOS 'va_const'
								cout << endl << endl << "\n-- START: va_const(varr)" ;
								const VArray<string>	va_const(varr) ;
								cout << endl << "\n----  result 'va_const': " << va_const ;

								cout << endl << "\n-- START: testing operator[] on va_const" ;
								for (size_t i = 0 ; i < va_const.size() ; i++) {
									if (i % ElementsPerRow == 0)			cout << endl ;
									cout << std::setw(SpacePerElement) << (va_const[i]) ;  // ERROR: va_const[i] = itostr(i )
									// if (i == 30)	cout << endl << "--- at(30) -->> va_const[size()]: " << va_const[va_const.size()] << endl ;
								}
							} // EOS 'va_const;

							{ // StOS 'va1'
								cout << endl << endl << "\n-- START: va1(varr)" ;
								VArray<string>	va1(varr) ;
								cout << endl << "\n----  result 'va1': " << va1 ;

								cout << endl << "\n-- START: testing operator[] on va1: change va1[i] to (size() - i)" ;
								for (size_t i = 0 ; i < va1.size() ; i++) {
									if (i % ElementsPerRow == 0)			cout << endl ;
									cout << std::setw(SpacePerElement) << (va1[i] = itostr(va1.size() - i)) ;  // OK: va1[i] = i
									// if (i == 30)	cout << endl << "--- at(30) -->> va1[size()]: " << va1[va1.size()] << endl ;
			}
								cout << endl << "\n----  result 'va1': " << va1 ;
							} // EOS 'va1'
						}   // CLose TESTING Block Scope
#endif

#ifdef TEST3_TEST3_ITERATORS_COMPARISONS
		{
		VArray<string>::iterator   it ;
		VArray<string>::const_iterator   cit = it ;
		VArray<string>			varr ;

		cout << endl << endl << "--- initialize varr with [0, 20)" ;
		for (size_t i = 0 ; i < num_elements ; i++)			varr.push_back(itostr(i)) ;
		cout << endl << "--- result is var: " << varr ;

		{ // Comparisons for (it < rho)
			cout << endl << "-- begin() " << (varr.begin() < varr.end() ? "<" : ">=") << " end()" ;
			cout << endl << "-- begin() " << (varr.begin() > varr.end() ? ">" : "<=") << " end()" ;
			cout << endl << "-- begin() " << (varr.begin() >= varr.end() ? ">=" : "<") << " end()" ;
			cout << endl << "-- begin() " << (varr.begin() <= varr.end() ? "<=" : ">") << " end()" ;
		}
		{ // Comparisons for (it = rho)
			cit = it = varr.begin() ;    // ERROR: it = cit ; 
			cout << endl << "-- (it = begin(), so: it " << (cit > varr.begin() ? ">" : "<=") << " begin()" ;
			cout << endl << "--                    it " << (cit >= varr.begin() ? " >= " : "<") << " begin()" ;
			cout << endl << "--                    it " << (cit < varr.begin() ? "<" : ">=") << " begin()" ;
			cout << endl << "--                    it " << (cit <= varr.begin() ? "<=" : ">") << " begin()" ;
		}
		{ // Comparisons for (it > rho)
			cit = it = varr.end() ;
			cout << endl << "-- (it = end() > begin(), so: it " << (cit > varr.begin() ? ">" : "<=") << " begin()" ;
			cout << endl << "--                            it " << (cit >= varr.begin() ? ">=" : "<") << " begin()" ;
			cout << endl << "--                            it " << (cit < varr.begin() ? "<" : ">=") << " begin()" ;
			cout << endl << "--                            it " << (cit <= varr.begin() ? "<=" : ">") << " begin()" ;
		}

		}
#endif

#ifdef TEST3_TEST3_CONST_ITERATOR
		{ // Testing ::iterator and ::const_iterator
			int   i = 0 ;

			cout << endl << "\n-- PREPARING var{1 - 10}:" ;
			VArray<int>		vi (VArray<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }) ;
			VArray<string>  varr ;
			for (auto i : vi) { varr.push_back(itostr(i)) ; }
			cout << endl << "\n---- result- varr<string> is: " << varr ;

			cout << endl << endl << "-- START:  reserve(60) - ", show_lapse(false) ;
			flOK = varr.reserve(num_elements + 40) ;
			cout << endl << "---- result: " << (flOK ? "<OK>> " : "<ERROR>> ") << varr ;

			cout << endl << endl << "-- START:  resize(+40, 99) - ", show_lapse(false) ;
			flOK = varr.resize(varr.size() + 40, "99") ;
			cout << endl << "---- result: " << (flOK ? "<OK> " : "<ERROR> ") << varr ;

			cout << endl << endl << "\n--------------- Tests on ::iterator follow: " ;

			auto it = varr.begin() ; VArray<string>::const_iterator  cit = it ; // it = cit ; does not compile
			for (i = 0, num = 10, it = (varr.begin() + num) ; it != varr.end() ; i++, it += 1) { // ++it) { //it += 1) { // it++)   {   // ++it) {
				(*it = itostr(varr.size() - i)) ;   // *cit = ... does not compile
			}
			cout << endl << "--- end of *() = : setting [begin() + num(10), size) to (size - i). Result in varr: " << varr ;
			cout << endl << "--------- and through ::const_iterator showing varr:" << endl ;
			for (i = 0, cit = varr.begin() ; cit != varr.endlim() ; cit += 1) {  // cit++) { // += 1) {   // ++it) {
				if (i++ % ElementsPerRow == 0)    cout << '\n' ;
				cout << std::setw(SpacePerElement) << (*cit) ;
			}
			cout << endl << "--- and, now -- skip 'not-used 'num(10)'' via (endlim() - 1 - num) and, BACK through --:" ;
			num = 10 ;
			for (i = 0, (cit = varr.endlim() - 1 - num) ; cit >= varr.begin() ; cit -= 1) { //--cit) {   // it -= 1)   {   // --it)   {    // it--) {
				if (i++ % ElementsPerRow == 0)    cout << '\n' ;
				cout << std::setw(SpacePerElement) << (*cit) ;
			}

			cout << endl << "-- show the last num(10) in reverse order" ;
			i = 0 ;
			for (cit = varr.endlim() - (varr.capacity() - varr.size()) - 1 ; cit >= varr.begin() + (varr.size() - num) ; cit--) {
				if (i++ % ElementsPerRow == 0)    cout << '\n' ;
				cout << std::setw(SpacePerElement) << (*cit) ;
			}
			cout << endl << endl << "-------------------- end of iterator tests";
		}
#endif

#ifdef TEST4_SWAP_ERASE_
		{ // swap() and .swap(), erase()
			// OK: std::initializer_list<string> il = { "1st", "2nd", "3d", "4th", "5th", "6th", "7th", "8th", "9th", "10th" } ;
			// OK: VArray<string>		va1 (il) ;        // OK: ({}) ;
			VArray<string>		va1 { "1st", "2nd", "3d", "4th", "5th", "6th", "7th", "8th", "9th", "10th" } ;
			cout << endl << endl << "--- to start with va1 is: " << va1 ;
			{
				VArray<string>		va2(VArray<string>(15, "{}", 20)) ;
				cout << endl << endl << "--- and, va2 is: " << va2 ;
				swap(va1, va2) ;
				cout << endl << endl << "--- after swap(va1,va2) va2 is: " << va2 ;
				cout << endl << endl << "--- and after the swap va1 is: " << va1 ;
				va1.swap(va2) ;
				cout << endl << endl << "--- after va1.swap(va2) va2 is: " << va2 ;
				cout << endl << "----------- va2 is to be destroyed" ;
			}
			cout << endl << endl << "--- and after the swap va1 is: " << va1 ;

			cout << endl << endl << "-- erase(end() - 1)" ;
			flOK = va1.erase(va1.end() - 1) ;
			cout << endl << "--- " << (flOK ? "<OK>" : "<ERROR>") << " and va1 is: " << va1 ;

			cout << endl << endl << "-- erase(begin()+2, begin() + 4)" ;
			flOK = va1.erase(va1.begin() + 2, va1.begin() + 4) ;
			cout << endl << "--- " << (flOK ? "<OK>" : "<ERROR>") << " and va1 is: " << va1 ;

			cout << endl << endl << "-- erase(begin()+2, end())" ;
			flOK = va1.erase(va1.begin() + 2, va1.end()) ; va1.pop_back() ;
			cout << endl << "--- " << (flOK ? "<OK>" : "<ERROR>") << " and va1 is: " << va1 ;

			cout << endl << endl << "-- erase(begin(), end())" ;
			flOK = va1.erase(va1.begin(), va1.end()) ;
			cout << endl << "--- " << (flOK ? "<OK>" : "<ERROR>") << " and va1 is: " << va1 ;
		}
#endif

#ifdef TEST5_INSERT_ERASE
		{ // insert(pos, from, to), insert_at(), insert(pos, &/&&), insert(pos, count, &val) 

			VArray<string>::const_iterator   cit_b ;
			VArray<string>::const_iterator   cit_e ;
			VArray<string>::iterator		 it_b ;
			VArray<string>::iterator		 it_e ;

			std::initializer_list<string> ilist1 = { "1st", "2nd", "3d", "4th", "5th", "6th", "7th", "8th", "9th", "10th" } ;
			std::initializer_list<string> ilist2 = { "-1-", "-2-", "-3-", "-4-", "-5-" } ;

			VArray<string>		va1(ilist1) ;
			cout << endl << endl << "--- to start with va1 is: " << va1 ;
			VArray<string>		va2(ilist2) ;
			cit_b = (it_b = begin(va2)), cit_e = (it_e = end(va2)) ;
			cout << endl << "--- sizeof va1 is " << (end(va1) - begin(va1)) << ", sizeof(va2) is " << (it_e - cit_b) ;

			// Insert at the end() 
			va1.insert(end(va1), cit_b, cit_e) ;         // _at(va1.size(), cit_b, cit_e) ;
			cout << endl << endl << "--- append va2 to va1 -> result in va1 is: " << va1 ;

			// Insert at []
			va1.insert(begin(va1) + 5, cit_b, cit_e) ;   // _at(5, cit_b, cit_e) ;
			cout << endl << endl << "--- insert va2 to va1 at [5]-> result in va1 is: " << va1 ;

			// Increase capacity
			va1.reserve(2 * (va1.capacity())) ;
			va1.insert(begin(va1), cit_b, cit_e) ;       // _at(0, cit_b, cit_e) ;
			cout << endl << endl << "--- capacity of va1 doubled;" << endl
				<< "    insert va2 to va1 at[0]->result in va1 is: " << va1 ;

			cout << endl << endl << "--- Testing insert(s) of values (one at a time) --------" << endl << endl ;

			string str { "=NC" } ; size_t i = va1.capacity() - va1.size() + 5 ;
			for (it_b = end(va1) ; i > 0 ; i--)    it_b = va1.insert(it_b, str) ;
			cout << endl << endl << "--- result of appending(const)): " << va1 ;

			va1.erase(end(va1) - size_t(20), end(va1)), va1.shrink_to_fit(), va1.reserve(2 * (va1.capacity()) - 5) ;
			cout << endl << endl << "--- insertions cleared; capacity doubled - 5. va1 is" << va1 ;

			i = va1.capacity() - va1.size() + 5 ;
			size_t  ni = i ;
			for (it_b = begin(va1) ; i > 0 ; i--)    it_b = va1.insert(it_b, str) ;
			cout << endl << endl << "--- result of inserting (const)): " << va1 ;

			va1.erase(begin(va1), begin(va1) + (ni)), va1.shrink_to_fit(), va1.reserve(2 * (va1.capacity()) - 5) ;
			cout << endl << endl << "--- insertions cleared; capacity doubled - 5. va1 is" << va1 ;

			i = va1.capacity() - va1.size() + 5 ;
			ni = i ;
			for (it_b = begin(va1) + 20 ; i > 0 ; i--)    it_b = va1.insert(it_b, itostr(ni - i)) ;
			cout << endl << endl << "--- result of inserting (move)): " << va1 ;

			va1.erase(begin(va1) + 20, begin(va1) + 20 + (ni)), va1.shrink_to_fit(), va1.reserve(2 * (va1.capacity()) - 5) ;
			cout << endl << endl << "--- insertions cleared; capacity doubled - 5. va1 is" << va1 ;
		}

#endif