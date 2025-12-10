#include <iostream>
#include <string>
using namespace std;
const int MAX_CAPACITY = 100;
struct EdgeNode 
{
    int destinationVertexId;
    int weight;
    EdgeNode* next;

    EdgeNode(int d = 0, int w = 0) 
    {
        destinationVertexId = d;
        weight = w;
        next = nullptr;
    }
};
struct Vertex 
{
    int id;
    string name;
    EdgeNode* head;
    Vertex* next;

    Vertex(int Id = 0, const string& n = "") 
    {
        id = Id;
        name = n;
        head = nullptr;
        next = nullptr;
    }
};

struct QNode
{
    int key;
    QNode* next;

    QNode(int k = 0)
    {
        key = k;
        next = nullptr;
    }
};
class Queue 
{
private:
    QNode* front;
    QNode* rear;

public:
    Queue()
    {
        front = nullptr;
        rear = nullptr;
    }

    bool isEmpty() 
    {
        return (front == nullptr);
    }

    void enqueue(int vid)
    {
        QNode* n = new QNode(vid);
        if (isEmpty())
        {
            front = rear = n;
        }
        else 
        { 
            rear->next = n;
            rear = n;
        }
    }

    int dequeue()
    {
        if (isEmpty())
        {
            return -1;
        }

        QNode* temp = front;
        int vid = temp->key;
        
        if (front == rear)
        {
            front = rear = nullptr;
        }
        else
        {
            front = front->next;
        }
        delete temp;
        return vid;
    }

    ~Queue() 
    {
        while (front)
        {
            QNode* n = front;
            front = front->next;
            delete n;
        }
        rear = nullptr;
    }
};

struct SNode
{
    int key;
    SNode* next;

    SNode(int k = 0)
    {
        key = k;
        next = nullptr;
    }
};
class Stack 
{
public:
    SNode* top;
    Stack()
    {
        top = nullptr;
    }

    bool isEmpty()
    { 
        return (top == nullptr);
    }

    void push(int vid) 
    {
        SNode* n = new SNode(vid);
        n->next = top;
        top = n;
    }

    int pop() 
    {
        if (isEmpty())
        {
            return -1;
        }

        SNode* temp = top;
        int vid = temp->key;
        top = top->next;
        delete temp;
        return vid;
    }

    ~Stack()
    {
        while (top) 
        {
            SNode* n = top;
            top = top->next;
            delete n;
        }
    }
};

struct HeapNode
{
    int vIndex;
    int key;

    HeapNode(int v = -1, int k = INT_MAX)
    {
        vIndex = v;
        key = k;
    }
};
class MinHeap 
{
private:
    HeapNode* harr;
    int capacity;
    int heapSize;
    int* pos;
    int posCapacity;

    void ensureCapacity(int needed) 
    {
        if (needed <= capacity)
        {
            return;
        }

        int newcap = capacity * 2;

        while (newcap < needed)
        {
            newcap *= 2;
        }

        HeapNode* newarr = new HeapNode[newcap];

        for (int i = 0; i < heapSize; ++i)
        {
            newarr[i] = harr[i];
        }
        delete[] harr;
        harr = newarr;
        capacity = newcap;
    }

public:
    MinHeap(int cap = MAX_CAPACITY, int maxVertices = MAX_CAPACITY)
    {
        if (cap < 1)
        {
            cap = 1;
        }
        capacity = cap;
        harr = new HeapNode[capacity];
        heapSize = 0;
        pos = nullptr;
        posCapacity = 0;
        initialPos(maxVertices);
    }

    void initialPos(int newMax)
    {
        if (pos)
        {
            delete[] pos;
        }

        pos = new int[newMax];
        for (int i = 0; i < newMax; ++i)
        {
            pos[i] = -1;
        }

        posCapacity = newMax;
        for (int i = 0; i < heapSize; ++i)
        {
            if (harr[i].vIndex >= 0 && harr[i].vIndex < posCapacity)
            {
                pos[harr[i].vIndex] = i;
            }
        }
    }

    ~MinHeap()
    {
        delete[] harr;
        delete[] pos;
    }

    bool empty() const
    {
        return heapSize == 0;
    }

    int parent(int i) const 
    {
        return (i - 1) / 2;
    }

    int left(int i) const 
    {
        return 2 * i + 1;
    }

    int right(int i) const
    {
        return 2 * i + 2; 
    }

    void swapNode(int i, int j) 
    {
        HeapNode tmp = harr[i];
        harr[i] = harr[j];
        harr[j] = tmp;
        if (harr[i].vIndex >= 0 && harr[i].vIndex < posCapacity)
        {
            pos[harr[i].vIndex] = i;
        }
        if (harr[j].vIndex >= 0 && harr[j].vIndex < posCapacity)
        {
            pos[harr[j].vIndex] = j;
        }
    }

    void minHeapify(int i) 
    {
        int l = left(i)
        int r = right(i);
        int smallest = i;

        if (l < heapSize && harr[l].key < harr[smallest].key)
        {
            smallest = l;
        }
        if (r < heapSize && harr[r].key < harr[smallest].key)
        {
            smallest = r;
        }
        if (smallest != i)
        { 
            swapNode(i, smallest);
            minHeapify(smallest);
        }
    }

    void insertKey(int vIndex, int key) 
    {
        ensureCapacity(heapSize + 1);
        int i = heapSize++;
        harr[i].vIndex = vIndex;
        harr[i].key = key;

        if (vIndex >= 0 && vIndex < posCapacity)
        {
            pos[vIndex] = i;
        }

        while (i != 0 && harr[parent(i)].key > harr[i].key)
        {
            swapNode(i, parent(i));
            i = parent(i);
        }
    }

    void decreaseKey(int vIndex, int newKey) 
    {
        if (vIndex < 0 || vIndex >= posCapacity)
        {
            return;
        }

        int i = pos[vIndex];

        if (i == -1)
        {
            return;
        }
        harr[i].key = newKey;

        while (i != 0 && harr[parent(i)].key > harr[i].key)
        {
            swapNode(i, parent(i));
            i = parent(i);
        }
    }

    HeapNode extractMin() 
    {
        if (heapSize <= 0)
        {
            return HeapNode(-1, INT_MAX);
        }

        HeapNode root = harr[0];

        if (heapSize == 1)
        {
            heapSize--;
        }
        else
        {
            harr[0] = harr[heapSize - 1];
            if (harr[0].vIndex >= 0 && harr[0].vIndex < posCapacity)
            {
                pos[harr[0].vIndex] = 0;
            }
            heapSize--;
            minHeapify(0);
        }
        if (root.vIndex >= 0 && root.vIndex < posCapacity)
        {
            pos[root.vIndex] = -1;
        }
        return root;
    }

    bool isInHeap(int vIndex) 
    {
        if (vIndex < 0 || vIndex >= posCapacity)
        {
            return false;
        }
        return pos[vIndex] != -1;
    }
};

class Graph 
{
private:
    Vertex* head;
    int vertexCount;

    Vertex* findVertexPtrById(int vid)
    {
        Vertex* cur = head;
        while (cur) 
        {
            if (cur->id == vid)
            {
                return cur;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    Vertex* findVertexPtrAndPrev(int vid, Vertex** prev)
    {
        Vertex* cur = head;
        *prev = nullptr;

        while (cur)
        {
            if (cur->id == vid)
            {
                return cur;
            }
            *prev = cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void freeAdjList(EdgeNode* h) 
    {
        EdgeNode* cur = h;

        while (cur)
        {
            EdgeNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

public:
    Graph() 
    {
        head = nullptr;
        vertexCount = 0;
    }
    
    ~Graph()
    {
        Vertex* cur = head;
    
        while (cur) 
        {
            Vertex* next = cur->next;
            freeAdjList(cur->head);
            delete cur;
            cur = next;
        }
        head = nullptr;
    }

    void addVertex(int vid, const string& name)
    {
        if (findVertexPtrById(vid))
        {
            cout << "Vertex exists." << endl;
            return;
        }
        Vertex* newNode = new Vertex(vid, name);
        newNode->next = head;
        head = newNode;
        vertexCount++;
    }

    void deleteVertex(int vid) 
    {
        Vertex* prevV;
        Vertex* vToDelete = findVertexPtrAndPrev(vid, &prevV);

        if (!vToDelete)
        {
            return;
        }
        Vertex* curV = head;
        
        while (curV)
        {
            if (curV->id != vid)
            {
                EdgeNode* curE = curV->head;
                EdgeNode* prevE = nullptr;
            
                while (curE)
                {
                    if (curE->destinationVertexId == vid) 
                    {
                        EdgeNode* toDelete = curE;
                        if (prevE) prevE->next = curE->next;
                        else curV->head = curE->next;
                        curE = curE->next;
                        delete toDelete;
                    }
                    else 
                    {
                        prevE = curE;
                        curE = curE->next;
                    }
                }
            }
            curV = curV->next;
        }

        freeAdjList(vToDelete->head);
        
        if (prevV)
        {
            prevV->next = vToDelete->next;
        }
        else
        {
            head = vToDelete->next;
        }
        delete vToDelete;
        vertexCount--;
    }

    void addEdge(int fromVid, int toVid, int w) 
    {
        Vertex* vF = findVertexPtrById(fromVid);
        Vertex* vT = findVertexPtrById(toVid);
        
        if (!vF || !vT)
        {
            return;
        }

        EdgeNode* cur = vF->head;
        
        while (cur) 
        {
            if (cur->destinationVertexId == toVid)
            {
                return;
            }
            cur = cur->next;
        }

        EdgeNode* n1 = new EdgeNode(toVid, w);
        n1->next = vF->head;
        vF->head = n1;
        EdgeNode* n2 = new EdgeNode(fromVid, w);
        n2->next = vT->head;
        vT->head = n2;
    }

    void deleteEdge(int fromVid, int toVid)
    {
        Vertex* vF = findVertexPtrById(fromVid);
        Vertex* vT = findVertexPtrById(toVid);
        if (!vF || !vT)
        {
            return;
        }
        EdgeNode* cur = vF->head;
        EdgeNode* prev = nullptr;
        
        while (cur)
        {
            if (cur->destinationVertexId == toVid) 
            {
                if (prev)
                {
                    prev->next = cur->next;
                }
                else
                {
                    vF->head = cur->next;
                }
                delete cur;
                break;
            }
            prev = cur;
            cur = cur->next;
        }

        cur = vT->head;
        prev = nullptr;

        while (cur) 
        {
            if (cur->destinationVertexId == fromVid) 
            {
                if (prev)
                {
                    prev->next = cur->next;
                }
                else
                {
                    vT->head = cur->next;
                }
                delete cur;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    void printGraph() 
    {
        Vertex* curV = head;
        while (curV)
        {
            cout << curV->name << " (" << curV->id << ") --> [";
            EdgeNode* curE = curV->head;
            while (curE) {
                cout << curE->destinationVertexId << "(" << curE->weight << ") --> ";
                curE = curE->next;
            }
            cout << "]" << endl;
            curV = curV->next;
        }
    }

    int getVertexIndex(int vid) 
    {
        Vertex* curV = head;
        int index = 0;

        while (curV) 
        {
            if (curV->id == vid)
            {
                return index;
            }
            curV = curV->next;
            index++;
        }
        return -1;
    }

    int getVertexIdFromIndex(int index) 
    {
        Vertex* curV = head;
        int i = 0;

        while (curV) 
        {
            if (i == index)
            {
                return curV->id;
            }
            curV = curV->next;
            i++;
        }
        return -1;
    }

    void BFS(int startVid)
    {
        if (vertexCount == 0)
        {
            return;
        }

        int startIdx = getVertexIndex(startVid);
        if (startIdx == -1)
        {
            return;
        }

        int* visited = new int[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            visited[i] = 0;
        }

        Queue q;
        q.enqueue(startVid);
        visited[startIdx] = 1;
        cout << "BFS: ";

        while (!q.isEmpty())
        {
            int curVid = q.dequeue();
            cout << curVid << " ";

            Vertex* v = findVertexPtrById(curVid);
            if (!v)
            {
                continue;
            }

            EdgeNode* cur = v->head;
            while (cur) 
            {
                int nbVid = cur->destinationVertexId;
                int nbIdx = getVertexIndex(nbVid);
                if (nbIdx != -1 && !visited[nbIdx])
                {
                    visited[nbIdx] = 1;
                    q.enqueue(nbVid);
                }
                cur = cur->next;
            }
        }
        cout << endl;
        delete[] visited;
    }

    void DFS(int startVid)
    {
        if (vertexCount == 0)
        {
            return;
        }

        int startIdx = getVertexIndex(startVid);
        if (startIdx == -1)
        {
            return;
        }

        int* visited = new int[vertexCount];
        for (int i = 0; i < vertexCount; ++i)
        {
            visited[i] = 0;
        }

        Stack st;
        st.push(startVid);
        visited[startIdx] = 1;
        cout << "DFS: ";

        while (!st.isEmpty()) 
        {
            int curVid = st.pop();
            cout << curVid << " ";
            Vertex* v = findVertexPtrById(curVid);
            if (!v)
            {
                continue;
            }

            EdgeNode* cur = v->head;
            while (cur) 
            {
                int nbVid = cur->destinationVertexId;
                int nbIdx = getVertexIndex(nbVid);
                if (nbIdx != -1 && !visited[nbIdx]) 
                {
                    visited[nbIdx] = 1;
                    st.push(nbVid);
                }
                cur = cur->next;
            }
        }
        cout << endl;
        delete[] visited;
    }

    void primMST(int startVid)
    {
        if (vertexCount == 0)
        {
            return;
        }

        int startIdx = getVertexIndex(startVid);
        if (startIdx == -1)
        {
            return;
        }
        int* key = new int[vertexCount];
        int* parent = new int[vertexCount];
        MinHeap mh(vertexCount * 2, vertexCount);
        mh.initialPos(vertexCount);

        for (int i = 0; i < vertexCount; ++i)
        {
            key[i] = INT_MAX;
            parent[i] = -1;
            mh.insertKey(i, INT_MAX);
        }

        key[startIdx] = 0;
        mh.decreaseKey(startIdx, 0);
        int totalWeight = 0;

        while (!mh.empty())
        {
            HeapNode hn = mh.extractMin();
            int uIdx = hn.vIndex;
            int uKey = hn.key;

            if (uKey == INT_MAX)
            {
                break;
            }
            if (parent[uIdx] != -1)
            {
                totalWeight += key[uIdx];
            }

            int uVid = getVertexIdFromIndex(uIdx);
            Vertex* uV = findVertexPtrById(uVid);
            if (!uV)
            {
                continue;
            }

            EdgeNode* cur = uV->head;
            
            while (cur) 
            {
                int nbVid = cur->destinationVertexId;
                int nbIdx = getVertexIndex(nbVid);

                if (nbIdx != -1 && mh.isInHeap(nbIdx) && cur->weight < key[nbIdx]) 
                {
                    key[nbIdx] = cur->weight;
                    parent[nbIdx] = uIdx;
                    mh.decreaseKey(nbIdx, key[nbIdx]);
                }
                cur = cur->next;
            }
        }

        cout << "Prim MST: " << totalWeight << endl;
        for (int i = 0; i < vertexCount; ++i) 
        {
            if (parent[i] != -1)
            {
                int uID = getVertexIdFromIndex(parent[i]);
                int vID = getVertexIdFromIndex(i);
                if (uID < vID) cout << uID << " -> " << vID << " : " << key[i] << endl;
            }
        }
        delete[] key;
        delete[] parent;
    }


    void dijkstra(int sourceVid) 
    {
        if (vertexCount == 0)
        {
            return;
        }

        int sIdx = getVertexIndex(sourceVid);
        if (sIdx == -1)
        {
            return;
        }
        int* dist = new int[vertexCount];
        int* parent = new int[vertexCount];
        MinHeap mh(vertexCount * 2, vertexCount);
        mh.initialPos(vertexCount);
        
        for (int i = 0; i < vertexCount; ++i)
        {
            dist[i] = INT_MAX;
            parent[i] = -1;
            mh.insertKey(i, INT_MAX);
        }

        dist[sIdx] = 0;
        mh.decreaseKey(sIdx, 0);
        
        while (!mh.empty())
        {
            HeapNode hn = mh.extractMin();
            int uIdx = hn.vIndex;
            int du = hn.key;
            if (du == INT_MAX)
            {
                break;
            }

            int uVid = getVertexIdFromIndex(uIdx);
            Vertex* uV = findVertexPtrById(uVid);
            
            if (!uV)
            {
                continue;
            }

            EdgeNode* cur = uV->head;
            while (cur)
            {
                int vVid = cur->destinationVertexId;
                int vIdx = getVertexIndex(vVid);

                if (vIdx != -1 && mh.isInHeap(vIdx))
                {
                    if (dist[uIdx] != INT_MAX && dist[uIdx] + cur->weight < dist[vIdx])
                    {
                        dist[vIdx] = dist[uIdx] + cur->weight;
                        parent[vIdx] = uIdx;
                        mh.decreaseKey(vIdx, dist[vIdx]);
                    }
                }
                cur = cur->next;
            }
        }
        cout << "Dijkstra from " << sourceVid << ":" << endl;

        for (int i = 0; i < vertexCount; ++i) 
        {
            int vid = getVertexIdFromIndex(i);
            cout << vid << " : ";
            if (dist[i] == INT_MAX) cout << "INF" << endl;
            else cout << dist[i] << endl;
        }
        delete[] dist;
        delete[] parent;
    }
};

extern "C" 
{

    Graph* createGraph() 
    {
        return new Graph();
    }

    void addVertex(Graph* g, int vid, const char* name) 
    {
        g->addVertex(vid, string(name));
    }

    void deleteVertex(Graph* g, int vid) 
    {
        g->deleteVertex(vid);
    }

    void addEdge(Graph* g, int fromVid, int toVid, int weight) 
    {
        g->addEdge(fromVid, toVid, weight);
    }

    void deleteEdge(Graph* g, int fromVid, int toVid) 
    {
        g->deleteEdge(fromVid, toVid);
    }

    void printGraph(Graph* g) 
    {
        g->printGraph();
    }

    void BFS(Graph* g, int startVid) 
    {
        g->BFS(startVid);
    }

    void DFS(Graph* g, int startVid)
    {
        g->DFS(startVid);
    }

    void primMST(Graph* g, int startVid) 
    {
        g->primMST(startVid);
    }

    void dijkstra(Graph* g, int startVid) 
    {
        g->dijkstra(startVid);
    }
}

