#define _CRT_SECURE_NO_WARNINGS
#include <iostream>


namespace iv222
{
	class String
	{
	public:
		String(char* _str = "")  // если нету аргументов то "" пустая строка, если есть то копирует содержимое
		{
			size = strlen(_str);
			str = new char[size + 1];
			strcpy(str, _str);
		}

		String(const String& rhs)	// копирует из String в String
		{
			size = rhs.size;
			str = new char[size + 1];
			strcpy(str, rhs.str);
		}

		~String()
		{
			delete str; // удаляем выделенную память
		}

		const int get_size()
		{
			return size; // возвращает размер строки
		}

		friend std::ostream& operator<< (std::ostream& os, const String& out_string)
		{
			return os << out_string.str;
		}

		bool operator==(const String& rhs)
		{
			return strcmp(str, rhs.str) ? false : true;
		}

		bool operator>(const String& rhs)
		{
			return size > rhs.size ? true : false;
		}

		bool operator<(const String& rhs)
		{
			return size < rhs.size ? true : false;
		}

		
		/*

		реализовать функции и операторы для работы с классом

		*/

	private:
		char *str;
		int size;
	};
}

using namespace iv222;

int main()
{
	String a; //1
	std::cout << a.get_size() << std::endl;
	String b = "alex"; //1
	std::cout << b.get_size() << std::endl;
	String c = b; //2
	std::cout << c.get_size() << std::endl;

	printf("%s", a < b? "TRUE": "FALSE");

	return 0;
}
