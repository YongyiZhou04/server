template <typename T>
class Node
{
    T val;
    Node<T> *next;
    Node(T val) : val(val), next(nullptr) {};
};

template <typename T>
class LinkedList
{
public:
    Node<T> *head;

    LinkedList() : head(nullptr) {};

    void insert(T val)
    {
        Node<T> *newNode = new Node(val);
        newNode.next = head;
        head = newNode;
    }

    void display()
    {
        Node *current = head;
        while (current)
        {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    Node<T> *remove(T val)
    {
        if (val == head.getVal())
        {
            prev_head =
                head = head.getNext();
            return
        }
        head = head.getNext();
        return head;
    };

    Node<T> *find(T val)
    {
        Node<T> *cur = head;
        while
            return head;
    };
};