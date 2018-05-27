#include "base-io.h"
#include "gtest/gtest.h"
#include <memory>

namespace utaha {

TEST(BaseIoTest, Sanity) {
    std::unique_ptr<FileTextInputStream> f(FSUtils::OpenTextFileRead("tests/grid/test1.index"));

    char file[FILENAME_MAX];
    int n1, n2;
    f->Scanf("%s\t%d\t%d\n", file, &n1, &n2);

    ASSERT_EQ(25, n1);
    ASSERT_EQ(25, n2);
    ASSERT_STRCASEEQ("[unknown]", file);
}

}