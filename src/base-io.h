#ifndef UTAHA_BASE_IO_H_
#define UTAHA_BASE_IO_H_

#include "base.h"
#include <stdio.h>
#include <vector>
#include <string>

namespace utaha {

class FileTextInputStream;
class FileTextOutputStream;
class FileIterator;
class FileEntry;

class Original {
public:
    Original();
    virtual ~Original();

    virtual std::string name() = 0;

    virtual FileTextInputStream *OpenTextFileRd(const std::string &file) = 0;
    virtual FileTextOutputStream *OpenTextFileWr(const std::string &file) = 0;

    virtual FileIterator *OpenDir(const std::string &dir) = 0;

    inline int GetFileNames(const std::string &dir,
                            std::vector<std::string> *names);

    virtual FileEntry *FileStat(const std::string &file) = 0;

    virtual bool FileExist(const std::string &file) = 0;

    bool FileNotExist(const std::string &file) {
        return !FileExist(file);
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

    std::string path() { return dir() + "/" + name(); }

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

} // namespace utaha 

#endif // UTAHA_BASE_IO_H_
