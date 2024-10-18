
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
#include "cpu.h"

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

const ssize_t MAX_BIN_FILE_SIZE{1<<20};
void usage(){
    cout << "Usage: TODO" << endl;
}

int main(int argc, char *argv[]) {
    int ret = 0;
    int help = 0, verbose = 0;
    int c;
    int32_t rom_loaded_at = 0;

    string binfile;

    struct option opts[] = {
        {
            "help",
            no_argument,
            &help,
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
        {0, 0, 0, 0}
    };


    while (1) {   
        int option_index = 0;

        c = getopt_long (argc, argv, "hv:b:l:",
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
    ifstream in{binfile, ios::binary};
    sim86::ROM rom(rom_loaded_at);
    rom << in;
    cout << "Bin size=" << rom.size() << endl;
    //cout << rom;

    sim86::CPU_8086 cpu;

    return ret;
}