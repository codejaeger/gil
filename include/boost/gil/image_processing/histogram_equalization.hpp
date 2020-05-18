//
// Copyright 2019 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#ifndef BOOST_GIL_EXTENSION_HISTOGRAM_HISTOGRAM_EQUALIZATION_HPP
#define BOOST_GIL_EXTENSION_HISTOGRAM_HISTOGRAM_EQUALIZATION_HPP

#include <boost/gil/histogram.hpp>
#include <boost/gil/extension/histogram/histogram_operations.hpp>

namespace boost { namespace gil {

/*
    Use a default container type (providing best performance), for all image processing histogram types.
*/


template <typename Container = std::vector<int, std::allocator<int> > >
struct histogram_equalization {
    template <typename SrcView, typename DstView>
    void operator()(SrcView const& srcview, DstView const& dstview) const {
    gil_function_requires<ImageViewConcept<SrcView>>();
    gil_function_requires<MutableImageViewConcept<DstView>>();
    static_assert(color_spaces_are_compatible
    <
        typename color_space_type<SrcView>::type,
        typename color_space_type<DstView>::type
    >::value, "Source and destination views must have same color space");
    
    // call imgHist overload for container
    Container hist;
    imgHist(srcview, hist);
    
    // Call overload for normalize
    normalize(hist); 

    //TODO
    }
};

} // namespace boost

} // namespace gil

#endif