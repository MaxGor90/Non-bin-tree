#include "file.h"
#include <iostream>

    File::File(w_r m, const std::string& name)
        : mode {m}
    {
        switch (mode)
        {
        case WRITE:
        {
            out_file.open(name, std::ios::trunc | std::ios::binary);
            if (!out_file)
            {
                // Print an error and exit
                std::cerr << name << "  could not be opened for writing!\n";
            }
            break;
        }
        case READ:
        {
            in_file.open(name, std::ios::binary);
            if (!in_file)
            {
                // Print an error and exit
                std::cerr << name << "  could not be opened for reading!\n";
            }
            break;
        }
        }
    }


    File::~File()
    {
        if (out_file.is_open())
            out_file.close();

        if (in_file.is_open())
            in_file.close();
    }
