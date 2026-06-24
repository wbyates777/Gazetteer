# Gazetteer

Gazetteer is a lightweight C++20 reference library for financial market and geographic identifiers.

It provides mappings between:
- 3103 Market Identifier Codes (MIC, ISO 10383),
- 256 Country codes (ISO 3166-1),
- 247 Currency codes (ISO 4217),
- 1981 City identifiers (IATA / UN/LOCODE),
- 599 Time zones (IANA), and
- Geographic coordinates and distance calculations.

Gazetteer uses compile-time static lookup tables making it suitable for latency-sensitive financial systems.

### Key Features
- no heap allocations,
- no hashing,
- no static initialisation order issues,
- predictable memory layout,
- excellent cache locality,
- constant binary size, and
- depends solely on STL.

---

## Background 

The Gazetteer class is a geographic dictionary or index for financial markets supporting ISO 4217, ISO 3166-1, and ISO 10383.
It allows the identification and location of exchanges, trading platforms, regulated or non-regulated markets and trade reporting facilities. 

Gazetteer provides four lightweight, standalone identification classes: Country, Currency, MarketId and City.
Country, Currency, and MarketId implement the ISO standards: ISO 3166-1 for country identification, ISO 4217 for currencies, and
ISO 10383 for market identification. The City class employs IATA and UN/LOCODE codes for city identification, IANA time zones for local time calculation, 
and latitude and longitude for geolocation. The helper class GeoCoord implements the public domain geocode system 'Geohash' for encoding and decoding geographical positions as strings, and the Vincenty metric for calculating distances (in metres) between geographical points.

The Gazetteer, Currency, Country, MarketId, and City classes are machine generated; a user is not supposed to edit these classes directly.

The code depends solely on the standard template library STL.

The following code:

```cpp
#include "Gazetteer.h"

int 
main( void )
{
    Gazetteer g;
    MarketId m;
    
    m.setMarketId( "XNYS" ); 

    std::cout << "market    : " << m << std::endl;
    std::cout << "name      : " << m.name() << std::endl;
    std::cout << "id        : " << short(m) << std::endl; 
    std::cout << "currency  : " << g.ccy(m).name() << " (" << g.ccy(m) << ")" << std::endl;
    std::cout << "city      : " << g.city(m).name() << ", " << g.city(m) << " (" << g.city(m).subdiv() << ")" << std::endl; 
    std::cout << "country   : " << g.country(m).name() << " (" << g.country(m) << ")" << std::endl;
    std::cout << "timezone  : " << g.city(m).timezone()  << std::endl; 
    std::cout << "region    : " << g.regionName(g.region(m)) << std::endl; 
    std::cout << "subregion : " << g.subregionName(g.subregion(m)) << std::endl;
    std::cout << "LOCODE    : " << g.city(m).locode() << std::endl << std::endl;

    City x, y;

    x.setCity( "LON" );
    y.setCity( "NYC" );

    std::cout << "The distance between " << x.name() << " and " << y.name() << " is " << GeoCoord::dist(x.pos(),y.pos()) / 1000.0 << " km" << std::endl;
    std::cout << "The Geohash for position (57.64911, 10.40744) is " << GeoCoord::geohash(57.64911, 10.40744, 11) << std::endl << std::endl;
    
    return EXIT_SUCCESS;
}
```
    
produces the output:

    market    : XNYS
    name      : NEW YORK STOCK EXCHANGE, INC.
    id        : 2717
    currency  : US Dollar (USD)
    city      : New York, NYC (NY)
    country   : United States (USA)
    timezone  : America/New_York
    region    : Americas
    subregion : Northern America
    LOCODE    : USNYC

    The distance between London and New York is 5579.86 km
    The Geohash for position (57.64911, 10.40744) is u4pruydqqvj 


 ## Build Instructions
 

On a platform that supports cmake you can use the CMakeList.txt file included in this project. Simply cd to the directory where you have saved this project and enter:

  ```mkdir build ; cd build ; cmake .. ; make ```

 Although we have specified C++20, the code will compile under C++17 if you replace std::format with std::snprintf (in GeoCoord.cpp).

## Copyright and Usage

Copyright © W.B. Yates. All rights reserved.

This repository is provided without an open-source license. You are welcome to view, download, and use the source code for personal, educational, and academic purposes.

Commercial use, redistribution, modification for redistribution, or incorporation into other software projects without prior written permission from the copyright holder is not permitted.

If you would like to use this software beyond the terms described above, please contact the copyright holder to discuss obtaining permission.

## Contributing

Bug reports and corrections to reference data are welcome.

For changes to financial or geographic mappings, please include references.

## Links


Countries   - https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3 <br/>
Currencies  - https://en.wikipedia.org/wiki/ISO_4217 <br/>
MICs        - https://en.wikipedia.org/wiki/Market_Identifier_Code <br/>
IATA        - https://en.wikipedia.org/wiki/IATA_airport_code <br/>
UN/LOCODE   - https://en.wikipedia.org/wiki/UN/LOCODE <br/>
IANA        - https://www.iana.org/time-zones <br/>
Geohash     - https://en.wikipedia.org/wiki/Geohash <br/>

Stars welcome. Please click here:  
 ![GitHub stars](https://img.shields.io/github/stars/wbyates777/Gazetteer?style=social)
![GitHub stars](https://img.shields.io/github/stars/wbyates777/Gazetteer.svg?style=social)       
