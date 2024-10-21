
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "basetypes.h"
#include "opcode.h"
#include "cpu.h"
#include "disassm.h"
#include "labels.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::filesystem::path;
using std::filesystem::exists;
using std::filesystem::file_size;
using std::ifstream;
using std::ios;

using sim86::bstring;
using sim86::pOpCode;
using sim86::OpCode;
using sim86::print20;
using sim86::Disassm;

const ssize_t MAX_BIN_FILE_SIZE{1<<20};
void usage(){
    cout << "Usage: cpurun -b binfile -l loaded_address" << endl;
}

int main(int argc, char *argv[]) {
    int ret = 0;
    int help = 0, verbose = 0;
    int c;
    int32_t rom_loaded_at = -1;
    int32_t start = 0;
    int bios = 0;
    string binfile;

    struct option opts[] = {
        {
            "help",
            no_argument,
            &help,
            1
        },
        {
            "bios",
            no_argument,
            &bios,
            1
        },
        {
            "verbose",
            required_argument,
            0,
            'v'
        },
        {
            "bin-file",
            required_argument,
            0,
            'b'
        },
        {
            "bin-loaded-at",
            required_argument,
            0,
            'l'
        },
        {
            "start-addr",
            required_argument,
            0,
            's'
        },
        {0, 0, 0, 0}
    };


    while (1) {   
        int option_index = 0;

        c = getopt_long (argc, argv, "hBv:b:l:s:",
                       opts, &option_index); 
        if (c == -1) {
            break;
        }                  
        switch (c) {
            case 0:
                if (opts[option_index].flag != 0)
                    break;
                cout << "option " << opts[option_index].name << endl;
                if (optarg)
                    cout << " with arg " << optarg;
                cout << endl;
                break;
            case 'v':
                verbose = atoi(optarg);
                if (!verbose) {
                    usage();
                    exit(1);
                }
            break;
            case 'l':
                rom_loaded_at = atoi(optarg);
                if (rom_loaded_at <= 0) {
                    usage();
                    exit(1);
                }
            break;
            case 'b':
                binfile = optarg;
            break;
            case 'B':
                bios = 1;
                break;
            case 's':
                start = atoi(optarg);
                if (start < 0) {
                    usage();
                    exit(1);
                }
            break;    
            default:
                usage();
                exit(1);
        }

    }
    if (help) {
        usage();
        exit(0);
    }
    
    path flocation(binfile);

    if (!exists(flocation)) {
        cerr << "Binfile " << binfile << " does not exists." << endl;
        exit(1);
    }
    auto fsize = file_size(flocation);
    if (fsize > MAX_BIN_FILE_SIZE) {
        cerr << "Only file up to " << (uint32_t) (MAX_BIN_FILE_SIZE) << "are proccessed" << endl;
        exit(1);
    }
    std::cout << "bios=" << bios << std::endl;
    size_t offset = 0;
    if (bios && rom_loaded_at == -1) {
        rom_loaded_at = 0x100000 - fsize;
        if (!start) {
            start = 0x100000 - 16;
        }
    }
    offset = start - rom_loaded_at;
    std::cout << "ROM assumed loaded at ";
    print20(std::cout, rom_loaded_at);
    std::cout << " start address at ";
    print20(std::cout, start);
    std::cout << " offset=";
    print20(std::cout, offset);
    std::cout << std::endl;
    
   

    ifstream in{binfile, ios::binary};
    sim86::ROM rom(rom_loaded_at);
    rom << in;
    cout << "Bin size=" << rom.size() << endl << endl;
    //cout << rom;

    sim86::CPU_8086 cpu;
    //auto pCode = std::shared_ptr<OpCode>(new OpCode(bstring{0xEA, 0x5B, 0xE0, 0xDD, 0xF0}));
    //std::cout << *pCode << std::endl;




    sim86::Disassm dis;
    sim86::LabelMap labels;
    try {
        while (1) {
            size_t old_offset = offset;
            auto pCode = dis.decode(rom, offset);
            if (!rom.was_visited(old_offset, offset - old_offset)){
                rom.mark_bits(old_offset, offset - old_offset);
            }

            //std::cout << "Decoded :"; 
            pCode->print(std::cout);
            std::cout << "; offset=";
            print20(std::cout, offset);
            std::cout << std::endl;
            size_t next_offset = rom.get_next_offset(pCode, offset);
            if (next_offset != offset) {
                labels.get_label(next_offset);
            }
            //TODO - adjust for subroutines
            offset = next_offset;
            std::cout << "\tNext offset=";
            print20(std::cout, offset);
            std::cout << std::endl;
        }
    } catch (std::invalid_argument & e) {
        std::cout << e.what();
    } catch (std::out_of_range &e) {
        std::cout << e.what();
    } catch (sim86::unimplemented_exception &e) {
        std::cout << e.what();
    }
    std::cout << std::endl;
    return ret;
}