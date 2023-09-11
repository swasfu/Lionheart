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
	EntityID RegisterEntity();
	EntityID RegisterEntity(std::string);

	template<typename T>
	std::unordered_map<EntityID, std::unique_ptr<ComponentBase>>& GetComponentMap();

	template<typename T>
	T* GetComponent(EntityID);

	template<typename T>
	T* AddComponent(EntityID);

	template<typename... Ts>
	std::vector<std::tuple<Ts...>>& View();

	template<typename T1, typename T2, typename... Ts>
	std::vector<EntityID> ViewIDs();

	template<typename T>
	std::vector<EntityID> ViewIDs();

	void LodgeUnresolvedID(EntityID*, std::string);
	void ResolveIDs();
	void FlushResolver();

	EntityID GetEntityCount();
private:
	EntityID entityCount_;
	std::unordered_map<std::string, EntityID> stringIDsToEntityIDs_;
	std::unordered_map<EntityID*, std::string> unresolvedEntityIDPtrsToStringIDs_;
	std::unordered_map<std::type_index, std::unordered_map<EntityID, std::unique_ptr<ComponentBase>>> componentTypeToMap_;
};

template<typename T>
std::unordered_map<EntityID, std::unique_ptr<ComponentBase>>& Registry::GetComponentMap()
{
	const std::type_index componentTypeIndex(typeid(T));
	return componentTypeToMap_[componentTypeIndex];
}

template<typename T>
T* Registry::AddComponent(EntityID id)
{
	auto& componentMap = GetComponentMap<T>();
	componentMap[id] = std::make_unique<T>();
	componentMap[id]->id = id;
	return static_cast<T*>(componentMap[id].get());
}

template<typename T>
T* Registry::GetComponent(EntityID id)
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
std::vector<EntityID> Registry::ViewIDs()
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
std::vector<EntityID> Registry::ViewIDs()
{
	auto matchingIDs = std::vector<EntityID>();
	auto& componentMap = GetComponentMap<T>();
	for (auto& componentMapItr : componentMap)
	{
		matchingIDs.push_back(componentMapItr.second.get()->id);
	}
	return matchingIDs;
}