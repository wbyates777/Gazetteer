/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 
 History: 
 
 Identifies a city (not an airport) using a large part of the IATA airport code list. 
 
 This class can represent 1980 distinct cities - most major cities of the world including capital cities and US state capitals.
 This is enough to describe every city associated with a Market Identification Code (MIC).
 
 Each IATA code corresponds to the city served by that airport. 

 https://www.iata.org/en/publications/directories/code-search/
 https://en.wikipedia.org/wiki/IATA_airport_code
 
 Latitude and longitude of cities (not airports) is provided for geograpical positioning.
 
 For cities that do not have a suitable IATA code we use an invented city3code terminated by a '0' (see table below).
 
 This class supports the 5 digit UN/LOCODE for most cities. For the 7 cities without a UN/LOCODE we have 
 used following LOCODES. The city 'No City' has also been assigned the code XXXXX.
 
 > select city, country2code, city3code, locode from city where locodenumber = -1;
 +-------------------+--------------+-----------+--------+
 | city              | country2code | city3code | locode |
 +-------------------+--------------+-----------+--------+
 | GIFT City         | IN           | GC0       | XXX    |
 | King Edward Point | GS           | GS0       | XXX    |
 | Jerusalem         | IL           | JRS       | XXX    |
 | Pukatawagan       | CA           | XPK       | XXW    |
 | Kuujjuarapik      | CA           | YGW       | XXX    |
 | Lac Brochet       | CA           | YLB       | XXY    |
 | Umiujaq           | CA           | YUD       | XXZ    |
 +-------------------+--------------+-----------+--------+
 | No City           | XX           | XXX       | XXX    |
 +-------------------+--------------+-----------+--------+
 
 Note that symbols [country2code]XXX are valid location codes while the symbol XXXXX is not an official UN/LOCODE
 
 https://unece.org/trade/uncefact/unlocode 
 https://en.wikipedia.org/wiki/UN/LOCODE 
 
 City names have been harmonised with UN/LOCODE names and now have hyphens and apostrophes i.e "Val-d'Or" 
 but not diacritic signs (accents). Some names have been anglicised i.e. "Muenchen" -> "Munich". 

 Each city also has an IANA time zone database identification string. The IANA Time Zone Database 
 (often called tz or zoneinfo) contains indexed information about the world's time zones and daylight saving 
 time rules. This facilites the calculation of accurate 'local time' for each city.

 https://www.iana.org/time-zones
 https://github.com/eggert/tz
 
 
 Note: List of cities that do not have a suitable IATA code:
 
 > select city, country2code, city3code  from city where city3code like '%0' order by city;
 +-------------------+--------------+-----------+
 | city              | country2code | city3code |
 +-------------------+--------------+-----------+
 | Adamstown         | PN           | PN0       |
 | Aylesbury         | GB           | AY0       |
 | Beulah            | US           | BL0       |
 | Center            | US           | CT0       |
 | Ebene             | MU           | EB0       |
 | Ecatepec          | MX           | EP0       |
 | Erdenet           | MN           | ER0       |
 | Esch-sur-Alzette  | LU           | EA0       |
 | Espoo             | FI           | ES0       |
 | Foster City       | US           | FC0       |
 | Gandhinagar       | IN           | GH0       |
 | GIFT City         | IN           | GC0       |
 | Greenwich         | US           | GW0       |
 | Grindsted         | DK           | GR0       |
 | Horsens           | DK           | HS0       |
 | Hradec Kralove    | CZ           | HK0       |
 | 's-Hertogenbosch  | NL           | SH0       |
 | King Edward Point | GS           | GS0       |
 | Lapu-Lapu         | PH           | LL0       |
 | Leuven            | BE           | LV0       |
 | Lucerne           | CH           | LU0       |
 | Mill Valley       | US           | MV0       |
 | Monticello        | US           | MC0       |
 | Nabulus           | PS           | PS0       |
 | New Salem         | US           | NS0       |
 | Nukunonu          | TK           | TK0       |
 | Oldenburg         | DE           | OB0       |
 | Pasig City        | PH           | PC0       |
 | Petersburg        | US           | PB0       |
 | Port-aux-Francais | TF           | TF0       |
 | Porto Acre        | BR           | PA0       |
 | Porto-Novo        | BJ           | PT0       |
 | Rainy River       | CA           | RR0       |
 | Randers           | DK           | RA0       |
 | Regensburg        | DE           | RB0       |
 | Rustavi           | GE           | RS0       |
 | San Marino City   | SM           | SM0       |
 | Schwerin          | DE           | SW0       |
 | Serekunda         | GM           | SK0       |
 | Silkeborg         | DK           | SI0       |
 | Stamford          | US           | ST0       |
 | Tell City         | US           | TC0       |
 | Vaduz             | LI           | LI0       |
 | Varazdin          | HR           | VZ0       |
 | Vatican City      | VA           | VA0       |
 | Vevay             | US           | VY0       |
 | Vienna            | US           | VN0       |
 | Winamac           | US           | WN0       |
 | Winter Park       | US           | WP0       |
 | Zug               | CH           | ZG0       |
 +-------------------+--------------+-----------+
 
 Example
 
 City x;
 x.setCity( "USNYC" );
 std::cout << x << std::endl;
 std::cout << x.name() << std::endl;
 std::cout << x.to3Code() << std::endl;
 std::cout << x.locode() << std::endl;
 std::cout << x.timezone() << std::endl;
 std::cout << short(x) << std::endl;
 std::cout << x.capital() << std::endl;
 std::cout << "latitude = " << x.lat() << std::endl;
 std::cout << "longitude = " << x.lon() << std::endl;
 
 City y;
 y.setCity( "LON" );
 std::cout << y << std::endl;
 std::cout << y.name() << std::endl;
 std::cout << y.to3Code() << std::endl;
 std::cout << y.locode() << std::endl;
 std::cout << y.timezone() << std::endl;
 std::cout << short(y) << std::endl;
 std::cout << y.capital() << std::endl;
 std::cout << "latitude = " << y.lat() << std::endl;
 std::cout << "longitude = " << y.lon() << std::endl;
 
 for (int i = 0; i < City::NUMCITY; ++i)
 {    
    std::cout << City::index(i) << " " << City::index(i).name() << std::endl;
 }
 

 exit(1);
 
 */


#ifndef __CITY_H__
#define __CITY_H__

#include <string>
#include <iostream>

#undef NAN // There is a CityCode 'NAN'

class City
{
public:
    
    // The value of the enum elements are my arbitrary numeric code for each city. 
    // Note XXX, XXXXX, NOCITY, MAXCITY, NUMCITY and any city codes that terminates in a '0' are not IATA or UN codes.
    enum CityCode : short { NOCITY = 0, 
        AAB = 1, DKAAB = 1, AAC = 2, EGAAC = 2, AAE = 3, DZAAE = 3, AAL = 4, DKAAL = 4, AAN = 5, AEAAN = 5, AAR = 6, DKAAR = 6, AAT = 7, CNAAT = 7, ABD = 8, IRABD = 8, ABE = 9, USAWN = 9, ABI = 10, USABI = 10, ABJ = 11, CIABJ = 11, 
        ABM = 12, AUABM = 12, ABQ = 13, USABQ = 13, ABR = 14, USABR = 14, ABS = 15, EGABS = 15, ABV = 16, NGABV = 16, ABX = 17, AUABX = 17, ABY = 18, USALB = 18, ABZ = 19, GBABD = 19, ACA = 20, MXACA = 20, ACC = 21, GHACC = 21, ACE = 22, ESACE = 22, 
        ACH = 23, CHATR = 23, ACI = 24, GGACI = 24, ACK = 25, USACK = 25, ACT = 26, USACT = 26, ACV = 27, USEKA = 27, ACY = 28, USAIY = 28, ADA = 29, TRADA = 29, ADD = 30, ETADD = 30, ADE = 31, YEADE = 31, ADF = 32, TRADI = 32, ADK = 33, USAXK = 33, 
        ADL = 34, AUADL = 34, ADQ = 35, USADQ = 35, ADY = 36, ZAADY = 36, ADZ = 37, COSAC = 37, AEH = 38, TDAEH = 38, AER = 39, RUAER = 39, AES = 40, NOAES = 40, AGA = 41, MAAGA = 41, AGB = 42, DEAGB = 42, AGP = 43, ESAGP = 43, AGS = 44, USAGS = 44, 
        AGU = 45, MXAGU = 45, AGZ = 46, ZAAGZ = 46, AHB = 47, SAKMX = 47, AHN = 48, USAHN = 48, AHO = 49, ITAHO = 49, AHU = 50, MAAHU = 50, AJA = 51, FRAJA = 51, AJF = 52, SAAJF = 52, AJN = 53, KMAJN = 53, AJU = 54, BRAJU = 54, AJY = 55, NEAJY = 55, 
        AKL = 56, NZAKL = 56, AKN = 57, USAKN = 57, AKT = 58, CYAKT = 58, ALA = 59, KZALA = 59, ALB = 60, USABY = 60, ALC = 61, ESALC = 61, ALF = 62, NOALF = 62, ALG = 63, DZALG = 63, ALH = 64, AUALH = 64, ALJ = 65, ZAALJ = 65, ALO = 66, USALO = 66, 
        ALP = 67, SYALP = 67, ALV = 68, ADALV = 68, ALW = 69, USALW = 69, ALY = 70, EGALY = 70, AMA = 71, USAMA = 71, AMD = 72, INAMD = 72, AMM = 73, JOAMM = 73, AMS = 74, NLAMS = 74, ANB = 75, USANB = 75, ANC = 76, USANC = 76, ANK = 77, TRANK = 77, 
        ANP = 78, USANP = 78, ANR = 79, BEANR = 79, ANU = 80, AGSJO = 80, AOI = 81, ITAOI = 81, AOJ = 82, JPAOJ = 82, AOK = 83, GRAOK = 83, AOO = 84, USAOO = 84, APF = 85, USAPF = 85, APW = 86, WSAPW = 86, AQJ = 87, JOAQJ = 87, ARB = 88, USARB = 88, 
        ARH = 89, RUARH = 89, ARK = 90, TZARK = 90, ASB = 91, TMAGT = 91, ASE = 92, USASE = 92, ASF = 93, RUASF = 93, ASI = 94, SHSHN = 94, ASJ = 95, JPASJ = 95, ASK = 96, CIASK = 96, ASM = 97, ERASM = 97, ASP = 98, AUASP = 98, ASR = 99, TRASR = 99, 
        ASU = 100, PYASU = 100, ASW = 101, EGASW = 101, ATH = 102, GRATH = 102, ATL = 103, USATL = 103, ATO = 104, USATO = 104, ATQ = 105, INATQ = 105, ATW = 106, USATW = 106, ATY = 107, USATY = 107, ATZ = 108, EGATZ = 108, AUA = 109, AWORJ = 109, AUE = 110, EGAUE = 110, 
        AUG = 111, USAUG = 111, AUH = 112, AEAUH = 112, AUR = 113, FRAUR = 113, AUS = 114, USAUS = 114, AUX = 115, BRAIN = 115, AVL = 116, USAEV = 116, AVP = 117, USWBE = 117, AXA = 118, AIVAL = 118, AXS = 119, USLTS = 119, AXT = 120, JPAXT = 120, AY0 = 121, GBAYL = 121, 
        AYQ = 122, AUAYQ = 122, AYR = 123, AUAYR = 123, AYT = 124, TRAYT = 124, AYW = 125, IDAYW = 125, AZB = 126, PGAZB = 126, AZO = 127, USAZO = 127, AZS = 128, DOAZS = 128, BAH = 129, BHAMH = 129, BAK = 130, AZBAK = 130, BAQ = 131, COBAQ = 131, BBI = 132, INBBI = 132, 
        BBM = 133, KHBBM = 133, BBR = 134, GPBBR = 134, BBT = 135, CFBBT = 135, BBU = 136, ROBUH = 136, BBY = 137, CFBBY = 137, BCN = 138, ESBCN = 138, BCO = 139, ETBCO = 139, BCT = 140, USBCT = 140, BCV = 141, BZBMP = 141, BDA = 142, BMBDA = 142, BDB = 143, AUBDB = 143, 
        BDL = 144, USHFD = 144, BDO = 145, IDBDO = 145, BDQ = 146, INBRC = 146, BDR = 147, USBDR = 147, BDS = 148, ITBDS = 148, BDU = 149, NOBDU = 149, BEB = 150, GBBBC = 150, BEG = 151, RSBEG = 151, BEH = 152, USBEH = 152, BEL = 153, BRBEL = 153, BEN = 154, LYBEN = 154, 
        BER = 155, DEBER = 155, BES = 156, FRBES = 156, BET = 157, USBET = 157, BEW = 158, MZBEW = 158, BEY = 159, LBBEY = 159, BFD = 160, USBFD = 160, BFL = 161, USBFL = 161, BFN = 162, ZABFN = 162, BFO = 163, ZWBFO = 163, BFS = 164, GBBEL = 164, BGA = 165, COBGA = 165, 
        BGF = 166, CFBGF = 166, BGI = 167, BBBGI = 167, BGM = 168, USJOY = 168, BGO = 169, NOBGO = 169, BGR = 170, USBGR = 170, BGU = 171, CFBGU = 171, BGW = 172, IQBGW = 172, BGY = 173, ITBGO = 173, BHE = 174, NZBHE = 174, BHM = 175, USBHM = 175, BHO = 176, INBHO = 176, 
        BHQ = 177, AUBHQ = 177, BHR = 178, NPBHR = 178, BHV = 179, PKBHV = 179, BHX = 180, GBBHM = 180, BIA = 181, FRBIA = 181, BIL = 182, USBIL = 182, BIO = 183, ESBIO = 183, BIQ = 184, FRBIQ = 184, BIS = 185, USBIS = 185, BIV = 186, CFBIV = 186, BJI = 187, USBJI = 187, 
        BJL = 188, GMBJL = 188, BJM = 189, BIBJM = 189, BJS = 190, CNBJS = 190, BJV = 191, TRBXN = 191, BJX = 192, MXLEN = 192, BJZ = 193, ESBJZ = 193, BKI = 194, MYBKI = 194, BKK = 195, THBKK = 195, BKO = 196, MLBKO = 196, BKW = 197, USBKW = 197, BKX = 198, USBKX = 198, 
        BL0 = 199, USXUL = 199, BLA = 200, VEBLA = 200, BLF = 201, USAFL = 201, BLI = 202, USBLI = 202, BLK = 203, GBBLK = 203, BLL = 204, DKBLL = 204, BLQ = 205, ITBLQ = 205, BLR = 206, INBLR = 206, BLT = 207, AUBLT = 207, BLZ = 208, MWBLZ = 208, BME = 209, AUBME = 209, 
        BMG = 210, USBMG = 210, BMI = 211, USB2L = 211, BMP = 212, AUBMP = 212, BNA = 213, USBNA = 213, BNE = 214, AUBNE = 214, BNJ = 215, DEBON = 215, BNK = 216, AUBNK = 216, BNN = 217, NOBNN = 217, BNP = 218, PKBNP = 218, BNX = 219, BABNX = 219, BOB = 220, PFBOB = 220, 
        BOD = 221, FRBOD = 221, BOG = 222, COBOG = 222, BOH = 223, GBBOH = 223, BOI = 224, USBOI = 224, BOJ = 225, BGBOJ = 225, BOM = 226, INBOM = 226, BON = 227, BQKRA = 227, BOO = 228, NOBOO = 228, BOS = 229, USBOS = 229, BOY = 230, BFBOY = 230, BPT = 231, USBPT = 231, 
        BQK = 232, USSSI = 232, BQN = 233, PRBQN = 233, BRC = 234, ARBRC = 234, BRD = 235, USBRD = 235, BRE = 236, DEBRE = 236, BRI = 237, ITBRI = 237, BRL = 238, USBRL = 238, BRN = 239, CHBRN = 239, BRQ = 240, CZBRQ = 240, BRR = 241, GBBRR = 241, BRS = 242, GBBRS = 242, 
        BRU = 243, BEBRU = 243, BSB = 244, BRBSB = 244, BSG = 245, GQBSG = 245, BSL = 246, CHBSL = 246, BSR = 247, IQBSR = 247, BTM = 248, USBTM = 248, BTQ = 249, RWBTQ = 249, BTR = 250, USBTR = 250, BTS = 251, SKBTS = 251, BTU = 252, MYBTU = 252, BTV = 253, USBTV = 253, 
        BUA = 254, PGBUA = 254, BUD = 255, HUBUD = 255, BUE = 256, ARBUE = 256, BUF = 257, USBUF = 257, BUG = 258, AOBUG = 258, BUQ = 259, ZWBUQ = 259, BUR = 260, USBUR = 260, BUS = 261, GEBUS = 261, BVB = 262, BRBVB = 262, BWI = 263, USBAL = 263, BWN = 264, BNBWN = 264, 
        BWT = 265, AUBWT = 265, BXS = 266, USBXS = 266, BYK = 267, CIBYK = 267, BYU = 268, DEBYU = 268, BZE = 269, BZBZE = 269, BZL = 270, BDBZL = 270, BZN = 271, USBZN = 271, BZV = 272, CGBZV = 272, CAB = 273, AOCAB = 273, CAE = 274, USCAE = 274, CAG = 275, ITCAG = 275, 
        CAI = 276, EGCAI = 276, CAK = 277, USAKH = 277, CAL = 278, GBCBT = 278, CAN = 279, CNGGZ = 279, CAY = 280, GFCAY = 280, CBB = 281, BOCBB = 281, CBG = 282, GBCMG = 282, CBR = 283, AUCBR = 283, CBU = 284, DECOT = 284, CCJ = 285, INCCJ = 285, CCK = 286, CCCCK = 286, 
        CCR = 287, USCCR = 287, CCS = 288, VECCS = 288, CCU = 289, INCCU = 289, CCZ = 290, BSCCZ = 290, CDC = 291, USCDC = 291, CDV = 292, USCDV = 292, CEB = 293, PHCEB = 293, CEC = 294, USCEC = 294, CED = 295, AUCED = 295, CEN = 296, MXCEN = 296, CEQ = 297, FRCEQ = 297, 
        CES = 298, AUCES = 298, CFE = 299, FRCFE = 299, CFG = 300, CUCFG = 300, CFN = 301, IECFN = 301, CFS = 302, AUCFS = 302, CFU = 303, GRCFU = 303, CGA = 304, USCGA = 304, CGB = 305, BRCBA = 305, CGN = 306, DECGN = 306, CGO = 307, CNZGZ = 307, CGP = 308, BDCGP = 308, 
        CGQ = 309, CNCGC = 309, CGR = 310, BRCGR = 310, CGU = 311, VECGU = 311, CHA = 312, USCHA = 312, CHC = 313, NZCHC = 313, CHG = 314, CNCYN = 314, CHI = 315, USCHI = 315, CHO = 316, USCHO = 316, CHQ = 317, GRCHQ = 317, CHS = 318, USCHS = 318, CHT = 319, NZWTG = 319, 
        CIC = 320, USCIC = 320, CID = 321, USCID = 321, CIP = 322, ZMCIP = 322, CIW = 323, VCCAN = 323, CJB = 324, INCJB = 324, CJC = 325, CLCJC = 325, CJL = 326, PKCJL = 326, CJS = 327, MXCJS = 327, CKB = 328, USCKB = 328, CKG = 329, CNCQI = 329, CKY = 330, GNCKY = 330, 
        CLD = 331, USCLD = 331, CLE = 332, USCLE = 332, CLJ = 333, ROCLJ = 333, CLL = 334, USCLL = 334, CLM = 335, USCLM = 335, CLO = 336, COCLO = 336, CLQ = 337, MXCOL = 337, CLT = 338, USCLT = 338, CLY = 339, FRCLY = 339, CMB = 340, LKCMB = 340, CME = 341, MXCME = 341, 
        CMF = 342, FRCMF = 342, CMH = 343, USCMH = 343, CMI = 344, USCMI = 344, CMN = 345, MACAS = 345, CMQ = 346, AUCMQ = 346, CMX = 347, USCMX = 347, CND = 348, ROCND = 348, CNF = 349, BRBHZ = 349, CNQ = 350, ARCNQ = 350, CNS = 351, AUCNS = 351, CNX = 352, THCNX = 352, 
        COD = 353, USCOD = 353, COK = 354, INCOK = 354, CON = 355, USCON = 355, COO = 356, BJCOO = 356, COQ = 357, MNCOQ = 357, COR = 358, ARCOR = 358, COS = 359, USCOS = 359, CPD = 360, AUCPD = 360, CPH = 361, DKCPH = 361, CPR = 362, USCPR = 362, CPT = 363, ZACPT = 363, 
        CRD = 364, ARCRD = 364, CRF = 365, CFCRF = 365, CRG = 366, USJAX = 366, CRP = 367, USCRP = 367, CRW = 368, USCRW = 368, CSG = 369, USCSG = 369, CSI = 370, AUCSI = 370, CSN = 371, USCSN = 371, CST = 372, FJCST = 372, CT0 = 373, USZT8 = 373, CTA = 374, ITCTA = 374, 
        CTC = 375, ARCTC = 375, CTG = 376, COCTG = 376, CTN = 377, AUCTN = 377, CTU = 378, CNCDU = 378, CUL = 379, MXCUL = 379, CUN = 380, MXCUN = 380, CUR = 381, CWWIL = 381, CUU = 382, MXCUU = 382, CVG = 383, USCVG = 383, CVM = 384, MXCVM = 384, CVQ = 385, AUCVQ = 385, 
        CVT = 386, GBCVT = 386, CWA = 387, USAUW = 387, CWB = 388, BRCWB = 388, CWL = 389, GBCDF = 389, CXI = 390, KICXI = 390, CXT = 391, AUCXT = 391, CYS = 392, USCYS = 392, CYU = 393, PHCYU = 393, CZA = 394, MXCZA = 394, CZL = 395, DZCZL = 395, CZM = 396, MXCZM = 396, 
        DAB = 397, USDAB = 397, DAC = 398, BDDAC = 398, DAL = 399, USDAL = 399, DAM = 400, SYDAM = 400, DAN = 401, USDAN = 401, DAR = 402, TZDAR = 402, DAY = 403, USDAY = 403, DBO = 404, AUDBO = 404, DBQ = 405, USDBQ = 405, DBV = 406, HRDBV = 406, DBY = 407, AUDBY = 407, 
        DCA = 408, USWAS = 408, DCF = 409, DMRSU = 409, DDI = 410, AUDDI = 410, DEC = 411, USDEC = 411, DEL = 412, INICD = 412, DEN = 413, USDEN = 413, DET = 414, USDET = 414, DHA = 415, SADHA = 415, DHN = 416, USDHN = 416, DIL = 417, TLDIL = 417, DJB = 418, IDDJB = 418, 
        DJE = 419, TNDJE = 419, DJJ = 420, IDDJJ = 420, DJO = 421, CIDJO = 421, DKI = 422, AUDKI = 422, DKR = 423, SNDKR = 423, DLA = 424, CMDLA = 424, DLC = 425, CNDAL = 425, DLG = 426, USDLG = 426, DLH = 427, USDLH = 427, DLM = 428, TRDLM = 428, DMM = 429, SADMM = 429, 
        DND = 430, GBDUN = 430, DNK = 431, UADNK = 431, DNR = 432, FRDNR = 432, DNZ = 433, TRDNZ = 433, DOD = 434, TZDOD = 434, DOH = 435, QADOH = 435, DOM = 436, DMDOM = 436, DOV = 437, USXEZ = 437, DPO = 438, AUDPO = 438, DPS = 439, IDDPS = 439, DRB = 440, AUDRB = 440, 
        DRO = 441, USDRO = 441, DRS = 442, DEDRS = 442, DRW = 443, AUDRW = 443, DSK = 444, PKDSK = 444, DSM = 445, USDSM = 445, DTM = 446, DEDTM = 446, DUB = 447, IEDUB = 447, DUD = 448, NZDUD = 448, DUJ = 449, USDUJ = 449, DUR = 450, ZADUR = 450, DUS = 451, DEDUS = 451, 
        DUT = 452, USDUT = 452, DVL = 453, USDVL = 453, DXB = 454, AEDXB = 454, DYA = 455, AUDYA = 455, DYU = 456, TJDYU = 456, DZA = 457, YTMAM = 457, EA0 = 458, LUEZT = 458, EAS = 459, ESEAS = 459, EAT = 460, USEAT = 460, EAU = 461, USEAU = 461, EB0 = 462, MUEBN = 462, 
        EBA = 463, ITEBA = 463, EBB = 464, UGEBB = 464, EBJ = 465, DKEBJ = 465, EBU = 466, FREBU = 466, EDI = 467, GBEDI = 467, EGC = 468, FREGC = 468, EGE = 469, USVAC = 469, EGS = 470, ISEGS = 470, EIN = 471, NLEIN = 471, EIS = 472, VGEIS = 472, EKI = 473, USEKI = 473, 
        EKO = 474, USEKO = 474, ELH = 475, BSELH = 475, ELL = 476, ZAELL = 476, ELM = 477, USELM = 477, ELP = 478, USELP = 478, ELS = 479, ZAELS = 479, ELY = 480, USELY = 480, EMA = 481, GBNTG = 481, EMD = 482, AUEMD = 482, EMY = 483, EGEMY = 483, ENA = 484, USENA = 484, 
        ENC = 485, FRENC = 485, ENF = 486, FIENF = 486, EP0 = 487, MXEPC = 487, EPR = 488, AUEPR = 488, ER0 = 489, MNERT = 489, ERC = 490, TRERC = 490, ERF = 491, DEERF = 491, ERI = 492, USERI = 492, ERN = 493, BRERN = 493, ERZ = 494, TRERZ = 494, ES0 = 495, FIES2 = 495, 
        ESC = 496, USESC = 496, ESF = 497, USESF = 497, ESH = 498, GBBSH = 498, ESR = 499, CLESR = 499, ETH = 500, ILETH = 500, EUG = 501, USEUG = 501, EUN = 502, EHEUN = 502, EVE = 503, NOEVE = 503, EVN = 504, AMEVN = 504, EVV = 505, USEVV = 505, EWN = 506, USEWN = 506, 
        EWR = 507, USJEC = 507, EXT = 508, GBEXE = 508, EYW = 509, USEYW = 509, FAE = 510, FOTOR = 510, FAI = 511, USFAI = 511, FAO = 512, PTFAO = 512, FAR = 513, USFAR = 513, FAT = 514, USFAT = 514, FAY = 515, USFBG = 515, FBM = 516, CDFBM = 516, FC0 = 517, USFCI = 517, 
        FCA = 518, USFCA = 518, FDF = 519, MQFDF = 519, FDH = 520, DEFDH = 520, FEZ = 521, MAFEZ = 521, FFT = 522, USFFT = 522, FHU = 523, USFHU = 523, FIE = 524, GBFIE = 524, FIH = 525, CDFIH = 525, FJR = 526, AEFJR = 526, FKB = 527, DEKAE = 527, FKI = 528, CDFKI = 528, 
        FKL = 529, USFKL = 529, FKS = 530, JPFKF = 530, FLG = 531, USFLG = 531, FLL = 532, USFLL = 532, FLN = 533, BRFLN = 533, FLO = 534, USFLO = 534, FLR = 535, ITFLR = 535, FMN = 536, USFMN = 536, FMO = 537, DEOSN = 537, FMY = 538, USFMY = 538, FNA = 539, SLFNA = 539, 
        FNC = 540, PTFNC = 540, FNI = 541, FRFNI = 541, FNJ = 542, KPFNJ = 542, FNT = 543, USFNT = 543, FOD = 544, USFOD = 544, FOE = 545, USTOP = 545, FOR = 546, BRFOR = 546, FOU = 547, GBFOA = 547, FPO = 548, BSFPO = 548, FRA = 549, DEFRA = 549, FRI = 550, USFRI = 550, 
        FRJ = 551, FRFRJ = 551, FRO = 552, NOFRO = 552, FRU = 553, KGFRU = 553, FRW = 554, BWFRW = 554, FSC = 555, FRFSC = 555, FSD = 556, USFSD = 556, FSM = 557, USFSM = 557, FSP = 558, PMFSP = 558, FUE = 559, ESFUE = 559, FUK = 560, JPFUK = 560, FUN = 561, TVFUN = 561, 
        FUT = 562, WFFUT = 562, FWA = 563, USFWA = 563, FYV = 564, USFYV = 564, GAD = 565, USGAD = 565, GAJ = 566, JPGAJ = 566, GAU = 567, INGAW = 567, GBE = 568, BWGBE = 568, GC0 = 569, INXXX = 569, GCC = 570, USGCC = 570, GCI = 571, GGSPT = 571, GCM = 572, KYGCM = 572, 
        GCN = 573, USGCN = 573, GDL = 574, MXGDL = 574, GDN = 575, PLGDN = 575, GDT = 576, TCGDT = 576, GDV = 577, USGDV = 577, GEG = 578, USGEG = 578, GEO = 579, GYGEO = 579, GET = 580, AUGET = 580, GEX = 581, AUGEX = 581, GFF = 582, AUGFF = 582, GFK = 583, USGFK = 583, 
        GGG = 584, USZLV = 584, GGW = 585, USGSG = 585, GH0 = 586, INGDH = 586, GHB = 587, BSGHB = 587, GIB = 588, GIGIB = 588, GID = 589, BIGID = 589, GIL = 590, PKGIL = 590, GJL = 591, DZGJL = 591, GJT = 592, USGJT = 592, GKL = 593, AUGKL = 593, GLA = 594, GBGLW = 594, 
        GLH = 595, USGLH = 595, GLT = 596, AUGLT = 596, GNB = 597, FRGNB = 597, GND = 598, GDGND = 598, GNE = 599, BEGNE = 599, GNV = 600, USGNV = 600, GOA = 601, ITGOA = 601, GOH = 602, GLGOH = 602, GOI = 603, INGOI = 603, GOJ = 604, RUGOJ = 604, GON = 605, USZGO = 605, 
        GOO = 606, AUGOO = 606, GOT = 607, SEGOT = 607, GOU = 608, CMGOU = 608, GOV = 609, AUGOV = 609, GOZ = 610, BGGOZ = 610, GPA = 611, GRARX = 611, GPT = 612, USGPT = 612, GPZ = 613, USGPZ = 613, GR0 = 614, DKGRN = 614, GRB = 615, USGRB = 615, GRJ = 616, ZAGRJ = 616, 
        GRO = 617, ESGRO = 617, GRQ = 618, NLGRQ = 618, GRR = 619, USGRR = 619, GRX = 620, ESJAE = 620, GRZ = 621, ATGRZ = 621, GS0 = 622, GSXXX = 622, GSI = 623, SBGSI = 623, GSO = 624, USGBO = 624, GSP = 625, USGV9 = 625, GTE = 626, AUGTE = 626, GTF = 627, USGTF = 627, 
        GTI = 628, DEQGT = 628, GUA = 629, GTGUA = 629, GUC = 630, USGUC = 630, GUM = 631, GUHGT = 631, GVA = 632, CHGVA = 632, GW0 = 633, USGRH = 633, GWD = 634, PKGWD = 634, GWE = 635, ZWGWE = 635, GWT = 636, DEGWT = 636, GWY = 637, IEGWY = 637, GYE = 638, ECGYE = 638, 
        GYI = 639, RWGYI = 639, GYN = 640, BRGYN = 640, GYP = 641, AUGYP = 641, GZA = 642, PSGZA = 642, GZT = 643, TRGZT = 643, HAC = 644, JPHAC = 644, HAG = 645, NLHAG = 645, HAH = 646, KMYVA = 646, HAJ = 647, DEHAJ = 647, HAM = 648, DEHAM = 648, HAN = 649, VNHAN = 649, 
        HAR = 650, USHAR = 650, HAU = 651, NOHAU = 651, HAV = 652, CUHAV = 652, HBA = 653, AUHBA = 653, HDD = 654, PKHDD = 654, HDN = 655, USSBS = 655, HDY = 656, THHDY = 656, HEA = 657, AFHEA = 657, HEL = 658, FIHEL = 658, HER = 659, GRHER = 659, HFA = 660, ILHFA = 660, 
        HFT = 661, NOHFT = 661, HGA = 662, SOHGA = 662, HGH = 663, CNHAZ = 663, HGO = 664, CIHGO = 664, HHH = 665, USHHH = 665, HIB = 666, USIBB = 666, HII = 667, USHII = 667, HIJ = 668, JPHIJ = 668, HIR = 669, SBHIR = 669, HIS = 670, AUHIS = 670, HK0 = 671, CZHKR = 671, 
        HKD = 672, JPHKD = 672, HKG = 673, HKHKG = 673, HKT = 674, THHKT = 674, HKY = 675, USHKY = 675, HLA = 676, ZAHLA = 676, HLH = 677, CNHLH = 677, HLN = 678, USHLN = 678, HLT = 679, AUHLT = 679, HLZ = 680, NZHLZ = 680, HMO = 681, MXHMO = 681, HNA = 682, JPMRW = 682, 
        HNH = 683, USHNH = 683, HNK = 684, AUHNK = 684, HNL = 685, USHNL = 685, HNS = 686, USHNS = 686, HOG = 687, CUHOG = 687, HOM = 688, USHOM = 688, HON = 689, USHON = 689, HOQ = 690, DEHOQ = 690, HOR = 691, PTHOR = 691, HOU = 692, USHOU = 692, HPN = 693, USWTS = 693, 
        HRB = 694, CNHRN = 694, HRE = 695, ZWHRE = 695, HRG = 696, EGHRG = 696, HRK = 697, UAHRK = 697, HRL = 698, USHRL = 698, HS0 = 699, DKHOR = 699, HSV = 700, USHSV = 700, HTA = 701, RUHTA = 701, HTI = 702, AUHTI = 702, HTS = 703, USHNU = 703, HTV = 704, USHTV = 704, 
        HUF = 705, USHUF = 705, HUH = 706, PFHUH = 706, HUI = 707, VNHUI = 707, HUX = 708, MXHTO = 708, HUY = 709, GBHUY = 709, HVB = 710, AUHVB = 710, HVN = 711, USHVN = 711, HVR = 712, USHVR = 712, HWN = 713, ZWHWN = 713, HYA = 714, USHYA = 714, HYD = 715, INHYD = 715, 
        HYG = 716, USHYG = 716, IAG = 717, USIAG = 717, IBZ = 718, ESIBZ = 718, ICN = 719, KRINC = 719, ICT = 720, USICT = 720, IDA = 721, USIDA = 721, IDR = 722, INIDR = 722, IEV = 723, UAIEV = 723, IFL = 724, AUIFL = 724, IFP = 725, USIFP = 725, IGH = 726, AUIGH = 726, 
        IGR = 727, ARIGR = 727, IJX = 728, USIJX = 728, IKT = 729, RUIKT = 729, ILE = 730, USILE = 730, ILG = 731, USILG = 731, ILI = 732, USILI = 732, ILM = 733, USILM = 733, ILP = 734, NCILP = 734, ILY = 735, GBISY = 735, ILZ = 736, SKILZ = 736, IND = 737, USIND = 737, 
        INI = 738, RSINI = 738, INL = 739, USINL = 739, INN = 740, ATINN = 740, INU = 741, NRYRN = 741, INV = 742, GBINV = 742, IOM = 743, IMDGS = 743, IOU = 744, NCIOU = 744, IPC = 745, CLIPC = 745, IPL = 746, USIDO = 746, IPT = 747, USIPT = 747, IQT = 748, PEIQT = 748, 
        IRG = 749, AUIRG = 749, IRO = 750, CFIRO = 750, ISA = 751, AUISA = 751, ISB = 752, PKISB = 752, ISG = 753, JPISG = 753, ISL = 754, USISN = 754, ISO = 755, USK5T = 755, ISP = 756, USXHO = 756, IST = 757, TRIST = 757, ITH = 758, USITH = 758, ITO = 759, USITO = 759, 
        IUE = 760, NUALO = 760, IVC = 761, NZIVC = 761, IVL = 762, FIIVL = 762, IXB = 763, INIXB = 763, IXC = 764, INIXC = 764, IXG = 765, INIXG = 765, IXJ = 766, INIXJ = 766, IXR = 767, INIXR = 767, IXW = 768, INIXW = 768, IYK = 769, USIYK = 769, IZM = 770, TRADB = 770, 
        JAA = 771, AFJAA = 771, JAC = 772, USJAC = 772, JAD = 773, AUJAD = 773, JAF = 774, LKJAF = 774, JAG = 775, PKJAG = 775, JAI = 776, INJAI = 776, JAK = 777, HTJAK = 777, JAL = 778, MXJAL = 778, JAM = 779, BGJAM = 779, JAN = 780, USJAN = 780, JAU = 781, PEJAU = 781, 
        JBR = 782, USJBR = 782, JCB = 783, BRJCB = 783, JCK = 784, AUJCK = 784, JCM = 785, BRJCA = 785, JDF = 786, BRJDF = 786, JDH = 787, INJDH = 787, JDZ = 788, CNJDZ = 788, JED = 789, SAJED = 789, JEE = 790, HTJEE = 790, JEF = 791, USJEF = 791, JER = 792, JESTH = 792, 
        JGA = 793, INJGA = 793, JGB = 794, INJGB = 794, JGN = 795, CNJGN = 795, JGS = 796, CNJIN = 796, JHB = 797, MYJHB = 797, JHE = 798, SEHEL = 798, JHG = 799, CNJOG = 799, JHM = 800, USJHM = 800, JHQ = 801, AUJHQ = 801, JHW = 802, USJHW = 802, JIB = 803, DJJIB = 803, 
        JIJ = 804, ETJIJ = 804, JIL = 805, CNJIL = 805, JIM = 806, ETJIM = 806, JIN = 807, UGJIN = 807, JIP = 808, ECJIP = 808, JIR = 809, NPJIR = 809, JIU = 810, CNJIU = 810, JIW = 811, PKJIW = 811, JJI = 812, PEJJI = 812, JJN = 813, CNJJI = 813, JKG = 814, SEJKG = 814, 
        JKH = 815, GRJKH = 815, JKR = 816, NPJKR = 816, JKT = 817, IDJKT = 817, JKV = 818, USJKV = 818, JLN = 819, USJLN = 819, JLR = 820, INJUC = 820, JLS = 821, BRJLS = 821, JMB = 822, AOJMB = 822, JMK = 823, GRJMK = 823, JMO = 824, NPJMO = 824, JMS = 825, USJMS = 825, 
        JMU = 826, CNJMU = 826, JNA = 827, BRJNA = 827, JNB = 828, ZAJNB = 828, JNG = 829, CNJNG = 829, JNI = 830, ARJNI = 830, JNU = 831, USJNU = 831, JNX = 832, GRJNX = 832, JNZ = 833, CNJZI = 833, JOE = 834, FIJOE = 834, JOI = 835, BRJOI = 835, JOL = 836, PHJOL = 836, 
        JON = 837, UMJON = 837, JOS = 838, NGJOS = 838, JPA = 839, BRJPA = 839, JQE = 840, PAJQE = 840, JRH = 841, INJRH = 841, JRO = 842, TZJRO = 842, JRS = 843, ILXXX = 843, JSA = 844, INJSA = 844, JSI = 845, GRJSI = 845, JSM = 846, ARJSM = 846, JST = 847, USJST = 847, 
        JTI = 848, BRJTI = 848, JTR = 849, GRJTR = 849, JUB = 850, SSJUB = 850, JUI = 851, DEJUI = 851, JUJ = 852, ARJUJ = 852, JUL = 853, PEJUL = 853, JUN = 854, AUJUN = 854, JUT = 855, HNJUT = 855, JUV = 856, GLJUV = 856, JVL = 857, USJVL = 857, JWA = 858, BWJWA = 858, 
        JXN = 859, USJXN = 859, JYV = 860, FIJYV = 860, KAJ = 861, FIKAJ = 861, KAN = 862, NGKAN = 862, KAO = 863, FIKAO = 863, KBL = 864, AFKBL = 864, KBY = 865, AUKBY = 865, KCC = 866, USKCC = 866, KCE = 867, AUKCE = 867, KCH = 868, MYKCH = 868, KCM = 869, TRKCM = 869, 
        KCZ = 870, JPKCZ = 870, KDD = 871, PKKDD = 871, KDH = 872, AFKDH = 872, KDU = 873, PKKDU = 873, KEL = 874, DEKEL = 874, KEM = 875, FIKEM = 875, KEN = 876, SLKEN = 876, KGC = 877, AUKGC = 877, KGD = 878, RUKGD = 878, KGI = 879, AUKGI = 879, KGL = 880, RWKGL = 880, 
        KGS = 881, GRKGS = 881, KHH = 882, TWKHH = 882, KHI = 883, PKKHI = 883, KHJ = 884, FIKHJ = 884, KHV = 885, RUKHV = 885, KID = 886, SEKID = 886, KIJ = 887, JPKIJ = 887, KIK = 888, IQKIK = 888, KIM = 889, ZAKIM = 889, KIN = 890, JMKIN = 890, KIR = 891, IEKIR = 891, 
        KIV = 892, MDKIV = 892, KIW = 893, ZMKIW = 893, KKN = 894, NOKKN = 894, KLA = 895, UGKLA = 895, KLR = 896, SEKLR = 896, KLU = 897, ATKLU = 897, KLW = 898, USKLW = 898, KLX = 899, GRKLX = 899, KLZ = 900, ZAKLZ = 900, KMI = 901, JPKMI = 901, KMJ = 902, JPKMJ = 902, 
        KMP = 903, NAKMP = 903, KMQ = 904, JPKMQ = 904, KMS = 905, GHKMS = 905, KNS = 906, AUKNS = 906, KNU = 907, INKNU = 907, KNX = 908, AUKNX = 908, KOA = 909, USKHH = 909, KOI = 910, GBKWL = 910, KOJ = 911, JPKOJ = 911, KOK = 912, FIKOK = 912, KRB = 913, AUKRB = 913, 
        KRK = 914, PLKRK = 914, KRN = 915, SEKRN = 915, KRP = 916, DKKRP = 916, KRS = 917, NOKRS = 917, KRT = 918, SDKRT = 918, KSD = 919, SEKSD = 919, KSL = 920, SDKSL = 920, KSS = 921, MLSIK = 921, KSU = 922, NOKSU = 922, KTA = 923, AUKTA = 923, KTB = 924, USKTB = 924, 
        KTM = 925, NPKTM = 925, KTN = 926, USKTN = 926, KTR = 927, AUKTR = 927, KTT = 928, FIKTT = 928, KUA = 929, MYKUA = 929, KUF = 930, RUKUF = 930, KUH = 931, JPKUH = 931, KUL = 932, MYKUL = 932, KUO = 933, FIKUO = 933, KVA = 934, GRKVA = 934, KWI = 935, KWKWI = 935, 
        KWL = 936, CNGUI = 936, KWM = 937, AUKWM = 937, KYA = 938, TRKYA = 938, KYN = 939, GBMIK = 939, KYS = 940, TRKSY = 940, KZN = 941, RUKZN = 941, LAD = 942, AOLAD = 942, LAE = 943, PGLAE = 943, LAF = 944, USLAF = 944, LAI = 945, FRLAI = 945, LAN = 946, USLAN = 946, 
        LAP = 947, MXLAP = 947, LAR = 948, USLAR = 948, LAS = 949, USLAS = 949, LAW = 950, USLAW = 950, LAX = 951, USLAX = 951, LBA = 952, GBLBA = 952, LBB = 953, USLBB = 953, LBE = 954, USLBE = 954, LBI = 955, FRBBI = 955, LBQ = 956, GALBQ = 956, LBU = 957, MYLBU = 957, 
        LBV = 958, GALBV = 958, LCA = 959, CYLCA = 959, LCG = 960, ESLCG = 960, LCH = 961, USLCH = 961, LDC = 962, AULDC = 962, LDE = 963, FRLDE = 963, LDK = 964, SELDK = 964, LDY = 965, GBLDY = 965, LEB = 966, USLN7 = 966, LED = 967, RULED = 967, LEI = 968, ESLEI = 968, 
        LEJ = 969, DELEJ = 969, LEK = 970, GNLEK = 970, LEQ = 971, GBLEQ = 971, LER = 972, AULER = 972, LEX = 973, USLEX = 973, LEY = 974, NLLEY = 974, LFI = 975, USYGZ = 975, LFT = 976, USLFT = 976, LFW = 977, TGLFW = 977, LGB = 978, USLGB = 978, LGG = 979, BELGG = 979, 
        LGK = 980, MYLGK = 980, LHE = 981, PKLHE = 981, LI0 = 982, LIVDZ = 982, LIF = 983, NCLIF = 983, LIG = 984, FRLIG = 984, LIH = 985, USLIU = 985, LIL = 986, FRLLE = 986, LIM = 987, PELIM = 987, LIQ = 988, CDLIQ = 988, LIS = 989, PTLIS = 989, LIT = 990, USLIT = 990, 
        LJU = 991, SILJU = 991, LKL = 992, NOLKL = 992, LKO = 993, INLKO = 993, LL0 = 994, PHNOP = 994, LLA = 995, SELLA = 995, LLW = 996, MWLLW = 996, LMM = 997, MXLMM = 997, LMP = 998, ITLMP = 998, LMT = 999, USLMT = 999, LNK = 1000, USLNK = 1000, LNO = 1001, AULNO = 1001, 
        LNS = 1002, USLNS = 1002, LNY = 1003, USLNY = 1003, LNZ = 1004, ATLNZ = 1004, LON = 1005, GBLON = 1005, LOS = 1006, NGLOS = 1006, LPA = 1007, ESLPG = 1007, LPB = 1008, BOLPB = 1008, LPL = 1009, GBLIV = 1009, LPP = 1010, FILPP = 1010, LRD = 1011, USLRD = 1011, LRE = 1012, AULRE = 1012, 
        LRF = 1013, USLRF = 1013, LRH = 1014, FRLRH = 1014, LRM = 1015, DOCDC = 1015, LRT = 1016, FRLRT = 1016, LSE = 1017, USLSE = 1017, LSI = 1018, GBLSI = 1018, LST = 1019, AULST = 1019, LSY = 1020, AULSY = 1020, LSZ = 1021, HRVLN = 1021, LTN = 1022, GBLUT = 1022, LTO = 1023, MXLRO = 1023, 
        LU0 = 1024, CHLZN = 1024, LUD = 1025, NALUD = 1025, LUG = 1026, CHLUG = 1026, LUJ = 1027, ZALUJ = 1027, LUM = 1028, CNLUM = 1028, LUN = 1029, ZMLUN = 1029, LUQ = 1030, ARLUQ = 1030, LUX = 1031, LULUX = 1031, LV0 = 1032, BELEU = 1032, LVO = 1033, AULVO = 1033, LWB = 1034, USLWB = 1034, 
        LWK = 1035, GBLER = 1035, LWO = 1036, UALWO = 1036, LWS = 1037, USLWS = 1037, LWT = 1038, USLWT = 1038, LXR = 1039, EGLXR = 1039, LYH = 1040, USLYH = 1040, LYO = 1041, USLYO = 1041, LYP = 1042, PKLYP = 1042, LYR = 1043, SJLYR = 1043, LYS = 1044, FRLIO = 1044, LYX = 1045, GBLYX = 1045, 
        LZC = 1046, MXLZC = 1046, MAA = 1047, INMAA = 1047, MAD = 1048, ESMAD = 1048, MAF = 1049, USMAF = 1049, MAH = 1050, ESMAH = 1050, MAJ = 1051, MHMAJ = 1051, MAM = 1052, MXMAM = 1052, MAN = 1053, GBMNC = 1053, MAO = 1054, BRMAO = 1054, MAR = 1055, VEMAR = 1055, MAU = 1056, PFMAU = 1056, 
        MAZ = 1057, PRMAZ = 1057, MBA = 1058, KEMBA = 1058, MBH = 1059, AUMBH = 1059, MBJ = 1060, JMMBJ = 1060, MBM = 1061, ZAMBM = 1061, MBS = 1062, USSGM = 1062, MBX = 1063, SIMBX = 1063, MC0 = 1064, USMOC = 1064, MCE = 1065, USMCE = 1065, MCI = 1066, USMKC = 1066, MCL = 1067, USMCL = 1067, 
        MCM = 1068, MCMON = 1068, MCN = 1069, USMCN = 1069, MCO = 1070, USORL = 1070, MCP = 1071, BRMCA = 1071, MCT = 1072, OMMCT = 1072, MCW = 1073, USMCW = 1073, MCY = 1074, AUMCY = 1074, MCZ = 1075, BRMCZ = 1075, MDC = 1076, IDMDC = 1076, MDE = 1077, COMDE = 1077, MDL = 1078, MMMDL = 1078, 
        MDQ = 1079, ARMDQ = 1079, MDY = 1080, UMMDY = 1080, MDZ = 1081, ARMDZ = 1081, MED = 1082, SAMED = 1082, MEE = 1083, NCMEE = 1083, MEI = 1084, USMEI = 1084, MEL = 1085, AUMEL = 1085, MEM = 1086, USMEM = 1086, MES = 1087, IDMES = 1087, MEX = 1088, MXMEX = 1088, MEZ = 1089, ZAMEZ = 1089, 
        MFE = 1090, USTXC = 1090, MFF = 1091, GAMFF = 1091, MFG = 1092, PKMFG = 1092, MFM = 1093, MOMFM = 1093, MFN = 1094, NZMFN = 1094, MFO = 1095, PGMFO = 1095, MFQ = 1096, NEMFQ = 1096, MFR = 1097, USMFR = 1097, MFU = 1098, ZMMFU = 1098, MGA = 1099, NIMGA = 1099, MGB = 1100, AUMGB = 1100, 
        MGF = 1101, BRMGF = 1101, MGH = 1102, ZAMGH = 1102, MGM = 1103, USMGM = 1103, MGQ = 1104, SOMGQ = 1104, MGW = 1105, USMGW = 1105, MHE = 1106, USMHE = 1106, MHH = 1107, BSMHH = 1107, MHQ = 1108, AXMHQ = 1108, MHT = 1109, USMHT = 1109, MIA = 1110, USMIA = 1110, MID = 1111, MXMID = 1111, 
        MIK = 1112, FIMIK = 1112, MIL = 1113, ITMIL = 1113, MIM = 1114, AUMIM = 1114, MIR = 1115, TNMIR = 1115, MJC = 1116, CIMJC = 1116, MJD = 1117, PKMJD = 1117, MJL = 1118, GAMJL = 1118, MJN = 1119, MGMJN = 1119, MJQ = 1120, USMJQ = 1120, MJT = 1121, GRMJT = 1121, MJV = 1122, ESMJV = 1122, 
        MKC = 1123, PHMKC = 1123, MKE = 1124, USMKE = 1124, MKG = 1125, USMKG = 1125, MKK = 1126, USKUA = 1126, MKL = 1127, USMKL = 1127, MKR = 1128, AUMKR = 1128, MKY = 1129, AUMKY = 1129, MLA = 1130, MTMLA = 1130, MLB = 1131, USMLB = 1131, MLE = 1132, MVMLE = 1132, MLH = 1133, FRMLH = 1133, 
        MLI = 1134, USMLI = 1134, MLM = 1135, MXMLM = 1135, MLS = 1136, USMLS = 1136, MLU = 1137, USMLU = 1137, MLW = 1138, LRMLW = 1138, MLX = 1139, TRMLX = 1139, MMA = 1140, SEMMA = 1140, MME = 1141, GBMME = 1141, MMG = 1142, AUMMG = 1142, MMJ = 1143, JPMMJ = 1143, MMK = 1144, RUMMK = 1144, 
        MMM = 1145, AUMMM = 1145, MMY = 1146, JPMMY = 1146, MNI = 1147, MSBRD = 1147, MNL = 1148, PHMNL = 1148, MOB = 1149, USMOB = 1149, MOD = 1150, USMOD = 1150, MON = 1151, NZMON = 1151, MOT = 1152, USMOT = 1152, MOV = 1153, AUMOV = 1153, MOW = 1154, RUMOW = 1154, MOZ = 1155, PFMOZ = 1155, 
        MPA = 1156, NAKAT = 1156, MPD = 1157, PKMPD = 1157, MPL = 1158, FRMPL = 1158, MPM = 1159, MZMPM = 1159, MPV = 1160, USMPI = 1160, MQL = 1161, AUMQL = 1161, MQM = 1162, TRMDN = 1162, MQQ = 1163, TDMQQ = 1163, MQT = 1164, USMQT = 1164, MRB = 1165, USMRB = 1165, MRS = 1166, FRMRS = 1166, 
        MRU = 1167, MUPLU = 1167, MRV = 1168, RUMRV = 1168, MRY = 1169, USMY3 = 1169, MRZ = 1170, AUMRZ = 1170, MSE = 1171, GBMSE = 1171, MSL = 1172, USUSH = 1172, MSN = 1173, USMSN = 1173, MSO = 1174, USMSO = 1174, MSP = 1175, USMES = 1175, MSQ = 1176, BYMSQ = 1176, MSR = 1177, TRMSR = 1177, 
        MST = 1178, NLMST = 1178, MSU = 1179, LSMSU = 1179, MSY = 1180, USMSY = 1180, MTH = 1181, USMTH = 1181, MTJ = 1182, USMTJ = 1182, MTL = 1183, AUMTL = 1183, MTM = 1184, USMTM = 1184, MTO = 1185, USMTO = 1185, MTS = 1186, SZMTS = 1186, MTT = 1187, MXMTT = 1187, MTY = 1188, MXMTY = 1188, 
        MUB = 1189, BWMUB = 1189, MUC = 1190, DEMUC = 1190, MUE = 1191, USMUE = 1191, MUH = 1192, EGMUH = 1192, MUX = 1193, PKMUX = 1193, MV0 = 1194, USVYV = 1194, MVB = 1195, GAMVE = 1195, MVD = 1196, UYMVD = 1196, MVR = 1197, CMMVR = 1197, MVY = 1198, USMVY = 1198, MVZ = 1199, ZWMVZ = 1199, 
        MWD = 1200, PKMWD = 1200, MWH = 1201, USMWH = 1201, MXL = 1202, MXMXL = 1202, MYA = 1203, AUMYA = 1203, MYD = 1204, KEMYD = 1204, MYJ = 1205, JPMYJ = 1205, MYQ = 1206, INMYQ = 1206, MYR = 1207, USMYR = 1207, MYY = 1208, MYMYY = 1208, MZG = 1209, TWMZG = 1209, MZM = 1210, FRMZM = 1210, 
        MZR = 1211, AFMZR = 1211, MZT = 1212, MXMZT = 1212, MZY = 1213, ZAMZY = 1213, NAA = 1214, AUNAA = 1214, NAG = 1215, INNAG = 1215, NAJ = 1216, AZNAJ = 1216, NAN = 1217, FJNAN = 1217, NAP = 1218, ITNAP = 1218, NAS = 1219, BSNAS = 1219, NAT = 1220, BRNAT = 1220, NBO = 1221, KENBO = 1221, 
        NCE = 1222, FRNCE = 1222, NCL = 1223, GBNCS = 1223, NCS = 1224, ZANCS = 1224, NCY = 1225, FRNCY = 1225, NDB = 1226, MRNDB = 1226, NDJ = 1227, TDNDJ = 1227, NDU = 1228, NANDU = 1228, NEV = 1229, KNNEV = 1229, NGE = 1230, CMNGE = 1230, NGO = 1231, JPNGO = 1231, NGS = 1232, JPNGS = 1232, 
        NIC = 1233, CYNIC = 1233, NIM = 1234, NENIM = 1234, NKC = 1235, MRNKC = 1235, NKW = 1236, IODGA = 1236, NLA = 1237, ZMNLA = 1237, NLD = 1238, MXNLD = 1238, NLK = 1239, NFNLK = 1239, NLP = 1240, ZANLP = 1240, NLV = 1241, UANLV = 1241, NNG = 1242, CNNIN = 1242, NOC = 1243, IENOC = 1243, 
        NOU = 1244, NCNOU = 1244, NQY = 1245, GBNQY = 1245, NRA = 1246, AUNRA = 1246, NRK = 1247, SENRK = 1247, NS0 = 1248, USNQY = 1248, NSA = 1249, AUNSA = 1249, NSN = 1250, NZNSN = 1250, NSO = 1251, AUNSO = 1251, NST = 1252, THNKT = 1252, NTE = 1253, FRNTE = 1253, NTL = 1254, AUNTL = 1254, 
        NTY = 1255, ZAPIB = 1255, NUE = 1256, DENUE = 1256, NWI = 1257, GBNRW = 1257, NYC = 1258, USNYC = 1258, OAG = 1259, AUOAG = 1259, OAJ = 1260, USOAJ = 1260, OAK = 1261, USOAK = 1261, OAX = 1262, MXOAX = 1262, OB0 = 1263, DEOLO = 1263, ODA = 1264, CFODA = 1264, ODB = 1265, ESODB = 1265, 
        ODE = 1266, DKODE = 1266, ODS = 1267, UAODS = 1267, OGG = 1268, USOGG = 1268, OHD = 1269, MKOHD = 1269, OHT = 1270, PKOHT = 1270, OIT = 1271, JPOIT = 1271, OKA = 1272, JPOKA = 1272, OKC = 1273, USOKC = 1273, OKD = 1274, JPSPK = 1274, OKJ = 1275, JPOKJ = 1275, OKU = 1276, NAOKU = 1276, 
        OLB = 1277, ITOLB = 1277, OLF = 1278, USOLF = 1278, OLM = 1279, USOLM = 1279, OLP = 1280, AUOLP = 1280, OMA = 1281, USOMA = 1281, OMD = 1282, NAOMD = 1282, OME = 1283, USOME = 1283, OMH = 1284, IRUIM = 1284, OMO = 1285, BAOMO = 1285, OND = 1286, NAOND = 1286, ONT = 1287, USONT = 1287, 
        OOL = 1288, AUGOC = 1288, OOM = 1289, AUCOM = 1289, OPO = 1290, PTOPO = 1290, ORB = 1291, SEORB = 1291, ORF = 1292, USORF = 1292, ORH = 1293, USORH = 1293, ORK = 1294, IEORK = 1294, ORN = 1295, DZORN = 1295, OSA = 1296, JPOSA = 1296, OSH = 1297, USOSH = 1297, OSI = 1298, HROSI = 1298, 
        OSL = 1299, NOOSL = 1299, OSM = 1300, IQOSM = 1300, OSR = 1301, CZOSR = 1301, OTH = 1302, USOTH = 1302, OTZ = 1303, USOTZ = 1303, OUA = 1304, BFOUA = 1304, OUD = 1305, MAOUD = 1305, OUH = 1306, ZAOUH = 1306, OUK = 1307, GBOUK = 1307, OUL = 1308, FIOUL = 1308, OUZ = 1309, MROUZ = 1309, 
        OVB = 1310, RUOVB = 1310, OVD = 1311, ESOVO = 1311, OWB = 1312, USOWB = 1312, OXB = 1313, GWOXB = 1313, OXR = 1314, USOXN = 1314, OZH = 1315, UAZPR = 1315, OZZ = 1316, MAOZZ = 1316, PA0 = 1317, BRPWA = 1317, PAC = 1318, PAPTY = 1318, PAD = 1319, DEPAD = 1319, PAH = 1320, USPAH = 1320, 
        PAP = 1321, HTPAP = 1321, PAR = 1322, FRPAR = 1322, PAT = 1323, INPAT = 1323, PB0 = 1324, USPB8 = 1324, PBC = 1325, MXPBC = 1325, PBH = 1326, BTTHI = 1326, PBI = 1327, USPBI = 1327, PBM = 1328, SRPBM = 1328, PBO = 1329, AUPBO = 1329, PBZ = 1330, ZAPBZ = 1330, PC0 = 1331, PHPSG = 1331, 
        PCT = 1332, USPCT = 1332, PDL = 1333, PTPDL = 1333, PDT = 1334, USPDT = 1334, PDX = 1335, USPDX = 1335, PEG = 1336, ITPEG = 1336, PEI = 1337, COPEI = 1337, PEN = 1338, MYPEN = 1338, PER = 1339, AUPER = 1339, PEW = 1340, PKPEW = 1340, PFN = 1341, USPFN = 1341, PFO = 1342, CYPFO = 1342, 
        PGA = 1343, USPGA = 1343, PGF = 1344, FRPGF = 1344, PGV = 1345, USPGV = 1345, PHC = 1346, NGPHC = 1346, PHE = 1347, AUPHE = 1347, PHF = 1348, USNNS = 1348, PHH = 1349, NPPKR = 1349, PHL = 1350, USPHL = 1350, PHW = 1351, ZAPHW = 1351, PHX = 1352, USPHX = 1352, PIA = 1353, USPIA = 1353, 
        PIB = 1354, USLUL = 1354, PIH = 1355, USPIH = 1355, PIR = 1356, USPIR = 1356, PIS = 1357, FRPIS = 1357, PIT = 1358, USPIT = 1358, PJG = 1359, PKPJG = 1359, PKB = 1360, USPKB = 1360, PKW = 1361, BWPKW = 1361, PLB = 1362, USPLB = 1362, PLN = 1363, USPXN = 1363, PLO = 1364, AUPLO = 1364, 
        PLZ = 1365, ZAPLZ = 1365, PMD = 1366, USQPL = 1366, PMI = 1367, ESPMI = 1367, PMO = 1368, ITPMO = 1368, PMR = 1369, NZPMR = 1369, PMV = 1370, VEMGT = 1370, PMW = 1371, BRPMW = 1371, PN0 = 1372, PNPCN = 1372, PNH = 1373, KHPNH = 1373, PNI = 1374, FMPNI = 1374, PNL = 1375, ITPNL = 1375, 
        PNQ = 1376, INPNQ = 1376, PNR = 1377, CGPNR = 1377, PNS = 1378, USPNS = 1378, POA = 1379, BRPOA = 1379, POG = 1380, GAPOG = 1380, POM = 1381, PGPOM = 1381, POP = 1382, DOPOP = 1382, POR = 1383, FIPOR = 1383, POS = 1384, TTPOS = 1384, POU = 1385, USPOU = 1385, POZ = 1386, PLPOZ = 1386, 
        PPG = 1387, ASPPG = 1387, PPP = 1388, AUPPP = 1388, PPT = 1389, PFPPT = 1389, PQI = 1390, USPQI = 1390, PQQ = 1391, AUPQQ = 1391, PRG = 1392, CZPRG = 1392, PRN = 1393, XXPRN = 1393, PRY = 1394, ZAPRY = 1394, PS0 = 1395, PSNAB = 1395, PSA = 1396, ITPSA = 1396, PSC = 1397, USPSC = 1397, 
        PSD = 1398, EGPSD = 1398, PSE = 1399, PRPSE = 1399, PSG = 1400, USPSG = 1400, PSI = 1401, PKPSI = 1401, PSP = 1402, USPSP = 1402, PSR = 1403, ITPSR = 1403, PSY = 1404, FKPSY = 1404, PT0 = 1405, BJPTN = 1405, PTG = 1406, ZAPOL = 1406, PTJ = 1407, AUPTJ = 1407, PUB = 1408, USPUB = 1408, 
        PUF = 1409, FRPUF = 1409, PUG = 1410, AUPUG = 1410, PUJ = 1411, DOPCN = 1411, PUQ = 1412, CLPUQ = 1412, PUS = 1413, KRPUS = 1413, PUW = 1414, USPUW = 1414, PUY = 1415, HRPUY = 1415, PVD = 1416, USPVD = 1416, PVH = 1417, BRPVH = 1417, PVK = 1418, GRPVK = 1418, PVR = 1419, MXPVR = 1419, 
        PWM = 1420, USPWM = 1420, PXM = 1421, MXPXM = 1421, PXO = 1422, PTPXO = 1422, PYB = 1423, INPYB = 1423, PYX = 1424, THPYX = 1424, PZB = 1425, ZAPZB = 1425, PZH = 1426, PKPZH = 1426, PZO = 1427, VEPZO = 1427, QCB = 1428, JPCHB = 1428, QGF = 1429, BRMON = 1429, QIC = 1430, ITBIE = 1430, 
        QLI = 1431, CYLMS = 1431, QML = 1432, PKMPR = 1432, QMN = 1433, SZQMN = 1433, QNB = 1434, INQNB = 1434, QND = 1435, RSNVS = 1435, QPA = 1436, ITPDA = 1436, QSA = 1437, ESSBP = 1437, RA0 = 1438, DKRAN = 1438, RAI = 1439, CVRAI = 1439, RAJ = 1440, INRAJ = 1440, RAK = 1441, MARAK = 1441, 
        RAP = 1442, USRAP = 1442, RAR = 1443, CKRAR = 1443, RAZ = 1444, PKRAZ = 1444, RB0 = 1445, DEREG = 1445, RBA = 1446, MARBA = 1446, RBR = 1447, BRRBR = 1447, RCB = 1448, ZARCB = 1448, RDD = 1449, USRDD = 1449, RDG = 1450, USRDG = 1450, RDM = 1451, USRMD = 1451, RDU = 1452, USRAG = 1452, 
        RDZ = 1453, FRRDZ = 1453, REC = 1454, BRREC = 1454, REG = 1455, ITREG = 1455, REK = 1456, ISREY = 1456, REU = 1457, ESRUS = 1457, RFD = 1458, USRFD = 1458, RFP = 1459, PFRFP = 1459, RGI = 1460, PFRGI = 1460, RGL = 1461, ARRGL = 1461, RGN = 1462, MMRGN = 1462, RHG = 1463, RWRHG = 1463, 
        RHI = 1464, USRHI = 1464, RHO = 1465, GRRHO = 1465, RIC = 1466, USRIC = 1466, RIO = 1467, BRRIO = 1467, RIX = 1468, LVRIX = 1468, RIZ = 1469, CNRZH = 1469, RJK = 1470, HRRJK = 1470, RKD = 1471, USRKD = 1471, RKS = 1472, USRKS = 1472, RKT = 1473, AERKT = 1473, RLT = 1474, NERLT = 1474, 
        RMF = 1475, EGRMF = 1475, RMI = 1476, ITRMI = 1476, RNB = 1477, SERNB = 1477, RNE = 1478, FRRNE = 1478, RNN = 1479, DKRNN = 1479, RNO = 1480, USRNO = 1480, RNS = 1481, FRRNS = 1481, ROA = 1482, USROA = 1482, ROC = 1483, USROC = 1483, ROK = 1484, AUROK = 1484, ROM = 1485, ITROM = 1485, 
        ROR = 1486, PWROR = 1486, ROS = 1487, ARROS = 1487, ROT = 1488, NZROT = 1488, ROU = 1489, BGRDU = 1489, ROV = 1490, RUROV = 1490, RR0 = 1491, CARRV = 1491, RRG = 1492, MURRG = 1492, RS0 = 1493, GEZAK = 1493, RSA = 1494, ARRSA = 1494, RSD = 1495, BSRSD = 1495, RST = 1496, USRST = 1496, 
        RTB = 1497, HNRTB = 1497, RTM = 1498, NLRTM = 1498, RUH = 1499, SARUH = 1499, RUN = 1500, RERUN = 1500, RVN = 1501, FIRVN = 1501, RWI = 1502, USRWI = 1502, RWP = 1503, PKRWP = 1503, RYK = 1504, PKRYK = 1504, SAF = 1505, USSAF = 1505, SAH = 1506, YESAH = 1506, SAI = 1507, KHREP = 1507, 
        SAL = 1508, SVSAL = 1508, SAN = 1509, USSAN = 1509, SAO = 1510, BRSAO = 1510, SAP = 1511, HNSAP = 1511, SAT = 1512, USSAT = 1512, SAV = 1513, USSAV = 1513, SBA = 1514, USSBA = 1514, SBH = 1515, BLSBH = 1515, SBK = 1516, FRSBK = 1516, SBN = 1517, USSBN = 1517, SBP = 1518, USCSL = 1518, 
        SBU = 1519, ZASBU = 1519, SBW = 1520, MYSBW = 1520, SBY = 1521, USSBY = 1521, SBZ = 1522, ROSBZ = 1522, SCC = 1523, USSCC = 1523, SCE = 1524, USSCE = 1524, SCF = 1525, USSTZ = 1525, SCK = 1526, USSCK = 1526, SCN = 1527, DESCN = 1527, SCQ = 1528, ESSCQ = 1528, SCU = 1529, CUSCU = 1529, 
        SDF = 1530, USLUI = 1530, SDH = 1531, HNSDH = 1531, SDJ = 1532, JPSDJ = 1532, SDL = 1533, SESDL = 1533, SDQ = 1534, DOSDQ = 1534, SDR = 1535, ESSDR = 1535, SDS = 1536, JPSOS = 1536, SDT = 1537, PKSDT = 1537, SDY = 1538, USSDY = 1538, SEA = 1539, USSEA = 1539, SEB = 1540, LYSEB = 1540, 
        SEL = 1541, KRSEL = 1541, SEN = 1542, GBSND = 1542, SEW = 1543, EGSEW = 1543, SEZ = 1544, SCVIC = 1544, SFA = 1545, TNSFA = 1545, SFG = 1546, MFMAR = 1546, SFJ = 1547, GLSFJ = 1547, SFN = 1548, ARSFN = 1548, SFO = 1549, USSFO = 1549, SGD = 1550, DKSGD = 1550, SGF = 1551, USSGF = 1551, 
        SGN = 1552, VNSGN = 1552, SGU = 1553, USSGU = 1553, SGY = 1554, USSGY = 1554, SH0 = 1555, NLHTB = 1555, SHA = 1556, CNSGH = 1556, SHD = 1557, USSZH = 1557, SHE = 1558, CNSHY = 1558, SHJ = 1559, AESHJ = 1559, SHR = 1560, USSHR = 1560, SHS = 1561, JPSHS = 1561, SHV = 1562, USSHV = 1562, 
        SI0 = 1563, DKSLB = 1563, SID = 1564, CVSID = 1564, SIN = 1565, SGSIN = 1565, SIP = 1566, UASIP = 1566, SIS = 1567, ZASIS = 1567, SIT = 1568, USSIT = 1568, SIX = 1569, AUSIX = 1569, SJC = 1570, USSJC = 1570, SJD = 1571, MXLCB = 1571, SJJ = 1572, BASJJ = 1572, SJO = 1573, CRSJO = 1573, 
        SJT = 1574, USSJT = 1574, SJU = 1575, PRSJU = 1575, SJY = 1576, FISJY = 1576, SK0 = 1577, GMSKE = 1577, SKB = 1578, KNSKB = 1578, SKD = 1579, UZSKD = 1579, SKG = 1580, GRSKG = 1580, SKP = 1581, MKSKP = 1581, SKS = 1582, DKSKR = 1582, SKV = 1583, EGSKV = 1583, SKX = 1584, RUSKX = 1584, 
        SKZ = 1585, PKSKZ = 1585, SLA = 1586, ARSLA = 1586, SLC = 1587, USSLC = 1587, SLE = 1588, USSLE = 1588, SLL = 1589, OMSLL = 1589, SLP = 1590, MXSLP = 1590, SLS = 1591, BGSLS = 1591, SLU = 1592, LCCAS = 1592, SLZ = 1593, BRSLZ = 1593, SM0 = 1594, SMSAI = 1594, SMA = 1595, PTSMA = 1595, 
        SMF = 1596, USSAC = 1596, SMI = 1597, GRSMI = 1597, SMX = 1598, USSMX = 1598, SNA = 1599, USSNA = 1599, SNC = 1600, ECSNC = 1600, SNN = 1601, IELMK = 1601, SNS = 1602, USSNS = 1602, SOF = 1603, BGSOF = 1603, SOG = 1604, NOSOG = 1604, SON = 1605, VUSAN = 1605, SOT = 1606, FISOT = 1606, 
        SOU = 1607, GBSOU = 1607, SPC = 1608, ESSPC = 1608, SPI = 1609, USSPI = 1609, SPN = 1610, MPSPN = 1610, SPS = 1611, USSPS = 1611, SPU = 1612, HRSPU = 1612, SPY = 1613, CISPY = 1613, SRA = 1614, BRSRA = 1614, SRB = 1615, BOSRB = 1615, SRL = 1616, MXSRL = 1616, SRQ = 1617, USZOR = 1617, 
        SRZ = 1618, BOSCS = 1618, SSA = 1619, BRSSA = 1619, SSG = 1620, GQSSG = 1620, SSH = 1621, EGSSH = 1621, SSL = 1622, COSSL = 1622, ST0 = 1623, USXLX = 1623, STI = 1624, CLSCL = 1624, STL = 1625, USSTL = 1625, STO = 1626, SESTO = 1626, STP = 1627, USSTP = 1627, STR = 1628, DESTR = 1628, 
        STS = 1629, USSTS = 1629, STT = 1630, VISTT = 1630, STV = 1631, INSTV = 1631, STX = 1632, VISTX = 1632, SUB = 1633, IDSUB = 1633, SUF = 1634, ITSUF = 1634, SUL = 1635, PKSUL = 1635, SUN = 1636, US5SV = 1636, SUV = 1637, FJSUV = 1637, SUX = 1638, USSUX = 1638, SVD = 1639, VCKTN = 1639, 
        SVG = 1640, NOSVG = 1640, SVL = 1641, FISVL = 1641, SVQ = 1642, ESSVQ = 1642, SVX = 1643, RUYEK = 1643, SW0 = 1644, DESWR = 1644, SWF = 1645, USSWF = 1645, SWP = 1646, NASWP = 1646, SWS = 1647, GBSWA = 1647, SXB = 1648, FRSXB = 1648, SXL = 1649, IESLI = 1649, SXM = 1650, SXPHI = 1650, 
        SXR = 1651, INSXR = 1651, SYD = 1652, AUSYD = 1652, SYR = 1653, USSYR = 1653, SYY = 1654, GBSTO = 1654, SZD = 1655, GBSHE = 1655, SZF = 1656, TRSSX = 1656, SZG = 1657, ATSZG = 1657, SZK = 1658, ZASZK = 1658, SZX = 1659, CNSNZ = 1659, SZZ = 1660, PLSZZ = 1660, TAB = 1661, TTSCR = 1661, 
        TAK = 1662, JPTAK = 1662, TAM = 1663, MXTAM = 1663, TAO = 1664, CNQIN = 1664, TAS = 1665, UZTAS = 1665, TAY = 1666, TWTPE = 1666, TBO = 1667, SNTOU = 1667, TBS = 1668, GETBS = 1668, TBU = 1669, TOTBU = 1669, TC0 = 1670, USTEL = 1670, TCA = 1671, AUTCA = 1671, TCB = 1672, BSTCB = 1672, 
        TCI = 1673, ESTCI = 1673, TCL = 1674, USTCL = 1674, TCU = 1675, ZATCU = 1675, TED = 1676, DKTED = 1676, TEM = 1677, AUTEM = 1677, TEQ = 1678, TRTEK = 1678, TER = 1679, PTTER = 1679, TEU = 1680, NZTEU = 1680, TEX = 1681, USQTR = 1681, TF0 = 1682, TFPFR = 1682, TGD = 1683, METGD = 1683, 
        TGU = 1684, HNTGU = 1684, TGV = 1685, BGTGV = 1685, TGZ = 1686, MXTGZ = 1686, THE = 1687, BRTHE = 1687, THR = 1688, IRTHR = 1688, THU = 1689, GLTHU = 1689, TIA = 1690, ALTIA = 1690, TIF = 1691, SATIF = 1691, TIJ = 1692, MXTIJ = 1692, TIP = 1693, LYTIP = 1693, TIS = 1694, AUTIS = 1694, 
        TIV = 1695, METIV = 1695, TK0 = 1696, TKNKU = 1696, TKA = 1697, USTKA = 1697, TKS = 1698, JPTKS = 1698, TKU = 1699, FITKU = 1699, TLH = 1700, USTLH = 1700, TLL = 1701, EETLL = 1701, TLS = 1702, FRTLS = 1702, TLV = 1703, ILTLV = 1703, TML = 1704, GHTML = 1704, TMP = 1705, FITMP = 1705, 
        TMS = 1706, STTMS = 1706, TMW = 1707, AUTMW = 1707, TMZ = 1708, UZTER = 1708, TNA = 1709, CNJNA = 1709, TNG = 1710, MATNG = 1710, TNR = 1711, MGTNR = 1711, TOD = 1712, MYTOD = 1712, TOL = 1713, USTOL = 1713, TOS = 1714, NOTOS = 1714, TOU = 1715, NCTOU = 1715, TOV = 1716, VGTOV = 1716, 
        TOY = 1717, JPTOY = 1717, TPA = 1718, USTPA = 1718, TPR = 1719, AUTPR = 1719, TPS = 1720, ITTPS = 1720, TRD = 1721, NOTRD = 1721, TRI = 1722, USOUI = 1722, TRN = 1723, ITTRN = 1723, TRO = 1724, AUTRO = 1724, TRS = 1725, ITTRS = 1725, TRV = 1726, INTRV = 1726, TRW = 1727, KITRW = 1727, 
        TRZ = 1728, INTRI = 1728, TSB = 1729, NATSB = 1729, TSE = 1730, KZAST = 1730, TSF = 1731, ITTRV = 1731, TSN = 1732, CNTNJ = 1732, TSV = 1733, AUTSV = 1733, TTN = 1734, USTTN = 1734, TUC = 1735, ARTUC = 1735, TUK = 1736, PKTUK = 1736, TUL = 1737, USTUL = 1737, TUN = 1738, TNTUN = 1738, 
        TUP = 1739, USTUP = 1739, TUS = 1740, USTUZ = 1740, TUU = 1741, SATUU = 1741, TVC = 1742, USTVC = 1742, TVF = 1743, USTVF = 1743, TVL = 1744, USTVL = 1744, TWB = 1745, AUTWB = 1745, TWF = 1746, USTWF = 1746, TWU = 1747, MYTWU = 1747, TXK = 1748, USTXK = 1748, TYN = 1749, CNTYU = 1749, 
        TYO = 1750, JPTYO = 1750, TYR = 1751, USTYR = 1751, TYS = 1752, USTYS = 1752, TZX = 1753, TRTZX = 1753, UAH = 1754, PFUAH = 1754, UAK = 1755, GLUAK = 1755, UAP = 1756, PFUAP = 1756, UBA = 1757, BRUBB = 1757, UBJ = 1758, JPUBJ = 1758, UBP = 1759, THUBP = 1759, UCA = 1760, USUCA = 1760, 
        UCT = 1761, RUUCT = 1761, UDE = 1762, NLUDE = 1762, UDI = 1763, BRUDI = 1763, UDJ = 1764, UAUZH = 1764, UDR = 1765, INUDR = 1765, UEE = 1766, AUUEE = 1766, UET = 1767, PKUET = 1767, UFA = 1768, RUUFA = 1768, UGC = 1769, UZUGC = 1769, UGO = 1770, AOUGO = 1770, UHE = 1771, CZUHE = 1771, 
        UII = 1772, HNUII = 1772, UIN = 1773, USUIN = 1773, UIO = 1774, ECUIO = 1774, UIP = 1775, FRUIP = 1775, UIT = 1776, MHUIT = 1776, UKB = 1777, JPUKB = 1777, UKI = 1778, USUKI = 1778, UKY = 1779, JPUKY = 1779, ULB = 1780, VUULB = 1780, ULD = 1781, ZAULD = 1781, ULN = 1782, MNULN = 1782, 
        ULU = 1783, UGULU = 1783, ULY = 1784, RUULY = 1784, UMD = 1785, GLUMD = 1785, UME = 1786, SEUME = 1786, UMR = 1787, AUUMR = 1787, UNI = 1788, VCUNI = 1788, UNK = 1789, USUNK = 1789, UNT = 1790, GBUNT = 1790, UPG = 1791, IDUPG = 1791, UPL = 1792, CRUPA = 1792, UPN = 1793, MX2NB = 1793, 
        UPP = 1794, USUPP = 1794, URC = 1795, CNURM = 1795, URG = 1796, BRURG = 1796, URM = 1797, VEURM = 1797, USH = 1798, ARUSH = 1798, USN = 1799, KRUSN = 1799, UTC = 1800, NLUTC = 1800, UTH = 1801, THUTH = 1801, UTN = 1802, ZAUTN = 1802, UTP = 1803, THUTP = 1803, UTT = 1804, ZAUTT = 1804, 
        UUD = 1805, RUUUD = 1805, UVE = 1806, GAOYE = 1806, UVF = 1807, LCVIF = 1807, UVL = 1808, EGUVL = 1808, VA0 = 1809, VAVAT = 1809, VAA = 1810, FIVAA = 1810, VAN = 1811, TRVAN = 1811, VAP = 1812, CLVAP = 1812, VAR = 1813, BGVAR = 1813, VAS = 1814, TRVAS = 1814, VBS = 1815, ITBRC = 1815, 
        VBY = 1816, SEVBY = 1816, VCE = 1817, ITVCE = 1817, VDE = 1818, ESVDH = 1818, VDZ = 1819, USVDZ = 1819, VEL = 1820, USVEL = 1820, VER = 1821, MXVER = 1821, VFA = 1822, ZWVFA = 1822, VGO = 1823, ESVGO = 1823, VID = 1824, BGVID = 1824, VIE = 1825, ATVIE = 1825, VIJ = 1826, VGVIJ = 1826, 
        VIS = 1827, USVIS = 1827, VIT = 1828, ESVIT = 1828, VIX = 1829, BRVIX = 1829, VLC = 1830, ESVLC = 1830, VLD = 1831, USVLD = 1831, VLI = 1832, VUVLI = 1832, VLL = 1833, ESVLL = 1833, VLN = 1834, VEVLN = 1834, VLU = 1835, RUVLU = 1835, VN0 = 1836, USVIV = 1836, VNO = 1837, LTVIL = 1837, 
        VNS = 1838, INVNS = 1838, VOG = 1839, RUVOG = 1839, VPS = 1840, USFWB = 1840, VRA = 1841, CUVRA = 1841, VRB = 1842, USVRB = 1842, VRK = 1843, FIVRK = 1843, VRN = 1844, ITVRN = 1844, VSA = 1845, MXVSA = 1845, VST = 1846, SEVST = 1846, VTE = 1847, LAVTE = 1847, VVO = 1848, RUVVO = 1848, 
        VXO = 1849, SEVXO = 1849, VY0 = 1850, USVYI = 1850, VYD = 1851, ZAVYD = 1851, VZ0 = 1852, HRVZN = 1852, WAM = 1853, AUWAM = 1853, WAW = 1854, PLWAW = 1854, WDH = 1855, NAWDH = 1855, WEI = 1856, AUWEI = 1856, WEL = 1857, ZAWEL = 1857, WGA = 1858, AUWGA = 1858, WHK = 1859, NZWHK = 1859, 
        WIC = 1860, GBWCK = 1860, WIE = 1861, DEWIB = 1861, WLB = 1862, USWLB = 1862, WLG = 1863, NZWLG = 1863, WLS = 1864, WFMAU = 1864, WMB = 1865, AUWMB = 1865, WN0 = 1866, USWLZ = 1866, WNS = 1867, PKWNS = 1867, WOL = 1868, AUWOL = 1868, WP0 = 1869, USWPK = 1869, WRE = 1870, NZWRE = 1870, 
        WRG = 1871, USWRG = 1871, WRL = 1872, USWRL = 1872, WRO = 1873, PLWRO = 1873, WSY = 1874, AUWSI = 1874, WUH = 1875, CNNHN = 1875, WUN = 1876, AUWUN = 1876, WUX = 1877, CNWXS = 1877, WVB = 1878, NAWVB = 1878, WYA = 1879, AUWYA = 1879, WYN = 1880, AUWYN = 1880, WYS = 1881, USWYS = 1881, 
        XCH = 1882, CXFFC = 1882, XIY = 1883, CNXYA = 1883, XMH = 1884, PFXMH = 1884, XMN = 1885, CNXAM = 1885, XPK = 1886, CAXXW = 1886, XRY = 1887, ESJRZ = 1887, XSI = 1888, CAXSI = 1888, XXX = 1889, XXXXX = 1889, YAK = 1890, USYAK = 1890, YAO = 1891, CMYAO = 1891, YAT = 1892, CAYAT = 1892, 
        YBE = 1893, CAYBE = 1893, YCB = 1894, CAYCB = 1894, YDF = 1895, CAYDF = 1895, YEA = 1896, CAEDM = 1896, YEV = 1897, CAYEV = 1897, YFA = 1898, CAFAL = 1898, YFB = 1899, CAIQL = 1899, YFC = 1900, CAFRE = 1900, YFO = 1901, CAYFO = 1901, YGW = 1902, CAXXX = 1902, YGX = 1903, CAYGX = 1903, 
        YHR = 1904, CAYHR = 1904, YHZ = 1905, CAHAL = 1905, YIF = 1906, CASAU = 1906, YIH = 1907, CNYIC = 1907, YKA = 1908, CAKML = 1908, YKM = 1909, USYKM = 1909, YKS = 1910, RUYKS = 1910, YLB = 1911, CAXXY = 1911, YLR = 1912, CALRA = 1912, YLW = 1913, CAKWL = 1913, YMM = 1914, CAYMM = 1914, 
        YMQ = 1915, CAMTR = 1915, YNB = 1916, SAYNB = 1916, YOK = 1917, JPYOK = 1917, YOP = 1918, CARBL = 1918, YOW = 1919, CAOTT = 1919, YPN = 1920, CAPME = 1920, YPR = 1921, CAPRR = 1921, YQB = 1922, CAQUE = 1922, YQD = 1923, CAYQD = 1923, YQG = 1924, CAWND = 1924, YQM = 1925, CAMNT = 1925, 
        YQR = 1926, CAREG = 1926, YQT = 1927, CATHU = 1927, YQX = 1928, CAGAN = 1928, YRB = 1929, CAREB = 1929, YSJ = 1930, CASJB = 1930, YSM = 1931, CAFSM = 1931, YSR = 1932, CANVK = 1932, YTH = 1933, CAYTH = 1933, YTO = 1934, CATOR = 1934, YUD = 1935, CAXXZ = 1935, YUM = 1936, USYUM = 1936, 
        YUX = 1937, CAHAB = 1937, YVB = 1938, CAYVB = 1938, YVO = 1939, CAYVO = 1939, YVP = 1940, CAFCM = 1940, YVQ = 1941, CAYVQ = 1941, YVR = 1942, CAVAN = 1942, YWG = 1943, CAWNP = 1943, YWK = 1944, CAYWK = 1944, YXE = 1945, CASAK = 1945, YXJ = 1946, CAFSJ = 1946, YXN = 1947, CAYXN = 1947, 
        YXS = 1948, CAPRG = 1948, YXT = 1949, CAYXT = 1949, YXU = 1950, CALOD = 1950, YXY = 1951, CAYXY = 1951, YYC = 1952, CACAL = 1952, YYD = 1953, CAYYD = 1953, YYJ = 1954, CAVIC = 1954, YYN = 1955, CAYYN = 1955, YYQ = 1956, CACHV = 1956, YYR = 1957, CAGOO = 1957, YYT = 1958, CASJF = 1958, 
        YYZ = 1959, CAMIS = 1959, YZF = 1960, CAYZF = 1960, YZP = 1961, CASSP = 1961, ZAD = 1962, HRZAD = 1962, ZAG = 1963, HRZAG = 1963, ZAZ = 1964, ESZAZ = 1964, ZBO = 1965, AUZBO = 1965, ZCL = 1966, MXZCL = 1966, ZG0 = 1967, CHZLM = 1967, ZIH = 1968, MXZIH = 1968, ZKE = 1969, CAZKE = 1969, 
        ZLO = 1970, MXZLO = 1970, ZND = 1971, NEZND = 1971, ZNE = 1972, AUNWM = 1972, ZQN = 1973, NZZQN = 1973, ZRH = 1974, CHZRH = 1974, ZSA = 1975, BSZSA = 1975, ZSS = 1976, CIZSS = 1976, ZTH = 1977, GRZTH = 1977, ZTM = 1978, CAZTM = 1978, ZVK = 1979, LASAV = 1979, ZYL = 1980, BDZYL = 1980,
        
        MAXCITY = 1981, NUMCITY = 1981 
    };

    
    City( void ): m_city(NOCITY) {}
    ~City( void ) { m_city = NOCITY; }
    
    // non-explicit constructors intentional here
    City( CityCode i ): m_city(i) {} // e.g. i = City::LON
    City( const std::string &s ): m_city(NOCITY) { setCity(s); }
    City( const char *s ): m_city(NOCITY) { if (s) setCity(s); } 
    
    // My numeric code for this city e.g. City::LON = 1003
    operator short( void ) const { return m_city; }
    
    // The 3 letter IATA code for this city e.g. "LON"
    std::string
    to3Code( void ) const { return m_codes3[m_fromISO[m_city]]; }
    
    // The 5 letter UN/LOCODE code for this city i.e "GBLON" 
    std::string 
    locode( void ) const { return m_codes5Print[m_fromISO[m_city]]; } 
    
    std::string 
    name( void ) const { return m_fullNames[m_fromISO[m_city]]; } // i.e "London" 

    // the 3 letter subdivison code for this location e.g. LDN
    std::string
    subdiv( void ) const { return m_subdiv[m_city] ? m_subdiv[m_city] : "XXX"; }
    
    //  IANA time zone for this city  i.e "Europe/London"
    std::string 
    timezone( void ) const { return m_timezoneNames[m_timezones[m_fromISO[m_city]]]; } 
    
    int
    timezoneid( void ) const { return m_timezones[m_fromISO[m_city]]; } 
    
    bool
    setCity( const std::string &s ); // e.g. s = "LON" or  s = "GBLON"
    
    bool
    set3City( const std::string &s ); // e.g. s = "LON"
    
    bool
    set5City( const std::string &s ); // e.g. s = "GBLON"
    
    void
    setCity( CityCode s ) { m_city = s; } // e.g. s = City::LON or City::GBLON
    
    bool
    capital( void ) const { return m_capital[m_fromISO[m_city]]; }
    
    
    // latitude
    double 
    lat( void ) const { return m_position[m_fromISO[m_city]][0]; }
    
    // longitude
    double 
    lon( void ) const { return m_position[m_fromISO[m_city]][1]; }
    
    std::pair<double,double> // (latitude, longitude)
    pos( void ) const 
    { 
        const float *p = m_position[m_fromISO[m_city]];
        return std::pair<double,double>(p[0], p[1]); 
    }

    
    static City
    index( int i ) { return CityCode(m_toISO3[i]); }
    
    static int
    index( const City &c ) { return m_fromISO[c]; }
    
    bool                
    valid( void ) const { return m_city != NOCITY; }
    
private:

    short m_city; 

    
    static const short         m_search3[28]; 
    static const short         m_search5[28]; 
    static const short         m_fromISO[MAXCITY]; 
    static const short         m_toISO3[NUMCITY];
    static const short         m_toISO5[NUMCITY];
    static const short         m_timezones[NUMCITY];
    static const float         m_position[NUMCITY][2];    
    static const unsigned char m_capital[NUMCITY];
    static const char * const  m_codes3[NUMCITY];
    static const char * const  m_codes5[NUMCITY];
    static const char * const  m_codes5Print[NUMCITY];
    static const char * const  m_fullNames[NUMCITY];
    static const char * const  m_timezoneNames[599]; 
    static const char * const  m_subdiv[NUMCITY];
};


std::ostream&
operator<<( std::ostream &ostr, const City &c );

std::istream&
operator>>( std::istream &istr, City &c );


#endif



