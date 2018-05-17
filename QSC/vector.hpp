#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
public:
	T *data;
	size_t currentlength;
	size_t maxsize;
	class const_iterator;
	class iterator {
	private:
		T *pos = NULL;
		vector *vec = NULL;
	public:
		/*
		* Constructs of iterator
		*/
		iterator(T *obj = NULL)
		{
			pos = obj;
		}
		iterator(T *obj = NULL, vector<T> *point = NULL)
		{
			pos = obj;
			vec = point;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			pos += n;
			return *this;
		}
		iterator operator-(const int &n) const
		{
			pos -= n;
			return *this;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (decltype(rhs) == decltype(pos))
				return(pos - rhs);
			else throw invaild_iterator;
		}
		iterator operator+=(const int &n)
		{
			pos += n;
			return *this;
		}
		iterator operator-=(const int &n)
		{
			pos -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			pos++;
			return *this;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
			pos++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			pos--;
			return *this;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
			pos--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return *pos;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return (pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const
		{
			return (pos == rhs.pos);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return (pos != rhs.pos);
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return (pos != rhs.pos);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	private:
		T *pos = NULL;
	public:
		const_iterator(T *obj = NULL)
		{
			pos = obj;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			pos += n;
			return *this;
		}
		const_iterator operator-(const int &n) const
		{
			pos -= n;
			return *this;
		}
		// return the distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator- (const iterator &rhs) const
		{
			if (decltype(rhs) = decltype(pos))
				return(pos - rhs);
			else throw invaild_iterator;
		}
		const_iterator operator+=(const int &n) const
		{
			pos += n;
			return *this;
		}
		const_iterator operator-=(const int &n) const
		{
			pos -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) const
		{
			pos++;
			return *this;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() const
		{
			pos++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) const
		{
			pos--;
			return *this;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() const
		{
			pos--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return *pos;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return (pos == rhs.pos);
		}
		bool operator==(const const_iterator &rhs) const
		{
			return (pos == rhs.pos);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return (pos != rhs.pos);
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return (pos != rhs.pos);
		}
	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */
	vector()
	{
		maxsize = 10;
		currentlength = 0;
		data = malloc(10 * sizeof(T));
	}
	vector(const vector &other)
	{
		data = other.data;
		currentlength = other.currentlength;
		maxsize = other.maxsize;
	}
	/**
	 * TODO Destructor
	 */
	~vector()
	{
		for (int i = 0; i < currentlength; i++)
			data[i].~T();
		free(data);
	}
	/*
	* 	Change the space of vector to a half more than before
	*/
	void changespace()
	{
		maxsize = maxsize * 3 / 2;
		T *tmp = data;
		data = malloc(maxsize * sizeof(T));
		for (int i = 0; i < currentlength; i++)
			data[i] = tmp[i];
		for (int i = 0; i < currentlength; i++)
			tmp[i].~T();
		free(tmp);
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
	{
		if (pos >= 0 && pos < currentlength)
			return data[pos];
		else throw index_out_of_bound;
	}
	const T & at(const size_t &pos) const
	{
		if (pos >= 0 && pos < currentlength)
			return data[pos];
		else throw index_out_of_bound;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
	{
		if (pos >= 0 && pos < currentlength)
			return data[pos];
		else throw index_out_of_bound;
	}
	const T & operator[](const size_t &pos) const
	{
		if (pos >= 0 && pos < currentlength)
			return data[pos];
		else throw index_out_of_bound;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other)
	{
		maxsize = other.maxsize;
		currentlength = other.currentlength;
		data = malloc(maxsize * sizeof(T));
		for (int i = 0; i < currentlength; i++)
			data[i] = other.data[i];
		return *this;
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */

	const T & front() const
	{
		if (currentlength != 0)
			return data[0];
		else throw container_is_empty;
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const
	{
		if (currentlength != 0)
			return data[currentlength - 1];
		else throw container_is_empty;
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin()
	{
		iterator tmp(data);
		return tmp;
	}
	const_iterator cbegin() const
	{
		const_iterator tmp(data);
		return tmp;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end()
	{
		iterator tmp(data);
		tmp += currentlength;
		return tmp;
	}
	const_iterator cend() const
	{
		const_iterator tmp(data);
		tmp += currentlength;
		return tmp;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const
	{
		return (currentlength == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return currentlength;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const
	{
		return maxsize;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		currentlength == 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value)
	{
		if (maxsize == currentlength)
			changespace();
		else
		{
			for (iterator iter = end() - 1; iterator != pos - 1; iter--)
				*(iter + 1) = *iter;
			*(pos) = value;
		}
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
	{
		if (maxsize == currentlength);
			changespace();
		else
		{
			if (ind > currentlength)
				throw index_out_of_bound;
			else
			{
				for(size_t i = currentlength; i >= ind; i++)
				{
					data[i + 1] = data;
				}
				data[i] = value;
				iterator tmp(data);
				tmp += ind;
				return tmp;
			}
		}
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
	{
		if (pos == end() - 1)
			return end();
		else
		{
			for (iterator iter = pos; iter != end() - 1; iter++)
				*(iter) = *(iter++);
				currentlength--;
			return pos;
		}
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind)
	{
		if (ind >= currentlength)
			throw index_out_of_bound;
		else
		{
			for (size_t i = ind; i < currentlength; i++)
				data[i] = data[i + 1];
			currentlength--;
		}
		iterator tmp(data);
		tmp += ind;
		return tmp;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
	{
		if (currentlength == maxsize)
			changespace();
		else
		{
			data[currentlength++] = value;
		}
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back()
	{
		if(currentlength == 0)
			throw container_is_empty;
		else
		{
			currentlength--;
		}
	}
};


}

#endif
