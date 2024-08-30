/* Gazetteer Demo 07/05/2023

 $$$$$$$$$$$$$$$$$$$
 $   Gazetteer.h   $
 $$$$$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

 Gazetteer - A geographic dictionary or index for financial markets.
 
*/

#include <iostream>

#ifndef __GAZETTEER_H__
#include "Gazetteer.h"
#endif

#ifndef __NAME_H__
#include "Name.h"
#endif

template <typename T>
std::ostream&
operator<<( std::ostream& ostr, const std::vector<T>& v )
{
    ostr << v.size() << '\n';
    int count = 1;
    
    for (typename std::vector<T>::const_iterator  i = v.begin(); i != v.end(); ++i, ++count)
    {
        ostr << *i << ' ';
        if ( (count % 20) == 0 )
            ostr << '\n';
    }

    return ostr;
}

int 
main(int argc, const char * argv[]) 
{
    std::cout << "Gazetteer\n" << std::endl;
  
    Gazetteer g;
    
    std::cout << "The countries of  SubRegion::SouthAmerica" << std::endl;
    std::vector<Country> southam = g.subRegion(  Gazetteer::SubRegion::SouthAmerica );
    for (Country c : southam)
        std::cout << c.name() << std::endl;
    std::cout << std::endl;

    std::cout << Currency("USD").name() << " " << g.country(Currency("USD")) << std::endl;
    std::cout << City("MAD").name() << ", " << g.country(City::MAD).name() << ", " << g.ccy( g.country(City::MAD) ).name() << std::endl;
    std::cout << Country("GBR").name() << ", " << g.capital(Country::GBR).name() << ", " << g.ccy(Country::GBR) << std::endl;     
    std::cout << Country(Country::CHE).name() << ", " << g.capital("CHE").name() << ", " << g.ccys(Country::CHE) << std::endl << std::endl;
    
    std::cout << "Spanish cities " << g.cities(Country::ESP) << std::endl << std::endl;
    std::cout << "Spanish markets " <<  g.markets(Country::ESP) << std::endl << std::endl;
    
    City x;
    x.setCity( "LON" );
    std::cout << x << std::endl;
    std::cout << x.name() << std::endl;
    std::cout << short(x) << std::endl;
    std::cout << x.capital() << std::endl;
    std::cout << g.ccy(x) << std::endl;
    std::cout << g.country( x ).name() << std::endl;
    std::cout << "latitude = " << x.lat() << std::endl;
    std::cout << "longitude = " << x.lon() << std::endl << std::endl;
    
    City y;
    y.setCity( "TYO" );
    std::cout << y << std::endl;
    std::cout << y.name() << std::endl;
    std::cout << short(y) << std::endl;
    std::cout << y.capital() << std::endl;
    std::cout << g.ccy(y) << std::endl;
    std::cout << "latitude = " << y.lat() << std::endl;
    std::cout << "longitude = " << y.lon() << std::endl << std::endl;
    
    std::cout << "distance between " << x.name() << " and " << y.name() << " is " << City::dist(x,y) / 1000.0 << " km" << std::endl << std::endl;
    
    MarketId market;
    market.setMarketId( "USA0" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subRegionName(g.subRegion(market)) << std::endl << std::endl;
    
    market.setMarketId( "XNYS" ); 
    std::cout << std::endl;
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subRegionName(g.subRegion(market)) << std::endl << std::endl;
    
    market.setMarketId( "A360T" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subRegionName(g.subRegion(market)) << std::endl << std::endl;

    Currency ccy("SVC");
    std::cout << "currency of " << g.country(ccy).name() << " is " << std::endl;
    std::cout << ccy.name() << std::endl;
    std::cout << ccy.toString() << std::endl;
    std::cout << short(ccy) << std::endl;

    // 
    //
    //
 
    Name xxx; // must define at least one for setup()
 
    std::string test1 = "M. R. Štəfánik and Č. Ibậñềz amd D. Wąlts and W. Bṙøñel";
    
    std::cout << std::endl;
    std::cout << test1 << std::endl;
    std::cout << Name::deaccent(test1) << std::endl;
    std::cout << std::endl;
    
    std::vector<std::string> results =  Name::split(Name::deaccent(test1), std::regex("( a.d )"));
    for (auto &str : results)
        std::cout << str << std::endl;
    std::cout << std::endl;
 
    std::string test2 = " \"Côte d'Ivoire\"  ";
    std::string test3 = " \'Côte d'Ivoire\'  ";
    std::string test4 = " 'Côte d'Ivoire'  ";
    std::string test5 = " $$Côte d'Ivoire$$  ";
    
    std::cout << "[" << test2 << "] --> [" <<  Name::deaccent(Name::clip(test2 ,"\""))      << "]" << std::endl;
    std::cout << "[" << test3 << "] --> [" <<  Name::clip(test3, "'")       << "]" << std::endl;
    std::cout << "[" << test4 << "] --> [" <<  Name::clip(test4, "\'")      << "]" << std::endl;
    std::cout << "[" << test5 << "] --> [" <<  Name::clip(test5, "$$")  << "]" << std::endl;
    
    std::cout <<  Name::capitalise("I would like a cup of tea") << std::endl;
}
