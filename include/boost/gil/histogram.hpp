//
// Copyright 2019 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#ifndef BOOST_GIL_EXTENSION_HISTOGRAM_HISTOGRAM_HPP
#define BOOST_GIL_EXTENSION_HISTOGRAM_HISTOGRAM_HPP

#include <boost/gil/concepts/concept_check.hpp>
#include <boost/gil/image_view.hpp>

#include <boost/concept_check.hpp>

#include <type_traits>
#include <vector>

namespace boost { namespace gil {

// For 1D histogram -> imgHist
// For 2D histograms -> imgHist2
// Reason - The calling syntax for 1D and 2D case would vary significantly thus they are not overloaded.

// For random access container refer -> https://www.boost.org/sgi/stl/RandomAccessContainer.html
template<typename SrcView, typename Container>
typename std::enable_if<std::is_arithmetic<typename Container::value_type>::value>::type
imgHist(SrcView const& srcview, Container const& hist, bool accumulate = false)
{
    gil_function_requires<ImageViewConcept<SrcView>>();
    gil_function_requires<RandomAccessContainer<Container>>();
    if (!accumulate)
    {
        //clear container
    }
    //TODO
}


// For pair associative containers refer -> https://www.boost.org/sgi/stl/PairAssociativeContainer.html
template<typename SrcView, typename Container>
typename std::enable_if<
            std::is_arithmetic<typename Container::key_type>::value &&
            std::is_arithmetic<typename Container::mapped_type>::value>::type
imgHist(SrcView const& srcview, Container const& hist, bool accumulate = false)
{
    gil_function_requires<ImageViewConcept<SrcView>>();
    gil_function_requires<PairAssociativeContainer<Container>>();
    if (!accumulate)
    {
        //clear container
    }
    //TODO
}

//Provide overloaded versions for external containers. This is just an example.
template<typename SrcView,typename T>
void imgHist(SrcView const& srcview, std::vector<std::vector<T>> const& hist, bool accumulate = false)
{
    //Do something
}

} // namespace gil

} // namespace boost

#endif

