#pragma once
/*
  #pragma once 대신에

  #ifndef __VILLAGE_H__
  #define __VILLAGE_H__

  맨 아래에 (여기서는 26라인) #endif
*/

# include <vector>

// Person.h를 include하기 보다는 하나 선언하고 사용
// 실제 정의하는 Village.cpp에서만 include 해줘도 됨
// 헤더 파일은 통째로 복붙이 되는데, 헤더 파일이 수정될 때 얘를 include하는 파일들도 변경이 됨 -> 컴파일 시 성능에 영향
class Person;

class Village
{
private:
	std::vector<Person> persons;

public:
	void add(Person);
};
