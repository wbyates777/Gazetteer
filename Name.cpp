/* Name 28/08/2024

 $$$$$$$$$$$$$$$$
 $   Name.cpp   $
 $$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Provides methods for removing newlines, escaping single character apostraphe ['] (but not [`]) 
 and mappimg characters with diacritic signs to ASCII characters i.e. [à] => [a] and [á] => [a]. 
 Also provides methods to remove leading and trailing white space, quotes, or brackets, 
 and to split strings by an arbitrary delimeter. 
 
 
  Countries with alphabets that employ diacritic signs include:
  AT, BO, BR, CH, CL, CR, DE, DK, FI, FO, FR, HU, IS, KR, MX, NO, PA, PE, PT, SE, SJ, TR and VN.  
 
  Escape characters:
 
  \a = \x07 = alert (bell)
  \b = \x08 = backspace
  \t = \x09 = horizonal tab
  \n = \x0A = newline (or line feed)
  \v = \x0B = vertical tab
  \f = \x0C = form feed
  \r = \x0D = carriage return
  \e = \x1B = escape (non-standard GCC extension)
 
 
 Punctuation characters:

  \" = quotation mark (backslash not required for '"')
  \' = apostrophe (backslash not required for "'")
  \? = question mark (used to avoid trigraphs)
  \\ = backslash
 
  Character literals for 'A'

   char     c0 =  'A'; 
   char     c1 =  u8'A'; 
   wchar_t  c2 =  L'A'; 
   char16_t c3 =  u'A'; 
   char32_t c4 =  U'A'; 
 
*/


#ifndef __NAME_H__
#include "Name.h"
#endif

#include <iostream>
#include <cassert>


// https://www.regular-expressions.info
// https://www.regexlib.com/Default.aspx
const std::regex Name::m_left_whitespace  = std::regex( R"(^\s*)" );      // WHITESPACE
const std::regex Name::m_right_whitespace = std::regex( R"(\s*$)" );
const std::regex Name::m_left_quotes      = std::regex( R"(^\s*['"])" );  // QUOTES
const std::regex Name::m_right_quotes     = std::regex( R"(['"]\s*$)" );

std::map<std::string, std::string> Name::m_diacritic;
std::map<std::string, std::string> Name::m_escape;



std::string
Name::deaccent( std::string str )
// substitute characters with accents -- slow
{
    assert(!m_diacritic.empty()); // call setup()
 
    // const std::map<std::string, std::string>::value_type& c
    for (auto &c : m_diacritic)
    {
        size_t pos = 0;
        while ((pos = str.find(c.first, pos)) != str.npos)
        {
            str.replace(pos, c.first.size(), c.second);
        }
    }
    
    return str;
}

bool
Name::isroman( const std::string &str )
// https://en.wikipedia.org/wiki/ASCII
// std::isalpha depends/varies on locale
{
    //  ASCII values
    // 32      = ' '  // space
    // 48-57   = [0-9]
    // 65-90   = [A-Z]
    // 97-122  = [a-z]
    // 126     = '~'
    // 128 = 
    for (char c : str)
    {
        if (!((c >= 32 && c <= 126) || std::isspace(c)))
            return false;
    }
    return true;
}


std::string
Name::escape( const std::string &str )
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

// removes any trailing string that corresponds to the value of sym (typically newline "\n")
// when sym = "" remove all trailing newlines - returns the total number of symbols removed 
int
Name::chomp( std::string &str, const std::string &sym )
{
    std::regex exp = (sym.empty()) ? std::regex("\n+$") : std::regex(Name::escape(sym) + "$");
    
    std::smatch match;
    if (std::regex_search(str, match, exp)) 
    {
        str =  match.prefix();
        return (int) match.length() / std::max(1, (int) sym.size());
    }
    
    return 0;
}

// returns the total number of symbols removed from all its arguments.
int
Name::chomp( std::vector<std::string> &strvec, const std::string &sym )
{
    int count = 0;
    for (std::string &s : strvec)
        count += chomp(s, sym);
 
    return count;
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
// countires with alphabets that employ diacritic signs include:
// AT, BO, BR, CH, CL, CR, DE, DK, FI, FO, FR, HU, IS, KR, MX, NO, PA, PE, PT, SE, SJ, TR and VN. 
// https://service.unece.org/trade/locode/2024-1%20UNLOCODE%20SecretariatNotes.pdf
// https://www.codetable.net/unicodecharacters
// https://www.fileformat.info/info/charset/UTF-16/list.htm
// https://www.fileformat.info/info/unicode/block/latin_extended_additional/images.htm
{
    //
    // std::regex special characters
    //
    
    m_escape["."]  = R"(\.)";
    m_escape[R"(\)"] = R"(\\)";
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
    
    // never use the same character in the accent key and the ascii value
    // i.e m_diacritic["'"] = R"(\')";
    // this will loop forever as "'" is matched again and again
    
    m_diacritic["Æ"] = "A"; // "AE";
    m_diacritic["æ"] = "a"; // "ae";

    m_diacritic["Œ"] = "O"; // "OE";
    m_diacritic["œ"] = "o"; // "oe";
    
    m_diacritic["ß"] = "ss";
    
    m_diacritic["Þ"] = "Th";
    m_diacritic["þ"] = "th";
 
    m_diacritic["Ā"] = "A"; // Latin A with macron
    m_diacritic["Á"] = "A"; // Latin A with acute
    m_diacritic["À"] = "A"; // Latin A with grave
    m_diacritic["Ã"] = "A"; // Latin A with tilde
    m_diacritic["Â"] = "A"; // Latin A with circumflex
    m_diacritic["Ä"] = "A"; // Latin A with diaeresis
    m_diacritic["Å"] = "A"; // Latin A with ring above
    m_diacritic["Ă"] = "A"; // Latin A with breve
    m_diacritic["Ą"] = "A"; // Latin A with ogonek
    
    m_diacritic["Č"] = "C";
    m_diacritic["Ç"] = "C";
    
    m_diacritic["Ḑ"] = "D"; 
    m_diacritic["Đ"] = "D"; 
    
    m_diacritic["É"] = "E"; 
    m_diacritic["È"] = "E";
    
    m_diacritic["Ħ"] = "H"; 
    m_diacritic["Ḩ"] = "H";
    
    m_diacritic["Í"] = "I"; 
    m_diacritic["Ì"] = "I";
    m_diacritic["İ"] = "I";  
    m_diacritic["Ï"] = "I";
    m_diacritic["Ī"] = "I";
    m_diacritic["Î"] = "I";
    
    m_diacritic["Ñ"] = "N"; 
    
    m_diacritic["Ò"] = "O";
    m_diacritic["Ó"] = "O";
    m_diacritic["Ô"] = "O";
    m_diacritic["Õ"] = "O";
    m_diacritic["Ö"] = "O"; 
    m_diacritic["Ø"] = "O";
    
    m_diacritic["Ķ"] = "K";
    
    m_diacritic["Ł"] = "L";
    
    m_diacritic["Š"] = "S";
    m_diacritic["Ş"] = "S"; 
    m_diacritic["Ś"] = "S"; 
    m_diacritic["Ș"] = "S";
    
    m_diacritic["Ţ"] = "T";
    m_diacritic["Ť"] = "T";
    m_diacritic["Ŧ"] = "T";
    m_diacritic["Ƭ"] = "T";
    m_diacritic["Ʈ"] = "T";
    m_diacritic["Ṭ"] = "T";
    
    m_diacritic["Ú"] = "U";
    m_diacritic["Ù"] = "U";
    m_diacritic["Û"] = "U";
    m_diacritic["Ü"] = "U";
    m_diacritic["Ŭ"] = "U";
    m_diacritic["Ũ"] = "U";
    m_diacritic["Ů"] = "U";
    m_diacritic["Ū"] = "U";
    
    
    m_diacritic["Ỳ"] = "Y"; 
    m_diacritic["Ÿ"] = "Y"; 
    m_diacritic["Ý"] = "Y"; 
    
    
    m_diacritic["Ż"] = "Z";  
    m_diacritic["Z̧"] = "Z"; 
    m_diacritic["Ž"] = "Z";
    m_diacritic["Ƶ"] = "Z";
    m_diacritic["Ž"] = "Z";
    m_diacritic["Ź"] = "Z";
    m_diacritic["Ȥ"] = "Z";
    
    //
    //
    //
    
    m_diacritic["à"] = "a";
    m_diacritic["á"] = "a"; 
    m_diacritic["â"] = "a";
    m_diacritic["ã"] = "a";
    m_diacritic["ä"] = "a";
    m_diacritic["å"] = "a";
    m_diacritic["ả"] = "a"; 
    m_diacritic["ậ"] = "a";
    m_diacritic["ằ"] = "a"; 
    m_diacritic["ắ"] = "a"; 
    m_diacritic["ā"] = "a"; 
    m_diacritic["ą"] = "a";   
    m_diacritic["ă"] = "a";
    m_diacritic["ầ"] = "a";
    m_diacritic["ẵ"] = "a";
    m_diacritic["ạ"] = "a";
    
    m_diacritic["ç"] = "c"; 
    m_diacritic["ć"] = "c";
    m_diacritic["č"] = "c"; 
    m_diacritic["ċ"] = "c"; 
    m_diacritic["ĉ"] = "c";
    m_diacritic["ƈ"] = "c";
    
    m_diacritic["ď"] = "d";
    m_diacritic["ḑ"] = "d";
    m_diacritic["đ"] = "d";
    
    m_diacritic["é"] = "e";   
    m_diacritic["è"] = "e"; 
    m_diacritic["ė"] = "e";
    m_diacritic["ë"] = "e"; 
    m_diacritic["ế"] = "e";
    m_diacritic["ề"] = "e";
    m_diacritic["ě"] = "e";
    m_diacritic["ê"] = "e";
    m_diacritic["ệ"] = "e";
    m_diacritic["ę"] = "e";
    m_diacritic["ē"] = "e";
    m_diacritic["ə"] = "e";
    
    m_diacritic["ġ"] = "g";
    m_diacritic["ğ"] = "g"; 
    m_diacritic["ĝ"] = "g";
    m_diacritic["ģ"] = "g";
    
    m_diacritic["ḩ"] = "h";           
    m_diacritic["ḥ"] = "h";
    m_diacritic["ħ"] = "h"; 
    m_diacritic["ĥ"] = "h";
    
    m_diacritic["í"] = "i"; 
    m_diacritic["ì"] = "i";
    m_diacritic["ĩ"] = "i";
    m_diacritic["î"] = "i"; 
    m_diacritic["ĭ"] = "i";
    m_diacritic["ī"] = "i";
    m_diacritic["ı"] = "i";
    m_diacritic["ï"] = "i";
    m_diacritic["ị"] = "i";

    m_diacritic["ł"] = "l";
    
    m_diacritic["ñ"] = "n";
    m_diacritic["ň"] = "n";
    m_diacritic["ń"] = "n";
    m_diacritic["ņ"] = "n";
    
    
    
    m_diacritic["ồ"] = "o"; 
    m_diacritic["ó"] = "o";
    m_diacritic["ò"] = "o"; 
    m_diacritic["ö"] = "o";
    m_diacritic["ǒ"] = "o";     
    m_diacritic["ô"] = "o";
    m_diacritic["ð"] = "o"; 
    m_diacritic["õ"] = "o";
    m_diacritic["ő"] = "o";
    m_diacritic["ọ"] = "o";
    m_diacritic["ơ"] = "o"; 
    m_diacritic["ō"] = "o";
    m_diacritic["ộ"] = "o"; 
    m_diacritic["ớ"] = "o";
    m_diacritic["ø"] = "o";
    m_diacritic["ǿ"] = "o";
    
    m_diacritic["ṟ"] = "r";
    m_diacritic["ṙ"] = "r"; 
    m_diacritic["ř"] = "r";     
    
    m_diacritic["š"] = "s"; 
    m_diacritic["ş"] = "s";
    m_diacritic["ś"] = "s";
    m_diacritic["ŝ"] = "s"; 
    m_diacritic["ș"] = "s";
    
    m_diacritic["ţ"] = "t"; 
    m_diacritic["ț"] = "t";
    m_diacritic["ṭ"] = "t";
    
    m_diacritic["ů"] = "u";
    m_diacritic["ừ"] = "u"; 
    m_diacritic["ú"] = "u";
    m_diacritic["ù"] = "u"; 
    m_diacritic["ū"] = "u";   
    m_diacritic["ü"] = "u";
    m_diacritic["ŭ"] = "u";
    m_diacritic["ũ"] = "u"; 
    m_diacritic["û"] = "u";
    m_diacritic["ư"] = "u";
    
    m_diacritic["ý"] = "y";
    m_diacritic["ỳ"] = "y"; 
    m_diacritic["ÿ"] = "y";
    
    m_diacritic["ż"] = "z"; 
    m_diacritic["ẕ"] = "z";
    m_diacritic["ž"] = "z"; 
    m_diacritic["ź"] = "z"; 
    m_diacritic["ż"] = "z";
    m_diacritic["z̧"] = "z";
    m_diacritic["ƶ"] = "z";
}


//







