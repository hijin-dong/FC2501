#pragma once
class StaticPerson
{
private:
	static int num;
	// 얘의 초깃값을 지정해주어야 실행이 되는데 헤더 파일은 여기저기서 쓰이니까 cpp에서 해주자!
	// static 초기화는 여기서 해도 되긴 함!
	// class 내에서 쓰는 전역변수! 딱 한 번만 생성됨

public:
	StaticPerson();
	void print();

	// static void print();
	// 이렇게 하면 instance를 통하지 않고, 클래스를 통해서 호출 가능
	// 단, 정적 멤버에만 접근 가능 (static 붙은 것)
	// 객체가 없기에 this가 넘어가지 않음 -> print const ()로 사용 불가
};