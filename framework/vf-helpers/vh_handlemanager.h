#ifndef VH_HANDLE
#define VH_HANDLE

#include "vfhelpers_export.h"

#include <QVector>
#include <QHash>

/**
 * @brief Universal utility classes for Vein Framework
 */
namespace VeinHelper
{
/**
   * @brief Implements a Handle management system
   * @note the type Key must be default constructible and must provide operator==() and operator++()
   */
template<typename Key, typename T> class VFHELPERS_EXPORT HandleManager
{
public:
    HandleManager() : m_max(Key()) {}

    /**
     * @brief returns the first handle with the value t_Value
     */
    Key key(const T &t_value, const Key &t_defaultKey) const { return m_currentHandles.key(t_value, t_defaultKey); }

    /**
     * @brief returns the value stored with Key t_Key or the t_DefaultValue if not found
     */
    T value(const Key &t_key, const T &t_defaultValue) const { return m_currentHandles.value(t_key, t_defaultValue); }

    /**
     * @brief inserts the value with the key nextKey()
     * @param t_value
     * @return the key obtained via nextKey()
     */
    Key append(const T &t_value)
    {
        Key k = nextKey();
        m_currentHandles.insert(k, t_value);
        return k;
    }

    /**
     * @brief removes t_key from the current handles and adds it to the recycling pool
     * @param t_key
     */
    void remove(const Key &t_key)
    {
        if(m_currentHandles.remove(t_key)>0)
        {
            m_recycledHandles.append(t_key);
        }
    }

    /**
     * @brief clears current and recycled handles, and sets m_max to Key()
     */
    void clear()
    {
        m_currentHandles.clear();
        m_recycledHandles.clear();
        m_max = Key();
    }

    /**
     * @brief returns a list of current values
     * @return
     */
    QList<T> values() const { return m_currentHandles.values(); }

    bool contains(Key t_key) const { return m_currentHandles.contains(t_key); }


private:
    /**
     * @brief returns an unused key with respect to recycled keys
     * @return a key that is not used in m_currentHandles
     */
    Key nextKey()
    {
        Key retVal;
        if(m_recycledHandles.isEmpty())
        {
            retVal = m_max;
            m_max++;
        }
        else
        {
            retVal = m_recycledHandles.takeFirst();
        }

        return retVal;
    }


    /**
     * @brief deleted handles end up here to be recycled with append
     */
    QVector<Key> m_recycledHandles;
    /**
     * @brief the currently stored data
     */
    QHash<Key, T> m_currentHandles;
    /**
     * @brief the next unused key
     * @note will only be incremented if no recycled keys are left
     */
    Key m_max;
};
}

#endif // VH_HANDLE

