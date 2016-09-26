#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <vector>
#include <initializer_list>

template <class T>
class CircularBuffer {
  public:
    CircularBuffer(): current_index_{}, data_{}
    {}

    CircularBuffer(std::vector<T> data): current_index_{}, data_{data}
    {}

    CircularBuffer(std::initializer_list<T> init ) : data_{init}
    {}

    ~CircularBuffer() {};

    int size() { return data_.size(); }

    void push_back(T elem ) {data_.push_back(elem);}

    T& get() { return data_[current_index_];}

    void next() {
        current_index_ = (current_index_ == data_.size() -1) ? 0 : current_index_ +1;
    }

  private:
    std::vector<T> data_;
    int current_index_;
};

#endif
