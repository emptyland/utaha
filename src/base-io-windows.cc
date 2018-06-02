#include "base-io.h"
#include "glog/logging.h"
#include "lazy-instance.h"
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


class FileBinaryInputStreamImpl : public FileBinaryInputStream {
public:
    FileBinaryInputStreamImpl(FILE *f) : f_(DCHECK_NOTNULL(f)) {}

    virtual ~FileBinaryInputStreamImpl() {
        fclose(f_);
        f_ = nullptr;
    }

    virtual size_t Read(void *data, size_t n) override {
        return fread_s(data, n, 1, n, f_);
    }

private:
    FILE * f_;
}; // class FileBinaryInputStreamImpl


class FileBinaryOutputStreamImpl : public FileBinaryOutputStream {
public:
    FileBinaryOutputStreamImpl(FILE *f) : f_(DCHECK_NOTNULL(f)) {}

    virtual ~FileBinaryOutputStreamImpl() {
        fclose(f_);
        f_ = nullptr;
    }

    virtual size_t Write(const void *data, size_t n) override {
        return fwrite(data, 1, n, f_);
    }

private:
    FILE * f_;
}; // class FileBinaryInputStreamImpl


class FileIteratorImpl : public FileIterator {
public:
    FileIteratorImpl(const std::string &dir)
        : dir_(dir) {
        memset(&find_data_, 0, sizeof(find_data_));
    }

    virtual ~FileIteratorImpl() {
        if (find_ != INVALID_HANDLE_VALUE) {
            FindClose(find_);
            find_ = INVALID_HANDLE_VALUE;
        }
    }

    virtual void Reset() override {
        find_ = FindFirstFileW(Original::UTF8toUTF16W(dir_).c_str(),
                               &find_data_);
    }

    virtual bool HasNext() override {
        return find_ != INVALID_HANDLE_VALUE;
    }

    virtual void Next() override {
        if (!FindNextFileW(find_, &find_data_)) {
            FindClose(find_);
            find_ = INVALID_HANDLE_VALUE;
        }
    }

    virtual std::string dir() override {
        return dir_;
    }

    virtual std::string name() override {
        DCHECK_NE(find_, INVALID_HANDLE_VALUE);
        return Original::UTF16WtoUTF8(find_data_.cFileName);
    }

    virtual bool is_dir() override {
        DCHECK_NE(find_, INVALID_HANDLE_VALUE);
        return find_data_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    }

    virtual bool is_link() override { 
        DCHECK_NE(find_, INVALID_HANDLE_VALUE);
        return find_data_.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT;
    }

    virtual bool is_hidden() override {
        DCHECK_NE(find_, INVALID_HANDLE_VALUE);
        return find_data_.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
    }

    virtual bool exist() override {
        return find_ != INVALID_HANDLE_VALUE;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileIteratorImpl);
private:
    std::string dir_;
    HANDLE find_ = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAW find_data_;
}; // class FileIteratorImpl


class FileEntryImpl : public FileEntry {
public:
    FileEntryImpl(const std::string &file, DWORD file_attr, bool has_exist)
        : file_attr_(file_attr)
        , exist_(has_exist) {
        ParseFileName(file);
    }
    virtual ~FileEntryImpl() = default;

    virtual std::string dir() override {
        return dir_;
    }

    virtual std::string name() override {
        return name_;
    }

    virtual bool is_dir() override {
        DCHECK(exist());
        return file_attr_ & FILE_ATTRIBUTE_DIRECTORY;
    }

    virtual bool is_link() override {
        DCHECK(exist());
        return file_attr_ & FILE_ATTRIBUTE_REPARSE_POINT;
    }

    virtual bool is_hidden() override {
        DCHECK(exist());
        return file_attr_ & FILE_ATTRIBUTE_HIDDEN;
    }

    virtual bool exist() {
        return exist_;
    }

    DISALLOW_IMPLICIT_CONSTRUCTORS(FileEntryImpl);
private:
    void ParseFileName(const std::string &file) {
        wchar_t path[MAX_PATH];
        auto size = GetFullPathNameW(Original::UTF8toUTF16W(file).c_str(),
                                     arraysize(path), path, nullptr);
        if (size == 0) {
            LOG(ERROR) << "GetFullPathNameW() return zero." << GetLastError();
            return;
        }

        std::wstring dir, name;
        for (int i = size - 1; i >= 0; --i) {
            if (i != size - 1 && path[i] == '\\') {
                name = path + i + 1;
                dir = std::wstring(path, i);
                break;
            }
        }
        dir_  = Original::UTF16WtoUTF8(dir);
        name_ = Original::UTF16WtoUTF8(name);
    }

    std::string dir_;
    std::string name_;
    DWORD file_attr_;
    bool exist_;
}; // class FileEntryImpl


class WindowsFileSystem : public Original {
public:
    WindowsFileSystem() = default;
    virtual ~WindowsFileSystem() {}

    virtual std::string name() override { return "windows-fs"; }

    virtual FileTextInputStream *
    OpenTextFileRd(const std::string &file) override {
        FILE *f = nullptr;
        auto err = _wfopen_s(&f, UTF8toUTF16W(file).c_str(), L"rb");
        if (err != 0) {
            LOG(ERROR) << "Can not open file for reading. " << file;
            return nullptr;
        }
        return new FileTextInputStreamImpl(f);
    }

    virtual FileTextOutputStream *
    OpenTextFileWr(const std::string &file) override {
        FILE *f = nullptr;
        auto err = _wfopen_s(&f, UTF8toUTF16W(file).c_str(), L"wb");
        if (err != 0) {
            LOG(ERROR) << "Can not open file for writting. " << file;
            return nullptr;
        }
        return new FileTextOutputStreamImpl(f);
    }

    virtual bool FileExist(const std::string &file) override {
        WIN32_FIND_DATAW wfd;
        HANDLE find = FindFirstFileW(UTF8toUTF16W(file).c_str(), &wfd);
        return find != INVALID_HANDLE_VALUE;
    }

    virtual FileBinaryInputStream *
        OpenBinaryFileRd(const std::string &file) override {
        FILE *f = nullptr;
        auto err = _wfopen_s(&f, UTF8toUTF16W(file).c_str(), L"rb");
        if (err != 0) {
            LOG(ERROR) << "Can not open file for reading. " << file;
            return nullptr;
        }
        return new FileBinaryInputStreamImpl(f);
    }

    virtual FileBinaryOutputStream *
        OpenBinaryFileWr(const std::string &file) override {
        FILE *f = nullptr;
        auto err = _wfopen_s(&f, UTF8toUTF16W(file).c_str(), L"wb");
        if (err != 0) {
            LOG(ERROR) << "Can not open file for writting. " << file;
            return nullptr;
        }
        return new FileBinaryOutputStreamImpl(f);
    }

    virtual FileIterator *OpenDir(const std::string &dir) override {
        return new FileIteratorImpl(dir);
    }

    virtual FileEntry *FileStat(const std::string &file) override {
        auto attr = GetFileAttributesW(UTF8toUTF16W(file).c_str());
        if (attr == INVALID_FILE_ATTRIBUTES) {
            auto err = GetLastError();
            if (err == ERROR_NOT_FOUND) {
                return new FileEntryImpl(file, attr, false);
            }
        } else {
            return new FileEntryImpl(file, attr, true);
        }
        return nullptr;
    }

private:
    
}; // class WindowsFileSystem

/*static*/ std::u16string Original::UTF8toUTF16(const std::string &input) {
    auto size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, input.c_str(),
                                    static_cast<int>(input.size()), nullptr, 0);
    if (size == 0) {
        LOG(ERROR) << "Convert utf8 to utf16 make zero size." << GetLastError();
        return u"";
    }
    std::u16string utf16;
    utf16.resize(size);
    size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, input.c_str(),
                               static_cast<int>(input.size()),
                               reinterpret_cast<LPWCH>(&utf16[0]),
        static_cast<int>(utf16.size()) + 1);
    if (size == 0) {
        LOG(ERROR) << "Convert utf8 to utf16 fail." << input;
        return u"";
    }
    return utf16;
}

/*static*/ std::string Original::UTF16ToUTF8(const std::u16string &input) {
    auto size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                    reinterpret_cast<LPCWCH>(input.c_str()),
        static_cast<int>(input.size()), nullptr, 0, nullptr, nullptr);
    if (size == 0) {
        LOG(ERROR) << "Convert utf16 to utf8 make zero size." << GetLastError();
        return u8"";
    }

    std::string utf8;
    utf8.resize(size);
    size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                               reinterpret_cast<LPCWCH>(input.c_str()),
                               static_cast<int>(input.size()), &utf8[0],
                               static_cast<int>(utf8.size()) + 1,
                               nullptr, nullptr);
    if (size == 0) {
        LOG(ERROR) << "Convert utf16 to utf8 fail." << GetLastError();
        return u8"";
    }
    return utf8;
}

LazyInstance<WindowsFileSystem> windows_fs(LAZY_INSTANCE_INITIALIZER);

Original *GetFileSystem() { return windows_fs.ptr(); }

} // namespace utaha
