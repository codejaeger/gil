//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_GIL_IMAGE_PROCESSING_ADAPTIVE_HISTOGRAM_EQUALIZATION_HPP
#define BOOST_GIL_IMAGE_PROCESSING_ADAPTIVE_HISTOGRAM_EQUALIZATION_HPP

#include <boost/gil/algorithm.hpp>
#include <boost/gil/histogram.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/image_view_factory.hpp>
#include <boost/gil/image_processing/histogram_equalization_temp.hpp>

// #include <iostream>
// #include <utility>
#include <map>
#include <vector>
#include <cmath>

namespace boost { namespace gil {

template <typename SrcView, typename DstView>
void non_overlapping_interpolated_clahe(
    SrcView const& src_view,
    DstView const& dst_view,
    std::size_t tile_width_x = 8,
    std::size_t tile_width_y = 8,
    double clip_limit = 0.5,
    std::size_t bin_width = 1.0,
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
    
    using source_channel_t = typename channel_type<SrcView>::type;
    using dst_channel_t    = typename channel_type<DstView>::type;
    using coord_t          = typename SrcView::x_coord_t;

    std::size_t const channels = num_channels<SrcView>::value;
    coord_t const width        = src_view.width();
    coord_t const height       = src_view.height();
    std::size_t pixel_max      = std::numeric_limits<dst_channel_t>::max();

    // Find control points

    std::vector<coord_t> sample_x;
    coord_t sample_x1 = tile_width_x / 2;
    coord_t sample_x2 = (tile_width_x + 1) / 2;
    coord_t sample_y1 = tile_width_y / 2;
    coord_t sample_y2 = (tile_width_y + 1) / 2;

    auto extend_left = tile_width_x;
    auto extend_top = tile_width_y;
    auto extend_right = (tile_width_x - width % tile_width_x) % tile_width_x + tile_width_x;
    auto extend_bottom = (tile_width_y - height % tile_width_y) % tile_width_y + tile_width_y;

    auto new_width = width + extend_left + extend_right;
    auto new_height = height + extend_top + extend_bottom;

    image<typename SrcView::value_type> padded_img(new_width, new_height);

    auto top_left_x = tile_width_x;
    auto top_left_y = tile_width_y;
    auto bottom_right_x = tile_width_x + width;
    auto bottom_right_y = tile_width_y + height;

    copy_pixels(src_view, subimage_view(view(padded_img), top_left_x, top_left_y, width, height));

    for (std::size_t k = 0; k < channels; k++)
    {
        std::vector<histogram<source_channel_t>> prev_row(new_width / tile_width_x), next_row((new_width / tile_width_x));
        std::vector<std::map<source_channel_t, source_channel_t>> prev_map(new_width / tile_width_x), next_map((new_width / tile_width_x));
        
        coord_t prev = 0, next = 1;
        auto channel_view = nth_channel_view(view(padded_img), k);

        for(std::ptrdiff_t i = top_left_y; i < bottom_right_y; ++i)
        {
            if((i - sample_y1) / tile_width_y >= next || i == top_left_y)
            {
                if (i != top_left_y)
                {
                    prev = next;
                    next++;
                }
                prev_row = next_row;
                prev_map = next_map;
                for(std::ptrdiff_t j = sample_x1; j < new_width; j += tile_width_x)
                {
                    auto img_view = subimage_view(channel_view,
                    next * tile_width_y,
                    j - sample_x1,                    
                    std::max<int>(std::min<int>((next + 1) * tile_width_y, bottom_right_y) - next * tile_width_y, 0),
                    std::max<int>(std::min<int>(tile_width_x + j - sample_x1, bottom_right_x) - (j - sample_x1), 0));
                    fill_histogram(img_view, next_row[(j - sample_x1) / tile_width_x], 1, false, false);
                    next_map[(j - sample_x1) / tile_width_x] = histogram_equalization(next_row[(j - sample_x1) / tile_width_x]);
                }
            }
            bool prev_row_mask = 1, next_row_mask = 1;
            if(prev == 0)
                prev_row_mask = false;
            else if(next + 1 == new_height / tile_width_y)
                next_row_mask = false;
            for(std::ptrdiff_t j = top_left_x; j < bottom_right_x; ++j)
            {
                bool prev_col_mask = true, next_col_mask = true;
                if((j - sample_x1) / tile_width_x == 0)
                    prev_col_mask = false;
                else if((j - sample_x1) / tile_width_x + 1 == new_width / tile_width_x - 1)
                    next_col_mask = false;
                
                // Bilinear interpolation

                point_t top_left((j - sample_x1) / tile_width_x * tile_width_x + sample_x1,
                                    prev * tile_width_y + sample_y1);
                point_t top_right(top_left.x + tile_width_x, top_left.y);
                point_t bottom_left(top_left.x, top_left.y + tile_width_y);
                point_t bottom_right(top_left.x + tile_width_x, top_left.y + tile_width_y);
                
                float eps = 0.001;
                long double top_left_area = std::max<double>((i - top_left.y), eps) * std::max<double>((j - top_left.x), eps);
                long double top_right_area = std::max<double>((i - top_right.y), eps) * std::max<double>((top_right.x - j), eps);
                long double bottom_left_area = std::max<double>((bottom_left.y - i), eps) * std::max<double>((j - bottom_left.x), eps);
                long double bottom_right_area = std::max<double>((bottom_right.y - i), eps) * std::max<double>((bottom_right.x - j), eps);


                long double numerator = (prev_row_mask & prev_col_mask) * top_left_area * prev_map[(top_left.x - sample_x1) / tile_width_x][channel_view(i, j)]
                                      + (prev_row_mask & next_col_mask) * top_right_area * prev_map[(top_right.x - sample_x1) / tile_width_x][channel_view(i, j)]
                                      + (next_row_mask & prev_col_mask) * bottom_left_area * next_map[(bottom_left.x - sample_x1) / tile_width_x][channel_view(i, j)]
                                      + (next_row_mask & next_col_mask) * bottom_right_area * next_map[(bottom_right.x - sample_x1) / tile_width_x][channel_view(i, j)];
                
                long double denominator = (prev_row_mask & prev_col_mask) * top_left_area
                                        + (prev_row_mask & next_col_mask) * top_right_area
                                        + (next_row_mask & prev_col_mask) * bottom_left_area
                                        + (next_row_mask & next_col_mask) * bottom_right_area;
                
                channel_view(i, j) = static_cast<source_channel_t>(numerator / denominator);
            }
        }
    }

    copy_pixels(subimage_view(view(padded_img), top_left_x, top_left_y, width, height), dst_view);

}

}}  //namespace boost::gil

#endif
