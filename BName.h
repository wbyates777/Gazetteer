/* BName 20/01/2024

 $$$$$$$$$$$$$$$
 $   BName.h   $
 $$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Convert names with accents to 'ASCII' names by removing newlines, escaping single 
 quote ['] (note that [‘] is a different character) and mapping, in a transparent manner, 
 'non-ASCII' characters such as: 
                                  à => a and á => a 

 This can be useful when generating search keys.

 Character literals

   char c0     =   'A'; 
   char c1     = u8'A'; 
   wchar_t c2  =  L'A'; 
   char16_t c3 =  u'A'; 
   char32_t c4 =  U'A'; 
 
 Example
 
 std::string test_str = "0ə2345ṟṙř Tềst12ø";
 std::cout << BName::clean(test_str) << std::endl;
 
*/


#ifndef __BNAME_H__
#define __BNAME_H__



#include <string>
#include <map>


class BName
{

public:

    BName( void );
    ~BName( void )=default;

    static std::string
    clean( const std:: string& str );
    
private:

    void
    setup( void );
       
    static std::string
    remove_accents( std::string str );
    
    static std::map<std::string, std::string> m_match;
};

#endif


