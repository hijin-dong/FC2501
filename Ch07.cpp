/*
	01. type traits (1)
	- 컴파일 시 타입에 대한 핸들링
*/
# include <iostream>
# include <type_traits>
using std::cout;
using std::endl;

template<typename T>
struct is_pointer
{
	static const bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
	static const bool value = true;
};

template<typename T>
void foo(T t)
{
	// T를 통해 구체화되기 전까지 type을 알 수 없을 경우에 활용!
	cout << is_pointer<T>::value << endl;
}

//
template<typename T>
struct add_pointer
{
	using type T*;
};

template<typename T>
struct add_pointer<T&>
{
	using type = T*;
};

template<typename T>
struct remove_pointer
{
	using type = T;
};

template<typename T>
struct remove_pointer<T*>
{
	using type = T;
};

int main()
{
	// 1. type_traits 구현해보기
	// cout이 bool타입 알파벳 형식 true/false로 출력하도록 설정
	cout << std::boolalpha;
	cout << is_pointer<int>::value << endl; // false
	cout << is_pointer<int*>::value << endl; // true

  // 2. 언제 사용하는가
	int num = 0;
	int* pNum = &num;
	foo(num); // false
	foo(pNum); // true

	// 3. 포인터를 추가/없애는 것도 가능!
	remove_pointer<int*>::type pointerNum;
	cout << typeid(pointerNum).name() << endl;
}

/*
	02. type traits (2)
*/
# include <iostream>
# include <type_traits>
using std::cout;
using std::endl;

enum Unscoped
{
	A, B
};

enum class Scoped : long long
{
	B, A = 100000000000000
};

template<typename T>
struct is_scoped_enum
{
	static const bool value
		= std::is_enum<T>::value && !!std::is_convertible<T, int>::value;
	// T가 int로 변환 가능한지: Scoped는 정수로 변환 X
};

// T가 Scoped enum일때만 받고 싶음
template<typename T, typename std::enable_if<is_scoped_enum<T>::value, int>::type = 0>
std::ostream& operator<<(std::ostream& os, const T& value)
{
	cout << "operator<<" << endl;
	
	static_assert(is_scoped_enum<T>::value, "T is not scoped enum"); // 첫 항이 true인가? 아니라면 두 번째 항 출력 & 컴파일 에러

	using t = std::underlying_type<Scoped>::type;
	cout << static_cast<t>(value);
	return cout;
}

int main()
{
	cout << std::boolalpha;

	cout << Unscoped::A << endl; // 0
	cout << static_cast<int>(Scoped::A) << endl; // overflow!
	cout << std::underlying_type<Scoped>::type(Scoped::A) << endl; // 100000000000000, 매번 다 적어주기엔 번거로움
	// -> ostream에 대한 operator overloading을 통해 해결
	cout << Scoped::A << endl; // 이거 왜 안 되는지 확인 필요

	cout << is_scoped_enum<Unscoped>::value << endl; // false
	cout << is_scoped_enum<Scoped>::value << endl; // true
}

/*
	03. RTTI
	- 런타임에 실시간으로 데이터의 타입을 얻어올 때 사용하는 방법
	- 가상함수 테이블에 정보 저장
*/
# include <iostream>
# include <typeinfo>
using namespace std;

class Parent
{
public:
	virtual ~Parent() {}
};

class Child : public Parent
{
};

int main()
{
	Parent* p = new Child;
	// 가상함수(여기서는 소멸자)가 없으면 컴파일 시 기준으로 class Parent 출력
	// 가상함수 있으면 런타임 시의 class Child 출력 & dynamic cast도 가능
	cout << typeid(*p).name() << endl;
}
