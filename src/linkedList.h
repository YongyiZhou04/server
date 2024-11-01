#pragma once
#include <memory>

/**
 * @brief A template for a node.
 *
 * The node is to be used in a doubly linked list.
 *
 * @param T The value type.
 *
 * @var T val The value stored by the node.
 * @var std::shared_ptr<Node<T>> next Shared pointer of the next node.
 * @var std::shared_ptr<Node<T>> prev Shared pointer of the previous node.
 */
template <typename T>
class Node
{
public:
    T val;
    std::shared_ptr<Node<T>> next;
    std::shared_ptr<Node<T>> prev;

    /* Constructor */
    Node(T val) : val(val) {};

    /* Method to print out the node */
    void display() { std::cout << val << std::endl; }
};

/**
 * @brief A template for a doubly (non-circular) linked list.
 *
 * @param T The value type that the linked list should store.
 */
template <typename T>
class LinkedList
{
public:
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;

    /* Constructor */
    LinkedList() {};

    /**
     * @brief Prepends a node with the given value to the linked list.
     *
     * @param val The value to be inserted.
     *
     * @return A shared pointer to the node.
     */
    std::shared_ptr<Node<T>> insertAtBeginning(T val)
    {
        std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(val);
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

    /**
     * @brief Appends a node with the given value to the end of the linked list.
     *
     * @param val The value to be inserted.
     *
     * @return A shared pointer to the node.
     */
    std::shared_ptr<Node<T>> insertAtEnd(T val)
    {
        std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(val);
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

    /**
     * @brief Removes the head of the linked list.
     *
     * @return A shared pointer to the node.
     */
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

    /**
     * @brief Removes the tail of the linked list.
     *
     * @return A shared pointer to the node.
     */
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

    /**
     * @brief Removes the first node with the given value from the linked list.
     *
     * @param val The value to be removed.
     *
     * @return A shared pointer to the node.
     */
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

    /**
     * @brief Finds and returns the node with the given value in the linked list.
     *
     * @param val The value to be found.
     *
     * @return A shared pointer to the node.
     */
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

    /* Method to print out the linked list */
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