/*
	01. Modules
	- 기존 헤더 파일 include의 문제를 해결
	- export를 통해 바깥에 노출시킬 부분을 설정할 수 있음
*/

/*
	02. concept
	- 템플릿 파라미터에 제약을 거는 기능
	- 컴파일 시 평가
*/
# include <iostream>
template<typename T>
concept NonPointer = !std::is_pointer<T>::value;

template<NonPointer T>
void swap(T& x, T& y)
{

}

int main()
{

}

/*
	03. Ranges
	- 반복자와 알고리즘을 합성
	- algorithm, view, action
*/
# include <iostream>
# include <vector>
# include <numeric>
# include <range/v3/all.hpp>
using namespace std;

int main()
{
	// 1) Algorithm
	vector<int> v(10);
	iota(v.begin(), v.end(), 0);
	auto iter = ranges::find(v, 3);
	cout << *iter << endl;

	// 2) View
	// 파이프(|)를 이용한 옵션 합성 가능. lazy하게 동작
	// container에 영향 X
	auto r =
		ranges::views::iota(0, 10) |
		ranges::views::reverse |
		ranges::views::filter([](const int& num)
			{
				return num % 2 == 0;
			});
	for (int num : r)
		cout << num << endl; // 8, 6, 4, 2, 0

	// 3) Action
	// 컨테이너에 영향
	auto a = ranges::views::iota(0, 20) | ranges::to<vector<int>>();
	a |= ranges::actions::reverse | ranges::actions::drop(2);
	cout << ranges::view_base::all(a) << endl; // 17, 16 ~ 1, 0
}

/*
	04. Coroutines
	- 일반 함수는 return을 만나야 종료, 코루틴은 중도에 main 복귀 가능
	- co_yield, co_await, co_return
*/
# include <iostream>
# include <future>
# include <chrono>
# include <experimental/generator>
using namespace std;

// ~1.
experimental::generator<int> gen()
{
	cout << "gen 0" << endl;
	co_yield 0;

	cout << "gen 1" << endl;
	co_yield 1;

	cout << "gen 2" << endl;
	co_yield 2;

	cout << "gen 3" << endl;
	co_yield 3;
}

// ~2.
int longTimeJob()
{
	this_thread::sleep_for(2s);
	return 100;
}

future<int> coroutine()
{
	cout << "Coroutine 0" << endl;
	int result = co_await async(longTimeJob);

	cout << "Coroutine 0" << endl;
	co_return result;

}

int main()
{
	// 1) co_yield
	cout << "========== Example 1 ==========" << endl;
	auto g = gen();
	auto iter = g.begin();

	cout << "main 0" << endl;
	cout << *iter << endl; 
	iter++;

	cout << "main 1" << endl;
	cout << *iter << endl;
	iter++;

	cout << "main 2" << endl;
	cout << *iter << endl;
	iter++;

	// 2) co_await & co_return
	cout << "========== Example 2 ==========" << endl;
	auto future = coroutine();
	cout << "main 0" << endl;

	cout << future.get() << endl;
}
