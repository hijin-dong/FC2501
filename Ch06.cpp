
/*
  01. 함수 템플릿
*/
# include <iostream>
using namespace std;

void swap(int& x, int& y) // type마다 함수를 만들어줘야 하는데..!
{
  int temp = x;
  x = y;
  y = temp;
}

template<typename T>
void templateSwap(T& x, T& y)
{
  T temp = x;
  x = y;
  y = temp;
}

template<class T>
void templateSwap01(T& x, T& y)
{
  T temp = x;
  x = y;
  y = temp;
}

// 기존에 배열은 포인터로 받았는데, 레퍼런스로 받을 수도 있음!
template<int N>
// 타입을 int로 명시해주었기 때문에 컴파일러가 int 배열이라는 것을 추론 가능
// 컴파일 추론이기에 정적 배열만 가능)
void func(int (&nums)[N])
{
  for (int i = 0; i < N; i++)
    cout << nums[i] << endl;
}

template<int N, typename T>
int func(T(&nums)[N])
{
  for (int i = 0; i < N; i++)
    cout << nums[i] << endl;
}

int main()
{
  // 01. 다양한 파라미터 형식을 받도록!
  int x = 10, y = 20;
  swap(x, y);
  cout << x << " " << y << endl;
  templateSwap(x, y);
  cout << x << " " << y << endl;
  templateSwap01<int>(x, y);
  cout << x << " " << y << endl;

  // 02. 배열 크기도 자유롭게!
  int nums[] = { 1, 2, 3 };
  func(nums);

  // 03. 1번과 2번을 합친 형태!
  float nums1[] = { 1, 2, 3 };
  func(nums);
}

/*
  02. 클래스 템플릿
*/
# include <iostream>
using namespace std;

template<typename T> // , int N>
class Queue
{
private:
	size_t _size; // 원소가 몇 개인지
	size_t _capacity; // items의 총 사이즈
	T* _items;
	// T _items[N];
	// capacity를 받아서 설정하는 것도 가능. 동적할당 관련 코드 모두 삭제

public:
	Queue();
	Queue() : _size(0), _capacity(4), _items(new T[_capacity])
	{}

	~Queue()
	{
		delete[] _items;
	}

	void push(T item);
	void pop();
	T& top();
};

template<typename T>
void Queue<T>::push(T item)
{
	if (_size < _capacity)
	{
		_items[_size++] = item;
	}
	else
	{
		size_t newCapacity = _capacity * 2;
		T* oldItems = _items;
		T* newItems = new T[newCapacity];

		copy(oldItems, oldItems + _capacity, newItems);
		_capacity = newCapacity;
		_items = newItems;

		delete[] oldItems;

		push(item);
	}
		// if (size < N) _items[_size++] = item;
		// else throw out_of_range("Overflow!");
}

template<typename T>
void Queue<T>::pop()
{
	if (_size == 0)
		throw out_of_range("underflow!");
	--_size;
}

template<typename T>
T& Queue<T>::top()
{
	if (_size == 0)
		throw out_of_range("underflow!");
	return _items[_size - 1];
}

int main()
{
	Queue<string> q;
	try
	{
		q.push("huhu");
		q.push("HUHU");
		cout << q.top() << endl;
		q.pop();
		cout << q.top() << endl;
	}
	catch(exception e)
	{
		cout << e.what() << endl;
	}
}

/*
	03. 템플릿 특수화
*/
# include <iostream>
using namespace std;

class Test {};

template <typename T>
void Swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}

template<>
void Swap(Test& x, Test& y) // 위의 함수와 동일한 레퍼런스 형태
{
	cout << "swap<Test>" << endl;
}

template <typename T>
void Swap(T* x, T* y) // 포인터를 위한 추가 함수. 오버로딩된 다른 함수
{
	cout << "swap pointer" << endl;
	T temp = *x;
	*x = *y;
	*y = temp;
}

//
template<typename T, typename S>
class Test1
{
public:
	T num0;
	S num1;
};

template<>
class Test1<int, float>
{};

template<typename T>
class Test1<T, T>
{
public:
	T nums;
};

template<typename T>
class Test1<T, int>
{
public:
	T a;
};
//

int main()
{
	// 01. 특정 type의 경우 다르게 처리하기
	Test t0, t1;
	Swap(t0, t1);

	// 02. 포인터의 경우에는..
	int x = 10, y = 20;
	int* px = &x, * py = &y;
	Swap(px, py);
	cout << x << " " << y << endl; // 기존 함수를 사용하면 포인터값만 바뀌고 실제 값은 변동없음
	cout << *(&x) << " " << *(&y) << endl;
	cout << *px << " " << *py << endl;

	// 03. 클래스 특수화
	Test1<int, int> t0;
	Test1<int, float> t1;
	Test1<float, int> t2; // 맨 아래 클래스 따라감 (구체적인 것이 우선순위 높음)
}

/*
	04. 템플릿 구체화
	- 함수 템플릿, 클래스 템플릿은 함수, 클래스 자체가 아님
	- 구체화가 필요함!
	- swap.h 참고! (swap.cpp 없음)
*/
# include <iostream>
# include "swap.h"

int main()
{
	int x = 10, y = 20;
	swap<int>(x, y); // <int>를 표기함으로써 구체화
}

/*
	05. 가변 인자
	printf("%d %d, 10, 10);
	위와 같은 함수를 실행할 때 %d 개수를 다양하게 받을 수 있어!
*/
# include <iostream>
# include <cstdarg>

int sum(int count...)
{
	int result = 0;
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++)
		result += va_arg(args, int);

	va_end(args);
	return result;
}

// 아래 템플릿 함수에서 재귀 반복하다가 마지막에 호출할 탈출 함수
int sumT(int value)
{
	return value;
}

template<typename... Args>
int sumT(int value, Args... args)
{
	return value + sumT(args...);
}

int main()
{
	// 1. c 스타일 가변 인자
	std::cout << sum(4, 10, 20, 30, 40) << std::endl;

	// 2. 템플릿을 이용한 가변 인자
	std::cout << sumT(10, 20, 30, 40) << std::endl;
}

/*
	05. 가변 인자
	printf("%d %d, 10, 10);
	위와 같은 함수를 실행할 때 %d 개수를 다양하게 받을 수 있어!
*/
# include <iostream>
# include <cstdarg>

int sum(int count...)
{
	int result = 0;
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++)
		result += va_arg(args, int);

	va_end(args);
	return result;
}

// 아래 템플릿 함수에서 재귀 반복하다가 마지막에 호출할 탈출 함수
template<typename T>
T sumT(T value)
{
	return value;
}

template<typename T, typename... Args>
T sumT(T value, Args... args)
{
	return value + sumT(args...);
}

int main()
{
	// 1. c 스타일 가변 인자
	std::cout << sum(4, 10, 20, 30, 40) << std::endl;

	// 2. 템플릿을 이용한 가변 인자
	std::cout << sumT(10, 20.3f, 30, 40) << std::endl;
	std::cout << sumT<float>(10, 20.3f, 30, 40) << std::endl;
}

/*
	06. 템플릿 메타 프로그래밍
*/
# include <iostream>

template<int N>
struct Factorial
{
	static const int value = N * Factorial<N - 1>::value;
};

// 탈출 조건은 특수화를 통해 만들어줌
template<>
struct Factorial<1>
{
	static const int value = 1;
};

int main()
{
	std::cout << "여기에 종단점 걸고 확인" << std::endl;
	// 컴파일 타임에 계산 끝내고 값 자체가 이미 적용됨 (디버깅 불가)
	std::cout << Factorial<5>::value << std::endl;
	// 컴파일 시에 계산하므로 변수는 못 넣음 (const는 가능)
}
