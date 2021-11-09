// test_List.cpp: testing, etc for class vlist (in vList.h)

#include <iostream>
#include <string>
#include <iterator>
using std::cout ;
using std::endl ;
using std::string ;
using std::move ;

#include <list>
using std::list ;

#include <sstream>

string
itostr(int i)
{
	std::stringstream	wwss ;
	wwss << i ;
	string	str ;
	wwss >> str ;
	return(str) ;
} // itostr()


#include "vList.h"

vlist<string>
cr_vlist(const std::initializer_list<string> il)
{
	return(vlist<string>(il)) ;
}


vlist <string>::iterator
vls_it_incr(vlist <string>::iterator it, size_t num)
{
	for ( ; num > 0 ; num--, ++it) ;
	return(it) ;
//	return(std::next(it)) ;
}

vlist <string>::iterator
vls_it_decr(vlist <string>::iterator it, size_t num)
{
	for (; num > 0 ; num--, --it) ;
	return(it) ;
//	return(std::prev(it)) ;
}

constexpr bool TESTS_COMPLETED_OK = false ;

int
main()
{
	{
		
		std::initializer_list<string> slist{ "1st", "2nd", "3d", "4th", "5th", "6th", "7th", "8th", "9th", "10th" } ;
		std::initializer_list<string> sl0{ "1st", "2nd", " 3d", "4th", "5th", "6th", "7th" } ;
		std::initializer_list<string> sl1{ "2nd", " 3d", "4th", "5th", "6th", "7th" } ;
		std::initializer_list<string> sl2{ " 3d", "4th", "5th", "6th", "7th", "8th" } ;
		std::initializer_list<string> sl3{ "4th", "5th", "6th", "7th", "8th", "9th" } ;
		std::initializer_list<string> sl4{ "5th", "6th", "7th", "8th", "9th", "10th" } ;

		vlist<string>::iterator it, it1, it2 , it3, it4 ;
		vlist<string>::const_iterator c_it, cit1, cit2, cit3, cit4 ;

		try {
			/* vlist<string> vls(++cbegin(ls), --cend(ls)) ;
			 show_it_traits(begin(vlse)) ; cout << endl ;
			 show_it_traits(cbegin(vlse)) ;
			 cout << "-- [++, --) list:" << endl << vls ;
			*/
			{
				vlist<string> vlse(cr_vlist(slist)) ; // cout << endl << endl << "-- temp before move(): " << endl << vlse ;
				vlist<string> vls(std::move(vlse)) ;
				// cout << endl << endl << "-- temp after  move(): " << endl << vlse ;
				// cout << endl << endl << "--            vls(&&): " << endl << vls ;

/*back\front*/  if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying BACK-FRONT block {}:" << endl ;

					vlist<string>   vl{ "first", "second" } ; // vl ;

					cout << endl << endl << endl						// << (vlist<string>()).front() << (vlist<string>()).back() 
						<< ">> back\\front of vl: {" << vl.front() << "} \\ {" 
						<< vl.back() << '}' ;
					vl.front() = "1st" , vl.back() = "2nd" ;
					cout << endl 
						<< ">> back\\front of vl: {" << vl.front() << "} \\ {"
						<< vl.back() << '}' ;
					auto& s1 = vl.front() ; auto& s2 = vl.back() ;
					s1 = "1st reference", s2 = "2nd reference" ;
					cout << endl 
						<< ">> back\\front of vl: {" << s1 << "} \\ {"
						<< s2 << '}' ;
					const string& cs1 = vl.front() ; const string& cs2 = vl.back() ;
					cout << endl
						<< ">> back\\front of vl: {" << cs1 << "} \\ {"
						<< cs2 << '}' ;
					cout << endl << endl ;

					cout << endl << endl << "-- end BACK-FRONT try {}; destructors follow:" ;
				}

/*emplace*/		if (TESTS_COMPLETED_OK)   {
					cout << endl << endl << ">> trying EMPLACE block {}:" << endl ;

					vlist<string> vl ;
					string s1("s1:1ste."), s2("s2:end"), s3{ "s3:front" }, s4{ "s4:2nd p" } ;

					vl.emplace_front(move(s1)) ,
					vl.emplace_back(move(s2)) ;
					vl.emplace_front(move(s3)) ;
					vl.emplace(++begin(vl), move(s4)) ;

					cout << endl << endl << "-- after emplace into vl: " << endl << vl ;
					cout << endl         << "--        s1, s2, s3, s4: {" << s1 << "}, {" << s2 << "}, {"
						<< s3 << "}, {" << s4 << "}" << endl << endl ;

					cout << endl << endl << "-- end EMPLACE try {}; destructors follow:" ;
				}

/*insert-el*/	if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying INSERT-el & resize() block {}:" << endl ;

					vlist<string> vl ;
					string s1("s1"), s2("s2"), s3{ "s3" }, s4{ "s4" } ;
					vlist<vlist<string>::const_iterator> vl_it ;

					cit1 = it1 = vl.insert(begin(vl), move(s1)) ,  vl_it.push_front(cit1) ; 
					cit2 = it2 = vl.insert(end(vl), move(s2)) ,	vl_it.push_front(cit2) ;
					cit3 = it3 = vl.insert(begin(vl), move(s3)) ,	vl_it.push_back(cit3) ; 
					cit4 = it4 = vl.insert(++begin(vl), move(s4)), vl_it.insert(--(end(vl_it)), cit4) ;

					cout << endl << endl << "-- after insert into vl {s3,s4,s1,s2}: " << endl << vl ;
					cout << endl << "--  and   s1, s2, s3, s4 should be {}: {" << s1 << "}, {" << s2 << "}, {"
						<< s3 << "}, {" << s4 << "}" << endl << endl ;

					cout << endl << "-- output through FRL should be {s2, s1, s4, s3}:" ;
					for (auto& its : vl_it)        cout << '{' << *its << " } ";

					vl.resize(2) , vl.resize(7, "s2") ;
					cout << endl << endl 
						<< "-- after resize(2). resize(7, {s2}) vl should be {s3, s4, s2, s2, s2, s2, s2}:" 
						<< endl << vl ;

					cout << endl << endl << "-- end INSERT-el and resize() try {}; destructors follow:" ;
				}

/*insert-num*/	if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying INSERT-num block {}:" << endl ;

					vlist<string> vl ;
					string s1("s1"), s2("s2"), s3{"s3"}, s4{"s4"} ;
					vlist<vlist<string>::const_iterator> vl_it ;
					constexpr vlist<string>::size_type  elnum = 7 ;

					cit1 = it1 = vl.insert(begin(vl), elnum, (s2)), vl_it.insert(begin(vl_it), cit1) ;
					cit2 = it2 = vl.insert(end(vl), elnum, (s4)), vl_it.insert(begin(vl_it), cit2) ;
					cit3 = it3 = vl.insert(begin(vl), elnum, (s1)), vl_it.insert(end(vl_it),   cit3) ;
					it4 = begin(vl) ; for (size_t i = 0 ; i < elnum*2 ; i++)    ++it4 ;
					cit4 = it4 = vl.insert(it4, elnum, (s3)), vl_it.insert(--(end(vl_it)), cit4) ;

					cout << endl << endl << "-- after insert vl should be {s1x7}{s2x7}{s3x7}{s4x7}: " << endl << vl ;
					cout << endl << "-- where s1, s2, s3, s4 are: {" << s1 << "}, {" << s2 << "}, {"
						<< s3 << "}, {" << s4 << "}" << endl << endl ;

					cout << endl << "-- output(of an element) through FRL should be {s4, s2, s3, s1}: " ;
					for (auto& its : vl_it)        cout << '{' << *its << "} ";
					
					cout << endl << "-- output(of all) through FRLs should be {s4x7}{s2x7}{s3x7}{s1x7}: " ;
					for (auto& its : vl_it) {
						int   i = 0 ; cout << endl ;
						for (auto wit = its ; i < elnum ; i++, ++wit) 	cout << '{' << *wit << "} ";
					}

					cout << endl << endl << "-- end INSERT-num try {}; destructors follow:" ;
				}

/*insert-seq*/	if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying INSERT-seq block {}:" << endl ;

					vlist<string> vl ;
					string s1("s1"), s2("s2"), s3{ "s3" }, s4{ "s4" } ;
					vlist<vlist<string>::const_iterator> vl_it ;
					constexpr vlist<string>::size_type  elnum = 7 ;

					cit1 = it1 = vl.insert(begin(vl), elnum, (s2)), vl_it.insert(begin(vl_it), cit1) ;
					cit2 = it2 = vl.insert(end(vl), elnum, (s4)), vl_it.insert(begin(vl_it), cit2) ;
					cit3 = it3 = vl.insert(begin(vl), elnum, (s1)), vl_it.insert(end(vl_it), cit3) ;
					it4 = begin(vl) ; for (size_t i = 0 ; i < elnum * 2 ; i++)    ++it4 ;
					cit4 = it4 = vl.insert(it4, elnum, (s3)), vl_it.insert(--(end(vl_it)), cit4) ;
					// cout << endl << endl << "-- after insert vl should be {s1x7}{s2x7}{s3x7}{s4x7}: " << endl << vl ;
					
					// insert from vl to vls
					vls.erase(vls_it_incr(begin(vls), elnum), end(vls)) ;		// truncated: size % elnum = 0
					it1 = vls.insert(end(vls),   it1, vls_it_incr(it1, elnum)) ;					// {s2 x elnum at the end
					it2 = vls.insert(begin(vls), it2, vls_it_incr(it2, elnum)) ;					// {s4 x elnum
					it3 = vls.insert(vls_it_incr(begin(vls), elnum), it3, vls_it_incr(it3, elnum)) ;// {s1 x elnum
					it4 = vls.insert(it3, it4, vls_it_incr(it4, elnum)) ;							// {s3 x elnum
					*(begin(vl_it)) = it1 , *(++(begin(vl_it))) = it2 , *(--end(vl_it)) = it3 , *(--(--end(vl_it))) = it4 ;

					cout << endl << endl << "-- after insert vls should be {s4x7}{s3x7}{s1x7}{slist[1,7]}{s2x7}: " << endl << vls ;
					cout << endl << "-- where s1, s2, s3, s4 are: {" << s1 << "}, {" << s2 << "}, {"
						<< s3 << "}, {" << s4 << "}" << endl << endl ;

					cout << endl << "-- output(of an element) through FRL should be {s2, s4, s3, s1}: " ;
					for (auto& its : vl_it)        cout << '{' << *its << "} ";

					cout << endl << "-- output(of all) through FRLs should be {s2x7}{s4x7}{s3x7}{s1x7}: " ;
					for (auto& its : vl_it) {
						int   i = 0 ; cout << endl ;
						for (auto wit = its ; i < elnum ; i++, ++wit) 	cout << '{' << *wit << "} ";
					}

					cout << endl << endl << "-- end INSERT-seq try {}; destructors follow:" ;
				}

/*insert-ilist*/if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying INSERT-inilist block {}:" << endl ;
					
					vlist<string> vl ;
					string s1("s1"), s2("s2"), s3{ "s3" }, s4{ "s4" } ;
					vlist<vlist<string>::const_iterator> vl_it ;
					constexpr vlist<string>::size_type  elnum = 7 ;

					vl.push_front(s3) , vl.push_front(s1) , vl.push_back(s4) , vl.insert(++begin(vl), s2) ;
					it = vl.insert(begin(vl), sl0) ;
					cout << endl << endl << "-- after insertions vl should be {sl0}{s1}{s2}{s3}{s4}: " << endl << endl << vl ;

					it1 = --(vl.insert(vls_it_incr(it, sl0.size() + 1), sl1)) ,		// to s1 
					it4 = --(vl.insert(end(vl), sl4)) ;								// to s4
					it2 = --(vl.insert(vls_it_incr(it1, elnum + 1), sl2)) ;				// to s2
					it3 = --(vl.insert(vls_it_incr(it2, elnum + 1), sl3)) ;				// to s3
					vl_it.push_front(it1) , vl_it.push_front(it2) , vl_it.push_front(it3), vl_it.push_front(it4) ,
					vl_it.push_back(it) ;

					cout << endl << endl << "-- after insertions vl should be {sl0}{s1,sl1}{s2,sl2}{s3,sl3}{s4,sl4}: " << endl << vl ;
					cout << endl << "-- where s1, s2, s3, s4 are: {" << s1 << "}, {" << s2 << "}, {"
						<< s3 << "}, {" << s4 << "}" << endl << endl ;

					cout << endl << "-- output(of an element) through FRL should be {s4, s3, s2, s1, 1st}: " ;
					for (auto& its : vl_it)        cout << '{' << *its << "} ";

					cout << endl << "-- output(of all) through FRLs should be {s4,sl4}{s3,sl3}{s2,sl3}{s1,sl1}{sl0}: " ;
					for (auto& its : vl_it) {
						int   i = 0 ; cout << endl ;
						for (auto wit = its ; i < elnum ; i++, ++wit) 	cout << '{' << *wit << "} ";
					}

					cout << endl << endl << "-- end INSERT-inilist try {}; destructors follow:" ;
				}

/*push-pop-*/	if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying PUSH-POP block {}:" << endl ;

					vlist<string> vl ;
					string s1("s1:1ste."), s2("s2:end"), s3{ "s3:front" }, s4{ "s4:2nd p" } ;

					vl.push_back((s3)), vl.push_front(move(s2)) ;
					vl.push_back(move(s4)), vl.push_front(move(s1)) ;
					cout << endl << endl << "-- after 4 push'es to empty: " << endl << vl ;
					cout << endl << "-- s1, s2, s3, s4: {" << s1 << "}, {" << s2 << "}, {"
							<< s3 << "}, {" << s4 << "}" << endl << endl ;

					vl.pop_back(), vl.pop_front() ;
					cout << endl << endl << "-- after 2 pop's: " << endl << vl ;
					vl.push_back(s4) ;
				    int i = 0 ;
					while (vl.size() > 1)     vl.pop_back() , vl.pop_front() , i++ ;
					cout << endl << endl << "-- after pop's loop(" << i << " t): " << endl << vl ;

					cout << endl << endl << "-- end PUSH-POP try {}; destructors follow:" ;
				}

/*iterartors*/	if (TESTS_COMPLETED_OK) {
					cout << endl << endl << ">> trying ITERATORS block {}:" << endl ;

					vlist<string> vl{ "1st", "2nd", "3d", "4th", "5th", "6th", "7th", "8th", "9th", "10th" } ;
					cout << endl << endl << "-- vl{}: " << endl << vl ;
					c_it = cbegin(vl) , *c_it ; cout << endl << "-- first of vl is : " << *c_it ; // = "GEPPI" ;
					it = begin(vl) , *(++it) = "GEPPI" ;
					cout << endl << endl << "-- vl: " << endl << vl ;

					cout << endl << endl << "-- end ITERATORS try {}; destructors follow:" ;
				}

/*assign*/		if (TESTS_COMPLETED_OK) { // testing assign
					cout << endl << endl << ">> trying ASSIGN block {}:" << endl ;

					vlist<string> vlse(cr_vlist(slist)) ;
					vlist<string> vls(vlse) ; // std::move(vlse)) ;
					vlse.assign(5, "GEPPI"), vls.assign(++begin(vls), --end(vls)) ;
					cout << endl << endl << ">>         vlse: " << endl << vlse ;
					cout << endl << endl << ">>         vlse: " << endl << vls ;
					vlse.assign(slist) ;
					cout << endl << endl << ">>     vlse(il): " << endl << vlse ;

					cout << endl << endl << "-- end ASSIGN try {}; destructors follow:" ;
				}

/* = 7,&& */	if (TESTS_COMPLETED_OK) { // testing operator =&/&&
					cout << endl << endl << ">> trying = &/&& block {}:" << endl ;

					vlist<string> vlse(cr_vlist(slist)) ;
					vlist<string> vls(vlse) ; // std::move(vlse)) ;
					vlse = /*std::forward<vlist<string>>*/ (vlist<string>{"first", "second"}) ;
					cout << endl << endl << ">> vlse = &&  -> vlse:" << endl << vlse ;
					cout << endl << endl << ">> vlse = vls -> vlse:" << endl << (vlse = vls) ;
					cout << endl << endl << ">>               vls:" << endl << vls ;

					cout << endl << endl << "-- end = &/&& try {}; destructors follow:" ;
				}

/* swap */		if (TESTS_COMPLETED_OK) {	// Testing swap()
					cout << endl << endl << ">> trying SWAP block {}:" << endl ;

					// swapping with an empty one
					swap(vlse, vls) , swap(vlse, vls) ;
					cout << endl << endl << "-- after swap(), swap()  temp: " << endl << vlse ;
					cout << endl << endl << "--                        vls: " << endl << vls ;
					// swapping two not empty
					vlist<string>	vls2{"first", "second"} ;
					swap(vls, vls2) ;
					cout << endl << endl << "-- after swap()  vls: " << endl << vls ;
					cout << endl << endl << "--              vls2: " << endl << vls2 ;
					swap(vls, vls2) ;
					cout << endl << endl << "-- after 2nd swap()  vls: " << endl << vls ;
					cout << endl << endl << "--                  vls2: " << endl << vls2 ;

					cout << endl << endl << "-- end SWAP try {}; destructors follow:" ;
				}

/* erase */		if (TESTS_COMPLETED_OK) {	// Testing .erase(): 
					cout << endl << endl << ">> trying ERASE block {}:" << endl ;

					vlist<string> vlse(cr_vlist(slist)) ;
					vlist<string> vls(vlse) ; // std::move(vlse)) ;

					c_it = (--end(vls)) ;  auto it1 = begin(vls) ;
					it = vls.erase((vls.erase(++begin(vls), --(--end(vls))))) ;
					cout << endl << endl << "-- list after erase(erase(++beg, --(--end)): " << endl << vls ;
					cout << endl << endl << "--	returned it points to: " ;
					for (; c_it != end(vls) ; ++c_it)		cout << *c_it << ", " ;
					cout << endl << "--	the set before erase points to: " ;
					for (; it != end(vls) ; ++it)			cout << *it << ", "; *(--it) = "GEPPI" ;

					cout << endl << "--	the set before erase points to: " ;
					for (; it1 != end(vls) ; ++it1)			cout << *it1 << ", ";

					vls.erase(begin(vls), end(vls)) ;
					cout << endl << endl << "-- completely erased: " << endl << vls ;

					cout << endl << endl << "-- end ERASE try {}; destructors follow:" ;
				}

/*remove[_if]*/	if (TESTS_COMPLETED_OK) {	// Testing remove[_if]()
					cout << endl << endl << ">> trying REMOVE[_IF] block {}:" << endl ;

					vlist<string>::size_type   count = 0 ;
					vlist<string> vls ;

					vls.insert(begin(vls), sl0), vls.insert(end(vls), sl0), 
					vls.insert(vls_it_incr(begin(vls), sl0.size()), sl0),
					vls.insert(begin(vls), sl0), vls.insert(begin(vls), sl0) ; 
					// cout << endl << endl << "-- vls has to be {sl0} x 5 times:" << endl << vls ;    // OK
					
					cout << endl << ">> attempt to remove({5}) should yield 0 -> result: " << (count = vls.remove(string("5"))) ;
					cout << endl << ">> ...     vls has to be intact -> {sl0} x 5 times:" << endl << vls ;

					cout << endl << ">> attempt to remove({5th}) should yield 5 -> result: " << (count = vls.remove(string("5th"))) ;	// OK

					it = vls_it_incr(begin(vls), 4) , vls.insert(it, "r1_5th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r2_5th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r3_5th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r4_5th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r5_5th") ;
					cout << endl << ">> after REMOVEs and INSERTs vls has to be {sl0: 5th->r?_5th} x 5 times:" << endl << vls ;		// OK

					cout << endl << endl << ">> attempt to remove_if(> {7}) should yield 5 -> result: " 
						<< vls.remove_if([](string str)->bool{return(str > "7" && str[0] == '7') ; }) ;								// OK

					it = vls_it_incr(begin(vls), 6), vls.insert(it, "r1_7th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r2_7th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r3_7th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r4_7th") ;
					it = vls_it_incr(it, sl0.size() - 1), vls.insert(it, "r5_7th") ;
					cout << endl << ">> after remove_if(> {7}) vls has to be {sl0: 7th->r?_7th} x 5 times:" << endl << vls ;		// OK

					cout << endl << endl << ">> attempt to remove_if(!=  {r...}) should yield 25 -> result: "
						<< vls.remove_if([](const string& str)->bool {return(str[0] != 'r') ; }) ;							
					cout << endl << endl << ">> after remove_if(< {r}) vls has to be seq {r?_(5/7)th} of size 10" << endl << vls ;

					cout << endl << endl << ">> attempt to remove_if(true) should yield 10 -> result: "
						<< vls.remove_if([](const string& str)->bool {return(true) ; }) ;
					vls.insert(begin(vls), slist) ;
					cout << endl << endl << ">> after remove_if(true) and insert slist vls should be {slist} of size 10" << endl << vls ;

					cout << endl << endl << "-- end REMOVE[_IF] try {}; destructors follow:" ;
				}

/*splice,++*/	if (TESTS_COMPLETED_OK) {	// Testing splice/sort/unique
/* sort */			cout << endl << endl << ">> trying SPLICE/SORT/UNIQUE block {}:" << endl ;
/* unique */
					vlist<string> vls ;
					vlist<string> vlt ;
					size_t			n = 0 ;

					vls.insert(begin(vls), sl0), vls.insert(end(vls), sl0),
					vls.insert(vls_it_incr(begin(vls), sl0.size()), sl0),
					vls.insert(begin(vls), sl0), vls.insert(begin(vls), sl0) ;
					cout << endl << ">> testing .insert(ilist)->_ins_vl()->_move_vl_seq():" ;
					cout << endl << "-- vls has to be {sl0} x 5 times:" << endl << vls ;    // OK
					
					cout << endl << endl << ">> testing splice(source list):" ;
					vlt.splice(begin(vlt), (vls)) ;
					cout << endl << endl << "-- after splice(vls): vls must be empty, vlt- as vls before -> result" ;
					cout << endl << endl << "-- showing vls: " << vls ; 
					cout << endl << endl << "-- showing vlt: " << vlt ; n = vlt.size() ;

					cout << endl << endl << ">> testing splice(source list, first, last):" ;

					vls.insert(begin(vls), sl1), vls.insert(begin(vls), "1st") ;
					cout << endl << endl << "---- update vls - must look like sl0 -> result(vls): " << vls ;

					vlt.splice(end(vlt), vls, ++(begin(vls)), end(vls)) ;
					cout << endl << endl << "-- after splice(vls, b+1, end()): vls must hold only {1st}-> result: " << vls ;
					cout << endl << endl << "-- vlt must have appended (like) sl1(2-7)-> result: " << vlt ;

					vlt.splice(vls_it_incr(begin(vlt), n), vls, begin(vls)) ;
					cout << endl << endl << "-- after last splice(vlt b + 35, vls, b, end()): vls must be empty-> result: " << vls ;
					cout << endl << endl << "-- vlt must look like {sl0} x 6 times-> result: " << vlt ;

					for (it = begin(vlt), n = 0 ; n < sl0.size() ; n++, ++it)   *it = ("x_" + *it) ;
					cout << endl << endl << "-- updated vlt is-> result: " << vlt ;
					it = begin(vlt) ;
					vlt.splice(end(vlt), vlt, it, vls_it_incr(it, sl0.size())) ;
					cout << endl << endl << "-- 1st line of vlt moved to the end through .splice() -> result: " << vlt ;
					
					it = vls_it_incr(begin(vlt), sl0.size()) , it2 = vls_it_decr(end(vlt), sl0.size()) ;
					vlt.splice(it, vlt, it2, end(vlt)) ;
					cout << endl << endl << "-- last line of vlt moved into 2nd line through .splice() -> result: " << vlt ;

					vlt.sort() ; // ([](string s1, string s2)->bool { return(s1 > s2) ; }) ;
					cout << endl << endl << "-- vlt must be sorted in ascending order of strings -> result: " << vlt ;
					vlt.sort([](string s1, string s2)->bool { return(s1 > s2) ; }) ;
					cout << endl << endl << "-- vlt must be sorted in descending order of strings -> result: " << vlt ;

					vlt.unique() ;
					cout << endl << endl << "-- vlt after unique() -> result: " << vlt ;

					cout << endl << endl << "-- end SPLICE/SORT/UNIQUE try {}; destructors follow:"  ;
				}

/*merge*/		if (true) {	// Testing merge, etc
					cout << endl << endl << ">> trying MERGE block {}:" << endl ;

					vlist<string> vls ;
					vlist<string> vlt ;
					size_t			n = 0 ;

					vls.insert(begin(vls), sl0), vls.insert(end(vls), sl0),
					vls.insert(vls_it_incr(begin(vls), sl0.size()), sl0),
					cout << endl << ">> to start with:" << endl <<  "-- vlt must be empty -> result: " << "checked OK --" ; // vlt ;
					cout << endl << "-- vls has to be {sl0} x 3 times --> result: " << vls ;    // OK

					cout << endl << endl << ">> attempting vlt.merge(vls): must swap list: ----" ;
					vlt.merge(vls) ;
					cout << endl << "-- vlt must be {sl0} x 3 times -> result: " << "checked OK --" ; // vlt ;
					 cout << endl << "-- vls has to be empty --> result: " << "checked OK --" ; // vls ;

					vls.insert(begin(vls), sl0) ;
					cout << endl << ">> vls.insert(sl0) -> result: " << "checked OK --" ; // vls ;
					vlt.sort() ;
					cout << endl << ">> vlt.sort() -> result: " << "checked OK --" ; // vlt ;

					cout << endl << endl << ">> attempting prefixing with [t/s] all vlt/vls nodes and sort them:" ;
					for (it = begin(vlt), n = 0 ; n < vlt.size() ; n++, ++it) {
						if ((*it)[0] == ' ') (*it)[0] = '_' ; *it = "t_" + *it ;
					}
					for (it = begin(vls), n = 0 ; n < vls.size() ; n++, ++it) {
						if ((*it)[0] == ' ') (*it)[0] = '_' ; *it = "s_" + *it ;
					}
					vls.sort() , vlt.sort() ;
					cout << endl << "-- result vlt: " << "checked OK --" ; // vlt ;
					cout << endl << "-- result vls: " << "checked OK --" ; // vls ;

					cout << endl << endl << ">> attempting vlt.merge(vls), as 's' < 't': vls must be sliced in begin(vlt): " ; vlt.merge(vls) ;
					cout << endl << "-- vlt.merge(vls) -> result vlt: " << "checked OK --" ; // vlt ;
					cout << endl << "--                   result vls: " << "checked OK --" ; // vls ;

					vls.splice(begin(vls), vlt, begin(vlt), vls_it_incr(begin(vlt), sl0.size())) ;
					cout << endl << endl << ">> restoring vls trough vls.splice(begin(vlt)):" ;
					cout << endl << "-- vls.splice(vlt) ->result vlt: " << "checked OK --" ; // vlt ;
					cout << endl << "--                   result vls: " << "checked OK --" ; // vls ;

					cout << endl << endl << ">> attempting vls.merge(vlt), as 's' < 't': vls must be sliced into end(vls): " ; vls.merge(vlt) ;
					cout << endl << "-- vls.merge(vlt) -> result vlt: " << "checked OK --" ; // vlt ;
					cout << endl << "--                   result vls: " << "checked OK --" ; // vls ;

					vlt.splice(end(vlt), vls, vls_it_incr(begin(vls), sl0.size()), end(vls)) ;
					cout << endl << endl << ">> restoring vlt trough vlt.splice(begin(vlt) + sl0):" ;
					cout << endl << "-- result vlt: " << "checked OK --" ; // vlt ;
					cout << endl << "-- result vls: " << "checked OK --" ; // vls ;

					cout << endl << endl << ">> changing prefixes with [#t/s] for all vlt/vls nodes and sort them:" ;
					for (it = begin(vlt), n = 0 ; n < vlt.size() ; n++, ++it) {
						(*it)[1] = 't' , (*it)[0] = (*it)[2] ;
					}
					for (it = begin(vls), n = 0 ; n < vls.size() ; n++, ++it) {
						(*it)[1] = 's', (*it)[0] = (*it)[2] ;
					}
					vls.sort() , vlt.sort() ;
					cout << endl << "-- result vlt: " << "checked OK --" ; // vlt ;
					cout << endl << "-- result vls: " << "checked OK --" ; // vls ;

					vlist<string>	vls_save , vlt_save ;
					vls_save = vls , vlt_save = vlt ;

					cout << endl << endl << ">> attempting vls.merge(vls): " ; vls.merge(vlt) ;
					cout << endl << "-- vlt.merge(vls) -> result vlt: " << vlt ;
					cout << endl << "--                   result vls: " << vls ;
					
					cout << endl << endl << ">> attempting vlt_save.merge(vls_save): " ; vlt_save.merge(vls_save) ;
					cout << endl << "-- vlt.merge(vls) -> result vlt_save: " << vlt_save ;
					cout << endl << "--                   result vls_save: " << vls_save ;

					cout << endl << endl << "-- vls_save and vlt must check EQUAL - result: "
						<< std::boolalpha << (vls_save == vlt) ;
					cout << endl << endl << "-- vls and vlt_save must check EQUAL - result: " 
						<< std::boolalpha << (vls == vlt_save) ;

					cout << endl << endl << "-- end MERGE try {}; destructors follow:" ;
				}

				cout << endl << endl << "___ end block{}:------" << endl ;
			}
		
		} catch (std::exception& e) {
			cerr << endl << endl
				<< endl << "___ Caught: " << e.what()
				<< endl << "___ Type  : " << typeid(e).name() << endl ;
		}
	} 

	cout << endl << endl << endl << "--- That's it ..." << endl ;
	return(0) ;
} // main()

// eof test_List.cpp