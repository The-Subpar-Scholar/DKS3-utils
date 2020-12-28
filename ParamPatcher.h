#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <map>
#include <cstddef>
#include <utility>

static std::map<std::string, unsigned long long> param_table;
Process DS3 = Process(nullptr);

class ParamPatcher{

    private:

        static std::map<unsigned int, unsigned long long> get_id_table(std::string param) {
            std::map<unsigned int, unsigned long long> id_table;
            unsigned long long addr = DS3.readQWORD(DS3.readQWORD(param_table[param]+0x68)+0x68);
            //std::cout<<addr<<std::endl;
            for (int i = 0; i < DS3.readWORD(addr + 0xA)-1; ++i) {
                //printf("%d | %IllX\n", DS3.readDWORD(addr + 0x40 + 0x18 * i),DS3.readDWORD(addr + 0x48 + 0x18 * i) + addr);
                id_table[DS3.readDWORD(addr + 0x40 + 0x18 * i)] = DS3.readDWORD(addr + 0x48 + 0x18 * i) + addr;
            }
            return id_table;
        }

    public:

        static void initialize(Process game) {
            DS3 = std::move(game);
            unsigned long long start = DS3.readQWORD(DS3.readQWORD(0x144785FE0)+0x10);
            unsigned long long end = DS3.readQWORD(DS3.readQWORD(0x144785FE0)+0x18);
            unsigned short count = (end - start)/8;
            std::string param_name;
            for (int i = 0; i < count ; ++i) {
                unsigned long long param_addr = DS3.readQWORD(start + (i * 8));
                if (DS3.readDWORD(param_addr + 0x20) > 7) {
                    param_name = DS3.readString(DS3.readQWORD(param_addr + 0x10),25,2);
                } else {
                    param_name = DS3.readString(param_addr + 0x10, 25,2);
                }
                param_table[param_name] = param_addr;//.erase(param_name.find("\0"),param_name.size()-param_name.find("\0")-1)
            }
        }

        static void enumerate_params() {
            for (const auto& param : param_table) {
                std::printf("%s | address is [0x%IllX]\n", param.first.c_str(), param.second);
            }
        }

        static void enumerate_id_table(std::string param) {
            for (const auto param : get_id_table(param)) {
                std::printf ("[id] %d's address is : [0x%IllX]\n", param.first, param.second);
            }
        }

        struct Patch {

            std::map<unsigned int, unsigned long long> id_table;
            unsigned long long patch_addr;
            unsigned long long all_patch_addr;

            std::map<unsigned long long,std::vector<unsigned char>> changes;

            unsigned long long getParamOffsetAddress(unsigned short offset) {
                return patch_addr + offset;
            }

            void patch4Byte(unsigned short offset, int value) {
                if (changes.find(offset) == changes.end()) {
                    changes[patch_addr + offset] = DS3.readBytes(patch_addr + offset, 4);
                }
                DS3.writeDWORD(patch_addr + offset, value);
            }

            void patch2Byte(unsigned short offset, short value) {
                if (changes.find(offset) == changes.end()) {
                    changes[patch_addr + offset] = DS3.readBytes(patch_addr + offset, 2);
                }
                DS3.writeWORD(patch_addr + offset, value);
            }

            void patchByte(unsigned short offset, char value) {
                if (changes.find(offset) == changes.end()) {
                    changes[patch_addr + offset] = DS3.readBytes(patch_addr + offset, 1);
                }
                DS3.writeCHAR(patch_addr + offset, value);
            }

            void patchFloat(unsigned short offset, float value) {
                if (changes.find(offset) == changes.end()) {
                    changes[patch_addr + offset] = DS3.readBytes(patch_addr + offset, 4);
                }
                DS3.writeFloat(patch_addr + offset, value);
            }

            void patchAll4Byte(unsigned short offset, int value) {
                for (auto pair : id_table) {
                    if (changes.find(offset) == changes.end()) {
                        changes[pair.second + offset] = DS3.readBytes(pair.second + offset, 4);
                    }
                    DS3.writeDWORD(pair.second + offset, value);
                }
            }

            void patchAll2Byte(unsigned short offset, short value) {
                for (auto pair : id_table) {
                    if (changes.find(offset) == changes.end()) {
                        changes[pair.second + offset] = DS3.readBytes(pair.second + offset, 2);
                    }
                    DS3.writeWORD(pair.second + offset, value);
                }
            }

            void patchAllByte(unsigned short offset, char value) {
                for (auto pair : id_table) {
                    if (changes.find(offset) == changes.end()) {
                        changes[pair.second + offset] = DS3.readBytes(pair.second + offset, 1);
                    }
                    DS3.writeCHAR(pair.second + offset, value);
                }
            }

            void patchAllFloat(unsigned short offset, float value) {
                for (auto pair : id_table) {
                    if (changes.find(offset) == changes.end()) {
                        changes[pair.second + offset] = DS3.readBytes(pair.second + offset, 4);
                    }
                    DS3.writeFloat(pair.second + offset, value);
                }
            }

            void restore() {
                for (const auto change : changes) {
                    DS3.writeBytes(change.first, change.second);
                }
            }

            void enumerate_changes() {
                for (const auto change : changes) {
                    printf("Address : %IllX | Original Bytes : ", change.first);
                    for (unsigned char byte : change.second) {
                        printf("%02X ", byte);
                    }
                    std::cout << '\n';
                }
            }

            Patch(std::string param, int id = -1) {
                id_table = ParamPatcher::get_id_table(param);
                all_patch_addr = param_table[param];
                if (id != -1) {
                    patch_addr = id_table[id];
                }
            }

        };

};