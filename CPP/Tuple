#include <tuple>
#include <iostream>
#include <string>

int main()
{

	//tuple(кортеж) может хранить разные типы данных

	std::tuple<int, char> a{1, 's'};
	std::tuple<std::string, double> b{ "alex", 3123.2 };

	//чтобы получить значение кортежа нужно использовать функцию get<index>(сам кортеж)

	std::cout << std::get<0>(b) << std::endl;

	auto result = std::make_tuple("dasxca", 'd', 21312, 123.0f, 3123.231);

	//make_tuple сам определит какого типа будет кортеж

	std::cout << std::tuple_size<decltype(result)>::value << std::endl;
	
	//выводит размер кортежа
}
