/* Country 04/01/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Country.cpp - code   $
 $$$$$$$$$$$$$$$%$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates
 History: Supports the full ISO 3166-1 Country Code List 	   
          
 Region and sub region codes are taken from

 "Standard Country or Area Codes for Statistical Use, Revision 4 (United Nations publication, Sales No. 98.XVII.9"
 
 **** Updated 10/10/2023 ****
 
 */

#ifndef __COUNTRY_H__
#include "Country.h"
#endif

#include <assert.h>


std::ostream&
operator<<( std::ostream& ostr, const Country& c )
{
    ostr << c.toString();
    return ostr;
}

std::istream&
operator>>( std::istream& istr, Country& c )
{
    std::string str;
    istr >> str;
    c.setCountry( str );
    return istr;
}

///

std::string
Country::name( void ) const 
{
    return m_fullCountryNames[m_fromISO[m_country]];	
}




// this speeds up setCountry quite a bit
const short Country::m_searchPoints[27] =
{
// A,  B,  C,  D,  E,  F,  G,  H,  I,  J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X    Y,   Z,   Z + 1
   1, 18, 39, 60, 66, 74, 80, 98, 104, 114, 118, 126, 137, 161, 173, 174, 188, 189, 193, 217, 232, 238, 245, 247, 252, 253, 256  
};



bool
Country::setCountry( const std::string& str )
{	
    //assert(str.size() == 3);
    if (str.size() != 3)
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
        const char * const cty = m_countryNames[mid];
        
        for (i = 1; i < 3; ++i)
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
        
        if (i == 3)
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
const char * const Country::m_countryNames[NUMCOUNTRY] = { 
    "NOCOUNTRY",
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
	"XXX", "YEM", "ZAF", "ZMB", "ZWE" };

// ISO standard country name
const char * const Country::m_fullCountryNames[NUMCOUNTRY] = { "No Country",
	"Aruba", "Afghanistan", "Angola", "Anguilla", "Aland Islands", "Albania", "Andorra", "United Arab Emirates", "Argentina", "Armenia", 
	"American Samoa", "Antarctica", "French Southern Territories", "Antigua and Barbuda", "Australia", "Austria", "Azerbaijan", "Burundi", "Belgium", "Benin", 
	"Bonaire, Saint Eustatius and Saba", "Burkina Faso", "Bangladesh", "Bulgaria", "Bahrain", "Bahamas", "Bosnia and Herzegovina", "Saint Barthelemy", "Belarus", "Belize", 
	"Bermuda", "Bolivia", "Brazil", "Barbados", "Brunei Darussalam", "Bhutan", "Bouvet Island", "Botswana", "Central African Republic", "Canada", 
	"Cocos Islands", "Switzerland", "Chile", "China", "Ivory Coast", "Cameroon", "Democratic Republic of Congo", "Republic of the Congo", "Cook Islands", "Colombia", 
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
	"Tanzania", "Uganda", "Ukraine", "United States Minor Outlying Islands", "Uruguay", "United States", "Uzbekistan", "Vatican", "Saint Vincent and the Grenadines", "Venezuela", 
	"British Virgin Islands", "United States Virgin Islands", "Viet Nam", "Vanuatu", "Wallis and Futuna", "Samoa", "Communaute Financiere Africaine (BEAC)", "East Caribbean", "Communaute Financiere Africaine (BCEAO)", "Comptoirs Francais du Pacifique", 
	"No Country", "Yemen", "South Africa", "Zambia", "Zimbabwe" };

const short Country::m_toISO[NUMCOUNTRY] = { NOCOUNTRY,
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
	XXX, YEM, ZAF, ZMB, ZWE };

const short Country::m_fromISO[MAXCOUNTRY] = {
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




//
//
//	






