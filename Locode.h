/* Locode 29/05/2025
 
 $$$$$$$$$$$$$$$$$$$$$$$$$
 $   Locode.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 
 History: 

 
 A subset of UN/LOCODE codes with status and function attributes.
 The 1975 locations represented in the City class each have a UN/LOCODE.
 
 UN/LOCODES location names can have hyphens and apostrophes i.e "Val-d'Or" but not diacritic signs (accents).
 Note that symbols [country2code]XXX are valid location codes while the symbol XXXXX is not an official UN/LOCODE.
 
 see https://unece.org/trade/uncefact/unlocode 
 
 see https://en.wikipedia.org/wiki/UN/LOCODE 
 
 see https://unece.org/trade/publications/recommendation-ndeg16-united-nations-code-trade-and-transport-locations
 
 
 Each UN/LOCODES has a status:
 
 UN/LOCODE Status Attributes Codes
 ----------------------------------------------------------------
 AM - Approved by the UN/LOCODE maintenance team.
 RL - Recognized location - Existence and representation of location name confirmed - relevance to international trade is not confirmed.
 RQ - Request under consideration. Until the request has been validated, it should not be used in international electronic communication.
 XX - Entry that will be removed from the next issue of UN/LOCODE.
 
 UR - Entry included on user's request; not officially approved.
 RR - Request rejected.
 QQ - Original entry not verified since date indicated.
 
 UN/LOCODE Legacy Status Attributes Codes
 ----------------------------------------------------------------
 AA - Approved by competent national government agency
 AC - Approved by Customs Authority
 AI - Code adopted by international organization (IATA or ECLAC)
 AF - Approved by national facilitation body
 AS - Approved by national standardization body
 AQ - Entry approved, functions not verified
 RN - Request from credible national sources for locations in their own country
 
 
 Each UN/LOCODES have function attributes:
 
 The function attribute code identifies the existence of either a facility providing a connection with a
 specific mode of transport or some other significant function not directly related to any mode of transport at this location.
 
 UN/LOCODE Function Attributes Codes
 ----------------------------------------------------------------------------------------------------------------------------
 Code      |   File Entry  | Description
 ----------------------------------------------------------------------------------------------------------------------------
   0       |   "________"  | Unknown/No functions
   1       |   "1_______"  | Seaport (at which seagoing vessels can load or discharge cargo).
   2       |   "_2______"  | Rail terminal (excluding passenger terminals).
   3       |   "__3_____"  | Road terminal. 
   4       |   "___4____"  | Airport/Spaceport (at which aircraft can load or discharge cargo).
   5       |   "____5___"  | Postal exchange office or International Mail Processing Centre (IMPC).
   6       |   "_____6__"  | Inland Clearance Depot (ICD) or \"Dry Port\", \"Inland Clearance Terminal\", etc.
   7       |   "______7_"  | Fixed transport functions (e.g. oil platform) the classifier "7" is reserved for this function. 
           |               | Noting that the description \"oil pipeline terminal\" would be 
           |               | more relevant, and could be extended to cover also electric power lines and ropeway terminals. 
   8       |   "_______8"  | Inland water transport.
   A       |   "_______A"  | Special Economic Zone (SEZ) - Any geographic region that has economic laws different
           |               | from a country's typical economic laws for the purposes of trade operations and duties and tariffs.
   B       |   "_______B"  | Border crossing function (former code not to be used). 
 ----------------------------------------------------------------------------------------------------------------------------
 For example a UN/LOCODE with a function sttribute entry of "1___5___" has a seaport and a postal exchange office.
 (see 'United Nations Code for Trade and Transport Locations', Recommendation N°.16, UNECE, 2020)

 
 Example 1
 
     // print out some simple stats
     std::vector<int> count(Locode::MAXSTATUS, 0);
     std::vector<int> coords(Locode::MAXSTATUS, 0);

     for( int i = 1; i < LOCODE::MAXLOCODE; ++i)
     {
         Locode code(i);
         count[code.status()]++;
         if (code.valid_pos())
             coords[code.status()]++;
     }
     
     for( int i = 0; i < Locode::MAXSTATUS; ++i)
     {
         if (count[i])
             std::cout << Locode::toString(Locode::Status(i)) << "  |  " << count[i] << "  |  " << coords[i]  << std::endl; 
     }
     
     exit(1);
 
 */


#ifndef __LOCODE_H__
#define __LOCODE_H__



#include <string>
#include <iostream>

// #define __LARGE__

// Note  NOLOCODE, XXXXX, MAXLOCODE, NUMLOCODE are not UN/LOCODE codes.


#ifdef __LARGE__

namespace LOCODE 
{ 
constexpr int NOLOCODE  = 0;
constexpr int XXXXX     = 114967;
constexpr int MAXLOCODE = 115985;
constexpr int NUMLOCODE = 115985;
};
// These defs are NOT necessary for compilation
// only need to be included where numeric LOCODEs i.e LOCODE::USNYC are being used 
// it is included here for convenience
#ifndef __LOCODES_LARGE_HEADER_H__
#include "LOCODES_LARGE_HEADER.h"
#endif

#else

namespace LOCODE 
{ 
// A small subset of 26857 UN/LOCODE codes with status AM, AA, AC, AI, AF or AS.
// There are also some codes with status RL, RQ, RN, QQ that are included from City.
constexpr int NOLOCODE  = 0;
constexpr int XXXXX     = 26687;
constexpr int MAXLOCODE = 26857;
constexpr int NUMLOCODE = 26857;
};
// These defs are NOT necessary for compilation
// only need to be included where numeric LOCODEs i.e LOCODE::USNYC are being used 
// it is included here for convenience
#ifndef __LOCODES_SMALL_HEADER_H__
#include "LOCODES_SMALL_HEADER.h"
#endif 

#endif



class Locode
{
public:
    
    typedef int Lotype ;
    
    enum Status : unsigned short int 
    {
        NOSTATUS = 0, AM, RL, RQ, XX,  AA, AC, AI, AF, AS, AQ, RN, UR, QQ, MAXSTATUS
    };
    
    enum Function : unsigned short int 
    {
        UNKNOWN      = 0,                                         
        SEAPORT      = (1u << 0),                               
        RAIL         = (1u << 1),                                
        ROAD         = (1u << 2),                              
        AIRPORT      = (1u << 3), SPACEPORT = (1u << 3),          
        POSTAL       = (1u << 4), IMPC      = (1u << 4),          
        DEPOT        = (1u << 5), ICD       = (1u << 5),         
        FIXED        = (1u << 6),                                 
        INLAND_WATER = (1u << 7),                                
        ZONE         = (1u << 8), SEZ = (1u << 8), A = (1u << 8),  // SPECIAL_ECONOMIC_ZONE 
        CROSSING     = (1u << 9), BC = (1u << 9), B = (1u << 9),   // BORDER_CROSSING
        MAXFUNCTION
    };
    
    // The value of the enum elements are my arbitrary numeric code for each LOCODE. 
    // Note XXXXX, NOLOCODE, MAXLOCODE, NUMLOCODE are not UN/LOCODE codes.
    Locode( void ): m_locode(LOCODE::NOLOCODE) {}
    ~Locode( void ) { m_locode = LOCODE::NOLOCODE; }
    
    // non-explicit constructors intentional here
    Locode( Lotype i ): m_locode(i) {} // e.g. i = LOCODE::GBLON
    Locode( const std::string &s ): m_locode(LOCODE::NOLOCODE) { setLocode(s); }
    Locode( const char *s ): m_locode(LOCODE::NOLOCODE) { if (s) setLocode(s); } 
    
    // my numeric code for this locode e.g. LOCODE::GBLON = 13844 
    operator int( void ) const { return m_locode; }
    
    // country2code i.e. GB
    std::string
    country( void ) const { return std::string(m_codes[m_locode], 2); }

    // location3code i.e. LON
    std::string
    location( void ) const { return std::string(m_codes[m_locode] + 2, 3); }
    
    // place name  i.e. "London" 
    std::string 
    name( void ) const { return m_fullNames[m_locode]; }
    

    // the 5 letter UN/LOCODE for this location e.g. "GBLON"
    std::string
    locode( void ) const { return m_codes[m_locode]; }
    
    // the 3 letter subdivison code for this location e.g. LDN
    std::string
    subdiv( void ) const { return (m_subdiv[m_locode]) ? m_subdiv[m_locode] : "XXX"; }
    
    bool
    setLocode( const std::string &s ); // e.g. s = "GBLON"
    
    void
    setLocode( Lotype s ) { m_locode = s; } // e.g. s = LOCODE::GBLON
    
    //
    bool 
    has( Function func ) const { return (func & m_function[m_locode]); } 
    
    Status 
    status( void ) const { return Status(m_function[m_locode] >> 11); } 
    //
    
    // latitude
    double 
    lat( void ) const { return m_position[m_locode][0]; }
    
    // longitude
    double 
    lon( void ) const { return m_position[m_locode][1]; }
    
    std::pair<double,double> // (latitude, longitude)
    pos( void ) const 
    { 
        const float *p = m_position[m_locode];
        return std::pair<double,double>(p[0], p[1]); 
    }

    // valid coordinates 
    bool 
    valid_pos( void ) const { return (m_function[m_locode] & (1u << 10)); }
    
    bool                
    valid( void ) const { return m_locode != LOCODE::NOLOCODE; }
    
    static Locode
    index( int i ) { return Locode::Lotype(i); }
    
    static int
    index( const Locode &c ) { return c; }
    
    static std::string 
    toString( Locode::Function s );
    
    static std::string 
    toString( Locode::Status s );

private:
    
    int m_locode;
    
    static const int             m_search[28]; 
    static const unsigned short  m_function[LOCODE::NUMLOCODE]; 
    static const float           m_position[LOCODE::NUMLOCODE][2];    
    static const char * const    m_codes[LOCODE::NUMLOCODE];
    static const char * const    m_fullNames[LOCODE::NUMLOCODE];
    static const char * const    m_subdiv[LOCODE::NUMLOCODE];

};


std::ostream&
operator<<( std::ostream &ostr, const Locode &c );

std::istream&
operator>>( std::istream &istr, Locode &c );


#endif



