/* City 09/06/2011
 
 $$$$$$$$$$$$$$$$$$$$$$$
 $   City.h - header   $
 $$$$$$$$$$$$$$$$$$$$$$$
 
 by W.B. Yates    
 Copyright (c) W.B. Yates. All rights reserved. 
 History: Identifies a city (not an airport) using a large part of the IATA airport code list. 
 This class can represent 2051 distinct cities including the capital cities of the world and all US state capitals.
 This is enough to describe (almost) every city associated with a Market Identification Code (MIC).
 
 Each IATA code corresponds to the city served by that airport. For the special cases where no airport
 serves a city we have invented a code terminated in a '0' (see table below).
 When a city with no airport is a capital city the city3code is the country 2 code followed by a 0.
 Note that distinct IATA codes can map to a single city i.e 'LHR', 'LGW', 'LTN', 'STN', 'LCY' all map to 'LON' - London
 while 'JFK', 'LGA' all map to 'NYC' - New York
 
 Latitude and longtitude of cities (not airports) is provided for geograpical positioning and the calculation of distances between cities.
 Geodetic distances (in metres) between points specified by latitude/longitude are calculated using the Vincenty formula.
 
 https://www.iata.org/en/publications/directories/code-search/
 https://en.wikipedia.org/wiki/IATA_airport_code
 
 **** Updated 16/01/2024 ****
 
 Note for 'Kansas City' which could be either 'Kansas City (KS)' or 'Kansas City (MO)' we have choosen '(MO)'
 
 select city, city3code, country3code from cities where city3code like '%0' order by city;
 +-------------------+-----------+--------------+
 | city              | city3code | country3code |
 +-------------------+-----------+--------------+
 | Aabenraa          | AB0       | DNK          |
 | Adamstown         | PN0       | PCN          |
 | Aylesbury         | AY0       | GBR          |
 | Ebene             | EB0       | MUS          |
 | Ecatepec          | EP0       | MEX          |
 | Esch-sur-Alzette  | EA0       | LUX          |
 | Espoo             | ES0       | FIN          |
 | Foster City       | FC0       | USA          |
 | Gandhinagar       | GH0       | IND          |
 | GIFT City         | GC0       | IND          |
 | Greenwich (CT)    | GW0       | USA          |
 | Grindsted         | GR0       | DNK          |
 | Horsens           | HS0       | DNK          |
 | King Edward Point | GS0       | SGS          |
 | Leuven            | LV0       | BEL          |
 | Lucerne           | LU0       | CHE          |
 | Makati City       | MC0       | PHL          |
 | Mill Valley (CA)  | MV0       | USA          |
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
 | Stamford (CT)     | ST0       | USA          |
 | Vaduz             | LI0       | LIE          |
 | Varazdin          | VZ0       | HRV          |
 | Vatican City      | VA0       | VAT          |
 | Vienna (VA)       | VN0       | USA          |
 | Winter Park (FL)  | WP0       | USA          |
 +-------------------+-----------+--------------+

 
 Example
 
 City x;
 x.setCity( "NYC" );
 std::cout << x << std::endl;
 std::cout << x.name() << std::endl;
 std::cout << short(x) << std::endl;
 std::cout << x.capital() << std::endl;
 std::cout << "latitude = " << x.lat() << std::endl;
 std::cout << "longitude = " << x.lon() << std::endl;
 
 City y;
 y.setCity( "LON" );
 std::cout << y << std::endl;
 std::cout << y.name() << std::endl;
 std::cout << short(y) << std::endl;
 std::cout << y.capital() << std::endl;
 std::cout << "latitude = " << y.lat() << std::endl;
 std::cout << "longitude = " << y.lon() << std::endl;
 
 for (int i = 0; i < City::NUMCITY; i++)
 {	
    std::cout << City::index(i) + " " + City::index(i).name() << std::endl;
 }
 
 std::cout << "The distance between " << x.name() << " and " << y.name() << " is " << City::dist(x,y) / 1000.0 << " km" << std::endl;
 
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
    // Note XXX, NOCITY, MAXCITY, NUMCITY and any city codes that terminates in a '0' are not IATA codes.
    enum CityCode : short {
        NOCITY = 0,
        AAC = 1000, AAE = 1001, AAL = 1056, AAN = 1002, AAR = 1003, AAT = 1004, AB0 = 1005, ABD = 1006, ABE = 1007, ABI = 1008, 
        ABJ = 1009, ABM = 1010, ABQ = 1011, ABR = 1012, ABS = 1013, ABV = 1014, ABX = 1015, ABY = 1016, ABZ = 1017, ACA = 1018, 
        ACC = 1019, ACE = 1020, ACH = 1021, ACI = 1022, ACK = 1023, ACT = 1024, ACV = 1025, ACY = 1026, ADA = 1027, ADB = 1749, 
        ADD = 1028, ADE = 1029, ADF = 1030, ADJ = 1071, ADL = 1031, ADQ = 1032, ADY = 1033, ADZ = 1034, AEH = 1035, AEP = 1246, 
        AER = 1036, AES = 1037, AGA = 1038, AGB = 1039, AGP = 1040, AGS = 1041, AGU = 1042, AGZ = 1043, AHB = 1044, AHN = 1045, 
        AHO = 1046, AHU = 1047, AJA = 1048, AJF = 1049, AJN = 1050, AJU = 1051, AJY = 1052, AKL = 1053, AKN = 1054, AKT = 1055, 
        ALA = 1057, ALB = 1058, ALC = 1059, ALF = 1060, ALG = 1061, ALH = 1062, ALJ = 1063, ALO = 1064, ALP = 1065, ALV = 1066, 
        ALW = 1067, ALY = 1068, AMA = 1069, AMD = 1070, AMM = 1071, AMS = 1072, ANB = 1073, ANC = 1074, ANK = 1075, ANP = 1076, 
        ANR = 1077, ANU = 1078, AOI = 1079, AOJ = 1080, AOK = 1081, AOO = 1082, APF = 1083, APW = 1084, AQJ = 1085, ARB = 1086, 
        ARH = 1087, ARK = 1088, ARN = 2590, ASB = 1089, ASE = 1090, ASI = 1091, ASJ = 1092, ASK = 1093, ASM = 1094, ASP = 1095, 
        ASR = 1096, ASU = 1097, ASW = 1098, ATH = 1099, ATL = 1100, ATO = 1101, ATQ = 1102, ATW = 1103, ATY = 1104, ATZ = 1105, 
        AUA = 1106, AUE = 1107, AUG = 1108, AUH = 1109, AUR = 1110, AUS = 1111, AVL = 1112, AVP = 1113, AXA = 1114, AXS = 1115, 
        AXT = 1116, AY0 = 1117, AYQ = 1118, AYR = 1119, AYT = 1120, AYU = 1121, AYW = 1122, AZB = 1123, AZO = 1124, AZP = 2063, 
        AZS = 1125, BAH = 1126, BAK = 1127, BAQ = 1128, BBI = 1129, BBR = 1130, BBT = 1131, BBU = 1249, BBY = 1132, BCN = 1133, 
        BCO = 1134, BCT = 1135, BCV = 1136, BDA = 1137, BDB = 1138, BDL = 1139, BDO = 1140, BDQ = 1141, BDR = 1142, BDS = 1143, 
        BDU = 1144, BEB = 1145, BEG = 1146, BEH = 1147, BEL = 1148, BEN = 1149, BEO = 2226, BER = 1150, BES = 1151, BET = 1152, 
        BEW = 1153, BEY = 1154, BFD = 1155, BFL = 1156, BFN = 1157, BFO = 1158, BFS = 1159, BGA = 1160, BGF = 1161, BGI = 1162, 
        BGM = 1163, BGO = 1164, BGR = 1165, BGU = 1166, BGW = 1167, BGY = 1168, BHC = 1169, BHD = 1159, BHE = 1170, BHM = 1171, 
        BHO = 1172, BHQ = 1173, BHV = 1174, BHX = 1175, BIA = 1176, BIL = 1177, BIO = 1178, BIQ = 1179, BIS = 1180, BIV = 1181, 
        BJI = 1182, BJL = 1183, BJM = 1184, BJS = 1185, BJV = 1186, BJX = 1187, BJZ = 1188, BKI = 1189, BKK = 1190, BKL = 1322, 
        BKO = 1191, BKW = 1192, BKX = 1193, BLA = 1194, BLF = 1195, BLI = 1196, BLK = 1197, BLL = 1198, BLQ = 1199, BLR = 1200, 
        BLT = 1201, BLZ = 1202, BMA = 2590, BME = 1203, BMG = 1204, BMI = 1205, BMP = 1206, BNA = 1207, BNE = 1208, BNJ = 1209, 
        BNK = 1210, BNN = 1211, BNP = 1212, BNX = 1213, BOB = 1214, BOD = 1215, BOG = 1216, BOH = 1217, BOI = 1218, BOJ = 1219, 
        BOM = 1220, BON = 1221, BOO = 1222, BOS = 1223, BOY = 1224, BPT = 1225, BQK = 1226, BQN = 1227, BRC = 1228, BRD = 1229, 
        BRE = 1230, BRI = 1231, BRL = 1232, BRN = 1233, BRR = 1234, BRS = 1235, BRU = 1236, BSB = 1237, BSL = 1238, BSR = 1239, 
        BTM = 1240, BTR = 1241, BTS = 1242, BTU = 1243, BTV = 1244, BUD = 1245, BUE = 1246, BUF = 1247, BUG = 1248, BUH = 1249, 
        BUQ = 1250, BUR = 1251, BVB = 1252, BWI = 1253, BWN = 1254, BWT = 1255, BXO = 2286, BXS = 1256, BYK = 1257, BYU = 1258, 
        BZE = 1259, BZL = 1260, BZN = 1261, BZV = 1262, CAB = 1263, CAE = 1264, CAG = 1265, CAI = 1266, CAK = 1267, CAL = 1268, 
        CAN = 1269, CAS = 1270, CAY = 1271, CBB = 1272, CBG = 1273, CBR = 1274, CBU = 1275, CCJ = 1276, CCK = 1277, CCR = 1278, 
        CCS = 1279, CCU = 1280, CCZ = 1281, CDC = 1282, CDG = 2293, CDV = 1283, CEB = 1284, CEC = 1285, CED = 1286, CEN = 1287, 
        CEQ = 1288, CES = 1289, CFE = 1290, CFG = 1291, CFN = 1292, CFS = 1293, CFU = 1294, CGA = 1295, CGB = 1296, CGH = 2473, 
        CGK = 1798, CGN = 1297, CGO = 1298, CGP = 1299, CGQ = 1300, CGR = 1301, CGU = 1302, CHA = 1303, CHC = 1304, CHG = 1305, 
        CHI = 1306, CHO = 1307, CHQ = 1308, CHS = 1309, CIA = 2451, CIC = 1310, CID = 1311, CIP = 1312, CIW = 1313, CJB = 1314, 
        CJC = 1315, CJL = 1316, CJS = 1317, CKB = 1318, CKG = 1319, CKY = 1320, CLD = 1321, CLE = 1322, CLJ = 1323, CLL = 1324, 
        CLM = 1325, CLO = 1326, CLQ = 1327, CLT = 1328, CLY = 1329, CMB = 1330, CME = 1331, CMF = 1332, CMH = 1333, CMI = 1334, 
        CMN = 1270, CMQ = 1335, CMX = 1336, CND = 1337, CNF = 1338, CNQ = 1339, CNS = 1340, CNX = 1341, COD = 1342, COK = 1343, 
        CON = 1344, COO = 1345, COR = 1346, COS = 1347, CPD = 1348, CPH = 1349, CPR = 1350, CPT = 1351, CRE = 2179, CRF = 1352, 
        CRG = 1762, CRP = 1353, CRW = 1354, CSG = 1355, CSI = 1356, CSN = 1357, CST = 1358, CTA = 1359, CTG = 1360, CTN = 1361, 
        CTS = 2246, CTU = 1362, CUL = 1363, CUN = 1364, CUR = 1365, CUU = 1366, CVG = 1367, CVM = 1368, CVQ = 1369, CVT = 1370, 
        CWA = 1371, CWB = 1372, CWL = 1373, CXI = 1374, CXT = 1375, CYS = 1376, CYU = 1377, CZA = 1378, CZL = 1379, CZM = 1380, 
        DAB = 1381, DAC = 1382, DAL = 1383, DAM = 1384, DAN = 1385, DAR = 1386, DAY = 1387, DBO = 1388, DBQ = 1389, DBV = 1390, 
        DBY = 1391, DCA = 1392, DCF = 1393, DDI = 1394, DEC = 1395, DEL = 1396, DEN = 1397, DET = 1431, DFW = 1383, DHA = 1398, 
        DHN = 1399, DIL = 1400, DJB = 1401, DJE = 1402, DJJ = 1403, DJO = 1404, DKI = 1405, DKR = 1406, DLA = 1407, DLC = 1408, 
        DLG = 1409, DLH = 1410, DLM = 1411, DLP = 1412, DME = 2127, DMK = 1190, DMM = 1413, DND = 1414, DNK = 1415, DNR = 1416, 
        DNZ = 1417, DOD = 1418, DOH = 1419, DOM = 1420, DOV = 1421, DPO = 1422, DPS = 1423, DRB = 1424, DRO = 1425, DRS = 1426, 
        DRW = 1427, DSA = 2620, DSK = 1428, DSM = 1429, DTM = 1430, DTT = 1431, DTW = 1431, DUB = 1432, DUD = 1433, DUJ = 1434, 
        DUR = 1435, DUS = 1436, DUT = 1437, DVL = 1438, DXB = 1439, DYA = 1440, DYU = 1441, DZA = 1442, EA0 = 1443, EAP = 2107, 
        EAS = 1444, EAT = 1445, EAU = 1446, EB0 = 1447, EBA = 1448, EBB = 1449, EBJ = 1450, EBU = 1451, EDI = 1452, EDR = 1467, 
        EGC = 1453, EGE = 1454, EGS = 1455, EIN = 1456, EIS = 1457, EKI = 1458, EKO = 1459, ELH = 1460, ELL = 1461, ELM = 1462, 
        ELP = 1463, ELS = 1464, ELY = 1465, EMA = 1466, EMD = 1467, EMY = 1468, ENA = 1469, ENC = 1470, ENF = 1471, EP0 = 1472, 
        EPR = 1473, ERC = 1474, ERF = 1475, ERI = 1476, ERS = 2816, ERZ = 1477, ES0 = 1478, ESB = 1075, ESC = 1479, ESF = 1480, 
        ESH = 1481, ESR = 1482, ETH = 1483, ETZ = 2183, EUG = 1484, EUN = 1485, EVE = 1486, EVN = 1487, EVV = 1488, EWN = 1489, 
        EWR = 1490, EXT = 1491, EYW = 1492, EZE = 1246, FAE = 1493, FAI = 1494, FAO = 1495, FAR = 1496, FAT = 1497, FAY = 1498, 
        FBM = 1499, FBU = 2273, FC0 = 1500, FCA = 1501, FCO = 2451, FDF = 1502, FDH = 1503, FEZ = 1504, FFT = 1505, FHU = 1506, 
        FIE = 1507, FIH = 1508, FJR = 1509, FKB = 1510, FKI = 1511, FKL = 1512, FKS = 1513, FLG = 1514, FLL = 1515, FLN = 1516, 
        FLO = 1517, FLR = 1518, FMN = 1519, FMO = 1520, FMY = 1521, FNA = 1522, FNC = 1523, FNI = 1524, FNJ = 1525, FNT = 1526, 
        FOD = 1527, FOE = 1528, FOR = 1529, FOU = 1530, FPO = 1531, FRA = 1532, FRI = 1533, FRJ = 1534, FRO = 1535, FRU = 1536, 
        FRW = 1537, FSC = 1538, FSD = 1539, FSM = 1540, FSP = 1541, FUE = 1542, FUK = 1543, FUN = 1544, FUT = 1545, FWA = 1546, 
        FYV = 1547, GAD = 1548, GAJ = 1549, GAU = 1550, GBE = 1551, GC0 = 1552, GCC = 1553, GCI = 1554, GCM = 1555, GCN = 1556, 
        GDL = 1557, GDN = 1558, GDT = 1559, GDV = 1560, GEG = 1561, GEO = 1562, GET = 1563, GEX = 1564, GFF = 1565, GFK = 1566, 
        GGG = 1567, GGW = 1568, GH0 = 1569, GHB = 1570, GIB = 1571, GIG = 2433, GIL = 1572, GJL = 1573, GJT = 1574, GKL = 1575, 
        GLA = 1576, GLH = 1577, GLT = 1578, GNB = 1579, GND = 1580, GNE = 1581, GNV = 1582, GOA = 1583, GOH = 1584, GOI = 1585, 
        GOJ = 1586, GON = 1587, GOO = 1588, GOT = 1589, GOU = 1590, GOV = 1591, GOZ = 1592, GPA = 1593, GPT = 1594, GPZ = 1595, 
        GR0 = 1596, GRB = 1597, GRJ = 1598, GRO = 1599, GRQ = 1600, GRR = 1601, GRU = 2473, GRX = 1602, GRZ = 1603, GS0 = 1604, 
        GSI = 1605, GSO = 1606, GSP = 1607, GTE = 1608, GTF = 1609, GTI = 1610, GTN = 1611, GUA = 1612, GUC = 1613, GUM = 1614, 
        GVA = 1615, GW0 = 1616, GWD = 1617, GWE = 1618, GWT = 1619, GWY = 1620, GYD = 1127, GYE = 1621, GYN = 1622, GYP = 1623, 
        GZA = 1624, GZT = 1625, HAC = 1626, HAG = 1643, HAH = 1627, HAJ = 1628, HAM = 1629, HAN = 1630, HAP = 1631, HAR = 1632, 
        HAU = 1633, HAV = 1634, HBA = 1635, HBH = 1068, HDD = 1636, HDN = 1637, HDY = 1638, HEL = 1639, HER = 1640, HEW = 1099, 
        HFA = 1641, HFT = 1642, HGH = 1644, HGO = 1645, HHH = 1646, HHN = 1532, HIB = 1647, HII = 1648, HIJ = 1649, HIR = 1650, 
        HIS = 1651, HKD = 1652, HKG = 1653, HKS = 1759, HKT = 1654, HKY = 1655, HLA = 1656, HLH = 1657, HLN = 1658, HLP = 1798, 
        HLT = 1659, HLZ = 1660, HMH = 1661, HMO = 1662, HNA = 1663, HND = 2712, HNH = 1664, HNK = 1665, HNL = 1666, HNS = 1667, 
        HOG = 1668, HOM = 1669, HON = 1670, HOQ = 1671, HOR = 1672, HOU = 1673, HPN = 1674, HRB = 1675, HRE = 1676, HRG = 1677, 
        HRK = 1678, HRL = 1679, HS0 = 1680, HSV = 1681, HTA = 1682, HTI = 1683, HTS = 1684, HUF = 1685, HUH = 1686, HUI = 1687, 
        HUX = 1688, HUY = 1689, HVB = 1690, HVN = 1691, HVR = 1692, HWN = 1693, HYA = 1694, HYD = 1695, HYG = 1696, IAD = 1392, 
        IAG = 1697, IAH = 1673, IBZ = 1698, ICN = 1699, ICT = 1700, IDA = 1701, IDR = 1702, IEV = 1703, IFL = 1704, IGH = 1705, 
        IGR = 1706, IJX = 1707, IKT = 1708, ILE = 1709, ILG = 1710, ILI = 1711, ILM = 1712, ILP = 1713, ILY = 1714, ILZ = 1715, 
        IND = 1716, INI = 1717, INL = 1718, INN = 1719, INU = 1720, INV = 1721, IOM = 1722, IOU = 1723, IPC = 1724, IPL = 1725, 
        IPT = 1726, IQT = 1727, IRG = 1728, IRO = 1729, ISA = 1730, ISB = 1731, ISG = 1732, ISL = 1733, ISO = 1734, ISP = 1735, 
        IST = 1736, ITH = 1737, ITM = 2270, ITO = 1738, IUE = 1739, IVC = 1740, IVL = 1741, IXB = 1742, IXC = 1743, IXG = 1744, 
        IXJ = 1745, IXR = 1746, IXW = 1747, IYK = 1748, IZM = 1749, JAA = 1750, JAC = 1751, JAD = 1752, JAF = 1753, JAG = 1754, 
        JAI = 1755, JAK = 1756, JAL = 1757, JAM = 1758, JAN = 1759, JAQ = 1760, JAU = 1761, JAX = 1762, JBR = 1763, JCB = 1764, 
        JCK = 1765, JCM = 1766, JDF = 1767, JDH = 1768, JDZ = 1769, JED = 1770, JEE = 1771, JEF = 1772, JER = 1773, JFK = 2230, 
        JGA = 1774, JGB = 1775, JGN = 1776, JGS = 1777, JHB = 1778, JHE = 1779, JHG = 1780, JHM = 1781, JHQ = 1782, JHW = 1783, 
        JIB = 1784, JIJ = 1785, JIL = 1786, JIM = 1787, JIN = 1788, JIP = 1789, JIR = 1790, JIU = 1791, JIW = 1792, JJI = 1793, 
        JJN = 1794, JKG = 1795, JKH = 1796, JKR = 1797, JKT = 1798, JKV = 1799, JLN = 1800, JLR = 1801, JLS = 1802, JMB = 1803, 
        JMK = 1804, JMO = 1805, JMS = 1806, JMU = 1807, JNA = 1808, JNB = 1809, JNG = 1810, JNI = 1811, JNU = 1812, JNX = 1813, 
        JNZ = 1814, JOE = 1815, JOI = 1816, JOL = 1817, JON = 1818, JOS = 1819, JPA = 1820, JQE = 1821, JRH = 1822, JRO = 1823, 
        JRS = 1824, JSA = 1825, JSI = 1826, JSM = 1827, JSR = 1828, JST = 1829, JTI = 1830, JTR = 1831, JUB = 1832, JUI = 1833, 
        JUJ = 1834, JUL = 1835, JUM = 1836, JUN = 1837, JUT = 1838, JUV = 1839, JVL = 1840, JWA = 1841, JXN = 1842, JYV = 1843, 
        KAJ = 1844, KAN = 1845, KAO = 1846, KBL = 1847, KBP = 1703, KBY = 1848, KCC = 1849, KCE = 1850, KCH = 1851, KCM = 1852, 
        KCZ = 1853, KDD = 1854, KDU = 1855, KEF = 2424, KEL = 1856, KEM = 1857, KGC = 1858, KGD = 1859, KGI = 1860, KGL = 1861, 
        KGS = 1862, KHH = 1863, KHI = 1864, KHJ = 1865, KHV = 1866, KID = 1867, KIJ = 1868, KIK = 1869, KIM = 1870, KIN = 1871, 
        KIR = 1872, KIV = 1873, KIW = 1874, KIX = 2270, KKN = 1875, KLA = 1876, KLR = 1877, KLU = 1878, KLW = 1879, KLX = 1880, 
        KLZ = 1881, KMI = 1882, KMJ = 1883, KMP = 1884, KMQ = 1885, KNO = 2062, KNS = 1886, KNU = 1887, KNX = 1888, KOA = 1889, 
        KOI = 1890, KOJ = 1891, KOK = 1892, KRB = 1893, KRK = 1894, KRN = 1895, KRP = 1896, KRS = 1897, KRT = 1898, KSD = 1899, 
        KSL = 1900, KSU = 1901, KTA = 1902, KTB = 1903, KTM = 1904, KTN = 1905, KTR = 1906, KTT = 1907, KUA = 1908, KUF = 1909, 
        KUH = 1910, KUL = 1911, KUO = 1912, KVA = 1913, KWI = 1914, KWL = 1915, KWM = 1916, KYA = 1917, KYS = 1918, KZN = 1919, 
        LAD = 1920, LAE = 1921, LAF = 1922, LAI = 1923, LAN = 1924, LAP = 1925, LAR = 1926, LAS = 1927, LAW = 1928, LAX = 1929, 
        LBA = 1930, LBB = 1931, LBE = 1932, LBG = 2293, LBI = 1933, LBQ = 1934, LBU = 1935, LBV = 1936, LCA = 1937, LCG = 1938, 
        LCH = 1939, LCY = 1983, LDC = 1940, LDE = 1941, LDK = 1942, LDY = 1943, LEA = 1944, LEB = 1945, LED = 1946, LEI = 1947, 
        LEJ = 1948, LEK = 1949, LEQ = 1950, LER = 1951, LEX = 1952, LEY = 1953, LFT = 1954, LFW = 1955, LGA = 2230, LGB = 1956, 
        LGG = 1957, LGK = 1958, LGW = 1983, LHE = 1959, LHR = 1983, LI0 = 1960, LIF = 1961, LIG = 1962, LIH = 1963, LIJ = 1964, 
        LIL = 1965, LIM = 1966, LIN = 2088, LIQ = 1967, LIS = 1968, LIT = 1969, LJU = 1970, LKL = 1971, LKO = 1972, LLA = 1973, 
        LLW = 1974, LMM = 1975, LMP = 1976, LMT = 1977, LNK = 1978, LNO = 1979, LNS = 1980, LNY = 1981, LNZ = 1982, LON = 1983, 
        LOS = 1984, LPA = 1985, LPB = 1986, LPL = 1987, LPP = 1988, LRD = 1989, LRE = 1990, LRF = 1991, LRH = 1992, LRM = 1993, 
        LRT = 1994, LSE = 1995, LSI = 1996, LST = 1997, LSY = 1998, LSZ = 1999, LTN = 1983, LTO = 2000, LU0 = 2001, LUD = 2002, 
        LUG = 2003, LUJ = 2004, LUM = 2005, LUN = 2006, LUX = 2007, LV0 = 2008, LVO = 2009, LWB = 2010, LWK = 2011, LWO = 2012, 
        LWS = 2013, LWT = 2014, LXR = 2015, LYH = 2016, LYO = 2017, LYP = 2018, LYR = 2019, LYS = 2020, LYX = 2021, LZC = 2022, 
        LZR = 2023, MAA = 2024, MAD = 2025, MAF = 2026, MAH = 2027, MAJ = 2028, MAN = 2029, MAO = 2030, MAR = 2031, MAU = 2032, 
        MAZ = 2033, MBA = 2034, MBH = 2035, MBJ = 2036, MBM = 2037, MBS = 2038, MBX = 2039, MC0 = 2040, MCE = 2041, MCI = 2042, 
        MCL = 2043, MCM = 2044, MCN = 2045, MCO = 2267, MCP = 2046, MCT = 2047, MCW = 2048, MCY = 2049, MCZ = 2050, MDC = 2051, 
        MDE = 2052, MDL = 2053, MDQ = 2054, MDT = 1632, MDW = 1306, MDY = 2055, MDZ = 2056, MED = 2057, MEE = 2058, MEI = 2059, 
        MEL = 2060, MEM = 2061, MES = 2062, MEX = 2063, MEZ = 2064, MFE = 2065, MFF = 2066, MFG = 2067, MFM = 2068, MFN = 2069, 
        MFO = 2070, MFQ = 2071, MFR = 2072, MFU = 2073, MGA = 2074, MGB = 2075, MGF = 2076, MGH = 2077, MGM = 2078, MGQ = 2079, 
        MGW = 2080, MHE = 2081, MHH = 2082, MHQ = 2083, MHT = 2084, MIA = 2085, MID = 2086, MIK = 2087, MIL = 2088, MIM = 2089, 
        MIR = 2090, MJC = 2091, MJD = 2092, MJL = 2093, MJN = 2094, MJQ = 2095, MJT = 2096, MJV = 2097, MKE = 2098, MKG = 2099, 
        MKK = 2100, MKL = 2101, MKR = 2102, MKY = 2103, MLA = 2104, MLB = 2105, MLE = 2106, MLH = 2107, MLI = 2108, MLM = 2109, 
        MLS = 2110, MLU = 2111, MLW = 2112, MLX = 2113, MMA = 2119, MME = 2114, MMG = 2115, MMJ = 2116, MMK = 2117, MMM = 2118, 
        MMX = 2119, MMY = 2120, MNI = 2121, MNL = 2122, MOB = 2123, MOD = 2124, MOT = 2125, MOV = 2126, MOW = 2127, MOZ = 2128, 
        MPA = 2129, MPD = 2130, MPL = 2131, MPM = 2132, MQL = 2133, MQM = 2134, MQP = 2212, MQQ = 2135, MQT = 2136, MRB = 2137, 
        MRS = 2138, MRU = 2139, MRV = 2140, MRY = 2141, MRZ = 2142, MSE = 2143, MSL = 2144, MSN = 2145, MSO = 2146, MSP = 2147, 
        MSQ = 2148, MSR = 2149, MST = 2150, MSU = 2151, MSY = 2152, MTH = 2153, MTJ = 2154, MTL = 2155, MTM = 2156, MTO = 2157, 
        MTS = 2158, MTT = 2159, MTY = 2160, MUB = 2161, MUC = 2162, MUE = 2163, MUH = 2164, MUX = 2165, MV0 = 2166, MVB = 2167, 
        MVD = 2168, MVR = 2169, MVY = 2170, MVZ = 2171, MWD = 2172, MWH = 2173, MXL = 2174, MXP = 2088, MYA = 2175, MYD = 2176, 
        MYJ = 2177, MYQ = 2178, MYR = 2179, MYY = 2180, MZF = 2181, MZG = 2182, MZM = 2183, MZT = 2184, MZY = 2185, NAA = 2186, 
        NAG = 2187, NAJ = 2188, NAN = 2189, NAP = 2190, NAS = 2191, NAT = 2192, NAY = 1185, NBO = 2193, NCE = 2194, NCL = 2195, 
        NCS = 2196, NCY = 2197, NDB = 2198, NDJ = 2199, NDU = 2200, NEV = 2201, NGE = 2202, NGO = 2203, NGS = 2204, NIC = 2205, 
        NIM = 2206, NIP = 1762, NKC = 2207, NKW = 2208, NLA = 2209, NLD = 2210, NLK = 2211, NLP = 2212, NLU = 2063, NLV = 2213, 
        NNG = 2214, NOC = 2215, NOP = 2216, NOU = 2217, NQY = 2218, NRA = 2219, NRK = 2220, NRT = 2712, NSA = 2221, NSN = 2222, 
        NSO = 2223, NST = 2224, NTE = 2225, NTL = 2226, NTR = 2160, NTY = 2227, NUE = 2228, NWI = 2229, NYC = 2230, NZC = 1762, 
        OAG = 2231, OAJ = 2232, OAK = 2233, OAX = 2234, OB0 = 2235, ODA = 2236, ODB = 2237, ODE = 2238, ODS = 2239, OGG = 2240, 
        OHD = 2241, OHT = 2242, OIT = 2243, OKA = 2244, OKC = 2245, OKD = 2246, OKJ = 2247, OKU = 2248, OLB = 2249, OLF = 2250, 
        OLM = 2251, OLP = 2252, OMA = 2253, OMD = 2254, OME = 2255, OMH = 2256, OMO = 2257, OND = 2258, ONT = 2259, OOL = 2260, 
        OOM = 2261, OPO = 2262, ORB = 2263, ORD = 1306, ORF = 2264, ORH = 2265, ORK = 2266, ORL = 2267, ORN = 2268, ORS = 2269, 
        ORY = 2293, OSA = 2270, OSH = 2271, OSI = 2272, OSL = 2273, OSM = 2274, OTH = 2275, OTP = 1249, OTZ = 2276, OUA = 2277, 
        OUD = 2278, OUH = 2279, OUK = 2280, OUL = 2281, OUZ = 2282, OVB = 2283, OVD = 2284, OWB = 2285, OXB = 2286, OXR = 2287, 
        OZZ = 2288, PAC = 2289, PAD = 2290, PAH = 2291, PAP = 2292, PAR = 2293, PAT = 2294, PBC = 2295, PBH = 2296, PBI = 2297, 
        PBM = 2298, PBO = 2299, PBZ = 2300, PC0 = 2301, PCT = 2302, PDL = 2303, PDT = 2304, PDX = 2305, PEG = 2306, PEI = 2307, 
        PEK = 1185, PEN = 2308, PER = 2309, PEW = 2310, PFN = 2311, PFO = 2312, PGA = 2313, PGF = 2314, PGV = 2315, PHC = 2316, 
        PHE = 2317, PHF = 2318, PHL = 2319, PHW = 2320, PHX = 2321, PIA = 2322, PIB = 2323, PIH = 2324, PIK = 1576, PIR = 2325, 
        PIS = 2326, PIT = 2327, PJG = 2328, PKB = 2329, PKW = 2330, PLB = 2331, PLN = 2332, PLO = 2333, PLZ = 2334, PMD = 2335, 
        PMI = 2336, PMO = 2337, PMR = 2338, PMV = 2339, PMW = 2340, PN0 = 2341, PNH = 2342, PNI = 2343, PNL = 2344, PNQ = 2345, 
        PNR = 2346, PNS = 2347, POA = 2348, POG = 2349, POM = 2350, POP = 2351, POR = 2352, POS = 2353, POU = 2354, POZ = 2355, 
        PPG = 2356, PPP = 2357, PPT = 2358, PQI = 2359, PQQ = 2360, PRG = 2361, PRN = 2362, PRY = 2363, PS0 = 2364, PSA = 2365, 
        PSC = 2366, PSD = 2367, PSE = 2368, PSG = 2369, PSI = 2370, PSP = 2371, PSR = 2372, PSY = 2373, PTG = 2374, PTJ = 2375, 
        PTP = 1130, PTY = 2289, PUB = 2376, PUF = 2377, PUG = 2378, PUJ = 2379, PUQ = 2380, PUS = 2381, PUW = 2382, PUY = 2383, 
        PVD = 2384, PVG = 2520, PVH = 2385, PVK = 2386, PVR = 2387, PWM = 2388, PXM = 2389, PXO = 2390, PYB = 2391, PYX = 2392, 
        PZB = 2393, PZH = 2394, PZO = 2395, QCB = 2396, QDU = 1436, QGF = 2397, QIC = 2398, QLI = 2399, QML = 2400, QMN = 2401, 
        QNB = 2402, QND = 2403, QPA = 2404, QPG = 2528, QSA = 2405, QUF = 2664, RA0 = 2406, RAI = 2407, RAJ = 2408, RAK = 2409, 
        RAP = 2410, RAR = 2411, RAZ = 2412, RB0 = 2413, RBA = 2414, RBR = 2415, RCB = 2416, RDD = 2417, RDG = 2418, RDM = 2419, 
        RDU = 2420, RDZ = 2421, REC = 2422, REG = 2423, REK = 2424, REU = 2425, RFD = 2426, RFP = 2427, RGI = 2428, RGN = 2429, 
        RHI = 2430, RHO = 2431, RIC = 2432, RIO = 2433, RIX = 2434, RIZ = 2435, RJK = 2436, RKD = 2437, RKS = 2438, RKT = 2439, 
        RKV = 2424, RLT = 2440, RMF = 2441, RMI = 2442, RNB = 2443, RNE = 2444, RNN = 2445, RNO = 2446, RNS = 2447, ROA = 2448, 
        ROB = 2112, ROC = 2449, ROK = 2450, ROM = 2451, ROR = 2452, ROS = 2453, ROT = 2454, ROU = 2455, ROV = 2456, RRG = 2457, 
        RSA = 2458, RSD = 2459, RST = 2460, RSW = 1521, RTB = 2461, RTM = 2462, RUH = 2463, RUN = 2464, RVI = 2456, RVN = 2465, 
        RWI = 2466, RWP = 2467, RYK = 2468, SAF = 2469, SAH = 2470, SAL = 2471, SAN = 2472, SAO = 2473, SAP = 2474, SAT = 2475, 
        SAV = 2476, SAW = 1736, SAY = 2477, SBA = 2478, SBH = 2479, SBK = 2480, SBN = 2481, SBP = 2482, SBU = 2483, SBW = 2484, 
        SBY = 2485, SBZ = 2486, SCC = 2487, SCE = 2488, SCF = 2489, SCK = 2490, SCL = 2588, SCN = 2491, SCQ = 2492, SCU = 2493, 
        SDF = 2494, SDH = 2495, SDJ = 2496, SDL = 2497, SDQ = 2498, SDR = 2499, SDS = 2500, SDT = 2501, SDU = 2433, SDY = 2502, 
        SEA = 2503, SEB = 2504, SEL = 2505, SEN = 2506, SEW = 2507, SEZ = 2508, SFA = 2509, SFG = 2510, SFJ = 2511, SFN = 2512, 
        SFO = 2513, SGD = 2514, SGF = 2515, SGN = 2516, SGU = 2517, SGY = 2518, SH0 = 2519, SHA = 2520, SHD = 2521, SHE = 2522, 
        SHJ = 2523, SHR = 2524, SHV = 2525, SI0 = 2526, SID = 2527, SIN = 2528, SIP = 2529, SIS = 2530, SIT = 2531, SIX = 2532, 
        SJC = 2533, SJD = 2534, SJI = 2536, SJJ = 2535, SJO = 2536, SJT = 2537, SJU = 2538, SJY = 2539, SK0 = 2540, SKB = 2541, 
        SKD = 2542, SKG = 2543, SKP = 2544, SKS = 2545, SKV = 2546, SKX = 2547, SKZ = 2548, SLA = 2549, SLC = 2550, SLE = 2551, 
        SLL = 2552, SLP = 2553, SLS = 2554, SLU = 2555, SLZ = 2556, SM0 = 2557, SMA = 2558, SMF = 2559, SMI = 2560, SMX = 2561, 
        SNA = 2562, SNC = 2563, SNN = 2564, SNS = 2565, SOF = 2566, SOG = 2567, SOI = 2568, SON = 2569, SOT = 2570, SOU = 2571, 
        SPC = 2572, SPI = 2573, SPK = 2246, SPN = 2574, SPS = 2575, SPU = 2576, SPY = 2577, SRA = 2578, SRB = 2579, SRL = 2580, 
        SRQ = 2581, SRZ = 2582, SSA = 2583, SSG = 2584, SSH = 2585, SSL = 2586, ST0 = 2587, STI = 2588, STL = 2589, STN = 1983, 
        STO = 2590, STP = 2591, STR = 2592, STS = 2593, STT = 2594, STV = 2595, STX = 2596, SUB = 2597, SUF = 2598, SUL = 2599, 
        SUM = 2600, SUN = 2601, SUV = 2602, SUX = 2603, SVD = 2604, SVG = 2605, SVL = 2606, SVO = 2127, SVQ = 2607, SVX = 2608, 
        SW0 = 2609, SWF = 2610, SWP = 2611, SWS = 2612, SXB = 2613, SXF = 1150, SXL = 2614, SXM = 2615, SXR = 2616, SYD = 2617, 
        SYR = 2618, SYY = 2619, SZB = 1911, SZD = 2620, SZF = 2621, SZG = 2622, SZK = 2623, SZX = 2624, SZZ = 2625, TAB = 2626, 
        TAK = 2627, TAM = 2628, TAO = 2629, TAS = 2630, TAY = 2631, TBS = 2632, TBU = 2633, TCA = 2634, TCB = 2635, TCI = 2646, 
        TCL = 2636, TCU = 2637, TED = 2638, TEM = 2639, TEQ = 2640, TER = 2641, TEU = 2642, TEX = 2643, TF0 = 2644, TFN = 2646, 
        TFS = 2646, TGD = 2647, TGU = 2648, TGV = 2649, TGZ = 2650, THE = 2651, THF = 1150, THR = 2652, TIA = 2653, TIF = 2654, 
        TIJ = 2655, TIP = 2656, TIS = 2657, TIV = 2658, TK0 = 2659, TKA = 2660, TKS = 2661, TKU = 2662, TLH = 2663, TLL = 2664, 
        TLS = 2665, TLV = 2666, TMP = 2667, TMS = 2668, TMW = 2669, TMZ = 2670, TNA = 2671, TNG = 2672, TNR = 2673, TOD = 2674, 
        TOL = 2675, TOS = 2676, TOU = 2677, TOV = 2678, TOY = 2679, TPA = 2680, TPE = 2631, TPR = 2681, TPS = 2682, TRD = 2683, 
        TRF = 2273, TRI = 2684, TRN = 2685, TRO = 2686, TRS = 2687, TRV = 2688, TRW = 2689, TRZ = 2690, TSA = 2631, TSB = 2691, 
        TSE = 2692, TSF = 2693, TSN = 2694, TSV = 2695, TTN = 2696, TUC = 2697, TUK = 2698, TUL = 2699, TUN = 2700, TUP = 2701, 
        TUS = 2702, TUU = 2703, TVC = 2704, TVF = 2705, TVL = 2706, TWB = 2707, TWF = 2708, TWU = 2709, TXK = 2710, TXL = 1150, 
        TYN = 2711, TYO = 2712, TYR = 2713, TYS = 2714, TZX = 2715, UAH = 2716, UAK = 2717, UAP = 2718, UBA = 2719, UBJ = 2720, 
        UBP = 2721, UCA = 2722, UCT = 2723, UDE = 2724, UDI = 2725, UDJ = 2726, UDR = 2727, UEE = 2728, UET = 2729, UFA = 2730, 
        UGC = 2731, UGO = 2732, UHE = 2733, UII = 2734, UIN = 2735, UIO = 2736, UIP = 2737, UIT = 2738, UKB = 2739, UKI = 2740, 
        UKY = 2741, ULB = 2742, ULD = 2743, ULN = 2744, ULU = 2745, UMD = 2746, UME = 2747, UMR = 2748, UNI = 2749, UNK = 2750, 
        UNT = 2751, UPG = 2752, UPL = 2753, UPN = 2754, UPP = 2755, URB = 2756, URC = 2757, URG = 2758, URM = 2759, URZ = 2760, 
        USH = 2761, USN = 2762, UTC = 2763, UTH = 2764, UTN = 2765, UTP = 2766, UTT = 2767, UUD = 2768, UVE = 2769, UVF = 2770, 
        UVL = 2771, VA0 = 2772, VAA = 2773, VAN = 2774, VAP = 2775, VAR = 2776, VAS = 2777, VBS = 2778, VBY = 2779, VCE = 2780, 
        VCP = 2473, VDA = 1483, VDE = 2781, VDZ = 2782, VEL = 2783, VER = 2784, VFA = 2785, VGO = 2786, VID = 2787, VIE = 2788, 
        VIJ = 2789, VIS = 2790, VIT = 2791, VIX = 2792, VKO = 2127, VLC = 2793, VLD = 2794, VLI = 2795, VLL = 2796, VLN = 2797, 
        VLU = 2798, VN0 = 2799, VNO = 2800, VNS = 2801, VOG = 2802, VPS = 2803, VRA = 2804, VRB = 2805, VRK = 2806, VRN = 2807, 
        VSA = 2808, VST = 2809, VTE = 2810, VVO = 2811, VXO = 2812, VYD = 2813, VZ0 = 2814, WAS = 1392, WAW = 2815, WDH = 2816, 
        WEI = 2817, WEL = 2818, WGA = 2819, WHK = 2820, WHM = 2821, WIC = 2822, WIE = 2823, WLB = 2824, WLG = 2825, WLS = 2826, 
        WMB = 2827, WNS = 2828, WOL = 2829, WP0 = 2830, WRE = 2831, WRG = 2832, WRL = 2833, WRO = 2834, WUH = 2835, WUN = 2836, 
        WUX = 2837, WVB = 2838, WYA = 2839, WYN = 2840, WYS = 2841, XCH = 2842, XIY = 2843, XLB = 2844, XMH = 2845, XMN = 2846, 
        XPK = 2847, XRY = 2848, XSI = 2849, XSP = 2528, XXX = 2850, YAK = 2851, YAO = 2852, YAT = 2853, YBE = 2854, YCB = 2855, 
        YDF = 2856, YEA = 2906, YEG = 2906, YEV = 2857, YFA = 2858, YFB = 2859, YFC = 2860, YFO = 2861, YGL = 2862, YGW = 2863, 
        YGX = 2864, YHM = 2865, YHR = 2866, YHZ = 2867, YIF = 2868, YIH = 2869, YKA = 2870, YKM = 2871, YKS = 2872, YLR = 2873, 
        YLW = 2874, YMM = 2875, YMQ = 2876, YMX = 2876, YNB = 2877, YOK = 2878, YOP = 2879, YOW = 2880, YPN = 2881, YPR = 2882, 
        YQB = 2883, YQD = 2884, YQG = 2885, YQM = 2886, YQR = 2887, YQT = 2888, YQX = 2889, YRB = 2890, YSJ = 2891, YSM = 2892, 
        YSR = 2893, YTH = 2894, YTO = 2895, YTZ = 2895, YUD = 2896, YUL = 2876, YUM = 2897, YUX = 2898, YVB = 2899, YVO = 2900, 
        YVP = 2901, YVQ = 2902, YVR = 2903, YWG = 2904, YWK = 2905, YXD = 2906, YXE = 2907, YXJ = 2908, YXN = 2909, YXS = 2910, 
        YXT = 2911, YXU = 2912, YXY = 2913, YYC = 2914, YYD = 2915, YYJ = 2916, YYQ = 2917, YYR = 2918, YYT = 2919, YYZ = 2920, 
        YZF = 2921, YZP = 2922, ZAD = 2923, ZAG = 2924, ZAZ = 2925, ZBO = 2926, ZCL = 2927, ZIH = 2928, ZJK = 1653, ZKE = 2929, 
        ZLO = 2930, ZND = 2931, ZNE = 2932, ZQN = 2933, ZRH = 2934, ZSA = 2935, ZSS = 2936, ZTH = 2937, ZTM = 2938, ZYL = 2939, 

        MAXCITY = 2940, NUMCITY = 2051 
        };

    
	City( void ): m_city(NOCITY) {}
	~City( void ) { m_city = NOCITY; }
	
	// non-explicit constructors intentional here
	City( CityCode i ): m_city(i) {} // i.e. i = City::LON
	City( const std::string& s ): m_city(NOCITY) { setCity(s); }
	City( const char *s ): m_city(NOCITY) { if (s) setCity(s); } 
    
	// My numeric code for this city i.e. City::LON = 1983
	operator short( void ) const { return m_city; }
	
	// The IATA 3 letter code for this city i.e. "LON"
    std::string
    toString( void ) const { return m_cityNames[m_fromISO[m_city]]; }
	
	std::string 
	name( void ) const { return m_fullCityNames[m_fromISO[m_city]]; } // i.e "London" 
    
	bool
	setCity( const std::string& s ); // i.e. s = "LON"
	
	void
	setCity( const CityCode s ) { m_city = s; } // i.e. s = City::LON
    
    bool
    capital( void ) const { return m_capital[m_fromISO[m_city]]; }
    
    // longitude
    float 
    lon( void ) const { return m_position[m_fromISO[m_city]][1]; }

    // latitude
    float 
    lat( void ) const { return m_position[m_fromISO[m_city]][0]; }
    
    // distance (in metres) calculated using Vincenty inverse solution
    static float
    dist( const City c1, const City c2 ) { return City::dist(c1.lat(), c1.lon(), c2.lat(), c2.lon()); };
    
    static float
    dist( float lat1, float lon1, float lat2, float lon2 );
    
	static City
	index( const int i ) { return CityCode(m_toISO[i]); }
    
    static int
	index( const City c ) { return CityCode(m_fromISO[c]); }
    
    bool                
	valid( void ) const { return m_city != NOCITY; }
	
private:
	
	short m_city; // we use short here as it simplifies streaming 
	
	static const char * const  m_cityNames[NUMCITY];
	static const char * const  m_fullCityNames[NUMCITY];
	static const short         m_toISO[NUMCITY];
    static const short         m_fromISO[MAXCITY]; 
	static const unsigned char m_capital[NUMCITY];
	static const float         m_position[NUMCITY][2];    
    static const short         m_searchPoints[27];    
};


std::ostream&
operator<<(std::ostream& ostr, const City& c);

std::istream&
operator>>(std::istream& istr, City& c);


#endif
