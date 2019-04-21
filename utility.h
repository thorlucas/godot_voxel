#ifndef HEADER_VOXEL_UTILITY_H
#define HEADER_VOXEL_UTILITY_H

#include "vector3i.h"
#include <core/pool_vector.h>
#include <core/ustring.h>
#include <core/vector.h>
#include <vector>

// Takes elements starting from a given position and moves them at the beginning,
// then shrink the array to fit them. Other elements are discarded.
template <typename T>
void shift_up(Vector<T> &v, int pos) {

	int j = 0;
	for (int i = pos; i < v.size(); ++i, ++j) {
		v.write[j] = v[i];
	}

	int remaining = v.size() - pos;
	v.resize(remaining);
}

// Pops the last element of the vector and place it at the given position.
// (The element that was at this position is the one removed).
template <typename T>
void unordered_remove(Vector<T> &v, int pos) {
	int last = v.size() - 1;
	v.write[pos] = v[last];
	v.resize(last);
}

template <typename T>
void copy_to(PoolVector<T> &to, const Vector<T> &from) {

	to.resize(from.size());

	typename PoolVector<T>::Write w = to.write();

	for (unsigned int i = 0; i < from.size(); ++i) {
		w[i] = from[i];
	}
}

inline String ptr2s(const void *p) {
	return String::num_uint64((uint64_t)p, 16);
}

template <typename T>
void raw_copy_to(PoolVector<T> &to, const std::vector<T> &from) {
	to.resize(from.size());
	typename PoolVector<T>::Write w = to.write();
	memcpy(w.ptr(), from.data(), from.size() * sizeof(T));
}

// Trilinear interpolation between corner values of a cube.
// Cube points respect the same position as in octree_utility.h
template <typename T>
inline T interpolate(const T v0, const T v1, const T v2, const T v3, const T v4, const T v5, const T v6, const T v7, Vector3 position) {

	const float one_min_x = 1.f - position.x;
	const float one_min_y = 1.f - position.y;
	const float one_min_z = 1.f - position.z;
	const float one_min_x_one_min_y = one_min_x * one_min_y;
	const float x_one_min_y = position.x * one_min_y;

	T res = one_min_z * (v0 * one_min_x_one_min_y + v1 * x_one_min_y + v4 * one_min_x * position.y);
	res += position.z * (v3 * one_min_x_one_min_y + v2 * x_one_min_y + v7 * one_min_x * position.y);
	res += position.x * position.y * (v5 * one_min_z + v6 * position.z);

	return res;
}

#endif // HEADER_VOXEL_UTILITY_H
