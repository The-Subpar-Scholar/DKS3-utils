#include "Process.h"
#include "ParamPatcher.h"

Process Param::DKS3 = Process("");

std::map<std::string, unsigned long long> Param::param_table;

std::map<unsigned long long, std::vector<unsigned char>> Param::all_change_list;

std::map<std::string,std::map<unsigned int, unsigned long long>> Param::all_id_table;

std::map<unsigned int, unsigned long long> Param::get_id_table(const std::string& param) {
    std::map<unsigned int, unsigned long long> id_table;
    unsigned long long addr = DKS3.readQWORD(DKS3.readQWORD(param_table[param]+0x68)+0x68);
    for (int i = 0; i < DKS3.readWORD(addr + 0xA); ++i) {
        id_table[DKS3.readDWORD(addr + 0x40 + 0x18 * i)] = DKS3.readDWORD(addr + 0x48 + 0x18 * i) + addr;
    }
    return id_table;
}

void Param::initialize(Process game) {
    DKS3 = game;
    unsigned long long start = DKS3.readQWORD(DKS3.readQWORD(0x144785FE0)+0x10);
    unsigned long long end = DKS3.readQWORD(DKS3.readQWORD(0x144785FE0)+0x18);
    unsigned short count = (end - start)/8;
    std::string param_name;
    for (int i = 0; i < count ; ++i) {
        unsigned long long param_addr = DKS3.readQWORD(start + (i * 8));
        if (DKS3.readDWORD(param_addr + 0x20) > 7) {
            param_name = DKS3.readString(DKS3.readQWORD(param_addr + 0x10),25,2);
        } else {
            param_name = DKS3.readString(param_addr + 0x10, 25,2);
        }
        param_table[param_name] = param_addr;
    }
}

void Param::enumerate_params() {
    for (const auto& param : param_table) {
        std::printf("%s | address is [0x%IllX]\n", param.first.c_str(), param.second);
    }
}

void Param::enumerate_id_table(const std::string& param) {
    for (const auto& param : get_id_table(param)){
        std::printf ("[id] %d's address is : [0x%IllX]\n", param.first, param.second);
    }
}

Param::Patch::Patch(const std::string& param, int id) {
    Param::all_id_table[param] = Param::get_id_table(param);
    param_name = param;
    if (id != -1) {
        patch_addr = Param::all_id_table[param][id];
    }
}

void Param::Patch::log_change(unsigned long long address, unsigned int size) {
    if (all_change_list.find(address) == all_change_list.end()) {
        all_change_list[address] = DKS3.readBytes(address, (SIZE_T)size);
    }
    changes[address] = all_change_list[address];
}
void Param::Patch::patch4Byte(unsigned int offset, int value) {
    log_change(patch_addr + offset, 4);
    DKS3.writeWORD(patch_addr + offset, value);
}

void Param::Patch::patch2Byte(unsigned int offset, short value) {
    log_change(patch_addr + offset, 2);
    DKS3.writeWORD(patch_addr + offset, value);
}

void Param::Patch::patchByte(unsigned int offset, char value) {
    log_change(patch_addr + offset, 1);
    DKS3.writeWORD(patch_addr + offset, value);
}

void Param::Patch::patchFloat(unsigned int offset, float value) {
    log_change(patch_addr + offset, 4);
    DKS3.writeWORD(patch_addr + offset, value);
}

void Param::Patch::patchAll4Byte(unsigned int offset, int value) {
    for (const auto& pair : all_id_table[param_name]) {
        log_change(pair.second + offset, 4);
        DKS3.writeDWORD(pair.second + offset, value);
    }
}

void Param::Patch::patchAll2Byte(unsigned int offset, short value) {
    for (const auto& pair : all_id_table[param_name]) {
        log_change(pair.second + offset, 4);
        DKS3.writeWORD(pair.second + offset, value);
    }
}

void Param::Patch::patchAllByte(unsigned int offset, char value) {
    for (const auto& pair : all_id_table[param_name]) {
        log_change(pair.second + offset, 1);
        DKS3.writeCHAR(pair.second + offset, value);
    }
}

void Param::Patch::patchAllFloat(unsigned int offset, float value) {
    for (const auto& pair : all_id_table[param_name]) {
        log_change(pair.second + offset, 4);
        DKS3.writeDWORD(pair.second + offset, value);
    }
}

void Param::Patch::enumerate_changes() {
    for (const auto& change : changes) {
        printf("Address : %IllX | Original Bytes : ", change.first);
        for (unsigned char byte : change.second) {
            printf("%02X ", byte);
        }
        std::cout << '\n';
    }
}

void Param::Patch::restore() {
    for (const auto& change : changes) {
        DKS3.writeBytes(change.first, change.second);
    }
}