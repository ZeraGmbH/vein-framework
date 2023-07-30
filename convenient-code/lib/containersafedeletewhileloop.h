#ifndef CONTAINERSAFEDELETEWHILELOOP_H
#define CONTAINERSAFEDELETEWHILELOOP_H

#include <unordered_set>

template <class ItemType, class ItemContainerType>
class ContainerSafeDeleteWhileLoop
{
public:
    void addItem(ItemType item) {
        m_items.insert(item);
    }

    void removeItem(ItemType item) {
        if(!m_inIteration)
            m_items.erase(item);
        else
            m_itemsToDeleteOnIterEnd.insert(item);
    }

    ItemType getFirst() {
        m_iterActualItem = m_items.begin();
        if(notEnd()) {
            m_inIteration = true;
            return *m_iterActualItem;
        }
        return {};
    }

    ItemType getNext() {
        if(notEnd()) {
            m_iterActualItem++;
            ignoreDeleted();
        }
        if(notEnd())
            return *m_iterActualItem;
        m_inIteration = false;
        doDeletions();
        return {};
    }
private:
    inline bool notEnd() const {
        return m_iterActualItem != m_items.end();
    }
    bool isToDelete(ItemType item) {
        return m_itemsToDeleteOnIterEnd.find(item) != m_itemsToDeleteOnIterEnd.end();
    }
    void ignoreDeleted() {
        while(notEnd() && isToDelete(*m_iterActualItem))
            m_iterActualItem++;
    }
    void doDeletions() {
        for(auto elemToPostDelete : m_itemsToDeleteOnIterEnd)
            removeItem(elemToPostDelete);
        m_itemsToDeleteOnIterEnd.clear();
    }
    bool m_inIteration = false;
    ItemContainerType m_items;
    typename ItemContainerType::iterator m_iterActualItem = m_items.end();
    std::unordered_set<ItemType> m_itemsToDeleteOnIterEnd;
};

#endif // CONTAINERSAFEDELETEWHILELOOP_H
