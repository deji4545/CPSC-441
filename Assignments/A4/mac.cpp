// Name - Ayodeji Osho
// Class - CPSC 441 T07
// Student ID -30071771

// Reference - Read File Code
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <typeinfo>
#include <string>

#include <iomanip>

using namespace std;
int K;
int M;
int probe = 0;
int success = 0;
int collision = 0;
int items_basket = 0;
vector<int> basket;
string txtFileName;
int customer_num = 0;
int probe_leaf = 0;

// REFERENCE - https://www.geeksforgeeks.org/binary-tree-set-1-introduction/
// Node class
struct Node
{
    int level;
    vector<int> items;
    struct Node *left;
    struct Node *right;
    struct Node *parent;

    // constructor
    Node(int val, vector<int> vect, Node *root)
    {
        level = val;
        items = vect;
        parent = root;
        left = NULL;
        right = NULL;
    }
};

// REFERENCE - https://www.geeksforgeeks.org/program-decimal-binary-conversion/
// Convert decimal to binary
string decToBinary(int n)
{
    string binary_string = "";
    for (int i = K - 1; i >= 0; i--)
    {
        int k = n >> i;
        if (k & 1)
            binary_string = binary_string + "1";
        else
            binary_string = binary_string + "0";
    }
    return binary_string;
}

// Returns a subset of numbers from the root and assigns it to the rest of the nodes
int subset(int level, vector<int> items, char bits, vector<int> &subset_items)
{
    for (auto j : items)
    {
        string bitString = decToBinary(j);
        if (bitString.at(level) == bits)
        {
            subset_items.push_back(j);
        }
    }
    return 0;
}

vector<int> subitems;

// Recursion to create a binary tree
int createBinary(Node *root)
{
    if (root->level == K)
    {
        return 0;
    }
    vector<int> left_subset_items;
    vector<int> right_subset_items;
    subset(root->level, root->items, '0', left_subset_items);
    subset(root->level, root->items, '1', right_subset_items);

    root->left = new Node((root->level) + 1, left_subset_items, root);
    root->right = new Node((root->level) + 1, right_subset_items, root);

    createBinary(root->left);
    createBinary(root->right);

    return 0;
}

// Recursion to search for collision and success
int searchRoot(Node *root)
{

    int count = 0;
    probe++;
    for (auto i : basket)
    {
        for (auto j : root->items)
        {
            if (i == j)
            {
                count++;
            }
        }
    }

    if (count == 0 && probe_leaf == 0)
    {
        return 0;
    }

    if (count == 1 && probe_leaf == 0)
    {
        success++;
        return 0;
    }
    if (count > 0 && probe_leaf == 0)
    {
        collision++;
    }

    if (root->level == K)
    {
        if (count == 1 && probe_leaf == 1)
        {
            success++;
        }
        return 0;
    }

    searchRoot(root->left);
    searchRoot(root->right);
    return 0;
}

// Read the text file of numbers
int pushBasket()
{
    // REFERENCE - https://www.w3schools.com/cpp/cpp_files.asp
    cout << "\n";
    string myText;
    ifstream MyReadFile(txtFileName);
    while (getline(MyReadFile, myText))
    {
        int num = stoi(myText);
        items_basket++;
        basket.push_back(num);
    }

    // Close the file
    MyReadFile.close();
    return 0;
}

// Calculate results of the program
int getResult(Node *root)
{

    searchRoot(root);

    int idle;
    int total;
    idle = probe - (success + collision);
    if (probe_leaf == 1)
    {
        total = M;
        idle = M - success;
    }
    total = collision + success + idle;
    float efficiency = ((float)success / (float)total) * 100;
    int gap = 10;

    // REFERENCE - https://www.codevscolor.com/c-plus-plus-print-data-table
    cout
        << left
        << setw(gap)
        << customer_num
        << left
        << setw(gap)
        << items_basket
        << left
        << setw(gap)
        << idle
        << left
        << setw(gap)
        << success
        << left
        << setw(gap)
        << collision
        << left
        << setw(gap)
        << total
        << left
        << setw(gap)
        << efficiency;

    return 0;
}

// Display results of program
int displayResult(Node *root)
{
    int gap = 10;

    cout
        << left
        << setw(gap)
        << "Customer"
        << left
        << setw(gap)
        << "Items"
        << left
        << setw(gap)
        << "Idle"
        << left
        << setw(gap)
        << "Success"
        << left
        << setw(gap)
        << "Collision"
        << left
        << setw(gap)
        << "Total"
        << left
        << setw(gap)
        << "Efficiency(%)";

    // Recursively searches tree and retrieves results for the customer text file
    for (int i; i < K; i++)
    {
        probe = 0;
        success = 0;
        collision = 0;
        items_basket = 0;
        basket.clear();
        customer_num = i + 1;
        txtFileName = "customer" + to_string(i + 1) + ".txt";
        pushBasket();
        getResult(root);
    }

    cout << "\n";
    return 0;
}

// Ask for the value for K to determine M then call the recusrsion algorithm
int main()
{
    cout << "Enter a value for K: ";
    cin >> K;
    M = pow(2, K);

    // print the value for K and M
    cout << "K =" << K << "\n"
         << "M =" << M << endl;

    // Pushes M items into a vector to be used by the binary tree
    vector<int> items;
    for (int i = 0; i < M; i++)
    {
        items.push_back(i);
    }

    struct Node *root = new Node(0, items, NULL);

    createBinary(root);
    cout << "Probing At Root Level:" << endl;
    displayResult(root);
    cout << "\nProbing At Leaf Level:" << endl;
    probe_leaf = 1;
    displayResult(root);

    return 0;
}
