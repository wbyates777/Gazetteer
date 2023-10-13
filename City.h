/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 History: Identifies a city using a part of the IATA airport code list. 
          Useful for describing a city associated with a Market Identification Codes (MIC).
 
 Each IATA code corresponds to the city served by that airport. For the special cases where no airport
 serves a city we have invented a code terminated in a '0'.
 When a city with no airport is a capital city the city3code is the country 2 code followed by a 0.
 
 Latitude and Longtitude of cities is provided for geograpical positioning
 and the calculation of distance between cities
 
 see https://www.iata.org/en/publications/directories/code-search/
 see https://en.wikipedia.org/wiki/IATA_airport_code
 
 **** Updated 10/10/2023 ****
 
 Added a number of cities including all US state capitals
 
 
 +-------------------+-----------+--------------+
 | city              | city3code | country3code |
 +-------------------+-----------+--------------+
 | Aabenraa          | AB0       | DNK          |
 | Aalborg           | AL0       | DNK          |
 | Adamstown         | PN0       | PCN          |
 | Andorra la Vella  | AD0       | AND          |
 | Aylesbury         | AY0       | GBR          |
 | Ebene             | EB0       | MUS          |
 | Ecatepec          | EP0       | MEX          |
 | Esch-sur-Alzette  | EA0       | LUX          |
 | Espoo             | ES0       | FIN          |
 | Foster City       | FC0       | USA          |
 | Gandhinagar       | GH0       | IND          |
 | GIFT City         | GC0       | IND          |
 | Greenwich         | GW0       | USA          |
 | Grindsted         | GR0       | DNK          |
 | Horsens           | HS0       | DNK          |
 | King Edward Point | GS0       | SGS          |
 | Leuven            | LV0       | BEL          |
 | Limassol          | LM0       | CYP          |
 | Lucerne           | LU0       | CHE          |
 | Makati City       | MC0       | PHL          |
 | Mill Valley       | MV0       | USA          |
 | Nablus            | PS0       | PSE          |
 | Nukunonu          | TK0       | TKL          |
 | Oldenburg         | OB0       | DEU          |
 | Pasig City        | PC0       | PHL          |
 | Port aux Francais | TF0       | ATF          |
 | Randers           | RA0       | DNK          |
 | Regensburg        | RB0       | DEU          |
 | s-Hertogenbosch   | SH0       | NLD          |
 | San Marino City   | SM0       | SMR          |
 | Schwerin          | SW0       | DEU          |
 | Shimonoseki       | SK0       | JPN          |
 | Silkeborg         | SI0       | DNK          |
 | Stamford          | ST0       | USA          |
 | The Hague         | HG0       | NLD          |
 | Vaduz             | LI0       | LIE          |
 | Varazdin          | VZ0       | HRV          |
 | Vatican City      | VA0       | VAT          |
 | Vienna            | VN0       | USA          |
 | Winter Park       | WP0       | USA          |
 +-------------------+-----------+--------------+

 
 Example
 
 City x;
 x.setCity( "NYC" );
 std::cout << x << std::endl;
 std::cout << x.name() << std::endl;
 std::cout << short(x) << std::endl;
 std::cout << x.capital() << std::endl;
 std::cout << "lat = " << x.lat() << std::endl;
 std::cout << "lon = " << x.lon() << std::endl;
 
 City y;
 y.setCity( "LON" );
 std::cout << y << std::endl;
 std::cout << y.name() << std::endl;
 std::cout << short(y) << std::endl;
 std::cout << y.capital() << std::endl;
 std::cout << "lat = " << y.lat() << std::endl;
 std::cout << "lon = " << y.lon() << std::endl;
 
 for (int i = 0; i < City::NUMCITY; i++)
 {	
    std::cout << City::index(i).toString() + " " + City::index(i).name() << std::endl;
 }
 
 
 std::cout << "The distance between London and New York is " << City::dist(x,y) / 1000.0 << " km"  << std::endl;
 exit(1);
 
 */


#ifndef __CITY_H__
#define __CITY_H__



#include <string>
#include <iostream>


class City
{
public:
	
	// The value of the enum elements are my arbitrary numeric code for each city. Note XXX, NOCITY, MAXCITY, NUMCITY and
	// any city codes that terminates in a '0' are not IATA codes
    enum CityCode : short {
        NOCITY = 0,
        AB0 = 1,   ABJ = 2,   ABV = 3,   ABZ = 4,   ACC = 5,   AD0 = 6,   ADB = 7,   ADD = 8,   AEP = 9,   AKL = 10, 
        AL0 = 11,  ALA = 12,  ALB = 13,  ALG = 14,  ALY = 15,  AMD = 16,  AMM = 17,  AMS = 18,  ANC = 19,  ANP = 20, 
        ANR = 21,  ANU = 22,  APW = 23,  ARH = 24,  ASB = 25,  ASI = 26,  ASM = 27,  ASU = 28,  ATH = 29,  ATL = 30, 
        ATQ = 31,  AUA = 32,  AUG = 33,  AUH = 34,  AUS = 35,  AXA = 36,  AY0 = 37,  BAH = 38,  BBR = 39,  BBU = 40, 
        BCN = 41,  BCT = 42,  BCV = 43,  BDL = 44,  BEG = 45,  BER = 46,  BEY = 47,  BFN = 48,  BFS = 49,  BGF = 50, 
        BGI = 51,  BGO = 52,  BGW = 53,  BGY = 54,  BHX = 55,  BIO = 56,  BIS = 57,  BJL = 58,  BJM = 59,  BJS = 60, 
        BKK = 61,  BKO = 62,  BLQ = 63,  BLR = 64,  BLZ = 65,  BMA = 66,  BNA = 67,  BNJ = 68,  BNX = 69,  BOG = 70, 
        BOI = 71,  BOM = 72,  BON = 73,  BOS = 74,  BRE = 75,  BRN = 76,  BRS = 77,  BRU = 78,  BSB = 79,  BSL = 80, 
        BTR = 81,  BTS = 82,  BTV = 83,  BUD = 84,  BWN = 85,  BZV = 86,  CAE = 87,  CAI = 88,  CAY = 89,  CBG = 90, 
        CBR = 91,  CCK = 92,  CCS = 93,  CCU = 94,  CGK = 95,  CGO = 96,  CGP = 97,  CHC = 98,  CHI = 99,  CHS = 100, 
        CKY = 101, CLJ = 102, CLT = 103, CMB = 104, CMH = 105, CMN = 106, CNQ = 107, CON = 108, COO = 109, COR = 110, 
        CPH = 111, CPT = 112, CRW = 113, CSN = 114, CUR = 115, CWB = 116, CWL = 117, CYS = 118, DAC = 119, DAM = 120, 
        DAR = 121, DCA = 122, DCF = 123, DEL = 124, DEN = 125, DFW = 126, DIL = 127, DKR = 128, DLA = 129, DLC = 130, 
        DND = 131, DNK = 132, DOH = 133, DOV = 134, DSM = 135, DUB = 136, DXB = 137, DYU = 138, DZA = 139, EA0 = 140, 
        EB0 = 141, EDI = 142, EIS = 143, EP0 = 144, ES0 = 145, ESB = 146, ESH = 147, ESR = 148, EUN = 149, EVN = 150, 
        EWR = 151, FAE = 152, FC0 = 153, FDF = 154, FFT = 155, FIH = 156, FLR = 157, FNA = 158, FNJ = 159, FOE = 160, 
        FRA = 161, FRU = 162, FSP = 163, FUK = 164, FUN = 165, GBE = 166, GC0 = 167, GCI = 168, GCM = 169, GDT = 170, 
        GEO = 171, GH0 = 172, GIB = 173, GLA = 174, GND = 175, GOA = 176, GOH = 177, GOI = 178, GOJ = 179, GOT = 180, 
        GR0 = 181, GRU = 182, GRX = 183, GRZ = 184, GS0 = 185, GUA = 186, GUM = 187, GVA = 188, GW0 = 189, GYD = 190, 
        GYE = 191, HAH = 192, HAJ = 193, HAM = 194, HAN = 195, HAV = 196, HEL = 197, HG0 = 198, HIJ = 199, HIR = 200, 
        HKG = 201, HLN = 202, HNL = 203, HRE = 204, HRK = 205, HS0 = 206, IAH = 207, IDR = 208, IEV = 209, ILG = 210, 
        ILZ = 211, IND = 212, INU = 213, IOM = 214, ISB = 215, IST = 216, IUE = 217, JAN = 218, JED = 219, JEF = 220, 
        JER = 221, JIB = 222, JNB = 223, JNU = 224, JSR = 225, JUB = 226, KBL = 227, KGL = 228, KHI = 229, KIJ = 230, 
        KIN = 231, KIV = 232, KLA = 233, KLU = 234, KRK = 235, KRT = 236, KTM = 237, KUF = 238, KUL = 239, KWI = 240, 
        LAD = 241, LAN = 242, LAX = 243, LBA = 244, LBU = 245, LBV = 246, LCA = 247, LCG = 248, LDY = 249, LED = 250, 
        LEJ = 251, LFW = 252, LHE = 253, LI0 = 254, LIM = 255, LIS = 256, LIT = 257, LJU = 258, LLW = 259, LM0 = 260, 
        LNK = 261, LNZ = 262, LON = 263, LOS = 264, LPB = 265, LPL = 266, LU0 = 267, LUN = 268, LUX = 269, LV0 = 270, 
        LYR = 271, LYS = 272, MAA = 273, MAD = 274, MAJ = 275, MAN = 276, MC0 = 277, MCI = 278, MCM = 279, MCT = 280, 
        MDT = 281, MDZ = 282, MEL = 283, MEX = 284, MFM = 285, MGA = 286, MGF = 287, MGN = 288, MGQ = 289, MHQ = 290, 
        MIA = 291, MIL = 292, MKE = 293, MLA = 294, MLE = 295, MLW = 296, MMX = 297, MNI = 298, MNL = 299, MOW = 300, 
        MPM = 301, MRS = 302, MRU = 303, MSN = 304, MSP = 305, MSQ = 306, MSU = 307, MUC = 308, MV0 = 309, MVD = 310, 
        NAP = 311, NAS = 312, NBO = 313, NCL = 314, NDJ = 315, NGO = 316, NIC = 317, NIM = 318, NKC = 319, NKW = 320, 
        NLK = 321, NOU = 322, NTL = 323, NYC = 324, OB0 = 325, ODS = 326, OKC = 327, OKD = 328, OLM = 329, OPO = 330, 
        ORK = 331, OSA = 332, OSL = 333, OUA = 334, OVB = 335, OXB = 336, PAC = 337, PAP = 338, PAR = 339, PBH = 340, 
        PBM = 341, PC0 = 342, PCT = 343, PHL = 344, PHX = 345, PIR = 346, PMI = 347, PN0 = 348, PNH = 349, PNI = 350, 
        POM = 351, POS = 352, PPG = 353, PPT = 354, PRG = 355, PRN = 356, PRY = 357, PS0 = 358, PSY = 359, PVD = 360, 
        QDU = 361, QIC = 362, QMN = 363, QND = 364, QPA = 365, QSA = 366, RA0 = 367, RAI = 368, RAR = 369, RB0 = 370, 
        RBA = 371, RDU = 372, RGN = 373, RIC = 374, RIO = 375, RIX = 376, RIZ = 377, RKV = 378, ROM = 379, ROR = 380, 
        ROS = 381, ROV = 382, RTM = 383, RUH = 384, RUN = 385, SAF = 386, SAH = 387, SAL = 388, SAP = 389, SBH = 390, 
        SBZ = 391, SDQ = 392, SDR = 393, SEL = 394, SEZ = 395, SFG = 396, SFN = 397, SFO = 398, SGN = 399, SH0 = 400, 
        SHA = 401, SI0 = 402, SIN = 403, SJI = 404, SJJ = 405, SJU = 406, SK0 = 407, SKB = 408, SKG = 409, SKP = 410, 
        SLC = 411, SLE = 412, SLU = 413, SM0 = 414, SMF = 415, SNN = 416, SOF = 417, SPI = 418, SPN = 419, SPU = 420, 
        SSG = 421, ST0 = 422, STI = 423, STP = 424, STR = 425, STT = 426, SUB = 427, SUV = 428, SVD = 429, SVX = 430, 
        SW0 = 431, SWS = 432, SXB = 433, SXM = 434, SYD = 435, SZG = 436, SZX = 437, TAS = 438, TBS = 439, TBU = 440, 
        TF0 = 441, TGD = 442, TGU = 443, THR = 444, TIA = 445, TIP = 446, TK0 = 447, TLH = 448, TLL = 449, TLS = 450, 
        TLV = 451, TMP = 452, TMS = 453, TNR = 454, TOS = 455, TRD = 456, TRN = 457, TRW = 458, TSA = 459, TSE = 460, 
        TTN = 461, TUC = 462, TUN = 463, TYO = 464, UIO = 465, UKB = 466, UKY = 467, ULN = 468, UTC = 469, VA0 = 470, 
        VFA = 471, VIE = 472, VIX = 473, VLC = 474, VLI = 475, VN0 = 476, VNO = 477, VOG = 478, VPS = 479, VTE = 480, 
        VVO = 481, VZ0 = 482, WAW = 483, WDH = 484, WLG = 485, WLS = 486, WP0 = 487, WRO = 488, WUX = 489, XCH = 490, 
        XXX = 491, YAO = 492, YHM = 493, YMQ = 494, YOW = 495, YTZ = 496, YVR = 497, YWG = 498, YYC = 499, YYZ = 500, 
        ZAG = 501, ZAZ = 502, ZRH = 503, 
        MAXCITY = 504, NUMCITY = 504 
        };

	City( void ): m_city(NOCITY) {}
	~City( void )  { m_city = NOCITY; }
	
	// non-explicit constructors intentional here
	City( CityCode i ): m_city(i) {} // i.e. i = City::LON
	City( const std::string& s ): m_city(NOCITY) { setCity(s); }
	City( const char *s ): m_city(NOCITY) { if (s) setCity(s); } 
    
	// My numeric code for this city i.e. City::LON = 78
	operator short( void ) const { return m_city; }
	
	// The IATA 3 letter code for this city i.e. "LON"
    std::string
    toString( void ) const { return m_cityNames[m_fromISO[m_city]]; }
	
	std::string
	name( void ) const; // i.e "London" 
	
	bool
	setCity( const std::string& s ); // i.e. s = "LON"
	
	void
	setCity( const CityCode s ) { m_city = s; } // i.e. s = City::LON
    
    bool
    capital( void ) const;
    
    float
    lat( void ) const;   // latitude  
    
	float
	lon( void ) const;   // longitude

    // distance (in metres) calculated using Vincenty inverse solution
    static float
    dist( const City c1, const City c2 ) { return City::dist(c1.lat(), c1.lon(), c2.lat(), c2.lon()); };
    
    static float
    dist( float lat1, float lon1, float lat2, float lon2 );
    
	static City
	index( const int i )  { return CityCode(m_toISO[i]); }
    
    static int
	index( const City c ) { return CityCode(m_fromISO[c]); }
    
    bool                
	valid( void ) const { return m_city != NOCITY; }
	
private:
	
	short m_city; // we use short here as it simplifies streaming 
	
	static const char * const  m_cityNames[NUMCITY];
	static const char * const  m_fullCityNames[NUMCITY];
	static const short         m_toISO[NUMCITY];
    static const short         m_fromISO[NUMCITY]; 
	static const unsigned char m_capital[NUMCITY];
	static const float         m_position[NUMCITY][2];    
    static const short         m_searchPoints[27];    
};


std::ostream&
operator<<(std::ostream& ostr, const City& c);

std::istream&
operator>>(std::istream& istr, City& c);


#endif
