#pragma once

#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cstring>
#include <sstream>

template<class T>
class vec {
private:
    T *array = nullptr;
    size_t size = 0;
    size_t capacity = 0;
public:
    vec() {};

    vec(size_t size);

    vec(T array[], size_t size, bool copy = true);

    vec(vec<T> &&other) noexcept;

    vec(const vec<T> &other);

    vec<T>& operator=(vec<T> &&other) noexcept;

    size_t get_size() const { return size; }

    size_t get_capacity() const { return capacity; }

    T *get_array() const { return array; }

    T *release_array();

    void push_back(T element);

    void replace(T element, size_t ind) const;

    T &at(size_t i) const;

    T &operator[](size_t i) const { return at(i); };

    void pop_back() { size -= size ? 1 : 0; }

    void clear();

    std::string to_string() const;

    ~vec() { clear(); }
};

template<class T>
vec<T>::vec(const vec<T> &other) : array(new T[other.size]), size(other.size), capacity(other.capacity) {
    for(size_t i = 0; i < size; i++)
        array[i] = other.array[i];
}

template<class T>
vec<T>::vec(vec<T> &&other) noexcept : array(other.array), size(other.size), capacity(other.capacity) {
    other.array = nullptr;
    other.size = other.capacity = 0;
}

template<class T>
vec<T> &vec<T>::operator=(vec<T> &&other) noexcept {
    array = other.array;
    size = other.size;
    capacity = other.capacity;
    other.array = nullptr;
    other.size = other.capacity = 0;
    return *this;
}

template<class T>
vec<T>::vec(T *array, size_t size, bool copy) {
    if(size == 0) return;
    if (copy) {
        this->array = new T[size];
        for(size_t i = 0; i < size; i++)
            this->array[i] = array[i];
    }
    else this->array = array;

    this->size = size;
    this->capacity = size;
}

template<class T>
vec<T>::vec(size_t size) : size(size), capacity(size) {
    if(size != 0)
        array = new T[size]();
}

template<class T>
T *vec<T>::release_array() {
    size = 0;
    capacity = 0;
    auto arrpcpy = array;
    array = nullptr;
    return arrpcpy;
}


template<class T>
std::string vec<T>::to_string() const {
    std::stringstream ss;
    ss << "{array ptr: " << array << ", size: " << size << ", capacity: " << capacity << "}" << std::endl;
    for(size_t i = 0; i < get_size() && i < 2; i++){
        ss << "[" << i << "] = " << array[i] << std::endl;
    }
    if(size > 3) ss << "..." << std::endl;
    if(size > 2) ss << "[" << size-1 << "] = " << array[size-1] << std::endl;

    return ss.str();
}

template<class T>
void vec<T>::clear() {
    delete[] array;
    array = nullptr;
    size = capacity = 0;
}

template<class T>
void vec<T>::replace(T element, size_t ind) const {
    this->at(ind) = element;
}

template<class T>
void vec<T>::push_back(T element) {
    if (capacity == 0) {
        capacity = 1;
        array = new T[1];
    } else if (size + 1 > capacity) {
        capacity *= 2;
        T *narr = new T[capacity];
        for (size_t i = 0 ; i < size; i++){
            narr[i] = std::move(array[i]);
        }
        delete[] array;
        array = narr;
    }
    array[size++] = element;
}

template<class T>
T &vec<T>::at(size_t i) const {
    if (i >= size)
        throw std::runtime_error("index out-of-bounds");
    return array[i];
}
