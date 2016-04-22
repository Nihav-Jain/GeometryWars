#pragma once
#include "pch.h"
#include "Factory.h"

namespace Library
{
	template<typename AbstractProductT>
	Hashmap<std::string, const Factory<AbstractProductT>* const> Factory<AbstractProductT>::sFactories;

	template<typename AbstractProductT>
	const Factory<AbstractProductT>* const Factory<AbstractProductT>::Find(const std::string& className)
	{
		Iterator itr = sFactories.Find(className);
		if (itr == sFactories.end())
			return nullptr;
		return (*itr).second;
	}

	template<typename AbstractProductT>
	AbstractProductT* Factory<AbstractProductT>::Create(const std::string& className)
	{
		const Factory* const factoryInstance = Find(className);
		if (factoryInstance == nullptr)
			return nullptr;
		return factoryInstance->Create();
	}

	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::Iterator Factory<AbstractProductT>::begin()
	{
		return sFactories.begin();
	}

	template<typename AbstractProductT>
	typename Factory<AbstractProductT>::Iterator Factory<AbstractProductT>::end()
	{
		return sFactories.end();
	}

	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Add(Factory& factory)
	{
		if (!sFactories.ContainsKey(factory.ClassName()))
			sFactories.Insert(factory.ClassName(), &factory);
	}

	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Remove(const Factory& factory)
	{
		sFactories.Remove(factory.ClassName());
	}
}