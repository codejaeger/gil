//
// Copyright 2019 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#ifndef BOOST_GIL_EXTENSION_HISTOGRAM_BOOST_HISTOGRAM_HPP
#define BOOST_GIL_EXTENSION_HISTOGRAM_BOOST_HISTOGRAM_HPP

#include <boost/gil/concepts/concept_check.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/histogram.hpp>
#include <boost/assert.hpp>

namespace boost{ namespace gil {

/*
    Provide overloads for new container tyoe here. Also provide overloads for utility functions
    specific to this container like normalization, sum, mean etc.
*/


// Overload for image to histogram imgHist function
template<typename SrcView, typename A, typename S>
void imgHist(SrcView const& srcview, boost::histogram::histogram<A, S> const& hist, bool accumulate)
{
    gil_function_requires<ImageViewConcept<SrcView>>();

    // For 1D Boost.Histogram -> imgHist
    // For 2D Boost.Histogram -> imgHist2
    BOOST_ASSERT(hist.rank() == 1);

    //Do something
}


// Overload for normalize function
template<typename A, typename S>
void normalize(boost::histogram::histogram<A, S> const& hist)
{
    //TODO
}


} // namespace gil

} // namespace boost
#endif