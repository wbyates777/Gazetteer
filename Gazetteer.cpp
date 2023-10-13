/* Gazetteer 15/03/2012.
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Gazetteer.cpp - code   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History:  
 
 This code depends on the fact that City, MarketId, Currency and Country are all represented as a short
 
 Occupies about 500k as .o file
 
 Although these tables are generated automatically one should be aware of a number of issues:
 
 i)   Countries that no longer exist
 ii)  Currencies that are no longer valid
 iii) Currency Unions
 iv)  Dependant Territories
 
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
Currency // the main/principle ccy for this country
Gazetteer::ccy( const Country& cid ) const
{
    return Currency::CurrencyCode(m_cid2ccy[Country::index(cid)]);
}

std::vector<Currency>
Gazetteer::ccys( const Country& cid ) const
{ 
    // NB the cast here depends on Country using short as an underlying type
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
Gazetteer::city( const Country& cid ) const
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
Gazetteer::market( const City& cty ) const
{
    const MarketId *market = reinterpret_cast<const MarketId*>(m_cty2mics[City::index(cty)]);
    return std::vector<MarketId>(market + 1, market + 1 + *market);
}


std::vector<MarketId>
Gazetteer::market( const Country& cid ) const
{
    std::vector<City> cty = city( cid );

    std::vector<MarketId> retVal;
    for (int i = 0; i < cty.size(); ++i)
    {
        std::vector<MarketId> m = market( cty[i] );
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
        case NOREGION: index = 0; break;
        case AntarcticRegion: index = 1; break;
        case Africa: index = 2; break;
        case Oceania: index = 3; break;
        case Americas: index = 4; break;
        case Asia: index = 5; break;
        case Europe: index = 6; break;
        default: index = 0; break;
    }

    const Country *cid = reinterpret_cast<const Country*>(m_reg2cid[index]);
    return std::vector<Country>(cid + 1, cid + 1 + *cid);
}

std::vector<Gazetteer::Region>
Gazetteer::region( void ) const
{
    return std::vector<Region>({ AntarcticRegion, Africa, Oceania, Americas, Asia, Europe  });   
}

std::string
Gazetteer::regionName( Region r ) const 
{
    switch (r)
    {
        case NOREGION: return "No Region"; break;
        case AntarcticRegion: return "Antarctic Region"; break;
        case Africa: return "Africa"; break;
        case Oceania: return "Oceania"; break;
        case Americas: return "Americas"; break;
        case Asia: return "Asia"; break;
        case Europe: return "Europe"; break;
        default: return "No Region"; break;
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
std::vector<Gazetteer::SubRegion>
Gazetteer::subRegion( const Region rid ) const
{
    int index = 0;
    switch (rid)
    {
        case NOREGION: index = 0; break;
        case AntarcticRegion: index = 1; break;
        case Africa: index = 2; break;
        case Oceania: index = 3; break;
        case Americas: index = 4; break;
        case Asia: index = 5; break;
        case Europe: index = 6; break;
        default: index = 0; break;
    }
    
    const SubRegion *subr = reinterpret_cast<const SubRegion*>(m_reg2subreg[index]);
    return std::vector<SubRegion>(subr + 1, subr + 1 + *subr);
}

std::vector<Country>
Gazetteer::subRegion( const SubRegion rid ) const
{
    int index = 0;
    switch (rid)
    {
        case NOSUBREGION: index = 0; break;
        case AntarcticSubregion : index = 1; break;
        case SouthAmerica: index = 2; break;
        case WesternAfrica: index = 3; break;
        case CentralAmerica: index = 4; break;
        case EasternAfrica: index = 5; break;
        case NorthernAfrica: index = 6; break;
        case MiddleAfrica: index = 7; break;
        case SouthernAfrica: index = 8; break;
        case NorthernAmerica: index = 9; break;
        case Caribbean: index = 10; break;
        case EasternAsia: index = 11; break;
        case SouthernAsia: index = 12; break;
        case SouthEasternAsia: index = 13; break;
        case SouthernEurope: index = 14; break;
        case AustraliaNewZealand: index = 15; break;
        case Melanesia: index = 16; break;
        case Micronesia: index = 17; break;
        case Polynesia: index = 18; break;            
        case CentralAsia: index = 19; break;
        case WesternAsia: index = 20; break;
        case EasternEurope: index = 21; break;
        case NorthernEurope: index = 22; break;
        case WesternEurope: index = 23; break;
        default: index = 0; break;
    }

    const Country *cid = reinterpret_cast<const Country*>(m_subreg2cid[index]);
    return std::vector<Country>(cid + 1, cid + 1 + *cid);
}

std::vector<Gazetteer::SubRegion>
Gazetteer::subRegion( void ) const
{
    return std::vector<SubRegion>({
        AntarcticSubregion,
        SouthAmerica,
        WesternAfrica,
        CentralAmerica,
        EasternAfrica,
        NorthernAfrica,
        MiddleAfrica,
        SouthernAfrica,
        NorthernAmerica,
        Caribbean,
        EasternAsia,
        SouthernAsia,
        SouthEasternAsia,
        SouthernEurope,
        AustraliaNewZealand,
        Melanesia,
        Micronesia,
        Polynesia,            
        CentralAsia,
        WesternAsia,
        EasternEurope,
        NorthernEurope,
        WesternEurope
    });   
}

std::string
Gazetteer::subRegionName( SubRegion sr ) const 
{
    switch (sr)
    {
        case NOSUBREGION: return "No Subregion"; break;
        case AntarcticSubregion : return "Antarctic Subregion"; break; // move this to end; least likely subregion
        case SouthAmerica: return "South America"; break;
        case WesternAfrica: return "Western Africa"; break;
        case CentralAmerica: return "Central America"; break;
        case EasternAfrica: return "Eastern Africa"; break;
        case NorthernAfrica: return "Northern Africa"; break;
        case MiddleAfrica: return "Middle Africa"; break;
        case SouthernAfrica: return "Southern Africa"; break;
        case NorthernAmerica: return "Northern America"; break;
        case Caribbean: return "Caribbean"; break;
        case EasternAsia: return "Eastern Asia"; break;
        case SouthernAsia: return "Southern Asia"; break;
        case SouthEasternAsia: return "South-Eastern Asia"; break;
        case SouthernEurope: return "Southern Europe"; break;
        case AustraliaNewZealand: return "Australia and New Zealand"; break;
        case Melanesia: return "Melanesia"; break;
        case Micronesia: return "Micronesia"; break;
        case Polynesia: return "Polynesia"; break;            
        case CentralAsia: return "Central Asia"; break;
        case WesternAsia: return "Western Asia"; break;
        case EasternEurope: return "Eastern Europe"; break;
        case NorthernEurope: return "Northern Europe"; break;
        case WesternEurope: return "Western Europe"; break;
        default: return "No Subregion"; break;
    }    
}

Gazetteer::SubRegion
Gazetteer::subRegion( const Country& cid ) const 
{
    return SubRegion(m_subregion[Country::index(cid)]);    
}

Gazetteer::SubRegion
Gazetteer::subRegion( const City& cty ) const 
{    
    return SubRegion(m_subregion[Country::index(country(cty))]);
}

Gazetteer::SubRegion
Gazetteer::subRegion( const MarketId& mic ) const 
{    
    return SubRegion(m_subregion[Country::index(country(mic))]);
}


//
// Tables
//

// MarketId to City
const short Gazetteer::m_mic2cty[MarketId::NUMMARKETID] = { 
City::NOCITY,
City::YHM, City::JNB, City::LON, City::FRA, City::JNB, City::PAR, City::ATH, City::EWR, City::MAD, City::ATH, 
City::AMS, City::AMS, City::CPH, City::SOF, City::AUA, City::AUH, City::BOM, City::NYC, City::AUH, City::AUH, 
City::YTZ, City::NYC, City::LON, City::BKK, City::EB0, City::KBL, City::AMS, City::AMS, City::AMS, City::AMS, 
City::AMS, City::AMS, City::LON, City::MAD, City::LAD, City::AXA, City::DUB, City::LON, City::BRN, City::TSE, 
City::MIL, City::MHQ, City::TIA, City::NYC, City::LON, City::MAD, City::CPH, City::JNB, City::AMS, City::BRU, 
City::LIS, City::PAR, City::LAX, City::AMM, City::LON, City::LON, City::NYC, City::AD0, City::LON, City::LON, 
City::LON, City::VIE, City::SIN, City::SIN, City::SYD, City::PAR, City::PAR, City::PAR, City::LON, City::LON, 
City::LON, City::LON, City::LON, City::LON, City::LON, City::NYC, City::LON, City::LON, City::LON, City::LON, 
City::NYC, City::LON, City::NYC, City::NYC, City::LON, City::AUH, City::DUB, City::AEP, City::LON, City::NYC, 
City::EVN, City::LI0, City::IAH, City::ATH, City::PPG, City::ST0, City::ST0, City::ST0, City::BOM, City::SYD, 
City::SYD, City::SYD, City::SYD, City::SYD, City::PRG, City::CHS, City::TF0, City::MIL, City::ANU, City::NIC, 
City::LON, City::NIC, City::YTZ, City::PAR, City::PAR, City::CBR, City::VIE, City::LON, City::FRA, City::LON, 
City::LON, City::MEL, City::SYD, City::HEL, City::GYD, City::MUC, City::AEP, City::HKG, City::HKG, City::LON, 
City::HKG, City::TYO, City::BGI, City::LON, City::NYC, City::NYC, City::NYC, City::LON, City::LON, City::AMS, 
City::VNO, City::NYC, City::LON, City::NYC, City::LON, City::LON, City::AMS, City::NYC, City::HKG, City::LON, 
City::HKG, City::TYO, City::LON, City::LON, City::LON, City::CHI, City::LON, City::CHI, City::CHI, City::DUB, 
City::DUB, City::SIN, City::NYC, City::NYC, City::LON, City::LON, City::MAD, City::LON, City::NYC, City::LUX, 
City::SFN, City::PMI, City::MGF, City::LON, City::LM0, City::MSQ, City::LON, City::LON, City::LON, City::MUC, 
City::BJM, City::LUX, City::SOF, City::BRU, City::SOF, City::BKK, City::QPA, City::BRU, City::BRU, City::BRU, 
City::COO, City::BER, City::BER, City::BER, City::BON, City::BUD, City::TYO, City::LON, City::AMS, City::AMS, 
City::AMS, City::AMS, City::AMS, City::AMS, City::OUA, City::BAH, City::LIS, City::SYD, City::LON, City::NYC, 
City::NYC, City::LON, City::TYO, City::LON, City::LON, City::DAC, City::MIL, City::HAM, City::LON, City::HAM, 
City::SOF, City::LUX, City::SOF, City::SIN, City::HAM, City::LON, City::BAH, City::NAS, City::NYC, City::WAW, 
City::NYC, City::SJJ, City::XXX, City::LUX, City::FRA, City::LON, City::MEX, City::BJS, City::BRU, City::BRU, 
City::DUB, City::ATL, City::LON, City::LON, City::KLU, City::MUC, City::BNX, City::MUC, City::MUC, City::MEL, 
City::MUC, City::MUC, City::NYC, City::SBH, City::LON, City::LON, City::BRU, City::MSQ, City::NYC, City::LON, 
City::NYC, City::LUX, City::SYD, City::BCV, City::MAD, City::LON, City::MAD, City::MAD, City::SBZ, City::SBZ, 
City::SBZ, City::LON, City::DUB, City::DUB, City::DUB, City::DUB, City::LON, City::BRU, City::YHM, City::NYC, 
City::NYC, City::ROM, City::PAR, City::NYC, City::BRU, City::NYC, City::LON, City::WAW, City::PAR, City::TYO, 
City::YTZ, City::LON, City::NYC, City::LON, City::LON, City::FRA, City::EDI, City::LPB, City::ROM, City::LON, 
City::NYC, City::WAW, City::LON, City::GBE, City::GBE, City::VPS, City::VIX, City::GOA, City::WAW, City::LON, 
City::NYC, City::PAR, City::BSB, City::BGI, City::BBU, City::BBU, City::BBU, City::TRN, City::PAR, City::LON, 
City::LON, City::GRU, City::BBU, City::BWN, City::LON, City::QSA, City::GYD, City::MUC, City::LON, City::BOM, 
City::LON, City::BOS, City::AMS, City::SIN, City::EWR, City::LON, City::NYC, City::AMS, City::LON, City::PBH, 
City::CHI, City::AMS, City::LON, City::CLJ, City::TUN, City::OSL, City::OSL, City::OSL, City::LON, City::CCS, 
City::GRU, City::LON, City::MVD, City::NYC, City::GBE, City::ZRH, City::CHI, City::CHI, City::CHI, City::BCN, 
City::AMS, City::NYC, City::BGF, City::NYC, City::YOW, City::YTZ, City::YTZ, City::AMS, City::MAD, City::WRO, 
City::BMA, City::CHI, City::FRA, City::AMS, City::NYC, City::LON, City::AMS, City::LON, City::FRA, City::FRA, 
City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::NYC, City::LON, City::ZRH, City::BTS, City::CHI, 
City::CHI, City::LON, City::TSE, City::CHI, City::SHA, City::CCK, City::LUX, City::LON, City::GVA, City::NYC, 
City::LON, City::AMS, City::AMS, City::NYC, City::NYC, City::FRA, City::BOM, City::MAD, City::NYC, City::AMS, 
City::PRG, City::DUB, City::LON, City::PRG, City::RIO, City::WAW, City::AMS, City::AMS, City::AMS, City::AMS, 
City::SYD, City::SHA, City::HKG, City::LON, City::LCA, City::DUB, City::NYC, City::TYO, City::TSA, City::ROM, 
City::ROM, City::ROM, City::FRA, City::FRA, City::FRA, City::ROM, City::ROM, City::SYD, City::LON, City::FRA, 
City::LON, City::LON, City::HKG, City::NYC, City::LON, City::LON, City::LON, City::MEX, City::ROM, City::ROM, 
City::ROM, City::ROM, City::NYC, City::BRN, City::LON, City::SYD, City::YTZ, City::LON, City::SIN, City::TYO, 
City::LON, City::TYO, City::TYO, City::LON, City::LON, City::STI, City::BJS, City::LON, City::BUD, City::LON, 
City::NYC, City::TYO, City::MAD, City::MAD, City::MAD, City::MAD, City::MAD, City::NYC, City::NYC, City::TYO, 
City::TYO, City::ABJ, City::SYD, City::LON, City::HKG, City::TYO, City::UTC, City::MNL, City::NYC, City::SIN, 
City::LON, City::MAD, City::PAR, City::LON, City::LON, City::LON, City::LON, City::CHI, City::TYO, City::LON, 
City::YAO, City::CLT, City::LON, City::SIN, City::OSL, City::PAR, City::FIH, City::CHI, City::BZV, City::CHI, 
City::RAR, City::BOG, City::HAH, City::MLA, City::AMS, City::BUD, City::CHI, City::CHS, City::YTZ, City::NYC, 
City::AMS, City::RAI, City::LON, City::NYC, City::BLQ, City::SJI, City::BMA, City::LON, City::BMA, City::ZRH, 
City::PRG, City::SYD, City::LON, City::LON, City::NYC, City::FRA, City::YTZ, City::LON, City::LON, City::LON, 
City::DUB, City::HKG, City::LON, City::TYO, City::LON, City::NYC, City::MAD, City::PRG, City::ZRH, City::LON, 
City::WUX, City::BMA, City::NYC, City::ZRH, City::CHI, City::HAV, City::NYC, City::CUR, City::SYD, City::SYD, 
City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::SYD, City::LON, 
City::XCH, City::LON, City::GCM, City::NIC, City::PRG, City::LON, City::HS0, City::FRA, City::CHI, City::CPH, 
City::LON, City::DUB, City::NYC, City::FRA, City::LON, City::LON, City::LON, City::FRA, City::FRA, City::HKG, 
City::LON, City::FRA, City::LUX, City::FRA, City::FRA, City::LON, City::NYC, City::LON, City::CPH, City::CUR, 
City::FLR, City::NYC, City::FRA, City::BER, City::AUH, City::DXB, City::HEL, City::LUX, City::RKV, City::DXB, 
City::JIB, City::BMA, City::BMA, City::BMA, City::HS0, City::BMA, City::DCF, City::CPH, City::HEL, City::RKV, 
City::CPH, City::BMA, City::SDQ, City::BMA, City::ZRH, City::MAD, City::LON, City::MAD, City::TYO, City::LON, 
City::DOH, City::CPH, City::BMA, City::DXB, City::QDU, City::QDU, City::QDU, City::QDU, City::LON, City::EWR, 
City::NYC, City::ALG, City::FRA, City::LON, City::BUD, City::FRA, City::ROM, City::BMA, City::BTS, City::ZRH, 
City::AMS, City::AMS, City::AMS, City::LON, City::AMS, City::LON, City::ZRH, City::LON, City::BOS, City::FRA, 
City::SIN, City::AMS, City::FRA, City::LON, City::LON, City::LON, City::UIO, City::AMS, City::DUB, City::CHI, 
City::CHI, City::CHI, City::MCI, City::DUB, City::CHI, City::CHI, City::PAR, City::EWR, City::SIN, City::IEV, 
City::JNB, City::SW0, City::NYC, City::VIE, City::CAI, City::LON, City::OSL, City::LON, City::OSL, City::OSL, 
City::OSL, City::PAR, City::LON, City::BBU, City::LON, City::MIL, City::MIL, City::MIL, City::MIL, City::PCT, 
City::LUX, City::LON, City::ATH, City::LON, City::SIN, City::TLS, City::BMA, City::LON, City::FRA, City::BRU, 
City::LIS, City::HKG, City::PAR, City::PCT, City::PCT, City::YTZ, City::DUB, City::LON, City::SIN, City::SIN, 
City::SIN, City::DUB, City::LON, City::BER, City::BER, City::BER, City::BER, City::NYC, City::BRN, City::ATH, 
City::FRA, City::ASM, City::CHI, City::BUD, City::EUN, City::TLS, City::MAD, City::NYC, City::TLL, City::BMA, 
City::MIL, City::ADD, City::MIL, City::AMS, City::ALA, City::ATH, City::DUB, City::ZRH, City::SOF, City::BMA, 
City::ZRH, City::ZRH, City::ZRH, City::STR, City::BMA, City::STR, City::LON, City::ESB, City::MLA, City::VIE, 
City::LON, City::LON, City::LON, City::LON, City::MIL, City::LON, City::LON, City::LON, City::LON, City::MIL, 
City::NYC, City::PAR, City::PAR, City::PAR, City::PAR, City::PAR, City::LON, City::PAR, City::PAR, City::IST, 
City::LON, City::PAR, City::LON, City::NYC, City::MIL, City::CHI, City::CHI, City::ES0, City::SFO, City::FRA, 
City::BMA, City::HEL, City::NYC, City::NYC, City::DCA, City::DCA, City::DCA, City::BGO, City::LON, City::SUV, 
City::PSY, City::AMS, City::AMS, City::AMS, City::PAR, City::NYC, City::CPH, City::OSL, City::TLL, City::HEL, 
City::NYC, City::NYC, City::RKV, City::NYC, City::VNO, City::RIX, City::BMA, City::LON, City::NYC, City::VIE, 
City::PAR, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::OSL, City::CHI, 
City::FAE, City::BRU, City::LON, City::NYC, City::TOS, City::PNI, City::HEL, City::PRG, City::PRG, City::NYC, 
City::MIA, City::NYC, City::BOM, City::NYC, City::DUB, City::LON, City::LON, City::NYC, City::LON, City::NYC, 
City::DUB, City::DUB, City::SIN, City::BOM, City::CHI, City::LON, City::LBV, City::BMA, City::EB0, City::LON, 
City::DUB, City::SOF, City::BMA, City::LON, City::TBS, City::VNO, City::NYC, City::SYD, City::LON, City::LON, 
City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::BMA, City::LON, City::PAR, 
City::SIN, City::LON, City::LON, City::GCI, City::ACC, City::GIB, City::CKY, City::PAR, City::CHI, City::NYC, 
City::NYC, City::AUH, City::BBR, City::CHI, City::EWR, City::BJL, City::SOF, City::LON, City::PAR, City::PAR, 
City::MUC, City::DXB, City::UTC, City::LON, City::NYC, City::PAR, City::LON, City::OXB, City::SSG, City::CHI, 
City::BOS, City::LM0, City::ATH, City::GND, City::CHI, City::LON, City::LON, City::GOH, City::MAD, City::LON, 
City::HKG, City::FRA, City::LON, City::GEO, City::NYC, City::NYC, City::FRA, City::LON, City::LON, City::LON, 
City::HKG, City::PAR, City::LON, City::BJS, City::HKG, City::SEL, City::GIB, City::HKG, City::HKG, City::TSA, 
City::NYC, City::GUA, City::NYC, City::YHM, City::EWR, City::CAY, City::GUM, City::GEO, City::HS0, City::HS0, 
City::HS0, City::VLI, City::HAM, City::HAM, City::HAM, City::HAM, City::HAM, City::HAJ, City::HAJ, City::PAR, 
City::WAW, City::MIL, City::AMS, City::ATH, City::ATH, City::CHI, City::FRA, City::ATH, City::ATH, City::ATH, 
City::HKG, City::HKG, City::MIL, City::MIL, City::TGU, City::ATH, City::PAR, City::PAR, City::PAR, City::PAR, 
City::BOS, City::LON, City::LON, City::DUB, City::MIL, City::LON, City::NYC, City::NYC, City::ZAG, City::LON, 
City::QDU, City::NYC, City::HAN, City::HKG, City::LON, City::PAP, City::BUD, City::BUD, City::LON, City::BUD, 
City::GW0, City::LON, City::FLR, City::ZAZ, City::SOF, City::BIO, City::FLR, City::GW0, City::NYC, City::DUB, 
City::SOF, City::LON, City::LON, City::BGO, City::LON, City::NYC, City::CGK, City::NYC, City::LON, City::LON, 
City::NYC, City::LON, City::TYO, City::TYO, City::PAR, City::PAR, City::LON, City::NYC, City::LON, City::TYO, 
City::EWR, City::TYO, City::LON, City::TYO, City::DUB, City::NYC, City::BOM, City::DUB, City::CGK, City::NYC, 
City::AMS, City::LON, City::LON, City::WAW, City::GW0, City::ATL, City::NYC, City::NYC, City::NYC, City::AUH, 
City::THR, City::YWG, City::NYC, City::LON, City::LON, City::OSL, City::LON, City::LON, City::LON, City::LON, 
City::SIN, City::MLA, City::NYC, City::LON, City::DUB, City::DUB, City::DUB, City::LON, City::NYC, City::GC0, 
City::QDU, City::LON, City::ATL, City::ATL, City::NYC, City::LON, City::LON, City::NYC, City::LON, City::AMS, 
City::ATL, City::CHI, City::AMS, City::LON, City::ATL, City::AMS, City::LON, City::THR, City::LON, City::ATL, 
City::LON, City::LON, City::ATL, City::MIL, City::LON, City::IOM, City::LON, City::LON, City::LON, City::LON, 
City::NYC, City::ST0, City::DEL, City::OSL, City::AMS, City::AMS, City::AMS, City::LON, City::WAW, City::GH0, 
City::WP0, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, 
City::LON, City::NKW, City::LON, City::LON, City::AMS, City::LON, City::LON, City::LON, City::DUB, City::THR, 
City::BGW, City::TRN, City::QIC, City::NYC, City::LON, City::RKV, City::BOM, City::RKV, City::TLV, City::LON, 
City::LON, City::LON, City::LON, City::LON, City::AMS, City::AMS, City::AMS, City::LON, City::AMS, City::LON, 
City::ROM, City::NYC, City::DUB, City::MIL, City::LON, City::YTZ, City::LED, City::SIN, City::KIN, City::TYO, 
City::TYO, City::SI0, City::SOF, City::FRA, City::LON, City::NYC, City::FRA, City::JER, City::LON, City::CHI, 
City::AMS, City::CHI, City::LON, City::AMS, City::NYC, City::AMM, City::NYC, City::LON, City::FRA, City::HKG, 
City::NYC, City::NYC, City::TYO, City::LON, City::JNB, City::NYC, City::JNB, City::NYC, City::NYC, City::LON, 
City::GR0, City::TYO, City::TYO, City::TSE, City::BRU, City::LUX, City::LUX, City::LON, City::LUX, City::LUX, 
City::BUD, City::WAW, City::BUD, City::NBO, City::FRU, City::BUD, City::PNH, City::TRW, City::LON, City::NYC, 
City::LON, City::ZRH, City::SKB, City::EWR, City::EWR, City::EWR, City::EWR, City::EWR, City::SEL, City::PRG, 
City::SEL, City::PAR, City::AUH, City::KWI, City::NYC, City::NYC, City::NYC, City::BCT, City::VTE, City::NYC, 
City::CPH, City::NYC, City::NYC, City::STR, City::LON, City::FRA, City::BEY, City::MLW, City::SOF, City::STR, 
City::STR, City::TIP, City::SLU, City::PAR, City::LON, City::AMS, City::NYC, City::LON, City::NYC, City::DUB, 
City::DUB, City::BOS, City::YTZ, City::LI0, City::NYC, City::RB0, City::LON, City::LON, City::LON, City::LON, 
City::AMS, City::NYC, City::NYC, City::CMB, City::VIE, City::LON, City::LON, City::LON, City::LON, City::SIN, 
City::LON, City::LON, City::TLL, City::BOS, City::LON, City::PAR, City::PAR, City::LON, City::LON, City::LON, 
City::LON, City::NYC, City::NYC, City::NYC, City::MSU, City::QDU, City::BOS, City::SFO, City::VNO, City::LUX, 
City::RIX, City::TYO, City::YTZ, City::MAD, City::MFM, City::SYD, City::MIL, City::LON, City::SFG, City::EWR, 
City::LON, City::SIN, City::MLE, City::AMS, City::DUB, City::HKG, City::LON, City::TYO, City::LON, City::LON, 
City::SYD, City::RBA, City::MAD, City::SIN, City::YTZ, City::AUH, City::ESH, City::OPO, City::WAW, City::LON, 
City::SOF, City::ROM, City::DUB, City::MCM, City::NYC, City::CPH, City::LON, City::LON, City::LON, City::BOM, 
City::LON, City::KIV, City::TNR, City::LIS, City::MAD, City::MLE, City::SYD, City::HKG, City::NYC, City::EWR, 
City::EWR, City::EWR, City::OSL, City::MAD, City::OSL, City::FRA, City::KUL, City::LON, City::MEX, City::LON, 
City::LIS, City::LON, City::LON, City::LON, City::QDU, City::AMS, City::LON, City::PAR, City::HEL, City::FRA, 
City::LON, City::MAJ, City::MAD, City::LUX, City::RKV, City::AUH, City::PCT, City::MOW, City::MIL, City::TYO, 
City::NYC, City::LM0, City::SKP, City::PAR, City::LON, City::NYC, City::PAR, City::PAR, City::LON, City::PAR, 
City::BKO, City::LON, City::LON, City::LON, City::LON, City::MLA, City::LON, City::NYC, City::BRU, City::LON, 
City::RGN, City::CPH, City::TGD, City::HEL, City::ULN, City::RKV, City::SPN, City::BMA, City::NYC, City::BMA, 
City::MIL, City::MPM, City::PCT, City::NKC, City::SYD, City::PAR, City::LON, City::NYC, City::PAR, City::WAW, 
City::DUB, City::FRA, City::LON, City::NYC, City::NYC, City::TYO, City::PAR, City::NYC, City::MNI, City::NYC, 
City::LON, City::NYC, City::BMA, City::NYC, City::ROM, City::MCT, City::MCT, City::MIL, City::MIL, City::MIL, 
City::PAR, City::FDF, City::LON, City::NYC, City::ROM, City::BRU, City::BRU, City::LON, City::ROM, City::ROM, 
City::WAW, City::LON, City::LON, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::AMS, 
City::LON, City::MIL, City::PAR, City::TYO, City::LON, City::MUC, City::MUC, City::MUC, City::MUC, City::MRU, 
City::LON, City::AMS, City::LON, City::COR, City::LLW, City::LON, City::AMS, City::EWR, City::KUL, City::DZA, 
City::LON, City::LON, City::LON, City::LON, City::LON, City::PAR, City::LON, City::WDH, City::MOW, City::BMA, 
City::VNO, City::NYC, City::BMA, City::LOS, City::PAR, City::LON, City::LON, City::NYC, City::IDR, City::DCA, 
City::KHI, City::NOU, City::FRA, City::LON, City::LON, City::AMS, City::AMS, City::NTL, City::BJS, City::YTZ, 
City::YTZ, City::YTZ, City::YTZ, City::NIM, City::FRA, City::LON, City::LON, City::LON, City::LON, City::LON, 
City::OSL, City::LON, City::LON, City::LON, City::AMS, City::NLK, City::BOS, City::BOS, City::BOS, City::ABV, 
City::YYC, City::HG0, City::OSL, City::MGA, City::CAI, City::IUE, City::NYC, City::HAJ, City::AMS, City::AMS, 
City::AMD, City::NYC, City::TYO, City::TYO, City::BMA, City::LON, City::GOJ, City::BMA, City::VN0, City::BMA, 
City::LON, City::BMA, City::LON, City::BMA, City::TRD, City::OSL, City::HAM, City::OSL, City::OSL, City::LON, 
City::OSL, City::BMA, City::PAR, City::HG0, City::CPH, City::KTM, City::NYC, City::INU, City::BMA, City::BMA, 
City::KRK, City::MEL, City::ST0, City::LON, City::LON, City::LON, City::EDI, City::AMS, City::TYO, City::LON, 
City::TYO, City::TYO, City::TYO, City::NYC, City::TYO, City::NYC, City::NYC, City::LON, City::NYC, City::NYC, 
City::CPH, City::SYD, City::WLG, City::OSL, City::LNZ, City::EWR, City::LON, City::NYC, City::PAR, City::PAR, 
City::TYO, City::LON, City::AMS, City::LON, City::OB0, City::NYC, City::MAD, City::YTZ, City::LIS, City::LIS, 
City::MCT, City::NYC, City::BMA, City::DCA, City::HEL, City::LIS, City::CHI, City::HKG, City::OSL, City::SIN, 
City::NYC, City::LON, City::NYC, City::NYC, City::NYC, City::NYC, City::BOM, City::BUD, City::BBU, City::BRN, 
City::LON, City::ISB, City::PAC, City::WAW, City::LON, City::PRG, City::BCN, City::ATH, City::SOF, City::LON, 
City::PN0, City::MC0, City::CHI, City::CHI, City::LON, City::BMA, City::BMA, City::BMA, City::BMA, City::BMA, 
City::LIM, City::IEV, City::IEV, City::LON, City::MNL, City::LON, City::NYC, City::NYC, City::NYC, City::NYC, 
City::NYC, City::GW0, City::LON, City::GW0, City::WAW, City::LON, City::WAW, City::WAW, City::WAW, City::WAW, 
City::LON, City::MUC, City::ROR, City::LIS, City::BMA, City::POM, City::WAW, City::WAW, City::LON, City::MAD, 
City::VLI, City::SYD, City::LON, City::SJU, City::FNJ, City::LON, City::MLA, City::NYC, City::LIS, City::ASU, 
City::PS0, City::NYC, City::NYC, City::WAW, City::NYC, City::NYC, City::NYC, City::NYC, City::YTZ, City::LON, 
City::BOM, City::PPT, City::DOH, City::BUD, City::LON, City::OSL, City::LON, City::LON, City::LON, City::UTC, 
City::NYC, City::EA0, City::PAR, City::LON, City::FRA, City::LON, City::LON, City::BUD, City::VIE, City::EA0, 
City::LON, City::NYC, City::NYC, City::NIC, City::AMS, City::AMS, City::LON, City::RUN, City::PRG, City::DXB, 
City::PHL, City::PHL, City::BOM, City::LNZ, City::JNB, City::JNB, City::DUB, City::LON, City::TSA, City::ROS, 
City::ZRH, City::KGL, City::BBU, City::MOW, City::WAW, City::AMS, City::KGL, City::LON, City::LON, City::SIN, 
City::MOW, City::NYC, City::ZRH, City::MOW, City::MOW, City::KGL, City::LU0, City::SFO, City::SDR, City::RUH, 
City::OSL, City::BCN, City::LON, City::TYO, City::BIO, City::TYO, City::TYO, City::SBZ, City::AB0, City::FRA, 
City::MAD, City::EDI, City::CHI, City::CHI, City::CHI, City::CHI, City::CHI, City::KRT, City::FRA, City::VNO, 
City::BMA, City::BMA, City::SEZ, City::SEZ, City::SEZ, City::LON, City::SEZ, City::LON, City::MIL, City::BMA, 
City::RIO, City::DUB, City::DKR, City::MAD, City::BMA, City::DNK, City::BMA, City::LON, City::LAX, City::NYC, 
City::NYC, City::SHA, City::NYC, City::NYC, City::PAR, City::PAR, City::PAR, City::LON, City::LON, City::LON, 
City::PAR, City::SIN, City::GS0, City::NYC, City::AY0, City::NYC, City::ASI, City::HKG, City::WAW, City::NIC, 
City::LON, City::DUB, City::LON, City::SYD, City::HKG, City::TYO, City::BOS, City::MAD, City::SYD, City::DUB, 
City::LON, City::LYR, City::LJU, City::RA0, City::LON, City::HIR, City::FNA, City::SZG, City::LON, City::SAL, 
City::LON, City::BRU, City::TYO, City::QDU, City::LON, City::LON, City::PAR, City::SIN, City::CHI, City::FRA, 
City::SM0, City::LON, City::AL0, City::LON, City::NYC, City::MGQ, City::PRG, City::NYC, City::MOW, City::LON, 
City::LON, City::FRA, City::PCT, City::LED, City::FSP, City::LON, City::TRD, City::EWR, City::ILZ, City::LON, 
City::BEG, City::MUC, City::MUC, City::LON, City::JUB, City::LON, City::LON, City::LON, City::BMA, City::ROM, 
City::BOS, City::HAM, City::LON, City::LON, City::OSL, City::LON, City::NYC, City::NYC, City::ZRH, City::TMS, 
City::NYC, City::PRG, City::LON, City::STR, City::STR, City::STR, City::STR, City::STR, City::STR, City::AMS, 
City::LON, City::LON, City::LON, City::CHI, City::PBM, City::BMA, City::BMA, City::BTS, City::LJU, City::SVD, 
City::LON, City::BMA, City::BMA, City::TLL, City::VNO, City::RIX, City::QMN, City::SXM, City::CPH, City::SEZ, 
City::SIN, City::DAM, City::HKG, City::SOF, City::LON, City::LON, City::WAW, City::WAW, City::GDT, City::NDJ, 
City::LON, City::LON, City::LON, City::LON, City::DUB, City::PAR, City::LON, City::LON, City::LON, City::LON, 
City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, 
City::LON, City::PAR, City::PAR, City::PAR, City::PAR, City::PAR, City::PAR, City::EWR, City::LON, City::SIN, 
City::MAD, City::BKK, City::SIN, City::LON, City::HKG, City::LON, City::LON, City::LON, City::NYC, City::LON, 
City::BER, City::LFW, City::BER, City::BKK, City::EWR, City::CHI, City::NYC, City::TYO, City::LON, City::DYU, 
City::TK0, City::ASB, City::MIL, City::PAR, City::DIL, City::NYC, City::LM0, City::DUB, City::ESB, City::NYC, 
City::LON, City::LON, City::YTZ, City::AMS, City::BRU, City::BRU, City::LON, City::HKG, City::AMS, City::MAD, 
City::MAD, City::AMS, City::TBU, City::AMS, City::LON, City::LON, City::FRA, City::PAR, City::LON, City::LON, 
City::LON, City::PAR, City::MAD, City::PAR, City::LON, City::PAR, City::PAR, City::LON, City::LON, City::LON, 
City::PAR, City::PAR, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::EWR, City::LON, 
City::LON, City::LON, City::EWR, City::LON, City::AMS, City::AMS, City::AMS, City::AMS, City::ILG, City::LON, 
City::HKG, City::LON, City::NYC, City::NYC, City::SIN, City::LON, City::LON, City::LON, City::LON, City::LON, 
City::LON, City::EWR, City::AMS, City::SEZ, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, 
City::LON, City::LON, City::NYC, City::NYC, City::LON, City::NYC, City::EWR, City::PAR, City::NYC, City::PAR, 
City::ILG, City::LON, City::LON, City::LON, City::NYC, City::FRA, City::FRA, City::LON, City::LON, City::LON, 
City::LON, City::SIN, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, City::LON, 
City::LON, City::POS, City::TUN, City::LON, City::ESB, City::FUN, City::AMS, City::AMS, City::AMS, City::AMS, 
City::LON, City::HKG, City::TYO, City::TYO, City::TSA, City::NYC, City::SIN, City::DAR, City::NYC, City::PRG, 
City::ATL, City::BGY, City::LON, City::BLQ, City::NYC, City::LON, City::ZRH, City::FRA, City::LON, City::ZRH, 
City::ZRH, City::FRA, City::FRA, City::NYC, City::NYC, City::ZRH, City::HKG, City::LON, City::VIE, City::SOF, 
City::MUC, City::BUD, City::MIL, City::MVD, City::KLA, City::LBA, City::IEV, City::LON, City::IEV, City::LON, 
City::LON, City::LON, City::IEV, City::LON, City::KLA, City::LON, City::LON, City::SVX, City::MVD, City::DCA, 
City::NYC, City::BMA, City::BMA, City::LON, City::TYO, City::TAS, City::NYC, City::LON, City::LON, City::NYC, 
City::VA0, City::LON, City::NYC, City::SVD, City::YTZ, City::LON, City::CCS, City::NYC, City::NYC, City::VFA, 
City::LON, City::DUB, City::NYC, City::DUB, City::LON, City::DUB, City::EIS, City::STT, City::NYC, City::LON, 
City::TYO, City::ZRH, City::AUH, City::FAE, City::LON, City::NYC, City::HAN, City::LON, City::FRA, City::BRU, 
City::PAR, City::LON, City::NYC, City::FRA, City::FRA, City::BTS, City::NYC, City::VLI, City::FRA, City::FRA, 
City::NYC, City::VIE, City::WAW, City::VIE, City::VIE, City::WRO, City::WAW, City::WAW, City::WAW, City::LON, 
City::WAW, City::GW0, City::LON, City::NYC, City::CLT, City::WAW, City::LON, City::PAR, City::WAW, City::WAW, 
City::LON, City::LON, City::WAW, City::WLS, City::WAW, City::PRG, City::WAW, City::LIS, City::NYC, City::LON, 
City::LON, City::APW, City::AEP, City::BMA, City::OSL, City::ABJ, City::SIN, City::AMS, City::ATH, City::DCA, 
City::AUH, City::AMS, City::PAR, City::EB0, City::MIL, City::YYC, City::ALG, City::TIA, City::LON, City::AMM, 
City::AMS, City::SIN, City::SIN, City::ANR, City::SYD, City::PAR, City::VVO, City::NYC, City::CHI, City::EVN, 
City::NYC, City::DUB, City::SYD, City::ATH, City::DUB, City::YTZ, City::YTZ, City::AKL, City::PHX, City::NAS, 
City::BGI, City::BAH, City::BLR, City::BCN, City::BCN, City::RIO, City::CGK, City::YTZ, City::COR, City::BUD, 
City::STI, City::MDZ, City::TGU, City::MDZ, City::YHM, City::LAD, City::BEG, City::BER, City::JNB, City::BEY, 
City::BRU, City::BIO, City::BGI, City::BKK, City::BKK, City::BNX, City::PAR, City::PAR, City::GRU, City::NYC, 
City::BUD, City::SJI, City::BOG, City::LPB, City::BOM, City::NYC, City::GBE, City::BOS, City::BTS, City::BRU, 
City::BRE, City::BBU, City::BRN, City::NYC, City::BRU, City::ABJ, City::WAW, City::BBU, City::BBU, City::GRU, 
City::EWR, City::ZRH, City::BUD, City::AEP, City::SOF, City::RAI, City::MPM, City::CWB, City::SDQ, City::NYC, 
City::CAI, City::CCU, City::BBU, City::CCS, City::CMN, City::GCM, City::CHI, City::CHI, City::CHI, City::CHI, 
City::NGO, City::CHI, City::DUB, City::NYC, City::VIE, City::TAS, City::SHA, City::NYC, City::HKG, City::CGP, 
City::CHI, City::GCI, City::NYC, City::CHI, City::CNQ, City::YTZ, City::CMB, City::LON, City::CHI, City::ZAG, 
City::NYC, City::CPH, City::PNH, City::TAS, City::NYC, City::SDQ, City::YTZ, City::YTZ, City::NIC, City::NIC, 
City::DAR, City::FRA, City::FRA, City::FRA, City::DLC, City::DEL, City::STR, City::IEV, City::DXB, City::DAC, 
City::ZRH, City::MIL, City::OSL, City::MAD, City::MAD, City::YTZ, City::DAM, City::IST, City::DLA, City::FRA, 
City::DUB, City::QDU, City::FRA, City::LV0, City::DUB, City::LON, City::FRA, City::LEJ, City::NIC, City::SKB, 
City::IST, City::LON, City::LEJ, City::LEJ, City::LEJ, City::FRA, City::IST, City::NYC, City::MEX, City::SH0, 
City::BER, City::IST, City::FRA, City::FRA, City::DUB, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, 
City::PAR, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::FRA, City::AMS, 
City::AMS, City::AMS, City::FRA, City::FRA, City::FRA, City::CHI, City::DUB, City::SFO, City::VLC, City::SFO, 
City::SYD, City::FUK, City::FUK, City::PAR, City::CPH, City::IST, City::DUB, City::HEL, City::FRA, City::AMS, 
City::ANU, City::BUD, City::BER, City::LON, City::LON, City::LON, City::HKG, City::LON, City::ACC, City::WAW, 
City::ROM, City::SJU, City::BER, City::TBS, City::LON, City::GUA, City::GYE, City::HAM, City::HAJ, City::HAJ, 
City::HEL, City::OSA, City::AMS, City::HIJ, City::HKG, City::HKG, City::HAN, City::HAN, City::SAP, City::IST, 
City::GYD, City::ZRH, City::RKV, City::YTZ, City::CGK, City::LON, City::DUB, City::FRA, City::HKG, City::TYO, 
City::OSL, City::BOM, City::TSA, City::CHI, City::SHA, City::NYC, City::FRA, City::CHI, City::LON, City::ATH, 
City::BGW, City::NYC, City::NYC, City::ISB, City::IST, City::NYC, City::KIN, City::TYO, City::CGK, City::CGK, 
City::TYO, City::JNB, City::LON, City::OSA, City::KHI, City::ALA, City::MCI, City::TAS, City::SEL, City::SEL, 
City::SEL, City::SEL, City::UKB, City::KRT, City::HRK, City::IEV, City::IEV, City::UKB, City::KUL, City::SEL, 
City::SEL, City::SEL, City::SEL, City::FRU, City::SK0, City::KWI, City::UKY, City::LHE, City::VTE, City::MAD, 
City::LON, City::LON, City::LON, City::LON, City::LON, City::LBU, City::LI0, City::LON, City::LIM, City::LIS, 
City::VNO, City::LJU, City::LJU, City::LJU, City::LI0, City::LON, City::LON, City::KUL, City::LON, City::LON, 
City::SYD, City::TIP, City::LON, City::LUN, City::LUX, City::AEP, City::CHI, City::MAD, City::SKP, City::BKK, 
City::MLA, City::MGA, City::MPM, City::PAR, City::MRU, City::MAD, City::TNR, City::MAA, City::BCN, City::CHI, 
City::AEP, City::MEX, City::MAD, City::MSP, City::MOW, City::CHI, City::MIL, City::MIL, City::PCT, City::PAR, 
City::LON, City::NIC, City::MVD, City::TGD, City::YMQ, City::YMQ, City::KIV, City::PAR, City::YMQ, City::MOW, 
City::MIL, City::MAD, City::MAD, City::DUB, City::BLZ, City::AEP, City::LON, City::MUC, City::MCT, City::SFN, 
City::MAD, City::NBO, City::WDH, City::NYC, City::BOM, City::NYC, City::WAW, City::NYC, City::IEV, City::BMA, 
City::NTL, City::AKL, City::KTM, City::FRA, City::NYC, City::BMA, City::NGO, City::NYC, City::KIJ, City::NYC, 
City::NGO, City::NYC, City::LON, City::BMA, City::NYC, City::LI0, City::HEL, City::NYC, City::LOS, City::BOM, 
City::NGO, City::AMS, City::AMS, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::NYC, City::WLG, 
City::OSL, City::OSL, City::OSL, City::OSL, City::OSL, City::CHI, City::ODS, City::XXX, City::BMA, City::BMA, 
City::OSL, City::OSL, City::OSL, City::OSA, City::OSA, City::OSL, City::OSA, City::OSA, City::VIE, City::DCA, 
City::SPU, City::DUB, City::PS0, City::LON, City::PAR, City::PHL, City::LED, City::PHL, City::PHL, City::PC0, 
City::LON, City::LED, City::ST0, City::LON, City::IST, City::LON, City::POM, City::DCA, City::DUB, City::LEJ, 
City::PAR, City::PRG, City::DNK, City::PRG, City::SFO, City::LEJ, City::HKG, City::NYC, City::PAC, City::LON, 
City::NYC, City::PRG, City::BUD, City::ZRH, City::ZRH, City::QDU, City::UIO, City::BBU, City::KUL, City::VIE, 
City::ZRH, City::DUB, City::RIO, City::RIX, City::PRG, City::BTS, City::PRG, City::ROS, City::ZRH, City::ROV, 
City::ROS, City::BBU, City::LON, City::FRA, City::MOW, City::JNB, City::KUF, City::OKD, City::BMA, City::RUH, 
City::TYO, City::SIN, City::FRA, City::FRA, City::FRA, City::SIN, City::SIN, City::SIN, City::FRA, City::ZRH, 
City::ZRH, City::ZRH, City::SZX, City::NYC, City::SIN, City::JNB, City::SYD, City::PAR, City::LON, City::SHA, 
City::STI, City::SZX, City::SHA, City::OVB, City::OVB, City::SIN, City::ZRH, City::SZX, City::LON, City::GRU, 
City::LJU, City::PAR, City::SUV, City::GRX, City::SHA, City::SJJ, City::SGN, City::TAS, City::STR, City::BOS, 
City::BMA, City::STR, City::STR, City::ZRH, City::ZRH, City::SUB, City::SAL, City::QMN, City::LON, City::ZRH, 
City::ZRH, City::ZRH, City::TLL, City::TSA, City::TLV, City::TSA, City::TSA, City::TLL, City::TYO, City::TLL, 
City::THR, City::YTZ, City::TYO, City::LON, City::TIA, City::TYO, City::TYO, City::TYO, City::NGO, City::TYO, 
City::TYO, City::TYO, City::MIL, City::BUD, City::YVR, City::YTZ, City::LON, City::NYC, City::CPH, City::NYC, 
City::POS, City::ZAG, City::YTZ, City::YTZ, City::TUC, City::TUN, City::LON, City::ADB, City::NYC, City::PAR, 
City::LON, City::IEV, City::LON, City::KLA, City::IEV, City::ULN, City::LON, City::TAS, City::BOM, City::VLC, 
City::VZ0, City::LUX, City::VNO, City::VIE, City::VVO, City::ASU, City::LI0, City::YVR, City::ZRH, City::WAW, 
City::VIE, City::YWG, City::GW0, City::FRA, City::XXX, City::XXX, City::SYD, City::TYO, City::ZAG, City::ZAG, 
City::ZAG, City::CGO, City::HRE, City::SAH, City::CHI, City::JNB, City::PRY, City::ZAG, City::JNB, City::SOF, 
City::CHI, City::JNB, City::ZRH, City::LUN, City::BER, City::LON, City::HRE };

// Country to capital City
const short Gazetteer::m_cid2cap[Country::NUMCOUNTRY] = { 
City::NOCITY,
    City::AUA, City::KBL, City::LAD, City::AXA, City::MHQ, City::TIA, City::AD0, City::AUH, City::AEP, City::EVN, 
    City::PPG, City::XXX, City::TF0, City::ANU, City::CBR, City::VIE, City::GYD, City::BJM, City::BRU, City::COO, 
    City::BON, City::OUA, City::DAC, City::SOF, City::BAH, City::NAS, City::SJJ, City::SBH, City::MSQ, City::BCV, 
    City::YHM, City::LPB, City::BSB, City::BGI, City::BWN, City::PBH, City::OSL, City::GBE, City::BGF, City::YOW, 
    City::CCK, City::BRN, City::STI, City::BJS, City::ABJ, City::YAO, City::FIH, City::BZV, City::RAR, City::BOG, 
    City::HAH, City::RAI, City::SJI, City::HAV, City::CUR, City::XCH, City::GCM, City::NIC, City::PRG, City::BER, 
    City::JIB, City::DCF, City::CPH, City::SDQ, City::ALG, City::UIO, City::CAI, City::ASM, City::EUN, City::MAD, 
    City::TLL, City::ADD, City::XXX, City::HEL, City::SUV, City::PSY, City::PAR, City::FAE, City::PNI, City::LBV, 
    City::LON, City::TBS, City::GCI, City::ACC, City::GIB, City::CKY, City::BBR, City::BJL, City::OXB, City::SSG, 
    City::ATH, City::GND, City::GOH, City::GUA, City::CAY, City::GUM, City::GEO, City::HKG, City::CBR, City::TGU, 
    City::ZAG, City::PAP, City::BUD, City::CGK, City::IOM, City::DEL, City::NKW, City::DUB, City::THR, City::BGW, 
    City::RKV, City::TLV, City::ROM, City::KIN, City::JER, City::AMM, City::TYO, City::TSE, City::NBO, City::FRU, 
    City::PNH, City::TRW, City::SKB, City::SEL, City::KWI, City::VTE, City::BEY, City::MLW, City::TIP, City::SLU, 
    City::LI0, City::CMB, City::MSU, City::VNO, City::LUX, City::RIX, City::MFM, City::SFG, City::RBA, City::MCM, 
    City::KIV, City::TNR, City::MLE, City::MEX, City::MAJ, City::SKP, City::BKO, City::MLA, City::RGN, City::TGD, 
    City::ULN, City::SPN, City::MPM, City::NKC, City::MNI, City::FDF, City::MRU, City::LLW, City::KUL, City::DZA, 
    City::WDH, City::NOU, City::NIM, City::NLK, City::ABV, City::MGA, City::IUE, City::AMS, City::OSL, City::KTM, 
    City::INU, City::WLG, City::MCT, City::ISB, City::PAC, City::PN0, City::LIM, City::MNL, City::ROR, City::POM, 
    City::WAW, City::SJU, City::FNJ, City::LIS, City::ASU, City::PS0, City::PPT, City::DOH, City::RUN, City::BBU, 
    City::MOW, City::KGL, City::RUH, City::KRT, City::DKR, City::SIN, City::GS0, City::ASI, City::LYR, City::HIR, 
    City::FNA, City::SAL, City::SM0, City::MGQ, City::FSP, City::BEG, City::JUB, City::TMS, City::PBM, City::BTS, 
    City::LJU, City::BMA, City::QMN, City::SXM, City::SEZ, City::DAM, City::GDT, City::NDJ, City::LFW, City::BKK, 
    City::DYU, City::TK0, City::ASB, City::DIL, City::TBU, City::POS, City::TUN, City::ESB, City::FUN, City::TSA, 
    City::DAR, City::KLA, City::IEV, City::DCA, City::MVD, City::DCA, City::TAS, City::VA0, City::SVD, City::CCS, 
    City::EIS, City::STT, City::HAN, City::VLI, City::WLS, City::APW, City::XXX, City::XXX, City::XXX, City::XXX, 
    City::XXX, City::SAH, City::PRY, City::LUN, City::HRE };

// City to Country
const short Gazetteer::m_cty2cid[City::NUMCITY] = { 
Country::NOCOUNTRY,
Country::DNK, Country::CIV, Country::NGA, Country::GBR, Country::GHA, Country::AND, Country::TUR, Country::ETH, Country::ARG, Country::NZL, 
Country::DNK, Country::KAZ, Country::USA, Country::DZA, Country::EGY, Country::IND, Country::JOR, Country::NLD, Country::USA, Country::USA, 
Country::BEL, Country::ATG, Country::WSM, Country::RUS, Country::TKM, Country::SHN, Country::ERI, Country::PRY, Country::GRC, Country::USA, 
Country::IND, Country::ABW, Country::USA, Country::ARE, Country::USA, Country::AIA, Country::GBR, Country::BHR, Country::GLP, Country::ROU, 
Country::ESP, Country::USA, Country::BLZ, Country::USA, Country::SRB, Country::DEU, Country::LBN, Country::ZAF, Country::GBR, Country::CAF, 
Country::BRB, Country::NOR, Country::IRQ, Country::ITA, Country::GBR, Country::ESP, Country::USA, Country::GMB, Country::BDI, Country::CHN, 
Country::THA, Country::MLI, Country::ITA, Country::IND, Country::MWI, Country::SWE, Country::USA, Country::DEU, Country::BIH, Country::COL, 
Country::USA, Country::IND, Country::BES, Country::USA, Country::DEU, Country::CHE, Country::GBR, Country::BEL, Country::BRA, Country::CHE, 
Country::USA, Country::SVK, Country::USA, Country::HUN, Country::BRN, Country::COG, Country::USA, Country::EGY, Country::GUF, Country::GBR, 
Country::AUS, Country::CCK, Country::VEN, Country::IND, Country::IDN, Country::CHN, Country::BGD, Country::NZL, Country::USA, Country::USA, 
Country::GIN, Country::ROU, Country::USA, Country::LKA, Country::USA, Country::MAR, Country::ARG, Country::USA, Country::BEN, Country::ARG, 
Country::DNK, Country::ZAF, Country::USA, Country::USA, Country::CUW, Country::BRA, Country::GBR, Country::USA, Country::BGD, Country::SYR, 
Country::TZA, Country::USA, Country::DMA, Country::IND, Country::USA, Country::USA, Country::TLS, Country::SEN, Country::CMR, Country::CHN, 
Country::GBR, Country::UKR, Country::QAT, Country::USA, Country::USA, Country::IRL, Country::ARE, Country::TJK, Country::MYT, Country::LUX, 
Country::MUS, Country::GBR, Country::VGB, Country::MEX, Country::FIN, Country::TUR, Country::GBR, Country::CHL, Country::ESH, Country::ARM, 
Country::USA, Country::FRO, Country::USA, Country::MTQ, Country::USA, Country::COD, Country::ITA, Country::SLE, Country::PRK, Country::USA, 
Country::DEU, Country::KGZ, Country::SPM, Country::JPN, Country::TUV, Country::BWA, Country::IND, Country::GGY, Country::CYM, Country::TCA, 
Country::GUY, Country::IND, Country::GIB, Country::GBR, Country::GRD, Country::ITA, Country::GRL, Country::IND, Country::RUS, Country::SWE, 
Country::DNK, Country::BRA, Country::ESP, Country::AUT, Country::SGS, Country::GTM, Country::GUM, Country::CHE, Country::USA, Country::AZE, 
Country::ECU, Country::COM, Country::DEU, Country::DEU, Country::VNM, Country::CUB, Country::FIN, Country::NLD, Country::JPN, Country::SLB, 
Country::HKG, Country::USA, Country::USA, Country::ZWE, Country::UKR, Country::DNK, Country::USA, Country::IND, Country::UKR, Country::USA, 
Country::SVK, Country::USA, Country::NRU, Country::IMN, Country::PAK, Country::TUR, Country::NIU, Country::USA, Country::SAU, Country::USA, 
Country::JEY, Country::DJI, Country::ZAF, Country::USA, Country::ISR, Country::SSD, Country::AFG, Country::RWA, Country::PAK, Country::JPN, 
Country::JAM, Country::MDA, Country::UGA, Country::AUT, Country::POL, Country::SDN, Country::NPL, Country::RUS, Country::MYS, Country::KWT, 
Country::AGO, Country::USA, Country::USA, Country::GBR, Country::MYS, Country::GAB, Country::CYP, Country::ESP, Country::GBR, Country::RUS, 
Country::DEU, Country::TGO, Country::PAK, Country::LIE, Country::PER, Country::PRT, Country::USA, Country::SVN, Country::MWI, Country::CYP, 
Country::USA, Country::AUT, Country::GBR, Country::NGA, Country::BOL, Country::GBR, Country::CHE, Country::ZMB, Country::LUX, Country::BEL, 
Country::SJM, Country::FRA, Country::IND, Country::ESP, Country::MHL, Country::GBR, Country::PHL, Country::USA, Country::MCO, Country::OMN, 
Country::USA, Country::ARG, Country::AUS, Country::MEX, Country::MAC, Country::NIC, Country::BRA, Country::USA, Country::SOM, Country::ALA, 
Country::USA, Country::ITA, Country::USA, Country::MLT, Country::MDV, Country::LBR, Country::SWE, Country::MSR, Country::PHL, Country::RUS, 
Country::MOZ, Country::FRA, Country::MUS, Country::USA, Country::USA, Country::BLR, Country::LSO, Country::DEU, Country::USA, Country::URY, 
Country::ITA, Country::BHS, Country::KEN, Country::GBR, Country::TCD, Country::JPN, Country::CYP, Country::NER, Country::MRT, Country::IOT, 
Country::NFK, Country::NCL, Country::AUS, Country::USA, Country::DEU, Country::UKR, Country::USA, Country::JPN, Country::USA, Country::PRT, 
Country::IRL, Country::JPN, Country::NOR, Country::BFA, Country::RUS, Country::GNB, Country::PAN, Country::HTI, Country::FRA, Country::BTN, 
Country::SUR, Country::PHL, Country::USA, Country::USA, Country::USA, Country::USA, Country::ESP, Country::PCN, Country::KHM, Country::FSM, 
Country::PNG, Country::TTO, Country::ASM, Country::PYF, Country::CZE, Country::XXX, Country::ZAF, Country::PSE, Country::FLK, Country::USA, 
Country::DEU, Country::ITA, Country::SWZ, Country::SRB, Country::ITA, Country::ESP, Country::DNK, Country::CPV, Country::COK, Country::DEU, 
Country::MAR, Country::USA, Country::MMR, Country::USA, Country::BRA, Country::LVA, Country::CHN, Country::ISL, Country::ITA, Country::PLW, 
Country::ARG, Country::RUS, Country::NLD, Country::SAU, Country::REU, Country::USA, Country::YEM, Country::SLV, Country::HND, Country::BLM, 
Country::ROU, Country::DOM, Country::ESP, Country::KOR, Country::SYC, Country::MAF, Country::ARG, Country::USA, Country::VNM, Country::NLD, 
Country::CHN, Country::DNK, Country::SGP, Country::CRI, Country::BIH, Country::PRI, Country::JPN, Country::KNA, Country::GRC, Country::MKD, 
Country::USA, Country::USA, Country::LCA, Country::SMR, Country::USA, Country::IRL, Country::BGR, Country::USA, Country::MNP, Country::HRV, 
Country::GNQ, Country::USA, Country::CHL, Country::USA, Country::DEU, Country::VIR, Country::IDN, Country::FJI, Country::VCT, Country::RUS, 
Country::DEU, Country::GBR, Country::FRA, Country::SXM, Country::AUS, Country::AUT, Country::CHN, Country::UZB, Country::GEO, Country::TON, 
Country::ATF, Country::MNE, Country::HND, Country::IRN, Country::ALB, Country::LBY, Country::TKL, Country::USA, Country::EST, Country::FRA, 
Country::ISR, Country::FIN, Country::STP, Country::MDG, Country::NOR, Country::NOR, Country::ITA, Country::KIR, Country::TWN, Country::KAZ, 
Country::USA, Country::ARG, Country::TUN, Country::JPN, Country::ECU, Country::JPN, Country::JPN, Country::MNG, Country::NLD, Country::VAT, 
Country::ZWE, Country::AUT, Country::BRA, Country::ESP, Country::VUT, Country::USA, Country::LTU, Country::RUS, Country::CHL, Country::LAO, 
Country::RUS, Country::HRV, Country::POL, Country::NAM, Country::NZL, Country::WLF, Country::USA, Country::POL, Country::CHN, Country::CXR, 
Country::XXX, Country::CMR, Country::BMU, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, Country::CAN, 
Country::HRV, Country::ESP, Country::CHE, };

// Country to principle Currency
const short Gazetteer::m_cid2ccy[Country::NUMCOUNTRY] = { 
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
Currency::XXX, Currency::YER, Currency::ZAR, Currency::ZMW, Currency::ZWL, };

// principle Currency to Country 
const short Gazetteer::m_ccy2cid[Currency::NUMCURRENCY] = {
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
Country::ZWE, };

// priniciple Currency to Countries
// the currencies CSK, DDM, RUR, YDD, YUM, and ZRN map to countries that no longer exist
// which were the countries CSK, DDR, SUN, YMD, YUG, and ZAR respectively
// modified the generated table by hand so that these currencies map to Country::XXX
static const short a_NOCURRENCY[2] = { 1, Country::NOCOUNTRY };
static const short a_ADP[2] = { 1, Country::AND };
static const short a_AED[2] = { 1, Country::ARE };
static const short a_AFA[2] = { 1, Country::AFG };
static const short a_AFN[2] = { 1, Country::AFG };
static const short a_ALL[2] = { 1, Country::ALB };
static const short a_AMD[2] = { 1, Country::ARM };
static const short a_ANG[3] = { 2, Country::CUW, Country::SXM };
static const short a_AOA[2] = { 1, Country::AGO };
static const short a_AON[2] = { 1, Country::AGO };
static const short a_AOR[2] = { 1, Country::AGO };
static const short a_ARS[2] = { 1, Country::ARG };
static const short a_ATS[2] = { 1, Country::AUT };
static const short a_AUD[9] = { 8, Country::AUS, Country::CCK, Country::CXR, Country::HMD, Country::KIR, Country::NFK, Country::NRU, Country::TUV };
static const short a_AWG[2] = { 1, Country::ABW };
static const short a_AZM[2] = { 1, Country::AZE };
static const short a_AZN[2] = { 1, Country::AZE };
static const short a_BAD[2] = { 1, Country::BIH };
static const short a_BAM[2] = { 1, Country::BIH };
static const short a_BBD[2] = { 1, Country::BRB };
static const short a_BDT[2] = { 1, Country::BGD };
static const short a_BEC[2] = { 1, Country::BEL };
static const short a_BEF[2] = { 1, Country::BEL };
static const short a_BEL[2] = { 1, Country::BEL };
static const short a_BGL[2] = { 1, Country::BGR };
static const short a_BGN[2] = { 1, Country::BGR };
static const short a_BHD[2] = { 1, Country::BHR };
static const short a_BIF[2] = { 1, Country::BDI };
static const short a_BMD[2] = { 1, Country::BMU };
static const short a_BND[2] = { 1, Country::BRN };
static const short a_BOB[2] = { 1, Country::BOL };
static const short a_BOV[2] = { 1, Country::BOL };
static const short a_BRL[2] = { 1, Country::BRA };
static const short a_BSD[2] = { 1, Country::BHS };
static const short a_BTN[2] = { 1, Country::BTN };
static const short a_BWP[2] = { 1, Country::BWA };
static const short a_BYN[2] = { 1, Country::BLR };
static const short a_BYR[2] = { 1, Country::BLR };
static const short a_BZD[2] = { 1, Country::BLZ };
static const short a_CAD[2] = { 1, Country::CAN };
static const short a_CDF[2] = { 1, Country::COD };
static const short a_CHC[2] = { 1, Country::CHE };
static const short a_CHE[2] = { 1, Country::CHE };
static const short a_CHF[3] = { 2, Country::CHE, Country::LIE };
static const short a_CHW[2] = { 1, Country::CHE };
static const short a_CLF[2] = { 1, Country::CHL };
static const short a_CLP[2] = { 1, Country::CHL };
static const short a_CNY[2] = { 1, Country::CHN };
static const short a_COP[2] = { 1, Country::COL };
static const short a_COU[2] = { 1, Country::COL };
static const short a_CRC[2] = { 1, Country::CRI };
static const short a_CSK[2] = { 1, Country::XXX };
static const short a_CUC[2] = { 1, Country::CUB };
static const short a_CUP[2] = { 1, Country::CUB };
static const short a_CVE[2] = { 1, Country::CPV };
static const short a_CYP[2] = { 1, Country::CYP };
static const short a_CZK[2] = { 1, Country::CZE };
static const short a_DDM[2] = { 1, Country::XXX };
static const short a_DEM[2] = { 1, Country::DEU };
static const short a_DJF[2] = { 1, Country::DJI };
static const short a_DKK[4] = { 3, Country::DNK, Country::FRO, Country::GRL };
static const short a_DOP[2] = { 1, Country::DOM };
static const short a_DZD[2] = { 1, Country::DZA };
static const short a_ECS[2] = { 1, Country::ECU };
static const short a_EEK[2] = { 1, Country::EST };
static const short a_EGP[2] = { 1, Country::EGY };
static const short a_ERN[2] = { 1, Country::ERI };
static const short a_ESA[2] = { 1, Country::ESP };
static const short a_ESB[2] = { 1, Country::ESP };
static const short a_ESP[2] = { 1, Country::ESP };
static const short a_ETB[2] = { 1, Country::ETH };
static const short a_EUR[36] = { 35, Country::ALA, Country::AND, Country::ATF, Country::AUT, Country::BEL, Country::BLM, Country::CYP, Country::DEU, Country::ESP, Country::EST, Country::FIN, Country::FRA, Country::GLP, Country::GRC, Country::GUF, Country::HRV, Country::IRL, Country::ITA, Country::LTU, Country::LUX, Country::LVA, Country::MAF, Country::MCO, Country::MLT, Country::MNE, Country::MTQ, Country::MYT, Country::NLD, Country::PRT, Country::REU, Country::SMR, Country::SPM, Country::SVK, Country::SVN, Country::VAT };
static const short a_FIM[2] = { 1, Country::FIN };
static const short a_FJD[2] = { 1, Country::FJI };
static const short a_FKP[2] = { 1, Country::FLK };
static const short a_FRF[2] = { 1, Country::FRA };
static const short a_GBP[6] = { 5, Country::GBR, Country::GGY, Country::IMN, Country::JEY, Country::SGS };
static const short a_GEL[2] = { 1, Country::GEO };
static const short a_GHS[2] = { 1, Country::GHA };
static const short a_GIP[2] = { 1, Country::GIB };
static const short a_GMD[2] = { 1, Country::GMB };
static const short a_GNF[2] = { 1, Country::GIN };
static const short a_GRD[2] = { 1, Country::GRC };
static const short a_GTQ[2] = { 1, Country::GTM };
static const short a_GWP[2] = { 1, Country::GNB };
static const short a_GYD[2] = { 1, Country::GUY };
static const short a_HKD[2] = { 1, Country::HKG };
static const short a_HNL[2] = { 1, Country::HND };
static const short a_HRK[2] = { 1, Country::HRV };
static const short a_HTG[2] = { 1, Country::HTI };
static const short a_HUF[2] = { 1, Country::HUN };
static const short a_IDR[2] = { 1, Country::IDN };
static const short a_IEP[2] = { 1, Country::IRL };
static const short a_ILS[3] = { 2, Country::ISR, Country::PSE };
static const short a_INR[3] = { 2, Country::BTN, Country::IND };
static const short a_IQD[2] = { 1, Country::IRQ };
static const short a_IRR[2] = { 1, Country::IRN };
static const short a_ISK[2] = { 1, Country::ISL };
static const short a_ITL[2] = { 1, Country::ITA };
static const short a_JMD[2] = { 1, Country::JAM };
static const short a_JOD[2] = { 1, Country::JOR };
static const short a_JPY[2] = { 1, Country::JPN };
static const short a_KES[2] = { 1, Country::KEN };
static const short a_KGS[2] = { 1, Country::KGZ };
static const short a_KHR[2] = { 1, Country::KHM };
static const short a_KMF[2] = { 1, Country::COM };
static const short a_KPW[2] = { 1, Country::PRK };
static const short a_KRW[2] = { 1, Country::KOR };
static const short a_KWD[2] = { 1, Country::KWT };
static const short a_KYD[2] = { 1, Country::CYM };
static const short a_KZT[2] = { 1, Country::KAZ };
static const short a_LAK[2] = { 1, Country::LAO };
static const short a_LBP[2] = { 1, Country::LBN };
static const short a_LKR[2] = { 1, Country::LKA };
static const short a_LRD[2] = { 1, Country::LBR };
static const short a_LSL[2] = { 1, Country::LSO };
static const short a_LTL[2] = { 1, Country::LTU };
static const short a_LUF[2] = { 1, Country::LUX };
static const short a_LVL[2] = { 1, Country::LVA };
static const short a_LYD[2] = { 1, Country::LBY };
static const short a_MAD[3] = { 2, Country::ESH, Country::MAR };
static const short a_MDL[2] = { 1, Country::MDA };
static const short a_MGA[2] = { 1, Country::MDG };
static const short a_MGF[2] = { 1, Country::MDG };
static const short a_MKD[2] = { 1, Country::MKD };
static const short a_MMK[2] = { 1, Country::MMR };
static const short a_MNT[2] = { 1, Country::MNG };
static const short a_MOP[2] = { 1, Country::MAC };
static const short a_MRO[2] = { 1, Country::MRT };
static const short a_MRU[2] = { 1, Country::MRT };
static const short a_MTL[2] = { 1, Country::MLT };
static const short a_MUR[2] = { 1, Country::MUS };
static const short a_MVR[2] = { 1, Country::MDV };
static const short a_MWK[2] = { 1, Country::MWI };
static const short a_MXN[2] = { 1, Country::MEX };
static const short a_MXV[2] = { 1, Country::MEX };
static const short a_MYR[2] = { 1, Country::MYS };
static const short a_MZM[2] = { 1, Country::MOZ };
static const short a_MZN[2] = { 1, Country::MOZ };
static const short a_NAD[2] = { 1, Country::NAM };
static const short a_NGN[2] = { 1, Country::NGA };
static const short a_NIO[2] = { 1, Country::NIC };
static const short a_NLG[2] = { 1, Country::NLD };
static const short a_NOK[4] = { 3, Country::BVT, Country::NOR, Country::SJM };
static const short a_NPR[2] = { 1, Country::NPL };
static const short a_NZD[6] = { 5, Country::COK, Country::NIU, Country::NZL, Country::PCN, Country::TKL };
static const short a_OMR[2] = { 1, Country::OMN };
static const short a_PAB[2] = { 1, Country::PAN };
static const short a_PEN[2] = { 1, Country::PER };
static const short a_PGK[2] = { 1, Country::PNG };
static const short a_PHP[2] = { 1, Country::PHL };
static const short a_PKR[2] = { 1, Country::PAK };
static const short a_PLN[2] = { 1, Country::POL };
static const short a_PLZ[2] = { 1, Country::POL };
static const short a_PTE[2] = { 1, Country::PRT };
static const short a_PYG[2] = { 1, Country::PRY };
static const short a_QAR[2] = { 1, Country::QAT };
static const short a_ROL[2] = { 1, Country::ROU };
static const short a_RON[2] = { 1, Country::ROU };
static const short a_RSD[2] = { 1, Country::SRB };
static const short a_RUB[2] = { 1, Country::RUS };
static const short a_RUR[2] = { 1, Country::XXX };
static const short a_RWF[2] = { 1, Country::RWA };
static const short a_SAR[2] = { 1, Country::SAU };
static const short a_SBD[2] = { 1, Country::SLB };
static const short a_SCR[2] = { 1, Country::SYC };
static const short a_SDD[2] = { 1, Country::SDN };
static const short a_SDG[2] = { 1, Country::SDN };
static const short a_SEK[2] = { 1, Country::SWE };
static const short a_SGD[2] = { 1, Country::SGP };
static const short a_SHP[2] = { 1, Country::SHN };
static const short a_SIT[2] = { 1, Country::SVN };
static const short a_SKK[2] = { 1, Country::SVK };
static const short a_SLE[2] = { 1, Country::SLE };
static const short a_SLL[2] = { 1, Country::SLE };
static const short a_SOS[2] = { 1, Country::SOM };
static const short a_SRD[2] = { 1, Country::SUR };
static const short a_SRG[2] = { 1, Country::SUR };
static const short a_SSP[2] = { 1, Country::SSD };
static const short a_STD[2] = { 1, Country::STP };
static const short a_STN[2] = { 1, Country::STP };
static const short a_SVC[2] = { 1, Country::SLV };
static const short a_SYP[2] = { 1, Country::SYR };
static const short a_SZL[2] = { 1, Country::SWZ };
static const short a_THB[2] = { 1, Country::THA };
static const short a_TJR[2] = { 1, Country::TJK };
static const short a_TJS[2] = { 1, Country::TJK };
static const short a_TMM[2] = { 1, Country::TKM };
static const short a_TMT[2] = { 1, Country::TKM };
static const short a_TND[2] = { 1, Country::TUN };
static const short a_TOP[2] = { 1, Country::TON };
static const short a_TPE[2] = { 1, Country::PRT };
static const short a_TRL[2] = { 1, Country::TUR };
static const short a_TRY[2] = { 1, Country::TUR };
static const short a_TTD[2] = { 1, Country::TTO };
static const short a_TWD[2] = { 1, Country::TWN };
static const short a_TZS[2] = { 1, Country::TZA };
static const short a_UAH[2] = { 1, Country::UKR };
static const short a_UAK[2] = { 1, Country::UKR };
static const short a_UGX[2] = { 1, Country::UGA };
static const short a_USD[20] = { 19, Country::ASM, Country::BES, Country::ECU, Country::FSM, Country::GUM, Country::HTI, Country::IOT, Country::MHL, Country::MNP, Country::PAN, Country::PLW, Country::PRI, Country::SLV, Country::TCA, Country::TLS, Country::UMI, Country::USA, Country::VGB, Country::VIR };
static const short a_USN[2] = { 1, Country::USA };
static const short a_USS[2] = { 1, Country::USA };
static const short a_UYI[2] = { 1, Country::URY };
static const short a_UYU[2] = { 1, Country::URY };
static const short a_UYW[2] = { 1, Country::URY };
static const short a_UZS[2] = { 1, Country::UZB };
static const short a_VEB[2] = { 1, Country::VEN };
static const short a_VED[2] = { 1, Country::VEN };
static const short a_VEF[2] = { 1, Country::VEN };
static const short a_VES[2] = { 1, Country::VEN };
static const short a_VND[2] = { 1, Country::VNM };
static const short a_VUV[2] = { 1, Country::VUT };
static const short a_WST[2] = { 1, Country::WSM };
static const short a_XAF[7] = { 6, Country::CAF, Country::CMR, Country::COG, Country::GAB, Country::GNQ, Country::TCD };
static const short a_XAG[2] = { 1, Country::XXX };
static const short a_XAU[2] = { 1, Country::XXX };
static const short a_XBA[2] = { 1, Country::EUR };
static const short a_XBB[2] = { 1, Country::EUR };
static const short a_XBC[2] = { 1, Country::EUR };
static const short a_XBD[2] = { 1, Country::EUR };
static const short a_XCD[9] = { 8, Country::AIA, Country::ATG, Country::DMA, Country::GRD, Country::KNA, Country::LCA, Country::MSR, Country::VCT };
static const short a_XDR[2] = { 1, Country::XXX };
static const short a_XOF[9] = { 8, Country::BEN, Country::BFA, Country::CIV, Country::GNB, Country::MLI, Country::NER, Country::SEN, Country::TGO };
static const short a_XPD[2] = { 1, Country::XXX };
static const short a_XPF[4] = { 3, Country::NCL, Country::PYF, Country::WLF };
static const short a_XPT[2] = { 1, Country::XXX };
static const short a_XSU[2] = { 1, Country::XXX };
static const short a_XTS[2] = { 1, Country::XXX };
static const short a_XUA[2] = { 1, Country::XXX };
static const short a_XXX[3] = { 2, Country::ATA, Country::XXX };
static const short a_YDD[2] = { 1, Country::XXX };
static const short a_YER[2] = { 1, Country::YEM };
static const short a_YUD[2] = { 1, Country::XXX };
static const short a_YUM[2] = { 1, Country::XXX };
static const short a_ZAL[2] = { 1, Country::ZAF };
static const short a_ZAR[4] = { 3, Country::LSO, Country::NAM, Country::ZAF };
static const short a_ZMK[2] = { 1, Country::ZMB };
static const short a_ZMW[2] = { 1, Country::ZMB };
static const short a_ZRN[2] = { 1, Country::XXX };
static const short a_ZWD[2] = { 1, Country::ZWE };
static const short a_ZWL[2] = { 1, Country::ZWE };

// priniciple Currency to Countries
const short * Gazetteer::m_ccy2cids[Currency::NUMCURRENCY] = { 
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
a_ZWL  };

// Country to Currencies
static const short b_NOCOUNTRY[2] = { 1, Currency::NOCURRENCY };
static const short b_ABW[2] = { 1, Currency::AWG };
static const short b_AFG[2] = { 1, Currency::AFN };
static const short b_AGO[2] = { 1, Currency::AOA };
static const short b_AIA[2] = { 1, Currency::XCD };
static const short b_ALA[2] = { 1, Currency::EUR };
static const short b_ALB[2] = { 1, Currency::ALL };
static const short b_AND[2] = { 1, Currency::EUR };
static const short b_ARE[2] = { 1, Currency::AED };
static const short b_ARG[2] = { 1, Currency::ARS };
static const short b_ARM[2] = { 1, Currency::AMD };
static const short b_ASM[2] = { 1, Currency::USD };
static const short b_ATA[2] = { 1, Currency::XXX };
static const short b_ATF[2] = { 1, Currency::EUR };
static const short b_ATG[2] = { 1, Currency::XCD };
static const short b_AUS[2] = { 1, Currency::AUD };
static const short b_AUT[2] = { 1, Currency::EUR };
static const short b_AZE[2] = { 1, Currency::AZN };
static const short b_BDI[2] = { 1, Currency::BIF };
static const short b_BEL[2] = { 1, Currency::EUR };
static const short b_BEN[2] = { 1, Currency::XOF };
static const short b_BES[2] = { 1, Currency::USD };
static const short b_BFA[2] = { 1, Currency::XOF };
static const short b_BGD[2] = { 1, Currency::BDT };
static const short b_BGR[2] = { 1, Currency::BGN };
static const short b_BHR[2] = { 1, Currency::BHD };
static const short b_BHS[2] = { 1, Currency::BSD };
static const short b_BIH[2] = { 1, Currency::BAM };
static const short b_BLM[2] = { 1, Currency::EUR };
static const short b_BLR[2] = { 1, Currency::BYN };
static const short b_BLZ[2] = { 1, Currency::BZD };
static const short b_BMU[2] = { 1, Currency::BMD };
static const short b_BOL[3] = { 2, Currency::BOB, Currency::BOV };
static const short b_BRA[2] = { 1, Currency::BRL };
static const short b_BRB[2] = { 1, Currency::BBD };
static const short b_BRN[2] = { 1, Currency::BND };
static const short b_BTN[3] = { 2, Currency::INR, Currency::BTN };
static const short b_BVT[2] = { 1, Currency::NOK };
static const short b_BWA[2] = { 1, Currency::BWP };
static const short b_CAF[2] = { 1, Currency::XAF };
static const short b_CAN[2] = { 1, Currency::CAD };
static const short b_CCK[2] = { 1, Currency::AUD };
static const short b_CHE[4] = { 3, Currency::CHF, Currency::CHE, Currency::CHW };
static const short b_CHL[3] = { 2, Currency::CLP, Currency::CLF };
static const short b_CHN[2] = { 1, Currency::CNY };
static const short b_CIV[2] = { 1, Currency::XOF };
static const short b_CMR[2] = { 1, Currency::XAF };
static const short b_COD[2] = { 1, Currency::CDF };
static const short b_COG[2] = { 1, Currency::XAF };
static const short b_COK[2] = { 1, Currency::NZD };
static const short b_COL[3] = { 2, Currency::COP, Currency::COU };
static const short b_COM[2] = { 1, Currency::KMF };
static const short b_CPV[2] = { 1, Currency::CVE };
static const short b_CRI[2] = { 1, Currency::CRC };
static const short b_CUB[3] = { 2, Currency::CUP, Currency::CUC };
static const short b_CUW[2] = { 1, Currency::ANG };
static const short b_CXR[2] = { 1, Currency::AUD };
static const short b_CYM[2] = { 1, Currency::KYD };
static const short b_CYP[2] = { 1, Currency::EUR };
static const short b_CZE[2] = { 1, Currency::CZK };
static const short b_DEU[2] = { 1, Currency::EUR };
static const short b_DJI[2] = { 1, Currency::DJF };
static const short b_DMA[2] = { 1, Currency::XCD };
static const short b_DNK[2] = { 1, Currency::DKK };
static const short b_DOM[2] = { 1, Currency::DOP };
static const short b_DZA[2] = { 1, Currency::DZD };
static const short b_ECU[2] = { 1, Currency::USD };
static const short b_EGY[2] = { 1, Currency::EGP };
static const short b_ERI[2] = { 1, Currency::ERN };
static const short b_ESH[2] = { 1, Currency::MAD };
static const short b_ESP[2] = { 1, Currency::EUR };
static const short b_EST[2] = { 1, Currency::EUR };
static const short b_ETH[2] = { 1, Currency::ETB };
static const short b_EUR[2] = { 1, Currency::EUR };
static const short b_FIN[2] = { 1, Currency::EUR };
static const short b_FJI[2] = { 1, Currency::FJD };
static const short b_FLK[2] = { 1, Currency::FKP };
static const short b_FRA[2] = { 1, Currency::EUR };
static const short b_FRO[2] = { 1, Currency::DKK };
static const short b_FSM[2] = { 1, Currency::USD };
static const short b_GAB[2] = { 1, Currency::XAF };
static const short b_GBR[2] = { 1, Currency::GBP };
static const short b_GEO[2] = { 1, Currency::GEL };
static const short b_GGY[2] = { 1, Currency::GBP };
static const short b_GHA[2] = { 1, Currency::GHS };
static const short b_GIB[2] = { 1, Currency::GIP };
static const short b_GIN[2] = { 1, Currency::GNF };
static const short b_GLP[2] = { 1, Currency::EUR };
static const short b_GMB[2] = { 1, Currency::GMD };
static const short b_GNB[2] = { 1, Currency::XOF };
static const short b_GNQ[2] = { 1, Currency::XAF };
static const short b_GRC[2] = { 1, Currency::EUR };
static const short b_GRD[2] = { 1, Currency::XCD };
static const short b_GRL[2] = { 1, Currency::DKK };
static const short b_GTM[2] = { 1, Currency::GTQ };
static const short b_GUF[2] = { 1, Currency::EUR };
static const short b_GUM[2] = { 1, Currency::USD };
static const short b_GUY[2] = { 1, Currency::GYD };
static const short b_HKG[2] = { 1, Currency::HKD };
static const short b_HMD[2] = { 1, Currency::AUD };
static const short b_HND[2] = { 1, Currency::HNL };
static const short b_HRV[2] = { 1, Currency::EUR };
static const short b_HTI[3] = { 2, Currency::HTG, Currency::USD };
static const short b_HUN[2] = { 1, Currency::HUF };
static const short b_IDN[2] = { 1, Currency::IDR };
static const short b_IMN[2] = { 1, Currency::GBP };
static const short b_IND[2] = { 1, Currency::INR };
static const short b_IOT[2] = { 1, Currency::USD };
static const short b_IRL[2] = { 1, Currency::EUR };
static const short b_IRN[2] = { 1, Currency::IRR };
static const short b_IRQ[2] = { 1, Currency::IQD };
static const short b_ISL[2] = { 1, Currency::ISK };
static const short b_ISR[2] = { 1, Currency::ILS };
static const short b_ITA[2] = { 1, Currency::EUR };
static const short b_JAM[2] = { 1, Currency::JMD };
static const short b_JEY[2] = { 1, Currency::GBP };
static const short b_JOR[2] = { 1, Currency::JOD };
static const short b_JPN[2] = { 1, Currency::JPY };
static const short b_KAZ[2] = { 1, Currency::KZT };
static const short b_KEN[2] = { 1, Currency::KES };
static const short b_KGZ[2] = { 1, Currency::KGS };
static const short b_KHM[2] = { 1, Currency::KHR };
static const short b_KIR[2] = { 1, Currency::AUD };
static const short b_KNA[2] = { 1, Currency::XCD };
static const short b_KOR[2] = { 1, Currency::KRW };
static const short b_KWT[2] = { 1, Currency::KWD };
static const short b_LAO[2] = { 1, Currency::LAK };
static const short b_LBN[2] = { 1, Currency::LBP };
static const short b_LBR[2] = { 1, Currency::LRD };
static const short b_LBY[2] = { 1, Currency::LYD };
static const short b_LCA[2] = { 1, Currency::XCD };
static const short b_LIE[2] = { 1, Currency::CHF };
static const short b_LKA[2] = { 1, Currency::LKR };
static const short b_LSO[3] = { 2, Currency::LSL, Currency::ZAR };
static const short b_LTU[2] = { 1, Currency::EUR };
static const short b_LUX[2] = { 1, Currency::EUR };
static const short b_LVA[2] = { 1, Currency::EUR };
static const short b_MAC[2] = { 1, Currency::MOP };
static const short b_MAF[2] = { 1, Currency::EUR };
static const short b_MAR[2] = { 1, Currency::MAD };
static const short b_MCO[2] = { 1, Currency::EUR };
static const short b_MDA[2] = { 1, Currency::MDL };
static const short b_MDG[2] = { 1, Currency::MGA };
static const short b_MDV[2] = { 1, Currency::MVR };
static const short b_MEX[3] = { 2, Currency::MXN, Currency::MXV };
static const short b_MHL[2] = { 1, Currency::USD };
static const short b_MKD[2] = { 1, Currency::MKD };
static const short b_MLI[2] = { 1, Currency::XOF };
static const short b_MLT[2] = { 1, Currency::EUR };
static const short b_MMR[2] = { 1, Currency::MMK };
static const short b_MNE[2] = { 1, Currency::EUR };
static const short b_MNG[2] = { 1, Currency::MNT };
static const short b_MNP[2] = { 1, Currency::USD };
static const short b_MOZ[2] = { 1, Currency::MZN };
static const short b_MRT[2] = { 1, Currency::MRU };
static const short b_MSR[2] = { 1, Currency::XCD };
static const short b_MTQ[2] = { 1, Currency::EUR };
static const short b_MUS[2] = { 1, Currency::MUR };
static const short b_MWI[2] = { 1, Currency::MWK };
static const short b_MYS[2] = { 1, Currency::MYR };
static const short b_MYT[2] = { 1, Currency::EUR };
static const short b_NAM[3] = { 2, Currency::NAD, Currency::ZAR };
static const short b_NCL[2] = { 1, Currency::XPF };
static const short b_NER[2] = { 1, Currency::XOF };
static const short b_NFK[2] = { 1, Currency::AUD };
static const short b_NGA[2] = { 1, Currency::NGN };
static const short b_NIC[2] = { 1, Currency::NIO };
static const short b_NIU[2] = { 1, Currency::NZD };
static const short b_NLD[2] = { 1, Currency::EUR };
static const short b_NOR[2] = { 1, Currency::NOK };
static const short b_NPL[2] = { 1, Currency::NPR };
static const short b_NRU[2] = { 1, Currency::AUD };
static const short b_NZL[2] = { 1, Currency::NZD };
static const short b_OMN[2] = { 1, Currency::OMR };
static const short b_PAK[2] = { 1, Currency::PKR };
static const short b_PAN[3] = { 2, Currency::PAB, Currency::USD };
static const short b_PCN[2] = { 1, Currency::NZD };
static const short b_PER[2] = { 1, Currency::PEN };
static const short b_PHL[2] = { 1, Currency::PHP };
static const short b_PLW[2] = { 1, Currency::USD };
static const short b_PNG[2] = { 1, Currency::PGK };
static const short b_POL[2] = { 1, Currency::PLN };
static const short b_PRI[2] = { 1, Currency::USD };
static const short b_PRK[2] = { 1, Currency::KPW };
static const short b_PRT[2] = { 1, Currency::EUR };
static const short b_PRY[2] = { 1, Currency::PYG };
static const short b_PSE[2] = { 1, Currency::ILS };
static const short b_PYF[2] = { 1, Currency::XPF };
static const short b_QAT[2] = { 1, Currency::QAR };
static const short b_REU[2] = { 1, Currency::EUR };
static const short b_ROU[2] = { 1, Currency::RON };
static const short b_RUS[2] = { 1, Currency::RUB };
static const short b_RWA[2] = { 1, Currency::RWF };
static const short b_SAU[2] = { 1, Currency::SAR };
static const short b_SDN[2] = { 1, Currency::SDG };
static const short b_SEN[2] = { 1, Currency::XOF };
static const short b_SGP[2] = { 1, Currency::SGD };
static const short b_SGS[2] = { 1, Currency::GBP };
static const short b_SHN[2] = { 1, Currency::SHP };
static const short b_SJM[2] = { 1, Currency::NOK };
static const short b_SLB[2] = { 1, Currency::SBD };
static const short b_SLE[3] = { 2, Currency::SLL, Currency::SLE };
static const short b_SLV[3] = { 2, Currency::SVC, Currency::USD };
static const short b_SMR[2] = { 1, Currency::EUR };
static const short b_SOM[2] = { 1, Currency::SOS };
static const short b_SPM[2] = { 1, Currency::EUR };
static const short b_SRB[2] = { 1, Currency::RSD };
static const short b_SSD[2] = { 1, Currency::SSP };
static const short b_STP[2] = { 1, Currency::STN };
static const short b_SUR[2] = { 1, Currency::SRD };
static const short b_SVK[2] = { 1, Currency::EUR };
static const short b_SVN[2] = { 1, Currency::EUR };
static const short b_SWE[2] = { 1, Currency::SEK };
static const short b_SWZ[2] = { 1, Currency::SZL };
static const short b_SXM[2] = { 1, Currency::ANG };
static const short b_SYC[2] = { 1, Currency::SCR };
static const short b_SYR[2] = { 1, Currency::SYP };
static const short b_TCA[2] = { 1, Currency::USD };
static const short b_TCD[2] = { 1, Currency::XAF };
static const short b_TGO[2] = { 1, Currency::XOF };
static const short b_THA[2] = { 1, Currency::THB };
static const short b_TJK[2] = { 1, Currency::TJS };
static const short b_TKL[2] = { 1, Currency::NZD };
static const short b_TKM[2] = { 1, Currency::TMT };
static const short b_TLS[2] = { 1, Currency::USD };
static const short b_TON[2] = { 1, Currency::TOP };
static const short b_TTO[2] = { 1, Currency::TTD };
static const short b_TUN[2] = { 1, Currency::TND };
static const short b_TUR[2] = { 1, Currency::TRY };
static const short b_TUV[2] = { 1, Currency::AUD };
static const short b_TWN[2] = { 1, Currency::TWD };
static const short b_TZA[2] = { 1, Currency::TZS };
static const short b_UGA[2] = { 1, Currency::UGX };
static const short b_UKR[2] = { 1, Currency::UAH };
static const short b_UMI[2] = { 1, Currency::USD };
static const short b_URY[4] = { 3, Currency::UYU, Currency::UYI, Currency::UYW };
static const short b_USA[3] = { 2, Currency::USD, Currency::USN };
static const short b_UZB[2] = { 1, Currency::UZS };
static const short b_VAT[2] = { 1, Currency::EUR };
static const short b_VCT[2] = { 1, Currency::XCD };
static const short b_VEN[3] = { 2, Currency::VES, Currency::VED };
static const short b_VGB[2] = { 1, Currency::USD };
static const short b_VIR[2] = { 1, Currency::USD };
static const short b_VNM[2] = { 1, Currency::VND };
static const short b_VUT[2] = { 1, Currency::VUV };
static const short b_WLF[2] = { 1, Currency::XPF };
static const short b_WSM[2] = { 1, Currency::WST };
static const short b_XAF[2] = { 1, Currency::XAF };
static const short b_XCD[2] = { 1, Currency::XCD };
static const short b_XOF[2] = { 1, Currency::XOF };
static const short b_XPF[2] = { 1, Currency::XPF };
static const short b_XXX[10] = { 9, Currency::XAU, Currency::XAG, Currency::XPT, Currency::XPD, Currency::XXX, Currency::XDR, Currency::XTS, Currency::XUA, Currency::XSU };
static const short b_YEM[2] = { 1, Currency::YER };
static const short b_ZAF[2] = { 1, Currency::ZAR };
static const short b_ZMB[2] = { 1, Currency::ZMW };
static const short b_ZWE[2] = { 1, Currency::ZWL };

// Country to Currencies
const short * Gazetteer::m_cid2ccys[Country::NUMCOUNTRY] = { 
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

// Country to Cities
static const short c_NOCOUNTRY[2] = { 1, City::NOCITY };
static const short c_ABW[2] = { 1, City::AUA };
static const short c_AFG[2] = { 1, City::KBL };
static const short c_AGO[2] = { 1, City::LAD };
static const short c_AIA[2] = { 1, City::AXA };
static const short c_ALA[2] = { 1, City::MHQ };
static const short c_ALB[2] = { 1, City::TIA };
static const short c_AND[2] = { 1, City::AD0 };
static const short c_ARE[3] = { 2, City::AUH, City::DXB };
static const short c_ARG[8] = { 7, City::AEP, City::CNQ, City::COR, City::MDZ, City::ROS, City::SFN, City::TUC };
static const short c_ARM[2] = { 1, City::EVN };
static const short c_ASM[2] = { 1, City::PPG };
static const short c_ATA[2] = { 1, City::XXX };
static const short c_ATF[2] = { 1, City::TF0 };
static const short c_ATG[2] = { 1, City::ANU };
static const short c_AUS[5] = { 4, City::CBR, City::MEL, City::NTL, City::SYD };
static const short c_AUT[6] = { 5, City::GRZ, City::KLU, City::LNZ, City::SZG, City::VIE };
static const short c_AZE[2] = { 1, City::GYD };
static const short c_BDI[2] = { 1, City::BJM };
static const short c_BEL[4] = { 3, City::ANR, City::BRU, City::LV0 };
static const short c_BEN[2] = { 1, City::COO };
static const short c_BES[2] = { 1, City::BON };
static const short c_BFA[2] = { 1, City::OUA };
static const short c_BGD[3] = { 2, City::CGP, City::DAC };
static const short c_BGR[2] = { 1, City::SOF };
static const short c_BHR[2] = { 1, City::BAH };
static const short c_BHS[2] = { 1, City::NAS };
static const short c_BIH[3] = { 2, City::BNX, City::SJJ };
static const short c_BLM[2] = { 1, City::SBH };
static const short c_BLR[2] = { 1, City::MSQ };
static const short c_BLZ[2] = { 1, City::BCV };
static const short c_BMU[2] = { 1, City::YHM };
static const short c_BOL[2] = { 1, City::LPB };
static const short c_BRA[7] = { 6, City::BSB, City::CWB, City::GRU, City::MGF, City::RIO, City::VIX };
static const short c_BRB[2] = { 1, City::BGI };
static const short c_BRN[2] = { 1, City::BWN };
static const short c_BTN[2] = { 1, City::PBH };
static const short c_BVT[2] = { 1, City::OSL };
static const short c_BWA[2] = { 1, City::GBE };
static const short c_CAF[2] = { 1, City::BGF };
static const short c_CAN[8] = { 7, City::YMQ, City::YOW, City::YTZ, City::YVR, City::YWG, City::YYC, City::YYZ };
static const short c_CCK[2] = { 1, City::CCK };
static const short c_CHE[6] = { 5, City::BRN, City::BSL, City::GVA, City::LU0, City::ZRH };
static const short c_CHL[4] = { 3, City::ESR, City::STI, City::VPS };
static const short c_CHN[8] = { 7, City::BJS, City::CGO, City::DLC, City::RIZ, City::SHA, City::SZX, City::WUX };
static const short c_CIV[2] = { 1, City::ABJ };
static const short c_CMR[3] = { 2, City::DLA, City::YAO };
static const short c_COD[2] = { 1, City::FIH };
static const short c_COG[2] = { 1, City::BZV };
static const short c_COK[2] = { 1, City::RAR };
static const short c_COL[2] = { 1, City::BOG };
static const short c_COM[2] = { 1, City::HAH };
static const short c_CPV[2] = { 1, City::RAI };
static const short c_CRI[2] = { 1, City::SJI };
static const short c_CUB[2] = { 1, City::HAV };
static const short c_CUW[2] = { 1, City::CUR };
static const short c_CXR[2] = { 1, City::XCH };
static const short c_CYM[2] = { 1, City::GCM };
static const short c_CYP[4] = { 3, City::LCA, City::LM0, City::NIC };
static const short c_CZE[2] = { 1, City::PRG };
static const short c_DEU[14] = { 13, City::BER, City::BNJ, City::BRE, City::FRA, City::HAJ, City::HAM, City::LEJ, City::MUC, City::OB0, City::QDU, City::RB0, City::STR, City::SW0 };
static const short c_DJI[2] = { 1, City::JIB };
static const short c_DMA[2] = { 1, City::DCF };
static const short c_DNK[8] = { 7, City::AB0, City::AL0, City::CPH, City::GR0, City::HS0, City::RA0, City::SI0 };
static const short c_DOM[2] = { 1, City::SDQ };
static const short c_DZA[2] = { 1, City::ALG };
static const short c_ECU[3] = { 2, City::GYE, City::UIO };
static const short c_EGY[3] = { 2, City::ALY, City::CAI };
static const short c_ERI[2] = { 1, City::ASM };
static const short c_ESH[2] = { 1, City::EUN };
static const short c_ESP[11] = { 10, City::BCN, City::BIO, City::GRX, City::LCG, City::MAD, City::PMI, City::QSA, City::SDR, City::VLC, City::ZAZ };
static const short c_EST[2] = { 1, City::TLL };
static const short c_ETH[2] = { 1, City::ADD };
static const short c_EUR[2] = { 1, City::XXX };
static const short c_FIN[4] = { 3, City::ES0, City::HEL, City::TMP };
static const short c_FJI[2] = { 1, City::SUV };
static const short c_FLK[2] = { 1, City::PSY };
static const short c_FRA[6] = { 5, City::LYS, City::MRS, City::PAR, City::SXB, City::TLS };
static const short c_FRO[2] = { 1, City::FAE };
static const short c_FSM[2] = { 1, City::PNI };
static const short c_GAB[2] = { 1, City::LBV };
static const short c_GBR[19] = { 18, City::ABZ, City::AY0, City::BFS, City::BHX, City::BRS, City::CBG, City::CWL, City::DND, City::EDI, City::ESH, City::GLA, City::LBA, City::LDY, City::LON, City::LPL, City::MAN, City::NCL, City::SWS };
static const short c_GEO[2] = { 1, City::TBS };
static const short c_GGY[2] = { 1, City::GCI };
static const short c_GHA[2] = { 1, City::ACC };
static const short c_GIB[2] = { 1, City::GIB };
static const short c_GIN[2] = { 1, City::CKY };
static const short c_GLP[2] = { 1, City::BBR };
static const short c_GMB[2] = { 1, City::BJL };
static const short c_GNB[2] = { 1, City::OXB };
static const short c_GNQ[2] = { 1, City::SSG };
static const short c_GRC[3] = { 2, City::ATH, City::SKG };
static const short c_GRD[2] = { 1, City::GND };
static const short c_GRL[2] = { 1, City::GOH };
static const short c_GTM[2] = { 1, City::GUA };
static const short c_GUF[2] = { 1, City::CAY };
static const short c_GUM[2] = { 1, City::GUM };
static const short c_GUY[2] = { 1, City::GEO };
static const short c_HKG[2] = { 1, City::HKG };
static const short c_HMD[2] = { 1, City::CBR };
static const short c_HND[3] = { 2, City::SAP, City::TGU };
static const short c_HRV[4] = { 3, City::SPU, City::VZ0, City::ZAG };
static const short c_HTI[2] = { 1, City::PAP };
static const short c_HUN[2] = { 1, City::BUD };
static const short c_IDN[3] = { 2, City::CGK, City::SUB };
static const short c_IMN[2] = { 1, City::IOM };
static const short c_IND[12] = { 11, City::AMD, City::ATQ, City::BLR, City::BOM, City::CCU, City::DEL, City::GC0, City::GH0, City::GOI, City::IDR, City::MAA };
static const short c_IOT[2] = { 1, City::NKW };
static const short c_IRL[4] = { 3, City::DUB, City::ORK, City::SNN };
static const short c_IRN[2] = { 1, City::THR };
static const short c_IRQ[2] = { 1, City::BGW };
static const short c_ISL[2] = { 1, City::RKV };
static const short c_ISR[3] = { 2, City::JSR, City::TLV };
static const short c_ITA[11] = { 10, City::BGY, City::BLQ, City::FLR, City::GOA, City::MIL, City::NAP, City::QIC, City::QPA, City::ROM, City::TRN };
static const short c_JAM[2] = { 1, City::KIN };
static const short c_JEY[2] = { 1, City::JER };
static const short c_JOR[2] = { 1, City::AMM };
static const short c_JPN[11] = { 10, City::FUK, City::HIJ, City::KIJ, City::NGO, City::OKD, City::OSA, City::SK0, City::TYO, City::UKB, City::UKY };
static const short c_KAZ[3] = { 2, City::ALA, City::TSE };
static const short c_KEN[2] = { 1, City::NBO };
static const short c_KGZ[2] = { 1, City::FRU };
static const short c_KHM[2] = { 1, City::PNH };
static const short c_KIR[2] = { 1, City::TRW };
static const short c_KNA[2] = { 1, City::SKB };
static const short c_KOR[2] = { 1, City::SEL };
static const short c_KWT[2] = { 1, City::KWI };
static const short c_LAO[2] = { 1, City::VTE };
static const short c_LBN[2] = { 1, City::BEY };
static const short c_LBR[2] = { 1, City::MLW };
static const short c_LBY[2] = { 1, City::TIP };
static const short c_LCA[2] = { 1, City::SLU };
static const short c_LIE[2] = { 1, City::LI0 };
static const short c_LKA[2] = { 1, City::CMB };
static const short c_LSO[2] = { 1, City::MSU };
static const short c_LTU[2] = { 1, City::VNO };
static const short c_LUX[3] = { 2, City::EA0, City::LUX };
static const short c_LVA[2] = { 1, City::RIX };
static const short c_MAC[2] = { 1, City::MFM };
static const short c_MAF[2] = { 1, City::SFG };
static const short c_MAR[3] = { 2, City::CMN, City::RBA };
static const short c_MCO[2] = { 1, City::MCM };
static const short c_MDA[2] = { 1, City::KIV };
static const short c_MDG[2] = { 1, City::TNR };
static const short c_MDV[2] = { 1, City::MLE };
static const short c_MEX[3] = { 2, City::EP0, City::MEX };
static const short c_MHL[2] = { 1, City::MAJ };
static const short c_MKD[2] = { 1, City::SKP };
static const short c_MLI[2] = { 1, City::BKO };
static const short c_MLT[2] = { 1, City::MLA };
static const short c_MMR[2] = { 1, City::RGN };
static const short c_MNE[2] = { 1, City::TGD };
static const short c_MNG[2] = { 1, City::ULN };
static const short c_MNP[2] = { 1, City::SPN };
static const short c_MOZ[2] = { 1, City::MPM };
static const short c_MRT[2] = { 1, City::NKC };
static const short c_MSR[2] = { 1, City::MNI };
static const short c_MTQ[2] = { 1, City::FDF };
static const short c_MUS[3] = { 2, City::EB0, City::MRU };
static const short c_MWI[3] = { 2, City::BLZ, City::LLW };
static const short c_MYS[3] = { 2, City::KUL, City::LBU };
static const short c_MYT[2] = { 1, City::DZA };
static const short c_NAM[2] = { 1, City::WDH };
static const short c_NCL[2] = { 1, City::NOU };
static const short c_NER[2] = { 1, City::NIM };
static const short c_NFK[2] = { 1, City::NLK };
static const short c_NGA[3] = { 2, City::ABV, City::LOS };
static const short c_NIC[2] = { 1, City::MGA };
static const short c_NIU[2] = { 1, City::IUE };
static const short c_NLD[6] = { 5, City::AMS, City::HG0, City::RTM, City::SH0, City::UTC };
static const short c_NOR[5] = { 4, City::BGO, City::OSL, City::TOS, City::TRD };
static const short c_NPL[2] = { 1, City::KTM };
static const short c_NRU[2] = { 1, City::INU };
static const short c_NZL[4] = { 3, City::AKL, City::CHC, City::WLG };
static const short c_OMN[2] = { 1, City::MCT };
static const short c_PAK[4] = { 3, City::ISB, City::KHI, City::LHE };
static const short c_PAN[2] = { 1, City::PAC };
static const short c_PCN[2] = { 1, City::PN0 };
static const short c_PER[2] = { 1, City::LIM };
static const short c_PHL[4] = { 3, City::MC0, City::MNL, City::PC0 };
static const short c_PLW[2] = { 1, City::ROR };
static const short c_PNG[2] = { 1, City::POM };
static const short c_POL[4] = { 3, City::KRK, City::WAW, City::WRO };
static const short c_PRI[2] = { 1, City::SJU };
static const short c_PRK[2] = { 1, City::FNJ };
static const short c_PRT[3] = { 2, City::LIS, City::OPO };
static const short c_PRY[2] = { 1, City::ASU };
static const short c_PSE[2] = { 1, City::PS0 };
static const short c_PYF[2] = { 1, City::PPT };
static const short c_QAT[2] = { 1, City::DOH };
static const short c_REU[2] = { 1, City::RUN };
static const short c_ROU[4] = { 3, City::BBU, City::CLJ, City::SBZ };
static const short c_RUS[11] = { 10, City::ARH, City::GOJ, City::KUF, City::LED, City::MOW, City::OVB, City::ROV, City::SVX, City::VOG, City::VVO };
static const short c_RWA[2] = { 1, City::KGL };
static const short c_SAU[3] = { 2, City::JED, City::RUH };
static const short c_SDN[2] = { 1, City::KRT };
static const short c_SEN[2] = { 1, City::DKR };
static const short c_SGP[2] = { 1, City::SIN };
static const short c_SGS[2] = { 1, City::GS0 };
static const short c_SHN[2] = { 1, City::ASI };
static const short c_SJM[2] = { 1, City::LYR };
static const short c_SLB[2] = { 1, City::HIR };
static const short c_SLE[2] = { 1, City::FNA };
static const short c_SLV[2] = { 1, City::SAL };
static const short c_SMR[2] = { 1, City::SM0 };
static const short c_SOM[2] = { 1, City::MGQ };
static const short c_SPM[2] = { 1, City::FSP };
static const short c_SRB[3] = { 2, City::BEG, City::QND };
static const short c_SSD[2] = { 1, City::JUB };
static const short c_STP[2] = { 1, City::TMS };
static const short c_SUR[2] = { 1, City::PBM };
static const short c_SVK[3] = { 2, City::BTS, City::ILZ };
static const short c_SVN[2] = { 1, City::LJU };
static const short c_SWE[4] = { 3, City::BMA, City::GOT, City::MMX };
static const short c_SWZ[2] = { 1, City::QMN };
static const short c_SXM[2] = { 1, City::SXM };
static const short c_SYC[2] = { 1, City::SEZ };
static const short c_SYR[2] = { 1, City::DAM };
static const short c_TCA[2] = { 1, City::GDT };
static const short c_TCD[2] = { 1, City::NDJ };
static const short c_TGO[2] = { 1, City::LFW };
static const short c_THA[2] = { 1, City::BKK };
static const short c_TJK[2] = { 1, City::DYU };
static const short c_TKL[2] = { 1, City::TK0 };
static const short c_TKM[2] = { 1, City::ASB };
static const short c_TLS[2] = { 1, City::DIL };
static const short c_TON[2] = { 1, City::TBU };
static const short c_TTO[2] = { 1, City::POS };
static const short c_TUN[2] = { 1, City::TUN };
static const short c_TUR[4] = { 3, City::ADB, City::ESB, City::IST };
static const short c_TUV[2] = { 1, City::FUN };
static const short c_TWN[2] = { 1, City::TSA };
static const short c_TZA[2] = { 1, City::DAR };
static const short c_UGA[2] = { 1, City::KLA };
static const short c_UKR[5] = { 4, City::DNK, City::HRK, City::IEV, City::ODS };
static const short c_UMI[2] = { 1, City::DCA };
static const short c_URY[2] = { 1, City::MVD };
static const short c_USA[76] = { 75, City::ALB, City::ANC, City::ANP, City::ATL, City::AUG, City::AUS, City::BCT, City::BDL, City::BIS, City::BNA, City::BOI, City::BOS, City::BTR, City::BTV, City::CAE, City::CHI, City::CHS, City::CLT, City::CMH, City::CON, City::CRW, City::CSN, City::CYS, City::DCA, City::DEN, City::DFW, City::DOV, City::DSM, City::EWR, City::FC0, City::FFT, City::FOE, City::GW0, City::HLN, City::HNL, City::IAH, City::ILG, City::IND, City::JAN, City::JEF, City::JNU, City::LAN, City::LAX, City::LIT, City::LNK, City::MCI, City::MDT, City::MGN, City::MIA, City::MKE, City::MSN, City::MSP, City::MV0, City::NYC, City::OKC, City::OLM, City::PCT, City::PHL, City::PHX, City::PIR, City::PVD, City::RDU, City::RIC, City::SAF, City::SFO, City::SLC, City::SLE, City::SMF, City::SPI, City::ST0, City::STP, City::TLH, City::TTN, City::VN0, City::WP0 };
static const short c_UZB[2] = { 1, City::TAS };
static const short c_VAT[2] = { 1, City::VA0 };
static const short c_VCT[2] = { 1, City::SVD };
static const short c_VEN[2] = { 1, City::CCS };
static const short c_VGB[2] = { 1, City::EIS };
static const short c_VIR[2] = { 1, City::STT };
static const short c_VNM[3] = { 2, City::HAN, City::SGN };
static const short c_VUT[2] = { 1, City::VLI };
static const short c_WLF[2] = { 1, City::WLS };
static const short c_WSM[2] = { 1, City::APW };
static const short c_XAF[2] = { 1, City::XXX };
static const short c_XCD[2] = { 1, City::XXX };
static const short c_XOF[2] = { 1, City::XXX };
static const short c_XPF[2] = { 1, City::XXX };
static const short c_XXX[3] = { 2, City::PRN, City::XXX };
static const short c_YEM[2] = { 1, City::SAH };
static const short c_ZAF[5] = { 4, City::BFN, City::CPT, City::JNB, City::PRY };
static const short c_ZMB[2] = { 1, City::LUN };
static const short c_ZWE[3] = { 2, City::HRE, City::VFA };

// Country to Cities
const short * Gazetteer::m_cid2ctys[Country::NUMCOUNTRY] = { 
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
c_XXX, c_YEM, c_ZAF, c_ZMB, c_ZWE,  };

// City to Markets
static const short d_NOCITY[2] = { 1, MarketId::NOMARKETID };
static const short d_AB0[2] = { 1, MarketId::SBSI };
static const short d_ABJ[4] = { 3, MarketId::CIV0, MarketId::XABJ, MarketId::XBRV };
static const short d_ABV[2] = { 1, MarketId::NGA0 };
static const short d_ABZ[2] = { 1, MarketId::XXXX };
static const short d_ACC[3] = { 2, MarketId::GHA0, MarketId::XGHA };
static const short d_AD0[2] = { 1, MarketId::AND0 };
static const short d_ADB[2] = { 1, MarketId::XTUR };
static const short d_ADD[2] = { 1, MarketId::ETH0 };
static const short d_AEP[8] = { 7, MarketId::ARG0, MarketId::BACE, MarketId::XA1X, MarketId::XBUE, MarketId::XMAB, MarketId::XMEV, MarketId::XMTB };
static const short d_AKL[3] = { 2, MarketId::XAUK, MarketId::XNEE };
static const short d_AL0[2] = { 1, MarketId::SNSI };
static const short d_ALA[3] = { 2, MarketId::ETSC, MarketId::XKAZ };
static const short d_ALB[2] = { 1, MarketId::XXXX };
static const short d_ALG[3] = { 2, MarketId::DZA0, MarketId::XALG };
static const short d_ALY[2] = { 1, MarketId::XXXX };
static const short d_AMD[2] = { 1, MarketId::NMCE };
static const short d_AMM[4] = { 3, MarketId::AMNL, MarketId::JOR0, MarketId::XAMM };
static const short d_AMS[100] = { 99, MarketId::ABNA, MarketId::ABNC, MarketId::AFSA, MarketId::AFSE, MarketId::AFSI, MarketId::AFSL, MarketId::AFSO, MarketId::AFSX, MarketId::ALXA, MarketId::BAPE, MarketId::BARU, MarketId::BEUD, MarketId::BEUE, MarketId::BEUF, MarketId::BEUO, MarketId::BEUP, MarketId::BEUT, MarketId::BTAM, MarketId::BTFE, MarketId::BTQE, MarketId::CABV, MarketId::CAPA, MarketId::CAVD, MarketId::CBAE, MarketId::CCRM, MarketId::CCXE, MarketId::CEDX, MarketId::CEUD, MarketId::CEUE, MarketId::CEUO, MarketId::CEUX, MarketId::COMM, MarketId::CPTX, MarketId::EBSD, MarketId::EBSF, MarketId::EBSI, MarketId::EBSN, MarketId::ECEU, MarketId::ECXE, MarketId::ETPA, MarketId::FLTB, MarketId::FLTR, MarketId::FLWX, MarketId::HCHC, MarketId::IEBS, MarketId::IMCO, MarketId::IMCT, MarketId::IMEQ, MarketId::INGB, MarketId::INGE, MarketId::INGF, MarketId::IPNL, MarketId::ISWN, MarketId::ISWO, MarketId::ISWP, MarketId::ISWT, MarketId::JLEU, MarketId::JNSI, MarketId::LEBV, MarketId::LISZ, MarketId::MANL, MarketId::MHBE, MarketId::MUBE, MarketId::MUSN, MarketId::MXNL, MarketId::NDEX, MarketId::NDXS, MarketId::NEXY, MarketId::NLD0, MarketId::NLPX, MarketId::NWNV, MarketId::OHVO, MarketId::RESE, MarketId::RESF, MarketId::RR4G, MarketId::STXS, MarketId::TNLA, MarketId::TOMD, MarketId::TOMX, MarketId::TOWR, MarketId::TQEA, MarketId::TQEB, MarketId::TQEM, MarketId::TQEX, MarketId::TRNL, MarketId::TWEA, MarketId::TWEM, MarketId::TWEO, MarketId::TWEU, MarketId::XACE, MarketId::XAEX, MarketId::XAMS, MarketId::XEUC, MarketId::XEUE, MarketId::XEUI, MarketId::XFTA, MarketId::XHFT, MarketId::XNXC, MarketId::XNXD };
static const short d_ANC[2] = { 1, MarketId::XXXX };
static const short d_ANP[2] = { 1, MarketId::XXXX };
static const short d_ANR[2] = { 1, MarketId::XANT };
static const short d_ANU[3] = { 2, MarketId::ATG0, MarketId::XFTX };
static const short d_APW[2] = { 1, MarketId::WSM0 };
static const short d_ARH[2] = { 1, MarketId::XXXX };
static const short d_ASB[2] = { 1, MarketId::TKM0 };
static const short d_ASI[2] = { 1, MarketId::SHN0 };
static const short d_ASM[2] = { 1, MarketId::ERI0 };
static const short d_ASU[3] = { 2, MarketId::PRY0, MarketId::XVPA };
static const short d_ATH[18] = { 17, MarketId::AAPA, MarketId::ABFI, MarketId::ASEX, MarketId::ENAX, MarketId::ERBX, MarketId::EUAX, MarketId::GRC0, MarketId::HDAT, MarketId::HEDE, MarketId::HEMO, MarketId::HESP, MarketId::HGSP, MarketId::HOTC, MarketId::PBGR, MarketId::XADE, MarketId::XATH, MarketId::XIPO };
static const short d_ATL[10] = { 9, MarketId::BKKT, MarketId::IEPA, MarketId::IMAG, MarketId::IMBD, MarketId::IMCR, MarketId::IMEN, MarketId::IMFX, MarketId::IMIR, MarketId::UBEC };
static const short d_ATQ[2] = { 1, MarketId::XXXX };
static const short d_AUA[2] = { 1, MarketId::ABW0 };
static const short d_AUG[2] = { 1, MarketId::XXXX };
static const short d_AUH[13] = { 12, MarketId::ABXX, MarketId::ACXC, MarketId::ACXL, MarketId::ARE0, MarketId::DEXE, MarketId::GLOM, MarketId::IFAD, MarketId::KRME, MarketId::MATX, MarketId::MIDC, MarketId::VMEX, MarketId::XADS };
static const short d_AUS[2] = { 1, MarketId::XXXX };
static const short d_AXA[2] = { 1, MarketId::AIA0 };
static const short d_AY0[2] = { 1, MarketId::SHAR };
static const short d_BAH[4] = { 3, MarketId::BFEX, MarketId::BHR0, MarketId::XBAH };
static const short d_BBR[2] = { 1, MarketId::GLP0 };
static const short d_BBU[14] = { 13, MarketId::BRDE, MarketId::BRDL, MarketId::BRDS, MarketId::BRMF, MarketId::EMCE, MarketId::OTPR, MarketId::ROU0, MarketId::XBRM, MarketId::XBSD, MarketId::XBSE, MarketId::XCAN, MarketId::XRAS, MarketId::XRPM };
static const short d_BCN[7] = { 6, MarketId::CABK, MarketId::PAVE, MarketId::SBAR, MarketId::XBAR, MarketId::XBAV, MarketId::XMEF };
static const short d_BCT[2] = { 1, MarketId::LAMP };
static const short d_BCV[2] = { 1, MarketId::BLZ0 };
static const short d_BDL[2] = { 1, MarketId::XXXX };
static const short d_BEG[3] = { 2, MarketId::SRB0, MarketId::XBEL };
static const short d_BER[16] = { 15, MarketId::BERA, MarketId::BERB, MarketId::BERC, MarketId::DEU0, MarketId::EQTA, MarketId::EQTB, MarketId::EQTC, MarketId::EQTD, MarketId::TGAT, MarketId::TGSI, MarketId::XBER, MarketId::XEQT, MarketId::XGAT, MarketId::XGRM, MarketId::ZOBX };
static const short d_BEY[3] = { 2, MarketId::LBN0, MarketId::XBEY };
static const short d_BFN[2] = { 1, MarketId::XXXX };
static const short d_BFS[2] = { 1, MarketId::XXXX };
static const short d_BGF[2] = { 1, MarketId::CAF0 };
static const short d_BGI[5] = { 4, MarketId::BAJM, MarketId::BRB0, MarketId::XBAB, MarketId::XBIS };
static const short d_BGO[3] = { 2, MarketId::FISH, MarketId::ICAS };
static const short d_BGW[3] = { 2, MarketId::IRQ0, MarketId::XIQS };
static const short d_BGY[2] = { 1, MarketId::UBIM };
static const short d_BHX[2] = { 1, MarketId::XXXX };
static const short d_BIO[4] = { 3, MarketId::IBGH, MarketId::SBIL, MarketId::XBIL };
static const short d_BIS[2] = { 1, MarketId::XXXX };
static const short d_BJL[2] = { 1, MarketId::GMB0 };
static const short d_BJM[2] = { 1, MarketId::BDI0 };
static const short d_BJS[5] = { 4, MarketId::BJSE, MarketId::CHN0, MarketId::GSXC, MarketId::NEEQ };
static const short d_BKK[8] = { 7, MarketId::AFET, MarketId::BEEX, MarketId::TFEX, MarketId::THA0, MarketId::XBKF, MarketId::XBKK, MarketId::XMAI };
static const short d_BKO[2] = { 1, MarketId::MLI0 };
static const short d_BLQ[3] = { 2, MarketId::CREM, MarketId::UBIS };
static const short d_BLR[2] = { 1, MarketId::XBAN };
static const short d_BLZ[2] = { 1, MarketId::XMSW };
static const short d_BMA[62] = { 61, MarketId::CASI, MarketId::CRYD, MarketId::CRYX, MarketId::CSTO, MarketId::DKED, MarketId::DKFI, MarketId::DKOB, MarketId::DKWB, MarketId::DNSE, MarketId::DOSE, MarketId::DSTO, MarketId::EBON, MarketId::ENSX, MarketId::ESTO, MarketId::EUOB, MarketId::EUWB, MarketId::FIED, MarketId::FNSE, MarketId::GBOB, MarketId::GBWB, MarketId::GFKS, MarketId::MNSE, MarketId::MOSE, MarketId::MSTO, MarketId::NAPA, MarketId::NASN, MarketId::NMTF, MarketId::NOCO, MarketId::NOED, MarketId::NOFI, MarketId::NOOB, MarketId::NOWB, MarketId::NSME, MarketId::NSPO, MarketId::ONSE, MarketId::PEPH, MarketId::PEPM, MarketId::PEPQ, MarketId::PEPW, MarketId::PEPY, MarketId::PNED, MarketId::SEBS, MarketId::SEBX, MarketId::SEED, MarketId::SEOB, MarketId::SEWB, MarketId::SSME, MarketId::SVES, MarketId::SVEX, MarketId::SWBI, MarketId::SWE0, MarketId::USOB, MarketId::USWB, MarketId::XABC, MarketId::XNDX, MarketId::XNGM, MarketId::XNMR, MarketId::XOME, MarketId::XOPV, MarketId::XSAT, MarketId::XSTO };
static const short d_BNA[2] = { 1, MarketId::XXXX };
static const short d_BNJ[2] = { 1, MarketId::XXXX };
static const short d_BNX[3] = { 2, MarketId::BLBF, MarketId::XBLB };
static const short d_BOG[3] = { 2, MarketId::COL0, MarketId::XBOG };
static const short d_BOI[2] = { 1, MarketId::XXXX };
static const short d_BOM[18] = { 17, MarketId::ACEX, MarketId::ASTR, MarketId::BSME, MarketId::CDSL, MarketId::FXCL, MarketId::FXSW, MarketId::ICXL, MarketId::ISEX, MarketId::MCXX, MarketId::OTCX, MarketId::PXIL, MarketId::RITS, MarketId::XBOM, MarketId::XIMC, MarketId::XNCD, MarketId::XNSE, MarketId::XUSE };
static const short d_BON[2] = { 1, MarketId::BES0 };
static const short d_BOS[15] = { 14, MarketId::BSTX, MarketId::EBXV, MarketId::GOVX, MarketId::HPPO, MarketId::LEVL, MarketId::LMNX, MarketId::LTAA, MarketId::NFSA, MarketId::NFSC, MarketId::NFSD, MarketId::SIGX, MarketId::SSTX, MarketId::XBOX, MarketId::XSTM };
static const short d_BRE[2] = { 1, MarketId::XBRE };
static const short d_BRN[6] = { 5, MarketId::AIXE, MarketId::CHE0, MarketId::EQWB, MarketId::OTXB, MarketId::XBRN };
static const short d_BRS[2] = { 1, MarketId::XXXX };
static const short d_BRU[24] = { 23, MarketId::ALXB, MarketId::BEAM, MarketId::BEL0, MarketId::BELB, MarketId::BELF, MarketId::BKBF, MarketId::BKBR, MarketId::BLPX, MarketId::BMTS, MarketId::BNPF, MarketId::ENXB, MarketId::FRRF, MarketId::KBCB, MarketId::MLXB, MarketId::MTSD, MarketId::MTSF, MarketId::SMBB, MarketId::TNLB, MarketId::TNLK, MarketId::VPXB, MarketId::XBFO, MarketId::XBRD, MarketId::XBRU };
static const short d_BSB[2] = { 1, MarketId::BRA0 };
static const short d_BSL[2] = { 1, MarketId::XXXX };
static const short d_BTR[2] = { 1, MarketId::XXXX };
static const short d_BTS[7] = { 6, MarketId::CBSK, MarketId::EBRA, MarketId::SVK0, MarketId::VUBA, MarketId::XBRA, MarketId::XRMS };
static const short d_BTV[2] = { 1, MarketId::XXXX };
static const short d_BUD[22] = { 21, MarketId::BETA, MarketId::CIBH, MarketId::CONC, MarketId::EBHU, MarketId::ERST, MarketId::HUDX, MarketId::HUN0, MarketId::HUPX, MarketId::KCCP, MarketId::KELR, MarketId::KHHU, MarketId::OTPB, MarketId::QMTF, MarketId::RBHU, MarketId::UCHU, MarketId::XBCE, MarketId::XBND, MarketId::XBUD, MarketId::XGAS, MarketId::XQLX, MarketId::XTND };
static const short d_BWN[2] = { 1, MarketId::BRN0 };
static const short d_BZV[2] = { 1, MarketId::COG0 };
static const short d_CAE[2] = { 1, MarketId::XXXX };
static const short d_CAI[4] = { 3, MarketId::EGY0, MarketId::NILX, MarketId::XCAI };
static const short d_CAY[2] = { 1, MarketId::GUF0 };
static const short d_CBG[2] = { 1, MarketId::XXXX };
static const short d_CBR[2] = { 1, MarketId::AUS0 };
static const short d_CCK[2] = { 1, MarketId::CCK0 };
static const short d_CCS[4] = { 3, MarketId::BVCA, MarketId::VEN0, MarketId::XCAR };
static const short d_CCU[2] = { 1, MarketId::XCAL };
static const short d_CGK[7] = { 6, MarketId::ICDX, MarketId::IDN0, MarketId::XBBJ, MarketId::XIDX, MarketId::XJKT, MarketId::XJNB };
static const short d_CGO[2] = { 1, MarketId::XZCE };
static const short d_CGP[2] = { 1, MarketId::XCHG };
static const short d_CHC[2] = { 1, MarketId::XXXX };
static const short d_CHI[65] = { 64, MarketId::BATO, MarketId::BATS, MarketId::BATY, MarketId::BTNL, MarketId::BYXD, MarketId::BZXD, MarketId::C2OX, MarketId::CAST, MarketId::CBSX, MarketId::CBTS, MarketId::CCFE, MarketId::CMES, MarketId::CODA, MarketId::COHR, MarketId::CONE, MarketId::CTWO, MarketId::DASH, MarketId::EDDP, MarketId::EDGA, MarketId::EDGD, MarketId::EDGO, MarketId::EDGX, MarketId::ERIS, MarketId::FCBT, MarketId::FCME, MarketId::FREX, MarketId::G1XX, MarketId::GLBX, MarketId::GLPS, MarketId::GOTC, MarketId::GREE, MarketId::HEGX, MarketId::IMCS, MarketId::JLEQ, MarketId::JLQD, MarketId::OPRA, MarketId::PDQD, MarketId::PDQX, MarketId::SCXA, MarketId::SCXF, MarketId::SCXM, MarketId::SCXO, MarketId::SCXS, MarketId::SMFE, MarketId::SUNT, MarketId::THRD, MarketId::XARC, MarketId::XCBD, MarketId::XCBF, MarketId::XCBO, MarketId::XCBT, MarketId::XCCX, MarketId::XCHI, MarketId::XCME, MarketId::XCRC, MarketId::XEUS, MarketId::XIMM, MarketId::XIOM, MarketId::XMAC, MarketId::XMER, MarketId::XMID, MarketId::XOCH, MarketId::YKNA, MarketId::ZERO };
static const short d_CHS[3] = { 2, MarketId::ATDF, MarketId::CORE };
static const short d_CKY[2] = { 1, MarketId::GIN0 };
static const short d_CLJ[2] = { 1, MarketId::BTRL };
static const short d_CLT[3] = { 2, MarketId::CMSF, MarketId::WELX };
static const short d_CMB[3] = { 2, MarketId::LKA0, MarketId::XCOL };
static const short d_CMH[2] = { 1, MarketId::XXXX };
static const short d_CMN[2] = { 1, MarketId::XCAS };
static const short d_CNQ[2] = { 1, MarketId::XCNF };
static const short d_CON[2] = { 1, MarketId::XXXX };
static const short d_COO[2] = { 1, MarketId::BEN0 };
static const short d_COR[3] = { 2, MarketId::MVCX, MarketId::XBCC };
static const short d_CPH[18] = { 17, MarketId::ABSI, MarketId::ALSI, MarketId::DASI, MarketId::DCSE, MarketId::DNDK, MarketId::DNK0, MarketId::DSME, MarketId::FNDK, MarketId::LASP, MarketId::MCSE, MarketId::MNDK, MarketId::NPGA, MarketId::NYSI, MarketId::SXSI, MarketId::XCSE, MarketId::XFND, MarketId::XTRA };
static const short d_CPT[2] = { 1, MarketId::XXXX };
static const short d_CRW[2] = { 1, MarketId::XXXX };
static const short d_CSN[2] = { 1, MarketId::XXXX };
static const short d_CUR[3] = { 2, MarketId::CUW0, MarketId::DCSX };
static const short d_CWB[2] = { 1, MarketId::XBVP };
static const short d_CWL[2] = { 1, MarketId::XXXX };
static const short d_CYS[2] = { 1, MarketId::XXXX };
static const short d_DAC[3] = { 2, MarketId::BGD0, MarketId::XDHA };
static const short d_DAM[3] = { 2, MarketId::SYR0, MarketId::XDSE };
static const short d_DAR[3] = { 2, MarketId::TZA0, MarketId::XDAR };
static const short d_DCA[10] = { 9, MarketId::FINO, MarketId::FINR, MarketId::FINY, MarketId::NBXO, MarketId::OOTC, MarketId::USA0, MarketId::XADF, MarketId::XOTC, MarketId::XPOR };
static const short d_DCF[2] = { 1, MarketId::DMA0 };
static const short d_DEL[3] = { 2, MarketId::IND0, MarketId::XDES };
static const short d_DEN[2] = { 1, MarketId::XXXX };
static const short d_DFW[2] = { 1, MarketId::XXXX };
static const short d_DIL[2] = { 1, MarketId::TLS0 };
static const short d_DKR[2] = { 1, MarketId::SEN0 };
static const short d_DLA[2] = { 1, MarketId::XDSX };
static const short d_DLC[2] = { 1, MarketId::XDCE };
static const short d_DND[2] = { 1, MarketId::XXXX };
static const short d_DNK[3] = { 2, MarketId::SEPE, MarketId::XPRI };
static const short d_DOH[3] = { 2, MarketId::DSMD, MarketId::QAT0 };
static const short d_DOV[2] = { 1, MarketId::XXXX };
static const short d_DSM[2] = { 1, MarketId::XXXX };
static const short d_DUB[59] = { 58, MarketId::AILT, MarketId::AREX, MarketId::BBIE, MarketId::BBIS, MarketId::BKDM, MarketId::BMLI, MarketId::BMLS, MarketId::BMLX, MarketId::BMSI, MarketId::CEPL, MarketId::CFIL, MarketId::CSGI, MarketId::DAVY, MarketId::EDBT, MarketId::EDGL, MarketId::EQIE, MarketId::EQSE, MarketId::EUCC, MarketId::FXFM, MarketId::FXRQ, MarketId::FXRS, MarketId::GBSI, MarketId::HREU, MarketId::IBSI, MarketId::ICUR, MarketId::ICXR, MarketId::IFXA, MarketId::IFXC, MarketId::IFXR, MarketId::IRL0, MarketId::ITGL, MarketId::LEUE, MarketId::LEUF, MarketId::MAQE, MarketId::MCID, MarketId::MSEL, MarketId::RMTF, MarketId::SEMX, MarketId::SIDX, MarketId::SISI, MarketId::TDGF, MarketId::TMEU, MarketId::VFIL, MarketId::VFSI, MarketId::VFXO, MarketId::XASM, MarketId::XATL, MarketId::XCDE, MarketId::XDUB, MarketId::XEBI, MarketId::XESM, MarketId::XEYE, MarketId::XFNX, MarketId::XIEX, MarketId::XMSM, MarketId::XPAC, MarketId::XPOS, MarketId::XRFQ };
static const short d_DXB[7] = { 6, MarketId::DGCX, MarketId::DIFX, MarketId::DUMX, MarketId::GMGD, MarketId::RFIM, MarketId::XDFM };
static const short d_DYU[2] = { 1, MarketId::TJK0 };
static const short d_DZA[2] = { 1, MarketId::MYT0 };
static const short d_EA0[3] = { 2, MarketId::RBCB, MarketId::RBSI };
static const short d_EB0[4] = { 3, MarketId::AFEX, MarketId::GBOT, MarketId::XAFX };
static const short d_EDI[4] = { 3, MarketId::BOFS, MarketId::NWMS, MarketId::SCOT };
static const short d_EIS[2] = { 1, MarketId::VGB0 };
static const short d_EP0[2] = { 1, MarketId::XXXX };
static const short d_ES0[2] = { 1, MarketId::FGEX };
static const short d_ESB[4] = { 3, MarketId::EWRM, MarketId::TMEX, MarketId::TUR0 };
static const short d_ESH[2] = { 1, MarketId::MAXD };
static const short d_ESR[2] = { 1, MarketId::XXXX };
static const short d_EUN[2] = { 1, MarketId::ESH0 };
static const short d_EVN[3] = { 2, MarketId::ARM0, MarketId::XARM };
static const short d_EWR[27] = { 26, MarketId::AATS, MarketId::BTEC, MarketId::DWFI, MarketId::EDXM, MarketId::GLPX, MarketId::GTXS, MarketId::ICSU, MarketId::KNCM, MarketId::KNEM, MarketId::KNIG, MarketId::KNLI, MarketId::KNMX, MarketId::MAGM, MarketId::MEMD, MarketId::MEMM, MarketId::MEMX, MarketId::MXOP, MarketId::OCEA, MarketId::SPTX, MarketId::TERA, MarketId::THEM, MarketId::TPSE, MarketId::TPSV, MarketId::TRFX, MarketId::TRWB, MarketId::XBTF };
static const short d_FAE[3] = { 2, MarketId::FRO0, MarketId::VMFX };
static const short d_FC0[2] = { 1, MarketId::XXXX };
static const short d_FDF[2] = { 1, MarketId::MTQ0 };
static const short d_FFT[2] = { 1, MarketId::XXXX };
static const short d_FIH[2] = { 1, MarketId::COD0 };
static const short d_FLR[4] = { 3, MarketId::DDTX, MarketId::IBEQ, MarketId::IBIS };
static const short d_FNA[2] = { 1, MarketId::SLE0 };
static const short d_FNJ[2] = { 1, MarketId::PRK0 };
static const short d_FOE[2] = { 1, MarketId::XXXX };
static const short d_FRA[104] = { 103, MarketId::A360T, MarketId::AUTO, MarketId::BINV, MarketId::BOCF, MarketId::CATS, MarketId::CBKA, MarketId::CBKC, MarketId::CBKD, MarketId::CBKE, MarketId::CBKF, MarketId::CBKG, MarketId::CBKS, MarketId::CDSI, MarketId::CGEC, MarketId::CGEE, MarketId::CGET, MarketId::CGMD, MarketId::CSDA, MarketId::DAPA, MarketId::DBAG, MarketId::DBDX, MarketId::DBES, MarketId::DBLN, MarketId::DBMO, MarketId::DBOX, MarketId::DEKA, MarketId::DZBK, MarketId::EBLX, MarketId::ECAG, MarketId::ECGS, MarketId::ENTW, MarketId::ERFQ, MarketId::FICX, MarketId::FRAA, MarketId::FRAB, MarketId::FRAD, MarketId::FRAS, MarketId::FRAU, MarketId::FRAV, MarketId::FRAW, MarketId::GSBE, MarketId::GSEI, MarketId::HELA, MarketId::JEFE, MarketId::JESI, MarketId::JPEU, MarketId::LBCW, MarketId::MESI, MarketId::MHEU, MarketId::MSEU, MarketId::NCME, MarketId::NESI, MarketId::RBCG, MarketId::SCAG, MarketId::SEBA, MarketId::SMFF, MarketId::SPEX, MarketId::TPDE, MarketId::TSFF, MarketId::TSFG, MarketId::UBSD, MarketId::UBSI, MarketId::UBSL, MarketId::VONT, MarketId::VTLS, MarketId::VTPS, MarketId::VWDA, MarketId::VWDX, MarketId::XDBC, MarketId::XDBV, MarketId::XDBX, MarketId::XDTB, MarketId::XDWZ, MarketId::XECB, MarketId::XEHQ, MarketId::XERE, MarketId::XERT, MarketId::XETA, MarketId::XETB, MarketId::XETC, MarketId::XETD, MarketId::XETE, MarketId::XETI, MarketId::XETR, MarketId::XETS, MarketId::XETU, MarketId::XETV, MarketId::XETW, MarketId::XETX, MarketId::XEUB, MarketId::XEUM, MarketId::XEUP, MarketId::XEUR, MarketId::XFRA, MarketId::XIGG, MarketId::XINV, MarketId::XNEW, MarketId::XRTR, MarketId::XSC1, MarketId::XSC2, MarketId::XSC3, MarketId::XSCO, MarketId::XXSC };
static const short d_FRU[3] = { 2, MarketId::KGZ0, MarketId::XKSE };
static const short d_FSP[2] = { 1, MarketId::SPM0 };
static const short d_FUK[3] = { 2, MarketId::XFFE, MarketId::XFKA };
static const short d_FUN[2] = { 1, MarketId::TUV0 };
static const short d_GBE[5] = { 4, MarketId::BOTE, MarketId::BOTV, MarketId::BWA0, MarketId::XBOT };
static const short d_GC0[2] = { 1, MarketId::IINX };
static const short d_GCI[3] = { 2, MarketId::GGY0, MarketId::XCIE };
static const short d_GCM[3] = { 2, MarketId::CYM0, MarketId::XCAY };
static const short d_GDT[2] = { 1, MarketId::TCA0 };
static const short d_GEO[3] = { 2, MarketId::GSCI, MarketId::GUY0 };
static const short d_GH0[2] = { 1, MarketId::INSE };
static const short d_GIB[3] = { 2, MarketId::GIB0, MarketId::GSXL };
static const short d_GLA[2] = { 1, MarketId::XXXX };
static const short d_GND[2] = { 1, MarketId::GRD0 };
static const short d_GOA[2] = { 1, MarketId::BPAS };
static const short d_GOH[2] = { 1, MarketId::GRL0 };
static const short d_GOI[2] = { 1, MarketId::XXXX };
static const short d_GOJ[2] = { 1, MarketId::NNCS };
static const short d_GOT[2] = { 1, MarketId::XXXX };
static const short d_GR0[2] = { 1, MarketId::JYSI };
static const short d_GRU[6] = { 5, MarketId::BRIX, MarketId::BVMF, MarketId::XBMF, MarketId::XBSP, MarketId::XSOM };
static const short d_GRX[2] = { 1, MarketId::XSRM };
static const short d_GRZ[2] = { 1, MarketId::XXXX };
static const short d_GS0[2] = { 1, MarketId::SGS0 };
static const short d_GUA[3] = { 2, MarketId::GTM0, MarketId::XGTG };
static const short d_GUM[2] = { 1, MarketId::GUM0 };
static const short d_GVA[2] = { 1, MarketId::CCMS };
static const short d_GW0[8] = { 7, MarketId::IATS, MarketId::IBKR, MarketId::IEOS, MarketId::PIPR, MarketId::PJCX, MarketId::WEED, MarketId::XWEE };
static const short d_GYD[4] = { 3, MarketId::AZE0, MarketId::BSEX, MarketId::XIBE };
static const short d_GYE[2] = { 1, MarketId::XGUA };
static const short d_HAH[2] = { 1, MarketId::COM0 };
static const short d_HAJ[6] = { 5, MarketId::HANA, MarketId::HANB, MarketId::NLBX, MarketId::XHAN, MarketId::XHCE };
static const short d_HAM[12] = { 11, MarketId::BGFI, MarketId::BGFX, MarketId::BGSI, MarketId::HAMA, MarketId::HAMB, MarketId::HAML, MarketId::HAMM, MarketId::HAMN, MarketId::NORD, MarketId::SSWM, MarketId::XHAM };
static const short d_HAN[5] = { 4, MarketId::HSTC, MarketId::VNM0, MarketId::XHNF, MarketId::XHNX };
static const short d_HAV[2] = { 1, MarketId::CUB0 };
static const short d_HEL[13] = { 12, MarketId::AXSI, MarketId::DHEL, MarketId::DNFI, MarketId::FIN0, MarketId::FNFI, MarketId::FSME, MarketId::MHEL, MarketId::MNFI, MarketId::OPCO, MarketId::XFOM, MarketId::XHEL, MarketId::XNOR };
static const short d_HG0[3] = { 2, MarketId::NIBC, MarketId::NPEX };
static const short d_HIJ[2] = { 1, MarketId::XHIR };
static const short d_HIR[2] = { 1, MarketId::SLB0 };
static const short d_HKG[38] = { 37, MarketId::BACR, MarketId::BAEP, MarketId::BAIP, MarketId::BASE, MarketId::BASP, MarketId::CFHK, MarketId::CGMH, MarketId::CLHK, MarketId::CSHK, MarketId::DBHK, MarketId::EOTC, MarketId::GSAL, MarketId::GSPL, MarketId::GSXH, MarketId::GSXM, MarketId::GSXN, MarketId::HKG0, MarketId::HKME, MarketId::HSXA, MarketId::JPMI, MarketId::MAQH, MarketId::MEHK, MarketId::OSDS, MarketId::SHSC, MarketId::SIGH, MarketId::SZSC, MarketId::TFSD, MarketId::TOCP, MarketId::TRAS, MarketId::TWHK, MarketId::UBSX, MarketId::XCGS, MarketId::XGEM, MarketId::XHKF, MarketId::XHKG, MarketId::XIHK, MarketId::XPST };
static const short d_HLN[2] = { 1, MarketId::XXXX };
static const short d_HNL[2] = { 1, MarketId::XXXX };
static const short d_HRE[3] = { 2, MarketId::XZIM, MarketId::ZWE0 };
static const short d_HRK[2] = { 1, MarketId::XKHR };
static const short d_HS0[6] = { 5, MarketId::DAMP, MarketId::DKTC, MarketId::GXGF, MarketId::GXGM, MarketId::GXGR };
static const short d_IAH[2] = { 1, MarketId::ASEF };
static const short d_IDR[2] = { 1, MarketId::NBOT };
static const short d_IEV[13] = { 12, MarketId::EESE, MarketId::PFTQ, MarketId::PFTS, MarketId::UICE, MarketId::UKEX, MarketId::UKR0, MarketId::XDFB, MarketId::XKIE, MarketId::XKIS, MarketId::XNDU, MarketId::XUAX, MarketId::XUKR };
static const short d_ILG[3] = { 2, MarketId::TRAI, MarketId::TSBX };
static const short d_ILZ[2] = { 1, MarketId::SPXE };
static const short d_IND[2] = { 1, MarketId::XXXX };
static const short d_INU[2] = { 1, MarketId::NRU0 };
static const short d_IOM[2] = { 1, MarketId::IMN0 };
static const short d_ISB[3] = { 2, MarketId::PAK0, MarketId::XISL };
static const short d_IST[10] = { 9, MarketId::EXTR, MarketId::XDSM, MarketId::XEDA, MarketId::XEID, MarketId::XEQY, MarketId::XFNO, MarketId::XIAB, MarketId::XIST, MarketId::XPMS };
static const short d_IUE[2] = { 1, MarketId::NIU0 };
static const short d_JAN[2] = { 1, MarketId::XXXX };
static const short d_JED[2] = { 1, MarketId::XXXX };
static const short d_JEF[2] = { 1, MarketId::XXXX };
static const short d_JER[2] = { 1, MarketId::JEY0 };
static const short d_JIB[2] = { 1, MarketId::DJI0 };
static const short d_JNB[16] = { 15, MarketId::A2XX, MarketId::A4AXE, MarketId::ALTX, MarketId::EESX, MarketId::JSEB, MarketId::JSER, MarketId::RMMS, MarketId::RMMX, MarketId::XBES, MarketId::XJSE, MarketId::XSAF, MarketId::XSFA, MarketId::YLDX, MarketId::ZARX, MarketId::ZFXM };
static const short d_JNU[2] = { 1, MarketId::XXXX };
static const short d_JSR[2] = { 1, MarketId::XXXX };
static const short d_JUB[2] = { 1, MarketId::SSD0 };
static const short d_KBL[2] = { 1, MarketId::AFG0 };
static const short d_KGL[4] = { 3, MarketId::ROTC, MarketId::RSEX, MarketId::RWA0 };
static const short d_KHI[3] = { 2, MarketId::NCEL, MarketId::XKAR };
static const short d_KIJ[2] = { 1, MarketId::XNII };
static const short d_KIN[3] = { 2, MarketId::JAM0, MarketId::XJAM };
static const short d_KIV[3] = { 2, MarketId::MDA0, MarketId::XMOL };
static const short d_KLA[4] = { 3, MarketId::UGA0, MarketId::ULTX, MarketId::XUGA };
static const short d_KLU[2] = { 1, MarketId::BKSK };
static const short d_KRK[2] = { 1, MarketId::NSSA };
static const short d_KRT[3] = { 2, MarketId::SDN0, MarketId::XKHA };
static const short d_KTM[3] = { 2, MarketId::NPL0, MarketId::XNEP };
static const short d_KUF[2] = { 1, MarketId::XSAM };
static const short d_KUL[6] = { 5, MarketId::MESQ, MarketId::MYS0, MarketId::XKLS, MarketId::XLOF, MarketId::XRBM };
static const short d_KWI[3] = { 2, MarketId::KWT0, MarketId::XKUW };
static const short d_LAD[3] = { 2, MarketId::AGO0, MarketId::XBDV };
static const short d_LAN[2] = { 1, MarketId::XXXX };
static const short d_LAX[3] = { 2, MarketId::AMLG, MarketId::SFOX };
static const short d_LBA[2] = { 1, MarketId::UGEN };
static const short d_LBU[2] = { 1, MarketId::XLFX };
static const short d_LBV[2] = { 1, MarketId::GAB0 };
static const short d_LCA[2] = { 1, MarketId::CFIF };
static const short d_LCG[2] = { 1, MarketId::XXXX };
static const short d_LDY[2] = { 1, MarketId::XXXX };
static const short d_LED[5] = { 4, MarketId::IXSP, MarketId::SPIM, MarketId::XPET, MarketId::XPIC };
static const short d_LEJ[7] = { 6, MarketId::XECC, MarketId::XEEE, MarketId::XEEO, MarketId::XEER, MarketId::XPOT, MarketId::XPSF };
static const short d_LFW[2] = { 1, MarketId::TGO0 };
static const short d_LHE[2] = { 1, MarketId::XLAH };
static const short d_LI0[7] = { 6, MarketId::ARTX, MarketId::LIE0, MarketId::XLGT, MarketId::XLLB, MarketId::XNOM, MarketId::XVPB };
static const short d_LIM[3] = { 2, MarketId::PER0, MarketId::XLIM };
static const short d_LIS[13] = { 12, MarketId::ALXL, MarketId::BFPT, MarketId::ENXL, MarketId::MDIP, MarketId::MFOX, MarketId::OMIC, MarketId::OMIP, MarketId::OPEX, MarketId::PMTS, MarketId::PRT0, MarketId::WQXL, MarketId::XLIS };
static const short d_LIT[2] = { 1, MarketId::XXXX };
static const short d_LJU[7] = { 6, MarketId::SKBB, MarketId::SVN0, MarketId::XLJM, MarketId::XLJS, MarketId::XLJU, MarketId::XSOP };
static const short d_LLW[2] = { 1, MarketId::MWI0 };
static const short d_LM0[5] = { 4, MarketId::BCSC, MarketId::GPBC, MarketId::MKAP, MarketId::TMCY };
static const short d_LNK[2] = { 1, MarketId::XXXX };
static const short d_LNZ[3] = { 2, MarketId::OBKL, MarketId::RLBO };
static const short d_LON[567] = { 566, MarketId::A3579, MarketId::AFDL, MarketId::AFTS, MarketId::AIMX, MarketId::ALGO, MarketId::AMPX, MarketId::AMTS, MarketId::ANLP, MarketId::ANTS, MarketId::ANZL, MarketId::AQSD, MarketId::AQSE, MarketId::AQSF, MarketId::AQSG, MarketId::AQSL, MarketId::AQSN, MarketId::AQST, MarketId::AQXA, MarketId::AQXD, MarketId::AQXE, MarketId::ARAX, MarketId::ARCH, MarketId::ARDA, MarketId::ARIA, MarketId::ATLB, MarketId::AUTB, MarketId::AUTP, MarketId::AUTX, MarketId::BAIK, MarketId::BALT, MarketId::BANA, MarketId::BAPA, MarketId::BARK, MarketId::BARO, MarketId::BART, MarketId::BASI, MarketId::BATD, MarketId::BATE, MarketId::BATF, MarketId::BATP, MarketId::BBSI, MarketId::BBSX, MarketId::BBVX, MarketId::BCRM, MarketId::BCSI, MarketId::BCSL, MarketId::BCXE, MarketId::BETX, MarketId::BGCB, MarketId::BGCI, MarketId::BGCM, MarketId::BGCO, MarketId::BGFU, MarketId::BGUK, MarketId::BISI, MarketId::BKLF, MarketId::BKLN, MarketId::BLNK, MarketId::BLOX, MarketId::BLTX, MarketId::BMCM, MarketId::BMLB, MarketId::BMTF, MarketId::BNPL, MarketId::BNTW, MarketId::BOAL, MarketId::BOAT, MarketId::BOSC, MarketId::BOTC, MarketId::BPLC, MarketId::BRFQ, MarketId::BRGA, MarketId::BRNX, MarketId::BSLB, MarketId::BSPL, MarketId::BTEE, MarketId::BTLX, MarketId::BTQG, MarketId::BUYN, MarketId::BVUK, MarketId::CAZE, MarketId::CBAL, MarketId::CBNL, MarketId::CCEU, MarketId::CCML, MarketId::CCO2, MarketId::CEPU, MarketId::CFIC, MarketId::CGMC, MarketId::CGME, MarketId::CGMG, MarketId::CGML, MarketId::CGMT, MarketId::CGMU, MarketId::CHEV, MarketId::CHID, MarketId::CHIO, MarketId::CHIX, MarketId::CHIY, MarketId::CIBC, MarketId::CIBP, MarketId::CLCH, MarketId::CLVE, MarketId::CMCM, MarketId::CMEC, MarketId::CMED, MarketId::CMEE, MarketId::CMMT, MarketId::CMTS, MarketId::CRDL, MarketId::CRYP, MarketId::CSBX, MarketId::CSCF, MarketId::CSEC, MarketId::CSEU, MarketId::CSFB, MarketId::CSIN, MarketId::CSLB, MarketId::CSSI, MarketId::CXOT, MarketId::CXRT, MarketId::DAIW, MarketId::DAUK, MarketId::DBCR, MarketId::DBCX, MarketId::DBDC, MarketId::DBIX, MarketId::DBSE, MarketId::DBVX, MarketId::DOWG, MarketId::DRSP, MarketId::DVFX, MarketId::EACM, MarketId::EBSM, MarketId::EBSO, MarketId::EBSX, MarketId::ECHO, MarketId::ECNL, MarketId::ECSL, MarketId::EIXE, MarketId::ELIX, MarketId::EMBX, MarketId::EMCH, MarketId::EMTS, MarketId::ENCL, MarketId::ENSY, MarketId::EQLD, MarketId::EQSL, MarketId::EVOL, MarketId::EXBO, MarketId::EXCP, MarketId::EXDC, MarketId::EXEU, MarketId::EXLP, MarketId::EXMP, MarketId::EXOR, MarketId::EXOT, MarketId::EXSI, MarketId::EXVP, MarketId::FAIR, MarketId::FISU, MarketId::FNUK, MarketId::FRTE, MarketId::FXGB, MarketId::FXMT, MarketId::FXOP, MarketId::G360, MarketId::GBR0, MarketId::GEMX, MarketId::GFBM, MarketId::GFBO, MarketId::GFIA, MarketId::GFIB, MarketId::GFIC, MarketId::GFIF, MarketId::GFIM, MarketId::GFIN, MarketId::GFIR, MarketId::GFOX, MarketId::GFSM, MarketId::GFSO, MarketId::GMEG, MarketId::GMGL, MarketId::GMTS, MarketId::GRIF, MarketId::GRIO, MarketId::GRSE, MarketId::GSBX, MarketId::GSIB, MarketId::GSIL, MarketId::GSLO, MarketId::GSSI, MarketId::HPSO, MarketId::HPSX, MarketId::HRSI, MarketId::HSBC, MarketId::HSXE, MarketId::HUNG, MarketId::IBAL, MarketId::ICAH, MarketId::ICAP, MarketId::ICAT, MarketId::ICEN, MarketId::ICEO, MarketId::ICEU, MarketId::ICPM, MarketId::ICSE, MarketId::ICTQ, MarketId::IECE, MarketId::IECL, MarketId::IFEN, MarketId::IFEU, MarketId::IFLL, MarketId::IFLO, MarketId::IFLS, MarketId::IFLX, MarketId::IFUT, MarketId::IGDL, MarketId::ILCM, MarketId::IMCD, MarketId::IMCE, MarketId::IMCM, MarketId::IMED, MarketId::IMET, MarketId::IMFD, MarketId::IMGB, MarketId::IMGI, MarketId::IMMM, MarketId::IMRD, MarketId::IMSB, MarketId::IMTF, MarketId::IMTS, MarketId::INGU, MarketId::INVE, MarketId::IOCD, MarketId::IOED, MarketId::IOFB, MarketId::IOFI, MarketId::IOFX, MarketId::IOGB, MarketId::IOGI, MarketId::IOIR, MarketId::IOMM, MarketId::IOTC, MarketId::IOTF, MarketId::IPSX, MarketId::IPXP, MarketId::IPXW, MarketId::ISDX, MarketId::ISSI, MarketId::ISWA, MarketId::ISWB, MarketId::ISWC, MarketId::ISWE, MarketId::ISWR, MarketId::ISWV, MarketId::IUOB, MarketId::JEFS, MarketId::JISI, MarketId::JLSI, MarketId::JPCB, MarketId::JPSI, MarketId::JSSI, MarketId::KBLM, MarketId::KLEU, MarketId::KMTS, MarketId::LBCM, MarketId::LCUR, MarketId::LELE, MarketId::LIQF, MarketId::LIQH, MarketId::LIQU, MarketId::LISX, MarketId::LMAD, MarketId::LMAE, MarketId::LMAF, MarketId::LMAO, MarketId::LMAX, MarketId::LMEC, MarketId::LMTS, MarketId::LOTC, MarketId::LOUI, MarketId::LOYD, MarketId::LPPM, MarketId::MAEL, MarketId::MAKX, MarketId::MAQI, MarketId::MAQL, MarketId::MAQU, MarketId::MBSI, MarketId::MCUR, MarketId::MCXR, MarketId::MCXS, MarketId::MCZK, MarketId::METZ, MarketId::MFGL, MarketId::MFXA, MarketId::MFXC, MarketId::MFXR, MarketId::MHBL, MarketId::MHIP, MarketId::MLAX, MarketId::MLEU, MarketId::MLIB, MarketId::MLIX, MarketId::MLRQ, MarketId::MLSI, MarketId::MLVE, MarketId::MLXN, MarketId::MSBI, MarketId::MSIP, MarketId::MSSI, MarketId::MTSA, MarketId::MTSG, MarketId::MTSS, MarketId::MTSW, MarketId::MUBL, MarketId::MUFP, MarketId::MUSE, MarketId::MUTI, MarketId::MXLM, MarketId::MYTR, MarketId::N2EX, MarketId::NABA, MarketId::NABE, MarketId::NABL, MarketId::NABU, MarketId::NAVE, MarketId::NBFL, MarketId::NCML, MarketId::NDCM, MarketId::NEXD, MarketId::NEXF, MarketId::NEXG, MarketId::NEXL, MarketId::NEXN, MarketId::NEXS, MarketId::NEXT, MarketId::NEXX, MarketId::NMTS, MarketId::NOFF, MarketId::NOME, MarketId::NOSI, MarketId::NTUK, MarketId::NURD, MarketId::NURO, MarketId::NXEU, MarketId::NYMX, MarketId::OCSI, MarketId::OFEX, MarketId::OILX, MarketId::OTCE, MarketId::OTXT, MarketId::PARX, MarketId::PCDS, MarketId::PEEL, MarketId::PFXD, MarketId::PIEU, MarketId::PIRM, MarketId::PLDX, MarketId::PLSX, MarketId::PORT, MarketId::PPEX, MarketId::PRME, MarketId::PVMF, MarketId::QMTS, MarketId::QWIX, MarketId::R5FX, MarketId::RABL, MarketId::RBCE, MarketId::RBCM, MarketId::RBCT, MarketId::RBSX, MarketId::REST, MarketId::RMTS, MarketId::RTSI, MarketId::RTSL, MarketId::SBEX, MarketId::SECF, MarketId::SEDR, MarketId::SFCL, MarketId::SGMX, MarketId::SGMY, MarketId::SGMZ, MarketId::SICS, MarketId::SIFX, MarketId::SISU, MarketId::SKYX, MarketId::SLKK, MarketId::SLXT, MarketId::SMBE, MarketId::SMBG, MarketId::SMTS, MarketId::SNUK, MarketId::SPDX, MarketId::SPEC, MarketId::SPRZ, MarketId::SQUA, MarketId::SSBT, MarketId::SSEX, MarketId::SSFX, MarketId::SSIL, MarketId::STAL, MarketId::STAN, MarketId::STFL, MarketId::STSI, MarketId::SUNB, MarketId::SUNM, MarketId::SUNO, MarketId::SWAP, MarketId::TBEN, MarketId::TBLA, MarketId::TCDS, MarketId::TCME, MarketId::TCML, MarketId::TDBL, MarketId::TECO, MarketId::TEEG, MarketId::TEFD, MarketId::TEFX, MarketId::TEGB, MarketId::TEGI, MarketId::TEIR, MarketId::TEMB, MarketId::TEMC, MarketId::TEMF, MarketId::TEMG, MarketId::TEMI, MarketId::TEMM, MarketId::TEMR, MarketId::TEOF, MarketId::TERE, MarketId::TFSC, MarketId::TFSE, MarketId::TFSG, MarketId::TFSS, MarketId::TFSV, MarketId::TIRD, MarketId::TMTS, MarketId::TMUK, MarketId::TNLL, MarketId::TPCD, MarketId::TPDA, MarketId::TPEL, MarketId::TPEO, MarketId::TPEQ, MarketId::TPFD, MarketId::TPID, MarketId::TPIE, MarketId::TPIM, MarketId::TPIS, MarketId::TPLF, MarketId::TPMF, MarketId::TPMG, MarketId::TPRE, MarketId::TPSD, MarketId::TPSL, MarketId::TPSO, MarketId::TPSP, MarketId::TPSY, MarketId::TRAL, MarketId::TRAX, MarketId::TRDE, MarketId::TRDX, MarketId::TREA, MarketId::TREO, MarketId::TREU, MarketId::TRFW, MarketId::TRQA, MarketId::TRQB, MarketId::TRQC, MarketId::TRQD, MarketId::TRQM, MarketId::TRQS, MarketId::TRQX, MarketId::TRSI, MarketId::TRUK, MarketId::TSCB, MarketId::TSCD, MarketId::TSED, MarketId::TSFI, MarketId::TSFX, MarketId::TSGB, MarketId::TSGI, MarketId::TSIR, MarketId::TSMB, MarketId::TSMC, MarketId::TSMG, MarketId::TSMI, MarketId::TSMM, MarketId::TSMR, MarketId::TSRE, MarketId::TSUK, MarketId::TUOB, MarketId::TWGP, MarketId::UBIN, MarketId::UBSB, MarketId::UBSE, MarketId::UBSY, MarketId::UKCA, MarketId::UKGD, MarketId::UKOR, MarketId::UKPX, MarketId::UKRE, MarketId::UMTS, MarketId::UNGB, MarketId::USWP, MarketId::VAGL, MarketId::VAGM, MarketId::VCMO, MarketId::VEGA, MarketId::VFGB, MarketId::VFUK, MarketId::VIUK, MarketId::VMTS, MarketId::VOLA, MarketId::VTBC, MarketId::WCLK, MarketId::WELN, MarketId::WFLB, MarketId::WINS, MarketId::WINX, MarketId::WSIL, MarketId::WSIN, MarketId::XALT, MarketId::XCOR, MarketId::XEBS, MarketId::XEDX, MarketId::XGCL, MarketId::XGCX, MarketId::XGDX, MarketId::XGFI, MarketId::XGSX, MarketId::XIEL, MarketId::XIPE, MarketId::XJWY, MarketId::XLBM, MarketId::XLCE, MarketId::XLCH, MarketId::XLDN, MarketId::XLDX, MarketId::XLIF, MarketId::XLME, MarketId::XLOD, MarketId::XLOM, MarketId::XLON, MarketId::XLTO, MarketId::XMLX, MarketId::XMTS, MarketId::XNLX, MarketId::XPAL, MarketId::XPHX, MarketId::XPLU, MarketId::XPOL, MarketId::XPUK, MarketId::XRSP, MarketId::XSGB, MarketId::XSMP, MarketId::XSWB, MarketId::XTFN, MarketId::XTPE, MarketId::XTUP, MarketId::XTXM, MarketId::XUBS, MarketId::XUMP, MarketId::ZODM };
static const short d_LOS[3] = { 2, MarketId::NASX, MarketId::XNSA };
static const short d_LPB[3] = { 2, MarketId::BOL0, MarketId::XBOL };
static const short d_LPL[2] = { 1, MarketId::XXXX };
static const short d_LU0[2] = { 1, MarketId::S3FM };
static const short d_LUN[3] = { 2, MarketId::XLUS, MarketId::ZMB0 };
static const short d_LUX[18] = { 17, MarketId::BCEE, MarketId::BDPL, MarketId::BGLU, MarketId::BILU, MarketId::BLUX, MarketId::CCLX, MarketId::DBLX, MarketId::DHLX, MarketId::EMTF, MarketId::KBLC, MarketId::KBLL, MarketId::KBLS, MarketId::KBLT, MarketId::LUX0, MarketId::MIBL, MarketId::XLUX, MarketId::XVES };
static const short d_LV0[2] = { 1, MarketId::XEAS };
static const short d_LYR[2] = { 1, MarketId::SJM0 };
static const short d_LYS[2] = { 1, MarketId::XXXX };
static const short d_MAA[2] = { 1, MarketId::XMDS };
static const short d_MAD[44] = { 43, MarketId::ABAN, MarketId::AGBP, MarketId::ALLT, MarketId::BBVA, MarketId::BMCL, MarketId::BMEA, MarketId::BMEX, MarketId::CAPI, MarketId::CECA, MarketId::CIMA, MarketId::CIMB, MarketId::CIMD, MarketId::CIME, MarketId::CIMV, MarketId::CMAP, MarketId::CSMD, MarketId::DOWE, MarketId::DOWM, MarketId::ESP0, MarketId::GROW, MarketId::MABX, MarketId::MARF, MarketId::MDRV, MarketId::MERF, MarketId::MIBG, MarketId::OMEL, MarketId::POSE, MarketId::SCLE, MarketId::SEND, MarketId::SIMD, MarketId::TEUR, MarketId::TOMF, MarketId::TOMG, MarketId::TPES, MarketId::XDPA, MarketId::XDRF, MarketId::XLAT, MarketId::XMAD, MarketId::XMCE, MarketId::XMFX, MarketId::XMPW, MarketId::XMRV, MarketId::XNAF };
static const short d_MAJ[2] = { 1, MarketId::MHL0 };
static const short d_MAN[2] = { 1, MarketId::XXXX };
static const short d_MC0[2] = { 1, MarketId::PDEX };
static const short d_MCI[3] = { 2, MarketId::EDGE, MarketId::XKBT };
static const short d_MCM[2] = { 1, MarketId::MCO0 };
static const short d_MCT[5] = { 4, MarketId::MSXB, MarketId::MSXO, MarketId::OMN0, MarketId::XMUS };
static const short d_MDT[2] = { 1, MarketId::XXXX };
static const short d_MDZ[3] = { 2, MarketId::XBCM, MarketId::XBCX };
static const short d_MEL[4] = { 3, MarketId::AWBX, MarketId::BLEV, MarketId::NSXB };
static const short d_MEX[6] = { 5, MarketId::BIVA, MarketId::CGMX, MarketId::MEX0, MarketId::XEMD, MarketId::XMEX };
static const short d_MFM[2] = { 1, MarketId::MAC0 };
static const short d_MGA[3] = { 2, MarketId::NIC0, MarketId::XMAN };
static const short d_MGF[2] = { 1, MarketId::BCMM };
static const short d_MGN[2] = { 1, MarketId::XXXX };
static const short d_MGQ[2] = { 1, MarketId::SOM0 };
static const short d_MHQ[2] = { 1, MarketId::ALA0 };
static const short d_MIA[2] = { 1, MarketId::FUSD };
static const short d_MIL[35] = { 34, MarketId::AKIS, MarketId::ATFX, MarketId::BGEM, MarketId::EMDR, MarketId::EMIB, MarketId::EMID, MarketId::EMIR, MarketId::ETFP, MarketId::ETLX, MarketId::EXGM, MarketId::EXPA, MarketId::FBSI, MarketId::HCER, MarketId::HMOD, MarketId::HMTF, MarketId::HRFQ, MarketId::IMMH, MarketId::ITSM, MarketId::MACX, MarketId::MIVX, MarketId::MOTX, MarketId::MTAA, MarketId::MTAH, MarketId::MTAX, MarketId::MUBM, MarketId::SEDX, MarketId::TLAB, MarketId::UCIT, MarketId::XAIM, MarketId::XDMI, MarketId::XMIF, MarketId::XMIL, MarketId::XMOT, MarketId::XTLX };
static const short d_MKE[2] = { 1, MarketId::XXXX };
static const short d_MLA[7] = { 6, MarketId::COMG, MarketId::EWSM, MarketId::IFSM, MarketId::MLT0, MarketId::PROS, MarketId::XMAL };
static const short d_MLE[3] = { 2, MarketId::MALX, MarketId::MDV0 };
static const short d_MLW[2] = { 1, MarketId::LBR0 };
static const short d_MMX[2] = { 1, MarketId::XXXX };
static const short d_MNI[2] = { 1, MarketId::MSR0 };
static const short d_MNL[3] = { 2, MarketId::CLPH, MarketId::PHL0 };
static const short d_MOW[11] = { 10, MarketId::MISX, MarketId::NAMX, MarketId::RPDX, MarketId::RTSX, MarketId::RUS0, MarketId::RUSX, MarketId::SPBE, MarketId::XMIC, MarketId::XMOS, MarketId::XRUS };
static const short d_MPM[4] = { 3, MarketId::MOZ0, MarketId::XBVM, MarketId::XMAP };
static const short d_MRS[2] = { 1, MarketId::XXXX };
static const short d_MRU[3] = { 2, MarketId::MUS0, MarketId::XMAU };
static const short d_MSN[2] = { 1, MarketId::XXXX };
static const short d_MSP[2] = { 1, MarketId::XMGE };
static const short d_MSQ[3] = { 2, MarketId::BCSE, MarketId::BLR0 };
static const short d_MSU[2] = { 1, MarketId::LSO0 };
static const short d_MUC[19] = { 18, MarketId::BAAD, MarketId::BDEA, MarketId::BLBB, MarketId::BLBS, MarketId::BLEQ, MarketId::BLFX, MarketId::BLIQ, MarketId::BSFX, MarketId::GMEX, MarketId::MUNA, MarketId::MUNB, MarketId::MUNC, MarketId::MUND, MarketId::PLUS, MarketId::SSBI, MarketId::SSBM, MarketId::UCDE, MarketId::XMUN };
static const short d_MV0[2] = { 1, MarketId::XXXX };
static const short d_MVD[5] = { 4, MarketId::BVUR, MarketId::UFEX, MarketId::URY0, MarketId::XMNT };
static const short d_NAP[2] = { 1, MarketId::XXXX };
static const short d_NAS[3] = { 2, MarketId::BHS0, MarketId::XBAA };
static const short d_NBO[3] = { 2, MarketId::KEN0, MarketId::XNAI };
static const short d_NCL[2] = { 1, MarketId::XXXX };
static const short d_NDJ[2] = { 1, MarketId::TCD0 };
static const short d_NGO[6] = { 5, MarketId::XCCE, MarketId::XNGO, MarketId::XNKS, MarketId::XNST, MarketId::XTKA };
static const short d_NIC[10] = { 9, MarketId::ATHL, MarketId::ATLN, MarketId::CYP0, MarketId::RENC, MarketId::SIBC, MarketId::XCYO, MarketId::XCYS, MarketId::XECM, MarketId::XMME };
static const short d_NIM[2] = { 1, MarketId::NER0 };
static const short d_NKC[2] = { 1, MarketId::MRT0 };
static const short d_NKW[2] = { 1, MarketId::IOT0 };
static const short d_NLK[2] = { 1, MarketId::NFK0 };
static const short d_NOU[2] = { 1, MarketId::NCL0 };
static const short d_NTL[3] = { 2, MarketId::NECD, MarketId::XNEC };
static const short d_NYC[262] = { 261, MarketId::ACKF, MarketId::ADVT, MarketId::ALDP, MarketId::AMXO, MarketId::AQUA, MarketId::ARCD, MarketId::ARCO, MarketId::ARCX, MarketId::ARKX, MarketId::BAML, MarketId::BAMP, MarketId::BAMX, MarketId::BARD, MarketId::BARL, MarketId::BARX, MarketId::BBOK, MarketId::BBSF, MarketId::BCDX, MarketId::BGCD, MarketId::BGCF, MarketId::BHSF, MarketId::BIDS, MarketId::BLKX, MarketId::BLTD, MarketId::BLUE, MarketId::BNDD, MarketId::BNDS, MarketId::BNPC, MarketId::BNPH, MarketId::BNYC, MarketId::BOSD, MarketId::BPOL, MarketId::BTEQ, MarketId::BVUS, MarketId::CAES, MarketId::CALH, MarketId::CAVE, MarketId::CBLC, MarketId::CCMX, MarketId::CDED, MarketId::CDEL, MarketId::CECS, MarketId::CFIM, MarketId::CGMI, MarketId::CGXS, MarketId::CICX, MarketId::CIOI, MarketId::CISD, MarketId::CLST, MarketId::CPGX, MarketId::CRED, MarketId::CSCL, MarketId::CSLP, MarketId::CSVW, MarketId::CURX, MarketId::DBAB, MarketId::DBSX, MarketId::DEAL, MarketId::DWSF, MarketId::EGMT, MarketId::EQUS, MarketId::ESPD, MarketId::EXPM, MarketId::FAST, MarketId::FINC, MarketId::FINN, MarketId::FNCS, MarketId::FNFT, MarketId::FNFX, MarketId::FNIX, MarketId::FNXB, MarketId::FSEF, MarketId::FTUS, MarketId::FXAL, MarketId::FXCM, MarketId::FXNM, MarketId::FXPS, MarketId::GFAM, MarketId::GLLC, MarketId::GLMX, MarketId::GMNI, MarketId::GSCO, MarketId::GSEF, MarketId::GTCO, MarketId::GTSX, MarketId::HRTF, MarketId::HRTX, MarketId::HSFX, MarketId::IBLX, MarketId::ICBX, MarketId::ICEL, MarketId::ICES, MarketId::ICRO, MarketId::ICUS, MarketId::IDXM, MarketId::IEXC, MarketId::IEXD, MarketId::IEXG, MarketId::IFED, MarketId::IFUS, MarketId::IIDX, MarketId::IMCC, MarketId::IMCG, MarketId::INCA, MarketId::ISDA, MarketId::ITGI, MarketId::JEFX, MarketId::JNST, MarketId::JPBX, MarketId::JPMS, MarketId::JPMX, MarketId::JSEF, MarketId::JSES, MarketId::JSJX, MarketId::KLSH, MarketId::LAFD, MarketId::LAFL, MarketId::LAFX, MarketId::LASF, MarketId::LATG, MarketId::LAVA, MarketId::LEDG, MarketId::LESI, MarketId::LIFI, MarketId::LIUH, MarketId::LIUS, MarketId::LPSF, MarketId::LQED, MarketId::LQFI, MarketId::MCRY, MarketId::MELO, MarketId::MKAA, MarketId::MLCO, MarketId::MLVX, MarketId::MOCX, MarketId::MSCO, MarketId::MSLC, MarketId::MSLP, MarketId::MSPL, MarketId::MSRP, MarketId::MSTC, MarketId::MSTX, MarketId::MTSB, MarketId::MTUS, MarketId::MTXA, MarketId::MTXC, MarketId::MTXM, MarketId::MTXS, MarketId::MTXX, MarketId::NASD, MarketId::NBLX, MarketId::NLAX, MarketId::NMRA, MarketId::NPMS, MarketId::NXUS, MarketId::NYFX, MarketId::NYMS, MarketId::NYPC, MarketId::NYSD, MarketId::OCTL, MarketId::OLLC, MarketId::ONEC, MarketId::OTCB, MarketId::OTCI, MarketId::OTCM, MarketId::OTCN, MarketId::OTCQ, MarketId::PINC, MarketId::PINI, MarketId::PINL, MarketId::PINX, MarketId::PIPE, MarketId::PRSE, MarketId::PSGM, MarketId::PSXD, MarketId::PULX, MarketId::PUMA, MarketId::PUMX, MarketId::PUND, MarketId::RAJA, MarketId::RCBX, MarketId::RCMA, MarketId::RTXF, MarketId::SGA2, MarketId::SGAS, MarketId::SGMA, MarketId::SGMT, MarketId::SHAD, MarketId::SHAW, MarketId::SOHO, MarketId::SPAX, MarketId::STFU, MarketId::STFX, MarketId::STRM, MarketId::TFSU, MarketId::THRE, MarketId::TMCC, MarketId::TMID, MarketId::TRCK, MarketId::TRCX, MarketId::TRU1, MarketId::TRU2, MarketId::TRUX, MarketId::TSAD, MarketId::TSEF, MarketId::TWSF, MarketId::U360, MarketId::UBSA, MarketId::UBSP, MarketId::UBSS, MarketId::USEF, MarketId::VABD, MarketId::VALX, MarketId::VCRS, MarketId::VERT, MarketId::VFCM, MarketId::VFMI, MarketId::VIRT, MarketId::VNDM, MarketId::VTEX, MarketId::VUSA, MarketId::WABR, MarketId::WELS, MarketId::WSAG, MarketId::XAQS, MarketId::XASE, MarketId::XBMK, MarketId::XBOS, MarketId::XBRT, MarketId::XBXO, MarketId::XCEC, MarketId::XCFF, MarketId::XCIS, MarketId::XCSC, MarketId::XCUR, MarketId::XELX, MarketId::XINS, MarketId::XISA, MarketId::XISE, MarketId::XISX, MarketId::XNAS, MarketId::XNCM, MarketId::XNDQ, MarketId::XNFI, MarketId::XNGS, MarketId::XNIM, MarketId::XNLI, MarketId::XNMS, MarketId::XNQL, MarketId::XNYC, MarketId::XNYE, MarketId::XNYF, MarketId::XNYL, MarketId::XNYM, MarketId::XNYS, MarketId::XPSX, MarketId::XPUS, MarketId::XSEF, MarketId::XTPZ, MarketId::XTRD, MarketId::XTXD };
static const short d_OB0[2] = { 1, MarketId::OLBB };
static const short d_ODS[2] = { 1, MarketId::XODE };
static const short d_OKC[2] = { 1, MarketId::XXXX };
static const short d_OKD[2] = { 1, MarketId::XSAP };
static const short d_OLM[2] = { 1, MarketId::XXXX };
static const short d_OPO[2] = { 1, MarketId::MBCP };
static const short d_ORK[2] = { 1, MarketId::XXXX };
static const short d_OSA[7] = { 6, MarketId::XHER, MarketId::XKAC, MarketId::XOSE, MarketId::XOSJ, MarketId::XOSM, MarketId::XOST };
static const short d_OSL[38] = { 37, MarketId::BULK, MarketId::BURG, MarketId::BURM, MarketId::CNSI, MarketId::ELEU, MarketId::ELNO, MarketId::ELSE, MarketId::ELUK, MarketId::FNDS, MarketId::FREI, MarketId::IFFX, MarketId::INFT, MarketId::MERD, MarketId::MERK, MarketId::NEXO, MarketId::NIBR, MarketId::NOR0, MarketId::NORX, MarketId::NOSC, MarketId::NOTC, MarketId::OAPA, MarketId::OSLC, MarketId::QUNT, MarketId::SB1M, MarketId::STEE, MarketId::XABG, MarketId::XDNB, MarketId::XIMA, MarketId::XOAA, MarketId::XOAD, MarketId::XOAM, MarketId::XOAS, MarketId::XOBD, MarketId::XOSA, MarketId::XOSC, MarketId::XOSD, MarketId::XOSL };
static const short d_OUA[2] = { 1, MarketId::BFA0 };
static const short d_OVB[3] = { 2, MarketId::XSIB, MarketId::XSIC };
static const short d_OXB[2] = { 1, MarketId::GNB0 };
static const short d_PAC[3] = { 2, MarketId::PAN0, MarketId::XPTY };
static const short d_PAP[2] = { 1, MarketId::HTI0 };
static const short d_PAR[98] = { 97, MarketId::AACA, MarketId::ALXP, MarketId::AQEA, MarketId::AQED, MarketId::AQEU, MarketId::AURB, MarketId::AURO, MarketId::BNPA, MarketId::BNPS, MarketId::BPSX, MarketId::BRED, MarketId::CMCI, MarketId::COAL, MarketId::EDRF, MarketId::ELXE, MarketId::EPEX, MarketId::EXSB, MarketId::EXSD, MarketId::EXSE, MarketId::EXSF, MarketId::EXSH, MarketId::EXSP, MarketId::EXSY, MarketId::EXYY, MarketId::FMTS, MarketId::FRA0, MarketId::GFPO, MarketId::GIPB, MarketId::GMEO, MarketId::GMES, MarketId::GMTF, MarketId::GSPX, MarketId::HBFR, MarketId::HPCO, MarketId::HPCS, MarketId::HPCV, MarketId::HPCX, MarketId::ICOR, MarketId::ICOT, MarketId::KOTF, MarketId::LCHC, MarketId::LNEQ, MarketId::LNFI, MarketId::MHBP, MarketId::MKTF, MarketId::MLER, MarketId::MLES, MarketId::MLEX, MarketId::MSAX, MarketId::MSCX, MarketId::MSNT, MarketId::MTCH, MarketId::MUBP, MarketId::NABP, MarketId::NATX, MarketId::NOWX, MarketId::ODDO, MarketId::ODOC, MarketId::RBCC, MarketId::SGMU, MarketId::SGMV, MarketId::SGMW, MarketId::SGOE, MarketId::SMBP, MarketId::TDON, MarketId::TEPF, MarketId::TEPG, MarketId::TEPI, MarketId::TEPM, MarketId::TEPR, MarketId::TEPX, MarketId::TLCM, MarketId::TPEE, MarketId::TPER, MarketId::TPEU, MarketId::TPFR, MarketId::TPIC, MarketId::TPIO, MarketId::TPIR, MarketId::TRXE, MarketId::TSAF, MarketId::VRXP, MarketId::WFSE, MarketId::XAFR, MarketId::XAPA, MarketId::XBLK, MarketId::XBLN, MarketId::XETF, MarketId::XFMN, MarketId::XMAT, MarketId::XMLI, MarketId::XMON, MarketId::XPAR, MarketId::XPOW, MarketId::XSGA, MarketId::XSPM, MarketId::XTXE };
static const short d_PBH[2] = { 1, MarketId::BTN0 };
static const short d_PBM[2] = { 1, MarketId::SUR0 };
static const short d_PC0[2] = { 1, MarketId::XPHS };
static const short d_PCT[8] = { 7, MarketId::EMLD, MarketId::EPRD, MarketId::EPRL, MarketId::MIHI, MarketId::MPRL, MarketId::SPHR, MarketId::XMIO };
static const short d_PHL[6] = { 5, MarketId::RICD, MarketId::RICX, MarketId::XPBT, MarketId::XPHL, MarketId::XPHO };
static const short d_PHX[2] = { 1, MarketId::XAZX };
static const short d_PIR[2] = { 1, MarketId::XXXX };
static const short d_PMI[2] = { 1, MarketId::BCMA };
static const short d_PN0[2] = { 1, MarketId::PCN0 };
static const short d_PNH[3] = { 2, MarketId::KHM0, MarketId::XCSX };
static const short d_PNI[2] = { 1, MarketId::FSM0 };
static const short d_POM[3] = { 2, MarketId::PNG0, MarketId::XPOM };
static const short d_POS[3] = { 2, MarketId::TTO0, MarketId::XTRN };
static const short d_PPG[2] = { 1, MarketId::ASM0 };
static const short d_PPT[2] = { 1, MarketId::PYF0 };
static const short d_PRG[21] = { 20, MarketId::ATAD, MarketId::CELP, MarketId::CESI, MarketId::CSAS, MarketId::CSOB, MarketId::CZE0, MarketId::FTFM, MarketId::FTFS, MarketId::KOME, MarketId::PATF, MarketId::RFBK, MarketId::SPAD, MarketId::STRT, MarketId::UBCZ, MarketId::WOOD, MarketId::XPRA, MarketId::XPRM, MarketId::XPXE, MarketId::XRMO, MarketId::XRMZ };
static const short d_PRN[2] = { 1, MarketId::XXXX };
static const short d_PRY[2] = { 1, MarketId::ZAF0 };
static const short d_PS0[3] = { 2, MarketId::PSE0, MarketId::XPAE };
static const short d_PSY[2] = { 1, MarketId::FLK0 };
static const short d_PVD[2] = { 1, MarketId::XXXX };
static const short d_QDU[12] = { 11, MarketId::DUSA, MarketId::DUSB, MarketId::DUSC, MarketId::DUSD, MarketId::HSBT, MarketId::IKBS, MarketId::LSSI, MarketId::MHBD, MarketId::SMBD, MarketId::XDUS, MarketId::XQTX };
static const short d_QIC[2] = { 1, MarketId::ISBV };
static const short d_QMN[3] = { 2, MarketId::SWZ0, MarketId::XSWA };
static const short d_QND[2] = { 1, MarketId::XXXX };
static const short d_QPA[2] = { 1, MarketId::BEIS };
static const short d_QSA[2] = { 1, MarketId::BSAB };
static const short d_RA0[2] = { 1, MarketId::SKSI };
static const short d_RAI[3] = { 2, MarketId::CPV0, MarketId::XBVC };
static const short d_RAR[2] = { 1, MarketId::COK0 };
static const short d_RB0[2] = { 1, MarketId::LIGA };
static const short d_RBA[2] = { 1, MarketId::MAR0 };
static const short d_RDU[2] = { 1, MarketId::XXXX };
static const short d_RGN[2] = { 1, MarketId::MMR0 };
static const short d_RIC[2] = { 1, MarketId::XXXX };
static const short d_RIO[5] = { 4, MarketId::CETI, MarketId::SELC, MarketId::XBBF, MarketId::XRIO };
static const short d_RIX[5] = { 4, MarketId::FNLV, MarketId::LVA0, MarketId::SWLV, MarketId::XRIS };
static const short d_RIZ[2] = { 1, MarketId::XXXX };
static const short d_RKV[9] = { 8, MarketId::DICE, MarketId::DNIS, MarketId::FNIS, MarketId::ISEC, MarketId::ISL0, MarketId::MICE, MarketId::MNIS, MarketId::XICE };
static const short d_ROM[21] = { 20, MarketId::BNLD, MarketId::BOND, MarketId::CGCM, MarketId::CGDB, MarketId::CGEB, MarketId::CGGD, MarketId::CGIT, MarketId::CGND, MarketId::CGQD, MarketId::CGQT, MarketId::CGTR, MarketId::EBMX, MarketId::ITA0, MarketId::MCAD, MarketId::MSWP, MarketId::MTSC, MarketId::MTSM, MarketId::MTSO, MarketId::SSOB, MarketId::XGME };
static const short d_ROR[2] = { 1, MarketId::PLW0 };
static const short d_ROS[4] = { 3, MarketId::ROFX, MarketId::XROS, MarketId::XROX };
static const short d_ROV[2] = { 1, MarketId::XROV };
static const short d_RTM[2] = { 1, MarketId::XXXX };
static const short d_RUH[3] = { 2, MarketId::SAU0, MarketId::XSAU };
static const short d_RUN[2] = { 1, MarketId::REU0 };
static const short d_SAF[2] = { 1, MarketId::XXXX };
static const short d_SAH[2] = { 1, MarketId::YEM0 };
static const short d_SAL[3] = { 2, MarketId::SLV0, MarketId::XSVA };
static const short d_SAP[2] = { 1, MarketId::XHON };
static const short d_SBH[2] = { 1, MarketId::BLM0 };
static const short d_SBZ[5] = { 4, MarketId::BMFA, MarketId::BMFM, MarketId::BMFX, MarketId::SBMF };
static const short d_SDQ[4] = { 3, MarketId::DOM0, MarketId::XBVR, MarketId::XCVD };
static const short d_SDR[2] = { 1, MarketId::SANT };
static const short d_SEL[12] = { 11, MarketId::GSXK, MarketId::KOCN, MarketId::KOR0, MarketId::XKCM, MarketId::XKEM, MarketId::XKFB, MarketId::XKFE, MarketId::XKON, MarketId::XKOR, MarketId::XKOS, MarketId::XKRX };
static const short d_SEZ[7] = { 6, MarketId::SECC, MarketId::SECD, MarketId::SECE, MarketId::SEDC, MarketId::SYC0, MarketId::TRPX };
static const short d_SFG[2] = { 1, MarketId::MAF0 };
static const short d_SFN[3] = { 2, MarketId::BCFS, MarketId::XMVL };
static const short d_SFO[7] = { 6, MarketId::FICO, MarketId::LTSE, MarketId::SAGE, MarketId::XFCI, MarketId::XFDA, MarketId::XPSE };
static const short d_SGN[2] = { 1, MarketId::XSTC };
static const short d_SH0[2] = { 1, MarketId::XEMS };
static const short d_SHA[9] = { 8, MarketId::CCFX, MarketId::CFBC, MarketId::SGEX, MarketId::XCFE, MarketId::XINE, MarketId::XSGE, MarketId::XSHG, MarketId::XSSC };
static const short d_SI0[2] = { 1, MarketId::JBSI };
static const short d_SIN[41] = { 40, MarketId::APCL, MarketId::APEX, MarketId::BBLX, MarketId::BGSG, MarketId::BTBS, MarketId::CHIE, MarketId::CLTD, MarketId::CNOD, MarketId::ECAL, MarketId::EEAL, MarketId::ENMS, MarketId::EQOC, MarketId::EQOD, MarketId::EQOS, MarketId::FXSM, MarketId::GFSG, MarketId::IFSG, MarketId::JADX, MarketId::LMAS, MarketId::MALM, MarketId::MASG, MarketId::OSSG, MarketId::RTSP, MarketId::SGP0, MarketId::SMEX, MarketId::SYFX, MarketId::TERM, MarketId::TFSA, MarketId::TRDC, MarketId::TSIG, MarketId::TWSG, MarketId::XABX, MarketId::XAND, MarketId::XANM, MarketId::XSBT, MarketId::XSCA, MarketId::XSCE, MarketId::XSCL, MarketId::XSES, MarketId::XSIM };
static const short d_SJI[3] = { 2, MarketId::CRI0, MarketId::XBNV };
static const short d_SJJ[3] = { 2, MarketId::BIH0, MarketId::XSSE };
static const short d_SJU[3] = { 2, MarketId::PRI0, MarketId::XGMX };
static const short d_SK0[2] = { 1, MarketId::XKST };
static const short d_SKB[3] = { 2, MarketId::KNA0, MarketId::XECS };
static const short d_SKG[2] = { 1, MarketId::XXXX };
static const short d_SKP[3] = { 2, MarketId::MKD0, MarketId::XMAE };
static const short d_SLC[2] = { 1, MarketId::XXXX };
static const short d_SLE[2] = { 1, MarketId::XXXX };
static const short d_SLU[2] = { 1, MarketId::LCA0 };
static const short d_SM0[2] = { 1, MarketId::SMR0 };
static const short d_SMF[2] = { 1, MarketId::XXXX };
static const short d_SNN[2] = { 1, MarketId::XXXX };
static const short d_SOF[19] = { 18, MarketId::ABUL, MarketId::BDSK, MarketId::BEBG, MarketId::BGHX, MarketId::BGR0, MarketId::EUFN, MarketId::GBUL, MarketId::GMBG, MarketId::IBEX, MarketId::IBUL, MarketId::JBUL, MarketId::LBUL, MarketId::MBUL, MarketId::PBUL, MarketId::T212, MarketId::UCBG, MarketId::XBUL, MarketId::ZBUL };
static const short d_SPI[2] = { 1, MarketId::XXXX };
static const short d_SPN[2] = { 1, MarketId::MNP0 };
static const short d_SPU[2] = { 1, MarketId::XOTP };
static const short d_SSG[2] = { 1, MarketId::GNQ0 };
static const short d_ST0[7] = { 6, MarketId::ASMT, MarketId::ASPI, MarketId::ASPN, MarketId::INCR, MarketId::NTRL, MarketId::XPIN };
static const short d_STI[4] = { 3, MarketId::CHL0, MarketId::XBCL, MarketId::XSGO };
static const short d_STP[2] = { 1, MarketId::XXXX };
static const short d_STR[16] = { 15, MarketId::EUWA, MarketId::EUWX, MarketId::LBBW, MarketId::LBWL, MarketId::LBWS, MarketId::STUA, MarketId::STUB, MarketId::STUC, MarketId::STUD, MarketId::STUE, MarketId::STUF, MarketId::XDEX, MarketId::XSTF, MarketId::XSTP, MarketId::XSTU };
static const short d_STT[2] = { 1, MarketId::VIR0 };
static const short d_SUB[2] = { 1, MarketId::XSUR };
static const short d_SUV[3] = { 2, MarketId::FJI0, MarketId::XSPS };
static const short d_SVD[3] = { 2, MarketId::SVXI, MarketId::VCT0 };
static const short d_SVX[2] = { 1, MarketId::URCE };
static const short d_SW0[2] = { 1, MarketId::EFTP };
static const short d_SWS[2] = { 1, MarketId::XXXX };
static const short d_SXB[2] = { 1, MarketId::XXXX };
static const short d_SXM[2] = { 1, MarketId::SXM0 };
static const short d_SYD[41] = { 40, MarketId::APXL, MarketId::ASXB, MarketId::ASXC, MarketId::ASXP, MarketId::ASXT, MarketId::ASXV, MarketId::AWEX, MarketId::BGCA, MarketId::BLXA, MarketId::CFAU, MarketId::CGMA, MarketId::CHIA, MarketId::CLAU, MarketId::CSAU, MarketId::CXAB, MarketId::CXAC, MarketId::CXAF, MarketId::CXAI, MarketId::CXAM, MarketId::CXAN, MarketId::CXAP, MarketId::CXAQ, MarketId::CXAR, MarketId::CXAV, MarketId::CXAW, MarketId::GFAU, MarketId::MACB, MarketId::MAQX, MarketId::MEAU, MarketId::MSAL, MarketId::NZFX, MarketId::POTL, MarketId::SIGA, MarketId::SIMV, MarketId::XAOM, MarketId::XASX, MarketId::XFEX, MarketId::XLQC, MarketId::XSFE, MarketId::XYIE };
static const short d_SZG[2] = { 1, MarketId::SLHB };
static const short d_SZX[4] = { 3, MarketId::XSEC, MarketId::XSHE, MarketId::XSME };
static const short d_TAS[7] = { 6, MarketId::UZB0, MarketId::XCET, MarketId::XCUE, MarketId::XKCE, MarketId::XSTE, MarketId::XUNI };
static const short d_TBS[3] = { 2, MarketId::GEO0, MarketId::XGSE };
static const short d_TBU[2] = { 1, MarketId::TON0 };
static const short d_TF0[2] = { 1, MarketId::ATF0 };
static const short d_TGD[3] = { 2, MarketId::MNE0, MarketId::XMNX };
static const short d_TGU[3] = { 2, MarketId::HND0, MarketId::XBCV };
static const short d_THR[5] = { 4, MarketId::IFBX, MarketId::IMEX, MarketId::IRN0, MarketId::XTEH };
static const short d_TIA[4] = { 3, MarketId::ALB0, MarketId::XALS, MarketId::XTIR };
static const short d_TIP[3] = { 2, MarketId::LBY0, MarketId::XLSM };
static const short d_TK0[2] = { 1, MarketId::TKL0 };
static const short d_TLH[2] = { 1, MarketId::XXXX };
static const short d_TLL[8] = { 7, MarketId::EST0, MarketId::FNEE, MarketId::LMNR, MarketId::SWEE, MarketId::XTAA, MarketId::XTAL, MarketId::XTAR };
static const short d_TLS[3] = { 2, MarketId::ENSL, MarketId::ESLO };
static const short d_TLV[3] = { 2, MarketId::ISR0, MarketId::XTAE };
static const short d_TMP[2] = { 1, MarketId::XXXX };
static const short d_TMS[2] = { 1, MarketId::STP0 };
static const short d_TNR[3] = { 2, MarketId::MDG0, MarketId::XMDG };
static const short d_TOS[2] = { 1, MarketId::FSHX };
static const short d_TRD[3] = { 2, MarketId::NOPS, MarketId::SPTR };
static const short d_TRN[3] = { 2, MarketId::BREA, MarketId::ISBA };
static const short d_TRW[2] = { 1, MarketId::KIR0 };
static const short d_TSA[9] = { 8, MarketId::CFTW, MarketId::GSXT, MarketId::ROCO, MarketId::TWN0, MarketId::XIME, MarketId::XTAD, MarketId::XTAF, MarketId::XTAI };
static const short d_TSE[4] = { 3, MarketId::AIXK, MarketId::CCEX, MarketId::KAZ0 };
static const short d_TTN[2] = { 1, MarketId::XXXX };
static const short d_TUC[2] = { 1, MarketId::XTUC };
static const short d_TUN[4] = { 3, MarketId::BTUN, MarketId::TUN0, MarketId::XTUN };
static const short d_TYO[63] = { 62, MarketId::BAJD, MarketId::BASX, MarketId::BETP, MarketId::BGCJ, MarketId::BNPX, MarketId::CFJP, MarketId::CHIJ, MarketId::CHIS, MarketId::CHIV, MarketId::CILH, MarketId::CITD, MarketId::CITX, MarketId::CLJP, MarketId::CMET, MarketId::CSJP, MarketId::DRCT, MarketId::ICHK, MarketId::ICKR, MarketId::ICSH, MarketId::ICSZ, MarketId::ICTW, MarketId::JASR, MarketId::JATA, MarketId::JPN0, MarketId::KABU, MarketId::KAIX, MarketId::LXJP, MarketId::MAQJ, MarketId::MIZX, MarketId::MSMS, MarketId::MUDX, MarketId::NMRJ, MarketId::NMSX, MarketId::NXBX, MarketId::NXFO, MarketId::NXJP, MarketId::NXSE, MarketId::NXVW, MarketId::ODXE, MarketId::SBIJ, MarketId::SBIU, MarketId::SBIV, MarketId::SIGJ, MarketId::SMBC, MarketId::TICT, MarketId::TWJP, MarketId::TWJT, MarketId::UTSL, MarketId::VKAB, MarketId::XIJP, MarketId::XJAS, MarketId::XJPX, MarketId::XSBI, MarketId::XTAM, MarketId::XTFF, MarketId::XTK1, MarketId::XTK2, MarketId::XTK3, MarketId::XTKO, MarketId::XTKS, MarketId::XTKT, MarketId::XYKT };
static const short d_UIO[3] = { 2, MarketId::ECU0, MarketId::XQUI };
static const short d_UKB[3] = { 2, MarketId::XKGT, MarketId::XKKT };
static const short d_UKY[2] = { 1, MarketId::XKYO };
static const short d_ULN[3] = { 2, MarketId::MNG0, MarketId::XULA };
static const short d_UTC[4] = { 3, MarketId::CLMX, MarketId::GMGE, MarketId::RABO };
static const short d_VA0[2] = { 1, MarketId::VAT0 };
static const short d_VFA[2] = { 1, MarketId::VFEX };
static const short d_VIE[17] = { 16, MarketId::APAW, MarketId::AUT0, MarketId::EGSI, MarketId::EXAA, MarketId::FPWB, MarketId::LLAT, MarketId::RBIV, MarketId::UCBA, MarketId::WBAH, MarketId::WBDM, MarketId::WBGF, MarketId::XCEG, MarketId::XOTB, MarketId::XRCB, MarketId::XVIE, MarketId::XWBO };
static const short d_VIX[2] = { 1, MarketId::BOVM };
static const short d_VLC[3] = { 2, MarketId::XFCM, MarketId::XVAL };
static const short d_VLI[4] = { 3, MarketId::GXMA, MarketId::POTC, MarketId::VUT0 };
static const short d_VN0[2] = { 1, MarketId::NODX };
static const short d_VNO[10] = { 9, MarketId::BAPX, MarketId::FNLT, MarketId::GETB, MarketId::LTU0, MarketId::NASB, MarketId::SEBL, MarketId::SWLT, MarketId::XLIT, MarketId::XVIA };
static const short d_VOG[2] = { 1, MarketId::XXXX };
static const short d_VPS[2] = { 1, MarketId::BOVA };
static const short d_VTE[3] = { 2, MarketId::LAO0, MarketId::XLAO };
static const short d_VVO[3] = { 2, MarketId::XAPI, MarketId::XVLA };
static const short d_VZ0[2] = { 1, MarketId::XVAR };
static const short d_WAW[41] = { 40, MarketId::BHWA, MarketId::BNPP, MarketId::BOSP, MarketId::BPKO, MarketId::CETO, MarketId::HBPL, MarketId::IENG, MarketId::INGW, MarketId::KDPW, MarketId::MBPL, MarketId::MSDM, MarketId::MTSP, MarketId::PARK, MarketId::PKOP, MarketId::PLPD, MarketId::PLPO, MarketId::PLPS, MarketId::PLPX, MarketId::POEE, MarketId::POL0, MarketId::PTPG, MarketId::RPWC, MarketId::SIAB, MarketId::TBSA, MarketId::TBSP, MarketId::WBCL, MarketId::WBLC, MarketId::WBON, MarketId::WCDE, MarketId::WDER, MarketId::WETP, MarketId::WGAS, MarketId::WIND, MarketId::WIPO, MarketId::WMTF, MarketId::WOPO, MarketId::XBRY, MarketId::XGLO, MarketId::XNCO, MarketId::XWAR };
static const short d_WDH[3] = { 2, MarketId::NAM0, MarketId::XNAM };
static const short d_WLG[3] = { 2, MarketId::NZL0, MarketId::XNZE };
static const short d_WLS[2] = { 1, MarketId::WLF0 };
static const short d_WP0[2] = { 1, MarketId::INTL };
static const short d_WRO[3] = { 2, MarketId::CAPL, MarketId::WBKP };
static const short d_WUX[2] = { 1, MarketId::CSSX };
static const short d_XCH[2] = { 1, MarketId::CXR0 };
static const short d_XXX[5] = { 4, MarketId::BILT, MarketId::XOFF, MarketId::XXX0, MarketId::XXXX };
static const short d_YAO[2] = { 1, MarketId::CMR0 };
static const short d_YHM[5] = { 4, MarketId::A24EX, MarketId::BMU0, MarketId::GTXE, MarketId::XBDA };
static const short d_YMQ[4] = { 3, MarketId::XMOC, MarketId::XMOD, MarketId::XMOO };
static const short d_YOW[2] = { 1, MarketId::CAN0 };
static const short d_YTZ[34] = { 33, MarketId::ADRK, MarketId::ATSA, MarketId::BNSX, MarketId::CAND, MarketId::CANX, MarketId::CHIC, MarketId::COTC, MarketId::CSE2, MarketId::EQCA, MarketId::IVZX, MarketId::LICA, MarketId::LYNX, MarketId::MATN, MarketId::NEOC, MarketId::NEOD, MarketId::NEOE, MarketId::NEON, MarketId::OMGA, MarketId::PURE, MarketId::TMXS, MarketId::VDRK, MarketId::XATS, MarketId::XATX, MarketId::XBBK, MarketId::XCNQ, MarketId::XCX2, MarketId::XCXD, MarketId::XDRK, MarketId::XICX, MarketId::XTFE, MarketId::XTOE, MarketId::XTSE, MarketId::XTSX };
static const short d_YVR[3] = { 2, MarketId::XTNX, MarketId::XVSE };
static const short d_YWG[3] = { 2, MarketId::IFCA, MarketId::XWCE };
static const short d_YYC[3] = { 2, MarketId::NGXC, MarketId::XALB };
static const short d_YYZ[2] = { 1, MarketId::XXXX };
static const short d_ZAG[8] = { 7, MarketId::HRV0, MarketId::XCRO, MarketId::XTRZ, MarketId::XZAG, MarketId::XZAM, MarketId::XZAP, MarketId::ZAPA };
static const short d_ZAZ[2] = { 1, MarketId::IBER };
static const short d_ZRH[40] = { 39, MarketId::BXDA, MarketId::CBOE, MarketId::CSAG, MarketId::CSOT, MarketId::CSZH, MarketId::DOTS, MarketId::EBSC, MarketId::EBSS, MarketId::EUCH, MarketId::EURM, MarketId::EUSC, MarketId::EUSP, MarketId::KMUX, MarketId::ROSR, MarketId::RULE, MarketId::STOX, MarketId::UBSC, MarketId::UBSF, MarketId::UBSG, MarketId::UBST, MarketId::VLEX, MarketId::XBTR, MarketId::XDLP, MarketId::XICB, MarketId::XQMH, MarketId::XQOD, MarketId::XREP, MarketId::XROT, MarketId::XSCU, MarketId::XSDX, MarketId::XSEB, MarketId::XSLS, MarketId::XSTV, MarketId::XSTX, MarketId::XSWM, MarketId::XSWO, MarketId::XSWX, MarketId::XVTX, MarketId::ZKBX };

// City to Markets
const short * Gazetteer::m_cty2mics[City::NUMCITY] = { 
d_NOCITY,
d_AB0, d_ABJ, d_ABV, d_ABZ, d_ACC, d_AD0, d_ADB, d_ADD, d_AEP, d_AKL, 
d_AL0, d_ALA, d_ALB, d_ALG, d_ALY, d_AMD, d_AMM, d_AMS, d_ANC, d_ANP, 
d_ANR, d_ANU, d_APW, d_ARH, d_ASB, d_ASI, d_ASM, d_ASU, d_ATH, d_ATL, 
d_ATQ, d_AUA, d_AUG, d_AUH, d_AUS, d_AXA, d_AY0, d_BAH, d_BBR, d_BBU, 
d_BCN, d_BCT, d_BCV, d_BDL, d_BEG, d_BER, d_BEY, d_BFN, d_BFS, d_BGF, 
d_BGI, d_BGO, d_BGW, d_BGY, d_BHX, d_BIO, d_BIS, d_BJL, d_BJM, d_BJS, 
d_BKK, d_BKO, d_BLQ, d_BLR, d_BLZ, d_BMA, d_BNA, d_BNJ, d_BNX, d_BOG, 
d_BOI, d_BOM, d_BON, d_BOS, d_BRE, d_BRN, d_BRS, d_BRU, d_BSB, d_BSL, 
d_BTR, d_BTS, d_BTV, d_BUD, d_BWN, d_BZV, d_CAE, d_CAI, d_CAY, d_CBG, 
d_CBR, d_CCK, d_CCS, d_CCU, d_CGK, d_CGO, d_CGP, d_CHC, d_CHI, d_CHS, 
d_CKY, d_CLJ, d_CLT, d_CMB, d_CMH, d_CMN, d_CNQ, d_CON, d_COO, d_COR, 
d_CPH, d_CPT, d_CRW, d_CSN, d_CUR, d_CWB, d_CWL, d_CYS, d_DAC, d_DAM, 
d_DAR, d_DCA, d_DCF, d_DEL, d_DEN, d_DFW, d_DIL, d_DKR, d_DLA, d_DLC, 
d_DND, d_DNK, d_DOH, d_DOV, d_DSM, d_DUB, d_DXB, d_DYU, d_DZA, d_EA0, 
d_EB0, d_EDI, d_EIS, d_EP0, d_ES0, d_ESB, d_ESH, d_ESR, d_EUN, d_EVN, 
d_EWR, d_FAE, d_FC0, d_FDF, d_FFT, d_FIH, d_FLR, d_FNA, d_FNJ, d_FOE, 
d_FRA, d_FRU, d_FSP, d_FUK, d_FUN, d_GBE, d_GC0, d_GCI, d_GCM, d_GDT, 
d_GEO, d_GH0, d_GIB, d_GLA, d_GND, d_GOA, d_GOH, d_GOI, d_GOJ, d_GOT, 
d_GR0, d_GRU, d_GRX, d_GRZ, d_GS0, d_GUA, d_GUM, d_GVA, d_GW0, d_GYD, 
d_GYE, d_HAH, d_HAJ, d_HAM, d_HAN, d_HAV, d_HEL, d_HG0, d_HIJ, d_HIR, 
d_HKG, d_HLN, d_HNL, d_HRE, d_HRK, d_HS0, d_IAH, d_IDR, d_IEV, d_ILG, 
d_ILZ, d_IND, d_INU, d_IOM, d_ISB, d_IST, d_IUE, d_JAN, d_JED, d_JEF, 
d_JER, d_JIB, d_JNB, d_JNU, d_JSR, d_JUB, d_KBL, d_KGL, d_KHI, d_KIJ, 
d_KIN, d_KIV, d_KLA, d_KLU, d_KRK, d_KRT, d_KTM, d_KUF, d_KUL, d_KWI, 
d_LAD, d_LAN, d_LAX, d_LBA, d_LBU, d_LBV, d_LCA, d_LCG, d_LDY, d_LED, 
d_LEJ, d_LFW, d_LHE, d_LI0, d_LIM, d_LIS, d_LIT, d_LJU, d_LLW, d_LM0, 
d_LNK, d_LNZ, d_LON, d_LOS, d_LPB, d_LPL, d_LU0, d_LUN, d_LUX, d_LV0, 
d_LYR, d_LYS, d_MAA, d_MAD, d_MAJ, d_MAN, d_MC0, d_MCI, d_MCM, d_MCT, 
d_MDT, d_MDZ, d_MEL, d_MEX, d_MFM, d_MGA, d_MGF, d_MGN, d_MGQ, d_MHQ, 
d_MIA, d_MIL, d_MKE, d_MLA, d_MLE, d_MLW, d_MMX, d_MNI, d_MNL, d_MOW, 
d_MPM, d_MRS, d_MRU, d_MSN, d_MSP, d_MSQ, d_MSU, d_MUC, d_MV0, d_MVD, 
d_NAP, d_NAS, d_NBO, d_NCL, d_NDJ, d_NGO, d_NIC, d_NIM, d_NKC, d_NKW, 
d_NLK, d_NOU, d_NTL, d_NYC, d_OB0, d_ODS, d_OKC, d_OKD, d_OLM, d_OPO, 
d_ORK, d_OSA, d_OSL, d_OUA, d_OVB, d_OXB, d_PAC, d_PAP, d_PAR, d_PBH, 
d_PBM, d_PC0, d_PCT, d_PHL, d_PHX, d_PIR, d_PMI, d_PN0, d_PNH, d_PNI, 
d_POM, d_POS, d_PPG, d_PPT, d_PRG, d_PRN, d_PRY, d_PS0, d_PSY, d_PVD, 
d_QDU, d_QIC, d_QMN, d_QND, d_QPA, d_QSA, d_RA0, d_RAI, d_RAR, d_RB0, 
d_RBA, d_RDU, d_RGN, d_RIC, d_RIO, d_RIX, d_RIZ, d_RKV, d_ROM, d_ROR, 
d_ROS, d_ROV, d_RTM, d_RUH, d_RUN, d_SAF, d_SAH, d_SAL, d_SAP, d_SBH, 
d_SBZ, d_SDQ, d_SDR, d_SEL, d_SEZ, d_SFG, d_SFN, d_SFO, d_SGN, d_SH0, 
d_SHA, d_SI0, d_SIN, d_SJI, d_SJJ, d_SJU, d_SK0, d_SKB, d_SKG, d_SKP, 
d_SLC, d_SLE, d_SLU, d_SM0, d_SMF, d_SNN, d_SOF, d_SPI, d_SPN, d_SPU, 
d_SSG, d_ST0, d_STI, d_STP, d_STR, d_STT, d_SUB, d_SUV, d_SVD, d_SVX, 
d_SW0, d_SWS, d_SXB, d_SXM, d_SYD, d_SZG, d_SZX, d_TAS, d_TBS, d_TBU, 
d_TF0, d_TGD, d_TGU, d_THR, d_TIA, d_TIP, d_TK0, d_TLH, d_TLL, d_TLS, 
d_TLV, d_TMP, d_TMS, d_TNR, d_TOS, d_TRD, d_TRN, d_TRW, d_TSA, d_TSE, 
d_TTN, d_TUC, d_TUN, d_TYO, d_UIO, d_UKB, d_UKY, d_ULN, d_UTC, d_VA0, 
d_VFA, d_VIE, d_VIX, d_VLC, d_VLI, d_VN0, d_VNO, d_VOG, d_VPS, d_VTE, 
d_VVO, d_VZ0, d_WAW, d_WDH, d_WLG, d_WLS, d_WP0, d_WRO, d_WUX, d_XCH, 
d_XXX, d_YAO, d_YHM, d_YMQ, d_YOW, d_YTZ, d_YVR, d_YWG, d_YYC, d_YYZ, 
d_ZAG, d_ZAZ, d_ZRH,  };


//
// region defs - should not change
//
static const short e_NoRegion[2] = { 1, Country::NOCOUNTRY };
static const short e_AntarcticRegion[5] = { 4, Country::ATA, Country::ATF, Country::BVT, Country::HMD };
static const short e_Africa[60] = { 59, Country::AGO, Country::BDI, Country::BEN, Country::BFA, Country::BWA, Country::CAF, Country::CIV, Country::CMR, Country::COD, Country::COG, Country::COM, Country::CPV, Country::DJI, Country::DZA, Country::EGY, Country::ERI, Country::ESH, Country::ETH, Country::GAB, Country::GHA, Country::GIN, Country::GMB, Country::GNB, Country::GNQ, Country::IOT, Country::KEN, Country::LBR, Country::LBY, Country::LSO, Country::MAR, Country::MDG, Country::MLI, Country::MOZ, Country::MRT, Country::MUS, Country::MWI, Country::MYT, Country::NAM, Country::NER, Country::NGA, Country::REU, Country::RWA, Country::SDN, Country::SEN, Country::SHN, Country::SLE, Country::SOM, Country::SSD, Country::STP, Country::SWZ, Country::SYC, Country::TCD, Country::TGO, Country::TUN, Country::TZA, Country::UGA, Country::ZAF, Country::ZMB, Country::ZWE };
static const short e_Oceania[28] = { 27, Country::ASM, Country::AUS, Country::CCK, Country::COK, Country::CXR, Country::FJI, Country::FSM, Country::GUM, Country::KIR, Country::MHL, Country::MNP, Country::NCL, Country::NFK, Country::NIU, Country::NRU, Country::NZL, Country::PCN, Country::PLW, Country::PNG, Country::PYF, Country::SLB, Country::TKL, Country::TON, Country::TUV, Country::VUT, Country::WLF, Country::WSM };
static const short e_Americas[58] = { 57, Country::ABW, Country::AIA, Country::ARG, Country::ATG, Country::BES, Country::BHS, Country::BLM, Country::BLZ, Country::BMU, Country::BOL, Country::BRA, Country::BRB, Country::CAN, Country::CHL, Country::COL, Country::CRI, Country::CUB, Country::CUW, Country::CYM, Country::DMA, Country::DOM, Country::ECU, Country::FLK, Country::GLP, Country::GRD, Country::GRL, Country::GTM, Country::GUF, Country::GUY, Country::HND, Country::HTI, Country::JAM, Country::KNA, Country::LCA, Country::MAF, Country::MEX, Country::MSR, Country::MTQ, Country::NIC, Country::PAN, Country::PER, Country::PRI, Country::PRY, Country::SGS, Country::SLV, Country::SPM, Country::SUR, Country::SXM, Country::TCA, Country::TTO, Country::UMI, Country::URY, Country::USA, Country::VCT, Country::VEN, Country::VGB, Country::VIR };
static const short e_Asia[52] = { 51, Country::AFG, Country::ARE, Country::ARM, Country::AZE, Country::BGD, Country::BHR, Country::BRN, Country::BTN, Country::CHN, Country::CYP, Country::GEO, Country::HKG, Country::IDN, Country::IND, Country::IRN, Country::IRQ, Country::ISR, Country::JOR, Country::JPN, Country::KAZ, Country::KGZ, Country::KHM, Country::KOR, Country::KWT, Country::LAO, Country::LBN, Country::LKA, Country::MAC, Country::MDV, Country::MMR, Country::MNG, Country::MYS, Country::NPL, Country::OMN, Country::PAK, Country::PHL, Country::PRK, Country::PSE, Country::QAT, Country::SAU, Country::SGP, Country::SYR, Country::THA, Country::TJK, Country::TKM, Country::TLS, Country::TUR, Country::TWN, Country::UZB, Country::VNM, Country::YEM };
static const short e_Europe[52] = { 51, Country::ALA, Country::ALB, Country::AND, Country::AUT, Country::BEL, Country::BGR, Country::BIH, Country::BLR, Country::CHE, Country::CZE, Country::DEU, Country::DNK, Country::ESP, Country::EST, Country::FIN, Country::FRA, Country::FRO, Country::GBR, Country::GGY, Country::GIB, Country::GRC, Country::HRV, Country::HUN, Country::IMN, Country::IRL, Country::ISL, Country::ITA, Country::JEY, Country::LIE, Country::LTU, Country::LUX, Country::LVA, Country::MCO, Country::MDA, Country::MKD, Country::MLT, Country::MNE, Country::NLD, Country::NOR, Country::POL, Country::PRT, Country::ROU, Country::RUS, Country::SJM, Country::SMR, Country::SRB, Country::SVK, Country::SVN, Country::SWE, Country::UKR, Country::VAT };

const short *Gazetteer::m_reg2cid[7] = { e_NoRegion, e_AntarcticRegion, e_Africa, e_Oceania, e_Americas, e_Asia, e_Europe,  }; 

static const short f_NoSubregion[2] = { 1, Country::NOCOUNTRY };
static const short f_AntarcticSubregion[5] = { 4, Country::ATA, Country::ATF, Country::BVT, Country::HMD };
static const short f_SouthAmerica[16] = { 15, Country::ARG, Country::BOL, Country::BRA, Country::CHL, Country::COL, Country::ECU, Country::FLK, Country::GUF, Country::GUY, Country::PER, Country::PRY, Country::SGS, Country::SUR, Country::URY, Country::VEN };
static const short f_WesternAfrica[18] = { 17, Country::BEN, Country::BFA, Country::CIV, Country::CPV, Country::GHA, Country::GIN, Country::GMB, Country::GNB, Country::LBR, Country::MLI, Country::MRT, Country::NER, Country::NGA, Country::SEN, Country::SHN, Country::SLE, Country::TGO };
static const short f_CentralAmerica[9] = { 8, Country::BLZ, Country::CRI, Country::GTM, Country::HND, Country::MEX, Country::NIC, Country::PAN, Country::SLV };
static const short f_EasternAfrica[21] = { 20, Country::BDI, Country::COM, Country::DJI, Country::ERI, Country::ETH, Country::IOT, Country::KEN, Country::MDG, Country::MOZ, Country::MUS, Country::MWI, Country::MYT, Country::REU, Country::RWA, Country::SOM, Country::SYC, Country::TZA, Country::UGA, Country::ZMB, Country::ZWE };
static const short f_NorthernAfrica[9] = { 8, Country::DZA, Country::EGY, Country::ESH, Country::LBY, Country::MAR, Country::SDN, Country::SSD, Country::TUN };
static const short f_MiddleAfrica[10] = { 9, Country::AGO, Country::CAF, Country::CMR, Country::COD, Country::COG, Country::GAB, Country::GNQ, Country::STP, Country::TCD };
static const short f_SouthernAfrica[6] = { 5, Country::BWA, Country::LSO, Country::NAM, Country::SWZ, Country::ZAF };
static const short f_NorthernAmerica[7] = { 6, Country::BMU, Country::CAN, Country::GRL, Country::SPM, Country::UMI, Country::USA };
static const short f_Caribbean[29] = { 28, Country::ABW, Country::AIA, Country::ATG, Country::BES, Country::BHS, Country::BLM, Country::BRB, Country::CUB, Country::CUW, Country::CYM, Country::DMA, Country::DOM, Country::GLP, Country::GRD, Country::HTI, Country::JAM, Country::KNA, Country::LCA, Country::MAF, Country::MSR, Country::MTQ, Country::PRI, Country::SXM, Country::TCA, Country::TTO, Country::VCT, Country::VGB, Country::VIR };
static const short f_EasternAsia[9] = { 8, Country::CHN, Country::HKG, Country::JPN, Country::KOR, Country::MAC, Country::MNG, Country::PRK, Country::TWN };
static const short f_SouthernAsia[10] = { 9, Country::AFG, Country::BGD, Country::BTN, Country::IND, Country::IRN, Country::LKA, Country::MDV, Country::NPL, Country::PAK };
static const short f_SouthEasternAsia[12] = { 11, Country::BRN, Country::IDN, Country::KHM, Country::LAO, Country::MMR, Country::MYS, Country::PHL, Country::SGP, Country::THA, Country::TLS, Country::VNM };
static const short f_SouthernEurope[17] = { 16, Country::ALB, Country::AND, Country::BIH, Country::ESP, Country::GIB, Country::GRC, Country::HRV, Country::ITA, Country::MKD, Country::MLT, Country::MNE, Country::PRT, Country::SMR, Country::SRB, Country::SVN, Country::VAT };
static const short f_AustraliaNewZealand[6] = { 5, Country::AUS, Country::CCK, Country::CXR, Country::NFK, Country::NZL };
static const short f_Melanesia[6] = { 5, Country::FJI, Country::NCL, Country::PNG, Country::SLB, Country::VUT };
static const short f_Micronesia[8] = { 7, Country::FSM, Country::GUM, Country::KIR, Country::MHL, Country::MNP, Country::NRU, Country::PLW };
static const short f_Polynesia[11] = { 10, Country::ASM, Country::COK, Country::NIU, Country::PCN, Country::PYF, Country::TKL, Country::TON, Country::TUV, Country::WLF, Country::WSM };
static const short f_CentralAsia[6] = { 5, Country::KAZ, Country::KGZ, Country::TJK, Country::TKM, Country::UZB };
static const short f_WesternAsia[19] = { 18, Country::ARE, Country::ARM, Country::AZE, Country::BHR, Country::CYP, Country::GEO, Country::IRQ, Country::ISR, Country::JOR, Country::KWT, Country::LBN, Country::OMN, Country::PSE, Country::QAT, Country::SAU, Country::SYR, Country::TUR, Country::YEM };
static const short f_EasternEurope[11] = { 10, Country::BGR, Country::BLR, Country::CZE, Country::HUN, Country::MDA, Country::POL, Country::ROU, Country::RUS, Country::SVK, Country::UKR };
static const short f_NorthernEurope[17] = { 16, Country::ALA, Country::DNK, Country::EST, Country::FIN, Country::FRO, Country::GBR, Country::GGY, Country::IMN, Country::IRL, Country::ISL, Country::JEY, Country::LTU, Country::LVA, Country::NOR, Country::SJM, Country::SWE };
static const short f_WesternEurope[10] = { 9, Country::AUT, Country::BEL, Country::CHE, Country::DEU, Country::FRA, Country::LIE, Country::LUX, Country::MCO, Country::NLD };

const short *Gazetteer::m_subreg2cid[24] = {
f_NoSubregion, f_AntarcticSubregion, f_SouthAmerica, f_WesternAfrica, f_CentralAmerica, f_EasternAfrica, f_NorthernAfrica, f_MiddleAfrica, f_SouthernAfrica, f_NorthernAmerica, 
f_Caribbean, f_EasternAsia, f_SouthernAsia, f_SouthEasternAsia, f_SouthernEurope, f_AustraliaNewZealand, f_Melanesia, f_Micronesia, f_Polynesia, f_CentralAsia, 
f_WesternAsia, f_EasternEurope, f_NorthernEurope, f_WesternEurope,  };

static const short g_NoRegion[2] = { 1, Gazetteer::NOREGION };
static const short g_AntarcticRegion[2] = { 1, Gazetteer::AntarcticSubregion };
static const short g_Africa[6] = { 5, Gazetteer::WesternAfrica, Gazetteer::EasternAfrica, Gazetteer::NorthernAfrica, Gazetteer::MiddleAfrica, Gazetteer::SouthernAfrica };
static const short g_Oceania[5] = { 4, Gazetteer::AustraliaNewZealand, Gazetteer::Melanesia, Gazetteer::Micronesia, Gazetteer::Polynesia };
static const short g_Americas[5] = { 4, Gazetteer::SouthAmerica, Gazetteer::CentralAmerica, Gazetteer::NorthernAmerica, Gazetteer::Caribbean };
static const short g_Asia[6] = { 5, Gazetteer::EasternAsia, Gazetteer::SouthernAsia, Gazetteer::SouthEasternAsia, Gazetteer::CentralAsia, Gazetteer::WesternAsia };
static const short g_Europe[5] = { 4, Gazetteer::SouthernEurope, Gazetteer::EasternEurope, Gazetteer::NorthernEurope, Gazetteer::WesternEurope };

const short *Gazetteer::m_reg2subreg[7] = { g_NoRegion, g_AntarcticRegion, g_Africa, g_Oceania, g_Americas, g_Asia, g_Europe };

const unsigned char Gazetteer::m_region[Country::NUMCOUNTRY] = { NOREGION,
	Americas, Asia, Africa, Americas, Europe, Europe, Europe, Asia, Americas, Asia, 
	Oceania, AntarcticRegion, AntarcticRegion, Americas, Oceania, Europe, Asia, Africa, Europe, Africa, 
	Americas, Africa, Asia, Europe, Asia, Americas, Europe, Americas, Europe, Americas, 
	Americas, Americas, Americas, Americas, Asia, Asia, AntarcticRegion, Africa, Africa, Americas, 
	Oceania, Europe, Americas, Asia, Africa, Africa, Africa, Africa, Oceania, Americas, 
	Africa, Africa, Americas, Americas, Americas, Oceania, Americas, Asia, Europe, Europe, 
	Africa, Americas, Europe, Americas, Africa, Americas, Africa, Africa, Africa, Europe, 
	Europe, Africa, NOREGION, Europe, Oceania, Americas, Europe, Europe, Oceania, Africa, 
	Europe, Asia, Europe, Africa, Europe, Africa, Americas, Africa, Africa, Africa, 
	Europe, Americas, Americas, Americas, Americas, Oceania, Americas, Asia, AntarcticRegion, Americas, 
	Europe, Americas, Europe, Asia, Europe, Asia, Africa, Europe, Asia, Asia, 
	Europe, Asia, Europe, Americas, Europe, Asia, Asia, Asia, Africa, Asia, 
	Asia, Oceania, Americas, Asia, Asia, Asia, Asia, Africa, Africa, Americas, 
	Europe, Asia, Africa, Europe, Europe, Europe, Asia, Americas, Africa, Europe, 
	Europe, Africa, Asia, Americas, Oceania, Europe, Africa, Europe, Asia, Europe, 
	Asia, Oceania, Africa, Africa, Americas, Americas, Africa, Africa, Asia, Africa, 
	Africa, Oceania, Africa, Oceania, Africa, Americas, Oceania, Europe, Europe, Asia, 
	Oceania, Oceania, Asia, Asia, Americas, Oceania, Americas, Asia, Oceania, Oceania, 
	Europe, Americas, Asia, Europe, Americas, Asia, Oceania, Asia, Africa, Europe, 
	Europe, Africa, Asia, Africa, Africa, Asia, Americas, Africa, Europe, Oceania, 
	Africa, Americas, Europe, Africa, Americas, Europe, Africa, Africa, Americas, Europe, 
	Europe, Europe, Africa, Americas, Africa, Asia, Americas, Africa, Africa, Asia, 
	Asia, Oceania, Asia, Asia, Oceania, Americas, Africa, Asia, Oceania, Asia, 
	Africa, Africa, Europe, Americas, Americas, Americas, Asia, Europe, Americas, Americas, 
	Americas, Americas, Asia, Oceania, Oceania, Oceania, NOREGION, NOREGION, NOREGION, NOREGION, 
	NOREGION, Asia, Africa, Africa, Africa 
};

const unsigned char Gazetteer::m_subregion[Country::NUMCOUNTRY] = { NOSUBREGION,
    Caribbean, SouthernAsia, MiddleAfrica, Caribbean, NorthernEurope, SouthernEurope, SouthernEurope, WesternAsia, SouthAmerica, WesternAsia, 
    Polynesia, AntarcticSubregion , AntarcticSubregion , Caribbean, AustraliaNewZealand, WesternEurope, WesternAsia, EasternAfrica, WesternEurope, WesternAfrica, 
    Caribbean, WesternAfrica, SouthernAsia, EasternEurope, WesternAsia, Caribbean, SouthernEurope, Caribbean, EasternEurope, CentralAmerica, 
    NorthernAmerica, SouthAmerica, SouthAmerica, Caribbean, SouthEasternAsia, SouthernAsia, AntarcticSubregion , SouthernAfrica, MiddleAfrica, NorthernAmerica, 
    AustraliaNewZealand, WesternEurope, SouthAmerica, EasternAsia, WesternAfrica, MiddleAfrica, MiddleAfrica, MiddleAfrica, Polynesia, SouthAmerica, 
    EasternAfrica, WesternAfrica, CentralAmerica, Caribbean, Caribbean, AustraliaNewZealand, Caribbean, WesternAsia, EasternEurope, WesternEurope, 
    EasternAfrica, Caribbean, NorthernEurope, Caribbean, NorthernAfrica, SouthAmerica, NorthernAfrica, EasternAfrica, NorthernAfrica, SouthernEurope, 
    NorthernEurope, EasternAfrica, NOSUBREGION, NorthernEurope, Melanesia, SouthAmerica, WesternEurope, NorthernEurope, Micronesia, MiddleAfrica, 
    NorthernEurope, WesternAsia, NorthernEurope, WesternAfrica, SouthernEurope, WesternAfrica, Caribbean, WesternAfrica, WesternAfrica, MiddleAfrica, 
    SouthernEurope, Caribbean, NorthernAmerica, CentralAmerica, SouthAmerica, Micronesia, SouthAmerica, EasternAsia, AntarcticSubregion , CentralAmerica, 
    SouthernEurope, Caribbean, EasternEurope, SouthEasternAsia, NorthernEurope, SouthernAsia, EasternAfrica, NorthernEurope, SouthernAsia, WesternAsia, 
    NorthernEurope, WesternAsia, SouthernEurope, Caribbean, NorthernEurope, WesternAsia, EasternAsia, CentralAsia, EasternAfrica, CentralAsia, 
    SouthEasternAsia, Micronesia, Caribbean, EasternAsia, WesternAsia, SouthEasternAsia, WesternAsia, WesternAfrica, NorthernAfrica, Caribbean, 
    WesternEurope, SouthernAsia, SouthernAfrica, NorthernEurope, WesternEurope, NorthernEurope, EasternAsia, Caribbean, NorthernAfrica, WesternEurope, 
    EasternEurope, EasternAfrica, SouthernAsia, CentralAmerica, Micronesia, SouthernEurope, WesternAfrica, SouthernEurope, SouthEasternAsia, SouthernEurope, 
    EasternAsia, Micronesia, EasternAfrica, WesternAfrica, Caribbean, Caribbean, EasternAfrica, EasternAfrica, SouthEasternAsia, EasternAfrica, 
    SouthernAfrica, Melanesia, WesternAfrica, AustraliaNewZealand, WesternAfrica, CentralAmerica, Polynesia, WesternEurope, NorthernEurope, SouthernAsia, 
    Micronesia, AustraliaNewZealand, WesternAsia, SouthernAsia, CentralAmerica, Polynesia, SouthAmerica, SouthEasternAsia, Micronesia, Melanesia, 
    EasternEurope, Caribbean, EasternAsia, SouthernEurope, SouthAmerica, WesternAsia, Polynesia, WesternAsia, EasternAfrica, EasternEurope, 
    EasternEurope, EasternAfrica, WesternAsia, NorthernAfrica, WesternAfrica, SouthEasternAsia, SouthAmerica, WesternAfrica, NorthernEurope, Melanesia, 
    WesternAfrica, CentralAmerica, SouthernEurope, EasternAfrica, NorthernAmerica, SouthernEurope, NorthernAfrica, MiddleAfrica, SouthAmerica, EasternEurope, 
    SouthernEurope, NorthernEurope, SouthernAfrica, Caribbean, EasternAfrica, WesternAsia, Caribbean, MiddleAfrica, WesternAfrica, SouthEasternAsia, 
    CentralAsia, Polynesia, CentralAsia, SouthEasternAsia, Polynesia, Caribbean, NorthernAfrica, WesternAsia, Polynesia, EasternAsia, 
    EasternAfrica, EasternAfrica, EasternEurope, NorthernAmerica, SouthAmerica, NorthernAmerica, CentralAsia, SouthernEurope, Caribbean, SouthAmerica, 
    Caribbean, Caribbean, SouthEasternAsia, Melanesia, Polynesia, Polynesia, NOSUBREGION, NOSUBREGION, NOSUBREGION, NOSUBREGION, 
    NOSUBREGION, WesternAsia, SouthernAfrica, EasternAfrica, EasternAfrica
};



