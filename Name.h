/* Name 20/01/2024

 $$$$$$$$$$$$$$
 $   Name.h   $
 $$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Helper class
 
 Provide a number of useful functions for std::string, in one place, with a simple, consistent syntax. 
 Specifically Name provides functions for:

 1)  removing newlines, escaping single character apostraphe ['] (but not [`]),
 2)  mappimg characters with diacritic signs to ASCII characters i.e. [à] => [a] and [á] => [a],
 3)  removing or replacing characters or substrings, 
 4)  removing leading and trailing white space, quotes, or brackets, 
 5)  splitting strings by an arbitrary delimeter. 
 6)  calculation of the Damerau–Levenshtein distance between strings.
 
 All are implemented using standard library types/functions.
 
 Countries with alphabets that employ diacritic signs include:
 AT, BO, BR, CH, CL, CR, DE, DK, FI, FO, FR, HU, IS, KR, MX, NO, PA, PE, PT, SE, SJ, TR and VN. 

 The Damerau–Levenshtein distance measures the similarity between strings. 
 It is typically used to measure distance between typographic errors or 'human misspellings'.
 Usefull for matching names with alternate or erroneous spellings.
 see https://en.wikipedia.org/wiki/Damerau–Levenshtein_distance
 
 I have used static methods so as to avoid introducing yet another string class.
 
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

 Example 4
 
 std::cout << "Name::dist(\"London\",\"Lomdon\") is" <<  Name::dist("London","Lomdon") << std::endl;
 std::cout << "Name::dist(\"London\",\"Londno\") is" <<  Name::dist("London","Lomdon") << std::endl;
 
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
    
    // return true if str only contains Roman characters; ASCII values [32, 126]
    static bool
    isroman( const std::string &str ); 
    
    // remove newline [\n] and carriage return [\r] also escape single quote ['] -> [\']
    static std::string
    denewln( std::string str );

    // Damerau–Levenshtein distance - measures the similarity between two strings - between 'human misspellings'
    // see https://en.wikipedia.org/wiki/Damerau–Levenshtein_distance
    static int
    dist(const std::string &str1, const std::string &str2);
   
    
    // remove all or first occurence of symbol 'match' 
    static std::string 
    strip(std::string str, const std::string &match, bool first = false) { return replace(str, match, "", first); }  
    
    static std::string 
    strip(std::string str, const std::regex &match, bool first = false) { return replace(str, match, "", first); }
  
    // replace all or first occurence of string 'match' with string 'with' 
    static std::string 
    replace(std::string str, const std::string &match, const std::string &with, bool first = false); 
    
    static std::string 
    replace(std::string str, const std::regex &match, const std::string &with, bool first = false);
    
    
    // convert the first character in each word to upper case
    static std::string 
    capitalise( const std::string &str );
    
    static std::string 
    toupper( std::string str );

    static std::string 
    tolower( std::string str );
    
    
    // split the string by delimiter 
    static std::vector<std::string> 
    split( const std::string &str, const std::string &delim );
    
    static std::vector<std::string>
    split( std::string str, const std::regex &delim );

    static std::string
    concat( const std::vector<std::string> &strvec, const std::string &delim = "");
    
  
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
    
    static std::vector<std::string> 
    trim( const std::vector<std::string> &strvec );
    
    static std::string 
    ltrim( const std::string &str ) { return lclip(str, m_left_whitespace); }
    
    static std::string 
    rtrim( const std::string &str ) { return rclip(str, m_right_whitespace); }

    
    // add/remove quote characters ["] ['] -- for more a sophisticated approach see std::quoted
    static std::string 
    quote( const std::string &str, const std::string &sym = "'" ) { return sym + str + sym; }
    
    static std::vector<std::string> 
    quote( const std::vector<std::string> &strvec, const std::string &sym = "'" );
    
    static std::string 
    unquote( const std::string &str, const std::string &sym ) { return rclip(lclip(str, sym), sym); }
    
    static std::vector<std::string> 
    unquote( const std::vector<std::string> &strvec, const std::string &sym );
    
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
    static const std::regex m_trail_newlines;

    static std::map<std::string, std::string> m_escape;
    static std::map<std::string, std::string> m_diacritic;

};

#endif


