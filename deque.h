#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <stdio.h>
#include <cstring>

// https://github.com/Avens8/deque

template <class T>
class Deque;

template <class T, bool constant = false>
class DequeIterator : public std::iterator<std::random_access_iterator_tag,
typename std::conditional<constant, const T, T>::type> {
    friend Deque<T>;
    
public:
    using iterator_ = DequeIterator<T, constant>;
    using pointer = typename std::conditional<constant, const T *, T *>::type;
    using reference = typename std::conditional<constant, const T &, T &>::type;
    
    DequeIterator(const DequeIterator<T, false> &other)
    : index(other.index),
    deque(other.deque) {}
    
    DequeIterator() : index(0), deque(nullptr) {}
    
    ~DequeIterator() {}
    
    
    iterator_ & operator = (const iterator_& other) {
        if (deque == other.deque && index == other.index)
            return *this;
        
        index = other.index;
        deque = other.deque;
        return *this;
    }
    
    reference operator *() {
        return const_cast<reference>(deque->buffer[deque->_index(deque->head + index)]);
    }
    
    pointer operator->() {
        return &**this;
    }
    
    T& operator[](size_t offset) {
        return *(*this + offset);
    }
    
    iterator_ & operator +=(const long add) {
        index += add;
        index %= deque->capacity();
        return *this;
    }
    
    /*iterator_ operator +(const long add) {
     iterator_ copy(*this);
     copy += add;
     return copy;
     }*/
    
    iterator_ & operator -=(const long sub) {
        index -= sub;
        index += deque->capacity();
        index %= deque->capacity();
        return *this;
    }
    
    /*iterator_ operator -(const long sub) {
     iterator_ copy(*this);
     copy -= sub;
     return copy;
     }*/
    
    iterator_ & operator ++() {
        *this += 1;
        
        return *this;
    }
    
    iterator_ operator ++(int) {
        iterator_ copy(*this);
        ++*this;
        return copy;
    }
    
    iterator_ operator --() {
        *this -= 1;
        
        return *this;
    }
    
    iterator_ operator --(int) {
        iterator_ copy(*this);
        --*this;
        
        return copy;
    }
    
    long operator -(const iterator_ &other) {
        return index - other.index;
    }
    
    int compare ( const iterator_ &b) const {  // -1 if < ## 0 if == ## 1 if >
        //       std::cout << deque->_index(index) << ' ' << b.deque->_index(b.index)<< std::endl;
        
        if ((deque->head + index) == (b.deque->head + b.index))
            return 0;
        else {
            if (deque->head + index > b.deque->head + b.index)
                return 1;
            else
                return -1;
        }
    }
    
    bool operator < (iterator_ const &b) const {
        return compare(b) == -1;
    }
    
    bool operator > (iterator_ const &b) const {
        return compare(b) == 1;
    }
    
    bool operator <= (iterator_ const &b) const {
        int comp = compare(b);
        return comp == -1 || comp == 0;
    }
    
    bool operator >= (iterator_ const &b) const {
        int comp = compare(b);
        return comp == 1 || comp == 0;
    }
    
    bool operator == (iterator_ const &b) const {
        return compare(b) == 0;
    }
    
    bool operator != (iterator_ const &b) const {
        return compare(b) != 0;
    }
private:
    using DequeT = typename std::conditional<constant, const Deque<T>, Deque<T>>::type;
    
    DequeIterator(DequeT *Deque, size_t index)
    : index(index),
    deque(Deque) {}
    
    size_t index;
    DequeT *deque;
};



template<class T, bool constant = false>
DequeIterator<T, constant> operator+(DequeIterator<T, constant> b, long long a)
{
    DequeIterator<T, constant> copy(b);
    copy += a;
    return copy;
}


template<class T, bool constant = false>
DequeIterator<T, constant> operator-(DequeIterator<T, constant> b, long long a)
{
    DequeIterator<T, constant> copy(b);
    copy -= a;
    return copy;
}

template<typename T>
class Deque {
public:
    
    explicit Deque(size_t minBufSize = 4)
    : bufSize(GetMinimalCapacity(minBufSize)),
    curSize(0),
    head(0),
    tail(0),
    buffer(new T[bufSize]) {
        std::memset(buffer, 0, capacity() * sizeof(T));
    }
    Deque(const Deque<T> &source)
    : bufSize(source.bufSize),
    curSize(source.curSize),
    head(source.head),
    tail(source.tail),
    buffer(new T[bufSize]) {
        std::memcpy(buffer, source.buffer, capacity() * sizeof(T));
        //        std::copy(buffer, buffer + bufSize, source.buffer);
    }
    
    ~Deque() {
        delete[] buffer;
    }
    
    Deque& operator=(const Deque &source) {
        if (&source == this)
            return const_cast<Deque<T> &>(source);
        bufSize = source.bufSize;
        curSize = source.curSize;
        head = source.head;
        tail = source.tail;
        buffer = new T[bufSize];
        for (size_t i = 0; i < bufSize; ++i)
            buffer[i] = source.buffer[i];
        return *this;
    }
    
    const T& operator[](size_t index) const {
        return buffer[_index(head + index)];
    }
    
    T & operator[](size_t index) {
        
        return buffer[_index(head + index)];
    }
    
    size_t capacity() const {
        return bufSize;
    }
    
    size_t size() const {
        return curSize;
    }
    
    bool empty() const {
        return size() == 0;
    }
    
    void push_front(const T &element) {
        growIfNecessary();
        
        if (head == 0)
            head = capacity();
        --head;
        
        buffer[head] = element;
        ++curSize;
    }
    
    void pop_front() {
        ++head;
        if (head == capacity())
            head = 0;
        
        --curSize;
        
        shrinkIfNecessary();
        
        return ;
    }
    
    void push_back(const T &element) {
        growIfNecessary();
        if (tail == capacity()) {
            tail = 0;
        }
        buffer[tail] = element;
        
        ++tail;
        
        ++curSize;
    }
    
    void pop_back() {
        assert(size() > 0);
        
        if (tail == 0)
            tail = capacity();
        --tail;
        --curSize;
        
        shrinkIfNecessary();
        
        return ;
    }
    
    const T & front() const {
        return (*this)[0];
    }
    
    T & front() {
        return (*this)[0];
    }
    
    const T & back() const {
        return (*this)[size() - 1];
    }
    
    T & back() {
        return (*this)[size() - 1];
    }
    
    using iterator = DequeIterator<T, false>;
    using const_iterator = DequeIterator<T, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    friend iterator;
    friend const_iterator;
    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }
    
    iterator begin() {
        return iterator(this, 0);
    }
    
    const_iterator begin() const {
        return cbegin();
    }
    
    const_iterator cend() const {
        return const_iterator(this, size());
    }
    
    iterator end() {
        return iterator(this, size());
    }
    
    const_iterator end() const {
        return cend();
    }
    
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    
    const_reverse_iterator rbegin() const {
        return crbegin();
    }
    
    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }
    
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    
    const_reverse_iterator rend() const {
        return crend();
    }
    
private:
    
    size_t GetMinimalCapacity(size_t desired) {
        size_t capacity = 2;
        while (desired & ~(capacity - 1))
            capacity <<= 1;
        
        return capacity;
    }
    
    void growIfNecessary() {
        if (size() + 1 == capacity())
            Realloc(bufSize * 2);
    }
    
    void shrinkIfNecessary() {
        if (size() && size() * 4 < capacity())
            Realloc(bufSize / 4);
    }
    size_t _index(long long n, long long _size = -1) const
    {
        if (_size == -1)
            _size = bufSize;
        return n % _size;
    }
    void Realloc(size_t capacity) {
        assert(capacity >= size());
        
        T *new_data = new T[capacity];
        if (head < tail) {
            std::memcpy(new_data, buffer + head, size() * sizeof(T));
        }
        else {
            const size_t left_part_size = tail;
            const size_t right_part_size = size() - left_part_size;
            std::memcpy(new_data + right_part_size, buffer, left_part_size * sizeof(T));
            std::memcpy(new_data, buffer + head, right_part_size * sizeof(T));
        }
        std::memset(new_data + size(), 0, (capacity - size()) * sizeof(T));
        
        head = 0;
        tail = size();
        
        delete[] buffer;
        buffer = new_data;
        
        bufSize = capacity;
    }
    
    size_t bufSize;
    size_t curSize;
    size_t head;
    size_t tail;
    T *buffer;
};




