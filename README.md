# Gazetteer

The Gazetteer class is a geographic dictionary or index for financial markets supporting ISO 4217, ISO 3166-1, and ISO 10383.
It allows the identification and location of exchanges, trading platforms, regulated or non-regulated markets and trade reporting facilities. 

Gazetteer provides four lightweight, standalone identification classes: Country, Currency, MarketId and City.
Country, Currency, and MarketId implement the ISO standards: ISO 3166-1 for country identification, ISO 4217 for currencies, and
ISO 10383 for market identification. The City class employs IATA and UN/LOCODE codes for city identification,
and latitude and longtitude for geolocation and distance.
 
See: 

    Countries  - https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
    Currencies - https://en.wikipedia.org/wiki/ISO_4217 
    MICs       - https://en.wikipedia.org/wiki/Market_Identifier_Code 
    Cities     - https://en.wikipedia.org/wiki/IATA_airport_code, and
                 https://en.wikipedia.org/wiki/UN/LOCODE 

The code depends soley on the standard template library STL

The following code:

    Gazetteer g;
    MarketId m;
    
    m.setMarketId( "XNYS" ); 

    std::cout << "market    : " << m << std::endl;
    std::cout << "name      : " << m.name() << std::endl;
    std::cout << "id        : " << short(m) << std::endl; 
    std::cout << "currency  : " << g.ccy(m).name() << " (" << g.ccy(m) << ")" << std::endl;
    std::cout << "city      : " << g.city(m).name() << " (" << g.city(m) << ")" << std::endl; 
    std::cout << "country   : " << g.country(m).name() << " (" << g.country(m) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(m)) << std::endl; 
    std::cout << "subregion : " << g.subRegionName(g.subRegion(m)) << std::endl << std::endl;
    std::cout << "LOCODE    : " << g.city(m).locode() << std::endl << std::endl;

    City x, y;

    x.setCity( "LON" );
    y.setCity( "NYC" );

    std::cout << "The distance between " << x.name() << " and " << y.name() << " is " << City::dist(x,y) / 1000.0 << " km" << std::endl << std::endl;

produces the output:

    market    : XNYS
    name      : NEW YORK STOCK EXCHANGE, INC.
    id        : 2639
    currency  : US Dollar (USD)
    city      : New York (NYC)
    country   : United States (USA)
    region    : Americas
    subregion : Northern America
    LOCODE    : USNYC

    The distance between London and New York is 5579.86 km


Please report any errors, omissions, or suggested extensions to the email above.
