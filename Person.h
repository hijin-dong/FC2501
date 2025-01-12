#pragma once

# include <string>
# include <iostream>

class Person
{
private:
    int _weight;
    int _height;
    const std::string _name;

public:
    // (ĸ��ȭ) �������� ���� ������ ���� param�� �������� �ٷ� �� �� ����!
    Person(float weight, float height, const std::string& name);
    // (ĸ��ȭ) �ڼ��� ���� ����� ���� ��� �޼ҵ尡 �ִٴ� ���� �� �� ����!
    void print();

    // inline���� ���� ���� �ִ�!
    /*
    inline void print()
    {
      std::cout << weight << std::endl;
    }
    */
};

void foo();