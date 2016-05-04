#include "pch.h"
#include "DefaultHash.h"

namespace Library
{
	std::uint32_t DefaultHash<std::int32_t>::operator()(const std::int32_t& key) const
	{
		return key;
	}

	std::uint32_t DefaultHash<std::int32_t*>::operator()(std::int32_t* key) const
	{
		std::uint32_t hash = 0;
		std::uint8_t* arr = reinterpret_cast<std::uint8_t*>(key);
		
		for (std::uint32_t i = 0; i < sizeof(key); i++)
		{
			hash += arr[i];
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
