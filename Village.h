#pragma once
/*
  #pragma once ��ſ�

  #ifndef __VILLAGE_H__
  #define __VILLAGE_H__

  �� �Ʒ��� (���⼭�� 26����) #endif
*/

# include <vector>

// Person.h�� include�ϱ� ���ٴ� �ϳ� �����ϰ� ���
// ���� �����ϴ� Village.cpp������ include ���൵ ��
// ��� ������ ��°�� ������ �Ǵµ�, ��� ������ ������ �� �긦 include�ϴ� ���ϵ鵵 ������ �� -> ������ �� ���ɿ� ����
class Person;

class Village
{
private:
	std::vector<Person> persons;

public:
	void add(Person);
};
