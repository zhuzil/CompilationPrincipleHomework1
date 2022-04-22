#include "pch.h"
#include "transcode.h"

int Count;
//求e闭包函数
void NFAtoDFA::EClosure()
{
	eclosure = new set<int>[StatesNum];
	for (int i = 0; i < StatesNum; ++i) {
		eclosure[i].insert(i);
		queue<int> que;
		que.push(i);//使用一个队列维护来进行宽度优先搜索（先找一步可达的所有位置）
		while (!que.empty()) {
			int ii = que.front();
			for (int j = 0; j < StatesNum; ++j) {
				if (TranTable[ii][j] == 'e'/*&&node_epsilon[i].find(j)==node_epsilon[i].end()*/) {
					eclosure[i].insert(j);
					que.push(j);
				}
			}
			que.pop();
		}
	}
}
void NFAtoDFA::Show(const set<int>& a)
{
	if (a.size() == 0)return;
	//cout<<a.size();
	cout << '{';
	set<int>::iterator it;
	for (it = a.begin(); it != a.end(); ++it)
		cout << *it << ',';
	cout << '\b' << '}';
}
bool NFAtoDFA::IsSame(set<int>& a, set<int>& b)
{
	if (a.size() != b.size())return false;
	else {
		set<int>::iterator ia = a.begin();
		set<int>::iterator ib = b.begin();
		for (; ia != a.end(); ++ia, ++ib) {
			if (*ia != *ib)return false;
		}
	}
	return true;
}
void NFAtoDFA::ReadInf()
{
	ifstream in;
	in.open("NFA.txt", ios::in);
	//转移字符个数
	in >> CharNum;
	//转移字符表
	TranChar = new char[CharNum + 1];
	TranChar[0] = 'e';
	for (int j = 1; j <= CharNum; j++)
		in >> TranChar[j];
	//状态个数
	in >> StatesNum;
	States = new int[StatesNum];
	for (int i = 0; i < StatesNum; i++)
		States[i] = i;
	//转移状态表
	TranTable = new char* [StatesNum];
	for (int i = 0; i < StatesNum; i++)
		TranTable[i] = new char[StatesNum];
	for (int i = 0; i < StatesNum; i++)
		for (int j = 0; j < StatesNum; j++)
			TranTable[i][j] = ' ';
	int start = 0;
	int end = 0;
	char c;
	while (in >> start >> end >> c)
	{
		if (start == 0 && end == 0 && c == 'e')break;
		TranTable[start][end] = c;
	}
	in.close();
}
void NFAtoDFA::NFATODFA()
{
	cout << endl << "NFA确定化过程中构造的状态表为：\n\n";
	DFAset.push_back(eclosure[0]);
	while (Count < DFAset.size())
	{
		cout << "s" << Count << ":   ";
		Show(DFAset[Count]);
		cout << '\t' << '\t';
		for (int i = 1; i < CharNum + 1; ++i) //对每个转移字符
		{
			//该集合接收c后
			char c = TranChar[i];
			set<int> temp;
			for (set<int>::iterator it = DFAset[Count].begin(); it != DFAset[Count].end(); it++)//对原始DFA集合中的每个状态
				for (int j = 0; j < StatesNum; ++j)
					if (TranTable[*it][j] == c)
						temp.insert(eclosure[j].begin(), eclosure[j].end());
			Show(temp);
			cout << '\t' << '\t';

			//判断该集合之前是否存在
			bool flag = false;
			for (int i = 0; i < DFAset.size(); i++)
				if (IsSame(DFAset[i], temp))
				{
					flag = true;
					break;
				}
			if (!flag)
				DFAset.push_back(temp);
		}
		Count++;
		cout << endl;
	}
}
void NFAtoDFA::Memory()
{//实际上是把转化过程再做一遍
	ofstream o;
	o.open("DFA.txt", ios::out | ios::trunc);
	int DFANum = DFAset.size();
	int finalNum = 0;
	//判断DFA中哪些状态为终态,记录终态数目
	for (int i = 0; i < DFAset.size(); i++)
	{
		for (set<int>::iterator it = DFAset[i].begin(); it != DFAset[i].end(); it++)
			if (*it == StatesNum - 1)
			{
				finalNum++;
				break;
			}
	}
	o << Count << "\t" << CharNum << "\t" << 1 << "\t" << finalNum << endl;
	o << 0 << endl;
	//判断DFA中哪些状态为终态,记录终态标号
	for (int i = 0; i < DFAset.size(); i++)
	{
		for (set<int>::iterator it = DFAset[i].begin(); it != DFAset[i].end(); it++)
			if (*it == StatesNum - 1)
			{
				o << i << "\t";
				break;
			}
	}
	o << endl;
	for (int j = 1; j <= CharNum; j++)
		o << TranChar[j] << "\t";
	o << endl;
	vector<set<int> > DFAsetcopy;
	DFAsetcopy.push_back(eclosure[0]);
	int num = 0;
	while (num < DFAsetcopy.size())
	{
		for (int i = 1; i < CharNum + 1; ++i) //对每个转移字符
		{
			//该集合接收c后
			char c = TranChar[i];
			set<int> temp;
			for (set<int>::iterator it = DFAsetcopy[num].begin(); it != DFAsetcopy[num].end(); it++)//对原始DFA集合中的每个状态
				for (int j = 0; j < StatesNum; ++j)
					if (TranTable[*it][j] == c)
						temp.insert(eclosure[j].begin(), eclosure[j].end());

			//判断该集合之前是否存在
			bool flag = false;
			for (int i = 0; i < DFAsetcopy.size(); i++)
				if (IsSame(DFAsetcopy[i], temp))
				{
					flag = true;
					break;
				}
			if (!flag)
				DFAsetcopy.push_back(temp);

			//判断该集合是DFAset中的哪一个
			for (int i = 0; i < DFAset.size(); i++)
				if (IsSame(DFAset[i], temp))
					o << num << "\t" << c << "\t" << i << endl;
		}
		num++;
	}
	o.close();
}