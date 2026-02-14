#pragma once
class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
	virtual void Remove(uint32 _entity) = 0;
};

template<typename T>
class ComponentPool : public IComponentPool
{
public:
    ComponentPool() = default;
	~ComponentPool() = default;
    void Add(uint32 _entity, T component);
	T* GComponent(uint32 _entity);
	void Remove(uint32 _entity) override;

    Vector<uint32> GEntityIds();
private:
	Vector<T> m_components;
	Vector<int32> m_index;
	Vector<uint32> m_emptySlots;
};

template<typename T>
inline void ComponentPool<T>::Add(uint32 _entity, T component)
{
    if (_entity >= m_index.size()) {
        m_index.resize(_entity + 1, -1);
    }

    if (m_index[_entity] == -1)
    {
        if (!m_emptySlots.empty()) {
            uint32 slot = m_emptySlots.back();
            m_emptySlots.pop_back();
            m_index[_entity] = slot;
            m_components[slot] = component;
        }
        else {
            m_index[_entity] = m_components.size();
            m_components.push_back(component);
        }
    }
    else
    {
        m_components[m_index[_entity]] = component;
    }
}

template<typename T>
inline T* ComponentPool<T>::GComponent(uint32 _entity)
{
    if (_entity >= m_index.size() || m_index[_entity] == -1) {
        return nullptr;
    }
    return &m_components[m_index[_entity]];
}

template<typename T>
inline void ComponentPool<T>::Remove(uint32 _entity)
{
    if (_entity < m_index.size() && m_index[_entity] != -1)
    {
        m_emptySlots.push_back(m_index[_entity]);
        m_index[_entity] = -1;
    }
    //blocks acces to the component
}
template<typename T>
Vector<uint32> ComponentPool<T>::GEntityIds()
{
    Vector<uint32> ids;
    for (auto allIds : m_index)
    {
        if (allIds != -1)
        {
            ids.push_back(allIds);
        }
    }
    return ids;
}
