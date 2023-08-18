#pragma once

#include "ComponentBase.h"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <cassert>


class Registry
{
public:
	Registry();
	unsigned int RegisterEntity(std::string);
	template<typename T>
	std::unordered_map<unsigned int, std::unique_ptr<ComponentBase>>& GetComponentMap();
	template<typename T>
	T* GetComponent(unsigned int);
	template<typename T>
	T* AddComponent(unsigned int);
	template<typename... Ts>
	std::vector<std::tuple<Ts...>>& View();
	template<typename T1, typename T2, typename... Ts>
	std::vector<unsigned int> ViewIDs();
	template<typename T>
	std::vector<unsigned int> ViewIDs();
private:
	std::unordered_map<std::string, unsigned int> ids_;
	std::unordered_map<std::type_index, std::unordered_map<unsigned int, std::unique_ptr<ComponentBase>>> componentMaps_;
};

template<typename T>
std::unordered_map<unsigned int, std::unique_ptr<ComponentBase>>& Registry::GetComponentMap()
{
	const std::type_index componentTypeIndex(typeid(T));
	return componentMaps_[componentTypeIndex];
}

template<typename T>
T* Registry::AddComponent(unsigned int id)
{
	auto& componentMap = GetComponentMap<T>();
	componentMap[id] = std::make_unique<T>();
	componentMap[id]->id_ = id;
	return static_cast<T*>(componentMap[id].get());
}

template<typename T>
T* Registry::GetComponent(unsigned int id)
{
	auto& componentMap = GetComponentMap<T>();
	if (componentMap.find(id) == componentMap.end()) return nullptr;
	return static_cast<T*>(componentMap[id].get());
}

template<typename... Ts>
std::vector<std::tuple<Ts...>>& Registry::View()
{
	auto matchingComponentTuples = std::vector<std::tuple<Ts...>>();

	// TODO!!!

	return matchingComponentTuples;
}

template<typename T1, typename T2, typename... Ts>
std::vector<unsigned int> Registry::ViewIDs()
{
	auto givenIDs = ViewIDs<T2, Ts...>();

	auto& componentMap = GetComponentMap<T1>();
	for (auto it = givenIDs.begin(); it != givenIDs.end();)
	{
		if (GetComponent<T1>(*it) == nullptr) it = givenIDs.erase(it);
		else ++it;
	}

	return givenIDs;
}

template<typename T>
std::vector<unsigned int> Registry::ViewIDs()
{
	auto matchingIDs = std::vector<unsigned int>();
	auto& componentMap = GetComponentMap<T>();
	for (auto& componentMapItr : componentMap)
	{
		matchingIDs.push_back(componentMapItr.second.get()->id_);
	}
	return matchingIDs;
}