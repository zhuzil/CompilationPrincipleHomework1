#include "pch.h"
#include "transcode.h"
#include<fstream>
#include<sstream>

/*
获得新的状态节点，统一产生，便于管理，不能产生重复的状态
并添加到state_set[]数组中
*/
state thompson::new_StateNode()
{
	state newState;
	newState.StateName = STATE_NUM + 65;//转换成大写字母
	STATE_NUM++;
	return newState;
}



//处理 a|b
cell thompson::do_Unite(cell Left, cell Right)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge Edge1, Edge2, Edge3, Edge4;
	//获得新的新状态节点
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//构建边
	Edge1.StartState = StartState;
	Edge1.EndState = Left.StartState;
	Edge1.TransSymbol = '#';  //代表空串

	Edge2.StartState = StartState;
	Edge2.EndState = Right.StartState;
	Edge2.TransSymbol = '#';

	Edge3.StartState = Left.EdgeSet[Left.EdgeCount - 1].EndState;
	Edge3.EndState = EndState;
	Edge3.TransSymbol = '#';

	Edge4.StartState = Right.EdgeSet[Right.EdgeCount - 1].EndState;
	Edge4.EndState = EndState;
	Edge4.TransSymbol = '#';

	//构建单元
	//先将Left和Right的EdgeSet复制到NewCell
	cell_EdgeSet_Link(NewCell, Left);
	cell_EdgeSet_Link(NewCell, Right);

	//将新构建的四条边加入EdgeSet
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge1;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge2;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge3;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge4;

	//构建NewCell的启示状态和结束状态
	NewCell.StartState = StartState;
	NewCell.EndState = EndState;

	return NewCell;
}


//处理 ab
cell thompson::do_Join(cell Left, cell Right)
{
	//将Left的结束状态和Right的开始状态合并，将Right的边复制给Left，将Left返回
	//将Right中所有以StartState开头的边全部修改
	for (int i = 0; i < Right.EdgeCount; i++)
	{
		if (Right.EdgeSet[i].StartState.StateName.compare(Right.StartState.StateName) == 0)
		{
			Right.EdgeSet[i].StartState = Left.EndState;
			//STATE_NUM--;//感觉有一点点问题呢
		}
		else if (Right.EdgeSet[i].EndState.StateName.compare(Right.StartState.StateName) == 0)
		{
			Right.EdgeSet[i].EndState = Left.EndState;
			//STATE_NUM--;
		}
	}
	Right.StartState = Left.EndState;

	cell_EdgeSet_Link(Left, Right);
	//将Left的结束状态更新为Right的结束状态
	Left.EndState = Right.EndState;
	return Left;
}


//处理 a*
cell thompson::do_Star(cell Cell)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge Edge1, Edge2, Edge3, Edge4;//增加了四条边
	//获得新的新状态节点
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//构建边
	Edge1.StartState = StartState;
	Edge1.EndState = EndState;
	Edge1.TransSymbol = '#';  //闭包取空串

	Edge2.StartState = Cell.EndState;
	Edge2.EndState = Cell.StartState;
	Edge2.TransSymbol = '#';  //取字符，自连接

	Edge3.StartState = StartState;
	Edge3.EndState = Cell.StartState;
	Edge3.TransSymbol = '#';

	Edge4.StartState = Cell.EndState;
	Edge4.EndState = EndState;
	Edge4.TransSymbol = '#';
	//构建单元
	//先将原来的Cell的EdgeSet添加到NewCell
	cell_EdgeSet_Link(NewCell, Cell);
	//将新构建的四条边加入EdgeSet
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge1;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge2;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge3;
	NewCell.EdgeSet[NewCell.EdgeCount++] = Edge4;
	//构建NewCell的启示状态和结束状态
	NewCell.StartState = StartState;
	NewCell.EndState = EndState;

	return NewCell;
}


//处理 a
cell thompson::do_Cell(char element)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge NewEdge;
	//获得新的新状态节点
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//构建边
	NewEdge.StartState = StartState;
	NewEdge.EndState = EndState;
	NewEdge.TransSymbol = element;
	//构建单元
	NewCell.EdgeSet[NewCell.EdgeCount] = NewEdge;
	NewCell.EdgeCount++;
	NewCell.StartState = NewCell.EdgeSet[0].StartState;
	NewCell.EndState = NewCell.EdgeSet[0].EndState;//EdgeCount此时为1
	return NewCell;
}


//将新增的cell添加到原来的cell中
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



//接收输入正规表达式，RegularExpression作为回传函数
void thompson::input(string& RegularExpression,string in)
{
	stringstream If(in);
	//cout << "请输入正则表达式：  （操作符：() * |;字符集：a~z A~Z）" << endl;
	do
	{
		If >> RegularExpression;
	} while (!check_Islegal(RegularExpression));

}



//检测输入的正则表达式是否合法
bool thompson::check_Islegal(string check_string)
{
	if (check_Ischaracter(check_string) && check_Ismatching(check_string))
	{
		return true;
	}
	return false;
}



/*
检查输入的字符是否合适 () * | a~z A~Z
合法返回true,非法返回false
*/
bool thompson::check_Ischaracter(string check_string)
{
	int length = check_string.size();
	for (int i = 0; i < length; i++)
	{
		char check = check_string.at(i);
		//原型： const char& at(size_t pos) const;
		//功能： 返回源字符串下标为n处的字符的引用。
		//说明： 不能被修改。
		if (is_letter(check))//小写和大写之间有5个字符，故不能连续判断
		{
			//cout<<"字母 合法";
		}
		else if (check == '(' || check == ')' || check == '*' || check == '|')
		{
			//cout<<"操作符 合法";
		}
		else
		{
			cout << "含有不合法的字符!" << endl;
			cout << "请重新输入:" << endl;
			return false;
		}
	}
	return true;
}



/**先检查括号是否匹配
*合法返回true,非法返回false
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
				cerr << "有多余的右括号" << endl;//暂时不记录匹配位置location
				cout << "请重新输入:" << endl;
				return false;
			}
			else
				STACK.pop();
		}
	}
	if (!STACK.empty())
	{
		//暂时不记录匹配位置location
		cerr << "有多余的左括号" << endl;
		cout << "请重新输入:" << endl;
		return false;
	}

	return true;
}



/**检测是否是字母
是返回true,否则false
*/
bool thompson::is_letter(char check)
{
	if (check >= 'a' && check <= 'z' || check >= 'A' && check <= 'Z')
		return true;
	return false;
}



/**添加交操作符“+”，便于中缀转后缀表达式
例如 abb->a+b+b
*/
string thompson::add_plus_symbol(string add_string)
{
	int length = add_string.size();
	int return_string_length = 0;
	char* return_string = new char[2 * length + 2];//最多是两倍 
	char first, second;
	if (add_string.size() == 1)return add_string;//只输入一个字符的情况
	for (int i = 0; i < length - 1; i++)
	{
		first = add_string.at(i);//读取字符串的第i个字符
		second = add_string.at(i + 1);
		return_string[return_string_length++] = first;
		//要加的可能性如ab 、 *b 、 a( 、 )b 等情况
		//若第二个是字母、第一个不是'('、'|'都要添加
		if (first != '(' && first != '|' && is_letter(second))
		{
			return_string[return_string_length++] = '+';
		}
		//若第二个是'(',第一个不是'|'、'(',也要加   
		else if (second == '(' && first != '|' && first != '(')
		{
			return_string[return_string_length++] = '+';
		}
	}
	//将最后一个字符写入
	return_string[return_string_length++] = second;
	return_string[return_string_length] = '\0';
	string STRING(return_string);//
	return STRING;
}



/*
构造优先级表规则：（1）先括号内，再括号外；（2）优先级由高到低：闭包、|、+；（3）同级别，先左后右。
优先级表：
	 #	(	*	|	+	)
isp  0	1	7	5	3	8
icp	 0	8	6	4	2	1
*/

//in stack priority  栈内优先级，栈顶的字符的优先级
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
	//若走到这一步，说明出错了
	cerr << "ERROR!" << endl;
	return false;
}



//in coming priority 栈外优先级，当前扫描到的字符的优先级
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
	//若走到这一步，说明出错了
	cerr << "ERROR!" << endl;
	return false;
}



/*中缀表达式转后缀表达式*/
string thompson::turn_postfix(string e)//输入的是一个中缀表达式
{
	//设定e的最后一个符号式“#”，而其“#”一开始先放在栈s的栈底
	e = e + "#";

	stack<char> s;
	char ch = '#', ch1, op;
	s.push(ch);
	//读一个字符
	string Postfix_Expression_String = "";
	int read_location = 0;
	ch = e.at(read_location++);//逐个读取字符
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
			//cout<<"输出操作符："<<ch<<endl;
			ch1 = s.top();
			if (isp(ch1) < icp(ch))
			{
				s.push(ch);
				//cout<<"压栈"<<ch<<"  读取下一个"<<endl;
				ch = e.at(read_location++);
			}
			else if (isp(ch1) > icp(ch))
			{
				op = s.top();
				s.pop();
				//cout<<"退栈"<<op<<" 添加到输出字符串"<<endl;
				Postfix_Expression_String = Postfix_Expression_String + op;
				//cout<<op;
			}
			else  //考虑优先级相等的情况
			{
				op = s.top();
				s.pop();
				//cout<<"退栈"<<op<<"  但不添加到输入字符串"<<endl;

				if (op == '(')
					ch = e.at(read_location++);
			}
		}
	}
	return Postfix_Expression_String;
}


/*表达式转NFA处理函数,返回最终的NFA结合*/
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
	cout << "处理完毕!" << endl;
	Cell = STACK.top();
	STACK.pop();

	return Cell;
}


//显示NFA

void thompson::Display(cell Cell)
{
	ofstream of("toNFA.txt");
	of << "NFA 的边数：" << Cell.EdgeCount << endl;
	of << "NFA 的起始状态：" << Cell.StartState.StateName << endl;
	of << "NFA 的结束状态：" << Cell.EndState.StateName << endl;
	for (int i = 0; i < Cell.EdgeCount; i++)
	{
		of << "第" << i + 1 << "条边的起始状态：" << Cell.EdgeSet[i].StartState.StateName
			<< "  结束状态：" << Cell.EdgeSet[i].EndState.StateName
			<< "  转换符：" << Cell.EdgeSet[i].TransSymbol << endl;
	}
	of << "结束" << endl;
	of.close();
}
