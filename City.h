/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 
 History: 
 
 Identifies a city (not an airport) using a large part of the IATA airport code list. 
 
 This class can represent 1975 distinct cities - most major cities of the world including capital cities and US state capitals.
 This is enough to describe every city associated with a Market Identification Code (MIC).
 
 Each IATA code corresponds to the city served by that airport. 

 https://www.iata.org/en/publications/directories/code-search/
 https://en.wikipedia.org/wiki/IATA_airport_code
 
 Latitude and longitude of cities (not airports) is provided for geograpical positioning.
 
 For cities that do not have a suitable IATA code we use an invented city3code terminated by a '0' (see table below).
 
 This class supports the 5 digit UN/LOCODE for most cities. For the 4 cities without a UN/LOCODE we have 
 used XXXXX. The city 'No City' has also been assigned the code XXXXX.
 
 > select city, country2code, city3code, locode from city where locode = 'XXXXX';
 +-------------------+--------------+-----------+--------+
 | city              | country2code | city3code | locode |
 +-------------------+--------------+-----------+--------+
 | GIFT City         | IN           | GC0       | XXXXX  |
 | King Edward Point | GS           | GS0       | XXXXX  |
 | Jerusalem         | IL           | JRS       | XXXXX  |
 | Lac Brochet       | CA           | YLB       | XXXXX  |
 | No City           | XX           | XXX       | XXXXX  |
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
 +-------------------+--------------+-----------+

 
 Example
 
 City x;
 x.setCity( "USNYC" );
 std::cout << x << std::endl;
 std::cout << x.name() << std::endl;
 std::cout << x.to3Code() << std::endl;
 std::cout << x.to5Code() << std::endl;
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
 std::cout << y.to5Code() << std::endl;
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
        BRU = 243, BEBRU = 243, BSB = 244, BRBSB = 244, BSG = 245, GWBSG = 245, BSL = 246, CHBSL = 246, BSR = 247, IQBSR = 247, BTM = 248, USBTM = 248, BTQ = 249, RWBTQ = 249, BTR = 250, USBTR = 250, BTS = 251, SKBTS = 251, BTU = 252, MYBTU = 252, BTV = 253, USBTV = 253, 
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
        HFT = 661, NOHFT = 661, HGA = 662, SOHGA = 662, HGH = 663, CNHAZ = 663, HGO = 664, CIHGO = 664, HHH = 665, USHHH = 665, HIB = 666, USIBB = 666, HII = 667, USHII = 667, HIJ = 668, JPHIJ = 668, HIR = 669, SBHIR = 669, HIS = 670, AUHIS = 670, HKD = 671, JPHKD = 671, 
        HKG = 672, HKHKG = 672, HKT = 673, THHKT = 673, HKY = 674, USHKY = 674, HLA = 675, ZAHLA = 675, HLH = 676, CNHLH = 676, HLN = 677, USHLN = 677, HLT = 678, AUHLT = 678, HLZ = 679, NZHLZ = 679, HMO = 680, MXHMO = 680, HNA = 681, JPMRW = 681, HNH = 682, USHNH = 682, 
        HNK = 683, AUHNK = 683, HNL = 684, USHNL = 684, HNS = 685, USHNS = 685, HOG = 686, CUHOG = 686, HOM = 687, USHOM = 687, HON = 688, USHON = 688, HOQ = 689, DEHOQ = 689, HOR = 690, PTHOR = 690, HOU = 691, USHOU = 691, HPN = 692, USWTS = 692, HRB = 693, CNHRN = 693, 
        HRE = 694, ZWHRE = 694, HRG = 695, EGHRG = 695, HRK = 696, UAHRK = 696, HRL = 697, USHRL = 697, HS0 = 698, DKHOR = 698, HSV = 699, USHSV = 699, HTA = 700, RUHTA = 700, HTI = 701, AUHTI = 701, HTS = 702, USHNU = 702, HTV = 703, USHTV = 703, HUF = 704, USHUF = 704, 
        HUH = 705, PFHUH = 705, HUI = 706, VNHUI = 706, HUX = 707, MXHTO = 707, HUY = 708, GBHUY = 708, HVB = 709, AUHVB = 709, HVN = 710, USHVN = 710, HVR = 711, USHVR = 711, HWN = 712, ZWHWN = 712, HYA = 713, USHYA = 713, HYD = 714, INHYD = 714, HYG = 715, USHYG = 715, 
        IAG = 716, USIAG = 716, IBZ = 717, ESIBZ = 717, ICN = 718, KRINC = 718, ICT = 719, USICT = 719, IDA = 720, USIDA = 720, IDR = 721, INIDR = 721, IEV = 722, UAIEV = 722, IFL = 723, AUIFL = 723, IFP = 724, USIFP = 724, IGH = 725, AUIGH = 725, IGR = 726, ARIGR = 726, 
        IJX = 727, USIJX = 727, IKT = 728, RUIKT = 728, ILE = 729, USILE = 729, ILG = 730, USILG = 730, ILI = 731, USILI = 731, ILM = 732, USILM = 732, ILP = 733, NCILP = 733, ILY = 734, GBISY = 734, ILZ = 735, SKILZ = 735, IND = 736, USIND = 736, INI = 737, RSINI = 737, 
        INL = 738, USINL = 738, INN = 739, ATINN = 739, INU = 740, NRYRN = 740, INV = 741, GBINV = 741, IOM = 742, IMDGS = 742, IOU = 743, NCIOU = 743, IPC = 744, CLIPC = 744, IPL = 745, USIDO = 745, IPT = 746, USIPT = 746, IQT = 747, PEIQT = 747, IRG = 748, AUIRG = 748, 
        IRO = 749, CFIRO = 749, ISA = 750, AUISA = 750, ISB = 751, PKISB = 751, ISG = 752, JPISG = 752, ISL = 753, USISN = 753, ISO = 754, USK5T = 754, ISP = 755, USXHO = 755, IST = 756, TRIST = 756, ITH = 757, USITH = 757, ITO = 758, USITO = 758, IUE = 759, NUALO = 759, 
        IVC = 760, NZIVC = 760, IVL = 761, FIIVL = 761, IXB = 762, INIXB = 762, IXC = 763, INIXC = 763, IXG = 764, INIXG = 764, IXJ = 765, INIXJ = 765, IXR = 766, INIXR = 766, IXW = 767, INIXW = 767, IYK = 768, USIYK = 768, IZM = 769, TRADB = 769, JAA = 770, AFJAA = 770, 
        JAC = 771, USJAC = 771, JAD = 772, AUJAD = 772, JAF = 773, LKJAF = 773, JAG = 774, PKJAG = 774, JAI = 775, INJAI = 775, JAK = 776, HTJAK = 776, JAL = 777, MXJAL = 777, JAM = 778, BGJAM = 778, JAN = 779, USJAN = 779, JAU = 780, PEJAU = 780, JBR = 781, USJBR = 781, 
        JCB = 782, BRJCB = 782, JCK = 783, AUJCK = 783, JCM = 784, BRJCA = 784, JDF = 785, BRJDF = 785, JDH = 786, INJDH = 786, JDZ = 787, CNJDZ = 787, JED = 788, SAJED = 788, JEE = 789, HTJEE = 789, JEF = 790, USJEF = 790, JER = 791, JESTH = 791, JGA = 792, INJGA = 792, 
        JGB = 793, INJGB = 793, JGN = 794, CNJGN = 794, JGS = 795, CNJIN = 795, JHB = 796, MYJHB = 796, JHE = 797, SEHEL = 797, JHG = 798, CNJOG = 798, JHM = 799, USJHM = 799, JHQ = 800, AUJHQ = 800, JHW = 801, USJHW = 801, JIB = 802, DJJIB = 802, JIJ = 803, ETJIJ = 803, 
        JIL = 804, CNJIL = 804, JIM = 805, ETJIM = 805, JIN = 806, UGJIN = 806, JIP = 807, ECJIP = 807, JIR = 808, NPJIR = 808, JIU = 809, CNJIU = 809, JIW = 810, PKJIW = 810, JJI = 811, PEJJI = 811, JJN = 812, CNJJI = 812, JKG = 813, SEJKG = 813, JKH = 814, GRJKH = 814, 
        JKR = 815, NPJKR = 815, JKT = 816, IDJKT = 816, JKV = 817, USJKV = 817, JLN = 818, USJLN = 818, JLR = 819, INJUC = 819, JLS = 820, BRJLS = 820, JMB = 821, AOJMB = 821, JMK = 822, GRJMK = 822, JMO = 823, NPJMO = 823, JMS = 824, USJMS = 824, JMU = 825, CNJMU = 825, 
        JNA = 826, BRJNA = 826, JNB = 827, ZAJNB = 827, JNG = 828, CNJNG = 828, JNI = 829, ARJNI = 829, JNU = 830, USJNU = 830, JNX = 831, GRJNX = 831, JNZ = 832, CNJZI = 832, JOE = 833, FIJOE = 833, JOI = 834, BRJOI = 834, JOL = 835, PHJOL = 835, JON = 836, UMJON = 836, 
        JOS = 837, NGJOS = 837, JPA = 838, BRJPA = 838, JQE = 839, PAJQE = 839, JRH = 840, INJRH = 840, JRO = 841, TZJRO = 841, JRS = 842, ILXXX = 842, JSA = 843, INJSA = 843, JSI = 844, GRJSI = 844, JSM = 845, ARJSM = 845, JST = 846, USJST = 846, JTI = 847, BRJTI = 847, 
        JTR = 848, GRJTR = 848, JUB = 849, SSJUB = 849, JUI = 850, DEJUI = 850, JUJ = 851, ARJUJ = 851, JUL = 852, PEJUL = 852, JUN = 853, AUJUN = 853, JUT = 854, HNJUT = 854, JUV = 855, GLJUV = 855, JVL = 856, USJVL = 856, JWA = 857, BWJWA = 857, JXN = 858, USJXN = 858, 
        JYV = 859, FIJYV = 859, KAJ = 860, FIKAJ = 860, KAN = 861, NGKAN = 861, KAO = 862, FIKAO = 862, KBL = 863, AFKBL = 863, KBY = 864, AUKBY = 864, KCC = 865, USKCC = 865, KCE = 866, AUKCE = 866, KCH = 867, MYKCH = 867, KCM = 868, TRKCM = 868, KCZ = 869, JPKCZ = 869, 
        KDD = 870, PKKDD = 870, KDH = 871, AFKDH = 871, KDU = 872, PKKDU = 872, KEL = 873, DEKEL = 873, KEM = 874, FIKEM = 874, KEN = 875, SLKEN = 875, KGC = 876, AUKGC = 876, KGD = 877, RUKGD = 877, KGI = 878, AUKGI = 878, KGL = 879, RWKGL = 879, KGS = 880, GRKGS = 880, 
        KHH = 881, TWKHH = 881, KHI = 882, PKKHI = 882, KHJ = 883, FIKHJ = 883, KHV = 884, RUKHV = 884, KID = 885, SEKID = 885, KIJ = 886, JPKIJ = 886, KIK = 887, IQKIK = 887, KIM = 888, ZAKIM = 888, KIN = 889, JMKIN = 889, KIR = 890, IEKIR = 890, KIV = 891, MDKIV = 891, 
        KIW = 892, ZMKIW = 892, KKN = 893, NOKKN = 893, KLA = 894, UGKLA = 894, KLR = 895, SEKLR = 895, KLU = 896, ATKLU = 896, KLW = 897, USKLW = 897, KLX = 898, GRKLX = 898, KLZ = 899, ZAKLZ = 899, KMI = 900, JPKMI = 900, KMJ = 901, JPKMJ = 901, KMP = 902, NAKMP = 902, 
        KMQ = 903, JPKMQ = 903, KMS = 904, GHKMS = 904, KNS = 905, AUKNS = 905, KNU = 906, INKNU = 906, KNX = 907, AUKNX = 907, KOA = 908, USKHH = 908, KOI = 909, GBKWL = 909, KOJ = 910, JPKOJ = 910, KOK = 911, FIKOK = 911, KRB = 912, AUKRB = 912, KRK = 913, PLKRK = 913, 
        KRN = 914, SEKRN = 914, KRP = 915, DKKRP = 915, KRS = 916, NOKRS = 916, KRT = 917, SDKRT = 917, KSD = 918, SEKSD = 918, KSL = 919, SDKSL = 919, KSS = 920, MLSIK = 920, KSU = 921, NOKSU = 921, KTA = 922, AUKTA = 922, KTB = 923, USKTB = 923, KTM = 924, NPKTM = 924, 
        KTN = 925, USKTN = 925, KTR = 926, AUKTR = 926, KTT = 927, FIKTT = 927, KUA = 928, MYKUA = 928, KUF = 929, RUKUF = 929, KUH = 930, JPKUH = 930, KUL = 931, MYKUL = 931, KUO = 932, FIKUO = 932, KVA = 933, GRKVA = 933, KWI = 934, KWKWI = 934, KWL = 935, CNGUI = 935, 
        KWM = 936, AUKWM = 936, KYA = 937, TRKYA = 937, KYS = 938, TRKSY = 938, KZN = 939, RUKZN = 939, LAD = 940, AOLAD = 940, LAE = 941, PGLAE = 941, LAF = 942, USLAF = 942, LAI = 943, FRLAI = 943, LAN = 944, USLAN = 944, LAP = 945, MXLAP = 945, LAR = 946, USLAR = 946, 
        LAS = 947, USLAS = 947, LAW = 948, USLAW = 948, LAX = 949, USLAX = 949, LBA = 950, GBLBA = 950, LBB = 951, USLBB = 951, LBE = 952, USLBE = 952, LBI = 953, FRBBI = 953, LBQ = 954, GALBQ = 954, LBU = 955, MYLBU = 955, LBV = 956, GALBV = 956, LCA = 957, CYLCA = 957, 
        LCG = 958, ESLCG = 958, LCH = 959, USLCH = 959, LDC = 960, AULDC = 960, LDE = 961, FRLDE = 961, LDK = 962, SELDK = 962, LDY = 963, GBLDY = 963, LEB = 964, USLN7 = 964, LED = 965, RULED = 965, LEI = 966, ESLEI = 966, LEJ = 967, DELEJ = 967, LEK = 968, GNLEK = 968, 
        LEQ = 969, GBLEQ = 969, LER = 970, AULER = 970, LEX = 971, USLEX = 971, LEY = 972, NLLEY = 972, LFI = 973, USYGZ = 973, LFT = 974, USLFT = 974, LFW = 975, TGLFW = 975, LGB = 976, USLGB = 976, LGG = 977, BELGG = 977, LGK = 978, MYLGK = 978, LHE = 979, PKLHE = 979, 
        LI0 = 980, LIVDZ = 980, LIF = 981, NCLIF = 981, LIG = 982, FRLIG = 982, LIH = 983, USLIU = 983, LIL = 984, FRLLE = 984, LIM = 985, PELIM = 985, LIQ = 986, CDLIQ = 986, LIS = 987, PTLIS = 987, LIT = 988, USLIT = 988, LJU = 989, SILJU = 989, LKL = 990, NOLKL = 990, 
        LKO = 991, INLKO = 991, LL0 = 992, PHNOP = 992, LLA = 993, SELLA = 993, LLW = 994, MWLLW = 994, LMM = 995, MXLMM = 995, LMP = 996, ITLMP = 996, LMT = 997, USLMT = 997, LNK = 998, USLNK = 998, LNO = 999, AULNO = 999, LNS = 1000, USLNS = 1000, LNY = 1001, USLNY = 1001, 
        LNZ = 1002, ATLNZ = 1002, LON = 1003, GBLON = 1003, LOS = 1004, NGLOS = 1004, LPA = 1005, ESLPG = 1005, LPB = 1006, BOLPB = 1006, LPL = 1007, GBLIV = 1007, LPP = 1008, FILPP = 1008, LRD = 1009, USLRD = 1009, LRE = 1010, AULRE = 1010, LRF = 1011, USLRF = 1011, LRH = 1012, FRLRH = 1012, 
        LRM = 1013, DOCDC = 1013, LRT = 1014, FRLRT = 1014, LSE = 1015, USLSE = 1015, LSI = 1016, GBLSI = 1016, LST = 1017, AULST = 1017, LSY = 1018, AULSY = 1018, LSZ = 1019, HRVLN = 1019, LTO = 1020, MXLRO = 1020, LU0 = 1021, CHLZN = 1021, LUD = 1022, NALUD = 1022, LUG = 1023, CHLUG = 1023, 
        LUJ = 1024, ZALUJ = 1024, LUM = 1025, CNLUM = 1025, LUN = 1026, ZMLUN = 1026, LUQ = 1027, ARLUQ = 1027, LUX = 1028, LULUX = 1028, LV0 = 1029, BELEU = 1029, LVO = 1030, AULVO = 1030, LWB = 1031, USLWB = 1031, LWK = 1032, GBLER = 1032, LWO = 1033, UALWO = 1033, LWS = 1034, USLWS = 1034, 
        LWT = 1035, USLWT = 1035, LXR = 1036, EGLXR = 1036, LYH = 1037, USLYH = 1037, LYO = 1038, USLYO = 1038, LYP = 1039, PKLYP = 1039, LYR = 1040, SJLYR = 1040, LYS = 1041, FRLIO = 1041, LYX = 1042, GBLYX = 1042, LZC = 1043, MXLZC = 1043, MAA = 1044, INMAA = 1044, MAD = 1045, ESMAD = 1045, 
        MAF = 1046, USMAF = 1046, MAH = 1047, ESMAH = 1047, MAJ = 1048, MHMAJ = 1048, MAM = 1049, MXMAM = 1049, MAN = 1050, GBMNC = 1050, MAO = 1051, BRMAO = 1051, MAR = 1052, VEMAR = 1052, MAU = 1053, PFMAU = 1053, MAZ = 1054, PRMAZ = 1054, MBA = 1055, KEMBA = 1055, MBH = 1056, AUMBH = 1056, 
        MBJ = 1057, JMMBJ = 1057, MBM = 1058, ZAMBM = 1058, MBS = 1059, USSGM = 1059, MBX = 1060, SIMBX = 1060, MC0 = 1061, USMOC = 1061, MCE = 1062, USMCE = 1062, MCI = 1063, USMKC = 1063, MCL = 1064, USMCL = 1064, MCM = 1065, MCMON = 1065, MCN = 1066, USMCN = 1066, MCO = 1067, USORL = 1067, 
        MCP = 1068, BRMCA = 1068, MCT = 1069, OMMCT = 1069, MCW = 1070, USMCW = 1070, MCY = 1071, AUMCY = 1071, MCZ = 1072, BRMCZ = 1072, MDC = 1073, IDMDC = 1073, MDE = 1074, COMDE = 1074, MDL = 1075, MMMDL = 1075, MDQ = 1076, ARMDQ = 1076, MDY = 1077, UMMDY = 1077, MDZ = 1078, ARMDZ = 1078, 
        MED = 1079, SAMED = 1079, MEE = 1080, NCMEE = 1080, MEI = 1081, USMEI = 1081, MEL = 1082, AUMEL = 1082, MEM = 1083, USMEM = 1083, MES = 1084, IDMES = 1084, MEX = 1085, MXMEX = 1085, MEZ = 1086, ZAMEZ = 1086, MFE = 1087, USTXC = 1087, MFF = 1088, GAMFF = 1088, MFG = 1089, PKMFG = 1089, 
        MFM = 1090, MOMFM = 1090, MFN = 1091, NZMFN = 1091, MFO = 1092, PGMFO = 1092, MFQ = 1093, NEMFQ = 1093, MFR = 1094, USMFR = 1094, MFU = 1095, ZMMFU = 1095, MGA = 1096, NIMGA = 1096, MGB = 1097, AUMGB = 1097, MGF = 1098, BRMGF = 1098, MGH = 1099, ZAMGH = 1099, MGM = 1100, USMGM = 1100, 
        MGQ = 1101, SOMGQ = 1101, MGW = 1102, USMGW = 1102, MHE = 1103, USMHE = 1103, MHH = 1104, BSMHH = 1104, MHQ = 1105, AXMHQ = 1105, MHT = 1106, USMHT = 1106, MIA = 1107, USMIA = 1107, MID = 1108, MXMID = 1108, MIK = 1109, FIMIK = 1109, MIL = 1110, ITMIL = 1110, MIM = 1111, AUMIM = 1111, 
        MIR = 1112, TNMIR = 1112, MJC = 1113, CIMJC = 1113, MJD = 1114, PKMJD = 1114, MJL = 1115, GAMJL = 1115, MJN = 1116, MGMJN = 1116, MJQ = 1117, USMJQ = 1117, MJT = 1118, GRMJT = 1118, MJV = 1119, ESMJV = 1119, MKC = 1120, PHMKC = 1120, MKE = 1121, USMKE = 1121, MKG = 1122, USMKG = 1122, 
        MKK = 1123, USKUA = 1123, MKL = 1124, USMKL = 1124, MKR = 1125, AUMKR = 1125, MKY = 1126, AUMKY = 1126, MLA = 1127, MTMLA = 1127, MLB = 1128, USMLB = 1128, MLE = 1129, MVMLE = 1129, MLH = 1130, FRMLH = 1130, MLI = 1131, USMLI = 1131, MLM = 1132, MXMLM = 1132, MLS = 1133, USMLS = 1133, 
        MLU = 1134, USMLU = 1134, MLW = 1135, LRMLW = 1135, MLX = 1136, TRMLX = 1136, MMA = 1137, SEMMA = 1137, MME = 1138, GBMME = 1138, MMG = 1139, AUMMG = 1139, MMJ = 1140, JPMMJ = 1140, MMK = 1141, RUMMK = 1141, MMM = 1142, AUMMM = 1142, MMY = 1143, JPMMY = 1143, MNI = 1144, MSBRD = 1144, 
        MNL = 1145, PHMNL = 1145, MOB = 1146, USMOB = 1146, MOD = 1147, USMOD = 1147, MON = 1148, NZMON = 1148, MOT = 1149, USMOT = 1149, MOV = 1150, AUMOV = 1150, MOW = 1151, RUMOW = 1151, MOZ = 1152, PFMOZ = 1152, MPA = 1153, NAKAT = 1153, MPD = 1154, PKMPD = 1154, MPL = 1155, FRMPL = 1155, 
        MPM = 1156, MZMPM = 1156, MPV = 1157, USMPI = 1157, MQL = 1158, AUMQL = 1158, MQM = 1159, TRMDN = 1159, MQQ = 1160, TDMQQ = 1160, MQT = 1161, USMQT = 1161, MRB = 1162, USMRB = 1162, MRS = 1163, FRMRS = 1163, MRU = 1164, MUPLU = 1164, MRV = 1165, RUMRV = 1165, MRY = 1166, USMY3 = 1166, 
        MRZ = 1167, AUMRZ = 1167, MSE = 1168, GBMSE = 1168, MSL = 1169, USUSH = 1169, MSN = 1170, USMSN = 1170, MSO = 1171, USMSO = 1171, MSP = 1172, USMES = 1172, MSQ = 1173, BYMSQ = 1173, MSR = 1174, TRMSR = 1174, MST = 1175, NLMST = 1175, MSU = 1176, LSMSU = 1176, MSY = 1177, USMSY = 1177, 
        MTH = 1178, USMTH = 1178, MTJ = 1179, USMTJ = 1179, MTL = 1180, AUMTL = 1180, MTM = 1181, USMTM = 1181, MTO = 1182, USMTO = 1182, MTS = 1183, SZMTS = 1183, MTT = 1184, MXMTT = 1184, MTY = 1185, MXMTY = 1185, MUB = 1186, BWMUB = 1186, MUC = 1187, DEMUC = 1187, MUE = 1188, USMUE = 1188, 
        MUH = 1189, EGMUH = 1189, MUX = 1190, PKMUX = 1190, MV0 = 1191, USVYV = 1191, MVB = 1192, GAMVE = 1192, MVD = 1193, UYMVD = 1193, MVR = 1194, CMMVR = 1194, MVY = 1195, USMVY = 1195, MVZ = 1196, ZWMVZ = 1196, MWD = 1197, PKMWD = 1197, MWH = 1198, USMWH = 1198, MXL = 1199, MXMXL = 1199, 
        MYA = 1200, AUMYA = 1200, MYD = 1201, KEMYD = 1201, MYJ = 1202, JPMYJ = 1202, MYQ = 1203, INMYQ = 1203, MYR = 1204, USMYR = 1204, MYY = 1205, MYMYY = 1205, MZG = 1206, TWMZG = 1206, MZM = 1207, FRMZM = 1207, MZR = 1208, AFMZR = 1208, MZT = 1209, MXMZT = 1209, MZY = 1210, ZAMZY = 1210, 
        NAA = 1211, AUNAA = 1211, NAG = 1212, INNAG = 1212, NAJ = 1213, AZNAJ = 1213, NAN = 1214, FJNAN = 1214, NAP = 1215, ITNAP = 1215, NAS = 1216, BSNAS = 1216, NAT = 1217, BRNAT = 1217, NBO = 1218, KENBO = 1218, NCE = 1219, FRNCE = 1219, NCL = 1220, GBNCS = 1220, NCS = 1221, ZANCS = 1221, 
        NCY = 1222, FRNCY = 1222, NDB = 1223, MRNDB = 1223, NDJ = 1224, TDNDJ = 1224, NDU = 1225, NANDU = 1225, NEV = 1226, KNNEV = 1226, NGE = 1227, CMNGE = 1227, NGO = 1228, JPNGO = 1228, NGS = 1229, JPNGS = 1229, NIC = 1230, CYNIC = 1230, NIM = 1231, NENIM = 1231, NKC = 1232, MRNKC = 1232, 
        NKW = 1233, IODGA = 1233, NLA = 1234, ZMNLA = 1234, NLD = 1235, MXNLD = 1235, NLK = 1236, NFNLK = 1236, NLP = 1237, ZANLP = 1237, NLV = 1238, UANLV = 1238, NNG = 1239, CNNIN = 1239, NOC = 1240, IENOC = 1240, NOU = 1241, NCNOU = 1241, NQY = 1242, GBNQY = 1242, NRA = 1243, AUNRA = 1243, 
        NRK = 1244, SENRK = 1244, NS0 = 1245, USNQY = 1245, NSA = 1246, AUNSA = 1246, NSN = 1247, NZNSN = 1247, NSO = 1248, AUNSO = 1248, NST = 1249, THNKT = 1249, NTE = 1250, FRNTE = 1250, NTL = 1251, AUNTL = 1251, NTY = 1252, ZAPIB = 1252, NUE = 1253, DENUE = 1253, NWI = 1254, GBNRW = 1254, 
        NYC = 1255, USNYC = 1255, OAG = 1256, AUOAG = 1256, OAJ = 1257, USOAJ = 1257, OAK = 1258, USOAK = 1258, OAX = 1259, MXOAX = 1259, OB0 = 1260, DEOLO = 1260, ODA = 1261, CFODA = 1261, ODB = 1262, ESODB = 1262, ODE = 1263, DKODE = 1263, ODS = 1264, UAODS = 1264, OGG = 1265, USOGG = 1265, 
        OHD = 1266, MKOHD = 1266, OHT = 1267, PKOHT = 1267, OIT = 1268, JPOIT = 1268, OKA = 1269, JPOKA = 1269, OKC = 1270, USOKC = 1270, OKD = 1271, JPSPK = 1271, OKJ = 1272, JPOKJ = 1272, OKU = 1273, NAOKU = 1273, OLB = 1274, ITOLB = 1274, OLF = 1275, USOLF = 1275, OLM = 1276, USOLM = 1276, 
        OLP = 1277, AUOLP = 1277, OMA = 1278, USOMA = 1278, OMD = 1279, NAOMD = 1279, OME = 1280, USOME = 1280, OMH = 1281, IRUIM = 1281, OMO = 1282, BAOMO = 1282, OND = 1283, NAOND = 1283, ONT = 1284, USONT = 1284, OOL = 1285, AUGOC = 1285, OOM = 1286, AUCOM = 1286, OPO = 1287, PTOPO = 1287, 
        ORB = 1288, SEORB = 1288, ORF = 1289, USORF = 1289, ORH = 1290, USORH = 1290, ORK = 1291, IEORK = 1291, ORN = 1292, DZORN = 1292, OSA = 1293, JPOSA = 1293, OSH = 1294, USOSH = 1294, OSI = 1295, HROSI = 1295, OSL = 1296, NOOSL = 1296, OSM = 1297, IQOSM = 1297, OSR = 1298, CZOSR = 1298, 
        OTH = 1299, USOTH = 1299, OTZ = 1300, USOTZ = 1300, OUA = 1301, BFOUA = 1301, OUD = 1302, MAOUD = 1302, OUH = 1303, ZAOUH = 1303, OUK = 1304, GBOUK = 1304, OUL = 1305, FIOUL = 1305, OUZ = 1306, MROUZ = 1306, OVB = 1307, RUOVB = 1307, OVD = 1308, ESOVO = 1308, OWB = 1309, USOWB = 1309, 
        OXB = 1310, GWOXB = 1310, OXR = 1311, USOXN = 1311, OZH = 1312, UAZPR = 1312, OZZ = 1313, MAOZZ = 1313, PA0 = 1314, BRPWA = 1314, PAC = 1315, PAPAC = 1315, PAD = 1316, DEPAD = 1316, PAH = 1317, USPAH = 1317, PAP = 1318, HTPAP = 1318, PAR = 1319, FRPAR = 1319, PAT = 1320, INPAT = 1320, 
        PB0 = 1321, USPB8 = 1321, PBC = 1322, MXPBC = 1322, PBH = 1323, BTTHI = 1323, PBI = 1324, USPBI = 1324, PBM = 1325, SRPBM = 1325, PBO = 1326, AUPBO = 1326, PBZ = 1327, ZAPBZ = 1327, PC0 = 1328, PHPSG = 1328, PCT = 1329, USPCT = 1329, PDL = 1330, PTPDL = 1330, PDT = 1331, USPDT = 1331, 
        PDX = 1332, USPDX = 1332, PEG = 1333, ITPEG = 1333, PEI = 1334, COPEI = 1334, PEN = 1335, MYPEN = 1335, PER = 1336, AUPER = 1336, PEW = 1337, PKPEW = 1337, PFN = 1338, USPFN = 1338, PFO = 1339, CYPFO = 1339, PGA = 1340, USPGA = 1340, PGF = 1341, FRPGF = 1341, PGV = 1342, USPGV = 1342, 
        PHC = 1343, NGPHC = 1343, PHE = 1344, AUPHE = 1344, PHF = 1345, USNNS = 1345, PHH = 1346, NPPKR = 1346, PHL = 1347, USPHL = 1347, PHW = 1348, ZAPHW = 1348, PHX = 1349, USPHX = 1349, PIA = 1350, USPIA = 1350, PIB = 1351, USLUL = 1351, PIH = 1352, USPIH = 1352, PIR = 1353, USPIR = 1353, 
        PIS = 1354, FRPIS = 1354, PIT = 1355, USPIT = 1355, PJG = 1356, PKPJG = 1356, PKB = 1357, USPKB = 1357, PKW = 1358, BWPKW = 1358, PLB = 1359, USPLB = 1359, PLN = 1360, USPXN = 1360, PLO = 1361, AUPLO = 1361, PLZ = 1362, ZAPLZ = 1362, PMD = 1363, USQPL = 1363, PMI = 1364, ESPMI = 1364, 
        PMO = 1365, ITPMO = 1365, PMR = 1366, NZPMR = 1366, PMV = 1367, VEMGT = 1367, PMW = 1368, BRPMW = 1368, PN0 = 1369, PNPCN = 1369, PNH = 1370, KHPNH = 1370, PNI = 1371, FMPNI = 1371, PNL = 1372, ITPNL = 1372, PNQ = 1373, INPNQ = 1373, PNR = 1374, CGPNR = 1374, PNS = 1375, USPNS = 1375, 
        POA = 1376, BRPOA = 1376, POG = 1377, GAPOG = 1377, POM = 1378, PGPOM = 1378, POP = 1379, DOPOP = 1379, POR = 1380, FIPOR = 1380, POS = 1381, TTPOS = 1381, POU = 1382, USPOU = 1382, POZ = 1383, PLPOZ = 1383, PPG = 1384, ASPPG = 1384, PPP = 1385, AUPPP = 1385, PPT = 1386, PFPPT = 1386, 
        PQI = 1387, USPQI = 1387, PQQ = 1388, AUPQQ = 1388, PRG = 1389, CZPRG = 1389, PRN = 1390, XXPRN = 1390, PRY = 1391, ZAPRY = 1391, PS0 = 1392, PSNAB = 1392, PSA = 1393, ITPSA = 1393, PSC = 1394, USPSC = 1394, PSD = 1395, EGPSD = 1395, PSE = 1396, PRPSE = 1396, PSG = 1397, USPSG = 1397, 
        PSI = 1398, PKPSI = 1398, PSP = 1399, USPSP = 1399, PSR = 1400, ITPSR = 1400, PSY = 1401, FKPSY = 1401, PT0 = 1402, BJPTN = 1402, PTG = 1403, ZAPOL = 1403, PTJ = 1404, AUPTJ = 1404, PUB = 1405, USPUB = 1405, PUF = 1406, FRPUF = 1406, PUG = 1407, AUPUG = 1407, PUJ = 1408, DOPCN = 1408, 
        PUQ = 1409, CLPUQ = 1409, PUS = 1410, KRPUS = 1410, PUW = 1411, USPUW = 1411, PUY = 1412, HRPUY = 1412, PVD = 1413, USPVD = 1413, PVH = 1414, BRPVH = 1414, PVK = 1415, GRPVK = 1415, PVR = 1416, MXPVR = 1416, PWM = 1417, USPWM = 1417, PXM = 1418, MXPXM = 1418, PXO = 1419, PTPXO = 1419, 
        PYB = 1420, INPYB = 1420, PYX = 1421, THPYX = 1421, PZB = 1422, ZAPZB = 1422, PZH = 1423, PKPZH = 1423, PZO = 1424, VEPZO = 1424, QCB = 1425, JPCHB = 1425, QGF = 1426, BRMON = 1426, QIC = 1427, ITBIE = 1427, QLI = 1428, CYLMS = 1428, QML = 1429, PKMPR = 1429, QMN = 1430, SZQMN = 1430, 
        QNB = 1431, INQNB = 1431, QND = 1432, RSNVS = 1432, QPA = 1433, ITPDA = 1433, QSA = 1434, ESSBP = 1434, RA0 = 1435, DKRAN = 1435, RAI = 1436, CVRAI = 1436, RAJ = 1437, INRAJ = 1437, RAK = 1438, MARAK = 1438, RAP = 1439, USRAP = 1439, RAR = 1440, CKRAR = 1440, RAZ = 1441, PKRAZ = 1441, 
        RB0 = 1442, DEREG = 1442, RBA = 1443, MARBA = 1443, RBR = 1444, BRRBR = 1444, RCB = 1445, ZARCB = 1445, RDD = 1446, USRDD = 1446, RDG = 1447, USRDG = 1447, RDM = 1448, USRMD = 1448, RDU = 1449, USRAG = 1449, RDZ = 1450, FRRDZ = 1450, REC = 1451, BRREC = 1451, REG = 1452, ITREG = 1452, 
        REK = 1453, ISREY = 1453, REU = 1454, ESRUS = 1454, RFD = 1455, USRFD = 1455, RFP = 1456, PFRFP = 1456, RGI = 1457, PFRGI = 1457, RGL = 1458, ARRGL = 1458, RGN = 1459, MMRGN = 1459, RHG = 1460, RWRHG = 1460, RHI = 1461, USRHI = 1461, RHO = 1462, GRRHO = 1462, RIC = 1463, USRIC = 1463, 
        RIO = 1464, BRRIO = 1464, RIX = 1465, LVRIX = 1465, RIZ = 1466, CNRZH = 1466, RJK = 1467, HRRJK = 1467, RKD = 1468, USRKD = 1468, RKS = 1469, USRKS = 1469, RKT = 1470, AERKT = 1470, RLT = 1471, NERLT = 1471, RMF = 1472, EGRMF = 1472, RMI = 1473, ITRMI = 1473, RNB = 1474, SERNB = 1474, 
        RNE = 1475, FRRNE = 1475, RNN = 1476, DKRNN = 1476, RNO = 1477, USRNO = 1477, RNS = 1478, FRRNS = 1478, ROA = 1479, USROA = 1479, ROC = 1480, USROC = 1480, ROK = 1481, AUROK = 1481, ROM = 1482, ITROM = 1482, ROR = 1483, PWROR = 1483, ROS = 1484, ARROS = 1484, ROT = 1485, NZROT = 1485, 
        ROU = 1486, BGRDU = 1486, ROV = 1487, RUROV = 1487, RR0 = 1488, CARRV = 1488, RRG = 1489, MURRG = 1489, RS0 = 1490, GEZAK = 1490, RSA = 1491, ARRSA = 1491, RSD = 1492, BSRSD = 1492, RST = 1493, USRST = 1493, RTB = 1494, HNRTB = 1494, RTM = 1495, NLRTM = 1495, RUH = 1496, SARUH = 1496, 
        RUN = 1497, RERUN = 1497, RVN = 1498, FIRVN = 1498, RWI = 1499, USRWI = 1499, RWP = 1500, PKRWP = 1500, RYK = 1501, PKRYK = 1501, SAF = 1502, USSAF = 1502, SAH = 1503, YESAH = 1503, SAI = 1504, KHREP = 1504, SAL = 1505, SVSAL = 1505, SAN = 1506, USSAN = 1506, SAO = 1507, BRSAO = 1507, 
        SAP = 1508, HNSAP = 1508, SAT = 1509, USSAT = 1509, SAV = 1510, USSAV = 1510, SBA = 1511, USSBA = 1511, SBH = 1512, BLSBH = 1512, SBK = 1513, FRSBK = 1513, SBN = 1514, USSBN = 1514, SBP = 1515, USCSL = 1515, SBU = 1516, ZASBU = 1516, SBW = 1517, MYSBW = 1517, SBY = 1518, USSBY = 1518, 
        SBZ = 1519, ROSBZ = 1519, SCC = 1520, USSCC = 1520, SCE = 1521, USSCE = 1521, SCF = 1522, USSTZ = 1522, SCK = 1523, USSCK = 1523, SCN = 1524, DESCN = 1524, SCQ = 1525, ESSCQ = 1525, SCU = 1526, CUSCU = 1526, SDF = 1527, USLUI = 1527, SDH = 1528, HNSDH = 1528, SDJ = 1529, JPSDJ = 1529, 
        SDL = 1530, SESDL = 1530, SDQ = 1531, DOSDQ = 1531, SDR = 1532, ESSDR = 1532, SDS = 1533, JPSOS = 1533, SDT = 1534, PKSDT = 1534, SDY = 1535, USSDY = 1535, SEA = 1536, USSEA = 1536, SEB = 1537, LYSEB = 1537, SEL = 1538, KRSEL = 1538, SEN = 1539, GBSND = 1539, SEW = 1540, EGSEW = 1540, 
        SEZ = 1541, SCVIC = 1541, SFA = 1542, TNSFA = 1542, SFG = 1543, MFMAR = 1543, SFJ = 1544, GLSFJ = 1544, SFN = 1545, ARSFN = 1545, SFO = 1546, USSFO = 1546, SGD = 1547, DKSGD = 1547, SGF = 1548, USSGF = 1548, SGN = 1549, VNSGN = 1549, SGU = 1550, USSGU = 1550, SGY = 1551, USSGY = 1551, 
        SH0 = 1552, NLHTB = 1552, SHA = 1553, CNSGH = 1553, SHD = 1554, USSZH = 1554, SHE = 1555, CNSHY = 1555, SHJ = 1556, AESHJ = 1556, SHR = 1557, USSHR = 1557, SHS = 1558, JPSHS = 1558, SHV = 1559, USSHV = 1559, SI0 = 1560, DKSLB = 1560, SID = 1561, CVSID = 1561, SIN = 1562, SGSIN = 1562, 
        SIP = 1563, UASIP = 1563, SIS = 1564, ZASIS = 1564, SIT = 1565, USSIT = 1565, SIX = 1566, AUSIX = 1566, SJC = 1567, USSJC = 1567, SJD = 1568, MXLCB = 1568, SJJ = 1569, BASJJ = 1569, SJO = 1570, CRSJO = 1570, SJT = 1571, USSJT = 1571, SJU = 1572, PRSJU = 1572, SJY = 1573, FISJY = 1573, 
        SK0 = 1574, GMSKE = 1574, SKB = 1575, KNSKB = 1575, SKD = 1576, UZSKD = 1576, SKG = 1577, GRSKG = 1577, SKP = 1578, MKSKP = 1578, SKS = 1579, DKSKR = 1579, SKV = 1580, EGSKV = 1580, SKX = 1581, RUSKX = 1581, SKZ = 1582, PKSKZ = 1582, SLA = 1583, ARSLA = 1583, SLC = 1584, USSLC = 1584, 
        SLE = 1585, USSLE = 1585, SLL = 1586, OMSLL = 1586, SLP = 1587, MXSLP = 1587, SLS = 1588, BGSLS = 1588, SLU = 1589, LCCAS = 1589, SLZ = 1590, BRSLZ = 1590, SM0 = 1591, SMSAI = 1591, SMA = 1592, PTSMA = 1592, SMF = 1593, USSAC = 1593, SMI = 1594, GRSMI = 1594, SMX = 1595, USSMX = 1595, 
        SNA = 1596, USSNA = 1596, SNC = 1597, ECSNC = 1597, SNN = 1598, IELMK = 1598, SNS = 1599, USSNS = 1599, SOF = 1600, BGSOF = 1600, SOG = 1601, NOSOG = 1601, SON = 1602, VUSAN = 1602, SOT = 1603, FISOT = 1603, SOU = 1604, GBSOU = 1604, SPC = 1605, ESSPC = 1605, SPI = 1606, USSPI = 1606, 
        SPN = 1607, MPSPN = 1607, SPS = 1608, USSPS = 1608, SPU = 1609, HRSPU = 1609, SPY = 1610, CISPY = 1610, SRA = 1611, BRSRA = 1611, SRB = 1612, BOSRB = 1612, SRL = 1613, MXSRL = 1613, SRQ = 1614, USZOR = 1614, SRZ = 1615, BOSCS = 1615, SSA = 1616, BRSSA = 1616, SSG = 1617, GQSSG = 1617, 
        SSH = 1618, EGSSH = 1618, SSL = 1619, COSSL = 1619, ST0 = 1620, USXLX = 1620, STI = 1621, CLSCL = 1621, STL = 1622, USSTL = 1622, STO = 1623, SESTO = 1623, STP = 1624, USSTP = 1624, STR = 1625, DESTR = 1625, STS = 1626, USSTS = 1626, STT = 1627, VISTT = 1627, STV = 1628, INSTV = 1628, 
        STX = 1629, VISTX = 1629, SUB = 1630, IDSUB = 1630, SUF = 1631, ITSUF = 1631, SUL = 1632, PKSUL = 1632, SUN = 1633, US5SV = 1633, SUV = 1634, FJSUV = 1634, SUX = 1635, USSUX = 1635, SVD = 1636, VCKTN = 1636, SVG = 1637, NOSVG = 1637, SVL = 1638, FISVL = 1638, SVQ = 1639, ESSVQ = 1639, 
        SVX = 1640, RUYEK = 1640, SW0 = 1641, DESWR = 1641, SWF = 1642, USSWF = 1642, SWP = 1643, NASWP = 1643, SWS = 1644, GBSWA = 1644, SXB = 1645, FRSXB = 1645, SXL = 1646, IESLI = 1646, SXM = 1647, SXPHI = 1647, SXR = 1648, INSXR = 1648, SYD = 1649, AUSYD = 1649, SYR = 1650, USSYR = 1650, 
        SYY = 1651, GBSTO = 1651, SZD = 1652, GBSHE = 1652, SZF = 1653, TRSSX = 1653, SZG = 1654, ATSZG = 1654, SZK = 1655, ZASZK = 1655, SZX = 1656, CNSNZ = 1656, SZZ = 1657, PLSZZ = 1657, TAB = 1658, TTSCR = 1658, TAK = 1659, JPTAK = 1659, TAM = 1660, MXTAM = 1660, TAO = 1661, CNQIN = 1661, 
        TAS = 1662, UZTAS = 1662, TAY = 1663, TWTPE = 1663, TBO = 1664, SNTOU = 1664, TBS = 1665, GETBS = 1665, TBU = 1666, TOTBU = 1666, TC0 = 1667, USTEL = 1667, TCA = 1668, AUTCA = 1668, TCB = 1669, BSTCB = 1669, TCI = 1670, ESTCI = 1670, TCL = 1671, USTCL = 1671, TCU = 1672, ZATCU = 1672, 
        TED = 1673, DKTED = 1673, TEM = 1674, AUTEM = 1674, TEQ = 1675, TRTEK = 1675, TER = 1676, PTTER = 1676, TEU = 1677, NZTEU = 1677, TEX = 1678, USQTR = 1678, TF0 = 1679, TFPFR = 1679, TGD = 1680, METGD = 1680, TGU = 1681, HNTGU = 1681, TGV = 1682, BGTGV = 1682, TGZ = 1683, MXTGZ = 1683, 
        THE = 1684, BRTHE = 1684, THR = 1685, IRTHR = 1685, THU = 1686, GLTHU = 1686, TIA = 1687, ALTIA = 1687, TIF = 1688, SATIF = 1688, TIJ = 1689, MXTIJ = 1689, TIP = 1690, LYTIP = 1690, TIS = 1691, AUTIS = 1691, TIV = 1692, METIV = 1692, TK0 = 1693, TKNKU = 1693, TKA = 1694, USTKA = 1694, 
        TKS = 1695, JPTKS = 1695, TKU = 1696, FITKU = 1696, TLH = 1697, USTLH = 1697, TLL = 1698, EETLL = 1698, TLS = 1699, FRTLS = 1699, TLV = 1700, ILTLV = 1700, TML = 1701, GHTML = 1701, TMP = 1702, FITMP = 1702, TMS = 1703, STTMS = 1703, TMW = 1704, AUTMW = 1704, TMZ = 1705, UZTER = 1705, 
        TNA = 1706, CNJNA = 1706, TNG = 1707, MATNG = 1707, TNR = 1708, MGTNR = 1708, TOD = 1709, MYTOD = 1709, TOL = 1710, USTOL = 1710, TOS = 1711, NOTOS = 1711, TOU = 1712, NCTOU = 1712, TOV = 1713, VGTOV = 1713, TOY = 1714, JPTOY = 1714, TPA = 1715, USTPA = 1715, TPR = 1716, AUTPR = 1716, 
        TPS = 1717, ITTPS = 1717, TRD = 1718, NOTRD = 1718, TRI = 1719, USOUI = 1719, TRN = 1720, ITTRN = 1720, TRO = 1721, AUTRO = 1721, TRS = 1722, ITTRS = 1722, TRV = 1723, INTRV = 1723, TRW = 1724, KITRW = 1724, TRZ = 1725, INTRI = 1725, TSB = 1726, NATSB = 1726, TSE = 1727, KZAST = 1727, 
        TSF = 1728, ITTRV = 1728, TSN = 1729, CNTNJ = 1729, TSV = 1730, AUTSV = 1730, TTN = 1731, USTTN = 1731, TUC = 1732, ARTUC = 1732, TUK = 1733, PKTUK = 1733, TUL = 1734, USTUL = 1734, TUN = 1735, TNTUN = 1735, TUP = 1736, USTUP = 1736, TUS = 1737, USTUZ = 1737, TUU = 1738, SATUU = 1738, 
        TVC = 1739, USTVC = 1739, TVF = 1740, USTVF = 1740, TVL = 1741, USTVL = 1741, TWB = 1742, AUTWB = 1742, TWF = 1743, USTWF = 1743, TWU = 1744, MYTWU = 1744, TXK = 1745, USTXK = 1745, TYN = 1746, CNTYU = 1746, TYO = 1747, JPTYO = 1747, TYR = 1748, USTYR = 1748, TYS = 1749, USTYS = 1749, 
        TZX = 1750, TRTZX = 1750, UAH = 1751, PFUAH = 1751, UAK = 1752, GLUAK = 1752, UAP = 1753, PFUAP = 1753, UBA = 1754, BRUBB = 1754, UBJ = 1755, JPUBJ = 1755, UBP = 1756, THUBP = 1756, UCA = 1757, USUCA = 1757, UCT = 1758, RUUCT = 1758, UDE = 1759, NLUDE = 1759, UDI = 1760, BRUDI = 1760, 
        UDJ = 1761, UAUZH = 1761, UDR = 1762, INUDR = 1762, UEE = 1763, AUUEE = 1763, UET = 1764, PKUET = 1764, UFA = 1765, RUUFA = 1765, UGC = 1766, UZUGC = 1766, UGO = 1767, AOUGO = 1767, UHE = 1768, CZUHE = 1768, UII = 1769, HNUII = 1769, UIN = 1770, USUIN = 1770, UIO = 1771, ECUIO = 1771, 
        UIP = 1772, FRUIP = 1772, UIT = 1773, MHUIT = 1773, UKB = 1774, JPUKB = 1774, UKI = 1775, USUKI = 1775, UKY = 1776, JPUKY = 1776, ULB = 1777, VUULB = 1777, ULD = 1778, ZAULD = 1778, ULN = 1779, MNULN = 1779, ULU = 1780, UGULU = 1780, ULY = 1781, RUULY = 1781, UMD = 1782, GLUMD = 1782, 
        UME = 1783, SEUME = 1783, UMR = 1784, AUUMR = 1784, UNI = 1785, VCUNI = 1785, UNK = 1786, USUNK = 1786, UNT = 1787, GBUNT = 1787, UPG = 1788, IDUPG = 1788, UPL = 1789, CRUPA = 1789, UPN = 1790, MX2NB = 1790, UPP = 1791, USUPP = 1791, URC = 1792, CNURM = 1792, URG = 1793, BRURG = 1793, 
        URM = 1794, VEURM = 1794, USH = 1795, ARUSH = 1795, USN = 1796, KRUSN = 1796, UTC = 1797, NLUTC = 1797, UTH = 1798, THUTH = 1798, UTN = 1799, ZAUTN = 1799, UTP = 1800, THUTP = 1800, UTT = 1801, ZAUTT = 1801, UUD = 1802, RUUUD = 1802, UVE = 1803, GAOYE = 1803, UVF = 1804, LCVIF = 1804, 
        UVL = 1805, EGUVL = 1805, VA0 = 1806, VAVAT = 1806, VAA = 1807, FIVAA = 1807, VAN = 1808, TRVAN = 1808, VAP = 1809, CLVAP = 1809, VAR = 1810, BGVAR = 1810, VAS = 1811, TRVAS = 1811, VBS = 1812, ITBRC = 1812, VBY = 1813, SEVBY = 1813, VCE = 1814, ITVCE = 1814, VDE = 1815, ESVDH = 1815, 
        VDZ = 1816, USVDZ = 1816, VEL = 1817, USVEL = 1817, VER = 1818, MXVER = 1818, VFA = 1819, ZWVFA = 1819, VGO = 1820, ESVGO = 1820, VID = 1821, BGVID = 1821, VIE = 1822, ATVIE = 1822, VIJ = 1823, VGVIJ = 1823, VIS = 1824, USVIS = 1824, VIT = 1825, ESVIT = 1825, VIX = 1826, BRVIX = 1826, 
        VLC = 1827, ESVLC = 1827, VLD = 1828, USVLD = 1828, VLI = 1829, VUVLI = 1829, VLL = 1830, ESVLL = 1830, VLN = 1831, VEVLN = 1831, VLU = 1832, RUVLU = 1832, VN0 = 1833, USVIV = 1833, VNO = 1834, LTVIL = 1834, VNS = 1835, INVNS = 1835, VOG = 1836, RUVOG = 1836, VPS = 1837, USFWB = 1837, 
        VRA = 1838, CUVRA = 1838, VRB = 1839, USVRB = 1839, VRK = 1840, FIVRK = 1840, VRN = 1841, ITVRN = 1841, VSA = 1842, MXVSA = 1842, VST = 1843, SEVST = 1843, VTE = 1844, LAVTE = 1844, VVO = 1845, RUVVO = 1845, VXO = 1846, SEVXO = 1846, VY0 = 1847, USVYI = 1847, VYD = 1848, ZAVYD = 1848, 
        VZ0 = 1849, HRVZN = 1849, WAM = 1850, AUWAM = 1850, WAW = 1851, PLWAW = 1851, WDH = 1852, NAWDH = 1852, WEI = 1853, AUWEI = 1853, WEL = 1854, ZAWEL = 1854, WGA = 1855, AUWGA = 1855, WHK = 1856, NZWHK = 1856, WIC = 1857, GBWCK = 1857, WIE = 1858, DEWIB = 1858, WLB = 1859, USWLB = 1859, 
        WLG = 1860, NZWLG = 1860, WLS = 1861, WFMAU = 1861, WMB = 1862, AUWMB = 1862, WN0 = 1863, USWLZ = 1863, WNS = 1864, PKWNS = 1864, WOL = 1865, AUWOL = 1865, WP0 = 1866, USWPK = 1866, WRE = 1867, NZWRE = 1867, WRG = 1868, USWRG = 1868, WRL = 1869, USWRL = 1869, WRO = 1870, PLWRO = 1870, 
        WSY = 1871, AUWSI = 1871, WUH = 1872, CNNHN = 1872, WUN = 1873, AUWUN = 1873, WUX = 1874, CNWXS = 1874, WVB = 1875, ZAWVB = 1875, WYA = 1876, AUWYA = 1876, WYN = 1877, AUWYN = 1877, WYS = 1878, USWYS = 1878, XCH = 1879, CXFFC = 1879, XIY = 1880, CNXYA = 1880, XMH = 1881, PFXMH = 1881, 
        XMN = 1882, CNXAM = 1882, XPK = 1883, CAXPK = 1883, XRY = 1884, ESJRZ = 1884, XSI = 1885, CAXSI = 1885, XXX = 1886, XXXXX = 1886, YAK = 1887, USYAK = 1887, YAO = 1888, CMYAO = 1888, YAT = 1889, CAYAT = 1889, YBE = 1890, CAYBE = 1890, YCB = 1891, CAYCB = 1891, YDF = 1892, CAYDF = 1892, 
        YEA = 1893, CAEDM = 1893, YEV = 1894, CAYEV = 1894, YFA = 1895, CAFAL = 1895, YFB = 1896, CAIQL = 1896, YFC = 1897, CAFRE = 1897, YFO = 1898, CAYFO = 1898, YGW = 1899, CAYGW = 1899, YGX = 1900, CAYGX = 1900, YHR = 1901, CAYHR = 1901, YHZ = 1902, CAHAL = 1902, YIF = 1903, CASAU = 1903, 
        YIH = 1904, CNYIC = 1904, YKA = 1905, CAKML = 1905, YKM = 1906, USYKM = 1906, YKS = 1907, RUYKS = 1907, YLB = 1908, CAXXX = 1908, YLR = 1909, CALRA = 1909, YLW = 1910, CAKWL = 1910, YMM = 1911, CAYMM = 1911, YMQ = 1912, CAMTR = 1912, YNB = 1913, SAYNB = 1913, YOK = 1914, JPYOK = 1914, 
        YOP = 1915, CARBL = 1915, YOW = 1916, CAOTT = 1916, YPN = 1917, CAPME = 1917, YPR = 1918, CAPRR = 1918, YQB = 1919, CAQUE = 1919, YQD = 1920, CAYQD = 1920, YQG = 1921, CAWND = 1921, YQM = 1922, CAMNT = 1922, YQR = 1923, CAREG = 1923, YQT = 1924, CATHU = 1924, YQX = 1925, CAGAN = 1925, 
        YRB = 1926, CAREB = 1926, YSJ = 1927, CASJB = 1927, YSM = 1928, CAFSM = 1928, YSR = 1929, CANVK = 1929, YTH = 1930, CAYTH = 1930, YTO = 1931, CATOR = 1931, YUD = 1932, CAYUD = 1932, YUM = 1933, USYUM = 1933, YUX = 1934, CAHAB = 1934, YVB = 1935, CAYVB = 1935, YVO = 1936, CAYVO = 1936, 
        YVP = 1937, CAFCM = 1937, YVQ = 1938, CAYVQ = 1938, YVR = 1939, CAVAN = 1939, YWG = 1940, CAWNP = 1940, YWK = 1941, CAYWK = 1941, YXE = 1942, CASAK = 1942, YXJ = 1943, CAFSJ = 1943, YXN = 1944, CAYXN = 1944, YXS = 1945, CAPRG = 1945, YXT = 1946, CAYXT = 1946, YXU = 1947, CALOD = 1947, 
        YXY = 1948, CAYXY = 1948, YYC = 1949, CACAL = 1949, YYD = 1950, CAYYD = 1950, YYJ = 1951, CAVIC = 1951, YYN = 1952, CAYYN = 1952, YYQ = 1953, CACHV = 1953, YYR = 1954, CAGOO = 1954, YYT = 1955, CASJF = 1955, YYZ = 1956, CAMIS = 1956, YZF = 1957, CAYZF = 1957, YZP = 1958, CASSP = 1958, 
        ZAD = 1959, HRZAD = 1959, ZAG = 1960, HRZAG = 1960, ZAZ = 1961, ESZAZ = 1961, ZBO = 1962, AUZBO = 1962, ZCL = 1963, MXZCL = 1963, ZIH = 1964, MXZIH = 1964, ZKE = 1965, CAZKE = 1965, ZLO = 1966, MXZLO = 1966, ZND = 1967, NEZND = 1967, ZNE = 1968, AUNWM = 1968, ZQN = 1969, NZZQN = 1969, 
        ZRH = 1970, CHZRH = 1970, ZSA = 1971, BSZSA = 1971, ZSS = 1972, CIZSS = 1972, ZTH = 1973, GRZTH = 1973, ZTM = 1974, CAZTM = 1974, ZVK = 1975, LASAV = 1975, ZYL = 1976, BDZYL = 1976, 
        MAXCITY = 1977, NUMCITY = 1977 
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
    to5Code( void ) const { return m_codes5Print[m_fromISO[m_city]]; } 
    
    std::string 
    name( void ) const { return m_fullNames[m_fromISO[m_city]]; } // i.e "London" 

    // the 3 letter subdivison code for this location e.g. LDN
    std::string
    subdiv( void ) const { return (m_subdiv[m_city]) ? m_subdiv[m_city] : "XXX"; }

    //  IANA time zone for this city  i.e "Europe/London"
    std::string 
    timezone( void ) const { return m_timezones[m_fromISO[m_city]]; } 
    
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
    static const float         m_position[NUMCITY][2];    
    static const unsigned char m_capital[NUMCITY];
    static const char * const  m_codes3[NUMCITY];
    static const char * const  m_codes5[NUMCITY];
    static const char * const  m_codes5Print[NUMCITY];
    static const char * const  m_fullNames[NUMCITY];
    static const char * const  m_timezones[NUMCITY];
    static const char * const  m_subdiv[NUMCITY];
};


std::ostream&
operator<<( std::ostream &ostr, const City &c );

std::istream&
operator>>( std::istream &istr, City &c );


#endif



