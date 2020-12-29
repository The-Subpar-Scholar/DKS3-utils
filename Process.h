#ifndef UNTITLED3_LIBRARY_H
#define UNTITLED3_LIBRARY_H

#include <windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>

extern HANDLE process_handle;

[[nodiscard]] unsigned long long
ptr(unsigned long long base, const std::vector<int> &offsets = {}); //takes a base and offsets, and builds a pointer


class Process {

public:

    Process(const std::string &window_name);

    ///READING
    [[nodiscard]] static long long readQWORD(unsigned long long address); // Returns QWORD at address

    [[nodiscard]] static int readDWORD(unsigned long long address); //returns DWORD at address

    [[nodiscard]] static short readWORD(unsigned long long address); //returns WORD at address

    [[nodiscard]] static char readCHAR(unsigned long long address); //returns CHAR (byte) at address

    [[nodiscard]] static double readDOUBLE(unsigned long long address); //returns double at address

    [[nodiscard]] static float readFLOAT(unsigned long long address); //returns float at address

    [[nodiscard]] static std::vector<unsigned char> readBytes(unsigned long long address, unsigned int num = 1); //returns a vector of bytes from address

    [[nodiscard]] static std::wstring readWstring(unsigned long long address, short size = 1);

    [[nodiscard]] static std::string readString(unsigned long long address, short length, short char_size = 1);


    ///WRITING
    static void writeQWORD(unsigned long long address, long long value); //writes a QWORD value to the address

    static void writeDWORD(unsigned long long address, int value); //writes a DWORD value to the address

    static void writeWORD(unsigned long long address, short value); //writes a WORD value to the address

    static void writeCHAR(unsigned long long address, char value); //writes a CHAR (byte) value to the address

    static void writeFloat(unsigned long long address, float value); //writes a value to the address

    static void writeDouble(unsigned long long address, double value);//writes a value to the address

    static void writeBytes(unsigned long long address, std::vector<unsigned char> bytes); //writes a byte table to the set of bytes at the address

    static void writeWstring(unsigned long long address, std::wstring string);

    static void writeString(unsigned long long address, std::string string, unsigned short char_size = 1);

};
#endif //UNTITLED3_LIBRARY_H
