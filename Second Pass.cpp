// #include <unordered_map>
// #include <vector>
// #include <string>
// #include <memory>
// #include <conio.h>
// #include <iostream>
// #include <algorithm>
// #include <ppl.h>
// #include <array>
// #include <tuple>
// #include <concurrent_vector.h>
// #include <windows.h> // For GetTickCount
// #include <iterator>
// #include <ctime>
// #include <cstdlib>
// #include <random>
// #include <cmath>
// #include <limits>
// #include <utility>
// 
// using namespace concurrency;
// using namespace std;
// 
// // Calls the provided work function and returns the number of milliseconds  
// // that it takes to call that function. 
// template <class Function>
// __int64 time_call(Function&& f)
// {
//     __int64 begin = GetTickCount();
//     f();
//     return GetTickCount() - begin;
// }
// 
// class Generator
// {
// public:
//     template <typename T>
//     std::vector<T> CreateVec()
//     {
//         const auto size = static_cast<size_t>(_dist(_randomizer) % 100000 + 1);
// 
//         const auto rangeGenerator = [&]()
//         {
//             const auto a = _dist(_randomizer);
//             const auto b = _dist(_randomizer);
// 
//             return a < b ? Interval(a, b) : Interval(b, a);
//         };
// 
//         auto vec = std::vector<T>();
//         vec.reserve(size);
//         std::generate_n(std::back_inserter(vec), size, rangeGenerator);
// 
//         return vec;
//     }
// 
// private:
//     std::random_device _rd;
//     std::mt19937_64 _randomizer = std::mt19937_64(_rd());
//     std::uniform_int_distribution<int64_t> _dist = std::uniform_int_distribution<int64_t>(0, (std::numeric_limits<int64_t>::max)());
// };
// 
// class Interval
// {
// public:
//     Interval(int64_t start, int64_t end)
//         : _start(start)
//         , _end(end)
//     {
//     }
// 
//     int64_t GetStart() const
//     {
//         return _start;
//     }
// 
//     int64_t GetEnd() const
//     {
//         return _end;
//     }
// 
// private:
//     int64_t _start;
//     int64_t _end;
// };
// 
// class Foo
// {
// public:
//     Foo(Interval range) : _range(std::move(range))
//     {
//     }
// 
//     const Interval& GetInterval() const
//     {
//         return _range;
//     }
// 
// private:
//     Interval _range;
// };
// 
// class Bar
// {
// public:
//     Bar(Interval range) : _range(std::move(range))
//     {
//     }
// 
//     const Interval& GetInterval() const
//     {
//         return _range;
//     }
// 
// private:
//     Interval _range;
// };
// 
// class Baz
// {
// public:
//     Baz(Interval range) : _range(std::move(range))
//     {
//     }
// 
//     const Interval& GetInterval() const
//     {
//         return _range;
//     }
// 
// private:
//     Interval _range;
// };
// 
// class Quux
// {
// public:
//     Quux(Interval range) : _range(std::move(range))
//     {
//     }
// 
//     const Interval& GetInterval() const
//     {
//         return _range;
//     }
// 
// private:
//     Interval _range;
// };
// 
// class IntervalLessThan
// {
// public:
//     IntervalLessThan(std::function<int64_t(const Interval&)> timeGetter) : _timeGetter(timeGetter)
//     {
//     }
// 
//     template <typename T>
//     bool operator()(const T& left, const T& right)
//     {
//         return _timeGetter(left.GetInterval()) < _timeGetter(right.GetInterval());
//     }
// 
// private:
//     std::function<int64_t(const Interval&)> _timeGetter;
// };
// 
// class IntervalStartLessThan
// {
// public:
//     template <typename T>
//     bool operator()(const T& left, const T& right)
//     {
//         return lt(left, right);
//     }
// 
// private:
//     std::function<int64_t(const Interval&)> _startGetter = [](const Interval& r)
//     {
//         return r.GetStart();
//     };
// 
//     IntervalLessThan lt = IntervalLessThan(_startGetter);
// };
// 
// class IntervalEndLessThan
// {
// public:
//     template <typename T>
//     bool operator()(const T& left, const T& right)
//     {
//         return lt(left, right);
//     }
// 
// private:
//     std::function<int64_t(const Interval&)> _endGetter = [](const Interval& r)
//     {
//         return r.GetEnd();
//     };
// 
//     IntervalLessThan lt = IntervalLessThan(_endGetter);
// };
// 
// void main()
// {
//     auto gen = Generator();
// 
//     const auto foos = gen.CreateVec<Foo>();
//     const auto bars = gen.CreateVec<Bar>();
//     const auto bazs = gen.CreateVec<Baz>();
//     const auto quuxs = gen.CreateVec<Quux>();
// 
//     int64_t min;
//     int64_t max;
// 
//     const auto elapsed = time_call([&]()
//     {
//         const auto minStarts =
//         {
//             std::min_element(foos.begin(), foos.end(), IntervalStartLessThan())->GetInterval().GetStart(),
//             std::min_element(bars.begin(), bars.end(), IntervalStartLessThan())->GetInterval().GetStart(),
//             std::min_element(bazs.begin(), bazs.end(), IntervalStartLessThan())->GetInterval().GetStart(),
//             std::min_element(quuxs.begin(), quuxs.end(), IntervalStartLessThan())->GetInterval().GetStart()
//         };
// 
//         const auto maxEnds =
//         {
//             std::max_element(foos.begin(), foos.end(), IntervalEndLessThan())->GetInterval().GetEnd(),
//             std::max_element(bars.begin(), bars.end(), IntervalEndLessThan())->GetInterval().GetEnd(),
//             std::max_element(bazs.begin(), bazs.end(), IntervalEndLessThan())->GetInterval().GetEnd(),
//             std::max_element(quuxs.begin(), quuxs.end(), IntervalEndLessThan())->GetInterval().GetEnd()
//         };
// 
//         min = *std::min_element(minStarts.begin(), minStarts.end());
//         max = *std::max_element(maxEnds.begin(), maxEnds.end());
//     });
// 
//     wcout << "min: " << min;
//     wcout << endl;
//     wcout << "max: " << max;
//     wcout << endl << endl;
//     wcout << "elapsed: " << elapsed << " ms";
// 
//     _getch();
// }
