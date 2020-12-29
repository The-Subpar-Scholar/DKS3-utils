#ifndef PARAM_PATCHER_H
#define PARAM_PATCHER_H

#include <string>
#include <map>
#include "Process.h"



namespace Param {

    extern Process DKS3;

    extern std::map<std::string, unsigned long long> param_table;

    extern std::map<unsigned long long, std::vector<unsigned char>> all_change_list;

    extern std::map<std::string, std::map<unsigned int, unsigned long long>> all_id_table;

    std::map<unsigned int, unsigned long long> get_id_table(const std::string &param);

    void initialize(Process Game);

    void enumerate_params();

    void enumerate_id_table(const std::string& param);

    class Patch {

        unsigned long long patch_addr;
        std::string param_name;
        std::map<unsigned long long, std::vector<unsigned char>> changes;
        void log_change(unsigned long long address, unsigned int size);
    public:

        void restore();

        void patch4Byte(unsigned int offset, int value);
        void patch2Byte(unsigned int offset, short value);
        void patchByte(unsigned int offset, char value);
        void patchFloat(unsigned int offset, float value);

        void patchAll4Byte(unsigned int offset, int value);
        void patchAll2Byte(unsigned int offset, short value);
        void patchAllByte(unsigned int offset, char value);
        void patchAllFloat(unsigned int offset, float value);

        void enumerate_changes();

        explicit Patch(const std::string &param, int id = -1);
    };
}
#endif //PARAMPATCHER_H
