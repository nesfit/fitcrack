#include "MaskSplitter.h"

#include <gtest/gtest.h>

static const std::string g_testMaskBasic = "a??b?dc?hd?He??f";
static const std::string g_basicMaskCharsets[] = {
	"0123456789",
	"0123456789abcdef",
	"0123456789ABCDEF"
};
using CharsetsType = std::vector<std::string>;

TEST(Basic, ZeroOffsetAndKeyspace)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 0, 0);
	ASSERT_EQ(1, result.keyspace);
	ASSERT_EQ("a??b0c0d0e??f", result.mask);
	ASSERT_TRUE(result.customCharsets.empty());
}

TEST(Basic, Offset1)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 1, 0);
	ASSERT_EQ(1, result.keyspace);
	ASSERT_EQ("a??b1c0d0e??f", result.mask);
	ASSERT_TRUE(result.customCharsets.empty());
}

TEST(Basic, Keyspace2)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 0, 2);
	ASSERT_EQ(2, result.keyspace);
	ASSERT_EQ("a??b?1c0d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[0].substr(0, 2)}, result.customCharsets);
}

TEST(Basic, FirstCharsetTwice)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 0, 20);
	ASSERT_EQ(20, result.keyspace);
	ASSERT_EQ("a??b?dc?1d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[1].substr(0, 2)}, result.customCharsets);
}

TEST(Basic, FirstTwoCharsets)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 0, 160);
	ASSERT_EQ(160, result.keyspace);
	ASSERT_EQ("a??b?dc?hd0e??f", result.mask);
	ASSERT_EQ(CharsetsType{}, result.customCharsets);
}

TEST(Advanced, BreakOnFirstCharset)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 4, 160);
	ASSERT_EQ(6, result.keyspace);
	ASSERT_EQ("a??b?1c0d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[0].substr(4)}, result.customCharsets);
}

TEST(Advanced, BreakOnSecondCharset)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 20, 160);
	ASSERT_EQ(140, result.keyspace);
	ASSERT_EQ("a??b?dc?1d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[1].substr(2)}, result.customCharsets);
}

TEST(Advanced, BreakOnFirstCharsetHugeKeyspace)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 4, 1600000);
	ASSERT_EQ(6, result.keyspace);
	ASSERT_EQ("a??b?1c0d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[0].substr(4)}, result.customCharsets);
}

TEST(Advanced, BreakOnSecondCharsetHugeKeyspace)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 20, 1600000);
	ASSERT_EQ(140, result.keyspace);
	ASSERT_EQ("a??b?dc?1d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[1].substr(2)}, result.customCharsets);
}

TEST(Advanced, BreakOnFirstCharsetSecondPartlyDone)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 24, 1600000);
	ASSERT_EQ(6, result.keyspace);
	ASSERT_EQ("a??b?1c2d0e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[0].substr(4)}, result.customCharsets);
}

TEST(Advanced, BreakOnThirdCharset)
{
	MaskSplitter splitter;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 160*3, 9999999);
	ASSERT_EQ(160*(g_basicMaskCharsets[2].size()-3), result.keyspace);
	ASSERT_EQ("a??b?dc?hd?1e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[2].substr(3)}, result.customCharsets);
}

TEST(Advanced, NoSmallLeftovers1)
{
	MaskSplitter splitter;
	size_t thirdCharsetLeftoverKeyspace = g_basicMaskCharsets[2].size()-3;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 160*3, 160*thirdCharsetLeftoverKeyspace-10);
	ASSERT_EQ(160*thirdCharsetLeftoverKeyspace, result.keyspace);
	ASSERT_EQ("a??b?dc?hd?1e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[2].substr(3)}, result.customCharsets);
}

TEST(Advanced, NoSmallLeftovers2)
{
	MaskSplitter splitter;
	size_t thirdCharsetLeftoverKeyspace = g_basicMaskCharsets[2].size()-3;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, 160*3, 160*thirdCharsetLeftoverKeyspace*0.6);
	ASSERT_EQ(160*(thirdCharsetLeftoverKeyspace/2), result.keyspace);
	ASSERT_EQ("a??b?dc?hd?1e??f", result.mask);
	ASSERT_EQ(CharsetsType{g_basicMaskCharsets[2].substr(3, thirdCharsetLeftoverKeyspace/2)}, result.customCharsets);
}

TEST(CustomCharsets, Inclusion)
{
	MaskSplitter splitter({"123", "?1345"});
	auto result = splitter.GetMaskSlice("?2as??d?2", 15, 9999999);
	ASSERT_EQ(10, result.keyspace);
	ASSERT_EQ("?2as??d?3", result.mask);
	CharsetsType expectedCharsets{"123", "12345", "45"};
	ASSERT_EQ(expectedCharsets, result.customCharsets);
}

TEST(Error, NonexistingCharset)
{
	try
	{
		MaskSplitter splitter({"asd", "?3pok"});
		FAIL()<<"Exception expected";
	}
	catch(const std::exception &e)
	{
		SUCCEED();
	}
}
