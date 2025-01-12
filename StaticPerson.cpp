#include "StaticPerson.h"
#include <iostream>

int StaticPerson::num = 0;
// const int StaticPerson::num = 0 ; 으로 해도 됨
// 대입이 아니고 초기화임

StaticPerson::StaticPerson()
{
	num++;
}

void StaticPerson::print()
{
	std::cout << num << std::endl;
}
