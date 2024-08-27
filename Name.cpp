/* Name 20/01/2024

 $$$$$$$$$$$$$$$$
 $   Name.cpp   $
 $$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Helper class
 
 Convert names with accents to 'ASCII' names by removing newlines, escaping single 
 quote ['] (note that [‘] is a different character) and mapping, in a transparent manner, 
 'non-ASCII' characters such as:  à => a and á => a. This can be useful for generating search keys.
 
 Also provides methods to split strings by arbitrary delimeter.
 
 Used to process the names in Country and City.
 
*/


#ifndef __NAME_H__
#include "Name.h"
#endif


std::map<std::string, std::string> Name::m_match;


Name::Name( void )
{
    setup();
}


std::string
Name::remove_accents( std::string str )
// substitute characters with accents 
{
    // const std::map<std::string, std::string>::value_type& c
    for (auto &c : m_match)
    {
        size_t pos = 0;
        while ((pos = str.find(c.first, pos)) != str.npos)
            str.replace(pos, c.first.size(), c.second);
    }
    
    return str;
}


std::string
Name::toASCII( const std:: string &str ) 
// remove newlines and escape single quote ['] - note [‘] is a different character
{
    std::string retVal = remove_accents(str);

    for (int k = 0; k < retVal.size(); ++k)
    {
        if (retVal[k] == '\'')
            retVal.insert(k++, 1, '\\');
        else if (retVal[k] == '\n')
            retVal.erase(k);
    }
    
    return retVal;
}


std::vector<std::string> 
Name::split( const std::string &str, const std::string &delim, int num ) 
{
    std::vector<std::string> retVal;
    retVal.reserve(num);

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
Name::split( std::string str, const std::regex &delim, int num ) 
{
    std::vector<std::string> retVal;
    retVal.reserve(num);

    std::smatch match;
    while (std::regex_search(str, match, delim))
    {
        if (!match.empty())
        {
            retVal.push_back( match.prefix() );
            str = match.suffix();   
        }
    }
    retVal.push_back( str );

    return retVal;
}


void
Name::setup( void )
// add/delete mappings as you see fit
// alphabets with accents to watch out for include 
// Romanian, Polish, Vietnamese, and Cyrillic.
{
    m_match["Æ"] = "AE";
    m_match["æ"] = "ae";

    m_match["Œ"] = "OE";
    m_match["œ"] = "oe";
    
    m_match["ß"] = "ss";
    m_match["Þ"] = "th";
    
    m_match["Ā"] = "A"; // Latin A with macron
    m_match["Á"] = "A"; // Latin A with acute
    m_match["À"] = "A"; // Latin A with grave
    m_match["Ã"] = "A"; // Latin A with tilde
    m_match["Â"] = "A"; // Latin A with circumflex
    m_match["Ä"] = "A"; // Latin A with diaeresis
    m_match["Å"] = "A"; // Latin A with ring above
    m_match["Ă"] = "A"; // Latin A with breve
    m_match["Ą"] = "A"; // Latin A with ogonek
    
    m_match["Č"] = "C";
    m_match["Ç"] = "C";
    
    m_match["Ḑ"] = "D"; 
    m_match["Đ"] = "D"; 
    
    m_match["É"] = "E"; 
    m_match["È"] = "E";
    
    m_match["Ħ"] = "H"; 
    m_match["Ḩ"] = "H";
    
    m_match["Í"] = "I"; 
    m_match["Ì"] = "I";
    m_match["İ"] = "I";  
    m_match["Ï"] = "I";
    m_match["Ī"] = "I";
    m_match["Î"] = "I";
    
    m_match["Ñ"] = "N"; 
    
    m_match["Ò"] = "O";
    m_match["Ó"] = "O";
    m_match["Ô"] = "O";
    m_match["Õ"] = "O";
    m_match["Ö"] = "O"; 
    m_match["Ø"] = "O";
    
    m_match["Ķ"] = "K";
    
    m_match["Ł"] = "L";
    
    m_match["Š"] = "S";
    m_match["Ş"] = "S"; 
    m_match["Ś"] = "S"; 
    m_match["Ș"] = "S";
    
    m_match["Ţ"] = "T";
    m_match["Ť"] = "T";
    m_match["Ŧ"] = "T";
    m_match["Ƭ"] = "T";
    m_match["Ʈ"] = "T";
    
    m_match["Ú"] = "U";
    m_match["Ù"] = "U";
    m_match["Û"] = "U";
    m_match["Ü"] = "U";
    
    m_match["Ż"] = "Z";  
    m_match["Z̧"] = "Z"; 
    m_match["Ž"] = "Z";
    m_match["Ƶ"] = "Z";
    m_match["Ž"] = "Z";
    m_match["Ź"] = "Z";
    m_match["Ȥ"] = "Z";
    
    //
    //
    //
    
    m_match["à"] = "a";
    m_match["á"] = "a"; 
    m_match["â"] = "a";
    m_match["ã"] = "a";
    m_match["ä"] = "a";
    m_match["å"] = "a";
    m_match["ả"] = "a"; 
    m_match["ậ"] = "a";
    m_match["ằ"] = "a"; 
    m_match["ắ"] = "a"; 
    m_match["ā"] = "a"; 
    m_match["ą"] = "a";   
    m_match["ă"] = "a";
    m_match["ầ"] = "a";
    m_match["ẵ"] = "a";
    m_match["ạ"] = "a";
    
    m_match["ç"] = "c"; 
    m_match["ć"] = "c";
    m_match["č"] = "c"; 
    m_match["ċ"] = "c"; 
    m_match["ĉ"] = "c";
    m_match["ƈ"] = "c";
    
    m_match["ď"] = "d";
    m_match["ḑ"] = "d";
    m_match["đ"] = "d";
    
    m_match["é"] = "e";   
    m_match["è"] = "e"; 
    m_match["ė"] = "e";
    m_match["ë"] = "e"; 
    m_match["ế"] = "e";
    m_match["ề"] = "e";
    m_match["ě"] = "e";
    m_match["ê"] = "e";
    m_match["ệ"] = "e";
    m_match["ę"] = "e";
    m_match["ē"] = "e";
    m_match["ə"] = "e";
    
    m_match["ġ"] = "g";
    m_match["ğ"] = "g"; 
    m_match["ĝ"] = "g";
    m_match["ģ"] = "g";
    
    m_match["ḩ"] = "h";           
    m_match["ḥ"] = "h";
    m_match["ħ"] = "h"; 
    m_match["ĥ"] = "h";
    
    m_match["í"] = "i"; 
    m_match["ì"] = "i";
    m_match["ĩ"] = "i";
    m_match["î"] = "i"; 
    
    m_match["ĭ"] = "i";
    m_match["ī"] = "i";
    m_match["ı"] = "i";
    m_match["ï"] = "i";
    m_match["ị"] = "i";

    m_match["ł"] = "l";
    
    m_match["ñ"] = "n";
    m_match["ň"] = "n";
    m_match["ń"] = "n";
    m_match["ņ"] = "n";
    
    m_match["ồ"] = "o"; 
    m_match["ó"] = "o";
    m_match["ò"] = "o"; 
    m_match["ö"] = "o";
    m_match["ǒ"] = "o";     
    m_match["ô"] = "o";
    m_match["ð"] = "o"; 
    m_match["õ"] = "o";
    m_match["ő"] = "o";
    m_match["ọ"] = "o";
    m_match["ơ"] = "o"; 
    m_match["ō"] = "o";
    m_match["ộ"] = "o"; 
    m_match["ớ"] = "o";
    m_match["ø"] = "o";
    m_match["ǿ"] = "o";
    
    m_match["ṟ"] = "r";
    m_match["ṙ"] = "r"; 
    m_match["ř"] = "r";     
    
    m_match["š"] = "s"; 
    m_match["ş"] = "s";
    m_match["ś"] = "s";
    m_match["ŝ"] = "s"; 
    m_match["ș"] = "s";
    
    m_match["ţ"] = "t"; 
    m_match["ț"] = "t";
    
    m_match["ů"] = "u";
    m_match["ừ"] = "u"; 
    m_match["ú"] = "u";
    m_match["ū"] = "u";   
    m_match["ü"] = "u";
    m_match["ŭ"] = "u";
    m_match["ũ"] = "u"; 
    m_match["û"] = "u";
    m_match["ư"] = "u";
    
    m_match["ý"] = "y";
    m_match["ÿ"] = "y";
    
    m_match["ż"] = "z"; 
    m_match["ẕ"] = "z";
    m_match["ž"] = "z"; 
    m_match["ź"] = "z"; 
    m_match["ż"] = "z";
    m_match["z̧"] = "z";
    m_match["ƶ"] = "z";
}


//







