/* Country 04/01/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Country.cpp - code   $
 $$$$$$$$$$$$$$$%$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates
 History: Supports the full ISO 3166-1 Country Code List 	   
     
 ISO 2 and 3 letter codes supported.
 
 Notes 
 
 (i)   User-assigned code elements are codes at the disposal of users who need to add further names of countries, 
       territories, or other geographical entities to their in-house application of ISO 3166-1, and the ISO 3166/MA 
       will never use these codes in the updating process of the standard. 
       The following alpha-2 codes can be user-assigned: AA, QM to QZ, XA to XZ, and ZZ.
 (ii)  there are no ISO codes for Kosovo at the time of writting
 (iii) there are no ISO codes for Zaire  at the time of writting
 (vi)  Netherlands Antilles (AN, ANT) is not a country - ceased to exist 10/10/2010 
 
 https://www.iso.org/iso/country_codes
 https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2
 https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
 
 TODO: could add 'UNK' - identifies Kosovo residents issued travel documents by United Nations Interim Administration in Kosovo (UNMIK)
 */


#ifndef __COUNTRY_H__
#include "Country.h"
#endif

#include <cassert>


std::ostream&
operator<<( std::ostream &ostr, const Country &c )
{
	ostr << c.to3Code();
	return ostr;
}

std::istream&
operator>>( std::istream &istr, Country &c )
{
	std::string str;
	istr >> str;
	c.setCountry( str );
	return istr;
}

//
//
//

// this speeds up setCountry quite a bit
const short Country::m_searchPoints[27] = {
    1, 34, 76, 116, 128, 144, 156, 193, 205, 225, 233, 252, 274, 321, 345, 347, 375, 377, 386, 431, 462, 474, 488, 492, 502, 505, 511
};

bool
Country::setCountry( const std::string &str )
// https://en.wikipedia.org/wiki/Binary_search_algorithm
{	
    
    const int len = (int) str.size();
    
    //assert(str.size() == 3 || str.size() == 4);
    if (len != 3 && len != 2)
	{
        m_country = Country::XXX; // NOCOUNTRY
		return false;
	}
    
    int index = str[0] - 'A'; // 'A' = 65;
    //assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_country = Country::XXX;
        return false;
    }
    
	int low   = m_searchPoints[index]; 
	int high  = m_searchPoints[index + 1]; 
    int i;
    
	while (low < high) 
	{
        int mid = low + ((high - low) >> 1);
        const char * const cty = m_countryCodes[mid];

        for (i = 1; i < len; ++i)
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
        
        if (i == len)
        {
            m_country = m_toISO[mid]; 
			return true;
        }
	}

    m_country = Country::XXX; // NOCOUNTRY
	return false;
}


// if you add countries make sure you add the names in the correct alphabetic order position
// or else the binary chop search in setCountry(std::string) won't work
const char * const Country::m_country3Codes[NUMCOUNTRY] = { "NOCOUNTRY", 
    "ABW", "AFG", "AGO", "AIA", "ALA", "ALB", "AND", "ARE", "ARG", "ARM", 
    "ASM", "ATA", "ATF", "ATG", "AUS", "AUT", "AZE", "BDI", "BEL", "BEN", 
    "BES", "BFA", "BGD", "BGR", "BHR", "BHS", "BIH", "BLM", "BLR", "BLZ", 
    "BMU", "BOL", "BRA", "BRB", "BRN", "BTN", "BVT", "BWA", "CAF", "CAN", 
    "CCK", "CHE", "CHL", "CHN", "CIV", "CMR", "COD", "COG", "COK", "COL", 
    "COM", "CPV", "CRI", "CUB", "CUW", "CXR", "CYM", "CYP", "CZE", "DEU", 
    "DJI", "DMA", "DNK", "DOM", "DZA", "ECU", "EGY", "ERI", "ESH", "ESP", 
    "EST", "ETH", "EUR", "FIN", "FJI", "FLK", "FRA", "FRO", "FSM", "GAB", 
    "GBR", "GEO", "GGY", "GHA", "GIB", "GIN", "GLP", "GMB", "GNB", "GNQ", 
    "GRC", "GRD", "GRL", "GTM", "GUF", "GUM", "GUY", "HKG", "HMD", "HND", 
    "HRV", "HTI", "HUN", "IDN", "IMN", "IND", "IOT", "IRL", "IRN", "IRQ", 
    "ISL", "ISR", "ITA", "JAM", "JEY", "JOR", "JPN", "KAZ", "KEN", "KGZ", 
    "KHM", "KIR", "KNA", "KOR", "KWT", "LAO", "LBN", "LBR", "LBY", "LCA", 
    "LIE", "LKA", "LSO", "LTU", "LUX", "LVA", "MAC", "MAF", "MAR", "MCO", 
    "MDA", "MDG", "MDV", "MEX", "MHL", "MKD", "MLI", "MLT", "MMR", "MNE", 
    "MNG", "MNP", "MOZ", "MRT", "MSR", "MTQ", "MUS", "MWI", "MYS", "MYT", 
    "NAM", "NCL", "NER", "NFK", "NGA", "NIC", "NIU", "NLD", "NOR", "NPL", 
    "NRU", "NZL", "OMN", "PAK", "PAN", "PCN", "PER", "PHL", "PLW", "PNG", 
    "POL", "PRI", "PRK", "PRT", "PRY", "PSE", "PYF", "QAT", "REU", "ROU", 
    "RUS", "RWA", "SAU", "SDN", "SEN", "SGP", "SGS", "SHN", "SJM", "SLB", 
    "SLE", "SLV", "SMR", "SOM", "SPM", "SRB", "SSD", "STP", "SUR", "SVK", 
    "SVN", "SWE", "SWZ", "SXM", "SYC", "SYR", "TCA", "TCD", "TGO", "THA", 
    "TJK", "TKL", "TKM", "TLS", "TON", "TTO", "TUN", "TUR", "TUV", "TWN", 
    "TZA", "UGA", "UKR", "UMI", "URY", "USA", "UZB", "VAT", "VCT", "VEN", 
    "VGB", "VIR", "VNM", "VUT", "WLF", "WSM", "XAF", "XCD", "XOF", "XPF", 
    "XXX", "YEM", "ZAF", "ZMB", "ZWE"
};

// in country3code order
const short Country::m_3toISO[NUMCOUNTRY] = {  NOCOUNTRY, 
    ABW, AFG, AGO, AIA, ALA, ALB, AND, ARE, ARG, ARM,
    ASM, ATA, ATF, ATG, AUS, AUT, AZE, BDI, BEL, BEN,
    BES, BFA, BGD, BGR, BHR, BHS, BIH, BLM, BLR, BLZ,
    BMU, BOL, BRA, BRB, BRN, BTN, BVT, BWA, CAF, CAN,
    CCK, CHE, CHL, CHN, CIV, CMR, COD, COG, COK, COL,
    COM, CPV, CRI, CUB, CUW, CXR, CYM, CYP, CZE, DEU,
    DJI, DMA, DNK, DOM, DZA, ECU, EGY, ERI, ESH, ESP,
    EST, ETH, EUR, FIN, FJI, FLK, FRA, FRO, FSM, GAB,
    GBR, GEO, GGY, GHA, GIB, GIN, GLP, GMB, GNB, GNQ,
    GRC, GRD, GRL, GTM, GUF, GUM, GUY, HKG, HMD, HND,
    HRV, HTI, HUN, IDN, IMN, IND, IOT, IRL, IRN, IRQ,
    ISL, ISR, ITA, JAM, JEY, JOR, JPN, KAZ, KEN, KGZ,
    KHM, KIR, KNA, KOR, KWT, LAO, LBN, LBR, LBY, LCA,
    LIE, LKA, LSO, LTU, LUX, LVA, MAC, MAF, MAR, MCO,
    MDA, MDG, MDV, MEX, MHL, MKD, MLI, MLT, MMR, MNE,
    MNG, MNP, MOZ, MRT, MSR, MTQ, MUS, MWI, MYS, MYT,
    NAM, NCL, NER, NFK, NGA, NIC, NIU, NLD, NOR, NPL,
    NRU, NZL, OMN, PAK, PAN, PCN, PER, PHL, PLW, PNG,
    POL, PRI, PRK, PRT, PRY, PSE, PYF, QAT, REU, ROU,
    RUS, RWA, SAU, SDN, SEN, SGP, SGS, SHN, SJM, SLB,
    SLE, SLV, SMR, SOM, SPM, SRB, SSD, STP, SUR, SVK,
    SVN, SWE, SWZ, SXM, SYC, SYR, TCA, TCD, TGO, THA,
    TJK, TKL, TKM, TLS, TON, TTO, TUN, TUR, TUV, TWN,
    TZA, UGA, UKR, UMI, URY, USA, UZB, VAT, VCT, VEN,
    VGB, VIR, VNM, VUT, WLF, WSM, XAF, XCD, XOF, XPF,
    XXX, YEM, ZAF, ZMB, ZWE
};

// Note country2codes here in country3code order i.e TF/ATF or  GS/SGS
const char * const Country::m_country2Codes[NUMCOUNTRY] = { "NOCOUNTRY", 
    "AW", "AF", "AO", "AI", "AX", "AL", "AD", "AE", "AR", "AM", 
    "AS", "AQ", "TF", "AG", "AU", "AT", "AZ", "BI", "BE", "BJ", 
    "BQ", "BF", "BD", "BG", "BH", "BS", "BA", "BL", "BY", "BZ", 
    "BM", "BO", "BR", "BB", "BN", "BT", "BV", "BW", "CF", "CA", 
    "CC", "CH", "CL", "CN", "CI", "CM", "CD", "CG", "CK", "CO", 
    "KM", "CV", "CR", "CU", "CW", "CX", "KY", "CY", "CZ", "DE", 
    "DJ", "DM", "DK", "DO", "DZ", "EC", "EG", "ER", "EH", "ES", 
    "EE", "ET", "EZ", "FI", "FJ", "FK", "FR", "FO", "FM", "GA", 
    "GB", "GE", "GG", "GH", "GI", "GN", "GP", "GM", "GW", "GQ", 
    "GR", "GD", "GL", "GT", "GF", "GU", "GY", "HK", "HM", "HN", 
    "HR", "HT", "HU", "ID", "IM", "IN", "IO", "IE", "IR", "IQ", 
    "IS", "IL", "IT", "JM", "JE", "JO", "JP", "KZ", "KE", "KG", 
    "KH", "KI", "KN", "KR", "KW", "LA", "LB", "LR", "LY", "LC", 
    "LI", "LK", "LS", "LT", "LU", "LV", "MO", "MF", "MA", "MC", 
    "MD", "MG", "MV", "MX", "MH", "MK", "ML", "MT", "MM", "ME", 
    "MN", "MP", "MZ", "MR", "MS", "MQ", "MU", "MW", "MY", "YT", 
    "NA", "NC", "NE", "NF", "NG", "NI", "NU", "NL", "NO", "NP", 
    "NR", "NZ", "OM", "PK", "PA", "PN", "PE", "PH", "PW", "PG", 
    "PL", "PR", "KP", "PT", "PY", "PS", "PF", "QA", "RE", "RO", 
    "RU", "RW", "SA", "SD", "SN", "SG", "GS", "SH", "SJ", "SB", 
    "SL", "SV", "SM", "SO", "PM", "RS", "SS", "ST", "SR", "SK", 
    "SI", "SE", "SZ", "SX", "SC", "SY", "TC", "TD", "TG", "TH", 
    "TJ", "TK", "TM", "TL", "TO", "TT", "TN", "TR", "TV", "TW", 
    "TZ", "UG", "UA", "UM", "UY", "US", "UZ", "VA", "VC", "VE", 
    "VG", "VI", "VN", "VU", "WF", "WS", "XA", "XC", "XO", "XP", 
    "XX", "YE", "ZA", "ZM", "ZW"
};

// country3code order
const char * const Country::m_fullCountryNames[NUMCOUNTRY] = { "No Country",
    "Aruba", "Afghanistan", "Angola", "Anguilla", "Aland Islands", "Albania", "Andorra", "United Arab Emirates", "Argentina", "Armenia", 
    "American Samoa", "Antarctica", "French Southern Territories", "Antigua and Barbuda", "Australia", "Austria", "Azerbaijan", "Burundi", "Belgium", "Benin", 
    "Bonaire, Saint Eustatius and Saba", "Burkina Faso", "Bangladesh", "Bulgaria", "Bahrain", "Bahamas", "Bosnia and Herzegovina", "Saint Barthelemy", "Belarus", "Belize", 
    "Bermuda", "Bolivia", "Brazil", "Barbados", "Brunei Darussalam", "Bhutan", "Bouvet Island", "Botswana", "Central African Republic", "Canada", 
    "Cocos Islands", "Switzerland", "Chile", "China", "Ivory Coast", "Cameroon", "Democratic Republic of the Congo", "Republic of the Congo", "Cook Islands", "Colombia", 
    "Comoros", "Cape Verde", "Costa Rica", "Cuba", "Curacao", "Christmas Island", "Cayman Islands", "Cyprus", "Czech Republic", "Germany", 
    "Djibouti", "Dominica", "Denmark", "Dominican Republic", "Algeria", "Ecuador", "Egypt", "Eritrea", "Western Sahara", "Spain", 
    "Estonia", "Ethiopia", "Euro Zone", "Finland", "Fiji", "Falkland Islands", "France", "Faroe Islands", "Federated States of Micronesia", "Gabon", 
    "United Kingdom", "Georgia", "Guernsey", "Ghana", "Gibraltar", "Guinea", "Guadeloupe", "Gambia", "Guinea-Bissau", "Equatorial Guinea", 
    "Greece", "Grenada", "Greenland", "Guatemala", "French Guiana", "Guam", "Guyana", "Hong Kong", "Heard and McDonald Islands", "Honduras", 
    "Croatia", "Haiti", "Hungary", "Indonesia", "Isle of Man", "India", "British Indian Ocean Territory", "Ireland", "Iran", "Iraq", 
    "Iceland", "Israel", "Italy", "Jamaica", "Jersey", "Jordan", "Japan", "Kazakhstan", "Kenya", "Kyrgyzstan", 
    "Cambodia", "Kiribati", "Saint Kitts and Nevis", "South Korea", "Kuwait", "Laos", "Lebanon", "Liberia", "Libya", "Saint Lucia", 
    "Liechtenstein", "Sri Lanka", "Lesotho", "Lithuania", "Luxembourg", "Latvia", "Macao", "Saint Martin", "Morocco", "Monaco", 
    "Moldova", "Madagascar", "Maldives", "Mexico", "Marshall Islands", "North Macedonia", "Mali", "Malta", "Myanmar", "Montenegro", 
    "Mongolia", "Northern Mariana Islands", "Mozambique", "Mauritania", "Montserrat", "Martinique", "Mauritius", "Malawi", "Malaysia", "Mayotte", 
    "Namibia", "New Caledonia", "Niger", "Norfolk Island", "Nigeria", "Nicaragua", "Niue", "Netherlands", "Norway", "Nepal", 
    "Nauru", "New Zealand", "Oman", "Pakistan", "Panama", "Pitcairn", "Peru", "Philippines", "Palau", "Papua New Guinea", 
    "Poland", "Puerto Rico", "North Korea", "Portugal", "Paraguay", "Palestinian Occupied Territory", "French Polynesia", "Qatar", "Reunion", "Romania", 
    "Russian Federation", "Rwanda", "Saudi Arabia", "Sudan", "Senegal", "Singapore", "South Georgia and the South Sandwich Islands", "Saint Helena, Ascension and Tristan da Cunha", "Svalbard and Jan Mayen", "Solomon Islands", 
    "Sierra Leone", "El Salvador", "San Marino", "Somalia", "Saint Pierre and Miquelon", "Serbia", "South Sudan", "Sao Tome and Principe", "Suriname", "Slovakia", 
    "Slovenia", "Sweden", "Swaziland", "Sint Maarten", "Seychelles", "Syria", "Turks and Caicos Islands", "Chad", "Togo", "Thailand", 
    "Tajikistan", "Tokelau", "Turkmenistan", "East Timor", "Tonga", "Trinidad and Tobago", "Tunisia", "Turkey", "Tuvalu", "Taiwan", 
    "Tanzania", "Uganda", "Ukraine", "United States Minor Outlying Islands", "Uruguay", "United States", "Uzbekistan", "Vatican City", "Saint Vincent and the Grenadines", "Venezuela", 
    "British Virgin Islands", "United States Virgin Islands", "Viet Nam", "Vanuatu", "Wallis and Futuna", "Samoa", "Communaute Financiere Africaine (BEAC)", "East Caribbean", "Communaute Financiere Africaine (BCEAO)", "Comptoirs Francais du Pacifique", 
    "No Country", "Yemen", "South Africa", "Zambia", "Zimbabwe"
};

// country3code order index
const short Country::m_fromISO[MAXCOUNTRY] = 
{
    0, 0, 0, 0, 2, 0, 0, 0, 6, 0, 
    12, 0, 65, 0, 0, 0, 11, 0, 0, 0, 
    7, 0, 0, 0, 3, 0, 0, 0, 14, 0, 
    0, 17, 9, 0, 0, 0, 15, 0, 0, 0, 
    16, 0, 0, 0, 26, 0, 0, 0, 25, 0, 
    23, 10, 34, 0, 0, 0, 19, 0, 0, 0, 
    31, 0, 0, 0, 36, 0, 0, 0, 32, 0, 
    27, 0, 38, 0, 37, 0, 33, 0, 0, 0, 
    0, 0, 0, 0, 30, 0, 107, 0, 0, 0, 
    200, 0, 241, 0, 0, 0, 35, 0, 0, 0, 
    24, 0, 0, 0, 149, 0, 0, 0, 18, 0, 
    0, 0, 29, 0, 0, 0, 121, 0, 0, 0, 
    46, 0, 0, 0, 40, 0, 0, 0, 0, 0, 
    0, 0, 52, 0, 0, 0, 57, 0, 0, 0, 
    39, 0, 0, 0, 132, 0, 0, 0, 218, 0, 
    0, 0, 43, 0, 0, 0, 44, 0, 230, 0, 
    0, 0, 56, 0, 0, 0, 41, 0, 0, 0, 
    50, 0, 0, 0, 51, 160, 0, 0, 48, 0, 
    47, 0, 0, 0, 49, 0, 0, 0, 53, 0, 
    0, 101, 54, 0, 0, 0, 58, 0, 0, 0, 
    0, 0, 0, 59, 20, 0, 0, 0, 63, 0, 
    0, 0, 62, 0, 64, 0, 0, 0, 66, 0, 
    0, 0, 202, 0, 0, 0, 90, 0, 0, 0, 
    0, 72, 68, 71, 78, 0, 0, 0, 76, 197, 
    0, 0, 75, 0, 0, 0, 74, 0, 5, 0, 
    77, 0, 0, 0, 95, 0, 0, 0, 187, 0, 
    13, 0, 61, 0, 0, 0, 80, 0, 82, 0, 
    88, 0, 0, 0, 0, 186, 60, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 
    0, 0, 85, 0, 0, 0, 122, 0, 0, 0, 
    91, 0, 0, 0, 93, 0, 0, 0, 92, 0, 
    0, 0, 87, 0, 0, 0, 96, 0, 0, 0, 
    94, 0, 0, 0, 86, 0, 0, 0, 97, 0, 
    0, 0, 102, 0, 99, 0, 238, 0, 0, 0, 
    100, 0, 0, 0, 98, 0, 0, 0, 103, 0, 
    0, 0, 111, 0, 0, 0, 106, 0, 0, 0, 
    104, 0, 0, 0, 109, 0, 0, 0, 110, 0, 
    0, 0, 108, 0, 0, 0, 112, 0, 0, 0, 
    113, 0, 0, 0, 45, 0, 0, 0, 114, 0, 
    0, 0, 117, 0, 0, 0, 0, 0, 118, 0, 
    116, 0, 0, 0, 119, 0, 0, 0, 183, 0, 
    124, 0, 0, 0, 125, 0, 0, 120, 126, 0, 
    0, 0, 127, 0, 0, 0, 133, 0, 136, 0, 
    128, 0, 0, 0, 129, 0, 0, 0, 131, 0, 
    134, 0, 135, 0, 0, 0, 137, 0, 0, 0, 
    142, 0, 0, 0, 158, 0, 0, 0, 159, 0, 
    0, 0, 143, 0, 0, 0, 147, 0, 0, 0, 
    148, 0, 0, 0, 156, 0, 0, 0, 154, 0, 
    157, 0, 0, 0, 144, 0, 0, 0, 0, 0, 
    0, 0, 140, 0, 0, 0, 151, 0, 141, 150, 
    155, 0, 0, 0, 139, 0, 0, 0, 153, 0, 
    0, 0, 173, 0, 0, 0, 161, 0, 0, 0, 
    171, 0, 0, 0, 170, 0, 0, 0, 168, 0, 
    0, 55, 0, 1, 214, 21, 0, 0, 0, 0, 
    162, 0, 0, 0, 0, 0, 0, 0, 244, 0, 
    0, 0, 0, 0, 172, 0, 0, 0, 166, 0, 
    0, 0, 163, 0, 0, 0, 165, 0, 0, 0, 
    167, 0, 0, 0, 164, 0, 0, 0, 169, 0, 
    152, 234, 0, 79, 145, 179, 174, 0, 0, 0, 
    0, 175, 0, 0, 0, 0, 0, 0, 180, 0, 
    185, 0, 0, 0, 177, 0, 0, 0, 178, 0, 
    0, 0, 176, 0, 0, 0, 181, 0, 0, 0, 
    184, 0, 0, 0, 89, 0, 224, 0, 0, 0, 
    182, 0, 0, 0, 188, 0, 0, 0, 189, 0, 
    0, 0, 190, 191, 0, 0, 192, 0, 0, 0, 
    0, 0, 28, 0, 198, 0, 0, 0, 0, 123, 
    4, 0, 130, 138, 0, 0, 205, 0, 0, 0, 
    239, 0, 0, 0, 203, 0, 0, 0, 208, 0, 
    0, 0, 193, 0, 0, 0, 195, 0, 206, 0, 
    215, 0, 0, 0, 201, 0, 0, 0, 0, 0, 
    0, 0, 196, 210, 243, 211, 204, 0, 0, 0, 
    253, 0, 0, 0, 0, 0, 255, 0, 0, 0, 
    0, 0, 0, 0, 70, 0, 0, 0, 207, 194, 
    0, 0, 69, 0, 0, 0, 0, 0, 0, 0, 
    209, 0, 0, 0, 199, 0, 0, 0, 213, 0, 
    0, 0, 212, 0, 0, 0, 42, 0, 0, 0, 
    216, 0, 221, 0, 220, 0, 0, 0, 219, 0, 
    0, 0, 222, 0, 0, 0, 225, 0, 0, 0, 
    226, 0, 0, 0, 8, 0, 0, 0, 227, 0, 
    0, 0, 228, 0, 0, 223, 217, 0, 229, 0, 
    232, 0, 0, 0, 233, 0, 0, 146, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 67, 0, 
    0, 0, 0, 0, 0, 0, 81, 0, 0, 0, 
    0, 83, 115, 105, 231, 0, 0, 0, 0, 0, 
    236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    242, 0, 0, 0, 22, 0, 0, 0, 235, 0, 
    237, 0, 240, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 245, 0, 0, 0, 
    0, 0, 246, 0, 0, 0, 0, 252, 0, 0, 
    0, 0, 0, 0, 254, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 73, 247, 248, 249, 250, 251
};

const char * const Country::m_countryCodes[2*NUMCOUNTRY-1] = { "NOCOUNTRY",  // we only have one 'NOCOUNTRY'
    "ABW", "AD", "AE", "AF", "AFG", "AG", "AGO", "AI", "AIA", "AL", "ALA", "ALB", "AM", "AND", "AO", "AQ", "AR", "ARE", "ARG", 
    "ARM", "AS", "ASM", "AT", "ATA", "ATF", "ATG", "AU", "AUS", "AUT", "AW", "AX", "AZ", "AZE", "BA", "BB", "BD", "BDI", "BE", "BEL", 
    "BEN", "BES", "BF", "BFA", "BG", "BGD", "BGR", "BH", "BHR", "BHS", "BI", "BIH", "BJ", "BL", "BLM", "BLR", "BLZ", "BM", "BMU", "BN", 
    "BO", "BOL", "BQ", "BR", "BRA", "BRB", "BRN", "BS", "BT", "BTN", "BV", "BVT", "BW", "BWA", "BY", "BZ", "CA", "CAF", "CAN", "CC", 
    "CCK", "CD", "CF", "CG", "CH", "CHE", "CHL", "CHN", "CI", "CIV", "CK", "CL", "CM", "CMR", "CN", "CO", "COD", "COG", "COK", "COL", 
    "COM", "CPV", "CR", "CRI", "CU", "CUB", "CUW", "CV", "CW", "CX", "CXR", "CY", "CYM", "CYP", "CZ", "CZE", "DE", "DEU", "DJ", "DJI", 
    "DK", "DM", "DMA", "DNK", "DO", "DOM", "DZ", "DZA", "EC", "ECU", "EE", "EG", "EGY", "EH", "ER", "ERI", "ES", "ESH", "ESP", "EST", 
    "ET", "ETH", "EUR", "EZ", "FI", "FIN", "FJ", "FJI", "FK", "FLK", "FM", "FO", "FR", "FRA", "FRO", "FSM", "GA", "GAB", "GB", "GBR", 
    "GD", "GE", "GEO", "GF", "GG", "GGY", "GH", "GHA", "GI", "GIB", "GIN", "GL", "GLP", "GM", "GMB", "GN", "GNB", "GNQ", "GP", "GQ", 
    "GR", "GRC", "GRD", "GRL", "GS", "GT", "GTM", "GU", "GUF", "GUM", "GUY", "GW", "GY", "HK", "HKG", "HM", "HMD", "HN", "HND", "HR", 
    "HRV", "HT", "HTI", "HU", "HUN", "ID", "IDN", "IE", "IL", "IM", "IMN", "IN", "IND", "IO", "IOT", "IQ", "IR", "IRL", "IRN", "IRQ", 
    "IS", "ISL", "ISR", "IT", "ITA", "JAM", "JE", "JEY", "JM", "JO", "JOR", "JP", "JPN", "KAZ", "KE", "KEN", "KG", "KGZ", "KH", "KHM", 
    "KI", "KIR", "KM", "KN", "KNA", "KOR", "KP", "KR", "KW", "KWT", "KY", "KZ", "LA", "LAO", "LB", "LBN", "LBR", "LBY", "LC", "LCA", 
    "LI", "LIE", "LK", "LKA", "LR", "LS", "LSO", "LT", "LTU", "LU", "LUX", "LV", "LVA", "LY", "MA", "MAC", "MAF", "MAR", "MC", "MCO", 
    "MD", "MDA", "MDG", "MDV", "ME", "MEX", "MF", "MG", "MH", "MHL", "MK", "MKD", "ML", "MLI", "MLT", "MM", "MMR", "MN", "MNE", "MNG", 
    "MNP", "MO", "MOZ", "MP", "MQ", "MR", "MRT", "MS", "MSR", "MT", "MTQ", "MU", "MUS", "MV", "MW", "MWI", "MX", "MY", "MYS", "MYT", 
    "MZ", "NA", "NAM", "NC", "NCL", "NE", "NER", "NF", "NFK", "NG", "NGA", "NI", "NIC", "NIU", "NL", "NLD", "NO", "NOR", "NP", "NPL", 
    "NR", "NRU", "NU", "NZ", "NZL", "OM", "OMN", "PA", "PAK", "PAN", "PCN", "PE", "PER", "PF", "PG", "PH", "PHL", "PK", "PL", "PLW", 
    "PM", "PN", "PNG", "POL", "PR", "PRI", "PRK", "PRT", "PRY", "PS", "PSE", "PT", "PW", "PY", "PYF", "QA", "QAT", "RE", "REU", "RO", 
    "ROU", "RS", "RU", "RUS", "RW", "RWA", "SA", "SAU", "SB", "SC", "SD", "SDN", "SE", "SEN", "SG", "SGP", "SGS", "SH", "SHN", "SI", 
    "SJ", "SJM", "SK", "SL", "SLB", "SLE", "SLV", "SM", "SMR", "SN", "SO", "SOM", "SPM", "SR", "SRB", "SS", "SSD", "ST", "STP", "SUR", 
    "SV", "SVK", "SVN", "SWE", "SWZ", "SX", "SXM", "SY", "SYC", "SYR", "SZ", "TC", "TCA", "TCD", "TD", "TF", "TG", "TGO", "TH", "THA", 
    "TJ", "TJK", "TK", "TKL", "TKM", "TL", "TLS", "TM", "TN", "TO", "TON", "TR", "TT", "TTO", "TUN", "TUR", "TUV", "TV", "TW", "TWN", 
    "TZ", "TZA", "UA", "UG", "UGA", "UKR", "UM", "UMI", "URY", "US", "USA", "UY", "UZ", "UZB", "VA", "VAT", "VC", "VCT", "VE", "VEN", 
    "VG", "VGB", "VI", "VIR", "VN", "VNM", "VU", "VUT", "WF", "WLF", "WS", "WSM", "XA", "XAF", "XC", "XCD", "XO", "XOF", "XP", "XPF", 
    "XX", "XXX", "YE", "YEM", "YT", "ZA", "ZAF", "ZM", "ZMB", "ZW", "ZWE"
};

const short Country::m_toISO[2*NUMCOUNTRY-1] = { NOCOUNTRY,  // we only have one 'NOCOUNTRY'
    ABW, AD, AE, AF, AFG, AG, AGO, AI, AIA, AL, ALA, ALB, AM, AND, AO, AQ, AR, ARE, ARG, 
    ARM, AS, ASM, AT, ATA, ATF, ATG, AU, AUS, AUT, AW, AX, AZ, AZE, BA, BB, BD, BDI, BE, BEL, 
    BEN, BES, BF, BFA, BG, BGD, BGR, BH, BHR, BHS, BI, BIH, BJ, BL, BLM, BLR, BLZ, BM, BMU, BN, 
    BO, BOL, BQ, BR, BRA, BRB, BRN, BS, BT, BTN, BV, BVT, BW, BWA, BY, BZ, CA, CAF, CAN, CC, 
    CCK, CD, CF, CG, CH, CHE, CHL, CHN, CI, CIV, CK, CL, CM, CMR, CN, CO, COD, COG, COK, COL, 
    COM, CPV, CR, CRI, CU, CUB, CUW, CV, CW, CX, CXR, CY, CYM, CYP, CZ, CZE, DE, DEU, DJ, DJI, 
    DK, DM, DMA, DNK, DO, DOM, DZ, DZA, EC, ECU, EE, EG, EGY, EH, ER, ERI, ES, ESH, ESP, EST, 
    ET, ETH, EUR, EZ, FI, FIN, FJ, FJI, FK, FLK, FM, FO, FR, FRA, FRO, FSM, GA, GAB, GB, GBR, 
    GD, GE, GEO, GF, GG, GGY, GH, GHA, GI, GIB, GIN, GL, GLP, GM, GMB, GN, GNB, GNQ, GP, GQ, 
    GR, GRC, GRD, GRL, GS, GT, GTM, GU, GUF, GUM, GUY, GW, GY, HK, HKG, HM, HMD, HN, HND, HR, 
    HRV, HT, HTI, HU, HUN, ID, IDN, IE, IL, IM, IMN, IN, IND, IO, IOT, IQ, IR, IRL, IRN, IRQ, 
    IS, ISL, ISR, IT, ITA, JAM, JE, JEY, JM, JO, JOR, JP, JPN, KAZ, KE, KEN, KG, KGZ, KH, KHM, 
    KI, KIR, KM, KN, KNA, KOR, KP, KR, KW, KWT, KY, KZ, LA, LAO, LB, LBN, LBR, LBY, LC, LCA, 
    LI, LIE, LK, LKA, LR, LS, LSO, LT, LTU, LU, LUX, LV, LVA, LY, MA, MAC, MAF, MAR, MC, MCO, 
    MD, MDA, MDG, MDV, ME, MEX, MF, MG, MH, MHL, MK, MKD, ML, MLI, MLT, MM, MMR, MN, MNE, MNG, 
    MNP, MO, MOZ, MP, MQ, MR, MRT, MS, MSR, MT, MTQ, MU, MUS, MV, MW, MWI, MX, MY, MYS, MYT, 
    MZ, NA, NAM, NC, NCL, NE, NER, NF, NFK, NG, NGA, NI, NIC, NIU, NL, NLD, NO, NOR, NP, NPL, 
    NR, NRU, NU, NZ, NZL, OM, OMN, PA, PAK, PAN, PCN, PE, PER, PF, PG, PH, PHL, PK, PL, PLW, 
    PM, PN, PNG, POL, PR, PRI, PRK, PRT, PRY, PS, PSE, PT, PW, PY, PYF, QA, QAT, RE, REU, RO, 
    ROU, RS, RU, RUS, RW, RWA, SA, SAU, SB, SC, SD, SDN, SE, SEN, SG, SGP, SGS, SH, SHN, SI, 
    SJ, SJM, SK, SL, SLB, SLE, SLV, SM, SMR, SN, SO, SOM, SPM, SR, SRB, SS, SSD, ST, STP, SUR, 
    SV, SVK, SVN, SWE, SWZ, SX, SXM, SY, SYC, SYR, SZ, TC, TCA, TCD, TD, TF, TG, TGO, TH, THA, 
    TJ, TJK, TK, TKL, TKM, TL, TLS, TM, TN, TO, TON, TR, TT, TTO, TUN, TUR, TUV, TV, TW, TWN, 
    TZ, TZA, UA, UG, UGA, UKR, UM, UMI, URY, US, USA, UY, UZ, UZB, VA, VAT, VC, VCT, VE, VEN, 
    VG, VGB, VI, VIR, VN, VNM, VU, VUT, WF, WLF, WS, WSM, XA, XAF, XC, XCD, XO, XOF, XP, XPF, 
    XX, XXX, YE, YEM, YT, ZA, ZAF, ZM, ZMB, ZW, ZWE
};

//
//
//	






