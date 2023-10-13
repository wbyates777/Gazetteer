# Gazetteer

The Gazetteer class is a geographic dictionary or index for financial markets supporting ISO 4217, ISO 3166-1, and ISO 10383.
It allows the identification and location of exchanges, trading platforms, regulated or non-regulated markets and trade reporting facilities. 

Gazetteer provides four lightweight, standalone identification classes: Country, Currency, MarketId and City.
Country, Currency, and MarketId implement the ISO standards: ISO 3166-1 for country identification, ISO 4217 for currencies and
ISO 10383 for market identification. The City class employs a small subset of IATA codes for city identification,
and positional information (latitude/longtitude) for geolocation and distance.
 
See Countries  - https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
    Currencies - https://en.wikipedia.org/wiki/ISO_4217 
    MICs       - https://en.wikipedia.org/wiki/Market_Identifier_Code 
    Cities     - https://en.wikipedia.org/wiki/IATA_airport_code 
    
The code compiles under c++11, c++14, c++17 and c++20, and has no external dependencies.

The following code:

    market.setMarketId( "XNYS" ); 

    std::cout << "market    : " << market << std::endl;
    std::cout << "name      : " << market.name() << std::endl;
    std::cout << "id        : " << short(market) << std::endl; 
    std::cout << "currency  : " << g.ccy(market).name() << " (" << g.ccy(market) << ")" << std::endl;
    std::cout << "city      : " << g.city(market).name() << " (" << g.city(market) << ")" << std::endl; 
    std::cout << "country   : " << g.country(market).name() << " (" << g.country(market) << ")" << std::endl;
    std::cout << "region    : " << g.regionName(g.region(market)) << std::endl; 
    std::cout << "subregion : " << g.subRegionName(g.subRegion(market)) << std::endl << std::endl;

produces the output:

    market    : XNYS
    name      : NEW YORK STOCK EXCHANGE, INC.
    id        : 2639
    currency  : US Dollar (USD)
    city      : New York (NYC)
    country   : United States (USA)
    region    : Americas
    subregion : Northern America


