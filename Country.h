/* Country 04/01/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$
 $   Country.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved 
 History: Supports most of ISO 3166-1 country code list
 
 **** Updated 10/10/2023 ****
 
 Note there are no ISO codes for Kosovo at the time of writting
 Note there are no ISO codes for Zaire  at the time of writting
 
 https://www.iso.org/iso/country_codes
 
 https://en.wikipedia.org/wiki/ISO_3166-1_alpha-3
 
 Region and sub region codes are taken from:
 
 "Standard Country or Area Codes for Statistical Use, Revision 4 (United Nations publication, Sales No. 98.XVII.9"
 
 // we should add this...
 // UNK identifies Kosovo residents to whom travel documents were issued by the United Nations Interim Administration in Kosovo (UNMIK)

 Example 
 
 Country x;
 x.setCountry( Country::US );
 std::cout << x << std::endl;
 std::cout << x.name() << std::endl;
 std::cout << short(x) << std::endl; 
 std::cout << x.regionName() << std::endl;
 std::cout << x.region() << std::endl;
 std::cout << x.subRegionName() << std::endl;
 std::cout << x.subRegion() << std::endl << std::endl;
 
 Country y;
 y.setCountry( "GBR" );
 std::cout << y << std::endl;
 std::cout << y.name() << std::endl;
 std::cout << short(y) << std::endl; 
 std::cout << y.regionName() << std::endl;
 std::cout << y.region() << std::endl;
 std::cout << y.subRegionName() << std::endl;
 std::cout << y.subRegion() << std::endl << std::endl;
 
 Country z;
 z.setCountry( "GH3" );
 std::cout << z << std::endl;
 std::cout << z.name() << std::endl;
 std::cout << short(z) << std::endl;
 std::cout << z.regionName() << std::endl;
 std::cout << z.region() << std::endl;
 std::cout << z.subRegionName() << std::endl;
 std::cout << z.subRegion() << std::endl;
 
 exit(1);
 
 */


#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#include <string>
#include <iostream>


class Country
{
public:
	
	// The value of the enum elements are the ISO numeric code for each country. Note, NOCOUNTRY, XXX, EUR, XAF, XCD, XOF, XPF, NUMCOUNTRY, and MAXCOUNTRY are not ISO country codes
	// The non-ISO codes EUR, XAF, XCD, XOF, XPF represent currency unions between distinct countries
	// 2 and 3 letter codes supported, although this class uses the 3 letter code
	enum CountryCode  : short {
		NOCOUNTRY = 0,
		AW = 533, ABW = 533, AF = 4, AFG = 4, AO = 24, AGO = 24, AI = 660, AIA = 660, AX = 248, ALA = 248, AL = 8, ALB = 8, AD = 20, AND = 20, AE = 784, ARE = 784, AR = 32, ARG = 32, AM = 51, ARM = 51, 
		AS = 16, ASM = 16, AQ = 10, ATA = 10, TF = 260, ATF = 260, AG = 28, ATG = 28, AU = 36, AUS = 36, AT = 40, AUT = 40, AZ = 31, AZE = 31, BI = 108, BDI = 108, BE = 56, BEL = 56, BJ = 204, BEN = 204, 
		BQ = 535, BES = 535, BF = 854, BFA = 854, BD = 50, BGD = 50, BG = 100, BGR = 100, BH = 48, BHR = 48, BS = 44, BHS = 44, BA = 70, BIH = 70, BL = 652, BLM = 652, BY = 112, BLR = 112, BZ = 84, BLZ = 84, 
		BM = 60, BMU = 60, BO = 68, BOL = 68, BR = 76, BRA = 76, BB = 52, BRB = 52, BN = 96, BRN = 96, BT = 64, BTN = 64, BV = 74, BVT = 74, BW = 72, BWA = 72, CF = 140, CAF = 140, CA = 124, CAN = 124, 
		CC = 166, CCK = 166, CH = 756, CHE = 756, CL = 152, CHL = 152, CN = 156, CHN = 156, CI = 384, CIV = 384, CM = 120, CMR = 120, CD = 180, COD = 180, CG = 178, COG = 178, CK = 184, COK = 184, CO = 170, COL = 170, 
		KM = 174, COM = 174, CV = 132, CPV = 132, CR = 188, CRI = 188, CU = 192, CUB = 192, CW = 531, CUW = 531, CX = 162, CXR = 162, KY = 136, CYM = 136, CY = 196, CYP = 196, CZ = 203, CZE = 203, DE = 276, DEU = 276, 
		DJ = 262, DJI = 262, DM = 212, DMA = 212, DK = 208, DNK = 208, DO = 214, DOM = 214, DZ = 12, DZA = 12, EC = 218, ECU = 218, EG = 818, EGY = 818, ER = 232, ERI = 232, EH = 732, ESH = 732, ES = 724, ESP = 724, 
		EE = 233, EST = 233, ET = 231, ETH = 231, EZ = 1001, EUR = 1001, FI = 246, FIN = 246, FJ = 242, FJI = 242, FK = 238, FLK = 238, FR = 250, FRA = 250, FO = 234, FRO = 234, FM = 583, FSM = 583, GA = 266, GAB = 266, 
		GB = 826, GBR = 826, GE = 268, GEO = 268, GG = 831, GGY = 831, GH = 288, GHA = 288, GI = 292, GIB = 292, GN = 324, GIN = 324, GP = 312, GLP = 312, GM = 270, GMB = 270, GW = 624, GNB = 624, GQ = 226, GNQ = 226, 
		GR = 300, GRC = 300, GD = 308, GRD = 308, GL = 304, GRL = 304, GT = 320, GTM = 320, GF = 254, GUF = 254, GU = 316, GUM = 316, GY = 328, GUY = 328, HK = 344, HKG = 344, HM = 334, HMD = 334, HN = 340, HND = 340, 
		HR = 191, HRV = 191, HT = 332, HTI = 332, HU = 348, HUN = 348, ID = 360, IDN = 360, IM = 833, IMN = 833, IN = 356, IND = 356, IO = 86, IOT = 86, IE = 372, IRL = 372, IR = 364, IRN = 364, IQ = 368, IRQ = 368, 
		IS = 352, ISL = 352, IL = 376, ISR = 376, IT = 380, ITA = 380, JM = 388, JAM = 388, JE = 832, JEY = 832, JO = 400, JOR = 400, JP = 392, JPN = 392, KZ = 398, KAZ = 398, KE = 404, KEN = 404, KG = 417, KGZ = 417, 
		KH = 116, KHM = 116, KI = 296, KIR = 296, KN = 659, KNA = 659, KR = 410, KOR = 410, KW = 414, KWT = 414, LA = 418, LAO = 418, LB = 422, LBN = 422, LR = 430, LBR = 430, LY = 434, LBY = 434, LC = 662, LCA = 662, 
		LI = 438, LIE = 438, LK = 144, LKA = 144, LS = 426, LSO = 426, LT = 440, LTU = 440, LU = 442, LUX = 442, LV = 428, LVA = 428, MO = 446, MAC = 446, MF = 663, MAF = 663, MA = 504, MAR = 504, MC = 492, MCO = 492, 
		MD = 498, MDA = 498, MG = 450, MDG = 450, MV = 462, MDV = 462, MX = 484, MEX = 484, MH = 584, MHL = 584, MK = 807, MKD = 807, ML = 466, MLI = 466, MT = 470, MLT = 470, MM = 104, MMR = 104, ME = 499, MNE = 499, 
		MN = 496, MNG = 496, MP = 580, MNP = 580, MZ = 508, MOZ = 508, MR = 478, MRT = 478, MS = 500, MSR = 500, MQ = 474, MTQ = 474, MU = 480, MUS = 480, MW = 454, MWI = 454, MY = 458, MYS = 458, YT = 175, MYT = 175, 
		NA = 516, NAM = 516, NC = 540, NCL = 540, NE = 562, NER = 562, NF = 574, NFK = 574, NG = 566, NGA = 566, NI = 558, NIC = 558, NU = 570, NIU = 570, NL = 528, NLD = 528, NO = 578, NOR = 578, NP = 524, NPL = 524, 
		NR = 520, NRU = 520, NZ = 554, NZL = 554, OM = 512, OMN = 512, PK = 586, PAK = 586, PA = 591, PAN = 591, PN = 612, PCN = 612, PE = 604, PER = 604, PH = 608, PHL = 608, PW = 585, PLW = 585, PG = 598, PNG = 598, 
		PL = 616, POL = 616, PR = 630, PRI = 630, KP = 408, PRK = 408, PT = 620, PRT = 620, PY = 600, PRY = 600, PS = 275, PSE = 275, PF = 258, PYF = 258, QA = 634, QAT = 634, RE = 638, REU = 638, RO = 642, ROU = 642, 
		RU = 643, RUS = 643, RW = 646, RWA = 646, SA = 682, SAU = 682, SD = 729, SDN = 729, SN = 686, SEN = 686, SG = 702, SGP = 702, GS = 239, SGS = 239, SH = 654, SHN = 654, SJ = 744, SJM = 744, SB = 90, SLB = 90, 
		SL = 694, SLE = 694, SV = 222, SLV = 222, SM = 674, SMR = 674, SO = 706, SOM = 706, PM = 666, SPM = 666, RS = 688, SRB = 688, SS = 728, SSD = 728, ST = 678, STP = 678, SR = 740, SUR = 740, SK = 703, SVK = 703, 
		SI = 705, SVN = 705, SE = 752, SWE = 752, SZ = 748, SWZ = 748, SX = 534, SXM = 534, SC = 690, SYC = 690, SY = 760, SYR = 760, TC = 796, TCA = 796, TD = 148, TCD = 148, TG = 768, TGO = 768, TH = 764, THA = 764, 
		TJ = 762, TJK = 762, TK = 772, TKL = 772, TM = 795, TKM = 795, TL = 626, TLS = 626, TO = 776, TON = 776, TT = 780, TTO = 780, TN = 788, TUN = 788, TR = 792, TUR = 792, TV = 798, TUV = 798, TW = 158, TWN = 158, 
		TZ = 834, TZA = 834, UG = 800, UGA = 800, UA = 804, UKR = 804, UM = 581, UMI = 581, UY = 858, URY = 858, US = 840, USA = 840, UZ = 860, UZB = 860, VA = 336, VAT = 336, VC = 670, VCT = 670, VE = 862, VEN = 862, 
		VG = 92, VGB = 92, VI = 850, VIR = 850, VN = 704, VNM = 704, VU = 548, VUT = 548, WF = 876, WLF = 876, WS = 882, WSM = 882, XA = 1002, XAF = 1002, XC = 1003, XCD = 1003, XO = 1004, XOF = 1004, XP = 1005, XPF = 1005, 
		XX = 1006, XXX = 1006, YE = 887, YEM = 887, ZA = 710, ZAF = 710, ZM = 894, ZMB = 894, ZW = 716, ZWE = 716,		
		MAXCOUNTRY = 1010, NUMCOUNTRY = 256
	};	
	
	Country( void ): m_country(NOCOUNTRY) {}
	~Country( void ) { m_country = NOCOUNTRY; }
	
	// non-explicit constructors intentional here
	Country( CountryCode i ): m_country(i) {} // i.e. i = Country::GB
	Country( const std::string& s ): m_country(NOCOUNTRY) { setCountry(s); }
	Country( const char *s ): m_country(NOCOUNTRY) { if (s) setCountry(s); } 
    
	// The ISO numeric code for this country i.e. Country::GBR = 826
	operator short( void ) const { return m_country; }
	
	// The ISO 3 letter code for this country i.e. "GBR"
    std::string
    toString( void ) const  { return m_countryNames[m_fromISO[m_country]]; } 

	std::string
	name( void ) const; // i.e "United Kingdom" 
	
	bool
	setCountry( const std::string& s ); // i.e. s = "GBR"
	
	void
	setCountry( const CountryCode s ) { m_country = s; } // i.e. s = Country::GBR

	static Country
	index(const int i)  { return CountryCode(m_toISO[i]); }

    static int
	index(const Country c) { return CountryCode(m_fromISO[c]); }
    
    bool                
	valid( void ) const { return m_country != NOCOUNTRY; }
	
private:
	
	short m_country; // we use short here as it simplifies streaming

	static const char * const m_countryNames[NUMCOUNTRY];
	static const char * const m_fullCountryNames[NUMCOUNTRY];
	static const short        m_toISO[NUMCOUNTRY];
	static const short        m_fromISO[MAXCOUNTRY];    
    static const short        m_searchPoints[27];    
};


std::ostream&
operator<<(std::ostream& ostr, const Country& c);

std::istream&
operator>>(std::istream& istr, Country& c);


#endif






