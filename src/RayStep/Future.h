#pragma once

#include <future>
#include <vector>

#include "utils/tplutils.h"

// TODO: enable other possible thread pooling mechanisms instead of std::async


/// present<T> remove any future prefix from type
/// For instance, if T is a common type
/// T == present<T> == present<future<T>> == present<future<future<T>>> == ...
template <typename A> struct present                 { typedef A type; };
template <typename A> struct present<std::future<A>> { typedef A type; };
template <typename A> using Present = typename present<A>::type;


/// Reccursively get futures until the type gets to a present type
template <typename A>
Present<A> getRec(A a) {
	return a;
}
template <typename A>
Present<A> getRec(std::future<A> fA) {
	return getRec(fA.get());
}

// unpack futures
template<
	typename F, typename ...A, typename B = typename std::result_of<F(A...)>::type,
	typename = typename std::enable_if<!std::is_void<B>::value>::type
>
static std::future<Present<B>> then_imp_imp(F&& handler, std::future<A>&... f) {
	return std::async([handler](std::future<A>&... f) {
		return getRec(handler(f.get()...));
	}, std::move(f)...);
}
// unpack futures (void specialization)
template<
	typename F, typename ...A, typename B = typename std::result_of<F(A...)>::type,
	typename = typename std::enable_if<std::is_void<B>::value>::type
>
static std::future<void> then_imp_imp(F&& handler, std::future<A>&... f) {
	return std::async([handler](std::future<A>&... f) {
		handler(f.get()...);
	}, std::move(f)...);
}

// unpack args at given indices
template<typename F, size_t ...Indices, typename ...A>
static auto then_imp(F&& handler, pack_indices<Indices...>, A&&... args) {
	return then_imp_imp(handler, get<Indices>(args...)...);
}

template<
	typename ...All,
	size_t N = sizeof...(All),
	typename Fun = typename pack_element<N - 1, All...>::type,
	typename Range = typename make_pack_indices<N - 1>::type
>
auto then(All&... all) {
	Range indices;
	return then_imp(get<N - 1>(all...), indices, all...);
}

//

template<typename A>
std::future<std::vector<A>> all(std::vector<std::future<A>> & futures) {
	return std::async([](std::vector<std::future<A>> futures) {
		std::vector<A> v;
		v.reserve(futures.size());
		for (auto & f : futures) {
			v.push_back(f.get());
		}
		return v;
	}, std::move(futures));
}

