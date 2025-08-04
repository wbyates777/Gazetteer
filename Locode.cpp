/* Locode 29/05/2025
 
 $$$$$$$$$$$$$$$$$$$$$$$$$
 $   Locode.cpp - code   $
 $$$$$$$$$$$$$$$$$%$$$$$$$
 
 by W.B. Yates    
 History: 
 
 
 */

#ifndef __LOCODE_H__
#include "Locode.h"
#endif

#include <cassert>

    
std::ostream&
operator<<( std::ostream &ostr, const Locode &c )
{
    ostr << c.locode();
    return ostr;
}

std::istream&
operator>>( std::istream &istr, Locode &c )
{
    std::string str;
    istr >> str;
    c.setLocode( str );
    return istr;
}

//
//
//

bool
Locode::setLocode( const std::string &str )
// https://en.wikipedia.org/wiki/Binary_search_algorithm
{    
    assert(str.size() == 5);

    const int index = str[0] - 'A'; // 'A' = 65;
    //assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_locode = LOCODE::XXXXX;
        return false;
    }
    
    int low   = m_search[index]; 
    int high  = m_search[index + 1]; 
    int mid   = ((high + low) >> 1); // divide by 2 (quickly)
    int i;
    
    while (low < high) 
    {
        const char * const cty = m_codes[mid];
        
        for (i = 1; i < 5; ++i)
        {
            const char &a = str[i];
            const char &b = cty[i];
            
            if (a < b)
            {
                high = mid; 
                break;
            }
            
            if (a > b)
            {
                low = mid + 1;
                break;
            }
        }
        
        if (i == 5)
        {
            m_locode = mid; 
            return true;
        }
        
        mid = ((high + low) >> 1);
    }
    
    m_locode = LOCODE::XXXXX; // NOLOCODE
    return false;
}


std::string 
Locode::toString( Locode::Function f )
{
    switch (f)
    {
        case Locode::UNKNOWN:      return "UNKNOWN"; break;
            
        case Locode::SEAPORT:      return "SEAPORT"; break;
        case Locode::RAIL:         return "RAIL"; break;
        case Locode::ROAD:         return "ROAD"; break;
        case Locode::AIRPORT:      return "AIRPORT"; break;
        case Locode::POSTAL:       return "POSTAL"; break;
        case Locode::DEPOT:        return "DEPOT"; break;
        case Locode::FIXED:        return "FIXED"; break;
        case Locode::INLAND_WATER: return "INLAND_WATER"; break;
        case Locode::ZONE:         return "ZONE"; break;
        case Locode::CROSSING:     return "CROSSING"; break;
        case Locode::MAXFUNCTION:  return "MAXFUNCTION"; break;
            
        default: exit(EXIT_FAILURE); break;
    }
}

std::string 
Locode::toString( Status s ) 
{
    switch (s)
    {
        case Locode::NOSTATUS: return "NONE"; break;
        case Locode::AM: return "AM"; break;
        case Locode::RL: return "RL"; break;
        case Locode::RQ: return "RQ"; break;
        case Locode::XX: return "XX"; break;
        case Locode::AA: return "AA"; break;
        case Locode::AC: return "AC"; break;
        case Locode::AI: return "AI"; break;
        case Locode::AF: return "AF"; break;
        case Locode::AS: return "AS"; break;
        case Locode::AQ: return "AQ"; break;
        case Locode::RN: return "RN"; break;
        case Locode::UR: return "UR"; break;
        case Locode::QQ: return "QQ"; break;
        case Locode::MAXSTATUS: return "MAXSTATUS"; break;

        default: exit(EXIT_FAILURE); break;
    }
}



#ifdef __LARGE__
#include "LOCODES_LARGE_BODY.txt"
#else
#include "LOCODES_SMALL_BODY.txt"
#endif


