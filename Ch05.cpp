/*
	01. 전통적인 예외처리
*/
# include <iostream>
using namespace std;

int main()
{
	// 1. 종료하기
	abort();
	
	// 2. 문제가 있음을 알리기
	exit(123); // 에러코드
	bool r = cout.fail(); // 문제 여부 확인
	 
	// 3. 기타..
}

/*
	02. try-catch
*/
# include <iostream>
using namespace std;

class Parent {};
class Child : public Parent {};

class DivideException : public exception
{
public:
	virtual const char* what() const override
	{
		return "Divided by Zero";
	}
};

double divide(double d, double v)
{
	if (v == 0)
	{
		//(1)
		// throw "Divided by zero";
		// 내부적으로 std::terminate() 가 호출됨
		// abort()와 달리 종료 상황을 세팅할 수 있음

		// (2)
		// throw Child();
		// parent 출력됨

		// (3) 커스텀 exception
		throw DivideException();
	}
	return d / v;
}

void func1()
{
	divide(10, 0);
	cout << "func1" << endl;
}

void func()
{
	func1();
	cout << "func" << endl;

}

void terminateFunc()
{
	cout << "Terminated" << endl;
	exit(77);
}

int main()
{
	set_terminate(terminateFunc);
	try
	{
		cout << divide(10, 0) << endl;
	}
	catch (const char* e)
	{
		cout << e << endl;
		// 터미널에 Divided by zero 출력
	}
	catch (Parent&)
	{
		cout << "Parent" << endl;
	}
	catch (Child&)
	{
		cout << "Child" << endl;
	}
	catch (exception& e) // 무슨 에러든지 잡아줌
	{
		cout << e.what() << endl;
	}

	// throw는 throw 호출 지점 ~ catch가 있는 지점까지의 호출 스택을 모두 삭제
	try
	{
		func();
	}
	catch (const char* e)
	{
		cout << e << endl;
	}
}

// main 함수 자체를 try - catch 문으로 작성하는 것도 가능
int main() try
{
	// do something
}
catch (int i) {
	// catch something
}

// 정의된 에러 가져다 쓰기
# include <stdexcept>
int main()
{
	std::logic_error e;
	std::runtime_error q;
	std::out_of_range o;
}

/*
	03. RAII
	Resource acquisition is initialization
*/
# include <iostream>
using namespace std;

class RAII
{
public:
	int* i;
	RAII()
	{
		i = new int;
	}
	~RAII()
	{
		delete i;
	}
};

int main()
{
	// (AS-IS) 동적 할당을 했을 경우에 모든 예외마다 delete를 해야하는데..
	int* i = nullptr;
	try
	{
		i = new int;
		delete i;
	}
	catch (out_of_range)
	{
		delete i;
	}
	catch (runtime_error)
	{
		delete i;
	}

	// (TO-BE) 소멸자 호출 시에 delete
	try
	{
		RAII raii;
	}
	catch (const char* e)
	{
		cout << e << endl;
	}
}

/*
	04. noexcept
*/
# include <iostream>
using namespace std;

// 1) main 함수에 붙일 수 있음
// 소멸자와 같은 메모리 관련 함수들에는 기본적으로 붙어있는 경우 많음 
int main() noexcept
{
  throw 1;
  // abort됨
}

// 2) 함수 정의 및 try-catch 문과 사용
// 예시의 경우, 마찬가지로 abort
void func() noexcept
{
  throw 1;
}

int main() // noexcept(noexcept(func())) // 주석 해제하고 위의 키워드와 함께 사용해도 동일한 결과
{
  try
  {
    func();
  }
  catch (int e)
  {
    cout << e << endl;
  }
}
