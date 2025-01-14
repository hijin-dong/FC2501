/*
  01. 산술 연산자 오버로딩 & Friend
  friend로 선언된 다른 클래스의 private 및 protected 멤버에 접근 가능
*/
# include <iostream>

using namespace std;

class Vector
{
public:
  float x;
  float y;
  float z;

  Vector operator+() const
    // 일반 포인터 -> const는 가능하지만 그 반대는 안 되니까 붙여줘서 나쁠게 없음!
  {
    return Vector{ x, y, z };
    // return *this;
  }

  Vector operator+(const Vector& v) const
  {
    return Vector{ x + v.x, y + v.y, z + v.z };
  }

  Vector& operator++() // ++a;
  {
    x++;
    y++;
    z++;
    return *this;
  }

  Vector operator++(int) // a++
  {
    Vector temp = *this;
    ++(*this);
    return temp;
    // 원본을 return한 이후에 ++해주는 느낌
  }

  void print()
  {
    cout << "x: " << x << " " << "y: " << y << " " << "z: " << z << endl;
  }
};

class Friend {
private:
  friend class Vector;
};

int main()
{
  Vector v0{ 0, 1, 2 };
  Vector v1{ 1, 2, 3 };

  Vector v2 = v0 + v1; // Vector v2 = v0.operator+(v1);
  Vector v3 = -v1; // Vector v3 = v1.operator-();
}

/*
  02. 연산자 오버로딩 (비교 & 관계)
*/
# include <iostream>
# include <cstring>
# include <compare>

using namespace std;

class String
{
private:
  char* _chars;

public:
  String(const char* chars) 
    : _chars(new char[strlen(chars) + 1])
  {
      strcpy(_chars, chars);
  }

  bool operator==(const String& s) const
  {
    return strcmp(_chars, s._chars) == 0;
  }

  bool operator!=(const String& s) const
  {
    return !(*this == s);
  }

  // 예전 C++에서는 >, <, ==, != 연산자들을 모두 정의해줘야했는데
  // Modern C++에서는 strong_ordering과 <=>로 모두 해결 가능
  strong_ordering operator<=>(const String& s) const
  {
    int result = strcmp(_chars, s._chars);
    if (result < 0)
      return strong_ordering::less;
    if (result > 0)
      return strong_ordering::greater;
    return strong_ordering::equal;
  }
};

int main()
{
  String s0("b");
  String s1("a");

  if ((s0 <=> s1) > 0) cout << "!!" << endl;
  // s1과 "b"를 비교하고 싶으면 클래스 내부에 friend 함수 만들어야!
}

/*
  03. 연산자 오버로딩 (논리 연산)
  AND나 OR 연산은 short evaluation인데
  friend 함수로 구현하면 인자를 둘 다 평가함.
  -> 기존의 논리 연산자와 특성이 달라지기에 꼭 구현이 필요한지 고민하자
*/

/*
  04. 연산자 오버로딩 (비트 연산)
  ~, &, |, ^, >>, <<
*/
# include <iostream>
# include <string> // stoi 함수 사용

using namespace std;

class Vector
{
private:
  int x;
  int y;
  int z;

public:
  Vector() {}
  Vector(int x, int y, int z) : x(x), y(y), z(z)
  {}

  friend ostream& operator << (ostream& os, const Vector& v)
  {
    os << v.x << " " << v.y << " " << v.z << " ";
    return os;
    // void를 리턴하면 cout << v 까지만 진행가능
  }

  friend istream& operator>>(istream& is, Vector& v)
  {
    string temp;

    is >> temp;
    v.x = stoi(temp);

    is >> temp;
    v.y = stoi(temp);

    is >> temp;
    v.z = stoi(temp);

    return is;
  }

  // ~ 연산
  Vector operator~() const
  {
    return Vector(~x, ~y, ~z);
  }

  // & 연산 (&는 앞의 조건식이 false여도 뒤의 조건식을 확인)
  // 조건이 null일 수 있거나.. 한 상황 등에 활용 가능
  // | 도 마찬가지임!
  Vector operator&(const Vector& v) const
  {
    return Vector(x & v.x, y & v.y, z & v.z);
  }

  Vector operator<<(int v) const
  {
    return Vector(x << v, y << v, z << v);
  }
};

int main()
{
  const Vector v{ 1,2,3 };
  cout << ~v << endl;
}

/*
  05. 연산자 오버로딩 (첨자 연산자)
  nums[1] <- 이런 대괄호 연산을 첨자 연산자라고 함!
*/
# include <iostream>
# include <cstring>
# include <vector>

using namespace std;

class Vector
{
public:
  float x, y, z;

  Vector(float x, float y, float z) : x(x), y(y), z(z)
  {}

  float& operator[](int index)
  {
    if (index < 1) return x;
    if (index == 1) return y;
    else return z;
  }
};

class String
{
private:
  char* _chars;

public:
  String(const char* chars) : _chars(new char[strlen(chars + 1)])
  {
    strcpy(_chars, chars);
  }

  char& operator[](int index)
  {
    return _chars[index];
  }

  friend ostream& operator<<(ostream& os, const String& s)
  {
    return os << s._chars;
  }
};

using Key = string;
using Value = string;
using KeyValue = pair<string, string>;

class Bucket
{
private:
  vector<KeyValue> _items;

public:
  Value& get(const Key& key)
  {
    for (KeyValue& keyValue : _items)
    {
      if (keyValue.first == key)
      {
        return keyValue.second;
      }
    }
    _items.push_back(std::make_pair(key, Value()));
    return _items.back().second;
  }
};

class HashTable
{
private:
  vector<Bucket> _buckets;

  int hash(const Key& key) const
  {
    int h = 0;
    for (char ch : key)
    {
      h += ch;
    }
    return h;
  }

public:
  HashTable(int size = 100) : _buckets(vector<Bucket>(size))
  {}

  Value& operator[](Key key)
  {
    int bucketIndex = hash(key) % _buckets.size();
    Bucket& bucket = _buckets[bucketIndex];
    return bucket.get(key);
  }
};

int main()
{
  Vector v{ 1, 2, 3 };
  v[1] = 100;
  // [] operator가 float를 리턴하기 때문에 동작하지 않음
  // float&를 리턴하도록 수정하면 가능하지요!

  String s = "huhu";
  s[0] = 'H';
  cout << s << endl;

  HashTable hashTable;
  hashTable["abc"] = "def";
  hashTable["cba"] = "123";

  cout << hashTable["abc"] << endl;
  cout << hashTable["cba"] << endl;
}

/*
  06. 연산자 오버로딩 (대입 연산자, 복사 생성자)
*/
# include <iostream>
# include <cstring>

using namespace std;

class Person
{
private:
  float _weight;
  float _height;
  //char _name[10];
  char* _name;

public:
  Person() { _name = nullptr; }
  Person(float weight, float height, const char* name)
    : _weight(weight), _height(height), _name(new char[strlen(name) + 1])
  {
    strcpy(_name, name);
  }
  // 셋의 Name이 가리키는 char의 주소값이 동일하기 때문에
  // 메모리 해지할 때 문제가 생김 (이미 delete한 것을 또 접근해서 지우게 됨)
  // 얕은 복사를 해서 생기는 문제! -> 아래의 복사 생성자, 대입 연산자 오버로딩으로 해결

  Person(Person& person) // 복사 생성자
    : Person(person._weight, person._height, person._name)
  {
    cout << "It is Copy!" << endl;
  }

  Person& operator=(const Person& person) // 대입 연산자
  {
    _weight = person._weight;
    _height = person._height;
    if (_person != nullptr) delete[] _name;

    _name = new char[strlen(person._name) + 1];
    strcpy(_name, person._name);

    return *this;
  }

  ~Person()
  {
    delete[] _name;
  }

  void print() const 
  {
    cout << _name << endl;
    cout << _weight << endl;
    cout << _height << endl;
    cout << "==========" << endl;
  }
};

void func(Person person)
{}

int main()
{
  Person person0{ 45.f, 153.f, "Json" };
  Person person1 = person0; // 복사 생성자. Person person1(person0); 
  Person person2;
  person2 = person0; // 대입 연산자, 할당 연산자

  person0.print();
  person1.print();
  person2.print();

  func(person2); // 호출해보면 복사가 일어나는 것을 알 수 있음!
}

/*
  07. 연산자 오버로딩 (변환 생성자, explicit)
*/
# include <iostream>

using namespace std;

class String
{
public:
  explicit String(const char* chars) : _chars(new char[strlen(chars) + 1])
  {
    strcpy(_chars, chars);
  }

  explicit String(const String& s)
  {}
  explicit String(std::initializer_list<const char*> strs)
  {
    for (const char* str : strs) cout << str << endl;
  }
  // if (s) <- 이런 것 하고 싶음!
  explicit operator bool() const
  {
    return strlen(_chars) > 0;
  }

private:
  char* _chars;
};

int main()
{
  // String s = "abc"; 암시적 형변환
  String s = String("abc"); // cf. C스타일: (String)"abc"
  String s1("abc");

  // String s1 = s; (안됨)
  s1 = String(s);
  String s2(s); // String 표시 안 해주면 안됨. 기존에 있던 s1(s)도 안됨
  // func(s2); 안됨.

  String s3 { "1", "2", "3", "4"};
  // String s3 = { "1", "2", "3", "4"}; 안됨

  if (s)
  {
    bool result = (bool)s;
    bool result1 = !s;
  }
}

/*
  08. 연산자 오버로딩 (호출 연산자 오버로딩, 함수 객체)
  함수 func() <- 저 괄호를 호출 연산자라고 함!
*/
# include <iostream>
# include <algorithm>
# include <numeric>

using namespace std;

class Max
{
private:
  int _max = numeric_limits<int>::min();

public:
  int operator()(int x, int y) // 기존 함수
  {
    return max(x, y);
  }

  int operator()(int x) // 커스텀
  {
    return _max = max(x, _max);
  }
};

// Struct로도 가능
// Struct와 Class는 무엇이 다른가 -> 기본 접근 제어가 각각 public, private
struct Print
{
public:
  void operator()(int v) const
  {
    cout << v << endl;
  }
};

int main()
{
  Max mx;
  cout << "Max: " << mx(1, 2) << endl;
  // 일반 함수와의 차이: 클래스이기 때문에 내부에 상태를 저장할 수 있음!

  Print p;
  int nums[] = { 1,2,3,4,5 };
  for (int num : nums) p(num);
}

/*
  09. 연산자 오버로딩 (사용자 정의 리터럴)
  123.f <- 처럼 붙이는 접미사를 리터럴이라고 함
  사용자 정의 리터럴은 앞에 _를 붙여줘야함
*/
# include <iostream>
# include <chrono>

using namespace std;

class Length
{
private:
    const long double _value;
    Length(long double value) : _value(value)
        // 생성자로 만드는 것은 허용하지 않고 리터럴로만 쓰도록!
        // private로 만들었기 때문에 friend함수가 필요
    {}

    // friend는 public/private 상관없음
    // friend라는 사실만 중요
    friend Length operator"" _m(unsigned long long value);
    friend Length operator"" _km(unsigned long long value);

    // friend Length operator"" _m(long double value); 이것도 정의해주면 1.0_m 이런 실수도 사용 가능!

public:
    long double m() const { return _value; }
    long double km() const { return _value * 0.001; }

    Length operator-() const
    {
        return Length(-_value);
    }

    Length operator+(const Length& length) const
    {
        return Length(_value + length._value);
    }

};

Length operator"" _m(unsigned long long value)
// class 안에서 정의까지 마쳐야 하는데 ms VS 에서는 오류가..
{
    return Length(value);
}

Length operator"" _km(unsigned long long value)
// class 안에서 정의까지 마쳐야 하는데 ms VS 에서는 오류가..
{
    return Length(value * 1000);
}

int main()
{
    chrono::minutes m = 24h + 5min;
    // h나 min의 정의로 이동해보자!

    Length len = -1_m + 1_km;
    cout << len.m() << endl;
}
