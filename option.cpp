template<typename T>
void drop(T&& m)
{
	T t(m);
}
template<typename T>
class Option
{
public:
	Option(const Option &) = delete;
	Option(Option && ref) 
	{
		memcpy(m_chunk, ref.m_chunk, sizeof(T));
		is_valid = ref.is_valid;
		ref.is_valid = false;
	}
	Option() :is_valid(false) {};
	Option(T&& value) :m_chunk{ 0 }
	{
		T* t = (T*)m_chunk;
		memcpy(m_chunk, &value, sizeof(T));
		(void)std::move(value);
		is_valid = true;
	}
	
	~Option()
	{
		if (is_valid)
		{
			drop(std::move(*(T*)m_chunk));
		}
	}
	bool ok()
	{
		return is_valid;
	}
	T& operator *()
	{
		return *(T*)m_chunk;
	}
	T* operator ->()
	{
		return (T*)m_chunk;
	}
	template<typename function>
	auto map(function func)
	{
		using OPTION = Option<decltype(func(*(T*)m_chunk))>;
		OPTION res = ok() ? OPTION(std::move(func(*(T*)m_chunk))) : OPTION();
		is_valid = false;
		return res;
	}
	T unwrap_or(const T && defaultValue)
	{
		T t = ok() ? std::move(*(T*)m_chunk) : defaultValue;

		is_valid = false;
		return t;
	}
	T unwrap()
	{
		return std::move(*(T*)m_chunk);
	}
private:
	bool is_valid;
	uint8_t m_chunk[sizeof(T)];
};
#include <string> 
int main()
{
	Option<int> t(5);
	auto a = t.map([](const int n)->std::string {
		return std::to_string(n);
	});
	if (t.ok())
	{
		std::cout << *t << std::endl;
	}
	if (a.ok())
	{
		std::cout << *a << std::endl;
	}
	return 0;
}
