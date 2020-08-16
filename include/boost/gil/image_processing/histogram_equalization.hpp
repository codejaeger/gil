//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_GIL_IMAGE_PROCESSING_HISTOGRAM_EQUALIZATION_HPP
#define BOOST_GIL_IMAGE_PROCESSING_HISTOGRAM_EQUALIZATION_HPP

#include <boost/gil/image.hpp>
#include <boost/gil/histogram.hpp>

// #include <iostream>
// #include <utility>
#include <map>
#include <vector>
#include <cmath>

namespace boost { namespace gil {

/*
    Algoithm :-
    1. If histogram A is to be equalized compute the cumulative histogram of A.
    2. Let CFD(A) refer to the cumulative histogram of A
    3. For a uniform histogram A', CDF(A') = A'
    4. We need to transfrom A to A' such that
    5. CDF(A') = CDF(A) => A' = CDF(A)
    6. Hence the pixel transform , px => histogram_of_ith_channel[px].
    TODO: Extend for bit aligned images
*/

template <typename SrcKeyType>
std::map<SrcKeyType, SrcKeyType> histogram_equalization(
    histogram<SrcKeyType> const& src_hist)
{
    histogram<SrcKeyType> dst_hist;
    return histogram_equalization(src_hist, dst_hist);
}

template <typename SrcKeyType, typename DstKeyType>
std::map<SrcKeyType, DstKeyType> histogram_equalization(
    histogram<SrcKeyType> const& src_hist,
    histogram<DstKeyType>& dst_hist)
{
    static_assert(
        std::is_integral<SrcKeyType>::value &&
        std::is_integral<DstKeyType>::value,
        "Source and destination histogram types are not appropriate");

    using value_t = typename histogram<SrcKeyType>::value_type;
    dst_hist.clear();
    double sum = src_hist.sum();
    // Use when setlimits are added to the algorithm
    // SrcKeyType min_key = std::get<0>(src_hist.min_key());
    // SrcKeyType max_key = std::get<0>(src_hist.max_key());
    SrcKeyType min_key = std::numeric_limits<SrcKeyType>::min();
    SrcKeyType max_key = std::numeric_limits<SrcKeyType>::max();
    auto cumltv_srchist = cumulative_histogram(src_hist);
    std::map<SrcKeyType, DstKeyType> color_map;
    std::for_each(cumltv_srchist.begin(), cumltv_srchist.end(), [&](value_t const& v) {
        DstKeyType trnsfrmd_key = static_cast<DstKeyType>(round(v.second / sum * (max_key - min_key)) + min_key);
        color_map[std::get<0>(v.first)] = trnsfrmd_key;
    });
    std::for_each(src_hist.begin(), src_hist.end(), [&](value_t const& v) {
        dst_hist[color_map[std::get<0>(v.first)]] += v.second;
    });
    return color_map;
}


template <typename SrcView, typename DstView>
void histogram_equalization(
    SrcView const& src_view,
    DstView const& dst_view,
    std::size_t bin_width = 1,
    bool mask = false,
    std::vector<std::vector<bool>> src_mask = {})
{
    gil_function_requires<ImageViewConcept<SrcView>>();
    gil_function_requires<MutableImageViewConcept<DstView>>();
    
    // static_assert(
    //     src_view.dimensions() == dst_view.dimensions(),
    //     "Source and destination views must have same dimensions.");

    static_assert(
        color_spaces_are_compatible<
            typename color_space_type<SrcView>::type,
            typename color_space_type<DstView>::type>::value,
        "Source and destination views must have same color space");
    
    // Defining channel type
    using source_channel_t = typename channel_type<SrcView>::type;
    using dst_channel_t    = typename channel_type<DstView>::type;
    using coord_t          = typename SrcView::x_coord_t;

    std::size_t const channels = num_channels<SrcView>::value;
    coord_t const width        = src_view.width();
    coord_t const height       = src_view.height();
    std::size_t pixel_max      = std::numeric_limits<dst_channel_t>::max();

    for (std::size_t i = 0; i < channels; i++)
    {
        histogram<source_channel_t> h;
        fill_histogram(nth_channel_view(src_view, i), h, bin_width, false, false, mask, src_mask);
        h.normalize();
        auto h2 = cumulative_histogram(h);
        // auto color_map = histogram_equalization(h);
        // for(auto it:h2)
        // {
        //     std::cout<<int(std::get<0>(it.first))<<" "<<it.second<<std::endl;
        // }
        for (std::ptrdiff_t src_y = 0; src_y < height; ++src_y)
        {
            auto src_it = nth_channel_view(src_view, i).row_begin(src_y);
            auto dst_it = nth_channel_view(dst_view, i).row_begin(src_y);
            for (std::ptrdiff_t src_x = 0; src_x < width; ++src_x)
            {
                if (mask && !src_mask[src_y][src_x])
                    dst_it[src_x][0] = src_it[src_x][0];
                else
                    dst_it[src_x][0] = static_cast<dst_channel_t>(
                        round(h2[src_it[src_x][0]] * pixel_max));
                    // dst_it[src_x][0] = color_map[src_it[src_x][0]];
            }
        }
    }
}

}}  //namespace boost::gil

#endif
