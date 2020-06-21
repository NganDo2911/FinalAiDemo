#include<iostream> 
#include <vector> 
#include <queue>
#include <list>
#include <map>
using namespace std;
typedef struct node {
	int src;			//vi tri nguoi bat dau
	int dest;			//vi tri ket thuc
	int wait1;			//thoi gian cho cua nguoi A
	int time1;			//thoi gian di cua nguoi A
	int wait2;			//thoi gian  cho cua nguoi B
	int time2;			//thoi gian di cua nguoi B
	int path_cost;		//chi phi hien tai
	struct node* parent;
}Node;

struct My_compare
{
	bool operator()(const Node* a, const Node* b) const
	{
		return a->path_cost > b->path_cost;
	}
};

class Graph {
	int V;
	vector<pair<int, int>>* adj; //luu danh sach ke
	int** time;				// ma tran luu thoi gian di toi cac dinh
public:
	Graph(int V);
	void addEdge(int start, int end, int distance);
	Node* bfs(int src, int dest);
	void printEdge();
	void print();
	int tinhKhoangCach(int u, int v);
	int heuristic(int src, int dest);
	Node* a_start(int start, int end);
	void Table();
};

Graph::Graph(int V) {
	this->V = V;
	adj = new vector<pair<int, int>>[V];
	time = new int* [V];
	for (int i = 0; i < V; i++)
	{
		time[i] = new int[V];
	}
}

void Graph::addEdge(int start, int end, int distance) {
	adj[start].push_back(make_pair(end, distance));
	adj[end].push_back(make_pair(start, distance));
}

void Graph::printEdge() {
	std::vector<pair<int, int>>::iterator i;
	for (int j = 0; j < V; j++) {
		cout << "Dinh " << j << endl;
		for (i = adj[j].begin(); i != adj[j].end(); ++i) {
			cout << i->first << " " << i->second << endl;
		}
	}
}

void Graph::print()
{
	for (int i = 0; i < V - 1; i++)
	{
		for (int j = i + 1; j < V; j++)
		{
			if (time[i][j] != 0)
				cout << i << " " << j << " " << time[i][j] << endl;
		}
	}
}

int Graph::tinhKhoangCach(int u, int d)
{
	vector<bool> visited(V, 0);
	vector<int> distance(V, 0);
	queue <int> q;
	distance[u] = 0;
	q.push(u);
	visited[u] = true;
	while (!q.empty())
	{
		int x = q.front();
		q.pop();
		for (auto i = adj[x].begin(); i != adj[x].end(); i++)
		{
			if (visited[i->first])
				continue;
			distance[i->first] = distance[x] + i->second;
			q.push(i->first);
			visited[i->first] = true;
		}
	}
	return distance[d];
}

void Graph::Table()
{
	for (int i = 0; i < V - 1; i++)
	{
		for (int j = i + 1; j < V; j++)
		{
			time[i][j] = tinhKhoangCach(i, j);
			time[j][i] = time[i][j];
		}
	}
}

bool checkExitProrityQueue(priority_queue< Node*, std::vector<Node*>, My_compare> frontier, int src, int dest) {
	priority_queue< Node*, std::vector<Node*>, My_compare> t(frontier);
	while (!t.empty()) {
		if (t.top()->src == src && t.top()->dest == dest)
			return true;
		t.pop();
	}
	return false;
}

void printPath(list<Node*> path) {
	while (!path.empty()) {
		cout << "src" << path.front()->src << endl;
		cout << "dest" << path.front()->dest << endl;
		path.pop_front();
	}
}

list<Node*> savePath(Node* node, int src) {
	list<Node*> pathA;
	while (node != NULL) {
		pathA.push_front(node);
		if (node->src == src) {
			return pathA;
		}
		node = node->parent;
	}
}

void printPathFromAToB(Node* node, std::string variable_names[], int src) {			//in ra duong di 

	int sum = 0;
	int wait = 0;
	list<Node*> path = savePath(node, src);
	std::list<Node*>::iterator i;
	cout << "Duong di cua nguoi A: ";
	for (i = path.begin(); i != path.end(); ++i) {
		cout << (*i)->src << "-";
		cout << variable_names[(*i)->src] << "  ";
		if ((*i)->wait1 > 0) {
			wait += (*i)->wait1;
		}
		sum = (*i)->time1;
	}
	cout << endl;
	cout << "Thoi gian di cua nguoi A la: " << sum << endl;
	cout << "Thoi gian cho cua nguoi A la: " << wait << endl;
	cout << "Tong thoi gian di va cho cua nguoi A la: " << sum + wait << endl;
	sum = 0;
	wait = 0;
	cout << "Duong di cua nguoi B: ";
	for (i = path.begin(); i != path.end(); ++i) {
		cout << (*i)->dest << "-";
		cout << variable_names[(*i)->dest] << "  ";
		if ((*i)->wait2 > 0) {
			wait += (*i)->wait2;
		}
		sum = (*i)->time2;
	}
	cout << endl;
	cout << "Thoi gian di cua nguoi B la: " << sum << endl;
	cout << "Thoi gian cho cua nguoi B la: " << wait << endl;
	cout << "Tong thoi gian di va cho cua nguoi B la: " << sum + wait << endl;
}

Node* Graph::bfs(int src, int dest) {
	priority_queue< Node*, std::vector<Node*>, My_compare>  frontier;
	priority_queue< Node*, std::vector<Node*>, My_compare>  explorer;
	Node* current = (Node*)malloc(sizeof(Node));
	current->src = src;
	current->dest = dest;
	current->time1 = 0;
	current->time2 = 0;
	if (current->src == current->dest) {
		return current;
	}
	frontier.push(current);
	while (current->src != current->dest && !frontier.empty()) {
		current = frontier.top();
		frontier.pop();
		std::vector<pair<int, int>>::iterator j;
		std::vector<pair<int, int>>::iterator i;
		Node* child;
		if (current->src == current->dest) {
			return current;
		}
		for (i = adj[current->src].begin(); i != adj[current->src].end(); ++i) {		//danh sach ke cua vi tri nguoi A
			for (j = adj[current->dest].begin(); j != adj[current->dest].end(); ++j) {	//danh sach ke cua vi tri nguoi B
				if (checkExitProrityQueue(frontier, i->first, j->first) == false
					|| checkExitProrityQueue(explorer, i->first, j->first) == false) {
					child = (Node*)malloc(sizeof(Node));
					child->parent = current;
					child->src = i->first;											// luu duong di cua nguoi A
					child->dest = j->first;											//duong di ua nguoi B
					child->time1 = child->parent->time1 + i->second;				// cong don thoi gian di cua nguoi A
					child->time2 = child->parent->time2 + j->second;				//cong don thoi gian di cua nguoi B
					int thoiGianCho = i->second - j->second;

					if (thoiGianCho > 0) {
						child->wait2 = thoiGianCho;
						child->wait1 = 0;
					}
					else {
						child->wait1 = abs(thoiGianCho);
						child->wait2 = 0;

					}
					if (child->src == child->dest) {
						return child;
					}
					child->path_cost = child->wait1 + child->time1 + child->wait2 + child->time2;   //tinh tong chi phi di đến thời điểm hiện tại

				}
				frontier.push(child);
			}
		}
		explorer.push(current);
	}
}

int Graph::heuristic(int src, int dest) {
	return time[src][dest];
}

Node* Graph::a_start(int src, int dest) {
	priority_queue< Node*, std::vector<Node*>, My_compare>  frontier;
	priority_queue< Node*, std::vector<Node*>, My_compare>  explorer;
	Node* current = (Node*)malloc(sizeof(Node));
	current->src = src;
	current->dest = dest;
	current->time1 = 0;
	current->time2 = 0;
	if (current->src == current->dest) {
		return current;
	}
	frontier.push(current);
	while (current->src != current->dest && !frontier.empty()) {
		current = frontier.top();
		frontier.pop();
		std::vector<pair<int, int>>::iterator j;
		std::vector<pair<int, int>>::iterator i;
		Node* child;
		if (current->src == current->dest) {
			return current;
		}
		for (i = adj[current->src].begin(); i != adj[current->src].end(); ++i) {		//danh sach ke cua vi tri nguoi A
			for (j = adj[current->dest].begin(); j != adj[current->dest].end(); ++j) {	//danh sach ke cua vi tri nguoi B
				if (checkExitProrityQueue(frontier, i->first, j->first) == false
					|| checkExitProrityQueue(explorer, i->first, j->first) == false) {
					child = (Node*)malloc(sizeof(Node));
					child->parent = current;
					child->src = i->first;											// luu duong di cua nguoi A
					child->dest = j->first;											//duong di ua nguoi B
					child->time1 = child->parent->time1 + i->second;				// cong don thoi gian di cua nguoi A
					child->time2 = child->parent->time2 + j->second;				//cong don thoi gian di cua nguoi B
					int thoiGianCho = i->second - j->second;

					if (thoiGianCho > 0) {
						child->wait2 = thoiGianCho;
						child->wait1 = 0;
					}
					else {
						child->wait1 = abs(thoiGianCho);
						child->wait2 = 0;
					}
					if (child->src == child->dest) {
						return child;
					}
					child->path_cost = child->wait1 + child->time1 + child->wait2 + child->time2 + heuristic(child->src, child->dest);   //tinh tong chi phi di đến thời điểm hiện tại

				}
				frontier.push(child);
			}
		}
		explorer.push(current);
	}
}

enum city {
	Oradea,
	Zerind,
	Arad,
	Sibiu,
	Timisoara,
	Lugoj,
	Mehadia,
	Drobeta,
	Craiova,
	RiminicuVilcea,
	Pitesti,
	Fagaras,
	Bucharest,
	Giurgiu,
	Urziceni,
	Vaslui,
	Iasi,
	Neamt,
	Hirsova,
	Eforie
};

void khoiTaoThanhPho() {
	std::cout << "0. Oradea" << std::endl;
	std::cout << "1. Zerind" << std::endl;
	std::cout << "2. Arad" << std::endl;
	std::cout << "3. Sibiu" << std::endl;
	std::cout << "4. Timisoara" << std::endl;
	std::cout << "5. Lugoj" << std::endl;
	std::cout << "6. Mehadia" << std::endl;
	std::cout << "7. Drobeta" << std::endl;
	std::cout << "8. Craivoa" << std::endl;
	std::cout << "9. RiminicuVilcea" << std::endl;
	std::cout << "10.Pitesti" << std::endl;
	std::cout << "11.Fagaras" << std::endl;
	std::cout << "12.Bucharest" << std::endl;
	std::cout << "13.Giurgiu" << std::endl;
	std::cout << "14.Urziceni" << std::endl;
	std::cout << "15.Vaslui" << std::endl;
	std::cout << "16.Iasi" << std::endl;
	std::cout << "17.Neamt" << std::endl;
	std::cout << "18.Hirsova" << std::endl;
	std::cout << "19.Eforie" << std::endl;
}

void chonThanhPho(int viTriNguoiA)
{
	switch (viTriNguoiA) {
	case city::Oradea:
		std::cout << "Ban chon Oradea" << std::endl;
		viTriNguoiA = city::Oradea;
		break;
	case city::Zerind:
		std::cout << "Ban chon Zerind" << std::endl;
		viTriNguoiA = city::Zerind;
		break;
	case city::Arad:
		std::cout << "Ban chon Arad" << std::endl;
		viTriNguoiA = city::Arad;
		break;
	case city::Sibiu:
		std::cout << "Ban chon Sibiu" << std::endl;
		viTriNguoiA = city::Sibiu;
		break;
	case city::Timisoara:
		std::cout << "Ban chon Timisoara" << std::endl;
		viTriNguoiA = city::Timisoara;
		break;
	case city::Lugoj:
		std::cout << "Ban chon Lugoj" << std::endl;
		viTriNguoiA = city::Lugoj;
		break;
	case city::Mehadia:
		std::cout << "Ban chon Mehadia" << std::endl;
		viTriNguoiA = city::Mehadia;
		break;
	case city::Drobeta:
		std::cout << "Ban chon Drobeta" << std::endl;
		viTriNguoiA = city::Drobeta;
		break;
	case city::Craiova:
		std::cout << "Ban chon Craiova" << std::endl;
		viTriNguoiA = city::Craiova;
		break;
	case city::RiminicuVilcea:
		std::cout << "Ban chon RiminicuVilcea" << std::endl;
		viTriNguoiA = city::RiminicuVilcea;
		break;
	case city::Pitesti:
		std::cout << "Ban chon Pitesti" << std::endl;
		viTriNguoiA = city::Pitesti;
		break;
	case city::Fagaras:
		std::cout << "Ban chon Fagaras" << std::endl;
		viTriNguoiA = city::Fagaras;
		break;
	case city::Bucharest:
		std::cout << "Ban chon Bucharest" << std::endl;
		viTriNguoiA = city::Bucharest;
		break;
	case city::Giurgiu:
		std::cout << "Ban chon Giurgiu" << std::endl;
		viTriNguoiA = city::Giurgiu;
		break;
	case city::Urziceni:
		std::cout << "Ban chon Urziceni" << std::endl;
		viTriNguoiA = city::Urziceni;
		break;
	case city::Vaslui:
		std::cout << "Ban chon Vaslui" << std::endl;
		viTriNguoiA = city::Neamt;
		break;
	case city::Iasi:
		std::cout << "Ban chon Iasi" << std::endl;
		viTriNguoiA = city::Iasi;
		break;
	case city::Neamt:
		std::cout << "Ban chon Neamt" << std::endl;
		viTriNguoiA = city::Vaslui;
		break;
	case city::Hirsova:
		std::cout << "Ban chon Hirsova" << std::endl;
		viTriNguoiA = city::Hirsova;
		break;
	case city::Eforie:
		std::cout << "Ban chon Eforie" << std::endl;
		viTriNguoiA = city::Eforie;
		break;
	default:
		std::cout << "Vui long chon thanh pho tren danh sach" << std::endl;
	};
}

int main()
{
	// Create a graph given in the above diagram 
	//int variables_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
	std::string variable_names[] = { "Oredea", "Zerind","Arad","Sibiu","Timisoara",
									"Lugoj","Mehadia","Drobeta","Cariova","Rimivicea",
									"Pitesti","Fafaras","Buchares","Giurgiu","Urzicen",
									"Vaslui","Iasi","Neamt","Hirsova","Eforie" };
	int N = 20;
	Graph g(N);
	g.addEdge(0, 1, 71);
	g.addEdge(0, 3, 151);
	g.addEdge(1, 2, 75);
	g.addEdge(2, 3, 140);
	g.addEdge(2, 4, 118);
	g.addEdge(3, 11, 99);
	g.addEdge(3, 9, 80);
	g.addEdge(4, 5, 111);
	g.addEdge(5, 6, 70);
	g.addEdge(6, 7, 75);
	g.addEdge(7, 8, 120);
	g.addEdge(8, 9, 146);
	g.addEdge(8, 10, 138);
	g.addEdge(9, 10, 97);
	g.addEdge(10, 12, 101);
	g.addEdge(11, 12, 211);
	g.addEdge(12, 13, 90);
	g.addEdge(12, 14, 85);
	g.addEdge(14, 18, 98);
	g.addEdge(14, 15, 142);
	g.addEdge(15, 16, 92);
	g.addEdge(16, 17, 87);
	g.addEdge(18, 19, 86);
	g.Table();
	//g.print();
	vector<bool> visited(N);
	int* pred = new int[N];
	vector<int> path(N);
	std::cout << "Chon thanh pho cho nguoi A" << std::endl;
	khoiTaoThanhPho();
	int viTriNguoiA;
	std::cout << "Vi tri nguoi A: ";
	std::cin >> viTriNguoiA;
	while (viTriNguoiA < 0 || viTriNguoiA >= 20) {
		std::cout << "Vi tri nguoi A: ";
		std::cin >> viTriNguoiA;
	}
	chonThanhPho(viTriNguoiA);
	std::cout << std::endl;
	std::cout << "Chon thanh pho cho nguoi B" << std::endl;
	khoiTaoThanhPho();
	int viTriNguoiB;
	std::cout << "Vi tri nguoi B: ";
	std::cin >> viTriNguoiB;
	while (viTriNguoiB < 0 || viTriNguoiB >= 20) {
		std::cout << "Vi tri nguoi B: ";
		std::cin >> viTriNguoiB;
	}
	chonThanhPho(viTriNguoiB);
	std::cout << std::endl;
	if (viTriNguoiA == viTriNguoiB) {
		cout << "Hai nguoi dang tren cung mot thanh pho" << endl;
	}
	else {
		cout << "thuat toan A*\n\n";
		Node* pathResultAStar = g.a_start(viTriNguoiA, viTriNguoiB);
		if (pathResultAStar != NULL) {
			printPathFromAToB(pathResultAStar, variable_names, viTriNguoiA);
		}
		else {
			cout << "Khong tim thay duong di" << endl;
		}
		cout << endl;
		cout << "------------***-----------------\n\n";
		cout << "thuat toan BFS";
		cout << endl;
		Node* pathResultBFS = g.bfs(viTriNguoiA, viTriNguoiB);
		if (pathResultBFS != NULL) {
			printPathFromAToB(pathResultBFS, variable_names, viTriNguoiA);
		}
		else {
			cout << "Khong tim thay duong di" << endl;
		}
	}
	return 0;
}