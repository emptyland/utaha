#include "base-io.h"
#include "glog/logging.h"
#include <Windows.h>
#include <stdarg.h>

namespace utaha {

class FileTextInputStreamImpl : public FileTextInputStream {
public:
    FileTextInputStreamImpl(FILE *f) : f_(DCHECK_NOTNULL(f)) {}

    virtual ~FileTextInputStreamImpl() {
        fclose(f_);
        f_ = nullptr;
    }

    virtual int Scanf(const char *fmt, ...) override {
        va_list ap;
        va_start(ap, fmt);
        int received = vfscanf(f_, fmt, ap);
        va_end(ap);
        return received;
    }

private:
    FILE *f_;
}; // class FileTextInputStreamImpl


class FileTextOutputStreamImpl : public FileTextOutputStream {
public:
    FileTextOutputStreamImpl(FILE *f) : f_(DCHECK_NOTNULL(f)) {}

    virtual ~FileTextOutputStreamImpl() {
        fclose(f_);
        f_ = nullptr;
    }

    virtual int Printf(const char *fmt, ...) override {
        va_list ap;
        va_start(ap, fmt);
        int bytes = vfprintf_s(f_, fmt, ap);
        va_end(ap);
        return bytes;
    }

private:
    FILE *f_;
}; // class FileTextOutputStreamImpl


class WindowsFileSystem : public Original {
public:
    WindowsFileSystem() = default;
    virtual ~WindowsFileSystem() {}

    virtual std::string name() override { return "windows-fs"; }

    virtual FileTextInputStream *
    OpenTextFileRd(const std::string &file) override {
        FILE *f = nullptr;
        auto err = fopen_s(&f, file.c_str(), "rb");
        if (err != 0) {
            LOG(ERROR) << "Can not open file for reading. " << file;
            return nullptr;
        }
        return new FileTextInputStreamImpl(f);
    }

    virtual FileTextOutputStream *
    OpenTextFileWr(const std::string &file) override {
        FILE *f = nullptr;
        auto err = fopen_s(&f, file.c_str(), "wb");
        if (err != 0) {
            LOG(ERROR) << "Can not open file for writting. " << file;
            return nullptr;
        }
        return new FileTextOutputStreamImpl(f);
    }

    virtual bool FileExist(const std::string &file) override {
        WIN32_FIND_DATAA wfd;
        HANDLE find = FindFirstFileA(file.c_str(), &wfd);
        return find != INVALID_HANDLE_VALUE;
    }

private:
    
}; // class WindowsFileSystem

LazyInstance<WindowsFileSystem> windows_fs(LAZY_INSTANCE_INITIALIZER);

Original *GetFileSystem() {
    return windows_fs.ptr();
}

} // namespace utaha
