template<typename T>
decltype(auto) iterator(const T& collection);
template<typename T, typename F>
decltype(auto) map(const T& collection, F func);
template<typename T, typename F>
decltype(auto) filter(const T& collection, F func);
template <typename RangeIt, typename FUNC>
class Filter
{
public:
	Filter(const RangeIt begin, const RangeIt end, FUNC f)
		:m_begin(begin), m_end(end), m_func(f)
	{
		while (m_begin != m_end)
		{
			if (m_func(*m_begin))
			{
				break;
			}
			++m_begin;
		}
	}
	class const_iterator
	{
	public:
		const_iterator(RangeIt it,RangeIt end, FUNC func)
			:m_it(it), m_func(func), m_end(end)
		{
			
		}
		const_iterator operator ++(int n)
		{
			const_iterator it = *this;
			++m_it;
			while (m_it != m_end)
			{
				if (m_func(*m_it))
				{
					break;
				}
				++m_it;
			}
			return it;
		}
		const_iterator& operator ++()
		{
			++m_it;
			while (m_it != m_end)
			{
				if (m_func(*m_it))
				{
					break;
				}
				++m_it;
			}
			return *this;
		}
		bool operator ==(const const_iterator& obj)
		{
			return obj.m_it == this->m_it;
		}
		bool operator !=(const const_iterator& obj)
		{
			return obj.m_it != this->m_it;
		}
		decltype(auto) operator *()
		{
			return *m_it;
		}
	private:
		RangeIt m_it;
		RangeIt m_end;
		FUNC m_func;
	};
	const_iterator begin() const
	{
		return const_iterator(m_begin, m_end, m_func);
	}
	const_iterator end() const
	{
		return const_iterator(m_end, m_end, m_func);
	}
	template<typename FUNC2>
	decltype(auto) map(FUNC2 func)const
	{
		return ::map(*this, func);
	}
	template<typename FUNC2>
	decltype(auto)  filter(FUNC2 func) const
	{
		return ::filter(*this, func);
	}
	template<typename FUNC>
	void foreach(FUNC func) const
	{
		for (auto & it : *this)
		{
			func(*it);
		}
	}
private:
	RangeIt m_begin;
	RangeIt m_end;
	FUNC m_func;
};
template <typename RangeIt, typename FUNC>
class Map
{
public:
	Map(const RangeIt begin, const RangeIt end, FUNC f)
		:m_begin(begin), m_end(end), m_func(f)
	{

	}
	class const_iterator
	{
	public:
		const_iterator(RangeIt it, FUNC func)
			:m_it(it), m_func(func)
		{
		}
		const_iterator operator ++(int n)
		{
			const_iterator it(m_it, m_func);
			this->m_it++;
			return it;
		}
		const_iterator& operator ++()
		{
			this->m_it++;
			return *this;
		}
		bool operator ==(const const_iterator& obj)
		{
			return obj.m_it == this->m_it;
		}
		bool operator !=(const const_iterator& obj)
		{
			return this->m_it != obj.m_it;
		}
		decltype(auto) operator *()
		{
			return m_func(*m_it);
		}
	private:
		RangeIt m_it;
		FUNC m_func;
	};
	const_iterator begin() const
	{
		return const_iterator(m_begin, m_func);
	}
	const_iterator end() const
	{
		return const_iterator(m_end, m_func);
	}
	template<typename FUNC2>
	decltype(auto) map(FUNC2 func)const
	{
		return ::map(*this, func);
	}
	template<typename FUNC2>
	decltype(auto)  filter(FUNC2 func) const
	{
		return ::filter(*this, func);
	}
	template<typename FUNC>
	void foreach(FUNC func) const
	{
		for (auto it : *this)
		{
			func(it);
		}
	}
private:
	RangeIt m_begin;
	RangeIt m_end;
	FUNC m_func;
};

template<typename RangeIt>
class Iterator
{
public:
	Iterator(const RangeIt& begin, const RangeIt& end) :
		m_begin(begin),
		m_end(end)
	{

	}
	class const_iterator
	{
	public:
		const_iterator(RangeIt it)
			:m_it(it)
		{
		}
		const_iterator operator ++(int n)
		{
			const_iterator it(m_it);
			this->m_it++;
			return it;
		}
		const_iterator& operator ++()
		{
			this->m_it++;
			return *this;
		}
		bool operator ==(const const_iterator& obj) const
		{
			return obj.m_it == this->m_it;
		}
		bool operator !=(const const_iterator& obj) const
		{
			return obj.m_it != this->m_it;
		}
		decltype(auto) operator *() const
		{
			return *m_it;
		}
	private:
		RangeIt m_it;
	};
	const_iterator begin() const
	{
		return const_iterator(m_begin);
	}
	const_iterator end() const
	{
		return const_iterator(m_end);
	}
	template<typename FUNC2>
	decltype(auto) map(FUNC2 func)const
	{
		return ::map(*this, func);
	}
	template<typename FUNC>
	decltype(auto)  filter(FUNC func) const
	{
		return ::filter(*this, func);
	}
	template<typename FUNC>
	void foreach(FUNC func) const
	{
		for (auto & it : *this)
		{
			func(*it);
		}
	}
private:
	RangeIt m_begin;
	RangeIt m_end;
};
template<typename T, typename F>
decltype(auto) map(const T& collection, F func)
{
	return Map<T::const_iterator, F>(collection.begin(), collection.end(), func);
}
template<typename T, typename F>
decltype(auto) filter(const T& collection, F func)
{
	return Filter<T::const_iterator, F>(collection.begin(), collection.end(), func);
}
template<typename T>
decltype(auto) iterator(const T& collection)
{
	return Iterator<T::const_iterator>(collection.begin(), collection.end());
}
#include <string> 
#include <sstream>
#include <vector>
int main()
{
	std::vector<int> aa{ 1,2,3,4,5 };
	iterator(aa).filter([](const int it) {
		return it % 2 == 1;
	}).map([](const int& it) {
		std::stringstream  s;
		s << "숫자 " << it << "는 홀수입니다";
		s.flush();
		return (std::string)s.str();
	}).foreach([](std::string& it) {
		std::cout << it << std::endl;
	});
	return 0;
}
