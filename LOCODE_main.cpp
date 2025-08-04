/* Gazetteer Demo 03/06/2025

 $$$$$$$$$$$$$$$$$$$
 $   Gazetteer.h   $
 $$$$$$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:

UN/LOCODEs
 
 see https://unece.org/trade/uncefact/unlocode 
 
 see https://en.wikipedia.org/wiki/UN/LOCODE 
 
 see https://unece.org/trade/publications/recommendation-ndeg16-united-nations-code-trade-and-transport-locations
 
*/

#include <iostream>


#ifndef __NAME_H__
#include "Name.h"
#endif

#ifndef __GEOCOORD_H__
#include "GeoCoord.h"
#endif

#ifndef __LOCODE_H__
#include "Locode.h"
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


void
search_pos( Locode city, Locode::Function criteria, double radius )
// radius in km
{
    int count = 0;
    for( int i = 1; i < LOCODE::MAXLOCODE; ++i)
    {
        Locode code(i);
       
        if (code.valid_pos() && code.has(criteria)) // quick check; could use city.country() == code.country();
        {
            count++;
            if (GeoCoord::dist(city.pos(), code.pos()) < radius * 1000.0)
            {
                std::cout << code.locode() << " " << code.name() << " has " << Locode::toString(criteria)<< std::endl;
            }
        }
    }
}


std::vector<std::string>
match_name( const std::string &name, int error = 3 )
{
    std::vector<std::string> retVal;
    
    for( int i = 1; i < LOCODE::MAXLOCODE; ++i)
    {
        Locode code(i);
        
        if (name == code.name()) 
        {
            retVal.push_back(code.name());
        }
        else if (error > 0 && code.name().size() > name.size() / 2) // filter out short matches....
        {
            int dist = Name::dist(code.name(), name);
            
            if (dist <= error)
            {
                retVal.push_back(code.name());
                std::cout << name <<  " - " << code.name() << " - " << dist << std::endl;
            }
        }
    }
    return retVal;
}


void
demoLOCODE( void )
{
    Locode nyc(LOCODE::USNYC);
    std::cout << nyc.locode() << " " << (int) nyc << " "  << nyc.name() << " " << nyc.lat() << " " << nyc.lon() << std::endl;
    
 
    // all cities are locodes; not all locodes are cities though -- check for City::XXX
    Locode lon("GBLON");
    
    std::cout << lon.locode() << " " << (int) lon << " "  << lon.name() << " " << lon.lat() << " " << lon.lon() << std::endl;
   


    Locode::Function feature  = Locode::AIRPORT;
    std::string feature_str = Locode::toString(feature);
    if (lon.has(feature) )
        std::cout  << lon.name() << " has " << feature_str << std::endl;
    else std::cout  << lon.name() << " does not have " << feature_str << std::endl;
    
    double radius = 50.0;
    std::cout << "\nSearch for " << feature_str << " that are "   << radius << " km from " << lon.name() << std::endl;
    search_pos(lon, feature, radius);
    
    std::string misspelling = "Hustone";
    std::cout << "\nMatch a location  called " << misspelling << std::endl;
    match_name(misspelling, 2);

    
    std::cout << "\n UN/LOCODE Composition" << std::endl;
    std::vector<int> status(Locode::MAXSTATUS, 0);
    std::vector<int> coords(Locode::MAXSTATUS, 0);
    int  total = 0;
    int sum = 0;
    for( int i = 0; i < LOCODE::MAXLOCODE; ++i)
    {
        Locode code(i);
        ++status[code.status()]; 
        sum++;
        if (code.valid_pos()) 
        {
            ++coords[code.status()];
            ++total;
        }
    }
   
    for( int i = 0; i < Locode::MAXSTATUS; ++i)
    {
        std::string stat = Locode::toString(Locode::Status(i));
        std::cout << stat << ", " << status[i] << ", " << coords[i] << std::endl;
    }
    std::cout  << sum << ", " << total << std::endl;
    std::cout << "Position coverage of " << 100.0 * total / double(sum) << "%" << std::endl;

}


int 
main(int argc, const char * argv[]) 
{
    std::cout << "UN/LOCODE Demo\n" << std::endl;
  
   
    demoName();
    demoGeohash();
    demoLOCODE();
   
}

//


    
    
