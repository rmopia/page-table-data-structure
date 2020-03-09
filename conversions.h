#ifndef CONVERSIONS
#define CONVERSIONS

/* Logical address to page # converter */
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);

/* convert int dec to hex string */
std::string DecToHex(unsigned int dec_num);

/* converts hex string to decimal int */
unsigned int HexToDec(std::string hex_num);

/* converts bin string to hex string */
std::string BinToHex(std::string bin_num);

#endif
