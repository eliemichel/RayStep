#pragma once

// Many thx to https://stackoverflow.com/questions/5484930/split-variadic-template-arguments

// make_pack_indices<5, 0>::type is actually the type pack_indices<0, 1, 2, 3, 4>

template <size_t...>
struct pack_indices {};

template <size_t Sp, class IntPack, size_t Ep>
struct make_indices_imp;

template <size_t Sp, size_t ... Indices, size_t Ep>
struct make_indices_imp<Sp, pack_indices<Indices...>, Ep>
{
	typedef typename make_indices_imp<Sp + 1, pack_indices<Indices..., Sp>, Ep>::type type;
};

template <size_t Ep, size_t ... Indices>
struct make_indices_imp<Ep, pack_indices<Indices...>, Ep>
{
	typedef pack_indices<Indices...> type;
};

template <size_t Ep, size_t Sp = 0>
struct make_pack_indices
{
	static_assert(Sp <= Ep, "__make_tuple_indices input error");
	typedef typename make_indices_imp<Sp, pack_indices<>, Ep>::type type;
};

// pack_element<N, Ts...>::type is the Nth type of the parameters pack

template <size_t _Ip, class _Tp>
class pack_element_imp;

template <class ..._Tp>
struct pack_types {};

template <size_t Ip>
class pack_element_imp<Ip, pack_types<> >
{
public:
	static_assert(Ip == 0, "tuple_element index out of range");
	static_assert(Ip != 0, "tuple_element index out of range");
};

template <class Hp, class ...Tp>
class pack_element_imp<0, pack_types<Hp, Tp...> >
{
public:
	typedef Hp type;
};

template <size_t Ip, class Hp, class ...Tp>
class pack_element_imp<Ip, pack_types<Hp, Tp...> >
{
public:
	typedef typename pack_element_imp<Ip - 1, pack_types<Tp...> >::type type;
};

template <size_t Ip, class ...Tp>
class pack_element
{
public:
	typedef typename pack_element_imp<Ip, pack_types<Tp...> >::type type;
};

// get<N>(ts...) return the Nth element of a parameters pack.

template <class R, size_t Ip, size_t Ij, class... Tp>
struct Get_impl
{
	static R& dispatch(Tp...);
};

template<class R, size_t Ip, size_t Jp, class Head, class... Tp>
struct Get_impl<R, Ip, Jp, Head, Tp...>
{
	static R& dispatch(Head& h, Tp&... tps)
	{
		return Get_impl<R, Ip, Jp + 1, Tp...>::dispatch(tps...);
	}
};

template<size_t Ip, class Head, class... Tp>
struct Get_impl<Head, Ip, Ip, Head, Tp...>
{
	static Head& dispatch(Head& h, Tp&... tps)
	{
		return h;
	}
};


template <size_t Ip, class ... Tp>
typename pack_element<Ip, Tp...>::type&
get(Tp&... tps)
{
	return Get_impl<typename pack_element<Ip, Tp...>::type, Ip, 0, Tp...>::dispatch(tps...);
}
