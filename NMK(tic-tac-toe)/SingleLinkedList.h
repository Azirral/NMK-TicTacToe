#include "Node.h"
class SingleLinkedList {
public:
    int length;
    Node* head;
    Node* tail;
    SingleLinkedList();
    void insertNodeAfter(Node* node);
    void deleteNode(Node* node);
    ~SingleLinkedList();
};

