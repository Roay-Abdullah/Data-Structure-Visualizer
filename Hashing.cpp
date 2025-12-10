#include <iostream>
#include <string>
#include <sstream>
using namespace std;
struct Node
{
    int key;
    string name;
    Node* next;

    Node() 
    {
        key = 0;
        name = "";
        next = nullptr;
    }
};

class SeparateChainingHash
{
private:
    Node** scTable;
    int tableSize;

    int hashFunction(int key) const
    {
        if (tableSize <= 0)
        {
            return 0;
        }
        return key % tableSize;
    }

    void resetTable() 
    {
        for (int i = 0; i < tableSize; ++i) 
        {
            Node* current = scTable[i];
            while (current != nullptr)
            {
                Node* next = current->next;
                delete current;
                current = next;
            }
            scTable[i] = nullptr;
        }
    }


public:
    SeparateChainingHash(int size = 11)
    {
        tableSize = size;
        scTable = new Node * [tableSize];
        for (int i = 0; i < tableSize; ++i)
        {
            scTable[i] = nullptr;
        }
    }

    ~SeparateChainingHash() 
    {
        resetTable();
        delete[] scTable;
    }


    bool insert(int key, const string& value)
    {
        if (key == 0)
        {
            return false;
        }

        int index = hashFunction(key);
        Node* current = scTable[index];

        while (current != nullptr) 
        {
            if (current->key == key)
            {
                current->name = value;
                return true;
            }
            current = current->next;
        }

        Node* new_node = new Node();
        new_node->key = key;
        new_node->name = value;
        new_node->next = scTable[index];
        scTable[index] = new_node;

        return true;
    }

    string search(int key) 
    {
        int index = hashFunction(key);
        Node* current = scTable[index];

        while (current != nullptr) 
        {
            if (current->key == key)
            {
                return current->name;
            }
            current = current->next;
        }
        return "Key Not Found";
    }

    string displayAsString()
    {
        ostringstream out;
        out << "Separate Chaining Hash Table (Size: " << tableSize << ")" << endl;

        for (int i = 0; i < tableSize; ++i)
        {
            out << "[" << i << "]: ";
            Node* current = scTable[i];
            if (!current)
            {
                out << "-> NULL";
            }
            else
            {
                while (current != nullptr)
                {
                    out << "-> (" << current->key << ", " << current->name << ")";
                    current = current->next;
                }
            }
            out << endl;
        }

        return out.str();
    }
};

extern "C"
{

    SeparateChainingHash* createSC(int size)
    {
        return new SeparateChainingHash(size);
    }

    bool scInsert(SeparateChainingHash* obj, int key, const char* value)
    {
        if (!obj)
        {
            return false;
        }
        return obj->insert(key, string(value));
    }

    const char* scSearch(SeparateChainingHash* obj, int key)
    {
        static string result;
        if (!obj) 
        {
            result = "Invalid pointer";
            return result.c_str();
        }
        result = obj->search(key);
        return result.c_str();
    }

    const char* scDisplay(SeparateChainingHash* obj) 
    {
        static string result;
        if (!obj) 
        {
            result = "Invalid pointer";
            return result.c_str();
        }
        result = obj->displayAsString();
        return result.c_str();
    }

} 
