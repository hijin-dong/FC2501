/*
  01. auto
*/
# include <iostream>
# include <map>
using namespace std;

// ~4
void func(float)
{}

// ~5
template<typename T, typename S>
auto add(T t, S s)
{
  cout << t + s << endl;
}

template<auto... Args>
auto sum()
{
  return (Args + ...);
}

// ~6
class Person
{
public:
  float height, weight;

  void func() {}
};

int main()
{
  // 1) 타입 추론
  map<int, int> m;
  // for (const pair<int, int>& pair : m)
  // pair<const int, int>& 로 적지 않으면 임시 객체 생성 가능성 있음 (key는 원칙적으로 수정 불가하기에)
  // auto를 사용함으로써 이러한 실수 방지
  for (const auto& pair : m) {}

  // 2) 키워드에 따라 auto의 역할이 달라질 수 있음
  // const와 reference는 떼는 경우가 많음
  const int a = 10;
  const int& b = a;
  const int* c = &a;

  auto aa = a; // int
  auto bb = b; // int (reference X)
  auto& bbb = b; // const int&: const가 없으면 기존 const 값을 수정할 수 있게 되기에 const로 추론
  auto cc = c; // const int*
  auto* ccc = c; // const int* (auto는 const int)

  // 3) 배열에 대한 추론
  int nums[] = { 1, 2, 3 };
  auto nums0 = nums; // int*
  auto& nums1 = nums; // int &nums[3] 배열 전체 레퍼런스

  // 4) 함수에 대한 추론
  auto func0 = func; // void (*func)(float) 함수 포인터
  auto& func1 = func; // 함수 레퍼런스

  // 5) 템플릿 매개변수 추론
  add(1, 1.1); // 2.1
  cout << sum<10, 200000000000, 1000000, 40000000000000>() << endl;

  // 6) 구조적 바인딩
  auto& [x, y, z] = nums; // const 붙이면 수정 방지
  cout << "x: " << x << ", y: " << y << ", z: " << z << endl;
  x = 10;
  cout << nums[0] << endl; // 10

  Person p{ 64.f, 172.f }; // class 내의 public으로 선언된 property도 가능
  auto [weight, height] = p;
  cout << "weight: " << weight << ", height: " << height << endl;
}

/*
  02. decltype [디클타입]
  decltype(표현식) : 표현식의 타입을 컴파일 시에 추론
*/
# include <iostream>
using namespace std;

// ~3
int func(float)
{
  return 10;
}

int f(float)
{
  // decltype는 실제로 함수를 실행하는 것은 아니기에 출력되지 않음
  cout << "function!" << endl;
  return 20;
}

struct Person
{
  float weight, height;
};

int main()
{
  // 1) 변수
  decltype(1) num = 10;
  decltype(1.1f) num1 = 10;
  decltype(num) num2 = 20;
  decltype(num)& num3 = num;
  const decltype(num) num4 = 10;
  decltype(1 + 22.2f) num5 = 1;
  decltype((num)) num6 = num; // int&
  decltype(func(1.f)) num7 = 20;

  // 2) 배열
  int nums[] = { 1, 2, 3 };
  decltype(nums) nums1 = { 1, 2, 4 }; // 같은 크기로만 가능
  auto nums2 = nums; // auto는 포인터로 가능했지만,
  // decltype(nums) nums1 = nums; // 불가

  // 3) 함수
  decltype(func) f; // 함수 시그니처 동일해야 함
  cout << f(10.f) << endl; // 20

  decltype(func)& f0 = func;
  cout << f0(10.f) << endl; // 10

  decltype(func)* f1 = func;
  cout << f1(10.f) << endl; // 10

  decltype(Person::weight) weight; // 실제로 실행되지 않기 때문에 인스턴스화 필요없음
}

/*
  03. lambda
  - 기본형 (no params)
  auto func = [](params) -> int // 반환형은 선택사항
  {};

  - 기본형 (params)
  auto func = [] -> int // 반환형은 선택사항
  {};
*/
#include <iostream>
#include <functional>
using namespace std;

// ~1.1
void foo(function<int(void)> func) // func는 void를 받고 int를 리턴
{
  cout << "1.1: " << func() << endl;
}

// ~1.3
struct Func
{
  int value;
  void operator()() // mutable이 없으면 여기에 const 키워드 추가됨
  {
    value++;
    cout << "1.3: " << value << endl;
  }
};

// ~1.5
auto func5()
{
  int num = 10;
  return [&]
    {
      return num;
    };
}

auto func6()
{
  int num = 10;
  return [=]
    {
      return num;
    };
}

// ~1.6
struct Test
{
  int num = 10;
  auto testFunc()
  {
    return [num = num] // 이니셜라이저 캡처
      {
        return num;
      };
  }
};

int main()
{
  // 1) 중괄호는 캡처
  int value = 10;
  auto func = [value]()
    {
      return value;
    };

  // 1.1) 캡처와 파라미터는 뭐가 다른가?
  // 호출하려는 파라미터의 함수 파라미터가 이미 정해져있는데, 별도로 넘겨주고 싶은 값이 있을 경우 캡처 활용
  foo(func);

  // 1.2) 레퍼런스로 캡처하지 않고 값 변경 가능
  // 레퍼런스가 아니므로 함수 내부에서만 임시로 변경되고 원본에는 영향 없음
  auto func1 = [value]() mutable
    {
      value = 20;
      cout << "1.2: " << value << endl;
    };
  func1();

  // 1.3) 1.2가 어떻게 가능할까? (람다함수 = 함수객체)
  Func func2;
  func2.value = 10; // capture
  func2(); // 11
  func2(); // 12
  // 같은 원리로 mutable 여러 번 호출 시, 원본에 영향을 안 주더라도 11이 아닌 12가 나올 수 있음

  // 1.4) 몽땅 캡처 & 레퍼런스로 몽땅 캡처 + Mix
  int num1 = 10;
  int num2 = 20;
  auto func3 = [=, &value]() // value는 레퍼런스로 캡처, 나머지는 값으로 캡처
    {
      cout << "1.4: " << num1 + num2 << endl;
    };
  func3();
  auto func4 = [&, value]() // value는 값으로 캡처, 나머지는 레퍼런스로 캡처
    {
      cout << "1.4: " << num1 + num2 << endl;
    };
  func4();

  // 1.5) 값 캡처와 레퍼런스 캡처의 차이
  auto f1 = func5();
  auto f2 = func6();
  cout << "1.5: " << f1() << endl; // 쓰레기값. 지역변수가 사라짐
  cout << "1.5: " << f2() << endl; // 10

  // 1.6) struct 캡처
  Test* t = new Test;
  auto func5 = t->testFunc();
  delete t;
  cout << "1.6: " << func5() << endl;


  // 2) 람다함수 즉시 실행도 가능
  [value]()
    {
      cout << "2: " << value << endl;
    }(); // 맨 뒤 소괄호는 람다 함수를 즉시 실행

  // 3) generic lambda
    auto func6 = [](auto value)
      {
        return value;
      };
    cout << "3: " << func6(10) << endl;
    cout << "3: " << func6("10") << endl;

    auto func7 = []<typename T>(T value) // C++20
    {
      return value;
    };
    cout << "3: " << func7(10) << endl;
    cout << "3: " << func7("10") << endl;
}

/*
	04. constexpr
	- 컴파일 시간에 미리 값을 계산 & 결정
	- C에서 # define NUMBER 5 작성 시 컴파일러가 NUMBER를 5로 치환
	- 매크로보다 더 많은 것을 컴파일 시간에 결정할 수 있도록 해주는 기능
	* 참고: https://nx006.tistory.com/22
*/

# include <iostream>
using namespace std;

// ~1
constexpr int fib(int num)
{
	return num <= 1 ? num : fib(num - 2) + fib(num - 1);
}

template<int N>
void print()
{
	cout << N << endl;
}

struct Item
{
	int power;
	// string name; // 리터럴 타입이 아닌 속성 추가 시 에러 발생
	constexpr Item(int power) : power(power)
	{}

	constexpr Item operator+(const Item& other) const
	{
		return Item(power + other.power);
	}
};

// ~2
# include <type_traits>
template<typename T>
auto getValue(T t)
{
	if constexpr (is_pointer<T>()) // constexpr 키워드 없으면 실행 안 됨 (int 역참조 불가)
		return *t;
	else
		return t;
}

int main()
{
	// 1) 상수로 취급
	constexpr int value = fib(10);
	int nums[value]; // 배열 선언 사용 가능
	print<value>(); // template 인자로 사용 가능
	constexpr Item item{ 10 }; // 내부 property가 모두 리터럴이면 struct도 가능

	// 2) constexpr if
	// 상수 시간에 계산되는 조건문
	int num = 10;
	int* pNum = &num;
	cout << getValue(num) << endl;
	cout << getValue(pNum) << endl;

	// 3) lambda에 constexpr 암시적으로 달려있음 (c++ 17~)
	auto func = [](int y) noexcept // noexcept 키워드를 붙여서 추가 최적화 가능
		{
			return y;
		};
	int nums1[func(10)]; // 람다 함수 리턴 값으로 배열 선언 사용 가능
}

/*
	05. 우측값 참조/이동 의미론
*/
# include <iostream>
# include <cstring>
# include <vector>
using namespace std;

// ~3: Destructor 오류 방지용
const char* Wrap(const char* str)
{
	return str ? str : "";
}

class String
{
private:
	char* _str;

public:
	// 암시적 변환을 막기 위해 explicit 추가
	explicit String(const char* str)
		: _str(new char[strlen(str) + 1])
	{
		strcpy(_str, str);
		cout << "Construct: " << _str << endl;
	}

	// 복사 연산자
	String(const String& other)
		: _str(new char[strlen(other._str) + 1])
	{
		strcpy(_str, other._str);
		cout << "Copy Construct: " << _str << endl;
	}

		// ~3.
		// copy가 아니라 move를 해보자!
		// 임시값(더 이상 사용되지 않을 값)이라고 판단되면 오버로딩된 함수로 들어옴
	String(String&& other) noexcept
		: _str(move(other._str))
	{
		cout << "Move Construct: " << _str << endl;
		other._str = nullptr;
	}

	~String()
	{
		cout << "Destruct: " << Wrap(_str) << endl;
		delete[] _str;
	}

	// 복사 대입 연산자
	String& operator=(const String& other)
	{
		cout << "Copy Operator: " << _str << " = " << other._str << endl;
		delete[] _str;

		_str = new char[strlen(other._str) + 1];
		strcpy(_str, other._str);
		return *this;
	}

		// ~3.
		// copy가 아니라 move를 해보자!
	String& operator=(String&& other) noexcept // noexcept 안 쓰면 이동이 아니라 복사가 될 수 있음
	{
		cout << "Move Operator: " << Wrap(_str) << " = " << other._str << endl;
		delete[] _str;
		// 깊은 복사나 new 필요 X
		_str = move(other._str);
		other._str = nullptr;
		return *this;
	}

	friend ostream& operator<<(ostream& os, String& str)
	{
		return (os << str._str);
	}
};

template<typename T>
void Swap(T& x, T& y)
{
	// T temp = x;
	T temp = move(x); // 이동 생성자를 호출 -> x는 비어있는 상태가 됨

	// x = y;
	x = move(y); // y는 비어있는 상태, x는 값을 가진 상태
	
	// y = temp;
	y = move(temp);
}

// ~5
String makeString(const char* str)
{
	return String(str);
}

int main()
{ 
	// 1) 예시 1
	cout << "===== Example 1 =====" << endl;
	String s0("abc");
	s0 = String("def");
	// 실행결과
		//Construct: abc
		//Construct : def
		//Copy Operator : abc = def
		//Destruct : def // 임시객체 삭제
		//Destruct : def // 프로그램 종료 전 s0 삭제

	// 2) 예시 2
	cout << "===== Example 2 =====" << endl;
	String s1("abc");
	String s2("def");
	Swap(s1, s2);
	cout << "After Swapping, S1: " << s1 << endl;
	cout << "After Swapping, S2: " << s2 << endl;
	// 실행결과: 불필요한 인스턴스 생성 다수 발생!
		//Construct: abc
		//Construct : def
		//Copy Construct : abc
		//Copy Operator : abc = def
		//Copy Operator : def = abc
		//Destruct : abc
		//Destruct : abc
		//Destruct : def
		//Destruct : def

	// 3) 위의 두 예시 개선: copy말고 이동을 해보자!
	// 이동: 얕은 복사 후 기존 변수에 널 포인터 세팅
		// 예시 1
		//Construct: abc
		//Construct : def
		//Move Operator : abc = def
		//Destruct :

		// 예시2
		//Construct: abc
		//Construct : def
		//Move Construct : abc
		//Move Operator : = def
		//Move Operator : = abc
		//Destruct :
		//Destruct : abc
		//Destruct : def
		//Destruct : def

	// 4) noexcept일 때만 이동, 아니면 복사
	// move_if_noexcept()

	// 5) Factory 함수
	cout << "===== Example 5 =====" << endl;
	makeString("abc");
	// 함수 내에서 move를 하지 않아도 Construct와 Destruct가 한 번씩 일어남
	// main 함수에서 진행한 것과 같은 효과 (return 하는 것에 대해서는 알아서 최적화)

	// 6) lvalue와 rvalue
	// - lvalue: 표현식에 id가 있고(메모리에 존재, 이름&영속성이 있음), 이동 불가. &a(레퍼런스)
	// - rvalue: 임시값, &&a
	int num0 = 10;
	int& num1 = num0;
	int&& num2 = 10; // 값이 같아도 논리상 num1은 넣어줄 수 없음!
	const int& num3 = 10; // const를 붙이면 lvalue에 rvalue 할당 가능
}

/*
	06. 전달(보편) 참조 / 완벽 전달
*/
# include <iostream>
# include <type_traits>
using namespace std;

// ~1
void goo(int&& value)
{
	cout << "goo(int&&)" << endl;
}

void goo(int& value)
{
	cout << "goo(int&)" << endl;
}

void foo(int&& value)
{
	// scope를 벗어나기 전까지 value는 유효함 -> params는 항상 lvalue
	// goo(value);
	goo(move(value));
}

void foo(int& value)
{
	goo(value);
}

// ~2.
template<typename T>
void foo2(T&& t)
{
	cout << "lvalue ref T: " << is_lvalue_reference_v<T> << endl;
	cout << "rvalue ref T: " << is_rvalue_reference_v<T> << endl;

	cout << "lvalue ref T&: " << is_lvalue_reference_v<T&> << endl;
	cout << "rvalue ref T&: " << is_rvalue_reference_v<T&> << endl;

	cout << "lvalue ref T&&: " << is_lvalue_reference_v<T&&> << endl;
	cout << "rvalue ref T&&: " << is_rvalue_reference_v<T&&> << endl;
}

// ~3.
template<typename T>
void foo3(T&& value)
{
	goo(forward<T>(value));
}

int main()
{
	// 1) rvalue, lvalue를 다르게 함수로 처리하기
	// 파라미터가 1개 늘어날 때마다 만들어야 하는 함수 개수가 2배가 됨 & 가변인자는?!
	cout << "===== Example 1 =====" << endl;
	foo(10); // goo(int&). rvalue를 넘겨줬는데 lvalue로 처리되고 있음 -> foo 내부 goo 호출 시 move 사용!
	int num = 10;
	foo(num); // goo(int&)

	// 2) 1번에서 나타난 문제 해결하기!
	move(10);
	move(num);
	// move 함수는 정의 1가지로 rvalue, lvalue 둘 다 처리!
	// -> 템플릿 파라미터에 &&를 붙이면 두 종류 다 받을 수 있음! (유니버셜 레퍼런스)
	// auto&&도 동일한 효과
	cout << "===== Example 2 =====" << endl;
	cout << boolalpha;
	int num0(10);
	foo2(num0); // lvalue
	cout << endl;

	int& num1 = num0;
	foo2(num0); // lvalue
	cout << endl;

	foo2(10); // rvalue
	cout << endl;

		// 1 & 2
		//lvalue ref T : true
		//rvalue ref T : false
		//lvalue ref T& : true
		//rvalue ref T& : false
		//lvalue ref T&& : true
		//rvalue ref T&& : false

		// 3
		//lvalue ref T : false
		//rvalue ref T : false
		//lvalue ref T& : true
		//rvalue ref T& : false
		//lvalue ref T&& : false
		//rvalue ref T&& : true

	// 3. 전달 참조 (보편 참조) 완벽 전달
	foo3(10);
}

/*
	07. 스마트 포인터
	- auto_ptr: 폐기 예정
	- unique_ptr
	- shared_ptr
	- weak_ptr
*/
# include <iostream>
# include <memory>
using namespace std;

struct Test
{
	int num;

	Test(int num) : num(num)
	{
		cout << "Construct: " << num << endl;
	}

	~Test()
	{
		cout << "Destruct: " << num << endl;
	}
};

// ~2
void foo(unique_ptr<Test> p)
{
	p.reset(new Test(100));
}

struct Deleter
{
	void operator()(Test* test) const
	{
		cout << "delete: " << test->num << endl;
		delete test;
	}
};

// ~4.
struct B;
struct A
{
	A() { cout << "Construct A" << endl; }
	~A() { cout << "Destruct A" << endl; }
	shared_ptr<B> b;
};
struct B
{
	B() { cout << "Construct B" << endl; }
	~B() { cout << "Destruct B" << endl; }
	// shared_ptr<A> a;
	weak_ptr<A> a;
};

int main()
{
	// 1) 문제 제기
	int* num0 = new int(10);
	int* num1 = num0;
	// delete num0;
	// delete num1; // 같은 포인터를 두 번 삭제하는 것 같은데..
	// 이런 상황을 쉽게 해결해보자!

	// 2) unique_ptr
	cout << "========== Example 2 ==========" << endl;
	Test* test = new Test(0);
	unique_ptr<Test> up0(test); // 파괴까지 알아서!
	unique_ptr<Test> up1(new Test(1)); // 이 방식이 더 안전! (직접 전달하니까 다른 곳에 쓰일 우려 X)
	auto up2 = make_unique<Test>(2); // 동일하게 동작! (c++14)

		// 2.1) 일반 포인터와의 공통점과 차이점
			// 공통점
	test->num;
	up0->num; // -> 를 이용해서 프로퍼티 접근

			// 차이점
	// unique_ptr<Test> p1 = p0; // 1번에서와 같은 문제 상황 발생 X. 할당 불가
	unique_ptr<Test> up3 = move(up0); // move하면 가능. p0은 유효하지 않은 상태가 됨

		// 2.2) 함수에 unique_ptr을 파라미터로 넘기려면..
	// foo(p3);
	foo(move(up3)); // unique_ptr은 unique 해야 함

		// 2.3) unique_ptr 회수도 가능
	Test* uniquePtr = up3.release();
	delete uniquePtr;
	// 이 때 delete도 커스텀 가능
	unique_ptr<Test, Deleter> p3(new Test(0));

	// 3) shared_ptr: 공동 소유
	cout << "========== Example 3 ==========" << endl;
	shared_ptr<Test> sp0(new Test(0));
	shared_ptr<Test> sp1 = sp0;
	shared_ptr<Test> sp2 = make_shared<Test>(0); // atomic 연산 가능 but 커스텀 deleter 사용 불가
	shared_ptr<Test> sp3 = sp2;

	cout << sp0.use_count() << endl;
	sp0.reset();
	cout << sp0.use_count() << endl;

	// 4) weak_ptr
	cout << "========== Example 4 ==========" << endl;
		// 4.1) 참조 포인터 레퍼런스 카운트를 증가시키지 않음
	auto a = make_shared<A>();
	auto b = make_shared<B>();
	a->b = b;
	b->a = a;
	// 현재 상황은 a와 b가 서로 물려있는 상황(순환참조)이기에 delete를 해주기가 난감 -> 메모리 누수
	// 둘 중 한 struct는 weak_ptr로 정의
	cout << a.use_count() << endl; // 1
	cout << b.use_count() << endl; // 2

		// 4.2) 프로퍼티에 접근하기
	auto wp0 = make_shared<Test>(0);
	weak_ptr<Test> wp1 = wp0;
	cout << wp0.use_count() << endl; // 1

	auto shared = wp1.lock();
	cout << "shared num: " << shared->num << endl;
	cout << wp0.use_count() << endl; // 2

		// 4.3) 기존 shared_ptr이 사라지면 weak_ptr은 lock이 되었더라도 무효가 됨
}

/*
	08. std::function
	- 호출 가능한 객체를 wrapping할 수 있음
*/
# include <iostream>
# include <functional>
using namespace std;

// ~1.
template<typename T>
void foo() { cout << typeid(T).name() << endl; }
int func(float) { return 1; }

struct Func0
{
	int operator()(float value) { return value; }
};

struct Func1
{
	static int func0(float value) { return value; }
	int func1(float value) const { return value; }
};

// ~2.
void func1(int n1, int n2, int n3)
{
	cout << "n1: " << n1 << endl;
	cout << "n2: " << n2 << endl;
	cout << "n3: " << n3 << endl;
}

int main()
{
	// 1) callable: 호출 가능한 객체: 함수, 함수 객체, 람다 ...
		// 1.1) 함수
	cout << "========== Example 1 ==========" << endl;
	foo<int(float)>(); // int __cdecl(float)

	function<int(float)> f0 = func;
	cout << f0(1.1f) << endl; // 1

		// 1.2) 람다
	function<int(float)> f1 = [](float value) { return value; };
	cout << f1(1.1f) << endl; // 1

		// 1.3) 함수 객체
	function<int(float)> f2 = Func0();
	cout << f2(1.1f) << endl; // 1

	function<int(float)> f3 = Func1::func0; // static 함수
	cout << f3(1.1f) << endl;

	int(Func1:: * f)(float) const = &Func1::func1; // non-static 함수
	cout << (Func1().*f)(2) << endl;

	// 2) Bind
	cout << "========== Example 2 ==========" << endl;
	bind(func1, placeholders::_3, placeholders::_1, placeholders::_2)
		(1, 2, 3); // 3, 1, 2
}

/*
	09. std::thread
*/
# include <iostream>
# include <thread>
# include <chrono>
using namespace std;

int main()
{
	cout << "thread ID: " << this_thread::get_id() << endl;

	thread th0; // 운영체제의 할당을 받지 않은 빈 스레드 객체
	thread th1([]
		{
			cout << "thread ID: " << this_thread::get_id() << endl;
			this_thread::sleep_for(1s);
		}); // callable 한 객체를 넣어 할당 요청
	th1.join(); // thread가 끝날때까지 대기
	cout << "Complete" << endl;
}
