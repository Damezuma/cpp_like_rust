template<typename T>
class Option
{
public:
	Option(const Option &) = delete;
	Option(Option && ref) :m_val(std::move(ref.m_val))
	{
		ref.m_val = nullptr;
	}
	Option() :m_val(nullptr) {};
	Option(const T&& value) :m_val(new T(std::move(value))) {};
	~Option()
	{
		if (m_val != nullptr)
		{
			delete m_val;
		}
	}
	bool ok()
	{
		return m_val != nullptr;
	}
	T& operator *()
	{
		return *m_val;
	}
	T* operator ->()
	{
		return m_val;
	}
	template<typename function>
	auto map(function func)
	{
		using OPTION = Option<decltype(func(*this->m_val))>;
		OPTION res = ok() ? OPTION(std::move(func(*this->m_val))) : OPTION();
		delete m_val;
		m_val = nullptr;
		return res;
	}
	T unwrap_or(const T && defaultValue)
	{
		T t = ok() ? std::move(*m_val) : defaultValue;
		delete m_val;
		m_val = nullptr;
		return t;
	}
	T unwrap()
	{
		return std::move(*m_val);
	}
private:
	T*m_val;
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
