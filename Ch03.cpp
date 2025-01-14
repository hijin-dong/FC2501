/*
  01. 상속의 기본
*/
# include <iostream>

using namespace std;

class Base
{
private:
  int num; 

public:
  Base() : num(0)
  {
    cout << "I am Basic Base" << endl;
  }
  Base(int num) : num(num)
  {
    cout << "I am num setting Base" << endl;
  }
  ~Base()
  {
    cout << "Delete Base" << endl;
  }

  void func()
  {
    cout << num << endl;
  }

protected: // 자기 자식에게만 공개!
  int shared;
};

class Derived : public Base
  // public 안 붙여도 상속은 됨
  // 기본적으로 parameter 없는 기본 생성자를 부모 인스턴스로 호출
{
  using Base::Base; // 부모 클래스의 생성자 (int num)을 받아올 수 있음

public:
  Derived() : Base(20)
  {
    cout << "I am Derived" << endl;
  }
  Derived(int num)
  {
    shared = num;
    // 가능은 하지만 부모 클래스에 있는 녀석을 자식 클래스에서 초기화하는 것은 좋지 않음
    // 부모클래스가 하도록 위임하는 것이 좋음
  }
  ~Derived()
  {
    cout << "Delete Derived" << endl;
  }
};

int main()
{
  Derived d(20);
  d.func();
  // private인 num에는 접근할 수 없지만 func()는 사용가능
  // 대신 이 func를 Derived 클래스에 정의하는 것은 불가능 (int 접근이 안되니까)
  // d 객체 안에 Base 객체가 생성됨
  // 
  // 순서는 부모 인스턴스 생성 후 -> 자식 인스턴스 생성
  // 삭제 순서는 자식 인스턴스 삭제 후 -> 부모 인스턴스 삭제

  Base* b = new Derived;
  b->func();
  delete b; // Derived 소멸자만 호출됨. 부모클래스 소멸자를 virtual ~Base()로 수정해야!

  //Derived* d = new Base; // 불가!
  // Derived에는 있고 Base에는 없는 함수를 실행할 수 없잖아!
}

/*
  02. 가상 함수
*/
# include <iostream>

using namespace std;

class Base
{
public:
  void func()
  {
    cout << "Base" << endl;
  }

  virtual ~Base()
  {
    cout << "~Base" << endl;

  }
};

class Derived : public Base
{
public:
  void func()
  {
    Base::func(); // 오버라이딩한 같은 이름 & 다른 클래스 함수 쓰고 싶으면 이렇게!
    cout << "Derived" << endl;
  }

  virtual ~Derived()
  {
    cout << "~Derived" << endl;

  }
};

void foo(Base& base)
{
  base.func();
}

int main()
{
  Base b;
  Derived d;

  b.func(); // Base
  d.func(); // Derived. 오버라이딩 (상위 클래스의 메서드를 하위 클래스가 재정의해서 사용)

  // 부모 클래스의 func에 virtual을 붙여주면 Derived가 나옴!
  // virtual이 없으면 인스턴스의 type만 보고 어떤 함수를 호출할지 결정
  // virtual func() override <- 이런식으로 뒤에 override를 붙여주면
  // 혹시 함수 이름을 오타 내더라도 실수를 막아줌! (virtual fun() 으로 해도 컴파일이 됨..)
  // 가상함수가 되려면 모양이 동일해야 함 (인수까지!) + 오버로딩 안 됨
  Base& b0 = d;
  b0.func(); // Base

  Base* b1 = &d;
  b1->func(); // Base

  foo(d); //Base
  /////

  Base* b2 = new Derived;
  delete b2;
  // Base 소멸자에 virtual 없으면 인스턴스의 type만 보고 부모 소멸자만 호출해버림
  // -> 자녀 소멸자가 정상적으로 호출되지 않아 메모리 누수 발생
}

/*
  03. 정적 결합, 동적 결합
  정적(static): 일반적으로 "컴파일 시"라는 의미 -> ex. 오버로딩 (asm 파일 뜯어보면 함수 이름이 다름)
  동적(dynamic): 일반적으로 "런타임 시"라는 의미 -> ex. 오버라이딩 (호출 객체 타입을 확인)
      virtual 없으면 정적 바인딩
      동적이기 때문에 디버깅 중에 값 바꿔주면 바뀐 함수로 적용됨
*/
# include <iostream>

using namespace std;

class A
{
public:
  int num;
};

class B : public A
{};

A operator+(const A& x, const A& y)
{
  A a;
  a.num = x.num + y.num;
  return a;
}

B operator+(const B& x, const B& y)
{
  B b;
  b.num = x.num * y.num;
  return b;
}

int main()
{
  B b0, b1;
  b0.num = 10;
  b1.num = 20;

  A& a0 = b0;
  A& a1 = b1;

  A a2 = a0 + a1;
  cout << a2.num << endl; // 30이 출력됨.
}

/*
  04. 추상 클래스, 순수 가상 함수
  - 추상 클래스: 1개 이상의 순수 가상 함수가 있는 클래스, 소멸자가 virtual로 만들어져 있어야 함
    ㄴ순수 가상 함수만 있는 추상 클래스 = 인터페이스 ()
  - 인스턴스화하기는 싫은데 만들 가상함수는 없을 경우에는 virtual 소멸자() = 0;
*/
# include <iostream>

using namespace std;

class Shape
{
public:
  // 도형마다 넓이를 구하는 방식이 다르기 때문에 각 도형이 정의해서 사용해야 함
  virtual double getArea() const = 0;
  // 소멸자를 virtual로 생성해줘야 메모리 누수 방지 가능
  virtual ~Shape() {}
};

class Circle : public Shape
{
private:
  double _radius;
public:
  Circle(double radius) : _radius(radius) {}

  // 순수 가상함수를 오버라이딩하지 않으면 Circle도 인스턴스화 불가 (추상 클래스가 됨)
  virtual double getArea() const override
  {
    return _radius * _radius * 3.14;
  }
};

class Square : public Shape
{
private:
  double _width;
  double _height;
public:
  Square(double width, double height) : _width(width), _height(height) {}

  virtual double getArea() const override
  {
    return _width * _height;
  }
};

class ShapeCalculator
{
private:
  double _total = 0;
  int _size = 0;

public:
  double operator()(Shape& shape)
  {
    _size += 1;
    _total += shape.getArea();
    return _total / _size;
  }
};

int main()
{
  // 추상클래스는 인스턴스로는 사용 불가, 포인터/레퍼런스로 사용 가능
  // 사용 예: ShapeCalculator 과 같은 Shape 전반에 적용되는 함수라던가!
  Circle c(10);
  Square s(10, 10);

  ShapeCalculator sc;
  cout << sc(c) << endl;
  cout << sc(s) << endl;
}

/*
  05. private / protected 상속
  : 사용하는 사례가 많지 않으니 쓸 때는 고민이 필요
*/
# include <iostream>
# include <vector>

using namespace std;

class Queue : private vector<int>
{
  // pravate: vector<int> v <= 이렇게 쓰는 것과 거의 차이 없고, 이거 사용 권장
public:
  void push(int value) { push_back(value); }
  void pop() { pop_back(); }
  int top() { return back(); }
};

int main()
{
  Queue queue;
  queue.push(10);
  cout << queue.top() << endl;
  queue.push(20);
  cout << queue.top() << endl;
  queue.pop();
  cout << queue.top() << endl;

  // 상속 키워드가 public일때와 private일때의 차이는
  // queue. <- 까지 입력해보면 나오는 함수/속성이 다름
}

/*
  06. 다중 상속
  : 조심해서 사용해야 해요!
*/
# include <iostream>

using namespace std;

class Base
{
public:
  int m = 10;
  Base() {}
  Base(int m) : m(m) { cout << "Base(" << m << ")" << endl; }
};

class BaseA : virtual public Base // virtual 안 붙여주면 각자 Base 생성함
{
public:
  BaseA() : Base(10) { cout << "Base A" << endl; }
  void foo() {}
};

class BaseB : virtual public Base 
{
public:
  BaseB() : Base(20) { cout << "Base B" << endl; }
  void foo() {}
};

class Derived : public BaseA, public BaseB
{
  using BaseA::foo;

public:
  Derived() { cout << "Derived" << endl; }
};

int main()
{
  Derived d;
  d.BaseA::foo();

  BaseA& aa = d;
  aa.foo(); // BaseA::foo()
}
