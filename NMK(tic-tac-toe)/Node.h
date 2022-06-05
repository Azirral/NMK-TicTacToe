#include <iostream>

using namespace std;

class Node {
public:
    int x, y;
    Node *next;

    explicit Node(int x, int y);
};