#pragma once
#include <memory>

template <typename T>
class Node
{
public:
    T val;
    std::shared_ptr<Node<T>> next;
    std::shared_ptr<Node<T>> prev;
    Node(T val) : val(val) {};
    void display() { std::cout << val << std::endl; }
};

template <typename T>
class LinkedList
{
public:
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;

    LinkedList() {};

    std::shared_ptr<Node<T>> insertAtBeginning(T val)
    {
        std::shared_ptr<Node<T>> newNode = std::make_unique<Node<T>>(val);
        newNode->next = head;
        if (head)
        {
            head->prev = newNode;
        }
        head = newNode;
        if (!tail)
        {
            tail = newNode;
        }
        return newNode;
    }

    std::shared_ptr<Node<T>> insertAtEnd(T val)
    {
        std::shared_ptr<Node<T>> newNode = std::make_unique<Node<T>>(val);
        newNode->prev = tail;
        if (tail)
        {
            tail->next = newNode;
        }
        tail = newNode;
        if (!head)
        {
            head = newNode;
        }
        return newNode;
    }

    std::shared_ptr<Node<T>> removeAtBeginning()
    {
        std::shared_ptr<Node<T>> prev_head = head;
        if (prev_head)
        {
            if (prev_head->next)
            {
                prev_head->next->prev = nullptr;
            }
            head = prev_head->next;
            prev_head->next = nullptr;
        }
        if (prev_head == tail)
        {
            tail = nullptr;
        }
        return prev_head;
    }

    std::shared_ptr<Node<T>> removeAtEnd()
    {
        std::shared_ptr<Node<T>> prev_tail = tail;
        if (prev_tail)
        {
            if (prev_tail->prev)
            {
                prev_tail->prev->next = nullptr;
            }
            tail = prev_tail->prev;
            prev_tail->prev = nullptr;
        }
        if (prev_tail == head)
        {
            head = nullptr;
        }
        return prev_tail;
    }

    std::shared_ptr<Node<T>> remove(T val)
    {
        std::shared_ptr<Node<T>> n = find(val);

        // if the node is null, then return null
        if (!n)
        {
            return nullptr;
        }

        // if the node is the head, then reset head
        if (n->prev)
        {
            n->prev->next = n->next;
        }
        if (n->next)
        {
            n->next->prev = n->prev;
        }
        if (n->val == head->val)
        {
            head = n->next;
        }
        return n;
    }

    // returns the removed node
    std::shared_ptr<Node<T>> find(T val)
    {
        std::shared_ptr<Node<T>> cur = head;
        while (cur)
        {
            if (cur->val == val)
            {
                return cur;
            }
            cur = cur->next();
        }
        return nullptr;
    }

    void display()
    {
        std::shared_ptr<Node<T>> current = head;
        while (current)
        {
            std::cout << current->val << " <-> ";
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }
};