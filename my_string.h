#pragma once
#include<assert.h>
#include<iostream>
using namespace std;

namespace bit	//为了和库中的string类进行区分，设置一个名字空间
{
	class string
	{
	public:
		typedef const char* citerator;

		const citerator begin() const
		{
			return _str;
		}

		const citerator end() const
		{
			return _str + _size;
		}

		typedef char* iterator;

		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		string(const char* str = "")	//浅拷贝
		{
			_str = new char[strlen(str) + 1];
			_size = strlen(str);
			_capacity = _size;

			//用memcpy是为了解决string对象以'\0'就终止,而不是以_size大小终止,strcpy是遇到'\0'终止的
			//strcpy(_str, str);
			memcpy(_str, str, _size + 1);
		}

		string(const string& str)		//深拷贝(复制拷贝构造)
		{
			_str = new char[str._capacity + 1];
			//strcpy(_str, str._str);
			memcpy(_str, str._str, _size + 1);
			_size = str._size;
			_capacity = str._capacity;
		}

		/*string& operator= (const string& str)
		{
			if (this != &str)
			{
				char* temp = new char[str._capacity + 1];
				memcpy(temp, str._str, str._size + 1);
				delete[] _str;
				_str = temp;
				
				_size = str._size;
				_capacity = str._capacity;
			}
			return *this;
		}*/

		/*string& operator= (const string& str)
		{
			if (this != &str)
			{
				string temp(str);
				swap(temp);
			}
			return *this;
		}*/

		string& operator= (string str)	//str不是引用，而且是一个局部变量
		{
			swap(str);
			return *this;
		}

		~string()
		{
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}

		const char* c_str() const
		{
			return _str;
		}

		size_t size() const
		{
			return _size;
		}

		char& operator[] (size_t pos)
		{
			assert(pos < _size);
			return *(_str + pos);
		}

		const char& operator[] (size_t pos) const
		{
			assert(pos < _size);
			return *(_str + pos);
		}

		void reserve(size_t n)
		{
			if (n > _capacity)
			{
				char* tmp = new char[n + 1];
				memcpy(tmp, _str, _size + 1);
				delete _str;
				_str = tmp;
				_capacity = n;

			}
		}

		void push_back(char ch)
		{
			if (_size == _capacity)
			{
				//2倍扩容
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}
			_str[_size++] = ch;
			_str[_size] = '\0';
		}

		void append(const char* str)
		{
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				//至少扩容到_size + len
				reserve(_size + len);
			}
			memcpy(_str + _size, str, _size + len + 1);
			_size += len;
		}

		string& operator+= (char ch)
		{
			push_back(ch);
			return *this;
		}

		string& operator+= (const char* str)
		{
			append(str);
			return *this;
		}

		//在pos位置下插入n个ch字符
		void insert(size_t pos, size_t n, char ch)
		{
			assert(pos <= _size);
			if (_size + n > _capacity)
			{
				reserve(_size + n);
			}

			//挪动数据
			int end = _size;		//若为size_t，则pos=0时，会死循环，因为size_t减为负数时，变为无穷大
			while (end >= (int)pos)
			{
				_str[end + n] = _str[end];
				--end;
			}

			//插入字符
			for (size_t i = 0; i < n; i++)
			{
				_str[pos + i] = ch;
			}
			_size += n;
		}

		void insert(size_t pos, const char* str)
		{
			assert(pos <= _size);
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(_size + len);
			}

			//挪动数据
			int end = _size;		//若为size_t，则pos=0时，会死循环，因为size_t减为负数时，变为无穷大
			while (end >= (int)pos)
			{
				_str[end + len] = _str[end];
				--end;
			}

			//插入字符
			for (size_t i = 0; i < len; i++)
			{
				_str[pos + i] = str[i];
			}
			_size += len;
		}

		void erase(size_t pos, const size_t len = npos)
		{
			assert(pos <= _size);
			if (len == npos || pos + len >= _size)
			{
				_str[pos] = '\0';
				_size = pos;
				_str[_size] = '\0';
			}
			else
			{
				size_t end = pos + len;
				while (end <= _size)
				{
					_str[pos++] = _str[end++];
				}
				_size -= len;
			}
		}

		size_t find(char ch, size_t pos = 0) const
		{
			assert(pos < _size);
			for (size_t i = pos; i < _size; i++)
			{
				if (ch == _str[i])
				{
					return i;
				}
			}
			return npos;
		}

		size_t find(const char* str, size_t pos = 0) const
		{
			assert(pos < _size);
			const char* ptr = strstr(_str, str);
			if (ptr)
			{
				return ptr - _str;
			}
			else
			{
				return npos;
			}
		}

		string substr(size_t pos = 0, size_t len = npos)
		{
			assert(pos < _size);
			size_t n = len;
			if (len + pos < _size || len == npos)	//有多少取多少
			{
				n = _size - pos;
			}

			string temp;
			temp.reserve(n);
			for (size_t i = pos; i < pos + n; i++)
			{
				temp += _str[i];
			}

			return temp;
		}

		void resize(size_t n, char ch = '\0')
		{
			if (n < _size)
			{
				_size = n;
				_str[_size] = '\0';
			}
			else
			{
				reserve(n);

				for (size_t i = _size; i < n; i++)
				{
					_str[i] = ch;
				}

				_str[n] = '\0';
				_size = n;
			}
		}

		void clear()
		{
			_str[0] = '\0';
			_size = 0;
		}

		bool operator< (const string& s)
		{
			size_t i1 = 0;
			size_t i2 = 0;
			while (i1 < _size && i2 < s._size)
			{
				if (_str[i1] < s._str[i2])
				{
					return true;
				}
				else if(_str[i1] > s._str[i2])
				{
					return false;
				}
				else
				{
					i1++;
					i2++;
				}
			}
			if (_size >= s._size)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		bool operator== (const string& s)
		{
			return _size == s._size
				&& memcpy(_str, s._str, _size) == 0;
		}

		bool operator<= (const string& s)
		{
			return *this < s || *this == s;
		}

		bool operator> (const string& s)
		{
			return !(*this <= s);
		}

		bool operator>= (const string& s)
		{
			return !(*this < s);
		}

		bool operator!= (const string & s)
		{
			return !(*this == s);
		}

		void swap(string& s)
		{
			std::swap(_str, s._str);
			std::swap(_size, s._size);
			std::swap(_capacity, s._capacity);
		}

//		friend ostream& operator<< (std::ostream& out, const string& s);
//		friend istream& operator>> (istream& out, string& s);

	private:
		char* _str;
		size_t _size;
		size_t _capacity;

		const static size_t npos = -1;
	};
/*
	ostream& operator<< (std::ostream& out, const bit::string& s)
	{
		for (size_t i = 0; i < s.size(); i++)
		{
			out << s[i] << " ";
		}
		out << std::endl;
		return out;
	}

	istream& operator>> (istream& in, bit::string& s)
	{
		s.clear();
		char ch = in.get();

		//处理前缓冲区前面的空格或者换行
		while (ch == ' ' || ch == '\0')
		{
			ch = in.get();
		}
		char buff[128];
		int i = 0;
		while (ch != ' ' && ch != '\0')
		{
			buff[i++] = ch;
			if (i == 127)
			{
				buff[i] = '\0';
				s += buff;
				i = 0;
			}
			//in >> ch;
			ch = in.get();
		}
		if (i != 0)
		{
			buff[i] = '\0';
			s += buff;
		}
		return in;
	}
*/
};


void test_string1();
void test_string2();
void test_string3();
void test_string4();
void test_string5();
