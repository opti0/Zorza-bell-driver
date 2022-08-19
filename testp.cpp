#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
using namespace std;

class listarr
{
	private:
		int* data;
		int count;
		int capacity;

		void copy(listarr& b)
		{
			data = new int[b.capacity];
			count = b.count;
			capacity = b.capacity;
			for (unsigned int i = 0; i < count; i++)
				data[i] = b.data[i];
		}
		void destroy()
		{
			delete[] data;
		}
		void grow()
		{
			capacity *= 2;
			int* temp = new int[capacity];
			for (unsigned int i = 0; i < count; i++)
				temp[i] = data[i];
			delete[] data;
			data = temp;
		}

	public:
		listarr()
		{
			data = new int[8];
			count = 0;
			capacity = 8;
		}
		listarr(listarr& b)
		{
			copy(b);
		}
		listarr(listarr&& b)
		{
			data = b.data;
			count = b.count;
			capacity = b.capacity;
			b.data = new int[8];
			b.count = 0;
			b.capacity = 8;
		}
		listarr& operator= (listarr& b)
		{
			if (this == &b) return *this;
			destroy();
			copy(b);
			return *this;
		}
		listarr& operator= (listarr&& b)
		{
			if (this == &b) return *this;
			destroy();
			data = b.data;
			count = b.count;
			capacity = b.capacity;
			b.data = new int[8];
			b.count = 0;
			b.capacity = 8;
			return *this;
		}
		~listarr()
		{
			destroy();
		}
		void addfirst(int i)
		{
			if (count == capacity) grow();
			for (unsigned int j = count; j > 0; j--)
				data[j] = data[j - 1];
			data[0] = i;
			count++;
		}
		void addlast(int i)
		{
			if (count == capacity) grow();
			data[count++] = i;
		}
		void addpos(int i, int p)
		{
			if (p <= 0)
			{
				addfirst(i);
				return;
			}
			if (p >= count)
			{
				addlast(i);
				return;
			}
			if (count == capacity) grow();
			for (unsigned int j = count; j > p; j--)
				data[j] = data[j - 1];
			data[p] = i;
			count++;
		}
		bool remove(int p)
		{
			if (p < 0) return false;
			if (p >= count) return false;
			for (unsigned int j = p + 1; j < count; j++)
				data[j - 1] = data[j];
			count--;
			return true;
		}
		int find(int i)
		{
			for (unsigned int j = 0; j < count; j++)
				if (data[j] == i) return j;
			return -1;
		}
		void show()
		{
			cout << "LIST (" << count << "):";
			for (unsigned int i = 0; i < count; i++)
				cout << " " << data[i];
			cout << endl;
		}
		void reverseshow()
		{
			cout << "LIST (" << count << "):";
			for (unsigned int i = count - 1; i < count; i--)
				cout << " " << data[i];
			cout << endl;
		}
		unsigned int getcount()
		{
			return count;
		}
};

struct node2w
{
	int value;
	node2w* next;
	node2w* prev;
};

class list2w
{
	protected:
		node2w* first;
		node2w* last;
		int count;
		void copy(list2w& b)
		{
			count = b.count;
			if (b.first == NULL)
				first = NULL;
			else
			{
				first = new node2w();
				first->value = b.first->value;
				first->next = NULL;
				first->prev = NULL;
				node2w* tempa = first;
				node2w* tempb = b.first->next;
				while (tempb != NULL)
				{
					tempa->next = new node2w();
					tempa->next->value = tempb->value;
					tempa->next->next = NULL;
					tempa->next->prev = tempa;
					tempb = tempb->next;
					tempa = tempa->next;
				}
				last = tempa;
			}
		}
		void destroy()
		{
			node2w* n1 = first;
			node2w* n2;
			while (n1 != NULL)
			{
				n2 = n1->next;
				delete n1;
				n1 = n2;
			}
			first = NULL;
		}

	public:
		list2w()
		{
			first = NULL;
			count = 0;
		}
		list2w(list2w& b)
		{
			copy(b);
		}
		list2w& operator=(list2w& b)
		{
			if (this == &b) return *this;
			destroy();
			copy(b);
			return *this;
		}
		list2w(list2w&& b)
		{
			first = b.first;
			b.first = NULL;
		}
		list2w& operator=(list2w&& b)
		{
			if (this == &b) return *this;
			destroy();
			first = b.first;
			b.first = NULL;
			return *this;
		}
		~list2w()
		{
			destroy();
		}
		void addfirst(int i)
		{
			node2w* n = new node2w();
			n->value = i;
			n->next = first;
			n->prev = NULL;
			if (first != NULL) first->prev = n;
			first = n;
			if (last == NULL) last = first;
			count++;
		}
		void addlast(int i)
		{
			node2w* n = new node2w();
			n->value = i;
			n->prev = last;
			n->next = NULL;
			if (last != NULL) last->next = n;
			last = n;
			if (first == NULL) first = last;
			count++;
		}
		void addpos(int i, int p)
		{
			if (p <= 0)
			{
				addfirst(i);
				return;
			}
			if (p >= count)
			{
				addlast(i);
				return;
			}
			p--;
			node2w* n1 = first;
			while (p--) n1 = n1->next;
			node2w* n2 = n1->next;
			n1->next = new node2w();
			n1->next->value = i;
			n1->next->next = n2;
			n1->next->prev = n1;
			if (n2 != NULL) n2->prev = n1->next;
			count++;
		}
		bool remove(int p)
		{
			if (p < 0) return false;
			if (p >= count) return false;
			node2w* n1;
			node2w* n2;
			if (p == 0)
			{
				n1 = first->next;
				delete first;
				first = n1;
				if (first != NULL) first->prev = NULL;
				else last = NULL;
				count--;
				return true;
			}
			if (p == count - 1)
			{
				n1 = last->prev;
				delete last;
				last = n1;
				if (last != NULL) last->next = NULL;
				else first = NULL;
				count--;
				return true;
			}
			p--;
			n1 = first;
			while(p--) n1 = n1->next;
			n2 = n1->next->next;
			delete n1->next;
			n1->next = n2;
			if (n2 != NULL) n2->prev = n1;
			else last = n1;
			count--;
			return true;
		}
		int find(int i)
		{
			node2w* n = first;
			int p = 0;
			while(n != NULL)
			{
				if (n->value == i)
					return p;
				p++;
				n = n->next;
			}
			return -1;
		}
		void show()
		{
			node2w* n = first;
			cout << "LIST (" << count << "):";
			while (n != NULL)
			{
				cout << " " << n->value;
				n = n->next;
			}
			cout << endl;
		}
		void reverseshow()
		{
			node2w* n = last;
			cout << "LIST (" << count << "):";
			while (n != NULL)
			{
				cout << " " << n->value;
				n = n->prev;
			}
			cout << endl;
		}
		unsigned int getcount()
		{
			return count;
		}
};

class heap
{
	private:
		int* data = new int[7];
		unsigned int capacity = 7;
		unsigned int growby = 8;
		unsigned int count = 0;

		unsigned int parent(unsigned int i) { return (i - 1) / 2; }
		unsigned int left(unsigned int i) { return i * 2 + 1; }
		unsigned int right(unsigned int i) { return i * 2 + 2; }

		void copy(heap& b)
		{
			data = new int[b.capacity];
			capacity = b.capacity;
			growby = b.growby;
			count = b.count;
			for (unsigned int i = 0; i < count; i++)
				data[i] = b.data[i];
		}
		void destroy()
		{
			delete[] data;
		}

		void shiftup(unsigned int i)
		{
			if (i == 0) return;
			unsigned int p = parent(i);
			if (data[p] < data[i])
			{
				swap(data[p], data[i]);
				shiftup(p);
			}
		}
		void shiftdown(unsigned int i)
		{
			unsigned int l = left(i);
			unsigned int r = right(i);
			if (l < count)
			{
				if (r < count && data[i] < data[r] && data[l] < data[r])
				{
					swap(data[i], data[r]);
					shiftdown(r);
				}
				else if (data[i] < data[l])
				{
					swap(data[i], data[l]);
					shiftdown(l);
				}
			}
		}

		void write(unsigned int i, unsigned int prefix)
		{
			if(i >= count) return;
			for (unsigned j = 0; j < prefix; j++)
				cout << " ";
			cout << data[i] << endl;
			write(left(i), prefix + 2);
			write(right(i), prefix + 2);
		}

	public:
		heap() {}
		heap(heap& b)
		{
			destroy();
			copy(b);
		}
		heap(heap&& b)
		{
			destroy();
			data = b.data;
			capacity = b.capacity;
			growby = b.growby;
			count = b.count;
			b.data = new int[7];
			b.capacity = 7;
			b.growby = 8;
			b.count = 0;
		}
		heap& operator= (heap& b)
		{
			if (this == &b) return *this;
			destroy();
			copy(b);
			return *this;
		}
		heap& operator= (heap&& b)
		{
			if (this == &b) return *this;
			destroy();
			data = b.data;
			capacity = b.capacity;
			growby = b.growby;
			count = b.count;
			b.data = new int[7];
			b.capacity = 7;
			b.growby = 8;
			b.count = 0;
			return *this;
		}
		~heap()
		{
			destroy();
		}
		bool empty()
		{
			return count == 0;
		}
		void add(int x)
		{
			if (count == capacity)
			{
				capacity += growby;
				growby *= 2;
				int* temp = new int[capacity];
				for (unsigned int i = 0; i < count; i++)
					temp[i] = data[i];
				delete[] data;
				data = temp;
			}
			data[count] = x;
			shiftup(count);
			count++;
		}
		int remove()
		{
			if (count == 0) return 0;
			int x = data[0];
			count--;
			if (count > 0)
			{
				swap(data[0], data[count]);
				shiftdown(0);
			}
			return x;
		}
		void write()
		{
			write(0, 0);
		}
};

struct bstnode
{
	int value;
	bstnode* left;
	bstnode* right;
	bstnode(int value)
	{
		this->value = value;
		left = 0;
		right = 0;
	}
	bstnode(bstnode& b)
	{
		this->value = b.value;
		if (b.left == 0)
			left = 0;
		else
			left = new bstnode(*b.left);
		if (b.right == 0)
			right = 0;
		else
			right = new bstnode(*b.right);
	}
	bstnode(bstnode&& b) = delete;
	bstnode& operator=(bstnode& b) = delete;
	bstnode& operator=(bstnode&& b) = delete;
	~bstnode()
	{
		delete left;
		delete right;
	}
};

class bst
{
	private:
		bstnode* root = 0;
		unsigned int count = 0;

		void copy(bst& b)
		{
			root = new bstnode(*b.root);
			count = b.count;
		}
		void destroy()
		{
			delete root;
		}

		bool add(bstnode* node, int x)
		{
			if (node->value == x)
				return false;
			else if (node->value > x)
			{
				if (node->left == 0)
				{
					node->left = new bstnode(x);
					count++;
					return true;
				}
				else
					return add(node->left, x);
			}
			else
			{
				if (node->right == 0)
				{
					node->right = new bstnode(x);
					count++;
					return true;
				}
				else
					return add(node->right, x);
			}
		}
		bstnode* remove(bstnode* node, int x, bool& removed)
		{
			if (node == 0)
			{
				return 0;
			}
			else if (node->value != x)
			{
				if (node->value > x)
					node->left = remove(node->left, x, removed);
				else
					node->right = remove(node->right, x, removed);
				return node;
			}
			else if (node->left == 0 && node->right == 0)
			{
				delete node;
				removed = true;
				return 0;
			}
			else if (node->left == 0)
			{
				bstnode* temp = node->right;
				node->right = 0;
				delete node;
				removed = true;
				return temp;
			}
			else if (node->right == 0)
			{
				bstnode* temp = node->left;
				node->left = 0;
				delete node;
				removed = true;
				return temp;
			}
			else
			{
				bstnode* presuccessor = node;
				bstnode* successor = node->right;
				while (successor->left != 0)
				{
					presuccessor = successor;
					successor = successor->left;
				}
				node->value = successor->value;
				if (node == presuccessor)
					presuccessor->right = remove(presuccessor->right, node->value, removed);
				else
					presuccessor->left = remove(presuccessor->left, node->value, removed);
				removed = true;
				return node;
			}
		}

		void write(bstnode* node, unsigned int prefix)
		{
			if (node == 0) return;
			for (unsigned int i = 0; i < prefix; i++)
				cout << " ";
			cout << node->value << endl;
			write(node->left, prefix + 2);
			write(node->right, prefix + 2);
		}

	public:
		bst() {}
		bst(bst& b)
		{
			copy(b);
		}
		bst(bst&& b)
		{
			root = b.root;
			count = b.count;
			b.root = 0;
			b.count = 0;
		}
		bst& operator=(bst& b)
		{
			if (this == &b) return *this;
			destroy();
			copy(b);
			return *this;
		}
		bst& operator=(bst&& b)
		{
			if (this == &b) return *this;
			destroy();
			root = b.root;
			count = b.count;
			b.root = 0;
			b.count = 0;
			return *this;
		}
		~bst()
		{
			delete root;
		}
		bool add(int x)
		{
			if (root == 0)
			{
				root = new bstnode(x);
				count++;
				return true;
			}
			else
			{
				bool outcome = add(root, x);
				return outcome;
			}
		}
		bool remove(int x)
		{
			bool removed = false;
			root = remove(root, x, removed);
			if (removed) count--;
			return removed;
		}
		bool find(int x)
		{
			bstnode* node = root;
			while (node != 0)
			{
				if (node->value == x)
					return true;
				else if (node->value > x)
					node = node->left;
				else
					node = node->right;
			}
			return false;
		}
		void write()
		{
			write(root, 0);
		}
		unsigned int getcount()
		{
			return count;
		}
};

int main()
{
	if (false)
	{
		chrono::time_point<chrono::steady_clock> start;
		chrono::time_point<chrono::steady_clock> end;
		for (unsigned int n = 1000; n <= 100000; n *= 10)
		{
			//listarr l;
			//list2w l;
			//vector<int> l;
			list<int> l;
			vector<int> v;
			for (int i = 0; i < n; i++)
				v.push_back(i);
			for (int i : v)
				//l.addlast(i);
				l.push_back(i);
			start = chrono::steady_clock::now();
			//for (int i : v)
			for (unsigned int j = 0; j < n; j++)
			{
				auto pos = l.begin();
				advance(pos, l.size() / 2);
				//Dodawanie na pocz¹tek:
				//l.addfirst(i);
				//l.insert(l.begin(), i);
				//l.push_front(i);
				//Dodawanie na koniec:
				//l.addlast(i);
				//l.push_back(i);
				//Dodawanie na œrodek:
				//l.addpos(i, l.getcount() / 2);
				//l.insert(pos, i);
				//Usuwanie z pocz¹tku:
				//l.remove(0);
				//l.erase(l.begin());
				//l.pop_front();
				//Usuwanie z koñca:
				//l.remove(l.getcount() - 1);
				//l.pop_back();
				//Usuwanie z œrodka:
				//l.remove(l.getcount() / 2);
				l.erase(pos);
			}
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
		}
	}
	if (false)
	{
		default_random_engine g;
		chrono::time_point<chrono::steady_clock> start;
		chrono::time_point<chrono::steady_clock> end;

		for (unsigned int n = 1000; n <= 100000; n *= 10)
		{
			srand(253257);
			vector<int> v;
			std::uniform_int_distribution<int> d(0, n * 1000 - 1);
			for (unsigned int i = 0; i < n; i++)
				v.push_back(d(g));
			heap h;
			start = chrono::steady_clock::now();
			for (int i : v)
				h.add(i);
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
			start = chrono::steady_clock::now();
			while (not h.empty())
				h.remove();
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
			int t[n];
			for (unsigned int i = 0; i < n; i++)
				t[i] = v[i];
			start = chrono::steady_clock::now();
			make_heap(t, t + n);
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
		}

		cout << endl;
		for (unsigned int n = 1000; n <= 100000; n *= 10)
		{
			srand(253257);
			vector<int> v;
			std::uniform_int_distribution<int> d(0, n * 1000 - 1);
			for (unsigned int i = 0; i < n; i++)
				v.push_back(d(g));
			bst b;
			set<int> s;
			start = chrono::steady_clock::now();
			for (int i : v)
				b.add(i);
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
			start = chrono::steady_clock::now();
			for (int i : v)
				s.insert(i);
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
			start = chrono::steady_clock::now();
			for (unsigned int i = n - 1; i < n; i--)
				b.remove(v[(i + n / 2) % n]);
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
			start = chrono::steady_clock::now();
			for (unsigned int i = n - 1; i < n; i--)
				s.erase(v[(i + n / 2) % n]);
			end = chrono::steady_clock::now();
			cout << n << " elementow: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
		}
	}

	cout << "Testowanie array list:" << endl;
	{
		listarr l;
		l.addfirst(0);
		l.addfirst(2);
		l.addfirst(1);
		l.addlast(5);
		l.addpos(6, 2);
		l.addpos(7, -7);
		l.addpos(8, 42);
		l.addpos(15, 4);
		l.show();
		l.reverseshow();
		l.remove(6);
		l.remove(0);
		l.remove(2);
		l.show();
		l.reverseshow();
	}
	cout << "Testowanie linked list:" << endl;
	{
		list2w l;
		l.addfirst(0);
		l.addfirst(2);
		l.addfirst(1);
		l.addlast(5);
		l.addpos(6, 2);
		l.addpos(7, -7);
		l.addpos(8, 42);
		l.show();
		l.reverseshow();
		l.remove(6);
		l.remove(0);
		l.remove(2);
		l.show();
		l.reverseshow();
	}
	cout << "Testowanie heap:" << endl;
	{
		srand(654651);
		heap h;
		for (int i = 0; i < 16; i++)
			h.add(rand()%100);
		h.write();
		cout << endl;
		h.remove();
		h.write();
		cout<<endl;
		while (!h.empty())
			cout << h.remove() << " ";
		cout << endl;
	}
	cout << "Testowanie BST:" << endl;
	{
		bst t;
		for (unsigned int i = 0; i < 16; i++)
			t.add(rand() % 16);
		t.write();
		t.add(10);
	}

	return 0;
}
