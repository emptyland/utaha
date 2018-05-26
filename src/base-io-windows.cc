#include "base-io.h"
#include "glog/logging.h"
#include <Windows.h>
#include <stdarg.h>

namespace utaha {

/*static*/
FileTextInputStream *FSUtils::OpenTextFileRead(const std::string &file) {
    FILE *f = nullptr;
    auto err = fopen_s(&f, file.c_str(), "rb");
    if (err != 0) {
        LOG(ERROR) << "Can not open file for reading. " << file;
        return nullptr;
    }
    return new FileTextInputStream(f);
}

/*static*/
FileTextOutputStream *FSUtils::OpenTextFileWrite(const std::string &file) {
    FILE *f = nullptr;
    auto err = fopen_s(&f, file.c_str(), "wb");
    if (err != 0) {
        LOG(ERROR) << "Can not open file for writting. " << file;
        return nullptr;
    }
    return new FileTextOutputStream(f);
}

/*static*/
bool FSUtils::FileExist(const std::string &file) {
    WIN32_FIND_DATAA wfd;
    HANDLE find = FindFirstFileA(file.c_str(), &wfd);
    return find != INVALID_HANDLE_VALUE;
}

///////////////////////////////////////////////////////////////////////////////
// class FileTextInputStream
///////////////////////////////////////////////////////////////////////////////


FileTextInputStream::FileTextInputStream(FILE *f)
    : f_(DCHECK_NOTNULL(f)) {
}

FileTextInputStream::~FileTextInputStream() {
    if (f_) {
        fclose(f_);
        f_ = nullptr;
    }
}

int FileTextInputStream::Scanf(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    int received = vfscanf_s(f_, fmt, ap);
    va_end(ap);

    return received;
}

///////////////////////////////////////////////////////////////////////////////
// class FileTextOutputStream
///////////////////////////////////////////////////////////////////////////////


FileTextOutputStream::FileTextOutputStream(FILE *f)
    : f_(DCHECK_NOTNULL(f)) {
}

FileTextOutputStream::~FileTextOutputStream() {
    if (f_) {
        fclose(f_);
        f_ = nullptr;
    }
}

int FileTextOutputStream::Printf(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    int bytes = vfprintf_s(f_, fmt, ap);
    va_end(ap);

    return bytes;
}

} // namespace utaha