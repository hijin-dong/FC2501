#include "StaticPerson.h"
#include <iostream>

int StaticPerson::num = 0;
// const int StaticPerson::num = 0 ; ���� �ص� ��
// ������ �ƴϰ� �ʱ�ȭ��

StaticPerson::StaticPerson()
{
	num++;
}

void StaticPerson::print()
{
	std::cout << num << std::endl;
}
