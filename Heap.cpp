#include <iostream>
#include <string>
#include <sstream>
using namespace std;
void swapInt(int& x, int& y)
{
    int temp = x;
    x = y;
    y = temp;
}
class MinHeap 
{
public:
    int* arr;
    int capacity;
    int size;

    MinHeap(int c) 
    {
        arr = new int[c];
        capacity = c;
        size = 0;
    }

    ~MinHeap() 
    {
        delete[] arr;
    }

    int parent(int i)
    {
        return (i - 1) / 2;
    }

    int left(int i)
    {
        return 2 * i + 1;
    }

    int right(int i)
    {
        return 2 * i + 2;
    }

    void insertValue(int val) 
    {
        if (size == capacity)
        {
            return;
        }

        int i = size;
        arr[size++] = val;
        heapifyUp(i);
    }

    int extractMin()
    {
        if (size <= 0)
        {
            return INT_MAX;
        }
        if (size == 1)
        {
            return arr[--size];
        }

        int root = arr[0];
        arr[0] = arr[size - 1];
        size--;
        heapifyDown(0);
        return root;
    }

    void heapifyUp(int i) 
    {
        while (i != 0 && arr[parent(i)] > arr[i]) 
        {
            swapInt(arr[i], arr[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) 
    {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        if (l < size && arr[l] < arr[smallest])
        {
            smallest = l;
        }
        if (r < size && arr[r] < arr[smallest])
        {
            smallest = r;
        }
        if (smallest != i) 
        {
            swapInt(arr[i], arr[smallest]);
            heapifyDown(smallest);
        }
    }

    string display() 
    {
        stringstream out;
        for (int i = 0; i < size; i++)
        {
            out << arr[i] << " ";
        }
        return out.str();
    }
};

extern "C" 
{
 
    MinHeap* createHeap(int capacity)
    {
        return new MinHeap(capacity);
    }

   
    void heapInsert(MinHeap* h, int val)
    {
        if (h)
        {
            h->insertValue(val);
        }
    }

    
    int heapExtractMin(MinHeap* h)
    {
        if (!h)
        {
            return INT_MAX;
        }
        return h->extractMin();
    }

    
    const char* heapDisplay(MinHeap* h) {
        static string result;
        if (!h)
        {
            result = "Invalid heap";
            return result.c_str();
        }
        result = h->display();
        return result.c_str();
    }

} 
