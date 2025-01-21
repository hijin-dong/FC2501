/*
	01. STL의 개요
	- Standard Template Library
	- C++ 표준 라이브러리 중 일부로 다양한 요구사항에 적용가능한 유연성을 지님
	- 컴포넌트
	  - 컨테이너: 추상 데이터 모음
		  - Sequence Container: array, vector, deque, list ...
			- Associate Container (Ordered / Unordered): set, map ...
		- 이터레이터
		- 알고리즘
*/
# include <iostream>
# include <vector>
# include <set>
# include <algorithm>
using namespace std;

int main()
{
	// 1) 컨테이너
	vector<int> v;
	set<int> s;

	// Sequence Container: push_back
	v.push_back(2);
	v.push_back(1);
	v.push_back(3);
	// Associate Container: insert
	s.insert(1);
	s.insert(2);
	s.insert(3);

	// 2) 이터레이터
	for (int i = 0; i < v.size(); i++)
		cout << "iter (1): " << v[i] << endl;
	for (int num : v)
		cout << "iter (2): " << num << endl;
	for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++)
		cout << "iter (3): " << *iter << endl;

	// 3) 알고리즘
	sort(v.begin(), v.end(), greater<int>());
	for (int num : v)
		cout << "sort: " << num << endl;
	//sort(v.begin() + 1, v.end());
	//for (int num : v)
	//	cout << "partial sort: " << num << endl;
}

/*
	02. Sequence Container (array, vector, deque)
*/
# include <iostream>
# include <array>
# include <vector>
# include <deque>
using namespace std;

int main()
{
	// 1. Array는 type, 개수 지정하여 선언
	array<int, 5> arr;

	// 2. Vector는 크기를 조절함
	vector<int> vec{ 1,2,3 };
	vector<int>::iterator iter = vec.begin();
	int& num = vec[0]; // 1

	vec[0] = 10;
	cout << *iter << endl; // 10
	cout << num << endl; // 10

	// 현재 capa보다 1이 크면 재할당
	vec.reserve(vec.capacity() + 1);

	vec[0] = 100;
	//cout << *iter << endl; // error
	cout << num << endl; // -572662307

	// 2-1. Vector의 size와 capacity // 
	// [1, 2, 3, ?] : size = 3, capacity = 4

	// push_back(4)
	// [1, 2, 3, 4] : size = 4, capacity = 4

	// push_back(5);
	// [1, 2, 3, 4, 5, ?, ?, ?, ?] : size = 5, capacity = 재할당
	// 주소가 바뀜

	vec.insert(vec.begin(), 10);
	// 기존 원소들을 다 뒤로 밀어야하기에 시간 복잡도 N
	// push_front를 지원하지 않는 이유

	// 3. Deque
	vector<int> v;

	v.push_back(1);
	cout << v[0] << endl;
	cout << &v[0] << endl;
	// [1, ...]

	v.insert(v.begin(), 10);
	cout << v[1] << endl;
	cout << &v[1] << endl; // 1의 주소값이 4칸 (정수 타입 크기만큼) 밀림
	// [10, 1, ...]

	cout << "----------" << endl;
	deque<int> nums;
	
	nums.push_front(1);
	cout << nums[0] << endl;
	cout << &nums[0] << endl;

	nums.push_front(2);
	cout << nums[1] << endl;
	cout << &nums[1] << endl; // 1의 주소값이 동일

	// 3-1. deque의 원소 주소가 바뀌지 않는 이유: 내부적으로 여러 개의 배열 사용
	// [0, 1, 2, 3, 4, 5, 6, ?]
	// push_front(10)
	// [?, ?, ?, 10] [0, 1, 2, 3, 4, 5] [6, ?, ?]
}

/*
	03. Sequence Container (list, forward_list)
*/
# include <iostream>
# include <list>
# include <forward_list>
# include <algorithm>
using namespace std;

bool condition(const int& value)
{
	return value % 2 == 0;
}

int main()
{
	// list //
	list<int> l{ 2, 1, 3 };

	// 1) 첨자 인덱싱 불가: 성능 문제
	// doubly linked list로 구현됨
	// Head <-> Node0 <-> Node1 <-> Node2 <-> Tail

	// 2) iteration
	list<int>::iterator iter = l.begin();
	advance(iter, 2);
	cout << "2: iteration: " << *iter << endl; // 3

	// 3) sort
	l.sort();
	for (int num : l)
		cout << "3: sort: " << num << endl; // 1, 2, 3
	// sort(l.begin(), l.end()); // Random access 이터레이터에만 사용 가능
	// list.begin()++는 가능하고, list.begin() + 3 은 불가

	// 4) remove: 삭제 속도가 빨라서 삭제 관련 메소드 다수
	l.remove_if(condition);
	for (int num : l)
		cout << "4. remove: " << num << endl; // 1, 3

	// 5) merge: 정렬이 되어 있어야 함
	list<int> l0 = { 1, 1, 2, 3 };
	list<int> l1 = { 1, 2, 3, 4 };

	l0.merge(l1);
	for (int num : l0)
		cout << "5. merge: " << num << endl; // 1, 1, 1, 2, 2, 3, 3, 4

	l0.unique(); // 삭제 관련 메소드
	for (int num : l0)
		cout << "5. merge & unique: " << num << endl; // 1, 2, 3, 4


	// forward_list //
	// singly linked list로 구현됨
	// 제약이 많아 잘 쓰이지는 않음
	// 메모리 제약이 있을 때 고려해봄직도..!

	// 1) insert의 위치 차이 (list와 비교)
	l = { 1, 2, 3 };
	l.insert(l.begin(), 10); // 10, 1, 2, 3

	forward_list<int> fl{ 1, 2, 3 };
	fl.insert_after(fl.begin(), 10); // 1, 10, 2, 3
}
