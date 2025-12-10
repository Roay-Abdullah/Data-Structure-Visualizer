#include <iostream>
#include <sstream>
#include <string>
using namespace std;
class TreeNode 
{
public:
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int v = 0) 
    {
        value = v;
        left = nullptr;
        right = nullptr;
    }
};
class AVLTree
{
public:
    TreeNode* root;

    AVLTree() 
    {
        root = nullptr;
    }

    
    int height(TreeNode* r)
    {
        if (!r)
        {
            return -1;
        }
        
        int lh = height(r->left);
        int rh = height(r->right);
        return ((lh > rh ? lh : rh) + 1);
    }

    
    int getBalanceFactor(TreeNode* r) 
    {
        if (!r)
        {
            return -1;
        }
        return (height(r->left) - height(r->right));
    }

    
    TreeNode* rightRotate(TreeNode* r)
    {
        TreeNode* x = r->left;
        TreeNode* T2 = x->right;

        x->right = r;
        r->left = T2;
        return x;
    }

    
    TreeNode* leftRotate(TreeNode* r)
    {
        TreeNode* x = r->right;
        TreeNode* T2 = x->left;

        x->left = r;
        r->right = T2;
        return x;
    }

    
    TreeNode* recursiveInsertion(TreeNode* r, TreeNode* n)
    {
        if (!r)
        {
            return n;
        }

        if (n->value < r->value)
        {
            r->left = recursiveInsertion(r->left, n);
        }
        else if (n->value > r->value)
        {
            r->right = recursiveInsertion(r->right, n);
        }
        else
        {
            return r;
        }

        int bf = getBalanceFactor(r);

        if (bf > 1 && n->value < r->left->value)
        {
            return rightRotate(r);
        }
        if (bf < -1 && n->value > r->right->value)
        {
            return leftRotate(r);
        }
        if (bf > 1 && n->value > r->left->value) 
        {
            r->left = leftRotate(r->left);
            return rightRotate(r);
        }
        if (bf < -1 && n->value < r->right->value) 
        {
            r->right = rightRotate(r->right);
            return leftRotate(r);
        }

        return r;
    }

    
    void insertValue(int val) 
    {
        TreeNode* newNode = new TreeNode(val);
        root = recursiveInsertion(root, newNode);
    }

    
    TreeNode* minValueNodeInRST(TreeNode* n)
    {
        while (n && n->left)
        {
            n = n->left;
        }
        return n;
    }

    
    TreeNode* deleteNode(TreeNode* r, int val)
    {
        if (!r)
        {
            return r;
        }
        if (val < r->value)
        {
            r->left = deleteNode(r->left, val);
        }
        else if (val > r->value)
        {
            r->right = deleteNode(r->right, val);
        }
        else
        {
            if (!r->left) 
            {
                TreeNode* temp = r->right;
                delete r;
                return temp;
            }
            else if (!r->right)
            {
                TreeNode* temp = r->left;
                delete r;
                return temp;
            }
            else 
            {
                TreeNode* temp = minValueNodeInRST(r->right);
                r->value = temp->value;
                r->right = deleteNode(r->right, temp->value);
            }
        }

        int bf = getBalanceFactor(r);

        if (bf > 1 && getBalanceFactor(r->left) >= 0)
        {
            return rightRotate(r);
        }
        if (bf > 1 && getBalanceFactor(r->left) < 0) 
        {
            r->left = leftRotate(r->left);
            return rightRotate(r);
        }
        if (bf < -1 && getBalanceFactor(r->right) <= 0)
        {
            return leftRotate(r);
        }
        if (bf < -1 && getBalanceFactor(r->right) > 0) 
        {
            r->right = rightRotate(r->right); 
            return leftRotate(r);
        }

        return r;
    }

    
    void deleteValue(int v)
    {
        root = deleteNode(root, v);
    }

    
    void inorder(TreeNode* r, stringstream& out) 
    {
        if (!r)
        {
            return;
        }
        inorder(r->left, out);
        out << r->value << " ";
        inorder(r->right, out);
    }

    string display()
    {
        stringstream out;
        inorder(root, out);
        return out.str();
    }
};


extern "C"
{

    AVLTree* createAVL()
    {
        return new AVLTree();
    }

    void avlInsert(AVLTree* t, int val) 
    {
        if (t) t->insertValue(val);
    }

    void avlDelete(AVLTree* t, int val)
    {
        if (t)
        {
            t->deleteValue(val);
        }
    }

    const char* avlDisplay(AVLTree* t) 
    {
        static string result;
        if (!t)
        {
            result = "Invalid AVL tree";
            return result.c_str();
        }
        result = t->display();
        return result.c_str();
    }

} 
