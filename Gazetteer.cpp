/* Gazetteer 15/03/2012.
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Gazetteer.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History:  

 This code depends on the fact that City, MarketId, Currency and Country can be converted to a short
 
 Although these tables are generated automatically one should be aware of a number of issues:
 
 i)   Countries that no longer exist
 ii)  Currencies that are no longer valid 
 iii) New currencies
 iv)  Currency Unions
 v)   Dependant Territories
 
 */

#ifndef __GAZETTEER_H__
#include "Gazetteer.h"
#endif

#include <regex>


//
// Countries
//
Country
Gazetteer::country( const City& cty ) const
{
    return Country::CountryCode(m_cty2cid[City::index(cty)]);
}

Country
Gazetteer::country( const MarketId& mic ) const
{
    return country(city(mic));
}

Country 
Gazetteer::country( const Currency& ccy ) const
{
    return Country::CountryCode(m_ccy2cid[Currency::index(ccy)]);    
}

std::vector<Country>
Gazetteer::countries( const Currency& ccy ) const
{ 
    // NB the cast here depends on Country using short as an underlying type
    const Country *cid = reinterpret_cast<const Country*>(m_ccy2cids[Currency::index(ccy)]);
    return std::vector<Country>(cid + 1, cid + 1 + *cid);
}

std::vector<Country>
Gazetteer::countries( const std::string& pattern ) const
{ 
    std::regex ex(pattern);
    std::vector<Country> match;
    for (int j = 1; j < Country::NUMCOUNTRY; ++j)
    {
        if ( std::regex_match(Country::index(j).name(), ex) )
            match.push_back(Country::index(j));
    }
    return match;
}


//
// Currencies
//
Currency // the main/principal ccy for this country
Gazetteer::ccy( const Country& cid ) const
{
    return Currency::CurrencyCode(m_cid2ccy[Country::index(cid)]);
}

std::vector<Currency>
Gazetteer::ccys( const Country& cid ) const
{ 
    // NB the cast here depends on Currency using short as an underlying type
    const Currency *ccy = reinterpret_cast<const Currency*>(m_cid2ccys[Country::index(cid)]);
    return std::vector<Currency>(ccy + 1, ccy + 1 + *ccy);
}


//
// Cities 
//
City
Gazetteer::capital( const Country& cid ) const
{
    return City::CityCode(m_cid2cap[Country::index(cid)]);
}

City
Gazetteer::city( const MarketId& mic ) const
{
    return City::CityCode(m_mic2cty[MarketId::index(mic)]);    
}

std::vector<City>
Gazetteer::cities( const Country& cid ) const
{
    // NB the cast here depends on City using short as an underlying type
    const City *city = reinterpret_cast<const City*>(m_cid2ctys[Country::index(cid)]);
    return std::vector<City>(city + 1, city + 1 + *city);
}

std::vector<City>
Gazetteer::cities( const std::string& pattern ) const
{ 
    std::regex ex(pattern);
    std::vector<City> match;
    for (int j = 1; j < City::NUMCITY; ++j)
    {
        if ( std::regex_match(City::index(j).name(), ex) )
            match.push_back(City::index(j));
    }
    return match;
}


//
// Markets
//
std::vector<MarketId>
Gazetteer::markets( const City& cty ) const
{
    const MarketId *market = reinterpret_cast<const MarketId*>(m_cty2mics[City::index(cty)]);
    return std::vector<MarketId>(market + 1, market + 1 + *market);
}

std::vector<MarketId>
Gazetteer::markets( const Country& cid ) const
{
    std::vector<City> cty = cities( cid );

    std::vector<MarketId> retVal;
    for (int i = 0; i < cty.size(); ++i)
    {
        std::vector<MarketId> m = markets( cty[i] );
        for (int j = 0; j < m.size(); ++j)
        {
            // some cities have no markets at the moment
            if (m[j] != MarketId::XXXX && m[j] != MarketId::XXX0)
            {
                retVal.push_back( m[j] );
            }
        }
    }

    //std::sort( retVal.begin(), retVal.end() );
    return retVal;
}


//
// Regions
//
std::vector<Country>
Gazetteer::region( const Region rid ) const
{
    int index = 0;
    switch (rid)
    {
        case NOREGION:         index = 0; break;
        case ANTARCTIC_REGION: index = 1; break;
        case AFRICA:           index = 2; break;
        case OCEANIA:          index = 3; break;
        case AMERICAS:         index = 4; break;
        case ASIA:             index = 5; break;
        case EUROPE:           index = 6; break;
        default:               index = 0; break;
    }

    const Country *cid = reinterpret_cast<const Country*>(m_reg2cid[index]);
    return std::vector<Country>(cid + 1, cid + 1 + *cid);
}

std::vector<Gazetteer::Region>
Gazetteer::region( void ) const
{
    return std::vector<Region>({ ANTARCTIC_REGION, AFRICA, OCEANIA, AMERICAS, ASIA, EUROPE });   
}

std::string
Gazetteer::regionName( Region r ) const 
{
    switch (r)
    {
        case NOREGION:         return "No Region"; break;
        case ANTARCTIC_REGION: return "Antarctic Region"; break;
        case AFRICA:           return "Africa"; break;
        case OCEANIA:          return "Oceania"; break;
        case AMERICAS:         return "Americas"; break;
        case ASIA:             return "Asia"; break;
        case EUROPE:           return "Europe"; break;
        default:               return "No Region"; break;
    }  
}

Gazetteer::Region
Gazetteer::region( const Country& cid ) const 
{
    return Region(m_region[Country::index(cid)]);    
}

Gazetteer::Region
Gazetteer::region( const City& cty ) const 
{
    return Region(m_region[Country::index(country(cty))]);    
}

Gazetteer::Region
Gazetteer::region( const MarketId& mic ) const 
{
    return Region(m_region[Country::index(country(mic))]);    
}


//
// Sub Regions
//
std::vector<Gazetteer::Subregion>
Gazetteer::subregion( Region rid ) const
{
    int index = 0;
    switch (rid)
    {
        case NOREGION:         index = 0; break;
        case ANTARCTIC_REGION: index = 1; break;
        case AFRICA:           index = 2; break;
        case OCEANIA:          index = 3; break;
        case AMERICAS:         index = 4; break;
        case ASIA:             index = 5; break;
        case EUROPE:           index = 6; break;
        default:               index = 0; break;
    }
    
    const Subregion *subr = reinterpret_cast<const Subregion*>(m_reg2subreg[index]);
    return std::vector<Subregion>(subr + 1, subr + 1 + *subr);
}

std::vector<Country>
Gazetteer::subregion( Subregion rid ) const
{
    int index = 0;
    switch (rid)
    {
         case NOSUBREGION:           index = 0; break;
         case ANTARCTIC_SUBREGION :  index = 1; break;
         case SOUTH_AMERICA:         index = 2; break;
         case WESTERN_AFRICA:        index = 3; break;
         case CENTRAL_AMERICA:       index = 4; break;
         case EASTERN_AFRICA:        index = 5; break;
         case NORTHERN_AFRICA:       index = 6; break;
         case MIDDLE_AFRICA:         index = 7; break;
         case SOUTHERN_AFRICA:       index = 8; break;
         case NORTHERN_AMERICA:      index = 9; break;
         case CARIBBEAN:             index = 10; break;
         case EASTERN_ASIA:          index = 11; break;
         case SOUTHERN_ASIA:         index = 12; break;
         case SOUTH_EASTERN_ASIA:    index = 13; break;
         case SOUTHERN_EUROPE:       index = 14; break;
         case AUSTRALIA_NEW_ZEALAND: index = 15; break;
         case MELANESIA:             index = 16; break;
         case MICRONESIA:            index = 17; break;
         case POLYNESIA:             index = 18; break;
         case CENTRAL_ASIA:          index = 19; break;
         case WESTERN_ASIA:          index = 20; break;
         case EASTERN_EUROPE:        index = 21; break;
         case NORTHERN_EUROPE:       index = 22; break;
         case WESTERN_EUROPE:        index = 23; break;
         default:                    index = 0; break;
    }

    const Country *cid = reinterpret_cast<const Country*>(m_subreg2cid[index]);
    return std::vector<Country>(cid + 1, cid + 1 + *cid);
}

std::vector<Gazetteer::Subregion>
Gazetteer::subregion( void ) const
{
    return std::vector<Subregion>({
         ANTARCTIC_SUBREGION,
         SOUTH_AMERICA,
         WESTERN_AFRICA,
         CENTRAL_AMERICA,
         EASTERN_AFRICA,
         NORTHERN_AFRICA,
         MIDDLE_AFRICA,
         SOUTHERN_AFRICA,
         NORTHERN_AMERICA,
         CARIBBEAN,
         EASTERN_ASIA,
         SOUTHERN_ASIA,
         SOUTH_EASTERN_ASIA,
         SOUTHERN_EUROPE,
         AUSTRALIA_NEW_ZEALAND,
         MELANESIA,
         MICRONESIA,
         POLYNESIA,
         CENTRAL_ASIA,
         WESTERN_ASIA,
         EASTERN_EUROPE,
         NORTHERN_EUROPE,
         WESTERN_EUROPE
    });   
}

std::string
Gazetteer::subregionName( Subregion sr ) const 
{
    switch (sr)
    {
        case NOSUBREGION:           return "No Subregion"; break;
        case ANTARCTIC_SUBREGION :  return "Antarctic Subregion"; break; // move this to end; least likely subregion
        case SOUTH_AMERICA:         return "South America"; break;
        case WESTERN_AFRICA:        return "Western Africa"; break;
        case CENTRAL_AMERICA:       return "Central America"; break;
        case EASTERN_AFRICA:        return "Eastern Africa"; break;
        case NORTHERN_AFRICA:       return "Northern Africa"; break;
        case MIDDLE_AFRICA:         return "Middle Africa"; break;
        case SOUTHERN_AFRICA:       return "Southern Africa"; break;
        case NORTHERN_AMERICA:      return "Northern America"; break;
        case CARIBBEAN:             return "Caribbean"; break;
        case EASTERN_ASIA:          return "Eastern Asia"; break;
        case SOUTHERN_ASIA:         return "Southern Asia"; break;
        case SOUTH_EASTERN_ASIA:    return "South-Eastern Asia"; break;
        case SOUTHERN_EUROPE:       return "Southern Europe"; break;
        case AUSTRALIA_NEW_ZEALAND: return "Australia and New Zealand"; break;
        case MELANESIA:             return "Melanesia"; break;
        case MICRONESIA:            return "Micronesia"; break;
        case POLYNESIA:             return "Polynesia"; break;            
        case CENTRAL_ASIA:          return "Central Asia"; break;
        case WESTERN_ASIA:          return "Western Asia"; break;
        case EASTERN_EUROPE:        return "Eastern Europe"; break;
        case NORTHERN_EUROPE:       return "Northern Europe"; break;
        case WESTERN_EUROPE:        return "Western Europe"; break;
        default:                    return "No Subregion"; break;
    }    
}

Gazetteer::Subregion
Gazetteer::subregion( const Country& cid ) const 
{
    return Subregion(m_subregion[Country::index(cid)]);    
}

Gazetteer::Subregion
Gazetteer::subregion( const City& cty ) const 
{    
    return Subregion(m_subregion[Country::index(country(cty))]);
}

Gazetteer::Subregion
Gazetteer::subregion( const MarketId& mic ) const 
{    
    return Subregion(m_subregion[Country::index(country(mic))]);
}

//
// region defs - should not change
//
static constexpr short e_NoRegion[2] = { 1, Country::NOCOUNTRY };
static constexpr short e_AntarcticRegion[5] = { 4, Country::ATA, Country::ATF, Country::BVT, Country::HMD };
static constexpr short e_Africa[60] = { 59, Country::AGO, Country::BDI, Country::BEN, Country::BFA, Country::BWA, Country::CAF, Country::CIV, Country::CMR, Country::COD, Country::COG, Country::COM, Country::CPV, Country::DJI, Country::DZA, Country::EGY, Country::ERI, Country::ESH, Country::ETH, Country::GAB, Country::GHA, Country::GIN, Country::GMB, Country::GNB, Country::GNQ, Country::IOT, Country::KEN, Country::LBR, Country::LBY, Country::LSO, Country::MAR, Country::MDG, Country::MLI, Country::MOZ, Country::MRT, Country::MUS, Country::MWI, Country::MYT, Country::NAM, Country::NER, Country::NGA, Country::REU, Country::RWA, Country::SDN, Country::SEN, Country::SHN, Country::SLE, Country::SOM, Country::SSD, Country::STP, Country::SWZ, Country::SYC, Country::TCD, Country::TGO, Country::TUN, Country::TZA, Country::UGA, Country::ZAF, Country::ZMB, Country::ZWE };
static constexpr short e_Oceania[28] = { 27, Country::ASM, Country::AUS, Country::CCK, Country::COK, Country::CXR, Country::FJI, Country::FSM, Country::GUM, Country::KIR, Country::MHL, Country::MNP, Country::NCL, Country::NFK, Country::NIU, Country::NRU, Country::NZL, Country::PCN, Country::PLW, Country::PNG, Country::PYF, Country::SLB, Country::TKL, Country::TON, Country::TUV, Country::VUT, Country::WLF, Country::WSM };
static constexpr short e_Americas[58] = { 57, Country::ABW, Country::AIA, Country::ARG, Country::ATG, Country::BES, Country::BHS, Country::BLM, Country::BLZ, Country::BMU, Country::BOL, Country::BRA, Country::BRB, Country::CAN, Country::CHL, Country::COL, Country::CRI, Country::CUB, Country::CUW, Country::CYM, Country::DMA, Country::DOM, Country::ECU, Country::FLK, Country::GLP, Country::GRD, Country::GRL, Country::GTM, Country::GUF, Country::GUY, Country::HND, Country::HTI, Country::JAM, Country::KNA, Country::LCA, Country::MAF, Country::MEX, Country::MSR, Country::MTQ, Country::NIC, Country::PAN, Country::PER, Country::PRI, Country::PRY, Country::SGS, Country::SLV, Country::SPM, Country::SUR, Country::SXM, Country::TCA, Country::TTO, Country::UMI, Country::URY, Country::USA, Country::VCT, Country::VEN, Country::VGB, Country::VIR };
static constexpr short e_Asia[52] = { 51, Country::AFG, Country::ARE, Country::ARM, Country::AZE, Country::BGD, Country::BHR, Country::BRN, Country::BTN, Country::CHN, Country::CYP, Country::GEO, Country::HKG, Country::IDN, Country::IND, Country::IRN, Country::IRQ, Country::ISR, Country::JOR, Country::JPN, Country::KAZ, Country::KGZ, Country::KHM, Country::KOR, Country::KWT, Country::LAO, Country::LBN, Country::LKA, Country::MAC, Country::MDV, Country::MMR, Country::MNG, Country::MYS, Country::NPL, Country::OMN, Country::PAK, Country::PHL, Country::PRK, Country::PSE, Country::QAT, Country::SAU, Country::SGP, Country::SYR, Country::THA, Country::TJK, Country::TKM, Country::TLS, Country::TUR, Country::TWN, Country::UZB, Country::VNM, Country::YEM };
static constexpr short e_Europe[52] = { 51, Country::ALA, Country::ALB, Country::AND, Country::AUT, Country::BEL, Country::BGR, Country::BIH, Country::BLR, Country::CHE, Country::CZE, Country::DEU, Country::DNK, Country::ESP, Country::EST, Country::FIN, Country::FRA, Country::FRO, Country::GBR, Country::GGY, Country::GIB, Country::GRC, Country::HRV, Country::HUN, Country::IMN, Country::IRL, Country::ISL, Country::ITA, Country::JEY, Country::LIE, Country::LTU, Country::LUX, Country::LVA, Country::MCO, Country::MDA, Country::MKD, Country::MLT, Country::MNE, Country::NLD, Country::NOR, Country::POL, Country::PRT, Country::ROU, Country::RUS, Country::SJM, Country::SMR, Country::SRB, Country::SVK, Country::SVN, Country::SWE, Country::UKR, Country::VAT };

constexpr const short * const Gazetteer::m_reg2cid[7] = { e_NoRegion, e_AntarcticRegion, e_Africa, e_Oceania, e_Americas, e_Asia, e_Europe,  }; 

static constexpr short f_NoSubregion[2] = { 1, Country::NOCOUNTRY };
static constexpr short f_AntarcticSubregion[5] = { 4, Country::ATA, Country::ATF, Country::BVT, Country::HMD };
static constexpr short f_SouthAmerica[16] = { 15, Country::ARG, Country::BOL, Country::BRA, Country::CHL, Country::COL, Country::ECU, Country::FLK, Country::GUF, Country::GUY, Country::PER, Country::PRY, Country::SGS, Country::SUR, Country::URY, Country::VEN };
static constexpr short f_WesternAfrica[18] = { 17, Country::BEN, Country::BFA, Country::CIV, Country::CPV, Country::GHA, Country::GIN, Country::GMB, Country::GNB, Country::LBR, Country::MLI, Country::MRT, Country::NER, Country::NGA, Country::SEN, Country::SHN, Country::SLE, Country::TGO };
static constexpr short f_CentralAmerica[9] = { 8, Country::BLZ, Country::CRI, Country::GTM, Country::HND, Country::MEX, Country::NIC, Country::PAN, Country::SLV };
static constexpr short f_EasternAfrica[21] = { 20, Country::BDI, Country::COM, Country::DJI, Country::ERI, Country::ETH, Country::IOT, Country::KEN, Country::MDG, Country::MOZ, Country::MUS, Country::MWI, Country::MYT, Country::REU, Country::RWA, Country::SOM, Country::SYC, Country::TZA, Country::UGA, Country::ZMB, Country::ZWE };
static constexpr short f_NorthernAfrica[9] = { 8, Country::DZA, Country::EGY, Country::ESH, Country::LBY, Country::MAR, Country::SDN, Country::SSD, Country::TUN };
static constexpr short f_MiddleAfrica[10] = { 9, Country::AGO, Country::CAF, Country::CMR, Country::COD, Country::COG, Country::GAB, Country::GNQ, Country::STP, Country::TCD };
static constexpr short f_SouthernAfrica[6] = { 5, Country::BWA, Country::LSO, Country::NAM, Country::SWZ, Country::ZAF };
static constexpr short f_NorthernAmerica[7] = { 6, Country::BMU, Country::CAN, Country::GRL, Country::SPM, Country::UMI, Country::USA };
static constexpr short f_Caribbean[29] = { 28, Country::ABW, Country::AIA, Country::ATG, Country::BES, Country::BHS, Country::BLM, Country::BRB, Country::CUB, Country::CUW, Country::CYM, Country::DMA, Country::DOM, Country::GLP, Country::GRD, Country::HTI, Country::JAM, Country::KNA, Country::LCA, Country::MAF, Country::MSR, Country::MTQ, Country::PRI, Country::SXM, Country::TCA, Country::TTO, Country::VCT, Country::VGB, Country::VIR };
static constexpr short f_EasternAsia[9] = { 8, Country::CHN, Country::HKG, Country::JPN, Country::KOR, Country::MAC, Country::MNG, Country::PRK, Country::TWN };
static constexpr short f_SouthernAsia[10] = { 9, Country::AFG, Country::BGD, Country::BTN, Country::IND, Country::IRN, Country::LKA, Country::MDV, Country::NPL, Country::PAK };
static constexpr short f_SouthEasternAsia[12] = { 11, Country::BRN, Country::IDN, Country::KHM, Country::LAO, Country::MMR, Country::MYS, Country::PHL, Country::SGP, Country::THA, Country::TLS, Country::VNM };
static constexpr short f_SouthernEurope[17] = { 16, Country::ALB, Country::AND, Country::BIH, Country::ESP, Country::GIB, Country::GRC, Country::HRV, Country::ITA, Country::MKD, Country::MLT, Country::MNE, Country::PRT, Country::SMR, Country::SRB, Country::SVN, Country::VAT };
static constexpr short f_AustraliaNewZealand[6] = { 5, Country::AUS, Country::CCK, Country::CXR, Country::NFK, Country::NZL };
static constexpr short f_Melanesia[6] = { 5, Country::FJI, Country::NCL, Country::PNG, Country::SLB, Country::VUT };
static constexpr short f_Micronesia[8] = { 7, Country::FSM, Country::GUM, Country::KIR, Country::MHL, Country::MNP, Country::NRU, Country::PLW };
static constexpr short f_Polynesia[11] = { 10, Country::ASM, Country::COK, Country::NIU, Country::PCN, Country::PYF, Country::TKL, Country::TON, Country::TUV, Country::WLF, Country::WSM };
static constexpr short f_CentralAsia[6] = { 5, Country::KAZ, Country::KGZ, Country::TJK, Country::TKM, Country::UZB };
static constexpr short f_WesternAsia[19] = { 18, Country::ARE, Country::ARM, Country::AZE, Country::BHR, Country::CYP, Country::GEO, Country::IRQ, Country::ISR, Country::JOR, Country::KWT, Country::LBN, Country::OMN, Country::PSE, Country::QAT, Country::SAU, Country::SYR, Country::TUR, Country::YEM };
static constexpr short f_EasternEurope[11] = { 10, Country::BGR, Country::BLR, Country::CZE, Country::HUN, Country::MDA, Country::POL, Country::ROU, Country::RUS, Country::SVK, Country::UKR };
static constexpr short f_NorthernEurope[17] = { 16, Country::ALA, Country::DNK, Country::EST, Country::FIN, Country::FRO, Country::GBR, Country::GGY, Country::IMN, Country::IRL, Country::ISL, Country::JEY, Country::LTU, Country::LVA, Country::NOR, Country::SJM, Country::SWE };
static constexpr short f_WesternEurope[10] = { 9, Country::AUT, Country::BEL, Country::CHE, Country::DEU, Country::FRA, Country::LIE, Country::LUX, Country::MCO, Country::NLD };

constexpr const short * const Gazetteer::m_subreg2cid[24] = {
f_NoSubregion, f_AntarcticSubregion, f_SouthAmerica, f_WesternAfrica, f_CentralAmerica, f_EasternAfrica, f_NorthernAfrica, f_MiddleAfrica, f_SouthernAfrica, f_NorthernAmerica, 
f_Caribbean, f_EasternAsia, f_SouthernAsia, f_SouthEasternAsia, f_SouthernEurope, f_AustraliaNewZealand, f_Melanesia, f_Micronesia, f_Polynesia, f_CentralAsia, 
f_WesternAsia, f_EasternEurope, f_NorthernEurope, f_WesternEurope,  };

static constexpr short g_NoRegion[2] = { 1, Gazetteer::NOREGION };
static constexpr short g_AntarcticRegion[2] = { 1, Gazetteer::ANTARCTIC_SUBREGION };
static constexpr short g_Africa[6] = { 5, Gazetteer::WESTERN_AFRICA, Gazetteer::EASTERN_AFRICA, Gazetteer::NORTHERN_AFRICA, Gazetteer::MIDDLE_AFRICA, Gazetteer::SOUTHERN_AFRICA };
static constexpr short g_Oceania[5] = { 4, Gazetteer::AUSTRALIA_NEW_ZEALAND, Gazetteer::MELANESIA, Gazetteer::MICRONESIA, Gazetteer::POLYNESIA };
static constexpr short g_Americas[5] = { 4, Gazetteer::SOUTH_AMERICA, Gazetteer::CENTRAL_AMERICA, Gazetteer::NORTHERN_AMERICA, Gazetteer::CARIBBEAN };
static constexpr short g_Asia[6] = { 5, Gazetteer::EASTERN_ASIA, Gazetteer::SOUTHERN_ASIA, Gazetteer::SOUTH_EASTERN_ASIA, Gazetteer::CENTRAL_ASIA, Gazetteer::WESTERN_ASIA };
static constexpr short g_Europe[5] = { 4, Gazetteer::SOUTHERN_EUROPE, Gazetteer::EASTERN_EUROPE, Gazetteer::NORTHERN_EUROPE, Gazetteer::WESTERN_EUROPE };

constexpr const short * const Gazetteer::m_reg2subreg[7] = { g_NoRegion, g_AntarcticRegion, g_Africa, g_Oceania, g_Americas, g_Asia, g_Europe };

constexpr unsigned char Gazetteer::m_region[Country::NUMCOUNTRY] = { NOREGION,
        AMERICAS, ASIA, AFRICA, AMERICAS, EUROPE, EUROPE, EUROPE, ASIA, AMERICAS, ASIA,
        OCEANIA, ANTARCTIC_REGION, ANTARCTIC_REGION, AMERICAS, OCEANIA, EUROPE, ASIA, AFRICA, EUROPE, AFRICA,
        AMERICAS, AFRICA, ASIA, EUROPE, ASIA, AMERICAS, EUROPE, AMERICAS, EUROPE, AMERICAS,
        AMERICAS, AMERICAS, AMERICAS, AMERICAS, ASIA, ASIA, ANTARCTIC_REGION, AFRICA, AFRICA, AMERICAS,
        OCEANIA, EUROPE, AMERICAS, ASIA, AFRICA, AFRICA, AFRICA, AFRICA, OCEANIA, AMERICAS,
        AFRICA, AFRICA, AMERICAS, AMERICAS, AMERICAS, OCEANIA, AMERICAS, ASIA, EUROPE, EUROPE,
        AFRICA, AMERICAS, EUROPE, AMERICAS, AFRICA, AMERICAS, AFRICA, AFRICA, AFRICA, EUROPE,
        EUROPE, AFRICA, NOREGION, EUROPE, OCEANIA, AMERICAS, EUROPE, EUROPE, OCEANIA, AFRICA,
        EUROPE, ASIA, EUROPE, AFRICA, EUROPE, AFRICA, AMERICAS, AFRICA, AFRICA, AFRICA,
        EUROPE, AMERICAS, AMERICAS, AMERICAS, AMERICAS, OCEANIA, AMERICAS, ASIA, ANTARCTIC_REGION, AMERICAS,
        EUROPE, AMERICAS, EUROPE, ASIA, EUROPE, ASIA, AFRICA, EUROPE, ASIA, ASIA,
        EUROPE, ASIA, EUROPE, AMERICAS, EUROPE, ASIA, ASIA, ASIA, AFRICA, ASIA,
        ASIA, OCEANIA, AMERICAS, ASIA, ASIA, ASIA, ASIA, AFRICA, AFRICA, AMERICAS,
        EUROPE, ASIA, AFRICA, EUROPE, EUROPE, EUROPE, ASIA, AMERICAS, AFRICA, EUROPE,
        EUROPE, AFRICA, ASIA, AMERICAS, OCEANIA, EUROPE, AFRICA, EUROPE, ASIA, EUROPE,
        ASIA, OCEANIA, AFRICA, AFRICA, AMERICAS, AMERICAS, AFRICA, AFRICA, ASIA, AFRICA,
        AFRICA, OCEANIA, AFRICA, OCEANIA, AFRICA, AMERICAS, OCEANIA, EUROPE, EUROPE, ASIA,
        OCEANIA, OCEANIA, ASIA, ASIA, AMERICAS, OCEANIA, AMERICAS, ASIA, OCEANIA, OCEANIA,
        EUROPE, AMERICAS, ASIA, EUROPE, AMERICAS, ASIA, OCEANIA, ASIA, AFRICA, EUROPE,
        EUROPE, AFRICA, ASIA, AFRICA, AFRICA, ASIA, AMERICAS, AFRICA, EUROPE, OCEANIA,
        AFRICA, AMERICAS, EUROPE, AFRICA, AMERICAS, EUROPE, AFRICA, AFRICA, AMERICAS, EUROPE,
        EUROPE, EUROPE, AFRICA, AMERICAS, AFRICA, ASIA, AMERICAS, AFRICA, AFRICA, ASIA,
        ASIA, OCEANIA, ASIA, ASIA, OCEANIA, AMERICAS, AFRICA, ASIA, OCEANIA, ASIA,
        AFRICA, AFRICA, EUROPE, AMERICAS, AMERICAS, AMERICAS, ASIA, EUROPE, AMERICAS, AMERICAS,
        AMERICAS, AMERICAS, ASIA, OCEANIA, OCEANIA, OCEANIA, NOREGION, NOREGION, NOREGION, NOREGION,
        NOREGION, ASIA, AFRICA, AFRICA, AFRICA
};

constexpr unsigned char Gazetteer::m_subregion[Country::NUMCOUNTRY] = { NOSUBREGION,
    CARIBBEAN, SOUTHERN_ASIA, MIDDLE_AFRICA, CARIBBEAN, NORTHERN_EUROPE, SOUTHERN_EUROPE, SOUTHERN_EUROPE, WESTERN_ASIA, SOUTH_AMERICA, WESTERN_ASIA,
    POLYNESIA, ANTARCTIC_SUBREGION , ANTARCTIC_SUBREGION , CARIBBEAN, AUSTRALIA_NEW_ZEALAND, WESTERN_EUROPE, WESTERN_ASIA, EASTERN_AFRICA, WESTERN_EUROPE, WESTERN_AFRICA,
    CARIBBEAN, WESTERN_AFRICA, SOUTHERN_ASIA, EASTERN_EUROPE, WESTERN_ASIA, CARIBBEAN, SOUTHERN_EUROPE, CARIBBEAN, EASTERN_EUROPE, CENTRAL_AMERICA,
    NORTHERN_AMERICA, SOUTH_AMERICA, SOUTH_AMERICA, CARIBBEAN, SOUTH_EASTERN_ASIA, SOUTHERN_ASIA, ANTARCTIC_SUBREGION , SOUTHERN_AFRICA, MIDDLE_AFRICA, NORTHERN_AMERICA,
    AUSTRALIA_NEW_ZEALAND, WESTERN_EUROPE, SOUTH_AMERICA, EASTERN_ASIA, WESTERN_AFRICA, MIDDLE_AFRICA, MIDDLE_AFRICA, MIDDLE_AFRICA, POLYNESIA, SOUTH_AMERICA,
    EASTERN_AFRICA, WESTERN_AFRICA, CENTRAL_AMERICA, CARIBBEAN, CARIBBEAN, AUSTRALIA_NEW_ZEALAND, CARIBBEAN, WESTERN_ASIA, EASTERN_EUROPE, WESTERN_EUROPE,
    EASTERN_AFRICA, CARIBBEAN, NORTHERN_EUROPE, CARIBBEAN, NORTHERN_AFRICA, SOUTH_AMERICA, NORTHERN_AFRICA, EASTERN_AFRICA, NORTHERN_AFRICA, SOUTHERN_EUROPE,
    NORTHERN_EUROPE, EASTERN_AFRICA, NOSUBREGION, NORTHERN_EUROPE, MELANESIA, SOUTH_AMERICA, WESTERN_EUROPE, NORTHERN_EUROPE, MICRONESIA, MIDDLE_AFRICA,
    NORTHERN_EUROPE, WESTERN_ASIA, NORTHERN_EUROPE, WESTERN_AFRICA, SOUTHERN_EUROPE, WESTERN_AFRICA, CARIBBEAN, WESTERN_AFRICA, WESTERN_AFRICA, MIDDLE_AFRICA,
    SOUTHERN_EUROPE, CARIBBEAN, NORTHERN_AMERICA, CENTRAL_AMERICA, SOUTH_AMERICA, MICRONESIA, SOUTH_AMERICA, EASTERN_ASIA, ANTARCTIC_SUBREGION , CENTRAL_AMERICA,
    SOUTHERN_EUROPE, CARIBBEAN, EASTERN_EUROPE, SOUTH_EASTERN_ASIA, NORTHERN_EUROPE, SOUTHERN_ASIA, EASTERN_AFRICA, NORTHERN_EUROPE, SOUTHERN_ASIA, WESTERN_ASIA,
    NORTHERN_EUROPE, WESTERN_ASIA, SOUTHERN_EUROPE, CARIBBEAN, NORTHERN_EUROPE, WESTERN_ASIA, EASTERN_ASIA, CENTRAL_ASIA, EASTERN_AFRICA, CENTRAL_ASIA,
    SOUTH_EASTERN_ASIA, MICRONESIA, CARIBBEAN, EASTERN_ASIA, WESTERN_ASIA, SOUTH_EASTERN_ASIA, WESTERN_ASIA, WESTERN_AFRICA, NORTHERN_AFRICA, CARIBBEAN,
    WESTERN_EUROPE, SOUTHERN_ASIA, SOUTHERN_AFRICA, NORTHERN_EUROPE, WESTERN_EUROPE, NORTHERN_EUROPE, EASTERN_ASIA, CARIBBEAN, NORTHERN_AFRICA, WESTERN_EUROPE,
    EASTERN_EUROPE, EASTERN_AFRICA, SOUTHERN_ASIA, CENTRAL_AMERICA, MICRONESIA, SOUTHERN_EUROPE, WESTERN_AFRICA, SOUTHERN_EUROPE, SOUTH_EASTERN_ASIA, SOUTHERN_EUROPE,
    EASTERN_ASIA, MICRONESIA, EASTERN_AFRICA, WESTERN_AFRICA, CARIBBEAN, CARIBBEAN, EASTERN_AFRICA, EASTERN_AFRICA, SOUTH_EASTERN_ASIA, EASTERN_AFRICA,
    SOUTHERN_AFRICA, MELANESIA, WESTERN_AFRICA, AUSTRALIA_NEW_ZEALAND, WESTERN_AFRICA, CENTRAL_AMERICA, POLYNESIA, WESTERN_EUROPE, NORTHERN_EUROPE, SOUTHERN_ASIA,
    MICRONESIA, AUSTRALIA_NEW_ZEALAND, WESTERN_ASIA, SOUTHERN_ASIA, CENTRAL_AMERICA, POLYNESIA, SOUTH_AMERICA, SOUTH_EASTERN_ASIA, MICRONESIA, MELANESIA,
    EASTERN_EUROPE, CARIBBEAN, EASTERN_ASIA, SOUTHERN_EUROPE, SOUTH_AMERICA, WESTERN_ASIA, POLYNESIA, WESTERN_ASIA, EASTERN_AFRICA, EASTERN_EUROPE,
    EASTERN_EUROPE, EASTERN_AFRICA, WESTERN_ASIA, NORTHERN_AFRICA, WESTERN_AFRICA, SOUTH_EASTERN_ASIA, SOUTH_AMERICA, WESTERN_AFRICA, NORTHERN_EUROPE, MELANESIA,
    WESTERN_AFRICA, CENTRAL_AMERICA, SOUTHERN_EUROPE, EASTERN_AFRICA, NORTHERN_AMERICA, SOUTHERN_EUROPE, NORTHERN_AFRICA, MIDDLE_AFRICA, SOUTH_AMERICA, EASTERN_EUROPE,
    SOUTHERN_EUROPE, NORTHERN_EUROPE, SOUTHERN_AFRICA, CARIBBEAN, EASTERN_AFRICA, WESTERN_ASIA, CARIBBEAN, MIDDLE_AFRICA, WESTERN_AFRICA, SOUTH_EASTERN_ASIA,
    CENTRAL_ASIA, POLYNESIA, CENTRAL_ASIA, SOUTH_EASTERN_ASIA, POLYNESIA, CARIBBEAN, NORTHERN_AFRICA, WESTERN_ASIA, POLYNESIA, EASTERN_ASIA,
    EASTERN_AFRICA, EASTERN_AFRICA, EASTERN_EUROPE, NORTHERN_AMERICA, SOUTH_AMERICA, NORTHERN_AMERICA, CENTRAL_ASIA, SOUTHERN_EUROPE, CARIBBEAN, SOUTH_AMERICA,
    CARIBBEAN, CARIBBEAN, SOUTH_EASTERN_ASIA, MELANESIA, POLYNESIA, POLYNESIA, NOSUBREGION, NOSUBREGION, NOSUBREGION, NOSUBREGION,
    NOSUBREGION, WESTERN_ASIA, SOUTHERN_AFRICA, EASTERN_AFRICA, EASTERN_AFRICA
};
//
// end region defs - should not change
//              


//
// Tables
//


// MarketId to City
constexpr short Gazetteer::m_mic2cty[MarketId::NUMMARKETID] = { 
    City::NOCITY,
    City::FRA, City::BDA, City::LON, City::FRA, City::FRA, City::JNB, City::JNB, City::PAR, City::ATH, City::EWR, 
    City::MAD, City::ATH, City::AMS, City::AMS, City::CPH, City::SOF, City::AUA, City::AUH, City::LON, City::BOM, 
    City::NYC, City::AUH, City::AUH, City::YTO, City::NYC, City::LON, City::BKK, City::EB0, City::KBL, City::AMS, 
    City::AMS, City::AMS, City::AMS, City::AMS, City::AMS, City::LON, City::MAD, City::LAD, City::AXA, City::DUB, 
    City::LON, City::BRN, City::TSE, City::MIL, City::MHQ, City::TIA, City::NYC, City::LON, City::MAD, City::NYC, 
    City::CPH, City::JNB, City::AMS, City::BRU, City::LIS, City::PAR, City::LAX, City::AMM, City::LON, City::LON, 
    City::NYC, City::ALV, City::LON, City::LON, City::LON, City::VIE, City::SIN, City::SIN, City::SYD, City::PAR, 
    City::PAR, City::PAR, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::NYC, 
    City::LON, City::LON, City::LON, City::LON, City::NYC, City::LON, City::NYC, City::NYC, City::LON, City::AUH, 
    City::DUB, City::BUE, City::LON, City::NYC, City::EVN, City::LI0, City::HOU, City::ATH, City::PPG, City::ST0, 
    City::ST0, City::ST0, City::BOM, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::PRG, City::NYC, 
    City::TF0, City::MIL, City::ANU, City::NIC, City::LON, City::NIC, City::YTO, City::PAR, City::PAR, City::CBR, 
    City::VIE, City::LON, City::FRA, City::LON, City::LON, City::MEL, City::SYD, City::HEL, City::BAK, City::MUC, 
    City::BUE, City::HKG, City::HKG, City::LON, City::HKG, City::TYO, City::BGI, City::LON, City::NYC, City::NYC, 
    City::NYC, City::LON, City::LON, City::AMS, City::VNO, City::NYC, City::LON, City::NYC, City::LON, City::LON, 
    City::AMS, City::NYC, City::HKG, City::LON, City::HKG, City::TYO, City::LON, City::LON, City::LON, City::CHI, 
    City::LON, City::CHI, City::CHI, City::DUB, City::DUB, City::SIN, City::NYC, City::NYC, City::LON, City::NYC, 
    City::LON, City::MAD, City::LON, City::NYC, City::LUX, City::SFN, City::PMI, City::MGF, City::LON, City::QLI, 
    City::MSQ, City::LON, City::LON, City::LON, City::MUC, City::BJM, City::LUX, City::SOF, City::BRU, City::SOF, 
    City::BKK, City::QPA, City::BRU, City::BRU, City::BRU, City::PT0, City::BER, City::BER, City::BER, City::BON, 
    City::BUD, City::TYO, City::LON, City::AMS, City::AMS, City::AMS, City::AMS, City::AMS, City::AMS, City::OUA, 
    City::BAH, City::LIS, City::SYD, City::LON, City::NYC, City::NYC, City::LON, City::TYO, City::LON, City::LON, 
    City::DAC, City::MIL, City::HAM, City::LON, City::HAM, City::SOF, City::LUX, City::SOF, City::SIN, City::HAM, 
    City::LON, City::BAH, City::NAS, City::NYC, City::WAW, City::NYC, City::SJJ, City::XXX, City::LUX, City::FRA, 
    City::LON, City::MEX, City::BJS, City::BRU, City::BRU, City::DUB, City::ATL, City::LON, City::LON, City::KLU, 
    City::MUC, City::BNX, City::MUC, City::MUC, City::MEL, City::MUC, City::MUC, City::NYC, City::SBH, City::LON, 
    City::LON, City::BRU, City::MSQ, City::NYC, City::LON, City::NYC, City::LUX, City::SYD, City::BCV, City::MAD, 
    City::LON, City::MAD, City::MAD, City::SBZ, City::SBZ, City::SBZ, City::LON, City::DUB, City::DUB, City::DUB, 
    City::DUB, City::LON, City::BRU, City::BDA, City::NYC, City::NYC, City::ROM, City::PAR, City::NYC, City::BRU, 
    City::NYC, City::LON, City::WAW, City::PAR, City::TYO, City::YTO, City::LON, City::NYC, City::LON, City::LON, 
    City::FRA, City::EDI, City::LPB, City::ROM, City::LON, City::NYC, City::WAW, City::LON, City::GBE, City::GBE, 
    City::VAP, City::VIX, City::FRA, City::GOA, City::WAW, City::LON, City::NYC, City::PAR, City::BSB, City::BGI, 
    City::BBU, City::BBU, City::BBU, City::TRN, City::PAR, City::LON, City::LON, City::SAO, City::BBU, City::BWN, 
    City::LON, City::QSA, City::BAK, City::MUC, City::LON, City::BOM, City::LON, City::BOS, City::AMS, City::SIN, 
    City::EWR, City::LON, City::NYC, City::AMS, City::LON, City::PBH, City::CHI, City::AMS, City::LON, City::CLJ, 
    City::TUN, City::OSL, City::OSL, City::OSL, City::LON, City::CCS, City::SAO, City::LON, City::MVD, City::NYC, 
    City::GBE, City::ZRH, City::CHI, City::CHI, City::CHI, City::BCN, City::AMS, City::NYC, City::BGF, City::NYC, 
    City::YOW, City::YTO, City::YTO, City::AMS, City::MAD, City::WRO, City::STO, City::CHI, City::FRA, City::AMS, 
    City::NYC, City::LON, City::AMS, City::LON, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, 
    City::FRA, City::NYC, City::PTG, City::LON, City::ZRH, City::BTS, City::CHI, City::CHI, City::LON, City::TSE, 
    City::CHI, City::SHA, City::CCK, City::LUX, City::LON, City::GVA, City::NYC, City::LON, City::AMS, City::AMS, 
    City::NYC, City::NYC, City::FRA, City::BOM, City::MAD, City::NYC, City::AMS, City::PRG, City::DUB, City::LON, 
    City::PRG, City::RIO, City::WAW, City::AMS, City::AMS, City::AMS, City::AMS, City::SYD, City::SHA, City::HKG, 
    City::LON, City::LCA, City::DUB, City::NYC, City::TYO, City::TAY, City::ROM, City::ROM, City::ROM, City::FRA, 
    City::FRA, City::FRA, City::ROM, City::ROM, City::SYD, City::LON, City::FRA, City::LON, City::LON, City::HKG, 
    City::NYC, City::LON, City::LON, City::LON, City::MEX, City::ROM, City::ROM, City::ROM, City::ROM, City::NYC, 
    City::BRN, City::LON, City::SYD, City::YTO, City::LON, City::SIN, City::TYO, City::LON, City::TYO, City::TYO, 
    City::LON, City::LON, City::STI, City::BJS, City::LON, City::BUD, City::LON, City::NYC, City::TYO, City::MAD, 
    City::MAD, City::MAD, City::MAD, City::MAD, City::NYC, City::NYC, City::TYO, City::TYO, City::ABJ, City::SYD, 
    City::LON, City::HKG, City::TYO, City::UTC, City::MNL, City::NYC, City::SIN, City::LON, City::MAD, City::PAR, 
    City::LON, City::LON, City::LON, City::LON, City::CHI, City::TYO, City::LON, City::YAO, City::CHO, City::LON, 
    City::SIN, City::OSL, City::PAR, City::FIH, City::CHI, City::BZV, City::CHI, City::RAR, City::BOG, City::HAH, 
    City::MLA, City::AMS, City::BUD, City::CHI, City::NYC, City::YTO, City::NYC, City::AMS, City::RAI, City::LON, 
    City::NYC, City::BLQ, City::SJO, City::STO, City::LON, City::STO, City::ZRH, City::PRG, City::SYD, City::LON, 
    City::LON, City::NYC, City::FRA, City::YTO, City::LON, City::LON, City::LON, City::DUB, City::HKG, City::LON, 
    City::TYO, City::LON, City::NYC, City::MAD, City::PRG, City::ZRH, City::LON, City::WUX, City::STO, City::NYC, 
    City::ZRH, City::CHI, City::HAV, City::NYC, City::CUR, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, 
    City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::LON, City::XCH, City::LON, City::GCM, 
    City::NIC, City::PRG, City::AMS, City::AMS, City::LON, City::HS0, City::AMS, City::FRA, City::CHI, City::CPH, 
    City::LON, City::DUB, City::NYC, City::FRA, City::LON, City::LON, City::LON, City::FRA, City::FRA, City::HKG, 
    City::LON, City::FRA, City::LUX, City::FRA, City::FRA, City::BRU, City::LON, City::NYC, City::LON, City::CPH, 
    City::CUR, City::FLR, City::DUB, City::NYC, City::FRA, City::BER, City::AUH, City::DXB, City::HEL, City::LUX, 
    City::REK, City::DXB, City::JIB, City::STO, City::STO, City::STO, City::HS0, City::STO, City::LIS, City::DCF, 
    City::MAD, City::MIL, City::CPH, City::HEL, City::REK, City::CPH, City::STO, City::SDQ, City::STO, City::OSL, 
    City::ZRH, City::MAD, City::LON, City::MAD, City::PAR, City::TYO, City::LON, City::DOH, City::CPH, City::STO, 
    City::DXB, City::DUS, City::DUS, City::DUS, City::DUS, City::LON, City::EWR, City::NYC, City::NYC, City::ALG, 
    City::FRA, City::LON, City::BUD, City::FRA, City::ROM, City::STO, City::BTS, City::ZRH, City::AMS, City::AMS, 
    City::AMS, City::LON, City::AMS, City::LON, City::ZRH, City::LON, City::BOS, City::FRA, City::SIN, City::AMS, 
    City::FRA, City::LON, City::LON, City::LON, City::UIO, City::AMS, City::DUB, City::CHI, City::CHI, City::CHI, 
    City::MCI, City::DUB, City::CHI, City::CHI, City::PAR, City::EWR, City::SIN, City::IEV, City::JNB, City::SW0, 
    City::NYC, City::VIE, City::CAI, City::LON, City::OSL, City::LON, City::OSL, City::OSL, City::OSL, City::PAR, 
    City::LON, City::BBU, City::LON, City::MIL, City::MIL, City::MIL, City::MIL, City::PCT, City::LUX, City::LON, 
    City::ATH, City::LON, City::SIN, City::TLS, City::STO, City::LON, City::FRA, City::BRU, City::LIS, City::HKG, 
    City::PAR, City::PCT, City::PCT, City::YTO, City::DUB, City::LON, City::SIN, City::SIN, City::SIN, City::DUB, 
    City::LON, City::BER, City::BER, City::BER, City::BER, City::NYC, City::BRN, City::ATH, City::FRA, City::ASM, 
    City::CHI, City::BUD, City::EUN, City::TLS, City::MAD, City::NYC, City::TLL, City::STO, City::MIL, City::ADD, 
    City::MIL, City::QLI, City::AMS, City::ALA, City::ATH, City::DUB, City::ZRH, City::SOF, City::STO, City::ZRH, 
    City::ZRH, City::ZRH, City::STR, City::STO, City::STR, City::LON, City::ANK, City::MLA, City::VIE, City::LON, 
    City::LON, City::LON, City::LON, City::MIL, City::LON, City::LON, City::LON, City::LON, City::MIL, City::NYC, 
    City::PAR, City::PAR, City::PAR, City::PAR, City::PAR, City::LON, City::PAR, City::PAR, City::IST, City::LON, 
    City::PAR, City::LON, City::NYC, City::MIL, City::CHI, City::CHI, City::ES0, City::SFO, City::FRA, City::STO, 
    City::HEL, City::NYC, City::NYC, City::DCA, City::DCA, City::DCA, City::BGO, City::LON, City::SUV, City::PSY, 
    City::AMS, City::AMS, City::AMS, City::PAR, City::SIN, City::NYC, City::CPH, City::OSL, City::TLL, City::HEL, 
    City::NYC, City::NYC, City::REK, City::NYC, City::VNO, City::RIX, City::STO, City::LON, City::NYC, City::VIE, 
    City::PAR, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::OSL, City::CHI, 
    City::FAE, City::BRU, City::LON, City::NYC, City::TOS, City::PNI, City::HEL, City::PRG, City::PRG, City::NYC, 
    City::MIA, City::NYC, City::BOM, City::NYC, City::DUB, City::LON, City::LON, City::NYC, City::LON, City::NYC, 
    City::DUB, City::DUB, City::SIN, City::BOM, City::CHI, City::LON, City::LBV, City::STO, City::EB0, City::LON, 
    City::DUB, City::SOF, City::STO, City::LON, City::TBS, City::VNO, City::NYC, City::SYD, City::LON, City::LON, 
    City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::STO, City::LON, City::PAR, 
    City::SIN, City::LON, City::LON, City::GCI, City::ACC, City::GIB, City::CKY, City::PAR, City::ZRH, City::NYC, 
    City::NYC, City::AUH, City::BBR, City::CHI, City::EWR, City::BJL, City::SOF, City::LON, City::PAR, City::PAR, 
    City::MUC, City::DXB, City::AMS, City::LON, City::NYC, City::PAR, City::LON, City::OXB, City::SSG, City::CHI, 
    City::BOS, City::QLI, City::ATH, City::GND, City::CHI, City::LON, City::LON, City::GOH, City::MAD, City::LON, 
    City::HKG, City::FRA, City::NYC, City::LON, City::GEO, City::NYC, City::NYC, City::FRA, City::LON, City::LON, 
    City::LON, City::HKG, City::PAR, City::LON, City::BJS, City::HKG, City::SEL, City::GIB, City::HKG, City::HKG, 
    City::TAY, City::NYC, City::GUA, City::NYC, City::BDA, City::EWR, City::CAY, City::GUM, City::GEO, City::HS0, 
    City::HS0, City::HS0, City::VLI, City::HAM, City::HAM, City::HAM, City::HAM, City::HAM, City::HAJ, City::HAJ, 
    City::HAJ, City::HAJ, City::HAJ, City::PAR, City::WAW, City::MIL, City::AMS, City::ATH, City::ATH, City::CHI, 
    City::FRA, City::ATH, City::ATH, City::ATH, City::HKG, City::HKG, City::MIL, City::MIL, City::TGU, City::ATH, 
    City::PAR, City::PAR, City::PAR, City::PAR, City::BOS, City::LON, City::LON, City::DUB, City::MIL, City::LON, 
    City::NYC, City::NYC, City::ZAG, City::LON, City::DUS, City::NYC, City::HAN, City::HKG, City::LON, City::PAP, 
    City::BUD, City::BUD, City::LON, City::BUD, City::BGO, City::GW0, City::LON, City::GW0, City::FLR, City::ZAZ, 
    City::SOF, City::BIO, City::FLR, City::GW0, City::NYC, City::DUB, City::SOF, City::LON, City::LON, City::BGO, 
    City::LON, City::NYC, City::JKT, City::NYC, City::LON, City::LON, City::NYC, City::LON, City::TYO, City::TYO, 
    City::PAR, City::PAR, City::LON, City::SIN, City::NYC, City::LON, City::TYO, City::EWR, City::TYO, City::LON, 
    City::TYO, City::DUB, City::NYC, City::BOM, City::DUB, City::JKT, City::NYC, City::AMS, City::LON, City::LON, 
    City::WAW, City::GW0, City::ATL, City::NYC, City::NYC, City::NYC, City::NYC, City::AUH, City::THR, City::YWG, 
    City::NYC, City::LON, City::LON, City::OSL, City::LON, City::LON, City::LON, City::LON, City::SIN, City::MLA, 
    City::NYC, City::LON, City::DUB, City::DUB, City::DUB, City::LON, City::NYC, City::GC0, City::DUS, City::LON, 
    City::ATL, City::ATL, City::NYC, City::LON, City::LON, City::NYC, City::LON, City::AMS, City::ATL, City::CHI, 
    City::AMS, City::LON, City::ATL, City::AMS, City::LON, City::THR, City::LON, City::ATL, City::LON, City::LON, 
    City::ATL, City::MIL, City::LON, City::IOM, City::LON, City::LON, City::LON, City::LON, City::NYC, City::ST0, 
    City::DEL, City::OSL, City::AMS, City::AMS, City::AMS, City::LON, City::WAW, City::GH0, City::WP0, City::LON, 
    City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::NKW, 
    City::LON, City::LON, City::AMS, City::LON, City::LON, City::LON, City::DUB, City::THR, City::BGW, City::TRN, 
    City::QIC, City::NYC, City::LON, City::REK, City::BOM, City::REK, City::TLV, City::LON, City::LON, City::LON, 
    City::LON, City::LON, City::AMS, City::AMS, City::AMS, City::LON, City::AMS, City::LON, City::ROM, City::NYC, 
    City::DUB, City::TSE, City::MIL, City::LON, City::YTO, City::LED, City::SIN, City::KIN, City::TYO, City::TYO, 
    City::SI0, City::SOF, City::TYO, City::FRA, City::LON, City::NYC, City::FRA, City::JER, City::LON, City::CHI, 
    City::AMS, City::CHI, City::LON, City::AMS, City::NYC, City::AMM, City::NYC, City::LON, City::FRA, City::LON, 
    City::HKG, City::NYC, City::NYC, City::TYO, City::LON, City::JNB, City::NYC, City::JNB, City::NYC, City::NYC, 
    City::LON, City::GR0, City::TYO, City::TYO, City::TSE, City::BRU, City::LUX, City::LUX, City::LON, City::LUX, 
    City::LUX, City::BUD, City::WAW, City::BUD, City::NBO, City::FRU, City::BUD, City::PNH, City::TRW, City::LON, 
    City::NYC, City::LON, City::ZRH, City::SKB, City::EWR, City::EWR, City::EWR, City::EWR, City::EWR, City::SEL, 
    City::PRG, City::SEL, City::PAR, City::AUH, City::KWI, City::NYC, City::NYC, City::NYC, City::NYC, City::BCT, 
    City::VTE, City::NYC, City::CPH, City::NYC, City::NYC, City::STR, City::LON, City::FRA, City::BEY, City::MLW, 
    City::SOF, City::STR, City::STR, City::TIP, City::SLU, City::PAR, City::LON, City::AMS, City::NYC, City::LON, 
    City::NYC, City::DUB, City::DUB, City::BOS, City::YTO, City::LI0, City::NYC, City::RB0, City::LON, City::LON, 
    City::LON, City::LON, City::AMS, City::NYC, City::NYC, City::CMB, City::VIE, City::LON, City::LON, City::LON, 
    City::LON, City::SIN, City::LON, City::LON, City::TLL, City::BOS, City::LON, City::PAR, City::PAR, City::LON, 
    City::LON, City::LON, City::LON, City::NYC, City::NYC, City::NYC, City::MSU, City::DUS, City::BOS, City::SFO, 
    City::VNO, City::LUX, City::RIX, City::TYO, City::YTO, City::AUH, City::MAD, City::MFM, City::SYD, City::MIL, 
    City::LON, City::SFG, City::EWR, City::LON, City::SIN, City::MLE, City::AMS, City::DUB, City::HKG, City::LON, 
    City::TYO, City::LON, City::LON, City::SYD, City::RBA, City::MAD, City::SIN, City::YTO, City::AUH, City::ESH, 
    City::OPO, City::WAW, City::LON, City::SOF, City::ROM, City::DUB, City::MCM, City::NYC, City::CPH, City::LON, 
    City::LON, City::LON, City::BOM, City::LON, City::KIV, City::TNR, City::LIS, City::MAD, City::MLE, City::SYD, 
    City::HKG, City::NYC, City::EWR, City::EWR, City::EWR, City::OSL, City::MAD, City::OSL, City::FRA, City::KUL, 
    City::LON, City::MEX, City::LON, City::LIS, City::LON, City::LON, City::LON, City::DUS, City::AMS, City::LON, 
    City::PAR, City::HEL, City::FRA, City::LON, City::MAJ, City::MAD, City::LUX, City::REK, City::AUH, City::PCT, 
    City::MOW, City::MIL, City::TYO, City::NYC, City::QLI, City::SKP, City::PAR, City::LON, City::NYC, City::PAR, 
    City::PAR, City::LON, City::PAR, City::BKO, City::LON, City::LON, City::LON, City::LON, City::MLA, City::LON, 
    City::NYC, City::BRU, City::LON, City::RGN, City::CPH, City::TGD, City::HEL, City::ULN, City::REK, City::SPN, 
    City::STO, City::NYC, City::STO, City::MIL, City::MPM, City::PCT, City::NKC, City::SYD, City::PAR, City::LON, 
    City::NYC, City::PAR, City::WAW, City::DUB, City::FRA, City::LON, City::NYC, City::NYC, City::TYO, City::PAR, 
    City::NYC, City::MNI, City::NYC, City::PAR, City::LON, City::NYC, City::STO, City::NYC, City::ROM, City::MCT, 
    City::MCT, City::MIL, City::MIL, City::MIL, City::PAR, City::FDF, City::LON, City::NYC, City::ROM, City::BRU, 
    City::BRU, City::LON, City::ROM, City::ROM, City::WAW, City::LON, City::LON, City::NYC, City::NYC, City::NYC, 
    City::NYC, City::NYC, City::NYC, City::AMS, City::LON, City::MIL, City::PAR, City::TYO, City::LON, City::MUC, 
    City::MUC, City::MUC, City::MUC, City::MRU, City::LON, City::AMS, City::LON, City::COR, City::LLW, City::LON, 
    City::AMS, City::EWR, City::KUL, City::DZA, City::LON, City::LON, City::LON, City::LON, City::LON, City::PAR, 
    City::LON, City::WDH, City::MOW, City::STO, City::VNO, City::NYC, City::STO, City::LOS, City::PAR, City::LON, 
    City::LON, City::NYC, City::IDR, City::DCA, City::KHI, City::NOU, City::FRA, City::LON, City::LON, City::AMS, 
    City::AMS, City::NTL, City::BJS, City::YTO, City::YTO, City::YTO, City::YTO, City::NIM, City::FRA, City::LON, 
    City::LON, City::LON, City::LON, City::LON, City::OSL, City::LON, City::LON, City::LON, City::AMS, City::NLK, 
    City::BOS, City::BOS, City::BOS, City::ABV, City::YYC, City::HAG, City::OSL, City::MGA, City::CAI, City::IUE, 
    City::NYC, City::HAJ, City::AMS, City::AMS, City::AMD, City::NYC, City::TYO, City::TYO, City::STO, City::LON, 
    City::GOJ, City::STO, City::VN0, City::STO, City::LON, City::STO, City::LON, City::STO, City::TRD, City::OSL, 
    City::HAM, City::OSL, City::OSL, City::LON, City::OSL, City::STO, City::PAR, City::HAG, City::CPH, City::KTM, 
    City::NYC, City::INU, City::STO, City::STO, City::KRK, City::MEL, City::ST0, City::LON, City::LON, City::LON, 
    City::EDI, City::AMS, City::TYO, City::LON, City::TYO, City::TYO, City::TYO, City::SEL, City::NYC, City::TYO, 
    City::NYC, City::NYC, City::LON, City::NYC, City::NYC, City::CPH, City::SYD, City::WLG, City::AKL, City::AKL, 
    City::OSL, City::LNZ, City::EWR, City::NYC, City::LON, City::NYC, City::NYC, City::AMS, City::LON, City::PAR, 
    City::PAR, City::TYO, City::TYO, City::LON, City::AMS, City::LON, City::OB0, City::NYC, City::MAD, City::YTO, 
    City::LIS, City::LIS, City::MCT, City::NYC, City::STO, City::DCA, City::HEL, City::LIS, City::CHI, City::LON, 
    City::HKG, City::OSL, City::SIN, City::NYC, City::LON, City::NYC, City::NYC, City::NYC, City::NYC, City::BOM, 
    City::BUD, City::BBU, City::BRN, City::LON, City::NYC, City::ISB, City::PAC, City::WAW, City::LON, City::PRG, 
    City::BCN, City::ATH, City::SOF, City::LON, City::PN0, City::MKC, City::CHI, City::CHI, City::LON, City::STO, 
    City::STO, City::STO, City::STO, City::STO, City::LIM, City::IEV, City::IEV, City::LON, City::SIN, City::MNL, 
    City::LON, City::LON, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::GW0, City::LON, City::GW0, 
    City::WAW, City::LON, City::WAW, City::WAW, City::WAW, City::WAW, City::LON, City::MUC, City::ROR, City::LIS, 
    City::STO, City::POM, City::WAW, City::WAW, City::LON, City::MAD, City::VLI, City::SYD, City::LON, City::SJU, 
    City::FNJ, City::LON, City::MLA, City::NYC, City::LIS, City::ASU, City::PS0, City::NYC, City::NYC, City::WAW, 
    City::NYC, City::NYC, City::NYC, City::NYC, City::YTO, City::NYC, City::LON, City::BOM, City::PPT, City::DOH, 
    City::BUD, City::LON, City::OSL, City::LON, City::LON, City::LON, City::UTC, City::NYC, City::EA0, City::PAR, 
    City::LON, City::FRA, City::LON, City::NYC, City::LON, City::BUD, City::VIE, City::EA0, City::LON, City::NYC, 
    City::NYC, City::NIC, City::AMS, City::AMS, City::LON, City::RUN, City::PRG, City::DXB, City::AMS, City::SIN, 
    City::LON, City::PHL, City::PHL, City::BOM, City::LNZ, City::JNB, City::JNB, City::DUB, City::LON, City::TAY, 
    City::ROS, City::ZRH, City::KGL, City::BBU, City::MOW, City::WAW, City::AMS, City::BBU, City::KGL, City::LON, 
    City::LON, City::SIN, City::MOW, City::NYC, City::ZRH, City::MOW, City::MOW, City::SZG, City::KGL, City::LU0, 
    City::SFO, City::SDR, City::RUH, City::OSL, City::BCN, City::LON, City::TYO, City::BIO, City::TYO, City::TYO, 
    City::SBZ, City::AAB, City::FRA, City::MUC, City::MAD, City::EDI, City::CHI, City::CHI, City::CHI, City::CHI, 
    City::CHI, City::KRT, City::FRA, City::VNO, City::STO, City::STO, City::SEZ, City::SEZ, City::SEZ, City::LON, 
    City::SEZ, City::LON, City::MIL, City::STO, City::RIO, City::DUB, City::DKR, City::MAD, City::STO, City::DNK, 
    City::STO, City::LON, City::LON, City::LAX, City::NYC, City::NYC, City::SHA, City::NYC, City::NYC, City::PAR, 
    City::PAR, City::PAR, City::LON, City::LON, City::LON, City::PAR, City::SIN, City::GS0, City::NYC, City::AY0, 
    City::NYC, City::ASI, City::HKG, City::WAW, City::NIC, City::LON, City::DUB, City::LON, City::SYD, City::HKG, 
    City::TYO, City::BOS, City::MAD, City::SYD, City::DUB, City::LON, City::LYR, City::LJU, City::RA0, City::LON, 
    City::HIR, City::FNA, City::SZG, City::LON, City::SAL, City::LON, City::BRU, City::TYO, City::DUS, City::LON, 
    City::LON, City::PAR, City::SIN, City::CHI, City::FRA, City::SM0, City::LON, City::AAL, City::LON, City::NYC, 
    City::MGQ, City::PRG, City::NYC, City::MOW, City::STO, City::LON, City::LON, City::STO, City::FRA, City::STO, 
    City::PCT, City::LED, City::FSP, City::STO, City::LON, City::TRD, City::EWR, City::ILZ, City::LON, City::BEG, 
    City::MUC, City::MUC, City::LON, City::JUB, City::LON, City::LON, City::LON, City::STO, City::ROM, City::BOS, 
    City::HAM, City::LON, City::LON, City::OSL, City::LON, City::NYC, City::NYC, City::ZRH, City::TMS, City::NYC, 
    City::PRG, City::LON, City::STR, City::STR, City::STR, City::STR, City::STR, City::STR, City::STR, City::AMS, 
    City::LON, City::LON, City::LON, City::CHI, City::PBM, City::STO, City::STO, City::BTS, City::LJU, City::SVD, 
    City::LON, City::STO, City::STO, City::TLL, City::VNO, City::RIX, City::QMN, City::SXM, City::CPH, City::SEZ, 
    City::SIN, City::FLR, City::DAM, City::HKG, City::SOF, City::LON, City::LON, City::WAW, City::WAW, City::GDT, 
    City::NDJ, City::LON, City::LON, City::LON, City::LON, City::DUB, City::PAR, City::GVA, City::LON, City::LON, 
    City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, 
    City::LON, City::LON, City::LON, City::PAR, City::PAR, City::PAR, City::PAR, City::PAR, City::PAR, City::EWR, 
    City::LON, City::SIN, City::MAD, City::BKK, City::SIN, City::LON, City::HKG, City::LON, City::LON, City::LON, 
    City::NYC, City::LON, City::BER, City::LFW, City::BER, City::BKK, City::EWR, City::CHI, City::NYC, City::TYO, 
    City::LON, City::DYU, City::TK0, City::ASB, City::MIL, City::PAR, City::DIL, City::NYC, City::QLI, City::DUB, 
    City::ANK, City::NYC, City::LON, City::LON, City::YTO, City::AMS, City::BRU, City::BRU, City::LON, City::HKG, 
    City::AMS, City::MAD, City::MAD, City::AMS, City::TBU, City::AMS, City::LON, City::LON, City::FRA, City::PAR, 
    City::LON, City::LON, City::LON, City::PAR, City::MAD, City::PAR, City::LON, City::PAR, City::PAR, City::LON, 
    City::LON, City::LON, City::PAR, City::PAR, City::LON, City::LON, City::LON, City::LON, City::LON, City::NYC, 
    City::LON, City::EWR, City::SIN, City::LON, City::LON, City::LON, City::EWR, City::LON, City::AMS, City::AMS, 
    City::AMS, City::AMS, City::ILG, City::LON, City::HKG, City::LON, City::BER, City::NYC, City::NYC, City::SIN, 
    City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::EWR, City::AMS, City::SEZ, City::LON, 
    City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::NYC, City::NYC, City::LON, 
    City::NYC, City::EWR, City::PAR, City::NYC, City::PAR, City::ILG, City::LON, City::LON, City::LON, City::NYC, 
    City::FRA, City::FRA, City::LON, City::LON, City::LON, City::LON, City::SIN, City::LON, City::LON, City::LON, 
    City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::POS, City::TUN, City::LON, City::ANK, 
    City::FUN, City::AMS, City::AMS, City::AMS, City::AMS, City::LON, City::HKG, City::TYO, City::TYO, City::TAY, 
    City::NYC, City::SIN, City::DAR, City::NYC, City::PRG, City::ATL, City::BGY, City::LON, City::BLQ, City::NYC, 
    City::LON, City::ZRH, City::FRA, City::LON, City::ZRH, City::ZRH, City::FRA, City::FRA, City::NYC, City::NYC, 
    City::ZRH, City::HKG, City::LON, City::VIE, City::SOF, City::MUC, City::BUD, City::MIL, City::MVD, City::KLA, 
    City::LBA, City::IEV, City::LON, City::IEV, City::LON, City::LON, City::LON, City::IEV, City::LON, City::KLA, 
    City::LON, City::LON, City::SVX, City::MVD, City::DCA, City::NYC, City::STO, City::STO, City::LON, City::TYO, 
    City::TAS, City::NYC, City::LON, City::LON, City::NYC, City::VA0, City::LON, City::NYC, City::SVD, City::YTO, 
    City::LON, City::CCS, City::NYC, City::NYC, City::VFA, City::LON, City::DUB, City::NYC, City::DUB, City::LON, 
    City::DUB, City::EIS, City::STT, City::NYC, City::LON, City::TYO, City::ZRH, City::AUH, City::FAE, City::LON, 
    City::NYC, City::HAN, City::LON, City::FRA, City::BRU, City::PAR, City::LON, City::NYC, City::FRA, City::FRA, 
    City::BTS, City::NYC, City::VLI, City::AMS, City::FRA, City::FRA, City::NYC, City::VIE, City::WAW, City::VIE, 
    City::VIE, City::WRO, City::WAW, City::VIE, City::WAW, City::WAW, City::LON, City::WAW, City::GW0, City::LON, 
    City::NYC, City::CHO, City::WAW, City::LON, City::NYC, City::PAR, City::WAW, City::WAW, City::LON, City::LON, 
    City::WAW, City::WLS, City::WAW, City::PRG, City::WAW, City::LIS, City::NYC, City::LON, City::LON, City::APW, 
    City::BUE, City::STO, City::OSL, City::ABJ, City::SIN, City::AMS, City::ATH, City::DCA, City::AUH, City::AMS, 
    City::PAR, City::EB0, City::MIL, City::YYC, City::ALG, City::TIA, City::LON, City::AMM, City::AMS, City::SIN, 
    City::SIN, City::SIN, City::ANR, City::SYD, City::PAR, City::VVO, City::NYC, City::CHI, City::EVN, City::NYC, 
    City::DUB, City::SYD, City::ATH, City::DUB, City::YTO, City::YTO, City::AKL, City::PHX, City::NAS, City::BGI, 
    City::BAH, City::BLR, City::BCN, City::BCN, City::RIO, City::JKT, City::YTO, City::COR, City::BUD, City::STI, 
    City::MDZ, City::TGU, City::MDZ, City::BDA, City::LAD, City::BEG, City::BER, City::JNB, City::BEY, City::BRU, 
    City::BIO, City::BGI, City::BKK, City::BKK, City::BNX, City::PAR, City::PAR, City::SAO, City::NYC, City::BUD, 
    City::SJO, City::BOG, City::LPB, City::BOM, City::NYC, City::GBE, City::BOS, City::BTS, City::BRU, City::BRE, 
    City::BBU, City::BRN, City::NYC, City::BRU, City::ABJ, City::WAW, City::BBU, City::BBU, City::SAO, City::EWR, 
    City::ZRH, City::BUD, City::BUE, City::SOF, City::RAI, City::MPM, City::CWB, City::SDQ, City::NYC, City::CAI, 
    City::CCU, City::BBU, City::CCS, City::CMN, City::GCM, City::CHI, City::CHI, City::CHI, City::CHI, City::NGO, 
    City::CHI, City::DUB, City::NYC, City::VIE, City::TAS, City::SHA, City::NYC, City::HKG, City::CGP, City::CHI, 
    City::GCI, City::NYC, City::CHI, City::CNQ, City::YTO, City::CMB, City::LON, City::CHI, City::ZAG, City::NYC, 
    City::CPH, City::PNH, City::STR, City::TAS, City::NYC, City::SDQ, City::YTO, City::YTO, City::NIC, City::NIC, 
    City::DAR, City::FRA, City::FRA, City::FRA, City::DLC, City::DEL, City::STR, City::IEV, City::DXB, City::DAC, 
    City::ZRH, City::MIL, City::OSL, City::MAD, City::MAD, City::YTO, City::DAM, City::IST, City::DLA, City::FRA, 
    City::DUB, City::DUS, City::FRA, City::LV0, City::DUB, City::LON, City::FRA, City::LEJ, City::NIC, City::SKB, 
    City::IST, City::LON, City::LEJ, City::LEJ, City::LEJ, City::FRA, City::IST, City::NYC, City::FRA, City::FRA, 
    City::MEX, City::FRA, City::SH0, City::BER, City::IST, City::FRA, City::FRA, City::DUB, City::FRA, City::FRA, 
    City::FRA, City::FRA, City::FRA, City::PAR, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, 
    City::FRA, City::FRA, City::AMS, City::AMS, City::AMS, City::FRA, City::FRA, City::FRA, City::CHI, City::DUB, 
    City::SFO, City::VLC, City::SFO, City::SYD, City::FUK, City::FUK, City::PAR, City::CPH, City::IST, City::DUB, 
    City::HEL, City::FRA, City::AMS, City::ANU, City::DXB, City::BUD, City::BER, City::LON, City::LON, City::LON, 
    City::HKG, City::LON, City::DXB, City::ACC, City::WAW, City::ROM, City::SJU, City::BER, City::TBS, City::LON, 
    City::GUA, City::GYE, City::HAM, City::HAJ, City::HAJ, City::HEL, City::OSA, City::AMS, City::HIJ, City::HKG, 
    City::HKG, City::HAN, City::HAN, City::SAP, City::IST, City::BAK, City::ZRH, City::REK, City::YTO, City::JKT, 
    City::LON, City::DUB, City::FRA, City::HKG, City::TYO, City::OSL, City::BOM, City::TAY, City::CHI, City::CHI, 
    City::SHA, City::NYC, City::FRA, City::CHI, City::LON, City::ATH, City::BGW, City::NYC, City::NYC, City::ISB, 
    City::IST, City::NYC, City::KIN, City::TYO, City::JKT, City::JKT, City::TYO, City::JNB, City::LON, City::OSA, 
    City::KHI, City::ALA, City::MCI, City::TAS, City::SEL, City::SEL, City::SEL, City::SEL, City::UKB, City::KRT, 
    City::HRK, City::IEV, City::IEV, City::UKB, City::KUL, City::SEL, City::SEL, City::SEL, City::SEL, City::FRU, 
    City::SHS, City::KWI, City::UKY, City::LHE, City::VTE, City::MAD, City::LON, City::LON, City::LON, City::LON, 
    City::LON, City::LBU, City::LI0, City::LON, City::LIM, City::LIS, City::VNO, City::LJU, City::LJU, City::LJU, 
    City::LI0, City::LON, City::LON, City::KUL, City::LON, City::LON, City::SYD, City::TIP, City::LON, City::LUN, 
    City::LUX, City::BUE, City::CHI, City::MAD, City::SKP, City::BKK, City::MLA, City::MGA, City::MPM, City::PAR, 
    City::MRU, City::MAD, City::TNR, City::MAA, City::BCN, City::CHI, City::BUE, City::MEX, City::MAD, City::MSP, 
    City::MOW, City::CHI, City::MIL, City::MIL, City::PCT, City::PAR, City::LON, City::NIC, City::MVD, City::TGD, 
    City::YMQ, City::YMQ, City::KIV, City::PAR, City::YMQ, City::MOW, City::MIL, City::MAD, City::MAD, City::DUB, 
    City::BLZ, City::BUE, City::BBU, City::LON, City::MUC, City::MCT, City::SFN, City::MAD, City::NBO, City::WDH, 
    City::NYC, City::BOM, City::NYC, City::WAW, City::NYC, City::IEV, City::STO, City::NTL, City::AKL, City::KTM, 
    City::FRA, City::NYC, City::STO, City::NGO, City::NYC, City::KIJ, City::NYC, City::NGO, City::NYC, City::LON, 
    City::STO, City::NYC, City::LI0, City::HEL, City::NYC, City::BBU, City::LOS, City::BOM, City::NGO, City::AMS, 
    City::AMS, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::AKL, City::OSL, City::OSL, 
    City::OSL, City::OSL, City::OSL, City::CHI, City::ODS, City::XXX, City::STO, City::STO, City::OSL, City::OSL, 
    City::OSL, City::OSA, City::OSA, City::OSL, City::OSA, City::OSA, City::VIE, City::DCA, City::SPU, City::DUB, 
    City::PS0, City::LON, City::PAR, City::PHL, City::LED, City::PHL, City::PHL, City::PC0, City::LON, City::LED, 
    City::ST0, City::LON, City::IST, City::LON, City::POM, City::DCA, City::DUB, City::LEJ, City::PAR, City::PRG, 
    City::DNK, City::PRG, City::SFO, City::LEJ, City::HKG, City::NYC, City::PAC, City::LON, City::NYC, City::PRG, 
    City::BUD, City::ZRH, City::ZRH, City::DUS, City::UIO, City::BBU, City::KUL, City::VIE, City::ZRH, City::DUB, 
    City::RIO, City::RIX, City::PRG, City::BTS, City::PRG, City::ROS, City::ZRH, City::ROV, City::ROS, City::BBU, 
    City::LON, City::FRA, City::MOW, City::JNB, City::KUF, City::OKD, City::STO, City::RUH, City::TYO, City::SIN, 
    City::FRA, City::FRA, City::FRA, City::SIN, City::SIN, City::SIN, City::FRA, City::ZRH, City::ZRH, City::ZRH, 
    City::SZX, City::NYC, City::SIN, City::JNB, City::SYD, City::PAR, City::LON, City::SHA, City::STI, City::SZX, 
    City::SHA, City::OVB, City::OVB, City::SIN, City::ZRH, City::SZX, City::LON, City::SAO, City::LJU, City::PAR, 
    City::SUV, City::GRX, City::SHA, City::SJJ, City::SGN, City::TAS, City::STR, City::BOS, City::STO, City::STR, 
    City::STR, City::ZRH, City::ZRH, City::SUB, City::SAL, City::QMN, City::LON, City::ZRH, City::ZRH, City::ZRH, 
    City::TLL, City::TAY, City::TLV, City::TAY, City::TAY, City::TLL, City::TYO, City::TLL, City::THR, City::YTO, 
    City::TYO, City::LON, City::TIA, City::TYO, City::TYO, City::TYO, City::NGO, City::TYO, City::TYO, City::TYO, 
    City::MIL, City::BUD, City::YVR, City::YTO, City::LON, City::NYC, City::CPH, City::NYC, City::POS, City::ZAG, 
    City::YTO, City::YTO, City::TUC, City::TUN, City::LON, City::IZM, City::NYC, City::PAR, City::LON, City::IEV, 
    City::LON, City::KLA, City::IEV, City::ULN, City::LON, City::TAS, City::BOM, City::VLC, City::VZ0, City::LUX, 
    City::VNO, City::VIE, City::VVO, City::ASU, City::LI0, City::YVR, City::ZRH, City::LON, City::WAW, City::VIE, 
    City::YWG, City::GW0, City::FRA, City::XXX, City::XXX, City::SYD, City::TYO, City::ZAG, City::ZAG, City::ZAG, 
    City::CGO, City::HRE, City::SAH, City::CHI, City::JNB, City::PRY, City::ZAG, City::JNB, City::SOF, City::CHI, 
    City::JNB, City::ZRH, City::LUN, City::BER, City::LON, City::HRE
};

// Country to capital City
constexpr short Gazetteer::m_cid2cap[Country::NUMCOUNTRY] = { 
    City::NOCITY,
    City::AUA, City::KBL, City::LAD, City::AXA, City::MHQ, City::TIA, City::ALV, City::AUH, City::BUE, City::EVN, 
    City::PPG, City::XXX, City::TF0, City::ANU, City::CBR, City::VIE, City::BAK, City::BJM, City::BRU, City::PT0, 
    City::BON, City::OUA, City::DAC, City::SOF, City::BAH, City::NAS, City::SJJ, City::SBH, City::MSQ, City::BCV, 
    City::BDA, City::LPB, City::BSB, City::BGI, City::BWN, City::PBH, City::OSL, City::GBE, City::BGF, City::YOW, 
    City::CCK, City::BRN, City::STI, City::BJS, City::ABJ, City::YAO, City::FIH, City::BZV, City::RAR, City::BOG, 
    City::HAH, City::RAI, City::SJO, City::HAV, City::CUR, City::XCH, City::GCM, City::NIC, City::PRG, City::BER, 
    City::JIB, City::DCF, City::CPH, City::SDQ, City::ALG, City::UIO, City::CAI, City::ASM, City::EUN, City::MAD, 
    City::TLL, City::ADD, City::XXX, City::HEL, City::SUV, City::PSY, City::PAR, City::FAE, City::PNI, City::LBV, 
    City::LON, City::TBS, City::GCI, City::ACC, City::GIB, City::CKY, City::BBR, City::BJL, City::OXB, City::SSG, 
    City::ATH, City::GND, City::GOH, City::GUA, City::CAY, City::GUM, City::GEO, City::HKG, City::CBR, City::TGU, 
    City::ZAG, City::PAP, City::BUD, City::JKT, City::IOM, City::DEL, City::NKW, City::DUB, City::THR, City::BGW, 
    City::REK, City::TLV, City::ROM, City::KIN, City::JER, City::AMM, City::TYO, City::TSE, City::NBO, City::FRU, 
    City::PNH, City::TRW, City::SKB, City::SEL, City::KWI, City::VTE, City::BEY, City::MLW, City::TIP, City::SLU, 
    City::LI0, City::CMB, City::MSU, City::VNO, City::LUX, City::RIX, City::MFM, City::SFG, City::RBA, City::MCM, 
    City::KIV, City::TNR, City::MLE, City::MEX, City::MAJ, City::SKP, City::BKO, City::MLA, City::RGN, City::TGD, 
    City::ULN, City::SPN, City::MPM, City::NKC, City::MNI, City::FDF, City::MRU, City::LLW, City::KUL, City::DZA, 
    City::WDH, City::NOU, City::NIM, City::NLK, City::ABV, City::MGA, City::IUE, City::AMS, City::OSL, City::KTM, 
    City::INU, City::WLG, City::MCT, City::ISB, City::PAC, City::PN0, City::LIM, City::MNL, City::ROR, City::POM, 
    City::WAW, City::SJU, City::FNJ, City::LIS, City::ASU, City::PS0, City::PPT, City::DOH, City::RUN, City::BBU, 
    City::MOW, City::KGL, City::RUH, City::KRT, City::DKR, City::SIN, City::GS0, City::ASI, City::LYR, City::HIR, 
    City::FNA, City::SAL, City::SM0, City::MGQ, City::FSP, City::BEG, City::JUB, City::TMS, City::PBM, City::BTS, 
    City::LJU, City::STO, City::QMN, City::SXM, City::SEZ, City::DAM, City::GDT, City::NDJ, City::LFW, City::BKK, 
    City::DYU, City::TK0, City::ASB, City::DIL, City::TBU, City::POS, City::TUN, City::ANK, City::FUN, City::TAY, 
    City::DAR, City::KLA, City::IEV, City::DCA, City::MVD, City::DCA, City::TAS, City::VA0, City::SVD, City::CCS, 
    City::EIS, City::STT, City::HAN, City::VLI, City::WLS, City::APW, City::XXX, City::XXX, City::XXX, City::XXX, 
    City::XXX, City::SAH, City::PRY, City::LUN, City::HRE
};

// City to Country
constexpr short Gazetteer::m_cty2cid[City::NUMCITY] = { 
    Country::NOCOUNTRY,
    Country::DNK, Country::EGY, Country::DZA, Country::DNK, Country::ARE, Country::DNK, Country::CHN, Country::IRN, Country::USA, Country::USA, 
    Country::CIV, Country::AUS, Country::USA, Country::USA, Country::EGY, Country::NGA, Country::AUS, Country::USA, Country::GBR, Country::MEX, 
    Country::GHA, Country::ESP, Country::CHE, Country::GGY, Country::USA, Country::USA, Country::USA, Country::USA, Country::TUR, Country::ETH, 
    Country::YEM, Country::TUR, Country::USA, Country::AUS, Country::USA, Country::ZAF, Country::COL, Country::TCD, Country::RUS, Country::NOR, 
    Country::MAR, Country::DEU, Country::ESP, Country::USA, Country::MEX, Country::ZAF, Country::SAU, Country::USA, Country::ITA, Country::MAR, 
    Country::FRA, Country::SAU, Country::COM, Country::BRA, Country::NER, Country::NZL, Country::USA, Country::CYP, Country::KAZ, Country::USA, 
    Country::ESP, Country::NOR, Country::DZA, Country::AUS, Country::ZAF, Country::USA, Country::SYR, Country::AND, Country::USA, Country::EGY, 
    Country::USA, Country::IND, Country::JOR, Country::NLD, Country::USA, Country::USA, Country::TUR, Country::USA, Country::BEL, Country::ATG, 
    Country::ITA, Country::JPN, Country::GRC, Country::USA, Country::USA, Country::WSM, Country::JOR, Country::USA, Country::RUS, Country::TZA, 
    Country::TKM, Country::USA, Country::RUS, Country::SHN, Country::JPN, Country::CIV, Country::ERI, Country::AUS, Country::TUR, Country::PRY, 
    Country::EGY, Country::GRC, Country::USA, Country::USA, Country::IND, Country::USA, Country::USA, Country::EGY, Country::ABW, Country::EGY, 
    Country::USA, Country::ARE, Country::FRA, Country::USA, Country::BRA, Country::USA, Country::USA, Country::AIA, Country::USA, Country::JPN, 
    Country::GBR, Country::AUS, Country::AUS, Country::TUR, Country::IDN, Country::PNG, Country::USA, Country::DOM, Country::BHR, Country::AZE, 
    Country::COL, Country::IND, Country::KHM, Country::GLP, Country::CAF, Country::ROU, Country::CAF, Country::ESP, Country::ETH, Country::USA, 
    Country::BLZ, Country::BMU, Country::AUS, Country::USA, Country::IDN, Country::IND, Country::USA, Country::ITA, Country::NOR, Country::GBR, 
    Country::SRB, Country::USA, Country::BRA, Country::LBY, Country::DEU, Country::FRA, Country::USA, Country::MOZ, Country::LBN, Country::USA, 
    Country::USA, Country::ZAF, Country::ZWE, Country::GBR, Country::COL, Country::CAF, Country::BRB, Country::USA, Country::NOR, Country::USA, 
    Country::CAF, Country::IRQ, Country::ITA, Country::NZL, Country::USA, Country::IND, Country::AUS, Country::NPL, Country::PAK, Country::GBR, 
    Country::FRA, Country::USA, Country::ESP, Country::FRA, Country::USA, Country::CAF, Country::USA, Country::GMB, Country::BDI, Country::CHN, 
    Country::TUR, Country::MEX, Country::ESP, Country::MYS, Country::THA, Country::MLI, Country::USA, Country::USA, Country::USA, Country::VEN, 
    Country::USA, Country::USA, Country::GBR, Country::DNK, Country::ITA, Country::IND, Country::AUS, Country::MWI, Country::AUS, Country::USA, 
    Country::USA, Country::AUS, Country::USA, Country::AUS, Country::DEU, Country::AUS, Country::NOR, Country::PAK, Country::BIH, Country::PYF, 
    Country::FRA, Country::COL, Country::GBR, Country::USA, Country::BGR, Country::IND, Country::BES, Country::NOR, Country::USA, Country::BFA, 
    Country::USA, Country::USA, Country::PRI, Country::ARG, Country::USA, Country::DEU, Country::ITA, Country::USA, Country::CHE, Country::CZE, 
    Country::GBR, Country::GBR, Country::BEL, Country::BRA, Country::GNB, Country::CHE, Country::IRQ, Country::USA, Country::RWA, Country::USA, 
    Country::SVK, Country::MYS, Country::USA, Country::PNG, Country::HUN, Country::ARG, Country::USA, Country::AGO, Country::ZWE, Country::USA, 
    Country::GEO, Country::BRA, Country::USA, Country::BRN, Country::AUS, Country::USA, Country::CIV, Country::DEU, Country::BLZ, Country::BGD, 
    Country::USA, Country::COG, Country::AGO, Country::USA, Country::ITA, Country::EGY, Country::USA, Country::GBR, Country::CHN, Country::GUF, 
    Country::BOL, Country::GBR, Country::AUS, Country::DEU, Country::IND, Country::CCK, Country::USA, Country::VEN, Country::IND, Country::BHS, 
    Country::USA, Country::USA, Country::PHL, Country::USA, Country::AUS, Country::MEX, Country::FRA, Country::AUS, Country::FRA, Country::CUB, 
    Country::IRL, Country::AUS, Country::GRC, Country::USA, Country::BRA, Country::DEU, Country::CHN, Country::BGD, Country::CHN, Country::BRA, 
    Country::VEN, Country::USA, Country::NZL, Country::CHN, Country::USA, Country::USA, Country::GRC, Country::USA, Country::NZL, Country::USA, 
    Country::USA, Country::ZMB, Country::VCT, Country::IND, Country::CHL, Country::PAK, Country::MEX, Country::USA, Country::CHN, Country::GIN, 
    Country::USA, Country::USA, Country::ROU, Country::USA, Country::USA, Country::COL, Country::MEX, Country::USA, Country::FRA, Country::LKA, 
    Country::MEX, Country::FRA, Country::USA, Country::USA, Country::MAR, Country::AUS, Country::USA, Country::ROU, Country::BRA, Country::ARG, 
    Country::AUS, Country::THA, Country::USA, Country::IND, Country::USA, Country::BEN, Country::MNG, Country::ARG, Country::USA, Country::AUS, 
    Country::DNK, Country::USA, Country::ZAF, Country::ARG, Country::CAF, Country::USA, Country::USA, Country::USA, Country::USA, Country::AUS, 
    Country::USA, Country::FJI, Country::USA, Country::ITA, Country::ARG, Country::COL, Country::AUS, Country::CHN, Country::MEX, Country::MEX, 
    Country::CUW, Country::MEX, Country::USA, Country::MEX, Country::AUS, Country::GBR, Country::USA, Country::BRA, Country::GBR, Country::KIR, 
    Country::AUS, Country::USA, Country::PHL, Country::MEX, Country::DZA, Country::MEX, Country::USA, Country::BGD, Country::USA, Country::SYR, 
    Country::USA, Country::TZA, Country::USA, Country::AUS, Country::USA, Country::HRV, Country::AUS, Country::USA, Country::DMA, Country::AUS, 
    Country::USA, Country::IND, Country::USA, Country::USA, Country::SAU, Country::USA, Country::TLS, Country::IDN, Country::TUN, Country::IDN, 
    Country::CIV, Country::AUS, Country::SEN, Country::CMR, Country::CHN, Country::USA, Country::USA, Country::TUR, Country::SAU, Country::GBR, 
    Country::UKR, Country::FRA, Country::TUR, Country::TZA, Country::QAT, Country::DMA, Country::USA, Country::AUS, Country::IDN, Country::AUS, 
    Country::USA, Country::DEU, Country::AUS, Country::PAK, Country::USA, Country::DEU, Country::IRL, Country::NZL, Country::USA, Country::ZAF, 
    Country::DEU, Country::USA, Country::USA, Country::ARE, Country::AUS, Country::TJK, Country::MYT, Country::LUX, Country::ESP, Country::USA, 
    Country::USA, Country::MUS, Country::ITA, Country::UGA, Country::DNK, Country::FRA, Country::GBR, Country::FRA, Country::USA, Country::ISL, 
    Country::NLD, Country::VGB, Country::USA, Country::USA, Country::BHS, Country::ZAF, Country::USA, Country::USA, Country::ZAF, Country::USA, 
    Country::GBR, Country::AUS, Country::EGY, Country::USA, Country::FRA, Country::FIN, Country::MEX, Country::AUS, Country::MNG, Country::TUR, 
    Country::DEU, Country::USA, Country::BRA, Country::TUR, Country::FIN, Country::USA, Country::USA, Country::GBR, Country::CHL, Country::ISR, 
    Country::USA, Country::ESH, Country::NOR, Country::ARM, Country::USA, Country::USA, Country::USA, Country::GBR, Country::USA, Country::FRO, 
    Country::USA, Country::PRT, Country::USA, Country::USA, Country::USA, Country::COD, Country::USA, Country::USA, Country::MTQ, Country::DEU, 
    Country::MAR, Country::USA, Country::USA, Country::GBR, Country::COD, Country::ARE, Country::DEU, Country::COD, Country::USA, Country::JPN, 
    Country::USA, Country::USA, Country::BRA, Country::USA, Country::ITA, Country::USA, Country::DEU, Country::USA, Country::SLE, Country::PRT, 
    Country::FRA, Country::PRK, Country::USA, Country::USA, Country::USA, Country::BRA, Country::GBR, Country::BHS, Country::DEU, Country::USA, 
    Country::FRA, Country::NOR, Country::KGZ, Country::BWA, Country::FRA, Country::USA, Country::USA, Country::SPM, Country::ESP, Country::JPN, 
    Country::TUV, Country::WLF, Country::USA, Country::USA, Country::USA, Country::JPN, Country::IND, Country::BWA, Country::IND, Country::USA, 
    Country::GGY, Country::CYM, Country::USA, Country::MEX, Country::POL, Country::TCA, Country::USA, Country::USA, Country::GUY, Country::AUS, 
    Country::AUS, Country::AUS, Country::USA, Country::USA, Country::USA, Country::IND, Country::BHS, Country::GIB, Country::BDI, Country::PAK, 
    Country::DZA, Country::USA, Country::AUS, Country::GBR, Country::USA, Country::AUS, Country::FRA, Country::GRD, Country::BEL, Country::USA, 
    Country::ITA, Country::GRL, Country::IND, Country::RUS, Country::USA, Country::AUS, Country::SWE, Country::CMR, Country::AUS, Country::BGR, 
    Country::GRC, Country::USA, Country::USA, Country::DNK, Country::USA, Country::ZAF, Country::ESP, Country::NLD, Country::USA, Country::ESP, 
    Country::AUT, Country::SGS, Country::SLB, Country::USA, Country::USA, Country::AUS, Country::USA, Country::DEU, Country::GTM, Country::USA, 
    Country::GUM, Country::CHE, Country::USA, Country::PAK, Country::ZWE, Country::DEU, Country::IRL, Country::ECU, Country::RWA, Country::BRA, 
    Country::AUS, Country::PSE, Country::TUR, Country::JPN, Country::NLD, Country::COM, Country::DEU, Country::DEU, Country::VNM, Country::USA, 
    Country::NOR, Country::CUB, Country::AUS, Country::PAK, Country::USA, Country::THA, Country::AFG, Country::FIN, Country::GRC, Country::ISR, 
    Country::NOR, Country::SOM, Country::CHN, Country::CIV, Country::USA, Country::USA, Country::USA, Country::JPN, Country::SLB, Country::AUS, 
    Country::JPN, Country::HKG, Country::THA, Country::USA, Country::ZAF, Country::CHN, Country::USA, Country::AUS, Country::NZL, Country::MEX, 
    Country::JPN, Country::USA, Country::AUS, Country::USA, Country::USA, Country::CUB, Country::USA, Country::USA, Country::DEU, Country::PRT, 
    Country::USA, Country::USA, Country::CHN, Country::ZWE, Country::EGY, Country::UKR, Country::USA, Country::DNK, Country::USA, Country::RUS, 
    Country::AUS, Country::USA, Country::USA, Country::USA, Country::PYF, Country::VNM, Country::MEX, Country::GBR, Country::AUS, Country::USA, 
    Country::USA, Country::ZWE, Country::USA, Country::IND, Country::USA, Country::USA, Country::ESP, Country::KOR, Country::USA, Country::USA, 
    Country::IND, Country::UKR, Country::AUS, Country::USA, Country::AUS, Country::ARG, Country::USA, Country::RUS, Country::USA, Country::USA, 
    Country::USA, Country::USA, Country::NCL, Country::GBR, Country::SVK, Country::USA, Country::SRB, Country::USA, Country::AUT, Country::NRU, 
    Country::GBR, Country::IMN, Country::NCL, Country::CHL, Country::USA, Country::USA, Country::PER, Country::AUS, Country::CAF, Country::AUS, 
    Country::PAK, Country::JPN, Country::USA, Country::USA, Country::USA, Country::TUR, Country::USA, Country::USA, Country::NIU, Country::NZL, 
    Country::FIN, Country::IND, Country::IND, Country::IND, Country::IND, Country::IND, Country::IND, Country::USA, Country::TUR, Country::AFG, 
    Country::USA, Country::AUS, Country::LKA, Country::PAK, Country::IND, Country::HTI, Country::MEX, Country::BGR, Country::USA, Country::PER, 
    Country::USA, Country::BRA, Country::AUS, Country::BRA, Country::BRA, Country::IND, Country::CHN, Country::SAU, Country::HTI, Country::USA, 
    Country::JEY, Country::IND, Country::IND, Country::CHN, Country::CHN, Country::MYS, Country::SWE, Country::CHN, Country::USA, Country::AUS, 
    Country::USA, Country::DJI, Country::ETH, Country::CHN, Country::ETH, Country::UGA, Country::ECU, Country::NPL, Country::CHN, Country::PAK, 
    Country::PER, Country::CHN, Country::SWE, Country::GRC, Country::NPL, Country::IDN, Country::USA, Country::USA, Country::IND, Country::BRA, 
    Country::AGO, Country::GRC, Country::NPL, Country::USA, Country::CHN, Country::BRA, Country::ZAF, Country::CHN, Country::ARG, Country::USA, 
    Country::GRC, Country::CHN, Country::FIN, Country::BRA, Country::PHL, Country::UMI, Country::NGA, Country::BRA, Country::PAN, Country::IND, 
    Country::TZA, Country::ISR, Country::IND, Country::GRC, Country::ARG, Country::USA, Country::BRA, Country::GRC, Country::SSD, Country::DEU, 
    Country::ARG, Country::PER, Country::AUS, Country::HND, Country::GRL, Country::USA, Country::BWA, Country::USA, Country::FIN, Country::FIN, 
    Country::NGA, Country::FIN, Country::AFG, Country::AUS, Country::USA, Country::AUS, Country::MYS, Country::TUR, Country::JPN, Country::PAK, 
    Country::AFG, Country::PAK, Country::DEU, Country::FIN, Country::SLE, Country::AUS, Country::RUS, Country::AUS, Country::RWA, Country::GRC, 
    Country::TWN, Country::PAK, Country::FIN, Country::RUS, Country::SWE, Country::JPN, Country::IRQ, Country::ZAF, Country::JAM, Country::IRL, 
    Country::MDA, Country::ZMB, Country::NOR, Country::UGA, Country::SWE, Country::AUT, Country::USA, Country::GRC, Country::ZAF, Country::JPN, 
    Country::JPN, Country::NAM, Country::JPN, Country::GHA, Country::AUS, Country::IND, Country::AUS, Country::USA, Country::GBR, Country::JPN, 
    Country::FIN, Country::AUS, Country::POL, Country::SWE, Country::DNK, Country::NOR, Country::SDN, Country::SWE, Country::SDN, Country::MLI, 
    Country::NOR, Country::AUS, Country::USA, Country::NPL, Country::USA, Country::AUS, Country::FIN, Country::MYS, Country::RUS, Country::JPN, 
    Country::MYS, Country::FIN, Country::GRC, Country::KWT, Country::CHN, Country::AUS, Country::TUR, Country::TUR, Country::RUS, Country::AGO, 
    Country::PNG, Country::USA, Country::FRA, Country::USA, Country::MEX, Country::USA, Country::USA, Country::USA, Country::USA, Country::GBR, 
    Country::USA, Country::USA, Country::FRA, Country::GAB, Country::MYS, Country::GAB, Country::CYP, Country::ESP, Country::USA, Country::AUS, 
    Country::FRA, Country::SWE, Country::GBR, Country::USA, Country::RUS, Country::ESP, Country::DEU, Country::GIN, Country::GBR, Country::AUS, 
    Country::USA, Country::NLD, Country::USA, Country::USA, Country::TGO, Country::USA, Country::BEL, Country::MYS, Country::PAK, Country::LIE, 
    Country::NCL, Country::FRA, Country::USA, Country::FRA, Country::PER, Country::COD, Country::PRT, Country::USA, Country::SVN, Country::NOR, 
    Country::IND, Country::PHL, Country::SWE, Country::MWI, Country::MEX, Country::ITA, Country::USA, Country::USA, Country::AUS, Country::USA, 
    Country::USA, Country::AUT, Country::GBR, Country::NGA, Country::ESP, Country::BOL, Country::GBR, Country::FIN, Country::USA, Country::AUS, 
    Country::USA, Country::FRA, Country::DOM, Country::FRA, Country::USA, Country::GBR, Country::AUS, Country::AUS, Country::HRV, Country::MEX, 
    Country::CHE, Country::NAM, Country::CHE, Country::ZAF, Country::CHN, Country::ZMB, Country::ARG, Country::LUX, Country::BEL, Country::AUS, 
    Country::USA, Country::GBR, Country::UKR, Country::USA, Country::USA, Country::EGY, Country::USA, Country::USA, Country::PAK, Country::SJM, 
    Country::FRA, Country::GBR, Country::MEX, Country::IND, Country::ESP, Country::USA, Country::ESP, Country::MHL, Country::MEX, Country::GBR, 
    Country::BRA, Country::VEN, Country::PYF, Country::PRI, Country::KEN, Country::AUS, Country::JAM, Country::ZAF, Country::USA, Country::SVN, 
    Country::USA, Country::USA, Country::USA, Country::USA, Country::MCO, Country::USA, Country::USA, Country::BRA, Country::OMN, Country::USA, 
    Country::AUS, Country::BRA, Country::IDN, Country::COL, Country::MMR, Country::ARG, Country::UMI, Country::ARG, Country::SAU, Country::NCL, 
    Country::USA, Country::AUS, Country::USA, Country::IDN, Country::MEX, Country::ZAF, Country::USA, Country::GAB, Country::PAK, Country::MAC, 
    Country::NZL, Country::PNG, Country::NER, Country::USA, Country::ZMB, Country::NIC, Country::AUS, Country::BRA, Country::ZAF, Country::USA, 
    Country::SOM, Country::USA, Country::USA, Country::BHS, Country::ALA, Country::USA, Country::USA, Country::MEX, Country::FIN, Country::ITA, 
    Country::AUS, Country::TUN, Country::CIV, Country::PAK, Country::GAB, Country::MDG, Country::USA, Country::GRC, Country::ESP, Country::PHL, 
    Country::USA, Country::USA, Country::USA, Country::USA, Country::AUS, Country::AUS, Country::MLT, Country::USA, Country::MDV, Country::FRA, 
    Country::USA, Country::MEX, Country::USA, Country::USA, Country::LBR, Country::TUR, Country::SWE, Country::GBR, Country::AUS, Country::JPN, 
    Country::RUS, Country::AUS, Country::JPN, Country::MSR, Country::PHL, Country::USA, Country::USA, Country::NZL, Country::USA, Country::AUS, 
    Country::RUS, Country::PYF, Country::NAM, Country::PAK, Country::FRA, Country::MOZ, Country::USA, Country::AUS, Country::TUR, Country::TCD, 
    Country::USA, Country::USA, Country::FRA, Country::MUS, Country::RUS, Country::USA, Country::AUS, Country::GBR, Country::USA, Country::USA, 
    Country::USA, Country::USA, Country::BLR, Country::TUR, Country::NLD, Country::LSO, Country::USA, Country::USA, Country::USA, Country::AUS, 
    Country::USA, Country::USA, Country::SWZ, Country::MEX, Country::MEX, Country::BWA, Country::DEU, Country::USA, Country::EGY, Country::PAK, 
    Country::USA, Country::GAB, Country::URY, Country::CMR, Country::USA, Country::ZWE, Country::PAK, Country::USA, Country::MEX, Country::AUS, 
    Country::KEN, Country::JPN, Country::IND, Country::USA, Country::MYS, Country::TWN, Country::FRA, Country::AFG, Country::MEX, Country::ZAF, 
    Country::AUS, Country::IND, Country::AZE, Country::FJI, Country::ITA, Country::BHS, Country::BRA, Country::KEN, Country::FRA, Country::GBR, 
    Country::ZAF, Country::FRA, Country::MRT, Country::TCD, Country::NAM, Country::KNA, Country::CMR, Country::JPN, Country::JPN, Country::CYP, 
    Country::NER, Country::MRT, Country::IOT, Country::ZMB, Country::MEX, Country::NFK, Country::ZAF, Country::UKR, Country::CHN, Country::IRL, 
    Country::NCL, Country::GBR, Country::AUS, Country::SWE, Country::USA, Country::AUS, Country::NZL, Country::AUS, Country::THA, Country::FRA, 
    Country::AUS, Country::ZAF, Country::DEU, Country::GBR, Country::USA, Country::AUS, Country::USA, Country::USA, Country::MEX, Country::DEU, 
    Country::CAF, Country::ESP, Country::DNK, Country::UKR, Country::USA, Country::MKD, Country::PAK, Country::JPN, Country::JPN, Country::USA, 
    Country::JPN, Country::JPN, Country::NAM, Country::ITA, Country::USA, Country::USA, Country::AUS, Country::USA, Country::NAM, Country::USA, 
    Country::IRN, Country::BIH, Country::NAM, Country::USA, Country::AUS, Country::AUS, Country::PRT, Country::SWE, Country::USA, Country::USA, 
    Country::IRL, Country::DZA, Country::JPN, Country::USA, Country::HRV, Country::NOR, Country::IRQ, Country::CZE, Country::USA, Country::USA, 
    Country::BFA, Country::MAR, Country::ZAF, Country::GBR, Country::FIN, Country::MRT, Country::RUS, Country::ESP, Country::USA, Country::GNB, 
    Country::USA, Country::UKR, Country::MAR, Country::BRA, Country::PAN, Country::DEU, Country::USA, Country::HTI, Country::FRA, Country::IND, 
    Country::USA, Country::MEX, Country::BTN, Country::USA, Country::SUR, Country::AUS, Country::ZAF, Country::PHL, Country::USA, Country::PRT, 
    Country::USA, Country::USA, Country::ITA, Country::COL, Country::MYS, Country::AUS, Country::PAK, Country::USA, Country::CYP, Country::USA, 
    Country::FRA, Country::USA, Country::NGA, Country::AUS, Country::USA, Country::NPL, Country::USA, Country::ZAF, Country::USA, Country::USA, 
    Country::USA, Country::USA, Country::USA, Country::FRA, Country::USA, Country::PAK, Country::USA, Country::BWA, Country::USA, Country::USA, 
    Country::AUS, Country::ZAF, Country::USA, Country::ESP, Country::ITA, Country::NZL, Country::VEN, Country::BRA, Country::PCN, Country::KHM, 
    Country::FSM, Country::ITA, Country::IND, Country::COG, Country::USA, Country::BRA, Country::GAB, Country::PNG, Country::DOM, Country::FIN, 
    Country::TTO, Country::USA, Country::POL, Country::ASM, Country::AUS, Country::PYF, Country::USA, Country::AUS, Country::CZE, Country::XXX, 
    Country::ZAF, Country::PSE, Country::ITA, Country::USA, Country::EGY, Country::PRI, Country::USA, Country::PAK, Country::USA, Country::ITA, 
    Country::FLK, Country::BEN, Country::ZAF, Country::AUS, Country::USA, Country::FRA, Country::AUS, Country::DOM, Country::CHL, Country::KOR, 
    Country::USA, Country::HRV, Country::USA, Country::BRA, Country::GRC, Country::MEX, Country::USA, Country::MEX, Country::PRT, Country::IND, 
    Country::THA, Country::ZAF, Country::PAK, Country::VEN, Country::JPN, Country::BRA, Country::ITA, Country::CYP, Country::PAK, Country::SWZ, 
    Country::IND, Country::SRB, Country::ITA, Country::ESP, Country::DNK, Country::CPV, Country::IND, Country::MAR, Country::USA, Country::COK, 
    Country::PAK, Country::DEU, Country::MAR, Country::BRA, Country::ZAF, Country::USA, Country::USA, Country::USA, Country::USA, Country::FRA, 
    Country::BRA, Country::ITA, Country::ISL, Country::ESP, Country::USA, Country::PYF, Country::PYF, Country::ARG, Country::MMR, Country::RWA, 
    Country::USA, Country::GRC, Country::USA, Country::BRA, Country::LVA, Country::CHN, Country::HRV, Country::USA, Country::USA, Country::ARE, 
    Country::NER, Country::EGY, Country::ITA, Country::SWE, Country::FRA, Country::DNK, Country::USA, Country::FRA, Country::USA, Country::USA, 
    Country::AUS, Country::ITA, Country::PLW, Country::ARG, Country::NZL, Country::BGR, Country::RUS, Country::CAN, Country::MUS, Country::GEO, 
    Country::ARG, Country::BHS, Country::USA, Country::HND, Country::NLD, Country::SAU, Country::REU, Country::FIN, Country::USA, Country::PAK, 
    Country::PAK, Country::USA, Country::YEM, Country::KHM, Country::SLV, Country::USA, Country::BRA, Country::HND, Country::USA, Country::USA, 
    Country::USA, Country::BLM, Country::FRA, Country::USA, Country::USA, Country::ZAF, Country::MYS, Country::USA, Country::ROU, Country::USA, 
    Country::USA, Country::USA, Country::USA, Country::DEU, Country::ESP, Country::CUB, Country::USA, Country::HND, Country::JPN, Country::SWE, 
    Country::DOM, Country::ESP, Country::JPN, Country::PAK, Country::USA, Country::USA, Country::LBY, Country::KOR, Country::GBR, Country::EGY, 
    Country::SYC, Country::TUN, Country::MAF, Country::GRL, Country::ARG, Country::USA, Country::DNK, Country::USA, Country::VNM, Country::USA, 
    Country::USA, Country::NLD, Country::CHN, Country::USA, Country::CHN, Country::ARE, Country::USA, Country::JPN, Country::USA, Country::DNK, 
    Country::CPV, Country::SGP, Country::UKR, Country::ZAF, Country::USA, Country::AUS, Country::USA, Country::MEX, Country::BIH, Country::CRI, 
    Country::USA, Country::PRI, Country::FIN, Country::GMB, Country::KNA, Country::UZB, Country::GRC, Country::MKD, Country::DNK, Country::EGY, 
    Country::RUS, Country::PAK, Country::ARG, Country::USA, Country::USA, Country::OMN, Country::MEX, Country::BGR, Country::LCA, Country::BRA, 
    Country::SMR, Country::PRT, Country::USA, Country::GRC, Country::USA, Country::USA, Country::ECU, Country::IRL, Country::USA, Country::BGR, 
    Country::NOR, Country::VUT, Country::FIN, Country::GBR, Country::ESP, Country::USA, Country::MNP, Country::USA, Country::HRV, Country::CIV, 
    Country::BRA, Country::BOL, Country::MEX, Country::USA, Country::BOL, Country::BRA, Country::GNQ, Country::EGY, Country::COL, Country::USA, 
    Country::CHL, Country::USA, Country::SWE, Country::USA, Country::DEU, Country::USA, Country::VIR, Country::IND, Country::VIR, Country::IDN, 
    Country::ITA, Country::PAK, Country::USA, Country::FJI, Country::USA, Country::VCT, Country::NOR, Country::FIN, Country::ESP, Country::RUS, 
    Country::DEU, Country::USA, Country::NAM, Country::GBR, Country::FRA, Country::IRL, Country::SXM, Country::IND, Country::AUS, Country::USA, 
    Country::GBR, Country::GBR, Country::TUR, Country::AUT, Country::ZAF, Country::CHN, Country::POL, Country::TTO, Country::JPN, Country::MEX, 
    Country::CHN, Country::UZB, Country::TWN, Country::SEN, Country::GEO, Country::TON, Country::USA, Country::AUS, Country::BHS, Country::ESP, 
    Country::USA, Country::ZAF, Country::DNK, Country::AUS, Country::TUR, Country::PRT, Country::NZL, Country::USA, Country::ATF, Country::MNE, 
    Country::HND, Country::BGR, Country::MEX, Country::BRA, Country::IRN, Country::GRL, Country::ALB, Country::SAU, Country::MEX, Country::LBY, 
    Country::AUS, Country::MNE, Country::TKL, Country::USA, Country::JPN, Country::FIN, Country::USA, Country::EST, Country::FRA, Country::ISR, 
    Country::GHA, Country::FIN, Country::STP, Country::AUS, Country::UZB, Country::CHN, Country::MAR, Country::MDG, Country::MYS, Country::USA, 
    Country::NOR, Country::NCL, Country::VGB, Country::JPN, Country::USA, Country::AUS, Country::ITA, Country::NOR, Country::USA, Country::ITA, 
    Country::AUS, Country::ITA, Country::IND, Country::KIR, Country::IND, Country::NAM, Country::KAZ, Country::ITA, Country::CHN, Country::AUS, 
    Country::USA, Country::ARG, Country::PAK, Country::USA, Country::TUN, Country::USA, Country::USA, Country::SAU, Country::USA, Country::USA, 
    Country::USA, Country::AUS, Country::USA, Country::MYS, Country::USA, Country::CHN, Country::JPN, Country::USA, Country::USA, Country::TUR, 
    Country::PYF, Country::GRL, Country::PYF, Country::BRA, Country::JPN, Country::THA, Country::USA, Country::RUS, Country::NLD, Country::BRA, 
    Country::UKR, Country::IND, Country::AUS, Country::PAK, Country::RUS, Country::UZB, Country::AGO, Country::CZE, Country::HND, Country::USA, 
    Country::ECU, Country::FRA, Country::MHL, Country::JPN, Country::USA, Country::JPN, Country::VUT, Country::ZAF, Country::MNG, Country::UGA, 
    Country::RUS, Country::GRL, Country::SWE, Country::AUS, Country::VCT, Country::USA, Country::GBR, Country::IDN, Country::CRI, Country::MEX, 
    Country::USA, Country::CHN, Country::BRA, Country::VEN, Country::ARG, Country::KOR, Country::NLD, Country::THA, Country::ZAF, Country::THA, 
    Country::ZAF, Country::RUS, Country::GAB, Country::LCA, Country::EGY, Country::VAT, Country::FIN, Country::TUR, Country::CHL, Country::BGR, 
    Country::TUR, Country::ITA, Country::SWE, Country::ITA, Country::ESP, Country::USA, Country::USA, Country::MEX, Country::ZWE, Country::ESP, 
    Country::BGR, Country::AUT, Country::VGB, Country::USA, Country::ESP, Country::BRA, Country::ESP, Country::USA, Country::VUT, Country::ESP, 
    Country::VEN, Country::RUS, Country::USA, Country::LTU, Country::IND, Country::RUS, Country::USA, Country::CUB, Country::USA, Country::FIN, 
    Country::ITA, Country::MEX, Country::SWE, Country::LAO, Country::RUS, Country::SWE, Country::USA, Country::ZAF, Country::HRV, Country::AUS, 
    Country::POL, Country::NAM, Country::AUS, Country::ZAF, Country::AUS, Country::NZL, Country::GBR, Country::DEU, Country::USA, Country::NZL, 
    Country::WLF, Country::AUS, Country::USA, Country::PAK, Country::AUS, Country::USA, Country::NZL, Country::USA, Country::USA, Country::POL, 
    Country::AUS, Country::CHN, Country::AUS, Country::CHN, Country::ZAF, Country::AUS, Country::AUS, Country::USA, Country::CXR, Country::CHN, 
    Country::PYF, Country::CHN, Country::CAN, Country::ESP, Country::CAN, Country::XXX, Country::USA, Country::CMR, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::CAN, Country::CHN, Country::CAN, Country::USA, Country::RUS, Country::CAN, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::SAU, Country::JPN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::USA, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, 
    Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::HRV, Country::HRV, 
    Country::ESP, Country::AUS, Country::MEX, Country::MEX, Country::CAN, Country::MEX, Country::NER, Country::AUS, Country::NZL, Country::CHE, 
    Country::BHS, Country::CIV, Country::GRC, Country::CAN, Country::LAO, Country::BGD
};

// Country to Principle Currency
constexpr short Gazetteer::m_cid2ccy[Country::NUMCOUNTRY] = { 
    Currency::NOCURRENCY,
    Currency::AWG, Currency::AFN, Currency::AOA, Currency::XCD, Currency::EUR, Currency::ALL, Currency::EUR, Currency::AED, Currency::ARS, Currency::AMD, 
    Currency::USD, Currency::XXX, Currency::EUR, Currency::XCD, Currency::AUD, Currency::EUR, Currency::AZN, Currency::BIF, Currency::EUR, Currency::XOF, 
    Currency::USD, Currency::XOF, Currency::BDT, Currency::BGN, Currency::BHD, Currency::BSD, Currency::BAM, Currency::EUR, Currency::BYN, Currency::BZD, 
    Currency::BMD, Currency::BOB, Currency::BRL, Currency::BBD, Currency::BND, Currency::BTN, Currency::NOK, Currency::BWP, Currency::XAF, Currency::CAD, 
    Currency::AUD, Currency::CHF, Currency::CLP, Currency::CNY, Currency::XOF, Currency::XAF, Currency::CDF, Currency::XAF, Currency::NZD, Currency::COP, 
    Currency::KMF, Currency::CVE, Currency::CRC, Currency::CUC, Currency::ANG, Currency::AUD, Currency::KYD, Currency::EUR, Currency::CZK, Currency::EUR, 
    Currency::DJF, Currency::XCD, Currency::DKK, Currency::DOP, Currency::DZD, Currency::USD, Currency::EGP, Currency::ERN, Currency::MAD, Currency::EUR, 
    Currency::EUR, Currency::ETB, Currency::EUR, Currency::EUR, Currency::FJD, Currency::FKP, Currency::EUR, Currency::DKK, Currency::USD, Currency::XAF, 
    Currency::GBP, Currency::GEL, Currency::GBP, Currency::GHS, Currency::GIP, Currency::GNF, Currency::EUR, Currency::GMD, Currency::XOF, Currency::XAF, 
    Currency::EUR, Currency::XCD, Currency::DKK, Currency::GTQ, Currency::EUR, Currency::USD, Currency::GYD, Currency::HKD, Currency::AUD, Currency::HNL, 
    Currency::EUR, Currency::HTG, Currency::HUF, Currency::IDR, Currency::GBP, Currency::INR, Currency::USD, Currency::EUR, Currency::IRR, Currency::IQD, 
    Currency::ISK, Currency::ILS, Currency::EUR, Currency::JMD, Currency::GBP, Currency::JOD, Currency::JPY, Currency::KZT, Currency::KES, Currency::KGS, 
    Currency::KHR, Currency::AUD, Currency::XCD, Currency::KRW, Currency::KWD, Currency::LAK, Currency::LBP, Currency::LRD, Currency::LYD, Currency::XCD, 
    Currency::CHF, Currency::LKR, Currency::LSL, Currency::EUR, Currency::EUR, Currency::EUR, Currency::MOP, Currency::EUR, Currency::MAD, Currency::EUR, 
    Currency::MDL, Currency::MGA, Currency::MVR, Currency::MXN, Currency::USD, Currency::MKD, Currency::XOF, Currency::EUR, Currency::MMK, Currency::EUR, 
    Currency::MNT, Currency::USD, Currency::MZN, Currency::MRU, Currency::XCD, Currency::EUR, Currency::MUR, Currency::MWK, Currency::MYR, Currency::EUR, 
    Currency::NAD, Currency::XPF, Currency::XOF, Currency::AUD, Currency::NGN, Currency::NIO, Currency::NZD, Currency::EUR, Currency::NOK, Currency::NPR, 
    Currency::AUD, Currency::NZD, Currency::OMR, Currency::PKR, Currency::PAB, Currency::NZD, Currency::PEN, Currency::PHP, Currency::USD, Currency::PGK, 
    Currency::PLN, Currency::USD, Currency::KPW, Currency::EUR, Currency::PYG, Currency::ILS, Currency::XPF, Currency::QAR, Currency::EUR, Currency::RON, 
    Currency::RUB, Currency::RWF, Currency::SAR, Currency::SDG, Currency::XOF, Currency::SGD, Currency::GBP, Currency::SHP, Currency::NOK, Currency::SBD, 
    Currency::SLL, Currency::USD, Currency::EUR, Currency::SOS, Currency::EUR, Currency::RSD, Currency::SSP, Currency::STN, Currency::SRD, Currency::EUR, 
    Currency::EUR, Currency::SEK, Currency::SZL, Currency::ANG, Currency::SCR, Currency::SYP, Currency::USD, Currency::XAF, Currency::XOF, Currency::THB, 
    Currency::TJS, Currency::NZD, Currency::TMT, Currency::USD, Currency::TOP, Currency::TTD, Currency::TND, Currency::TRY, Currency::AUD, Currency::TWD, 
    Currency::TZS, Currency::UGX, Currency::UAH, Currency::USD, Currency::UYU, Currency::USD, Currency::UZS, Currency::EUR, Currency::XCD, Currency::VES, 
    Currency::USD, Currency::USD, Currency::VND, Currency::VUV, Currency::XPF, Currency::WST, Currency::XAF, Currency::XCD, Currency::XOF, Currency::XPF, 
    Currency::XXX, Currency::YER, Currency::ZAR, Currency::ZMW, Currency::ZWL
};

// Currency to Principle Country
constexpr short Gazetteer::m_ccy2cid[Currency::NUMCURRENCY] = {
    Country::NOCOUNTRY,
    Country::AND, Country::ARE, Country::AFG, Country::AFG, Country::ALB, Country::ARM, Country::XXX, Country::AGO, Country::AGO, Country::AGO, 
    Country::ARG, Country::AUT, Country::AUS, Country::ABW, Country::AZE, Country::AZE, Country::BIH, Country::BIH, Country::BRB, Country::BGD, 
    Country::BEL, Country::BEL, Country::BEL, Country::BGR, Country::BGR, Country::BHR, Country::BDI, Country::BMU, Country::BRN, Country::BOL, 
    Country::BOL, Country::BRA, Country::BHS, Country::IND, Country::BWA, Country::BLR, Country::BLR, Country::BLZ, Country::CAN, Country::COD, 
    Country::CHE, Country::CHE, Country::CHE, Country::CHE, Country::CHL, Country::CHL, Country::CHN, Country::COL, Country::COL, Country::CRI, 
    Country::XXX, Country::CUB, Country::CUB, Country::CPV, Country::CYP, Country::CZE, Country::XXX, Country::DEU, Country::DJI, Country::DNK, 
    Country::DOM, Country::DZA, Country::ECU, Country::EST, Country::EGY, Country::ERI, Country::ESP, Country::ESP, Country::ESP, Country::ETH, 
    Country::ALA, Country::FIN, Country::FJI, Country::FLK, Country::FRA, Country::GBR, Country::GEO, Country::GHA, Country::GIB, Country::GMB, 
    Country::GIN, Country::GRC, Country::GTM, Country::GNB, Country::GUY, Country::HKG, Country::HND, Country::HRV, Country::HTI, Country::HUN, 
    Country::IDN, Country::IRL, Country::ISR, Country::IND, Country::IRQ, Country::IRN, Country::ISL, Country::ITA, Country::JAM, Country::JOR, 
    Country::JPN, Country::KEN, Country::KGZ, Country::KHM, Country::COM, Country::PRK, Country::KOR, Country::KWT, Country::CYM, Country::KAZ, 
    Country::LAO, Country::LBN, Country::LKA, Country::LBR, Country::LSO, Country::LTU, Country::LUX, Country::LVA, Country::LBY, Country::MAR, 
    Country::MDA, Country::MDG, Country::MDG, Country::MKD, Country::MMR, Country::MNG, Country::MAC, Country::MRT, Country::MRT, Country::MLT, 
    Country::MUS, Country::MDV, Country::MWI, Country::MEX, Country::MEX, Country::MYS, Country::MOZ, Country::MOZ, Country::NAM, Country::NGA, 
    Country::NIC, Country::NLD, Country::NOR, Country::NPL, Country::NZL, Country::OMN, Country::PAN, Country::PER, Country::PNG, Country::PHL, 
    Country::PAK, Country::POL, Country::POL, Country::PRT, Country::PRY, Country::QAT, Country::ROU, Country::ROU, Country::SRB, Country::RUS, 
    Country::XXX, Country::RWA, Country::SAU, Country::SLB, Country::SYC, Country::SDN, Country::SDN, Country::SWE, Country::SGP, Country::SHN, 
    Country::SVN, Country::SVK, Country::SLE, Country::SLE, Country::SOM, Country::SUR, Country::SUR, Country::SSD, Country::STP, Country::STP, 
    Country::SLV, Country::SYR, Country::SWZ, Country::THA, Country::TJK, Country::TJK, Country::TKM, Country::TKM, Country::TUN, Country::TON, 
    Country::PRT, Country::TUR, Country::TUR, Country::TTO, Country::TWN, Country::TZA, Country::UKR, Country::UKR, Country::UGA, Country::USA, 
    Country::USA, Country::USA, Country::URY, Country::URY, Country::URY, Country::UZB, Country::VEN, Country::VEN, Country::VEN, Country::VEN, 
    Country::VNM, Country::VUT, Country::WSM, Country::XAF, Country::XXX, Country::XXX, Country::EUR, Country::EUR, Country::EUR, Country::EUR, 
    Country::XCD, Country::XXX, Country::XOF, Country::XXX, Country::XPF, Country::XXX, Country::XXX, Country::XXX, Country::XXX, Country::XXX, 
    Country::XXX, Country::YEM, Country::XXX, Country::XXX, Country::ZAF, Country::ZAF, Country::ZMB, Country::ZMB, Country::XXX, Country::ZWE, 
    Country::ZWE
};

static constexpr short a_NOCURRENCY[2] = { 1, Country::NOCOUNTRY };
static constexpr short a_ADP[2] = { 1, Country::AND };
static constexpr short a_AED[2] = { 1, Country::ARE };
static constexpr short a_AFA[2] = { 1, Country::AFG };
static constexpr short a_AFN[2] = { 1, Country::AFG };
static constexpr short a_ALL[2] = { 1, Country::ALB };
static constexpr short a_AMD[2] = { 1, Country::ARM };
static constexpr short a_ANG[3] = { 2, Country::CUW, Country::SXM };
static constexpr short a_AOA[2] = { 1, Country::AGO };
static constexpr short a_AON[2] = { 1, Country::AGO };
static constexpr short a_AOR[2] = { 1, Country::AGO };
static constexpr short a_ARS[2] = { 1, Country::ARG };
static constexpr short a_ATS[2] = { 1, Country::AUT };
static constexpr short a_AUD[9] = { 8, Country::AUS, Country::CCK, Country::CXR, Country::HMD, Country::KIR, Country::NFK, Country::NRU, Country::TUV };
static constexpr short a_AWG[2] = { 1, Country::ABW };
static constexpr short a_AZM[2] = { 1, Country::AZE };
static constexpr short a_AZN[2] = { 1, Country::AZE };
static constexpr short a_BAD[2] = { 1, Country::BIH };
static constexpr short a_BAM[2] = { 1, Country::BIH };
static constexpr short a_BBD[2] = { 1, Country::BRB };
static constexpr short a_BDT[2] = { 1, Country::BGD };
static constexpr short a_BEC[2] = { 1, Country::BEL };
static constexpr short a_BEF[2] = { 1, Country::BEL };
static constexpr short a_BEL[2] = { 1, Country::BEL };
static constexpr short a_BGL[2] = { 1, Country::BGR };
static constexpr short a_BGN[2] = { 1, Country::BGR };
static constexpr short a_BHD[2] = { 1, Country::BHR };
static constexpr short a_BIF[2] = { 1, Country::BDI };
static constexpr short a_BMD[2] = { 1, Country::BMU };
static constexpr short a_BND[2] = { 1, Country::BRN };
static constexpr short a_BOB[2] = { 1, Country::BOL };
static constexpr short a_BOV[2] = { 1, Country::BOL };
static constexpr short a_BRL[2] = { 1, Country::BRA };
static constexpr short a_BSD[2] = { 1, Country::BHS };
static constexpr short a_BTN[2] = { 1, Country::BTN };
static constexpr short a_BWP[2] = { 1, Country::BWA };
static constexpr short a_BYN[2] = { 1, Country::BLR };
static constexpr short a_BYR[2] = { 1, Country::BLR };
static constexpr short a_BZD[2] = { 1, Country::BLZ };
static constexpr short a_CAD[2] = { 1, Country::CAN };
static constexpr short a_CDF[2] = { 1, Country::COD };
static constexpr short a_CHC[2] = { 1, Country::CHE };
static constexpr short a_CHE[2] = { 1, Country::CHE };
static constexpr short a_CHF[3] = { 2, Country::CHE, Country::LIE };
static constexpr short a_CHW[2] = { 1, Country::CHE };
static constexpr short a_CLF[2] = { 1, Country::CHL };
static constexpr short a_CLP[2] = { 1, Country::CHL };
static constexpr short a_CNY[2] = { 1, Country::CHN };
static constexpr short a_COP[2] = { 1, Country::COL };
static constexpr short a_COU[2] = { 1, Country::COL };
static constexpr short a_CRC[2] = { 1, Country::CRI };
static constexpr short a_CSK[2] = { 1, Country::XXX };
static constexpr short a_CUC[2] = { 1, Country::CUB };
static constexpr short a_CUP[2] = { 1, Country::CUB };
static constexpr short a_CVE[2] = { 1, Country::CPV };
static constexpr short a_CYP[2] = { 1, Country::CYP };
static constexpr short a_CZK[2] = { 1, Country::CZE };
static constexpr short a_DDM[2] = { 1, Country::XXX };
static constexpr short a_DEM[2] = { 1, Country::DEU };
static constexpr short a_DJF[2] = { 1, Country::DJI };
static constexpr short a_DKK[4] = { 3, Country::DNK, Country::FRO, Country::GRL };
static constexpr short a_DOP[2] = { 1, Country::DOM };
static constexpr short a_DZD[2] = { 1, Country::DZA };
static constexpr short a_ECS[2] = { 1, Country::ECU };
static constexpr short a_EEK[2] = { 1, Country::EST };
static constexpr short a_EGP[2] = { 1, Country::EGY };
static constexpr short a_ERN[2] = { 1, Country::ERI };
static constexpr short a_ESA[2] = { 1, Country::ESP };
static constexpr short a_ESB[2] = { 1, Country::ESP };
static constexpr short a_ESP[2] = { 1, Country::ESP };
static constexpr short a_ETB[2] = { 1, Country::ETH };
static constexpr short a_EUR[36] = { 35, Country::ALA, Country::AND, Country::ATF, Country::AUT, Country::BEL, Country::BLM, Country::CYP, Country::DEU, Country::ESP, Country::EST, Country::FIN, Country::FRA, Country::GLP, Country::GRC, Country::GUF, Country::HRV, Country::IRL, Country::ITA, Country::LTU, Country::LUX, Country::LVA, Country::MAF, Country::MCO, Country::MLT, Country::MNE, Country::MTQ, Country::MYT, Country::NLD, Country::PRT, Country::REU, Country::SMR, Country::SPM, Country::SVK, Country::SVN, Country::VAT };
static constexpr short a_FIM[2] = { 1, Country::FIN };
static constexpr short a_FJD[2] = { 1, Country::FJI };
static constexpr short a_FKP[2] = { 1, Country::FLK };
static constexpr short a_FRF[2] = { 1, Country::FRA };
static constexpr short a_GBP[6] = { 5, Country::GBR, Country::GGY, Country::IMN, Country::JEY, Country::SGS };
static constexpr short a_GEL[2] = { 1, Country::GEO };
static constexpr short a_GHS[2] = { 1, Country::GHA };
static constexpr short a_GIP[2] = { 1, Country::GIB };
static constexpr short a_GMD[2] = { 1, Country::GMB };
static constexpr short a_GNF[2] = { 1, Country::GIN };
static constexpr short a_GRD[2] = { 1, Country::GRC };
static constexpr short a_GTQ[2] = { 1, Country::GTM };
static constexpr short a_GWP[2] = { 1, Country::GNB };
static constexpr short a_GYD[2] = { 1, Country::GUY };
static constexpr short a_HKD[2] = { 1, Country::HKG };
static constexpr short a_HNL[2] = { 1, Country::HND };
static constexpr short a_HRK[2] = { 1, Country::HRV };
static constexpr short a_HTG[2] = { 1, Country::HTI };
static constexpr short a_HUF[2] = { 1, Country::HUN };
static constexpr short a_IDR[2] = { 1, Country::IDN };
static constexpr short a_IEP[2] = { 1, Country::IRL };
static constexpr short a_ILS[3] = { 2, Country::ISR, Country::PSE };
static constexpr short a_INR[3] = { 2, Country::BTN, Country::IND };
static constexpr short a_IQD[2] = { 1, Country::IRQ };
static constexpr short a_IRR[2] = { 1, Country::IRN };
static constexpr short a_ISK[2] = { 1, Country::ISL };
static constexpr short a_ITL[2] = { 1, Country::ITA };
static constexpr short a_JMD[2] = { 1, Country::JAM };
static constexpr short a_JOD[2] = { 1, Country::JOR };
static constexpr short a_JPY[2] = { 1, Country::JPN };
static constexpr short a_KES[2] = { 1, Country::KEN };
static constexpr short a_KGS[2] = { 1, Country::KGZ };
static constexpr short a_KHR[2] = { 1, Country::KHM };
static constexpr short a_KMF[2] = { 1, Country::COM };
static constexpr short a_KPW[2] = { 1, Country::PRK };
static constexpr short a_KRW[2] = { 1, Country::KOR };
static constexpr short a_KWD[2] = { 1, Country::KWT };
static constexpr short a_KYD[2] = { 1, Country::CYM };
static constexpr short a_KZT[2] = { 1, Country::KAZ };
static constexpr short a_LAK[2] = { 1, Country::LAO };
static constexpr short a_LBP[2] = { 1, Country::LBN };
static constexpr short a_LKR[2] = { 1, Country::LKA };
static constexpr short a_LRD[2] = { 1, Country::LBR };
static constexpr short a_LSL[2] = { 1, Country::LSO };
static constexpr short a_LTL[2] = { 1, Country::LTU };
static constexpr short a_LUF[2] = { 1, Country::LUX };
static constexpr short a_LVL[2] = { 1, Country::LVA };
static constexpr short a_LYD[2] = { 1, Country::LBY };
static constexpr short a_MAD[3] = { 2, Country::ESH, Country::MAR };
static constexpr short a_MDL[2] = { 1, Country::MDA };
static constexpr short a_MGA[2] = { 1, Country::MDG };
static constexpr short a_MGF[2] = { 1, Country::MDG };
static constexpr short a_MKD[2] = { 1, Country::MKD };
static constexpr short a_MMK[2] = { 1, Country::MMR };
static constexpr short a_MNT[2] = { 1, Country::MNG };
static constexpr short a_MOP[2] = { 1, Country::MAC };
static constexpr short a_MRO[2] = { 1, Country::MRT };
static constexpr short a_MRU[2] = { 1, Country::MRT };
static constexpr short a_MTL[2] = { 1, Country::MLT };
static constexpr short a_MUR[2] = { 1, Country::MUS };
static constexpr short a_MVR[2] = { 1, Country::MDV };
static constexpr short a_MWK[2] = { 1, Country::MWI };
static constexpr short a_MXN[2] = { 1, Country::MEX };
static constexpr short a_MXV[2] = { 1, Country::MEX };
static constexpr short a_MYR[2] = { 1, Country::MYS };
static constexpr short a_MZM[2] = { 1, Country::MOZ };
static constexpr short a_MZN[2] = { 1, Country::MOZ };
static constexpr short a_NAD[2] = { 1, Country::NAM };
static constexpr short a_NGN[2] = { 1, Country::NGA };
static constexpr short a_NIO[2] = { 1, Country::NIC };
static constexpr short a_NLG[2] = { 1, Country::NLD };
static constexpr short a_NOK[4] = { 3, Country::BVT, Country::NOR, Country::SJM };
static constexpr short a_NPR[2] = { 1, Country::NPL };
static constexpr short a_NZD[6] = { 5, Country::COK, Country::NIU, Country::NZL, Country::PCN, Country::TKL };
static constexpr short a_OMR[2] = { 1, Country::OMN };
static constexpr short a_PAB[2] = { 1, Country::PAN };
static constexpr short a_PEN[2] = { 1, Country::PER };
static constexpr short a_PGK[2] = { 1, Country::PNG };
static constexpr short a_PHP[2] = { 1, Country::PHL };
static constexpr short a_PKR[2] = { 1, Country::PAK };
static constexpr short a_PLN[2] = { 1, Country::POL };
static constexpr short a_PLZ[2] = { 1, Country::POL };
static constexpr short a_PTE[2] = { 1, Country::PRT };
static constexpr short a_PYG[2] = { 1, Country::PRY };
static constexpr short a_QAR[2] = { 1, Country::QAT };
static constexpr short a_ROL[2] = { 1, Country::ROU };
static constexpr short a_RON[2] = { 1, Country::ROU };
static constexpr short a_RSD[2] = { 1, Country::SRB };
static constexpr short a_RUB[2] = { 1, Country::RUS };
static constexpr short a_RUR[2] = { 1, Country::XXX };
static constexpr short a_RWF[2] = { 1, Country::RWA };
static constexpr short a_SAR[2] = { 1, Country::SAU };
static constexpr short a_SBD[2] = { 1, Country::SLB };
static constexpr short a_SCR[2] = { 1, Country::SYC };
static constexpr short a_SDD[2] = { 1, Country::SDN };
static constexpr short a_SDG[2] = { 1, Country::SDN };
static constexpr short a_SEK[2] = { 1, Country::SWE };
static constexpr short a_SGD[2] = { 1, Country::SGP };
static constexpr short a_SHP[2] = { 1, Country::SHN };
static constexpr short a_SIT[2] = { 1, Country::SVN };
static constexpr short a_SKK[2] = { 1, Country::SVK };
static constexpr short a_SLE[2] = { 1, Country::SLE };
static constexpr short a_SLL[2] = { 1, Country::SLE };
static constexpr short a_SOS[2] = { 1, Country::SOM };
static constexpr short a_SRD[2] = { 1, Country::SUR };
static constexpr short a_SRG[2] = { 1, Country::SUR };
static constexpr short a_SSP[2] = { 1, Country::SSD };
static constexpr short a_STD[2] = { 1, Country::STP };
static constexpr short a_STN[2] = { 1, Country::STP };
static constexpr short a_SVC[2] = { 1, Country::SLV };
static constexpr short a_SYP[2] = { 1, Country::SYR };
static constexpr short a_SZL[2] = { 1, Country::SWZ };
static constexpr short a_THB[2] = { 1, Country::THA };
static constexpr short a_TJR[2] = { 1, Country::TJK };
static constexpr short a_TJS[2] = { 1, Country::TJK };
static constexpr short a_TMM[2] = { 1, Country::TKM };
static constexpr short a_TMT[2] = { 1, Country::TKM };
static constexpr short a_TND[2] = { 1, Country::TUN };
static constexpr short a_TOP[2] = { 1, Country::TON };
static constexpr short a_TPE[2] = { 1, Country::PRT };
static constexpr short a_TRL[2] = { 1, Country::TUR };
static constexpr short a_TRY[2] = { 1, Country::TUR };
static constexpr short a_TTD[2] = { 1, Country::TTO };
static constexpr short a_TWD[2] = { 1, Country::TWN };
static constexpr short a_TZS[2] = { 1, Country::TZA };
static constexpr short a_UAH[2] = { 1, Country::UKR };
static constexpr short a_UAK[2] = { 1, Country::UKR };
static constexpr short a_UGX[2] = { 1, Country::UGA };
static constexpr short a_USD[20] = { 19, Country::ASM, Country::BES, Country::ECU, Country::FSM, Country::GUM, Country::HTI, Country::IOT, Country::MHL, Country::MNP, Country::PAN, Country::PLW, Country::PRI, Country::SLV, Country::TCA, Country::TLS, Country::UMI, Country::USA, Country::VGB, Country::VIR };
static constexpr short a_USN[2] = { 1, Country::USA };
static constexpr short a_USS[2] = { 1, Country::USA };
static constexpr short a_UYI[2] = { 1, Country::URY };
static constexpr short a_UYU[2] = { 1, Country::URY };
static constexpr short a_UYW[2] = { 1, Country::URY };
static constexpr short a_UZS[2] = { 1, Country::UZB };
static constexpr short a_VEB[2] = { 1, Country::VEN };
static constexpr short a_VED[2] = { 1, Country::VEN };
static constexpr short a_VEF[2] = { 1, Country::VEN };
static constexpr short a_VES[2] = { 1, Country::VEN };
static constexpr short a_VND[2] = { 1, Country::VNM };
static constexpr short a_VUV[2] = { 1, Country::VUT };
static constexpr short a_WST[2] = { 1, Country::WSM };
static constexpr short a_XAF[7] = { 6, Country::CAF, Country::CMR, Country::COG, Country::GAB, Country::GNQ, Country::TCD };
static constexpr short a_XAG[2] = { 1, Country::XXX };
static constexpr short a_XAU[2] = { 1, Country::XXX };
static constexpr short a_XBA[2] = { 1, Country::EUR };
static constexpr short a_XBB[2] = { 1, Country::EUR };
static constexpr short a_XBC[2] = { 1, Country::EUR };
static constexpr short a_XBD[2] = { 1, Country::EUR };
static constexpr short a_XCD[9] = { 8, Country::AIA, Country::ATG, Country::DMA, Country::GRD, Country::KNA, Country::LCA, Country::MSR, Country::VCT };
static constexpr short a_XDR[2] = { 1, Country::XXX };
static constexpr short a_XOF[9] = { 8, Country::BEN, Country::BFA, Country::CIV, Country::GNB, Country::MLI, Country::NER, Country::SEN, Country::TGO };
static constexpr short a_XPD[2] = { 1, Country::XXX };
static constexpr short a_XPF[4] = { 3, Country::NCL, Country::PYF, Country::WLF };
static constexpr short a_XPT[2] = { 1, Country::XXX };
static constexpr short a_XSU[2] = { 1, Country::XXX };
static constexpr short a_XTS[2] = { 1, Country::XXX };
static constexpr short a_XUA[2] = { 1, Country::XXX };
static constexpr short a_XXX[3] = { 2, Country::ATA, Country::XXX };
static constexpr short a_YDD[2] = { 1, Country::XXX };
static constexpr short a_YER[2] = { 1, Country::YEM };
static constexpr short a_YUD[2] = { 1, Country::XXX };
static constexpr short a_YUM[2] = { 1, Country::XXX };
static constexpr short a_ZAL[2] = { 1, Country::ZAF };
static constexpr short a_ZAR[4] = { 3, Country::LSO, Country::NAM, Country::ZAF };
static constexpr short a_ZMK[2] = { 1, Country::ZMB };
static constexpr short a_ZMW[2] = { 1, Country::ZMB };
static constexpr short a_ZRN[2] = { 1, Country::XXX };
static constexpr short a_ZWD[2] = { 1, Country::ZWE };
static constexpr short a_ZWL[2] = { 1, Country::ZWE };

constexpr const short * Gazetteer::m_ccy2cids[Currency::NUMCURRENCY] = { 
a_NOCURRENCY,
a_ADP, a_AED, a_AFA, a_AFN, a_ALL, a_AMD, a_ANG, a_AOA, a_AON, a_AOR, 
a_ARS, a_ATS, a_AUD, a_AWG, a_AZM, a_AZN, a_BAD, a_BAM, a_BBD, a_BDT, 
a_BEC, a_BEF, a_BEL, a_BGL, a_BGN, a_BHD, a_BIF, a_BMD, a_BND, a_BOB, 
a_BOV, a_BRL, a_BSD, a_BTN, a_BWP, a_BYN, a_BYR, a_BZD, a_CAD, a_CDF, 
a_CHC, a_CHE, a_CHF, a_CHW, a_CLF, a_CLP, a_CNY, a_COP, a_COU, a_CRC, 
a_CSK, a_CUC, a_CUP, a_CVE, a_CYP, a_CZK, a_DDM, a_DEM, a_DJF, a_DKK, 
a_DOP, a_DZD, a_ECS, a_EEK, a_EGP, a_ERN, a_ESA, a_ESB, a_ESP, a_ETB, 
a_EUR, a_FIM, a_FJD, a_FKP, a_FRF, a_GBP, a_GEL, a_GHS, a_GIP, a_GMD, 
a_GNF, a_GRD, a_GTQ, a_GWP, a_GYD, a_HKD, a_HNL, a_HRK, a_HTG, a_HUF, 
a_IDR, a_IEP, a_ILS, a_INR, a_IQD, a_IRR, a_ISK, a_ITL, a_JMD, a_JOD, 
a_JPY, a_KES, a_KGS, a_KHR, a_KMF, a_KPW, a_KRW, a_KWD, a_KYD, a_KZT, 
a_LAK, a_LBP, a_LKR, a_LRD, a_LSL, a_LTL, a_LUF, a_LVL, a_LYD, a_MAD, 
a_MDL, a_MGA, a_MGF, a_MKD, a_MMK, a_MNT, a_MOP, a_MRO, a_MRU, a_MTL, 
a_MUR, a_MVR, a_MWK, a_MXN, a_MXV, a_MYR, a_MZM, a_MZN, a_NAD, a_NGN, 
a_NIO, a_NLG, a_NOK, a_NPR, a_NZD, a_OMR, a_PAB, a_PEN, a_PGK, a_PHP, 
a_PKR, a_PLN, a_PLZ, a_PTE, a_PYG, a_QAR, a_ROL, a_RON, a_RSD, a_RUB, 
a_RUR, a_RWF, a_SAR, a_SBD, a_SCR, a_SDD, a_SDG, a_SEK, a_SGD, a_SHP, 
a_SIT, a_SKK, a_SLE, a_SLL, a_SOS, a_SRD, a_SRG, a_SSP, a_STD, a_STN, 
a_SVC, a_SYP, a_SZL, a_THB, a_TJR, a_TJS, a_TMM, a_TMT, a_TND, a_TOP, 
a_TPE, a_TRL, a_TRY, a_TTD, a_TWD, a_TZS, a_UAH, a_UAK, a_UGX, a_USD, 
a_USN, a_USS, a_UYI, a_UYU, a_UYW, a_UZS, a_VEB, a_VED, a_VEF, a_VES, 
a_VND, a_VUV, a_WST, a_XAF, a_XAG, a_XAU, a_XBA, a_XBB, a_XBC, a_XBD, 
a_XCD, a_XDR, a_XOF, a_XPD, a_XPF, a_XPT, a_XSU, a_XTS, a_XUA, a_XXX, 
a_YDD, a_YER, a_YUD, a_YUM, a_ZAL, a_ZAR, a_ZMK, a_ZMW, a_ZRN, a_ZWD, 
a_ZWL,  };

static constexpr short b_NOCOUNTRY[2] = { 1, Currency::NOCURRENCY };
static constexpr short b_ABW[2] = { 1, Currency::AWG };
static constexpr short b_AFG[2] = { 1, Currency::AFN };
static constexpr short b_AGO[2] = { 1, Currency::AOA };
static constexpr short b_AIA[2] = { 1, Currency::XCD };
static constexpr short b_ALA[2] = { 1, Currency::EUR };
static constexpr short b_ALB[2] = { 1, Currency::ALL };
static constexpr short b_AND[2] = { 1, Currency::EUR };
static constexpr short b_ARE[2] = { 1, Currency::AED };
static constexpr short b_ARG[2] = { 1, Currency::ARS };
static constexpr short b_ARM[2] = { 1, Currency::AMD };
static constexpr short b_ASM[2] = { 1, Currency::USD };
static constexpr short b_ATA[2] = { 1, Currency::XXX };
static constexpr short b_ATF[2] = { 1, Currency::EUR };
static constexpr short b_ATG[2] = { 1, Currency::XCD };
static constexpr short b_AUS[2] = { 1, Currency::AUD };
static constexpr short b_AUT[2] = { 1, Currency::EUR };
static constexpr short b_AZE[2] = { 1, Currency::AZN };
static constexpr short b_BDI[2] = { 1, Currency::BIF };
static constexpr short b_BEL[2] = { 1, Currency::EUR };
static constexpr short b_BEN[2] = { 1, Currency::XOF };
static constexpr short b_BES[2] = { 1, Currency::USD };
static constexpr short b_BFA[2] = { 1, Currency::XOF };
static constexpr short b_BGD[2] = { 1, Currency::BDT };
static constexpr short b_BGR[2] = { 1, Currency::BGN };
static constexpr short b_BHR[2] = { 1, Currency::BHD };
static constexpr short b_BHS[2] = { 1, Currency::BSD };
static constexpr short b_BIH[2] = { 1, Currency::BAM };
static constexpr short b_BLM[2] = { 1, Currency::EUR };
static constexpr short b_BLR[2] = { 1, Currency::BYN };
static constexpr short b_BLZ[2] = { 1, Currency::BZD };
static constexpr short b_BMU[2] = { 1, Currency::BMD };
static constexpr short b_BOL[3] = { 2, Currency::BOB, Currency::BOV };
static constexpr short b_BRA[2] = { 1, Currency::BRL };
static constexpr short b_BRB[2] = { 1, Currency::BBD };
static constexpr short b_BRN[2] = { 1, Currency::BND };
static constexpr short b_BTN[3] = { 2, Currency::INR, Currency::BTN };
static constexpr short b_BVT[2] = { 1, Currency::NOK };
static constexpr short b_BWA[2] = { 1, Currency::BWP };
static constexpr short b_CAF[2] = { 1, Currency::XAF };
static constexpr short b_CAN[2] = { 1, Currency::CAD };
static constexpr short b_CCK[2] = { 1, Currency::AUD };
static constexpr short b_CHE[4] = { 3, Currency::CHF, Currency::CHE, Currency::CHW };
static constexpr short b_CHL[3] = { 2, Currency::CLP, Currency::CLF };
static constexpr short b_CHN[2] = { 1, Currency::CNY };
static constexpr short b_CIV[2] = { 1, Currency::XOF };
static constexpr short b_CMR[2] = { 1, Currency::XAF };
static constexpr short b_COD[2] = { 1, Currency::CDF };
static constexpr short b_COG[2] = { 1, Currency::XAF };
static constexpr short b_COK[2] = { 1, Currency::NZD };
static constexpr short b_COL[3] = { 2, Currency::COP, Currency::COU };
static constexpr short b_COM[2] = { 1, Currency::KMF };
static constexpr short b_CPV[2] = { 1, Currency::CVE };
static constexpr short b_CRI[2] = { 1, Currency::CRC };
static constexpr short b_CUB[3] = { 2, Currency::CUP, Currency::CUC };
static constexpr short b_CUW[2] = { 1, Currency::ANG };
static constexpr short b_CXR[2] = { 1, Currency::AUD };
static constexpr short b_CYM[2] = { 1, Currency::KYD };
static constexpr short b_CYP[2] = { 1, Currency::EUR };
static constexpr short b_CZE[2] = { 1, Currency::CZK };
static constexpr short b_DEU[2] = { 1, Currency::EUR };
static constexpr short b_DJI[2] = { 1, Currency::DJF };
static constexpr short b_DMA[2] = { 1, Currency::XCD };
static constexpr short b_DNK[2] = { 1, Currency::DKK };
static constexpr short b_DOM[2] = { 1, Currency::DOP };
static constexpr short b_DZA[2] = { 1, Currency::DZD };
static constexpr short b_ECU[2] = { 1, Currency::USD };
static constexpr short b_EGY[2] = { 1, Currency::EGP };
static constexpr short b_ERI[2] = { 1, Currency::ERN };
static constexpr short b_ESH[2] = { 1, Currency::MAD };
static constexpr short b_ESP[2] = { 1, Currency::EUR };
static constexpr short b_EST[2] = { 1, Currency::EUR };
static constexpr short b_ETH[2] = { 1, Currency::ETB };
static constexpr short b_EUR[2] = { 1, Currency::EUR };
static constexpr short b_FIN[2] = { 1, Currency::EUR };
static constexpr short b_FJI[2] = { 1, Currency::FJD };
static constexpr short b_FLK[2] = { 1, Currency::FKP };
static constexpr short b_FRA[2] = { 1, Currency::EUR };
static constexpr short b_FRO[2] = { 1, Currency::DKK };
static constexpr short b_FSM[2] = { 1, Currency::USD };
static constexpr short b_GAB[2] = { 1, Currency::XAF };
static constexpr short b_GBR[2] = { 1, Currency::GBP };
static constexpr short b_GEO[2] = { 1, Currency::GEL };
static constexpr short b_GGY[2] = { 1, Currency::GBP };
static constexpr short b_GHA[2] = { 1, Currency::GHS };
static constexpr short b_GIB[2] = { 1, Currency::GIP };
static constexpr short b_GIN[2] = { 1, Currency::GNF };
static constexpr short b_GLP[2] = { 1, Currency::EUR };
static constexpr short b_GMB[2] = { 1, Currency::GMD };
static constexpr short b_GNB[2] = { 1, Currency::XOF };
static constexpr short b_GNQ[2] = { 1, Currency::XAF };
static constexpr short b_GRC[2] = { 1, Currency::EUR };
static constexpr short b_GRD[2] = { 1, Currency::XCD };
static constexpr short b_GRL[2] = { 1, Currency::DKK };
static constexpr short b_GTM[2] = { 1, Currency::GTQ };
static constexpr short b_GUF[2] = { 1, Currency::EUR };
static constexpr short b_GUM[2] = { 1, Currency::USD };
static constexpr short b_GUY[2] = { 1, Currency::GYD };
static constexpr short b_HKG[2] = { 1, Currency::HKD };
static constexpr short b_HMD[2] = { 1, Currency::AUD };
static constexpr short b_HND[2] = { 1, Currency::HNL };
static constexpr short b_HRV[2] = { 1, Currency::EUR };
static constexpr short b_HTI[3] = { 2, Currency::HTG, Currency::USD };
static constexpr short b_HUN[2] = { 1, Currency::HUF };
static constexpr short b_IDN[2] = { 1, Currency::IDR };
static constexpr short b_IMN[2] = { 1, Currency::GBP };
static constexpr short b_IND[2] = { 1, Currency::INR };
static constexpr short b_IOT[2] = { 1, Currency::USD };
static constexpr short b_IRL[2] = { 1, Currency::EUR };
static constexpr short b_IRN[2] = { 1, Currency::IRR };
static constexpr short b_IRQ[2] = { 1, Currency::IQD };
static constexpr short b_ISL[2] = { 1, Currency::ISK };
static constexpr short b_ISR[2] = { 1, Currency::ILS };
static constexpr short b_ITA[2] = { 1, Currency::EUR };
static constexpr short b_JAM[2] = { 1, Currency::JMD };
static constexpr short b_JEY[2] = { 1, Currency::GBP };
static constexpr short b_JOR[2] = { 1, Currency::JOD };
static constexpr short b_JPN[2] = { 1, Currency::JPY };
static constexpr short b_KAZ[2] = { 1, Currency::KZT };
static constexpr short b_KEN[2] = { 1, Currency::KES };
static constexpr short b_KGZ[2] = { 1, Currency::KGS };
static constexpr short b_KHM[2] = { 1, Currency::KHR };
static constexpr short b_KIR[2] = { 1, Currency::AUD };
static constexpr short b_KNA[2] = { 1, Currency::XCD };
static constexpr short b_KOR[2] = { 1, Currency::KRW };
static constexpr short b_KWT[2] = { 1, Currency::KWD };
static constexpr short b_LAO[2] = { 1, Currency::LAK };
static constexpr short b_LBN[2] = { 1, Currency::LBP };
static constexpr short b_LBR[2] = { 1, Currency::LRD };
static constexpr short b_LBY[2] = { 1, Currency::LYD };
static constexpr short b_LCA[2] = { 1, Currency::XCD };
static constexpr short b_LIE[2] = { 1, Currency::CHF };
static constexpr short b_LKA[2] = { 1, Currency::LKR };
static constexpr short b_LSO[3] = { 2, Currency::LSL, Currency::ZAR };
static constexpr short b_LTU[2] = { 1, Currency::EUR };
static constexpr short b_LUX[2] = { 1, Currency::EUR };
static constexpr short b_LVA[2] = { 1, Currency::EUR };
static constexpr short b_MAC[2] = { 1, Currency::MOP };
static constexpr short b_MAF[2] = { 1, Currency::EUR };
static constexpr short b_MAR[2] = { 1, Currency::MAD };
static constexpr short b_MCO[2] = { 1, Currency::EUR };
static constexpr short b_MDA[2] = { 1, Currency::MDL };
static constexpr short b_MDG[2] = { 1, Currency::MGA };
static constexpr short b_MDV[2] = { 1, Currency::MVR };
static constexpr short b_MEX[3] = { 2, Currency::MXN, Currency::MXV };
static constexpr short b_MHL[2] = { 1, Currency::USD };
static constexpr short b_MKD[2] = { 1, Currency::MKD };
static constexpr short b_MLI[2] = { 1, Currency::XOF };
static constexpr short b_MLT[2] = { 1, Currency::EUR };
static constexpr short b_MMR[2] = { 1, Currency::MMK };
static constexpr short b_MNE[2] = { 1, Currency::EUR };
static constexpr short b_MNG[2] = { 1, Currency::MNT };
static constexpr short b_MNP[2] = { 1, Currency::USD };
static constexpr short b_MOZ[2] = { 1, Currency::MZN };
static constexpr short b_MRT[2] = { 1, Currency::MRU };
static constexpr short b_MSR[2] = { 1, Currency::XCD };
static constexpr short b_MTQ[2] = { 1, Currency::EUR };
static constexpr short b_MUS[2] = { 1, Currency::MUR };
static constexpr short b_MWI[2] = { 1, Currency::MWK };
static constexpr short b_MYS[2] = { 1, Currency::MYR };
static constexpr short b_MYT[2] = { 1, Currency::EUR };
static constexpr short b_NAM[3] = { 2, Currency::NAD, Currency::ZAR };
static constexpr short b_NCL[2] = { 1, Currency::XPF };
static constexpr short b_NER[2] = { 1, Currency::XOF };
static constexpr short b_NFK[2] = { 1, Currency::AUD };
static constexpr short b_NGA[2] = { 1, Currency::NGN };
static constexpr short b_NIC[2] = { 1, Currency::NIO };
static constexpr short b_NIU[2] = { 1, Currency::NZD };
static constexpr short b_NLD[2] = { 1, Currency::EUR };
static constexpr short b_NOR[2] = { 1, Currency::NOK };
static constexpr short b_NPL[2] = { 1, Currency::NPR };
static constexpr short b_NRU[2] = { 1, Currency::AUD };
static constexpr short b_NZL[2] = { 1, Currency::NZD };
static constexpr short b_OMN[2] = { 1, Currency::OMR };
static constexpr short b_PAK[2] = { 1, Currency::PKR };
static constexpr short b_PAN[3] = { 2, Currency::PAB, Currency::USD };
static constexpr short b_PCN[2] = { 1, Currency::NZD };
static constexpr short b_PER[2] = { 1, Currency::PEN };
static constexpr short b_PHL[2] = { 1, Currency::PHP };
static constexpr short b_PLW[2] = { 1, Currency::USD };
static constexpr short b_PNG[2] = { 1, Currency::PGK };
static constexpr short b_POL[2] = { 1, Currency::PLN };
static constexpr short b_PRI[2] = { 1, Currency::USD };
static constexpr short b_PRK[2] = { 1, Currency::KPW };
static constexpr short b_PRT[2] = { 1, Currency::EUR };
static constexpr short b_PRY[2] = { 1, Currency::PYG };
static constexpr short b_PSE[2] = { 1, Currency::ILS };
static constexpr short b_PYF[2] = { 1, Currency::XPF };
static constexpr short b_QAT[2] = { 1, Currency::QAR };
static constexpr short b_REU[2] = { 1, Currency::EUR };
static constexpr short b_ROU[2] = { 1, Currency::RON };
static constexpr short b_RUS[2] = { 1, Currency::RUB };
static constexpr short b_RWA[2] = { 1, Currency::RWF };
static constexpr short b_SAU[2] = { 1, Currency::SAR };
static constexpr short b_SDN[2] = { 1, Currency::SDG };
static constexpr short b_SEN[2] = { 1, Currency::XOF };
static constexpr short b_SGP[2] = { 1, Currency::SGD };
static constexpr short b_SGS[2] = { 1, Currency::GBP };
static constexpr short b_SHN[2] = { 1, Currency::SHP };
static constexpr short b_SJM[2] = { 1, Currency::NOK };
static constexpr short b_SLB[2] = { 1, Currency::SBD };
static constexpr short b_SLE[3] = { 2, Currency::SLL, Currency::SLE };
static constexpr short b_SLV[3] = { 2, Currency::SVC, Currency::USD };
static constexpr short b_SMR[2] = { 1, Currency::EUR };
static constexpr short b_SOM[2] = { 1, Currency::SOS };
static constexpr short b_SPM[2] = { 1, Currency::EUR };
static constexpr short b_SRB[2] = { 1, Currency::RSD };
static constexpr short b_SSD[2] = { 1, Currency::SSP };
static constexpr short b_STP[2] = { 1, Currency::STN };
static constexpr short b_SUR[2] = { 1, Currency::SRD };
static constexpr short b_SVK[2] = { 1, Currency::EUR };
static constexpr short b_SVN[2] = { 1, Currency::EUR };
static constexpr short b_SWE[2] = { 1, Currency::SEK };
static constexpr short b_SWZ[2] = { 1, Currency::SZL };
static constexpr short b_SXM[2] = { 1, Currency::ANG };
static constexpr short b_SYC[2] = { 1, Currency::SCR };
static constexpr short b_SYR[2] = { 1, Currency::SYP };
static constexpr short b_TCA[2] = { 1, Currency::USD };
static constexpr short b_TCD[2] = { 1, Currency::XAF };
static constexpr short b_TGO[2] = { 1, Currency::XOF };
static constexpr short b_THA[2] = { 1, Currency::THB };
static constexpr short b_TJK[2] = { 1, Currency::TJS };
static constexpr short b_TKL[2] = { 1, Currency::NZD };
static constexpr short b_TKM[2] = { 1, Currency::TMT };
static constexpr short b_TLS[2] = { 1, Currency::USD };
static constexpr short b_TON[2] = { 1, Currency::TOP };
static constexpr short b_TTO[2] = { 1, Currency::TTD };
static constexpr short b_TUN[2] = { 1, Currency::TND };
static constexpr short b_TUR[2] = { 1, Currency::TRY };
static constexpr short b_TUV[2] = { 1, Currency::AUD };
static constexpr short b_TWN[2] = { 1, Currency::TWD };
static constexpr short b_TZA[2] = { 1, Currency::TZS };
static constexpr short b_UGA[2] = { 1, Currency::UGX };
static constexpr short b_UKR[2] = { 1, Currency::UAH };
static constexpr short b_UMI[2] = { 1, Currency::USD };
static constexpr short b_URY[4] = { 3, Currency::UYU, Currency::UYI, Currency::UYW };
static constexpr short b_USA[3] = { 2, Currency::USD, Currency::USN };
static constexpr short b_UZB[2] = { 1, Currency::UZS };
static constexpr short b_VAT[2] = { 1, Currency::EUR };
static constexpr short b_VCT[2] = { 1, Currency::XCD };
static constexpr short b_VEN[3] = { 2, Currency::VES, Currency::VED };
static constexpr short b_VGB[2] = { 1, Currency::USD };
static constexpr short b_VIR[2] = { 1, Currency::USD };
static constexpr short b_VNM[2] = { 1, Currency::VND };
static constexpr short b_VUT[2] = { 1, Currency::VUV };
static constexpr short b_WLF[2] = { 1, Currency::XPF };
static constexpr short b_WSM[2] = { 1, Currency::WST };
static constexpr short b_XAF[2] = { 1, Currency::XAF };
static constexpr short b_XCD[2] = { 1, Currency::XCD };
static constexpr short b_XOF[2] = { 1, Currency::XOF };
static constexpr short b_XPF[2] = { 1, Currency::XPF };
static constexpr short b_XXX[10] = { 9, Currency::XAU, Currency::XAG, Currency::XPT, Currency::XPD, Currency::XXX, Currency::XDR, Currency::XTS, Currency::XUA, Currency::XSU };
static constexpr short b_YEM[2] = { 1, Currency::YER };
static constexpr short b_ZAF[2] = { 1, Currency::ZAR };
static constexpr short b_ZMB[2] = { 1, Currency::ZMW };
static constexpr short b_ZWE[2] = { 1, Currency::ZWL };

constexpr const short * Gazetteer::m_cid2ccys[Country::NUMCOUNTRY] = { 
b_NOCOUNTRY,
b_ABW, b_AFG, b_AGO, b_AIA, b_ALA, b_ALB, b_AND, b_ARE, b_ARG, b_ARM, 
b_ASM, b_ATA, b_ATF, b_ATG, b_AUS, b_AUT, b_AZE, b_BDI, b_BEL, b_BEN, 
b_BES, b_BFA, b_BGD, b_BGR, b_BHR, b_BHS, b_BIH, b_BLM, b_BLR, b_BLZ, 
b_BMU, b_BOL, b_BRA, b_BRB, b_BRN, b_BTN, b_BVT, b_BWA, b_CAF, b_CAN, 
b_CCK, b_CHE, b_CHL, b_CHN, b_CIV, b_CMR, b_COD, b_COG, b_COK, b_COL, 
b_COM, b_CPV, b_CRI, b_CUB, b_CUW, b_CXR, b_CYM, b_CYP, b_CZE, b_DEU, 
b_DJI, b_DMA, b_DNK, b_DOM, b_DZA, b_ECU, b_EGY, b_ERI, b_ESH, b_ESP, 
b_EST, b_ETH, b_EUR, b_FIN, b_FJI, b_FLK, b_FRA, b_FRO, b_FSM, b_GAB, 
b_GBR, b_GEO, b_GGY, b_GHA, b_GIB, b_GIN, b_GLP, b_GMB, b_GNB, b_GNQ, 
b_GRC, b_GRD, b_GRL, b_GTM, b_GUF, b_GUM, b_GUY, b_HKG, b_HMD, b_HND, 
b_HRV, b_HTI, b_HUN, b_IDN, b_IMN, b_IND, b_IOT, b_IRL, b_IRN, b_IRQ, 
b_ISL, b_ISR, b_ITA, b_JAM, b_JEY, b_JOR, b_JPN, b_KAZ, b_KEN, b_KGZ, 
b_KHM, b_KIR, b_KNA, b_KOR, b_KWT, b_LAO, b_LBN, b_LBR, b_LBY, b_LCA, 
b_LIE, b_LKA, b_LSO, b_LTU, b_LUX, b_LVA, b_MAC, b_MAF, b_MAR, b_MCO, 
b_MDA, b_MDG, b_MDV, b_MEX, b_MHL, b_MKD, b_MLI, b_MLT, b_MMR, b_MNE, 
b_MNG, b_MNP, b_MOZ, b_MRT, b_MSR, b_MTQ, b_MUS, b_MWI, b_MYS, b_MYT, 
b_NAM, b_NCL, b_NER, b_NFK, b_NGA, b_NIC, b_NIU, b_NLD, b_NOR, b_NPL, 
b_NRU, b_NZL, b_OMN, b_PAK, b_PAN, b_PCN, b_PER, b_PHL, b_PLW, b_PNG, 
b_POL, b_PRI, b_PRK, b_PRT, b_PRY, b_PSE, b_PYF, b_QAT, b_REU, b_ROU, 
b_RUS, b_RWA, b_SAU, b_SDN, b_SEN, b_SGP, b_SGS, b_SHN, b_SJM, b_SLB, 
b_SLE, b_SLV, b_SMR, b_SOM, b_SPM, b_SRB, b_SSD, b_STP, b_SUR, b_SVK, 
b_SVN, b_SWE, b_SWZ, b_SXM, b_SYC, b_SYR, b_TCA, b_TCD, b_TGO, b_THA, 
b_TJK, b_TKL, b_TKM, b_TLS, b_TON, b_TTO, b_TUN, b_TUR, b_TUV, b_TWN, 
b_TZA, b_UGA, b_UKR, b_UMI, b_URY, b_USA, b_UZB, b_VAT, b_VCT, b_VEN, 
b_VGB, b_VIR, b_VNM, b_VUT, b_WLF, b_WSM, b_XAF, b_XCD, b_XOF, b_XPF, 
b_XXX, b_YEM, b_ZAF, b_ZMB, b_ZWE,  };

static constexpr short c_NOCOUNTRY[2] = { 1, City::NOCITY };
static constexpr short c_ABW[2] = { 1, City::AUA };
static constexpr short c_AFG[6] = { 5, City::HEA, City::JAA, City::KBL, City::KDH, City::MZR };
static constexpr short c_AGO[6] = { 5, City::BUG, City::CAB, City::JMB, City::LAD, City::UGO };
static constexpr short c_AIA[2] = { 1, City::AXA };
static constexpr short c_ALA[2] = { 1, City::MHQ };
static constexpr short c_ALB[2] = { 1, City::TIA };
static constexpr short c_AND[2] = { 1, City::ALV };
static constexpr short c_ARE[7] = { 6, City::AAN, City::AUH, City::DXB, City::FJR, City::RKT, City::SHJ };
static constexpr short c_ARG[21] = { 20, City::BRC, City::BUE, City::CNQ, City::COR, City::CRD, City::CTC, City::IGR, City::JNI, City::JSM, City::JUJ, City::LUQ, City::MDQ, City::MDZ, City::RGL, City::ROS, City::RSA, City::SFN, City::SLA, City::TUC, City::USH };
static constexpr short c_ARM[2] = { 1, City::EVN };
static constexpr short c_ASM[2] = { 1, City::PPG };
static constexpr short c_ATA[2] = { 1, City::XXX };
static constexpr short c_ATF[2] = { 1, City::TF0 };
static constexpr short c_ATG[2] = { 1, City::ANU };
static constexpr short c_AUS[132] = { 131, City::ABM, City::ABX, City::ADL, City::ALH, City::ASP, City::AYQ, City::AYR, City::BDB, City::BHQ, City::BLT, City::BME, City::BMP, City::BNE, City::BNK, City::BWT, City::CBR, City::CED, City::CES, City::CFS, City::CMQ, City::CNS, City::CPD, City::CSI, City::CTN, City::CVQ, City::CXT, City::DBO, City::DBY, City::DDI, City::DKI, City::DPO, City::DRB, City::DRW, City::DYA, City::EMD, City::EPR, City::GET, City::GEX, City::GFF, City::GKL, City::GLT, City::GOO, City::GOV, City::GTE, City::GYP, City::HBA, City::HIS, City::HLT, City::HNK, City::HTI, City::HVB, City::IFL, City::IGH, City::IRG, City::ISA, City::JAD, City::JCK, City::JHQ, City::JUN, City::KBY, City::KCE, City::KGC, City::KGI, City::KNS, City::KNX, City::KRB, City::KTA, City::KTR, City::KWM, City::LDC, City::LER, City::LNO, City::LRE, City::LST, City::LSY, City::LVO, City::MBH, City::MCY, City::MEL, City::MGB, City::MIM, City::MKR, City::MKY, City::MMG, City::MMM, City::MOV, City::MQL, City::MRZ, City::MTL, City::MYA, City::NAA, City::NRA, City::NSA, City::NSO, City::NTL, City::OAG, City::OLP, City::OOL, City::OOM, City::PBO, City::PER, City::PHE, City::PLO, City::PPP, City::PQQ, City::PTJ, City::PUG, City::ROK, City::SIX, City::SYD, City::TCA, City::TEM, City::TIS, City::TMW, City::TPR, City::TRO, City::TSV, City::TWB, City::UEE, City::UMR, City::WAM, City::WEI, City::WGA, City::WMB, City::WOL, City::WSY, City::WUN, City::WYA, City::WYN, City::ZBO, City::ZNE };
static constexpr short c_AUT[7] = { 6, City::GRZ, City::INN, City::KLU, City::LNZ, City::SZG, City::VIE };
static constexpr short c_AZE[3] = { 2, City::BAK, City::NAJ };
static constexpr short c_BDI[3] = { 2, City::BJM, City::GID };
static constexpr short c_BEL[6] = { 5, City::ANR, City::BRU, City::GNE, City::LGG, City::LV0 };
static constexpr short c_BEN[3] = { 2, City::COO, City::PT0 };
static constexpr short c_BES[2] = { 1, City::BON };
static constexpr short c_BFA[3] = { 2, City::BOY, City::OUA };
static constexpr short c_BGD[5] = { 4, City::BZL, City::CGP, City::DAC, City::ZYL };
static constexpr short c_BGR[10] = { 9, City::BOJ, City::GOZ, City::JAM, City::ROU, City::SLS, City::SOF, City::TGV, City::VAR, City::VID };
static constexpr short c_BHR[2] = { 1, City::BAH };
static constexpr short c_BHS[10] = { 9, City::CCZ, City::ELH, City::FPO, City::GHB, City::MHH, City::NAS, City::RSD, City::TCB, City::ZSA };
static constexpr short c_BIH[4] = { 3, City::BNX, City::OMO, City::SJJ };
static constexpr short c_BLM[2] = { 1, City::SBH };
static constexpr short c_BLR[2] = { 1, City::MSQ };
static constexpr short c_BLZ[3] = { 2, City::BCV, City::BZE };
static constexpr short c_BMU[2] = { 1, City::BDA };
static constexpr short c_BOL[5] = { 4, City::CBB, City::LPB, City::SRB, City::SRZ };
static constexpr short c_BRA[44] = { 43, City::AJU, City::AUX, City::BEL, City::BSB, City::BVB, City::CGB, City::CGR, City::CNF, City::CWB, City::ERN, City::FLN, City::FOR, City::GYN, City::JCB, City::JCM, City::JDF, City::JLS, City::JNA, City::JOI, City::JPA, City::JTI, City::MAO, City::MCP, City::MCZ, City::MGF, City::NAT, City::PA0, City::PMW, City::POA, City::PVH, City::QGF, City::RBR, City::REC, City::RIO, City::SAO, City::SLZ, City::SRA, City::SSA, City::THE, City::UBA, City::UDI, City::URG, City::VIX };
static constexpr short c_BRB[2] = { 1, City::BGI };
static constexpr short c_BRN[2] = { 1, City::BWN };
static constexpr short c_BTN[2] = { 1, City::PBH };
static constexpr short c_BVT[2] = { 1, City::OSL };
static constexpr short c_BWA[6] = { 5, City::FRW, City::GBE, City::JWA, City::MUB, City::PKW };
static constexpr short c_CAF[9] = { 8, City::BBT, City::BBY, City::BGF, City::BGU, City::BIV, City::CRF, City::IRO, City::ODA };
static constexpr short c_CAN[70] = { 69, City::RR0, City::XPK, City::XSI, City::YAT, City::YBE, City::YCB, City::YDF, City::YEA, City::YEV, City::YFA, City::YFB, City::YFC, City::YFO, City::YGW, City::YGX, City::YHR, City::YHZ, City::YIF, City::YKA, City::YLB, City::YLR, City::YLW, City::YMM, City::YMQ, City::YOP, City::YOW, City::YPN, City::YPR, City::YQB, City::YQD, City::YQG, City::YQM, City::YQR, City::YQT, City::YQX, City::YRB, City::YSJ, City::YSM, City::YSR, City::YTH, City::YTO, City::YUD, City::YUX, City::YVB, City::YVO, City::YVP, City::YVQ, City::YVR, City::YWG, City::YWK, City::YXE, City::YXJ, City::YXN, City::YXS, City::YXT, City::YXU, City::YXY, City::YYC, City::YYD, City::YYJ, City::YYN, City::YYQ, City::YYR, City::YYT, City::YYZ, City::YZF, City::YZP, City::ZKE, City::ZTM };
static constexpr short c_CCK[2] = { 1, City::CCK };
static constexpr short c_CHE[8] = { 7, City::ACH, City::BRN, City::BSL, City::GVA, City::LU0, City::LUG, City::ZRH };
static constexpr short c_CHL[7] = { 6, City::CJC, City::ESR, City::IPC, City::PUQ, City::STI, City::VAP };
static constexpr short c_CHN[40] = { 39, City::AAT, City::BJS, City::CAN, City::CGO, City::CGQ, City::CHG, City::CKG, City::CTU, City::DLC, City::HGH, City::HLH, City::HRB, City::JDZ, City::JGN, City::JGS, City::JHG, City::JIL, City::JIU, City::JJN, City::JMU, City::JNG, City::JNZ, City::KWL, City::LUM, City::NNG, City::RIZ, City::SHA, City::SHE, City::SZX, City::TAO, City::TNA, City::TSN, City::TYN, City::URC, City::WUH, City::WUX, City::XIY, City::XMN, City::YIH };
static constexpr short c_CIV[9] = { 8, City::ABJ, City::ASK, City::BYK, City::DJO, City::HGO, City::MJC, City::SPY, City::ZSS };
static constexpr short c_CMR[6] = { 5, City::DLA, City::GOU, City::MVR, City::NGE, City::YAO };
static constexpr short c_COD[5] = { 4, City::FBM, City::FIH, City::FKI, City::LIQ };
static constexpr short c_COG[3] = { 2, City::BZV, City::PNR };
static constexpr short c_COK[2] = { 1, City::RAR };
static constexpr short c_COL[10] = { 9, City::ADZ, City::BAQ, City::BGA, City::BOG, City::CLO, City::CTG, City::MDE, City::PEI, City::SSL };
static constexpr short c_COM[3] = { 2, City::AJN, City::HAH };
static constexpr short c_CPV[3] = { 2, City::RAI, City::SID };
static constexpr short c_CRI[3] = { 2, City::SJO, City::UPL };
static constexpr short c_CUB[6] = { 5, City::CFG, City::HAV, City::HOG, City::SCU, City::VRA };
static constexpr short c_CUW[2] = { 1, City::CUR };
static constexpr short c_CXR[2] = { 1, City::XCH };
static constexpr short c_CYM[2] = { 1, City::GCM };
static constexpr short c_CYP[6] = { 5, City::AKT, City::LCA, City::NIC, City::PFO, City::QLI };
static constexpr short c_CZE[5] = { 4, City::BRQ, City::OSR, City::PRG, City::UHE };
static constexpr short c_DEU[33] = { 32, City::AGB, City::BER, City::BNJ, City::BRE, City::BYU, City::CBU, City::CGN, City::DRS, City::DTM, City::DUS, City::ERF, City::FDH, City::FKB, City::FMO, City::FRA, City::GTI, City::GWT, City::HAJ, City::HAM, City::HOQ, City::JUI, City::KEL, City::LEJ, City::MUC, City::NUE, City::OB0, City::PAD, City::RB0, City::SCN, City::STR, City::SW0, City::WIE };
static constexpr short c_DJI[2] = { 1, City::JIB };
static constexpr short c_DMA[3] = { 2, City::DCF, City::DOM };
static constexpr short c_DNK[17] = { 16, City::AAB, City::AAL, City::AAR, City::BLL, City::CPH, City::EBJ, City::GR0, City::HS0, City::KRP, City::ODE, City::RA0, City::RNN, City::SGD, City::SI0, City::SKS, City::TED };
static constexpr short c_DOM[6] = { 5, City::AZS, City::LRM, City::POP, City::PUJ, City::SDQ };
static constexpr short c_DZA[6] = { 5, City::AAE, City::ALG, City::CZL, City::GJL, City::ORN };
static constexpr short c_ECU[5] = { 4, City::GYE, City::JIP, City::SNC, City::UIO };
static constexpr short c_EGY[18] = { 17, City::AAC, City::ABS, City::ALY, City::ASW, City::ATZ, City::AUE, City::CAI, City::EMY, City::HRG, City::LXR, City::MUH, City::PSD, City::RMF, City::SEW, City::SKV, City::SSH, City::UVL };
static constexpr short c_ERI[2] = { 1, City::ASM };
static constexpr short c_ESH[2] = { 1, City::EUN };
static constexpr short c_ESP[35] = { 34, City::ACE, City::AGP, City::ALC, City::BCN, City::BIO, City::BJZ, City::EAS, City::FUE, City::GRO, City::GRX, City::IBZ, City::LCG, City::LEI, City::LPA, City::MAD, City::MAH, City::MJV, City::ODB, City::OVD, City::PMI, City::QSA, City::REU, City::SCQ, City::SDR, City::SPC, City::SVQ, City::TCI, City::VDE, City::VGO, City::VIT, City::VLC, City::VLL, City::XRY, City::ZAZ };
static constexpr short c_EST[2] = { 1, City::TLL };
static constexpr short c_ETH[5] = { 4, City::ADD, City::BCO, City::JIJ, City::JIM };
static constexpr short c_EUR[2] = { 1, City::XXX };
static constexpr short c_FIN[26] = { 25, City::ENF, City::ES0, City::HEL, City::IVL, City::JOE, City::JYV, City::KAJ, City::KAO, City::KEM, City::KHJ, City::KOK, City::KTT, City::KUO, City::LPP, City::MIK, City::OUL, City::POR, City::RVN, City::SJY, City::SOT, City::SVL, City::TKU, City::TMP, City::VAA, City::VRK };
static constexpr short c_FJI[4] = { 3, City::CST, City::NAN, City::SUV };
static constexpr short c_FLK[2] = { 1, City::PSY };
static constexpr short c_FRA[45] = { 44, City::AJA, City::AUR, City::BES, City::BIA, City::BIQ, City::BOD, City::CEQ, City::CFE, City::CLY, City::CMF, City::DNR, City::EBU, City::EGC, City::ENC, City::FNI, City::FRJ, City::FSC, City::GNB, City::LAI, City::LBI, City::LDE, City::LIG, City::LIL, City::LRH, City::LRT, City::LYS, City::MLH, City::MPL, City::MRS, City::MZM, City::NCE, City::NCY, City::NTE, City::PAR, City::PGF, City::PIS, City::PUF, City::RDZ, City::RNE, City::RNS, City::SBK, City::SXB, City::TLS, City::UIP };
static constexpr short c_FRO[2] = { 1, City::FAE };
static constexpr short c_FSM[2] = { 1, City::PNI };
static constexpr short c_GAB[8] = { 7, City::LBQ, City::LBV, City::MFF, City::MJL, City::MVB, City::POG, City::UVE };
static constexpr short c_GBR[48] = { 47, City::ABZ, City::AY0, City::BEB, City::BFS, City::BHX, City::BLK, City::BOH, City::BRR, City::BRS, City::CAL, City::CBG, City::CVT, City::CWL, City::DND, City::EDI, City::EMA, City::ESH, City::EXT, City::FIE, City::FOU, City::GLA, City::HUY, City::ILY, City::INV, City::KOI, City::LBA, City::LDY, City::LEQ, City::LON, City::LPL, City::LSI, City::LWK, City::LYX, City::MAN, City::MME, City::MSE, City::NCL, City::NQY, City::NWI, City::OUK, City::SEN, City::SOU, City::SWS, City::SYY, City::SZD, City::UNT, City::WIC };
static constexpr short c_GEO[4] = { 3, City::BUS, City::RS0, City::TBS };
static constexpr short c_GGY[3] = { 2, City::ACI, City::GCI };
static constexpr short c_GHA[4] = { 3, City::ACC, City::KMS, City::TML };
static constexpr short c_GIB[2] = { 1, City::GIB };
static constexpr short c_GIN[3] = { 2, City::CKY, City::LEK };
static constexpr short c_GLP[2] = { 1, City::BBR };
static constexpr short c_GMB[3] = { 2, City::BJL, City::SK0 };
static constexpr short c_GNB[3] = { 2, City::BSG, City::OXB };
static constexpr short c_GNQ[2] = { 1, City::SSG };
static constexpr short c_GRC[21] = { 20, City::AOK, City::ATH, City::CFU, City::CHQ, City::GPA, City::HER, City::JKH, City::JMK, City::JNX, City::JSI, City::JTR, City::KGS, City::KLX, City::KVA, City::MJT, City::PVK, City::RHO, City::SKG, City::SMI, City::ZTH };
static constexpr short c_GRD[2] = { 1, City::GND };
static constexpr short c_GRL[7] = { 6, City::GOH, City::JUV, City::SFJ, City::THU, City::UAK, City::UMD };
static constexpr short c_GTM[2] = { 1, City::GUA };
static constexpr short c_GUF[2] = { 1, City::CAY };
static constexpr short c_GUM[2] = { 1, City::GUM };
static constexpr short c_GUY[2] = { 1, City::GEO };
static constexpr short c_HKG[2] = { 1, City::HKG };
static constexpr short c_HMD[2] = { 1, City::CBR };
static constexpr short c_HND[7] = { 6, City::JUT, City::RTB, City::SAP, City::SDH, City::TGU, City::UII };
static constexpr short c_HRV[10] = { 9, City::DBV, City::LSZ, City::OSI, City::PUY, City::RJK, City::SPU, City::VZ0, City::ZAD, City::ZAG };
static constexpr short c_HTI[4] = { 3, City::JAK, City::JEE, City::PAP };
static constexpr short c_HUN[2] = { 1, City::BUD };
static constexpr short c_IDN[11] = { 10, City::AYW, City::BDO, City::DJB, City::DJJ, City::DPS, City::JKT, City::MDC, City::MES, City::SUB, City::UPG };
static constexpr short c_IMN[2] = { 1, City::IOM };
static constexpr short c_IND[48] = { 47, City::AMD, City::ATQ, City::BBI, City::BDQ, City::BHO, City::BLR, City::BOM, City::CCJ, City::CCU, City::CJB, City::COK, City::DEL, City::GAU, City::GC0, City::GH0, City::GOI, City::HYD, City::IDR, City::IXB, City::IXC, City::IXG, City::IXJ, City::IXR, City::IXW, City::JAI, City::JDH, City::JGA, City::JGB, City::JLR, City::JRH, City::JSA, City::KNU, City::LKO, City::MAA, City::MYQ, City::NAG, City::PAT, City::PNQ, City::PYB, City::QNB, City::RAJ, City::STV, City::SXR, City::TRV, City::TRZ, City::UDR, City::VNS };
static constexpr short c_IOT[2] = { 1, City::NKW };
static constexpr short c_IRL[9] = { 8, City::CFN, City::DUB, City::GWY, City::KIR, City::NOC, City::ORK, City::SNN, City::SXL };
static constexpr short c_IRN[4] = { 3, City::ABD, City::OMH, City::THR };
static constexpr short c_IRQ[5] = { 4, City::BGW, City::BSR, City::KIK, City::OSM };
static constexpr short c_ISL[3] = { 2, City::EGS, City::REK };
static constexpr short c_ISR[5] = { 4, City::ETH, City::HFA, City::JRS, City::TLV };
static constexpr short c_ITA[34] = { 33, City::AHO, City::AOI, City::BDS, City::BGY, City::BLQ, City::BRI, City::CAG, City::CTA, City::EBA, City::FLR, City::GOA, City::LMP, City::MIL, City::NAP, City::OLB, City::PEG, City::PMO, City::PNL, City::PSA, City::PSR, City::QIC, City::QPA, City::REG, City::RMI, City::ROM, City::SUF, City::TPS, City::TRN, City::TRS, City::TSF, City::VBS, City::VCE, City::VRN };
static constexpr short c_JAM[3] = { 2, City::KIN, City::MBJ };
static constexpr short c_JEY[2] = { 1, City::JER };
static constexpr short c_JOR[3] = { 2, City::AMM, City::AQJ };
static constexpr short c_JPN[41] = { 40, City::AOJ, City::ASJ, City::AXT, City::FKS, City::FUK, City::GAJ, City::HAC, City::HIJ, City::HKD, City::HNA, City::ISG, City::KCZ, City::KIJ, City::KMI, City::KMJ, City::KMQ, City::KOJ, City::KUH, City::MMJ, City::MMY, City::MYJ, City::NGO, City::NGS, City::OIT, City::OKA, City::OKD, City::OKJ, City::OSA, City::QCB, City::SDJ, City::SDS, City::SHS, City::TAK, City::TKS, City::TOY, City::TYO, City::UBJ, City::UKB, City::UKY, City::YOK };
static constexpr short c_KAZ[3] = { 2, City::ALA, City::TSE };
static constexpr short c_KEN[4] = { 3, City::MBA, City::MYD, City::NBO };
static constexpr short c_KGZ[2] = { 1, City::FRU };
static constexpr short c_KHM[4] = { 3, City::BBM, City::PNH, City::SAI };
static constexpr short c_KIR[3] = { 2, City::CXI, City::TRW };
static constexpr short c_KNA[3] = { 2, City::NEV, City::SKB };
static constexpr short c_KOR[5] = { 4, City::ICN, City::PUS, City::SEL, City::USN };
static constexpr short c_KWT[2] = { 1, City::KWI };
static constexpr short c_LAO[3] = { 2, City::VTE, City::ZVK };
static constexpr short c_LBN[2] = { 1, City::BEY };
static constexpr short c_LBR[2] = { 1, City::MLW };
static constexpr short c_LBY[4] = { 3, City::BEN, City::SEB, City::TIP };
static constexpr short c_LCA[3] = { 2, City::SLU, City::UVF };
static constexpr short c_LIE[2] = { 1, City::LI0 };
static constexpr short c_LKA[3] = { 2, City::CMB, City::JAF };
static constexpr short c_LSO[2] = { 1, City::MSU };
static constexpr short c_LTU[2] = { 1, City::VNO };
static constexpr short c_LUX[3] = { 2, City::EA0, City::LUX };
static constexpr short c_LVA[2] = { 1, City::RIX };
static constexpr short c_MAC[2] = { 1, City::MFM };
static constexpr short c_MAF[2] = { 1, City::SFG };
static constexpr short c_MAR[10] = { 9, City::AGA, City::AHU, City::CMN, City::FEZ, City::OUD, City::OZZ, City::RAK, City::RBA, City::TNG };
static constexpr short c_MCO[2] = { 1, City::MCM };
static constexpr short c_MDA[2] = { 1, City::KIV };
static constexpr short c_MDG[3] = { 2, City::MJN, City::TNR };
static constexpr short c_MDV[2] = { 1, City::MLE };
static constexpr short c_MEX[48] = { 47, City::ACA, City::AGU, City::BJX, City::CEN, City::CJS, City::CLQ, City::CME, City::CUL, City::CUN, City::CUU, City::CVM, City::CZA, City::CZM, City::EP0, City::GDL, City::HMO, City::HUX, City::JAL, City::LAP, City::LMM, City::LTO, City::LZC, City::MAM, City::MEX, City::MID, City::MLM, City::MTT, City::MTY, City::MXL, City::MZT, City::NLD, City::OAX, City::PBC, City::PVR, City::PXM, City::SJD, City::SLP, City::SRL, City::TAM, City::TGZ, City::TIJ, City::UPN, City::VER, City::VSA, City::ZCL, City::ZIH, City::ZLO };
static constexpr short c_MHL[3] = { 2, City::MAJ, City::UIT };
static constexpr short c_MKD[3] = { 2, City::OHD, City::SKP };
static constexpr short c_MLI[3] = { 2, City::BKO, City::KSS };
static constexpr short c_MLT[2] = { 1, City::MLA };
static constexpr short c_MMR[3] = { 2, City::MDL, City::RGN };
static constexpr short c_MNE[3] = { 2, City::TGD, City::TIV };
static constexpr short c_MNG[4] = { 3, City::COQ, City::ER0, City::ULN };
static constexpr short c_MNP[2] = { 1, City::SPN };
static constexpr short c_MOZ[3] = { 2, City::BEW, City::MPM };
static constexpr short c_MRT[4] = { 3, City::NDB, City::NKC, City::OUZ };
static constexpr short c_MSR[2] = { 1, City::MNI };
static constexpr short c_MTQ[2] = { 1, City::FDF };
static constexpr short c_MUS[4] = { 3, City::EB0, City::MRU, City::RRG };
static constexpr short c_MWI[3] = { 2, City::BLZ, City::LLW };
static constexpr short c_MYS[14] = { 13, City::BKI, City::BTU, City::JHB, City::KCH, City::KUA, City::KUL, City::LBU, City::LGK, City::MYY, City::PEN, City::SBW, City::TOD, City::TWU };
static constexpr short c_MYT[2] = { 1, City::DZA };
static constexpr short c_NAM[11] = { 10, City::KMP, City::LUD, City::MPA, City::NDU, City::OKU, City::OMD, City::OND, City::SWP, City::TSB, City::WDH };
static constexpr short c_NCL[7] = { 6, City::ILP, City::IOU, City::LIF, City::MEE, City::NOU, City::TOU };
static constexpr short c_NER[6] = { 5, City::AJY, City::MFQ, City::NIM, City::RLT, City::ZND };
static constexpr short c_NFK[2] = { 1, City::NLK };
static constexpr short c_NGA[6] = { 5, City::ABV, City::JOS, City::KAN, City::LOS, City::PHC };
static constexpr short c_NIC[2] = { 1, City::MGA };
static constexpr short c_NIU[2] = { 1, City::IUE };
static constexpr short c_NLD[11] = { 10, City::AMS, City::EIN, City::GRQ, City::HAG, City::LEY, City::MST, City::RTM, City::SH0, City::UDE, City::UTC };
static constexpr short c_NOR[20] = { 19, City::AES, City::ALF, City::BDU, City::BGO, City::BNN, City::BOO, City::EVE, City::FRO, City::HAU, City::HFT, City::KKN, City::KRS, City::KSU, City::LKL, City::OSL, City::SOG, City::SVG, City::TOS, City::TRD };
static constexpr short c_NPL[7] = { 6, City::BHR, City::JIR, City::JKR, City::JMO, City::KTM, City::PHH };
static constexpr short c_NRU[2] = { 1, City::INU };
static constexpr short c_NZL[18] = { 17, City::AKL, City::BHE, City::CHC, City::CHT, City::DUD, City::HLZ, City::IVC, City::MFN, City::MON, City::NSN, City::PMR, City::ROT, City::TEU, City::WHK, City::WLG, City::WRE, City::ZQN };
static constexpr short c_OMN[3] = { 2, City::MCT, City::SLL };
static constexpr short c_PAK[36] = { 35, City::BHV, City::BNP, City::CJL, City::DSK, City::GIL, City::GWD, City::HDD, City::ISB, City::JAG, City::JIW, City::KDD, City::KDU, City::KHI, City::LHE, City::LYP, City::MFG, City::MJD, City::MPD, City::MUX, City::MWD, City::OHT, City::PEW, City::PJG, City::PSI, City::PZH, City::QML, City::RAZ, City::RWP, City::RYK, City::SDT, City::SKZ, City::SUL, City::TUK, City::UET, City::WNS };
static constexpr short c_PAN[3] = { 2, City::JQE, City::PAC };
static constexpr short c_PCN[2] = { 1, City::PN0 };
static constexpr short c_PER[6] = { 5, City::IQT, City::JAU, City::JJI, City::JUL, City::LIM };
static constexpr short c_PHL[8] = { 7, City::CEB, City::CYU, City::JOL, City::LL0, City::MKC, City::MNL, City::PC0 };
static constexpr short c_PLW[2] = { 1, City::ROR };
static constexpr short c_PNG[6] = { 5, City::AZB, City::BUA, City::LAE, City::MFO, City::POM };
static constexpr short c_POL[7] = { 6, City::GDN, City::KRK, City::POZ, City::SZZ, City::WAW, City::WRO };
static constexpr short c_PRI[5] = { 4, City::BQN, City::MAZ, City::PSE, City::SJU };
static constexpr short c_PRK[2] = { 1, City::FNJ };
static constexpr short c_PRT[10] = { 9, City::FAO, City::FNC, City::HOR, City::LIS, City::OPO, City::PDL, City::PXO, City::SMA, City::TER };
static constexpr short c_PRY[2] = { 1, City::ASU };
static constexpr short c_PSE[3] = { 2, City::GZA, City::PS0 };
static constexpr short c_PYF[11] = { 10, City::BOB, City::HUH, City::MAU, City::MOZ, City::PPT, City::RFP, City::RGI, City::UAH, City::UAP, City::XMH };
static constexpr short c_QAT[2] = { 1, City::DOH };
static constexpr short c_REU[2] = { 1, City::RUN };
static constexpr short c_ROU[5] = { 4, City::BBU, City::CLJ, City::CND, City::SBZ };
static constexpr short c_RUS[27] = { 26, City::AER, City::ARH, City::ASF, City::GOJ, City::HTA, City::IKT, City::KGD, City::KHV, City::KUF, City::KZN, City::LED, City::MMK, City::MOW, City::MRV, City::OVB, City::ROV, City::SKX, City::SVX, City::UCT, City::UFA, City::ULY, City::UUD, City::VLU, City::VOG, City::VVO, City::YKS };
static constexpr short c_RWA[5] = { 4, City::BTQ, City::GYI, City::KGL, City::RHG };
static constexpr short c_SAU[11] = { 10, City::AHB, City::AJF, City::DHA, City::DMM, City::JED, City::MED, City::RUH, City::TIF, City::TUU, City::YNB };
static constexpr short c_SDN[3] = { 2, City::KRT, City::KSL };
static constexpr short c_SEN[3] = { 2, City::DKR, City::TBO };
static constexpr short c_SGP[2] = { 1, City::SIN };
static constexpr short c_SGS[2] = { 1, City::GS0 };
static constexpr short c_SHN[2] = { 1, City::ASI };
static constexpr short c_SJM[2] = { 1, City::LYR };
static constexpr short c_SLB[3] = { 2, City::GSI, City::HIR };
static constexpr short c_SLE[3] = { 2, City::FNA, City::KEN };
static constexpr short c_SLV[2] = { 1, City::SAL };
static constexpr short c_SMR[2] = { 1, City::SM0 };
static constexpr short c_SOM[3] = { 2, City::HGA, City::MGQ };
static constexpr short c_SPM[2] = { 1, City::FSP };
static constexpr short c_SRB[4] = { 3, City::BEG, City::INI, City::QND };
static constexpr short c_SSD[2] = { 1, City::JUB };
static constexpr short c_STP[2] = { 1, City::TMS };
static constexpr short c_SUR[2] = { 1, City::PBM };
static constexpr short c_SVK[3] = { 2, City::BTS, City::ILZ };
static constexpr short c_SVN[3] = { 2, City::LJU, City::MBX };
static constexpr short c_SWE[20] = { 19, City::GOT, City::JHE, City::JKG, City::KID, City::KLR, City::KRN, City::KSD, City::LDK, City::LLA, City::MMA, City::NRK, City::ORB, City::RNB, City::SDL, City::STO, City::UME, City::VBY, City::VST, City::VXO };
static constexpr short c_SWZ[3] = { 2, City::MTS, City::QMN };
static constexpr short c_SXM[2] = { 1, City::SXM };
static constexpr short c_SYC[2] = { 1, City::SEZ };
static constexpr short c_SYR[3] = { 2, City::ALP, City::DAM };
static constexpr short c_TCA[2] = { 1, City::GDT };
static constexpr short c_TCD[4] = { 3, City::AEH, City::MQQ, City::NDJ };
static constexpr short c_TGO[2] = { 1, City::LFW };
static constexpr short c_THA[10] = { 9, City::BKK, City::CNX, City::HDY, City::HKT, City::NST, City::PYX, City::UBP, City::UTH, City::UTP };
static constexpr short c_TJK[2] = { 1, City::DYU };
static constexpr short c_TKL[2] = { 1, City::TK0 };
static constexpr short c_TKM[2] = { 1, City::ASB };
static constexpr short c_TLS[2] = { 1, City::DIL };
static constexpr short c_TON[2] = { 1, City::TBU };
static constexpr short c_TTO[3] = { 2, City::POS, City::TAB };
static constexpr short c_TUN[5] = { 4, City::DJE, City::MIR, City::SFA, City::TUN };
static constexpr short c_TUR[25] = { 24, City::ADA, City::ADF, City::ANK, City::ASR, City::AYT, City::BJV, City::DLM, City::DNZ, City::ERC, City::ERZ, City::GZT, City::IST, City::IZM, City::KCM, City::KYA, City::KYS, City::MLX, City::MQM, City::MSR, City::SZF, City::TEQ, City::TZX, City::VAN, City::VAS };
static constexpr short c_TUV[2] = { 1, City::FUN };
static constexpr short c_TWN[4] = { 3, City::KHH, City::MZG, City::TAY };
static constexpr short c_TZA[5] = { 4, City::ARK, City::DAR, City::DOD, City::JRO };
static constexpr short c_UGA[5] = { 4, City::EBB, City::JIN, City::KLA, City::ULU };
static constexpr short c_UKR[10] = { 9, City::DNK, City::HRK, City::IEV, City::LWO, City::NLV, City::ODS, City::OZH, City::SIP, City::UDJ };
static constexpr short c_UMI[3] = { 2, City::JON, City::MDY };
static constexpr short c_URY[2] = { 1, City::MVD };
static constexpr short c_USA[467] = { 466, City::ABE, City::ABI, City::ABQ, City::ABR, City::ABY, City::ACK, City::ACT, City::ACV, City::ACY, City::ADK, City::ADQ, City::AGS, City::AHN, City::AKN, City::ALB, City::ALO, City::ALW, City::AMA, City::ANB, City::ANC, City::ANP, City::AOO, City::APF, City::ARB, City::ASE, City::ATL, City::ATO, City::ATW, City::ATY, City::AUG, City::AUS, City::AVL, City::AVP, City::AXS, City::AZO, City::BCT, City::BDL, City::BDR, City::BEH, City::BET, City::BFD, City::BFL, City::BGM, City::BGR, City::BHM, City::BIL, City::BIS, City::BJI, City::BKW, City::BKX, City::BL0, City::BLF, City::BLI, City::BMG, City::BMI, City::BNA, City::BOI, City::BOS, City::BPT, City::BQK, City::BRD, City::BRL, City::BTM, City::BTR, City::BTV, City::BUF, City::BUR, City::BWI, City::BXS, City::BZN, City::CAE, City::CAK, City::CCR, City::CDC, City::CDV, City::CEC, City::CGA, City::CHA, City::CHI, City::CHO, City::CHS, City::CIC, City::CID, City::CKB, City::CLD, City::CLE, City::CLL, City::CLM, City::CLT, City::CMH, City::CMI, City::CMX, City::COD, City::CON, City::COS, City::CPR, City::CRG, City::CRP, City::CRW, City::CSG, City::CSN, City::CT0, City::CVG, City::CWA, City::CYS, City::DAB, City::DAL, City::DAN, City::DAY, City::DBQ, City::DCA, City::DEC, City::DEN, City::DET, City::DHN, City::DLG, City::DLH, City::DOV, City::DRO, City::DSM, City::DUJ, City::DUT, City::DVL, City::EAT, City::EAU, City::EGE, City::EKI, City::EKO, City::ELM, City::ELP, City::ELY, City::ENA, City::ERI, City::ESC, City::ESF, City::EUG, City::EVV, City::EWN, City::EWR, City::EYW, City::FAI, City::FAR, City::FAT, City::FAY, City::FC0, City::FCA, City::FFT, City::FHU, City::FKL, City::FLG, City::FLL, City::FLO, City::FMN, City::FMY, City::FNT, City::FOD, City::FOE, City::FRI, City::FSD, City::FSM, City::FWA, City::FYV, City::GAD, City::GCC, City::GCN, City::GDV, City::GEG, City::GFK, City::GGG, City::GGW, City::GJT, City::GLH, City::GNV, City::GON, City::GPT, City::GPZ, City::GRB, City::GRR, City::GSO, City::GSP, City::GTF, City::GUC, City::GW0, City::HAR, City::HDN, City::HHH, City::HIB, City::HII, City::HKY, City::HLN, City::HNH, City::HNL, City::HNS, City::HOM, City::HON, City::HOU, City::HPN, City::HRL, City::HSV, City::HTS, City::HTV, City::HUF, City::HVN, City::HVR, City::HYA, City::HYG, City::IAG, City::ICT, City::IDA, City::IFP, City::IJX, City::ILE, City::ILG, City::ILI, City::ILM, City::IND, City::INL, City::IPL, City::IPT, City::ISL, City::ISO, City::ISP, City::ITH, City::ITO, City::IYK, City::JAC, City::JAN, City::JBR, City::JEF, City::JHM, City::JHW, City::JKV, City::JLN, City::JMS, City::JNU, City::JST, City::JVL, City::JXN, City::KCC, City::KLW, City::KOA, City::KTB, City::KTN, City::LAF, City::LAN, City::LAR, City::LAS, City::LAW, City::LAX, City::LBB, City::LBE, City::LCH, City::LEB, City::LEX, City::LFI, City::LFT, City::LGB, City::LIH, City::LIT, City::LMT, City::LNK, City::LNS, City::LNY, City::LRD, City::LRF, City::LSE, City::LWB, City::LWS, City::LWT, City::LYH, City::LYO, City::MAF, City::MBS, City::MC0, City::MCE, City::MCI, City::MCL, City::MCN, City::MCO, City::MCW, City::MEI, City::MEM, City::MFE, City::MFR, City::MGM, City::MGW, City::MHE, City::MHT, City::MIA, City::MJQ, City::MKE, City::MKG, City::MKK, City::MKL, City::MLB, City::MLI, City::MLS, City::MLU, City::MOB, City::MOD, City::MOT, City::MPV, City::MQT, City::MRB, City::MRY, City::MSL, City::MSN, City::MSO, City::MSP, City::MSY, City::MTH, City::MTJ, City::MTM, City::MTO, City::MUE, City::MV0, City::MVY, City::MWH, City::MYR, City::NS0, City::NYC, City::OAJ, City::OAK, City::OGG, City::OKC, City::OLF, City::OLM, City::OMA, City::OME, City::ONT, City::ORF, City::ORH, City::OSH, City::OTH, City::OTZ, City::OWB, City::OXR, City::PAH, City::PB0, City::PBI, City::PCT, City::PDT, City::PDX, City::PFN, City::PGA, City::PGV, City::PHF, City::PHL, City::PHX, City::PIA, City::PIB, City::PIH, City::PIR, City::PIT, City::PKB, City::PLB, City::PLN, City::PMD, City::PNS, City::POU, City::PQI, City::PSC, City::PSG, City::PSP, City::PUB, City::PUW, City::PVD, City::PWM, City::RAP, City::RDD, City::RDG, City::RDM, City::RDU, City::RFD, City::RHI, City::RIC, City::RKD, City::RKS, City::RNO, City::ROA, City::ROC, City::RST, City::RWI, City::SAF, City::SAN, City::SAT, City::SAV, City::SBA, City::SBN, City::SBP, City::SBY, City::SCC, City::SCE, City::SCF, City::SCK, City::SDF, City::SDY, City::SEA, City::SFO, City::SGF, City::SGU, City::SGY, City::SHD, City::SHR, City::SHV, City::SIT, City::SJC, City::SJT, City::SLC, City::SLE, City::SMF, City::SMX, City::SNA, City::SNS, City::SPI, City::SPS, City::SRQ, City::ST0, City::STL, City::STP, City::STS, City::SUN, City::SUX, City::SWF, City::SYR, City::TC0, City::TCL, City::TEX, City::TKA, City::TLH, City::TOL, City::TPA, City::TRI, City::TTN, City::TUL, City::TUP, City::TUS, City::TVC, City::TVF, City::TVL, City::TWF, City::TXK, City::TYR, City::TYS, City::UCA, City::UIN, City::UKI, City::UNK, City::UPP, City::VDZ, City::VEL, City::VIS, City::VLD, City::VN0, City::VPS, City::VRB, City::VY0, City::WLB, City::WN0, City::WP0, City::WRG, City::WRL, City::WYS, City::YAK, City::YKM, City::YUM };
static constexpr short c_UZB[5] = { 4, City::SKD, City::TAS, City::TMZ, City::UGC };
static constexpr short c_VAT[2] = { 1, City::VA0 };
static constexpr short c_VCT[4] = { 3, City::CIW, City::SVD, City::UNI };
static constexpr short c_VEN[9] = { 8, City::BLA, City::CCS, City::CGU, City::MAR, City::PMV, City::PZO, City::URM, City::VLN };
static constexpr short c_VGB[4] = { 3, City::EIS, City::TOV, City::VIJ };
static constexpr short c_VIR[3] = { 2, City::STT, City::STX };
static constexpr short c_VNM[4] = { 3, City::HAN, City::HUI, City::SGN };
static constexpr short c_VUT[4] = { 3, City::SON, City::ULB, City::VLI };
static constexpr short c_WLF[3] = { 2, City::FUT, City::WLS };
static constexpr short c_WSM[2] = { 1, City::APW };
static constexpr short c_XAF[2] = { 1, City::XXX };
static constexpr short c_XCD[2] = { 1, City::XXX };
static constexpr short c_XOF[2] = { 1, City::XXX };
static constexpr short c_XPF[2] = { 1, City::XXX };
static constexpr short c_XXX[3] = { 2, City::PRN, City::XXX };
static constexpr short c_YEM[3] = { 2, City::ADE, City::SAH };
static constexpr short c_ZAF[40] = { 39, City::ADY, City::AGZ, City::ALJ, City::BFN, City::CPT, City::DUR, City::ELL, City::ELS, City::GRJ, City::HLA, City::JNB, City::KIM, City::KLZ, City::LUJ, City::MBM, City::MEZ, City::MGH, City::MZY, City::NCS, City::NLP, City::NTY, City::OUH, City::PBZ, City::PHW, City::PLZ, City::PRY, City::PTG, City::PZB, City::RCB, City::SBU, City::SIS, City::SZK, City::TCU, City::ULD, City::UTN, City::UTT, City::VYD, City::WEL, City::WVB };
static constexpr short c_ZMB[6] = { 5, City::CIP, City::KIW, City::LUN, City::MFU, City::NLA };
static constexpr short c_ZWE[8] = { 7, City::BFO, City::BUQ, City::GWE, City::HRE, City::HWN, City::MVZ, City::VFA };

constexpr const short * Gazetteer::m_cid2ctys[Country::NUMCOUNTRY] = { 
    c_NOCOUNTRY,
    c_ABW, c_AFG, c_AGO, c_AIA, c_ALA, c_ALB, c_AND, c_ARE, c_ARG, c_ARM, 
    c_ASM, c_ATA, c_ATF, c_ATG, c_AUS, c_AUT, c_AZE, c_BDI, c_BEL, c_BEN, 
    c_BES, c_BFA, c_BGD, c_BGR, c_BHR, c_BHS, c_BIH, c_BLM, c_BLR, c_BLZ, 
    c_BMU, c_BOL, c_BRA, c_BRB, c_BRN, c_BTN, c_BVT, c_BWA, c_CAF, c_CAN, 
    c_CCK, c_CHE, c_CHL, c_CHN, c_CIV, c_CMR, c_COD, c_COG, c_COK, c_COL, 
    c_COM, c_CPV, c_CRI, c_CUB, c_CUW, c_CXR, c_CYM, c_CYP, c_CZE, c_DEU, 
    c_DJI, c_DMA, c_DNK, c_DOM, c_DZA, c_ECU, c_EGY, c_ERI, c_ESH, c_ESP, 
    c_EST, c_ETH, c_EUR, c_FIN, c_FJI, c_FLK, c_FRA, c_FRO, c_FSM, c_GAB, 
    c_GBR, c_GEO, c_GGY, c_GHA, c_GIB, c_GIN, c_GLP, c_GMB, c_GNB, c_GNQ, 
    c_GRC, c_GRD, c_GRL, c_GTM, c_GUF, c_GUM, c_GUY, c_HKG, c_HMD, c_HND, 
    c_HRV, c_HTI, c_HUN, c_IDN, c_IMN, c_IND, c_IOT, c_IRL, c_IRN, c_IRQ, 
    c_ISL, c_ISR, c_ITA, c_JAM, c_JEY, c_JOR, c_JPN, c_KAZ, c_KEN, c_KGZ, 
    c_KHM, c_KIR, c_KNA, c_KOR, c_KWT, c_LAO, c_LBN, c_LBR, c_LBY, c_LCA, 
    c_LIE, c_LKA, c_LSO, c_LTU, c_LUX, c_LVA, c_MAC, c_MAF, c_MAR, c_MCO, 
    c_MDA, c_MDG, c_MDV, c_MEX, c_MHL, c_MKD, c_MLI, c_MLT, c_MMR, c_MNE, 
    c_MNG, c_MNP, c_MOZ, c_MRT, c_MSR, c_MTQ, c_MUS, c_MWI, c_MYS, c_MYT, 
    c_NAM, c_NCL, c_NER, c_NFK, c_NGA, c_NIC, c_NIU, c_NLD, c_NOR, c_NPL, 
    c_NRU, c_NZL, c_OMN, c_PAK, c_PAN, c_PCN, c_PER, c_PHL, c_PLW, c_PNG, 
    c_POL, c_PRI, c_PRK, c_PRT, c_PRY, c_PSE, c_PYF, c_QAT, c_REU, c_ROU, 
    c_RUS, c_RWA, c_SAU, c_SDN, c_SEN, c_SGP, c_SGS, c_SHN, c_SJM, c_SLB, 
    c_SLE, c_SLV, c_SMR, c_SOM, c_SPM, c_SRB, c_SSD, c_STP, c_SUR, c_SVK, 
    c_SVN, c_SWE, c_SWZ, c_SXM, c_SYC, c_SYR, c_TCA, c_TCD, c_TGO, c_THA, 
    c_TJK, c_TKL, c_TKM, c_TLS, c_TON, c_TTO, c_TUN, c_TUR, c_TUV, c_TWN, 
    c_TZA, c_UGA, c_UKR, c_UMI, c_URY, c_USA, c_UZB, c_VAT, c_VCT, c_VEN, 
    c_VGB, c_VIR, c_VNM, c_VUT, c_WLF, c_WSM, c_XAF, c_XCD, c_XOF, c_XPF, 
    c_XXX, c_YEM, c_ZAF, c_ZMB, c_ZWE
};

static constexpr short d_NOCITY[2] = { 1, MarketId::NOMARKETID };
static constexpr short d_AAB[2] = { 1, MarketId::SBSI };
static constexpr short d_AAC[2] = { 1, MarketId::XXXX };
static constexpr short d_AAE[2] = { 1, MarketId::XXXX };
static constexpr short d_AAL[2] = { 1, MarketId::SNSI };
static constexpr short d_AAN[2] = { 1, MarketId::XXXX };
static constexpr short d_AAR[2] = { 1, MarketId::XXXX };
static constexpr short d_AAT[2] = { 1, MarketId::XXXX };
static constexpr short d_ABD[2] = { 1, MarketId::XXXX };
static constexpr short d_ABE[2] = { 1, MarketId::XXXX };
static constexpr short d_ABI[2] = { 1, MarketId::XXXX };
static constexpr short d_ABJ[4] = { 3, MarketId::CIV0, MarketId::XABJ, MarketId::XBRV };
static constexpr short d_ABM[2] = { 1, MarketId::XXXX };
static constexpr short d_ABQ[2] = { 1, MarketId::XXXX };
static constexpr short d_ABR[2] = { 1, MarketId::XXXX };
static constexpr short d_ABS[2] = { 1, MarketId::XXXX };
static constexpr short d_ABV[2] = { 1, MarketId::NGA0 };
static constexpr short d_ABX[2] = { 1, MarketId::XXXX };
static constexpr short d_ABY[2] = { 1, MarketId::XXXX };
static constexpr short d_ABZ[2] = { 1, MarketId::XXXX };
static constexpr short d_ACA[2] = { 1, MarketId::XXXX };
static constexpr short d_ACC[3] = { 2, MarketId::GHA0, MarketId::XGHA };
static constexpr short d_ACE[2] = { 1, MarketId::XXXX };
static constexpr short d_ACH[2] = { 1, MarketId::XXXX };
static constexpr short d_ACI[2] = { 1, MarketId::XXXX };
static constexpr short d_ACK[2] = { 1, MarketId::XXXX };
static constexpr short d_ACT[2] = { 1, MarketId::XXXX };
static constexpr short d_ACV[2] = { 1, MarketId::XXXX };
static constexpr short d_ACY[2] = { 1, MarketId::XXXX };
static constexpr short d_ADA[2] = { 1, MarketId::XXXX };
static constexpr short d_ADD[2] = { 1, MarketId::ETH0 };
static constexpr short d_ADE[2] = { 1, MarketId::XXXX };
static constexpr short d_ADF[2] = { 1, MarketId::XXXX };
static constexpr short d_ADK[2] = { 1, MarketId::XXXX };
static constexpr short d_ADL[2] = { 1, MarketId::XXXX };
static constexpr short d_ADQ[2] = { 1, MarketId::XXXX };
static constexpr short d_ADY[2] = { 1, MarketId::XXXX };
static constexpr short d_ADZ[2] = { 1, MarketId::XXXX };
static constexpr short d_AEH[2] = { 1, MarketId::XXXX };
static constexpr short d_AER[2] = { 1, MarketId::XXXX };
static constexpr short d_AES[2] = { 1, MarketId::XXXX };
static constexpr short d_AGA[2] = { 1, MarketId::XXXX };
static constexpr short d_AGB[2] = { 1, MarketId::XXXX };
static constexpr short d_AGP[2] = { 1, MarketId::XXXX };
static constexpr short d_AGS[2] = { 1, MarketId::XXXX };
static constexpr short d_AGU[2] = { 1, MarketId::XXXX };
static constexpr short d_AGZ[2] = { 1, MarketId::XXXX };
static constexpr short d_AHB[2] = { 1, MarketId::XXXX };
static constexpr short d_AHN[2] = { 1, MarketId::XXXX };
static constexpr short d_AHO[2] = { 1, MarketId::XXXX };
static constexpr short d_AHU[2] = { 1, MarketId::XXXX };
static constexpr short d_AJA[2] = { 1, MarketId::XXXX };
static constexpr short d_AJF[2] = { 1, MarketId::XXXX };
static constexpr short d_AJN[2] = { 1, MarketId::XXXX };
static constexpr short d_AJU[2] = { 1, MarketId::XXXX };
static constexpr short d_AJY[2] = { 1, MarketId::XXXX };
static constexpr short d_AKL[6] = { 5, MarketId::NZXC, MarketId::NZXM, MarketId::XAUK, MarketId::XNEE, MarketId::XNZE };
static constexpr short d_AKN[2] = { 1, MarketId::XXXX };
static constexpr short d_AKT[2] = { 1, MarketId::XXXX };
static constexpr short d_ALA[3] = { 2, MarketId::ETSC, MarketId::XKAZ };
static constexpr short d_ALB[2] = { 1, MarketId::XXXX };
static constexpr short d_ALC[2] = { 1, MarketId::XXXX };
static constexpr short d_ALF[2] = { 1, MarketId::XXXX };
static constexpr short d_ALG[3] = { 2, MarketId::DZA0, MarketId::XALG };
static constexpr short d_ALH[2] = { 1, MarketId::XXXX };
static constexpr short d_ALJ[2] = { 1, MarketId::XXXX };
static constexpr short d_ALO[2] = { 1, MarketId::XXXX };
static constexpr short d_ALP[2] = { 1, MarketId::XXXX };
static constexpr short d_ALV[2] = { 1, MarketId::AND0 };
static constexpr short d_ALW[2] = { 1, MarketId::XXXX };
static constexpr short d_ALY[2] = { 1, MarketId::XXXX };
static constexpr short d_AMA[2] = { 1, MarketId::XXXX };
static constexpr short d_AMD[2] = { 1, MarketId::NMCE };
static constexpr short d_AMM[4] = { 3, MarketId::AMNL, MarketId::JOR0, MarketId::XAMM };
static constexpr short d_AMS[107] = { 106, MarketId::ABNA, MarketId::ABNC, MarketId::AFSA, MarketId::AFSE, MarketId::AFSI, MarketId::AFSL, MarketId::AFSO, MarketId::AFSX, MarketId::ALXA, MarketId::BAPE, MarketId::BARU, MarketId::BEUD, MarketId::BEUE, MarketId::BEUF, MarketId::BEUO, MarketId::BEUP, MarketId::BEUT, MarketId::BTAM, MarketId::BTFE, MarketId::BTQE, MarketId::CABV, MarketId::CAPA, MarketId::CAVD, MarketId::CBAE, MarketId::CCRM, MarketId::CCXE, MarketId::CEDX, MarketId::CEUD, MarketId::CEUE, MarketId::CEUO, MarketId::CEUX, MarketId::COMM, MarketId::CPTX, MarketId::D2XC, MarketId::D2XG, MarketId::DAMS, MarketId::EBSD, MarketId::EBSF, MarketId::EBSI, MarketId::EBSN, MarketId::ECEU, MarketId::ECXE, MarketId::ETPA, MarketId::FLTB, MarketId::FLTR, MarketId::FLWX, MarketId::GMGE, MarketId::HCHC, MarketId::IEBS, MarketId::IMCO, MarketId::IMCT, MarketId::IMEQ, MarketId::INGB, MarketId::INGE, MarketId::INGF, MarketId::IPNL, MarketId::ISWN, MarketId::ISWO, MarketId::ISWP, MarketId::ISWT, MarketId::JLEU, MarketId::JNSI, MarketId::LEBV, MarketId::LISZ, MarketId::MANL, MarketId::MHBE, MarketId::MUBE, MarketId::MUSN, MarketId::MXNL, MarketId::NDEX, MarketId::NDXS, MarketId::NEXY, MarketId::NLD0, MarketId::NLPX, MarketId::NWNV, MarketId::OCXE, MarketId::OHVO, MarketId::RESE, MarketId::RESF, MarketId::RFQN, MarketId::RR4G, MarketId::STXS, MarketId::TNLA, MarketId::TOMD, MarketId::TOMX, MarketId::TOWR, MarketId::TQEA, MarketId::TQEB, MarketId::TQEM, MarketId::TQEX, MarketId::TRNL, MarketId::TWEA, MarketId::TWEM, MarketId::TWEO, MarketId::TWEU, MarketId::VWAP, MarketId::XACE, MarketId::XAEX, MarketId::XAMS, MarketId::XEUC, MarketId::XEUE, MarketId::XEUI, MarketId::XFTA, MarketId::XHFT, MarketId::XNXC, MarketId::XNXD };
static constexpr short d_ANB[2] = { 1, MarketId::XXXX };
static constexpr short d_ANC[2] = { 1, MarketId::XXXX };
static constexpr short d_ANK[4] = { 3, MarketId::EWRM, MarketId::TMEX, MarketId::TUR0 };
static constexpr short d_ANP[2] = { 1, MarketId::XXXX };
static constexpr short d_ANR[2] = { 1, MarketId::XANT };
static constexpr short d_ANU[3] = { 2, MarketId::ATG0, MarketId::XFTX };
static constexpr short d_AOI[2] = { 1, MarketId::XXXX };
static constexpr short d_AOJ[2] = { 1, MarketId::XXXX };
static constexpr short d_AOK[2] = { 1, MarketId::XXXX };
static constexpr short d_AOO[2] = { 1, MarketId::XXXX };
static constexpr short d_APF[2] = { 1, MarketId::XXXX };
static constexpr short d_APW[2] = { 1, MarketId::WSM0 };
static constexpr short d_AQJ[2] = { 1, MarketId::XXXX };
static constexpr short d_ARB[2] = { 1, MarketId::XXXX };
static constexpr short d_ARH[2] = { 1, MarketId::XXXX };
static constexpr short d_ARK[2] = { 1, MarketId::XXXX };
static constexpr short d_ASB[2] = { 1, MarketId::TKM0 };
static constexpr short d_ASE[2] = { 1, MarketId::XXXX };
static constexpr short d_ASF[2] = { 1, MarketId::XXXX };
static constexpr short d_ASI[2] = { 1, MarketId::SHN0 };
static constexpr short d_ASJ[2] = { 1, MarketId::XXXX };
static constexpr short d_ASK[2] = { 1, MarketId::XXXX };
static constexpr short d_ASM[2] = { 1, MarketId::ERI0 };
static constexpr short d_ASP[2] = { 1, MarketId::XXXX };
static constexpr short d_ASR[2] = { 1, MarketId::XXXX };
static constexpr short d_ASU[3] = { 2, MarketId::PRY0, MarketId::XVPA };
static constexpr short d_ASW[2] = { 1, MarketId::XXXX };
static constexpr short d_ATH[18] = { 17, MarketId::AAPA, MarketId::ABFI, MarketId::ASEX, MarketId::ENAX, MarketId::ERBX, MarketId::EUAX, MarketId::GRC0, MarketId::HDAT, MarketId::HEDE, MarketId::HEMO, MarketId::HESP, MarketId::HGSP, MarketId::HOTC, MarketId::PBGR, MarketId::XADE, MarketId::XATH, MarketId::XIPO };
static constexpr short d_ATL[10] = { 9, MarketId::BKKT, MarketId::IEPA, MarketId::IMAG, MarketId::IMBD, MarketId::IMCR, MarketId::IMEN, MarketId::IMFX, MarketId::IMIR, MarketId::UBEC };
static constexpr short d_ATO[2] = { 1, MarketId::XXXX };
static constexpr short d_ATQ[2] = { 1, MarketId::XXXX };
static constexpr short d_ATW[2] = { 1, MarketId::XXXX };
static constexpr short d_ATY[2] = { 1, MarketId::XXXX };
static constexpr short d_ATZ[2] = { 1, MarketId::XXXX };
static constexpr short d_AUA[2] = { 1, MarketId::ABW0 };
static constexpr short d_AUE[2] = { 1, MarketId::XXXX };
static constexpr short d_AUG[2] = { 1, MarketId::XXXX };
static constexpr short d_AUH[14] = { 13, MarketId::ABXX, MarketId::ACXC, MarketId::ACXL, MarketId::ARE0, MarketId::DEXE, MarketId::GLOM, MarketId::IFAD, MarketId::KRME, MarketId::M2AE, MarketId::MATX, MarketId::MIDC, MarketId::VMEX, MarketId::XADS };
static constexpr short d_AUR[2] = { 1, MarketId::XXXX };
static constexpr short d_AUS[2] = { 1, MarketId::XXXX };
static constexpr short d_AUX[2] = { 1, MarketId::XXXX };
static constexpr short d_AVL[2] = { 1, MarketId::XXXX };
static constexpr short d_AVP[2] = { 1, MarketId::XXXX };
static constexpr short d_AXA[2] = { 1, MarketId::AIA0 };
static constexpr short d_AXS[2] = { 1, MarketId::XXXX };
static constexpr short d_AXT[2] = { 1, MarketId::XXXX };
static constexpr short d_AY0[2] = { 1, MarketId::SHAR };
static constexpr short d_AYQ[2] = { 1, MarketId::XXXX };
static constexpr short d_AYR[2] = { 1, MarketId::XXXX };
static constexpr short d_AYT[2] = { 1, MarketId::XXXX };
static constexpr short d_AYW[2] = { 1, MarketId::XXXX };
static constexpr short d_AZB[2] = { 1, MarketId::XXXX };
static constexpr short d_AZO[2] = { 1, MarketId::XXXX };
static constexpr short d_AZS[2] = { 1, MarketId::XXXX };
static constexpr short d_BAH[4] = { 3, MarketId::BFEX, MarketId::BHR0, MarketId::XBAH };
static constexpr short d_BAK[4] = { 3, MarketId::AZE0, MarketId::BSEX, MarketId::XIBE };
static constexpr short d_BAQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BBI[2] = { 1, MarketId::XXXX };
static constexpr short d_BBM[2] = { 1, MarketId::XXXX };
static constexpr short d_BBR[2] = { 1, MarketId::GLP0 };
static constexpr short d_BBT[2] = { 1, MarketId::XXXX };
static constexpr short d_BBU[17] = { 16, MarketId::BRDE, MarketId::BRDL, MarketId::BRDS, MarketId::BRMF, MarketId::EMCE, MarketId::OTPR, MarketId::ROU0, MarketId::RRSI, MarketId::XBRM, MarketId::XBSD, MarketId::XBSE, MarketId::XCAN, MarketId::XMTI, MarketId::XNRG, MarketId::XRAS, MarketId::XRPM };
static constexpr short d_BBY[2] = { 1, MarketId::XXXX };
static constexpr short d_BCN[7] = { 6, MarketId::CABK, MarketId::PAVE, MarketId::SBAR, MarketId::XBAR, MarketId::XBAV, MarketId::XMEF };
static constexpr short d_BCO[2] = { 1, MarketId::XXXX };
static constexpr short d_BCT[2] = { 1, MarketId::LAMP };
static constexpr short d_BCV[2] = { 1, MarketId::BLZ0 };
static constexpr short d_BDA[5] = { 4, MarketId::_24EX, MarketId::BMU0, MarketId::GTXE, MarketId::XBDA };
static constexpr short d_BDB[2] = { 1, MarketId::XXXX };
static constexpr short d_BDL[2] = { 1, MarketId::XXXX };
static constexpr short d_BDO[2] = { 1, MarketId::XXXX };
static constexpr short d_BDQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BDR[2] = { 1, MarketId::XXXX };
static constexpr short d_BDS[2] = { 1, MarketId::XXXX };
static constexpr short d_BDU[2] = { 1, MarketId::XXXX };
static constexpr short d_BEB[2] = { 1, MarketId::XXXX };
static constexpr short d_BEG[3] = { 2, MarketId::SRB0, MarketId::XBEL };
static constexpr short d_BEH[2] = { 1, MarketId::XXXX };
static constexpr short d_BEL[2] = { 1, MarketId::XXXX };
static constexpr short d_BEN[2] = { 1, MarketId::XXXX };
static constexpr short d_BER[17] = { 16, MarketId::BERA, MarketId::BERB, MarketId::BERC, MarketId::DEU0, MarketId::EQTA, MarketId::EQTB, MarketId::EQTC, MarketId::EQTD, MarketId::TGAT, MarketId::TGSI, MarketId::TRBX, MarketId::XBER, MarketId::XEQT, MarketId::XGAT, MarketId::XGRM, MarketId::ZOBX };
static constexpr short d_BES[2] = { 1, MarketId::XXXX };
static constexpr short d_BET[2] = { 1, MarketId::XXXX };
static constexpr short d_BEW[2] = { 1, MarketId::XXXX };
static constexpr short d_BEY[3] = { 2, MarketId::LBN0, MarketId::XBEY };
static constexpr short d_BFD[2] = { 1, MarketId::XXXX };
static constexpr short d_BFL[2] = { 1, MarketId::XXXX };
static constexpr short d_BFN[2] = { 1, MarketId::XXXX };
static constexpr short d_BFO[2] = { 1, MarketId::XXXX };
static constexpr short d_BFS[2] = { 1, MarketId::XXXX };
static constexpr short d_BGA[2] = { 1, MarketId::XXXX };
static constexpr short d_BGF[2] = { 1, MarketId::CAF0 };
static constexpr short d_BGI[5] = { 4, MarketId::BAJM, MarketId::BRB0, MarketId::XBAB, MarketId::XBIS };
static constexpr short d_BGM[2] = { 1, MarketId::XXXX };
static constexpr short d_BGO[4] = { 3, MarketId::FISH, MarketId::HWHE, MarketId::ICAS };
static constexpr short d_BGR[2] = { 1, MarketId::XXXX };
static constexpr short d_BGU[2] = { 1, MarketId::XXXX };
static constexpr short d_BGW[3] = { 2, MarketId::IRQ0, MarketId::XIQS };
static constexpr short d_BGY[2] = { 1, MarketId::UBIM };
static constexpr short d_BHE[2] = { 1, MarketId::XXXX };
static constexpr short d_BHM[2] = { 1, MarketId::XXXX };
static constexpr short d_BHO[2] = { 1, MarketId::XXXX };
static constexpr short d_BHQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BHR[2] = { 1, MarketId::XXXX };
static constexpr short d_BHV[2] = { 1, MarketId::XXXX };
static constexpr short d_BHX[2] = { 1, MarketId::XXXX };
static constexpr short d_BIA[2] = { 1, MarketId::XXXX };
static constexpr short d_BIL[2] = { 1, MarketId::XXXX };
static constexpr short d_BIO[4] = { 3, MarketId::IBGH, MarketId::SBIL, MarketId::XBIL };
static constexpr short d_BIQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BIS[2] = { 1, MarketId::XXXX };
static constexpr short d_BIV[2] = { 1, MarketId::XXXX };
static constexpr short d_BJI[2] = { 1, MarketId::XXXX };
static constexpr short d_BJL[2] = { 1, MarketId::GMB0 };
static constexpr short d_BJM[2] = { 1, MarketId::BDI0 };
static constexpr short d_BJS[5] = { 4, MarketId::BJSE, MarketId::CHN0, MarketId::GSXC, MarketId::NEEQ };
static constexpr short d_BJV[2] = { 1, MarketId::XXXX };
static constexpr short d_BJX[2] = { 1, MarketId::XXXX };
static constexpr short d_BJZ[2] = { 1, MarketId::XXXX };
static constexpr short d_BKI[2] = { 1, MarketId::XXXX };
static constexpr short d_BKK[8] = { 7, MarketId::AFET, MarketId::BEEX, MarketId::TFEX, MarketId::THA0, MarketId::XBKF, MarketId::XBKK, MarketId::XMAI };
static constexpr short d_BKO[2] = { 1, MarketId::MLI0 };
static constexpr short d_BKW[2] = { 1, MarketId::XXXX };
static constexpr short d_BKX[2] = { 1, MarketId::XXXX };
static constexpr short d_BL0[2] = { 1, MarketId::XXXX };
static constexpr short d_BLA[2] = { 1, MarketId::XXXX };
static constexpr short d_BLF[2] = { 1, MarketId::XXXX };
static constexpr short d_BLI[2] = { 1, MarketId::XXXX };
static constexpr short d_BLK[2] = { 1, MarketId::XXXX };
static constexpr short d_BLL[2] = { 1, MarketId::XXXX };
static constexpr short d_BLQ[3] = { 2, MarketId::CREM, MarketId::UBIS };
static constexpr short d_BLR[2] = { 1, MarketId::XBAN };
static constexpr short d_BLT[2] = { 1, MarketId::XXXX };
static constexpr short d_BLZ[2] = { 1, MarketId::XMSW };
static constexpr short d_BME[2] = { 1, MarketId::XXXX };
static constexpr short d_BMG[2] = { 1, MarketId::XXXX };
static constexpr short d_BMI[2] = { 1, MarketId::XXXX };
static constexpr short d_BMP[2] = { 1, MarketId::XXXX };
static constexpr short d_BNA[2] = { 1, MarketId::XXXX };
static constexpr short d_BNE[2] = { 1, MarketId::XXXX };
static constexpr short d_BNJ[2] = { 1, MarketId::XXXX };
static constexpr short d_BNK[2] = { 1, MarketId::XXXX };
static constexpr short d_BNN[2] = { 1, MarketId::XXXX };
static constexpr short d_BNP[2] = { 1, MarketId::XXXX };
static constexpr short d_BNX[3] = { 2, MarketId::BLBF, MarketId::XBLB };
static constexpr short d_BOB[2] = { 1, MarketId::XXXX };
static constexpr short d_BOD[2] = { 1, MarketId::XXXX };
static constexpr short d_BOG[3] = { 2, MarketId::COL0, MarketId::XBOG };
static constexpr short d_BOH[2] = { 1, MarketId::XXXX };
static constexpr short d_BOI[2] = { 1, MarketId::XXXX };
static constexpr short d_BOJ[2] = { 1, MarketId::XXXX };
static constexpr short d_BOM[18] = { 17, MarketId::ACEX, MarketId::ASTR, MarketId::BSME, MarketId::CDSL, MarketId::FXCL, MarketId::FXSW, MarketId::ICXL, MarketId::ISEX, MarketId::MCXX, MarketId::OTCX, MarketId::PXIL, MarketId::RITS, MarketId::XBOM, MarketId::XIMC, MarketId::XNCD, MarketId::XNSE, MarketId::XUSE };
static constexpr short d_BON[2] = { 1, MarketId::BES0 };
static constexpr short d_BOO[2] = { 1, MarketId::XXXX };
static constexpr short d_BOS[15] = { 14, MarketId::BSTX, MarketId::EBXV, MarketId::GOVX, MarketId::HPPO, MarketId::LEVL, MarketId::LMNX, MarketId::LTAA, MarketId::NFSA, MarketId::NFSC, MarketId::NFSD, MarketId::SIGX, MarketId::SSTX, MarketId::XBOX, MarketId::XSTM };
static constexpr short d_BOY[2] = { 1, MarketId::XXXX };
static constexpr short d_BPT[2] = { 1, MarketId::XXXX };
static constexpr short d_BQK[2] = { 1, MarketId::XXXX };
static constexpr short d_BQN[2] = { 1, MarketId::XXXX };
static constexpr short d_BRC[2] = { 1, MarketId::XXXX };
static constexpr short d_BRD[2] = { 1, MarketId::XXXX };
static constexpr short d_BRE[2] = { 1, MarketId::XBRE };
static constexpr short d_BRI[2] = { 1, MarketId::XXXX };
static constexpr short d_BRL[2] = { 1, MarketId::XXXX };
static constexpr short d_BRN[6] = { 5, MarketId::AIXE, MarketId::CHE0, MarketId::EQWB, MarketId::OTXB, MarketId::XBRN };
static constexpr short d_BRQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BRR[2] = { 1, MarketId::XXXX };
static constexpr short d_BRS[2] = { 1, MarketId::XXXX };
static constexpr short d_BRU[25] = { 24, MarketId::ALXB, MarketId::BEAM, MarketId::BEL0, MarketId::BELB, MarketId::BELF, MarketId::BKBF, MarketId::BKBR, MarketId::BLPX, MarketId::BMTS, MarketId::BNPF, MarketId::DBRU, MarketId::ENXB, MarketId::FRRF, MarketId::KBCB, MarketId::MLXB, MarketId::MTSD, MarketId::MTSF, MarketId::SMBB, MarketId::TNLB, MarketId::TNLK, MarketId::VPXB, MarketId::XBFO, MarketId::XBRD, MarketId::XBRU };
static constexpr short d_BSB[2] = { 1, MarketId::BRA0 };
static constexpr short d_BSG[2] = { 1, MarketId::XXXX };
static constexpr short d_BSL[2] = { 1, MarketId::XXXX };
static constexpr short d_BSR[2] = { 1, MarketId::XXXX };
static constexpr short d_BTM[2] = { 1, MarketId::XXXX };
static constexpr short d_BTQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BTR[2] = { 1, MarketId::XXXX };
static constexpr short d_BTS[7] = { 6, MarketId::CBSK, MarketId::EBRA, MarketId::SVK0, MarketId::VUBA, MarketId::XBRA, MarketId::XRMS };
static constexpr short d_BTU[2] = { 1, MarketId::XXXX };
static constexpr short d_BTV[2] = { 1, MarketId::XXXX };
static constexpr short d_BUA[2] = { 1, MarketId::XXXX };
static constexpr short d_BUD[22] = { 21, MarketId::BETA, MarketId::CIBH, MarketId::CONC, MarketId::EBHU, MarketId::ERST, MarketId::HUDX, MarketId::HUN0, MarketId::HUPX, MarketId::KCCP, MarketId::KELR, MarketId::KHHU, MarketId::OTPB, MarketId::QMTF, MarketId::RBHU, MarketId::UCHU, MarketId::XBCE, MarketId::XBND, MarketId::XBUD, MarketId::XGAS, MarketId::XQLX, MarketId::XTND };
static constexpr short d_BUE[8] = { 7, MarketId::ARG0, MarketId::BACE, MarketId::XA1X, MarketId::XBUE, MarketId::XMAB, MarketId::XMEV, MarketId::XMTB };
static constexpr short d_BUF[2] = { 1, MarketId::XXXX };
static constexpr short d_BUG[2] = { 1, MarketId::XXXX };
static constexpr short d_BUQ[2] = { 1, MarketId::XXXX };
static constexpr short d_BUR[2] = { 1, MarketId::XXXX };
static constexpr short d_BUS[2] = { 1, MarketId::XXXX };
static constexpr short d_BVB[2] = { 1, MarketId::XXXX };
static constexpr short d_BWI[2] = { 1, MarketId::XXXX };
static constexpr short d_BWN[2] = { 1, MarketId::BRN0 };
static constexpr short d_BWT[2] = { 1, MarketId::XXXX };
static constexpr short d_BXS[2] = { 1, MarketId::XXXX };
static constexpr short d_BYK[2] = { 1, MarketId::XXXX };
static constexpr short d_BYU[2] = { 1, MarketId::XXXX };
static constexpr short d_BZE[2] = { 1, MarketId::XXXX };
static constexpr short d_BZL[2] = { 1, MarketId::XXXX };
static constexpr short d_BZN[2] = { 1, MarketId::XXXX };
static constexpr short d_BZV[2] = { 1, MarketId::COG0 };
static constexpr short d_CAB[2] = { 1, MarketId::XXXX };
static constexpr short d_CAE[2] = { 1, MarketId::XXXX };
static constexpr short d_CAG[2] = { 1, MarketId::XXXX };
static constexpr short d_CAI[4] = { 3, MarketId::EGY0, MarketId::NILX, MarketId::XCAI };
static constexpr short d_CAK[2] = { 1, MarketId::XXXX };
static constexpr short d_CAL[2] = { 1, MarketId::XXXX };
static constexpr short d_CAN[2] = { 1, MarketId::XXXX };
static constexpr short d_CAY[2] = { 1, MarketId::GUF0 };
static constexpr short d_CBB[2] = { 1, MarketId::XXXX };
static constexpr short d_CBG[2] = { 1, MarketId::XXXX };
static constexpr short d_CBR[2] = { 1, MarketId::AUS0 };
static constexpr short d_CBU[2] = { 1, MarketId::XXXX };
static constexpr short d_CCJ[2] = { 1, MarketId::XXXX };
static constexpr short d_CCK[2] = { 1, MarketId::CCK0 };
static constexpr short d_CCR[2] = { 1, MarketId::XXXX };
static constexpr short d_CCS[4] = { 3, MarketId::BVCA, MarketId::VEN0, MarketId::XCAR };
static constexpr short d_CCU[2] = { 1, MarketId::XCAL };
static constexpr short d_CCZ[2] = { 1, MarketId::XXXX };
static constexpr short d_CDC[2] = { 1, MarketId::XXXX };
static constexpr short d_CDV[2] = { 1, MarketId::XXXX };
static constexpr short d_CEB[2] = { 1, MarketId::XXXX };
static constexpr short d_CEC[2] = { 1, MarketId::XXXX };
static constexpr short d_CED[2] = { 1, MarketId::XXXX };
static constexpr short d_CEN[2] = { 1, MarketId::XXXX };
static constexpr short d_CEQ[2] = { 1, MarketId::XXXX };
static constexpr short d_CES[2] = { 1, MarketId::XXXX };
static constexpr short d_CFE[2] = { 1, MarketId::XXXX };
static constexpr short d_CFG[2] = { 1, MarketId::XXXX };
static constexpr short d_CFN[2] = { 1, MarketId::XXXX };
static constexpr short d_CFS[2] = { 1, MarketId::XXXX };
static constexpr short d_CFU[2] = { 1, MarketId::XXXX };
static constexpr short d_CGA[2] = { 1, MarketId::XXXX };
static constexpr short d_CGB[2] = { 1, MarketId::XXXX };
static constexpr short d_CGN[2] = { 1, MarketId::XXXX };
static constexpr short d_CGO[2] = { 1, MarketId::XZCE };
static constexpr short d_CGP[2] = { 1, MarketId::XCHG };
static constexpr short d_CGQ[2] = { 1, MarketId::XXXX };
static constexpr short d_CGR[2] = { 1, MarketId::XXXX };
static constexpr short d_CGU[2] = { 1, MarketId::XXXX };
static constexpr short d_CHA[2] = { 1, MarketId::XXXX };
static constexpr short d_CHC[2] = { 1, MarketId::XXXX };
static constexpr short d_CHG[2] = { 1, MarketId::XXXX };
static constexpr short d_CHI[65] = { 64, MarketId::BATO, MarketId::BATS, MarketId::BATY, MarketId::BTNL, MarketId::BYXD, MarketId::BZXD, MarketId::C2OX, MarketId::CAST, MarketId::CBSX, MarketId::CBTS, MarketId::CCFE, MarketId::CMES, MarketId::CODA, MarketId::COHR, MarketId::CONE, MarketId::CTWO, MarketId::DASH, MarketId::EDDP, MarketId::EDGA, MarketId::EDGD, MarketId::EDGO, MarketId::EDGX, MarketId::ERIS, MarketId::FCBT, MarketId::FCME, MarketId::FREX, MarketId::G1XX, MarketId::GLPS, MarketId::GOTC, MarketId::GREE, MarketId::HEGX, MarketId::IMCS, MarketId::JLEQ, MarketId::JLQD, MarketId::OPRA, MarketId::PDQD, MarketId::PDQX, MarketId::SCXA, MarketId::SCXF, MarketId::SCXM, MarketId::SCXO, MarketId::SCXS, MarketId::SMFE, MarketId::SUNT, MarketId::THRD, MarketId::XARC, MarketId::XCBD, MarketId::XCBF, MarketId::XCBO, MarketId::XCBT, MarketId::XCCX, MarketId::XCHI, MarketId::XCME, MarketId::XCRC, MarketId::XEUS, MarketId::XIMM, MarketId::XIMX, MarketId::XIOM, MarketId::XMAC, MarketId::XMER, MarketId::XMID, MarketId::XOCH, MarketId::YKNA, MarketId::ZERO };
static constexpr short d_CHO[3] = { 2, MarketId::CMSF, MarketId::WELX };
static constexpr short d_CHQ[2] = { 1, MarketId::XXXX };
static constexpr short d_CHS[2] = { 1, MarketId::XXXX };
static constexpr short d_CHT[2] = { 1, MarketId::XXXX };
static constexpr short d_CIC[2] = { 1, MarketId::XXXX };
static constexpr short d_CID[2] = { 1, MarketId::XXXX };
static constexpr short d_CIP[2] = { 1, MarketId::XXXX };
static constexpr short d_CIW[2] = { 1, MarketId::XXXX };
static constexpr short d_CJB[2] = { 1, MarketId::XXXX };
static constexpr short d_CJC[2] = { 1, MarketId::XXXX };
static constexpr short d_CJL[2] = { 1, MarketId::XXXX };
static constexpr short d_CJS[2] = { 1, MarketId::XXXX };
static constexpr short d_CKB[2] = { 1, MarketId::XXXX };
static constexpr short d_CKG[2] = { 1, MarketId::XXXX };
static constexpr short d_CKY[2] = { 1, MarketId::GIN0 };
static constexpr short d_CLD[2] = { 1, MarketId::XXXX };
static constexpr short d_CLE[2] = { 1, MarketId::XXXX };
static constexpr short d_CLJ[2] = { 1, MarketId::BTRL };
static constexpr short d_CLL[2] = { 1, MarketId::XXXX };
static constexpr short d_CLM[2] = { 1, MarketId::XXXX };
static constexpr short d_CLO[2] = { 1, MarketId::XXXX };
static constexpr short d_CLQ[2] = { 1, MarketId::XXXX };
static constexpr short d_CLT[2] = { 1, MarketId::XXXX };
static constexpr short d_CLY[2] = { 1, MarketId::XXXX };
static constexpr short d_CMB[3] = { 2, MarketId::LKA0, MarketId::XCOL };
static constexpr short d_CME[2] = { 1, MarketId::XXXX };
static constexpr short d_CMF[2] = { 1, MarketId::XXXX };
static constexpr short d_CMH[2] = { 1, MarketId::XXXX };
static constexpr short d_CMI[2] = { 1, MarketId::XXXX };
static constexpr short d_CMN[2] = { 1, MarketId::XCAS };
static constexpr short d_CMQ[2] = { 1, MarketId::XXXX };
static constexpr short d_CMX[2] = { 1, MarketId::XXXX };
static constexpr short d_CND[2] = { 1, MarketId::XXXX };
static constexpr short d_CNF[2] = { 1, MarketId::XXXX };
static constexpr short d_CNQ[2] = { 1, MarketId::XCNF };
static constexpr short d_CNS[2] = { 1, MarketId::XXXX };
static constexpr short d_CNX[2] = { 1, MarketId::XXXX };
static constexpr short d_COD[2] = { 1, MarketId::XXXX };
static constexpr short d_COK[2] = { 1, MarketId::XXXX };
static constexpr short d_CON[2] = { 1, MarketId::XXXX };
static constexpr short d_COO[2] = { 1, MarketId::XXXX };
static constexpr short d_COQ[2] = { 1, MarketId::XXXX };
static constexpr short d_COR[3] = { 2, MarketId::MVCX, MarketId::XBCC };
static constexpr short d_COS[2] = { 1, MarketId::XXXX };
static constexpr short d_CPD[2] = { 1, MarketId::XXXX };
static constexpr short d_CPH[18] = { 17, MarketId::ABSI, MarketId::ALSI, MarketId::DASI, MarketId::DCSE, MarketId::DNDK, MarketId::DNK0, MarketId::DSME, MarketId::FNDK, MarketId::LASP, MarketId::MCSE, MarketId::MNDK, MarketId::NPGA, MarketId::NYSI, MarketId::SXSI, MarketId::XCSE, MarketId::XFND, MarketId::XTRA };
static constexpr short d_CPR[2] = { 1, MarketId::XXXX };
static constexpr short d_CPT[2] = { 1, MarketId::XXXX };
static constexpr short d_CRD[2] = { 1, MarketId::XXXX };
static constexpr short d_CRF[2] = { 1, MarketId::XXXX };
static constexpr short d_CRG[2] = { 1, MarketId::XXXX };
static constexpr short d_CRP[2] = { 1, MarketId::XXXX };
static constexpr short d_CRW[2] = { 1, MarketId::XXXX };
static constexpr short d_CSG[2] = { 1, MarketId::XXXX };
static constexpr short d_CSI[2] = { 1, MarketId::XXXX };
static constexpr short d_CSN[2] = { 1, MarketId::XXXX };
static constexpr short d_CST[2] = { 1, MarketId::XXXX };
static constexpr short d_CT0[2] = { 1, MarketId::XXXX };
static constexpr short d_CTA[2] = { 1, MarketId::XXXX };
static constexpr short d_CTC[2] = { 1, MarketId::XXXX };
static constexpr short d_CTG[2] = { 1, MarketId::XXXX };
static constexpr short d_CTN[2] = { 1, MarketId::XXXX };
static constexpr short d_CTU[2] = { 1, MarketId::XXXX };
static constexpr short d_CUL[2] = { 1, MarketId::XXXX };
static constexpr short d_CUN[2] = { 1, MarketId::XXXX };
static constexpr short d_CUR[3] = { 2, MarketId::CUW0, MarketId::DCSX };
static constexpr short d_CUU[2] = { 1, MarketId::XXXX };
static constexpr short d_CVG[2] = { 1, MarketId::XXXX };
static constexpr short d_CVM[2] = { 1, MarketId::XXXX };
static constexpr short d_CVQ[2] = { 1, MarketId::XXXX };
static constexpr short d_CVT[2] = { 1, MarketId::XXXX };
static constexpr short d_CWA[2] = { 1, MarketId::XXXX };
static constexpr short d_CWB[2] = { 1, MarketId::XBVP };
static constexpr short d_CWL[2] = { 1, MarketId::XXXX };
static constexpr short d_CXI[2] = { 1, MarketId::XXXX };
static constexpr short d_CXT[2] = { 1, MarketId::XXXX };
static constexpr short d_CYS[2] = { 1, MarketId::XXXX };
static constexpr short d_CYU[2] = { 1, MarketId::XXXX };
static constexpr short d_CZA[2] = { 1, MarketId::XXXX };
static constexpr short d_CZL[2] = { 1, MarketId::XXXX };
static constexpr short d_CZM[2] = { 1, MarketId::XXXX };
static constexpr short d_DAB[2] = { 1, MarketId::XXXX };
static constexpr short d_DAC[3] = { 2, MarketId::BGD0, MarketId::XDHA };
static constexpr short d_DAL[2] = { 1, MarketId::XXXX };
static constexpr short d_DAM[3] = { 2, MarketId::SYR0, MarketId::XDSE };
static constexpr short d_DAN[2] = { 1, MarketId::XXXX };
static constexpr short d_DAR[3] = { 2, MarketId::TZA0, MarketId::XDAR };
static constexpr short d_DAY[2] = { 1, MarketId::XXXX };
static constexpr short d_DBO[2] = { 1, MarketId::XXXX };
static constexpr short d_DBQ[2] = { 1, MarketId::XXXX };
static constexpr short d_DBV[2] = { 1, MarketId::XXXX };
static constexpr short d_DBY[2] = { 1, MarketId::XXXX };
static constexpr short d_DCA[10] = { 9, MarketId::FINO, MarketId::FINR, MarketId::FINY, MarketId::NBXO, MarketId::OOTC, MarketId::USA0, MarketId::XADF, MarketId::XOTC, MarketId::XPOR };
static constexpr short d_DCF[2] = { 1, MarketId::DMA0 };
static constexpr short d_DDI[2] = { 1, MarketId::XXXX };
static constexpr short d_DEC[2] = { 1, MarketId::XXXX };
static constexpr short d_DEL[3] = { 2, MarketId::IND0, MarketId::XDES };
static constexpr short d_DEN[2] = { 1, MarketId::XXXX };
static constexpr short d_DET[2] = { 1, MarketId::XXXX };
static constexpr short d_DHA[2] = { 1, MarketId::XXXX };
static constexpr short d_DHN[2] = { 1, MarketId::XXXX };
static constexpr short d_DIL[2] = { 1, MarketId::TLS0 };
static constexpr short d_DJB[2] = { 1, MarketId::XXXX };
static constexpr short d_DJE[2] = { 1, MarketId::XXXX };
static constexpr short d_DJJ[2] = { 1, MarketId::XXXX };
static constexpr short d_DJO[2] = { 1, MarketId::XXXX };
static constexpr short d_DKI[2] = { 1, MarketId::XXXX };
static constexpr short d_DKR[2] = { 1, MarketId::SEN0 };
static constexpr short d_DLA[2] = { 1, MarketId::XDSX };
static constexpr short d_DLC[2] = { 1, MarketId::XDCE };
static constexpr short d_DLG[2] = { 1, MarketId::XXXX };
static constexpr short d_DLH[2] = { 1, MarketId::XXXX };
static constexpr short d_DLM[2] = { 1, MarketId::XXXX };
static constexpr short d_DMM[2] = { 1, MarketId::XXXX };
static constexpr short d_DND[2] = { 1, MarketId::XXXX };
static constexpr short d_DNK[3] = { 2, MarketId::SEPE, MarketId::XPRI };
static constexpr short d_DNR[2] = { 1, MarketId::XXXX };
static constexpr short d_DNZ[2] = { 1, MarketId::XXXX };
static constexpr short d_DOD[2] = { 1, MarketId::XXXX };
static constexpr short d_DOH[3] = { 2, MarketId::DSMD, MarketId::QAT0 };
static constexpr short d_DOM[2] = { 1, MarketId::XXXX };
static constexpr short d_DOV[2] = { 1, MarketId::XXXX };
static constexpr short d_DPO[2] = { 1, MarketId::XXXX };
static constexpr short d_DPS[2] = { 1, MarketId::XXXX };
static constexpr short d_DRB[2] = { 1, MarketId::XXXX };
static constexpr short d_DRO[2] = { 1, MarketId::XXXX };
static constexpr short d_DRS[2] = { 1, MarketId::XXXX };
static constexpr short d_DRW[2] = { 1, MarketId::XXXX };
static constexpr short d_DSK[2] = { 1, MarketId::XXXX };
static constexpr short d_DSM[2] = { 1, MarketId::XXXX };
static constexpr short d_DTM[2] = { 1, MarketId::XXXX };
static constexpr short d_DUB[60] = { 59, MarketId::AILT, MarketId::AREX, MarketId::BBIE, MarketId::BBIS, MarketId::BKDM, MarketId::BMLI, MarketId::BMLS, MarketId::BMLX, MarketId::BMSI, MarketId::CEPL, MarketId::CFIL, MarketId::CSGI, MarketId::DAVY, MarketId::DDUB, MarketId::EDBT, MarketId::EDGL, MarketId::EQIE, MarketId::EQSE, MarketId::EUCC, MarketId::FXFM, MarketId::FXRQ, MarketId::FXRS, MarketId::GBSI, MarketId::HREU, MarketId::IBSI, MarketId::ICUR, MarketId::ICXR, MarketId::IFXA, MarketId::IFXC, MarketId::IFXR, MarketId::IRL0, MarketId::ITGL, MarketId::LEUE, MarketId::LEUF, MarketId::MAQE, MarketId::MCID, MarketId::MSEL, MarketId::RMTF, MarketId::SEMX, MarketId::SIDX, MarketId::SISI, MarketId::TDGF, MarketId::TMEU, MarketId::VFIL, MarketId::VFSI, MarketId::VFXO, MarketId::XASM, MarketId::XATL, MarketId::XCDE, MarketId::XDUB, MarketId::XEBI, MarketId::XESM, MarketId::XEYE, MarketId::XFNX, MarketId::XIEX, MarketId::XMSM, MarketId::XPAC, MarketId::XPOS, MarketId::XRFQ };
static constexpr short d_DUD[2] = { 1, MarketId::XXXX };
static constexpr short d_DUJ[2] = { 1, MarketId::XXXX };
static constexpr short d_DUR[2] = { 1, MarketId::XXXX };
static constexpr short d_DUS[12] = { 11, MarketId::DUSA, MarketId::DUSB, MarketId::DUSC, MarketId::DUSD, MarketId::HSBT, MarketId::IKBS, MarketId::LSSI, MarketId::MHBD, MarketId::SMBD, MarketId::XDUS, MarketId::XQTX };
static constexpr short d_DUT[2] = { 1, MarketId::XXXX };
static constexpr short d_DVL[2] = { 1, MarketId::XXXX };
static constexpr short d_DXB[9] = { 8, MarketId::DGCX, MarketId::DIFX, MarketId::DUMX, MarketId::GMGD, MarketId::RFIM, MarketId::XDFM, MarketId::XGAI, MarketId::XGGI };
static constexpr short d_DYA[2] = { 1, MarketId::XXXX };
static constexpr short d_DYU[2] = { 1, MarketId::TJK0 };
static constexpr short d_DZA[2] = { 1, MarketId::MYT0 };
static constexpr short d_EA0[3] = { 2, MarketId::RBCB, MarketId::RBSI };
static constexpr short d_EAS[2] = { 1, MarketId::XXXX };
static constexpr short d_EAT[2] = { 1, MarketId::XXXX };
static constexpr short d_EAU[2] = { 1, MarketId::XXXX };
static constexpr short d_EB0[4] = { 3, MarketId::AFEX, MarketId::GBOT, MarketId::XAFX };
static constexpr short d_EBA[2] = { 1, MarketId::XXXX };
static constexpr short d_EBB[2] = { 1, MarketId::XXXX };
static constexpr short d_EBJ[2] = { 1, MarketId::XXXX };
static constexpr short d_EBU[2] = { 1, MarketId::XXXX };
static constexpr short d_EDI[4] = { 3, MarketId::BOFS, MarketId::NWMS, MarketId::SCOT };
static constexpr short d_EGC[2] = { 1, MarketId::XXXX };
static constexpr short d_EGE[2] = { 1, MarketId::XXXX };
static constexpr short d_EGS[2] = { 1, MarketId::XXXX };
static constexpr short d_EIN[2] = { 1, MarketId::XXXX };
static constexpr short d_EIS[2] = { 1, MarketId::VGB0 };
static constexpr short d_EKI[2] = { 1, MarketId::XXXX };
static constexpr short d_EKO[2] = { 1, MarketId::XXXX };
static constexpr short d_ELH[2] = { 1, MarketId::XXXX };
static constexpr short d_ELL[2] = { 1, MarketId::XXXX };
static constexpr short d_ELM[2] = { 1, MarketId::XXXX };
static constexpr short d_ELP[2] = { 1, MarketId::XXXX };
static constexpr short d_ELS[2] = { 1, MarketId::XXXX };
static constexpr short d_ELY[2] = { 1, MarketId::XXXX };
static constexpr short d_EMA[2] = { 1, MarketId::XXXX };
static constexpr short d_EMD[2] = { 1, MarketId::XXXX };
static constexpr short d_EMY[2] = { 1, MarketId::XXXX };
static constexpr short d_ENA[2] = { 1, MarketId::XXXX };
static constexpr short d_ENC[2] = { 1, MarketId::XXXX };
static constexpr short d_ENF[2] = { 1, MarketId::XXXX };
static constexpr short d_EP0[2] = { 1, MarketId::XXXX };
static constexpr short d_EPR[2] = { 1, MarketId::XXXX };
static constexpr short d_ER0[2] = { 1, MarketId::XXXX };
static constexpr short d_ERC[2] = { 1, MarketId::XXXX };
static constexpr short d_ERF[2] = { 1, MarketId::XXXX };
static constexpr short d_ERI[2] = { 1, MarketId::XXXX };
static constexpr short d_ERN[2] = { 1, MarketId::XXXX };
static constexpr short d_ERZ[2] = { 1, MarketId::XXXX };
static constexpr short d_ES0[2] = { 1, MarketId::FGEX };
static constexpr short d_ESC[2] = { 1, MarketId::XXXX };
static constexpr short d_ESF[2] = { 1, MarketId::XXXX };
static constexpr short d_ESH[2] = { 1, MarketId::MAXD };
static constexpr short d_ESR[2] = { 1, MarketId::XXXX };
static constexpr short d_ETH[2] = { 1, MarketId::XXXX };
static constexpr short d_EUG[2] = { 1, MarketId::XXXX };
static constexpr short d_EUN[2] = { 1, MarketId::ESH0 };
static constexpr short d_EVE[2] = { 1, MarketId::XXXX };
static constexpr short d_EVN[3] = { 2, MarketId::ARM0, MarketId::XARM };
static constexpr short d_EVV[2] = { 1, MarketId::XXXX };
static constexpr short d_EWN[2] = { 1, MarketId::XXXX };
static constexpr short d_EWR[27] = { 26, MarketId::AATS, MarketId::BTEC, MarketId::DWFI, MarketId::EDXM, MarketId::GLPX, MarketId::GTXS, MarketId::ICSU, MarketId::KNCM, MarketId::KNEM, MarketId::KNIG, MarketId::KNLI, MarketId::KNMX, MarketId::MAGM, MarketId::MEMD, MarketId::MEMM, MarketId::MEMX, MarketId::MXOP, MarketId::OCEA, MarketId::SPTX, MarketId::TERA, MarketId::THEM, MarketId::TPSE, MarketId::TPSV, MarketId::TRFX, MarketId::TRWB, MarketId::XBTF };
static constexpr short d_EXT[2] = { 1, MarketId::XXXX };
static constexpr short d_EYW[2] = { 1, MarketId::XXXX };
static constexpr short d_FAE[3] = { 2, MarketId::FRO0, MarketId::VMFX };
static constexpr short d_FAI[2] = { 1, MarketId::XXXX };
static constexpr short d_FAO[2] = { 1, MarketId::XXXX };
static constexpr short d_FAR[2] = { 1, MarketId::XXXX };
static constexpr short d_FAT[2] = { 1, MarketId::XXXX };
static constexpr short d_FAY[2] = { 1, MarketId::XXXX };
static constexpr short d_FBM[2] = { 1, MarketId::XXXX };
static constexpr short d_FC0[2] = { 1, MarketId::XXXX };
static constexpr short d_FCA[2] = { 1, MarketId::XXXX };
static constexpr short d_FDF[2] = { 1, MarketId::MTQ0 };
static constexpr short d_FDH[2] = { 1, MarketId::XXXX };
static constexpr short d_FEZ[2] = { 1, MarketId::XXXX };
static constexpr short d_FFT[2] = { 1, MarketId::XXXX };
static constexpr short d_FHU[2] = { 1, MarketId::XXXX };
static constexpr short d_FIE[2] = { 1, MarketId::XXXX };
static constexpr short d_FIH[2] = { 1, MarketId::COD0 };
static constexpr short d_FJR[2] = { 1, MarketId::XXXX };
static constexpr short d_FKB[2] = { 1, MarketId::XXXX };
static constexpr short d_FKI[2] = { 1, MarketId::XXXX };
static constexpr short d_FKL[2] = { 1, MarketId::XXXX };
static constexpr short d_FKS[2] = { 1, MarketId::XXXX };
static constexpr short d_FLG[2] = { 1, MarketId::XXXX };
static constexpr short d_FLL[2] = { 1, MarketId::XXXX };
static constexpr short d_FLN[2] = { 1, MarketId::XXXX };
static constexpr short d_FLO[2] = { 1, MarketId::XXXX };
static constexpr short d_FLR[5] = { 4, MarketId::DDTX, MarketId::IBEQ, MarketId::IBIS, MarketId::SYNK };
static constexpr short d_FMN[2] = { 1, MarketId::XXXX };
static constexpr short d_FMO[2] = { 1, MarketId::XXXX };
static constexpr short d_FMY[2] = { 1, MarketId::XXXX };
static constexpr short d_FNA[2] = { 1, MarketId::SLE0 };
static constexpr short d_FNC[2] = { 1, MarketId::XXXX };
static constexpr short d_FNI[2] = { 1, MarketId::XXXX };
static constexpr short d_FNJ[2] = { 1, MarketId::PRK0 };
static constexpr short d_FNT[2] = { 1, MarketId::XXXX };
static constexpr short d_FOD[2] = { 1, MarketId::XXXX };
static constexpr short d_FOE[2] = { 1, MarketId::XXXX };
static constexpr short d_FOR[2] = { 1, MarketId::XXXX };
static constexpr short d_FOU[2] = { 1, MarketId::XXXX };
static constexpr short d_FPO[2] = { 1, MarketId::XXXX };
static constexpr short d_FRA[110] = { 109, MarketId::_21XX, MarketId::_360T, MarketId::_360X, MarketId::AUTO, MarketId::BINV, MarketId::BOCF, MarketId::BPAG, MarketId::CATS, MarketId::CBKA, MarketId::CBKC, MarketId::CBKD, MarketId::CBKE, MarketId::CBKF, MarketId::CBKG, MarketId::CBKS, MarketId::CDSI, MarketId::CGEC, MarketId::CGEE, MarketId::CGET, MarketId::CGMD, MarketId::CSDA, MarketId::DAPA, MarketId::DBAG, MarketId::DBDX, MarketId::DBES, MarketId::DBLN, MarketId::DBMO, MarketId::DBOX, MarketId::DEKA, MarketId::DZBK, MarketId::EBLX, MarketId::ECAG, MarketId::ECGS, MarketId::ENTW, MarketId::ERFQ, MarketId::FICX, MarketId::FRAA, MarketId::FRAB, MarketId::FRAD, MarketId::FRAS, MarketId::FRAU, MarketId::FRAV, MarketId::FRAW, MarketId::GSBE, MarketId::GSEI, MarketId::HELA, MarketId::JEFE, MarketId::JESI, MarketId::JPEU, MarketId::LBCW, MarketId::MESI, MarketId::MHEU, MarketId::MSEU, MarketId::NCME, MarketId::NESI, MarketId::RBCG, MarketId::SCAG, MarketId::SEBA, MarketId::SMFF, MarketId::SPEX, MarketId::TPDE, MarketId::TSFF, MarketId::TSFG, MarketId::UBSD, MarketId::UBSI, MarketId::UBSL, MarketId::VONT, MarketId::VTLS, MarketId::VTPS, MarketId::VWDA, MarketId::VWDX, MarketId::XDBC, MarketId::XDBV, MarketId::XDBX, MarketId::XDTB, MarketId::XDWZ, MarketId::XECB, MarketId::XEHQ, MarketId::XEMA, MarketId::XEMB, MarketId::XEMI, MarketId::XERE, MarketId::XERT, MarketId::XETA, MarketId::XETB, MarketId::XETC, MarketId::XETD, MarketId::XETE, MarketId::XETI, MarketId::XETR, MarketId::XETS, MarketId::XETU, MarketId::XETV, MarketId::XETW, MarketId::XETX, MarketId::XEUB, MarketId::XEUM, MarketId::XEUP, MarketId::XEUR, MarketId::XFRA, MarketId::XIGG, MarketId::XINV, MarketId::XNEW, MarketId::XRTR, MarketId::XSC1, MarketId::XSC2, MarketId::XSC3, MarketId::XSCO, MarketId::XXSC };
static constexpr short d_FRI[2] = { 1, MarketId::XXXX };
static constexpr short d_FRJ[2] = { 1, MarketId::XXXX };
static constexpr short d_FRO[2] = { 1, MarketId::XXXX };
static constexpr short d_FRU[3] = { 2, MarketId::KGZ0, MarketId::XKSE };
static constexpr short d_FRW[2] = { 1, MarketId::XXXX };
static constexpr short d_FSC[2] = { 1, MarketId::XXXX };
static constexpr short d_FSD[2] = { 1, MarketId::XXXX };
static constexpr short d_FSM[2] = { 1, MarketId::XXXX };
static constexpr short d_FSP[2] = { 1, MarketId::SPM0 };
static constexpr short d_FUE[2] = { 1, MarketId::XXXX };
static constexpr short d_FUK[3] = { 2, MarketId::XFFE, MarketId::XFKA };
static constexpr short d_FUN[2] = { 1, MarketId::TUV0 };
static constexpr short d_FUT[2] = { 1, MarketId::XXXX };
static constexpr short d_FWA[2] = { 1, MarketId::XXXX };
static constexpr short d_FYV[2] = { 1, MarketId::XXXX };
static constexpr short d_GAD[2] = { 1, MarketId::XXXX };
static constexpr short d_GAJ[2] = { 1, MarketId::XXXX };
static constexpr short d_GAU[2] = { 1, MarketId::XXXX };
static constexpr short d_GBE[5] = { 4, MarketId::BOTE, MarketId::BOTV, MarketId::BWA0, MarketId::XBOT };
static constexpr short d_GC0[2] = { 1, MarketId::IINX };
static constexpr short d_GCC[2] = { 1, MarketId::XXXX };
static constexpr short d_GCI[3] = { 2, MarketId::GGY0, MarketId::XCIE };
static constexpr short d_GCM[3] = { 2, MarketId::CYM0, MarketId::XCAY };
static constexpr short d_GCN[2] = { 1, MarketId::XXXX };
static constexpr short d_GDL[2] = { 1, MarketId::XXXX };
static constexpr short d_GDN[2] = { 1, MarketId::XXXX };
static constexpr short d_GDT[2] = { 1, MarketId::TCA0 };
static constexpr short d_GDV[2] = { 1, MarketId::XXXX };
static constexpr short d_GEG[2] = { 1, MarketId::XXXX };
static constexpr short d_GEO[3] = { 2, MarketId::GSCI, MarketId::GUY0 };
static constexpr short d_GET[2] = { 1, MarketId::XXXX };
static constexpr short d_GEX[2] = { 1, MarketId::XXXX };
static constexpr short d_GFF[2] = { 1, MarketId::XXXX };
static constexpr short d_GFK[2] = { 1, MarketId::XXXX };
static constexpr short d_GGG[2] = { 1, MarketId::XXXX };
static constexpr short d_GGW[2] = { 1, MarketId::XXXX };
static constexpr short d_GH0[2] = { 1, MarketId::INSE };
static constexpr short d_GHB[2] = { 1, MarketId::XXXX };
static constexpr short d_GIB[3] = { 2, MarketId::GIB0, MarketId::GSXL };
static constexpr short d_GID[2] = { 1, MarketId::XXXX };
static constexpr short d_GIL[2] = { 1, MarketId::XXXX };
static constexpr short d_GJL[2] = { 1, MarketId::XXXX };
static constexpr short d_GJT[2] = { 1, MarketId::XXXX };
static constexpr short d_GKL[2] = { 1, MarketId::XXXX };
static constexpr short d_GLA[2] = { 1, MarketId::XXXX };
static constexpr short d_GLH[2] = { 1, MarketId::XXXX };
static constexpr short d_GLT[2] = { 1, MarketId::XXXX };
static constexpr short d_GNB[2] = { 1, MarketId::XXXX };
static constexpr short d_GND[2] = { 1, MarketId::GRD0 };
static constexpr short d_GNE[2] = { 1, MarketId::XXXX };
static constexpr short d_GNV[2] = { 1, MarketId::XXXX };
static constexpr short d_GOA[2] = { 1, MarketId::BPAS };
static constexpr short d_GOH[2] = { 1, MarketId::GRL0 };
static constexpr short d_GOI[2] = { 1, MarketId::XXXX };
static constexpr short d_GOJ[2] = { 1, MarketId::NNCS };
static constexpr short d_GON[2] = { 1, MarketId::XXXX };
static constexpr short d_GOO[2] = { 1, MarketId::XXXX };
static constexpr short d_GOT[2] = { 1, MarketId::XXXX };
static constexpr short d_GOU[2] = { 1, MarketId::XXXX };
static constexpr short d_GOV[2] = { 1, MarketId::XXXX };
static constexpr short d_GOZ[2] = { 1, MarketId::XXXX };
static constexpr short d_GPA[2] = { 1, MarketId::XXXX };
static constexpr short d_GPT[2] = { 1, MarketId::XXXX };
static constexpr short d_GPZ[2] = { 1, MarketId::XXXX };
static constexpr short d_GR0[2] = { 1, MarketId::JYSI };
static constexpr short d_GRB[2] = { 1, MarketId::XXXX };
static constexpr short d_GRJ[2] = { 1, MarketId::XXXX };
static constexpr short d_GRO[2] = { 1, MarketId::XXXX };
static constexpr short d_GRQ[2] = { 1, MarketId::XXXX };
static constexpr short d_GRR[2] = { 1, MarketId::XXXX };
static constexpr short d_GRX[2] = { 1, MarketId::XSRM };
static constexpr short d_GRZ[2] = { 1, MarketId::XXXX };
static constexpr short d_GS0[2] = { 1, MarketId::SGS0 };
static constexpr short d_GSI[2] = { 1, MarketId::XXXX };
static constexpr short d_GSO[2] = { 1, MarketId::XXXX };
static constexpr short d_GSP[2] = { 1, MarketId::XXXX };
static constexpr short d_GTE[2] = { 1, MarketId::XXXX };
static constexpr short d_GTF[2] = { 1, MarketId::XXXX };
static constexpr short d_GTI[2] = { 1, MarketId::XXXX };
static constexpr short d_GUA[3] = { 2, MarketId::GTM0, MarketId::XGTG };
static constexpr short d_GUC[2] = { 1, MarketId::XXXX };
static constexpr short d_GUM[2] = { 1, MarketId::GUM0 };
static constexpr short d_GVA[3] = { 2, MarketId::CCMS, MarketId::TDXS };
static constexpr short d_GW0[9] = { 8, MarketId::IATS, MarketId::IBCO, MarketId::IBKR, MarketId::IEOS, MarketId::PIPR, MarketId::PJCX, MarketId::WEED, MarketId::XWEE };
static constexpr short d_GWD[2] = { 1, MarketId::XXXX };
static constexpr short d_GWE[2] = { 1, MarketId::XXXX };
static constexpr short d_GWT[2] = { 1, MarketId::XXXX };
static constexpr short d_GWY[2] = { 1, MarketId::XXXX };
static constexpr short d_GYE[2] = { 1, MarketId::XGUA };
static constexpr short d_GYI[2] = { 1, MarketId::XXXX };
static constexpr short d_GYN[2] = { 1, MarketId::XXXX };
static constexpr short d_GYP[2] = { 1, MarketId::XXXX };
static constexpr short d_GZA[2] = { 1, MarketId::XXXX };
static constexpr short d_GZT[2] = { 1, MarketId::XXXX };
static constexpr short d_HAC[2] = { 1, MarketId::XXXX };
static constexpr short d_HAG[3] = { 2, MarketId::NIBC, MarketId::NPEX };
static constexpr short d_HAH[2] = { 1, MarketId::COM0 };
static constexpr short d_HAJ[9] = { 8, MarketId::HANA, MarketId::HANB, MarketId::HANC, MarketId::HAND, MarketId::HANE, MarketId::NLBX, MarketId::XHAN, MarketId::XHCE };
static constexpr short d_HAM[12] = { 11, MarketId::BGFI, MarketId::BGFX, MarketId::BGSI, MarketId::HAMA, MarketId::HAMB, MarketId::HAML, MarketId::HAMM, MarketId::HAMN, MarketId::NORD, MarketId::SSWM, MarketId::XHAM };
static constexpr short d_HAN[5] = { 4, MarketId::HSTC, MarketId::VNM0, MarketId::XHNF, MarketId::XHNX };
static constexpr short d_HAR[2] = { 1, MarketId::XXXX };
static constexpr short d_HAU[2] = { 1, MarketId::XXXX };
static constexpr short d_HAV[2] = { 1, MarketId::CUB0 };
static constexpr short d_HBA[2] = { 1, MarketId::XXXX };
static constexpr short d_HDD[2] = { 1, MarketId::XXXX };
static constexpr short d_HDN[2] = { 1, MarketId::XXXX };
static constexpr short d_HDY[2] = { 1, MarketId::XXXX };
static constexpr short d_HEA[2] = { 1, MarketId::XXXX };
static constexpr short d_HEL[13] = { 12, MarketId::AXSI, MarketId::DHEL, MarketId::DNFI, MarketId::FIN0, MarketId::FNFI, MarketId::FSME, MarketId::MHEL, MarketId::MNFI, MarketId::OPCO, MarketId::XFOM, MarketId::XHEL, MarketId::XNOR };
static constexpr short d_HER[2] = { 1, MarketId::XXXX };
static constexpr short d_HFA[2] = { 1, MarketId::XXXX };
static constexpr short d_HFT[2] = { 1, MarketId::XXXX };
static constexpr short d_HGA[2] = { 1, MarketId::XXXX };
static constexpr short d_HGH[2] = { 1, MarketId::XXXX };
static constexpr short d_HGO[2] = { 1, MarketId::XXXX };
static constexpr short d_HHH[2] = { 1, MarketId::XXXX };
static constexpr short d_HIB[2] = { 1, MarketId::XXXX };
static constexpr short d_HII[2] = { 1, MarketId::XXXX };
static constexpr short d_HIJ[2] = { 1, MarketId::XHIR };
static constexpr short d_HIR[2] = { 1, MarketId::SLB0 };
static constexpr short d_HIS[2] = { 1, MarketId::XXXX };
static constexpr short d_HKD[2] = { 1, MarketId::XXXX };
static constexpr short d_HKG[38] = { 37, MarketId::BACR, MarketId::BAEP, MarketId::BAIP, MarketId::BASE, MarketId::BASP, MarketId::CFHK, MarketId::CGMH, MarketId::CLHK, MarketId::CSHK, MarketId::DBHK, MarketId::EOTC, MarketId::GSAL, MarketId::GSPL, MarketId::GSXH, MarketId::GSXM, MarketId::GSXN, MarketId::HKG0, MarketId::HKME, MarketId::HSXA, MarketId::JPMI, MarketId::MAQH, MarketId::MEHK, MarketId::OSDS, MarketId::SHSC, MarketId::SIGH, MarketId::SZSC, MarketId::TFSD, MarketId::TOCP, MarketId::TRAS, MarketId::TWHK, MarketId::UBSX, MarketId::XCGS, MarketId::XGEM, MarketId::XHKF, MarketId::XHKG, MarketId::XIHK, MarketId::XPST };
static constexpr short d_HKT[2] = { 1, MarketId::XXXX };
static constexpr short d_HKY[2] = { 1, MarketId::XXXX };
static constexpr short d_HLA[2] = { 1, MarketId::XXXX };
static constexpr short d_HLH[2] = { 1, MarketId::XXXX };
static constexpr short d_HLN[2] = { 1, MarketId::XXXX };
static constexpr short d_HLT[2] = { 1, MarketId::XXXX };
static constexpr short d_HLZ[2] = { 1, MarketId::XXXX };
static constexpr short d_HMO[2] = { 1, MarketId::XXXX };
static constexpr short d_HNA[2] = { 1, MarketId::XXXX };
static constexpr short d_HNH[2] = { 1, MarketId::XXXX };
static constexpr short d_HNK[2] = { 1, MarketId::XXXX };
static constexpr short d_HNL[2] = { 1, MarketId::XXXX };
static constexpr short d_HNS[2] = { 1, MarketId::XXXX };
static constexpr short d_HOG[2] = { 1, MarketId::XXXX };
static constexpr short d_HOM[2] = { 1, MarketId::XXXX };
static constexpr short d_HON[2] = { 1, MarketId::XXXX };
static constexpr short d_HOQ[2] = { 1, MarketId::XXXX };
static constexpr short d_HOR[2] = { 1, MarketId::XXXX };
static constexpr short d_HOU[2] = { 1, MarketId::ASEF };
static constexpr short d_HPN[2] = { 1, MarketId::XXXX };
static constexpr short d_HRB[2] = { 1, MarketId::XXXX };
static constexpr short d_HRE[3] = { 2, MarketId::XZIM, MarketId::ZWE0 };
static constexpr short d_HRG[2] = { 1, MarketId::XXXX };
static constexpr short d_HRK[2] = { 1, MarketId::XKHR };
static constexpr short d_HRL[2] = { 1, MarketId::XXXX };
static constexpr short d_HS0[6] = { 5, MarketId::DAMP, MarketId::DKTC, MarketId::GXGF, MarketId::GXGM, MarketId::GXGR };
static constexpr short d_HSV[2] = { 1, MarketId::XXXX };
static constexpr short d_HTA[2] = { 1, MarketId::XXXX };
static constexpr short d_HTI[2] = { 1, MarketId::XXXX };
static constexpr short d_HTS[2] = { 1, MarketId::XXXX };
static constexpr short d_HTV[2] = { 1, MarketId::XXXX };
static constexpr short d_HUF[2] = { 1, MarketId::XXXX };
static constexpr short d_HUH[2] = { 1, MarketId::XXXX };
static constexpr short d_HUI[2] = { 1, MarketId::XXXX };
static constexpr short d_HUX[2] = { 1, MarketId::XXXX };
static constexpr short d_HUY[2] = { 1, MarketId::XXXX };
static constexpr short d_HVB[2] = { 1, MarketId::XXXX };
static constexpr short d_HVN[2] = { 1, MarketId::XXXX };
static constexpr short d_HVR[2] = { 1, MarketId::XXXX };
static constexpr short d_HWN[2] = { 1, MarketId::XXXX };
static constexpr short d_HYA[2] = { 1, MarketId::XXXX };
static constexpr short d_HYD[2] = { 1, MarketId::XXXX };
static constexpr short d_HYG[2] = { 1, MarketId::XXXX };
static constexpr short d_IAG[2] = { 1, MarketId::XXXX };
static constexpr short d_IBZ[2] = { 1, MarketId::XXXX };
static constexpr short d_ICN[2] = { 1, MarketId::XXXX };
static constexpr short d_ICT[2] = { 1, MarketId::XXXX };
static constexpr short d_IDA[2] = { 1, MarketId::XXXX };
static constexpr short d_IDR[2] = { 1, MarketId::NBOT };
static constexpr short d_IEV[13] = { 12, MarketId::EESE, MarketId::PFTQ, MarketId::PFTS, MarketId::UICE, MarketId::UKEX, MarketId::UKR0, MarketId::XDFB, MarketId::XKIE, MarketId::XKIS, MarketId::XNDU, MarketId::XUAX, MarketId::XUKR };
static constexpr short d_IFL[2] = { 1, MarketId::XXXX };
static constexpr short d_IFP[2] = { 1, MarketId::XXXX };
static constexpr short d_IGH[2] = { 1, MarketId::XXXX };
static constexpr short d_IGR[2] = { 1, MarketId::XXXX };
static constexpr short d_IJX[2] = { 1, MarketId::XXXX };
static constexpr short d_IKT[2] = { 1, MarketId::XXXX };
static constexpr short d_ILE[2] = { 1, MarketId::XXXX };
static constexpr short d_ILG[3] = { 2, MarketId::TRAI, MarketId::TSBX };
static constexpr short d_ILI[2] = { 1, MarketId::XXXX };
static constexpr short d_ILM[2] = { 1, MarketId::XXXX };
static constexpr short d_ILP[2] = { 1, MarketId::XXXX };
static constexpr short d_ILY[2] = { 1, MarketId::XXXX };
static constexpr short d_ILZ[2] = { 1, MarketId::SPXE };
static constexpr short d_IND[2] = { 1, MarketId::XXXX };
static constexpr short d_INI[2] = { 1, MarketId::XXXX };
static constexpr short d_INL[2] = { 1, MarketId::XXXX };
static constexpr short d_INN[2] = { 1, MarketId::XXXX };
static constexpr short d_INU[2] = { 1, MarketId::NRU0 };
static constexpr short d_INV[2] = { 1, MarketId::XXXX };
static constexpr short d_IOM[2] = { 1, MarketId::IMN0 };
static constexpr short d_IOU[2] = { 1, MarketId::XXXX };
static constexpr short d_IPC[2] = { 1, MarketId::XXXX };
static constexpr short d_IPL[2] = { 1, MarketId::XXXX };
static constexpr short d_IPT[2] = { 1, MarketId::XXXX };
static constexpr short d_IQT[2] = { 1, MarketId::XXXX };
static constexpr short d_IRG[2] = { 1, MarketId::XXXX };
static constexpr short d_IRO[2] = { 1, MarketId::XXXX };
static constexpr short d_ISA[2] = { 1, MarketId::XXXX };
static constexpr short d_ISB[3] = { 2, MarketId::PAK0, MarketId::XISL };
static constexpr short d_ISG[2] = { 1, MarketId::XXXX };
static constexpr short d_ISL[2] = { 1, MarketId::XXXX };
static constexpr short d_ISO[2] = { 1, MarketId::XXXX };
static constexpr short d_ISP[2] = { 1, MarketId::XXXX };
static constexpr short d_IST[10] = { 9, MarketId::EXTR, MarketId::XDSM, MarketId::XEDA, MarketId::XEID, MarketId::XEQY, MarketId::XFNO, MarketId::XIAB, MarketId::XIST, MarketId::XPMS };
static constexpr short d_ITH[2] = { 1, MarketId::XXXX };
static constexpr short d_ITO[2] = { 1, MarketId::XXXX };
static constexpr short d_IUE[2] = { 1, MarketId::NIU0 };
static constexpr short d_IVC[2] = { 1, MarketId::XXXX };
static constexpr short d_IVL[2] = { 1, MarketId::XXXX };
static constexpr short d_IXB[2] = { 1, MarketId::XXXX };
static constexpr short d_IXC[2] = { 1, MarketId::XXXX };
static constexpr short d_IXG[2] = { 1, MarketId::XXXX };
static constexpr short d_IXJ[2] = { 1, MarketId::XXXX };
static constexpr short d_IXR[2] = { 1, MarketId::XXXX };
static constexpr short d_IXW[2] = { 1, MarketId::XXXX };
static constexpr short d_IYK[2] = { 1, MarketId::XXXX };
static constexpr short d_IZM[2] = { 1, MarketId::XTUR };
static constexpr short d_JAA[2] = { 1, MarketId::XXXX };
static constexpr short d_JAC[2] = { 1, MarketId::XXXX };
static constexpr short d_JAD[2] = { 1, MarketId::XXXX };
static constexpr short d_JAF[2] = { 1, MarketId::XXXX };
static constexpr short d_JAG[2] = { 1, MarketId::XXXX };
static constexpr short d_JAI[2] = { 1, MarketId::XXXX };
static constexpr short d_JAK[2] = { 1, MarketId::XXXX };
static constexpr short d_JAL[2] = { 1, MarketId::XXXX };
static constexpr short d_JAM[2] = { 1, MarketId::XXXX };
static constexpr short d_JAN[2] = { 1, MarketId::XXXX };
static constexpr short d_JAU[2] = { 1, MarketId::XXXX };
static constexpr short d_JBR[2] = { 1, MarketId::XXXX };
static constexpr short d_JCB[2] = { 1, MarketId::XXXX };
static constexpr short d_JCK[2] = { 1, MarketId::XXXX };
static constexpr short d_JCM[2] = { 1, MarketId::XXXX };
static constexpr short d_JDF[2] = { 1, MarketId::XXXX };
static constexpr short d_JDH[2] = { 1, MarketId::XXXX };
static constexpr short d_JDZ[2] = { 1, MarketId::XXXX };
static constexpr short d_JED[2] = { 1, MarketId::XXXX };
static constexpr short d_JEE[2] = { 1, MarketId::XXXX };
static constexpr short d_JEF[2] = { 1, MarketId::XXXX };
static constexpr short d_JER[2] = { 1, MarketId::JEY0 };
static constexpr short d_JGA[2] = { 1, MarketId::XXXX };
static constexpr short d_JGB[2] = { 1, MarketId::XXXX };
static constexpr short d_JGN[2] = { 1, MarketId::XXXX };
static constexpr short d_JGS[2] = { 1, MarketId::XXXX };
static constexpr short d_JHB[2] = { 1, MarketId::XXXX };
static constexpr short d_JHE[2] = { 1, MarketId::XXXX };
static constexpr short d_JHG[2] = { 1, MarketId::XXXX };
static constexpr short d_JHM[2] = { 1, MarketId::XXXX };
static constexpr short d_JHQ[2] = { 1, MarketId::XXXX };
static constexpr short d_JHW[2] = { 1, MarketId::XXXX };
static constexpr short d_JIB[2] = { 1, MarketId::DJI0 };
static constexpr short d_JIJ[2] = { 1, MarketId::XXXX };
static constexpr short d_JIL[2] = { 1, MarketId::XXXX };
static constexpr short d_JIM[2] = { 1, MarketId::XXXX };
static constexpr short d_JIN[2] = { 1, MarketId::XXXX };
static constexpr short d_JIP[2] = { 1, MarketId::XXXX };
static constexpr short d_JIR[2] = { 1, MarketId::XXXX };
static constexpr short d_JIU[2] = { 1, MarketId::XXXX };
static constexpr short d_JIW[2] = { 1, MarketId::XXXX };
static constexpr short d_JJI[2] = { 1, MarketId::XXXX };
static constexpr short d_JJN[2] = { 1, MarketId::XXXX };
static constexpr short d_JKG[2] = { 1, MarketId::XXXX };
static constexpr short d_JKH[2] = { 1, MarketId::XXXX };
static constexpr short d_JKR[2] = { 1, MarketId::XXXX };
static constexpr short d_JKT[7] = { 6, MarketId::ICDX, MarketId::IDN0, MarketId::XBBJ, MarketId::XIDX, MarketId::XJKT, MarketId::XJNB };
static constexpr short d_JKV[2] = { 1, MarketId::XXXX };
static constexpr short d_JLN[2] = { 1, MarketId::XXXX };
static constexpr short d_JLR[2] = { 1, MarketId::XXXX };
static constexpr short d_JLS[2] = { 1, MarketId::XXXX };
static constexpr short d_JMB[2] = { 1, MarketId::XXXX };
static constexpr short d_JMK[2] = { 1, MarketId::XXXX };
static constexpr short d_JMO[2] = { 1, MarketId::XXXX };
static constexpr short d_JMS[2] = { 1, MarketId::XXXX };
static constexpr short d_JMU[2] = { 1, MarketId::XXXX };
static constexpr short d_JNA[2] = { 1, MarketId::XXXX };
static constexpr short d_JNB[16] = { 15, MarketId::_4AXE, MarketId::A2XX, MarketId::ALTX, MarketId::EESX, MarketId::JSEB, MarketId::JSER, MarketId::RMMS, MarketId::RMMX, MarketId::XBES, MarketId::XJSE, MarketId::XSAF, MarketId::XSFA, MarketId::YLDX, MarketId::ZARX, MarketId::ZFXM };
static constexpr short d_JNG[2] = { 1, MarketId::XXXX };
static constexpr short d_JNI[2] = { 1, MarketId::XXXX };
static constexpr short d_JNU[2] = { 1, MarketId::XXXX };
static constexpr short d_JNX[2] = { 1, MarketId::XXXX };
static constexpr short d_JNZ[2] = { 1, MarketId::XXXX };
static constexpr short d_JOE[2] = { 1, MarketId::XXXX };
static constexpr short d_JOI[2] = { 1, MarketId::XXXX };
static constexpr short d_JOL[2] = { 1, MarketId::XXXX };
static constexpr short d_JON[2] = { 1, MarketId::XXXX };
static constexpr short d_JOS[2] = { 1, MarketId::XXXX };
static constexpr short d_JPA[2] = { 1, MarketId::XXXX };
static constexpr short d_JQE[2] = { 1, MarketId::XXXX };
static constexpr short d_JRH[2] = { 1, MarketId::XXXX };
static constexpr short d_JRO[2] = { 1, MarketId::XXXX };
static constexpr short d_JRS[2] = { 1, MarketId::XXXX };
static constexpr short d_JSA[2] = { 1, MarketId::XXXX };
static constexpr short d_JSI[2] = { 1, MarketId::XXXX };
static constexpr short d_JSM[2] = { 1, MarketId::XXXX };
static constexpr short d_JST[2] = { 1, MarketId::XXXX };
static constexpr short d_JTI[2] = { 1, MarketId::XXXX };
static constexpr short d_JTR[2] = { 1, MarketId::XXXX };
static constexpr short d_JUB[2] = { 1, MarketId::SSD0 };
static constexpr short d_JUI[2] = { 1, MarketId::XXXX };
static constexpr short d_JUJ[2] = { 1, MarketId::XXXX };
static constexpr short d_JUL[2] = { 1, MarketId::XXXX };
static constexpr short d_JUN[2] = { 1, MarketId::XXXX };
static constexpr short d_JUT[2] = { 1, MarketId::XXXX };
static constexpr short d_JUV[2] = { 1, MarketId::XXXX };
static constexpr short d_JVL[2] = { 1, MarketId::XXXX };
static constexpr short d_JWA[2] = { 1, MarketId::XXXX };
static constexpr short d_JXN[2] = { 1, MarketId::XXXX };
static constexpr short d_JYV[2] = { 1, MarketId::XXXX };
static constexpr short d_KAJ[2] = { 1, MarketId::XXXX };
static constexpr short d_KAN[2] = { 1, MarketId::XXXX };
static constexpr short d_KAO[2] = { 1, MarketId::XXXX };
static constexpr short d_KBL[2] = { 1, MarketId::AFG0 };
static constexpr short d_KBY[2] = { 1, MarketId::XXXX };
static constexpr short d_KCC[2] = { 1, MarketId::XXXX };
static constexpr short d_KCE[2] = { 1, MarketId::XXXX };
static constexpr short d_KCH[2] = { 1, MarketId::XXXX };
static constexpr short d_KCM[2] = { 1, MarketId::XXXX };
static constexpr short d_KCZ[2] = { 1, MarketId::XXXX };
static constexpr short d_KDD[2] = { 1, MarketId::XXXX };
static constexpr short d_KDH[2] = { 1, MarketId::XXXX };
static constexpr short d_KDU[2] = { 1, MarketId::XXXX };
static constexpr short d_KEL[2] = { 1, MarketId::XXXX };
static constexpr short d_KEM[2] = { 1, MarketId::XXXX };
static constexpr short d_KEN[2] = { 1, MarketId::XXXX };
static constexpr short d_KGC[2] = { 1, MarketId::XXXX };
static constexpr short d_KGD[2] = { 1, MarketId::XXXX };
static constexpr short d_KGI[2] = { 1, MarketId::XXXX };
static constexpr short d_KGL[4] = { 3, MarketId::ROTC, MarketId::RSEX, MarketId::RWA0 };
static constexpr short d_KGS[2] = { 1, MarketId::XXXX };
static constexpr short d_KHH[2] = { 1, MarketId::XXXX };
static constexpr short d_KHI[3] = { 2, MarketId::NCEL, MarketId::XKAR };
static constexpr short d_KHJ[2] = { 1, MarketId::XXXX };
static constexpr short d_KHV[2] = { 1, MarketId::XXXX };
static constexpr short d_KID[2] = { 1, MarketId::XXXX };
static constexpr short d_KIJ[2] = { 1, MarketId::XNII };
static constexpr short d_KIK[2] = { 1, MarketId::XXXX };
static constexpr short d_KIM[2] = { 1, MarketId::XXXX };
static constexpr short d_KIN[3] = { 2, MarketId::JAM0, MarketId::XJAM };
static constexpr short d_KIR[2] = { 1, MarketId::XXXX };
static constexpr short d_KIV[3] = { 2, MarketId::MDA0, MarketId::XMOL };
static constexpr short d_KIW[2] = { 1, MarketId::XXXX };
static constexpr short d_KKN[2] = { 1, MarketId::XXXX };
static constexpr short d_KLA[4] = { 3, MarketId::UGA0, MarketId::ULTX, MarketId::XUGA };
static constexpr short d_KLR[2] = { 1, MarketId::XXXX };
static constexpr short d_KLU[2] = { 1, MarketId::BKSK };
static constexpr short d_KLW[2] = { 1, MarketId::XXXX };
static constexpr short d_KLX[2] = { 1, MarketId::XXXX };
static constexpr short d_KLZ[2] = { 1, MarketId::XXXX };
static constexpr short d_KMI[2] = { 1, MarketId::XXXX };
static constexpr short d_KMJ[2] = { 1, MarketId::XXXX };
static constexpr short d_KMP[2] = { 1, MarketId::XXXX };
static constexpr short d_KMQ[2] = { 1, MarketId::XXXX };
static constexpr short d_KMS[2] = { 1, MarketId::XXXX };
static constexpr short d_KNS[2] = { 1, MarketId::XXXX };
static constexpr short d_KNU[2] = { 1, MarketId::XXXX };
static constexpr short d_KNX[2] = { 1, MarketId::XXXX };
static constexpr short d_KOA[2] = { 1, MarketId::XXXX };
static constexpr short d_KOI[2] = { 1, MarketId::XXXX };
static constexpr short d_KOJ[2] = { 1, MarketId::XXXX };
static constexpr short d_KOK[2] = { 1, MarketId::XXXX };
static constexpr short d_KRB[2] = { 1, MarketId::XXXX };
static constexpr short d_KRK[2] = { 1, MarketId::NSSA };
static constexpr short d_KRN[2] = { 1, MarketId::XXXX };
static constexpr short d_KRP[2] = { 1, MarketId::XXXX };
static constexpr short d_KRS[2] = { 1, MarketId::XXXX };
static constexpr short d_KRT[3] = { 2, MarketId::SDN0, MarketId::XKHA };
static constexpr short d_KSD[2] = { 1, MarketId::XXXX };
static constexpr short d_KSL[2] = { 1, MarketId::XXXX };
static constexpr short d_KSS[2] = { 1, MarketId::XXXX };
static constexpr short d_KSU[2] = { 1, MarketId::XXXX };
static constexpr short d_KTA[2] = { 1, MarketId::XXXX };
static constexpr short d_KTB[2] = { 1, MarketId::XXXX };
static constexpr short d_KTM[3] = { 2, MarketId::NPL0, MarketId::XNEP };
static constexpr short d_KTN[2] = { 1, MarketId::XXXX };
static constexpr short d_KTR[2] = { 1, MarketId::XXXX };
static constexpr short d_KTT[2] = { 1, MarketId::XXXX };
static constexpr short d_KUA[2] = { 1, MarketId::XXXX };
static constexpr short d_KUF[2] = { 1, MarketId::XSAM };
static constexpr short d_KUH[2] = { 1, MarketId::XXXX };
static constexpr short d_KUL[6] = { 5, MarketId::MESQ, MarketId::MYS0, MarketId::XKLS, MarketId::XLOF, MarketId::XRBM };
static constexpr short d_KUO[2] = { 1, MarketId::XXXX };
static constexpr short d_KVA[2] = { 1, MarketId::XXXX };
static constexpr short d_KWI[3] = { 2, MarketId::KWT0, MarketId::XKUW };
static constexpr short d_KWL[2] = { 1, MarketId::XXXX };
static constexpr short d_KWM[2] = { 1, MarketId::XXXX };
static constexpr short d_KYA[2] = { 1, MarketId::XXXX };
static constexpr short d_KYS[2] = { 1, MarketId::XXXX };
static constexpr short d_KZN[2] = { 1, MarketId::XXXX };
static constexpr short d_LAD[3] = { 2, MarketId::AGO0, MarketId::XBDV };
static constexpr short d_LAE[2] = { 1, MarketId::XXXX };
static constexpr short d_LAF[2] = { 1, MarketId::XXXX };
static constexpr short d_LAI[2] = { 1, MarketId::XXXX };
static constexpr short d_LAN[2] = { 1, MarketId::XXXX };
static constexpr short d_LAP[2] = { 1, MarketId::XXXX };
static constexpr short d_LAR[2] = { 1, MarketId::XXXX };
static constexpr short d_LAS[2] = { 1, MarketId::XXXX };
static constexpr short d_LAW[2] = { 1, MarketId::XXXX };
static constexpr short d_LAX[3] = { 2, MarketId::AMLG, MarketId::SFOX };
static constexpr short d_LBA[2] = { 1, MarketId::UGEN };
static constexpr short d_LBB[2] = { 1, MarketId::XXXX };
static constexpr short d_LBE[2] = { 1, MarketId::XXXX };
static constexpr short d_LBI[2] = { 1, MarketId::XXXX };
static constexpr short d_LBQ[2] = { 1, MarketId::XXXX };
static constexpr short d_LBU[2] = { 1, MarketId::XLFX };
static constexpr short d_LBV[2] = { 1, MarketId::GAB0 };
static constexpr short d_LCA[2] = { 1, MarketId::CFIF };
static constexpr short d_LCG[2] = { 1, MarketId::XXXX };
static constexpr short d_LCH[2] = { 1, MarketId::XXXX };
static constexpr short d_LDC[2] = { 1, MarketId::XXXX };
static constexpr short d_LDE[2] = { 1, MarketId::XXXX };
static constexpr short d_LDK[2] = { 1, MarketId::XXXX };
static constexpr short d_LDY[2] = { 1, MarketId::XXXX };
static constexpr short d_LEB[2] = { 1, MarketId::XXXX };
static constexpr short d_LED[5] = { 4, MarketId::IXSP, MarketId::SPIM, MarketId::XPET, MarketId::XPIC };
static constexpr short d_LEI[2] = { 1, MarketId::XXXX };
static constexpr short d_LEJ[7] = { 6, MarketId::XECC, MarketId::XEEE, MarketId::XEEO, MarketId::XEER, MarketId::XPOT, MarketId::XPSF };
static constexpr short d_LEK[2] = { 1, MarketId::XXXX };
static constexpr short d_LEQ[2] = { 1, MarketId::XXXX };
static constexpr short d_LER[2] = { 1, MarketId::XXXX };
static constexpr short d_LEX[2] = { 1, MarketId::XXXX };
static constexpr short d_LEY[2] = { 1, MarketId::XXXX };
static constexpr short d_LFI[2] = { 1, MarketId::XXXX };
static constexpr short d_LFT[2] = { 1, MarketId::XXXX };
static constexpr short d_LFW[2] = { 1, MarketId::TGO0 };
static constexpr short d_LGB[2] = { 1, MarketId::XXXX };
static constexpr short d_LGG[2] = { 1, MarketId::XXXX };
static constexpr short d_LGK[2] = { 1, MarketId::XXXX };
static constexpr short d_LHE[2] = { 1, MarketId::XLAH };
static constexpr short d_LI0[7] = { 6, MarketId::ARTX, MarketId::LIE0, MarketId::XLGT, MarketId::XLLB, MarketId::XNOM, MarketId::XVPB };
static constexpr short d_LIF[2] = { 1, MarketId::XXXX };
static constexpr short d_LIG[2] = { 1, MarketId::XXXX };
static constexpr short d_LIH[2] = { 1, MarketId::XXXX };
static constexpr short d_LIL[2] = { 1, MarketId::XXXX };
static constexpr short d_LIM[3] = { 2, MarketId::PER0, MarketId::XLIM };
static constexpr short d_LIQ[2] = { 1, MarketId::XXXX };
static constexpr short d_LIS[14] = { 13, MarketId::ALXL, MarketId::BFPT, MarketId::DLIS, MarketId::ENXL, MarketId::MDIP, MarketId::MFOX, MarketId::OMIC, MarketId::OMIP, MarketId::OPEX, MarketId::PMTS, MarketId::PRT0, MarketId::WQXL, MarketId::XLIS };
static constexpr short d_LIT[2] = { 1, MarketId::XXXX };
static constexpr short d_LJU[7] = { 6, MarketId::SKBB, MarketId::SVN0, MarketId::XLJM, MarketId::XLJS, MarketId::XLJU, MarketId::XSOP };
static constexpr short d_LKL[2] = { 1, MarketId::XXXX };
static constexpr short d_LKO[2] = { 1, MarketId::XXXX };
static constexpr short d_LL0[2] = { 1, MarketId::XXXX };
static constexpr short d_LLA[2] = { 1, MarketId::XXXX };
static constexpr short d_LLW[2] = { 1, MarketId::MWI0 };
static constexpr short d_LMM[2] = { 1, MarketId::XXXX };
static constexpr short d_LMP[2] = { 1, MarketId::XXXX };
static constexpr short d_LMT[2] = { 1, MarketId::XXXX };
static constexpr short d_LNK[2] = { 1, MarketId::XXXX };
static constexpr short d_LNO[2] = { 1, MarketId::XXXX };
static constexpr short d_LNS[2] = { 1, MarketId::XXXX };
static constexpr short d_LNY[2] = { 1, MarketId::XXXX };
static constexpr short d_LNZ[3] = { 2, MarketId::OBKL, MarketId::RLBO };
static constexpr short d_LON[575] = { 574, MarketId::_3579, MarketId::ACCX, MarketId::AFDL, MarketId::AFTS, MarketId::AIMX, MarketId::ALGO, MarketId::AMPX, MarketId::AMTS, MarketId::ANLP, MarketId::ANTS, MarketId::ANZL, MarketId::AQSD, MarketId::AQSE, MarketId::AQSF, MarketId::AQSG, MarketId::AQSL, MarketId::AQSN, MarketId::AQST, MarketId::AQXA, MarketId::AQXD, MarketId::AQXE, MarketId::ARAX, MarketId::ARCH, MarketId::ARDA, MarketId::ARIA, MarketId::ATLB, MarketId::AUTB, MarketId::AUTP, MarketId::AUTX, MarketId::BAIK, MarketId::BALT, MarketId::BANA, MarketId::BAPA, MarketId::BARK, MarketId::BARO, MarketId::BART, MarketId::BASI, MarketId::BATD, MarketId::BATE, MarketId::BATF, MarketId::BATP, MarketId::BBSI, MarketId::BBSX, MarketId::BBVX, MarketId::BCRM, MarketId::BCSI, MarketId::BCSL, MarketId::BCXE, MarketId::BETX, MarketId::BGCB, MarketId::BGCI, MarketId::BGCM, MarketId::BGCO, MarketId::BGFU, MarketId::BGUK, MarketId::BISI, MarketId::BKLF, MarketId::BKLN, MarketId::BLNK, MarketId::BLOX, MarketId::BLTX, MarketId::BMCM, MarketId::BMLB, MarketId::BMTF, MarketId::BNPL, MarketId::BNTW, MarketId::BOAL, MarketId::BOAT, MarketId::BOSC, MarketId::BOTC, MarketId::BPLC, MarketId::BRFQ, MarketId::BRGA, MarketId::BRNX, MarketId::BSLB, MarketId::BSPL, MarketId::BTEE, MarketId::BTLX, MarketId::BTQG, MarketId::BUYN, MarketId::BVUK, MarketId::CAZE, MarketId::CBAL, MarketId::CBNL, MarketId::CCEU, MarketId::CCML, MarketId::CCO2, MarketId::CEPU, MarketId::CFIC, MarketId::CGMC, MarketId::CGME, MarketId::CGMG, MarketId::CGML, MarketId::CGMT, MarketId::CGMU, MarketId::CHEV, MarketId::CHID, MarketId::CHIO, MarketId::CHIX, MarketId::CHIY, MarketId::CIBC, MarketId::CIBP, MarketId::CLCH, MarketId::CLVE, MarketId::CMCM, MarketId::CMEC, MarketId::CMED, MarketId::CMEE, MarketId::CMMT, MarketId::CMTS, MarketId::CRDL, MarketId::CRYP, MarketId::CSBX, MarketId::CSCF, MarketId::CSEC, MarketId::CSEU, MarketId::CSFB, MarketId::CSIN, MarketId::CSLB, MarketId::CSSI, MarketId::CXOT, MarketId::CXRT, MarketId::DAIW, MarketId::DAUK, MarketId::DBCR, MarketId::DBCX, MarketId::DBDC, MarketId::DBIX, MarketId::DBSE, MarketId::DBVX, MarketId::DOWG, MarketId::DRSP, MarketId::DVFX, MarketId::EACM, MarketId::EBSM, MarketId::EBSO, MarketId::EBSX, MarketId::ECHO, MarketId::ECNL, MarketId::ECSL, MarketId::EIXE, MarketId::ELIX, MarketId::EMBX, MarketId::EMCH, MarketId::EMTS, MarketId::ENCL, MarketId::ENSY, MarketId::EQLD, MarketId::EQSL, MarketId::EVOL, MarketId::EXBO, MarketId::EXCP, MarketId::EXDC, MarketId::EXEU, MarketId::EXLP, MarketId::EXMP, MarketId::EXOR, MarketId::EXOT, MarketId::EXSI, MarketId::EXVP, MarketId::FAIR, MarketId::FISU, MarketId::FNUK, MarketId::FRTE, MarketId::FXGB, MarketId::FXMT, MarketId::FXOP, MarketId::G360, MarketId::GBR0, MarketId::GEMX, MarketId::GFBM, MarketId::GFBO, MarketId::GFIA, MarketId::GFIB, MarketId::GFIC, MarketId::GFIF, MarketId::GFIM, MarketId::GFIN, MarketId::GFIR, MarketId::GFOX, MarketId::GFSM, MarketId::GFSO, MarketId::GMEG, MarketId::GMGL, MarketId::GMTS, MarketId::GRIF, MarketId::GRIO, MarketId::GRSE, MarketId::GSBX, MarketId::GSIB, MarketId::GSIL, MarketId::GSLO, MarketId::GSSI, MarketId::HPSO, MarketId::HPSX, MarketId::HRSI, MarketId::HSBC, MarketId::HSXE, MarketId::HUNG, MarketId::IBAL, MarketId::ICAH, MarketId::ICAP, MarketId::ICAT, MarketId::ICEN, MarketId::ICEO, MarketId::ICEU, MarketId::ICPM, MarketId::ICSE, MarketId::ICTQ, MarketId::IECE, MarketId::IECL, MarketId::IFEN, MarketId::IFEU, MarketId::IFLL, MarketId::IFLO, MarketId::IFLS, MarketId::IFLX, MarketId::IFUT, MarketId::IGDL, MarketId::ILCM, MarketId::IMCD, MarketId::IMCE, MarketId::IMCM, MarketId::IMED, MarketId::IMET, MarketId::IMFD, MarketId::IMGB, MarketId::IMGI, MarketId::IMMM, MarketId::IMRD, MarketId::IMSB, MarketId::IMTF, MarketId::IMTS, MarketId::INGU, MarketId::INVE, MarketId::IOCD, MarketId::IOED, MarketId::IOFB, MarketId::IOFI, MarketId::IOFX, MarketId::IOGB, MarketId::IOGI, MarketId::IOIR, MarketId::IOMM, MarketId::IOTC, MarketId::IOTF, MarketId::IPSX, MarketId::IPXP, MarketId::IPXW, MarketId::ISDX, MarketId::ISSI, MarketId::ISWA, MarketId::ISWB, MarketId::ISWC, MarketId::ISWE, MarketId::ISWR, MarketId::ISWV, MarketId::IUOB, MarketId::JEFS, MarketId::JISI, MarketId::JLSI, MarketId::JPCB, MarketId::JPJX, MarketId::JPSI, MarketId::JSSI, MarketId::KBLM, MarketId::KLEU, MarketId::KMTS, MarketId::LBCM, MarketId::LCUR, MarketId::LELE, MarketId::LIQF, MarketId::LIQH, MarketId::LIQU, MarketId::LISX, MarketId::LMAD, MarketId::LMAE, MarketId::LMAF, MarketId::LMAO, MarketId::LMAX, MarketId::LMEC, MarketId::LMTS, MarketId::LOTC, MarketId::LOUI, MarketId::LOYD, MarketId::LPPM, MarketId::MAEL, MarketId::MAKX, MarketId::MAQI, MarketId::MAQL, MarketId::MAQU, MarketId::MBSI, MarketId::MCUR, MarketId::MCXR, MarketId::MCXS, MarketId::MCZK, MarketId::METZ, MarketId::MFGL, MarketId::MFXA, MarketId::MFXC, MarketId::MFXR, MarketId::MHBL, MarketId::MHIP, MarketId::MLAX, MarketId::MLEU, MarketId::MLIB, MarketId::MLIX, MarketId::MLRQ, MarketId::MLSI, MarketId::MLVE, MarketId::MLXN, MarketId::MSBI, MarketId::MSIP, MarketId::MSSI, MarketId::MTSA, MarketId::MTSG, MarketId::MTSS, MarketId::MTSW, MarketId::MUBL, MarketId::MUFP, MarketId::MUSE, MarketId::MUTI, MarketId::MXLM, MarketId::MYTR, MarketId::N2EX, MarketId::NABA, MarketId::NABE, MarketId::NABL, MarketId::NABU, MarketId::NAVE, MarketId::NBFL, MarketId::NCML, MarketId::NDCM, MarketId::NEXD, MarketId::NEXF, MarketId::NEXG, MarketId::NEXL, MarketId::NEXN, MarketId::NEXS, MarketId::NEXT, MarketId::NEXX, MarketId::NMTS, MarketId::NOFF, MarketId::NOME, MarketId::NOSI, MarketId::NTUK, MarketId::NURD, MarketId::NURO, MarketId::NXEU, MarketId::NYMX, MarketId::OCSI, MarketId::OCXL, MarketId::OFEX, MarketId::OILX, MarketId::OPTX, MarketId::OTCE, MarketId::OTXT, MarketId::PARX, MarketId::PCDS, MarketId::PEEL, MarketId::PFXD, MarketId::PHSI, MarketId::PIEU, MarketId::PIRM, MarketId::PLDX, MarketId::PLSX, MarketId::PORT, MarketId::PPEX, MarketId::PRME, MarketId::PVMF, MarketId::QMTS, MarketId::QWIX, MarketId::R5FX, MarketId::RABL, MarketId::RBCE, MarketId::RBCM, MarketId::RBCT, MarketId::RBSX, MarketId::REST, MarketId::RFQU, MarketId::RMTS, MarketId::RTSI, MarketId::RTSL, MarketId::SBEX, MarketId::SECF, MarketId::SEDR, MarketId::SFCL, MarketId::SFMP, MarketId::SGMX, MarketId::SGMY, MarketId::SGMZ, MarketId::SICS, MarketId::SIFX, MarketId::SISU, MarketId::SKYX, MarketId::SLKK, MarketId::SLXT, MarketId::SMBE, MarketId::SMBG, MarketId::SMTS, MarketId::SNUK, MarketId::SPDX, MarketId::SPEC, MarketId::SPRZ, MarketId::SQUA, MarketId::SSBT, MarketId::SSEX, MarketId::SSFX, MarketId::SSIL, MarketId::STAL, MarketId::STAN, MarketId::STFL, MarketId::STSI, MarketId::SUNB, MarketId::SUNM, MarketId::SUNO, MarketId::SWAP, MarketId::TBEN, MarketId::TBLA, MarketId::TCDS, MarketId::TCME, MarketId::TCML, MarketId::TDBL, MarketId::TECO, MarketId::TEEG, MarketId::TEFD, MarketId::TEFX, MarketId::TEGB, MarketId::TEGI, MarketId::TEIR, MarketId::TEMB, MarketId::TEMC, MarketId::TEMF, MarketId::TEMG, MarketId::TEMI, MarketId::TEMM, MarketId::TEMR, MarketId::TEOF, MarketId::TERE, MarketId::TFSC, MarketId::TFSE, MarketId::TFSG, MarketId::TFSS, MarketId::TFSV, MarketId::TIRD, MarketId::TMTS, MarketId::TMUK, MarketId::TNLL, MarketId::TPCD, MarketId::TPDA, MarketId::TPEL, MarketId::TPEO, MarketId::TPEQ, MarketId::TPFD, MarketId::TPID, MarketId::TPIE, MarketId::TPIM, MarketId::TPIS, MarketId::TPLF, MarketId::TPMF, MarketId::TPMG, MarketId::TPRE, MarketId::TPSD, MarketId::TPSL, MarketId::TPSO, MarketId::TPSP, MarketId::TPSY, MarketId::TRAL, MarketId::TRAX, MarketId::TRDE, MarketId::TRDX, MarketId::TREA, MarketId::TREO, MarketId::TREU, MarketId::TRFW, MarketId::TRQA, MarketId::TRQB, MarketId::TRQC, MarketId::TRQD, MarketId::TRQM, MarketId::TRQS, MarketId::TRQX, MarketId::TRSI, MarketId::TRUK, MarketId::TSCB, MarketId::TSCD, MarketId::TSED, MarketId::TSFI, MarketId::TSFX, MarketId::TSGB, MarketId::TSGI, MarketId::TSIR, MarketId::TSMB, MarketId::TSMC, MarketId::TSMG, MarketId::TSMI, MarketId::TSMM, MarketId::TSMR, MarketId::TSRE, MarketId::TSUK, MarketId::TUOB, MarketId::TWGP, MarketId::UBIN, MarketId::UBSB, MarketId::UBSE, MarketId::UBSY, MarketId::UKCA, MarketId::UKGD, MarketId::UKOR, MarketId::UKPX, MarketId::UKRE, MarketId::UMTS, MarketId::UNGB, MarketId::USWP, MarketId::VAGL, MarketId::VAGM, MarketId::VCMO, MarketId::VEGA, MarketId::VFGB, MarketId::VFUK, MarketId::VIUK, MarketId::VMTS, MarketId::VOLA, MarketId::VTBC, MarketId::WCLK, MarketId::WELN, MarketId::WFLB, MarketId::WINS, MarketId::WINX, MarketId::WSIL, MarketId::WSIN, MarketId::XALT, MarketId::XCOR, MarketId::XEBS, MarketId::XEDX, MarketId::XGCL, MarketId::XGCX, MarketId::XGDX, MarketId::XGFI, MarketId::XGSX, MarketId::XIEL, MarketId::XIPE, MarketId::XJWY, MarketId::XLBM, MarketId::XLCE, MarketId::XLCH, MarketId::XLDN, MarketId::XLDX, MarketId::XLIF, MarketId::XLME, MarketId::XLOD, MarketId::XLOM, MarketId::XLON, MarketId::XLTO, MarketId::XMLX, MarketId::XMTS, MarketId::XNLX, MarketId::XPAL, MarketId::XPHX, MarketId::XPLU, MarketId::XPOL, MarketId::XPUK, MarketId::XRSP, MarketId::XSGB, MarketId::XSMP, MarketId::XSWB, MarketId::XTFN, MarketId::XTPE, MarketId::XTUP, MarketId::XTXM, MarketId::XUBS, MarketId::XUMP, MarketId::XWAP, MarketId::ZODM };
static constexpr short d_LOS[3] = { 2, MarketId::NASX, MarketId::XNSA };
static constexpr short d_LPA[2] = { 1, MarketId::XXXX };
static constexpr short d_LPB[3] = { 2, MarketId::BOL0, MarketId::XBOL };
static constexpr short d_LPL[2] = { 1, MarketId::XXXX };
static constexpr short d_LPP[2] = { 1, MarketId::XXXX };
static constexpr short d_LRD[2] = { 1, MarketId::XXXX };
static constexpr short d_LRE[2] = { 1, MarketId::XXXX };
static constexpr short d_LRF[2] = { 1, MarketId::XXXX };
static constexpr short d_LRH[2] = { 1, MarketId::XXXX };
static constexpr short d_LRM[2] = { 1, MarketId::XXXX };
static constexpr short d_LRT[2] = { 1, MarketId::XXXX };
static constexpr short d_LSE[2] = { 1, MarketId::XXXX };
static constexpr short d_LSI[2] = { 1, MarketId::XXXX };
static constexpr short d_LST[2] = { 1, MarketId::XXXX };
static constexpr short d_LSY[2] = { 1, MarketId::XXXX };
static constexpr short d_LSZ[2] = { 1, MarketId::XXXX };
static constexpr short d_LTO[2] = { 1, MarketId::XXXX };
static constexpr short d_LU0[2] = { 1, MarketId::S3FM };
static constexpr short d_LUD[2] = { 1, MarketId::XXXX };
static constexpr short d_LUG[2] = { 1, MarketId::XXXX };
static constexpr short d_LUJ[2] = { 1, MarketId::XXXX };
static constexpr short d_LUM[2] = { 1, MarketId::XXXX };
static constexpr short d_LUN[3] = { 2, MarketId::XLUS, MarketId::ZMB0 };
static constexpr short d_LUQ[2] = { 1, MarketId::XXXX };
static constexpr short d_LUX[18] = { 17, MarketId::BCEE, MarketId::BDPL, MarketId::BGLU, MarketId::BILU, MarketId::BLUX, MarketId::CCLX, MarketId::DBLX, MarketId::DHLX, MarketId::EMTF, MarketId::KBLC, MarketId::KBLL, MarketId::KBLS, MarketId::KBLT, MarketId::LUX0, MarketId::MIBL, MarketId::XLUX, MarketId::XVES };
static constexpr short d_LV0[2] = { 1, MarketId::XEAS };
static constexpr short d_LVO[2] = { 1, MarketId::XXXX };
static constexpr short d_LWB[2] = { 1, MarketId::XXXX };
static constexpr short d_LWK[2] = { 1, MarketId::XXXX };
static constexpr short d_LWO[2] = { 1, MarketId::XXXX };
static constexpr short d_LWS[2] = { 1, MarketId::XXXX };
static constexpr short d_LWT[2] = { 1, MarketId::XXXX };
static constexpr short d_LXR[2] = { 1, MarketId::XXXX };
static constexpr short d_LYH[2] = { 1, MarketId::XXXX };
static constexpr short d_LYO[2] = { 1, MarketId::XXXX };
static constexpr short d_LYP[2] = { 1, MarketId::XXXX };
static constexpr short d_LYR[2] = { 1, MarketId::SJM0 };
static constexpr short d_LYS[2] = { 1, MarketId::XXXX };
static constexpr short d_LYX[2] = { 1, MarketId::XXXX };
static constexpr short d_LZC[2] = { 1, MarketId::XXXX };
static constexpr short d_MAA[2] = { 1, MarketId::XMDS };
static constexpr short d_MAD[45] = { 44, MarketId::ABAN, MarketId::AGBP, MarketId::ALLT, MarketId::BBVA, MarketId::BMCL, MarketId::BMEA, MarketId::BMEX, MarketId::CAPI, MarketId::CECA, MarketId::CIMA, MarketId::CIMB, MarketId::CIMD, MarketId::CIME, MarketId::CIMV, MarketId::CMAP, MarketId::CSMD, MarketId::DMAD, MarketId::DOWE, MarketId::DOWM, MarketId::ESP0, MarketId::GROW, MarketId::MABX, MarketId::MARF, MarketId::MDRV, MarketId::MERF, MarketId::MIBG, MarketId::OMEL, MarketId::POSE, MarketId::SCLE, MarketId::SEND, MarketId::SIMD, MarketId::TEUR, MarketId::TOMF, MarketId::TOMG, MarketId::TPES, MarketId::XDPA, MarketId::XDRF, MarketId::XLAT, MarketId::XMAD, MarketId::XMCE, MarketId::XMFX, MarketId::XMPW, MarketId::XMRV, MarketId::XNAF };
static constexpr short d_MAF[2] = { 1, MarketId::XXXX };
static constexpr short d_MAH[2] = { 1, MarketId::XXXX };
static constexpr short d_MAJ[2] = { 1, MarketId::MHL0 };
static constexpr short d_MAM[2] = { 1, MarketId::XXXX };
static constexpr short d_MAN[2] = { 1, MarketId::XXXX };
static constexpr short d_MAO[2] = { 1, MarketId::XXXX };
static constexpr short d_MAR[2] = { 1, MarketId::XXXX };
static constexpr short d_MAU[2] = { 1, MarketId::XXXX };
static constexpr short d_MAZ[2] = { 1, MarketId::XXXX };
static constexpr short d_MBA[2] = { 1, MarketId::XXXX };
static constexpr short d_MBH[2] = { 1, MarketId::XXXX };
static constexpr short d_MBJ[2] = { 1, MarketId::XXXX };
static constexpr short d_MBM[2] = { 1, MarketId::XXXX };
static constexpr short d_MBS[2] = { 1, MarketId::XXXX };
static constexpr short d_MBX[2] = { 1, MarketId::XXXX };
static constexpr short d_MC0[2] = { 1, MarketId::XXXX };
static constexpr short d_MCE[2] = { 1, MarketId::XXXX };
static constexpr short d_MCI[3] = { 2, MarketId::EDGE, MarketId::XKBT };
static constexpr short d_MCL[2] = { 1, MarketId::XXXX };
static constexpr short d_MCM[2] = { 1, MarketId::MCO0 };
static constexpr short d_MCN[2] = { 1, MarketId::XXXX };
static constexpr short d_MCO[2] = { 1, MarketId::XXXX };
static constexpr short d_MCP[2] = { 1, MarketId::XXXX };
static constexpr short d_MCT[5] = { 4, MarketId::MSXB, MarketId::MSXO, MarketId::OMN0, MarketId::XMUS };
static constexpr short d_MCW[2] = { 1, MarketId::XXXX };
static constexpr short d_MCY[2] = { 1, MarketId::XXXX };
static constexpr short d_MCZ[2] = { 1, MarketId::XXXX };
static constexpr short d_MDC[2] = { 1, MarketId::XXXX };
static constexpr short d_MDE[2] = { 1, MarketId::XXXX };
static constexpr short d_MDL[2] = { 1, MarketId::XXXX };
static constexpr short d_MDQ[2] = { 1, MarketId::XXXX };
static constexpr short d_MDY[2] = { 1, MarketId::XXXX };
static constexpr short d_MDZ[3] = { 2, MarketId::XBCM, MarketId::XBCX };
static constexpr short d_MED[2] = { 1, MarketId::XXXX };
static constexpr short d_MEE[2] = { 1, MarketId::XXXX };
static constexpr short d_MEI[2] = { 1, MarketId::XXXX };
static constexpr short d_MEL[4] = { 3, MarketId::AWBX, MarketId::BLEV, MarketId::NSXB };
static constexpr short d_MEM[2] = { 1, MarketId::XXXX };
static constexpr short d_MES[2] = { 1, MarketId::XXXX };
static constexpr short d_MEX[6] = { 5, MarketId::BIVA, MarketId::CGMX, MarketId::MEX0, MarketId::XEMD, MarketId::XMEX };
static constexpr short d_MEZ[2] = { 1, MarketId::XXXX };
static constexpr short d_MFE[2] = { 1, MarketId::XXXX };
static constexpr short d_MFF[2] = { 1, MarketId::XXXX };
static constexpr short d_MFG[2] = { 1, MarketId::XXXX };
static constexpr short d_MFM[2] = { 1, MarketId::MAC0 };
static constexpr short d_MFN[2] = { 1, MarketId::XXXX };
static constexpr short d_MFO[2] = { 1, MarketId::XXXX };
static constexpr short d_MFQ[2] = { 1, MarketId::XXXX };
static constexpr short d_MFR[2] = { 1, MarketId::XXXX };
static constexpr short d_MFU[2] = { 1, MarketId::XXXX };
static constexpr short d_MGA[3] = { 2, MarketId::NIC0, MarketId::XMAN };
static constexpr short d_MGB[2] = { 1, MarketId::XXXX };
static constexpr short d_MGF[2] = { 1, MarketId::BCMM };
static constexpr short d_MGH[2] = { 1, MarketId::XXXX };
static constexpr short d_MGM[2] = { 1, MarketId::XXXX };
static constexpr short d_MGQ[2] = { 1, MarketId::SOM0 };
static constexpr short d_MGW[2] = { 1, MarketId::XXXX };
static constexpr short d_MHE[2] = { 1, MarketId::XXXX };
static constexpr short d_MHH[2] = { 1, MarketId::XXXX };
static constexpr short d_MHQ[2] = { 1, MarketId::ALA0 };
static constexpr short d_MHT[2] = { 1, MarketId::XXXX };
static constexpr short d_MIA[2] = { 1, MarketId::FUSD };
static constexpr short d_MID[2] = { 1, MarketId::XXXX };
static constexpr short d_MIK[2] = { 1, MarketId::XXXX };
static constexpr short d_MIL[36] = { 35, MarketId::AKIS, MarketId::ATFX, MarketId::BGEM, MarketId::DMIL, MarketId::EMDR, MarketId::EMIB, MarketId::EMID, MarketId::EMIR, MarketId::ETFP, MarketId::ETLX, MarketId::EXGM, MarketId::EXPA, MarketId::FBSI, MarketId::HCER, MarketId::HMOD, MarketId::HMTF, MarketId::HRFQ, MarketId::IMMH, MarketId::ITSM, MarketId::MACX, MarketId::MIVX, MarketId::MOTX, MarketId::MTAA, MarketId::MTAH, MarketId::MTAX, MarketId::MUBM, MarketId::SEDX, MarketId::TLAB, MarketId::UCIT, MarketId::XAIM, MarketId::XDMI, MarketId::XMIF, MarketId::XMIL, MarketId::XMOT, MarketId::XTLX };
static constexpr short d_MIM[2] = { 1, MarketId::XXXX };
static constexpr short d_MIR[2] = { 1, MarketId::XXXX };
static constexpr short d_MJC[2] = { 1, MarketId::XXXX };
static constexpr short d_MJD[2] = { 1, MarketId::XXXX };
static constexpr short d_MJL[2] = { 1, MarketId::XXXX };
static constexpr short d_MJN[2] = { 1, MarketId::XXXX };
static constexpr short d_MJQ[2] = { 1, MarketId::XXXX };
static constexpr short d_MJT[2] = { 1, MarketId::XXXX };
static constexpr short d_MJV[2] = { 1, MarketId::XXXX };
static constexpr short d_MKC[2] = { 1, MarketId::PDEX };
static constexpr short d_MKE[2] = { 1, MarketId::XXXX };
static constexpr short d_MKG[2] = { 1, MarketId::XXXX };
static constexpr short d_MKK[2] = { 1, MarketId::XXXX };
static constexpr short d_MKL[2] = { 1, MarketId::XXXX };
static constexpr short d_MKR[2] = { 1, MarketId::XXXX };
static constexpr short d_MKY[2] = { 1, MarketId::XXXX };
static constexpr short d_MLA[7] = { 6, MarketId::COMG, MarketId::EWSM, MarketId::IFSM, MarketId::MLT0, MarketId::PROS, MarketId::XMAL };
static constexpr short d_MLB[2] = { 1, MarketId::XXXX };
static constexpr short d_MLE[3] = { 2, MarketId::MALX, MarketId::MDV0 };
static constexpr short d_MLH[2] = { 1, MarketId::XXXX };
static constexpr short d_MLI[2] = { 1, MarketId::XXXX };
static constexpr short d_MLM[2] = { 1, MarketId::XXXX };
static constexpr short d_MLS[2] = { 1, MarketId::XXXX };
static constexpr short d_MLU[2] = { 1, MarketId::XXXX };
static constexpr short d_MLW[2] = { 1, MarketId::LBR0 };
static constexpr short d_MLX[2] = { 1, MarketId::XXXX };
static constexpr short d_MMA[2] = { 1, MarketId::XXXX };
static constexpr short d_MME[2] = { 1, MarketId::XXXX };
static constexpr short d_MMG[2] = { 1, MarketId::XXXX };
static constexpr short d_MMJ[2] = { 1, MarketId::XXXX };
static constexpr short d_MMK[2] = { 1, MarketId::XXXX };
static constexpr short d_MMM[2] = { 1, MarketId::XXXX };
static constexpr short d_MMY[2] = { 1, MarketId::XXXX };
static constexpr short d_MNI[2] = { 1, MarketId::MSR0 };
static constexpr short d_MNL[3] = { 2, MarketId::CLPH, MarketId::PHL0 };
static constexpr short d_MOB[2] = { 1, MarketId::XXXX };
static constexpr short d_MOD[2] = { 1, MarketId::XXXX };
static constexpr short d_MON[2] = { 1, MarketId::XXXX };
static constexpr short d_MOT[2] = { 1, MarketId::XXXX };
static constexpr short d_MOV[2] = { 1, MarketId::XXXX };
static constexpr short d_MOW[11] = { 10, MarketId::MISX, MarketId::NAMX, MarketId::RPDX, MarketId::RTSX, MarketId::RUS0, MarketId::RUSX, MarketId::SPBE, MarketId::XMIC, MarketId::XMOS, MarketId::XRUS };
static constexpr short d_MOZ[2] = { 1, MarketId::XXXX };
static constexpr short d_MPA[2] = { 1, MarketId::XXXX };
static constexpr short d_MPD[2] = { 1, MarketId::XXXX };
static constexpr short d_MPL[2] = { 1, MarketId::XXXX };
static constexpr short d_MPM[4] = { 3, MarketId::MOZ0, MarketId::XBVM, MarketId::XMAP };
static constexpr short d_MPV[2] = { 1, MarketId::XXXX };
static constexpr short d_MQL[2] = { 1, MarketId::XXXX };
static constexpr short d_MQM[2] = { 1, MarketId::XXXX };
static constexpr short d_MQQ[2] = { 1, MarketId::XXXX };
static constexpr short d_MQT[2] = { 1, MarketId::XXXX };
static constexpr short d_MRB[2] = { 1, MarketId::XXXX };
static constexpr short d_MRS[2] = { 1, MarketId::XXXX };
static constexpr short d_MRU[3] = { 2, MarketId::MUS0, MarketId::XMAU };
static constexpr short d_MRV[2] = { 1, MarketId::XXXX };
static constexpr short d_MRY[2] = { 1, MarketId::XXXX };
static constexpr short d_MRZ[2] = { 1, MarketId::XXXX };
static constexpr short d_MSE[2] = { 1, MarketId::XXXX };
static constexpr short d_MSL[2] = { 1, MarketId::XXXX };
static constexpr short d_MSN[2] = { 1, MarketId::XXXX };
static constexpr short d_MSO[2] = { 1, MarketId::XXXX };
static constexpr short d_MSP[2] = { 1, MarketId::XMGE };
static constexpr short d_MSQ[3] = { 2, MarketId::BCSE, MarketId::BLR0 };
static constexpr short d_MSR[2] = { 1, MarketId::XXXX };
static constexpr short d_MST[2] = { 1, MarketId::XXXX };
static constexpr short d_MSU[2] = { 1, MarketId::LSO0 };
static constexpr short d_MSY[2] = { 1, MarketId::XXXX };
static constexpr short d_MTH[2] = { 1, MarketId::XXXX };
static constexpr short d_MTJ[2] = { 1, MarketId::XXXX };
static constexpr short d_MTL[2] = { 1, MarketId::XXXX };
static constexpr short d_MTM[2] = { 1, MarketId::XXXX };
static constexpr short d_MTO[2] = { 1, MarketId::XXXX };
static constexpr short d_MTS[2] = { 1, MarketId::XXXX };
static constexpr short d_MTT[2] = { 1, MarketId::XXXX };
static constexpr short d_MTY[2] = { 1, MarketId::XXXX };
static constexpr short d_MUB[2] = { 1, MarketId::XXXX };
static constexpr short d_MUC[20] = { 19, MarketId::BAAD, MarketId::BDEA, MarketId::BLBB, MarketId::BLBS, MarketId::BLEQ, MarketId::BLFX, MarketId::BLIQ, MarketId::BSFX, MarketId::GMEX, MarketId::MUNA, MarketId::MUNB, MarketId::MUNC, MarketId::MUND, MarketId::PLUS, MarketId::SCLB, MarketId::SSBI, MarketId::SSBM, MarketId::UCDE, MarketId::XMUN };
static constexpr short d_MUE[2] = { 1, MarketId::XXXX };
static constexpr short d_MUH[2] = { 1, MarketId::XXXX };
static constexpr short d_MUX[2] = { 1, MarketId::XXXX };
static constexpr short d_MV0[2] = { 1, MarketId::XXXX };
static constexpr short d_MVB[2] = { 1, MarketId::XXXX };
static constexpr short d_MVD[5] = { 4, MarketId::BVUR, MarketId::UFEX, MarketId::URY0, MarketId::XMNT };
static constexpr short d_MVR[2] = { 1, MarketId::XXXX };
static constexpr short d_MVY[2] = { 1, MarketId::XXXX };
static constexpr short d_MVZ[2] = { 1, MarketId::XXXX };
static constexpr short d_MWD[2] = { 1, MarketId::XXXX };
static constexpr short d_MWH[2] = { 1, MarketId::XXXX };
static constexpr short d_MXL[2] = { 1, MarketId::XXXX };
static constexpr short d_MYA[2] = { 1, MarketId::XXXX };
static constexpr short d_MYD[2] = { 1, MarketId::XXXX };
static constexpr short d_MYJ[2] = { 1, MarketId::XXXX };
static constexpr short d_MYQ[2] = { 1, MarketId::XXXX };
static constexpr short d_MYR[2] = { 1, MarketId::XXXX };
static constexpr short d_MYY[2] = { 1, MarketId::XXXX };
static constexpr short d_MZG[2] = { 1, MarketId::XXXX };
static constexpr short d_MZM[2] = { 1, MarketId::XXXX };
static constexpr short d_MZR[2] = { 1, MarketId::XXXX };
static constexpr short d_MZT[2] = { 1, MarketId::XXXX };
static constexpr short d_MZY[2] = { 1, MarketId::XXXX };
static constexpr short d_NAA[2] = { 1, MarketId::XXXX };
static constexpr short d_NAG[2] = { 1, MarketId::XXXX };
static constexpr short d_NAJ[2] = { 1, MarketId::XXXX };
static constexpr short d_NAN[2] = { 1, MarketId::XXXX };
static constexpr short d_NAP[2] = { 1, MarketId::XXXX };
static constexpr short d_NAS[3] = { 2, MarketId::BHS0, MarketId::XBAA };
static constexpr short d_NAT[2] = { 1, MarketId::XXXX };
static constexpr short d_NBO[3] = { 2, MarketId::KEN0, MarketId::XNAI };
static constexpr short d_NCE[2] = { 1, MarketId::XXXX };
static constexpr short d_NCL[2] = { 1, MarketId::XXXX };
static constexpr short d_NCS[2] = { 1, MarketId::XXXX };
static constexpr short d_NCY[2] = { 1, MarketId::XXXX };
static constexpr short d_NDB[2] = { 1, MarketId::XXXX };
static constexpr short d_NDJ[2] = { 1, MarketId::TCD0 };
static constexpr short d_NDU[2] = { 1, MarketId::XXXX };
static constexpr short d_NEV[2] = { 1, MarketId::XXXX };
static constexpr short d_NGE[2] = { 1, MarketId::XXXX };
static constexpr short d_NGO[6] = { 5, MarketId::XCCE, MarketId::XNGO, MarketId::XNKS, MarketId::XNST, MarketId::XTKA };
static constexpr short d_NGS[2] = { 1, MarketId::XXXX };
static constexpr short d_NIC[10] = { 9, MarketId::ATHL, MarketId::ATLN, MarketId::CYP0, MarketId::RENC, MarketId::SIBC, MarketId::XCYO, MarketId::XCYS, MarketId::XECM, MarketId::XMME };
static constexpr short d_NIM[2] = { 1, MarketId::NER0 };
static constexpr short d_NKC[2] = { 1, MarketId::MRT0 };
static constexpr short d_NKW[2] = { 1, MarketId::IOT0 };
static constexpr short d_NLA[2] = { 1, MarketId::XXXX };
static constexpr short d_NLD[2] = { 1, MarketId::XXXX };
static constexpr short d_NLK[2] = { 1, MarketId::NFK0 };
static constexpr short d_NLP[2] = { 1, MarketId::XXXX };
static constexpr short d_NLV[2] = { 1, MarketId::XXXX };
static constexpr short d_NNG[2] = { 1, MarketId::XXXX };
static constexpr short d_NOC[2] = { 1, MarketId::XXXX };
static constexpr short d_NOU[2] = { 1, MarketId::NCL0 };
static constexpr short d_NQY[2] = { 1, MarketId::XXXX };
static constexpr short d_NRA[2] = { 1, MarketId::XXXX };
static constexpr short d_NRK[2] = { 1, MarketId::XXXX };
static constexpr short d_NS0[2] = { 1, MarketId::XXXX };
static constexpr short d_NSA[2] = { 1, MarketId::XXXX };
static constexpr short d_NSN[2] = { 1, MarketId::XXXX };
static constexpr short d_NSO[2] = { 1, MarketId::XXXX };
static constexpr short d_NST[2] = { 1, MarketId::XXXX };
static constexpr short d_NTE[2] = { 1, MarketId::XXXX };
static constexpr short d_NTL[3] = { 2, MarketId::NECD, MarketId::XNEC };
static constexpr short d_NTY[2] = { 1, MarketId::XXXX };
static constexpr short d_NUE[2] = { 1, MarketId::XXXX };
static constexpr short d_NWI[2] = { 1, MarketId::XXXX };
static constexpr short d_NYC[277] = { 276, MarketId::ACKF, MarketId::ADVT, MarketId::ALDP, MarketId::ALPX, MarketId::AMXO, MarketId::AQUA, MarketId::ARCD, MarketId::ARCO, MarketId::ARCX, MarketId::ARKX, MarketId::ATDF, MarketId::BAML, MarketId::BAMP, MarketId::BAMX, MarketId::BARD, MarketId::BARL, MarketId::BARX, MarketId::BBOK, MarketId::BBSF, MarketId::BBSN, MarketId::BCDX, MarketId::BGCD, MarketId::BGCF, MarketId::BHSF, MarketId::BIDS, MarketId::BLKX, MarketId::BLTD, MarketId::BLUE, MarketId::BNDD, MarketId::BNDS, MarketId::BNPC, MarketId::BNPH, MarketId::BNYC, MarketId::BOSD, MarketId::BPOL, MarketId::BTEQ, MarketId::BVUS, MarketId::CAES, MarketId::CALH, MarketId::CAVE, MarketId::CBLC, MarketId::CCMX, MarketId::CDED, MarketId::CDEL, MarketId::CECS, MarketId::CFIM, MarketId::CGMI, MarketId::CGXS, MarketId::CICX, MarketId::CIOI, MarketId::CISD, MarketId::CLST, MarketId::CORE, MarketId::CPGX, MarketId::CRED, MarketId::CSCL, MarketId::CSLP, MarketId::CSVW, MarketId::CURX, MarketId::DBAB, MarketId::DBSX, MarketId::DEAL, MarketId::DWIN, MarketId::DWSF, MarketId::EGMT, MarketId::EQUS, MarketId::ESPD, MarketId::EXPM, MarketId::FAST, MarketId::FINC, MarketId::FINN, MarketId::FNCS, MarketId::FNFT, MarketId::FNFX, MarketId::FNIX, MarketId::FNXB, MarketId::FSEF, MarketId::FTUS, MarketId::FXAL, MarketId::FXCM, MarketId::FXNM, MarketId::FXPS, MarketId::GFAM, MarketId::GLLC, MarketId::GLMX, MarketId::GMNI, MarketId::GSBS, MarketId::GSCO, MarketId::GSEF, MarketId::GTCO, MarketId::GTSX, MarketId::HRTF, MarketId::HRTX, MarketId::HSFX, MarketId::IBLX, MarketId::ICBX, MarketId::ICEL, MarketId::ICES, MarketId::ICRO, MarketId::ICUS, MarketId::IDXM, MarketId::IEXA, MarketId::IEXC, MarketId::IEXD, MarketId::IEXG, MarketId::IFED, MarketId::IFUS, MarketId::IIDX, MarketId::IMCC, MarketId::IMCG, MarketId::INCA, MarketId::ISDA, MarketId::ITGI, MarketId::JEFX, MarketId::JNST, MarketId::JPBX, MarketId::JPMS, MarketId::JPMX, MarketId::JSEF, MarketId::JSES, MarketId::JSJX, MarketId::KLSH, MarketId::LAFD, MarketId::LAFL, MarketId::LAFX, MarketId::LAKE, MarketId::LASF, MarketId::LATG, MarketId::LAVA, MarketId::LEDG, MarketId::LESI, MarketId::LIFI, MarketId::LIUH, MarketId::LIUS, MarketId::LPSF, MarketId::LQED, MarketId::LQFI, MarketId::MCRY, MarketId::MELO, MarketId::MKAA, MarketId::MLCO, MarketId::MLVX, MarketId::MOCX, MarketId::MSCO, MarketId::MSLC, MarketId::MSLP, MarketId::MSPL, MarketId::MSRP, MarketId::MSTC, MarketId::MSTX, MarketId::MTSB, MarketId::MTUS, MarketId::MTXA, MarketId::MTXC, MarketId::MTXM, MarketId::MTXS, MarketId::MTXX, MarketId::NASD, MarketId::NBLX, MarketId::NLAX, MarketId::NMRA, MarketId::NPMS, MarketId::NXUS, MarketId::NYFX, MarketId::NYMS, MarketId::NYPC, MarketId::NYSD, MarketId::OCFX, MarketId::OCTC, MarketId::OCTL, MarketId::OLLC, MarketId::ONEC, MarketId::OTCB, MarketId::OTCI, MarketId::OTCM, MarketId::OTCN, MarketId::OTCQ, MarketId::OYLD, MarketId::PINC, MarketId::PINI, MarketId::PINL, MarketId::PINX, MarketId::PIPE, MarketId::PRSE, MarketId::PSGM, MarketId::PSXD, MarketId::PULX, MarketId::PUMA, MarketId::PUMX, MarketId::PUND, MarketId::PVBL, MarketId::RAJA, MarketId::RBCS, MarketId::RCBX, MarketId::RCMA, MarketId::RTXF, MarketId::SGA2, MarketId::SGAS, MarketId::SGMA, MarketId::SGMT, MarketId::SHAD, MarketId::SHAW, MarketId::SOHO, MarketId::SPAX, MarketId::STFU, MarketId::STFX, MarketId::STRM, MarketId::TFSU, MarketId::THRE, MarketId::TMCC, MarketId::TMID, MarketId::TPSB, MarketId::TRCK, MarketId::TRCX, MarketId::TRU1, MarketId::TRU2, MarketId::TRUX, MarketId::TSAD, MarketId::TSEF, MarketId::TWSF, MarketId::U360, MarketId::UBSA, MarketId::UBSP, MarketId::UBSS, MarketId::USEF, MarketId::VABD, MarketId::VALX, MarketId::VCRS, MarketId::VERT, MarketId::VFCM, MarketId::VFMI, MarketId::VIRT, MarketId::VNDM, MarketId::VTEX, MarketId::VUSA, MarketId::WABR, MarketId::WELS, MarketId::WFLP, MarketId::WSAG, MarketId::XAQS, MarketId::XASE, MarketId::XBMK, MarketId::XBOS, MarketId::XBRT, MarketId::XBXO, MarketId::XCEC, MarketId::XCFF, MarketId::XCIS, MarketId::XCSC, MarketId::XCUR, MarketId::XELX, MarketId::XINS, MarketId::XISA, MarketId::XISE, MarketId::XISX, MarketId::XNAS, MarketId::XNCM, MarketId::XNDQ, MarketId::XNFI, MarketId::XNGS, MarketId::XNIM, MarketId::XNLI, MarketId::XNMS, MarketId::XNQL, MarketId::XNYC, MarketId::XNYE, MarketId::XNYF, MarketId::XNYL, MarketId::XNYM, MarketId::XNYS, MarketId::XPSX, MarketId::XPUS, MarketId::XSEF, MarketId::XTPZ, MarketId::XTRD, MarketId::XTXD };
static constexpr short d_OAG[2] = { 1, MarketId::XXXX };
static constexpr short d_OAJ[2] = { 1, MarketId::XXXX };
static constexpr short d_OAK[2] = { 1, MarketId::XXXX };
static constexpr short d_OAX[2] = { 1, MarketId::XXXX };
static constexpr short d_OB0[2] = { 1, MarketId::OLBB };
static constexpr short d_ODA[2] = { 1, MarketId::XXXX };
static constexpr short d_ODB[2] = { 1, MarketId::XXXX };
static constexpr short d_ODE[2] = { 1, MarketId::XXXX };
static constexpr short d_ODS[2] = { 1, MarketId::XODE };
static constexpr short d_OGG[2] = { 1, MarketId::XXXX };
static constexpr short d_OHD[2] = { 1, MarketId::XXXX };
static constexpr short d_OHT[2] = { 1, MarketId::XXXX };
static constexpr short d_OIT[2] = { 1, MarketId::XXXX };
static constexpr short d_OKA[2] = { 1, MarketId::XXXX };
static constexpr short d_OKC[2] = { 1, MarketId::XXXX };
static constexpr short d_OKD[2] = { 1, MarketId::XSAP };
static constexpr short d_OKJ[2] = { 1, MarketId::XXXX };
static constexpr short d_OKU[2] = { 1, MarketId::XXXX };
static constexpr short d_OLB[2] = { 1, MarketId::XXXX };
static constexpr short d_OLF[2] = { 1, MarketId::XXXX };
static constexpr short d_OLM[2] = { 1, MarketId::XXXX };
static constexpr short d_OLP[2] = { 1, MarketId::XXXX };
static constexpr short d_OMA[2] = { 1, MarketId::XXXX };
static constexpr short d_OMD[2] = { 1, MarketId::XXXX };
static constexpr short d_OME[2] = { 1, MarketId::XXXX };
static constexpr short d_OMH[2] = { 1, MarketId::XXXX };
static constexpr short d_OMO[2] = { 1, MarketId::XXXX };
static constexpr short d_OND[2] = { 1, MarketId::XXXX };
static constexpr short d_ONT[2] = { 1, MarketId::XXXX };
static constexpr short d_OOL[2] = { 1, MarketId::XXXX };
static constexpr short d_OOM[2] = { 1, MarketId::XXXX };
static constexpr short d_OPO[2] = { 1, MarketId::MBCP };
static constexpr short d_ORB[2] = { 1, MarketId::XXXX };
static constexpr short d_ORF[2] = { 1, MarketId::XXXX };
static constexpr short d_ORH[2] = { 1, MarketId::XXXX };
static constexpr short d_ORK[2] = { 1, MarketId::XXXX };
static constexpr short d_ORN[2] = { 1, MarketId::XXXX };
static constexpr short d_OSA[7] = { 6, MarketId::XHER, MarketId::XKAC, MarketId::XOSE, MarketId::XOSJ, MarketId::XOSM, MarketId::XOST };
static constexpr short d_OSH[2] = { 1, MarketId::XXXX };
static constexpr short d_OSI[2] = { 1, MarketId::XXXX };
static constexpr short d_OSL[39] = { 38, MarketId::BULK, MarketId::BURG, MarketId::BURM, MarketId::CNSI, MarketId::DOSL, MarketId::ELEU, MarketId::ELNO, MarketId::ELSE, MarketId::ELUK, MarketId::FNDS, MarketId::FREI, MarketId::IFFX, MarketId::INFT, MarketId::MERD, MarketId::MERK, MarketId::NEXO, MarketId::NIBR, MarketId::NOR0, MarketId::NORX, MarketId::NOSC, MarketId::NOTC, MarketId::OAPA, MarketId::OSLC, MarketId::QUNT, MarketId::SB1M, MarketId::STEE, MarketId::XABG, MarketId::XDNB, MarketId::XIMA, MarketId::XOAA, MarketId::XOAD, MarketId::XOAM, MarketId::XOAS, MarketId::XOBD, MarketId::XOSA, MarketId::XOSC, MarketId::XOSD, MarketId::XOSL };
static constexpr short d_OSM[2] = { 1, MarketId::XXXX };
static constexpr short d_OSR[2] = { 1, MarketId::XXXX };
static constexpr short d_OTH[2] = { 1, MarketId::XXXX };
static constexpr short d_OTZ[2] = { 1, MarketId::XXXX };
static constexpr short d_OUA[2] = { 1, MarketId::BFA0 };
static constexpr short d_OUD[2] = { 1, MarketId::XXXX };
static constexpr short d_OUH[2] = { 1, MarketId::XXXX };
static constexpr short d_OUK[2] = { 1, MarketId::XXXX };
static constexpr short d_OUL[2] = { 1, MarketId::XXXX };
static constexpr short d_OUZ[2] = { 1, MarketId::XXXX };
static constexpr short d_OVB[3] = { 2, MarketId::XSIB, MarketId::XSIC };
static constexpr short d_OVD[2] = { 1, MarketId::XXXX };
static constexpr short d_OWB[2] = { 1, MarketId::XXXX };
static constexpr short d_OXB[2] = { 1, MarketId::GNB0 };
static constexpr short d_OXR[2] = { 1, MarketId::XXXX };
static constexpr short d_OZH[2] = { 1, MarketId::XXXX };
static constexpr short d_OZZ[2] = { 1, MarketId::XXXX };
static constexpr short d_PA0[2] = { 1, MarketId::XXXX };
static constexpr short d_PAC[3] = { 2, MarketId::PAN0, MarketId::XPTY };
static constexpr short d_PAD[2] = { 1, MarketId::XXXX };
static constexpr short d_PAH[2] = { 1, MarketId::XXXX };
static constexpr short d_PAP[2] = { 1, MarketId::HTI0 };
static constexpr short d_PAR[100] = { 99, MarketId::AACA, MarketId::ALXP, MarketId::AQEA, MarketId::AQED, MarketId::AQEU, MarketId::AURB, MarketId::AURO, MarketId::BNPA, MarketId::BNPS, MarketId::BPSX, MarketId::BRED, MarketId::CMCI, MarketId::COAL, MarketId::DPAR, MarketId::EDRF, MarketId::ELXE, MarketId::EPEX, MarketId::EXSB, MarketId::EXSD, MarketId::EXSE, MarketId::EXSF, MarketId::EXSH, MarketId::EXSP, MarketId::EXSY, MarketId::EXYY, MarketId::FMTS, MarketId::FRA0, MarketId::GFPO, MarketId::GIPB, MarketId::GMEO, MarketId::GMES, MarketId::GMTF, MarketId::GSPX, MarketId::HBFR, MarketId::HPCO, MarketId::HPCS, MarketId::HPCV, MarketId::HPCX, MarketId::ICOR, MarketId::ICOT, MarketId::KOTF, MarketId::LCHC, MarketId::LNEQ, MarketId::LNFI, MarketId::MHBP, MarketId::MKTF, MarketId::MLER, MarketId::MLES, MarketId::MLEX, MarketId::MSAX, MarketId::MSCX, MarketId::MSNT, MarketId::MSSA, MarketId::MTCH, MarketId::MUBP, MarketId::NABP, MarketId::NATX, MarketId::NOWX, MarketId::ODDO, MarketId::ODOC, MarketId::RBCC, MarketId::SGMU, MarketId::SGMV, MarketId::SGMW, MarketId::SGOE, MarketId::SMBP, MarketId::TDON, MarketId::TEPF, MarketId::TEPG, MarketId::TEPI, MarketId::TEPM, MarketId::TEPR, MarketId::TEPX, MarketId::TLCM, MarketId::TPEE, MarketId::TPER, MarketId::TPEU, MarketId::TPFR, MarketId::TPIC, MarketId::TPIO, MarketId::TPIR, MarketId::TRXE, MarketId::TSAF, MarketId::VRXP, MarketId::WFSE, MarketId::XAFR, MarketId::XAPA, MarketId::XBLK, MarketId::XBLN, MarketId::XETF, MarketId::XFMN, MarketId::XMAT, MarketId::XMLI, MarketId::XMON, MarketId::XPAR, MarketId::XPOW, MarketId::XSGA, MarketId::XSPM, MarketId::XTXE };
static constexpr short d_PAT[2] = { 1, MarketId::XXXX };
static constexpr short d_PB0[2] = { 1, MarketId::XXXX };
static constexpr short d_PBC[2] = { 1, MarketId::XXXX };
static constexpr short d_PBH[2] = { 1, MarketId::BTN0 };
static constexpr short d_PBI[2] = { 1, MarketId::XXXX };
static constexpr short d_PBM[2] = { 1, MarketId::SUR0 };
static constexpr short d_PBO[2] = { 1, MarketId::XXXX };
static constexpr short d_PBZ[2] = { 1, MarketId::XXXX };
static constexpr short d_PC0[2] = { 1, MarketId::XPHS };
static constexpr short d_PCT[8] = { 7, MarketId::EMLD, MarketId::EPRD, MarketId::EPRL, MarketId::MIHI, MarketId::MPRL, MarketId::SPHR, MarketId::XMIO };
static constexpr short d_PDL[2] = { 1, MarketId::XXXX };
static constexpr short d_PDT[2] = { 1, MarketId::XXXX };
static constexpr short d_PDX[2] = { 1, MarketId::XXXX };
static constexpr short d_PEG[2] = { 1, MarketId::XXXX };
static constexpr short d_PEI[2] = { 1, MarketId::XXXX };
static constexpr short d_PEN[2] = { 1, MarketId::XXXX };
static constexpr short d_PER[2] = { 1, MarketId::XXXX };
static constexpr short d_PEW[2] = { 1, MarketId::XXXX };
static constexpr short d_PFN[2] = { 1, MarketId::XXXX };
static constexpr short d_PFO[2] = { 1, MarketId::XXXX };
static constexpr short d_PGA[2] = { 1, MarketId::XXXX };
static constexpr short d_PGF[2] = { 1, MarketId::XXXX };
static constexpr short d_PGV[2] = { 1, MarketId::XXXX };
static constexpr short d_PHC[2] = { 1, MarketId::XXXX };
static constexpr short d_PHE[2] = { 1, MarketId::XXXX };
static constexpr short d_PHF[2] = { 1, MarketId::XXXX };
static constexpr short d_PHH[2] = { 1, MarketId::XXXX };
static constexpr short d_PHL[6] = { 5, MarketId::RICD, MarketId::RICX, MarketId::XPBT, MarketId::XPHL, MarketId::XPHO };
static constexpr short d_PHW[2] = { 1, MarketId::XXXX };
static constexpr short d_PHX[2] = { 1, MarketId::XAZX };
static constexpr short d_PIA[2] = { 1, MarketId::XXXX };
static constexpr short d_PIB[2] = { 1, MarketId::XXXX };
static constexpr short d_PIH[2] = { 1, MarketId::XXXX };
static constexpr short d_PIR[2] = { 1, MarketId::XXXX };
static constexpr short d_PIS[2] = { 1, MarketId::XXXX };
static constexpr short d_PIT[2] = { 1, MarketId::XXXX };
static constexpr short d_PJG[2] = { 1, MarketId::XXXX };
static constexpr short d_PKB[2] = { 1, MarketId::XXXX };
static constexpr short d_PKW[2] = { 1, MarketId::XXXX };
static constexpr short d_PLB[2] = { 1, MarketId::XXXX };
static constexpr short d_PLN[2] = { 1, MarketId::XXXX };
static constexpr short d_PLO[2] = { 1, MarketId::XXXX };
static constexpr short d_PLZ[2] = { 1, MarketId::XXXX };
static constexpr short d_PMD[2] = { 1, MarketId::XXXX };
static constexpr short d_PMI[2] = { 1, MarketId::BCMA };
static constexpr short d_PMO[2] = { 1, MarketId::XXXX };
static constexpr short d_PMR[2] = { 1, MarketId::XXXX };
static constexpr short d_PMV[2] = { 1, MarketId::XXXX };
static constexpr short d_PMW[2] = { 1, MarketId::XXXX };
static constexpr short d_PN0[2] = { 1, MarketId::PCN0 };
static constexpr short d_PNH[3] = { 2, MarketId::KHM0, MarketId::XCSX };
static constexpr short d_PNI[2] = { 1, MarketId::FSM0 };
static constexpr short d_PNL[2] = { 1, MarketId::XXXX };
static constexpr short d_PNQ[2] = { 1, MarketId::XXXX };
static constexpr short d_PNR[2] = { 1, MarketId::XXXX };
static constexpr short d_PNS[2] = { 1, MarketId::XXXX };
static constexpr short d_POA[2] = { 1, MarketId::XXXX };
static constexpr short d_POG[2] = { 1, MarketId::XXXX };
static constexpr short d_POM[3] = { 2, MarketId::PNG0, MarketId::XPOM };
static constexpr short d_POP[2] = { 1, MarketId::XXXX };
static constexpr short d_POR[2] = { 1, MarketId::XXXX };
static constexpr short d_POS[3] = { 2, MarketId::TTO0, MarketId::XTRN };
static constexpr short d_POU[2] = { 1, MarketId::XXXX };
static constexpr short d_POZ[2] = { 1, MarketId::XXXX };
static constexpr short d_PPG[2] = { 1, MarketId::ASM0 };
static constexpr short d_PPP[2] = { 1, MarketId::XXXX };
static constexpr short d_PPT[2] = { 1, MarketId::PYF0 };
static constexpr short d_PQI[2] = { 1, MarketId::XXXX };
static constexpr short d_PQQ[2] = { 1, MarketId::XXXX };
static constexpr short d_PRG[21] = { 20, MarketId::ATAD, MarketId::CELP, MarketId::CESI, MarketId::CSAS, MarketId::CSOB, MarketId::CZE0, MarketId::FTFM, MarketId::FTFS, MarketId::KOME, MarketId::PATF, MarketId::RFBK, MarketId::SPAD, MarketId::STRT, MarketId::UBCZ, MarketId::WOOD, MarketId::XPRA, MarketId::XPRM, MarketId::XPXE, MarketId::XRMO, MarketId::XRMZ };
static constexpr short d_PRN[2] = { 1, MarketId::XXXX };
static constexpr short d_PRY[2] = { 1, MarketId::ZAF0 };
static constexpr short d_PS0[3] = { 2, MarketId::PSE0, MarketId::XPAE };
static constexpr short d_PSA[2] = { 1, MarketId::XXXX };
static constexpr short d_PSC[2] = { 1, MarketId::XXXX };
static constexpr short d_PSD[2] = { 1, MarketId::XXXX };
static constexpr short d_PSE[2] = { 1, MarketId::XXXX };
static constexpr short d_PSG[2] = { 1, MarketId::XXXX };
static constexpr short d_PSI[2] = { 1, MarketId::XXXX };
static constexpr short d_PSP[2] = { 1, MarketId::XXXX };
static constexpr short d_PSR[2] = { 1, MarketId::XXXX };
static constexpr short d_PSY[2] = { 1, MarketId::FLK0 };
static constexpr short d_PT0[2] = { 1, MarketId::BEN0 };
static constexpr short d_PTG[2] = { 1, MarketId::CBMS };
static constexpr short d_PTJ[2] = { 1, MarketId::XXXX };
static constexpr short d_PUB[2] = { 1, MarketId::XXXX };
static constexpr short d_PUF[2] = { 1, MarketId::XXXX };
static constexpr short d_PUG[2] = { 1, MarketId::XXXX };
static constexpr short d_PUJ[2] = { 1, MarketId::XXXX };
static constexpr short d_PUQ[2] = { 1, MarketId::XXXX };
static constexpr short d_PUS[2] = { 1, MarketId::XXXX };
static constexpr short d_PUW[2] = { 1, MarketId::XXXX };
static constexpr short d_PUY[2] = { 1, MarketId::XXXX };
static constexpr short d_PVD[2] = { 1, MarketId::XXXX };
static constexpr short d_PVH[2] = { 1, MarketId::XXXX };
static constexpr short d_PVK[2] = { 1, MarketId::XXXX };
static constexpr short d_PVR[2] = { 1, MarketId::XXXX };
static constexpr short d_PWM[2] = { 1, MarketId::XXXX };
static constexpr short d_PXM[2] = { 1, MarketId::XXXX };
static constexpr short d_PXO[2] = { 1, MarketId::XXXX };
static constexpr short d_PYB[2] = { 1, MarketId::XXXX };
static constexpr short d_PYX[2] = { 1, MarketId::XXXX };
static constexpr short d_PZB[2] = { 1, MarketId::XXXX };
static constexpr short d_PZH[2] = { 1, MarketId::XXXX };
static constexpr short d_PZO[2] = { 1, MarketId::XXXX };
static constexpr short d_QCB[2] = { 1, MarketId::XXXX };
static constexpr short d_QGF[2] = { 1, MarketId::XXXX };
static constexpr short d_QIC[2] = { 1, MarketId::ISBV };
static constexpr short d_QLI[6] = { 5, MarketId::BCSC, MarketId::ETOR, MarketId::GPBC, MarketId::MKAP, MarketId::TMCY };
static constexpr short d_QML[2] = { 1, MarketId::XXXX };
static constexpr short d_QMN[3] = { 2, MarketId::SWZ0, MarketId::XSWA };
static constexpr short d_QNB[2] = { 1, MarketId::XXXX };
static constexpr short d_QND[2] = { 1, MarketId::XXXX };
static constexpr short d_QPA[2] = { 1, MarketId::BEIS };
static constexpr short d_QSA[2] = { 1, MarketId::BSAB };
static constexpr short d_RA0[2] = { 1, MarketId::SKSI };
static constexpr short d_RAI[3] = { 2, MarketId::CPV0, MarketId::XBVC };
static constexpr short d_RAJ[2] = { 1, MarketId::XXXX };
static constexpr short d_RAK[2] = { 1, MarketId::XXXX };
static constexpr short d_RAP[2] = { 1, MarketId::XXXX };
static constexpr short d_RAR[2] = { 1, MarketId::COK0 };
static constexpr short d_RAZ[2] = { 1, MarketId::XXXX };
static constexpr short d_RB0[2] = { 1, MarketId::LIGA };
static constexpr short d_RBA[2] = { 1, MarketId::MAR0 };
static constexpr short d_RBR[2] = { 1, MarketId::XXXX };
static constexpr short d_RCB[2] = { 1, MarketId::XXXX };
static constexpr short d_RDD[2] = { 1, MarketId::XXXX };
static constexpr short d_RDG[2] = { 1, MarketId::XXXX };
static constexpr short d_RDM[2] = { 1, MarketId::XXXX };
static constexpr short d_RDU[2] = { 1, MarketId::XXXX };
static constexpr short d_RDZ[2] = { 1, MarketId::XXXX };
static constexpr short d_REC[2] = { 1, MarketId::XXXX };
static constexpr short d_REG[2] = { 1, MarketId::XXXX };
static constexpr short d_REK[9] = { 8, MarketId::DICE, MarketId::DNIS, MarketId::FNIS, MarketId::ISEC, MarketId::ISL0, MarketId::MICE, MarketId::MNIS, MarketId::XICE };
static constexpr short d_REU[2] = { 1, MarketId::XXXX };
static constexpr short d_RFD[2] = { 1, MarketId::XXXX };
static constexpr short d_RFP[2] = { 1, MarketId::XXXX };
static constexpr short d_RGI[2] = { 1, MarketId::XXXX };
static constexpr short d_RGL[2] = { 1, MarketId::XXXX };
static constexpr short d_RGN[2] = { 1, MarketId::MMR0 };
static constexpr short d_RHG[2] = { 1, MarketId::XXXX };
static constexpr short d_RHI[2] = { 1, MarketId::XXXX };
static constexpr short d_RHO[2] = { 1, MarketId::XXXX };
static constexpr short d_RIC[2] = { 1, MarketId::XXXX };
static constexpr short d_RIO[5] = { 4, MarketId::CETI, MarketId::SELC, MarketId::XBBF, MarketId::XRIO };
static constexpr short d_RIX[5] = { 4, MarketId::FNLV, MarketId::LVA0, MarketId::SWLV, MarketId::XRIS };
static constexpr short d_RIZ[2] = { 1, MarketId::XXXX };
static constexpr short d_RJK[2] = { 1, MarketId::XXXX };
static constexpr short d_RKD[2] = { 1, MarketId::XXXX };
static constexpr short d_RKS[2] = { 1, MarketId::XXXX };
static constexpr short d_RKT[2] = { 1, MarketId::XXXX };
static constexpr short d_RLT[2] = { 1, MarketId::XXXX };
static constexpr short d_RMF[2] = { 1, MarketId::XXXX };
static constexpr short d_RMI[2] = { 1, MarketId::XXXX };
static constexpr short d_RNB[2] = { 1, MarketId::XXXX };
static constexpr short d_RNE[2] = { 1, MarketId::XXXX };
static constexpr short d_RNN[2] = { 1, MarketId::XXXX };
static constexpr short d_RNO[2] = { 1, MarketId::XXXX };
static constexpr short d_RNS[2] = { 1, MarketId::XXXX };
static constexpr short d_ROA[2] = { 1, MarketId::XXXX };
static constexpr short d_ROC[2] = { 1, MarketId::XXXX };
static constexpr short d_ROK[2] = { 1, MarketId::XXXX };
static constexpr short d_ROM[21] = { 20, MarketId::BNLD, MarketId::BOND, MarketId::CGCM, MarketId::CGDB, MarketId::CGEB, MarketId::CGGD, MarketId::CGIT, MarketId::CGND, MarketId::CGQD, MarketId::CGQT, MarketId::CGTR, MarketId::EBMX, MarketId::ITA0, MarketId::MCAD, MarketId::MSWP, MarketId::MTSC, MarketId::MTSM, MarketId::MTSO, MarketId::SSOB, MarketId::XGME };
static constexpr short d_ROR[2] = { 1, MarketId::PLW0 };
static constexpr short d_ROS[4] = { 3, MarketId::ROFX, MarketId::XROS, MarketId::XROX };
static constexpr short d_ROT[2] = { 1, MarketId::XXXX };
static constexpr short d_ROU[2] = { 1, MarketId::XXXX };
static constexpr short d_ROV[2] = { 1, MarketId::XROV };
static constexpr short d_RR0[2] = { 1, MarketId::XXXX };
static constexpr short d_RRG[2] = { 1, MarketId::XXXX };
static constexpr short d_RS0[2] = { 1, MarketId::XXXX };
static constexpr short d_RSA[2] = { 1, MarketId::XXXX };
static constexpr short d_RSD[2] = { 1, MarketId::XXXX };
static constexpr short d_RST[2] = { 1, MarketId::XXXX };
static constexpr short d_RTB[2] = { 1, MarketId::XXXX };
static constexpr short d_RTM[2] = { 1, MarketId::XXXX };
static constexpr short d_RUH[3] = { 2, MarketId::SAU0, MarketId::XSAU };
static constexpr short d_RUN[2] = { 1, MarketId::REU0 };
static constexpr short d_RVN[2] = { 1, MarketId::XXXX };
static constexpr short d_RWI[2] = { 1, MarketId::XXXX };
static constexpr short d_RWP[2] = { 1, MarketId::XXXX };
static constexpr short d_RYK[2] = { 1, MarketId::XXXX };
static constexpr short d_SAF[2] = { 1, MarketId::XXXX };
static constexpr short d_SAH[2] = { 1, MarketId::YEM0 };
static constexpr short d_SAI[2] = { 1, MarketId::XXXX };
static constexpr short d_SAL[3] = { 2, MarketId::SLV0, MarketId::XSVA };
static constexpr short d_SAN[2] = { 1, MarketId::XXXX };
static constexpr short d_SAO[6] = { 5, MarketId::BRIX, MarketId::BVMF, MarketId::XBMF, MarketId::XBSP, MarketId::XSOM };
static constexpr short d_SAP[2] = { 1, MarketId::XHON };
static constexpr short d_SAT[2] = { 1, MarketId::XXXX };
static constexpr short d_SAV[2] = { 1, MarketId::XXXX };
static constexpr short d_SBA[2] = { 1, MarketId::XXXX };
static constexpr short d_SBH[2] = { 1, MarketId::BLM0 };
static constexpr short d_SBK[2] = { 1, MarketId::XXXX };
static constexpr short d_SBN[2] = { 1, MarketId::XXXX };
static constexpr short d_SBP[2] = { 1, MarketId::XXXX };
static constexpr short d_SBU[2] = { 1, MarketId::XXXX };
static constexpr short d_SBW[2] = { 1, MarketId::XXXX };
static constexpr short d_SBY[2] = { 1, MarketId::XXXX };
static constexpr short d_SBZ[5] = { 4, MarketId::BMFA, MarketId::BMFM, MarketId::BMFX, MarketId::SBMF };
static constexpr short d_SCC[2] = { 1, MarketId::XXXX };
static constexpr short d_SCE[2] = { 1, MarketId::XXXX };
static constexpr short d_SCF[2] = { 1, MarketId::XXXX };
static constexpr short d_SCK[2] = { 1, MarketId::XXXX };
static constexpr short d_SCN[2] = { 1, MarketId::XXXX };
static constexpr short d_SCQ[2] = { 1, MarketId::XXXX };
static constexpr short d_SCU[2] = { 1, MarketId::XXXX };
static constexpr short d_SDF[2] = { 1, MarketId::XXXX };
static constexpr short d_SDH[2] = { 1, MarketId::XXXX };
static constexpr short d_SDJ[2] = { 1, MarketId::XXXX };
static constexpr short d_SDL[2] = { 1, MarketId::XXXX };
static constexpr short d_SDQ[4] = { 3, MarketId::DOM0, MarketId::XBVR, MarketId::XCVD };
static constexpr short d_SDR[2] = { 1, MarketId::SANT };
static constexpr short d_SDS[2] = { 1, MarketId::XXXX };
static constexpr short d_SDT[2] = { 1, MarketId::XXXX };
static constexpr short d_SDY[2] = { 1, MarketId::XXXX };
static constexpr short d_SEA[2] = { 1, MarketId::XXXX };
static constexpr short d_SEB[2] = { 1, MarketId::XXXX };
static constexpr short d_SEL[13] = { 12, MarketId::GSXK, MarketId::KOCN, MarketId::KOR0, MarketId::NXTE, MarketId::XKCM, MarketId::XKEM, MarketId::XKFB, MarketId::XKFE, MarketId::XKON, MarketId::XKOR, MarketId::XKOS, MarketId::XKRX };
static constexpr short d_SEN[2] = { 1, MarketId::XXXX };
static constexpr short d_SEW[2] = { 1, MarketId::XXXX };
static constexpr short d_SEZ[7] = { 6, MarketId::SECC, MarketId::SECD, MarketId::SECE, MarketId::SEDC, MarketId::SYC0, MarketId::TRPX };
static constexpr short d_SFA[2] = { 1, MarketId::XXXX };
static constexpr short d_SFG[2] = { 1, MarketId::MAF0 };
static constexpr short d_SFJ[2] = { 1, MarketId::XXXX };
static constexpr short d_SFN[3] = { 2, MarketId::BCFS, MarketId::XMVL };
static constexpr short d_SFO[7] = { 6, MarketId::FICO, MarketId::LTSE, MarketId::SAGE, MarketId::XFCI, MarketId::XFDA, MarketId::XPSE };
static constexpr short d_SGD[2] = { 1, MarketId::XXXX };
static constexpr short d_SGF[2] = { 1, MarketId::XXXX };
static constexpr short d_SGN[2] = { 1, MarketId::XSTC };
static constexpr short d_SGU[2] = { 1, MarketId::XXXX };
static constexpr short d_SGY[2] = { 1, MarketId::XXXX };
static constexpr short d_SH0[2] = { 1, MarketId::XEMS };
static constexpr short d_SHA[9] = { 8, MarketId::CCFX, MarketId::CFBC, MarketId::SGEX, MarketId::XCFE, MarketId::XINE, MarketId::XSGE, MarketId::XSHG, MarketId::XSSC };
static constexpr short d_SHD[2] = { 1, MarketId::XXXX };
static constexpr short d_SHE[2] = { 1, MarketId::XXXX };
static constexpr short d_SHJ[2] = { 1, MarketId::XXXX };
static constexpr short d_SHR[2] = { 1, MarketId::XXXX };
static constexpr short d_SHS[2] = { 1, MarketId::XKST };
static constexpr short d_SHV[2] = { 1, MarketId::XXXX };
static constexpr short d_SI0[2] = { 1, MarketId::JBSI };
static constexpr short d_SID[2] = { 1, MarketId::XXXX };
static constexpr short d_SIN[47] = { 46, MarketId::APCL, MarketId::APEX, MarketId::BBLX, MarketId::BGSG, MarketId::BTBS, MarketId::CHIE, MarketId::CLTD, MarketId::CNOD, MarketId::ECAL, MarketId::EEAL, MarketId::ENMS, MarketId::EQOC, MarketId::EQOD, MarketId::EQOS, MarketId::FMXS, MarketId::FXSM, MarketId::GFSG, MarketId::ICPS, MarketId::IFSG, MarketId::JADX, MarketId::LMAS, MarketId::MALM, MarketId::MASG, MarketId::OSSG, MarketId::PGTP, MarketId::RFQS, MarketId::RTSP, MarketId::SGP0, MarketId::SMEX, MarketId::SYFX, MarketId::TERM, MarketId::TFSA, MarketId::TPSG, MarketId::TRDC, MarketId::TSIG, MarketId::TWSG, MarketId::XABX, MarketId::XAND, MarketId::XANM, MarketId::XANS, MarketId::XSBT, MarketId::XSCA, MarketId::XSCE, MarketId::XSCL, MarketId::XSES, MarketId::XSIM };
static constexpr short d_SIP[2] = { 1, MarketId::XXXX };
static constexpr short d_SIS[2] = { 1, MarketId::XXXX };
static constexpr short d_SIT[2] = { 1, MarketId::XXXX };
static constexpr short d_SIX[2] = { 1, MarketId::XXXX };
static constexpr short d_SJC[2] = { 1, MarketId::XXXX };
static constexpr short d_SJD[2] = { 1, MarketId::XXXX };
static constexpr short d_SJJ[3] = { 2, MarketId::BIH0, MarketId::XSSE };
static constexpr short d_SJO[3] = { 2, MarketId::CRI0, MarketId::XBNV };
static constexpr short d_SJT[2] = { 1, MarketId::XXXX };
static constexpr short d_SJU[3] = { 2, MarketId::PRI0, MarketId::XGMX };
static constexpr short d_SJY[2] = { 1, MarketId::XXXX };
static constexpr short d_SK0[2] = { 1, MarketId::XXXX };
static constexpr short d_SKB[3] = { 2, MarketId::KNA0, MarketId::XECS };
static constexpr short d_SKD[2] = { 1, MarketId::XXXX };
static constexpr short d_SKG[2] = { 1, MarketId::XXXX };
static constexpr short d_SKP[3] = { 2, MarketId::MKD0, MarketId::XMAE };
static constexpr short d_SKS[2] = { 1, MarketId::XXXX };
static constexpr short d_SKV[2] = { 1, MarketId::XXXX };
static constexpr short d_SKX[2] = { 1, MarketId::XXXX };
static constexpr short d_SKZ[2] = { 1, MarketId::XXXX };
static constexpr short d_SLA[2] = { 1, MarketId::XXXX };
static constexpr short d_SLC[2] = { 1, MarketId::XXXX };
static constexpr short d_SLE[2] = { 1, MarketId::XXXX };
static constexpr short d_SLL[2] = { 1, MarketId::XXXX };
static constexpr short d_SLP[2] = { 1, MarketId::XXXX };
static constexpr short d_SLS[2] = { 1, MarketId::XXXX };
static constexpr short d_SLU[2] = { 1, MarketId::LCA0 };
static constexpr short d_SLZ[2] = { 1, MarketId::XXXX };
static constexpr short d_SM0[2] = { 1, MarketId::SMR0 };
static constexpr short d_SMA[2] = { 1, MarketId::XXXX };
static constexpr short d_SMF[2] = { 1, MarketId::XXXX };
static constexpr short d_SMI[2] = { 1, MarketId::XXXX };
static constexpr short d_SMX[2] = { 1, MarketId::XXXX };
static constexpr short d_SNA[2] = { 1, MarketId::XXXX };
static constexpr short d_SNC[2] = { 1, MarketId::XXXX };
static constexpr short d_SNN[2] = { 1, MarketId::XXXX };
static constexpr short d_SNS[2] = { 1, MarketId::XXXX };
static constexpr short d_SOF[19] = { 18, MarketId::ABUL, MarketId::BDSK, MarketId::BEBG, MarketId::BGHX, MarketId::BGR0, MarketId::EUFN, MarketId::GBUL, MarketId::GMBG, MarketId::IBEX, MarketId::IBUL, MarketId::JBUL, MarketId::LBUL, MarketId::MBUL, MarketId::PBUL, MarketId::T212, MarketId::UCBG, MarketId::XBUL, MarketId::ZBUL };
static constexpr short d_SOG[2] = { 1, MarketId::XXXX };
static constexpr short d_SON[2] = { 1, MarketId::XXXX };
static constexpr short d_SOT[2] = { 1, MarketId::XXXX };
static constexpr short d_SOU[2] = { 1, MarketId::XXXX };
static constexpr short d_SPC[2] = { 1, MarketId::XXXX };
static constexpr short d_SPI[2] = { 1, MarketId::XXXX };
static constexpr short d_SPN[2] = { 1, MarketId::MNP0 };
static constexpr short d_SPS[2] = { 1, MarketId::XXXX };
static constexpr short d_SPU[2] = { 1, MarketId::XOTP };
static constexpr short d_SPY[2] = { 1, MarketId::XXXX };
static constexpr short d_SRA[2] = { 1, MarketId::XXXX };
static constexpr short d_SRB[2] = { 1, MarketId::XXXX };
static constexpr short d_SRL[2] = { 1, MarketId::XXXX };
static constexpr short d_SRQ[2] = { 1, MarketId::XXXX };
static constexpr short d_SRZ[2] = { 1, MarketId::XXXX };
static constexpr short d_SSA[2] = { 1, MarketId::XXXX };
static constexpr short d_SSG[2] = { 1, MarketId::GNQ0 };
static constexpr short d_SSH[2] = { 1, MarketId::XXXX };
static constexpr short d_SSL[2] = { 1, MarketId::XXXX };
static constexpr short d_ST0[7] = { 6, MarketId::ASMT, MarketId::ASPI, MarketId::ASPN, MarketId::INCR, MarketId::NTRL, MarketId::XPIN };
static constexpr short d_STI[4] = { 3, MarketId::CHL0, MarketId::XBCL, MarketId::XSGO };
static constexpr short d_STL[2] = { 1, MarketId::XXXX };
static constexpr short d_STO[66] = { 65, MarketId::CASI, MarketId::CRYD, MarketId::CRYX, MarketId::CSTO, MarketId::DKED, MarketId::DKFI, MarketId::DKOB, MarketId::DKWB, MarketId::DNSE, MarketId::DOSE, MarketId::DSTO, MarketId::EBON, MarketId::ENSX, MarketId::ESTO, MarketId::EUOB, MarketId::EUWB, MarketId::FIED, MarketId::FNSE, MarketId::GBOB, MarketId::GBWB, MarketId::GFKS, MarketId::MNSE, MarketId::MOSE, MarketId::MSTO, MarketId::NAPA, MarketId::NASN, MarketId::NMTF, MarketId::NOCO, MarketId::NOED, MarketId::NOFI, MarketId::NOOB, MarketId::NOWB, MarketId::NSME, MarketId::NSPO, MarketId::ONSE, MarketId::PEPH, MarketId::PEPM, MarketId::PEPQ, MarketId::PEPW, MarketId::PEPY, MarketId::PNED, MarketId::SEBS, MarketId::SEBX, MarketId::SEED, MarketId::SEOB, MarketId::SEWB, MarketId::SPDK, MarketId::SPEU, MarketId::SPFI, MarketId::SPNO, MarketId::SSME, MarketId::SVES, MarketId::SVEX, MarketId::SWBI, MarketId::SWE0, MarketId::USOB, MarketId::USWB, MarketId::XABC, MarketId::XNDX, MarketId::XNGM, MarketId::XNMR, MarketId::XOME, MarketId::XOPV, MarketId::XSAT, MarketId::XSTO };
static constexpr short d_STP[2] = { 1, MarketId::XXXX };
static constexpr short d_STR[18] = { 17, MarketId::EUWA, MarketId::EUWX, MarketId::LBBW, MarketId::LBWL, MarketId::LBWS, MarketId::STUA, MarketId::STUB, MarketId::STUC, MarketId::STUD, MarketId::STUE, MarketId::STUF, MarketId::STUH, MarketId::XCTS, MarketId::XDEX, MarketId::XSTF, MarketId::XSTP, MarketId::XSTU };
static constexpr short d_STS[2] = { 1, MarketId::XXXX };
static constexpr short d_STT[2] = { 1, MarketId::VIR0 };
static constexpr short d_STV[2] = { 1, MarketId::XXXX };
static constexpr short d_STX[2] = { 1, MarketId::XXXX };
static constexpr short d_SUB[2] = { 1, MarketId::XSUR };
static constexpr short d_SUF[2] = { 1, MarketId::XXXX };
static constexpr short d_SUL[2] = { 1, MarketId::XXXX };
static constexpr short d_SUN[2] = { 1, MarketId::XXXX };
static constexpr short d_SUV[3] = { 2, MarketId::FJI0, MarketId::XSPS };
static constexpr short d_SUX[2] = { 1, MarketId::XXXX };
static constexpr short d_SVD[3] = { 2, MarketId::SVXI, MarketId::VCT0 };
static constexpr short d_SVG[2] = { 1, MarketId::XXXX };
static constexpr short d_SVL[2] = { 1, MarketId::XXXX };
static constexpr short d_SVQ[2] = { 1, MarketId::XXXX };
static constexpr short d_SVX[2] = { 1, MarketId::URCE };
static constexpr short d_SW0[2] = { 1, MarketId::EFTP };
static constexpr short d_SWF[2] = { 1, MarketId::XXXX };
static constexpr short d_SWP[2] = { 1, MarketId::XXXX };
static constexpr short d_SWS[2] = { 1, MarketId::XXXX };
static constexpr short d_SXB[2] = { 1, MarketId::XXXX };
static constexpr short d_SXL[2] = { 1, MarketId::XXXX };
static constexpr short d_SXM[2] = { 1, MarketId::SXM0 };
static constexpr short d_SXR[2] = { 1, MarketId::XXXX };
static constexpr short d_SYD[41] = { 40, MarketId::APXL, MarketId::ASXB, MarketId::ASXC, MarketId::ASXP, MarketId::ASXT, MarketId::ASXV, MarketId::AWEX, MarketId::BGCA, MarketId::BLXA, MarketId::CFAU, MarketId::CGMA, MarketId::CHIA, MarketId::CLAU, MarketId::CSAU, MarketId::CXAB, MarketId::CXAC, MarketId::CXAF, MarketId::CXAI, MarketId::CXAM, MarketId::CXAN, MarketId::CXAP, MarketId::CXAQ, MarketId::CXAR, MarketId::CXAV, MarketId::CXAW, MarketId::GFAU, MarketId::MACB, MarketId::MAQX, MarketId::MEAU, MarketId::MSAL, MarketId::NZFX, MarketId::POTL, MarketId::SIGA, MarketId::SIMV, MarketId::XAOM, MarketId::XASX, MarketId::XFEX, MarketId::XLQC, MarketId::XSFE, MarketId::XYIE };
static constexpr short d_SYR[2] = { 1, MarketId::XXXX };
static constexpr short d_SYY[2] = { 1, MarketId::XXXX };
static constexpr short d_SZD[2] = { 1, MarketId::XXXX };
static constexpr short d_SZF[2] = { 1, MarketId::XXXX };
static constexpr short d_SZG[3] = { 2, MarketId::RVSA, MarketId::SLHB };
static constexpr short d_SZK[2] = { 1, MarketId::XXXX };
static constexpr short d_SZX[4] = { 3, MarketId::XSEC, MarketId::XSHE, MarketId::XSME };
static constexpr short d_SZZ[2] = { 1, MarketId::XXXX };
static constexpr short d_TAB[2] = { 1, MarketId::XXXX };
static constexpr short d_TAK[2] = { 1, MarketId::XXXX };
static constexpr short d_TAM[2] = { 1, MarketId::XXXX };
static constexpr short d_TAO[2] = { 1, MarketId::XXXX };
static constexpr short d_TAS[7] = { 6, MarketId::UZB0, MarketId::XCET, MarketId::XCUE, MarketId::XKCE, MarketId::XSTE, MarketId::XUNI };
static constexpr short d_TAY[9] = { 8, MarketId::CFTW, MarketId::GSXT, MarketId::ROCO, MarketId::TWN0, MarketId::XIME, MarketId::XTAD, MarketId::XTAF, MarketId::XTAI };
static constexpr short d_TBO[2] = { 1, MarketId::XXXX };
static constexpr short d_TBS[3] = { 2, MarketId::GEO0, MarketId::XGSE };
static constexpr short d_TBU[2] = { 1, MarketId::TON0 };
static constexpr short d_TC0[2] = { 1, MarketId::XXXX };
static constexpr short d_TCA[2] = { 1, MarketId::XXXX };
static constexpr short d_TCB[2] = { 1, MarketId::XXXX };
static constexpr short d_TCI[2] = { 1, MarketId::XXXX };
static constexpr short d_TCL[2] = { 1, MarketId::XXXX };
static constexpr short d_TCU[2] = { 1, MarketId::XXXX };
static constexpr short d_TED[2] = { 1, MarketId::XXXX };
static constexpr short d_TEM[2] = { 1, MarketId::XXXX };
static constexpr short d_TEQ[2] = { 1, MarketId::XXXX };
static constexpr short d_TER[2] = { 1, MarketId::XXXX };
static constexpr short d_TEU[2] = { 1, MarketId::XXXX };
static constexpr short d_TEX[2] = { 1, MarketId::XXXX };
static constexpr short d_TF0[2] = { 1, MarketId::ATF0 };
static constexpr short d_TGD[3] = { 2, MarketId::MNE0, MarketId::XMNX };
static constexpr short d_TGU[3] = { 2, MarketId::HND0, MarketId::XBCV };
static constexpr short d_TGV[2] = { 1, MarketId::XXXX };
static constexpr short d_TGZ[2] = { 1, MarketId::XXXX };
static constexpr short d_THE[2] = { 1, MarketId::XXXX };
static constexpr short d_THR[5] = { 4, MarketId::IFBX, MarketId::IMEX, MarketId::IRN0, MarketId::XTEH };
static constexpr short d_THU[2] = { 1, MarketId::XXXX };
static constexpr short d_TIA[4] = { 3, MarketId::ALB0, MarketId::XALS, MarketId::XTIR };
static constexpr short d_TIF[2] = { 1, MarketId::XXXX };
static constexpr short d_TIJ[2] = { 1, MarketId::XXXX };
static constexpr short d_TIP[3] = { 2, MarketId::LBY0, MarketId::XLSM };
static constexpr short d_TIS[2] = { 1, MarketId::XXXX };
static constexpr short d_TIV[2] = { 1, MarketId::XXXX };
static constexpr short d_TK0[2] = { 1, MarketId::TKL0 };
static constexpr short d_TKA[2] = { 1, MarketId::XXXX };
static constexpr short d_TKS[2] = { 1, MarketId::XXXX };
static constexpr short d_TKU[2] = { 1, MarketId::XXXX };
static constexpr short d_TLH[2] = { 1, MarketId::XXXX };
static constexpr short d_TLL[8] = { 7, MarketId::EST0, MarketId::FNEE, MarketId::LMNR, MarketId::SWEE, MarketId::XTAA, MarketId::XTAL, MarketId::XTAR };
static constexpr short d_TLS[3] = { 2, MarketId::ENSL, MarketId::ESLO };
static constexpr short d_TLV[3] = { 2, MarketId::ISR0, MarketId::XTAE };
static constexpr short d_TML[2] = { 1, MarketId::XXXX };
static constexpr short d_TMP[2] = { 1, MarketId::XXXX };
static constexpr short d_TMS[2] = { 1, MarketId::STP0 };
static constexpr short d_TMW[2] = { 1, MarketId::XXXX };
static constexpr short d_TMZ[2] = { 1, MarketId::XXXX };
static constexpr short d_TNA[2] = { 1, MarketId::XXXX };
static constexpr short d_TNG[2] = { 1, MarketId::XXXX };
static constexpr short d_TNR[3] = { 2, MarketId::MDG0, MarketId::XMDG };
static constexpr short d_TOD[2] = { 1, MarketId::XXXX };
static constexpr short d_TOL[2] = { 1, MarketId::XXXX };
static constexpr short d_TOS[2] = { 1, MarketId::FSHX };
static constexpr short d_TOU[2] = { 1, MarketId::XXXX };
static constexpr short d_TOV[2] = { 1, MarketId::XXXX };
static constexpr short d_TOY[2] = { 1, MarketId::XXXX };
static constexpr short d_TPA[2] = { 1, MarketId::XXXX };
static constexpr short d_TPR[2] = { 1, MarketId::XXXX };
static constexpr short d_TPS[2] = { 1, MarketId::XXXX };
static constexpr short d_TRD[3] = { 2, MarketId::NOPS, MarketId::SPTR };
static constexpr short d_TRI[2] = { 1, MarketId::XXXX };
static constexpr short d_TRN[3] = { 2, MarketId::BREA, MarketId::ISBA };
static constexpr short d_TRO[2] = { 1, MarketId::XXXX };
static constexpr short d_TRS[2] = { 1, MarketId::XXXX };
static constexpr short d_TRV[2] = { 1, MarketId::XXXX };
static constexpr short d_TRW[2] = { 1, MarketId::KIR0 };
static constexpr short d_TRZ[2] = { 1, MarketId::XXXX };
static constexpr short d_TSB[2] = { 1, MarketId::XXXX };
static constexpr short d_TSE[5] = { 4, MarketId::AIXK, MarketId::CCEX, MarketId::ITSL, MarketId::KAZ0 };
static constexpr short d_TSF[2] = { 1, MarketId::XXXX };
static constexpr short d_TSN[2] = { 1, MarketId::XXXX };
static constexpr short d_TSV[2] = { 1, MarketId::XXXX };
static constexpr short d_TTN[2] = { 1, MarketId::XXXX };
static constexpr short d_TUC[2] = { 1, MarketId::XTUC };
static constexpr short d_TUK[2] = { 1, MarketId::XXXX };
static constexpr short d_TUL[2] = { 1, MarketId::XXXX };
static constexpr short d_TUN[4] = { 3, MarketId::BTUN, MarketId::TUN0, MarketId::XTUN };
static constexpr short d_TUP[2] = { 1, MarketId::XXXX };
static constexpr short d_TUS[2] = { 1, MarketId::XXXX };
static constexpr short d_TUU[2] = { 1, MarketId::XXXX };
static constexpr short d_TVC[2] = { 1, MarketId::XXXX };
static constexpr short d_TVF[2] = { 1, MarketId::XXXX };
static constexpr short d_TVL[2] = { 1, MarketId::XXXX };
static constexpr short d_TWB[2] = { 1, MarketId::XXXX };
static constexpr short d_TWF[2] = { 1, MarketId::XXXX };
static constexpr short d_TWU[2] = { 1, MarketId::XXXX };
static constexpr short d_TXK[2] = { 1, MarketId::XXXX };
static constexpr short d_TYN[2] = { 1, MarketId::XXXX };
static constexpr short d_TYO[65] = { 64, MarketId::BAJD, MarketId::BASX, MarketId::BETP, MarketId::BGCJ, MarketId::BNPX, MarketId::CFJP, MarketId::CHIJ, MarketId::CHIS, MarketId::CHIV, MarketId::CILH, MarketId::CITD, MarketId::CITX, MarketId::CLJP, MarketId::CMET, MarketId::CSJP, MarketId::DRCT, MarketId::ICHK, MarketId::ICKR, MarketId::ICSH, MarketId::ICSZ, MarketId::ICTW, MarketId::JASR, MarketId::JATA, MarketId::JEFA, MarketId::JPN0, MarketId::KABU, MarketId::KAIX, MarketId::LXJP, MarketId::MAQJ, MarketId::MIZX, MarketId::MSMS, MarketId::MUDX, MarketId::NMRJ, MarketId::NMSX, MarketId::NXBX, MarketId::NXFO, MarketId::NXJP, MarketId::NXSE, MarketId::NXVW, MarketId::ODST, MarketId::ODXE, MarketId::SBIJ, MarketId::SBIU, MarketId::SBIV, MarketId::SIGJ, MarketId::SMBC, MarketId::TICT, MarketId::TWJP, MarketId::TWJT, MarketId::UTSL, MarketId::VKAB, MarketId::XIJP, MarketId::XJAS, MarketId::XJPX, MarketId::XSBI, MarketId::XTAM, MarketId::XTFF, MarketId::XTK1, MarketId::XTK2, MarketId::XTK3, MarketId::XTKO, MarketId::XTKS, MarketId::XTKT, MarketId::XYKT };
static constexpr short d_TYR[2] = { 1, MarketId::XXXX };
static constexpr short d_TYS[2] = { 1, MarketId::XXXX };
static constexpr short d_TZX[2] = { 1, MarketId::XXXX };
static constexpr short d_UAH[2] = { 1, MarketId::XXXX };
static constexpr short d_UAK[2] = { 1, MarketId::XXXX };
static constexpr short d_UAP[2] = { 1, MarketId::XXXX };
static constexpr short d_UBA[2] = { 1, MarketId::XXXX };
static constexpr short d_UBJ[2] = { 1, MarketId::XXXX };
static constexpr short d_UBP[2] = { 1, MarketId::XXXX };
static constexpr short d_UCA[2] = { 1, MarketId::XXXX };
static constexpr short d_UCT[2] = { 1, MarketId::XXXX };
static constexpr short d_UDE[2] = { 1, MarketId::XXXX };
static constexpr short d_UDI[2] = { 1, MarketId::XXXX };
static constexpr short d_UDJ[2] = { 1, MarketId::XXXX };
static constexpr short d_UDR[2] = { 1, MarketId::XXXX };
static constexpr short d_UEE[2] = { 1, MarketId::XXXX };
static constexpr short d_UET[2] = { 1, MarketId::XXXX };
static constexpr short d_UFA[2] = { 1, MarketId::XXXX };
static constexpr short d_UGC[2] = { 1, MarketId::XXXX };
static constexpr short d_UGO[2] = { 1, MarketId::XXXX };
static constexpr short d_UHE[2] = { 1, MarketId::XXXX };
static constexpr short d_UII[2] = { 1, MarketId::XXXX };
static constexpr short d_UIN[2] = { 1, MarketId::XXXX };
static constexpr short d_UIO[3] = { 2, MarketId::ECU0, MarketId::XQUI };
static constexpr short d_UIP[2] = { 1, MarketId::XXXX };
static constexpr short d_UIT[2] = { 1, MarketId::XXXX };
static constexpr short d_UKB[3] = { 2, MarketId::XKGT, MarketId::XKKT };
static constexpr short d_UKI[2] = { 1, MarketId::XXXX };
static constexpr short d_UKY[2] = { 1, MarketId::XKYO };
static constexpr short d_ULB[2] = { 1, MarketId::XXXX };
static constexpr short d_ULD[2] = { 1, MarketId::XXXX };
static constexpr short d_ULN[3] = { 2, MarketId::MNG0, MarketId::XULA };
static constexpr short d_ULU[2] = { 1, MarketId::XXXX };
static constexpr short d_ULY[2] = { 1, MarketId::XXXX };
static constexpr short d_UMD[2] = { 1, MarketId::XXXX };
static constexpr short d_UME[2] = { 1, MarketId::XXXX };
static constexpr short d_UMR[2] = { 1, MarketId::XXXX };
static constexpr short d_UNI[2] = { 1, MarketId::XXXX };
static constexpr short d_UNK[2] = { 1, MarketId::XXXX };
static constexpr short d_UNT[2] = { 1, MarketId::XXXX };
static constexpr short d_UPG[2] = { 1, MarketId::XXXX };
static constexpr short d_UPL[2] = { 1, MarketId::XXXX };
static constexpr short d_UPN[2] = { 1, MarketId::XXXX };
static constexpr short d_UPP[2] = { 1, MarketId::XXXX };
static constexpr short d_URC[2] = { 1, MarketId::XXXX };
static constexpr short d_URG[2] = { 1, MarketId::XXXX };
static constexpr short d_URM[2] = { 1, MarketId::XXXX };
static constexpr short d_USH[2] = { 1, MarketId::XXXX };
static constexpr short d_USN[2] = { 1, MarketId::XXXX };
static constexpr short d_UTC[3] = { 2, MarketId::CLMX, MarketId::RABO };
static constexpr short d_UTH[2] = { 1, MarketId::XXXX };
static constexpr short d_UTN[2] = { 1, MarketId::XXXX };
static constexpr short d_UTP[2] = { 1, MarketId::XXXX };
static constexpr short d_UTT[2] = { 1, MarketId::XXXX };
static constexpr short d_UUD[2] = { 1, MarketId::XXXX };
static constexpr short d_UVE[2] = { 1, MarketId::XXXX };
static constexpr short d_UVF[2] = { 1, MarketId::XXXX };
static constexpr short d_UVL[2] = { 1, MarketId::XXXX };
static constexpr short d_VA0[2] = { 1, MarketId::VAT0 };
static constexpr short d_VAA[2] = { 1, MarketId::XXXX };
static constexpr short d_VAN[2] = { 1, MarketId::XXXX };
static constexpr short d_VAP[2] = { 1, MarketId::BOVA };
static constexpr short d_VAR[2] = { 1, MarketId::XXXX };
static constexpr short d_VAS[2] = { 1, MarketId::XXXX };
static constexpr short d_VBS[2] = { 1, MarketId::XXXX };
static constexpr short d_VBY[2] = { 1, MarketId::XXXX };
static constexpr short d_VCE[2] = { 1, MarketId::XXXX };
static constexpr short d_VDE[2] = { 1, MarketId::XXXX };
static constexpr short d_VDZ[2] = { 1, MarketId::XXXX };
static constexpr short d_VEL[2] = { 1, MarketId::XXXX };
static constexpr short d_VER[2] = { 1, MarketId::XXXX };
static constexpr short d_VFA[2] = { 1, MarketId::VFEX };
static constexpr short d_VGO[2] = { 1, MarketId::XXXX };
static constexpr short d_VID[2] = { 1, MarketId::XXXX };
static constexpr short d_VIE[18] = { 17, MarketId::APAW, MarketId::AUT0, MarketId::EGSI, MarketId::EXAA, MarketId::FPWB, MarketId::LLAT, MarketId::RBIV, MarketId::UCBA, MarketId::WBAH, MarketId::WBDM, MarketId::WBGF, MarketId::WBMA, MarketId::XCEG, MarketId::XOTB, MarketId::XRCB, MarketId::XVIE, MarketId::XWBO };
static constexpr short d_VIJ[2] = { 1, MarketId::XXXX };
static constexpr short d_VIS[2] = { 1, MarketId::XXXX };
static constexpr short d_VIT[2] = { 1, MarketId::XXXX };
static constexpr short d_VIX[2] = { 1, MarketId::BOVM };
static constexpr short d_VLC[3] = { 2, MarketId::XFCM, MarketId::XVAL };
static constexpr short d_VLD[2] = { 1, MarketId::XXXX };
static constexpr short d_VLI[4] = { 3, MarketId::GXMA, MarketId::POTC, MarketId::VUT0 };
static constexpr short d_VLL[2] = { 1, MarketId::XXXX };
static constexpr short d_VLN[2] = { 1, MarketId::XXXX };
static constexpr short d_VLU[2] = { 1, MarketId::XXXX };
static constexpr short d_VN0[2] = { 1, MarketId::NODX };
static constexpr short d_VNO[10] = { 9, MarketId::BAPX, MarketId::FNLT, MarketId::GETB, MarketId::LTU0, MarketId::NASB, MarketId::SEBL, MarketId::SWLT, MarketId::XLIT, MarketId::XVIA };
static constexpr short d_VNS[2] = { 1, MarketId::XXXX };
static constexpr short d_VOG[2] = { 1, MarketId::XXXX };
static constexpr short d_VPS[2] = { 1, MarketId::XXXX };
static constexpr short d_VRA[2] = { 1, MarketId::XXXX };
static constexpr short d_VRB[2] = { 1, MarketId::XXXX };
static constexpr short d_VRK[2] = { 1, MarketId::XXXX };
static constexpr short d_VRN[2] = { 1, MarketId::XXXX };
static constexpr short d_VSA[2] = { 1, MarketId::XXXX };
static constexpr short d_VST[2] = { 1, MarketId::XXXX };
static constexpr short d_VTE[3] = { 2, MarketId::LAO0, MarketId::XLAO };
static constexpr short d_VVO[3] = { 2, MarketId::XAPI, MarketId::XVLA };
static constexpr short d_VXO[2] = { 1, MarketId::XXXX };
static constexpr short d_VY0[2] = { 1, MarketId::XXXX };
static constexpr short d_VYD[2] = { 1, MarketId::XXXX };
static constexpr short d_VZ0[2] = { 1, MarketId::XVAR };
static constexpr short d_WAM[2] = { 1, MarketId::XXXX };
static constexpr short d_WAW[41] = { 40, MarketId::BHWA, MarketId::BNPP, MarketId::BOSP, MarketId::BPKO, MarketId::CETO, MarketId::HBPL, MarketId::IENG, MarketId::INGW, MarketId::KDPW, MarketId::MBPL, MarketId::MSDM, MarketId::MTSP, MarketId::PARK, MarketId::PKOP, MarketId::PLPD, MarketId::PLPO, MarketId::PLPS, MarketId::PLPX, MarketId::POEE, MarketId::POL0, MarketId::PTPG, MarketId::RPWC, MarketId::SIAB, MarketId::TBSA, MarketId::TBSP, MarketId::WBCL, MarketId::WBLC, MarketId::WBON, MarketId::WCDE, MarketId::WDER, MarketId::WETP, MarketId::WGAS, MarketId::WIND, MarketId::WIPO, MarketId::WMTF, MarketId::WOPO, MarketId::XBRY, MarketId::XGLO, MarketId::XNCO, MarketId::XWAR };
static constexpr short d_WDH[3] = { 2, MarketId::NAM0, MarketId::XNAM };
static constexpr short d_WEI[2] = { 1, MarketId::XXXX };
static constexpr short d_WEL[2] = { 1, MarketId::XXXX };
static constexpr short d_WGA[2] = { 1, MarketId::XXXX };
static constexpr short d_WHK[2] = { 1, MarketId::XXXX };
static constexpr short d_WIC[2] = { 1, MarketId::XXXX };
static constexpr short d_WIE[2] = { 1, MarketId::XXXX };
static constexpr short d_WLB[2] = { 1, MarketId::XXXX };
static constexpr short d_WLG[2] = { 1, MarketId::NZL0 };
static constexpr short d_WLS[2] = { 1, MarketId::WLF0 };
static constexpr short d_WMB[2] = { 1, MarketId::XXXX };
static constexpr short d_WN0[2] = { 1, MarketId::XXXX };
static constexpr short d_WNS[2] = { 1, MarketId::XXXX };
static constexpr short d_WOL[2] = { 1, MarketId::XXXX };
static constexpr short d_WP0[2] = { 1, MarketId::INTL };
static constexpr short d_WRE[2] = { 1, MarketId::XXXX };
static constexpr short d_WRG[2] = { 1, MarketId::XXXX };
static constexpr short d_WRL[2] = { 1, MarketId::XXXX };
static constexpr short d_WRO[3] = { 2, MarketId::CAPL, MarketId::WBKP };
static constexpr short d_WSY[2] = { 1, MarketId::XXXX };
static constexpr short d_WUH[2] = { 1, MarketId::XXXX };
static constexpr short d_WUN[2] = { 1, MarketId::XXXX };
static constexpr short d_WUX[2] = { 1, MarketId::CSSX };
static constexpr short d_WVB[2] = { 1, MarketId::XXXX };
static constexpr short d_WYA[2] = { 1, MarketId::XXXX };
static constexpr short d_WYN[2] = { 1, MarketId::XXXX };
static constexpr short d_WYS[2] = { 1, MarketId::XXXX };
static constexpr short d_XCH[2] = { 1, MarketId::CXR0 };
static constexpr short d_XIY[2] = { 1, MarketId::XXXX };
static constexpr short d_XMH[2] = { 1, MarketId::XXXX };
static constexpr short d_XMN[2] = { 1, MarketId::XXXX };
static constexpr short d_XPK[2] = { 1, MarketId::XXXX };
static constexpr short d_XRY[2] = { 1, MarketId::XXXX };
static constexpr short d_XSI[2] = { 1, MarketId::XXXX };
static constexpr short d_XXX[5] = { 4, MarketId::BILT, MarketId::XOFF, MarketId::XXX0, MarketId::XXXX };
static constexpr short d_YAK[2] = { 1, MarketId::XXXX };
static constexpr short d_YAO[2] = { 1, MarketId::CMR0 };
static constexpr short d_YAT[2] = { 1, MarketId::XXXX };
static constexpr short d_YBE[2] = { 1, MarketId::XXXX };
static constexpr short d_YCB[2] = { 1, MarketId::XXXX };
static constexpr short d_YDF[2] = { 1, MarketId::XXXX };
static constexpr short d_YEA[2] = { 1, MarketId::XXXX };
static constexpr short d_YEV[2] = { 1, MarketId::XXXX };
static constexpr short d_YFA[2] = { 1, MarketId::XXXX };
static constexpr short d_YFB[2] = { 1, MarketId::XXXX };
static constexpr short d_YFC[2] = { 1, MarketId::XXXX };
static constexpr short d_YFO[2] = { 1, MarketId::XXXX };
static constexpr short d_YGW[2] = { 1, MarketId::XXXX };
static constexpr short d_YGX[2] = { 1, MarketId::XXXX };
static constexpr short d_YHR[2] = { 1, MarketId::XXXX };
static constexpr short d_YHZ[2] = { 1, MarketId::XXXX };
static constexpr short d_YIF[2] = { 1, MarketId::XXXX };
static constexpr short d_YIH[2] = { 1, MarketId::XXXX };
static constexpr short d_YKA[2] = { 1, MarketId::XXXX };
static constexpr short d_YKM[2] = { 1, MarketId::XXXX };
static constexpr short d_YKS[2] = { 1, MarketId::XXXX };
static constexpr short d_YLB[2] = { 1, MarketId::XXXX };
static constexpr short d_YLR[2] = { 1, MarketId::XXXX };
static constexpr short d_YLW[2] = { 1, MarketId::XXXX };
static constexpr short d_YMM[2] = { 1, MarketId::XXXX };
static constexpr short d_YMQ[4] = { 3, MarketId::XMOC, MarketId::XMOD, MarketId::XMOO };
static constexpr short d_YNB[2] = { 1, MarketId::XXXX };
static constexpr short d_YOK[2] = { 1, MarketId::XXXX };
static constexpr short d_YOP[2] = { 1, MarketId::XXXX };
static constexpr short d_YOW[2] = { 1, MarketId::CAN0 };
static constexpr short d_YPN[2] = { 1, MarketId::XXXX };
static constexpr short d_YPR[2] = { 1, MarketId::XXXX };
static constexpr short d_YQB[2] = { 1, MarketId::XXXX };
static constexpr short d_YQD[2] = { 1, MarketId::XXXX };
static constexpr short d_YQG[2] = { 1, MarketId::XXXX };
static constexpr short d_YQM[2] = { 1, MarketId::XXXX };
static constexpr short d_YQR[2] = { 1, MarketId::XXXX };
static constexpr short d_YQT[2] = { 1, MarketId::XXXX };
static constexpr short d_YQX[2] = { 1, MarketId::XXXX };
static constexpr short d_YRB[2] = { 1, MarketId::XXXX };
static constexpr short d_YSJ[2] = { 1, MarketId::XXXX };
static constexpr short d_YSM[2] = { 1, MarketId::XXXX };
static constexpr short d_YSR[2] = { 1, MarketId::XXXX };
static constexpr short d_YTH[2] = { 1, MarketId::XXXX };
static constexpr short d_YTO[34] = { 33, MarketId::ADRK, MarketId::ATSA, MarketId::BNSX, MarketId::CAND, MarketId::CANX, MarketId::CHIC, MarketId::COTC, MarketId::CSE2, MarketId::EQCA, MarketId::IVZX, MarketId::LICA, MarketId::LYNX, MarketId::MATN, MarketId::NEOC, MarketId::NEOD, MarketId::NEOE, MarketId::NEON, MarketId::OMGA, MarketId::PURE, MarketId::TMXS, MarketId::VDRK, MarketId::XATS, MarketId::XATX, MarketId::XBBK, MarketId::XCNQ, MarketId::XCX2, MarketId::XCXD, MarketId::XDRK, MarketId::XICX, MarketId::XTFE, MarketId::XTOE, MarketId::XTSE, MarketId::XTSX };
static constexpr short d_YUD[2] = { 1, MarketId::XXXX };
static constexpr short d_YUM[2] = { 1, MarketId::XXXX };
static constexpr short d_YUX[2] = { 1, MarketId::XXXX };
static constexpr short d_YVB[2] = { 1, MarketId::XXXX };
static constexpr short d_YVO[2] = { 1, MarketId::XXXX };
static constexpr short d_YVP[2] = { 1, MarketId::XXXX };
static constexpr short d_YVQ[2] = { 1, MarketId::XXXX };
static constexpr short d_YVR[3] = { 2, MarketId::XTNX, MarketId::XVSE };
static constexpr short d_YWG[3] = { 2, MarketId::IFCA, MarketId::XWCE };
static constexpr short d_YWK[2] = { 1, MarketId::XXXX };
static constexpr short d_YXE[2] = { 1, MarketId::XXXX };
static constexpr short d_YXJ[2] = { 1, MarketId::XXXX };
static constexpr short d_YXN[2] = { 1, MarketId::XXXX };
static constexpr short d_YXS[2] = { 1, MarketId::XXXX };
static constexpr short d_YXT[2] = { 1, MarketId::XXXX };
static constexpr short d_YXU[2] = { 1, MarketId::XXXX };
static constexpr short d_YXY[2] = { 1, MarketId::XXXX };
static constexpr short d_YYC[3] = { 2, MarketId::NGXC, MarketId::XALB };
static constexpr short d_YYD[2] = { 1, MarketId::XXXX };
static constexpr short d_YYJ[2] = { 1, MarketId::XXXX };
static constexpr short d_YYN[2] = { 1, MarketId::XXXX };
static constexpr short d_YYQ[2] = { 1, MarketId::XXXX };
static constexpr short d_YYR[2] = { 1, MarketId::XXXX };
static constexpr short d_YYT[2] = { 1, MarketId::XXXX };
static constexpr short d_YYZ[2] = { 1, MarketId::XXXX };
static constexpr short d_YZF[2] = { 1, MarketId::XXXX };
static constexpr short d_YZP[2] = { 1, MarketId::XXXX };
static constexpr short d_ZAD[2] = { 1, MarketId::XXXX };
static constexpr short d_ZAG[8] = { 7, MarketId::HRV0, MarketId::XCRO, MarketId::XTRZ, MarketId::XZAG, MarketId::XZAM, MarketId::XZAP, MarketId::ZAPA };
static constexpr short d_ZAZ[2] = { 1, MarketId::IBER };
static constexpr short d_ZBO[2] = { 1, MarketId::XXXX };
static constexpr short d_ZCL[2] = { 1, MarketId::XXXX };
static constexpr short d_ZIH[2] = { 1, MarketId::XXXX };
static constexpr short d_ZKE[2] = { 1, MarketId::XXXX };
static constexpr short d_ZLO[2] = { 1, MarketId::XXXX };
static constexpr short d_ZND[2] = { 1, MarketId::XXXX };
static constexpr short d_ZNE[2] = { 1, MarketId::XXXX };
static constexpr short d_ZQN[2] = { 1, MarketId::XXXX };
static constexpr short d_ZRH[41] = { 40, MarketId::BXDA, MarketId::CBOE, MarketId::CSAG, MarketId::CSOT, MarketId::CSZH, MarketId::DOTS, MarketId::EBSC, MarketId::EBSS, MarketId::EUCH, MarketId::EURM, MarketId::EUSC, MarketId::EUSP, MarketId::GLBX, MarketId::KMUX, MarketId::ROSR, MarketId::RULE, MarketId::STOX, MarketId::UBSC, MarketId::UBSF, MarketId::UBSG, MarketId::UBST, MarketId::VLEX, MarketId::XBTR, MarketId::XDLP, MarketId::XICB, MarketId::XQMH, MarketId::XQOD, MarketId::XREP, MarketId::XROT, MarketId::XSCU, MarketId::XSDX, MarketId::XSEB, MarketId::XSLS, MarketId::XSTV, MarketId::XSTX, MarketId::XSWM, MarketId::XSWO, MarketId::XSWX, MarketId::XVTX, MarketId::ZKBX };
static constexpr short d_ZSA[2] = { 1, MarketId::XXXX };
static constexpr short d_ZSS[2] = { 1, MarketId::XXXX };
static constexpr short d_ZTH[2] = { 1, MarketId::XXXX };
static constexpr short d_ZTM[2] = { 1, MarketId::XXXX };
static constexpr short d_ZVK[2] = { 1, MarketId::XXXX };
static constexpr short d_ZYL[2] = { 1, MarketId::XXXX };

constexpr const short * Gazetteer::m_cty2mics[City::NUMCITY] = { 
    d_NOCITY,
    d_AAB, d_AAC, d_AAE, d_AAL, d_AAN, d_AAR, d_AAT, d_ABD, d_ABE, d_ABI, 
    d_ABJ, d_ABM, d_ABQ, d_ABR, d_ABS, d_ABV, d_ABX, d_ABY, d_ABZ, d_ACA, 
    d_ACC, d_ACE, d_ACH, d_ACI, d_ACK, d_ACT, d_ACV, d_ACY, d_ADA, d_ADD, 
    d_ADE, d_ADF, d_ADK, d_ADL, d_ADQ, d_ADY, d_ADZ, d_AEH, d_AER, d_AES, 
    d_AGA, d_AGB, d_AGP, d_AGS, d_AGU, d_AGZ, d_AHB, d_AHN, d_AHO, d_AHU, 
    d_AJA, d_AJF, d_AJN, d_AJU, d_AJY, d_AKL, d_AKN, d_AKT, d_ALA, d_ALB, 
    d_ALC, d_ALF, d_ALG, d_ALH, d_ALJ, d_ALO, d_ALP, d_ALV, d_ALW, d_ALY, 
    d_AMA, d_AMD, d_AMM, d_AMS, d_ANB, d_ANC, d_ANK, d_ANP, d_ANR, d_ANU, 
    d_AOI, d_AOJ, d_AOK, d_AOO, d_APF, d_APW, d_AQJ, d_ARB, d_ARH, d_ARK, 
    d_ASB, d_ASE, d_ASF, d_ASI, d_ASJ, d_ASK, d_ASM, d_ASP, d_ASR, d_ASU, 
    d_ASW, d_ATH, d_ATL, d_ATO, d_ATQ, d_ATW, d_ATY, d_ATZ, d_AUA, d_AUE, 
    d_AUG, d_AUH, d_AUR, d_AUS, d_AUX, d_AVL, d_AVP, d_AXA, d_AXS, d_AXT, 
    d_AY0, d_AYQ, d_AYR, d_AYT, d_AYW, d_AZB, d_AZO, d_AZS, d_BAH, d_BAK, 
    d_BAQ, d_BBI, d_BBM, d_BBR, d_BBT, d_BBU, d_BBY, d_BCN, d_BCO, d_BCT, 
    d_BCV, d_BDA, d_BDB, d_BDL, d_BDO, d_BDQ, d_BDR, d_BDS, d_BDU, d_BEB, 
    d_BEG, d_BEH, d_BEL, d_BEN, d_BER, d_BES, d_BET, d_BEW, d_BEY, d_BFD, 
    d_BFL, d_BFN, d_BFO, d_BFS, d_BGA, d_BGF, d_BGI, d_BGM, d_BGO, d_BGR, 
    d_BGU, d_BGW, d_BGY, d_BHE, d_BHM, d_BHO, d_BHQ, d_BHR, d_BHV, d_BHX, 
    d_BIA, d_BIL, d_BIO, d_BIQ, d_BIS, d_BIV, d_BJI, d_BJL, d_BJM, d_BJS, 
    d_BJV, d_BJX, d_BJZ, d_BKI, d_BKK, d_BKO, d_BKW, d_BKX, d_BL0, d_BLA, 
    d_BLF, d_BLI, d_BLK, d_BLL, d_BLQ, d_BLR, d_BLT, d_BLZ, d_BME, d_BMG, 
    d_BMI, d_BMP, d_BNA, d_BNE, d_BNJ, d_BNK, d_BNN, d_BNP, d_BNX, d_BOB, 
    d_BOD, d_BOG, d_BOH, d_BOI, d_BOJ, d_BOM, d_BON, d_BOO, d_BOS, d_BOY, 
    d_BPT, d_BQK, d_BQN, d_BRC, d_BRD, d_BRE, d_BRI, d_BRL, d_BRN, d_BRQ, 
    d_BRR, d_BRS, d_BRU, d_BSB, d_BSG, d_BSL, d_BSR, d_BTM, d_BTQ, d_BTR, 
    d_BTS, d_BTU, d_BTV, d_BUA, d_BUD, d_BUE, d_BUF, d_BUG, d_BUQ, d_BUR, 
    d_BUS, d_BVB, d_BWI, d_BWN, d_BWT, d_BXS, d_BYK, d_BYU, d_BZE, d_BZL, 
    d_BZN, d_BZV, d_CAB, d_CAE, d_CAG, d_CAI, d_CAK, d_CAL, d_CAN, d_CAY, 
    d_CBB, d_CBG, d_CBR, d_CBU, d_CCJ, d_CCK, d_CCR, d_CCS, d_CCU, d_CCZ, 
    d_CDC, d_CDV, d_CEB, d_CEC, d_CED, d_CEN, d_CEQ, d_CES, d_CFE, d_CFG, 
    d_CFN, d_CFS, d_CFU, d_CGA, d_CGB, d_CGN, d_CGO, d_CGP, d_CGQ, d_CGR, 
    d_CGU, d_CHA, d_CHC, d_CHG, d_CHI, d_CHO, d_CHQ, d_CHS, d_CHT, d_CIC, 
    d_CID, d_CIP, d_CIW, d_CJB, d_CJC, d_CJL, d_CJS, d_CKB, d_CKG, d_CKY, 
    d_CLD, d_CLE, d_CLJ, d_CLL, d_CLM, d_CLO, d_CLQ, d_CLT, d_CLY, d_CMB, 
    d_CME, d_CMF, d_CMH, d_CMI, d_CMN, d_CMQ, d_CMX, d_CND, d_CNF, d_CNQ, 
    d_CNS, d_CNX, d_COD, d_COK, d_CON, d_COO, d_COQ, d_COR, d_COS, d_CPD, 
    d_CPH, d_CPR, d_CPT, d_CRD, d_CRF, d_CRG, d_CRP, d_CRW, d_CSG, d_CSI, 
    d_CSN, d_CST, d_CT0, d_CTA, d_CTC, d_CTG, d_CTN, d_CTU, d_CUL, d_CUN, 
    d_CUR, d_CUU, d_CVG, d_CVM, d_CVQ, d_CVT, d_CWA, d_CWB, d_CWL, d_CXI, 
    d_CXT, d_CYS, d_CYU, d_CZA, d_CZL, d_CZM, d_DAB, d_DAC, d_DAL, d_DAM, 
    d_DAN, d_DAR, d_DAY, d_DBO, d_DBQ, d_DBV, d_DBY, d_DCA, d_DCF, d_DDI, 
    d_DEC, d_DEL, d_DEN, d_DET, d_DHA, d_DHN, d_DIL, d_DJB, d_DJE, d_DJJ, 
    d_DJO, d_DKI, d_DKR, d_DLA, d_DLC, d_DLG, d_DLH, d_DLM, d_DMM, d_DND, 
    d_DNK, d_DNR, d_DNZ, d_DOD, d_DOH, d_DOM, d_DOV, d_DPO, d_DPS, d_DRB, 
    d_DRO, d_DRS, d_DRW, d_DSK, d_DSM, d_DTM, d_DUB, d_DUD, d_DUJ, d_DUR, 
    d_DUS, d_DUT, d_DVL, d_DXB, d_DYA, d_DYU, d_DZA, d_EA0, d_EAS, d_EAT, 
    d_EAU, d_EB0, d_EBA, d_EBB, d_EBJ, d_EBU, d_EDI, d_EGC, d_EGE, d_EGS, 
    d_EIN, d_EIS, d_EKI, d_EKO, d_ELH, d_ELL, d_ELM, d_ELP, d_ELS, d_ELY, 
    d_EMA, d_EMD, d_EMY, d_ENA, d_ENC, d_ENF, d_EP0, d_EPR, d_ER0, d_ERC, 
    d_ERF, d_ERI, d_ERN, d_ERZ, d_ES0, d_ESC, d_ESF, d_ESH, d_ESR, d_ETH, 
    d_EUG, d_EUN, d_EVE, d_EVN, d_EVV, d_EWN, d_EWR, d_EXT, d_EYW, d_FAE, 
    d_FAI, d_FAO, d_FAR, d_FAT, d_FAY, d_FBM, d_FC0, d_FCA, d_FDF, d_FDH, 
    d_FEZ, d_FFT, d_FHU, d_FIE, d_FIH, d_FJR, d_FKB, d_FKI, d_FKL, d_FKS, 
    d_FLG, d_FLL, d_FLN, d_FLO, d_FLR, d_FMN, d_FMO, d_FMY, d_FNA, d_FNC, 
    d_FNI, d_FNJ, d_FNT, d_FOD, d_FOE, d_FOR, d_FOU, d_FPO, d_FRA, d_FRI, 
    d_FRJ, d_FRO, d_FRU, d_FRW, d_FSC, d_FSD, d_FSM, d_FSP, d_FUE, d_FUK, 
    d_FUN, d_FUT, d_FWA, d_FYV, d_GAD, d_GAJ, d_GAU, d_GBE, d_GC0, d_GCC, 
    d_GCI, d_GCM, d_GCN, d_GDL, d_GDN, d_GDT, d_GDV, d_GEG, d_GEO, d_GET, 
    d_GEX, d_GFF, d_GFK, d_GGG, d_GGW, d_GH0, d_GHB, d_GIB, d_GID, d_GIL, 
    d_GJL, d_GJT, d_GKL, d_GLA, d_GLH, d_GLT, d_GNB, d_GND, d_GNE, d_GNV, 
    d_GOA, d_GOH, d_GOI, d_GOJ, d_GON, d_GOO, d_GOT, d_GOU, d_GOV, d_GOZ, 
    d_GPA, d_GPT, d_GPZ, d_GR0, d_GRB, d_GRJ, d_GRO, d_GRQ, d_GRR, d_GRX, 
    d_GRZ, d_GS0, d_GSI, d_GSO, d_GSP, d_GTE, d_GTF, d_GTI, d_GUA, d_GUC, 
    d_GUM, d_GVA, d_GW0, d_GWD, d_GWE, d_GWT, d_GWY, d_GYE, d_GYI, d_GYN, 
    d_GYP, d_GZA, d_GZT, d_HAC, d_HAG, d_HAH, d_HAJ, d_HAM, d_HAN, d_HAR, 
    d_HAU, d_HAV, d_HBA, d_HDD, d_HDN, d_HDY, d_HEA, d_HEL, d_HER, d_HFA, 
    d_HFT, d_HGA, d_HGH, d_HGO, d_HHH, d_HIB, d_HII, d_HIJ, d_HIR, d_HIS, 
    d_HKD, d_HKG, d_HKT, d_HKY, d_HLA, d_HLH, d_HLN, d_HLT, d_HLZ, d_HMO, 
    d_HNA, d_HNH, d_HNK, d_HNL, d_HNS, d_HOG, d_HOM, d_HON, d_HOQ, d_HOR, 
    d_HOU, d_HPN, d_HRB, d_HRE, d_HRG, d_HRK, d_HRL, d_HS0, d_HSV, d_HTA, 
    d_HTI, d_HTS, d_HTV, d_HUF, d_HUH, d_HUI, d_HUX, d_HUY, d_HVB, d_HVN, 
    d_HVR, d_HWN, d_HYA, d_HYD, d_HYG, d_IAG, d_IBZ, d_ICN, d_ICT, d_IDA, 
    d_IDR, d_IEV, d_IFL, d_IFP, d_IGH, d_IGR, d_IJX, d_IKT, d_ILE, d_ILG, 
    d_ILI, d_ILM, d_ILP, d_ILY, d_ILZ, d_IND, d_INI, d_INL, d_INN, d_INU, 
    d_INV, d_IOM, d_IOU, d_IPC, d_IPL, d_IPT, d_IQT, d_IRG, d_IRO, d_ISA, 
    d_ISB, d_ISG, d_ISL, d_ISO, d_ISP, d_IST, d_ITH, d_ITO, d_IUE, d_IVC, 
    d_IVL, d_IXB, d_IXC, d_IXG, d_IXJ, d_IXR, d_IXW, d_IYK, d_IZM, d_JAA, 
    d_JAC, d_JAD, d_JAF, d_JAG, d_JAI, d_JAK, d_JAL, d_JAM, d_JAN, d_JAU, 
    d_JBR, d_JCB, d_JCK, d_JCM, d_JDF, d_JDH, d_JDZ, d_JED, d_JEE, d_JEF, 
    d_JER, d_JGA, d_JGB, d_JGN, d_JGS, d_JHB, d_JHE, d_JHG, d_JHM, d_JHQ, 
    d_JHW, d_JIB, d_JIJ, d_JIL, d_JIM, d_JIN, d_JIP, d_JIR, d_JIU, d_JIW, 
    d_JJI, d_JJN, d_JKG, d_JKH, d_JKR, d_JKT, d_JKV, d_JLN, d_JLR, d_JLS, 
    d_JMB, d_JMK, d_JMO, d_JMS, d_JMU, d_JNA, d_JNB, d_JNG, d_JNI, d_JNU, 
    d_JNX, d_JNZ, d_JOE, d_JOI, d_JOL, d_JON, d_JOS, d_JPA, d_JQE, d_JRH, 
    d_JRO, d_JRS, d_JSA, d_JSI, d_JSM, d_JST, d_JTI, d_JTR, d_JUB, d_JUI, 
    d_JUJ, d_JUL, d_JUN, d_JUT, d_JUV, d_JVL, d_JWA, d_JXN, d_JYV, d_KAJ, 
    d_KAN, d_KAO, d_KBL, d_KBY, d_KCC, d_KCE, d_KCH, d_KCM, d_KCZ, d_KDD, 
    d_KDH, d_KDU, d_KEL, d_KEM, d_KEN, d_KGC, d_KGD, d_KGI, d_KGL, d_KGS, 
    d_KHH, d_KHI, d_KHJ, d_KHV, d_KID, d_KIJ, d_KIK, d_KIM, d_KIN, d_KIR, 
    d_KIV, d_KIW, d_KKN, d_KLA, d_KLR, d_KLU, d_KLW, d_KLX, d_KLZ, d_KMI, 
    d_KMJ, d_KMP, d_KMQ, d_KMS, d_KNS, d_KNU, d_KNX, d_KOA, d_KOI, d_KOJ, 
    d_KOK, d_KRB, d_KRK, d_KRN, d_KRP, d_KRS, d_KRT, d_KSD, d_KSL, d_KSS, 
    d_KSU, d_KTA, d_KTB, d_KTM, d_KTN, d_KTR, d_KTT, d_KUA, d_KUF, d_KUH, 
    d_KUL, d_KUO, d_KVA, d_KWI, d_KWL, d_KWM, d_KYA, d_KYS, d_KZN, d_LAD, 
    d_LAE, d_LAF, d_LAI, d_LAN, d_LAP, d_LAR, d_LAS, d_LAW, d_LAX, d_LBA, 
    d_LBB, d_LBE, d_LBI, d_LBQ, d_LBU, d_LBV, d_LCA, d_LCG, d_LCH, d_LDC, 
    d_LDE, d_LDK, d_LDY, d_LEB, d_LED, d_LEI, d_LEJ, d_LEK, d_LEQ, d_LER, 
    d_LEX, d_LEY, d_LFI, d_LFT, d_LFW, d_LGB, d_LGG, d_LGK, d_LHE, d_LI0, 
    d_LIF, d_LIG, d_LIH, d_LIL, d_LIM, d_LIQ, d_LIS, d_LIT, d_LJU, d_LKL, 
    d_LKO, d_LL0, d_LLA, d_LLW, d_LMM, d_LMP, d_LMT, d_LNK, d_LNO, d_LNS, 
    d_LNY, d_LNZ, d_LON, d_LOS, d_LPA, d_LPB, d_LPL, d_LPP, d_LRD, d_LRE, 
    d_LRF, d_LRH, d_LRM, d_LRT, d_LSE, d_LSI, d_LST, d_LSY, d_LSZ, d_LTO, 
    d_LU0, d_LUD, d_LUG, d_LUJ, d_LUM, d_LUN, d_LUQ, d_LUX, d_LV0, d_LVO, 
    d_LWB, d_LWK, d_LWO, d_LWS, d_LWT, d_LXR, d_LYH, d_LYO, d_LYP, d_LYR, 
    d_LYS, d_LYX, d_LZC, d_MAA, d_MAD, d_MAF, d_MAH, d_MAJ, d_MAM, d_MAN, 
    d_MAO, d_MAR, d_MAU, d_MAZ, d_MBA, d_MBH, d_MBJ, d_MBM, d_MBS, d_MBX, 
    d_MC0, d_MCE, d_MCI, d_MCL, d_MCM, d_MCN, d_MCO, d_MCP, d_MCT, d_MCW, 
    d_MCY, d_MCZ, d_MDC, d_MDE, d_MDL, d_MDQ, d_MDY, d_MDZ, d_MED, d_MEE, 
    d_MEI, d_MEL, d_MEM, d_MES, d_MEX, d_MEZ, d_MFE, d_MFF, d_MFG, d_MFM, 
    d_MFN, d_MFO, d_MFQ, d_MFR, d_MFU, d_MGA, d_MGB, d_MGF, d_MGH, d_MGM, 
    d_MGQ, d_MGW, d_MHE, d_MHH, d_MHQ, d_MHT, d_MIA, d_MID, d_MIK, d_MIL, 
    d_MIM, d_MIR, d_MJC, d_MJD, d_MJL, d_MJN, d_MJQ, d_MJT, d_MJV, d_MKC, 
    d_MKE, d_MKG, d_MKK, d_MKL, d_MKR, d_MKY, d_MLA, d_MLB, d_MLE, d_MLH, 
    d_MLI, d_MLM, d_MLS, d_MLU, d_MLW, d_MLX, d_MMA, d_MME, d_MMG, d_MMJ, 
    d_MMK, d_MMM, d_MMY, d_MNI, d_MNL, d_MOB, d_MOD, d_MON, d_MOT, d_MOV, 
    d_MOW, d_MOZ, d_MPA, d_MPD, d_MPL, d_MPM, d_MPV, d_MQL, d_MQM, d_MQQ, 
    d_MQT, d_MRB, d_MRS, d_MRU, d_MRV, d_MRY, d_MRZ, d_MSE, d_MSL, d_MSN, 
    d_MSO, d_MSP, d_MSQ, d_MSR, d_MST, d_MSU, d_MSY, d_MTH, d_MTJ, d_MTL, 
    d_MTM, d_MTO, d_MTS, d_MTT, d_MTY, d_MUB, d_MUC, d_MUE, d_MUH, d_MUX, 
    d_MV0, d_MVB, d_MVD, d_MVR, d_MVY, d_MVZ, d_MWD, d_MWH, d_MXL, d_MYA, 
    d_MYD, d_MYJ, d_MYQ, d_MYR, d_MYY, d_MZG, d_MZM, d_MZR, d_MZT, d_MZY, 
    d_NAA, d_NAG, d_NAJ, d_NAN, d_NAP, d_NAS, d_NAT, d_NBO, d_NCE, d_NCL, 
    d_NCS, d_NCY, d_NDB, d_NDJ, d_NDU, d_NEV, d_NGE, d_NGO, d_NGS, d_NIC, 
    d_NIM, d_NKC, d_NKW, d_NLA, d_NLD, d_NLK, d_NLP, d_NLV, d_NNG, d_NOC, 
    d_NOU, d_NQY, d_NRA, d_NRK, d_NS0, d_NSA, d_NSN, d_NSO, d_NST, d_NTE, 
    d_NTL, d_NTY, d_NUE, d_NWI, d_NYC, d_OAG, d_OAJ, d_OAK, d_OAX, d_OB0, 
    d_ODA, d_ODB, d_ODE, d_ODS, d_OGG, d_OHD, d_OHT, d_OIT, d_OKA, d_OKC, 
    d_OKD, d_OKJ, d_OKU, d_OLB, d_OLF, d_OLM, d_OLP, d_OMA, d_OMD, d_OME, 
    d_OMH, d_OMO, d_OND, d_ONT, d_OOL, d_OOM, d_OPO, d_ORB, d_ORF, d_ORH, 
    d_ORK, d_ORN, d_OSA, d_OSH, d_OSI, d_OSL, d_OSM, d_OSR, d_OTH, d_OTZ, 
    d_OUA, d_OUD, d_OUH, d_OUK, d_OUL, d_OUZ, d_OVB, d_OVD, d_OWB, d_OXB, 
    d_OXR, d_OZH, d_OZZ, d_PA0, d_PAC, d_PAD, d_PAH, d_PAP, d_PAR, d_PAT, 
    d_PB0, d_PBC, d_PBH, d_PBI, d_PBM, d_PBO, d_PBZ, d_PC0, d_PCT, d_PDL, 
    d_PDT, d_PDX, d_PEG, d_PEI, d_PEN, d_PER, d_PEW, d_PFN, d_PFO, d_PGA, 
    d_PGF, d_PGV, d_PHC, d_PHE, d_PHF, d_PHH, d_PHL, d_PHW, d_PHX, d_PIA, 
    d_PIB, d_PIH, d_PIR, d_PIS, d_PIT, d_PJG, d_PKB, d_PKW, d_PLB, d_PLN, 
    d_PLO, d_PLZ, d_PMD, d_PMI, d_PMO, d_PMR, d_PMV, d_PMW, d_PN0, d_PNH, 
    d_PNI, d_PNL, d_PNQ, d_PNR, d_PNS, d_POA, d_POG, d_POM, d_POP, d_POR, 
    d_POS, d_POU, d_POZ, d_PPG, d_PPP, d_PPT, d_PQI, d_PQQ, d_PRG, d_PRN, 
    d_PRY, d_PS0, d_PSA, d_PSC, d_PSD, d_PSE, d_PSG, d_PSI, d_PSP, d_PSR, 
    d_PSY, d_PT0, d_PTG, d_PTJ, d_PUB, d_PUF, d_PUG, d_PUJ, d_PUQ, d_PUS, 
    d_PUW, d_PUY, d_PVD, d_PVH, d_PVK, d_PVR, d_PWM, d_PXM, d_PXO, d_PYB, 
    d_PYX, d_PZB, d_PZH, d_PZO, d_QCB, d_QGF, d_QIC, d_QLI, d_QML, d_QMN, 
    d_QNB, d_QND, d_QPA, d_QSA, d_RA0, d_RAI, d_RAJ, d_RAK, d_RAP, d_RAR, 
    d_RAZ, d_RB0, d_RBA, d_RBR, d_RCB, d_RDD, d_RDG, d_RDM, d_RDU, d_RDZ, 
    d_REC, d_REG, d_REK, d_REU, d_RFD, d_RFP, d_RGI, d_RGL, d_RGN, d_RHG, 
    d_RHI, d_RHO, d_RIC, d_RIO, d_RIX, d_RIZ, d_RJK, d_RKD, d_RKS, d_RKT, 
    d_RLT, d_RMF, d_RMI, d_RNB, d_RNE, d_RNN, d_RNO, d_RNS, d_ROA, d_ROC, 
    d_ROK, d_ROM, d_ROR, d_ROS, d_ROT, d_ROU, d_ROV, d_RR0, d_RRG, d_RS0, 
    d_RSA, d_RSD, d_RST, d_RTB, d_RTM, d_RUH, d_RUN, d_RVN, d_RWI, d_RWP, 
    d_RYK, d_SAF, d_SAH, d_SAI, d_SAL, d_SAN, d_SAO, d_SAP, d_SAT, d_SAV, 
    d_SBA, d_SBH, d_SBK, d_SBN, d_SBP, d_SBU, d_SBW, d_SBY, d_SBZ, d_SCC, 
    d_SCE, d_SCF, d_SCK, d_SCN, d_SCQ, d_SCU, d_SDF, d_SDH, d_SDJ, d_SDL, 
    d_SDQ, d_SDR, d_SDS, d_SDT, d_SDY, d_SEA, d_SEB, d_SEL, d_SEN, d_SEW, 
    d_SEZ, d_SFA, d_SFG, d_SFJ, d_SFN, d_SFO, d_SGD, d_SGF, d_SGN, d_SGU, 
    d_SGY, d_SH0, d_SHA, d_SHD, d_SHE, d_SHJ, d_SHR, d_SHS, d_SHV, d_SI0, 
    d_SID, d_SIN, d_SIP, d_SIS, d_SIT, d_SIX, d_SJC, d_SJD, d_SJJ, d_SJO, 
    d_SJT, d_SJU, d_SJY, d_SK0, d_SKB, d_SKD, d_SKG, d_SKP, d_SKS, d_SKV, 
    d_SKX, d_SKZ, d_SLA, d_SLC, d_SLE, d_SLL, d_SLP, d_SLS, d_SLU, d_SLZ, 
    d_SM0, d_SMA, d_SMF, d_SMI, d_SMX, d_SNA, d_SNC, d_SNN, d_SNS, d_SOF, 
    d_SOG, d_SON, d_SOT, d_SOU, d_SPC, d_SPI, d_SPN, d_SPS, d_SPU, d_SPY, 
    d_SRA, d_SRB, d_SRL, d_SRQ, d_SRZ, d_SSA, d_SSG, d_SSH, d_SSL, d_ST0, 
    d_STI, d_STL, d_STO, d_STP, d_STR, d_STS, d_STT, d_STV, d_STX, d_SUB, 
    d_SUF, d_SUL, d_SUN, d_SUV, d_SUX, d_SVD, d_SVG, d_SVL, d_SVQ, d_SVX, 
    d_SW0, d_SWF, d_SWP, d_SWS, d_SXB, d_SXL, d_SXM, d_SXR, d_SYD, d_SYR, 
    d_SYY, d_SZD, d_SZF, d_SZG, d_SZK, d_SZX, d_SZZ, d_TAB, d_TAK, d_TAM, 
    d_TAO, d_TAS, d_TAY, d_TBO, d_TBS, d_TBU, d_TC0, d_TCA, d_TCB, d_TCI, 
    d_TCL, d_TCU, d_TED, d_TEM, d_TEQ, d_TER, d_TEU, d_TEX, d_TF0, d_TGD, 
    d_TGU, d_TGV, d_TGZ, d_THE, d_THR, d_THU, d_TIA, d_TIF, d_TIJ, d_TIP, 
    d_TIS, d_TIV, d_TK0, d_TKA, d_TKS, d_TKU, d_TLH, d_TLL, d_TLS, d_TLV, 
    d_TML, d_TMP, d_TMS, d_TMW, d_TMZ, d_TNA, d_TNG, d_TNR, d_TOD, d_TOL, 
    d_TOS, d_TOU, d_TOV, d_TOY, d_TPA, d_TPR, d_TPS, d_TRD, d_TRI, d_TRN, 
    d_TRO, d_TRS, d_TRV, d_TRW, d_TRZ, d_TSB, d_TSE, d_TSF, d_TSN, d_TSV, 
    d_TTN, d_TUC, d_TUK, d_TUL, d_TUN, d_TUP, d_TUS, d_TUU, d_TVC, d_TVF, 
    d_TVL, d_TWB, d_TWF, d_TWU, d_TXK, d_TYN, d_TYO, d_TYR, d_TYS, d_TZX, 
    d_UAH, d_UAK, d_UAP, d_UBA, d_UBJ, d_UBP, d_UCA, d_UCT, d_UDE, d_UDI, 
    d_UDJ, d_UDR, d_UEE, d_UET, d_UFA, d_UGC, d_UGO, d_UHE, d_UII, d_UIN, 
    d_UIO, d_UIP, d_UIT, d_UKB, d_UKI, d_UKY, d_ULB, d_ULD, d_ULN, d_ULU, 
    d_ULY, d_UMD, d_UME, d_UMR, d_UNI, d_UNK, d_UNT, d_UPG, d_UPL, d_UPN, 
    d_UPP, d_URC, d_URG, d_URM, d_USH, d_USN, d_UTC, d_UTH, d_UTN, d_UTP, 
    d_UTT, d_UUD, d_UVE, d_UVF, d_UVL, d_VA0, d_VAA, d_VAN, d_VAP, d_VAR, 
    d_VAS, d_VBS, d_VBY, d_VCE, d_VDE, d_VDZ, d_VEL, d_VER, d_VFA, d_VGO, 
    d_VID, d_VIE, d_VIJ, d_VIS, d_VIT, d_VIX, d_VLC, d_VLD, d_VLI, d_VLL, 
    d_VLN, d_VLU, d_VN0, d_VNO, d_VNS, d_VOG, d_VPS, d_VRA, d_VRB, d_VRK, 
    d_VRN, d_VSA, d_VST, d_VTE, d_VVO, d_VXO, d_VY0, d_VYD, d_VZ0, d_WAM, 
    d_WAW, d_WDH, d_WEI, d_WEL, d_WGA, d_WHK, d_WIC, d_WIE, d_WLB, d_WLG, 
    d_WLS, d_WMB, d_WN0, d_WNS, d_WOL, d_WP0, d_WRE, d_WRG, d_WRL, d_WRO, 
    d_WSY, d_WUH, d_WUN, d_WUX, d_WVB, d_WYA, d_WYN, d_WYS, d_XCH, d_XIY, 
    d_XMH, d_XMN, d_XPK, d_XRY, d_XSI, d_XXX, d_YAK, d_YAO, d_YAT, d_YBE, 
    d_YCB, d_YDF, d_YEA, d_YEV, d_YFA, d_YFB, d_YFC, d_YFO, d_YGW, d_YGX, 
    d_YHR, d_YHZ, d_YIF, d_YIH, d_YKA, d_YKM, d_YKS, d_YLB, d_YLR, d_YLW, 
    d_YMM, d_YMQ, d_YNB, d_YOK, d_YOP, d_YOW, d_YPN, d_YPR, d_YQB, d_YQD, 
    d_YQG, d_YQM, d_YQR, d_YQT, d_YQX, d_YRB, d_YSJ, d_YSM, d_YSR, d_YTH, 
    d_YTO, d_YUD, d_YUM, d_YUX, d_YVB, d_YVO, d_YVP, d_YVQ, d_YVR, d_YWG, 
    d_YWK, d_YXE, d_YXJ, d_YXN, d_YXS, d_YXT, d_YXU, d_YXY, d_YYC, d_YYD, 
    d_YYJ, d_YYN, d_YYQ, d_YYR, d_YYT, d_YYZ, d_YZF, d_YZP, d_ZAD, d_ZAG, 
    d_ZAZ, d_ZBO, d_ZCL, d_ZIH, d_ZKE, d_ZLO, d_ZND, d_ZNE, d_ZQN, d_ZRH, 
    d_ZSA, d_ZSS, d_ZTH, d_ZTM, d_ZVK, d_ZYL    
};

// 


