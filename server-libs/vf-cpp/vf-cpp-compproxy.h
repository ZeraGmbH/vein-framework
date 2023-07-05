#ifndef VEINCOMPPROXY_H
#define VEINCOMPPROXY_H

#include "vf-cpp-component.h"

namespace VfCpp {

// Type safe shared pointer wrapper of cVeinModuleComponent
template <class T> class VeinCompProxy{
public:
    void* operator new(std::size_t size) = delete;
    VeinCompProxy() :
        m_component(nullptr)
    {
    };
    VeinCompProxy(cVeinModuleComponent::Ptr obj) :
        m_component(obj)
    {
    };
    VeinCompProxy(VeinCompProxy &obj) {
        m_component = obj.m_component;
    };
    ~VeinCompProxy() {
        m_component.clear();
    };

    T value() {
        if(m_component != nullptr)
            return m_component->getValue().value<T>();
        return T();
    };
    void setValue(T val) {
       if(m_component != nullptr)
            m_component->setValue(val);
    };

    VeinCompProxy<T>& operator=(const T& other) {
        if(m_component != nullptr)
            setValue(other);
        return *this;
    };
    VeinCompProxy<T>& operator=(cVeinModuleComponent::Ptr other) {
        m_component = other;
        return *this;
    };
    bool operator==(const T& val) {
        return val == value();
    };
private:
    cVeinModuleComponent::Ptr m_component;
};

}

#endif // VEINCOMPPROXY_H
