#include "Process.h"
#include <windows.h>
#include <vector>
#include <iostream>

HANDLE process_handle;

[[nodiscard]] unsigned long long ptr(unsigned long long base, const std::vector<int> &offsets) { //builds a pointer
    for (int offset : offsets) {
        ReadProcessMemory(process_handle, (LPVOID) base, &base, (SIZE_T) 8, nullptr);
        base += offset;
    }
    return base;
}
///CONSTRUCTOR
[[nodiscard]] long long Process::readQWORD(unsigned long long address) { // Returns QWORD at address
    ReadProcessMemory(process_handle, (LPVOID) address, &address, (SIZE_T) 8, nullptr);
    return address;
}

///READS

[[nodiscard]] int Process::readDWORD(unsigned long long address) { //returns DWORD at address
    ReadProcessMemory(process_handle, (LPVOID) address, &address, (SIZE_T) 4, nullptr);
    return address;
}

[[nodiscard]] short Process::readWORD(unsigned long long address) { //returns WORD at address
    ReadProcessMemory(process_handle, (LPVOID) address, &address, (SIZE_T) 2, nullptr);
    return address;
}

[[nodiscard]] char Process::readCHAR(unsigned long long address) { //returns CHAR (byte) at address
    ReadProcessMemory(process_handle, (LPVOID) address, &address, (SIZE_T) 1, nullptr);
    return address;
}

[[nodiscard]] double Process::readDOUBLE(unsigned long long address) { //returns double at address
    ReadProcessMemory(process_handle, (LPVOID) address, &address, (SIZE_T) 8, nullptr);
    return address;
}

[[nodiscard]] float Process::readFLOAT(unsigned long long address) { //returns float at address
    ReadProcessMemory(process_handle, (LPVOID) address, &address, (SIZE_T) 4, nullptr);
    return address;
}

[[nodiscard]] std::vector<unsigned char> Process::readBytes(unsigned long long address, unsigned int num) { //returns a vector of bytes from address
    std::vector<unsigned char> bytes;
    bytes.resize(num);
    for (int i = 0; i < num; ++i) {
        ReadProcessMemory(process_handle, (LPVOID) (address + i), &bytes[i], (SIZE_T) 1, nullptr);
    }
    return bytes;
}

[[nodiscard]] std::wstring Process::readWstring(unsigned long long address, short size) { //returns a widestring of a specified size from address
    std::wstring result = L"";
    wchar_t character;
    for (int i = 0; i < size; ++i) {
        ReadProcessMemory(process_handle, (LPVOID) (address + i * 2), &character, (SIZE_T) 2, nullptr);
        if (character == '\0') return result;
        result += character;
    }
    return result;
}

[[nodiscard]] std::string Process::readString(unsigned long long address, short length, short char_size) { //returns a string of a specified size from address, char_size can be used to obtain ascii chars from unicode by skipping the second unicode byte
    std::string result = "";
    char character;
    for (int i = 0; i < length; ++i) {
        ReadProcessMemory(process_handle, (LPVOID) (address + i * char_size), &character, (SIZE_T) 1, nullptr);
        if (character == '\0') return result;
        result += character;
    }
    return result;
}

///WRITES

void Process::writeQWORD(unsigned long long address, long long value) {
    WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)8, nullptr);
}

void Process::writeDWORD(unsigned long long address, int value) {
    WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)4, nullptr);
}

void Process::writeWORD(unsigned long long address, short value) {
    WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)2, nullptr);
}

void Process::writeCHAR(unsigned long long address, char value) {
    WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)1, nullptr);
}

void Process::writeFloat(unsigned long long address, float value) {
    WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)4, nullptr);
}

void Process::writeDouble(unsigned long long address, double value) {
    WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)8, nullptr);
}

void Process::writeBytes(unsigned long long address, std::vector<unsigned char> bytes) {
    for (int i = 0; i < bytes.size(); i++){
        WriteProcessMemory(process_handle, (LPVOID)(address + i), &bytes[i], (SIZE_T)1, nullptr);
    }
}

void Process::writeWstring(unsigned long long address, std::wstring string) {
    unsigned int place = 0;
    for (int i = 0; i < string.length(); ++i) {
        WriteProcessMemory(process_handle, (LPVOID)(address + place), &string.at(i), (SIZE_T)sizeof(string.at(i)), nullptr);
        place += sizeof(string.at(i));
    }
}

void Process::writeString(unsigned long long address, std::string string, unsigned short char_size) {
    for (int i = 0; i < string.length(); ++i) {
        WriteProcessMemory(process_handle, (LPVOID)(address + i * char_size), &string.at(i), (SIZE_T)1, nullptr);
    }
}