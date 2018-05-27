#include "base-io.h"
#include "lazy-instance.h"
#include "glog/logging.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>

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
        int bytes = vfprintf(f_, fmt, ap);
        va_end(ap);
        return bytes;
    }

private:
    FILE *f_;
}; // class FileTextOutputStreamImpl

class UnixFileIterator : public FileIterator {
public:
    UnixFileIterator(const std::string &dir) : dir_(dir) {}

    virtual ~UnixFileIterator() {
        if (handle_) {
            closedir(handle_);
            handle_ = nullptr;
        }
    }

    virtual void Reset() override {
        if (handle_) {
            closedir(handle_);
            handle_ = nullptr;
        }
        handle_ = opendir(dir_.c_str());
        if (handle_) {
            ent_ = readdir(handle_);
        } else {
            ent_ = nullptr;
        }
    }

    virtual bool HasNext() override {
        return ent_ != nullptr;
    }

    virtual void Next() override {
        ent_ = readdir(handle_);
    }

    virtual std::string dir() override { return dir_; }

    virtual std::string name() override {
        DCHECK_NOTNULL(ent_);
        return ent_->d_name;
    }

    virtual bool is_dir() override {
        DCHECK_NOTNULL(ent_);
        return ent_->d_type & DT_DIR;
    }

    virtual bool is_link() override {
        DCHECK_NOTNULL(ent_);
        return ent_->d_type & DT_LNK;
    }

    virtual bool is_hidden() override {
        DCHECK_NOTNULL(ent_);
        return ent_->d_name[0] == '.';
    }

    virtual bool exist() override { return true; }

    DISALLOW_IMPLICIT_CONSTRUCTORS(UnixFileIterator);
private:
    std::string dir_;
    DIR *handle_ = nullptr;
    struct dirent* ent_ = nullptr;
}; // class UnixFileIterator;


class UnixFileEntry : public FileEntry {
public:
    UnixFileEntry(const std::string &name) {
        exist_ = (::stat(name.c_str(), &stat_) == 0);
        if (exist_) {
            ParseFileName(name);
        }
    }

    virtual ~UnixFileEntry() {}

    virtual std::string dir() override {
        DCHECK(exist());
        return dir_;
    }

    virtual std::string name() override {
        DCHECK(exist());
        return name_;
    }

    virtual bool is_dir() override {
        DCHECK(exist());
        return stat_.st_mode & S_IFDIR;
    }

    virtual bool is_link() override {
        DCHECK(exist());
        return stat_.st_mode & S_IFLNK;
    }

    virtual bool is_hidden() override {
        DCHECK(exist());
        return name_ != "." && name_ != ".." && name_[0] == '.';
    }

    virtual bool exist() override {
        return exist_;
    }

private:
    void ParseFileName(const std::string &name) {
        char buf[FILENAME_MAX];
        char *work_dir = getcwd(buf, arraysize(buf));
        if (!work_dir) {
            PLOG(ERROR) << "Can not get work dir.";
            exist_ = false;
        }

        size_t p = name.rfind("/");
        if (p == std::string::npos) {
            dir_.assign(work_dir);
            name_.assign(name);
        } else {
            dir_.append(work_dir).append("/").append(name.substr(0, p));
            name_.assign(name.substr(p + 1));
        }
    }

    std::string dir_;
    std::string name_;
    bool exist_ = false;
    struct stat stat_;
}; // class UnixFileEntry

class UnixFileSystem : public Original {
public:
    UnixFileSystem() = default;

    virtual ~UnixFileSystem() {}

    virtual std::string name() override {
        return "unix-filesystem";
    }

    virtual FileTextInputStream *
    OpenTextFileRd(const std::string &file) override {
        FILE *f = fopen(file.c_str(), "rb");
        if (!f) {
            PLOG(ERROR) << "Can not open file: " << file;
            return nullptr;
        }
        return new FileTextInputStreamImpl(f);
    }

    virtual FileTextOutputStream *
    OpenTextFileWr(const std::string &file) override {
        FILE *f = fopen(file.c_str(), "wb");
        if (!f) {
            PLOG(ERROR) << "Can not open file: " << file;
            return nullptr;
        }
        return new FileTextOutputStreamImpl(f);
    }

    virtual bool FileExist(const std::string &file) override {
        return access(file.c_str(), F_OK) == 0;
    }

    virtual FileIterator *OpenDir(const std::string &dir) override {
        return new UnixFileIterator(dir);
    }

    virtual FileEntry *FileStat(const std::string &file) override {
        return new UnixFileEntry(file);
    }

private:

}; // class UnixFileSystem

LazyInstance<UnixFileSystem> unix_fs(LAZY_INSTANCE_INITIALIZER);

Original *GetFileSystem() {
    return unix_fs.ptr();
}

} // namespace utaha
