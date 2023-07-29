#ifndef CONTAINERSAFEDELETEWHILELOOP_H
#define CONTAINERSAFEDELETEWHILELOOP_H

#include <unordered_set>

template <class T>
class ContainerSafeDeleteWhileLoop
{
public:
    void addElem(T elem) {
        m_elems.insert(elem);
    }
    void removeElem(T elem) {
        if(!m_inIteration)
            m_elems.erase(elem);
        else
            m_elemsToDeleteOnIterEnd.insert(elem);
    }
    T getFirst() {
        m_actualElem = m_elems.begin();
        if(notEnd()) {
            m_inIteration = true;
            return *m_actualElem;
        }
        return {};
    }
    T getNext() {
        if(notEnd()) {
            m_actualElem++;
            ignoreDeleted();
        }
        if(notEnd())
            return *m_actualElem;
        m_inIteration = false;
        doDeletions();
        return {};
    }
private:
    inline bool notEnd() const {
        return m_actualElem != m_elems.end();
    }
    bool isToDelete(T elem) {
        return m_elemsToDeleteOnIterEnd.find(elem) != m_elemsToDeleteOnIterEnd.end();
    }
    void ignoreDeleted() {
        while(notEnd() && isToDelete(*m_actualElem))
            m_actualElem++;
    }
    void doDeletions() {
        for(auto elemToPostDelete : m_elemsToDeleteOnIterEnd)
            removeElem(elemToPostDelete);
        m_elemsToDeleteOnIterEnd.clear();
    }
    bool m_inIteration = false;
    std::unordered_set<T> m_elems;
    std::unordered_set<T> m_elemsToDeleteOnIterEnd;
    typename std::unordered_set<T>::iterator m_actualElem = m_elems.end();
};

#endif // CONTAINERSAFEDELETEWHILELOOP_H
