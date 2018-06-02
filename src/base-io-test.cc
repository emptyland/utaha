#include "base-io.h"
#include "gtest/gtest.h"
#include <memory>

namespace utaha {

TEST(BaseIoTest, Sanity) {
    std::unique_ptr<FileTextInputStream> f(GetFileSystem()->OpenTextFileRd("tests/grid/test1.index"));

    char file[FILENAME_MAX];
    int n1, n2;
    f->Scanf("%s\t%d\t%d\n", file, &n1, &n2);

    ASSERT_STRCASEEQ("[unknown]", file);
}

TEST(BaseIoTest, FileIterator) {
    std::unique_ptr<FileIterator> iter(GetFileSystem()->OpenDir("tests"));
    for (iter->Reset(); iter->HasNext(); iter->Next()) {
        ASSERT_FALSE(iter->name().empty());
    }
}

TEST(BaseIoTest, GetFileNames) {
    std::vector<std::string> files;
    int n = GetFileSystem()->GetFileNames("tests", &files);
    for (const auto &name : files) {
        ASSERT_NE(".", name);
        ASSERT_NE("..", name);
    }
    ASSERT_GT(n, 0);
}

TEST(BaseIoTest, GetFileStat) {
    std::unique_ptr<FileEntry> file(GetFileSystem()->FileStat("tests/grid"));
    ASSERT_TRUE(!!file);
    EXPECT_TRUE(file->is_dir());
    EXPECT_EQ("grid", file->name());
    printf("%s\n", file->dir().c_str());
}

TEST(BaseIoTest, Sprintf) {
    EXPECT_EQ("1/2/test.3", Original::sprintf("%d/%d/%s.%d", 1, 2, "test", 3));

    std::string large;
    large.resize(1024, 'A');

    auto format = Original::sprintf("%d/%s/%d", 100, large.c_str(), 111);
    EXPECT_EQ("100/" + large + "/111" , format);
}

#if defined(UTAHA_OS_WINDOWS) 
TEST(BaseIoTest, UnicodeConverting) {
    std::u16string k16(u"ºº×Ö£¡");
    auto u8 = Original::UTF16ToUTF8(k16);
    ASSERT_NE(0, u8.size());
    ASSERT_EQ(9, u8.size());

    auto u16 = Original::UTF8toUTF16(u8);
    ASSERT_EQ(k16.size(), u16.size());
    ASSERT_EQ(k16, u16);
}
#endif

}
