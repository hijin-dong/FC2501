# include "Person.h"

// String�̾ �ּҸ� ������ �� �ִ� ���� �ƴ϶�, const�̱� ������ �б� �����̾ ����
Person::Person(float weight, float height, const std::string& name)
	: _weight(weight), _height(height), _name(name)
{}

void Person::print()
{
	std::cout << _name << "(" << _weight << ", " << _height << ")�� �����Ǿ����ϴ�." << std::endl;
}

void foo()
{
	std::cout << "FOO!" << std::endl;
}