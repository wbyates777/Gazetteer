/* Name 20/01/2024

 $$$$$$$$$$$$$$
 $   Name.h   $
 $$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Helper class
 
 Convert names with accents to 'ASCII' names by removing newlines, escaping single 
 quote ['] (note that [‘] is a different character) and mapping, in a transparent manner, 
 'non-ASCII' characters such as:  à => a and á => a. This can be useful for generating search keys.
 
 Also provides methods to split strings by arbitrary delimeter.
 
 Used to process the names in Country and City.

 Example
 
 Name xxx; // must define at least once for setup()
 
 std::string test_str = "M. R. Štəfánik and Č. Ibậñềz amd D. Wąlts and W. Bṙøñel";

 test_str = Name::toASCII(test_str);
 
 std::cout << test_str << std::endl;
 
 std::vector<std::string> res =  Name::split(test_str, std::regex("( a.d )"));
 
 std::cout << res.size() << std::endl;
 for (int i = 0; i < res.size(); ++i)
     std::cout << res[i] << std::endl
 
*/


#ifndef __NAME_H__
#define __NAME_H__


#include <string>
#include <vector>
#include <map>
#include <regex>

class Name
{

public:

    Name( void );
    ~Name( void )=default;

    static std::string
    toASCII( const std::string &str );
    
    static std::vector<std::string>
    split( std::string str, const std::regex &delim, int num = 7 );
    
    // this is faster
    static std::vector<std::string> 
    split( const std::string &str, const std::string &delim, int num = 7 );

    
private:

    void
    setup( void );
    
    static std::string
    remove_accents( std::string str );
    
    static std::map<std::string, std::string> m_match;

};

#endif


