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
    return 20;
}

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
}
