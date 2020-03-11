#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "conversions.h"

using namespace std;

void LogicalToPhysicalAddr(unsigned int addr,
                           int offset_mask, int frame, int page_size){
    cout << DecToHex(addr) << " -> ";
    /* offset obtained from logical addr and num of bits of offset */
    int offset = addr & offset_mask;
    /* virtual addr converted to physical addr */
    int result = (frame)*(page_size) + offset;
    cout << DecToHex(result) << endl;
}

/* find page number function */
unsigned int LogicalToPage(unsigned int LogicalAddress,
                           unsigned int Mask, unsigned int Shift){
    unsigned int bitwise_and_res;
    /* bitwise and to remove unnecessary bits */
    bitwise_and_res = LogicalAddress & Mask;
    /* bitwise shift to remove trailing zeros */
    unsigned int bitwise_shift_res = bitwise_and_res >> Shift;
    return bitwise_shift_res;
}

/* convert int dec to hex string */
string DecToHex(unsigned int dec_num){
    string hex_str;
    stringstream ss;
    /* fills in leading zeros at the limit of 8 char */
    ss << setfill('0') << setw(8) << hex << dec_num;
    hex_str = ss.str();
    return hex_str;
}

/* convert hex string to dec */
unsigned int HexToDec(string hex_num){
    unsigned int dec_num;
    stringstream ss;
    ss << hex << hex_num;
    ss >> dec_num;
    return dec_num;
}

/* converts binary string to hex string */
string BinToHex(string bin_num){
    string hex_str, byte;
    /* reads every 4 bits of binary and converts it to hex */
	for (int i = 0; i < bin_num.length(); i += 4){
        /* gets ith bit and gets the substr in a group of 4 */
		byte = bin_num.substr(i, 4);
		if (byte.compare("0000")==0){ hex_str += "0"; }
		else if (byte.compare("0001")==0){ hex_str += "1"; }
		else if (byte.compare("0010")==0){ hex_str += "2"; }
		else if (byte.compare("0011")==0){ hex_str += "3"; }
		else if (byte.compare("0100")==0){ hex_str += "4"; }
		else if (byte.compare("0101")==0){ hex_str += "5"; }
		else if (byte.compare("0110")==0){ hex_str += "6"; }
		else if (byte.compare("0111")==0){ hex_str += "7"; }
		else if (byte.compare("1000")==0){ hex_str += "8"; }
		else if (byte.compare("1001")==0){ hex_str += "9"; }
		else if (byte.compare("1010")==0){ hex_str += "A"; }
		else if (byte.compare("1011")==0){ hex_str += "B"; }
		else if (byte.compare("1100")==0){ hex_str += "C"; }
		else if (byte.compare("1101")==0){ hex_str += "D"; }
		else if (byte.compare("1110")==0){ hex_str += "E"; }
		else if (byte.compare("1111")==0){ hex_str += "F"; }
	}
	return hex_str;
}
