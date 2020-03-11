#ifndef CONVERSIONS
#define CONVERSIONS

/* Logical addr to physical addr translation */
void LogicalToPhysicalAddr(unsigned int addr,
                           int offset_mask, int frame, int page_size);

/* Logical address to page # converter */
unsigned int LogicalToPage(unsigned int LogicalAddress,
                           unsigned int Mask, unsigned int Shift);

/* convert int dec to hex string */
std::string DecToHex(unsigned int dec_num);

/* converts hex string to decimal int */
unsigned int HexToDec(std::string hex_num);

/* converts bin string to hex string */
std::string BinToHex(std::string bin_num);

#endif
