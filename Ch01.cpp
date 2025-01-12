/*
  01. ��ü������ �⺻
*/
# include <iostream>
# include <string>

using namespace std;

class Player
{
private:
  int _health;
  int _power;
  const string _name;

public:
  Player() : Player(100, 50, "anonymous")
  {
    cout << _name << "(" << _health << ", " << _power << ")�� �����Ǿ����ϴ�!" << endl;
  }
  ~Player()
  {
    cout << _name << "(" << _health << ", " << _power << ")�� �Ҹ�Ǿ����ϴ�." << endl;
  }
  Player(int health, int power, string name)
    : _health(health), _power(power), _name(name)
  {
    cout << _name << "(" << _health << ", " << _power << ")�� �����Ǿ����ϴ�." << endl;
  }

  void attack(Player& target)
  {
    cout << _name << " attacks " << target._name << endl;
    target.damaged(_power);
  }

  void damaged(int power)
  {
    _health -= power;
    if (_health < -0) cout << "Died" << endl;
  }

  void print()
  {
    cout << _name << _health << _power << endl;
  }
};

int main()
{
  Player player0 = Player();
  Player player1 = Player(100, 40, "Jill");
  Player* player2 = new Player(100, 30, "Jake");
}


/*
  02. ��ü ������ �⺻ (2)
*/
# include <iostream>
# include <cstring>

using namespace std;

class String
{
private:
  char* _str;

public:
  String(const char* str)
  {
    int len = strlen(str);
    _str = new char[len + 1];
    strcpy(_str, str);
  }

  ~String()
  {
    delete[] _str;
  }

  void print()
  {
    cout << _str << endl;
  }
};

int main()
{
  String str("abc");
  String str1 = String("huhu");
  int a(1);
  str.print();
}


/*
  03. ����� ����
  Person.h�� Person.cpp ����
*/
# include "Person.h"

int main()
{
 Person huhu(100, 100, "huhu");
 huhu.print();
}

/*
  04. ����� ���� (2)
  Village.h�� Village.cpp ����
*/

/*
  05. this ������
*/
# include <iostream>

using namespace std;

class Person
{
private:
  const string _name;
  float _weight;
  float _height;

public:
  Person(const string& name, float weight, float height) 
    : _name(name), _weight(weight), _height(height)
  {}

  float getBMI()
  {
    return _weight / (_height * _height * 10000);
  }

  Person& compete(Person& person)
  {
    if (this->getBMI() < person.getBMI()) return *this;
    else return person;
  }

  void doCeremony()
  {
    cout << _name << " Wins!!!" << endl;
  }
};

int main()
{
  Person person("Jack", 67.3f, 172.3f);
  Person person1("Jill", 58.2f, 167.3f);
  person.compete(person1).doCeremony();
}

/*
  06. this ������ (2)
*/
# include <iostream>

using namespace std;

struct Transaction
{
  const int txID;
  const int fromID;
  const int toID;
  const int value;

  class Builder
  {
  private:
    int _fromID;
    int _toID;
    int _value;

  public:
    Transaction build()
    {
      int txID = _fromID ^ _toID ^ _value;
      return Transaction{ txID, _fromID, _toID, _value };
    }

    Builder& setFromID(int fromID)
    {
      _fromID = fromID;
      return *this;
    }

    Builder& setToID(int toID)
    {
      _toID = toID;
      return *this;
    }

    Builder& setValue(int value)
    {
      _value = value;
      return *this;
    }
  }; // class
}; // structure

int main()
{
  Transaction tx = Transaction::Builder()
    .setFromID(1212)
    .setToID(4321)
    .setValue(4321)
    .build();
}

/*
  07. const
*/
# include <iostream>

using namespace std;

class Person
{
private:
  const string _name; // ������ ���� const�� �ʱ� �� ���� ���� ����
  float _weight;
  float _height;

public:
  Person(const string& name, float weight, float height) 
    : _name(name), _weight(weight), _height(height)
  {}

  float getWeight(/* const Person* this */) const
  {
    // _weight -= 1; �� ���� �Ұ�
    return _weight;
  }
};

int main()
{
  const Person* person0 = new Person("Dan", 56.f, 174.f);
  // Person* person1 = person0;
  // �Ϲ� �����͸� const �����ͷ� ���� �� ������,
  // const �����͸� �Ϲ� �����ͷ� ���� ���� ����
}

/*
  08. Ŭ������ ���� ���
  ��� ����, �Լ� �տ� static�� ���̸� ���� ����� ��
  1) ���� ������ ���� �� (���α׷� ���� �� �޸� �Ҵ� 1ȸ, ���α׷� ���� �� ����)
*/
# include <iostream>

using namespace std;

int main()
{
  for (int i = 0; i < 10; i++)
  {
    int autoVar = 0;
    autoVar++;

    static int staticVar = 0;
    staticVar++;

    // ����ϸ� autoVar�� �Ź� 1�ε�, staticVar�� ������
  }
}

/*
  09. Ŭ������ ���� ��� (2)
  StaticPerson Ŭ���� ����
*/
# include <iostream>
# include "StaticPerson.h"

using namespace std;

int main()
{
  StaticPerson p0;
  StaticPerson p1;

  p0.print();
  p1.print();

  StaticPerson::print();
}

/*
  10. ��� �Լ� ������
*/
# include <iostream>
# include <functional>

using namespace std;

class Person
{
public:
  void print(int i)
  {
    cout << i << endl;
  }
};

int main()
{
  //void (Person:: * fn)(int) = &Person::print;
  
  // (1)
  //typedef void (Person::* FuncType)(int);
  
  // (2)
  //using FuncType = void (Person::*)(int);
  //
  //FuncType fn = &Person::print;

  //Person person;
  //(person.*fn)(1);

  function<void(Person*, int)> func = &Person::print;
  Person person;
  func(&person, 1);
}