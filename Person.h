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
    // (캡슐화) 생성자의 내부 구조는 몰라도 param이 무엇인지 바로 알 수 있음!
    Person(float weight, float height, const std::string& name);
    // (캡슐화) 자세한 동작 방법은 몰라도 출력 메소드가 있다는 것을 알 수 있음!
    void print();

    // inline으로 적을 수도 있다!
    /*
    inline void print()
    {
      std::cout << weight << std::endl;
    }
    */
};

void foo();