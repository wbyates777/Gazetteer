/* Currency 10/09/09

		$$$$$$$$$$$$$$$$$$$$$$$$$$$
		$   Currency.cpp - code   $
		$$$$$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates    
   Copyright (c) W.B. Yates
   History: Supports most of the ISO 4217 Currency Code List
 
  **** Updated 10/10/2023 ****
 
*/

#ifndef __CURRENCY_H__
#include "Currency.h"
#endif

#include <assert.h>

std::ostream&
operator<<( std::ostream& ostr, const Currency& c )
{
	ostr << c.toString();
	return ostr;
}

std::istream&
operator>>( std::istream& istr, Currency& c )
{
	std::string str;
	istr >> str;
	c.setCurrency( str );
	return istr;
}



// default base currency
Currency Currency::m_baseCurrency = Currency::USD;

std::string
Currency::name( void ) const 
{
	return m_denomNames[m_fromISO[m_ccy]];
}


// the binary chop algorithm complexity is log2(N)−1 for the expected number of 
// probes in an average successful search, and the worst case is log2(N)
// the search points array means that N is now the size of a (smaller) alphabetic 
// partiton at the cost of a single probe.
// also note that the algorithm work efficiently in the presence of singleton currencies such as OMR and QAR
// note NOCURRENCY is 0
const short Currency::m_searchPoints[27] = {
//  A,  B,  C,  D,  E,  F,  G,  H,  I,  J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X    Y,   Z,  Z + 1 
    1, 17, 39, 57, 63, 72, 76, 86, 91, 99, 102, 111, 120, 139, 146, 147, 156, 157, 163, 184, 197, 207, 213, 214, 231, 235, 242, 
};

// this speeds up setCurrency quite a bit as E leads to EUR, G leads to GBP, J leads to JPY, and U to USD 
// in practice these are heavily used currencies
//const short Currency::m_midPoints[26] =
const short Currency::m_midPoints[26] =
{
//  A,  B,  C,  D,  E,  F,  G,  H,  I,  J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X    Y,   Z
 
    9, 27, 47, 59, 71, 73, 76, 88, 94, 101, 106, 115, 129, 142, 146, 151, 156, 159, 173, 190, 200, 209, 213, 222, 232, 238,  
};

bool
Currency::setCurrency( const std::string& str )
// see http://en.wikipedia.org/wiki/Binary_search_algorithm
{	
    // assert(str.size() == 3);
    if (str.size() != 3)
    {
        m_ccy = Currency::XXX; // NOCURRENCY
        return false;
    }
	
    int index = str[0] - 'A'; // 'A' = 65;
    
    // assert(index > -1 && index < 26);
    if (index < 0 || index > 25)
    {
        m_ccy = Currency::XXX;
        return false;
    }
    
    int low   = m_searchPoints[index]; 
    int high  = m_searchPoints[index + 1]; 
    int mid   = m_midPoints[index];
    int i;
    
    while (low < high)  
    {
        const char * const ccy = m_ccyNames[mid];
        
        for (i = 1; i < 3; ++i)
        {
             const char &a = str[i];	
             const char &b = ccy[i];
 
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
            m_ccy = m_toISO[mid]; 
            return true;
        }
        
        mid = low + ((high - low) >> 1);
    } 

    m_ccy = Currency::XXX; // NOCURRENCY
    return false;
}

// tables generated automatically



const short Currency::m_toISO[NUMCURRENCY] = { NOCURRENCY,
ADP, AED, AFA, AFN, ALL, AMD, ANG, AOA, AON, AOR, 
ARS, ATS, AUD, AWG, AZM, AZN, BAD, BAM, BBD, BDT, 
BEC, BEF, BEL, BGL, BGN, BHD, BIF, BMD, BND, BOB, 
BOV, BRL, BSD, BTN, BWP, BYN, BYR, BZD, CAD, CDF, 
CHC, CHE, CHF, CHW, CLF, CLP, CNY, COP, COU, CRC, 
CSK, CUC, CUP, CVE, CYP, CZK, DDM, DEM, DJF, DKK, 
DOP, DZD, ECS, EEK, EGP, ERN, ESA, ESB, ESP, ETB, 
EUR, FIM, FJD, FKP, FRF, GBP, GEL, GHS, GIP, GMD, 
GNF, GRD, GTQ, GWP, GYD, HKD, HNL, HRK, HTG, HUF, 
IDR, IEP, ILS, INR, IQD, IRR, ISK, ITL, JMD, JOD, 
JPY, KES, KGS, KHR, KMF, KPW, KRW, KWD, KYD, KZT, 
LAK, LBP, LKR, LRD, LSL, LTL, LUF, LVL, LYD, MAD, 
MDL, MGA, MGF, MKD, MMK, MNT, MOP, MRO, MRU, MTL, 
MUR, MVR, MWK, MXN, MXV, MYR, MZM, MZN, NAD, NGN, 
NIO, NLG, NOK, NPR, NZD, OMR, PAB, PEN, PGK, PHP, 
PKR, PLN, PLZ, PTE, PYG, QAR, ROL, RON, RSD, RUB, 
RUR, RWF, SAR, SBD, SCR, SDD, SDG, SEK, SGD, SHP, 
SIT, SKK, SLE, SLL, SOS, SRD, SRG, SSP, STD, STN, 
SVC, SYP, SZL, THB, TJR, TJS, TMM, TMT, TND, TOP, 
TPE, TRL, TRY, TTD, TWD, TZS, UAH, UAK, UGX, USD, 
USN, USS, UYI, UYU, UYW, UZS, VEB, VED, VEF, VES, 
VND, VUV, WST, XAF, XAG, XAU, XBA, XBB, XBC, XBD, 
XCD, XDR, XOF, XPD, XPF, XPT, XSU, XTS, XUA, XXX, 
YDD, YER, YUD, YUM, ZAL, ZAR, ZMK, ZMW, ZRN, ZWD, 
ZWL, 
};

const short Currency::m_fromISO[MAXCURRENCY] = {
0, 0, 0, 0, 3, 0, 0, 0, 5, 0, 
0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 
1, 0, 0, 0, 9, 0, 0, 0, 0, 0, 
0, 15, 11, 0, 0, 0, 13, 0, 0, 0, 
12, 0, 0, 0, 33, 0, 0, 0, 26, 0, 
20, 6, 19, 0, 0, 0, 22, 0, 0, 0, 
28, 0, 0, 0, 34, 0, 0, 0, 30, 0, 
17, 0, 35, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 38, 0, 0, 0, 0, 0, 
164, 0, 0, 0, 0, 0, 29, 0, 0, 0, 
24, 0, 0, 0, 125, 0, 0, 0, 27, 0, 
0, 0, 0, 0, 0, 0, 104, 0, 0, 0, 
0, 0, 0, 0, 39, 0, 0, 0, 0, 0, 
0, 0, 54, 0, 0, 0, 109, 0, 0, 0, 
0, 0, 0, 0, 113, 0, 0, 0, 0, 0, 
0, 0, 46, 0, 0, 0, 47, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
48, 0, 0, 0, 105, 0, 0, 0, 0, 0, 
239, 0, 0, 0, 0, 0, 0, 0, 50, 0, 
0, 88, 53, 0, 0, 0, 55, 0, 0, 0, 
51, 0, 0, 56, 0, 0, 0, 0, 60, 0, 
0, 0, 0, 0, 61, 0, 0, 0, 63, 0, 
0, 0, 181, 0, 0, 0, 0, 0, 0, 0, 
70, 0, 66, 64, 0, 0, 0, 0, 74, 0, 
0, 0, 73, 0, 0, 0, 72, 0, 0, 0, 
75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 59, 0, 0, 0, 0, 0, 0, 0, 
80, 0, 0, 0, 0, 0, 58, 0, 57, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 79, 0, 0, 0, 0, 0, 0, 0, 
82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
83, 0, 0, 0, 81, 0, 0, 0, 85, 0, 
0, 0, 89, 0, 0, 0, 0, 0, 0, 0, 
87, 0, 0, 0, 86, 0, 0, 0, 90, 0, 
0, 0, 97, 0, 0, 0, 94, 0, 0, 0, 
91, 0, 0, 0, 96, 0, 0, 0, 95, 0, 
0, 0, 92, 0, 0, 0, 93, 0, 0, 0, 
98, 0, 0, 0, 0, 0, 0, 0, 99, 0, 
0, 0, 101, 0, 0, 0, 0, 0, 110, 0, 
100, 0, 0, 0, 102, 0, 0, 0, 106, 0, 
107, 0, 0, 0, 108, 0, 0, 103, 111, 0, 
0, 0, 112, 0, 0, 0, 115, 0, 118, 0, 
114, 0, 0, 0, 119, 0, 0, 0, 0, 0, 
116, 0, 117, 0, 0, 0, 127, 0, 0, 0, 
123, 0, 0, 0, 133, 0, 0, 0, 136, 0, 
0, 0, 132, 0, 0, 0, 0, 0, 0, 0, 
130, 0, 0, 0, 0, 0, 0, 0, 128, 0, 
131, 0, 0, 0, 134, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 126, 0, 121, 0, 
0, 0, 0, 0, 120, 0, 0, 0, 137, 0, 
0, 0, 146, 0, 0, 0, 139, 0, 0, 0, 
0, 0, 0, 0, 144, 0, 0, 0, 142, 0, 
0, 0, 7, 14, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 212, 0, 
0, 0, 0, 0, 145, 0, 0, 0, 141, 0, 
0, 0, 0, 0, 0, 0, 140, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 143, 0, 
0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 
147, 0, 0, 0, 0, 0, 0, 0, 149, 0, 
155, 0, 0, 0, 148, 0, 0, 0, 150, 0, 
0, 0, 0, 0, 0, 0, 153, 0, 0, 0, 
154, 0, 0, 0, 84, 0, 191, 0, 0, 0, 
0, 0, 0, 0, 156, 0, 0, 0, 0, 0, 
0, 0, 157, 160, 0, 0, 162, 0, 0, 0, 
0, 0, 0, 0, 170, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 179, 0, 
0, 0, 163, 0, 0, 0, 0, 0, 0, 0, 
165, 0, 0, 0, 174, 0, 0, 0, 0, 0, 
0, 0, 169, 172, 211, 171, 175, 0, 0, 0, 
236, 0, 0, 0, 0, 0, 240, 0, 0, 0, 
231, 0, 0, 0, 69, 0, 0, 0, 178, 0, 
0, 0, 0, 0, 0, 0, 166, 0, 0, 0, 
177, 0, 0, 0, 0, 0, 0, 0, 183, 0, 
0, 0, 168, 0, 0, 0, 43, 0, 0, 0, 
182, 0, 185, 0, 184, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 190, 0, 0, 0, 
194, 0, 0, 0, 2, 0, 0, 0, 189, 0, 
0, 0, 192, 0, 0, 187, 0, 0, 0, 0, 
199, 0, 0, 0, 198, 0, 0, 124, 0, 0, 
161, 0, 0, 0, 0, 0, 0, 0, 65, 0, 
0, 0, 0, 0, 0, 0, 76, 0, 0, 0, 
0, 0, 0, 0, 196, 0, 0, 0, 0, 0, 
200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 204, 0, 
206, 0, 207, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 213, 0, 0, 0, 232, 0, 0, 0, 
233, 234, 0, 0, 237, 0, 0, 0, 0, 0, 
0, 195, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 173, 208, 205, 210, 129, 
180, 52, 241, 36, 188, 0, 78, 209, 167, 0, 
203, 159, 0, 138, 16, 0, 158, 42, 44, 193, 
214, 221, 223, 225, 0, 217, 218, 219, 220, 216, 
222, 215, 226, 228, 224, 229, 0, 238, 176, 122, 
49, 4, 186, 8, 37, 25, 40, 18, 71, 135, 
197, 77, 10, 0, 31, 152, 32, 0, 0, 0, 
45, 235, 23, 21, 227, 68, 67, 201, 202, 230, 

};


const char * const Currency::m_ccyNames[NUMCURRENCY] = { "NOCURRENCY", 
"ADP", "AED", "AFA", "AFN", "ALL", "AMD", "ANG", "AOA", "AON", "AOR", 
"ARS", "ATS", "AUD", "AWG", "AZM", "AZN", "BAD", "BAM", "BBD", "BDT", 
"BEC", "BEF", "BEL", "BGL", "BGN", "BHD", "BIF", "BMD", "BND", "BOB", 
"BOV", "BRL", "BSD", "BTN", "BWP", "BYN", "BYR", "BZD", "CAD", "CDF", 
"CHC", "CHE", "CHF", "CHW", "CLF", "CLP", "CNY", "COP", "COU", "CRC", 
"CSK", "CUC", "CUP", "CVE", "CYP", "CZK", "DDM", "DEM", "DJF", "DKK", 
"DOP", "DZD", "ECS", "EEK", "EGP", "ERN", "ESA", "ESB", "ESP", "ETB", 
"EUR", "FIM", "FJD", "FKP", "FRF", "GBP", "GEL", "GHS", "GIP", "GMD", 
"GNF", "GRD", "GTQ", "GWP", "GYD", "HKD", "HNL", "HRK", "HTG", "HUF", 
"IDR", "IEP", "ILS", "INR", "IQD", "IRR", "ISK", "ITL", "JMD", "JOD", 
"JPY", "KES", "KGS", "KHR", "KMF", "KPW", "KRW", "KWD", "KYD", "KZT", 
"LAK", "LBP", "LKR", "LRD", "LSL", "LTL", "LUF", "LVL", "LYD", "MAD", 
"MDL", "MGA", "MGF", "MKD", "MMK", "MNT", "MOP", "MRO", "MRU", "MTL", 
"MUR", "MVR", "MWK", "MXN", "MXV", "MYR", "MZM", "MZN", "NAD", "NGN", 
"NIO", "NLG", "NOK", "NPR", "NZD", "OMR", "PAB", "PEN", "PGK", "PHP", 
"PKR", "PLN", "PLZ", "PTE", "PYG", "QAR", "ROL", "RON", "RSD", "RUB", 
"RUR", "RWF", "SAR", "SBD", "SCR", "SDD", "SDG", "SEK", "SGD", "SHP", 
"SIT", "SKK", "SLE", "SLL", "SOS", "SRD", "SRG", "SSP", "STD", "STN", 
"SVC", "SYP", "SZL", "THB", "TJR", "TJS", "TMM", "TMT", "TND", "TOP", 
"TPE", "TRL", "TRY", "TTD", "TWD", "TZS", "UAH", "UAK", "UGX", "USD", 
"USN", "USS", "UYI", "UYU", "UYW", "UZS", "VEB", "VED", "VEF", "VES", 
"VND", "VUV", "WST", "XAF", "XAG", "XAU", "XBA", "XBB", "XBC", "XBD", 
"XCD", "XDR", "XOF", "XPD", "XPF", "XPT", "XSU", "XTS", "XUA", "XXX", 
"YDD", "YER", "YUD", "YUM", "ZAL", "ZAR", "ZMK", "ZMW", "ZRN", "ZWD", 
"ZWL", 
};

const char * const Currency::m_denomNames[NUMCURRENCY] = { "No Currency",
"Andorran Peseta (1:1 peg to the Spanish Peseta)", "UAE Dirham", "Afghani", "Afghani", "Lek", "Armenian Dram", "Netherlands Antillian Guilder", "Kwanza", "Angolan New Kwanza", "Angolan Kwanza Readjustado", 
"Argentine Peso", "Austrian Schilling", "Australian Dollar", "Aruban Guilder", "Azerbaijani Manat", "Azerbaijanian Manat", "Bosnia and Herzegovina Dinar", "Convertible Marks", "Barbados Dollar", "Taka", 
"Belgian Franc (convertible)", "Belgian Franc (currency union with LUF)", "Belgian Franc (financial)", "Bulgarian Lev A/99", "Bulgarian Lev", "Bahraini Dinar", "Burundi Franc", "Bermudian Dollar", "Brunei Dollar", "Boliviano", 
"Mvdol", "Brazilian Real", "Bahamian Dollar", "Ngultrum", "Pula", "Belarussian Ruble", "Belarussian Ruble", "Belize Dollar", "Canadian Dollar", "Congolese Franc", 
"WIR Franc", "WIR Euro", "Swiss Franc", "WIR Franc", "Unidades de Fomento", "Chilean Peso", "Yuan Renminbi", "Colombian Peso", "Unidad de Valor Real", "Costa Rican Colon", 
"Czechoslovak Koruna", "Peso Convertible", "Cuban Peso", "Cape Verde Escudo", "Cyprus Pound", "Czech Koruna", "East German Mark", "Deutsche Mark", "Djibouti Franc", "Danish Krone", 
"Dominican Peso", "Algerian Dinar", "Ecuador Sucre", "Kroon", "Egyptian Pound", "Nakfa", "Spanish Peseta (Account A)", "Spanish Peseta (Account B)", "Spanish Peseta", "Ethiopian Birr", 
"Euro", "Finnish Markka", "Fiji Dollar", "Falkland Island Pound", "French Franc", "Pound Sterling", "Lari", "Cedi", "Gibraltar Pound", "Dalasi", 
"Guinea Franc", "Greek Drachma", "Quetzal", "Guinea-Bissau Peso", "Guyana Dollar", "Hong Kong Dollar", "Lempira", "Croatian Kuna", "Gourde", "Forint", 
"Rupiah", "Irish Punt", "New Israeli Sheqel", "Indian Rupee", "Iraqi Dinar", "Iranian Rial", "Iceland Krona", "Italian Lira", "Jamaican Dollar", "Jordanian Dinar", 
"Yen", "Kenyan Shilling", "Som", "Riel", "Comoro Franc", "North Korean Won", "Won", "Kuwaiti Dinar", "Cayman Islands Dollar", "Tenge", 
"Kip", "Lebanese Pound", "Sri Lanka Rupee", "Liberian Dollar", "Loti", "Lithuanian Litas", "Luxembourg Franc (currency union with BEF)", "Latvian Lats", "Libyan Dinar", "Moroccan Dirham", 
"Moldovan Leu", "Malagasy Ariary", "Malagasy Franc", "Denar", "Kyat", "Tugrik", "Pataca", "Ouguiya", "Ouguiya", "Maltese Lira", 
"Mauritius Rupee", "Rufiyaa", "Kwacha", "Mexican Peso", "Mexican Unidad de Inversion (UDI)", "Malaysian Ringgit", "Mozambican Metical", "Metical", "Namibia Dollar", "Naira", 
"Cordoba Oro", "Netherlands Guilder", "Norwegian Krone", "Nepalese Rupee", "New Zealand Dollar", "Rial Omani", "Balboa", "Nuevo Sol", "Kina", "Philippine Peso", 
"Pakistan Rupee", "Zloty", "Polish Zloty A/94", "Portuguese Escudo", "Guarani", "Qatari Rial", "Romanian Leu A/05", "New Leu", "Serbian Dinar", "Russian Ruble", 
"Russian RubleA/97", "Rwanda Franc", "Saudi Riyal", "Solomon Islands Dollar", "Seychelles Rupee", "Sudanese Dinar", "Sudanese Pound", "Swedish Krona", "Singapore Dollar", "Saint Helena Pound", 
"Slovenian Tolar", "Slovak Koruna", "New Leone", "Leone", "Somali Shilling", "Surinam Dollar", "Suriname Guilder", "South Sudanese Pound", "Dobra", "Sao Tome and Principe Dobra", 
"El Salvador Colon", "Syrian Pound", "Lilangeni", "Baht", "Tajikistan Ruble", "Somoni", "Manat", "Manat", "Tunisian Dinar", "Paanga", 
"Portuguese Timorese Escudo", "Turkish Lira A/05", "Turkish Lira", "Trinidad and Tobago Dollar", "New Taiwan Dollar", "Tanzanian Shilling", "Hryvnia", "Ukrainian Karbovanets", "Uganda Shilling", "US Dollar", 
"US Dollar (Next Day)", "US Dollar (Same Day)", "Uruguay Peso en Unidades Indexadas", "Uruguayan Peso", "Unidad Previsional Uruguayo", "Uzbekistan Sum", "Venezuelan Bolivar", "Venezuelan Digital Bolívar", "Bolivar Fuerte", "Venezuelan Sovereign Bolivar", 
"Dong", "Vatu", "Tala", "CFA Franc BEAC", "Silver", "Gold", "EURCO", "E.M.U.-6", "E.U.A.-9", "E.U.A.-17", 
"East Caribbean Dollar", "Special Drawing Rights", "CFA Franc BCEAO", "Palladium", "CFP Franc", "Platinum", "SUCRE", "Test Code", "ADB Unit of Account", "No Currency", 
"South Yemeni Dinar", "Yemeni Rial", "Yugoslav Dinar", "Yugoslav Dinar", "South African Financial Rand (funds code)", "Rand", "Zambian Kwacha", "Zambian Kwacha", "Zairean New Zaire", "Zimbabwe Dollar", 
"Zimbabwe Dollar", 
};
//
//
//	




