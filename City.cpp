/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$
 $   City.cpp - h   $
 $$$$$$$$$$$$$$$%$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved.
 History: Supports some of the IATA airport/city code list 	   
 
 **** Updated 10/10/2023 ****
 
 */

#ifndef __CITY_H__
#include "City.h"
#endif

#include <assert.h>

	
std::ostream&
operator<<( std::ostream& ostr, const City& c )
{
	ostr << c.toString();
	return ostr;
}

std::istream&
operator>>( std::istream& istr, City& c )
{
	std::string str;
	istr >> str;
	c.setCity( str );
	return istr;
}

//
//
//

std::string
City::name( void ) const 
{
	return m_fullCityNames[m_city];	
}

bool
City::capital( void ) const
{
	return m_capital[m_city];
}

float
City::lon( void ) const
{
	return m_position[m_city][1];
}

float
City::lat( void ) const
{
	return m_position[m_city][0];
}


// this speeds up setCurrency quite a bit
const short City::m_searchPoints[] = {
    1, 38, 87, 119, 140, 152, 166, 192, 207, 218, 227, 241, 273, 311, 325, 337, 361, 367, 386, 438, 465, 470, 483, 490, 492, 501, 504, 
};

bool
City::setCity( const std::string& str )
{	
    if (str.size() != 3)
	{
        m_city = City::XXX; // NOCITY
		return false;
	}
    
    int index = str[0] - 'A'; // 'A' = 65;
    
    //assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_city = City::XXX;
        return false;
    }
    
	int low   = m_searchPoints[index]; 
	int high  = m_searchPoints[index + 1]; 
    
	while (low < high) 
	{
        int mid = low + ((high - low) >> 1);
        const char * const cty = m_cityNames[mid];
        
        int i = 1;
        
        for (; i < 3; ++i)
        {
            const char &a = str[i];
            const char &b = cty[i];
            
            if (a < b)
            {
                high = mid; 
                break;
            }
            
            if (a > b)
            {
                low = mid + 1;
                break;
            }
        }
        
        if (i > 2)
        {
            m_city = m_toISO[mid]; 
            return true;
        }
	}
    
    m_city = City::XXX; // NOCITY
	return false;
}





// if you add cities make sure you add the names in the correct alphabetic order position
// or else the binary chop search in setCity(std::string) won't work
const char * const City::m_cityNames[NUMCITY] = { "NOCITY", 
    "AB0", "ABJ", "ABV", "ABZ", "ACC", "AD0", "ADB", "ADD", "AEP", "AKL", 
    "AL0", "ALA", "ALB", "ALG", "ALY", "AMD", "AMM", "AMS", "ANC", "ANP", 
    "ANR", "ANU", "APW", "ARH", "ASB", "ASI", "ASM", "ASU", "ATH", "ATL", 
    "ATQ", "AUA", "AUG", "AUH", "AUS", "AXA", "AY0", "BAH", "BBR", "BBU", 
    "BCN", "BCT", "BCV", "BDL", "BEG", "BER", "BEY", "BFN", "BFS", "BGF", 
    "BGI", "BGO", "BGW", "BGY", "BHX", "BIO", "BIS", "BJL", "BJM", "BJS", 
    "BKK", "BKO", "BLQ", "BLR", "BLZ", "BMA", "BNA", "BNJ", "BNX", "BOG", 
    "BOI", "BOM", "BON", "BOS", "BRE", "BRN", "BRS", "BRU", "BSB", "BSL", 
    "BTR", "BTS", "BTV", "BUD", "BWN", "BZV", "CAE", "CAI", "CAY", "CBG", 
    "CBR", "CCK", "CCS", "CCU", "CGK", "CGO", "CGP", "CHC", "CHI", "CHS", 
    "CKY", "CLJ", "CLT", "CMB", "CMH", "CMN", "CNQ", "CON", "COO", "COR", 
    "CPH", "CPT", "CRW", "CSN", "CUR", "CWB", "CWL", "CYS", "DAC", "DAM", 
    "DAR", "DCA", "DCF", "DEL", "DEN", "DFW", "DIL", "DKR", "DLA", "DLC", 
    "DND", "DNK", "DOH", "DOV", "DSM", "DUB", "DXB", "DYU", "DZA", "EA0", 
    "EB0", "EDI", "EIS", "EP0", "ES0", "ESB", "ESH", "ESR", "EUN", "EVN", 
    "EWR", "FAE", "FC0", "FDF", "FFT", "FIH", "FLR", "FNA", "FNJ", "FOE", 
    "FRA", "FRU", "FSP", "FUK", "FUN", "GBE", "GC0", "GCI", "GCM", "GDT", 
    "GEO", "GH0", "GIB", "GLA", "GND", "GOA", "GOH", "GOI", "GOJ", "GOT", 
    "GR0", "GRU", "GRX", "GRZ", "GS0", "GUA", "GUM", "GVA", "GW0", "GYD", 
    "GYE", "HAH", "HAJ", "HAM", "HAN", "HAV", "HEL", "HG0", "HIJ", "HIR", 
    "HKG", "HLN", "HNL", "HRE", "HRK", "HS0", "IAH", "IDR", "IEV", "ILG", 
    "ILZ", "IND", "INU", "IOM", "ISB", "IST", "IUE", "JAN", "JED", "JEF", 
    "JER", "JIB", "JNB", "JNU", "JSR", "JUB", "KBL", "KGL", "KHI", "KIJ", 
    "KIN", "KIV", "KLA", "KLU", "KRK", "KRT", "KTM", "KUF", "KUL", "KWI", 
    "LAD", "LAN", "LAX", "LBA", "LBU", "LBV", "LCA", "LCG", "LDY", "LED", 
    "LEJ", "LFW", "LHE", "LI0", "LIM", "LIS", "LIT", "LJU", "LLW", "LM0", 
    "LNK", "LNZ", "LON", "LOS", "LPB", "LPL", "LU0", "LUN", "LUX", "LV0", 
    "LYR", "LYS", "MAA", "MAD", "MAJ", "MAN", "MC0", "MCI", "MCM", "MCT", 
    "MDT", "MDZ", "MEL", "MEX", "MFM", "MGA", "MGF", "MGN", "MGQ", "MHQ", 
    "MIA", "MIL", "MKE", "MLA", "MLE", "MLW", "MMX", "MNI", "MNL", "MOW", 
    "MPM", "MRS", "MRU", "MSN", "MSP", "MSQ", "MSU", "MUC", "MV0", "MVD", 
    "NAP", "NAS", "NBO", "NCL", "NDJ", "NGO", "NIC", "NIM", "NKC", "NKW", 
    "NLK", "NOU", "NTL", "NYC", "OB0", "ODS", "OKC", "OKD", "OLM", "OPO", 
    "ORK", "OSA", "OSL", "OUA", "OVB", "OXB", "PAC", "PAP", "PAR", "PBH", 
    "PBM", "PC0", "PCT", "PHL", "PHX", "PIR", "PMI", "PN0", "PNH", "PNI", 
    "POM", "POS", "PPG", "PPT", "PRG", "PRN", "PRY", "PS0", "PSY", "PVD", 
    "QDU", "QIC", "QMN", "QND", "QPA", "QSA", "RA0", "RAI", "RAR", "RB0", 
    "RBA", "RDU", "RGN", "RIC", "RIO", "RIX", "RIZ", "RKV", "ROM", "ROR", 
    "ROS", "ROV", "RTM", "RUH", "RUN", "SAF", "SAH", "SAL", "SAP", "SBH", 
    "SBZ", "SDQ", "SDR", "SEL", "SEZ", "SFG", "SFN", "SFO", "SGN", "SH0", 
    "SHA", "SI0", "SIN", "SJI", "SJJ", "SJU", "SK0", "SKB", "SKG", "SKP", 
    "SLC", "SLE", "SLU", "SM0", "SMF", "SNN", "SOF", "SPI", "SPN", "SPU", 
    "SSG", "ST0", "STI", "STP", "STR", "STT", "SUB", "SUV", "SVD", "SVX", 
    "SW0", "SWS", "SXB", "SXM", "SYD", "SZG", "SZX", "TAS", "TBS", "TBU", 
    "TF0", "TGD", "TGU", "THR", "TIA", "TIP", "TK0", "TLH", "TLL", "TLS", 
    "TLV", "TMP", "TMS", "TNR", "TOS", "TRD", "TRN", "TRW", "TSA", "TSE", 
    "TTN", "TUC", "TUN", "TYO", "UIO", "UKB", "UKY", "ULN", "UTC", "VA0", 
    "VFA", "VIE", "VIX", "VLC", "VLI", "VN0", "VNO", "VOG", "VPS", "VTE", 
    "VVO", "VZ0", "WAW", "WDH", "WLG", "WLS", "WP0", "WRO", "WUX", "XCH", 
    "XXX", "YAO", "YHM", "YMQ", "YOW", "YTZ", "YVR", "YWG", "YYC", "YYZ", 
    "ZAG", "ZAZ", "ZRH"
};


// IATA city name (which corresponds to the IATA airport code for that city)
const char * const City::m_fullCityNames[NUMCITY] = { "No City",
    "Aabenraa", "Abidjan", "Abuja", "Aberdeen", "Accra", "Andorra la Vella", "Izmir", "Addis Ababa", "Buenos Aires", "Auckland", 
    "Aalborg", "Almaty", "Albany", "Algiers", "Alexandria", "Ahmedabad", "Amman", "Amsterdam", "Anchorage", "Annapolis", 
    "Antwerp", "Saint Johns", "Apia", "Arkhangelsk", "Ashgabat", "Jamestown", "Asmara", "Asuncion", "Athens", "Atlanta", 
    "Amritsar", "Oranjestad", "Augusta", "Abu Dhabi", "Austin", "The Valley", "Aylesbury", "Manama", "Basse Terre", "Bucharest", 
    "Barcelona", "Boca Raton", "Belmopan", "Hartford", "Belgrade", "Berlin", "Beirut", "Bloemfontein", "Belfast", "Bangui", 
    "Bridgetown", "Bergen", "Baghdad", "Bergamo", "Birmingham", "Bilbao", "Bismarck", "Banjul", "Bujumbura", "Beijing", 
    "Bangkok", "Bamako", "Bologna", "Bangalore", "Blantyre", "Stockholm", "Nashville", "Bonn", "Banja Luka", "Bogota", 
    "Boise", "Mumbai", "Kralendijk", "Boston", "Bremen", "Bern", "Bristol", "Brussels", "Brasilia", "Basel", 
    "Baton Rouge", "Bratislava", "Montpelier", "Budapest", "Bandar Seri Begawan", "Brazzaville", "Columbia", "Cairo", "Cayenne", "Cambridge", 
    "Canberra", "West Island", "Caracas", "Calcutta", "Jakarta", "Zhengzhou", "Chittagong", "Christchurch", "Chicago", "Mount Pleasant", 
    "Conakry", "Cluj-Napoca", "Charlotte", "Colombo", "Columbus", "Casablanca", "Corrientes", "Concord", "Porto Novo", "Cordoba", 
    "Copenhagen", "Cape Town", "Charleston", "Carson City", "Willemstad", "Curitiba", "Cardiff", "Cheyenne", "Dhaka", "Damascus", 
    "Dar es Salaam", "Washington", "Roseau", "New Delhi", "Denver", "Dallas", "Dili", "Dakar", "Douala", "Dalian", 
    "Dundee", "Dnipropetrovsk", "Doha", "Dover", "Des Moines", "Dublin", "Dubai", "Dushanbe", "Mamoudzou", "Esch-sur-Alzette", 
    "Ebene", "Edinburgh", "Road Town", "Ecatepec", "Espoo", "Ankara", "Brighton", "El Salvador", "El Aaiun", "Yerevan", 
    "Jersey City", "Torshavn", "Foster City", "Fort de France", "Frankfort", "Kinshasa", "Florence", "Freetown", "Pyongyang", "Topeka", 
    "Frankfurt", "Bishkek", "Saint Pierre", "Fukuoka", "Funafuti", "Gaborone", "GIFT City", "Saint Peter Port", "George Town", "Cockburn Town", 
    "Georgetown", "Gandhinagar", "Gibraltar", "Glasgow", "Saint Georges", "Genoa", "Nuuk", "Goa", "Nizhny Novgorod", "Gothenburg", 
    "Grindsted", "Sao Paulo", "Jaen", "Graz", "King Edward Point", "Guatemala City", "Hagatna", "Geneva", "Greenwich", "Baku", 
    "Guayaquil", "Moroni", "Hannover", "Hamburg", "Hanoi", "Havana", "Helsinki", "The Hague", "Hiroshima", "Honiara", 
    "Hong Kong", "Helena", "Honolulu", "Harare", "Kharkov", "Horsens", "Houston", "Indore", "Kyiv", "Wilmington", 
    "Zilina", "Indianapolis", "Yaren", "Douglas", "Islamabad", "Istanbul", "Alofi", "Jackson", "Jeddah", "Jefferson City", 
    "Saint Helier", "Djibouti City", "Johannesburg", "Juneau", "Jerusalem", "Juba", "Kabul", "Kigali", "Karachi", "Niigata", 
    "Kingston", "Chisinau", "Kampala", "Klagenfurt", "Krakow", "Khartoum", "Kathmandu", "Samara", "Kuala Lumpur", "Kuwait City", 
    "Luanda", "Lansing", "Los Angeles", "Leeds", "Labuan", "Libreville", "Larnaca", "La Coruna", "Londonderry", "Saint Petersburg", 
    "Leipzig", "Lome", "Lahore", "Vaduz", "Lima", "Lisbon", "Little Rock", "Ljubljana", "Lilongwe", "Limassol", 
    "Lincoln", "Linz", "London", "Lagos", "La Paz", "Liverpool", "Lucerne", "Lusaka", "Luxembourg", "Leuven", 
    "Longyearbyen", "Lyon", "Madras", "Madrid", "Majuro", "Manchester", "Makati City", "Kansas City", "Monaco", "Muscat", 
    "Harrisburg", "Mendoza", "Melbourne", "Mexico City", "Macao", "Managua", "Maringa", "Montgomery", "Mogadishu", "Mariehamn", 
    "Miami", "Milan", "Milwaukee", "Valletta", "Male", "Monrovia", "Malmo", "Brades", "Manila", "Moscow", 
    "Maputo", "Marseille", "Port Louis", "Madison", "Minneapolis", "Minsk", "Maseru", "Munich", "Mill Valley", "Montevideo", 
    "Naples", "Nassau", "Nairobi", "Newcastle", "N Djamena", "Nagoya", "Nicosia", "Niamey", "Nouakchott", "Diego Garcia", 
    "Kingston", "Noumea", "Newcastle", "New York", "Oldenburg", "Odessa", "Oklahoma City", "Sapporo", "Olympia", "Porto", 
    "Cork", "Osaka", "Oslo", "Ouagadougou", "Novosibirsk", "Bissau", "Panama City", "Port au Prince", "Paris", "Thimphu", 
    "Paramaribo", "Pasig City", "Princeton", "Philadelphia", "Phoenix", "Pierre", "Palma", "Adamstown", "Phnom Penh", "Palikir", 
    "Port Moresby", "Port of Spain", "Pago Pago", "Papeete", "Prague", "Pristina", "Pretoria", "Nablus", "Stanley", "Providence", 
    "Duesseldorf", "Biella", "Mbabane", "Novi Sad", "Padua", "Sabadell", "Randers", "Praia", "Avarua", "Regensburg", 
    "Rabat", "Raleigh", "Yangon", "Richmond", "Rio de Janeiro", "Riga", "Rizhao", "Reykjavik", "Rome", "Ngerulmud", 
    "Rosario", "Rostov", "Rotterdam", "Riyadh", "Saint Denis", "Santa Fe", "Sanaa", "San Salvador", "San Pedro Sula", "Gustavia", 
    "Sibiu", "Santo Domingo", "Santander", "Seoul", "Victoria", "Marigot", "Santa Fe", "San Francisco", "Ho Chi Minh City", "s-Hertogenbosch", 
    "Shanghai", "Silkeborg", "Singapore", "San Jose", "Sarajevo", "San Juan", "Shimonoseki", "Basseterre", "Thessaloniki", "Skopje", 
    "Salt Lake City", "Salem", "Castries", "San Marino City", "Sacramento", "Limerick", "Sofia", "Springfield", "Capital Hill", "Split", 
    "Malabo", "Stamford", "Santiago", "Saint Paul", "Stuttgart", "Charlotte Amalie", "Surabaya", "Suva", "Kingstown", "Yekaterinburg", 
    "Schwerin", "Swansea", "Strasbourg", "Philipsburg", "Sydney", "Salzburg", "Shenzhen", "Tashkent", "Tbilisi", "Nukualofa", 
    "Port aux Francais", "Podgorica", "Tegucigalpa", "Tehran", "Tirana", "Tripoli", "Nukunonu", "Tallahassee", "Tallinn", "Toulouse", 
    "Tel Aviv", "Tampere", "Sao Tome", "Antananarivo", "Tromso", "Trondheim", "Turin", "Tarawa", "Taipei", "Astana", 
    "Trenton", "Tucuman", "Tunis", "Tokyo", "Quito", "Kobe", "Kyoto", "Ulan Bator", "Utrecht", "Vatican City", 
    "Victoria Falls", "Vienna", "Vitoria", "Valencia", "Port Vila", "Vienna", "Vilnius", "Volgograd", "Valparaiso", "Vientiane", 
    "Vladivostok", "Varazdin", "Warsaw", "Windhoek", "Wellington", "Mata Utu", "Winter Park", "Wroclaw", "Wuxi", "Flying Fish Cove", 
    "No City", "Yaounde", "Hamilton", "Montreal", "Ottawa", "Toronto", "Vancouver", "Winnipeg", "Calgary", "Mississauga", 
    "Zagreb", "Zaragoza", "Zurich"
};


// convert the alphabetical index for a 3 letter currency to the numeric code  
const short City::m_toISO[NUMCITY] = { NOCITY,
    AB0, ABJ, ABV, ABZ, ACC, AD0, ADB, ADD, AEP, AKL, 
    AL0, ALA, ALB, ALG, ALY, AMD, AMM, AMS, ANC, ANP, 
    ANR, ANU, APW, ARH, ASB, ASI, ASM, ASU, ATH, ATL, 
    ATQ, AUA, AUG, AUH, AUS, AXA, AY0, BAH, BBR, BBU, 
    BCN, BCT, BCV, BDL, BEG, BER, BEY, BFN, BFS, BGF, 
    BGI, BGO, BGW, BGY, BHX, BIO, BIS, BJL, BJM, BJS, 
    BKK, BKO, BLQ, BLR, BLZ, BMA, BNA, BNJ, BNX, BOG, 
    BOI, BOM, BON, BOS, BRE, BRN, BRS, BRU, BSB, BSL, 
    BTR, BTS, BTV, BUD, BWN, BZV, CAE, CAI, CAY, CBG, 
    CBR, CCK, CCS, CCU, CGK, CGO, CGP, CHC, CHI, CHS, 
    CKY, CLJ, CLT, CMB, CMH, CMN, CNQ, CON, COO, COR, 
    CPH, CPT, CRW, CSN, CUR, CWB, CWL, CYS, DAC, DAM, 
    DAR, DCA, DCF, DEL, DEN, DFW, DIL, DKR, DLA, DLC, 
    DND, DNK, DOH, DOV, DSM, DUB, DXB, DYU, DZA, EA0, 
    EB0, EDI, EIS, EP0, ES0, ESB, ESH, ESR, EUN, EVN, 
    EWR, FAE, FC0, FDF, FFT, FIH, FLR, FNA, FNJ, FOE, 
    FRA, FRU, FSP, FUK, FUN, GBE, GC0, GCI, GCM, GDT, 
    GEO, GH0, GIB, GLA, GND, GOA, GOH, GOI, GOJ, GOT, 
    GR0, GRU, GRX, GRZ, GS0, GUA, GUM, GVA, GW0, GYD, 
    GYE, HAH, HAJ, HAM, HAN, HAV, HEL, HG0, HIJ, HIR, 
    HKG, HLN, HNL, HRE, HRK, HS0, IAH, IDR, IEV, ILG, 
    ILZ, IND, INU, IOM, ISB, IST, IUE, JAN, JED, JEF, 
    JER, JIB, JNB, JNU, JSR, JUB, KBL, KGL, KHI, KIJ, 
    KIN, KIV, KLA, KLU, KRK, KRT, KTM, KUF, KUL, KWI, 
    LAD, LAN, LAX, LBA, LBU, LBV, LCA, LCG, LDY, LED, 
    LEJ, LFW, LHE, LI0, LIM, LIS, LIT, LJU, LLW, LM0, 
    LNK, LNZ, LON, LOS, LPB, LPL, LU0, LUN, LUX, LV0, 
    LYR, LYS, MAA, MAD, MAJ, MAN, MC0, MCI, MCM, MCT, 
    MDT, MDZ, MEL, MEX, MFM, MGA, MGF, MGN, MGQ, MHQ, 
    MIA, MIL, MKE, MLA, MLE, MLW, MMX, MNI, MNL, MOW, 
    MPM, MRS, MRU, MSN, MSP, MSQ, MSU, MUC, MV0, MVD, 
    NAP, NAS, NBO, NCL, NDJ, NGO, NIC, NIM, NKC, NKW, 
    NLK, NOU, NTL, NYC, OB0, ODS, OKC, OKD, OLM, OPO, 
    ORK, OSA, OSL, OUA, OVB, OXB, PAC, PAP, PAR, PBH, 
    PBM, PC0, PCT, PHL, PHX, PIR, PMI, PN0, PNH, PNI, 
    POM, POS, PPG, PPT, PRG, PRN, PRY, PS0, PSY, PVD, 
    QDU, QIC, QMN, QND, QPA, QSA, RA0, RAI, RAR, RB0, 
    RBA, RDU, RGN, RIC, RIO, RIX, RIZ, RKV, ROM, ROR, 
    ROS, ROV, RTM, RUH, RUN, SAF, SAH, SAL, SAP, SBH, 
    SBZ, SDQ, SDR, SEL, SEZ, SFG, SFN, SFO, SGN, SH0, 
    SHA, SI0, SIN, SJI, SJJ, SJU, SK0, SKB, SKG, SKP, 
    SLC, SLE, SLU, SM0, SMF, SNN, SOF, SPI, SPN, SPU, 
    SSG, ST0, STI, STP, STR, STT, SUB, SUV, SVD, SVX, 
    SW0, SWS, SXB, SXM, SYD, SZG, SZX, TAS, TBS, TBU, 
    TF0, TGD, TGU, THR, TIA, TIP, TK0, TLH, TLL, TLS, 
    TLV, TMP, TMS, TNR, TOS, TRD, TRN, TRW, TSA, TSE, 
    TTN, TUC, TUN, TYO, UIO, UKB, UKY, ULN, UTC, VA0, 
    VFA, VIE, VIX, VLC, VLI, VN0, VNO, VOG, VPS, VTE, 
    VVO, VZ0, WAW, WDH, WLG, WLS, WP0, WRO, WUX, XCH, 
    XXX, YAO, YHM, YMQ, YOW, YTZ, YVR, YWG, YYC, YYZ, 
    ZAG, ZAZ, ZRH
};

const short City::m_fromISO[MAXCITY] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 
    250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 
    260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 
    270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 
    280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 
    290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 
    300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 
    310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 
    320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 
    330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 
    340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 
    350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 
    360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 
    370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 
    380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 
    390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 
    400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 
    410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 
    420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 
    430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 
    440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 
    450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 
    460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 
    470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 
    480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 
    490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 
    500, 501, 502, 503, 
};

const unsigned char City::m_capital[NUMCITY] = { 0, // NOCITY
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 
    0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 
    0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 
    0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 
    1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 
    1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 
    0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 
    0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 
    1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 
    1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 
    1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 
    0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 
    0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 
    0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 
    0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 
    1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 
    0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 
    1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 
    0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 
    1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 
    1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 
    0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 
    1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 
    0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 
    0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 
    1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 
    0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
    1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 
    0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 
    1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 
    0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 
    0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 
    0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 
    0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 
    1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 
    0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 
    1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 
    0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 
    0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 
    0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 
    1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 
    1, 0, 0
};

// lat, lon
const float City::m_position[NUMCITY][2] = { { 0.0, 0.0 }, // NOCITY
    {55.044444, 9.418056}, {5.317, -4.033}, {9.067, 7.483}, {57.1526, -2.1100}, {5.55, -0.2}, {42.5, 1.5}, {38.433, 27.15}, {9.03, 38.74}, {-34.60333, -58.38167}, {-36.840417, 174.7398694}, 
    {57.05, 9.916667}, {43.2775, 76.89583}, {42.6525, -73.757222}, {36.7, 3.217}, {31.2, 29.916667}, {23.03, 72.58}, {31.933, 35.933}, {52.37306, 4.89222}, {61.21806, -149.8925}, {38.973056, -76.501111}, 
    {51.217, 4.4}, {17.116667, -61.85}, {-13.833, -171.75}, {64.55, 40.533333}, {37.933, 58.367}, {-15.924444, -5.718056}, {15.333, 38.933}, {-25.267, -57.667}, {37.967, 23.717}, {33.755, -84.39}, 
    {31.64, 74.86}, {12.519, -70.037}, {44.310556, -69.779444}, {24.467, 54.367}, {30.25, -97.75}, {18.220833, -63.051667}, {51.8333333, -0.8333333}, {26.217, 50.583}, {15.9958, -61.7292}, {44.4325, 26.10389}, 
    {41.41389, 2.175}, {26.368611, -80.1}, {17.25, -88.767}, {41.7625, -72.674167}, {44.82056, 20.46222}, {52.50056, 13.39889}, {33.88694, 35.51306}, {-29.1, 26.217}, {54.597, -5.93}, {4.367, 18.583}, 
    {13.106, -59.632}, {60.38944, 5.33}, {33.325, 44.422}, {45.695, 9.67}, {52.4796, -1.9030}, {43.25694, -2.92361}, {46.808333, -100.783611}, {13.45306, -16.5775}, {-3.383, 29.367}, {39.90639, 116.37972}, 
    {13.75222, 100.49389}, {12.65, -8}, {44.493889, 11.342778}, {12.967, 77.567}, {-15.78611, 35.00583}, {59.32944, 18.06861}, {36.162222, -86.774444}, {50.73389, 7.09972}, {44.78, 17.19}, {4.59806, -74.07583}, 
    {43.615833, -116.201667}, {18.975, 72.82583}, {12.1507, -68.2767}, {42.35778, -71.06167}, {53.07583, 8.8075}, {46.9524056, 7.439583}, {51.45, -2.583}, {50.85, 4.35}, {-15.8, -47.9}, {47.567, 7.6}, 
    {30.4475, -91.178611}, {48.14389, 17.10972}, {44.259444, -72.575833}, {47.47194, 19.05028}, {4.89028, 114.94222}, {-4.267, 15.283}, {34.000556, -81.034722}, {30.05806, 31.22889}, {4.9227, -52.3269}, {52.205, 0.119}, 
    {-35.30806, 149.12444}, {-12.116667, 96.9}, {10.5, -66.917}, {22.567, 88.367}, {-6.2, 106.8}, {34.767, 113.65}, {22.367, 91.8}, {-43.53, 172.62028}, {41.88194, -87.62778}, {32.786944, -79.876389}, 
    {9.50917, -13.71222}, {46.766667, 23.583333}, {35.226944, -80.843333}, {6.93444, 79.84278}, {39.962222, -83.000556}, {33.533, -7.583}, {-27.43889, -58.7675}, {43.206667, -71.538056}, {6.49722, 2.605}, {-31.416667, -64.183333}, 
    {55.67611, 12.56833}, {-33.925278, 18.423889}, {38.347222, -81.633333}, {39.164444, -119.766944}, {12.117, -68.933}, {-25.417, -49.25}, {51.483, -3.183}, {41.14, -104.820278}, {23.7, 90.375}, {33.513, 36.292}, 
    {-6.8, 39.283}, {38.895111, -77.036667}, {15.30139, -61.38833}, {28.61389, 77.20889}, {39.7392, -104.985}, {32.782778, -96.803889}, {-8.55, 125.583}, {14.69278, -17.44667}, {4.05, 9.7}, {38.92083, 121.63917}, 
    {56.464, -2.970}, {48.45, 34.983333}, {25.28667, 51.533333}, {39.158056, -75.524444}, {41.590833, -93.620833}, {53.34778, -6.25972}, {25.2697, 55.3095}, {38.53667, 68.78}, {-12.7806, 45.2278}, {49.4969, 5.9806}, 
    {-20.243889, 57.49}, {55.95306, -3.18889}, {18.43139, -64.62306}, {19.6, -99.05}, {60.205556, 24.655556}, {39.941, 32.864}, {50.820833, -0.1375}, {13.667, -89.167}, {27.15361, -13.20333}, {40.183, 44.517}, 
    {40.711417, -74.06476}, {62.01167, -6.7675}, {37.55139, -122.26639}, {14.6, -61.0833}, {38.2, -84.866667}, {-4.325, 15.32222}, {43.771389, 11.254167}, {8.48444, -13.23444}, {39.01944, 125.73806}, {39.055833, -95.689444}, 
    {50.11167, 8.68583}, {42.87472, 74.61222}, {46.783333, -56.166667}, {33.583, 130.4}, {-8.517, 179.217}, {-24.65806, 25.91222}, {23.159722, 72.684444}, {49.45, -2.533}, {19.3034, -81.3863}, {21.459, -71.139}, 
    {6.8, -58.166667}, {23.223, 72.65}, {36.143, -5.353}, {55.858, -4.259}, {12.05, -61.75}, {44.411111, 8.932778}, {64.1666667, -51.7333333}, {15.563, 73.818}, {56.333333, 44}, {57.7, 11.966667}, 
    {55.75819, 8.92617}, {-23.55, -46.633}, {37.766667, -3.771111}, {47.066667, 15.433333}, {-54.283333, -36.5}, {14.652083, -90.505972}, {13.483, 144.75}, {46.2, 6.15}, {41.038889, -73.613611}, {40.39528, 49.88222}, 
    {-2.183, -79.883}, {-11.75, 43.2}, {52.36667, 9.71667}, {53.56528, 10.00139}, {21.033333, 105.85}, {23.133, -82.383}, {60.17083, 24.9375}, {52.083333, 4.316667}, {34.39806, 132.475806}, {-9.467, 159.817}, 
    {22.27833, 114.15889}, {46.591111, -112.020278}, {21.30889, -157.82611}, {-17.86389, 31.02972}, {49.916667, 36.316667}, {55.867, 9.833}, {29.762778, -95.383056}, {22.42, 75.54}, {50.45, 30.52333}, {39.745833, -75.546667}, 
    {49.222778, 18.74}, {39.768611, -86.158056}, {-0.5477, 166.920867}, {54.14521, -4.48172}, {33.43, 73.04}, {41.01224, 28.976018}, {-19.058873, -169.875412}, {32.298889, -90.184722}, {21.543333, 39.172778}, {38.567778, -92.175833}, 
    {49.187, -2.107}, {11.588, 43.145}, {-26.20444, 28.04556}, {58.3, -134.416}, {31.783, 35.217}, {4.85, 31.6}, {34.53306, 69.16611}, {-1.94389, 30.05944}, {24.86, 67.01}, {37.916111, 139.036389}, 
    {17.983333, -76.8}, {47.0107, 28.8687}, {0.31361, 32.58111}, {46.616667, 14.3}, {50.061389, 19.938333}, {15.63306, 32.53306}, {27.7, 85.333}, {53.233333, 50.166667}, {3.1357, 101.688}, {29.36972, 47.97833}, 
    {-8.83833, 13.23444}, {42.714167, -84.56}, {34.05, -118.25}, {53.79972, -1.54917}, {5.3203222, 115.2112278}, {0.3901, 9.4544}, {34.916667, 33.633333}, {43.371491, -8.395970}, {54.9958, -7.3074}, {59.95, 30.317}, 
    {51.333333, 12.383333}, {6.137778, 1.2125}, {31.54972, 74.34361}, {47.141, 9.521}, {-12.04333, -77.02833}, {38.7138194, -9.1393861}, {34.736111, -92.331111}, {46.05556, 14.50833}, {-13.983, 33.783}, {34.674722, 33.044167}, 
    {40.809167, -96.678056}, {48.305833, 14.286389}, {51.50722, -0.1275}, {6.45306, 3.39583}, {-16.5, -68.133333}, {53.4, -2.983333}, {47.05, 8.3}, {-15.417, 28.283}, {49.6, 6.117}, {50.883, 4.7}, 
    {78.217, 15.55}, {45.759722, 4.842222}, {13.08389, 80.27}, {40.4, -3.683}, {7.067, 171.267}, {53.466667, -2.233333}, {14.55, 121.033}, {39.109722, -94.588611}, {43.73278, 7.41972}, {23.61, 58.54}, 
    {40.269722, -76.875556}, {-32.883333, -68.816667}, {-37.81361, 144.96306}, {19.433, -99.133}, {22.167, 113.55}, {12.13639, -86.25139}, {-23.4, -51.916667}, {32.361667, -86.279167}, {2.067, 45.367}, {60.1, 19.933}, 
    {25.787676, -80.224145}, {45.46417, 9.19028}, {43.05, -87.95}, {35.89778, 14.5125}, {4.175278, 73.508889}, {6.317, -10.8}, {55.583333, 13.033333}, {16.79278, -62.21056}, {14.583, 120.967}, {55.75, 37.617}, 
    {-25.967, 32.583}, {43.2964, 5.37}, {-20.167, 57.5}, {43.074722, -89.384167}, {44.983, -93.267}, {53.9, 27.567}, {-29.31, 27.48}, {48.133333, 11.566667}, {37.90611, -122.545}, {-34.88361, -56.18194}, 
    {40.845, 14.258333}, {25.06, -77.345}, {-1.283, 36.817}, {54.98, -1.61}, {12.112, 15.035}, {35.1814556, 136.9063861}, {35.167, 33.367}, {13.52139, 2.10528}, {18.1, -15.95}, {-7.3, 72.4}, 
    {-29.066667, 167.966667}, {-22.2758, 166.4580}, {-32.916667, 151.75}, {40.664167, -73.938611}, {53.143889, 8.213889}, {46.467, 30.733}, {35.468611, -97.521389}, {43.067, 141.35}, {47.037778, -122.900833}, {41.162142, -8.621953}, 
    {51.897222, -8.47}, {34.69389, 135.50222}, {59.94944, 10.75639}, {12.35722, -1.53528}, {55.017, 82.933}, {11.85, -15.567}, {8.983, -79.517}, {18.533, -72.333}, {48.8567, 2.3508}, {27.4666667, 89.64167}, 
    {5.867, -55.167}, {14.58722, 121.06111}, {40.357115, -74.670165}, {39.95, -75.17}, {33.4482, -112.0738}, {44.374722, -100.320556}, {39.566667, 2.65}, {-25.066667, -130.1}, {11.55, 104.91667}, {6.917778, 158.185}, 
    {-9.417, 147.283}, {10.667, -61.517}, {-14.27944, -170.70056}, {-17.535, -149.5696}, {50.083, 14.417}, {42.666667, 21.166667}, {-25.74611, 28.18806}, {32.22028, 35.27889}, {-51.6921, -57.8589}, {41.823611, -71.422222}, 
    {51.233, 6.783}, {45.566667, 8.066667}, {-26.317, 31.133}, {42.25, 19.85}, {45.416667, 11.866667}, {41.548592, 2.1075}, {56.457, 10.039}, {14.92083, -23.50833}, {-21.2, -159.767}, {49.016667, 12.083333}, 
    {34.033, -6.833}, {35.854167, -78.761944}, {16.8, 96.15}, {37.540833, -77.436667}, {-22.90833, -43.19639}, {56.94889, 24.10639}, {35.4275, 119.4553}, {64.1333333, -21.9333333}, {41.9, 12.5}, {7.50056, 134.62417}, 
    {-32.95056, -60.70417}, {57.183, 39.417}, {51.921667, 4.481111}, {24.633, 46.717}, {-20.8789, 55.4481}, {35.667222, -105.964444}, {15.348333, 44.206389}, {13.69, -89.19}, {15.5, -88.033333}, {17.897908, -62.850556}, 
    {45.792784, 24.152069}, {18.5, -69.983333}, {43.462778, -3.805}, {37.5665361, 126.9779694}, {-4.6167, 55.45}, {18.0731, -63.0822}, {-31.633333, -60.7}, {37.7793, -122.4192}, {10.76944, 106.68194}, {51.683333, 5.3}, 
    {31.233, 121.467}, {56.183333, 9.551667}, {1.367, 103.8}, {9.933333, -84.083333}, {43.8476, 18.3564}, {18.45, -66.066667}, {33.95, 130.933}, {17.3, -62.733}, {40.65, 22.9}, {42, 21.433333}, 
    {40.760833, -111.891111}, {44.939167, -123.039444}, {14.017, -60.983}, {43.9346, 12.4473}, {38.581667, -121.494444}, {52.6652, -8.6238}, {42.7, 23.333}, {39.798333, -89.675833}, {15.2, 145.751944}, {43.51, 16.44}, 
    {3.75, 8.783}, {41.096667, -73.552222}, {-33.43783, -70.65033}, {44.944167, -93.093611}, {48.77861, 9.17944}, {18.35, -64.95}, {-7.26528, 112.7425}, {-18.1416, 178.4419}, {13.167, -61.233}, {56.833, 60.583}, 
    {53.633333, 11.416667}, {51.616667, -3.95}, {48.5844, 7.7486}, {18.0237, -63.0458}, {-33.859972, 151.21111}, {47.8, 13.0333}, {22.55, 114.1}, {41.267, 69.217}, {41.717, 44.783}, {-21.133333, -175.2}, 
    {-49.35, 70.216667}, {42.467, 19.267}, {14.0833, -87.2167}, {35.69611, 51.42306}, {41.326, 19.816}, {32.90222, 13.18583}, {-9.16833, -171.80972}, {30.455, -84.253333}, {59.43722, 24.74528}, {43.6045, 1.444}, 
    {32.083, 34.8}, {61.5, 23.766667}, {0.33611, 6.68139}, {-18.933, 47.517}, {69.67583, 18.91944}, {63.429722, 10.393333}, {45.066667, 7.7}, {1.417, 173.033}, {25.033, 121.633}, {51.166667, 71.433333}, 
    {40.223841, -74.763624}, {-26.816667, -65.216667}, {36.8, 10.183}, {35.70056, 139.715}, {-0.25, -78.5833333}, {34.69, 135.19556}, {35.01167, 135.76833}, {47.92, 106.92}, {52.091, 5.122}, {41.904, 12.453}, 
    {-17.933333, 25.833333}, {48.208333, 16.373056}, {-20.288889, -40.308333}, {39.470239, -0.376805}, {-17.75, 168.3}, {38.9, -77.266667}, {54.683, 25.283}, {48.7, 44.516667}, {-33.05, -71.617}, {17.9666667, 102.6}, 
    {43.133, 131.9}, {46.312, 16.361}, {52.23, 21.01083}, {-22.57, 17.08361}, {-41.28889, 174.77722}, {-13.283, -176.183}, {28.596111, -81.346667}, {51.11, 17.0325}, {31.566667, 120.3}, {-10.42167, 105.67806}, 
    {0.0, 0.0}, {3.867, 11.517}, {32.293, -64.782}, {45.50889, -73.55417}, {45.417, -75.7}, {43.7165889, -79.3406861}, {49.25, -123.1}, {49.89944, -97.13917}, {51.05, -114.067}, {43.6, -79.65}, 
    {45.816667, 15.983333}, {41.65, -0.883333}, {47.367, 8.55}
};



 /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
 /* Vincenty Inverse Solution of Geodesics on the Ellipsoid (c) Chris Veness 2002-2011             */
 /*                                                                                                */
 /* from: Vincenty inverse formula - T Vincenty, "Direct and Inverse Solutions of Geodesics on the */
 /*       Ellipsoid with application of nested equations", Survey Review, vol XXII no 176, 1975    */
 /*       http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf                                             */
 /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
 
 // see https://en.wikipedia.org/wiki/Great-circle_distance
 // and http://www.movable-type.co.uk/scripts/latlong-vincenty.html
 // and https://en.wikipedia.org/wiki/Vincenty%27s_formulae

 // Table taken from GMT project. Radii in metres 
 //   Name,          Date,  Eq. rad,    Pole rad,         Flattening
 // { "WGS_84",      1984,  6378137.0,  6356752.31424518, 1.0/298.257223563 },
 // { "OSU91A",      1991,  6378136.3,  6356751.61633668, 1.0/298.25722 },
 // { "OSU86F",      1986,  6378136.2,  6356751.51667196, 1.0/298.25722 },
 // { "Engelis",     1985,  6378136.05, 6356751.32272154, 1.0/298.2566 },
 // { "SGS_85",      1985,  6378136.0,  6356751.30156878, 1.0/298.257 },
 // { "TOPEX",       1990,  6378136.3,  6356751.60056294, 1.0/298.257 },
 // { "MERIT_83",    1983,  6378137.0,  6356752.29821597, 1.0/298.257 },
 // { "GRS_80",      1980,  6378137.0,  6356752.31414036, 1.0/298.257222101 },
 // { "Hughes_1980", 1980,  6378273.0,  6356889.44820259, 1.0/298.2794 },

float
City::dist( float lat1, float lon1, float lat2, float lon2 ) 
/**
 * Calculates geodetic distance in metres between two points specified by latitude/longitude using 
 * Vincenty inverse formula for ellipsoids
 *
 * @param   {Number} lat1, lon1: first point in decimal degrees
 * @param   {Number} lat2, lon2: second point in decimal degrees
 * @returns (Number} distance in metres between points
 * 
 */
{
    const double D2R = (M_PI / 180.0); 
    const double R2D = (180.0 / M_PI); 
    
    //   { "WGS_84",  1984,  6378137.0, 6356752.31424518,  1.0/298.257223563 },
    double a = 6378137;           // theEllipsoid->eqRadius();    // ellipsoid param
    double b = 6356752.31424518;  // theEllipsoid->polRadius();   // ellipsoid param
    double f = 1.0/298.257223563; // theEllipsoid->flattening();  // ellipsoid param
    
    double L = (lon2-lon1) * D2R;
    double U1 = std::atan((1.0-f) * std::tan(lat1 * D2R));
    double U2 = std::atan((1.0-f) * std::tan(lat2 * D2R));
    double sinU1 = std::sin(U1), cosU1 = std::cos(U1);
    double sinU2 = std::sin(U2), cosU2 = std::cos(U2);
    
    double lambda = L, lambdaP;
    double cosSqAlpha;
    double sigma;
    double sinSigma;
    double cosSigma;
    double cos2SigmaM;
    int iterLimit = 5000;
    double sinLambda;
    double cosLambda;
    
    do 
    {
        sinLambda = std::sin(lambda);
        cosLambda = std::cos(lambda);
        
        sinSigma = sqrt( (cosU2 * sinLambda) * (cosU2 * sinLambda) + 
                        ((cosU1 * sinU2) - (sinU1 * cosU2 * cosLambda)) * ((cosU1 * sinU2)-(sinU1 * cosU2 * cosLambda)));
        
        if (sinSigma == 0)
            return 0;  // co-incident points
        
        cosSigma = (sinU1 * sinU2) + (cosU1 * cosU2 * cosLambda);
        sigma = atan2(sinSigma, cosSigma);
        
        double sinAlpha = (cosU1 * cosU2 * sinLambda) / sinSigma;
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        cos2SigmaM = cosSigma - ((2.0*sinU1*sinU2) / cosSqAlpha); 
        
        if (std::isnan(cos2SigmaM))  
            cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (§6)
        
        double C = f / (16.0 * cosSqAlpha * (4.0 + f * (4.0 - (3.0 * cosSqAlpha))));
        lambdaP = lambda;
        lambda = L + (1.0-C) * f * sinAlpha *
        (sigma + (C * sinSigma * (cos2SigmaM + C * cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM))));    
    } 
    while ((fabs(lambda - lambdaP) > 1e-12) && (--iterLimit > 0));
    
    if (iterLimit == 0)
        return -1.0;  // formula failed to converge
    
    double uSq = cosSqAlpha * (a * a - b * b) / (b*b);
    double A = 1.0 + uSq / 16384.0 * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq)));
    double B = uSq / (1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq))));
    double deltaSigma = B * sinSigma * (cos2SigmaM + B / 4.0 * (cosSigma * (-1.0 + 2.0 * cos2SigmaM * cos2SigmaM) -
                                                                B / 6.0 * cos2SigmaM * (-3.0 + 4.0 * sinSigma * sinSigma) * (-3.0 + 4.0 * cos2SigmaM * cos2SigmaM)));
    double s = b * A * (sigma - deltaSigma);
    
    return s;
    
    // note: to return initial/final bearings in addition to distance, use something like:
    // double fwdAz = atan2(cosU2 * sinLambda,  cosU1 * sinU2 - sinU1 * cosU2 * cosLambda);
    // double revAz = atan2(cosU1 * sinLambda, -sinU1 * cosU2 + cosU1 * sinU2 * cosLambda);
    // return Pair<double,double>( fwdAz * R2D, revAz * R2D );
}

