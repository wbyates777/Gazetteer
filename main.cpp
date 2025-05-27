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

#ifndef __GEOCOORD_H__
#include "GeoCoord.h"
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
    std::cout << "\nCity\n" << std::endl;
    
    City xx;
    xx.setCity( "LON" );
    std::cout << xx << std::endl;
    std::cout << xx.name() << std::endl;
    std::cout << xx.to3Code() << std::endl;
    std::cout << xx.to5Code() << std::endl;
    std::cout << xx.timezone() << std::endl;
    std::cout << short(xx) << std::endl;
    std::cout << xx.capital() << std::endl;
    std::cout << "latitude = " << xx.lat() << std::endl;
    std::cout << "longitude = " << xx.lon() << std::endl << std::endl;
    
    City yy;
    yy.setCity( "USNYC" );
    std::cout << yy << std::endl;
    std::cout << yy.name() << std::endl;
    std::cout << yy.to3Code() << std::endl;
    std::cout << yy.to5Code() << std::endl;
    std::cout << yy.timezone() << std::endl;
    std::cout << short(yy) << std::endl;
    std::cout << yy.capital() << std::endl;
    std::cout << "latitude = " << yy.lat() << std::endl;
    std::cout << "longitude = " << yy.lon() << std::endl << std::endl;
    
    std::cout << "The distance between " << xx.name() << " and " << yy.name() << " is " << GeoCoord::dist(xx.pos(),yy.pos()) / 1000.0 << " km" << std::endl << std::endl;
    
    City zz;
    zz.setCity( "SCQ" );
    std::cout << zz << std::endl;
    std::cout << zz.name() << std::endl;
    std::cout << zz.to3Code() << std::endl;
    std::cout << zz.to5Code() << std::endl;
    std::cout << zz.timezone() << std::endl;
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
    z.setCountry( "ATF" );
    std::cout << z << std::endl;
    std::cout << z.name() << std::endl;
    std::cout << z.to3Code() << std::endl;
    std::cout << short(z) << std::endl;
    std::cout  << std::endl; 
    
    Country t1("ATG");
    std::cout << t1.to3Code() << std::endl;
    std::cout << t1.to2Code() << std::endl;
    std::cout << t1.name() << std::endl;
    std::cout  << std::endl; 
    
    Country t2("YEM");
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
    std::cout << "timezone  : " << g.city(market).timezone() << std::endl;
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
    std::cout << "timezone  : " << g.city(market).timezone() << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
    
    market.setMarketId( "3579" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "timezone  : " << g.city(market).timezone() << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
    
    market.setMarketId( "XSEB" ); 
    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "timezone  : " << g.city(market).timezone() << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(market)) << std::endl;
    std::cout << "latitude  : " << g.city(market).lat() << std::endl;
    std::cout << "longitude : " << g.city(market).lon() << std::endl << std::endl;
}

void
demoGeohash(void)
{
    std::cout << "Geohash "  << std::endl << std::endl;

    double lat1 = 68.80092998;
    double lon1 = 171.91227751;
    std::string code1 = GeoCoord::geohash(lat1,lon1,12);
    std::cout << "The geohash for (" << lat1 << ", " << lon1 << ") is " << code1 << " and has length " << code1.size() << std::endl;
    std::pair<double,double> out1 = GeoCoord::geohash(code1);
    std::cout << "Position from geohash is " << out1.first << " " << out1.second << std::endl << std::endl;

    double lat2 = -66.19907004;
    double lon2 = -143.08772234;
    std::string code2 = GeoCoord::geohash(lat2,lon2,12);
    std::cout << "The geohash for (" << lat2 << ", " << lon2 << ") is " << code2 << " and has length " << code2.size() << std::endl;
    std::pair<double,double> out2 = GeoCoord::geohash(code2);
    std::cout << "Position from geohash is " << out2.first << " " << out2.second << std::endl << std::endl;
}

int 
main(int argc, const char * argv[]) 
{
    std::cout << "Gazetteer\n" << std::endl;
  
    Gazetteer g;
    
    
    std::cout << "The countries of  Subregion::SouthAmerica" << std::endl;
    std::vector<Country> southam = g.subregion(  Gazetteer::Subregion::SOUTH_AMERICA );
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
    std::cout << ccy.to3Code() << std::endl;
    std::cout << short(ccy) << std::endl;

    demoCity();
    demoMarket();
    demoCountry();
    demoName();
    demoGeohash();
    
   

}

//


    
    
