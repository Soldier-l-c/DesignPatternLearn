#pragma once
class Logger
{
public:
	Logger();
	Logger(int32_t level);

	~Logger();

	template <class T>
	Logger& operator<<(const T& t)
	{
		std::cout << t;
		return *this;
	}

private:
	int32_t level{ 30 };
};