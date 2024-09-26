/* Gazetteer 15/03/2012.
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Gazetteer.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History:  A geographic dictionary or index 
 

 Region and subregion codes are taken from:
 "Standard Country or Area Codes for Statistical Use, Revision 4 (United Nations publication, Sales No. 98.XVII.9"
 
 
 
 Examples
 
 Gazetteer g;
 std::cout << "Gazetter\n" << std::endl;
 
 std::cout << Currency("USD").name() << " " << g.country(Currency("USD")) << std::endl;
 std::cout << City("MAD").name() << ", " << g.country(City::MAD).name() << ", " << g.ccy( g.country(City::MAD) ).name() << std::endl;
 std::cout << Country("GB").name() << ", " << g.capital(Country::GBR).name() << ", " << g.ccy(Country::GB) << std::endl;     
 std::cout << Country(Country::CH).name() << ", " << g.capital("CHE").name() << ", " << g.ccys(Country::CHE) << std::endl;
 
 std::cout << "Spanish cities " << g.cities(Country::ESP) << std::endl << std::endl;
 std::cout << "Spanish markets " <<  g.markets(Country::ESP) << std::endl << std::endl;
 
 std::vector<City> rs =  g.cities(std::string("D.*"));
 for (int i= 0; i < rs.size(); ++i)
 {
    std::cout << rs[i].name() << std::endl;
 }
 
 std::cout << "The countries of  Subregion::SOUTHERN_EUROPE" << std::endl;
 std::vector<Country> southern_europe = g.subregion(Gazetteer::Subregion::SOUTHERN_EUROPE);
 std::cout << southern_europe << std::endl;

 
 */


#ifndef __GAZETTEER_H__
#define __GAZETTEER_H__

#include <vector>
#include <string>


#ifndef __MARKETID_H__
#include "MarketId.h"
#endif

#ifndef __CITY_H__
#include "City.h"
#endif

#ifndef __COUNTRY_H__
#include "Country.h"
#endif

#ifndef __CURRENCY_H__
#include "Currency.h"
#endif

class Gazetteer 
{
public:
    
    //
    // United Nations area codes
    //
    
    // we have renamed ANTARCTICA here as ANTARCTIC_REGION to prevent a clash with ANTARCTICA the country
    enum Region : short { NOREGION = 0, ANTARCTIC_REGION = 1, AFRICA = 2, OCEANIA = 9, AMERICAS = 19, ASIA = 142, EUROPE = 150 };
         
     // we have renamed ANTARCTICA here as ANTARCTIC_SUBREGION to prevent a clash with with ANTARCTICA the country or region
    enum Subregion : short 
    {
        NOSUBREGION = 0,
        ANTARCTIC_SUBREGION  = 1, SOUTH_AMERICA = 5, WESTERN_AFRICA = 11, CENTRAL_AMERICA = 13, EASTERN_AFRICA = 14,
        NORTHERN_AFRICA = 15, MIDDLE_AFRICA = 17, SOUTHERN_AFRICA = 18, NORTHERN_AMERICA = 21, CARIBBEAN = 29,
        EASTERN_ASIA = 30, SOUTHERN_ASIA = 34, SOUTH_EASTERN_ASIA = 35, SOUTHERN_EUROPE = 39, AUSTRALIA_NEW_ZEALAND = 53,
        MELANESIA = 54, MICRONESIA = 57, POLYNESIA = 61, CENTRAL_ASIA = 143, WESTERN_ASIA = 145,
        EASTERN_EUROPE = 151, NORTHERN_EUROPE = 154, WESTERN_EUROPE = 155 
    };
    
    Gazetteer( void )=default;
    ~Gazetteer( void )=default;

    //
    // Countries
    //
    Country
    country( const MarketId &mic ) const;
    
    Country
    country( const City &cty ) const;

    Country 
    country( const Currency &c ) const;
    
    std::vector<Country>
    countries( const Currency &c ) const;
    
    std::vector<Country>
    countries( const std::string &pattern ) const;

    
    //
    // Currencies
    //
    Currency // the main/principal ccy for this market
    ccy( const MarketId &mic ) const { return ccy( country( mic ) ); }

    Currency // the main/principal ccy for this city
    ccy( const City &cty ) const { return ccy( country( cty ) ); }
    
    Currency // the main/principal ccy for this country
    ccy( const Country &cid ) const;
    
    std::vector<Currency> // all the ccys for this country
    ccys( const Country &cid ) const; 
    
    
    //
    // Cities and Markets
    //
    City
    capital( const Country &cid ) const;
    
    City
    city( const MarketId &mic ) const;
    
    std::vector<City>
    cities( const Country &cid ) const; 
    
    std::vector<City>
    cities( const std::string &pattern ) const;
    
    std::vector<MarketId>
    markets( const City &cty ) const;
        
    std::vector<MarketId>
    markets( const Country &cid ) const; 

    
    //
    // Regions
    //
    std::vector<Country>
    region( Region rid ) const;

    std::string
    regionName( Region rid ) const;
    
    std::vector<Region> // all regions;
    region( void ) const;
    
    Region
    region( const Country &cid ) const;
    
    Region
    region( const City &cty ) const;
    
    Region
    region( const MarketId &mic ) const;
    
    
    //
    // Subregions
    //
    std::vector<Country>
    subregion( Subregion rid ) const;
    
    std::string 
    subregionName( Subregion rid ) const;
    
    std::vector<Subregion> // all subregions; 
    subregion( void ) const;
    
    std::vector<Subregion> // all subregions of region rid
    subregion( Region rid ) const;
    
    Subregion
    subregion( const Country &cid ) const;
    
    Subregion
    subregion( const City &cty ) const;
    
    Subregion
    subregion( const MarketId &mic  ) const;
        
private:

    static const short m_cty2cid[City::NUMCITY]; 
    static const short m_cid2ccy[Country::NUMCOUNTRY];
    static const short m_cid2cap[Country::NUMCOUNTRY];
    static const short m_mic2cty[MarketId::NUMMARKETID]; 
    static const short m_ccy2cid[Currency::NUMCURRENCY];
    
    static const short * const m_ccy2cids[Currency::NUMCURRENCY];
    static const short * const m_cid2ccys[Country::NUMCOUNTRY];
    static const short * const m_cid2ctys[Country::NUMCOUNTRY];  
    static const short * const m_cty2mics[City::NUMCITY];
    
    static const short * const m_reg2cid[7];
    static const short * const m_subreg2cid[24];
    static const short * const m_reg2subreg[7];
    
    static const unsigned char m_region[Country::NUMCOUNTRY];
    static const unsigned char m_subregion[Country::NUMCOUNTRY];
};



#endif



