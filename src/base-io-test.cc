#include "base-io.h"
#include "gtest/gtest.h"
#include <memory>

namespace utaha {

TEST(BaseIoTest, Sanity) {
    std::unique_ptr<FileTextInputStream> f(GetFileSystem()->OpenTextFileRd("tests/grid/test1.index"));

    char file[FILENAME_MAX];
    int n1, n2;
    f->Scanf("%s\t%d\t%d\n", file, &n1, &n2);

//    ASSERT_EQ(25, n1);
//    ASSERT_EQ(25, n2);
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

}
