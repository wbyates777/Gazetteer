/* Currency 10/09/09

        $$$$$$$$$$$$$$$$$$$$$$$$$$$
        $   Currency.h - header   $
        $$$$$$$$$$$$$$$$$$$$$$$$$$$

   by W.B. Yates    
   Copyright (c) W.B. Yates. All rights reserved 
   History: Supports most of the ISO 4217 currency code list as of 10/10/2023  

   
   Notes
   
     i)   There is no currency ZRZ (the Zaire) as there is no ISO country code for Zaire 
          which ceased to exist in 1997 (see Democratic Republic of the Congo).
       
     ii)  El Salvadore currency number 316 ommited beacuse its 3 letter code (SVC) clashes with new code
       
     iii) The following are not ISO codes and therefore have no ISO currency number, although they are used commercially:
              IMP - Isle of Man Pound; we treat this as GBP 
              JEP - Jersey Pound; we treat this as GBP 
              GGP - Gurnsey Pound; we treat this as GBP
              TVD - Tuvalu Dollar; we treat this as AUD
       
       https://www.currency-iso.org/
       https://en.wikipedia.org/wiki/ISO_4217

   Example 1

   Currency ccy1(Currency::USD);
   std::cout << ccy1 << std::endl;    

   Currency ccy2("USD");
   std::cout << ccy2 << std::endl;

   Currency ccy3("XYZ");
   std::cout << ccy3 << std::endl;

   Currency ccy4(Currency::SVC);
   std::cout << ccy4 << std::endl;    

   Currency ccy5("SVC");
   std::cout << ccy5.name() << std::endl;
   std::cout << ccy5.to3Code() << std::endl;

   Currency ccy6(Currency::SVC);
   std::cout << short(ccy6) << std::endl;

   Currency ccy;
   int iters = 100000;
   for (int i = 0; i < iters; ++i)
   {
      for (int j = 1; j < Currency::NUMCURRENCY; ++j)
      {
         ccy.setCurrency( Currency::index(j).to3Code() );
      }
   }

   exit(1);
 
*/

#ifndef __CURRENCY_H__
#define __CURRENCY_H__

#include <string>
#include <iostream>



class Currency
{
public:

    // The value of the enum elements are the ISO numeric code for each currency. 
    // Note NOCURRENCY, MAXCURRENCY, and NUMCURRENCY are not ISO codes.
    enum CurrencyCode : short {
    NOCURRENCY = 0,
    ADP = 20,  AED = 784, AFA = 4,   AFN = 971, ALL = 8,   AMD = 51,  ANG = 532, AOA = 973, AON = 24,  AOR = 982, 
    ARS = 32,  ATS = 40,  AUD = 36,  AWG = 533, AZM = 31,  AZN = 944, BAD = 70,  BAM = 977, BBD = 52,  BDT = 50, 
    BEC = 993, BEF = 56,  BEL = 992, BGL = 100, BGN = 975, BHD = 48,  BIF = 108, BMD = 60,  BND = 96,  BOB = 68, 
    BOV = 984, BRL = 986, BSD = 44,  BTN = 64,  BWP = 72,  BYN = 933, BYR = 974, BZD = 84,  CAD = 124, CDF = 976, 
    CHC = 948, CHE = 947, CHF = 756, CHW = 948, CLF = 990, CLP = 152, CNY = 156, COP = 170, COU = 970, CRC = 188, 
    CSK = 200, CUC = 931, CUP = 192, CVE = 132, CYP = 196, CZK = 203, DDM = 278, DEM = 276, DJF = 262, DKK = 208, 
    DOP = 214, DZD = 12,  ECS = 218, EEK = 233, EGP = 818, ERN = 232, ESA = 996, ESB = 995, ESP = 724, ETB = 230, 
    EUR = 978, FIM = 246, FJD = 242, FKP = 238, FRF = 250, GBP = 826, GEL = 981, GHS = 936, GIP = 292, GMD = 270, 
    GNF = 324, GRD = 300, GTQ = 320, GWP = 624, GYD = 328, HKD = 344, HNL = 340, HRK = 191, HTG = 332, HUF = 348, 
    IDR = 360, IEP = 372, ILS = 376, INR = 356, IQD = 368, IRR = 364, ISK = 352, ITL = 380, JMD = 388, JOD = 400, 
    JPY = 392, KES = 404, KGS = 417, KHR = 116, KMF = 174, KPW = 408, KRW = 410, KWD = 414, KYD = 136, KZT = 398, 
    LAK = 418, LBP = 422, LKR = 144, LRD = 430, LSL = 426, LTL = 440, LUF = 442, LVL = 428, LYD = 434, MAD = 504, 
    MDL = 498, MGA = 969, MGF = 450, MKD = 807, MMK = 104, MNT = 496, MOP = 446, MRO = 478, MRU = 929, MTL = 470, 
    MUR = 480, MVR = 462, MWK = 454, MXN = 484, MXV = 979, MYR = 458, MZM = 508, MZN = 943, NAD = 516, NGN = 566, 
    NIO = 558, NLG = 528, NOK = 578, NPR = 524, NZD = 554, OMR = 512, PAB = 590, PEN = 604, PGK = 598, PHP = 608, 
    PKR = 586, PLN = 985, PLZ = 616, PTE = 620, PYG = 600, QAR = 634, ROL = 642, RON = 946, RSD = 941, RUB = 643, 
    RUR = 810, RWF = 646, SAR = 682, SBD = 90,  SCR = 690, SDD = 736, SDG = 938, SEK = 752, SGD = 702, SHP = 654, 
    SIT = 705, SKK = 703, SLE = 925, SLL = 694, SOS = 706, SRD = 968, SRG = 740, SSP = 728, STD = 678, STN = 930, 
    SVC = 222, SYP = 760, SZL = 748, THB = 764, TJR = 762, TJS = 972, TMM = 795, TMT = 934, TND = 788, TOP = 776, 
    TPE = 626, TRL = 792, TRY = 949, TTD = 780, TWD = 901, TZS = 834, UAH = 980, UAK = 804, UGX = 800, USD = 840, 
    USN = 997, USS = 998, UYI = 940, UYU = 858, UYW = 927, UZS = 860, VEB = 862, VED = 926, VEF = 937, VES = 928, 
    VND = 704, VUV = 548, WST = 882, XAF = 950, XAG = 961, XAU = 959, XBA = 955, XBB = 956, XBC = 957, XBD = 958, 
    XCD = 951, XDR = 960, XOF = 952, XPD = 964, XPF = 953, XPT = 962, XSU = 994, XTS = 963, XUA = 965, XXX = 999, 
    YDD = 720, YER = 886, YUD = 890, YUM = 891, ZAL = 991, ZAR = 710, ZMK = 894, ZMW = 967, ZRN = 180, ZWD = 716, 
    ZWL = 932, 
    MAXCURRENCY = 1000, NUMCURRENCY = 242 
    };
    
    Currency( void ): m_ccy(m_baseCurrency) {}
    ~Currency( void ) { m_ccy = NOCURRENCY; }
    
    // non-explicit constructors intentional here
    Currency( CurrencyCode i ): m_ccy(i) {} // e.g. i = Currency::GBP
    Currency( const std::string &s ): m_ccy(NOCURRENCY) { setCurrency(s); }
    Currency( const char *s ): m_ccy(NOCURRENCY) { if (s) setCurrency(s); } 

    // The ISO numeric code for this currency e.g. Currency::GBP = 826 
    operator short( void ) const { return m_ccy; }

    // The ISO 3 letter code for this currency e.g. "GBP"
    std::string
    to3Code( void ) const { return m_codes[m_fromISO[m_ccy]]; }
        
    // e.g. s = "GBP"
    bool
    setCurrency( const std::string &s ); 

    // e.g. s = Currency::GBP
    void
    setCurrency( CurrencyCode s ) { m_ccy = s; } 

    std::string
    name( void ) const  { return m_fullNames[m_fromISO[m_ccy]]; }  // e.g. "Pound Sterling"

    static Currency
    baseCurrency( void ) { return m_baseCurrency; }
    
    static void // not thread safe
    baseCurrency( const Currency &c ) { m_baseCurrency = c; }

    static Currency
    index( int i ) { return CurrencyCode(m_toISO[i]); }    

    static int
    index( const Currency &c ) { return m_fromISO[c]; }
    
    bool                
    valid( void ) const { return m_ccy != NOCURRENCY; }
    
private:
    
    short m_ccy; 
    
    static const short m_search[28]; 
    static const short m_midPoints[26]; 
    static const short m_fromISO[MAXCURRENCY]; 
    static const short m_toISO[NUMCURRENCY]; 
    static const char * const m_codes[NUMCURRENCY];
    static const char * const m_fullNames[NUMCURRENCY];
    
    static Currency m_baseCurrency;
};


std::ostream&
operator<<( std::ostream &ostr, const Currency &c );

std::istream&
operator>>( std::istream &istr, Currency &c );


#endif


