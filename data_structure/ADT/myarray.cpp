//
//  myarray.cpp
//  data_structure
//
//  Created by ChanningTong on 10/10/24.
//

#include "myarray.hpp"
#include <iostream>
#include <stdexcept>

namespace tcb {
template<class Object>
class Array<Object>::Iterator{
    Object* ptr;
public:
    Iterator(Object* p) : ptr(p){}
    Object& operator*(){return *ptr;}
    Iterator& operator++(){
        ++ptr;
        return *this;
    }
    Iterator operator++(int){
        Iterator temp = *this;
        ++ptr;
        return temp;
    }
    bool operator !=(const Iterator& rhs) const{
        return ptr != rhs.ptr;
    }
    bool operator ==(const Iterator& rhs) const{
        return ptr == rhs.ptr;
    }
};

template <class Object>
Array<Object>::Array(const Array& rhs) : size(rhs.size){
    data = new Object(size);
    for (size_t i = 0; i < size; i++)
        data[i] = rhs.data[i];
}

template <class Object>
Array<Object>::Array(Array&& rhs) : size(rhs.size),data(rhs.data){
    rhs.data = nullptr;
    rhs.size = 0;
}

template<class Object>
Object& Array<Object>::operator[](size_t index){
    return data[index];
}

template<class Object>
Object& Array<Object>::at(size_t index){
    if (index >= size)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template<class Object>
void Array<Object>::fill(const Object& val){
    for (size_t i = 0; i<size; i++) {
        data[i] = val;
    }
}

template<class Object>
void Array<Object>::set(size_t index,const Object& val){
    if (index >= size)
        throw std::out_of_range("Index out of range");
    data[index] = val;
}
}
