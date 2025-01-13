/*
  01. 형변환 규칙
*/
# include <iostream>
using namespace std;

int main()
{
  ///////////////////
  // 1. 자료형 변환 //
  ///////////////////

  float f = 1; // 확장 변환
  char ch = 1544; // 값 손실 발생. 축소 변환 
  int i = 1.1f; // 값 손실 발생. 축소 변환

  // int i_ = { 1.1f }; // 중괄호에 넣으면 축소변환이 방지됨!
  // 중괄호에 변수를 넣을 수 없으나 const로 고정하여 컴파일 시에 값을 알 수 있으면 가능!
  const int num = 10;
  char ch{ num }; 

  // int 아래 단위는 자동으로 확장되어 오버플로우가 나지 않음
  unsigned short s0 = 40000;
  cout << s0 + s0 << endl;

  // 오버플로우
  unsigned int i0 = 410000000;
  cout << typeid(i0 + i0).name() << endl;
  // 오버플로우가 나지 않는 예시 (int + long long => long long)
  long long ll0 = 410000000;
  cout << i0 + ll0 << endl;
  
  // unsigned와 signed를 더하면 unsigned가 됨
  unsigned int ui0 = 10;
  int mi1 = -110;
  cout << ui0 + mi1 << endl; // underflow 발생하여 429496764.. 값 나옴

  ///////////////////
  //2. 컨텍스트 변환//
  ///////////////////
  // explicit 예약어가 있더라도 for, while, &&, || 등 안에서 bool 변환됨

  ///////////////////
  // 3. 암시적 변환 //
  ///////////////////
  // 할당, 복사할 때
}

/*
  02. static_cast
  - 컴파일 시 형변환 타입 오류를 잡아줌. 기본 캐스팅 형태
*/
# include <iostream>
using namespace std;

class ClassA
{};

class ClassB
{};

enum class Type
{
  A, B, C
};

class Test
{
  // explicit 이 있어도 static_cast으로 형변환 가능!
public:
  explicit Test(int num) { cout << num << endl; }
  explicit operator bool() const { return true; }
};

int main()
{
  float f = 1.1f;
  int* i = (int*)(&f);
  // 위의 할당은 실행되면 메모리 크기 차이로 문제가 생김
  // 아래처럼 static_cast를 쓰면 문제 있다고 알려주지!
  // int* i = static_cast<int*>(&f);
  
  Type type = static_cast<Type>(0);
  cout << static_cast<int>(type) << endl;

  Test t = static_cast<Test>(1);
  Test t1(10);
  bool b = static_cast<bool>(t1);
}

/*
  03. const_cast
  - const, volatile 속성을 제거함
*/
# include <iostream>
using namespace std;

void func(const int& i)
{
	int& j = const_cast<int&>(i);
	j = 20;
}

class String
{
private:
	char* _chars;

public:
	String(const char* chars) : _chars(new char[strlen(chars) + 1])
	{
		strcpy(_chars, chars);
	}
	char& operator[](int index)
	{
		// char& operator[](int index) const 로 정의하는 대신 아래와 같이 사용 가능
		const String& s = *this;
		const char& c = s[index];
		return const_cast<char&>(c);
	}
	const char& operator[](int index) const
	{
		return _chars[index];
	}
};

int main()
{
	/* Volatile이란 ? */
	int i = 0;
	i++; i++; i++;
	// -> 컴파일 과정에서 +3으로 최적화됨

	volatile int i1 = 0;
	i++; i++; i++;
	// -> 컴파일 과정에서 최적화를 하지 않고 +1, +1, +1 진행
	// 컴파일 시에는 알 수 없는 멀티 스레드, 레지스터 매핑 등에 사용

	/* const 떼어내기 */
	func(i);
	cout << i << endl; // 20이 출력됨
	// i 자체가 const로 정의되어 있을 때는 사용할 때 매우 주의 필요! (undefined behavior)

	/* const_cast를 쓰는 좋은 예 */
	const String& s("abc");
	cout << s[0] << endl;
}

/*
	04. dynamic_cast
	- 부모 클래스 -> 자식 클래스 다운 캐스팅 (dynamic_cast)
	- 자식 클래스 -> 부모 클래스 업 캐스팅 (암시적)
*/
# include <iostream>
using namespace std;

class Parent
{
public:
	virtual ~Parent() {}
	// virtual이 없으면 main에서 child 소멸자 생성 안 함 (정적 바인딩)
	// virtual 함수가 없으면 다형 클래스가 아니라는 의미 -> dynamic cast 사용 불가
	// 상속을 했으면 소멸자가 필요하니 엥간해서는 virtual 함수가 있음
};

class Child : public Parent
{
public:
	void func()
	{
		cout << "func" << endl;
	}
};

class Child_ : public Parent
{};

class GrandChild : public Child, public Child_
{};

void foo(Parent* p)
{
	Child* child = dynamic_cast<Child*>(p);
	// 실제 p가 가리키는 타입이 child가 아니면 dynamic_cast가 null 반환
	// p를 reference형으로 받아오면 null 반환이 아니라 아예 에러 뱉음
	// -> 포인터로 사용하고 예외처리
	if (child != nullptr)
		child->func(); 
}

int main()
{
	Parent* p = new Child;
	// Parent* p = new Parent; 로 하면 foo 함수 내부의 child가 null이 됨
	foo(p);

	// Sibling 끼리의 cast // 
	GrandChild gc;
	Child* c = &gc;
	Child_* c_ = dynamic_cast<Child_*>(c); 
}

/*
	05. reinterpret_cast
	- 아예 관련없는 자료형으로 casting 가능
  - 데이터를 재해석 
*/
# include <iostream>
using namespace std;

int main()
{
	// 0000,0000,0000,0000,0000,0000,0000,0001
	unsigned int i = 1;
	float* a = reinterpret_cast<float*>(&i);

	cout << *a << endl; // FLT_TRUE_MIN. i의 배열을 float으로 재해석

	float b = i;
	cout << b << endl; // i값을 float 형태로 변환한 것
}

/*
	06. C 스타일/함수 스타일 캐스팅
	- CPP 창시자는 이 두 가지를 안 쓰는 것을 권장
*/
# include <iostream>
using namespace std;

enum class Type
{
	A, B, C
};

int main()
{
	int num0 = (int)Type::A; // C 스타일 캐스팅
	int num1 = int(Type::A); // 함수 스타일 캐스팅

	// C 스타일 캐스팅은 다양한 일을 할 수 있다! (dynamic cast는 X) //
	// 1) reinterpret cast
	int i = 10;
	float& f = (float&)i;

	// 2) const cast
	const int& j = i;
	int& k = (int&)j;

	// 3) static cast
	i = (int)Type::A;
}
