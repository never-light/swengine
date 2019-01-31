#pragma once

template<class T>
typename std::vector<T>::iterator insertSorted(std::vector<T> & vec, T const& item) {
	return vec.insert(std::upper_bound(vec.begin(), vec.end(), item), item);
}