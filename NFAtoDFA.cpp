#include "pch.h"
#include "transcode.h"

int Count;
//��e�հ�����
void NFAtoDFA::EClosure()
{
	eclosure = new set<int>[StatesNum];
	for (int i = 0; i < StatesNum; ++i) {
		eclosure[i].insert(i);
		queue<int> que;
		que.push(i);//ʹ��һ������ά�������п����������������һ���ɴ������λ�ã�
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
	//ת���ַ�����
	in >> CharNum;
	//ת���ַ���
	TranChar = new char[CharNum + 1];
	TranChar[0] = 'e';
	for (int j = 1; j <= CharNum; j++)
		in >> TranChar[j];
	//״̬����
	in >> StatesNum;
	States = new int[StatesNum];
	for (int i = 0; i < StatesNum; i++)
		States[i] = i;
	//ת��״̬��
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
	cout << endl << "NFAȷ���������й����״̬��Ϊ��\n\n";
	DFAset.push_back(eclosure[0]);
	while (Count < DFAset.size())
	{
		cout << "s" << Count << ":   ";
		Show(DFAset[Count]);
		cout << '\t' << '\t';
		for (int i = 1; i < CharNum + 1; ++i) //��ÿ��ת���ַ�
		{
			//�ü��Ͻ���c��
			char c = TranChar[i];
			set<int> temp;
			for (set<int>::iterator it = DFAset[Count].begin(); it != DFAset[Count].end(); it++)//��ԭʼDFA�����е�ÿ��״̬
				for (int j = 0; j < StatesNum; ++j)
					if (TranTable[*it][j] == c)
						temp.insert(eclosure[j].begin(), eclosure[j].end());
			Show(temp);
			cout << '\t' << '\t';

			//�жϸü���֮ǰ�Ƿ����
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
{//ʵ�����ǰ�ת����������һ��
	ofstream o;
	o.open("DFA.txt", ios::out | ios::trunc);
	int DFANum = DFAset.size();
	int finalNum = 0;
	//�ж�DFA����Щ״̬Ϊ��̬,��¼��̬��Ŀ
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
	//�ж�DFA����Щ״̬Ϊ��̬,��¼��̬���
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
		for (int i = 1; i < CharNum + 1; ++i) //��ÿ��ת���ַ�
		{
			//�ü��Ͻ���c��
			char c = TranChar[i];
			set<int> temp;
			for (set<int>::iterator it = DFAsetcopy[num].begin(); it != DFAsetcopy[num].end(); it++)//��ԭʼDFA�����е�ÿ��״̬
				for (int j = 0; j < StatesNum; ++j)
					if (TranTable[*it][j] == c)
						temp.insert(eclosure[j].begin(), eclosure[j].end());

			//�жϸü���֮ǰ�Ƿ����
			bool flag = false;
			for (int i = 0; i < DFAsetcopy.size(); i++)
				if (IsSame(DFAsetcopy[i], temp))
				{
					flag = true;
					break;
				}
			if (!flag)
				DFAsetcopy.push_back(temp);

			//�жϸü�����DFAset�е���һ��
			for (int i = 0; i < DFAset.size(); i++)
				if (IsSame(DFAset[i], temp))
					o << num << "\t" << c << "\t" << i << endl;
		}
		num++;
	}
	o.close();
}