#pragma once

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cassert>

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
	
	/*const Deque<T>& operator=(const Deque<T> &source) {
		if (source == *this)
			return *this;

		bufSize = source.bufSize;
		curSize = source.curSize;
		head = source.head;
		tail = source.tail;

		delete[] buffer;

		buffer = new T[bufSize];
		std::memcpy(buffer, source.buffer, capacity() * sizeof(T));

		return *this;
	}*/
    
	const T& operator[](size_t index) const {
		index += head;
		if (index > size())
			index -= size();

		return buffer[index];
	}

	T & operator[](size_t index) {
		index += head;
		if (index > size())
			index -= size();

		return buffer[index];
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

	T pop_front() {
		T element = buffer[head];

		++head;
		if (head == capacity())
			head = 0;

		--curSize;

		shrinkIfNecessary();

		return element;
	}

	void push_back(const T &element) {
		growIfNecessary();

		buffer[tail] = element;

		++tail;
		if (tail == capacity()) {
			tail = 0;
		}
		++curSize;
	}

	T pop_back() {
		assert(size() > 0);

		if (tail == 0)
			tail = capacity();
		--tail;
		--curSize;

		T element = buffer[tail];

		shrinkIfNecessary();

		return element;
	}

	const T & front() const {
		return (*this)[0];
	}

	T & Front() {
		return (*this)[0];
	}

	const T & back() const {
		return (*this)[size() ? size() - 1 : 0];
	}

	T & Back() {
		return (*this)[size() ? size() - 1 : 0];
	}

	template <bool constant = false>
	class DequeIterator : public std::iterator<std::random_access_iterator_tag,
		typename std::conditional<constant, const T, T>::type> {
		friend Deque<T>;

	public:
		using iterator = DequeIterator<constant>;
		using pointer = typename std::conditional<constant, const T *, T *>::type;
		using reference = typename std::conditional<constant, const T &, T &>::type;
                 
		DequeIterator(const DequeIterator<false> &other)
			: index(other.index),
			deque(other.deque) {}

        iterator & operator = (const iterator& other) {
            index = other.index;
            deque = other.deque;
            return *this;
        }

		reference operator *() {
			return deque[index];
		}
            
        pointer operator->() const {
            return &deque[index];
        }
            
		T& operator[](size_t offset) {
			return *(*this + offset);
		}

		iterator & operator +=(const long add) {
			index += add;
			return *this;
		}

		iterator operator +(const long add) {
			iterator copy(*this);
			copy += add;

			return copy;
		}

		iterator & operator -=(const long sub) {
			return *this += (-sub);
		}

		iterator operator -(const long sub) {
			return *this + (-sub);
		}

		iterator & operator ++() {
			*this += 1;

			return *this;
		}

		iterator & operator ++(int) {
			iterator copy(*this);
			++*this;

			return copy;
		}

		iterator operator --() {
			*this -= 1;

			return *this;
		}

		iterator operator --(int) {
			iterator copy(*this);
			--*this;

			return copy;
		}

		long operator -(const iterator &other) {
			return index - other.index;
		}
        
        int compare ( const iterator &b) const {  // -1 if < ## 0 if == ## 1 if >
            if (index - b.index == 0)
                return 0;
            else {
                if (index - b.index < 0)
                    return 1;
                else
                    return -1;
            }
        }
    
        bool operator < (iterator const &b) const {
            return compare(b) == -1;
        }
        
        bool operator > (iterator const &b) const {
            return compare(b) == 1;
        }
        
        bool operator <= (iterator const &b) const {
            int comp = compare(b);
            return comp == -1 || comp == 0;
        }
        
        bool operator >= (iterator const &b) const {
            int comp = compare(b);
            return comp == 1 || comp == 0;
        }
        
        bool operator == (iterator const &b) const {
            return compare(b) == 0;
        }
        
        bool operator != (iterator const &b) const {
            return compare(b) != 0;
        }
	private:
		using DequeT = typename std::conditional<constant, const Deque<T>, Deque<T>>::type;

		DequeIterator(DequeT &Deque, size_t index)
			: index(index),
			deque(Deque) {}

		size_t index;
		DequeT &deque;
	};

	using Iterator = DequeIterator<false>;
	using ConstantIterator = DequeIterator<true>;
	using ReverseIterator = std::reverse_iterator<Iterator>;
	using ConstantReverseIterator = std::reverse_iterator<ConstantIterator>;

	ConstantIterator cbegin() const {
		return ConstantIterator(*this, 0);
	}

	Iterator begin() {
		return Iterator(*this, 0);
	}

	ConstantIterator begin() const {
		return cbegin();
	}

	ConstantIterator cend() const {
		return ConstantIterator(*this, size());
	}

	Iterator end() {
		return Iterator(*this, size());
	}

	ConstantIterator end() const {
		return cend();
	}

	ConstantReverseIterator crbegin() const {
		return ConstantReverseIterator(cend());
	}

	ReverseIterator rbegin() {
		return ReverseIterator(end());
	}

	ConstantIterator rbegin() const {
		return crbegin();
	}

	ConstantReverseIterator crend() const {
		return ConstantReverseIterator(cbegin());
	}

	ReverseIterator rend() {
		return ReverseIterator(begin());
	}

	ConstantIterator rend() const {
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

