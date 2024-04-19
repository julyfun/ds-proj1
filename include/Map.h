//
// Created by [NAME] on [DATE].
//

#ifndef PROJECT_MAP_MAP_H
#define PROJECT_MAP_MAP_H

/**
 * @brief Abstract class (ADT) that represents a map.
 * @tparam K Type of the key.
 * @tparam V Type of the value.
 */
template<typename K, typename V>
class Map {
public:
    /**
     * @brief Insert a key-value pair into the map. If the key already exists, update the value.
     * @param key The key to insert / update.
     * @param value The value to insert / update.
     */
    virtual void insert(K key, V value) = 0;

    /**
     * @brief Erase the key and its value from the map.
     * @param key The key to erase.
     */
    virtual void erase(K key) = 0;

    /**
     * @brief Return the pointer to the value of the key. If the key does not exist, return nullptr.
     * @param key The key to find.
     * @return The pointer to the value of the key.
     */
    virtual V* find(K key) = 0;

    /**
     * @brief Return the reference of the value of the key.
     * If the key does not exist, insert the key with a default value and return the reference of the value.
     * @param key The key to find.
     * @return The reference of the value of the key.
     */
    virtual V& operator[](K key) = 0;

    /**
     * @brief Return the reference of the value of the key.
     * If the key does not exist, throw an exception of type std::out_of_range.
     * @param key The key to find.
     * @return The reference of the value of the key.
     */
    virtual V& at(K key) = 0;

    /**
     * @brief Return the number of key-value pairs in the map.
     * @return The number of key-value pairs in the map.
     */
    virtual int size() = 0;

    /**
     * @brief Return whether the map is empty.
     * @return Whether the map is empty.
     */
    virtual bool empty() = 0;

    /**
     * @brief Delete all key-value pairs in the map.
     */
    virtual void clear() = 0;

    /**
     * @brief Destructor of the map.
     * You need to fully release / delete all nodes in the map to avoid memory leaks.
     */
    virtual ~Map() = default;
};

#endif //PROJECT_MAP_MAP_H
