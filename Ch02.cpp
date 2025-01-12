/*
  01. ��� ������ �����ε� & Friend
  friend�� ����� �ٸ� Ŭ������ private �� protected ����� ���� ����
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
    // �Ϲ� ������ -> const�� ���������� �� �ݴ�� �� �Ǵϱ� �ٿ��༭ ���ܰ� ����!
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
    // ������ return�� ���Ŀ� ++���ִ� ����
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
  02. ������ �����ε� (�� & ����)
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

  // ���� C++������ >, <, ==, != �����ڵ��� ��� ����������ߴµ�
  // Modern C++������ strong_ordering�� <=>�� ��� �ذ� ����
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
  // s1�� "b"�� ���ϰ� ������ Ŭ���� ���ο� friend �Լ� ������!
}

/*
  03. ������ �����ε� (�� ����)
  AND�� OR ������ short evaluation�ε�
  friend �Լ��� �����ϸ� ���ڸ� �� �� ����.
  -> ������ �� �����ڿ� Ư���� �޶����⿡ �� ������ �ʿ����� �������
*/

/*
  04. ������ �����ε� (��Ʈ ����)
  ~, &, |, ^, >>, <<
*/
# include <iostream>
# include <string> // stoi �Լ� ���

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
    // void�� �����ϸ� cout << v ������ ���డ��
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

  // ~ ����
  Vector operator~() const
  {
    return Vector(~x, ~y, ~z);
  }

  // & ���� (&�� ���� ���ǽ��� false���� ���� ���ǽ��� Ȯ��)
  // ������ null�� �� �ְų�.. �� ��Ȳ � Ȱ�� ����
  // | �� ����������!
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
  05. ������ �����ε� (÷�� ������)
  nums[1] <- �̷� ���ȣ ������ ÷�� �����ڶ�� ��!
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
  // [] operator�� float�� �����ϱ� ������ �������� ����
  // float&�� �����ϵ��� �����ϸ� ����������!

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
  06. ������ �����ε� (���� ������, ���� ������)
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
  Person() {}
  Person(float weight, float height, const char* name)
    //: _weight(weight), _height(height), _name{}
    : _weight(weight), _height(height), _name(new char[strlen(name) + 1])
  {
    strcpy(_name, name);
  }
  // ���� Name�� ����Ű�� char�� �ּҰ��� �����ϱ� ������
  // �޸� ������ �� ������ ���� (�̹� delete�� ���� �� �����ؼ� ����� ��)
  // ���� ���縦 �ؼ� ����� ����!

  Person(Person& person)
    : Person(person._weight, person._height, person._name)
  {
    cout << "It is Copy!" << endl;
  }

  Person& operator=(const Person& person)
  {
    _weight = person._weight;
    _height = person._height;
    // delete[] _name;

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
  Person person1 = person0; // ���� ������
  Person person2;
  person2 = person0; // ���� ������, �Ҵ� ������

  person0.print();
  person1.print();
  person2.print();

  func(person2); // ȣ���غ��� ���簡 �Ͼ�� ���� �� �� ����!
}

/*
  07. ������ �����ε� (��ȯ ������, explicit)
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
  // if (s) <- �̷� �� �ϰ� ����!
  explicit operator bool() const
  {
    return strlen(_chars) > 0;
  }

private:
  char* _chars;
};

int main()
{
  // String s = "abc"; �Ͻ��� ����ȯ
  String s = String("abc"); // cf. C��Ÿ��: (String)"abc"
  String s1("abc");

  // String s1 = s; (�ȵ�)
  s1 = String(s);
  String s2(s); // String ǥ�� �� ���ָ� �ȵ�. ������ �ִ� s1(s)�� �ȵ�
  // func(s2); �ȵ�.

  String s3 { "1", "2", "3", "4"};
  // String s3 = { "1", "2", "3", "4"}; �ȵ�

  if (s)
  {
    bool result = (bool)s;
    bool result1 = !s;
  }
}

/*
  08. ������ �����ε� (ȣ�� ������ �����ε�, �Լ� ��ü)
  �Լ� func() <- �� ��ȣ�� ȣ�� �����ڶ�� ��!
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
  int operator()(int x, int y) // ���� �Լ�
  {
    return max(x, y);
  }

  int operator()(int x) // Ŀ����
  {
    return _max = max(x, _max);
  }
};

// Struct�ε� ����
// Struct�� Class�� ������ �ٸ��� -> �⺻ ���� ��� ���� public, private
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
  // �Ϲ� �Լ����� ����: Ŭ�����̱� ������ ���ο� ���¸� ������ �� ����!

  Print p;
  int nums[] = { 1,2,3,4,5 };
  for (int num : nums) p(num);
}

/*
  09. ������ �����ε� (����� ���� ���ͷ�)
  123.f <- ó�� ���̴� ���̻縦 ���ͷ��̶�� ��
  ����� ���� ���ͷ��� �տ� _�� �ٿ������
*/
# include <iostream>
# include <chrono>

using namespace std;

class Length
{
private:
    const long double _value;
    Length(long double value) : _value(value)
        // �����ڷ� ����� ���� ������� �ʰ� ���ͷ��θ� ������!
        // private�� ������� ������ friend�Լ��� �ʿ�
    {}

    // friend�� public/private �������
    // friend��� ��Ǹ� �߿�
    friend Length operator"" _m(unsigned long long value);
    friend Length operator"" _km(unsigned long long value);

    // friend Length operator"" _m(long double value); �̰͵� �������ָ� 1.0_m �̷� �Ǽ��� ��� ����!

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
// class �ȿ��� ���Ǳ��� ���ľ� �ϴµ� ms VS ������ ������..
{
    return Length(value);
}

Length operator"" _km(unsigned long long value)
// class �ȿ��� ���Ǳ��� ���ľ� �ϴµ� ms VS ������ ������..
{
    return Length(value * 1000);
}

int main()
{
    chrono::minutes m = 24h + 5min;
    // h�� min�� ���Ƿ� �̵��غ���!

    Length len = -1_m + 1_km;
    cout << len.m() << endl;
}
