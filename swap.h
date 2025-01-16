#pragma once

template<typename T>
void swap(T& x, T& y);

// 얘는 함수가 아니라 템플릿일 뿐!
// 그래서 헤더에 넣어줌
template<typename T>
void swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}
