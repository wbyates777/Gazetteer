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

void
demoName(void)
{
    Name xxx; // must define at least once for setup()
    
    // Example 1
    
    std::string names = "M. È. Štəfánik and Č. Ibậñềz amd Đ. Wąltóṙs and W. Bṙøñe";
    
    std::cout << names << std::endl;
    names = Name::deaccent(names);
    std::cout << names << std::endl;
    
    std::vector<std::string> res =  Name::split(names, std::regex("( a.d )"));
    
    std::cout << res.size() << std::endl;
    for (int i = 0; i < res.size(); ++i)
        std::cout << '[' << res[i] << ']' << std::endl;
    
    
    // Example 2
    
    std::string test2 = " <g \"Côte d'Ivoire\"  />    ";
    std::cout << "[" <<  test2 << "]" << std::endl;
    std::cout << "[" <<  Name::deaccent(Name::unquote(Name::clip(test2, "<g", "/>"))) << "]" << std::endl;

 
   // Example 3
   
    std::string test3 = " \"Côte d'Ivoire\"  ";
    std::string test4 = " \'Côte d'Ivoire\'  ";
    std::string test5 = " 'Côte d'Ivoire'  ";
    std::string test6 = " $$Côte d'Ivoire^^  ";
    
    std::cout << "[" << test3 << "] --> [" <<  Name::clip(test3 ,"\"")        << "]" << std::endl;
    std::cout << "[" << test4 << "] --> [" <<  Name::clip(test4, "'")         << "]" << std::endl;
    std::cout << "[" << test5 << "] --> [" <<  Name::clip(test5, "\'")        << "]" << std::endl;
    std::cout << "[" << test6 << "] --> [" <<  Name::clip(test6, "$$", "^^")  << "]" << std::endl;


    // Example 4
    
    std::cout <<  Name::capitalise("I would like a cup of tea") << std::endl;
}

void
demoCity(void)
{
    City xx;
    xx.setCity( "LGW" );
    std::cout << xx << std::endl;
    std::cout << xx.name() << std::endl;
    std::cout << xx.locode() << std::endl;
    std::cout << short(xx) << std::endl;
    std::cout << xx.capital() << std::endl;
    std::cout << "latitude = " << xx.lat() << std::endl;
    std::cout << "longitude = " << xx.lon() << std::endl << std::endl;
    
    City yy;
    yy.setCity( "JFK" );
    std::cout << yy << std::endl;
    std::cout << yy.name() << std::endl;
    std::cout << yy.locode() << std::endl;
    std::cout << short(yy) << std::endl;
    std::cout << yy.capital() << std::endl;
    std::cout << "latitude = " << yy.lat() << std::endl;
    std::cout << "longitude = " << yy.lon() << std::endl << std::endl;
    
    std::cout << "The distance between " << xx.name() << " and " << yy.name() << " is " << City::dist(xx,yy) / 1000.0 << " km" << std::endl << std::endl;
    
    City zz;
    zz.setCity( "SCQ" );
    std::cout << zz << std::endl;
    std::cout << zz.name() << std::endl;
    std::cout << zz.locode() << std::endl;
    std::cout << short(zz) << std::endl;
    std::cout << zz.capital() << std::endl;
    std::cout << "latitude = " << zz.lat() << std::endl;
    std::cout << "longitude = " << zz.lon() << std::endl << std::endl;
    
    Gazetteer g;
    
    std::cout << g.ccy(zz) << std::endl;
    std::cout << g.country( zz ).name() << std::endl;
    std::cout << g.regionName(g.region( zz )) << " (" << g.region( zz ) << ")" << std::endl;
    std::cout << g.subregionName(g.subregion( zz )) << " (" << g.subregion( zz ) << ")" << std::endl;
}

void
demoCountry(void)
{
    Country w;
    w.setCountry( "QAT" );
    std::cout << w << std::endl;
    std::cout << w.name() << std::endl;
    std::cout << w.to2Code() << std::endl;
    std::cout << w.to3Code() << std::endl;
    std::cout << short(w) << std::endl; 
    std::cout  << std::endl; 
    
    Country x;
    x.setCountry( Country::US );
    std::cout << x << std::endl;
    std::cout << x.name() << std::endl;
    std::cout << x.to2Code() << std::endl;
    std::cout << x.to3Code() << std::endl;
    std::cout << short(x) << std::endl; 
    std::cout  << std::endl; 
    
    Country y;
    y.setCountry( "SGS" );
    std::cout << y << std::endl;
    std::cout << y.name() << std::endl;
    std::cout << y.to2Code() << std::endl;
    std::cout << short(y) << std::endl; 
    std::cout  << std::endl; 
    
    Country z;
    z.setCountry( "GS" );
    std::cout << z << std::endl;
    std::cout << z.name() << std::endl;
    std::cout << z.to3Code() << std::endl;
    std::cout << short(z) << std::endl;
    std::cout  << std::endl; 
    
    Country t1("KY");
    std::cout << t1.to3Code() << std::endl;
    std::cout << t1.to2Code() << std::endl;
    std::cout << t1.name() << std::endl;
    std::cout  << std::endl; 
    
    Country t2("KZ");
    std::cout << t2.to3Code() << std::endl;
    std::cout << t2.to2Code() << std::endl;
    std::cout << t2.name() << std::endl;
    std::cout  << std::endl; 
    
    Country cid;
    int iters = 1;
    
    for (int i = 0; i < iters; ++i)
    {
        int count = 0;
        for (int j = 1; j < Country::NUMCOUNTRY; ++j)
        {
            cid.setCountry( Country::index(j).to3Code() );
            std::cout << cid << ", ";
            if (++count % 20 == 0)
                std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
}

void
demoMarket(void)
{
    Gazetteer g;
    
    MarketId market;
    market.setMarketId( "XLON" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
    
    market.setMarketId( "XNYS" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
    
    market.setMarketId( "A3579" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
    
    market.setMarketId( "A360X" ); 
    std::cout << market << std::endl;
    std::cout << market.name() << std::endl;
    std::cout << short(market) << std::endl;
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
}

int 
main(int argc, const char * argv[]) 
{
    std::cout << "Gazetteer\n" << std::endl;
  
    Gazetteer g;
    
    std::cout << "The countries of  Subregion::SouthAmerica" << std::endl;
    std::vector<Country> southam = g.subregion(  Gazetteer::Subregion::SouthAmerica );
    for (Country c : southam)
        std::cout << c.name() << std::endl;
    std::cout << std::endl;

    std::cout << Currency("USD").name() << " " << g.country(Currency("USD")) << std::endl;
    std::cout << City("MAD").name() << ", " << g.country(City::MAD).name() << ", " << g.ccy( g.country(City::MAD) ).name() << std::endl;
    std::cout << Country("GBR").name() << ", " << g.capital(Country::GBR).name() << ", " << g.ccy(Country::GBR) << std::endl;     
    std::cout << Country(Country::CHE).name() << ", " << g.capital("CHE").name() << ", " << g.ccys(Country::CHE) << std::endl << std::endl;
    
    std::cout << "Spanish cities " << g.cities(Country::ESP) << std::endl << std::endl;
    std::cout << "Spanish markets " <<  g.markets(Country::ESP) << std::endl << std::endl;
    
    Currency ccy("SVC");
    std::cout << "currency of " << g.country(ccy).name() << " is " << std::endl;
    std::cout << ccy.name() << std::endl;
    std::cout << ccy.toString() << std::endl;
    std::cout << short(ccy) << std::endl;

    demoVCity();
    demoMarket();
    demoCountry();
    demoName();
    
    
