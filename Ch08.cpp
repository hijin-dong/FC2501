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

/*
  04. Ordered Associate Container (1): map
  binary tree로 구현됨 (tree의 높이만큼의 시간복잡도. log N)
*/
# include <iostream>
# include <map>
using namespace std;

int main()
{
  // map //
  // key와 value 쌍 //
  map<int, string> m; 
  m[1] = "a";
  
  // 1) 첨자 연산자
  // 아래 코드들은 const map으로 선언되면 에러 발생
  cout << m[2] << endl; // 없는 key를 출력 시도하면 공백 출력
  cout << m.size() << endl; // 그리고 그 과정에서 디폴트 생성자 호출해서 넣어줌, 2 출력

  // 2) 삽입
  map<int, string> m1{
    {2, "huhu"},
    pair(1, "hoho"),
    pair<int, string>(7, "HUHU"), // 버전에 따라 위의 방법대신 이것으로 해야할 수도
    make_pair(4, "rulu")
  };

  m1[1] = "HOU!"; // key = 1이 존재하고 있다면 덮어씌워짐
  cout << m1[1] << endl;
  m1.insert({ 1, "hoho" }); // 위의 예시 모두 적용가능, key가 없을 경우에만 삽입
  cout << m1[1] << endl;
  
 // bool은 insert 성공 여부, iter는 성공 시 해당 pair를 가리키는 이터레이터 (실패 시 key 1값인 이미 추가된 pair)
  cout << boolalpha;
  pair<map<int, string>::iterator, bool> result = m1.insert({1, "hoho"});
  auto [iter, success] = m.insert({1, "hoho"}); // 버전에 따라 안 될수도 있음
  cout << result.second << endl;
  cout << result.first->first << endl;
  cout << result.first->second << endl;

  // 3) 조회
  cout << m1.count(1) << endl; // 해당 key를 가진 pair의 개수 조회 (0 or 1)

  map<int, string>::iterator iter = m1.find(3); // auto 사용 가능
  if (iter != m1.end())
    cout << iter->second << endl;

  for (const pair<int, string>& p : m1) // key가 정렬된 상태로 순회함
    cout << p.first << ": " << p.second << endl;

  // 4) 삭제
  if (auto iter = m1.find(3); iter != m1.end())
    m1.erase(iter);
  
  // 5) 값 변경
  // key는 변경 불가(const. 트리 정합성 깨짐), value는 가능
  for (auto& p : m1)
  {
    p.second = "HUHU!";
    cout << p.first << ": " << p.second << endl;
  }

  // 6) iterator
  //for (map<int, string>::iterator iter = m1.begin(); iter != m1.end(); iter++)
  for (auto iter = m1.begin(); iter != m1.end(); iter++)
  {
    cout << iter->first << ": " << iter->second << endl;
    auto [key, value] = *iter;
  }
 
  //for (map<int, string>::const_iterator iter = m1.cbegin(); iter != m1.cend(); iter++)
  for (auto iter = m1.cbegin(); iter != m1.cend(); iter++)
    cout << iter->first << ": " << iter->second << endl; // second 수정 불가
 
  for (auto& [key, value] : m1)
    cout << key << ": " << value << endl;
}

/*
  05. Ordered Associate Container (2): set, multiset, multimap
*/
# include <iostream>
# include <map>
# include <set>
# include <functional>
using namespace std;

struct Greater
{
  bool operator()(const int& lhs, const int& rhs) const
  {
    return lhs > rhs;
  }
};

int main()
{
  cout << boolalpha;

  //// multiMap ////
  // key 중복 가능
  multimap<int, string> mm
  {
    {1, "huhu"},
    {1, "HUHU"},
    {2, "huhu"},
    {2, "HUHU"},
  };

  for (const auto& [k, v] : mm)
    cout << k << ": " << v << endl;

  // 1) 조회: key 중복이 가능하므로 첨자 연산은 불가 & 결과가 여러 개일 수 있음
  auto lower = mm.lower_bound(1);
  auto upper = mm.upper_bound(1);
  for (auto iter = lower; iter != upper; iter++)
    cout << iter->second << endl;

  auto [lower, upper] = mm.equal_range(1);
  for (auto iter = lower; iter != upper; iter++)
    cout << iter->second << endl;

  // 2) 삽입: key 중복이 가능하므로 무조건 성공


  //// set ////
  // key와 value가 동일한 map
  set<int> s
  {
    1, 2, 3, 4
  };

  // 1) 삽입
  if (auto [iter, success] = s.insert(10); success)
    cout << *iter << endl; // 10

  // 2) multi set
  // set과 비슷하나 중복 가능
  multiset<int> ms
  {
    1, 1, 3, 10, 10, 3
  };

  for (const auto& num : ms)
    cout << num << endl; // 1, 1, 3, 3, 10, 10

  auto ms_iter = ms.insert(1); // 중복을 허용하기에 무조건 성공
  cout << *ms_iter << endl;

  // 3) 정렬 (map도 가능)
  // multiset<int, greater<int>> ms_ { 4,2,7,1,7,8,10 };
  multiset<int, Greater> ms_{ 4,2,7,1,7,8,10 }; // 커스텀 연산도 가능!
  for (const auto& n : ms_)
    cout << n << endl;
}

/*
  06. Unordered Associate Container (2): unordered_(multi)set, unordered_(multi)map
  hash table으로 구현됨
  - 해시 충돌이 없을 경우 상수 시간에 삽입, 탐색, 삭제
  - 순서 보장 X
*/
# include <iostream>
# include <unordered_map>
using namespace std;

struct HashFunction
{
  size_t operator()(const int& key) const
  {
    return key;
  }
};

int main()
{
  // 해싱 함수 커스텀 가능
  unordered_map<int, string, HashFunction> um
  {
    {1, "huhu"},
    {2, "Huhu"},
    {3, "HuHu"},
    {4, "HUHU"},
  };
}

/*
  07. Iterator
*/
# include <iostream>
# include <vector>
# include <list>
# include <forward_list>
# include <iterator>
# include "Ranges.h"
using namespace std;

// ~2
template<typename TIterator, typename T>
bool has(TIterator begin, TIterator end, T value)
{
  for (auto iter = begin; iter != end; iter++)
  {
    if (*iter == value)
      return true;
  }
  return false;
}

// ~4-5
void ostreamIter(ostream& os)
{
  ostream_iterator<int> iter(os, " ");
  *iter = 1;
  *iter = 2;
  *iter = 3;
}

int main()
{
  cout << boolalpha;
  vector<int> v{ 1, 2, 3 };
  list<string> l{ "1", "2", "3" };

  // 1) Mutable & Immutable
  for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++)
    *iter *= 2;
  for (vector<int>::const_iterator iter = v.cbegin(); iter != v.cend(); iter++)
    cout << *iter << endl;
  
  // 2) 알고리즘/함수와의 연결고리: 이터레이터로 컨테이너 추상화 가능
  cout << has(v.begin(), v.end(), 1) << endl;
  cout << has(l.begin(), l.end(), "1") << endl;

  // 3) ForEach
  // Ranges.h, Ranges.cpp 참조
  // 실제 for, range도 이터레이터 인터페이스로 구성되어 있음
  for (int num : Ranges(0, 3))
    cout << "Ranges(0, 3): " << num << endl; // 0, 1, 2
  for (int num : Ranges(3))
    cout << "Ranges(3): " << num << endl; // 0, 1, 2

  // 4) Iterator의 종류
  // 4-1) forward iterator
  // forward_list, unordered xxx (unordered는 컴파일러에 따라 다를 수 있음)
  // ++
  
  // 4-2) bidirectional iterator
  // list, set, map, multiset, multimap
  // ++, --

  // 4-3) random access iterator: 상수 시간에 접근 가능
  // array, vector, deque
  // ++, --, +1, -2 ..

  // 4-4) contiguous access iterator (C++ 20)
  // array, vector
  // deque는 array를 여러 개 사용하기 때문에 연속적인지 않아서 제외
  // ++, --, +, -, physical memory contiguous
  v = { 1, 2, 3 };
  auto iter = v.begin();
  cout << *(iter + 2) << endl; // 3 (이터레이터로 접근)
  cout << *(addressof(*iter) + 2) << endl; // 3(주소로 접근), addressof == & 연산이나, & 연산자 오버로딩으로 인해 주소값이 나오지 않는 경우에 대비하여 사용

  // 4-5) output iterator
  ostreamIter(cout);
}

/*
  08. Algorithm
*/
# include <iostream>
# include <algorithm>
# include <unordered_set>
using namespace std;

// ~1
struct Func
{
    int operator()(int value) const
    {
        return value;
    }
};

int main()
{
    // 1) 호출 가능한 함수의 형태
    Func func;
    func(10);

    // 2) 람다 함수 (익명 함수)
    // [외부 변수] (인자)
    int num = 0;
    auto func0 = [num](int value)
        {
            return num + value; // 리턴 타입 추론
        };
    cout << func0(1) << endl; // 1

    // 3) find
    // 멤버함수와 std함수 둘 다 있을 경우에는 멤버함수를 사용하는 것이 더 빠름
    unordered_set<int> s{ 1, 2, 3 };
    auto iter = s.find(3); // O(1)
    //auto iter - find(s.begin(), s.end(), 3); // O(N)

    // 4) algorithm + lambda
    vector<int> v{ 1, 2, 3 };
    auto v_iter = find_if(v.begin(), v.end(), [](const int& value)
        {
            return (value % 2 == 0);
        }); // O(N)
    if (v_iter != v.end())
        cout << "v_iter: " << *v_iter << endl;

    // 5) remove
    v = { 1, 2, 3, 2 };
    remove(v.begin(), v.end(), 2);

    for (auto num : v)
        cout << "after remove: " << num << endl; // 1, 3, 3, 2
}
