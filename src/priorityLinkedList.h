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
template <typename T, typename K>
class Node
{
public:
    T val;
    K key;
    std::shared_ptr<Node<T, K>> next;
    std::shared_ptr<Node<T, K>> prev;

    /* Constructor */
    Node(T val, K key) : val(val), key(key) {};

    /* Method to print out the node */
    void display() { std::cout << val << key << std::endl; }
};

/**
 * @brief A template for a doubly (non-circular) linked list.
 *
 * @param T The value type that the linked list should store.
 */
template <typename T, typename K>
class PriorityLinkedList
{

private:
    std::mutex mutex;

public:
    std::shared_ptr<Node<T, K>> head;
    std::shared_ptr<Node<T, K>> tail;

    PriorityLinkedList(std::shared_ptr<Node<T, K>> head = nullptr, std::shared_ptr<Node<T, K>> tail = nullptr) : head(head), tail(tail) {};
    /**
     * @brief Chronologically inserts a node with the given value to the linked list.
     *
     * @param val The value to be inserted.
     *
     * @return A shared pointer to the node.
     */
    std::shared_ptr<Node<T, K>> insert(T val, K key)
    {
        std::lock_guard<std::mutex> lock(mutex);
        // Create the new node to insert.
        std::shared_ptr<Node<T, K>> newNode = std::make_shared<Node<T, K>>(val, key);

        if (!head)
        {
            // Case 1: Empty list - new node becomes head and tail.
            head = newNode;
            tail = newNode;
        }
        else
        {
            // Traverse the list from the tail to find the correct position.
            std::shared_ptr<Node<T, K>> curNode = tail;
            while (curNode && curNode->key > key)
            {
                curNode = curNode->prev;
            }

            if (!curNode)
            {
                // Case 2: Insertion at the head (new key is the smallest).
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
            else if (curNode == tail)
            {
                // Case 3: Insertion at the tail (new key is the largest).
                newNode->prev = tail;
                tail->next = newNode;
                tail = newNode;
            }
            else
            {
                // Case 4: Insertion in the middle.
                std::shared_ptr<Node<T, K>> nextNode = curNode->next;
                newNode->next = nextNode;
                newNode->prev = curNode;
                curNode->next = newNode;

                if (nextNode)
                {
                    nextNode->prev = newNode;
                }
            }
        }

        return newNode;
    }

    /**
     * @brief Removes the head of the linked list.
     *
     * @return A shared pointer to the node.
     */
    std::shared_ptr<Node<T, K>> removeAtBeginning()
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::shared_ptr<Node<T, K>> prev_head = head;
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
    std::shared_ptr<Node<T, K>> removeAtEnd()
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::shared_ptr<Node<T, K>> prev_tail = tail;
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
    std::shared_ptr<Node<T, K>> remove(T val, K key)
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::shared_ptr<Node<T, K>> n = find(val);

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
     * @brief Removes the first node with the given value from the linked list.
     *
     * @param val The value to be removed.
     *
     * @return A shared pointer to the node.
     */
    std::shared_ptr<Node<T, K>> remove(std::shared_ptr<Node<T, K>> n)
    {
        std::lock_guard<std::mutex> lock(mutex);
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
    std::shared_ptr<Node<T, K>> find(T val)
    {
        std::shared_ptr<Node<T, K>> cur = head;
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
        std::shared_ptr<Node<T, K>> current = head;
        while (current)
        {
            std::cout << current->val << " <-> ";
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }
};