/* Name 20/01/2024

 $$$$$$$$$$$$$$
 $   Name.h   $
 $$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Helper class used to process the names in MarketId, Currency, Country and City.
 
 Provides methods for removing newlines, escaping single character apostraphe ['] (but not [`]) 
 and mappimg characters with diacritic signs to ASCII characters i.e. [à] => [a] and [á] => [a]. 
 Also provides methods to remove leading and trailing white space, quotes, or brackets, 
 and to split strings by an arbitrary delimeter. 
 
 Countries with alphabets that employ diacritic signs include:
 AT, BO, BR, CH, CL, CR, DE, DK, FI, FO, FR, HU, IS, KR, MX, NO, PA, PE, PT, SE, SJ, TR and VN. 
 
 See
 
 https://en.wikipedia.org/wiki/Romanization
 
 https://www.regular-expressions.info
 https://www.regexlib.com/Default.aspx
 
 https://www.fileformat.info/info/charset/UTF-16/list.htm
 https://www.codetable.net/unicodecharacters
 
 
 Name xxx; // must define at least once for setup()
 
 
 Example 1
 
 std::string names = "M. È. Štəfánik and Č. Ibậñềz amd Đ. Wąltóṙs and W. Bṙøñe";
 
 std::cout << names << std::endl;
 names = Name::deaccent(names);
 std::cout << names << std::endl;
 
 std::vector<std::string> res =  Name::split(names, std::regex("( a.d )"));
 
 std::cout << res.size() << std::endl;
 for (int i = 0; i < res.size(); ++i)
     std::cout << '[' << res[i] << ']' << std::endl;
 
 
 Example 2
 
 std::string test2 = " <g \"Côte d'Ivoire\"  />    ";
 std::cout << "[" <<  test2 << "]" << std::endl;
 std::cout << "[" <<  Name::deaccent(Name::unquote(Name::clip(test2, "<g", "/>"))) << "]" << std::endl;
 
 
 Example 3
 
 std::string test3 = " \"Côte d'Ivoire\"  ";
 std::string test4 = " \'Côte d'Ivoire\'  ";
 std::string test5 = " 'Côte d'Ivoire'  ";
 std::string test6 = " $$Côte d'Ivoire^^  ";
 
 std::cout << "[" << test3 << "] --> [" <<  Name::clip(test3 ,"\"")        << "]" << std::endl;
 std::cout << "[" << test4 << "] --> [" <<  Name::clip(test4, "'")         << "]" << std::endl;
 std::cout << "[" << test5 << "] --> [" <<  Name::clip(test5, "\'")        << "]" << std::endl;
 std::cout << "[" << test6 << "] --> [" <<  Name::clip(test6, "$$", "^^")  << "]" << std::endl;

*/


#ifndef __NAME_H__
#define __NAME_H__


#include <map>
#include <string>
#include <vector>
#include <regex>


class Name
{

public:

    Name( void ) { setup(); }
    ~Name( void )=default;

    // replace accented characters such as [à] with a Roman or ASCII equivalent [a]
    static std::string
    deaccent( std::string str );
    
    // return false if str contains non-ASCII characters
    static bool
    isroman( const std::string &str ); 
    
    // remove newline [\n] and carriage return [\r] also escape single quote ['] -> [\']
    static std::string
    denewln( std::string str );

    // convert the first character in each word to upper case
    static std::string 
    capitalise( const std::string &str );
    
    static std::string 
    toUpper( std::string str ) 
    { 
        std::transform(str.begin(), str.end(), str.begin(), [](char c){ return std::toupper(c); });
        return str;
    }

    static std::string 
    toLower( std::string str ) 
    {
        std::transform(str.begin(), str.end(), str.begin(), [](char c){ return std::tolower(c); });
        return str;
    }
    
    
    // split the string by delimiter 
    static std::vector<std::string> 
    split( const std::string &str, const std::string &delim );
    
    static std::vector<std::string>
    split( std::string str, const std::regex &delim );

    
    // removes any trailing string that equals the value of sym (typically newline "\n")
    // when sym = "" remove all trailing newlines - returns number of symbols removed
    static int
    chomp( std::string &str, const std::string &sym = "\n" );

    // returns the total number of symbols removed from all its arguments.
    static int
    chomp( std::vector<std::string> &strvec, const std::string &sym = "\n" );
    
    
    // remove whitespace 
    static std::string 
    trim( const std::string &str ) { return rtrim(ltrim(str)); }
    
    static std::string 
    ltrim( const std::string &str ) { return lclip(str, m_left_whitespace); }
    
    static std::string 
    rtrim( const std::string &str ) { return rclip(str, m_right_whitespace); }

    
    // add/remove  quote characters ["] [']
    static std::string 
    quote( const std::string &str, const std::string &sym = "'" ) { return sym + str + sym; }
    
    static std::string 
    unquote( const std::string &str, const std::string &sym ) { return rclip(lclip(str, sym), sym); }
    
    static std::string 
    unquote( const std::string &str ) { return rclip( lclip(str, m_left_quotes), m_right_quotes ); }
    
    static std::string 
    lunquote( const std::string &str ) { return lclip(str, m_left_quotes); }
    
    static std::string 
    runquote( const std::string &str ) { return rclip(str, m_right_quotes); }


    // remove symbol or regular expresion from left/right i.e [(] and [)] or [<g] and [/>] 
    static std::string 
    clip( const std::string &str, const std::string &lsym, const std::string &rsym ) { return rclip(lclip(str, lsym), rsym); }
  
    static std::string 
    clip( const std::string &str, const std::regex &lexp, const std::regex &rexp ) { return rclip(lclip(str, lexp), rexp); }
    
    static std::string 
    clip( const std::string &str, const std::string &sym ) { return rclip(lclip(str, sym), sym); }
    
    static std::string 
    clip( const std::string &str, const std::regex &exp ) { return rclip(lclip(str, exp), exp); }
    
    static std::string 
    lclip( const std::string &str, const std::string &sym ) { return lclip(str, std::regex(R"(^\s*)" + escape(sym))); }

    static std::string 
    rclip( const std::string &str, const std::string &sym ) { return rclip(str, std::regex(escape(sym) + R"(\s*$)")); }
    
    static std::string 
    lclip( std::string str, const std::regex &exp )
    {
        std::smatch match;
        return (std::regex_search(str, match, exp)) ? match.suffix() : str;
    }

    static std::string 
    rclip( std::string str, const std::regex &exp )
    {
        std::smatch match;
        return (std::regex_search(str, match, exp)) ? match.prefix() : str;
    }

    
    // add/override mappings as you see fit
    // never use the same character in the accent key and the ascii value 
    static void
    addAccent( const std::string &accent, const std::string &ascii ) { m_diacritic[accent] = ascii; }
    
    // escape the std::regex special characters: 
    // . \ + * ? [ ^ ] $ ( ) { } = ! < > | : -
    static std::string 
    escape( const std::string &str );
    
    
private:

    static void setup( void );
    
    static const std::regex m_left_whitespace;
    static const std::regex m_right_whitespace;
    static const std::regex m_left_quotes;
    static const std::regex m_right_quotes;

    static std::map<std::string, std::string> m_escape;
    static std::map<std::string, std::string> m_diacritic;

};

#endif


