//
// Copyright 2019 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#ifndef BOOST_GIL_EXTENSION_HISTOGRAM_HISTOGRAM_OPERATIONS_HPP
#define BOOST_GIL_EXTENSION_HISTOGRAM_HISTOGRAM_OPERATIONS_HPP

#include <boost/gil/concepts/concept_check.hpp>

#include <boost/concept_check.hpp>

#include <type_traits>

namespace boost { namespace gil {

/*
    Provide histogram utilities like normalization, cumulative histogram, mean, sum etc .
    Add here only for STL containers
*/



// For the case of Random Access Containers
template<typename Container>
typename std::enable_if<std::is_arithmetic<typename Container::value_type>::value>::type
normalize(Container const& hist)
{
    gil_function_requires<RandomAccessContainer<Container>>();

    //TODO
}

// For the case of Pair Associative Containers
template<typename Container>
typename std::enable_if<
            std::is_arithmetic<typename Container::key_type>::value &&
            std::is_arithmetic<typename Container::mapped_type>::value>::type
normalize(Container const& hist)
{
    gil_function_requires<PairAssociativeContainer<Container>>();

    //TODO
}

// For the case of Random Access Containers
template<typename Container1, typename Container2>
typename std::enable_if<std::is_arithmetic<typename Container1::value_type>::value &&
                        std::is_arithmetic<typename Container2::value_type>::value>::type
normalize(Container1 const& SrcHist, Container2 const& DstHist)
{
    gil_function_requires<RandomAccessContainer<Container1>>();
    gil_function_requires<RandomAccessContainer<Container2>>();

    //TODO
}

// For the case of Pair Associative Containers
template<typename Container1, typename Container2>
typename std::enable_if<
            std::is_arithmetic<typename Container1::key_type>::value &&
            std::is_arithmetic<typename Container1::mapped_type>::value &&
            std::is_arithmetic<typename Container2::key_type>::value &&
            std::is_arithmetic<typename Container2::mapped_type>::value>::type
normalize(Container1 const& SrcHist, Container2 const& DstHist)
{
    gil_function_requires<PairAssociativeContainer<Container1>>();
    gil_function_requires<PairAssociativeContainer<Container2>>();

    //TODO
}


} // namespace gil

} // namespace boost

#endif