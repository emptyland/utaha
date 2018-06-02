#ifndef UTAHA_BASE_IO_H_
#define UTAHA_BASE_IO_H_

#include "base.h"
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <string>

namespace utaha {

class FileTextInputStream;
class FileTextOutputStream;
class FileBinaryInputStream;
class FileBinaryOutputStream;
class FileIterator;
class FileEntry;

class Original {
public:
    Original();
    virtual ~Original();

    virtual std::string name() = 0;

    virtual FileTextInputStream *OpenTextFileRd(const std::string &file) = 0;
    virtual FileTextOutputStream *OpenTextFileWr(const std::string &file) = 0;

    virtual FileBinaryInputStream *
    OpenBinaryFileRd(const std::string &file) = 0;
    virtual FileBinaryOutputStream *
    OpenBinaryFileWr(const std::string &file) = 0;

    virtual FileIterator *OpenDir(const std::string &dir) = 0;

    inline int GetFileNames(const std::string &dir,
                            std::vector<std::string> *names);

    virtual FileEntry *FileStat(const std::string &file) = 0;

    virtual bool FileExist(const std::string &file) = 0;

    bool FileNotExist(const std::string &file) {
        return !FileExist(file);
    }

    static inline std::string vsprintf(const char *fmt, va_list ap);

    static inline std::string sprintf(const char *fmt, ...);

    static std::u16string UTF8toUTF16(const std::string &input);

    static inline std::wstring UTF8toUTF16W(const std::string &input) {
        auto u16 = UTF8toUTF16(input);
        return std::wstring(
            reinterpret_cast<const wchar_t *>(u16.data()),
            u16.size());
    }

    static std::string UTF16ToUTF8(const std::u16string &input);

    static inline std::string UTF16WtoUTF8(const std::wstring &input) {
        return UTF16ToUTF8(std::u16string(
            reinterpret_cast<const char16_t *>(input.data()),
            input.size()));
    }

}; // class Original

class FileTextInputStream {
public:
    FileTextInputStream();
    virtual ~FileTextInputStream();

    virtual int Scanf(const char *fmt, ...) = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileTextInputStream);
}; // class FileTextInputStream


class FileTextOutputStream {
public:
    FileTextOutputStream();
    virtual ~FileTextOutputStream();

    virtual int Printf(const char *fmt, ...) = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileTextOutputStream);
}; // class FileTextOutputStream


class FileBinaryInputStream {
public:
    FileBinaryInputStream();
    virtual ~FileBinaryInputStream();

    virtual size_t Read(void *data, size_t n) = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileBinaryInputStream);
}; // class FileBinaryInputStream


class FileBinaryOutputStream {
public:
    FileBinaryOutputStream();
    virtual ~FileBinaryOutputStream();

    virtual size_t Write(const void *data, size_t n) = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileBinaryOutputStream);
}; // class FileBinaryOutputStream


class FileEntry {
public:
    FileEntry();
    virtual ~FileEntry();

    virtual std::string dir() = 0;
    virtual std::string name() = 0;
    virtual bool is_dir() = 0;
    virtual bool is_link() = 0;
    virtual bool is_hidden() = 0;
    virtual bool exist() = 0;

#if defined(UTAHA_OS_WINDOWS)
    std::string path() { return dir() + "\\" + name(); }
#else
    std::string path() { return dir() + "/" + name(); }
#endif

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileEntry);
}; // class FileEntry


// for (iter->Rest(); iter->HasNext(); iter->Next()) {...}
class FileIterator : public FileEntry {
public:
    FileIterator();
    virtual ~FileIterator();

    virtual void Reset() = 0;
    virtual bool HasNext() = 0;
    virtual void Next() = 0;

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileIterator);
}; // class FileIterator;

Original *GetFileSystem();

inline int Original::GetFileNames(const std::string &dir,
                                  std::vector<std::string> *names) {
    names->clear();
    std::unique_ptr<FileIterator> iter(OpenDir(dir));
    for (iter->Reset(); iter->HasNext(); iter->Next()) {
        if (iter->name() != "." && iter->name() != "..") {
            names->push_back(iter->name());
        }
    }
    return static_cast<int>(names->size());
}

/*static*/ inline std::string Original::vsprintf(const char *fmt, va_list ap) {
    va_list copied;
    int len = 0, rv = len;
    std::string result;
    do {
        len = rv + 128;
        result.resize(len, 0);
        va_copy(copied, ap);
        rv = vsnprintf(&result[0], len, fmt, ap);
        va_copy(ap, copied);
    } while (rv > len);
    result.resize(rv);
    return result;
}

/*static*/ inline std::string Original::sprintf(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    std::string result = Original::vsprintf(fmt, ap);
    va_end(ap);
    return result;
}

} // namespace utaha 

#endif // UTAHA_BASE_IO_H_
