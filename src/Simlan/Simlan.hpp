// Simlan.hpp 
#pragma Simlan
// Simlan programme 
/*
* @author: Gusem Fowage 
* @email: <cdsocjpw@163.com>
**/
#ifndef SIMLAN_HPP
#define SIMLAN_HPP

#define __SIMLAN__

#include <filesystem>

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>

#include <string>
#include <list>
#include <queue>
#include <vector>

#include <memory>
#include <exception>
#include <limits>

namespace Simlan {
    // basic types
    typedef char            char_t;
    typedef int             int_t;
    typedef unsigned int    uint_t;
    // filesystem
    using Path = ::std::filesystem::path;
    // memory
    template<typename T>
    using shared = ::std::shared_ptr<T>;
    using ::std::make_shared;
    using ::std::static_pointer_cast;
    template<typename T>
    using unique = ::std::unique_ptr<T>;
    using ::std::make_unique;
    // string
    using string = ::std::basic_string<char_t>;
    using string_view = ::std::basic_string_view<char_t>;
    using ::std::getline;
    // list
    template<typename T>
    using List = ::std::list<T>;
    // queue
    template<typename T>
    using Queue = ::std::queue<T>;
    // vector
    template<typename T>
    using Vector = ::std::vector<T>;
    // map
    template<typename K, typename V, typename C = std::less<K>>
    using Map = ::std::map<K, V, C>;
    template<typename K, typename V>
    using UnorderedMap = ::std::unordered_map<K, V>;
    // set
    template<typename T>
    using Set = ::std::set<T>;
    template<typename T>
    using UnorderedSet = ::std::unordered_set<T>;
    // exception
    using Exception = ::std::exception;
    // std
    using ::std::move;
    using ::std::numeric_limits;

    using size_t = unsigned long long;
}

constexpr bool DEBUG_MODE = true;

#endif
