#include <gtest/gtest.h>
#include <src/stringutils.h>

TEST(StringUtils, repeated)
{
	EXPECT_EQ(StringUtils::repeated("", 0), std::string());
	EXPECT_EQ(StringUtils::repeated("", 1), std::string());
	EXPECT_EQ(StringUtils::repeated("", 2), std::string());
	EXPECT_EQ(StringUtils::repeated("", 3), std::string());

	std::string str("ABC");
	EXPECT_EQ(StringUtils::repeated(str, 0), std::string());
	EXPECT_EQ(StringUtils::repeated(str, 1), str);
	EXPECT_EQ(StringUtils::repeated(str, 2), std::string(str + str));
	EXPECT_EQ(StringUtils::repeated(str, 3), std::string(str + str + str));
}

TEST(StringUtils, trimmed)
{
	EXPECT_EQ(StringUtils::trimmed(""), std::string());
	EXPECT_EQ(StringUtils::trimmed(" "), std::string());
	EXPECT_EQ(StringUtils::trimmed("  "), std::string());
	
	EXPECT_EQ(StringUtils::trimmed(","), std::string(","));
	EXPECT_EQ(StringUtils::trimmed(", "), std::string(","));
	EXPECT_EQ(StringUtils::trimmed(",  "), std::string(","));

	EXPECT_EQ(StringUtils::trimmed(","), std::string(","));
	EXPECT_EQ(StringUtils::trimmed(" ,"), std::string(","));
	EXPECT_EQ(StringUtils::trimmed("  ,"), std::string(","));

	EXPECT_EQ(StringUtils::trimmed(","), std::string(","));
	EXPECT_EQ(StringUtils::trimmed(" , "), std::string(","));
	EXPECT_EQ(StringUtils::trimmed("  ,  "), std::string(","));

	EXPECT_EQ(StringUtils::trimmed(", ,"), std::string(", ,"));
	EXPECT_EQ(StringUtils::trimmed(" ,  , "), std::string(",  ,"));
	EXPECT_EQ(StringUtils::trimmed("  ,   ,  "), std::string(",   ,"));
}

TEST(StringUtils, simplified)
{
	EXPECT_EQ(StringUtils::simplified(""), StringUtils::trimmed(""));
	EXPECT_EQ(StringUtils::simplified(" "), StringUtils::trimmed(" "));
	EXPECT_EQ(StringUtils::simplified("  "), StringUtils::trimmed("  "));
	
	EXPECT_EQ(StringUtils::simplified(","), StringUtils::trimmed(","));
	EXPECT_EQ(StringUtils::simplified(", "), StringUtils::trimmed(", "));
	EXPECT_EQ(StringUtils::simplified(",  "), StringUtils::trimmed(",  "));

	EXPECT_EQ(StringUtils::simplified(","), StringUtils::trimmed(","));
	EXPECT_EQ(StringUtils::simplified(" ,"), StringUtils::trimmed(" ,"));
	EXPECT_EQ(StringUtils::simplified("  ,"), StringUtils::trimmed("  ,"));

	EXPECT_EQ(StringUtils::simplified(","), StringUtils::trimmed(","));
	EXPECT_EQ(StringUtils::simplified(" , "), StringUtils::trimmed(" , "));
	EXPECT_EQ(StringUtils::simplified("  ,  "), StringUtils::trimmed("  ,  "));

	EXPECT_EQ(StringUtils::simplified(", ,"), std::string(", ,"));
	EXPECT_EQ(StringUtils::simplified(" ,  , "), std::string(", ,"));
	EXPECT_EQ(StringUtils::simplified("  ,    ,  "), std::string(", ,"));
}

TEST(StringUtils, fromBase64)
{
	EXPECT_EQ(StringUtils::fromBase64(""), std::string(""));
	EXPECT_EQ(StringUtils::fromBase64("dGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZyAwMTIzNDU2Nzg5"),
		std::string("the quick brown fox jumps over the lazy dog 0123456789"));
}

TEST(StringUtils, toBase64)
{
	EXPECT_EQ(StringUtils::toBase64(""), std::string(""));
	EXPECT_EQ(StringUtils::toBase64("the quick brown fox jumps over the lazy dog 0123456789"),
		std::string("dGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZyAwMTIzNDU2Nzg5"));
}

TEST(StringUtils, fromHex)
{
	EXPECT_EQ(StringUtils::fromHex(""), std::string(""));
	EXPECT_EQ(StringUtils::fromHex("74686520717569636B2062726F776E20666F78206A756D7073206F76657220746865206C617A7920646F672030313233343536373839"),
		std::string("the quick brown fox jumps over the lazy dog 0123456789"));
	EXPECT_EQ(StringUtils::fromHex("74686520717569636b2062726f776e20666f78206a756d7073206f76657220746865206c617a7920646f672030313233343536373839"),
		std::string("the quick brown fox jumps over the lazy dog 0123456789"));
}

TEST(StringUtils, toHex)
{
	EXPECT_EQ(StringUtils::toHex(""), std::string(""));
	EXPECT_EQ(StringUtils::toHex("the quick brown fox jumps over the lazy dog 0123456789"),
		std::string("74686520717569636B2062726F776E20666F78206A756D7073206F76657220746865206C617A7920646F672030313233343536373839"));
	EXPECT_NE(StringUtils::toHex("the quick brown fox jumps over the lazy dog 0123456789"),
		std::string("74686520717569636b2062726f776e20666f78206a756d7073206f76657220746865206c617a7920646f672030313233343536373839"));
}