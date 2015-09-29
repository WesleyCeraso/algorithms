#include <gtest/gtest.h>
#include <src/numberutils.h>

TEST(NumberUtils, toRoman)
{
    EXPECT_THROW(NumberUtils::toRoman(0), std::range_error);
    EXPECT_EQ(NumberUtils::toRoman(1), std::string("I"));
    EXPECT_EQ(NumberUtils::toRoman(1234), std::string("MCCXXXIV"));
    EXPECT_EQ(NumberUtils::toRoman(4999), std::string("MMMMCMXCIX"));
    EXPECT_THROW(NumberUtils::toRoman(5000), std::range_error);
}

TEST(NumberUtils, fromRoman)
{
    EXPECT_THROW(NumberUtils::fromRoman(""), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("1234"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("ABCD"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("IIII"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("IVIV"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("VVVV"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("IXIX"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("XXXX"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("XLXL"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("LLLL"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("XCXC"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("CCCC"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("CDCD"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("DDDD"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("CMCM"), std::domain_error);
    EXPECT_THROW(NumberUtils::fromRoman("MMMMM"), std::domain_error);
    EXPECT_EQ(NumberUtils::fromRoman("I"), 1);
    EXPECT_EQ(NumberUtils::fromRoman("MCCXXXIV"), 1234);
    EXPECT_EQ(NumberUtils::fromRoman("MMMMCMXCIX"), 4999);
}

TEST(NumberUtils, toSpeech)
{
    EXPECT_EQ(NumberUtils::toSpeech(0), std::string("zero"));
    EXPECT_EQ(NumberUtils::toSpeech(1), std::string("one"));
    EXPECT_EQ(NumberUtils::toSpeech(9), std::string("nine"));
    EXPECT_EQ(NumberUtils::toSpeech(10), std::string("ten"));
    EXPECT_EQ(NumberUtils::toSpeech(11), std::string("eleven"));
    EXPECT_EQ(NumberUtils::toSpeech(19), std::string("nineteen"));
    EXPECT_EQ(NumberUtils::toSpeech(20), std::string("twenty"));
    EXPECT_EQ(NumberUtils::toSpeech(21), std::string("twenty one"));
    EXPECT_EQ(NumberUtils::toSpeech(29), std::string("twenty nine"));
    EXPECT_EQ(NumberUtils::toSpeech(100), std::string("one hundred"));
    EXPECT_EQ(NumberUtils::toSpeech(110), std::string("one hundred ten"));
    EXPECT_EQ(NumberUtils::toSpeech(190), std::string("one hundred ninety"));
    EXPECT_EQ(NumberUtils::toSpeech(1000), std::string("one thousand"));
    EXPECT_EQ(NumberUtils::toSpeech(10000), std::string("ten thousands"));
    EXPECT_EQ(NumberUtils::toSpeech(100000), std::string("one hundred thousands"));
    EXPECT_EQ(NumberUtils::toSpeech(1000000), std::string("one million"));
    EXPECT_EQ(NumberUtils::toSpeech(1000000000), std::string("one billion"));
}