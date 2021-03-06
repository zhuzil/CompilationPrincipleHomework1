#pragma once
#include <iostream>
#include <stdio.h>
#include <cctype>
#include <stack>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include<queue>
#include<memory.h> 
#include<algorithm>
using namespace std;

#define MAX 100

//节点，定义成结构体，便于以后扩展
struct state
{
	string StateName;
};

//边，空转换符用'#'表示
struct edge
{
	state StartState;
	state EndState;
	string TransSymbol;
};

//NFA单元
struct cell
{
	edge EdgeSet[MAX];
	int EdgeCount;
	state StartState;
	state EndState;
};

/***************NFA的矩阵结构****************/

struct node
{
	edge* In_edges;
	edge* Out_edges;
	//state
};

class simulateNFA{
public:
	cell DFA;
	set<string> SET;
	simulateNFA() {
		DFA.EndState.StateName.append("运算符 标识符 数字");
		DFA.StartState.StateName.append("开始");

		DFA.EdgeSet[0].StartState.StateName.append("开始");
		DFA.EdgeSet[0].TransSymbol.append("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
		DFA.EdgeSet[0].EndState.StateName.append("标识符");

		DFA.EdgeSet[1].StartState.StateName.append("标识符");
		DFA.EdgeSet[1].TransSymbol.append("qwertyuiopasdfghjklzxcvbnm_QWERTYUIOPASDFGHJKLZXCVBNM1234567890");
		DFA.EdgeSet[1].EndState.StateName.append("标识符");

		DFA.EdgeSet[2].StartState.StateName.append("开始");
		DFA.EdgeSet[2].TransSymbol.append("0123456789");
		DFA.EdgeSet[2].EndState.StateName.append("数字");

		DFA.EdgeSet[3].StartState.StateName.append("数字");
		DFA.EdgeSet[3].TransSymbol.append("0123456789");
		DFA.EdgeSet[3].EndState.StateName.append("数字");

		DFA.EdgeSet[4].StartState.StateName.append("开始");
		DFA.EdgeSet[4].TransSymbol.append("+-*/=");
		DFA.EdgeSet[4].EndState.StateName.append("运算符");

		DFA.EdgeSet[5].StartState.StateName.append("运算符");
		DFA.EdgeSet[5].TransSymbol.append("=");
		DFA.EdgeSet[5].EndState.StateName.append("运算符");

		DFA.EdgeSet[6].StartState.StateName.append("开始");
		DFA.EdgeSet[6].TransSymbol.append("&\"");
		DFA.EdgeSet[6].EndState.StateName.append("特殊符号");

		DFA.EdgeSet[7].StartState.StateName.append("特殊符号");
		DFA.EdgeSet[7].TransSymbol.append("&");
		DFA.EdgeSet[7].EndState.StateName.append("特殊符号");

		DFA.EdgeSet[8].StartState.StateName.append("数字");
		DFA.EdgeSet[8].TransSymbol.append(".");
		DFA.EdgeSet[8].EndState.StateName.append("小数");

		DFA.EdgeSet[8].StartState.StateName.append("小数");
		DFA.EdgeSet[8].TransSymbol.append("0123456789");
		DFA.EdgeSet[8].EndState.StateName.append("小数");

		DFA.EdgeSet[9].StartState.StateName.append("开始");
		DFA.EdgeSet[9].TransSymbol.append("<>:");
		DFA.EdgeSet[9].EndState.StateName.append("比较运算符");

		DFA.EdgeSet[10].StartState.StateName.append("比较运算符");
		DFA.EdgeSet[10].TransSymbol.append("=");
		DFA.EdgeSet[10].EndState.StateName.append("比较运算符");

		DFA.EdgeSet[11].StartState.StateName.append("开始");
		DFA.EdgeSet[11].TransSymbol.append(".,();{}");
		DFA.EdgeSet[11].EndState.StateName.append("界符");

		DFA.EdgeSet[12].StartState.StateName.append("开始");
		DFA.EdgeSet[12].TransSymbol.append(":");
		DFA.EdgeSet[12].EndState.StateName.append("赋值符号");

		DFA.EdgeSet[13].StartState.StateName.append("赋值符号");
		DFA.EdgeSet[13].TransSymbol.append("=");
		DFA.EdgeSet[13].EndState.StateName.append("赋值符号");

		DFA.EdgeSet[14].StartState.StateName.append("关键字");
		DFA.EdgeSet[14].TransSymbol.append(".,();{}:");
		DFA.EdgeSet[14].EndState.StateName.append("结束");

		DFA.EdgeSet[15].StartState.StateName.append("数字");
		DFA.EdgeSet[15].TransSymbol.append(".,();{}:");
		DFA.EdgeSet[15].EndState.StateName.append("结束");

		DFA.EdgeSet[16].StartState.StateName.append("小数");
		DFA.EdgeSet[16].TransSymbol.append(".,();{}:");
		DFA.EdgeSet[16].EndState.StateName.append("结束");

		DFA.EdgeSet[17].StartState.StateName.append("开始");
		DFA.EdgeSet[17].TransSymbol.append("#");
		DFA.EdgeSet[17].EndState.StateName.append("不等号");

		DFA.EdgeSet[18].StartState.StateName.append("开始");
		DFA.EdgeSet[18].TransSymbol.append(";");
		DFA.EdgeSet[18].EndState.StateName.append("界符");
		DFA.EdgeCount = 18;

		SET.insert("int");
		SET.insert("double");
		SET.insert("include");
		SET.insert("for");
		SET.insert("public");
		SET.insert("return");
		SET.insert("if");
		SET.insert("import");
		SET.insert("begin");
		SET.insert("package");
		SET.insert("static");
		SET.insert("private");
		SET.insert("const");
		SET.insert("new");

		SET.insert("else");
		SET.insert("for");
		SET.insert("while");

		SET.insert("write");
		SET.insert("call");
		SET.insert("do");
		SET.insert("end");
		SET.insert("odd");
		SET.insert("procedure");
		SET.insert("read");
		SET.insert("var");
	}
	state RunTo(cell& DFA, state& sta, const char& c) {
		for (int i = 0; i < DFA.EdgeCount; i++) {
			if (sta.StateName == DFA.EdgeSet[i].StartState.StateName && DFA.EdgeSet[i].TransSymbol.find(c) != string::npos) {
				return DFA.EdgeSet[i].EndState;
			}
		}
		return DFA.StartState;
	}

	bool CanRead(char & c) {
		std::string S = "0123456789/*-+.qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_={}[];:#%";
		if (S.find(c) != string::npos)
			return true;
		return false;
	}

	void Func(string Dir) {
		//模拟DFA的函数
		//ifstream If(Dir);
		//用sstream流读入数据
		stringstream If(Dir);
		ofstream Of("ta.txt");
		char c = ' ';
		while (!If.eof()) {
			state Sta = DFA.StartState;
			//读入文件中的每一个字符；
			vector<string> V1, V2;
			int i = 0;
			If.get(c);
			//Of << c;
			while (c != '\n' && !If.eof())
			{

				while (CanRead(c)&& !If.eof()) {

					Sta = RunTo(DFA, Sta, c);

					string x;
					x = c;
					
					if (Sta.StateName == "结束") {
						V1.push_back(x);
						V2.push_back("界符");
						i++;
						If.get(c);
						continue;
					}
					
					if (V1.empty() || i >= V1.size()) {

						V1.push_back(x);
						If.get(c);
						//Of << c;
						continue;
					}
					V1[i].append(x);//加入字该母
					If.get(c);
					//Of << c;
				}

				if (!V1.empty() && i < V1.size()) {
					if (SET.find(V1[i]) != SET.end()) {
						V2.push_back("保留关键字");
						i++;
					}
					else {
						V2.push_back(Sta.StateName);
						i++;
					}
				}
				Sta.StateName = DFA.StartState.StateName;
				If.get(c);
				//Of << c;
			}
			Of << endl;
			for (int j = 0; j < i; j++) {
				Of << V1[j] << " : " << V2[j] << endl;
			}
			Of << endl;

		}

		Of.close();
	}
};

class thompson{
public:
	//函数声明
	void input(string& , string);
	bool check_Islegal(string);
	bool check_Ischaracter(string);
	bool check_Ismatching(string);
	bool is_letter(char);
	//添加“+”，便于转后缀表达式
	string add_plus_symbol(string);
	//中缀转后缀
	string turn_postfix(string);
	//优先级 in stack priority
	int isp(char);
	//优先级 in coming priority
	int icp(char);
	//表达式转NFA
	cell Expression_to_NFA(string);
	//处理 a|b
	cell do_Unite(cell, cell);
	//处理 ab
	cell do_Join(cell, cell);
	//处理 a*
	cell do_Star(cell);
	//处理 a
	cell do_Cell(char);
	//将一个单元的边的集合复制到另一个单元
	void cell_EdgeSet_Link(cell&, cell);
	//产生一个新的状态节点，便于管理
	state new_StateNode();
	//显示
	void Display(cell);
	int STATE_NUM = 0;

	void Func(string in) {
		string Regular_Expression;
		cell NFA_Cell;
		//接受输入
		input(Regular_Expression, in);
		//添加“+”，便于转后缀表达式
		Regular_Expression = add_plus_symbol(Regular_Expression);
		//中缀转后缀
		Regular_Expression = turn_postfix(Regular_Expression);
		//表达式转NFA
		NFA_Cell = Expression_to_NFA(Regular_Expression);
		//显示
		Display(NFA_Cell);
	}
};

class NFAtoDFA{
public:
	//求e闭包函数
	void EClosure();
	void NFATODFA();
	void Memory();
	void ReadInf(string Dir);
	int CharNum = 0;	//转移字符个数
	char* TranChar;		//转移字符数组
	int StatesNum = 0;	//状态个数
	int* States;		//状态数组
	char** TranTable;	//转移矩阵
	set<int>* eclosure;	//每个状态的闭包集
	vector<set<int>> DFAset;//DFA的中间状态
	void Show(const set<int>& a);//输出e闭包函数
	bool IsSame(set<int>& a, set<int>& b); //判断两个闭包是否相等函数

	void run(string Dir) {
		ReadInf(Dir);
		EClosure();
		NFATODFA();
		Memory();
	}
};


class minimizeDFA {
public:
	int m; // 状态数目
	int sn; // 开始状态数目
	int* s; // 开始状态集合
	int an; // 接收状态数目
	int* a; // 接受状态集合
	int n; // 字母表的规模
	char* t; // 字母表
	int* c[1000]; // 状态转换函数
	ifstream inc;
	ofstream ouc;
	minimizeDFA() {
		
		
	}

	void set() { // 输入和存储DFA
		//inc.open("complexdfa.txt");
		inc.open("complexdfa.txt");
		inc >> m >> n >> sn >> an;
		int i;
		//m = q1, n = q2, sn = q3, an = q4;
		s = new int[sn];
		for (i = 0; i < sn; i++) inc >> s[i]; // 输入开始状态集合
		a = new int[an];
		for (i = 0; i < an; i++) inc >> a[i]; // 输入接受状态集合
		t = new char[n+10];
		for (i = 0; i < n; i++) {
			inc >> t[i]; // 输入字母表
		}
		for (i = 0; i < m; i++) c[i] = new int[n]; // 初始化转换表,有m个状态，经过n个字母转换
		//inc.ignore();
		string line;
		while (getline(inc, line)) {
			stringstream ss(line);//用于分割字符串
			if (line != " ") {
				int state = -1;
				char letter = ' ';
				int afterState = -1;
				while (ss) {
					ss >> state >> letter >> afterState;
					int charLoc = include(letter);
					c[state][charLoc] = afterState;// 输入状态转换表，c[i][j]表示状态i经过字母j转换后的状态
				}
			}

		}
		inc.close();

	}
	void remove() {
		int i;
		delete[] s; 
		delete[] a;
		//delete[] t;
		for (i = 0; i < m; i++) delete[] c[i];
	}
	int include(char ch) {//返回ch在字母表中的位置
		for (int i = 0; i < n; i++) {
			if (ch == t[i]) return i;
		}
		return -1;
	}

	bool correct() { // 检查输入的DFA是否合格
		int i, j;
		for (i = 0; i < n; i++) for (j = i + 1; j < n; j++) { // 检查字母表是否重复
			if (t[i] == t[j]) return false;
		}

		if (sn == 1) { // 检查开始状态是否唯一且是否在状态集合里面
			if (*s >= 0 && *s < m) return true;
			else return false;
		}
		else return false;

		if (an == 0) return false; // 检查接受状态集合是否为空
		else {
			for (i = 0; i < an; i++) {
				if (a[i] < 0 || a[i] >= m) return false; // 检查状态是否在状态集合中
				for (j = i + 1; j < an; j++) { // 检查接受状态是否重复
					if (a[i] == a[j]) return false;
				}
			}
		}

		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++) { // 检查状态转换表是否合格
				if (c[i][j] < -1 || c[i][j] >= m) return false;
			}
		// 本程序的特殊之处在于不会出现不全的状态转换表输入
	}
	bool detect(int d[], int len) {//len表示字母表的长度，d[]字母表的序号
		int i, j;
		j = *s;
		for (i = 0; i < len; i++) { // DFA状态转换
			j = c[j][d[i]];
			if (j < 0) return false;
		}
		for (i = 0; i < an; i++) { // 检查最终是否落到接受状态
			if (j == a[i]) return true;
		}
		return false;
	}
	void written() { // 写出函数
		ouc.open("mindfa.txt");
		int i, j;
		ouc << m << ' ' << n << ' ' << sn << ' ' << an << endl;
		for (i = 0; i < sn; i++) ouc << s[i] << ' ';
		ouc << endl;
		for (i = 0; i < an; i++) ouc << a[i] << ' ';
		ouc << endl;
		for (i = 0; i < n; i++) ouc << t[i] << ' ';
		ouc << endl;
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				ouc << c[i][j] << ' ';
			}
			ouc << endl;
		}
		ouc << endl;
		ouc.close();
	}
	void output() { // 写出函数
		int i, j;
		cout << "The new DFA: \n";
		cout << m << ' ' << n << ' ' << sn << ' ' << an << endl;
		for (i = 0; i < sn; i++) cout << s[i] << ' ';
		cout << endl;
		for (i = 0; i < an; i++) cout << a[i] << ' ';
		cout << endl;
		for (i = 0; i < n; i++) cout << t[i] << ' ';
		cout << endl;
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				cout << c[i][j] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}
	void create(minimizeDFA& D, minimizeDFA& F) {
		int i, j, k, l, m;
		int* s; // 集合划分数组
		int up, tp; // 新划分的集合规模、辅助数字
		bool test = true;
		s = new int[D.m];//给每个状态标号，标号相同的的状态在同一集合中
		//s[i]=0表示状态i所在的集合编号为0
		for (i = 0; i < D.m; i++) s[i] = 0; // 初始化，标号相同都为0，所有元素在同一个集合中
		if (D.m == D.an) up = 1;//所有的状态都是接受状态，划分为一个集合
		else {
			up = 2;//终态和非终态各划分为一个集合，总共为2个
			for (i = 0; i < D.an; i++) s[D.a[i]] = 1; // 所有的终态标号为1，非终态标号为0
		}
		while (test) { // 划分集合的操作
			test = false;
			for (i = 0; i < up; i++) { // 遍历每个集合
				for (j = 0; j < D.m; j++) { // 找到第i个集合的第一个状态
					if (s[j] == i) break;
				}
				for (k = 0; k < D.n; k++) { // 遍历每个字母
					tp = D.c[j][k]; // 状态j在字母k下的转换状态
					if (tp >= 0) tp = s[tp]; // tp状态所在的集合
					test = false;
					for (l = j + 1; l < D.m; l++) { // 继续遍历该集合的状态
						if (s[l] == i) { // 是该集合里的
							m = D.c[l][k];
							if (m >= 0) m = s[m];
							if (m != tp) { // 字母k下新状态指向不同的状态
								test = true;
								s[l] = up; // 划入新的集合
							}
						}
					}
					if (test) up++; // 如果执行了扩展集合操作，集合扩展
				}
			}
		}
		F.sn = F.an = 0; F.m = up; F.n = D.n;
		F.s = new int[D.sn]; F.a = new int[D.an];
		for (i = 0; i < D.sn; i++) { // 解决新自动机的开始集合
			if (F.sn == 0) F.s[F.sn++] = s[D.s[i]];
			else {
				for (j = 0; j < F.sn; j++) {
					if (s[D.s[i]] == F.s[j]) break; // 已经插入过了
				}
				if (j == F.sn) F.s[F.sn++] = s[D.s[i]];
			}
		}
		for (i = 0; i < D.an; i++) { // 解决新自动机的接收状态集合
			if (F.an == 0) F.a[F.an++] = s[D.a[i]];
			else {
				for (j = 0; j < F.an; j++) {
					if (s[D.a[i]] == F.a[j]) break;
				}
				if (j == F.an) F.a[F.an++] = s[D.a[i]];//没有加入，就将此终态所在的集合编号加入F的终态集合中
			}
		}
		F.t = new char[D.n]; // 字母表
		for (i = 0; i < D.n; i++) F.t[i] = D.t[i];
		for (i = 0; i < F.m; i++) F.c[i] = new int[F.n]; // 状态转换表初始化
		for (i = 0; i < F.m; i++) for (j = 0; j < F.n; j++)
			F.c[i][j] = -1;
		for (i = 0; i < D.m; i++) { // 状态转换表的设置
			for (j = 0; j < D.n; j++) {
				if (D.c[i][j] < 0) F.c[s[i]][j] = -1;
				else F.c[s[i]][j] = s[D.c[i][j]];
			}
		}

		cout << "The DFA\'s status mapping: \n";
		for (i = 0; i < up; i++) {
			cout << "Status " << i << ": ";
			for (j = 0; j < D.m; j++) {
				if (s[j] == i) cout << j << ", ";
			}
			cout << endl;
		}
	}
	void run() {
		ouc.open("mindfa.txt");
		minimizeDFA D, F; int m, n, sn, an;
		D.set();
		if (!D.correct()) {
			ouc << "Unqualified DFA! \n";
		}
		else {
			create(D, F);
			F.written(); F.output();
			D.remove();
			F.remove();
		}

		//while (inc >> m >> n >> sn >> an) { // 依次输入状态数、字母数、开始状态数目、接受状态数目
		//	D.set(m, n, sn, an);
		//	if (!D.correct()) {
		//		ouc << "Unqualified DFA! \n";
		//		continue;
		//	}
		//	create(D, F);
		//	F.written(); F.output();
		//	D.remove(); 
		//	F.remove();
		//}
		
		ouc.close();
	}
};

class transPL0 : public simulateNFA, public thompson,public NFAtoDFA,public minimizeDFA {
public:
	thompson project1;
	NFAtoDFA project2;
	minimizeDFA project3;
	simulateNFA project4;
	transPL0() {
		//初始化
		//project1.run();
		//project2.run();
		//project3.run();
	}

	void simulate(string input) {
		project4.Func(input);
	}

	void toNFA(string input) {
		project1.Func(input);
	}

	void NFAtoDFA(string input) {
		project2.run(input);
		project3.run();
	}
};