#include "pch.h"
#include "transcode.h"
#include<fstream>
#include<sstream>

/*
����µ�״̬�ڵ㣬ͳһ���������ڹ������ܲ����ظ���״̬
����ӵ�state_set[]������
*/
state thompson::new_StateNode()
{
	state newState;
	newState.StateName = STATE_NUM + 65;//ת���ɴ�д��ĸ
	STATE_NUM++;
	return newState;
}



//���� a|b
cell thompson::do_Unite(cell Left, cell Right)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge Edge1, Edge2, Edge3, Edge4;
	//����µ���״̬�ڵ�
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//������
	Edge1.StartState = StartState;
	Edge1.EndState = Left.StartState;
	Edge1.TransSymbol = '#';  //����մ�

	Edge2.StartState = StartState;
	Edge2.EndState = Right.StartState;
	Edge2.TransSymbol = '#';

	Edge3.StartState = Left.EdgeSet[Left.EdgeCount - 1].EndState;
	Edge3.EndState = EndState;
	Edge3.TransSymbol = '#';

	Edge4.StartState = Right.EdgeSet[Right.EdgeCount - 1].EndState;
	Edge4.EndState = EndState;
	Edge4.TransSymbol = '#';

	//������Ԫ
	//�Ƚ�Left��Right��EdgeSet���Ƶ�NewCell
	cell_EdgeSet_Link(NewCell, Left);
	cell_EdgeSet_Link(NewCell, Right);

	//���¹����������߼���EdgeSet
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge1;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge2;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge3;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge4;

	//����NewCell����ʾ״̬�ͽ���״̬
	NewCell.StartState = StartState;
	NewCell.EndState = EndState;

	return NewCell;
}


//���� ab
cell thompson::do_Join(cell Left, cell Right)
{
	//��Left�Ľ���״̬��Right�Ŀ�ʼ״̬�ϲ�����Right�ı߸��Ƹ�Left����Left����
	//��Right��������StartState��ͷ�ı�ȫ���޸�
	for (int i = 0; i < Right.EdgeCount; i++)
	{
		if (Right.EdgeSet[i].StartState.StateName.compare(Right.StartState.StateName) == 0)
		{
			Right.EdgeSet[i].StartState = Left.EndState;
			//STATE_NUM--;//�о���һ���������
		}
		else if (Right.EdgeSet[i].EndState.StateName.compare(Right.StartState.StateName) == 0)
		{
			Right.EdgeSet[i].EndState = Left.EndState;
			//STATE_NUM--;
		}
	}
	Right.StartState = Left.EndState;

	cell_EdgeSet_Link(Left, Right);
	//��Left�Ľ���״̬����ΪRight�Ľ���״̬
	Left.EndState = Right.EndState;
	return Left;
}


//���� a*
cell thompson::do_Star(cell Cell)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge Edge1, Edge2, Edge3, Edge4;//������������
	//����µ���״̬�ڵ�
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//������
	Edge1.StartState = StartState;
	Edge1.EndState = EndState;
	Edge1.TransSymbol = '#';  //�հ�ȡ�մ�

	Edge2.StartState = Cell.EndState;
	Edge2.EndState = Cell.StartState;
	Edge2.TransSymbol = '#';  //ȡ�ַ���������

	Edge3.StartState = StartState;
	Edge3.EndState = Cell.StartState;
	Edge3.TransSymbol = '#';

	Edge4.StartState = Cell.EndState;
	Edge4.EndState = EndState;
	Edge4.TransSymbol = '#';
	//������Ԫ
	//�Ƚ�ԭ����Cell��EdgeSet��ӵ�NewCell
	cell_EdgeSet_Link(NewCell, Cell);
	//���¹����������߼���EdgeSet
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge1;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge2;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge3;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge4;
	//����NewCell����ʾ״̬�ͽ���״̬
	NewCell.StartState = StartState;
	NewCell.EndState = EndState;

	return NewCell;
}


//���� a
cell thompson::do_Cell(char element)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge NewEdge;
	//����µ���״̬�ڵ�
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//������
	NewEdge.StartState = StartState;
	NewEdge.EndState = EndState;
	NewEdge.TransSymbol = element;
	//������Ԫ
	NewCell.EdgeSet[NewCell.EdgeCount] = NewEdge;
	NewCell.EdgeCount++;
	NewCell.StartState = NewCell.EdgeSet[0].StartState;
	NewCell.EndState = NewCell.EdgeSet[0].EndState;//EdgeCount��ʱΪ1
	return NewCell;
}


//��������cell��ӵ�ԭ����cell��
void thompson::cell_EdgeSet_Link(cell& Destination, cell Source)
{
	int D_count = Destination.EdgeCount;
	int S_count = Source.EdgeCount;
	for (int i = 0; i < S_count; i++)
	{
		Destination.EdgeSet[D_count + i] = Source.EdgeSet[i];
	}
	Destination.EdgeCount = D_count + S_count;
}



//��������������ʽ��RegularExpression��Ϊ�ش�����
void thompson::input(string& RegularExpression,string in)
{
	stringstream If(in);
	//cout << "������������ʽ��  ����������() * |;�ַ�����a~z A~Z��" << endl;
	do
	{
		If >> RegularExpression;
	} while (!check_Islegal(RegularExpression));

}



//��������������ʽ�Ƿ�Ϸ�
bool thompson::check_Islegal(string check_string)
{
	if (check_Ischaracter(check_string) && check_Ismatching(check_string))
	{
		return true;
	}
	return false;
}



/*
���������ַ��Ƿ���� () * | a~z A~Z
�Ϸ�����true,�Ƿ�����false
*/
bool thompson::check_Ischaracter(string check_string)
{
	int length = check_string.size();
	for (int i = 0; i < length; i++)
	{
		char check = check_string.at(i);
		//ԭ�ͣ� const char& at(size_t pos) const;
		//���ܣ� ����Դ�ַ����±�Ϊn�����ַ������á�
		//˵���� ���ܱ��޸ġ�
		if (is_letter(check))//Сд�ʹ�д֮����5���ַ����ʲ��������ж�
		{
			//cout<<"��ĸ �Ϸ�";
		}
		else if (check == '(' || check == ')' || check == '*' || check == '|')
		{
			//cout<<"������ �Ϸ�";
		}
		else
		{
			cout << "���в��Ϸ����ַ�!" << endl;
			cout << "����������:" << endl;
			return false;
		}
	}
	return true;
}



/**�ȼ�������Ƿ�ƥ��
*�Ϸ�����true,�Ƿ�����false
*/
bool thompson::check_Ismatching(string check_string)
{
	int length = check_string.size();
	//char * check = new char[length+1];
	//wcscpy(check, length+1, check_string.c_str());
	string check = check_string;
	stack<int> STACK;
	for (int i = 0; i < length; i++)
	{
		if (check[i] == '(')
			STACK.push(i);
		else if (check[i] == ')')
		{
			if (STACK.empty())
			{
				cerr << "�ж����������" << endl;//��ʱ����¼ƥ��λ��location
				cout << "����������:" << endl;
				return false;
			}
			else
				STACK.pop();
		}
	}
	if (!STACK.empty())
	{
		//��ʱ����¼ƥ��λ��location
		cerr << "�ж����������" << endl;
		cout << "����������:" << endl;
		return false;
	}

	return true;
}



/**����Ƿ�����ĸ
�Ƿ���true,����false
*/
bool thompson::is_letter(char check)
{
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	return false;
}



/**��ӽ���������+����������׺ת��׺���ʽ
���� abb->a+b+b
*/
string thompson::add_plus_symbol(string add_string)
{
	int length = add_string.size();
	int return_string_length = 0;
	char* return_string = new char[2 * length + 2];//��������� 
	char first, second;
	if (add_string.size() == 1)return add_string;//ֻ����һ���ַ������
	for (int i = 0; i < length - 1; i++)
	{
		first = add_string.at(i);//��ȡ�ַ����ĵ�i���ַ�
		second = add_string.at(i + 1);
		return_string[return_string_length++] = first;
		//Ҫ�ӵĿ�������ab �� *b �� a( �� )b �����
		//���ڶ�������ĸ����һ������'('��'|'��Ҫ���
		if (first != '(' && first != '|' && is_letter(second))
		{
			return_string[return_string_length++] = '+';
		}
		//���ڶ�����'(',��һ������'|'��'(',ҲҪ��   
		else if (second == '(' && first != '|' && first != '(')
		{
			return_string[return_string_length++] = '+';
		}
	}
	//�����һ���ַ�д��
	return_string[return_string_length++] = second;
	return_string[return_string_length] = '\0';
	string STRING(return_string);//
	return STRING;
}



/*
�������ȼ�����򣺣�1���������ڣ��������⣻��2�����ȼ��ɸߵ��ͣ��հ���|��+����3��ͬ����������ҡ�
���ȼ���
	 #	(	*	|	+	)
isp  0	1	7	5	3	8
icp	 0	8	6	4	2	1
*/

//in stack priority  ջ�����ȼ���ջ�����ַ������ȼ�
int thompson::isp(char c)
{
	switch (c)
	{
	case '#': return 0;
	case '(': return 1;
	case '*': return 7;
	case '|': return 5;
	case '+': return 3;
	case ')': return 8;
	}
	//���ߵ���һ����˵��������
	cerr << "ERROR!" << endl;
	return false;
}



//in coming priority ջ�����ȼ�����ǰɨ�赽���ַ������ȼ�
int thompson::icp(char c)
{
	switch (c)
	{
	case '#': return 0;
	case '(': return 8;
	case '*': return 6;
	case '|': return 4;
	case '+': return 2;
	case ')': return 1;
	}
	//���ߵ���һ����˵��������
	cerr << "ERROR!" << endl;
	return false;
}



/*��׺���ʽת��׺���ʽ*/
string thompson::turn_postfix(string e)//�������һ����׺���ʽ
{
	//�趨e�����һ������ʽ��#�������䡰#��һ��ʼ�ȷ���ջs��ջ��
	e = e + "#";

	stack<char> s;
	char ch = '#', ch1, op;
	s.push(ch);
	//��һ���ַ�
	string Postfix_Expression_String = "";
	int read_location = 0;
	ch = e.at(read_location++);//�����ȡ�ַ�
	while (!s.empty())
	{
		if (is_letter(ch))
		{
			Postfix_Expression_String = Postfix_Expression_String + ch;
			//cout<<ch;
			ch = e.at(read_location++);
		}
		else
		{
			//cout<<"�����������"<<ch<<endl;
			ch1 = s.top();
			if (isp(ch1) < icp(ch))
			{
				s.push(ch);
				//cout<<"ѹջ"<<ch<<"  ��ȡ��һ��"<<endl;
				ch = e.at(read_location++);
			}
			else if (isp(ch1) > icp(ch))
			{
				op = s.top();
				s.pop();
				//cout<<"��ջ"<<op<<" ��ӵ�����ַ���"<<endl;
				Postfix_Expression_String = Postfix_Expression_String + op;
				//cout<<op;
			}
			else  //�������ȼ���ȵ����
			{
				op = s.top();
				s.pop();
				//cout<<"��ջ"<<op<<"  ������ӵ������ַ���"<<endl;

				if (op == '(')
					ch = e.at(read_location++);
			}
		}
	}
	return Postfix_Expression_String;
}


/*���ʽתNFA������,�������յ�NFA���*/
cell thompson::Expression_to_NFA(string expression)
{
	int length = expression.size();
	char element;
	cell Cell, Left, Right;
	stack<cell> STACK;
	for (int i = 0; i < length; i++)
	{
		element = expression.at(i);
		switch (element)
		{
		case '|':
			Right = STACK.top();
			STACK.pop();
			Left = STACK.top();
			STACK.pop();
			Cell = do_Unite(Left, Right);
			STACK.push(Cell);
			break;
		case '*':
			Left = STACK.top();
			STACK.pop();
			Cell = do_Star(Left);
			STACK.push(Cell);
			break;
		case '+':
			Right = STACK.top();
			STACK.pop();
			Left = STACK.top();
			STACK.pop();
			Cell = do_Join(Left, Right);
			STACK.push(Cell);
			break;
		default:
			Cell = do_Cell(element);
			STACK.push(Cell);
		}
	}
	cout << "�������!" << endl;
	Cell = STACK.top();
	STACK.pop();

	return Cell;
}


//��ʾNFA

void thompson::Display(cell Cell)
{
	ofstream of("toNFA.txt");
	of << "NFA �ı�����" << Cell.EdgeCount << endl;
	of << "NFA ����ʼ״̬��" << Cell.StartState.StateName << endl;
	of << "NFA �Ľ���״̬��" << Cell.EndState.StateName << endl;
	for (int i = 0; i < Cell.EdgeCount; i++)
	{
		of << "��" << i + 1 << "���ߵ���ʼ״̬��" << Cell.EdgeSet[i].StartState.StateName
			<< "  ����״̬��" << Cell.EdgeSet[i].EndState.StateName
			<< "  ת������" << Cell.EdgeSet[i].TransSymbol << endl;
	}
	of << "����" << endl;
	of.close();
}
