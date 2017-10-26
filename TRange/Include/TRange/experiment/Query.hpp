#pragma once
#include<vector>
#include<algorithm>
#include<numeric>
#include<functional>
#include<unordered_map>
#include"../Concept.hpp"
namespace query
{

	const auto _default = [](auto&& v) {return v; };
	const auto _pred_default = [](auto&& v)->bool {return true; };

	namespace algorithm
	{
		template<class Range>
		using range_value_t = typename std::iterator_traits<decltype(std::begin(std::declval<Range&>()))>::value_type;

		template<class Range>
		auto max(const Range& range)
		{
			return *std::max_element(std::begin(range), std::end(range));
		}

		template<class Range>
		auto min(const Range& range)
		{
			return *std::min_element(std::begin(range), std::end(range));
		}

		template<class Range>
		auto sum(const Range& range)
		{
			return std::accumulate(std::begin(range), std::end(range), 0);
		}

		template<class Range>
		auto average(const Range& range)
		{
			return algorithm::sum(range) / static_cast<double>(std::size(range));
		}

		template<class Range>
		auto count(const Range& range)
		{
			return std::size(range);
		}

		template<class Range, class Type, class Pred>
		auto aggregate(const Range& range, const Type& init, Pred pred)
		{
			Type result = init;
			for (auto&& elm : range)
			{
				result = pred(result, elm);
			}
			return 	result;
		}

		template<class Range, class Pred>
		bool allOf(const Range& range, Pred pred)
		{
			return std::all_of(std::begin(range), std::end(range), pred);
		}

		template<class Range, class Pred>
		bool anyOf(const Range& range, Pred pred)
		{
			return std::any_of(std::begin(range), std::end(range), pred);
		}

		template<class Range, class Pred>
		bool noneOf(const Range& range, Pred pred)
		{
			return std::none_of(std::begin(range), std::end(range), pred);
		}

		template<class Range, class Type = range_value_t<Range>>
		bool contains(const Range& range, const Type& value)
		{
			return std::find(std::begin(range), std::end(range), value) != std::end(range);
		}

		template<class Range1, class Range2>
		bool sequenceEqual(const Range1& range, const Range2& other)
		{
			if (std::size(range) != std::size(other))
				return false;
			auto i1 = std::begin(range);
			auto i2 = std::begin(other);
			for (; i1 != std::end(range) && i2 != std::end(other); ++i1, ++i2)
			{
				if (*i1 != *i2)
				{
					return false;
				}
			}
			return true;
		}

		template<class Range>
		auto elmentAt(const Range& range, std::size_t num)->tc::where<range_value_t<Range>,
			tc::concept::RandomAccessIterator<decltype(std::begin(range))>
		>
		{
			return range[num];
		}
		template<class Range>
		auto elmentAt(const Range& range, std::size_t num)->tc::where<range_value_t<Range>,
			tc::Not<tc::concept::RandomAccessIterator<decltype(std::begin(range))>>
		>
		{
			auto it = std::begin(range);
			for (std::size_t i = 0; i < num; ++i, ++it)
			{
			}
			return *it;
		}

		template<class Range, class Pred>
		auto rangeToVector(const Range& range, Pred pred)
		{
			std::vector<range_value_t<Range>> result;
			std::copy_if(std::begin(range), std::end(range), std::back_inserter(result), pred);
			return std::move(result);
		}

		template<class Range, class Pred>
		auto first(const Range& range, Pred pred)
		{
			auto result = rangeToVector(range, pred);
			return result.at(0);
		}
		template<class Range>
		auto first(const Range& range)
		{
			return first(range, _pred_default);
		}

		template<class Range, class Pred>
		auto last(const Range& range, Pred pred)
		{
			auto result = rangeToVector(range, pred);
			return result.at(result.size() - 1);
		}
		template<class Range>
		auto last(const Range& range)
		{
			return last(range, _pred_default);
		}

		template<class Range>
		auto skip(const Range& range, std::size_t num)
		{
			std::vector<range_value_t<Range>> ret;
			ret.reserve(std::size(range) - num);

			for (std::size_t i = num; i < std::size(range); ++i)
			{
				ret.emplace_back(elmentAt(range, i));
			}
			return std::move(ret);
		}

		template<class Range, class Pred>
		auto skipWhile(const Range& range, Pred pred)
		{
			std::vector<range_value_t<Range>> ret;
			ret.reserve(std::size(range));
			bool skip = true;

			for (std::size_t i = 0; i < std::size(range); ++i)
			{
				auto&& elm = elmentAt(range, i);

				if (skip)
				{
					if (!pred(elm))
					{
						skip = false;
					}
					else
						continue;
				}

				ret.emplace_back(elm);
			}
			return std::move(ret);
		}

		template<class Range>
		auto take(const Range& range, std::size_t num)
		{
			std::vector<range_value_t<Range>> ret;

			ret.reserve(num);
			for (std::size_t i = 0; i < num&&i < std::size(range); ++i)
			{
				ret.emplace_back(elmentAt(range, i));
			}
			return std::move(ret);
		}

		template<class Range, class Pred>
		auto takeWhile(const Range& range, Pred pred)
		{
			std::vector<range_value_t<Range>> ret;
			ret.reserve(std::size(range));
			for (std::size_t i = 0; i < std::size(range); ++i)
			{
				auto&& elm = elmentAt(range, i);
				if (!pred(elm))
					break;
				ret.emplace_back(elm);
			}
			return std::move(ret);
		}

		template<class Range, class Pred>
		auto where(const Range& range, Pred pred)
		{
			std::vector<range_value_t<Range>> ret;
			ret.reserve(std::size(range));
			for (std::size_t i = 0; i < std::size(range); ++i)
			{
				auto&& elm = elmentAt(range, i);
				if (pred(elm))
				{
					ret.emplace_back(elm);
				}
			}
			return std::move(ret);
		}

		template<class Range>
		auto distinct(const Range& range)
		{
			std::vector<range_value_t<Range>> ret;
			ret.reserve(std::size(range));

			for (std::size_t i = 0; i < std::size(range); ++i)
			{
				auto&& elm = elmentAt(range, i);
				if (algorithm::noneOf(ret, [&elm](const auto& data) {return data == elm; }))
					ret.emplace_back(elm);
			}
			return std::move(ret);
		}

		template<class Range1, class Range2>
		auto combine(const Range1& range, const Range2& other)
		{
			auto ret = algorithm::distinct(range);

			for (auto&& elm : other)
			{
				if (!algorithm::contains(ret, elm))
				{
					ret.emplace_back(elm);
				}
			}
			return std::move(ret);
		}

		template<class Range1, class Range2>
		auto intersect(const Range1& range, const Range2& other)
		{
			auto ret = algorithm::distinct(range);

			return algorithm::where(ret, [&other](const auto& elm) {
				return algorithm::contains(other, elm);
			});
		}

		template<class Range1, class Range2>
		auto except(const Range1& c, const Range2& other)
		{
			auto ret = algorithm::distinct(c);
			return algorithm::where(ret, [&other](const auto& elm) {
				return !algorithm::contains(other, elm);
			});
		}

		template<class Range, class Pred>
		auto orderBy(const Range& range, Pred pred)
		{
			auto ret = rangeToVector(range, _pred_default);
			std::sort(ret.begin(), ret.end(),
				[&pred](const auto& lhs, const auto& rhs)
			{
				return pred(lhs) < pred(rhs);
			});
			return std::move(ret);
		}
		template<class Range>
		auto orderBy(const Range& range)
		{
			return orderBy(range, _default);
		}
		template<class Range, class Pred>
		auto orderByDescending(const Range& range, Pred pred)
		{
			auto ret = rangeToVector(range, _pred_default);
			std::sort(ret.begin(), ret.end(),
				[&pred](const auto& lhs, const auto& rhs)
			{
				return pred(lhs) > pred(rhs);
			});
			return std::move(ret);
		}
		template<class Range>
		auto orderByDescending(const Range& range)
		{
			return orderByDescending(range, _default);
		}
		template<class Range>
		auto reverse(const Range& range)
		{
			auto ret = rangeToVector(range, _default);
			std::reverse(std::begin(ret), std::end(ret));
			return std::move(ret);
		}
		namespace detail
		{
			template<class Func, class Args, class = void>
			struct SelectTest {
				using normal_type = decltype(std::declval<Func>()(std::declval<Args>()));
			};
			template<class Func, class F, class S>
			struct SelectTest<Func, std::pair<F, S>, std::void_t<std::invoke_result_t<Func, F, S>>>
			{
				using pair_type = decltype(std::declval<Func>()(std::declval<F>(), std::declval<S>()));
			};
			template<class Func, class ...Args>
			struct SelectTest<Func, std::tuple<Args...>, std::void_t<std::invoke_result_t<Func, Args...>>>
			{
				using tuple_type = decltype(std::declval<Func>()(std::declval<Args>()...));
			};

			template<class Range, class Func>
			using select_result = SelectTest<Func, range_value_t<Range>>;

			template<class Func, class... TupleArgs, class... Args>
			auto tuple_pack(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
				std::enable_if_t<sizeof...(Args) == sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...>>
			{
				return func(args...);
			}
			template<class Func, class... TupleArgs, class... Args>
			auto tuple_pack(Func func, std::tuple<TupleArgs...> tuple, Args&&... args)->
				std::enable_if_t<sizeof...(Args) < sizeof...(TupleArgs), std::invoke_result_t<Func, TupleArgs...>>
			{
				return tuple_pack(func, tuple, args..., std::get<sizeof...(Args)>(tuple));
			}
		}

		template<class Range, class Pred>
		auto select(const Range& range, Pred pred)->std::vector<typename  detail::select_result<Range, Pred>::normal_type>
		{
			using value_type = typename  detail::select_result<Range, Pred>::normal_type;
			std::vector<value_type> ret;
			ret.reserve(std::size(range));
			for (auto&& elm : range)
			{
				ret.emplace_back(pred(elm));
			}
			return std::move(ret);
		}
		template<class Range, class Pred>
		auto select(const Range& range, Pred pred)->std::vector<typename  detail::select_result<Range, Pred>::pair_type>
		{
			using value_type = typename  detail::select_result<Range, Pred>::pair_type;
			std::vector<value_type> ret;
			ret.reserve(std::size(range));
			for (auto&& elm : range)
			{
				ret.emplace_back(pred(elm.first, elm.second));
			}
			return std::move(ret);
		}

		template<class Range, class Pred>
		auto select(const Range& range, Pred pred)->std::vector<typename  detail::select_result<Range, Pred>::tuple_type>
		{
			using value_type = typename  detail::select_result<Range, Pred>::tuple_type;
			std::vector<value_type> ret;
			ret.reserve(std::size(range));
			for (auto&& elm : range)
			{
				ret.emplace_back(detail::tuple_pack(pred,elm));
			}
			return std::move(ret);
		}

		template<class Range, class Pred>
		auto selectMany(const Range& range, Pred pred)
		{
			using range_t = range_value_t<Range>;
			using value_type = range_value_t<decltype(pred(std::declval<range_t&>()))>;
			std::vector<value_type> ret;
			for (auto&& elm : range)
			{
				for (auto&& each : pred(elm))
				{
					ret.emplace_back(each);
				}
			}
			return std::move(ret);
		}

		template<class Range, class Pred>
		auto groupBy(const Range& range, Pred pred)
		{
			using source_t = range_value_t<Range>;
			using key_t = decltype(pred(std::declval<source_t&>()));

			std::unordered_map<key_t, std::vector<source_t>> ret;

			for (auto&& elm : range)
			{
				auto key = pred(elm);
				if (ret.count(key) == 0)
				{
					ret[key] = std::vector<source_t>{};
				}
				ret[key].emplace_back(elm);
			}
			return std::move(ret);
		}
		template<class Range1, class Range2, class Pred>
		auto zip(const Range1& range, const Range2& other, Pred pred)
		{
			auto i1 = std::begin(range);
			auto i2 = std::begin(other);
			using value_type = decltype(pred(*i1, *i2));
			std::vector<value_type> ret;
			ret.reserve(std::min(std::size(range), std::size(other)));

			for (; i1 != std::end(range) && i2 != std::end(other); ++i1, ++i2)
			{
				ret.emplace_back(pred(*i1, *i2));
			}
			return std::move(ret);
		}
		template<class OuterRange, class OuterKey, class InnerRange, class InnerKey, class Result>
		auto join(const OuterRange& outer, OuterKey outerKey, const InnerRange& inner, InnerKey innerKey, Result result)
		{
			std::vector<std::pair<range_value_t<OuterRange>, range_value_t<InnerRange>>> v;
			for (const auto& o : outer)
			{
				auto&& ok=outerKey(o);
				for (const auto& i : inner)
				{
					auto&& ik = innerKey(i);

					if (ok == ik)
					{
						v.emplace_back(o, i);
					}

				}
			}
			return algorithm::select(v,result);
		}
	}
	namespace detail
	{
		template<class Func, class Sort>
		struct IsSortFunc
		{};
		template<class Func, class Sort>
		struct SortFunc
		{
			Func get;
			Sort sortFunc;
			template<class T>
			auto operator()(T&& v)const
			{
				auto c = get(v);

				return sortFunc(c);
			}
		};

		template<class Func>
		class QueryBase;

		template<class Func>
		QueryBase<Func> create(Func);

		template<class Func>
		class QueryBase
		{
		protected:
			Func get;
		public:
			QueryBase(Func func) :
				get(func)
			{}


			template<class Range>
			auto operator()(const Range& range)
			{
				return get(range);
			}

			///<summary>
			///指定した位置にある要素を返します
			///</summary>
			auto elmentAt(std::size_t num)const
			{
				auto f = [num, this](auto&& v)
				{
					auto c = get(v);

					return c.at(num);
				};
				return create(f);
			}

			///<summary>
			///指定した位置にある要素を返します
			///</summary>
			template<class Pred>
			auto first(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::first(c, pred);
				};
				return create(f);
			}
			auto first()const
			{
				return this->first(_pred_default);
			}

			///<summary>
			///指定した位置にある要素を返します
			///</summary>
			template<class Pred>
			auto last(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::last(c, pred);
				};
				return create(f);
			}
			auto last()const
			{
				return this->last(_pred_default);
			}

			///<summary>
			///先頭から要素のスキップ
			///</summary>
			auto skip(std::size_t num)const
			{
				auto f = [num, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::skip(c, num);
				};
				return create(f);
			}

			///<summary>
			///先頭から条件をみたさなくなるまでの要素をスキップ
			///</summary>
			template<class Pred>
			auto skipWhile(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);

					return algorithm::skipWhile(c, pred);
				};
				return create(f);
			}

			///<summary>
			///先頭から要素をとりだす
			///</summary>
			auto take(std::size_t num)const
			{
				auto f = [num, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::take(c, num);
				};
				return create(f);
			}

			///<summary>
			///先頭から条件をみたさなくなるまでの要素をとりだす
			///</summary>
			template<class Pred>
			auto takeWhile(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::takeWhile(c, pred);
				};
				return create(f);
			}

			///<summary>
			///条件をみたす要素をすべてとりだす
			///</summary>
			template<class Pred>
			auto where(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::where(c, pred);
				};
				return create(f);
			}

			///<summary>
			///重複する要素を削除
			///</summary>
			auto distinct()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return algorithm::distinct(c);
				};
				return create(f);
			}

			///<summary>
			///最大値を求める
			///</summary>
			auto max()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return algorithm::max(c);
				};
				return create(f);
			}

			///<summary>
			///最小値を求める
			///</summary>
			auto min()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return algorithm::min(c);
				};
				return create(f);
			}

			///<summary>
			///アキュムレータ関数で処理した結果を求める
			///</summary>
			template<class Type, class Pred>
			auto aggregate(const Type& init, Pred pred)const
			{
				auto f = [init, pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::aggregate(c, init, pred);
				};
				return create(f);
			}

			///<summary>
			///総和を求める
			///</summary>
			auto sum()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return algorithm::sum(c);
				};
				return create(f);
			}

			///<summary>
			///総乗を求める
			///</summary>
			auto product()const
			{
				return this->aggregate(1, [](const auto& l, const auto& r) {return l*r; });
			}

			///<summary>
			///平均を求める
			///</summary>
			auto average()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return algorithm::average(c);
				};
				return create(f);
			}

			///<summary>
			///要素数を取得
			///</summary>
			auto count()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return c.size();
				};
				return create(f);
			}


			///<summary>
			///すべての要素が条件を満たすか
			///</summary>
			template<class Pred>
			auto all(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::allOf(c, pred);
				};
				return create(f);
			}

			///<summary>
			///いずれかの要素が条件を満たすか
			///</summary>
			template<class Pred>
			auto any(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::anyOf(c, pred);
				};
				return create(f);
			}

			///<summary>
			////条件を満たす要素が一つもないか
			///</summary>
			template<class Pred>
			auto none(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::noneOf(c, pred);
				};
				return create(f);
			}

			///<summary>
			///指定した要素を含むか
			///</summary>
			template<class Type>
			auto contains(const Type& value)const
			{
				auto f = [value, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::contains(c, value);
				};
				return create(f);
			}

			///<summary>
			///2つのシーケンスが等しいか
			///</summary>
			template<class Range>
			auto sequenceEqual(const Range& other)const
			{
				auto f = [other, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::sequenceEqual(c, other);
				};
				return create(f);
			}
			template<class Type>
			auto sequenceEqual(const std::initializer_list<Type>& other)const
			{
				return this->sequenceEqual<std::initializer_list<Type>>(other);
			}

			///<summary>
			///和集合
			///</summary>
			template<class List>
			auto combine(const List& other)const
			{
				auto f = [other, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::combine(c, other);
				};
				return create(f);
			}
			template<class Type>
			auto combine(const std::initializer_list<Type>& other)const
			{
				return this->combine<std::initializer_list<Type>>(other);
			}

			///<summary>
			///積集合
			///</summary>
			template<class Range>
			auto intersect(const Range& other)const
			{
				auto f = [other, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::intersect(c, other);
				};
				return create(f);
			}
			template<class Type>
			auto intersect(const std::initializer_list<Type>& other)const
			{
				return this->intersect<std::initializer_list<Type>>(other);
			}

			///<summary>
			///差集合
			///</summary>
			template<class Range>
			auto except(const Range& other)const
			{
				auto f = [other, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::except(c, other);
				};
				return create(f);
			}
			template<class Type>
			auto except(const std::initializer_list<Type>& other)const
			{
				return this->except<std::initializer_list<Type>>(other);
			}

			///<summary>
			///昇順にソートしたシーケンスを返す
			///</summary>
			template<class Pred>
			auto orderBy(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					return algorithm::orderBy(v, pred);
				};
				return create(get, f);
			}

			///<summary>
			///降順にソートしたシーケンスを返す
			///</summary>
			template<class Pred>
			auto orderByDescending(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					return algorithm::orderByDescending(v, pred);
				};
				return create(get, f);
			}

			///<summary>
			///逆順にソートしたシーケンスを返す
			///</summary>
			auto reverse()const
			{
				auto f = [this](auto&& v)
				{
					auto c = get(v);
					return algorithm::reverse(c);
				};
				return create(f);
			}

			///<summary>
			///シーケンスの各要素を新しいフォームに射影
			///</summary>
			template<class Pred>
			auto select(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::select(c, pred);
				};
				return create(f);
			}
			///<summary>
			///シーケンスの各要素を複数の要素に射影し、平坦化
			///</summary>
			template<class Pred>
			auto selectMany(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::selectMany(c, pred);
				};
				return create(f);
			}

			///<summary>
			///指定のキーで要素をグループ化する
			///</summary>
			template<class Pred>
			auto groupBy(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::groupBy(c, pred);
				};
				return create(f);
			}

			///<summary>
			///2つのシーケンスを1つのシーケンスにマージする
			///</summary>
			template<class Range, class Pred>
			auto zip(const Range& range, Pred pred)const
			{
				auto f = [range, pred, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::zip(c, range, pred);
				};
				return create(f);
			}
			template<class Type, class Pred>
			auto zip(const std::initializer_list<Type>& list, Pred pred)const
			{
				return this->zip<std::initializer_list<Type>>(list, pred);
			}

			///<summary>
			///内部結合
			///</summary>
			template<class OuterKey, class InnerRange, class InnerKey, class Result>
			auto join(OuterKey outerKey, const InnerRange& inner, InnerKey innerKey, Result result)const
			{
				auto f = [outerKey, inner, innerKey,result, this](auto&& v)
				{
					auto c = get(v);
					return algorithm::join(c,outerKey, inner, innerKey, result);
				};
				return create(f);
			}
		};

		template<class Func, class Sort>
		class QueryBase<IsSortFunc<Func, Sort>> : public QueryBase<SortFunc<Func, Sort>>
		{
		public:
			using QueryBase<SortFunc<Func, Sort>>::QueryBase;

			QueryBase(const Func& func, const Sort& sort) :
				QueryBase(SortFunc<Func, Sort>{func, sort})
			{}
			///<summary>
			///ソートしたシーケンスに対しキーが等しい要素同士を昇順にソート
			///</summary>
			template<class Pred>
			auto thenBy(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = algorithm::orderBy(v, pred);
					return get.sortFunc(c);
				};
				return create(get.get, f);
			}
			///<summary>
			///ソートしたシーケンスに対しキーが等しい要素同士を降順にソート
			///</summary>
			template<class Pred>
			auto thenByDescending(Pred pred)const
			{
				auto f = [pred, this](auto&& v)
				{
					auto c = algorithm::orderByDescending(v, pred);
					return get.sortFunc(c);
				};
				return create(get.get, f);
			}

		};

		template<class Func>
		QueryBase<Func> create(Func func)
		{
			return QueryBase<Func>(func);
		}

		template<class Func, class Sort>
		QueryBase<IsSortFunc<Func, Sort>> create(Func func, Sort sort)
		{
			return QueryBase<IsSortFunc<Func, Sort>>(func, sort);
		}
	}

	const auto from = detail::create(query::_default);

}
