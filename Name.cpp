/* Name 28/08/2024

 $$$$$$$$$$$$$$$$
 $   Name.cpp   $
 $$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Helper class used to process the names in MarketId, Currency, Country and City.
 
 Process names with accents, by removing newlines, escaping single character 
 quote ['] (NB not [`]) and mappimg, in a transparent manner, non-ASCII characters such as: à => a and á => a. 

 Also provides methods to remove leading and trailing white space, quotes or brackets,
 and to split strings by an arbitrary delimeter. 
 
  
  \a = \x07 = alert (bell)
  \b = \x08 = backspace
  \t = \x09 = horizonal tab
  \n = \x0A = newline (or line feed)
  \v = \x0B = vertical tab
  \f = \x0C = form feed
  \r = \x0D = carriage return
 
*/


#ifndef __NAME_H__
#include "Name.h"
#endif

#include <cassert>

// https://www.regular-expressions.info
// https://www.regexlib.com/Default.aspx

const std::regex Name::m_left_whitespace   = std::regex( R"(^\s*)" );                // WHITESPACE
const std::regex Name::m_right_whitespace  = std::regex( R"(\s*$)" );
const std::regex Name::m_left_quotes       = std::regex( R"(^\s*[`'"])" );           // QUOTES
const std::regex Name::m_right_quotes      = std::regex( R"([`'"]\s*$)" );

std::map<std::string, std::string> Name::m_accent;
std::map<std::string, std::string> Name::m_escape;


std::string
Name::deaccent( std::string str )
// substitute characters with accents -- slow
{
    // const std::map<std::string, std::string>::value_type& c
    for (auto &c : m_accent)
    {
        size_t pos = 0;
        while ((pos = str.find(c.first, pos)) != str.npos)
            str.replace(pos, c.first.size(), c.second);
    }
    
    return str;
}

std::string
Name::escape( const std::string& str )
// escape std::regex special characters 
{
    std::string retVal;
    
    for (char c : str)
    {
        std::string sym(1, c);
        auto fitr = m_escape.find(sym);
        retVal += (fitr != m_escape.end()) ? fitr->second : sym;
    }

    return retVal;
}

std::string
Name::denewln( std::string str )
// remove newln '\n' and carriage return '\r', and escape single quote '\''
{
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == '\'')
            str.insert(i++, 1, '\\');
        else if (str[i] == '\n' || str[i] == '\r')
            str.erase(i);
    }
    
    return str;
}

std::vector<std::string> 
Name::split( const std::string &str, const std::string &delim ) 
{
    std::vector<std::string> retVal;

    // we don't need to escape(delim) as we search using std::string find
    size_t pos_start = 0, pos_end = 0, delim_len = delim.length();
    while ((pos_end = str.find(delim, pos_start)) != std::string::npos) 
    {
        retVal.push_back(str.substr(pos_start, pos_end - pos_start));
        pos_start = pos_end + delim_len;
    }
    retVal.push_back(str.substr(pos_start));

    return retVal;    
}

std::vector<std::string>
Name::split( std::string str, const std::regex &delim ) 
{
    std::vector<std::string> retVal;

    std::smatch match;
    while (std::regex_search(str, match, delim))
    {
        retVal.push_back( match.prefix() );
        str = match.suffix();   
    }
    retVal.push_back( str );

    return retVal;
}

std::string 
Name::capitalise( const std::string &str )
{
    std::string retVal;

    char last = ' ';
    for (char c : str)
    {
        retVal += (last == ' ' && c != ' ') ? std::toupper(c) : std::tolower(c);
        last = c;
    }

    return retVal;
}


void
Name::setup( void )
// alphabets with accents to watch out for include 
// Romanian, Polish, Vietnamese, and Cyrillic.
// https://www.fileformat.info/info/charset/UTF-16/list.htm
// https://www.codetable.net/unicodecharacters
{
    //
    // std::regex special characters
    //
    
    m_escape["."]  = R"(\.)";
    m_escape["\\"] = R"(\\)";
    m_escape["+"]  = R"(\+)";
    m_escape["*"]  = R"(\*)";
    m_escape["?"]  = R"(\?)";
    m_escape["["]  = R"(\[)";
    m_escape["^"]  = R"(\^)";
    m_escape["]"]  = R"(\])";
    m_escape["$"]  = R"(\$)";
    m_escape["("]  = R"(\()";
    m_escape[")"]  = R"(\))";
    m_escape["{"]  = R"(\{)";
    m_escape["}"]  = R"(\})";
    m_escape["="]  = R"(\=)";
    m_escape["!"]  = R"(\!)";
    m_escape["<"]  = R"(\<)";
    m_escape[">"]  = R"(\>)";
    m_escape["|"]  = R"(\|)";
    m_escape[":"]  = R"(\:)";
    m_escape["-"]  = R"(\-)";
    
    //
    // accents and their replacements
    //
    
    // NB never have the same character in the key and the value
    // i.e m_accent["'"] = R"(\')";
    // this will loop forever as "'" is matched again and again
    
    m_accent["Æ"] = "AE";
    m_accent["æ"] = "ae";

    m_accent["Œ"] = "OE";
    m_accent["œ"] = "oe";
    
    m_accent["ß"] = "ss";
    
    m_accent["Þ"] = "Th";
    m_accent["þ"] = "th";
 
    m_accent["Ā"] = "A"; // Latin A with macron
    m_accent["Á"] = "A"; // Latin A with acute
    m_accent["À"] = "A"; // Latin A with grave
    m_accent["Ã"] = "A"; // Latin A with tilde
    m_accent["Â"] = "A"; // Latin A with circumflex
    m_accent["Ä"] = "A"; // Latin A with diaeresis
    m_accent["Å"] = "A"; // Latin A with ring above
    m_accent["Ă"] = "A"; // Latin A with breve
    m_accent["Ą"] = "A"; // Latin A with ogonek
    
    m_accent["Č"] = "C";
    m_accent["Ç"] = "C";
    
    m_accent["Ḑ"] = "D"; 
    m_accent["Đ"] = "D"; 
    
    m_accent["É"] = "E"; 
    m_accent["È"] = "E";
    
    m_accent["Ħ"] = "H"; 
    m_accent["Ḩ"] = "H";
    
    m_accent["Í"] = "I"; 
    m_accent["Ì"] = "I";
    m_accent["İ"] = "I";  
    m_accent["Ï"] = "I";
    m_accent["Ī"] = "I";
    m_accent["Î"] = "I";
    
    m_accent["Ñ"] = "N"; 
    
    m_accent["Ò"] = "O";
    m_accent["Ó"] = "O";
    m_accent["Ô"] = "O";
    m_accent["Õ"] = "O";
    m_accent["Ö"] = "O"; 
    m_accent["Ø"] = "O";
    
    m_accent["Ķ"] = "K";
    
    m_accent["Ł"] = "L";
    
    m_accent["Š"] = "S";
    m_accent["Ş"] = "S"; 
    m_accent["Ś"] = "S"; 
    m_accent["Ș"] = "S";
    
    m_accent["Ţ"] = "T";
    m_accent["Ť"] = "T";
    m_accent["Ŧ"] = "T";
    m_accent["Ƭ"] = "T";
    m_accent["Ʈ"] = "T";
    
    m_accent["Ú"] = "U";
    m_accent["Ù"] = "U";
    m_accent["Û"] = "U";
    m_accent["Ü"] = "U";
    
    m_accent["Ż"] = "Z";  
    m_accent["Z̧"] = "Z"; 
    m_accent["Ž"] = "Z";
    m_accent["Ƶ"] = "Z";
    m_accent["Ž"] = "Z";
    m_accent["Ź"] = "Z";
    m_accent["Ȥ"] = "Z";
    
    //
    //
    //
    
    m_accent["à"] = "a";
    m_accent["á"] = "a"; 
    m_accent["â"] = "a";
    m_accent["ã"] = "a";
    m_accent["ä"] = "a";
    m_accent["å"] = "a";
    m_accent["ả"] = "a"; 
    m_accent["ậ"] = "a";
    m_accent["ằ"] = "a"; 
    m_accent["ắ"] = "a"; 
    m_accent["ā"] = "a"; 
    m_accent["ą"] = "a";   
    m_accent["ă"] = "a";
    m_accent["ầ"] = "a";
    m_accent["ẵ"] = "a";
    m_accent["ạ"] = "a";
    
    m_accent["ç"] = "c"; 
    m_accent["ć"] = "c";
    m_accent["č"] = "c"; 
    m_accent["ċ"] = "c"; 
    m_accent["ĉ"] = "c";
    m_accent["ƈ"] = "c";
    
    m_accent["ď"] = "d";
    m_accent["ḑ"] = "d";
    m_accent["đ"] = "d";
    
    m_accent["é"] = "e";   
    m_accent["è"] = "e"; 
    m_accent["ė"] = "e";
    m_accent["ë"] = "e"; 
    m_accent["ế"] = "e";
    m_accent["ề"] = "e";
    m_accent["ě"] = "e";
    m_accent["ê"] = "e";
    m_accent["ệ"] = "e";
    m_accent["ę"] = "e";
    m_accent["ē"] = "e";
    m_accent["ə"] = "e";
    
    m_accent["ġ"] = "g";
    m_accent["ğ"] = "g"; 
    m_accent["ĝ"] = "g";
    m_accent["ģ"] = "g";
    
    m_accent["ḩ"] = "h";           
    m_accent["ḥ"] = "h";
    m_accent["ħ"] = "h"; 
    m_accent["ĥ"] = "h";
    
    m_accent["í"] = "i"; 
    m_accent["ì"] = "i";
    m_accent["ĩ"] = "i";
    m_accent["î"] = "i"; 
    m_accent["ĭ"] = "i";
    m_accent["ī"] = "i";
    m_accent["ı"] = "i";
    m_accent["ï"] = "i";
    m_accent["ị"] = "i";

    m_accent["ł"] = "l";
    
    m_accent["ñ"] = "n";
    m_accent["ň"] = "n";
    m_accent["ń"] = "n";
    m_accent["ņ"] = "n";
    
    m_accent["ồ"] = "o"; 
    m_accent["ó"] = "o";
    m_accent["ò"] = "o"; 
    m_accent["ö"] = "o";
    m_accent["ǒ"] = "o";     
    m_accent["ô"] = "o";
    m_accent["ð"] = "o"; 
    m_accent["õ"] = "o";
    m_accent["ő"] = "o";
    m_accent["ọ"] = "o";
    m_accent["ơ"] = "o"; 
    m_accent["ō"] = "o";
    m_accent["ộ"] = "o"; 
    m_accent["ớ"] = "o";
    m_accent["ø"] = "o";
    m_accent["ǿ"] = "o";
    
    m_accent["ṟ"] = "r";
    m_accent["ṙ"] = "r"; 
    m_accent["ř"] = "r";     
    
    m_accent["š"] = "s"; 
    m_accent["ş"] = "s";
    m_accent["ś"] = "s";
    m_accent["ŝ"] = "s"; 
    m_accent["ș"] = "s";
    
    m_accent["ţ"] = "t"; 
    m_accent["ț"] = "t";
    
    m_accent["ů"] = "u";
    m_accent["ừ"] = "u"; 
    m_accent["ú"] = "u";
    m_accent["ū"] = "u";   
    m_accent["ü"] = "u";
    m_accent["ŭ"] = "u";
    m_accent["ũ"] = "u"; 
    m_accent["û"] = "u";
    m_accent["ư"] = "u";
    
    m_accent["ý"] = "y";
    m_accent["ÿ"] = "y";
    
    m_accent["ż"] = "z"; 
    m_accent["ẕ"] = "z";
    m_accent["ž"] = "z"; 
    m_accent["ź"] = "z"; 
    m_accent["ż"] = "z";
    m_accent["z̧"] = "z";
    m_accent["ƶ"] = "z";
}


//

