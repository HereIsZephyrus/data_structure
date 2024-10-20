//
//  linear.hpp
//  data_structure
//
//  Created by ChanningTong on 10/10/24.
//

#ifndef linear_hpp
#define linear_hpp

#include <cstring>
#include <string>
#include <stdexcept>
namespace tcb{
template <class Object>
class Array {
    size_t size;
    Object* data;
public:
    Array():size(0),data(nullptr){};
    explicit Array(size_t setSize,Object initVal = Object()): size(setSize){
        data = new Object[size];
        std::fill(data,data+size, initVal);
    }
    ~Array(){
        delete [] data;
    }
    Array(const Array& rhs): size(rhs.size){
        data = new Object[size];
        std::copy(rhs.data, rhs.data+size, data);
    }
    Array(Array && rhs){
        if (size != rhs.size)
            throw std::length_error("The aray size is incomparable");
        data = rhs.data;
        rhs.data = nullptr;
        rhs.size = 0;
    }
    const Array& operator= (const Array& rhs){
        if (this == &rhs)
            return *this;
        if (size != rhs.size && size>0)
            throw std::length_error("The aray size is incomparable");
        delete [] data;
        data = new Object[rhs.size];
        size = rhs.size;
        std::copy(rhs.data, rhs.data+size, data);
        return *this;
    }
    Object& operator[](size_t index){ return data[index];} // fast access
    const Object& operator[] (size_t index) const{return data[index];}
    Object& at(size_t index){ // security access
        if (index >= size || index < 0)
            throw std::out_of_range("Index out of range");
        return data[index];
    }
    size_t getSize() const{return size;};
    void fill(const Object& val){std::fill(data,data+size, val);}
    void set(size_t index,const Object& val){
        if (index >= size || index < 0)
            throw std::out_of_range("Index out of range");
        data[index] = val;
    }
    class element_iter {
        // element_iter traits
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Object;
        using pointer = Object *;
        using reference = const Object &;
    private:
        pointer ptr;
    public:
        element_iter(pointer p) : ptr(p) {}
        element_iter& operator=(const element_iter& it) {ptr = it.ptr;}
        bool operator==(const element_iter& it) const {return ptr == it.ptr;}
        bool operator!=(const element_iter& it) const {return ptr != it.ptr;}
        Object& operator*() {return *ptr;}
        element_iter& operator++() {
            ptr++;
            return *this;
        }
        element_iter operator ++(int) {
            element_iter ret_ptr = *this;
            ++(*this);
            return ret_ptr;
        }
        element_iter& operator--() {
            ptr--;
            return *this;
        }
        element_iter operator --(int) {
            element_iter ret_ptr = *this;
            --(*this);
            return ret_ptr;
        }
    };
    using iterator = element_iter;
    iterator begin() {Object* head = &data[0];return iterator(head);}
    iterator end() {Object* head = &data[0];return iterator(head + size);}
    using const_iterator = const iterator;
    const_iterator begin() const{Object* head = &data[0];return const_iterator::begin(head);}
    const_iterator end() const{Object* head = &data[0];return const_iterator::end(head + size);}
};

template <class Object>
class Vector {
    size_t size,capacity;
    Object* data;
public:
    enum { SPARE_CAPACITY = 8};
    Vector():size(0),capacity(SPARE_CAPACITY){
        data = new Object[capacity];
    };
    explicit Vector(size_t initSize,Object initVal = Object()):size(initSize){
        capacity = initSize+SPARE_CAPACITY;
        data = new Object[capacity];
        std::fill(data,data+size,initVal);
    }
    ~Vector(){delete [] data;};
    Vector(const Vector& rhs) : size(rhs.size),capacity(rhs.capacity){
        data = new Object[capacity];
        std::copy(rhs.data,rhs.data + size,data);
    }
    Vector(Vector && rhs): size(rhs.size),capacity(rhs.capacity){
        data = rhs.data;
        rhs.data = nullptr;
        rhs.size = 0;
    }
    const Vector& operator=(const Vector& rhs){
        if (this == &rhs)
            return *this;
        delete [] data;
        size = rhs.size;
        capacity = rhs.capacity;
        data = new Object[capacity];
        std::copy(rhs.data,rhs.data+size,data);
        return *this;
    }
    Object& operator[](size_t index){return data[index];}
    const Object& operator[](size_t index) const{return data[index];}
    Object& at(size_t index){
        if (index >= size || index < 0)
            throw std::out_of_range("Index out of range");
        return data[index];
    }
    void reserve(size_t newCapacity){
        if (newCapacity < size)
            return;
        Object *datum = data;
        capacity = newCapacity;
        data = new Object[capacity];
        std::copy(datum,datum+size,data);
        delete [] datum;
    }
    void resize(size_t newSize){
        if (newSize > capacity)
            reserve(newSize * 2 + 1);
        if (newSize > size)
            std::fill(data + size,data + newSize, Object());
        size = newSize;
    }
    bool isEmpty() const{return size == 0;}
    size_t getSize() const{return size;}
    size_t getCapacity() const{return capacity;}
    void push_back(const Object& x){
        if (size == capacity)
            reserve(size * 2 + 1);
        data[size++] = x;
    }
    void pop_back(){
        if (isEmpty())
            throw std::runtime_error("Pop an empty vector");
        size --;
    }
    void clear(){
        size = 0;
        capacity = SPARE_CAPACITY;
        data = new Object[capacity];
    }
    const Object& back(){return data[size-1];}
    class element_iter{
        // element_iter traits
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Object;
        using pointer = Object *;
        using reference = const Object &;
    private:
        pointer ptr;
    public:
        element_iter(pointer p) : ptr(p){}
        element_iter& operator=(const element_iter& it){ptr = it.ptr;}
        bool operator == (const element_iter& it) const {return ptr == it.ptr;}
        bool operator != (const element_iter& it) const {return ptr != it.ptr;}
        Object& operator *() {return *ptr;}
        element_iter& operator++(){
            ptr++;
            return *this;
        }
        element_iter operator ++(int){
            element_iter ret_ptr = *this;
            ++(*this);
            return ret_ptr;
        }
        element_iter& operator--(){
            ptr--;
            return *this;
        }
        element_iter operator--(int){
            element_iter ret_ptr = *this;
            --(*this);
        }
    };
    using iterator = element_iter;
    iterator begin() {Object* head = &data[0];return iterator(head);}
    iterator end() {Object* head = &data[0];return iterator(head + size);}
    using const_iterator = const iterator;
    const_iterator begin() const{Object* head = &data[0];return const_iterator::begin(head);}
    const_iterator end() const{Object* head = &data[0];return const_iterator::end(head + size);}
};
}
#endif /* linear_hpp */
