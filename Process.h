#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>

class Process
{
    std::string process_name;

public:

    HANDLE process_handle;

    explicit Process(LPCSTR window_name) {
        if (window_name == nullptr) return;
        DWORD PID;
        GetWindowThreadProcessId(FindWindowA(nullptr, window_name), &PID);
        process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
        process_name = window_name;
    }

    //receives qword args
    [[nodiscard]] unsigned long long build_ptr(unsigned long long base, const std::vector<int>& offsets = {}) const {
        for (int offset : offsets) {
            ReadProcessMemory(process_handle, (LPVOID)base, &base, (SIZE_T)8, nullptr);
            base += offset;
        }
        return base;
    }

    //////WRITES////////////////////////////////////////////////////////////////////////////////////
    //eight byte integer (quad word)
    void writeQWORD(unsigned long long address, long long value) const {
        WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)8, nullptr);
    }

    //four byte integer (double word)
    void writeDWORD(unsigned long long address, int value) const {
        WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)4, nullptr);
    }

    //two byte integer (word)
    void writeWORD(unsigned long long address, short value) const {
        WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)2, nullptr);
    }

    //one byte integer
    void writeCHAR(unsigned long long address, char value) const { //
        WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)1, nullptr);
    }

    //single precision floating point value
    void writeFloat(unsigned long long address, float value) const {
        WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)4, nullptr);
    }

    //double precision floating point value
    void writeDouble(unsigned long long address, double value) const {
        WriteProcessMemory(process_handle, (LPVOID)address, &value, (SIZE_T)8, nullptr);
    }

    void writeBytes(unsigned long long address, std::vector<unsigned char> bytes){
        for (int i = 0; i < bytes.size(); i++){
            WriteProcessMemory(process_handle, (LPVOID)(address + i), &bytes[i], (SIZE_T)1, nullptr);
        }
    }
/*
		bool writeWideString(unsigned long long address, short size = 1) {
			return WriteProcessMemory(process_handle, (LPVOID)address, &result, (SIZE_T)size, NULL);
		}

		bool writeString(unsigned long long address, short size = 1) {
			return WriteProcessMemory(process_handle, (LPVOID)address, &result, (SIZE_T) size, NULL);
		}
*/
    //////READS/////////////////////////////////////////////////////////////////////////////////////
    //eight byte integer (quad word)
    [[nodiscard]] long long readQWORD(unsigned long long address) const {
        ReadProcessMemory(process_handle, (LPVOID)address, &address, (SIZE_T)8, nullptr);
        return address;
    }

    //four byte integer (double word)
    [[nodiscard]] int readDWORD(unsigned long long address) const {
        ReadProcessMemory(process_handle, (LPVOID)address, &address, (SIZE_T)4, nullptr);
        return address;
    }

    //two byte integer (word)
    [[nodiscard]] short readWORD(unsigned long long address) const {
        ReadProcessMemory(process_handle, (LPVOID)address, &address, (SIZE_T)2, nullptr);
        return address;
    }

    //one byte integer
    [[nodiscard]] char readCHAR(unsigned long long address) const { //
        ReadProcessMemory(process_handle, (LPVOID)address, &address, (SIZE_T)1, nullptr);
        return address;
    }

    //single precision floating point value
    [[nodiscard]] float readFloat(unsigned long long address) const {
        ReadProcessMemory(process_handle, (LPVOID)address, &address, (SIZE_T)4, nullptr);
        return address;
    }

    //double precision floating point value
    [[nodiscard]] double readDouble(unsigned long long address) const {
        ReadProcessMemory(process_handle, (LPVOID)address, &address, (SIZE_T)8, nullptr);
        return address;
    }

    [[nodiscard]] std::vector<unsigned char> readBytes(unsigned long long address, unsigned int num) const {
        std::vector<unsigned char> bytes;
        bytes.resize(num);
        for (int i = 0; i < num; ++i) {
            ReadProcessMemory(process_handle, (LPVOID) (address + i), &bytes[i], (SIZE_T)1, nullptr);
        }
        return bytes;
    }

    [[nodiscard]] std::wstring readWstring(unsigned long long address, short size = 1) const {
        std::wstring result = L"";
        wchar_t character;
        for (int i = 0; i < size; ++i) {
            ReadProcessMemory(process_handle, (LPVOID)(address + i*2), &character, (SIZE_T) 2, nullptr);
            if (character == '\0') return result;
            result += character;
        }
        return result;
    }

    [[nodiscard]] std::string readString(unsigned long long address, short length, short char_size = 1) const {
        std::string result = "";
        char character;
        for (int i = 0; i < length; ++i) {
            ReadProcessMemory(process_handle, (LPVOID)(address + i*char_size), &character, (SIZE_T) 1, nullptr);
            if (character == '\0') return result;
            result += character;
        }
        return result;
    }
};