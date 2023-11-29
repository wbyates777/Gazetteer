/* Gazetteer 15/03/2012.
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Gazetteer.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History:  A geographic dictionary or index 
 
 **** Updated 29/11/2023 ****
 
 Examples
 
 Gazetteer g;
 std::cout << "Gazetter\n" << std::endl;
 
 std::cout << Currency("USD").name() << " " << g.country(Currency("USD")) << std::endl;
 std::cout << City("MAD").name() << ", " << g.country(City::MAD).name() << ", " << g.ccy( g.country(City::MAD) ).name() << std::endl;
 std::cout << Country("GBR").name() << ", " << g.capital(Country::GBR).name() << ", " << g.ccy(Country::GBR) << std::endl;     
 std::cout << Country(Country::CHE).name() << ", " << g.capital("CHE").name() << ", " << g.ccys(Country::CHE) << std::endl;
 
 std::cout << "Spanish cities " << g.city(Country::ESP) << std::endl << std::endl;
 std::cout << "Spanish markets " <<  g.market(Country::ESP) << std::endl << std::endl;
 
 std::vector<City> rs =  g.cities(std::string("D.*"));
 for (int i= 0; i < rs.size(); ++i)
    std::cout << rs[i].name() << std::endl;
 
 exit(1);
 
 */


#ifndef __GAZETTEER_H__
#define __GAZETTEER_H__

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


#include <vector>
#include <string>


class Gazetteer 
{
public:
    
    // United Nations area codes
    // we have renamed Antarctica here as AntarcticRegion  to prevent a clash with Antarctica the country
	enum Region  : short { NOREGION = 0, AntarcticRegion = 1, Africa = 2, Oceania = 9, Americas = 19, Asia = 142, Europe = 150 };
	
    // we have renamed Antarctica here as AntarcticSubregion to prevent a clash with with Antarctica the country and Region enum
	enum SubRegion  : short { NOSUBREGION = 0,
		AntarcticSubregion  = 1, SouthAmerica = 5, WesternAfrica = 11, CentralAmerica = 13, EasternAfrica = 14, 
		NorthernAfrica = 15, MiddleAfrica = 17, SouthernAfrica = 18, NorthernAmerica = 21, Caribbean = 29, 
		EasternAsia = 30, SouthernAsia = 34, SouthEasternAsia = 35, SouthernEurope = 39, AustraliaNewZealand = 53, 
		Melanesia = 54, Micronesia = 57, Polynesia = 61, CentralAsia = 143, WesternAsia = 145, 
		EasternEurope = 151, NorthernEurope = 154, WesternEurope = 155 };
    
    
    Gazetteer( void )=default;
    ~Gazetteer( void )=default;

    
    //
    // Countries
	//
    Country
	country( const MarketId& mic ) const;
    
	Country
	country( const City& cty ) const;

    Country 
	country( const Currency& c ) const;
    
	std::vector<Country>
	countries( const Currency& c ) const;
    
    std::vector<Country>
    countries( const std::string& pattern ) const;

    
    //
    // Currencies
    //
    Currency // the main/principle ccy for this market
	ccy( const MarketId& mic ) const { return ccy( country( mic ) ); }

    Currency // the main/principle ccy for this city
	ccy( const City& cty ) const { return ccy( country( cty ) ); }
    
	Currency // the main/principle ccy for this country
	ccy( const Country& cid ) const;
    
    std::vector<Currency> // all the ccys for this country
	ccys( const Country& cid ) const; // needs more testing
    
    
    //
    // Cities and Markets
    //
	City
	capital( const Country& cid ) const;
	
	City
	city( const MarketId& mic ) const;
	
    std::vector<City>
	city( const Country& cid ) const; 
    
    std::vector<City>
    cities( const std::string& regex_pattern ) const;
    
    std::vector<MarketId>
	market( const City& cty ) const;
    	
	std::vector<MarketId>
	market( const Country& cid ) const; 

    
    //
    // Regions
    //
    std::vector<Country>
    region( const Region rid ) const;

    std::string
    regionName( const Region rid ) const;
    
    
    std::vector<Region> // all regions;
    region( void ) const;
    
    Region
    region( const Country& cid ) const;
    
    Region
    region( const City& cty ) const;
    
    Region
    region( const MarketId& mic ) const;
    
    
    //
    // Sub Regions
    //
	std::vector<Country>
	subRegion( const SubRegion rid ) const;
    
    std::string // slow
    subRegionName( const SubRegion rid ) const;
    
    std::vector<SubRegion> // all sub regions; 
    subRegion( void ) const;
    
    std::vector<SubRegion> // all sub regions of region rid
	subRegion( const Region rid ) const;
    
    
	SubRegion
	subRegion( const Country& cid ) const;
    
    SubRegion
	subRegion( const City& cty ) const;
    
    SubRegion
	subRegion( const MarketId& mic  ) const;
    	
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





