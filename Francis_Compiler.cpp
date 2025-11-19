#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctype.h>
#include <sstream>

using namespace std ;

struct Table {
	string name ;
	bool exist ;
};

struct idTable {
	string id ;
	int sub ;
	int type ;
	int ptr ;
	bool exist ;
};

struct Token {
	string token ;
	int type ;
	int value ;
};

struct Line {
	int num ;
	string origin ;
	vector <Token> tkns ;
};

struct imt {
	Token operate ;
	bool ope1 ;
	Token operand1 ;
	bool ope2 ;
	Token operand2 ;
	bool res ;
	Token result ;
	bool error ;
	string input ;
};

struct lab {
	string label ;
	int scope ;
	int line ;
	int origin ;
};

class Hash {
public:
	void make( Table * & table ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table[i].name = "" ;
			table[i].exist = false ;
		}
	}
	
	int getHash( string name ) {
		int result = 0 ;
		for ( int i = 0 ; i < strlen( name.c_str() ) ; i++ ) {
			int plus = (int)name.c_str()[i] ;
			result = result + plus ;
		}
		
		return result % 100 ;
	}
	
	int getTable( string name, Table * & table ) {
		int value = getHash( name ) ;
		return findOrInsert( value, name, table ) ;
	}
	
	int findOrInsert( int value, string name, Table * & table ) {
		int i = 0 ;
		while ( i < 100 ) {
			if ( table[value].exist == false ) {
				table[value].exist = true ;
				table[value].name = name ;
				return value ;
			}
			else if ( name == table[value].name ) {
				return value ;
			}
			else {
				value = ( value + 1 ) % 100 ;
				i++ ;
			}
		}
		
		return -1 ;
	}
};

class IdHash {
public:
	void make( idTable * & table ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			table[i].id = "" ;
			table[i].sub = -1 ;
			table[i].type = -1 ;
			table[i].ptr = -1 ;
			table[i].exist = false ;
		}
	}
	
	int getHash( string name ) {
		int result = 0 ;
		for ( int i = 0 ; i < strlen( name.c_str() ) ; i++ ) {
			int plus = (int)name.c_str()[i] ;
			result = result + plus ;
		}
		
		return result % 100 ;
	}
	
	int getTable( string name, int scope, idTable * & table, bool special ) {
		int value = getHash( name ) ;
		return findOrInsert( value, name, scope, table, special ) ;
	}
	
	int findOrInsert( int value, string name, int scope, idTable * & table, bool special ) {
		int i = 0 ;
		while ( i < 100 ) {
			if ( table[value].exist == false ) {
				table[value].id = name ;
				if ( !special )
					table[value].sub = scope ;
				table[value].exist = true ;
				return value ;
			}
			else if ( name == table[value].id && ( ( scope != -1 && scope == table[value].sub ) || special ) ) {
				return value ;
			}
			else {
				value = ( value + 1 ) % 100 ;
				i++ ;
			}
		}
		
		return -1 ;
	}
};

class AllTable {
	Hash hash ;
	IdHash hash_id ;
	vector <string> table1 ;
	vector <string> table2 ;	
	Table * table3 ;
	Table * table4 ;
	idTable * table5 ;
public:		
	AllTable() {
		table3 = new Table[100] ;
		table4 = new Table[100] ;
		table5 = new idTable[100] ;
	}
	
	void makeHash() {
		hash.make( table3 );
		hash.make( table4 );
		hash_id.make( table5 );
	}
	
	void MakeTable1() {
		table1.push_back( ";" ) ;
		table1.push_back( "(" ) ;
		table1.push_back( ")" ) ;
		table1.push_back( "=" ) ;
		table1.push_back( "+" ) ;
		table1.push_back( "-" ) ;
		table1.push_back( "*" ) ;
		table1.push_back( "/" ) ;
		table1.push_back( "^" )  ; 
		table1.push_back( "\'" ) ;
		table1.push_back( "\"" ) ;
		table1.push_back( ":" ) ;
		table1.push_back( "," ) ;
	}
	
	void MakeTable2() {
		table2.push_back( "AND" ) ;
		table2.push_back( "BOOLEAN" ) ;
		table2.push_back( "CALL" ) ;
		table2.push_back( "DIMENSION" ) ;
		table2.push_back( "ELSE" ) ;
		table2.push_back( "ENP" ) ;
		table2.push_back( "ENS" ) ;
		table2.push_back( "EQ" ) ;
		table2.push_back( "GE" ) ;
		table2.push_back( "GT" ) ;
		table2.push_back( "GTO" ) ;
		table2.push_back( "IF" ) ;
		table2.push_back( "INPUT" ) ;
		table2.push_back( "INTEGER" ) ;
		table2.push_back( "LABEL" ) ;
		table2.push_back( "LE" ) ;
		table2.push_back( "LT" ) ;
		table2.push_back( "NE" ) ;
		table2.push_back( "OR" ) ;
		table2.push_back( "OUTPUT" ) ;
		table2.push_back( "PROGRAM" ) ;
		table2.push_back( "REAL" ) ;
		table2.push_back( "SUBROUTINE" ) ;
		table2.push_back( "THEN" ) ;
		table2.push_back( "VARIABLE" ) ;
	}
	
	void make() {
		MakeTable1();
		MakeTable2();
		makeHash() ;
	}
	
	int getHashValue( string token, int scope, int table, bool special ) {
		if ( table == 3 )
			return hash.getTable( token, table3 ) ;
		else if ( table == 4 )
			return hash.getTable( token, table4 ) ;
		else if ( table == 5 )
			return hash_id.getTable( token, scope, table5, special ) ;		
	}
	
	int delSize() {
		return table1.size() ;
	}
	
	string delToken( int pos ) {
		return table1.at( pos ) ;
	}
	
	int resSize() {
		return table2.size() ;
	}
	
	string resToken( int pos ) {
		return table2.at( pos ) ;
	}
	
	void setType( string token, int dataType, int scope ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( table5[i].id == token && table5[i].sub == scope ) {
				table5[i].type = dataType ;
				break ;
			} 
		}
		
	}
	int getType( string token, int scope ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( table5[i].id == token && table5[i].sub == scope ) {
				return table5[i].type ;
			} 
		}
		
		return -1 ;
	}
	
	void setPtr( string token, int ptrNum ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( table5[i].id == token && table5[i].sub == -1 ) {
				table5[i].ptr = ptrNum ;
				break ;
			} 
		}
		
	}
	
	void setPtr( string token, int ptrNum, int scope ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( table5[i].id == token && table5[i].sub == scope ) {
				table5[i].ptr = ptrNum ;
				break ;
			} 
		}
		
	}
	
	int getPtr( string token, int scope ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( table5[i].id == token && table5[i].sub == scope ) {
				return table5[i].ptr ;
			} 
		}
		
		return -1 ;
	}
	
	bool isLabel( string token, int scope ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( table5[i].id == token && table5[i].sub == scope ) {
				if ( table5[i].type == 5 )
					return true ;
				else
					return false ;	
			} 
		}
		
		return false ;
	}
	
	void print( int num ) {
		for ( int i = 0 ; i < 100 ; i++ ) {
			cout << i << "	" << table5[i].id << "	" << table5[i].sub << "	" ;
			cout << table5[i].type << "	" << table5[i].ptr << endl ;
		}
	}
	
	void clear() {
		hash.make( table3 ) ;
		hash.make( table4 ) ;
		hash_id.make( table5 ) ;
	}
};

vector <string> input ;

class MakeToken {
	string token ;
	int scope ;
	bool set, call ;
public:	
	MakeToken() {
		token = "" ;
		scope = -1 ;
		set = false ;
		call = false ;
	}

	bool IsWhiteSpace( char ch ) {
		if ( ch ==' ' || ch == '\t' || ch == '\n' )
			return true ;
		return false ;	
	}
	
	bool IsDelimiter( char ch, AllTable table ) {
		for ( int i = 0 ; i < table.delSize() ; i++ ) {
			if ( table.delToken( i ).c_str()[0] == ch )
				return true ;
		}
		return false ;
	}
	
	bool IsUp( char ch ) {
		string up = "¡ô" ;
		if ( ch == up[0] )
			return true ;
		return false ;	
	}
	
	bool IsDelimiter( string token, int & value, AllTable table ) {
		for ( int i = 0 ; i < table.delSize() ;i++ ) {
			if ( token ==  table.delToken( i ) ) {
				value = i + 1 ;
				return true ;
			}
		}
		
		value = 0 ;
		return false ;
	}
	
	bool IsRes( string token, int & value, AllTable table ) {
		for ( int i = 0 ; i < table.resSize() ; i++ ) {
			if ( stricmp( token.c_str(), table.resToken( i ).c_str() ) == 0 ) {
				value = i + 1 ;
				return true ;	
			}
		}
		
		value = 0 ;
		return false ;
	}

	
	bool IsInteger( string token ) {
		for ( int i = 0 ; i < token.length() ; i++ ) {
			if ( token[i] < '0' || token[i] > '9' ) 
				return false ;
		}
	
		return true ;
	}
	
	bool IsReal( string token ) {
		int count = 0 ;
		for ( int i = 0 ; i < token.length() ; i++ ) {
			if ( token[i] < '0' || token[i] > '9' ) {
				if ( token[i] == '.' && count == 0 ) {
					count++ ;
				}
				else
					return false ;
			}
				
		}
	
		return true ;
	}
	
	void getValue( string token, int & type, int & value, AllTable & table ) {
		if ( IsDelimiter( token, value, table ) ) {
			type = 1 ;
		}
		else if ( IsRes( token, value, table ) ) {
			type = 2 ;
		}
		else if ( IsInteger( token ) ) {
			type = 3 ;
			value = table.getHashValue( token, -1, 3, false ) ;		
		}
		else if ( IsReal( token ) ) {
			type = 4 ;
			value = table.getHashValue( token, -1, 4, false ) ;
		}
		else { 
			type = 5 ;
			value = table.getHashValue( token, scope, 5, call ) ;
			call = false ;
		}
		
		if ( set ) {
			scope = value ;
			set = false ;
		}
		
	}

	void addResult( int type, int value, string token, Line & curLine ) {
		Token temp ;
		temp.type = type ;
		temp.value = value ;
		temp.token = token ;
		curLine.tkns.push_back( temp );
		
		if ( token == "PROGRAM" || token == "SUBROUTINE" ) {
			scope = -1 ;
			set = true ;
			call = true ;
		}	
		if ( token == "CALL" )
			call = true ;	
			
	}
	
	void getToken( AllTable & table, string inputLine, vector <Line> & tokens, int pos ) {
		int type, value ;
		Line temp ;
		temp.origin = inputLine ;
		temp.num = pos + 1 ;
		for ( int i = 0 ; i < inputLine.size() ; i++ ) {
			if ( IsWhiteSpace( inputLine[i] ) ) {
				if ( token != "" ) {
					getValue( token, type, value, table ) ;
					addResult( type, value, token, temp ) ;
				}
				token = "" ;
			}
			else if ( IsDelimiter( inputLine[i], table ) ) {
				if ( token != "" ) {
					getValue( token, type, value, table ) ;
					addResult( type, value, token, temp ) ;					
				}
				
				if ( IsUp( inputLine[i] ) ) {
					token = "¡ô" ;	
					i++ ;
				}	
				else {
					token = inputLine[i] ;
				}	
				getValue( token, type, value, table ) ;
				addResult( type, value, token, temp ) ;
				token = "" ;
			}
			else {
				token = token + inputLine[i] ;
			}
		}
		
		if ( !temp.origin.empty() && temp.origin[temp.origin.length()-1] == '\n') {
    		temp.origin.erase(temp.origin.length()-1);
		}
		
		tokens.push_back( temp ) ;
		
	}
	
	int getScope() {
		return scope ;
	}
	
	void print( vector <Line> tokens ) {
		cout << endl ;
		int j = 0 ;
		for ( int i = 0 ; i < tokens.size() ; i++ ) {
			cout << tokens.at( i ).origin << endl ;
			for ( int j = 0 ; j < tokens.at( i ).tkns.size() ; j++ ) 	
				cout << "(" << tokens.at( i ).tkns.at( j ).type << "," << tokens.at( i ).tkns.at( j ).value << ")" ;	
			cout << endl ;	
		}
		
	}
};



class CodeGen {
	vector <imt> table6 ;	// quadruple table
	vector <int> table7 ;	// information table
	vector <lab> table8 ;	// label forward reference table
	int psuedo ;
public:
	CodeGen() {
		psuedo = 0 ;
	}
	
	bool isType( string token ) {
		if ( token == "ARRAY" )
			return true ;
		else if ( token == "BOOLEAN" )	
			return true ;
		else if ( token == "CHARACTER" )
			return true ;
		else if ( token == "INTEGER" )
			return true ;
		else if ( token == "LABEL" )
			return true ;
		else if ( token == "REAL" )
			return true ;
		else
			return false ;			 	
	}
	
	bool checkRes( Line tkn, int pos, AllTable table, int scope, int ifstate, int line ) {
		if ( tkn.tkns[pos].token == "PROGRAM" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+1].type != 5 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+2].token != ";" || pos+2 != tkn.tkns.size()-1 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else
				return true ;
		}
		else if ( tkn.tkns[pos].token == "VARIABLE" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( !isType( tkn.tkns[pos+1].token ) ) {
			    imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+2].token != ":" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else {
				int last_type = tkn.tkns[pos+2].type ;
				for ( int i = pos+3 ; i < tkn.tkns.size() ; i++ ) {		
					if ( last_type == 1 ) {	
						if ( tkn.tkns[i].type == 5 ) {
							if ( table.getType( tkn.tkns[i].token, scope ) != -1 ) {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Dupilcate Defined" ;
								table6.push_back( temp ) ;
								return false ;
							}
							else {
								last_type = 5 ;
							}
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_type == 5 ) {
						if ( tkn.tkns[i].token == "," ) 
							last_type = 1 ;
						else if (tkn.tkns[i].token == ";" && i == tkn.tkns.size()-1 ) 
							break ;
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else {
						imt temp ;
						temp.error = true ;
						temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
						table6.push_back( temp ) ;
						return false ;
					}
						
				}
			}
			
			return true ;
			     
		}
		else if ( tkn.tkns[pos].token == "DIMENSION" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( !isType( tkn.tkns[pos+1].token ) ) {
			    imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+2].token != ":" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else {
				bool inarray = false ;
				int last_type = tkn.tkns[pos+2].type ;
				string last_token = tkn.tkns[pos+2].token ;
				for ( int i = pos+3 ; i < tkn.tkns.size() ; i++ ) {		
					if ( last_token == ":" ) {
						if ( tkn.tkns[i].type != 5 ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else {
							last_type = 5 ;
							last_token = tkn.tkns[i].token ;
						}
							
					}
					else if ( last_type == 5 ) {
						if ( tkn.tkns[i].token != "(" ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else {
							last_type = tkn.tkns[i].type ;
							last_token = "(" ;
							inarray = true ;
						}
							
					}
					else if ( last_token == "(" ) {
						if ( tkn.tkns[i].type != 3 ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
					}
					else if ( last_type == 3 ) {
						if ( tkn.tkns[i].token == "," ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].token == ")" ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == "," ) {
						if ( tkn.tkns[i].type == 3 && inarray ) {
							
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].type == 5 && !inarray ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == ")" ) {
						inarray = false ;
						if ( tkn.tkns[i].token == "," ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if (tkn.tkns[i].token == ";" && i == tkn.tkns.size()-1 ) 
							break ;
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else {
						imt temp ;
						temp.error = true ;
						temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
						table6.push_back( temp ) ;
						return false ;
					}
				}
			}
			
			return true ;
		}
		else if ( tkn.tkns[pos].token == "LABEL" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else {
				int last_type = 1 ;
				for ( int i = pos+1 ; i < tkn.tkns.size() ; i++ ) {
					if ( last_type == 1 ) {
						if ( tkn.tkns[i].type != 5 ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else
							last_type = 5 ;
					}
					else if ( last_type == 5 ) {
						if ( tkn.tkns[i].token == "," ) {
							last_type = tkn.tkns[i].type ;
						}
						else if (tkn.tkns[i].token == ";" && i == tkn.tkns.size()-1 ) 
							break ;
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else {
						imt temp ;
						temp.error = true ;
						temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
						table6.push_back( temp ) ;
						return false ;
					}
				}
			}
			return true ;
		}
		else if ( tkn.tkns[pos].token == "GTO" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( table.getType( tkn.tkns[pos+1].token, scope ) != 5 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Not Defined" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+2].token != ";" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( pos+2 != tkn.tkns.size()-1 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else
				return true ;
		}
		else if ( tkn.tkns[pos].token == "SUBROUTINE" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+1].type != 5 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+2].token != "(" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else {
				int last_type = tkn.tkns[pos+2].type ;
				string last_token = tkn.tkns[pos+2].token ;
				for ( int i = pos+3 ; i < tkn.tkns.size() ; i++ ) {
					if ( last_token == "(" ) {
						if ( !isType( tkn.tkns[i].token ) ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
					}
					else if ( isType( last_token ) ) {
						if ( tkn.tkns[i].token != ":" ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
					}
					else if ( last_token == ":" ) {
						if ( tkn.tkns[i].type != 5 ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
					}
					else if ( last_type == 5 ) {
						if ( tkn.tkns[i].token == "," ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].token == ")" ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == "," ) {
						if ( tkn.tkns[i].token == ")" ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( isType( last_token ) ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].type == 5 ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == ")" ) {
						if ( tkn.tkns[i].token == ";" && i == tkn.tkns.size()-1 )
							break ;
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
				}
			}	
			return true ;
		}
		else if ( tkn.tkns[pos].token == "CALL" ) {
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			if ( tkn.tkns[pos+1].type != 5 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( tkn.tkns[pos+2].token != "(" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else {
				int last_type = tkn.tkns[pos+2].type ;
				string last_token = tkn.tkns[pos+2].token ;
				for ( int i = pos+3 ; i < tkn.tkns.size() ; i++ ) {
					if ( last_token == "(" || last_token == "," ) {
						if ( tkn.tkns[i].type == 5 ) {
							if ( table.getType( tkn.tkns[i].token, scope ) == -1 ) {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " :" + tkn.tkns[i].token +" Not Defined" ;
								table6.push_back( temp ) ;
								return false ;
							}
							else {
								last_type = tkn.tkns[i].type ;
								last_token = tkn.tkns[i].token ;
							}
						}
						else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 4 ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_type == 3 || last_type == 4 || last_type == 5 ) {
						if (tkn.tkns[i].token == "," || tkn.tkns[i].token == ")" ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == ")" ) {
						if ( tkn.tkns[i].token == ";" && i == tkn.tkns.size()-1 )
							break ;
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}	
					}
				}
			}
			
			return true ;
		}
		else if ( tkn.tkns[pos].token == "IF" ) {
			return true ;
			
			if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else {
				int last_type = tkn.tkns[pos].type ;
				string last_token = tkn.tkns[pos].token ;
				int pos_then ;
				for ( int i = pos+1 ; i < tkn.tkns.size() ; i++ ) {
					if ( last_token == "IF" || isCondition( tkn.tkns[i].token ) ) {
						if ( tkn.tkns[i].type == 5 ) {
							if ( table.getType( tkn.tkns[i].token, scope ) == -1 ) {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
								table6.push_back( temp ) ;
								return false ;
							}
							else {
								last_type = tkn.tkns[i].type ;
								last_token = tkn.tkns[i].token ;
							}
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_type == 5 ) {
						if ( isCondition( tkn.tkns[i].token ) ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
						else if ( tkn.tkns[i].token == "THEN" ) {
							pos_then = i ;
							break ;
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
				}
				
				Line substate ;
				for ( int i = pos_then+1 ; i < tkn.tkns.size() ; i++ ) {
					substate.num = tkn.num ;
					substate.origin = tkn.origin ;
					if ( tkn.tkns[i].token == "ELSE" ) {
						;
					}
					else if ( tkn.tkns[i].token == ";" ) {
						break ;
					}
				}
			}
		}
		else if ( tkn.tkns[pos].token == "ENP" ) {
			if ( tkn.tkns[pos+1].token != ";" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( pos+1 != tkn.tkns.size()-1 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else
				return true ;
		}
		else if ( tkn.tkns[pos].token == "ENS" ) {
			if ( tkn.tkns[pos+1].token != ";" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else if ( pos+1 != tkn.tkns.size()-1 ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			}
			else
				return true ;
		}
		return true ;
		
	}
	
	bool checkArray( Line tkn, int pos, AllTable table, int scope, int line, int & end ) {
		int total = table7.at( table.getPtr( tkn.tkns[pos].token, scope )-1+1 );
		int count = 0 ;
		if ( tkn.tkns[pos+1].token != "(" ) {
			imt temp ;
			temp.error = true ;
			temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
			table6.push_back( temp ) ;
			return false ;	
		}
		else {
			int last_type = tkn.tkns[pos+1].type ;
			string last_token = tkn.tkns[pos+1].token ;
			for ( int i = pos+2 ; i < tkn.tkns.size() ; i++ ) {
				if ( last_token == "(" || last_token == "," ) {
					if ( tkn.tkns[i].type == 5 ) {
						if ( table.getType( tkn.tkns[i].token, scope ) == -1 ) {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;	
						}
						else {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
							count++ ;
						}
					}
					else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 4 ) {
						last_type = tkn.tkns[i].type ;
						last_token = tkn.tkns[i].token ;
						count++ ;
					}
				}
				else if ( last_type == 3 || last_type == 4 || last_type == 5 ) {
					if ( tkn.tkns[i].token == "," || tkn.tkns[i].token == ")" ) {
						last_type = tkn.tkns[i].type ;
						last_token = tkn.tkns[i].token ;
					}
					else {
						imt temp ;
						temp.error = true ;
						temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
						table6.push_back( temp ) ;
						return false ;	
					}
				}
				else if ( last_token == ")" ) {
					if ( total == count ) {
						end = i ;
						return true ;
					}
					else {
						imt temp ;
						temp.error = true ;
						temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
						table6.push_back( temp ) ;
						return false ;
					}
				}
			}
			
		}
		
		return true ;
	}
	
	bool isCondition( string token ) {
		if ( token == "AND" )
			return true ;	
		else if ( token == "EQ" )
			return true ;
		else if ( token == "GE" )
			return true ;
		else if ( token == "GT" )
			return true ;
		else if ( token == "LE" )
			return true ;
		else if ( token == "LT" )
			return true ;
		else if ( token == "NE" )
			return true ;
		else if ( token == "NOT" )
			return true ;
		else if ( token == "OR" )
			return true ;			
		else
			return false ;					
	}
	
	bool isOperator( string token ) {
		if ( token == "+" ) 
			return true ;
		else if ( token == "-" ) 
			return true ;	
		else if ( token == "*" ) 
			return true ;
		else if ( token == "/" ) 
			return true ;	
		else if ( token == "^" ) 
			return true ;
		else
			return false ;			
	}
	
	bool checkAssignment( Line tkn, int pos, AllTable table, int scope, int ifstate, int line ) {
		int end, left = 0 ;
		if ( tkn.tkns[tkn.tkns.size()-1].token != ";" ){
			imt temp ;
			temp.error = true ;
			temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
			table6.push_back( temp ) ;
			return false ;
		}
		else if ( tkn.tkns[pos].type != 5 ) {
			imt temp ;
			temp.error = true ;
			temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
			table6.push_back( temp ) ;
			return false ;
		}
		else if ( table.getType( tkn.tkns[pos].token, scope ) == -1 ) {
			imt temp ;
			temp.error = true ;
			temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
			table6.push_back( temp ) ;
			return false ;
		}
		else {
			if ( table.getType( tkn.tkns[pos].token, scope ) == 1 ) {
				if ( checkArray( tkn, pos, table, scope, line, end ) ) ;
				else
					return false ;
				pos = end-1 ;	
			}
			
			
			if ( tkn.tkns[pos+1].token != "=" ) {
				imt temp ;
				temp.error = true ;
				temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
				table6.push_back( temp ) ;
				return false ;
			} 
			else {
				int last_type = tkn.tkns[pos+1].type ;
				string last_token = tkn.tkns[pos+1].token ;
				for ( int i = pos+2 ; i < tkn.tkns.size() ; i++ ) {
					if ( last_token == "=" || isOperator( last_token ) ) {
						if ( tkn.tkns[i].type == 5 ) {
							if ( table.getType( tkn.tkns[i].token, scope ) == 1 ) {
								if ( checkArray( tkn, i, table, scope, line, end ) ) {
									i = end-1 ;
									last_type = 1 ;
									last_token = ")" ;
								}
									
								else {
									return false ;
								}	
							}
							else if ( table.getType( tkn.tkns[i].token, scope ) == -1 ) {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
								table6.push_back( temp ) ;
								return false ;
							}
							else {
								last_type = tkn.tkns[i].type ;
								last_token = tkn.tkns[i].token ;
							}
						}
						else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 4 ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].token == "(" ) {
							left++ ;
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_type == 3 || last_type == 4 || last_type == 5 ) {
						if ( isOperator( tkn.tkns[i].token ) ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].token == ")" ) {
							left-- ;
							if ( left >= 0 ) {
								last_type = tkn.tkns[i].type ;
								last_token = tkn.tkns[i].token ;
							}
							else {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
								table6.push_back( temp ) ;
								return false ;
							}
						}
						else if ( tkn.tkns[i].token == ";" ) {
							if ( i == tkn.tkns.size()-1 )
								break ;
							else {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
								table6.push_back( temp ) ;
								return false ;
							}	
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == "(" ) {
						if ( tkn.tkns[i].type == 5 ) {
							if ( table.getType( tkn.tkns[i].token, scope ) == 1 ) {
								if ( checkArray( tkn, i, table, scope, line, end ) ) {
									i = end-1 ;
									last_type = 1 ;
									last_token = ")" ;
								}
									
								else {
									return false ;
								}	
							}
							else if ( table.getType( tkn.tkns[i].token, scope ) == -1 ) {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
								table6.push_back( temp ) ;
								return false ;
							}
							else {
								last_type = tkn.tkns[i].type ;
								last_token = tkn.tkns[i].token ;
							}
						}
						else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 4 ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else {
							
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else if ( last_token == ")" ) {
						if ( isOperator( tkn.tkns[i].token ) ) {
							last_type = tkn.tkns[i].type ;
							last_token = tkn.tkns[i].token ;
						}
						else if ( tkn.tkns[i].token == ";" ) {
							if ( i == tkn.tkns.size()-1 )
								break ;
							else {
								imt temp ;
								temp.error = true ;
								temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
								table6.push_back( temp ) ;
								return false ;
							}	
						}
						else {
							imt temp ;
							temp.error = true ;
							temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
							table6.push_back( temp ) ;
							return false ;
						}
					}
					else {
						imt temp ;
						temp.error = true ;
						temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
						table6.push_back( temp ) ;
						return false ;
					}
					
				}
			}
		}
		
		if ( left == 0 )
			return true ;
		else {
			imt temp ;
			temp.error = true ;
			temp.input = "Line " + intToStr(line)+ " " + tkn.origin + " " + ":Syntax Error" ;
			table6.push_back( temp ) ;
			return false ;
		}	
	}

	bool grammer( AllTable & table, Line tkn, int scope, int state, int line ) {
		if ( tkn.tkns.size() == 0 )
			return false ;
		else if ( tkn.tkns[0].type == 2 )
			return checkRes( tkn , 0, table, scope, state, line ) ;
		else if ( table.isLabel( tkn.tkns[0].token, scope ) ) {
			if ( tkn.tkns[1].type == 2 ) {
				return checkRes( tkn, 1, table, scope, state, line ) ;
			}
				
			else if ( tkn.tkns[1].type == 5 )
				return checkAssignment( tkn, 1, table, scope, state, line ) ;
		}
		else
			return checkAssignment( tkn, 0, table, scope, state, line ) ;	
	}
	
	
	int findRes( Line tkn, int pos, AllTable table, int scope, int ifstate ) {
		int cnt = 0 ;
		bool process = false, ifgto = false ;
		if ( pos != 0 ) 
			process = true ;
		if ( tkn.tkns[pos].token == "PROGRAM" ) {
			table.setPtr( tkn.tkns[pos+1].token, table6.size()+1 ) ;
		}
		else if ( tkn.tkns[pos].token == "VARIABLE" ) {
			int dataType ;
			if ( tkn.tkns[pos+1].token == "ARRAY" ) 
				dataType = 1 ; 
			else if ( tkn.tkns[pos+1].token == "BOOLEAN" ) 
				dataType = 2 ; 
			else if ( tkn.tkns[pos+1].token == "CHARACTER" ) 
				dataType = 3 ; 
			else if ( tkn.tkns[pos+1].token == "INTEGER" )
				dataType = 4; 
			else if ( tkn.tkns[pos+1].token == "LABEL" )
				dataType = 5; 
			else if ( tkn.tkns[pos+1].token == "REAL" ) 
				dataType = 6;
			
			for ( int i = pos+2 ; i < tkn.tkns.size() ; i++ ) {
				if ( tkn.tkns[i].type == 5 ) {
					table.setType( tkn.tkns[i].token, dataType, scope ) ;
					imt temp ;
					temp.operate = tkn.tkns[i] ;
					temp.ope1 = false ;
					temp.ope2 = false ;
					temp.res = false ;
					temp.error = false ;
					if ( process ) {
						temp.input = tkn.tkns[0].token+ " " + tkn.tkns[i].token ;
						process = false ;
					}
					else
						temp.input = tkn.tkns[i].token ;
					table6.push_back( temp ) ;
					
					if ( ifstate != -1 ){
						cnt++ ;	
					}
				}
			} 
		}
		else if ( tkn.tkns[pos].token == "DIMENSION" ) {
			int arrayType, count ;
			vector <int> num ;
			if ( tkn.tkns[pos+1].token == "ARRAY" ) 
				arrayType = 1 ; 
			else if ( tkn.tkns[pos+1].token == "BOOLEAN" ) 
				arrayType = 2 ; 
			else if ( tkn.tkns[pos+1].token == "CHARACTER" ) 
				arrayType = 3 ; 
			else if ( tkn.tkns[pos+1].token == "INTEGER" )
				arrayType = 4; 
			else if ( tkn.tkns[pos+1].token == "LABEL" )
				arrayType = 5; 
			else if ( tkn.tkns[pos+1].token == "REAL" ) 
				arrayType = 6;
			
			for ( int i = pos+2 ; i < tkn.tkns.size() ; i++ ) {
				if ( tkn.tkns[i].type == 5 ) {
					imt temp ;
					table.setType( tkn.tkns[i].token, 1, scope ) ;
					table.setPtr( tkn.tkns[i].token, table7.size()+1, scope ) ;
					temp.operate = tkn.tkns[i] ;
					temp.ope1 = false ;
					temp.ope2 = false ;
					temp.res = false ;
					temp.error = false ;
					if ( process ) {
						temp.input = tkn.tkns[0].token + + " " + tkn.tkns[i].token ;
						process = false ;
					}
					else
						temp.input = tkn.tkns[i].token ;
					table6.push_back( temp ) ;
					
					if ( ifstate != -1 )
					    cnt++ ;
				}
				else if ( tkn.tkns[i].token == "(" ) {
					count = 0 ;
					num.clear() ;
				}
				else if ( tkn.tkns[i].token == ")" ) {
					table7.push_back( arrayType ) ;
					table7.push_back( count ) ;
					for ( int j = 0 ; j < num.size() ; j++ ) {
						table7.push_back( num.at( j ) ) ;
					}
				}
				else if ( tkn.tkns[i].type == 3 ) {
					num.push_back( atoi( tkn.tkns[i].token.c_str() ) ) ;
					count++ ;
				}
			} 
		}
		else if ( tkn.tkns[pos].token == "LABEL" ) {
			for ( int i = pos+1 ; i < tkn.tkns.size() ; i++ ) {
				if ( tkn.tkns[i].type == 5 ) {
					table.setType( tkn.tkns[i].token, 5, scope ) ;
					imt temp ;
					temp.operate = tkn.tkns[i] ;
					temp.ope1 = false ;
					temp.ope2 = false ;
					temp.res = false ;
					temp.error = false ;
					if ( process ) {
						temp.input = tkn.tkns[0].token + " "  + tkn.tkns[i].token ;
						process = false ;
					}
					else
						temp.input = tkn.tkns[i].token ;
					table6.push_back( temp ) ;
					
					if ( ifstate != -1 )
					    cnt++ ;
				}
			} 
		}
		else if ( tkn.tkns[pos].token == "GTO" ) {
			imt temp ;
			temp.operate = tkn.tkns[pos] ;
			temp.ope1 = false ;
			temp.ope2 = false ;
			temp.res = true ;
			int num = table.getPtr( tkn.tkns[pos+1].token, scope ) ;
			temp.result.token = tkn.tkns[pos+1].token ;
			temp.result.type = 6 ;
			temp.error = false ;
			if ( num != -1 )
				temp.result.value = num ;
			else {
				lab tmp ;
				tmp.label = tkn.tkns[pos+1].token ;
				tmp.scope = scope ;
				tmp.origin = tkn.num ;
				tmp.line = table6.size() + 1 ;
				table8.push_back( tmp ) ;
			}	
				
			if ( process ) {
				temp.input = tkn.tkns[0].token + tkn.tkns[pos].token + " " + tkn.tkns[pos+1].token ;
				process = false ;
			}
			else
				temp.input = tkn.tkns[pos].token + " " + tkn.tkns[pos+1].token ;
			
			
			table6.push_back( temp ) ;
			
			if ( ifstate == 1 ) {
				cnt++ ;
				ifgto = true ;
			}
				
		}
		else if ( tkn.tkns[pos].token == "SUBROUTINE" ) {
			table.setPtr( tkn.tkns[pos+1].token, table6.size()+1 ) ;
			int dataType ;			
			for ( int i = pos+3 ; i < tkn.tkns.size() ; i++ ) {
				if ( tkn.tkns[i].token == "ARRAY" ) 
					dataType = 1 ; 
				else if ( tkn.tkns[i].token == "BOOLEAN" ) 
					dataType = 2 ; 
				else if ( tkn.tkns[i].token == "CHARACTER" ) 
					dataType = 3 ; 
				else if ( tkn.tkns[i].token == "INTEGER" )
					dataType = 4; 
				else if ( tkn.tkns[i].token == "LABEL" )
					dataType = 5; 
				else if ( tkn.tkns[i].token == "REAL" ) 
					dataType = 6;
				if ( tkn.tkns[i].type == 5 ) {
					table.setType( tkn.tkns[i].token, dataType, scope ) ;
					imt temp ;
					temp.operate = tkn.tkns[i] ;
					temp.ope1 = false ;
					temp.ope2 = false ;
					temp.res = false ;
					temp.input = tkn.tkns[i].token ;
					temp.error = false ;
					table6.push_back( temp ) ;
					
					if ( ifstate != -1 )
					    cnt++ ;
				}
			} 
		}
		else if ( tkn.tkns[pos].token == "CALL" ) {
			vector <Token> var ;
			imt temp ;
			int count ;
			temp.operate = tkn.tkns[pos] ;
			temp.ope1 = true ;
			temp.operand1 = tkn.tkns[pos+1] ;
			temp.ope2 = false ;
			temp.res = true ;
			temp.result.token = "" ;
			temp.result.type = 7 ;
			temp.result.value = table7.size() + 1 ;
			temp.error = false ;
			if ( process ) {
				temp.input = tkn.tkns[0].token + tkn.tkns[pos].token + " " +  tkn.tkns[pos+1].token ;
				process = false ;
			}
			else
				temp.input = tkn.tkns[pos].token + " " +  tkn.tkns[pos+1].token ;
			for ( int i = pos+2 ; i < tkn.tkns.size() ; i++ ) {
				if ( tkn.tkns[i].token == "(" ) {
					count = 0 ;
					var.clear() ;
				}
				else if ( tkn.tkns[i].token == ")" ) {
					table7.push_back( count ) ;
					for ( int j = 0 ; j < var.size() ; j++ ) {
						table7.push_back( var.at( j ).type ) ;
						table7.push_back( var.at( j ).value ) ;
					}
				}
				else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 5 ) {
					var.push_back( tkn.tkns[i] ) ;
					count++ ;
				}
				
				if ( tkn.tkns[i].token != ";" )
					temp.input = temp.input + tkn.tkns[i].token ;
			} 
			table6.push_back( temp ) ;
			
			if ( ifstate != -1 )
				cnt++ ;
		}
		else if ( tkn.tkns[pos].token == "IF" ) {
			stack <Token> operand ;
			stack <Token> operate ;
			int current ;
			int i ;
			for ( i = pos+1 ; i < tkn.tkns.size() ; i++ ) {
				if ( tkn.tkns[i].token == "THEN" ) {
					imt temp ;
					temp.operate = operate.top() ;
					operate.pop() ;
					temp.ope2 = true ;
					temp.operand2 = operand.top() ;
					operand.pop() ;
					temp.ope1 = true ;
					temp.operand1 = operand.top() ;
					operand.pop() ;
					temp.res = true ;
					psuedo++ ;
					string num = intToStr( psuedo ) ;
					temp.result.token = "T" + num ;
					temp.result.type = 0 ;
					temp.result.value = psuedo ;
					temp.error = false ;
					temp.input = temp.result.token + " = " + temp.operand1.token + " " ;
					temp.input = temp.input + temp.operate.token + " " + temp.operand2.token ;
					table6.push_back( temp ) ;	
					
					temp.operate.token = "IF" ;
					temp.operate.type = 2 ;
					temp.operate.value = 12 ;
					temp.operand1 = temp.result ;
					temp.operand2.type = 6 ;
					temp.operand2.value = table6.size()+2 ;
					temp.result.type = 6 ;
					temp.result.value = -1 ;
					temp.error = false ;
					temp.input = "IF " + temp.operand1.token + " THEN GO TO " + intToStr( temp.operand2.value ) ;
					table6.push_back( temp ) ;
					current = table6.size()-1 ;
					break ;
				}
				else if ( tkn.tkns[i].type == 5 ) {
					if ( table.getType( tkn.tkns[i].token, scope ) == 1 ) {
						int ptr ;
						int size = arrayDim( tkn.tkns[i].token, table, scope, ptr ) ;
						if ( size == 1 ) {
							if ( operand.empty() ) {
								operand.push( tkn.tkns[i] ) ;
								operand.push( tkn.tkns[i+2] ) ;
							}
							else {
								imt temp ;	
								temp.operate.token = "=" ;
								temp.operate.type = 1 ;
								temp.operate.value = 4 ;
								temp.ope1 = true ;
								temp.operand1 = tkn.tkns[i] ;
								temp.ope2 = true ;
								temp.operand2 = tkn.tkns[i+2] ;
								temp.res = true ;
								psuedo++ ;
								string num = intToStr( psuedo ) ;
								temp.result.token = "T" + num ;
								temp.result.type = 0 ;
								temp.result.value = psuedo ;
								operand.push( temp.result ) ;
								temp.error = false ;
								temp.input = temp.result.token + " " + temp.operate.token + " " + temp.operand1.token ;
								temp.input = temp.input + "(" + temp.operand2.token + ")" ;
								table6.push_back( temp ) ;
							}
						
							i = i + 3 ;
						}
						else {	// size = 2
							imt temp ;
							temp.operate.token = "-" ;
							temp.operate.type = 1 ;
							temp.operate.value = 6 ;
							temp.ope1 = true ;
							temp.operand1 = tkn.tkns[i+4] ;
							temp.ope2 = true ;
							temp.operand2.token = "1" ;
							temp.operand2.type = 3 ;
							temp.operand2.value = table.getHashValue( temp.operand2.token, -1, 3, false ) ;
							temp.res = true ;
							psuedo++ ;
							string num = intToStr( psuedo ) ;
							temp.result.token = "T" + num ;
							temp.result.type = 0 ;
							temp.result.value = psuedo ;
							temp.error = false ;
							temp.input = temp.result.token + " = " + temp.operand1.token ;
							temp.input = temp.input + temp.operate.token + temp.operand2.token ;
							table6.push_back( temp ) ;
								
							temp.operate.token = "*" ;
							temp.operate.type = 1 ;
							temp.operate.value = 7 ;
							temp.operand1 = temp.result ;
							temp.operand2.token = intToStr( table7.at( ptr-1+2 ) ) ;
							temp.operand2.type = 3 ;
							temp.operand2.value = table.getHashValue( temp.operand2.token, -1, 3, false ) ;
							psuedo++ ;
							num = intToStr( psuedo ) ;
							temp.result.token = "T" + num ;
							temp.result.type = 0 ;
							temp.result.value = psuedo ;
							temp.error = false ;
							temp.input = temp.result.token + " = " + temp.operand1.token ;
							temp.input = temp.input + temp.operate.token + temp.operand2.token ;
							table6.push_back( temp ) ;
							
							temp.operate.token = "+" ;
							temp.operate.type = 1 ;
							temp.operate.value = 5 ;
							temp.operand1 = tkn.tkns[i+2] ;
							temp.operand2 = temp.result ;
							psuedo++ ;
							num = intToStr( psuedo ) ;
							temp.result.token = "T" + num ;
							temp.result.type = 0 ;
							temp.result.value = psuedo ;
							temp.error = false ;
							temp.input = temp.result.token + " = " + temp.operand1.token ;
							temp.input = temp.input + temp.operate.token + temp.operand2.token ;	
							table6.push_back( temp ) ;
							
							if ( operand.empty() ) {
								operand.push( tkn.tkns[i] ) ;
								operand.push( temp.result ) ;
							}
							else {
								temp.operate.token = "=" ;
								temp.operate.type = 1 ;
								temp.operate.value = 4 ;
								temp.operand1 = tkn.tkns[i] ;
								temp.operand2 = temp.result ;
								psuedo++ ;
								string num = intToStr( psuedo ) ;
								temp.result.token = "T" + num ;
								temp.result.type = 0 ;
								temp.result.value = psuedo ;
								operand.push( temp.result ) ;
								temp.error = false ;
								temp.input = temp.result.token + " " + temp.operate.token + " " + temp.operand1.token ;
								temp.input = temp.input + "(" + temp.operand2.token + ")" ;
								table6.push_back( temp ) ;
							}
							
							i = i + 5 ;
						}
					}
					else
						operand.push( tkn.tkns[i] ) ;
				}
				else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 4 ) {
					operand.push( tkn.tkns[i] ) ;
				}
				else {
					if ( operate.empty() ) 
						operate.push( tkn.tkns[i] ) ;
					else {
						imt temp ;
						temp.operate = operate.top() ;
						operate.pop() ;
						temp.ope2 = true ;
						temp.operand2 = operand.top() ;
						operand.pop() ;
						temp.ope1 = true ;
						temp.operand1 = operand.top() ;
						operand.pop() ;
						temp.res = true ;
						psuedo++ ;
						string num = intToStr( psuedo ) ;
						temp.result.token = "T" + num ;
						temp.result.type = 0 ;
						temp.result.value = psuedo ;
						operand.push( temp.result ) ;
						temp.error = false ;
						temp.input = temp.result.token + " = " + temp.operand1.token + " " ;
						temp.input = temp.input + temp.operate.token + " " + temp.operand2.token ;
						table6.push_back( temp ) ;
						operate.push( tkn.tkns[i] ) ;
					}	
				}
			
			
			}
		
			Line substate ;
			vector <int> ifcount ;
			for (i = i+1; i < tkn.tkns.size() ; i++ ) {
				substate.num = tkn.num ;
				substate.origin = tkn.origin ;
				if ( tkn.tkns[i].token == "ELSE" ) {
					int count1 = codeGen( table, substate, scope, 1 ) ;
					ifcount.push_back( count1 ) ;
					substate.tkns.clear() ;
				}
				else if ( tkn.tkns[i].token == ";" ) {
					int count2 = codeGen( table, substate, scope, 2 ) ;
					ifcount.push_back( count2 ) ;
					break ;
				}
				else {
					substate.tkns.push_back( tkn.tkns[i] ) ;
				}		
			}
			table6.at( current ).result.value = table6.at( current ).operand2.value + ifcount.at( 0 ) ;
			table6.at( current ).input = table6.at( current ).input + ", ELSE GO TO "  + intToStr( table6.at( current ).result.value ) ;
			
			if ( table6.at( current+ifcount.at( 0 ) ).result.value == -1 && ifcount.size() == 2 ) {
				 table6.at( current+ifcount.at( 0 ) ).result.value = current+ifcount.at( 0 )+ifcount.at( 1 ) ;
				 table6.at( current+ifcount.at( 0 ) ).input = "GO TO" + intToStr( table6.at( current ).result.value ) ;
			}
			
		}
		else if ( tkn.tkns[pos].token == "ENP" ) {
			imt temp ;
			temp.operate = tkn.tkns[pos] ;
			temp.ope1 = false ;
			temp.ope2 = false ;
			temp.res = false ;
			temp.error = false ;
			if ( process ) {
				temp.input = tkn.tkns[0].token + " " + tkn.tkns[pos].token ;
				process = false ;
			}
			else
				temp.input = tkn.tkns[pos].token ;
			table6.push_back( temp ) ;
		}
		else if ( tkn.tkns[pos].token == "ENS" ) {
			imt temp ;
			temp.operate = tkn.tkns[pos] ;
			temp.ope1 = false ;
			temp.ope2 = false ;
			temp.res = false ;
			temp.error = false ;
			if ( process ) {
				temp.input = tkn.tkns[0].token  + " " + tkn.tkns[pos].token ;
				process = false ;
			}
			else
				temp.input = tkn.tkns[pos].token ;
			table6.push_back( temp ) ;
		}
		if ( ifstate == 1 && !ifgto ) {
			imt temp ;
			temp.operate.token = "GTO" ;
			temp.operate.type = 2 ;
			temp.operate.value = 11 ;
			temp.ope1 = false ;
			temp.ope2 = false ;
			temp.res = true ;
			temp.error = false ;
			temp.result.type = 6 ;
			temp.result.value = -1 ;
			table6.push_back( temp ) ;
			cnt++ ;
				
		}
			
		if ( ifstate != -1 ) {
			return cnt ;
		}
		else 
			return -1 ;
	}
	
	void setLabel( string token, AllTable table, int scope ) {
		table.setPtr( token, table6.size()+1, scope ) ;
	}
	
	int arrayDim( string token, AllTable table, int scope, int & ptrNum ) {
		ptrNum = table.getPtr( token, scope ) ;
		return table7.at( ptrNum-1+1 ) ;
	}
	
	bool isBiggerThan( string outside, string inside ) {
		if ( outside == "^" ) {
			return true ;
		}	
		else if ( outside == "*" || outside == "/" ) {
			if ( inside != "^" && inside != "*" && inside != "/" )
				return true ;	
		}
		else if ( outside == "+" || outside == "-" ) {
			if ( inside == "(" || inside == "=" ) {
				return true ;
			}
		}
				
		return false ;
	}
	
	string intToStr( int num ) {
		stringstream ss ;
		string str ;
		ss << num ;
    	ss >> str ;
		return str ;	
	}
	
	int findAssignment( Line tkn, int pos, AllTable table, int scope, int ifstate ) {
		bool process = false ;
		stack <Token> operand ;
		stack <Token> operate ;
		int cnt = 0 ;
		for ( int i = pos ; i < tkn.tkns.size() ; i++ ) {
			if ( tkn.tkns[i].token == ";" )
				break ;
			else if ( tkn.tkns[i].type == 5 ) {
				if ( table.getType( tkn.tkns[i].token, scope ) == 1 ) {
					int ptr ;
					int size = arrayDim( tkn.tkns[i].token, table, scope, ptr ) ;
					if ( size == 1 ) {
						if ( operand.empty() ) {
							operand.push( tkn.tkns[i] ) ;
							operand.push( tkn.tkns[i+2] ) ;
						}
						else {
							imt temp ;
							temp.operate.token = "=" ;
							temp.operate.type = 1 ;
							temp.operate.value = 4 ;
							temp.ope1 = true ;
							temp.operand1 = tkn.tkns[i] ;
							temp.ope2 = true ;
							temp.operand2 = tkn.tkns[i+2] ;
							temp.res = true ;
							psuedo++ ;
							string num = intToStr( psuedo ) ;
							temp.result.token = "T" + num ;
							temp.result.type = 0 ;
							temp.result.value = psuedo ;
							operand.push( temp.result ) ;
							temp.error = false ;
							temp.input = temp.result.token + " " + temp.operate.token + " " + temp.operand1.token ;
							temp.input = temp.input + "(" + temp.operand2.token + ")" ;
							table6.push_back( temp ) ;
							
							if ( ifstate != -1 )
					            cnt++ ;
						}
						
						i = i + 3 ;
					}
					else {	// size = 2
						imt temp ;
						temp.operate.token = "-" ;
						temp.operate.type = 1 ;
						temp.operate.value = 6 ;
						temp.ope1 = true ;
						temp.operand1 = tkn.tkns[i+4] ;
						temp.ope2 = true ;
						temp.operand2.token = "1" ;
						temp.operand2.type = 3 ;
						temp.operand2.value = table.getHashValue( temp.operand2.token, -1, 3, false ) ;
						temp.res = true ;
						psuedo++ ;
						string num = intToStr( psuedo ) ;
						temp.result.token = "T" + num ;
						temp.result.type = 0 ;
						temp.result.value = psuedo ;
						temp.error = false ;
						temp.input = temp.result.token + " = " + temp.operand1.token ;
						temp.input = temp.input + temp.operate.token + temp.operand2.token ;
						table6.push_back( temp ) ;
						if ( ifstate != -1 )
					        cnt++ ;
							
						temp.operate.token = "*" ;
						temp.operate.type = 1 ;
						temp.operate.value = 7 ;
						temp.operand1 = temp.result ;
						temp.operand2.token = intToStr( table7.at( ptr-1+2 ) ) ;
						temp.operand2.type = 3 ;
						temp.operand2.value = table.getHashValue( temp.operand2.token, -1, 3, false ) ;
						psuedo++ ;
						num = intToStr( psuedo ) ;
						temp.result.token = "T" + num ;
						temp.result.type = 0 ;
						temp.result.value = psuedo ;
						temp.error = false ;
						temp.input = temp.result.token + " = " + temp.operand1.token ;
						temp.input = temp.input + temp.operate.token + temp.operand2.token ;
						table6.push_back( temp ) ;
						if ( ifstate != -1 )
					        cnt++ ;
						
						temp.operate.token = "+" ;
						temp.operate.type = 1 ;
						temp.operate.value = 5 ;
						temp.operand1 = tkn.tkns[i+2] ;
						temp.operand2 = temp.result ;
						psuedo++ ;
						num = intToStr( psuedo ) ;
						temp.result.token = "T" + num ;
						temp.result.type = 0 ;
						temp.result.value = psuedo ;
						temp.error = false ;
						temp.input = temp.result.token + " = " + temp.operand1.token ;
						temp.input = temp.input + temp.operate.token + temp.operand2.token ;	
						table6.push_back( temp ) ;
						if ( ifstate != -1 )
					        cnt++ ;
						
						if ( operand.empty() ) {
							operand.push( tkn.tkns[i] ) ;
							operand.push( temp.result ) ;
						}
						else {
							temp.operate.token = "=" ;
							temp.operate.type = 1 ;
							temp.operate.value = 4 ;
							temp.operand1 = tkn.tkns[i] ;
							temp.operand2 = temp.result ;
							psuedo++ ;
							string num = intToStr( psuedo ) ;
							temp.result.token = "T" + num ;
							temp.result.type = 0 ;
							temp.result.value = psuedo ;
							operand.push( temp.result ) ;
							temp.error = false ;
							temp.input = temp.result.token + " " + temp.operate.token + " " + temp.operand1.token ;
							temp.input = temp.input + "(" + temp.operand2.token + ")" ;
							table6.push_back( temp ) ;
							if ( ifstate != -1 )
					            cnt++ ;
						}
						
						i = i + 5 ;
					}
				}
				else
					operand.push( tkn.tkns[i] ) ;
			}
			else if ( tkn.tkns[i].type == 3 || tkn.tkns[i].type == 4 ) {
				operand.push( tkn.tkns[i] ) ;
			}
			else if ( tkn.tkns[i].token == "(" )
				operate.push( tkn.tkns[i] ) ;
			else if ( tkn.tkns[i].token == ")" )  {
				while ( operate.top().token != "(" ) {
					imt temp ;
					temp.operate = operate.top() ;
					operate.pop() ;
					temp.ope2 = true ;
					temp.operand2 = operand.top() ;
					operand.pop() ;
					temp.ope1 = true ;
					temp.operand1 = operand.top() ;
					operand.pop() ;
					temp.res = true ;
					psuedo++ ;
					string num = intToStr( psuedo ) ;
					temp.result.token = "T" + num ;
					temp.result.type = 0 ;
					temp.result.value = psuedo ;
					operand.push( temp.result ) ;
					temp.error = false ;
					temp.input = temp.result.token + " = " + temp.operand1.token ;
					temp.input = temp.input + temp.operate.token + temp.operand2.token ;
					table6.push_back( temp ) ;
					if ( ifstate != -1 )
					    cnt++ ;
				}
				
				operate.pop() ;
			}
			else {
				if ( operate.empty() ) 
					operate.push( tkn.tkns[i] ) ;
				else if ( isBiggerThan( tkn.tkns[i].token, operate.top().token ) )	
					operate.push( tkn.tkns[i] ) ;
				else {
					imt temp ;
					temp.operate = operate.top() ;
					operate.pop() ;
					temp.ope2 = true ;
					temp.operand2 = operand.top() ;
					operand.pop() ;
					temp.ope1 = true ;
					temp.operand1 = operand.top() ;
					operand.pop() ;
					temp.res = true ;
					psuedo++ ;
					string num = intToStr( psuedo ) ;
					temp.result.token = "T" + num ;
					temp.result.type = 0 ;
					temp.result.value = psuedo ;
					operand.push( temp.result ) ;
					temp.error = false ;
					temp.input = temp.result.token + " = " + temp.operand1.token ;
					temp.input = temp.input + temp.operate.token + temp.operand2.token ;
					table6.push_back( temp ) ;
					if ( ifstate != -1 )
					    cnt++ ;
					while ( operate.top().token == "^" ) {
						temp.operate = operate.top() ;
						operate.pop() ;
						temp.operand2 = operand.top() ;
						operand.pop() ;
						temp.operand1 = operand.top() ;
						operand.pop() ;
						psuedo++ ;
						string num = intToStr( psuedo ) ;
						temp.result.token = "T" + num ;
						temp.result.type = 0 ;
						temp.result.value = psuedo ;
						operand.push( temp.result ) ;
						temp.error = false ;
						temp.input = temp.result.token + " = " + temp.operand1.token ;
						temp.input = temp.input + temp.operate.token + temp.operand2.token ;
						table6.push_back( temp ) ;
						if ( ifstate != -1 )
					        cnt++ ;
					}
					operate.push( tkn.tkns[i] ) ;
				}	
			}
			
		}
		
		while ( !operate.empty() ) {
			imt temp ;
			temp.operate = operate.top() ;
			operate.pop() ;
			if ( temp.operate.token == "=" && operand.size() == 2 ) {
				temp.ope1 = true ;
				temp.operand1 = operand.top() ;
				operand.pop() ;
				temp.ope2 = false ;
				temp.res = true ;
				temp.result = operand.top() ;
				operand.pop() ;
				temp.error = false ;
				temp.input = temp.result.token + " " + temp.operate.token + " " + temp.operand1.token ;
				table6.push_back( temp ) ;
				if ( ifstate != -1 )
					cnt++ ;
			}
			else if ( temp.operate.token == "=" && operand.size() == 3 ) {			// type array equal
				temp.ope1 = true ;
				temp.operand1 = operand.top() ;
				operand.pop() ;
				temp.res = true ;
				temp.result = operand.top() ;
				operand.pop() ;
				temp.ope2 = true ;
				temp.operand2 = operand.top() ;
				operand.pop() ;
				temp.error = false ;
				temp.input = temp.operand2.token + "(" + temp.result.token + ") " ;
				temp.input = temp.input + temp.operate.token + " " + temp.operand1.token ;
				table6.push_back( temp ) ;
				if ( ifstate != -1 )
					cnt++ ;
			}
			else {
				temp.ope2 = true ;
				temp.operand2 = operand.top() ;
				operand.pop() ;
				temp.ope1 = true ;
				temp.operand1 = operand.top() ;
				operand.pop() ;
				temp.res = true ;
				psuedo++ ;
				string num = intToStr( psuedo ) ;
				temp.result.token = "T" + num ;
				temp.result.type = 0 ;
				temp.result.value = psuedo ;
				operand.push( temp.result ) ;
				temp.error = false ;
				temp.input = temp.result.token + " = " + temp.operand1.token ;
				temp.input = temp.input + temp.operate.token + temp.operand2.token ;
				table6.push_back( temp ) ;
				if ( ifstate != -1 )
					cnt++ ;
			}
		}
		
		if ( ifstate == 1 ) {
			imt temp ;
			temp.operate.token = "GTO" ;
			temp.operate.type = 2 ;
			temp.operate.value = 11 ;
			temp.ope1 = false ;
			temp.ope2 = false ;
			temp.res = true ;
			temp.error = false ;
			temp.result.type = 6 ;
			temp.result.value = -1 ;
			table6.push_back( temp ) ;
			cnt++ ;
			
		}
		
		if ( ifstate != -1 ) {		
			return cnt ;
		}
		else
			return -1 ;
	}
	
	int codeGen( AllTable & table, Line tkn, int scope, int state ) {
		if ( tkn.tkns[0].type == 2 )
			return findRes( tkn , 0, table, scope, state ) ;
		else if ( table.isLabel( tkn.tkns[0].token, scope ) ) {
			setLabel( tkn.tkns[0].token, table, scope ) ;
			if ( tkn.tkns[1].type == 2 ) 
				return findRes( tkn, 1, table, scope, state ) ;
			else if ( tkn.tkns[1].type == 5 )
				return findAssignment( tkn, 1, table, scope, state ) ;
		}
		else
			return findAssignment( tkn, 0, table, scope, state ) ;	
	}	
	
	void fix( AllTable & table, int num ) {
		
	}
	
	void forward( AllTable & table ) {
		for ( int i = 0 ; i < table8.size() ; i++ ) {
			int ptr = table.getPtr( table8.at( i ).label, table8.at( i ).scope ) ;
			if (  ptr != -1 ) {
				table6.at( table8.at( i ).line-1 ).result.value = ptr ;
			}
			else {
				table6.at( table8.at( i ).line-1 ).error = true ;
				table6.at( table8.at( i ).line-1 ).input = "Line " + intToStr( table8.at( i ).origin ) + ": Syntax Error: Label not find" ;
				//fix( table, table8.at( at( i ).line-1 ) ) ;
			}
		}
	}
	
	void print() {
		int success = 1 ;
		for ( int i = 0 ; i < table6.size() ; i++ ) {
			if ( !table6.at( i ).error ) {
				cout << success << "	((" << table6.at( i ).operate.type << "," << table6.at( i ).operate.value << ")	," ;
				if ( table6.at( i ).ope1 )
					cout << "(" << table6.at( i ).operand1.type << "," << table6.at( i ).operand1.value << ")" ;
				cout << "	," ;
				if ( table6.at( i ).ope2 )
					cout << "(" << table6.at( i ).operand2.type << "," << table6.at( i ).operand2.value << ")" ;
				cout << "	," ;
				if ( table6.at( i ).res )
					cout << "(" << table6.at( i ).result.type << "," << table6.at( i ).result.value << ")" ;
				cout << "	)	"	<< table6.at( i ).input << endl ;
				success++ ;
			}
			else {
				cout << table6.at( i ).input << endl ;
			}
			
		}
		
		cout << endl ;
	}
	
	void out( string fileName ) {
		fstream outFile ;
		outFile.open( fileName.c_str(), fstream::out );
		int success = 1 ; 
		for ( int i = 0 ; i < table6.size() ; i++ ) {
			if ( !table6.at( i ).error ) {
				outFile << success << "	((" << table6.at( i ).operate.type << "," << table6.at( i ).operate.value << ")	," ;
				if ( table6.at( i ).ope1 )
					outFile << "(" << table6.at( i ).operand1.type << "," << table6.at( i ).operand1.value << ")" ;
				outFile << "	," ;
				if ( table6.at( i ).ope2 )
					outFile << "(" << table6.at( i ).operand2.type << "," << table6.at( i ).operand2.value << ")" ;
				outFile << "	," ;
				if ( table6.at( i ).res )
					outFile << "(" << table6.at( i ).result.type << "," << table6.at( i ).result.value << ")" ;
				outFile << "	)	"	<< table6.at( i ).input << endl ;
				success++ ;
			}
			else {
				outFile << table6.at( i ).input << endl ;
			}
			
		}
		
		outFile.close();
		
	}
	
	void printTableS() {
		for ( int i = 0 ; i < table7.size() ; i++ ) {
			cout << i+1 << "	" << table7.at( i ) << endl ;
		}
		
		cout << "==========================" << endl;
	}
	
	void clear() {
		table6.clear() ;
		table7.clear() ;
		table8.clear() ;
	}
	
};

class Compiler {
	MakeToken token ;
	CodeGen code ; 
public:
	void make( AllTable & table ) {
		vector <Line> tokens ;
		int scope ;
		for ( int i = 0 ; i < input.size() ; i++ ) {
			token.getToken( table, input.at( i ), tokens, i ) ;	// lexical analysis
			scope = token.getScope() ;
			if ( code.grammer( table, tokens.at( i ), scope, -1, i+1 ) ) {
				code.codeGen( table, tokens.at( i ), scope, -1 ) ;
			}

		}
		
		//token.print( tokens ) ;
		code.forward( table ) ;
		//table.print( 5 ) ;
		//code.printTableS() ;
		code.print() ;
		
	}
	
	void outFile( string fileName ) {
		code.out( fileName ) ;
	}
	
	void clear() {
		code.clear() ;
	}
};

bool readFile( string fileName ) {
	fstream file ;
	file.open( fileName.c_str(), fstream::in ) ;
	if ( !file.is_open() ) {
		cout << endl << "### The input file is not on the directory! ###" << endl ;
		return false ;
	}
	else {
		string temp ;
		while ( getline( file, temp ) ) {
			temp = temp + '\n' ;
			input.push_back( temp ) ;
		}
		
		file.close();
		return true ;
	}	
}

int main() {
	string command ;
	AllTable table ;
	table.make() ;
	Compiler compiler ;
	do {
		table.clear() ;
		compiler.clear() ;
		input.clear() ;
		string fileName ;
		cout << endl << "**** Francis Compiler ****" ;
		cout << endl << "* 0. Exit                *" ;
		cout << endl << "* 1. Implementation      *" ;
		cout << endl << "**************************" ;
		cout << endl << "Input a command(0, 1): " ;
		cin >> command ;
		if ( command.compare( "1" ) == 0 ) {
			cout << endl << "Input file name: " ;
			cin >> fileName ;
			if ( readFile( fileName ) ) {
				compiler.make( table ) ;
				cout << endl << "Output file name([0]Quit): " ;
				cin >> fileName ;
				if ( fileName != "0" )
					compiler.outFile( fileName ) ;			
			}
			
			table.makeHash() ;			
		}
		else if ( command.compare( "0" ) == 0 )	
			break ;
		else
			cout << endl << "### Command does not exist! ###" << endl ;			
	} while( true ) ;
	system("pause") ;
} 
