#pragma once
#include <map>

template<typename T1, typename T2>
class ResourceRepository
{
private:
	std::map<T1, T2> m_repo;

public:
	ResourceRepository();
	~ResourceRepository();

	const T2& add(T1 key, T2 val);
	void remove(T1 key);
	bool exists(T1 key);

	const T2& find(T1 key);

};

template<typename T1, typename T2>
inline ResourceRepository<T1, T2>::ResourceRepository()
{
}

template<typename T1, typename T2>
inline ResourceRepository<T1, T2>::~ResourceRepository()
{
}

template<typename T1, typename T2>
inline const T2& ResourceRepository<T1, T2>::add(T1 key, T2 val)
{
	auto it = m_repo.find(key);

	if (it != m_repo.cend())
		return it->second;


	m_repo.insert({ key, val });

	it = m_repo.find(key);
	return it->second;

	
}

template<typename T1, typename T2>
inline void ResourceRepository<T1, T2>::remove(T1 key)
{
	auto it = m_repo.find(key);

	if (it == m_repo.cend())
		return;
	
	m_repo.erase(key);
}

template<typename T1, typename T2>
inline bool ResourceRepository<T1, T2>::exists(T1 key)
{
	return m_repo.count(key);		
}

template<typename T1, typename T2>
inline const T2& ResourceRepository<T1, T2>::find(T1 key)
{
	auto it = m_repo.find(key);
	if (it != m_repo.cend())
		return it->second;
	return NULL;
}
