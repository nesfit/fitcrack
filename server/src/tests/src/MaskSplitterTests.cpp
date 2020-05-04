#include "MaskSplitter.h"

#include <gtest/gtest.h>

#include <fstream>

static const std::string g_testMaskBasic = "a??b?dc?hd?He??f";
using CharsetsType = std::vector<std::string>;

std::vector<std::string> LoadMaskSuccession()
{
	std::ifstream input(TESTDATA_DIR "/generatedPasswords.txt");
	std::string line;
	std::vector<std::string> result;
	while(std::getline(input, line))
	{
		result.push_back(std::move(line));
	}
	return result;
}

static std::vector<std::string> g_correctMasks = LoadMaskSuccession();

TEST(Basic, ZeroOffsetAndKeyspace)
{
	MaskSplitter splitter;
	size_t startingIndex = 0;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 0);
	ASSERT_EQ(1, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Basic, Offset1)
{
	MaskSplitter splitter;
	size_t startingIndex = 1;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 0);
	ASSERT_EQ(1, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Basic, Keyspace2)
{
	MaskSplitter splitter;
	size_t startingIndex = 0;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 2);
	ASSERT_EQ(2, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Basic, FirstCharsetTwice)
{
	MaskSplitter splitter;
	size_t startingIndex = 0;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 20);
	ASSERT_EQ(20, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Basic, FirstTwoCharsets)
{
	MaskSplitter splitter;
	size_t startingIndex = 0;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 160);
	ASSERT_EQ(160, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, BreakOnFirstCharset)
{
	MaskSplitter splitter;
	size_t startingIndex = 4;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 160);
	ASSERT_EQ(6, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, BreakOnSecondCharset)
{
	MaskSplitter splitter;
	size_t startingIndex = 20;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 160);
	ASSERT_EQ(140, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, BreakOnFirstCharsetHugeKeyspace)
{
	MaskSplitter splitter;
	size_t startingIndex = 4;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 1600000);
	ASSERT_EQ(6, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, BreakOnSecondCharsetHugeKeyspace)
{
	MaskSplitter splitter;
	size_t startingIndex = 20;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 1600000);
	ASSERT_EQ(140, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, BreakOnFirstCharsetSecondPartlyDone)
{
	MaskSplitter splitter;
	size_t startingIndex = 24;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 1600000);
	ASSERT_EQ(6, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, BreakOnThirdCharset)
{
	MaskSplitter splitter;
	size_t startingIndex = 160*3;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 9999999);
	ASSERT_EQ(160*(16-3), result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, NoSmallLeftovers1)
{
	MaskSplitter splitter;
	size_t startingIndex = 160*3;
	size_t thirdCharsetLeftoverKeyspace = 16-3;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 160*thirdCharsetLeftoverKeyspace-10);
	ASSERT_EQ(160*thirdCharsetLeftoverKeyspace, result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(Advanced, NoSmallLeftovers2)
{
	MaskSplitter splitter;
	size_t startingIndex = 160*3;
	size_t thirdCharsetLeftoverKeyspace = 16-3;
	auto result = splitter.GetMaskSlice(g_testMaskBasic, startingIndex, 160*thirdCharsetLeftoverKeyspace*0.6);
	ASSERT_EQ(160*(thirdCharsetLeftoverKeyspace/2), result.keyspace);
	MaskSplitter testSplitter(result.customCharsets);
	for(size_t i = 0; i < result.keyspace; ++i)
	{
		auto testRes = testSplitter.GetMaskSlice(g_testMaskBasic, i+startingIndex, 1);
		ASSERT_EQ(g_correctMasks[i+startingIndex], testRes.mask);
	}
}

TEST(CustomCharsets, Inclusion)
{
	MaskSplitter splitter({"123", "?1345"});
	size_t startingIndex = 15;
	auto result = splitter.GetMaskSlice("?2as??d?2", startingIndex, 9999999);
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
