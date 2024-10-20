//
//  myarray.hpp
//  data_structure
//
//  Created by ChanningTong on 10/10/24.
//

#ifndef myarray_hpp
#define myarray_hpp

#include <cstring>
#include <string>
#include <stdexcept>
namespace tcb{
template <class Object>
class Array {
    size_t size;
    Object* data;
public:
    Array() = delete;
    explicit Array(size_t setSize): size(setSize){
        data = new Object[size];
    }
    ~Array(){
        delete [] data;
    }
    Array(const Array& rhs){
        data = new Object(size);
        for (size_t i = 0; i < size; i++)
            data[i] = rhs.data[i];
    }
    Array(Array && rhs){
        rhs.data = nullptr;
        rhs.size = 0;
    }
    Object& operator[](size_t index){ return data[index];} // fast access
    Object& at(size_t index){ // security access
        if (index >= size || index < 0)
            throw std::out_of_range("Index out of range");
        return data[index];
    }
    size_t getSize() const{return size;};
    void fill(const Object& val){
        for (size_t i = 0; i < size; i++)
            data[i] = val;
    }
    void set(size_t index,const Object& val){
        if (index >= size || index < 0)
            throw std::out_of_range("Index out of range");
        data[index] = val;
    }
    class element_iter {
            // element_iter traits
        using iterator_category = std::forward_iterator_tag;
        using difference_type = long;
        using value_type = Object;
        using pointer = Object *;
        using reference = const Object &;
    private:
        pointer ptr;
    public:
        element_iter(pointer p) :ptr(p) {}
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
    element_iter begin() {Object* head = &data[0];return element_iter(head);}
    element_iter end() {Object* head = &data[0];return element_iter(head + size);}
    using iterator = element_iter;
    using const_iterator = const iterator;
    const_iterator begin() const{Object* head = &data[0];return const_iterator::begin(head);}
    const_iterator end() const{Object* head = &data[0];return const_iterator::end(head + size);}
};
}
#endif /* myarray_hpp */
