#include <iostream>
#include <fstream>
#include <string> 
#include <Windows.h>

using namespace std;

typedef int Info;
struct Node {
	Node* left, * right;
	Info  info;
	int   count;
};

void  PrintTree(Node* root, int level);
Node* Insert(Node* r, Info value);
Node* BinarySearchInsert(Node*& root, Info value, bool& found);
Node* rightRotation(Node* r);
Node* leftRotation(Node* r);
void LeftRotation(Node*& root);
void RightRotation(Node*& root);
Node* FindMin(Node* root);
Node* deleteMin(Node* p);
Node* Delete(Node* p, int value);
Node* Find(Node* root, Info value);
void LoadFromFile(Node*& root, string filename);
int Height(Node* root);
int BFactor(Node* root);
Node* balanceHeight(Node* r);
void BalanceHeight(Node*& root);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Node* root = NULL;
	Info value;
	bool found;
	LoadFromFile(root, "t.txt");

	int choice;
	do {
		cout << "Оберіть номер потрібної команди:" << endl << endl;
		cout << " [1] - Знайти вузол " << endl;
		cout << " [2] - Додати вузол " << endl;
		cout << " [3] - Видалити вузол " << endl;
		cout << endl;
		cout << " [0] - Вихід із програми" << endl << endl;
		cout << "Введіть номер команди: ";
	
		cin >> choice;
		cout << endl;
		switch (choice) {
		case 1:
			cout << "Введіть значення, яке треба знайти: " << endl;
			cin >> value;
			if (Find(root, value) == NULL) {
				cout << "Вузол із заданим значенням НЕ знайдено." << endl;
			}
			else {
				cout << "Вузол із заданим значенням знайдено." << endl;
			}
			break;
		case 2:
			cout << "Введіть значення, яке треба додати: " << endl;
			cin >> value;
			root = Insert(root, value);
			PrintTree(root, 0);
			break;
		case 3:
			cout << "Введіть значення, яке треба видалити: " << endl;
			cin >> value;
			root = Delete(root, value);
			PrintTree(root, 0);
			break;
		case 0:
			break;
		default:
			cout << "Ви ввели неправильний номер команди " << endl;
		}
	}	while (choice != 0);
	return 0;
}

void LoadFromFile(Node*& root, string filename) {
	ifstream f(filename);
	if (!f) {
		cerr << "file " << filename << " not found" << endl;
		exit(1);
	}
	Info value;
	bool found;
	while (f >> value) {
		cout << "Додати число " << value << endl;
		BinarySearchInsert(root, value, found);
	}
	cout << endl;
	cout << "Незбалансоване дерево: " << endl;
	PrintTree(root, 0);
	cout << endl;
	
	BalanceHeight(root);
	cout << endl;
	cout << "Збалансоване дерево: " << endl;
	PrintTree(root, 0);
	cout << endl;
}

Node* Find(Node* root, Info value) {
	if (root == NULL || value == root->info) return root;

	if (value < root->info) return Find(root->left, value);
	else return Find(root->right, value);
}

void PrintTree(Node* root, int level) {
	if (root != NULL) {
		PrintTree(root->right, level + 1);

		for (int i = 1; i <= level; i++) {
			cout << "  ";
		}
		cout << root->info << endl;

		PrintTree(root->left, level + 1);
	}
}

Node* rightRotation(Node* p) {
	Node* q = p->left;
	p->left = q->right;
	q->right = p;
	return q;
}

Node* leftRotation(Node* q) {
	Node* p = q->right;
	q->right = p->left;
	p->left = q;
	return p;
}

int Height(Node* root) {
	if (root == NULL) return 0;

	int hL = Height(root->left);
	int hR = Height(root->right);

	return (hL > hR ? hL : hR) + 1;
}

int BFactor(Node* root) {
	return Height(root->right) - Height(root->left);
}

Node* balanceHeight(Node* p) {
	if (BFactor(p) == 2) {
		if (BFactor(p->right) < 0) p->right = rightRotation(p->right);
		return leftRotation(p);
	}

	if (BFactor(p) == -2) {
		if (BFactor(p->left) > 0) p->left = leftRotation(p->left);
		return rightRotation(p);
	}
	return p;
}

void LeftRotation(Node*& root) {
	Node* tmp1 = root->right;
	Node* tmp2 = tmp1->left;
	tmp1->left = root;
	root->right = tmp2;
	root = tmp1;
}

void RightRotation(Node*& root) {
	Node* tmp1 = root->left;
	Node* tmp2 = tmp1->right;
	tmp1->right = root;
	root->left = tmp2;
	root = tmp1;
}

void BalanceHeight(Node*& root) {
	if (root != NULL) {
		while (BFactor(root) >= 2) {
			if (BFactor(root->right) < 0) RightRotation(root->right);
			LeftRotation(root);
		}
		while (BFactor(root) <= -2) {
			if (BFactor(root->left) > 0) LeftRotation(root->left);
			RightRotation(root);
		}

		BalanceHeight(root->left);
		BalanceHeight(root->right);
	}
}

Node* Insert(Node* p, Info value) {
	if (!p) {
		p = new Node;
		p->info = value;
		p->left = NULL;
		p->right = NULL;
		return p;
	}

	if (value < p->info) p->left = Insert(p->left, value);
	else if (value > p->info) p->right = Insert(p->right, value);

	return balanceHeight(p);
}

Node* BinarySearchInsert(Node*& root, Info value, bool& found) {
	if (root == NULL) {
		root = new Node;
		root->info = value;
		root->left = NULL;
		root->right = NULL;
		found = false;
		return root;
	}
	else if (value == root->info) {
		found = true;
		return root;
	}
	else if (value < root->info) return BinarySearchInsert(root->left, value, found);
	else return BinarySearchInsert(root->right, value, found);
}

Node* FindMin(Node* root) {
	if (root->left != NULL) return FindMin(root->left);
	else return root;
}

Node* deleteMin(Node* p) {
	if (p->left == 0) return p->right;
	p->left = deleteMin(p->left);
	return balanceHeight(p);
}

Node* Delete(Node* p, int value) {
	if (!p)
		return 0;
	 
		p->left = Delete(p->left, value);
		p->right = Delete(p->right, value);
		if (value == p->info)
	{
		Node* q = p->left;
		Node* r = p->right;
		delete p;
		if (!r)
			return q;
		Node* min = FindMin(r);
		min->right = deleteMin(r);
		min->left = q;
		return balanceHeight(min);
	}
	return balanceHeight(p);
}
