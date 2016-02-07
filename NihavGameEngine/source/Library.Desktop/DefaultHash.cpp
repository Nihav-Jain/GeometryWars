#include "pch.h"
#include "DefaultHash.h"

namespace Library
{
	std::uint32_t DefaultHash<int>::operator()(const int& key) const
	{
		std::uint32_t hash = 0;
		int number = key;
		while (number != 0)
		{
			hash += (std::uint32_t)(number % 10);
			number /= 10;
		}
		return hash;
	}

	std::uint32_t DefaultHash<int*>::operator()(int* key) const
	{
		std::uint32_t hash = 0;
		unsigned char* arr = reinterpret_cast<unsigned char*>(key);
		
		while (*arr != '\0')
		{
			hash += (std::uint32_t)(*arr);
			arr = arr + 1;
		}

		return hash;
	}

	std::uint32_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		std::uint32_t hash = 0;
		for (std::uint32_t i = 0; i < key.length(); i++)
		{
			hash += ((std::uint32_t)key.length() - i) * (std::uint32_t)key.at(i);
		}
		return hash;
	}
	
	std::uint32_t DefaultHash<char*>::operator()(const char* key) const
	{
		std::uint32_t hash = 0;
	
		while (*key != '\0')
		{
			hash += (std::uint32_t)(*key);
			key = key + 1;
		}
	
		return hash;
	}
}
