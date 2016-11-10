#pragma once

#include <bitset>
#include <string>

namespace trifle
{

	template<typename BlockType = char>
	class DynamicBitset {

	public:
		DynamicBitset();
		DynamicBitset(const unsigned int length);
		DynamicBitset(const DynamicBitset& obj);
		~DynamicBitset();

		static const char BITS_IN_BYTE = 8;

		bool set(const unsigned int pos);
		bool reset(const unsigned int pos);
		bool test(const unsigned int pos);

		void setLength(const unsigned int newLength);
		const unsigned int getLength() const;
		const unsigned int getSize() const;
		char getBlock(const unsigned int index) const;

		template<typename NumberType>
		bool setNumber(const NumberType& num, const unsigned int pos);

		std::string toString();
		std::string toSeparatedString();
		std::string toSeparatedStringFull();

		bool operator[](const unsigned int pos);
		void operator<<=(const unsigned int shift);
		void operator>>=(const unsigned int shift);
		DynamicBitset<BlockType>& operator=(const DynamicBitset<BlockType>& obj);

	protected:
		void setBit(BlockType& first, const unsigned char pos);
		void resetBit(BlockType& first, const unsigned char pos);
		bool testBit(BlockType& first, const unsigned char pos) const;

	private:
		void init(const unsigned int size);

	private:
		const unsigned char _bitsInBlockType;
		const unsigned char _powOf_2;

		unsigned int _length;
		unsigned int _arraySize;
		BlockType* _array;
	};

	template<typename BlockType>
	DynamicBitset<BlockType>::DynamicBitset()
		:
		_bitsInBlockType(sizeof(BlockType) * BITS_IN_BYTE),
		_powOf_2(log(_bitsInBlockType) / log(2)),
		_length(0),
		_arraySize(0),
		_array(nullptr)
	{
	}

	template<typename BlockType>
	DynamicBitset<BlockType>::DynamicBitset(const unsigned int length)
		:
		_bitsInBlockType(sizeof(BlockType) * BITS_IN_BYTE),
		_powOf_2(char(log(_bitsInBlockType) / log(2))),
		_array(nullptr)
	{
		init(length);
	}

	template<typename BlockType>
	DynamicBitset<BlockType>::DynamicBitset(const DynamicBitset& obj)
		:
		_bitsInBlockType(sizeof(BlockType) * BITS_IN_BYTE),
		_powOf_2(log(_bitsInBlockType) / log(2))
	{
		if (nullptr != obj._array)
		{
			_length = obj._length;
			_arraySize = obj._arraySize;
			_array = new BlockType[getSize()]{};
			std::memcpy(_array, obj._array, getSize());
		}
		else
		{
			_length = 0;
			_arraySize = 0;
			_array = nullptr;
		}
	}

	template<typename BlockType>
	DynamicBitset<BlockType>::~DynamicBitset()
	{
		if (nullptr != _array)
			delete[] _array;
	}

	template<typename BlockType>
	bool DynamicBitset<BlockType>::set(const unsigned int pos)
	{
		if (_array == nullptr || pos >= _length)
			return false;

		const int cellNumber = pos >> _powOf_2;
		setBit(_array[cellNumber], char(pos - (cellNumber << _powOf_2)));
		return true;
	}

	template<typename BlockType>
	bool DynamicBitset<BlockType>::reset(const unsigned int pos)
	{
		if (_array == nullptr || pos >= _length)
			return false;

		const int cellNumber = pos >> _powOf_2;
		resetBit(_array[cellNumber], char(pos - (cellNumber << _powOf_2)));
		return true;
	}

	template<typename BlockType>
	bool DynamicBitset<BlockType>::test(const unsigned int pos)
	{
		if (_array == nullptr || pos >= _length)
			return false;

		const int cellNumber = pos >> _powOf_2;
		return testBit(_array[cellNumber], char(pos - (cellNumber << _powOf_2)));
	}

	template<typename BlockType>
	void DynamicBitset<BlockType>::setLength(const unsigned int newLength)
	{
		if (nullptr == _array)
		{
			init(newLength);
		}
		else
		{
			if (newLength == 0)
			{
				_length = 0;
				_arraySize = 0;
				delete[] _array;
				_array = nullptr;
				return;
			}

			_length = newLength;
			unsigned int newArrayLen = int(ceilf((float)newLength / _bitsInBlockType));
			if (newArrayLen == _arraySize)
				return;

			BlockType* tmp = new BlockType[newArrayLen]{};
			if (newArrayLen > _arraySize)
				memcpy(tmp, _array, getSize());
			else
				memcpy(tmp, _array, newArrayLen * sizeof(BlockType));

			delete[] _array;
			_array = tmp;
			_arraySize = newArrayLen;
		}
	}

	template<typename BlockType>
	const unsigned int DynamicBitset<BlockType>::getLength() const
	{
		return _length;
	}

	template<typename BlockType>
	const unsigned int DynamicBitset<BlockType>::getSize() const
	{
		return _arraySize * sizeof(BlockType);
	}

	template<typename BlockType>
	char DynamicBitset<BlockType>::getBlock(const unsigned int index) const
	{
		if (index < _arraySize)
			return _array[index];
	}

	template<typename BlockType>
	template<typename NumberType>
	bool DynamicBitset<BlockType>::setNumber(const NumberType& num, const unsigned int pos)
	{
		const unsigned char numLen = sizeof(num) * BITS_IN_BYTE;
		if (_length - pos < numLen)
			return false;

		std::bitset<numLen> numBitset(num);

		const unsigned int startPos = pos + numLen - 1;
		for (int i = 0; i < numLen; i++)
		{
			if (numBitset.test(i))
				set(startPos - i);
			else
				reset(startPos - i);
		}
		return true;
	}

	template<typename BlockType>
	bool DynamicBitset<BlockType>::operator[](const unsigned int pos)
	{
		return test(pos);
	}


	template<typename BlockType>
	void DynamicBitset<BlockType>::operator<<=(const unsigned int shift)
	{
		for (int i = 0, curr = shift; curr < _length;)
		{

			if (test(curr))
				set(i);
			else
				reset(i);

			reset(curr);

			++i;
			++curr;
		}
	}

	template<typename BlockType>
	void DynamicBitset<BlockType>::operator>>=(const unsigned int shift)
	{
		for (int i = _length - 1, curr = i - shift; curr >= 0;)
		{

			if (test(curr))
				set(i);
			else
				reset(i);

			reset(curr);

			--i;
			--curr;
		}
	}

	template<typename BlockType>
	DynamicBitset<BlockType>& DynamicBitset<BlockType>::operator=(const DynamicBitset<BlockType>& obj)
	{
		DynamicBitset<BlockType>::DynamicBitset<BlockType>(obj);
		return *this;
	}

	template<typename BlockType>
	void DynamicBitset<BlockType>::setBit(BlockType& first, const unsigned char pos)
	{
		first |= 1 << (_bitsInBlockType - 1) - pos;
	}

	template<typename BlockType>
	void DynamicBitset<BlockType>::resetBit(BlockType& first, const unsigned char pos)
	{
		first &= ~(1 << (_bitsInBlockType - 1) - pos);
	}

	template<typename BlockType>
	bool DynamicBitset<BlockType>::testBit(BlockType& first, const unsigned char pos) const
	{
		return (first & (1 << (_bitsInBlockType - 1) - pos));// ? true : false;
	}

	template<typename BlockType>
	void DynamicBitset<BlockType>::init(const unsigned int length)
	{
		_length = length;
		const int s = int(ceilf((float)length / _bitsInBlockType));
		_arraySize = s;
		if (nullptr != _array)
			delete[] _array;
		_array = new BlockType[s]{};
	}

	template<typename BlockType>
	std::string DynamicBitset<BlockType>::toString()
	{
		std::string str = std::string();
		for (unsigned int i = 0; i < _length;)
		{
			if (test(i))
				str += '1';
			else
				str += '0';
			++i;
		}

		return str;
	}

	template<typename BlockType>
	std::string DynamicBitset<BlockType>::toSeparatedString()
	{
		std::string str = std::string();
		for (unsigned int i = 0; i < _length;)
		{
			if (test(i))
				str += '1';
			else
				str += '0';

			++i;

			if (i % _bitsInBlockType == 0)
				str += ' ';
		}

		return str;
	}

	template<typename BlockType>
	std::string DynamicBitset<BlockType>::toSeparatedStringFull()
	{
		std::string str = std::string();
		unsigned int len = _arraySize * _bitsInBlockType;
		for (unsigned int i = 0; i < len;)
		{
			if (test(i))
				str += '1';
			else
				str += '0';

			++i;

			if (i % _bitsInBlockType == 0)
				str += ' ';
		}

		return str;
	}

}