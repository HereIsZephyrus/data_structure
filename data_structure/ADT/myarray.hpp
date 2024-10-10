//
//  myarray.hpp
//  data_structure
//
//  Created by ChanningTong on 10/10/24.
//

#ifndef myarray_hpp
#define myarray_hpp

#include <stdio.h>
namespace tcb{
template <class Object>
class Array {
    size_t size;
    Object* data;
public:
    explicit Array(size_t setSize): size(setSize){
        data = new Object(size);
    }
    ~Array(){
        delete [] data;
    }
    Array(const Array& rhs);
    Array(Array && rhs);
    Object& operator[](size_t index); // fast access
    Object& at(size_t index); // security access
    class Iterator;
    size_t getSize() const{return size;};
    void fill(const Object& val);
    void set(size_t index,const Object& val);
};
}
#endif /* myarray_hpp */
