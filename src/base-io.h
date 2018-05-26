#ifndef UTAHA_BASE_IO_H_
#define UTAHA_BASE_IO_H_

#include "base.h"
#include <stdio.h>
#include <string>

namespace utaha {

class FileTextInputStream {
public:
    FileTextInputStream(FILE *f);
    ~FileTextInputStream();

    int Scanf(const char *fmt, ...);

    bool eof() const { feof(f_); }

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileTextInputStream);
private:
    FILE *f_;
}; // class FileTextInputStream


class FileTextOutputStream {
public:
    FileTextOutputStream(FILE *f);
    ~FileTextOutputStream();

    int Printf(const char *fmt, ...);

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileTextOutputStream);
private:
    FILE *f_ = nullptr;
}; // class FileTextOutputStream


class FSUtils {
public:
    static FileTextInputStream * OpenTextFileRead(const std::string &file);

    static FileTextOutputStream *OpenTextFileWrite(const std::string &file);

    static bool FileExist(const std::string &file);
}; // class FSUtils

} // namespace utaha 

#endif // UTAHA_BASE_IO_H_
