#ifndef FILE_H
#define FILE_H

#include <fstream>


class File
{
public:

    enum w_r { WRITE, READ } mode;
    
    std::ofstream out_file;
    std::ifstream in_file;

    File(w_r m, const std::string& name);

    bool compare(File& file);


    ~File();

};





#endif // FILE_H
