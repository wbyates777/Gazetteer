/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 
 History: 
 
 Identifies a city (not an airport) using a large part of the IATA airport code list. 
 
 This class can represent 1981 distinct cities - most major cities of the world including capital cities and US state capitals.
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
        GCN = 573, USGCN = 573, GDL = 574, MXGDL = 574, GDN = 575, PLGDN = 575, GDT = 576, TCGDT = 576, GDV = 577, USGDV = 577, GEG = 578, USGEG = 578, GEM = 579, GQGEM = 579, GEO = 580, GYGEO = 580, GET = 581, AUGET = 581, GEX = 582, AUGEX = 582, GFF = 583, AUGFF = 583, 
        GFK = 584, USGFK = 584, GGG = 585, USZLV = 585, GGW = 586, USGSG = 586, GH0 = 587, INGDH = 587, GHB = 588, BSGHB = 588, GIB = 589, GIGIB = 589, GID = 590, BIGID = 590, GIL = 591, PKGIL = 591, GJL = 592, DZGJL = 592, GJT = 593, USGJT = 593, GKL = 594, AUGKL = 594, 
        GLA = 595, GBGLW = 595, GLH = 596, USGLH = 596, GLT = 597, AUGLT = 597, GNB = 598, FRGNB = 598, GND = 599, GDGND = 599, GNE = 600, BEGNE = 600, GNV = 601, USGNV = 601, GOA = 602, ITGOA = 602, GOH = 603, GLGOH = 603, GOI = 604, INGOI = 604, GOJ = 605, RUGOJ = 605, 
        GON = 606, USZGO = 606, GOO = 607, AUGOO = 607, GOT = 608, SEGOT = 608, GOU = 609, CMGOU = 609, GOV = 610, AUGOV = 610, GOZ = 611, BGGOZ = 611, GPA = 612, GRARX = 612, GPT = 613, USGPT = 613, GPZ = 614, USGPZ = 614, GR0 = 615, DKGRN = 615, GRB = 616, USGRB = 616, 
        GRJ = 617, ZAGRJ = 617, GRO = 618, ESGRO = 618, GRQ = 619, NLGRQ = 619, GRR = 620, USGRR = 620, GRX = 621, ESJAE = 621, GRZ = 622, ATGRZ = 622, GS0 = 623, GSXXX = 623, GSI = 624, SBGSI = 624, GSO = 625, USGBO = 625, GSP = 626, USGV9 = 626, GTE = 627, AUGTE = 627, 
        GTF = 628, USGTF = 628, GTI = 629, DEQGT = 629, GUA = 630, GTGUA = 630, GUC = 631, USGUC = 631, GUM = 632, GUHGT = 632, GVA = 633, CHGVA = 633, GW0 = 634, USGRH = 634, GWD = 635, PKGWD = 635, GWE = 636, ZWGWE = 636, GWT = 637, DEGWT = 637, GWY = 638, IEGWY = 638, 
        GYE = 639, ECGYE = 639, GYI = 640, RWGYI = 640, GYN = 641, BRGYN = 641, GYP = 642, AUGYP = 642, GZA = 643, PSGZA = 643, GZT = 644, TRGZT = 644, HAC = 645, JPHAC = 645, HAG = 646, NLHAG = 646, HAH = 647, KMYVA = 647, HAJ = 648, DEHAJ = 648, HAM = 649, DEHAM = 649, 
        HAN = 650, VNHAN = 650, HAR = 651, USHAR = 651, HAU = 652, NOHAU = 652, HAV = 653, CUHAV = 653, HBA = 654, AUHBA = 654, HDD = 655, PKHDD = 655, HDN = 656, USSBS = 656, HDY = 657, THHDY = 657, HEA = 658, AFHEA = 658, HEL = 659, FIHEL = 659, HER = 660, GRHER = 660, 
        HFA = 661, ILHFA = 661, HFT = 662, NOHFT = 662, HGA = 663, SOHGA = 663, HGH = 664, CNHAZ = 664, HGO = 665, CIHGO = 665, HHH = 666, USHHH = 666, HIB = 667, USIBB = 667, HII = 668, USHII = 668, HIJ = 669, JPHIJ = 669, HIR = 670, SBHIR = 670, HIS = 671, AUHIS = 671, 
        HK0 = 672, CZHKR = 672, HKD = 673, JPHKD = 673, HKG = 674, HKHKG = 674, HKT = 675, THHKT = 675, HKY = 676, USHKY = 676, HLA = 677, ZAHLA = 677, HLH = 678, CNHLH = 678, HLN = 679, USHLN = 679, HLT = 680, AUHLT = 680, HLZ = 681, NZHLZ = 681, HMO = 682, MXHMO = 682, 
        HNA = 683, JPMRW = 683, HNH = 684, USHNH = 684, HNK = 685, AUHNK = 685, HNL = 686, USHNL = 686, HNS = 687, USHNS = 687, HOG = 688, CUHOG = 688, HOM = 689, USHOM = 689, HON = 690, USHON = 690, HOQ = 691, DEHOQ = 691, HOR = 692, PTHOR = 692, HOU = 693, USHOU = 693, 
        HPN = 694, USWTS = 694, HRB = 695, CNHRN = 695, HRE = 696, ZWHRE = 696, HRG = 697, EGHRG = 697, HRK = 698, UAHRK = 698, HRL = 699, USHRL = 699, HS0 = 700, DKHOR = 700, HSV = 701, USHSV = 701, HTA = 702, RUHTA = 702, HTI = 703, AUHTI = 703, HTS = 704, USHNU = 704, 
        HTV = 705, USHTV = 705, HUF = 706, USHUF = 706, HUH = 707, PFHUH = 707, HUI = 708, VNHUI = 708, HUX = 709, MXHTO = 709, HUY = 710, GBHUY = 710, HVB = 711, AUHVB = 711, HVN = 712, USHVN = 712, HVR = 713, USHVR = 713, HWN = 714, ZWHWN = 714, HYA = 715, USHYA = 715, 
        HYD = 716, INHYD = 716, HYG = 717, USHYG = 717, IAG = 718, USIAG = 718, IBZ = 719, ESIBZ = 719, ICN = 720, KRINC = 720, ICT = 721, USICT = 721, IDA = 722, USIDA = 722, IDR = 723, INIDR = 723, IEV = 724, UAIEV = 724, IFL = 725, AUIFL = 725, IFP = 726, USIFP = 726, 
        IGH = 727, AUIGH = 727, IGR = 728, ARIGR = 728, IJX = 729, USIJX = 729, IKT = 730, RUIKT = 730, ILE = 731, USILE = 731, ILG = 732, USILG = 732, ILI = 733, USILI = 733, ILM = 734, USILM = 734, ILP = 735, NCILP = 735, ILY = 736, GBISY = 736, ILZ = 737, SKILZ = 737, 
        IND = 738, USIND = 738, INI = 739, RSINI = 739, INL = 740, USINL = 740, INN = 741, ATINN = 741, INU = 742, NRYRN = 742, INV = 743, GBINV = 743, IOM = 744, IMDGS = 744, IOU = 745, NCIOU = 745, IPC = 746, CLIPC = 746, IPL = 747, USIDO = 747, IPT = 748, USIPT = 748, 
        IQT = 749, PEIQT = 749, IRG = 750, AUIRG = 750, IRO = 751, CFIRO = 751, ISA = 752, AUISA = 752, ISB = 753, PKISB = 753, ISG = 754, JPISG = 754, ISL = 755, USISN = 755, ISO = 756, USK5T = 756, ISP = 757, USXHO = 757, IST = 758, TRIST = 758, ITH = 759, USITH = 759, 
        ITO = 760, USITO = 760, IUE = 761, NUALO = 761, IVC = 762, NZIVC = 762, IVL = 763, FIIVL = 763, IXB = 764, INIXB = 764, IXC = 765, INIXC = 765, IXG = 766, INIXG = 766, IXJ = 767, INIXJ = 767, IXR = 768, INIXR = 768, IXW = 769, INIXW = 769, IYK = 770, USIYK = 770, 
        IZM = 771, TRADB = 771, JAA = 772, AFJAA = 772, JAC = 773, USJAC = 773, JAD = 774, AUJAD = 774, JAF = 775, LKJAF = 775, JAG = 776, PKJAG = 776, JAI = 777, INJAI = 777, JAK = 778, HTJAK = 778, JAL = 779, MXJAL = 779, JAM = 780, BGJAM = 780, JAN = 781, USJAN = 781, 
        JAU = 782, PEJAU = 782, JBR = 783, USJBR = 783, JCB = 784, BRJCB = 784, JCK = 785, AUJCK = 785, JCM = 786, BRJCA = 786, JDF = 787, BRJDF = 787, JDH = 788, INJDH = 788, JDZ = 789, CNJDZ = 789, JED = 790, SAJED = 790, JEE = 791, HTJEE = 791, JEF = 792, USJEF = 792, 
        JER = 793, JESTH = 793, JGA = 794, INJGA = 794, JGB = 795, INJGB = 795, JGN = 796, CNJGN = 796, JGS = 797, CNJIN = 797, JHB = 798, MYJHB = 798, JHE = 799, SEHEL = 799, JHG = 800, CNJOG = 800, JHM = 801, USJHM = 801, JHQ = 802, AUJHQ = 802, JHW = 803, USJHW = 803, 
        JIB = 804, DJJIB = 804, JIJ = 805, ETJIJ = 805, JIL = 806, CNJIL = 806, JIM = 807, ETJIM = 807, JIN = 808, UGJIN = 808, JIP = 809, ECJIP = 809, JIR = 810, NPJIR = 810, JIU = 811, CNJIU = 811, JIW = 812, PKJIW = 812, JJI = 813, PEJJI = 813, JJN = 814, CNJJI = 814, 
        JKG = 815, SEJKG = 815, JKH = 816, GRJKH = 816, JKR = 817, NPJKR = 817, JKT = 818, IDJKT = 818, JKV = 819, USJKV = 819, JLN = 820, USJLN = 820, JLR = 821, INJUC = 821, JLS = 822, BRJLS = 822, JMB = 823, AOJMB = 823, JMK = 824, GRJMK = 824, JMO = 825, NPJMO = 825, 
        JMS = 826, USJMS = 826, JMU = 827, CNJMU = 827, JNA = 828, BRJNA = 828, JNB = 829, ZAJNB = 829, JNG = 830, CNJNG = 830, JNI = 831, ARJNI = 831, JNU = 832, USJNU = 832, JNX = 833, GRJNX = 833, JNZ = 834, CNJZI = 834, JOE = 835, FIJOE = 835, JOI = 836, BRJOI = 836, 
        JOL = 837, PHJOL = 837, JON = 838, UMJON = 838, JOS = 839, NGJOS = 839, JPA = 840, BRJPA = 840, JQE = 841, PAJQE = 841, JRH = 842, INJRH = 842, JRO = 843, TZJRO = 843, JRS = 844, ILXXX = 844, JSA = 845, INJSA = 845, JSI = 846, GRJSI = 846, JSM = 847, ARJSM = 847, 
        JST = 848, USJST = 848, JTI = 849, BRJTI = 849, JTR = 850, GRJTR = 850, JUB = 851, SSJUB = 851, JUI = 852, DEJUI = 852, JUJ = 853, ARJUJ = 853, JUL = 854, PEJUL = 854, JUN = 855, AUJUN = 855, JUT = 856, HNJUT = 856, JUV = 857, GLJUV = 857, JVL = 858, USJVL = 858, 
        JWA = 859, BWJWA = 859, JXN = 860, USJXN = 860, JYV = 861, FIJYV = 861, KAJ = 862, FIKAJ = 862, KAN = 863, NGKAN = 863, KAO = 864, FIKAO = 864, KBL = 865, AFKBL = 865, KBY = 866, AUKBY = 866, KCC = 867, USKCC = 867, KCE = 868, AUKCE = 868, KCH = 869, MYKCH = 869, 
        KCM = 870, TRKCM = 870, KCZ = 871, JPKCZ = 871, KDD = 872, PKKDD = 872, KDH = 873, AFKDH = 873, KDU = 874, PKKDU = 874, KEL = 875, DEKEL = 875, KEM = 876, FIKEM = 876, KEN = 877, SLKEN = 877, KGC = 878, AUKGC = 878, KGD = 879, RUKGD = 879, KGI = 880, AUKGI = 880, 
        KGL = 881, RWKGL = 881, KGS = 882, GRKGS = 882, KHH = 883, TWKHH = 883, KHI = 884, PKKHI = 884, KHJ = 885, FIKHJ = 885, KHV = 886, RUKHV = 886, KID = 887, SEKID = 887, KIJ = 888, JPKIJ = 888, KIK = 889, IQKIK = 889, KIM = 890, ZAKIM = 890, KIN = 891, JMKIN = 891, 
        KIR = 892, IEKIR = 892, KIV = 893, MDKIV = 893, KIW = 894, ZMKIW = 894, KKN = 895, NOKKN = 895, KLA = 896, UGKLA = 896, KLR = 897, SEKLR = 897, KLU = 898, ATKLU = 898, KLW = 899, USKLW = 899, KLX = 900, GRKLX = 900, KLZ = 901, ZAKLZ = 901, KMI = 902, JPKMI = 902, 
        KMJ = 903, JPKMJ = 903, KMP = 904, NAKMP = 904, KMQ = 905, JPKMQ = 905, KMS = 906, GHKMS = 906, KNS = 907, AUKNS = 907, KNU = 908, INKNU = 908, KNX = 909, AUKNX = 909, KOA = 910, USKHH = 910, KOI = 911, GBKWL = 911, KOJ = 912, JPKOJ = 912, KOK = 913, FIKOK = 913, 
        KRB = 914, AUKRB = 914, KRK = 915, PLKRK = 915, KRN = 916, SEKRN = 916, KRP = 917, DKKRP = 917, KRS = 918, NOKRS = 918, KRT = 919, SDKRT = 919, KSD = 920, SEKSD = 920, KSL = 921, SDKSL = 921, KSS = 922, MLSIK = 922, KSU = 923, NOKSU = 923, KTA = 924, AUKTA = 924, 
        KTB = 925, USKTB = 925, KTM = 926, NPKTM = 926, KTN = 927, USKTN = 927, KTR = 928, AUKTR = 928, KTT = 929, FIKTT = 929, KUA = 930, MYKUA = 930, KUF = 931, RUKUF = 931, KUH = 932, JPKUH = 932, KUL = 933, MYKUL = 933, KUO = 934, FIKUO = 934, KVA = 935, GRKVA = 935, 
        KWI = 936, KWKWI = 936, KWL = 937, CNGUI = 937, KWM = 938, AUKWM = 938, KYA = 939, TRKYA = 939, KYN = 940, GBMIK = 940, KYS = 941, TRKSY = 941, KZN = 942, RUKZN = 942, LAD = 943, AOLAD = 943, LAE = 944, PGLAE = 944, LAF = 945, USLAF = 945, LAI = 946, FRLAI = 946, 
        LAN = 947, USLAN = 947, LAP = 948, MXLAP = 948, LAR = 949, USLAR = 949, LAS = 950, USLAS = 950, LAW = 951, USLAW = 951, LAX = 952, USLAX = 952, LBA = 953, GBLBA = 953, LBB = 954, USLBB = 954, LBE = 955, USLBE = 955, LBI = 956, FRBBI = 956, LBQ = 957, GALBQ = 957, 
        LBU = 958, MYLBU = 958, LBV = 959, GALBV = 959, LCA = 960, CYLCA = 960, LCG = 961, ESLCG = 961, LCH = 962, USLCH = 962, LDC = 963, AULDC = 963, LDE = 964, FRLDE = 964, LDK = 965, SELDK = 965, LDY = 966, GBLDY = 966, LEB = 967, USLN7 = 967, LED = 968, RULED = 968, 
        LEI = 969, ESLEI = 969, LEJ = 970, DELEJ = 970, LEK = 971, GNLEK = 971, LEQ = 972, GBLEQ = 972, LER = 973, AULER = 973, LEX = 974, USLEX = 974, LEY = 975, NLLEY = 975, LFI = 976, USYGZ = 976, LFT = 977, USLFT = 977, LFW = 978, TGLFW = 978, LGB = 979, USLGB = 979, 
        LGG = 980, BELGG = 980, LGK = 981, MYLGK = 981, LHE = 982, PKLHE = 982, LI0 = 983, LIVDZ = 983, LIF = 984, NCLIF = 984, LIG = 985, FRLIG = 985, LIH = 986, USLIU = 986, LIL = 987, FRLLE = 987, LIM = 988, PELIM = 988, LIQ = 989, CDLIQ = 989, LIS = 990, PTLIS = 990, 
        LIT = 991, USLIT = 991, LJU = 992, SILJU = 992, LKL = 993, NOLKL = 993, LKO = 994, INLKO = 994, LL0 = 995, PHNOP = 995, LLA = 996, SELLA = 996, LLW = 997, MWLLW = 997, LMM = 998, MXLMM = 998, LMP = 999, ITLMP = 999, LMT = 1000, USLMT = 1000, LNK = 1001, USLNK = 1001, 
        LNO = 1002, AULNO = 1002, LNS = 1003, USLNS = 1003, LNY = 1004, USLNY = 1004, LNZ = 1005, ATLNZ = 1005, LON = 1006, GBLON = 1006, LOS = 1007, NGLOS = 1007, LPA = 1008, ESLPG = 1008, LPB = 1009, BOLPB = 1009, LPL = 1010, GBLIV = 1010, LPP = 1011, FILPP = 1011, LRD = 1012, USLRD = 1012, 
        LRE = 1013, AULRE = 1013, LRF = 1014, USLRF = 1014, LRH = 1015, FRLRH = 1015, LRM = 1016, DOCDC = 1016, LRT = 1017, FRLRT = 1017, LSE = 1018, USLSE = 1018, LSI = 1019, GBLSI = 1019, LST = 1020, AULST = 1020, LSY = 1021, AULSY = 1021, LSZ = 1022, HRVLN = 1022, LTN = 1023, GBLUT = 1023, 
        LTO = 1024, MXLRO = 1024, LU0 = 1025, CHLZN = 1025, LUD = 1026, NALUD = 1026, LUG = 1027, CHLUG = 1027, LUJ = 1028, ZALUJ = 1028, LUM = 1029, CNLUM = 1029, LUN = 1030, ZMLUN = 1030, LUQ = 1031, ARLUQ = 1031, LUX = 1032, LULUX = 1032, LV0 = 1033, BELEU = 1033, LVO = 1034, AULVO = 1034, 
        LWB = 1035, USLWB = 1035, LWK = 1036, GBLER = 1036, LWO = 1037, UALWO = 1037, LWS = 1038, USLWS = 1038, LWT = 1039, USLWT = 1039, LXR = 1040, EGLXR = 1040, LYH = 1041, USLYH = 1041, LYO = 1042, USLYO = 1042, LYP = 1043, PKLYP = 1043, LYR = 1044, SJLYR = 1044, LYS = 1045, FRLIO = 1045, 
        LYX = 1046, GBLYX = 1046, LZC = 1047, MXLZC = 1047, MAA = 1048, INMAA = 1048, MAD = 1049, ESMAD = 1049, MAF = 1050, USMAF = 1050, MAH = 1051, ESMAH = 1051, MAJ = 1052, MHMAJ = 1052, MAM = 1053, MXMAM = 1053, MAN = 1054, GBMNC = 1054, MAO = 1055, BRMAO = 1055, MAR = 1056, VEMAR = 1056, 
        MAU = 1057, PFMAU = 1057, MAZ = 1058, PRMAZ = 1058, MBA = 1059, KEMBA = 1059, MBH = 1060, AUMBH = 1060, MBJ = 1061, JMMBJ = 1061, MBM = 1062, ZAMBM = 1062, MBS = 1063, USSGM = 1063, MBX = 1064, SIMBX = 1064, MC0 = 1065, USMOC = 1065, MCE = 1066, USMCE = 1066, MCI = 1067, USMKC = 1067, 
        MCL = 1068, USMCL = 1068, MCM = 1069, MCMON = 1069, MCN = 1070, USMCN = 1070, MCO = 1071, USORL = 1071, MCP = 1072, BRMCA = 1072, MCT = 1073, OMMCT = 1073, MCW = 1074, USMCW = 1074, MCY = 1075, AUMCY = 1075, MCZ = 1076, BRMCZ = 1076, MDC = 1077, IDMDC = 1077, MDE = 1078, COMDE = 1078, 
        MDL = 1079, MMMDL = 1079, MDQ = 1080, ARMDQ = 1080, MDY = 1081, UMMDY = 1081, MDZ = 1082, ARMDZ = 1082, MED = 1083, SAMED = 1083, MEE = 1084, NCMEE = 1084, MEI = 1085, USMEI = 1085, MEL = 1086, AUMEL = 1086, MEM = 1087, USMEM = 1087, MES = 1088, IDMES = 1088, MEX = 1089, MXMEX = 1089, 
        MEZ = 1090, ZAMEZ = 1090, MFE = 1091, USTXC = 1091, MFF = 1092, GAMFF = 1092, MFG = 1093, PKMFG = 1093, MFM = 1094, MOMFM = 1094, MFN = 1095, NZMFN = 1095, MFO = 1096, PGMFO = 1096, MFQ = 1097, NEMFQ = 1097, MFR = 1098, USMFR = 1098, MFU = 1099, ZMMFU = 1099, MGA = 1100, NIMGA = 1100, 
        MGB = 1101, AUMGB = 1101, MGF = 1102, BRMGF = 1102, MGH = 1103, ZAMGH = 1103, MGM = 1104, USMGM = 1104, MGQ = 1105, SOMGQ = 1105, MGW = 1106, USMGW = 1106, MHE = 1107, USMHE = 1107, MHH = 1108, BSMHH = 1108, MHQ = 1109, AXMHQ = 1109, MHT = 1110, USMHT = 1110, MIA = 1111, USMIA = 1111, 
        MID = 1112, MXMID = 1112, MIK = 1113, FIMIK = 1113, MIL = 1114, ITMIL = 1114, MIM = 1115, AUMIM = 1115, MIR = 1116, TNMIR = 1116, MJC = 1117, CIMJC = 1117, MJD = 1118, PKMJD = 1118, MJL = 1119, GAMJL = 1119, MJN = 1120, MGMJN = 1120, MJQ = 1121, USMJQ = 1121, MJT = 1122, GRMJT = 1122, 
        MJV = 1123, ESMJV = 1123, MKC = 1124, PHMKC = 1124, MKE = 1125, USMKE = 1125, MKG = 1126, USMKG = 1126, MKK = 1127, USKUA = 1127, MKL = 1128, USMKL = 1128, MKR = 1129, AUMKR = 1129, MKY = 1130, AUMKY = 1130, MLA = 1131, MTMLA = 1131, MLB = 1132, USMLB = 1132, MLE = 1133, MVMLE = 1133, 
        MLH = 1134, FRMLH = 1134, MLI = 1135, USMLI = 1135, MLM = 1136, MXMLM = 1136, MLS = 1137, USMLS = 1137, MLU = 1138, USMLU = 1138, MLW = 1139, LRMLW = 1139, MLX = 1140, TRMLX = 1140, MMA = 1141, SEMMA = 1141, MME = 1142, GBMME = 1142, MMG = 1143, AUMMG = 1143, MMJ = 1144, JPMMJ = 1144, 
        MMK = 1145, RUMMK = 1145, MMM = 1146, AUMMM = 1146, MMY = 1147, JPMMY = 1147, MNI = 1148, MSBRD = 1148, MNL = 1149, PHMNL = 1149, MOB = 1150, USMOB = 1150, MOD = 1151, USMOD = 1151, MON = 1152, NZMON = 1152, MOT = 1153, USMOT = 1153, MOV = 1154, AUMOV = 1154, MOW = 1155, RUMOW = 1155, 
        MOZ = 1156, PFMOZ = 1156, MPA = 1157, NAKAT = 1157, MPD = 1158, PKMPD = 1158, MPL = 1159, FRMPL = 1159, MPM = 1160, MZMPM = 1160, MPV = 1161, USMPI = 1161, MQL = 1162, AUMQL = 1162, MQM = 1163, TRMDN = 1163, MQQ = 1164, TDMQQ = 1164, MQT = 1165, USMQT = 1165, MRB = 1166, USMRB = 1166, 
        MRS = 1167, FRMRS = 1167, MRU = 1168, MUPLU = 1168, MRV = 1169, RUMRV = 1169, MRY = 1170, USMY3 = 1170, MRZ = 1171, AUMRZ = 1171, MSE = 1172, GBMSE = 1172, MSL = 1173, USUSH = 1173, MSN = 1174, USMSN = 1174, MSO = 1175, USMSO = 1175, MSP = 1176, USMES = 1176, MSQ = 1177, BYMSQ = 1177, 
        MSR = 1178, TRMSR = 1178, MST = 1179, NLMST = 1179, MSU = 1180, LSMSU = 1180, MSY = 1181, USMSY = 1181, MTH = 1182, USMTH = 1182, MTJ = 1183, USMTJ = 1183, MTL = 1184, AUMTL = 1184, MTM = 1185, USMTM = 1185, MTO = 1186, USMTO = 1186, MTS = 1187, SZMTS = 1187, MTT = 1188, MXMTT = 1188, 
        MTY = 1189, MXMTY = 1189, MUB = 1190, BWMUB = 1190, MUC = 1191, DEMUC = 1191, MUE = 1192, USMUE = 1192, MUH = 1193, EGMUH = 1193, MUX = 1194, PKMUX = 1194, MV0 = 1195, USVYV = 1195, MVB = 1196, GAMVE = 1196, MVD = 1197, UYMVD = 1197, MVR = 1198, CMMVR = 1198, MVY = 1199, USMVY = 1199, 
        MVZ = 1200, ZWMVZ = 1200, MWD = 1201, PKMWD = 1201, MWH = 1202, USMWH = 1202, MXL = 1203, MXMXL = 1203, MYA = 1204, AUMYA = 1204, MYD = 1205, KEMYD = 1205, MYJ = 1206, JPMYJ = 1206, MYQ = 1207, INMYQ = 1207, MYR = 1208, USMYR = 1208, MYY = 1209, MYMYY = 1209, MZG = 1210, TWMZG = 1210, 
        MZM = 1211, FRMZM = 1211, MZR = 1212, AFMZR = 1212, MZT = 1213, MXMZT = 1213, MZY = 1214, ZAMZY = 1214, NAA = 1215, AUNAA = 1215, NAG = 1216, INNAG = 1216, NAJ = 1217, AZNAJ = 1217, NAN = 1218, FJNAN = 1218, NAP = 1219, ITNAP = 1219, NAS = 1220, BSNAS = 1220, NAT = 1221, BRNAT = 1221, 
        NBO = 1222, KENBO = 1222, NCE = 1223, FRNCE = 1223, NCL = 1224, GBNCS = 1224, NCS = 1225, ZANCS = 1225, NCY = 1226, FRNCY = 1226, NDB = 1227, MRNDB = 1227, NDJ = 1228, TDNDJ = 1228, NDU = 1229, NANDU = 1229, NEV = 1230, KNNEV = 1230, NGE = 1231, CMNGE = 1231, NGO = 1232, JPNGO = 1232, 
        NGS = 1233, JPNGS = 1233, NIC = 1234, CYNIC = 1234, NIM = 1235, NENIM = 1235, NKC = 1236, MRNKC = 1236, NKW = 1237, IODGA = 1237, NLA = 1238, ZMNLA = 1238, NLD = 1239, MXNLD = 1239, NLK = 1240, NFNLK = 1240, NLP = 1241, ZANLP = 1241, NLV = 1242, UANLV = 1242, NNG = 1243, CNNIN = 1243, 
        NOC = 1244, IENOC = 1244, NOU = 1245, NCNOU = 1245, NQY = 1246, GBNQY = 1246, NRA = 1247, AUNRA = 1247, NRK = 1248, SENRK = 1248, NS0 = 1249, USNQY = 1249, NSA = 1250, AUNSA = 1250, NSN = 1251, NZNSN = 1251, NSO = 1252, AUNSO = 1252, NST = 1253, THNKT = 1253, NTE = 1254, FRNTE = 1254, 
        NTL = 1255, AUNTL = 1255, NTY = 1256, ZAPIB = 1256, NUE = 1257, DENUE = 1257, NWI = 1258, GBNRW = 1258, NYC = 1259, USNYC = 1259, OAG = 1260, AUOAG = 1260, OAJ = 1261, USOAJ = 1261, OAK = 1262, USOAK = 1262, OAX = 1263, MXOAX = 1263, OB0 = 1264, DEOLO = 1264, ODA = 1265, CFODA = 1265, 
        ODB = 1266, ESODB = 1266, ODE = 1267, DKODE = 1267, ODS = 1268, UAODS = 1268, OGG = 1269, USOGG = 1269, OHD = 1270, MKOHD = 1270, OHT = 1271, PKOHT = 1271, OIT = 1272, JPOIT = 1272, OKA = 1273, JPOKA = 1273, OKC = 1274, USOKC = 1274, OKD = 1275, JPSPK = 1275, OKJ = 1276, JPOKJ = 1276, 
        OKU = 1277, NAOKU = 1277, OLB = 1278, ITOLB = 1278, OLF = 1279, USOLF = 1279, OLM = 1280, USOLM = 1280, OLP = 1281, AUOLP = 1281, OMA = 1282, USOMA = 1282, OMD = 1283, NAOMD = 1283, OME = 1284, USOME = 1284, OMH = 1285, IRUIM = 1285, OMO = 1286, BAOMO = 1286, OND = 1287, NAOND = 1287, 
        ONT = 1288, USONT = 1288, OOL = 1289, AUGOC = 1289, OOM = 1290, AUCOM = 1290, OPO = 1291, PTOPO = 1291, ORB = 1292, SEORB = 1292, ORF = 1293, USORF = 1293, ORH = 1294, USORH = 1294, ORK = 1295, IEORK = 1295, ORN = 1296, DZORN = 1296, OSA = 1297, JPOSA = 1297, OSH = 1298, USOSH = 1298, 
        OSI = 1299, HROSI = 1299, OSL = 1300, NOOSL = 1300, OSM = 1301, IQOSM = 1301, OSR = 1302, CZOSR = 1302, OTH = 1303, USOTH = 1303, OTZ = 1304, USOTZ = 1304, OUA = 1305, BFOUA = 1305, OUD = 1306, MAOUD = 1306, OUH = 1307, ZAOUH = 1307, OUK = 1308, GBOUK = 1308, OUL = 1309, FIOUL = 1309, 
        OUZ = 1310, MROUZ = 1310, OVB = 1311, RUOVB = 1311, OVD = 1312, ESOVO = 1312, OWB = 1313, USOWB = 1313, OXB = 1314, GWOXB = 1314, OXR = 1315, USOXN = 1315, OZH = 1316, UAZPR = 1316, OZZ = 1317, MAOZZ = 1317, PA0 = 1318, BRPWA = 1318, PAC = 1319, PAPTY = 1319, PAD = 1320, DEPAD = 1320, 
        PAH = 1321, USPAH = 1321, PAP = 1322, HTPAP = 1322, PAR = 1323, FRPAR = 1323, PAT = 1324, INPAT = 1324, PB0 = 1325, USPB8 = 1325, PBC = 1326, MXPBC = 1326, PBH = 1327, BTTHI = 1327, PBI = 1328, USPBI = 1328, PBM = 1329, SRPBM = 1329, PBO = 1330, AUPBO = 1330, PBZ = 1331, ZAPBZ = 1331, 
        PC0 = 1332, PHPSG = 1332, PCT = 1333, USPCT = 1333, PDL = 1334, PTPDL = 1334, PDT = 1335, USPDT = 1335, PDX = 1336, USPDX = 1336, PEG = 1337, ITPEG = 1337, PEI = 1338, COPEI = 1338, PEN = 1339, MYPEN = 1339, PER = 1340, AUPER = 1340, PEW = 1341, PKPEW = 1341, PFN = 1342, USPFN = 1342, 
        PFO = 1343, CYPFO = 1343, PGA = 1344, USPGA = 1344, PGF = 1345, FRPGF = 1345, PGV = 1346, USPGV = 1346, PHC = 1347, NGPHC = 1347, PHE = 1348, AUPHE = 1348, PHF = 1349, USNNS = 1349, PHH = 1350, NPPKR = 1350, PHL = 1351, USPHL = 1351, PHW = 1352, ZAPHW = 1352, PHX = 1353, USPHX = 1353, 
        PIA = 1354, USPIA = 1354, PIB = 1355, USLUL = 1355, PIH = 1356, USPIH = 1356, PIR = 1357, USPIR = 1357, PIS = 1358, FRPIS = 1358, PIT = 1359, USPIT = 1359, PJG = 1360, PKPJG = 1360, PKB = 1361, USPKB = 1361, PKW = 1362, BWPKW = 1362, PLB = 1363, USPLB = 1363, PLN = 1364, USPXN = 1364, 
        PLO = 1365, AUPLO = 1365, PLZ = 1366, ZAPLZ = 1366, PMD = 1367, USQPL = 1367, PMI = 1368, ESPMI = 1368, PMO = 1369, ITPMO = 1369, PMR = 1370, NZPMR = 1370, PMV = 1371, VEMGT = 1371, PMW = 1372, BRPMW = 1372, PN0 = 1373, PNPCN = 1373, PNH = 1374, KHPNH = 1374, PNI = 1375, FMPNI = 1375, 
        PNL = 1376, ITPNL = 1376, PNQ = 1377, INPNQ = 1377, PNR = 1378, CGPNR = 1378, PNS = 1379, USPNS = 1379, POA = 1380, BRPOA = 1380, POG = 1381, GAPOG = 1381, POM = 1382, PGPOM = 1382, POP = 1383, DOPOP = 1383, POR = 1384, FIPOR = 1384, POS = 1385, TTPOS = 1385, POU = 1386, USPOU = 1386, 
        POZ = 1387, PLPOZ = 1387, PPG = 1388, ASPPG = 1388, PPP = 1389, AUPPP = 1389, PPT = 1390, PFPPT = 1390, PQI = 1391, USPQI = 1391, PQQ = 1392, AUPQQ = 1392, PRG = 1393, CZPRG = 1393, PRN = 1394, XXPRN = 1394, PRY = 1395, ZAPRY = 1395, PS0 = 1396, PSNAB = 1396, PSA = 1397, ITPSA = 1397, 
        PSC = 1398, USPSC = 1398, PSD = 1399, EGPSD = 1399, PSE = 1400, PRPSE = 1400, PSG = 1401, USPSG = 1401, PSI = 1402, PKPSI = 1402, PSP = 1403, USPSP = 1403, PSR = 1404, ITPSR = 1404, PSY = 1405, FKPSY = 1405, PT0 = 1406, BJPTN = 1406, PTG = 1407, ZAPOL = 1407, PTJ = 1408, AUPTJ = 1408, 
        PUB = 1409, USPUB = 1409, PUF = 1410, FRPUF = 1410, PUG = 1411, AUPUG = 1411, PUJ = 1412, DOPCN = 1412, PUQ = 1413, CLPUQ = 1413, PUS = 1414, KRPUS = 1414, PUW = 1415, USPUW = 1415, PUY = 1416, HRPUY = 1416, PVD = 1417, USPVD = 1417, PVH = 1418, BRPVH = 1418, PVK = 1419, GRPVK = 1419, 
        PVR = 1420, MXPVR = 1420, PWM = 1421, USPWM = 1421, PXM = 1422, MXPXM = 1422, PXO = 1423, PTPXO = 1423, PYB = 1424, INPYB = 1424, PYX = 1425, THPYX = 1425, PZB = 1426, ZAPZB = 1426, PZH = 1427, PKPZH = 1427, PZO = 1428, VEPZO = 1428, QCB = 1429, JPCHB = 1429, QGF = 1430, BRMON = 1430, 
        QIC = 1431, ITBIE = 1431, QLI = 1432, CYLMS = 1432, QML = 1433, PKMPR = 1433, QMN = 1434, SZQMN = 1434, QNB = 1435, INQNB = 1435, QND = 1436, RSNVS = 1436, QPA = 1437, ITPDA = 1437, QSA = 1438, ESSBP = 1438, RA0 = 1439, DKRAN = 1439, RAI = 1440, CVRAI = 1440, RAJ = 1441, INRAJ = 1441, 
        RAK = 1442, MARAK = 1442, RAP = 1443, USRAP = 1443, RAR = 1444, CKRAR = 1444, RAZ = 1445, PKRAZ = 1445, RB0 = 1446, DEREG = 1446, RBA = 1447, MARBA = 1447, RBR = 1448, BRRBR = 1448, RCB = 1449, ZARCB = 1449, RDD = 1450, USRDD = 1450, RDG = 1451, USRDG = 1451, RDM = 1452, USRMD = 1452, 
        RDU = 1453, USRAG = 1453, RDZ = 1454, FRRDZ = 1454, REC = 1455, BRREC = 1455, REG = 1456, ITREG = 1456, REK = 1457, ISREY = 1457, REU = 1458, ESRUS = 1458, RFD = 1459, USRFD = 1459, RFP = 1460, PFRFP = 1460, RGI = 1461, PFRGI = 1461, RGL = 1462, ARRGL = 1462, RGN = 1463, MMRGN = 1463, 
        RHG = 1464, RWRHG = 1464, RHI = 1465, USRHI = 1465, RHO = 1466, GRRHO = 1466, RIC = 1467, USRIC = 1467, RIO = 1468, BRRIO = 1468, RIX = 1469, LVRIX = 1469, RIZ = 1470, CNRZH = 1470, RJK = 1471, HRRJK = 1471, RKD = 1472, USRKD = 1472, RKS = 1473, USRKS = 1473, RKT = 1474, AERKT = 1474, 
        RLT = 1475, NERLT = 1475, RMF = 1476, EGRMF = 1476, RMI = 1477, ITRMI = 1477, RNB = 1478, SERNB = 1478, RNE = 1479, FRRNE = 1479, RNN = 1480, DKRNN = 1480, RNO = 1481, USRNO = 1481, RNS = 1482, FRRNS = 1482, ROA = 1483, USROA = 1483, ROC = 1484, USROC = 1484, ROK = 1485, AUROK = 1485, 
        ROM = 1486, ITROM = 1486, ROR = 1487, PWROR = 1487, ROS = 1488, ARROS = 1488, ROT = 1489, NZROT = 1489, ROU = 1490, BGRDU = 1490, ROV = 1491, RUROV = 1491, RR0 = 1492, CARRV = 1492, RRG = 1493, MURRG = 1493, RS0 = 1494, GEZAK = 1494, RSA = 1495, ARRSA = 1495, RSD = 1496, BSRSD = 1496, 
        RST = 1497, USRST = 1497, RTB = 1498, HNRTB = 1498, RTM = 1499, NLRTM = 1499, RUH = 1500, SARUH = 1500, RUN = 1501, RERUN = 1501, RVN = 1502, FIRVN = 1502, RWI = 1503, USRWI = 1503, RWP = 1504, PKRWP = 1504, RYK = 1505, PKRYK = 1505, SAF = 1506, USSAF = 1506, SAH = 1507, YESAH = 1507, 
        SAI = 1508, KHREP = 1508, SAL = 1509, SVSAL = 1509, SAN = 1510, USSAN = 1510, SAO = 1511, BRSAO = 1511, SAP = 1512, HNSAP = 1512, SAT = 1513, USSAT = 1513, SAV = 1514, USSAV = 1514, SBA = 1515, USSBA = 1515, SBH = 1516, BLSBH = 1516, SBK = 1517, FRSBK = 1517, SBN = 1518, USSBN = 1518, 
        SBP = 1519, USCSL = 1519, SBU = 1520, ZASBU = 1520, SBW = 1521, MYSBW = 1521, SBY = 1522, USSBY = 1522, SBZ = 1523, ROSBZ = 1523, SCC = 1524, USSCC = 1524, SCE = 1525, USSCE = 1525, SCF = 1526, USSTZ = 1526, SCK = 1527, USSCK = 1527, SCN = 1528, DESCN = 1528, SCQ = 1529, ESSCQ = 1529, 
        SCU = 1530, CUSCU = 1530, SDF = 1531, USLUI = 1531, SDH = 1532, HNSDH = 1532, SDJ = 1533, JPSDJ = 1533, SDL = 1534, SESDL = 1534, SDQ = 1535, DOSDQ = 1535, SDR = 1536, ESSDR = 1536, SDS = 1537, JPSOS = 1537, SDT = 1538, PKSDT = 1538, SDY = 1539, USSDY = 1539, SEA = 1540, USSEA = 1540, 
        SEB = 1541, LYSEB = 1541, SEL = 1542, KRSEL = 1542, SEN = 1543, GBSND = 1543, SEW = 1544, EGSEW = 1544, SEZ = 1545, SCVIC = 1545, SFA = 1546, TNSFA = 1546, SFG = 1547, MFMAR = 1547, SFJ = 1548, GLSFJ = 1548, SFN = 1549, ARSFN = 1549, SFO = 1550, USSFO = 1550, SGD = 1551, DKSGD = 1551, 
        SGF = 1552, USSGF = 1552, SGN = 1553, VNSGN = 1553, SGU = 1554, USSGU = 1554, SGY = 1555, USSGY = 1555, SH0 = 1556, NLHTB = 1556, SHA = 1557, CNSGH = 1557, SHD = 1558, USSZH = 1558, SHE = 1559, CNSHY = 1559, SHJ = 1560, AESHJ = 1560, SHR = 1561, USSHR = 1561, SHS = 1562, JPSHS = 1562, 
        SHV = 1563, USSHV = 1563, SI0 = 1564, DKSLB = 1564, SID = 1565, CVSID = 1565, SIN = 1566, SGSIN = 1566, SIP = 1567, UASIP = 1567, SIS = 1568, ZASIS = 1568, SIT = 1569, USSIT = 1569, SIX = 1570, AUSIX = 1570, SJC = 1571, USSJC = 1571, SJD = 1572, MXLCB = 1572, SJJ = 1573, BASJJ = 1573, 
        SJO = 1574, CRSJO = 1574, SJT = 1575, USSJT = 1575, SJU = 1576, PRSJU = 1576, SJY = 1577, FISJY = 1577, SK0 = 1578, GMSKE = 1578, SKB = 1579, KNSKB = 1579, SKD = 1580, UZSKD = 1580, SKG = 1581, GRSKG = 1581, SKP = 1582, MKSKP = 1582, SKS = 1583, DKSKR = 1583, SKV = 1584, EGSKV = 1584, 
        SKX = 1585, RUSKX = 1585, SKZ = 1586, PKSKZ = 1586, SLA = 1587, ARSLA = 1587, SLC = 1588, USSLC = 1588, SLE = 1589, USSLE = 1589, SLL = 1590, OMSLL = 1590, SLP = 1591, MXSLP = 1591, SLS = 1592, BGSLS = 1592, SLU = 1593, LCCAS = 1593, SLZ = 1594, BRSLZ = 1594, SM0 = 1595, SMSAI = 1595, 
        SMA = 1596, PTSMA = 1596, SMF = 1597, USSAC = 1597, SMI = 1598, GRSMI = 1598, SMX = 1599, USSMX = 1599, SNA = 1600, USSNA = 1600, SNC = 1601, ECSNC = 1601, SNN = 1602, IELMK = 1602, SNS = 1603, USSNS = 1603, SOF = 1604, BGSOF = 1604, SOG = 1605, NOSOG = 1605, SON = 1606, VUSAN = 1606, 
        SOT = 1607, FISOT = 1607, SOU = 1608, GBSOU = 1608, SPC = 1609, ESSPC = 1609, SPI = 1610, USSPI = 1610, SPN = 1611, MPSPN = 1611, SPS = 1612, USSPS = 1612, SPU = 1613, HRSPU = 1613, SPY = 1614, CISPY = 1614, SRA = 1615, BRSRA = 1615, SRB = 1616, BOSRB = 1616, SRL = 1617, MXSRL = 1617, 
        SRQ = 1618, USZOR = 1618, SRZ = 1619, BOSCS = 1619, SSA = 1620, BRSSA = 1620, SSG = 1621, GQSSG = 1621, SSH = 1622, EGSSH = 1622, SSL = 1623, COSSL = 1623, ST0 = 1624, USXLX = 1624, STI = 1625, CLSCL = 1625, STL = 1626, USSTL = 1626, STO = 1627, SESTO = 1627, STP = 1628, USSTP = 1628, 
        STR = 1629, DESTR = 1629, STS = 1630, USSTS = 1630, STT = 1631, VISTT = 1631, STV = 1632, INSTV = 1632, STX = 1633, VISTX = 1633, SUB = 1634, IDSUB = 1634, SUF = 1635, ITSUF = 1635, SUL = 1636, PKSUL = 1636, SUN = 1637, US5SV = 1637, SUV = 1638, FJSUV = 1638, SUX = 1639, USSUX = 1639, 
        SVD = 1640, VCKTN = 1640, SVG = 1641, NOSVG = 1641, SVL = 1642, FISVL = 1642, SVQ = 1643, ESSVQ = 1643, SVX = 1644, RUYEK = 1644, SW0 = 1645, DESWR = 1645, SWF = 1646, USSWF = 1646, SWP = 1647, NASWP = 1647, SWS = 1648, GBSWA = 1648, SXB = 1649, FRSXB = 1649, SXL = 1650, IESLI = 1650, 
        SXM = 1651, SXPHI = 1651, SXR = 1652, INSXR = 1652, SYD = 1653, AUSYD = 1653, SYR = 1654, USSYR = 1654, SYY = 1655, GBSTO = 1655, SZD = 1656, GBSHE = 1656, SZF = 1657, TRSSX = 1657, SZG = 1658, ATSZG = 1658, SZK = 1659, ZASZK = 1659, SZX = 1660, CNSNZ = 1660, SZZ = 1661, PLSZZ = 1661, 
        TAB = 1662, TTSCR = 1662, TAK = 1663, JPTAK = 1663, TAM = 1664, MXTAM = 1664, TAO = 1665, CNQIN = 1665, TAS = 1666, UZTAS = 1666, TAY = 1667, TWTPE = 1667, TBO = 1668, SNTOU = 1668, TBS = 1669, GETBS = 1669, TBU = 1670, TOTBU = 1670, TC0 = 1671, USTEL = 1671, TCA = 1672, AUTCA = 1672, 
        TCB = 1673, BSTCB = 1673, TCI = 1674, ESTCI = 1674, TCL = 1675, USTCL = 1675, TCU = 1676, ZATCU = 1676, TED = 1677, DKTED = 1677, TEM = 1678, AUTEM = 1678, TEQ = 1679, TRTEK = 1679, TER = 1680, PTTER = 1680, TEU = 1681, NZTEU = 1681, TEX = 1682, USQTR = 1682, TF0 = 1683, TFPFR = 1683, 
        TGD = 1684, METGD = 1684, TGU = 1685, HNTGU = 1685, TGV = 1686, BGTGV = 1686, TGZ = 1687, MXTGZ = 1687, THE = 1688, BRTHE = 1688, THR = 1689, IRTHR = 1689, THU = 1690, GLTHU = 1690, TIA = 1691, ALTIA = 1691, TIF = 1692, SATIF = 1692, TIJ = 1693, MXTIJ = 1693, TIP = 1694, LYTIP = 1694, 
        TIS = 1695, AUTIS = 1695, TIV = 1696, METIV = 1696, TK0 = 1697, TKNKU = 1697, TKA = 1698, USTKA = 1698, TKS = 1699, JPTKS = 1699, TKU = 1700, FITKU = 1700, TLH = 1701, USTLH = 1701, TLL = 1702, EETLL = 1702, TLS = 1703, FRTLS = 1703, TLV = 1704, ILTLV = 1704, TML = 1705, GHTML = 1705, 
        TMP = 1706, FITMP = 1706, TMS = 1707, STTMS = 1707, TMW = 1708, AUTMW = 1708, TMZ = 1709, UZTER = 1709, TNA = 1710, CNJNA = 1710, TNG = 1711, MATNG = 1711, TNR = 1712, MGTNR = 1712, TOD = 1713, MYTOD = 1713, TOL = 1714, USTOL = 1714, TOS = 1715, NOTOS = 1715, TOU = 1716, NCTOU = 1716, 
        TOV = 1717, VGTOV = 1717, TOY = 1718, JPTOY = 1718, TPA = 1719, USTPA = 1719, TPR = 1720, AUTPR = 1720, TPS = 1721, ITTPS = 1721, TRD = 1722, NOTRD = 1722, TRI = 1723, USOUI = 1723, TRN = 1724, ITTRN = 1724, TRO = 1725, AUTRO = 1725, TRS = 1726, ITTRS = 1726, TRV = 1727, INTRV = 1727, 
        TRW = 1728, KITRW = 1728, TRZ = 1729, INTRI = 1729, TSB = 1730, NATSB = 1730, TSE = 1731, KZAST = 1731, TSF = 1732, ITTRV = 1732, TSN = 1733, CNTNJ = 1733, TSV = 1734, AUTSV = 1734, TTN = 1735, USTTN = 1735, TUC = 1736, ARTUC = 1736, TUK = 1737, PKTUK = 1737, TUL = 1738, USTUL = 1738, 
        TUN = 1739, TNTUN = 1739, TUP = 1740, USTUP = 1740, TUS = 1741, USTUZ = 1741, TUU = 1742, SATUU = 1742, TVC = 1743, USTVC = 1743, TVF = 1744, USTVF = 1744, TVL = 1745, USTVL = 1745, TWB = 1746, AUTWB = 1746, TWF = 1747, USTWF = 1747, TWU = 1748, MYTWU = 1748, TXK = 1749, USTXK = 1749, 
        TYN = 1750, CNTYU = 1750, TYO = 1751, JPTYO = 1751, TYR = 1752, USTYR = 1752, TYS = 1753, USTYS = 1753, TZX = 1754, TRTZX = 1754, UAH = 1755, PFUAH = 1755, UAK = 1756, GLUAK = 1756, UAP = 1757, PFUAP = 1757, UBA = 1758, BRUBB = 1758, UBJ = 1759, JPUBJ = 1759, UBP = 1760, THUBP = 1760, 
        UCA = 1761, USUCA = 1761, UCT = 1762, RUUCT = 1762, UDE = 1763, NLUDE = 1763, UDI = 1764, BRUDI = 1764, UDJ = 1765, UAUZH = 1765, UDR = 1766, INUDR = 1766, UEE = 1767, AUUEE = 1767, UET = 1768, PKUET = 1768, UFA = 1769, RUUFA = 1769, UGC = 1770, UZUGC = 1770, UGO = 1771, AOUGO = 1771, 
        UHE = 1772, CZUHE = 1772, UII = 1773, HNUII = 1773, UIN = 1774, USUIN = 1774, UIO = 1775, ECUIO = 1775, UIP = 1776, FRUIP = 1776, UIT = 1777, MHUIT = 1777, UKB = 1778, JPUKB = 1778, UKI = 1779, USUKI = 1779, UKY = 1780, JPUKY = 1780, ULB = 1781, VUULB = 1781, ULD = 1782, ZAULD = 1782, 
        ULN = 1783, MNULN = 1783, ULU = 1784, UGULU = 1784, ULY = 1785, RUULY = 1785, UMD = 1786, GLUMD = 1786, UME = 1787, SEUME = 1787, UMR = 1788, AUUMR = 1788, UNI = 1789, VCUNI = 1789, UNK = 1790, USUNK = 1790, UNT = 1791, GBUNT = 1791, UPG = 1792, IDUPG = 1792, UPL = 1793, CRUPA = 1793, 
        UPN = 1794, MX2NB = 1794, UPP = 1795, USUPP = 1795, URC = 1796, CNURM = 1796, URG = 1797, BRURG = 1797, URM = 1798, VEURM = 1798, USH = 1799, ARUSH = 1799, USN = 1800, KRUSN = 1800, UTC = 1801, NLUTC = 1801, UTH = 1802, THUTH = 1802, UTN = 1803, ZAUTN = 1803, UTP = 1804, THUTP = 1804, 
        UTT = 1805, ZAUTT = 1805, UUD = 1806, RUUUD = 1806, UVE = 1807, GAOYE = 1807, UVF = 1808, LCVIF = 1808, UVL = 1809, EGUVL = 1809, VA0 = 1810, VAVAT = 1810, VAA = 1811, FIVAA = 1811, VAN = 1812, TRVAN = 1812, VAP = 1813, CLVAP = 1813, VAR = 1814, BGVAR = 1814, VAS = 1815, TRVAS = 1815, 
        VBS = 1816, ITBRC = 1816, VBY = 1817, SEVBY = 1817, VCE = 1818, ITVCE = 1818, VDE = 1819, ESVDH = 1819, VDZ = 1820, USVDZ = 1820, VEL = 1821, USVEL = 1821, VER = 1822, MXVER = 1822, VFA = 1823, ZWVFA = 1823, VGO = 1824, ESVGO = 1824, VID = 1825, BGVID = 1825, VIE = 1826, ATVIE = 1826, 
        VIJ = 1827, VGVIJ = 1827, VIS = 1828, USVIS = 1828, VIT = 1829, ESVIT = 1829, VIX = 1830, BRVIX = 1830, VLC = 1831, ESVLC = 1831, VLD = 1832, USVLD = 1832, VLI = 1833, VUVLI = 1833, VLL = 1834, ESVLL = 1834, VLN = 1835, VEVLN = 1835, VLU = 1836, RUVLU = 1836, VN0 = 1837, USVIV = 1837, 
        VNO = 1838, LTVIL = 1838, VNS = 1839, INVNS = 1839, VOG = 1840, RUVOG = 1840, VPS = 1841, USFWB = 1841, VRA = 1842, CUVRA = 1842, VRB = 1843, USVRB = 1843, VRK = 1844, FIVRK = 1844, VRN = 1845, ITVRN = 1845, VSA = 1846, MXVSA = 1846, VST = 1847, SEVST = 1847, VTE = 1848, LAVTE = 1848, 
        VVO = 1849, RUVVO = 1849, VXO = 1850, SEVXO = 1850, VY0 = 1851, USVYI = 1851, VYD = 1852, ZAVYD = 1852, VZ0 = 1853, HRVZN = 1853, WAM = 1854, AUWAM = 1854, WAW = 1855, PLWAW = 1855, WDH = 1856, NAWDH = 1856, WEI = 1857, AUWEI = 1857, WEL = 1858, ZAWEL = 1858, WGA = 1859, AUWGA = 1859, 
        WHK = 1860, NZWHK = 1860, WIC = 1861, GBWCK = 1861, WIE = 1862, DEWIB = 1862, WLB = 1863, USWLB = 1863, WLG = 1864, NZWLG = 1864, WLS = 1865, WFMAU = 1865, WMB = 1866, AUWMB = 1866, WN0 = 1867, USWLZ = 1867, WNS = 1868, PKWNS = 1868, WOL = 1869, AUWOL = 1869, WP0 = 1870, USWPK = 1870, 
        WRE = 1871, NZWRE = 1871, WRG = 1872, USWRG = 1872, WRL = 1873, USWRL = 1873, WRO = 1874, PLWRO = 1874, WSY = 1875, AUWSI = 1875, WUH = 1876, CNNHN = 1876, WUN = 1877, AUWUN = 1877, WUX = 1878, CNWXS = 1878, WVB = 1879, NAWVB = 1879, WYA = 1880, AUWYA = 1880, WYN = 1881, AUWYN = 1881, 
        WYS = 1882, USWYS = 1882, XCH = 1883, CXFFC = 1883, XIY = 1884, CNXYA = 1884, XMH = 1885, PFXMH = 1885, XMN = 1886, CNXAM = 1886, XPK = 1887, CAXXW = 1887, XRY = 1888, ESJRZ = 1888, XSI = 1889, CAXSI = 1889, XXX = 1890, XXXXX = 1890, YAK = 1891, USYAK = 1891, YAO = 1892, CMYAO = 1892, 
        YAT = 1893, CAYAT = 1893, YBE = 1894, CAYBE = 1894, YCB = 1895, CAYCB = 1895, YDF = 1896, CAYDF = 1896, YEA = 1897, CAEDM = 1897, YEV = 1898, CAYEV = 1898, YFA = 1899, CAFAL = 1899, YFB = 1900, CAIQL = 1900, YFC = 1901, CAFRE = 1901, YFO = 1902, CAYFO = 1902, YGW = 1903, CAXXX = 1903, 
        YGX = 1904, CAYGX = 1904, YHR = 1905, CAYHR = 1905, YHZ = 1906, CAHAL = 1906, YIF = 1907, CASAU = 1907, YIH = 1908, CNYIC = 1908, YKA = 1909, CAKML = 1909, YKM = 1910, USYKM = 1910, YKS = 1911, RUYKS = 1911, YLB = 1912, CAXXY = 1912, YLR = 1913, CALRA = 1913, YLW = 1914, CAKWL = 1914, 
        YMM = 1915, CAYMM = 1915, YMQ = 1916, CAMTR = 1916, YNB = 1917, SAYNB = 1917, YOK = 1918, JPYOK = 1918, YOP = 1919, CARBL = 1919, YOW = 1920, CAOTT = 1920, YPN = 1921, CAPME = 1921, YPR = 1922, CAPRR = 1922, YQB = 1923, CAQUE = 1923, YQD = 1924, CAYQD = 1924, YQG = 1925, CAWND = 1925, 
        YQM = 1926, CAMNT = 1926, YQR = 1927, CAREG = 1927, YQT = 1928, CATHU = 1928, YQX = 1929, CAGAN = 1929, YRB = 1930, CAREB = 1930, YSJ = 1931, CASJB = 1931, YSM = 1932, CAFSM = 1932, YSR = 1933, CANVK = 1933, YTH = 1934, CAYTH = 1934, YTO = 1935, CATOR = 1935, YUD = 1936, CAXXZ = 1936, 
        YUM = 1937, USYUM = 1937, YUX = 1938, CAHAB = 1938, YVB = 1939, CAYVB = 1939, YVO = 1940, CAYVO = 1940, YVP = 1941, CAFCM = 1941, YVQ = 1942, CAYVQ = 1942, YVR = 1943, CAVAN = 1943, YWG = 1944, CAWNP = 1944, YWK = 1945, CAYWK = 1945, YXE = 1946, CASAK = 1946, YXJ = 1947, CAFSJ = 1947, 
        YXN = 1948, CAYXN = 1948, YXS = 1949, CAPRG = 1949, YXT = 1950, CAYXT = 1950, YXU = 1951, CALOD = 1951, YXY = 1952, CAYXY = 1952, YYC = 1953, CACAL = 1953, YYD = 1954, CAYYD = 1954, YYJ = 1955, CAVIC = 1955, YYN = 1956, CAYYN = 1956, YYQ = 1957, CACHV = 1957, YYR = 1958, CAGOO = 1958, 
        YYT = 1959, CASJF = 1959, YYZ = 1960, CAMIS = 1960, YZF = 1961, CAYZF = 1961, YZP = 1962, CASSP = 1962, ZAD = 1963, HRZAD = 1963, ZAG = 1964, HRZAG = 1964, ZAZ = 1965, ESZAZ = 1965, ZBO = 1966, AUZBO = 1966, ZCL = 1967, MXZCL = 1967, ZG0 = 1968, CHZLM = 1968, ZIH = 1969, MXZIH = 1969, 
        ZKE = 1970, CAZKE = 1970, ZLO = 1971, MXZLO = 1971, ZND = 1972, NEZND = 1972, ZNE = 1973, AUNWM = 1973, ZQN = 1974, NZZQN = 1974, ZRH = 1975, CHZRH = 1975, ZSA = 1976, BSZSA = 1976, ZSS = 1977, CIZSS = 1977, ZTH = 1978, GRZTH = 1978, ZTM = 1979, CAZTM = 1979, ZVK = 1980, LASAV = 1980, 
        ZYL = 1981, BDZYL = 1981, 
        MAXCITY = 1982, NUMCITY = 1982 
    };
    
    City( void ): m_city(NOCITY) {}
    ~City( void )=default; 
    
    // non-explicit constructors intentional here
    City( CityCode i ): m_city(i) {} // e.g. i = City::LON
    City( const std::string &str ): m_city(NOCITY) { setCity(str); }
    City( const char *str ): m_city(NOCITY) { if (str) setCity(str); } 
    
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
    setCity( const std::string &str ); // e.g. s = "LON" or  s = "GBLON"
    
    bool
    set3City( const std::string &str ); // e.g. s = "LON"
    
    bool
    set5City( const std::string &str ); // e.g. s = "GBLON"
    
    void
    setCity( CityCode c ) { m_city = c; } // e.g. s = City::LON or City::GBLON
    
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



