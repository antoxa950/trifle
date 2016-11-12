#include "gtest/gtest.h"
#include "DynamicBitset.h"

#include <time.h>
#include <iostream>

inline void initRandom()
{
	srand( static_cast<unsigned int>(time(0)) );
}

inline int random(const unsigned int a, const unsigned int b)
{
	return a + rand() % (b - a + 1);
}

class DynamicBitset : public ::testing::Test {
};

TEST_F(DynamicBitset, setNumber)
{
	initRandom();
	trifle::DynamicBitset<unsigned char> db = trifle::DynamicBitset<unsigned char>(10);

	unsigned char randValue = 0;
	char shift = 0;
	for (char i = 0; i < 15; ++i)
	{
		//from 0
		randValue = random(1, 255);
		db.setNumber<decltype(randValue)>(randValue, 0);
		EXPECT_EQ(db.getBlock(0), randValue);
		db.resetAll();

		// from shift
		shift = random(-7, 7);
		randValue = random(1, 255);
		if (db.setNumber<decltype(randValue)>(randValue, shift))
		{
			randValue >>= shift;
			EXPECT_EQ(db.getBlock(0), randValue);
		}
		db.resetAll();
	}
}

TEST_F(DynamicBitset, set)
{
	trifle::DynamicBitset<unsigned char> db = trifle::DynamicBitset<unsigned char>(16);

	db.set(7);
	EXPECT_EQ(db.getBlock(0), 1);
	db.set(6);
	EXPECT_EQ(db.getBlock(0), 3);
	db.set(4);
	EXPECT_EQ(db.getBlock(0), 11);
	db.set(0);
	EXPECT_EQ(db.getBlock(0), 139);
	db.resetAll();

	db.set(15);
	EXPECT_EQ(db.getBlock(1), 1);
	db.set(14);
	EXPECT_EQ(db.getBlock(1), 3);
	db.set(12);
	EXPECT_EQ(db.getBlock(1), 11);
	db.set(8);
	EXPECT_EQ(db.getBlock(1), 139);

	if (true == db.set(-1))
	{
		FAIL();
	}

	if (true == db.set(17))
	{
		FAIL();
	}
}

TEST_F(DynamicBitset, reset)
{
	trifle::DynamicBitset<unsigned char> db = trifle::DynamicBitset<unsigned char>(16);
	db.setAll();

	db.reset(7);
	EXPECT_EQ(db.getBlock(0), 254);
	db.reset(6);
	EXPECT_EQ(db.getBlock(0), 252);
	db.reset(4);
	EXPECT_EQ(db.getBlock(0), 244);
	db.reset(0);
	EXPECT_EQ(db.getBlock(0), 116);

	db.setAll();

	db.reset(15);
	EXPECT_EQ(db.getBlock(1), 254);
	db.reset(14);
	EXPECT_EQ(db.getBlock(1), 252);
	db.reset(12);
	EXPECT_EQ(db.getBlock(1), 244);
	db.reset(8);
	EXPECT_EQ(db.getBlock(1), 116);

	if (true == db.reset(-1))
	{
		FAIL();
	}

	if (true == db.reset(17))
	{
		FAIL();
	}
}

TEST_F(DynamicBitset, test)
{
	trifle::DynamicBitset<unsigned char> db = trifle::DynamicBitset<unsigned char>(16);
	db.setAll();

	EXPECT_TRUE(db.test(8));
	db.reset(7);
	EXPECT_FALSE(db.test(7));
	EXPECT_TRUE(db.test(6));
	db.reset(6);
	EXPECT_FALSE(db.test(6));
	db.reset(4);
	EXPECT_FALSE(db.test(4));
	db.reset(0);
	EXPECT_FALSE(db.test(0));

	db.setAll();

	db.reset(15);
	EXPECT_FALSE(db.test(15));
	EXPECT_TRUE(db.test(14));
	db.reset(14);
	EXPECT_FALSE(db.test(14));
	db.reset(12);
	EXPECT_FALSE(db.test(12));
	db.reset(8);
	EXPECT_FALSE(db.test(8));

	if (true == db.test(-1))
	{
		FAIL();
	}

	if (true == db.test(17))
	{
		FAIL();
	}
}