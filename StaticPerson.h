#pragma once
class StaticPerson
{
private:
	static int num;
	// ���� �ʱ갪�� �������־�� ������ �Ǵµ� ��� ������ �������⼭ ���̴ϱ� cpp���� ������!
	// static �ʱ�ȭ�� ���⼭ �ص� �Ǳ� ��!
	// class ������ ���� ��������! �� �� ���� ������

public:
	StaticPerson();
	void print();

	// static void print();
	// �̷��� �ϸ� instance�� ������ �ʰ�, Ŭ������ ���ؼ� ȣ�� ����
	// ��, ���� ������� ���� ���� (static ���� ��)
	// ��ü�� ���⿡ this�� �Ѿ�� ���� -> print const ()�� ��� �Ұ�
};