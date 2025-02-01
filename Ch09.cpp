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
