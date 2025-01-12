/*
  01. ����� �⺻
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

protected: // �ڱ� �ڽĿ��Ը� ����!
  int shared;
};

class Derived : public Base
  // public �� �ٿ��� ����� ��
  // �⺻������ parameter ���� �⺻ �����ڸ� �θ� �ν��Ͻ��� ȣ��
{
  using Base::Base; // �θ� Ŭ������ ������ (int num)�� �޾ƿ� �� ����

public:
  Derived() : Base(20)
  {
    cout << "I am Derived" << endl;
  }
  Derived(int num)
  {
    shared = num;
    // ������ ������ �θ� Ŭ������ �ִ� �༮�� �ڽ� Ŭ�������� �ʱ�ȭ�ϴ� ���� ���� ����
    // �θ�Ŭ������ �ϵ��� �����ϴ� ���� ����
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
  // private�� num���� ������ �� ������ func()�� ��밡��
  // ��� �� func�� Derived Ŭ������ �����ϴ� ���� �Ұ��� (int ������ �ȵǴϱ�)
  // d ��ü �ȿ� Base ��ü�� ������
  // 
  // ������ �θ� �ν��Ͻ� ���� �� -> �ڽ� �ν��Ͻ� ����
  // ���� ������ �ڽ� �ν��Ͻ� ���� �� -> �θ� �ν��Ͻ� ����

  Base* b = new Derived;
  b->func();
  delete b; // Derived �Ҹ��ڸ� ȣ���. �θ�Ŭ���� �Ҹ��ڸ� virtual ~Base()�� �����ؾ�!

  //Derived* d = new Base; // �Ұ�!
  // Derived���� �ְ� Base���� ���� �Լ��� ������ �� ���ݾ�!
}

/*
  02. ���� �Լ�
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
    Base::func(); // �������̵��� ���� �̸� & �ٸ� Ŭ���� �Լ� ���� ������ �̷���!
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
  d.func(); // Derived. �������̵� (���� Ŭ������ �޼��带 ���� Ŭ������ �������ؼ� ���)

  // �θ� Ŭ������ func�� virtual�� �ٿ��ָ� Derived�� ����!
  // virtual�� ������ �ν��Ͻ��� type�� ���� � �Լ��� ȣ������ ����
  // virtual func() override <- �̷������� �ڿ� override�� �ٿ��ָ�
  // Ȥ�� �Լ� �̸��� ��Ÿ ������ �Ǽ��� ������! (virtual fun() ���� �ص� �������� ��..)
  // �����Լ��� �Ƿ��� ����� �����ؾ� �� (�μ�����!) + �����ε� �� ��
  Base& b0 = d;
  b0.func(); // Base

  Base* b1 = &d;
  b1->func(); // Base

  foo(d); //Base
  /////

  Base* b2 = new Derived;
  delete b2;
  // Base �Ҹ��ڿ� virtual ������ �ν��Ͻ��� type�� ���� �θ� �Ҹ��ڸ� ȣ���ع���
  // -> �ڳ� �Ҹ��ڰ� ���������� ȣ����� �ʾ� �޸� ���� �߻�
}

/*
  03. ���� ����, ���� ����
  ����(static): �Ϲ������� "������ ��"��� �ǹ� -> ex. �����ε� (asm ���� ���� �Լ� �̸��� �ٸ�)
  ����(dynamic): �Ϲ������� "��Ÿ�� ��"��� �ǹ� -> ex. �������̵� (ȣ�� ��ü Ÿ���� Ȯ��)
      virtual ������ ���� ���ε�
      �����̱� ������ ����� �߿� �� �ٲ��ָ� �ٲ� �Լ��� �����
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
  b.num = x.num + y.num;
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
  cout << a2.num << endl; // 30�� ��µ�.
}

/*
  04. �߻� Ŭ����, ���� ���� �Լ�
  - �߻� Ŭ����: 1�� �̻��� ���� ���� �Լ��� �ִ� Ŭ����, �Ҹ��ڰ� virtual�� ������� �־�� ��
    ������ ���� �Լ��� �ִ� �߻� Ŭ���� = �������̽� ()
  - �ν��Ͻ�ȭ�ϱ�� ������ ���� �����Լ��� ���� ��쿡�� virtual �Ҹ���() = 0;
*/
# include <iostream>

using namespace std;

class Shape
{
public:
  // �������� ���̸� ���ϴ� ����� �ٸ��� ������ �� ������ �����ؼ� ����ؾ� ��
  virtual double getArea() const = 0;
  // �Ҹ��ڸ� virtual�� ��������� �޸� ���� ���� ����
  virtual ~Shape() {}
};

class Circle : public Shape
{
private:
  double _radius;
public:
  Circle(double radius) : _radius(radius) {}

  // ���� �����Լ��� �������̵����� ������ Circle�� �ν��Ͻ�ȭ �Ұ� (�߻� Ŭ������ ��)
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
  // �߻�Ŭ������ �ν��Ͻ��δ� ��� �Ұ�, ������/���۷����� ��� ����
  // ��� ��: ShapeCalculator �� ���� Shape ���ݿ� ����Ǵ� �Լ������!
  Circle c(10);
  Square s(10, 10);

  ShapeCalculator sc;
  cout << sc(c) << endl;
  cout << sc(s) << endl;
}

/*
  05. private / protected ���
  : ����ϴ� ��ʰ� ���� ������ �� ���� ����� �ʿ�
*/
# include <iostream>
# include <vector>

using namespace std;

class Queue : private vector<int>
{
  // pravate: vector<int> v <= �̷��� ���� �Ͱ� ���� ���� ����, �̰� ��� ����
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

  // ��� Ű���尡 public�϶��� private�϶��� ���̴�
  // queue. <- ���� �Է��غ��� ������ �Լ�/�Ӽ��� �ٸ�
}

/*
  06. ���� ���
  : �����ؼ� ����ؾ� �ؿ�!
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

class BaseA : virtual public Base // virtual �� �ٿ��ָ� ���� Base ������
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