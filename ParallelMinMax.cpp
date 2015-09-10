#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <ppl.h>
#include <array>
#include <tuple>
#include <concurrent_vector.h>
#include <windows.h> // For GetTickCount
#include <iterator>
#include <ctime>
#include <cstdlib>
#include <random>
#include <cmath>
#include <limits>
#include <utility>
#include <sstream>

using namespace concurrency;
using namespace std;

class Interval
{
public:
    Interval(int64_t start, int64_t end)
        : _start(start)
        , _end(end)
    {
    }

    int64_t GetStart() const
    {
        return _start;
    }

    int64_t GetEnd() const
    {
        return _end;
    }

private:
    int64_t _start;
    int64_t _end;
};

class Foo
{
public:
    Foo(Interval range) : _range(std::move(range))
    {
    }

    const Interval& GetInterval() const
    {
        return _range;
    }

private:
    Interval _range;
};

class Bar
{
public:
    Bar(Interval range) : _range(std::move(range))
    {
    }

    const Interval& GetInterval() const
    {
        return _range;
    }

private:
    Interval _range;
};

class Baz
{
public:
    Baz(Interval range) : _range(std::move(range))
    {
    }

    const Interval& GetInterval() const
    {
        return _range;
    }

private:
    Interval _range;
};

class Quux
{
public:
    Quux(Interval range) : _range(std::move(range))
    {
    }

    const Interval& GetInterval() const
    {
        return _range;
    }

private:
    Interval _range;
};

using FooVec = std::vector < Foo > ;
using BarVec = std::vector < Bar > ;
using BazVec = std::vector < Baz > ;
using QuuxVec = std::vector < Quux > ;

template <class Function>
__int64 time_call(Function&& f)
{
    __int64 begin = GetTickCount();
    f();
    return GetTickCount() - begin;
}

class Generator
{
public:
    template <typename T>
    std::vector<T> CreateVec()
    {
        const auto size = static_cast<size_t>(_dist(_randomizer) % 10000000 + 1);

        const auto rangeGenerator = [&]()
        {
            const auto a = _dist(_randomizer);
            const auto b = _dist(_randomizer);

            return a < b ? Interval(a, b) : Interval(b, a);
        };

        auto vec = std::vector<T>();
        vec.reserve(size);
        std::generate_n(std::back_inserter(vec), size, rangeGenerator);

        return vec;
    }

private:
    std::random_device _rd;
    std::mt19937_64 _randomizer = std::mt19937_64(_rd());
    std::uniform_int_distribution<int64_t> _dist = std::uniform_int_distribution<int64_t>(0, (std::numeric_limits<int64_t>::max)());
};

class IntervalLessThan
{
public:
    IntervalLessThan(std::function<int64_t(const Interval&)> timeGetter) : _timeGetter(timeGetter)
    {
    }

    template <typename T>
    bool operator()(const T& left, const T& right)
    {
        return _timeGetter(left.GetInterval()) < _timeGetter(right.GetInterval());
    }

private:
    std::function<int64_t(const Interval&)> _timeGetter;
};

class IntervalStartLessThan
{
public:
    template <typename T>
    bool operator()(const T& left, const T& right)
    {
        return lt(left, right);
    }

private:
    std::function<int64_t(const Interval&)> _startGetter = [](const Interval& r)
    {
        return r.GetStart();
    };

    IntervalLessThan lt = IntervalLessThan(_startGetter);
};

class IntervalEndLessThan
{
public:
    template <typename T>
    bool operator()(const T& left, const T& right)
    {
        return lt(left, right);
    }

private:
    std::function<int64_t(const Interval&)> _endGetter = [](const Interval& r)
    {
        return r.GetEnd();
    };

    IntervalLessThan lt = IntervalLessThan(_endGetter);
};

template <typename T>
int64_t GetMin(const std::vector<T>& vec)
{
    return std::min_element(vec.begin(), vec.end(), IntervalStartLessThan())->GetInterval().GetStart();
};

template <typename T>
int64_t GetMax(const std::vector<T>& vec)
{
    return std::max_element(vec.begin(), vec.end(), IntervalEndLessThan())->GetInterval().GetEnd();
};

void ComputeSerial(const FooVec& foos, const BarVec& bars, const BazVec& bazs, const QuuxVec& quuxs)
{
    auto minFoo = 0ll;
    auto minBar = 0ll;
    auto minBaz = 0ll;
    auto minQuux = 0ll;

    auto maxFoo = 0ll;
    auto maxBar = 0ll;
    auto maxBaz = 0ll;
    auto maxQuux = 0ll;

    const auto elapsed = time_call([&]()
    {
        minFoo = GetMin(foos);
        minBar = GetMin(bars);
        minBaz = GetMin(bazs);
        minQuux = GetMin(quuxs);
        maxFoo = GetMax(foos);
        maxBar = GetMax(bars);
        maxBaz = GetMax(bazs);
        maxQuux = GetMax(quuxs);
    });

    const auto bounds = std::minmax({ minFoo, minBar, minBaz, minQuux, maxFoo, maxBar, maxBaz, maxQuux });

    wcout << "min: " << bounds.first << endl << "max: " << bounds.second << endl << endl << "serial: " << elapsed << " ms";
}

void ComputeParallel(const FooVec& foos, const BarVec& bars, const BazVec& bazs, const QuuxVec& quuxs)
{
    auto minFoo = 0ll;
    auto minBar = 0ll;
    auto minBaz = 0ll;
    auto minQuux = 0ll;

    auto maxFoo = 0ll;
    auto maxBar = 0ll;
    auto maxBaz = 0ll;
    auto maxQuux = 0ll;

    const auto elapsed = time_call([&]()
    {
        parallel_invoke(
            [&]{ minFoo = GetMin(foos); },
            [&]{ minBar = GetMin(bars); },
            [&]{ minBaz = GetMin(bazs); },
            [&]{ minQuux = GetMin(quuxs); },
            [&]{ maxFoo = GetMax(foos); },
            [&]{ maxBar = GetMax(bars); },
            [&]{ maxBaz = GetMax(bazs); },
            [&]{ maxQuux = GetMax(quuxs); }
        );
    });

    const auto bounds = std::minmax({ minFoo, minBar, minBaz, minQuux, maxFoo, maxBar, maxBaz, maxQuux });

    wcout << "min: " << bounds.first << endl << "max: " << bounds.second << endl << endl << "parallel: " << elapsed << " ms";
}

void main()
{
    auto generator = Generator();

    const auto foos = generator.CreateVec<Foo>();
    const auto bars = generator.CreateVec<Bar>();
    const auto bazs = generator.CreateVec<Baz>();
    const auto quuxs = generator.CreateVec<Quux>();

    ComputeSerial(foos, bars, bazs, quuxs);

    cout << endl << endl;

    ComputeParallel(foos, bars, bazs, quuxs);

    _getch();
}
