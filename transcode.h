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

//�ڵ㣬����ɽṹ�壬�����Ժ���չ
struct state
{
	string StateName;
};

//�ߣ���ת������'#'��ʾ
struct edge
{
	state StartState;
	state EndState;
	string TransSymbol;
};

//NFA��Ԫ
struct cell
{
	edge EdgeSet[MAX];
	int EdgeCount;
	state StartState;
	state EndState;
};

/***************NFA�ľ���ṹ****************/

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
		DFA.EndState.StateName.append("����� ��ʶ�� ����");
		DFA.StartState.StateName.append("��ʼ");
		DFA.EdgeSet[0].StartState.StateName.append("��ʼ");
		DFA.EdgeSet[0].TransSymbol.append("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
		DFA.EdgeSet[0].EndState.StateName.append("��ʶ��");
		DFA.EdgeSet[1].StartState.StateName.append("��ʶ��");
		DFA.EdgeSet[1].TransSymbol.append("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890");
		DFA.EdgeSet[1].EndState.StateName.append("��ʶ��");
		DFA.EdgeSet[2].StartState.StateName.append("��ʼ");
		DFA.EdgeSet[2].TransSymbol.append("0123456789");
		DFA.EdgeSet[2].EndState.StateName.append("����");
		DFA.EdgeSet[3].StartState.StateName.append("����");
		DFA.EdgeSet[3].TransSymbol.append("0123456789");
		DFA.EdgeSet[3].EndState.StateName.append("����");
		DFA.EdgeSet[4].StartState.StateName.append("��ʼ");
		DFA.EdgeSet[4].TransSymbol.append("+-*/=");
		DFA.EdgeSet[4].EndState.StateName.append("�����");
		DFA.EdgeSet[5].StartState.StateName.append("�����");
		DFA.EdgeSet[5].TransSymbol.append("=");
		DFA.EdgeSet[5].EndState.StateName.append("�����");
		DFA.EdgeSet[6].StartState.StateName.append("��ʼ");
		DFA.EdgeSet[6].TransSymbol.append("&");
		DFA.EdgeSet[6].EndState.StateName.append("�������");
		DFA.EdgeSet[7].StartState.StateName.append("�������");
		DFA.EdgeSet[7].TransSymbol.append("&");
		DFA.EdgeSet[7].EndState.StateName.append("�������");
		DFA.EdgeCount = 7;
		
		SET.insert("int");
		SET.insert("double");
		SET.insert("include");
		SET.insert("for");
		SET.insert("public");
		SET.insert("return");
		SET.insert("if");
		SET.insert("import");

		SET.insert("package");
		SET.insert("static");
		SET.insert("private");
		SET.insert("const");
		SET.insert("new");

		SET.insert("else");
		SET.insert("for");
		SET.insert("while");
	}
	state RunTo(cell& DFA, state& sta, const char& c) {
		for (int i = 0; i < DFA.EdgeCount; i++) {
			if (sta.StateName == DFA.EdgeSet[i].StartState.StateName && DFA.EdgeSet[i].TransSymbol.find(c) != string::npos) {
				return DFA.EdgeSet[i].EndState;
			}
		}
		return DFA.StartState;
	}
	void Func(string Dir) {
		//ģ��DFA�ĺ���
		//ifstream If(Dir);
		//��sstream����������
		stringstream If(Dir);
		ofstream Of("ta.txt");
		char c = ' ';
		while (!If.eof()) {
			state Sta = DFA.StartState;
			//�����ļ��е�ÿһ���ַ���
			vector<string> V1, V2;
			int i = 0;
			If.get(c);
			//Of << c;
			while (c != '\n' && !If.eof())
			{

				while (c != ' ' && c != '\n' && c != ',' && c != ';' && c != '\t' && c != '(' && c != ')' && c != '{' && c != '}' && c != '[' && c != ']' && c != '.' && c != ':') {

					Sta = RunTo(DFA, Sta, c);

					string x;
					x = c;
					if (V1.empty() || i >= V1.size()) {

						V1.push_back(x);
						If.get(c);
						//Of << c;
						continue;
					}
					V1[i].append(x);//�������ĸ
					If.get(c);
					//Of << c;

				}
				if (!V1.empty() && i < V1.size()) {
					if (SET.find(V1[i]) != SET.end()) {
						V2.push_back("�����ؼ���");
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
				Of << V1[j] << ':' << V2[j] << endl;
			}
			Of << endl;

		}

		Of.close();
	}
};

class thompson{
public:
};

class NFAtoDFA{
public:
	//��e�հ�����
	void EClosure();
	void NFATODFA();
	void Memory();
	void ReadInf();
	int CharNum = 0;	//ת���ַ�����
	char* TranChar;		//ת���ַ�����
	int StatesNum = 0;	//״̬����
	int* States;		//״̬����
	char** TranTable;	//ת�ƾ���
	set<int>* eclosure;	//ÿ��״̬�ıհ���
	vector<set<int>> DFAset;//DFA���м�״̬
	void Show(const set<int>& a);//���e�հ�����
	bool IsSame(set<int>& a, set<int>& b); //�ж������հ��Ƿ���Ⱥ���

	void run() {
		ReadInf();
		EClosure();
		NFATODFA();
		Memory();
	}
};

class minimizeDFA {
public:
	int m; // ״̬��Ŀ
	int sn; // ��ʼ״̬��Ŀ
	int* s; // ��ʼ״̬����
	int an; // ����״̬��Ŀ
	int* a; // ����״̬����
	int n; // ��ĸ��Ĺ�ģ
	char* t; // ��ĸ��
	int* c[1000]; // ״̬ת������
	ifstream inc;
	ofstream ouc;

	minimizeDFA() {
		inc.open("complexdfa.txt");
		ouc.open("mindfa.txt");
	}

	void set(int q1, int q2, int q3, int q4) { // ����ʹ洢DFA
		
		int i;
		m = q1, n = q2, sn = q3, an = q4;
		s = new int[sn];
		for (i = 0; i < sn; i++) inc >> s[i]; // ���뿪ʼ״̬����
		a = new int[an];
		for (i = 0; i < an; i++) inc >> a[i]; // �������״̬����
		t = new char[n];
		for (i = 0; i < n; i++) inc >> t[i]; // ������ĸ��
		for (i = 0; i < m; i++) c[i] = new int[n]; // ��ʼ��ת����,��m��״̬������n����ĸת��
		//inc.ignore();
		string line;
		while (getline(inc, line)) {
			stringstream ss(line);//���ڷָ��ַ���
			if (line != " ") {
				int state = -1;
				char letter = ' ';
				int afterState = -1;
				while (ss) {
					ss >> state >> letter >> afterState;
					int charLoc = include(letter);
					c[state][charLoc] = afterState;// ����״̬ת����c[i][j]��ʾ״̬i������ĸjת�����״̬
				}
			}

		}
	}
	void remove() {
		int i;
		delete[] s; delete[] a; delete[] t;
		for (i = 0; i < m; i++) delete[] c[i];
	}
	int include(char ch) {//����ch����ĸ���е�λ��
		for (int i = 0; i < n; i++) {
			if (ch == t[i]) return i;
		}
		return -1;
	}

	bool correct() { // ��������DFA�Ƿ�ϸ�
		int i, j;
		for (i = 0; i < n; i++) for (j = i + 1; j < n; j++) { // �����ĸ���Ƿ��ظ�
			if (t[i] == t[j]) return false;
		}

		if (sn == 1) { // ��鿪ʼ״̬�Ƿ�Ψһ���Ƿ���״̬��������
			if (*s >= 0 && *s < m) return true;
			else return false;
		}
		else return false;

		if (an == 0) return false; // ������״̬�����Ƿ�Ϊ��
		else {
			for (i = 0; i < an; i++) {
				if (a[i] < 0 || a[i] >= m) return false; // ���״̬�Ƿ���״̬������
				for (j = i + 1; j < an; j++) { // ������״̬�Ƿ��ظ�
					if (a[i] == a[j]) return false;
				}
			}
		}

		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++) { // ���״̬ת�����Ƿ�ϸ�
				if (c[i][j] < -1 || c[i][j] >= m) return false;
			}
		// �����������֮�����ڲ�����ֲ�ȫ��״̬ת��������
	}
	bool detect(int d[], int len) {//len��ʾ��ĸ��ĳ��ȣ�d[]��ĸ������
		int i, j;
		j = *s;
		for (i = 0; i < len; i++) { // DFA״̬ת��
			j = c[j][d[i]];
			if (j < 0) return false;
		}
		for (i = 0; i < an; i++) { // ��������Ƿ��䵽����״̬
			if (j == a[i]) return true;
		}
		return false;
	}
	void written() { // д������
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
	}
	void output() { // д������
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
		int* s; // ���ϻ�������
		int up, tp; // �»��ֵļ��Ϲ�ģ����������
		bool test = true;
		s = new int[D.m];//��ÿ��״̬��ţ������ͬ�ĵ�״̬��ͬһ������
		//s[i]=0��ʾ״̬i���ڵļ��ϱ��Ϊ0
		for (i = 0; i < D.m; i++) s[i] = 0; // ��ʼ���������ͬ��Ϊ0������Ԫ����ͬһ��������
		if (D.m == D.an) up = 1;//���е�״̬���ǽ���״̬������Ϊһ������
		else {
			up = 2;//��̬�ͷ���̬������Ϊһ�����ϣ��ܹ�Ϊ2��
			for (i = 0; i < D.an; i++) s[D.a[i]] = 1; // ���е���̬���Ϊ1������̬���Ϊ0
		}
		while (test) { // ���ּ��ϵĲ���
			test = false;
			for (i = 0; i < up; i++) { // ����ÿ������
				for (j = 0; j < D.m; j++) { // �ҵ���i�����ϵĵ�һ��״̬
					if (s[j] == i) break;
				}
				for (k = 0; k < D.n; k++) { // ����ÿ����ĸ
					tp = D.c[j][k]; // ״̬j����ĸk�µ�ת��״̬
					if (tp >= 0) tp = s[tp]; // tp״̬���ڵļ���
					test = false;
					for (l = j + 1; l < D.m; l++) { // ���������ü��ϵ�״̬
						if (s[l] == i) { // �Ǹü������
							m = D.c[l][k];
							if (m >= 0) m = s[m];
							if (m != tp) { // ��ĸk����״ָ̬��ͬ��״̬
								test = true;
								s[l] = up; // �����µļ���
							}
						}
					}
					if (test) up++; // ���ִ������չ���ϲ�����������չ
				}
			}
		}
		F.sn = F.an = 0; F.m = up; F.n = D.n;
		F.s = new int[D.sn]; F.a = new int[D.an];
		for (i = 0; i < D.sn; i++) { // ������Զ����Ŀ�ʼ����
			if (F.sn == 0) F.s[F.sn++] = s[D.s[i]];
			else {
				for (j = 0; j < F.sn; j++) {
					if (s[D.s[i]] == F.s[j]) break; // �Ѿ��������
				}
				if (j == F.sn) F.s[F.sn++] = s[D.s[i]];
			}
		}
		for (i = 0; i < D.an; i++) { // ������Զ����Ľ���״̬����
			if (F.an == 0) F.a[F.an++] = s[D.a[i]];
			else {
				for (j = 0; j < F.an; j++) {
					if (s[D.a[i]] == F.a[j]) break;
				}
				if (j == F.an) F.a[F.an++] = s[D.a[i]];//û�м��룬�ͽ�����̬���ڵļ��ϱ�ż���F����̬������
			}
		}
		F.t = new char[D.n]; // ��ĸ��
		for (i = 0; i < D.n; i++) F.t[i] = D.t[i];
		for (i = 0; i < F.m; i++) F.c[i] = new int[F.n]; // ״̬ת�����ʼ��
		for (i = 0; i < F.m; i++) for (j = 0; j < F.n; j++)
			F.c[i][j] = -1;
		for (i = 0; i < D.m; i++) { // ״̬ת���������
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
		minimizeDFA D, F; int m, n, sn, an;
		while (inc >> m >> n >> sn >> an) { // ��������״̬������ĸ������ʼ״̬��Ŀ������״̬��Ŀ
			D.set(m, n, sn, an);
			if (!D.correct()) {
				cout << "Unqualified DFA! \n";
				continue;
			}
			create(D, F);
			F.written(); F.output();
			D.remove(); F.remove();
		}
		inc.close();
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
		//��ʼ��
		//project1.run();
		project2.run();
		project3.run();
	}

	void simulate(string input) {
		project4.Func(input);
	}
};