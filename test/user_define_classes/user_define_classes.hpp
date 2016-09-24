#ifndef _USER_DEFINE_CLASSES_H
#define _USER_DEFINE_CLASSES_H

#ifdef ENABLE_BOOST_SERIALIZATION
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/stack.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/hash_map.hpp>
#include <boost/serialization/hash_set.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/boost_unordered_map.hpp>
#include <boost/serialization/boost_unordered_set.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#endif

#ifdef ENABLE_MSGPACK
#include <msgpack.hpp>
#endif

#ifdef ENABLE_JSON
#include <kapok/Kapok.hpp>
#endif

struct person_info_req
{
    int card_id;
    std::string name;

#ifdef ENABLE_BOOST_SERIALIZATION
    template<class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & card_id;
        ar & name;
    }
#endif

#ifdef ENABLE_MSGPACK
    MSGPACK_DEFINE(card_id, name);
#endif

#ifdef ENABLE_JSON
    META(card_id, name);
#endif
};

struct person_info_res 
{
    int card_id;
    std::string name;
    int age;
    std::string national;

#ifdef ENABLE_BOOST_SERIALIZATION
    template<class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & card_id;
        ar & name;
        ar & age;
        ar & national;
    }
#endif
    
#ifdef ENABLE_MSGPACK
    MSGPACK_DEFINE(card_id, name, age, national);
#endif

#ifdef ENABLE_JSON
    META(card_id, name, age, national);
#endif
};

#endif
