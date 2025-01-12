# include "Person.h"

// String이어서 주소를 참조할 수 있는 것이 아니라, const이기 때문에 읽기 전용이어서 가능
Person::Person(float weight, float height, const std::string& name)
	: _weight(weight), _height(height), _name(name)
{}

void Person::print()
{
	std::cout << _name << "(" << _weight << ", " << _height << ")이 생성되었습니다." << std::endl;
}

void foo()
{
	std::cout << "FOO!" << std::endl;
}