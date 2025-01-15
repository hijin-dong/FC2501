
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

template<typename T>
class Queue
{
private:
	size_t _size; // 원소가 몇 개인지
	site_t _capacity; // items의 사이즈
	T* _items;
public:
	Queue() : _size(0), _capacity(4), _items(new T[_capacity])
	{

	}

	~Queue()
	{
		delete[] _items;
	}

	void push(T item)
	{

	}
};

int main()
{

}
