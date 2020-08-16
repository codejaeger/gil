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

// template <typename SrcView, typename DstView>
// void non_overlapping_interpolated_clahe(
//     SrcView const& src_view,
//     DstView const& dst_view,
//     std::size_t tile_width_x = 8,
//     std::size_t tile_width_y = 8,
//     double clip_limit = 1.0,
//     std::size_t bin_width = 1.0,
//     bool mask = false,
//     std::vector<std::vector<bool>> src_mask = {})
// {
//     gil_function_requires<ImageViewConcept<SrcView>>();
//     gil_function_requires<MutableImageViewConcept<DstView>>();

//     static_assert(
//         src_view.dimensions() == dst_view.dimensions(),
//         "Source and destination views must have same dimensions.");

//     static_assert(
//         color_spaces_are_compatible<
//             typename color_space_type<SrcView>::type,
//             typename color_space_type<DstView>::type>::value,
//         "Source and destination views must have same color space");
    
//     using source_channel_t = typename channel_type<SrcView>::type;
//     using dst_channel_t    = typename channel_type<DstView>::type;
//     using coord_t          = typename SrcView::x_coord_t;

//     std::size_t const channels = num_channels<SrcView>::value;
//     coord_t const width        = src_view.width();
//     coord_t const height       = src_view.height();
//     std::size_t pixel_max      = std::numeric_limits<dst_channel_t>::max();

//     // Find control points

//     vector<coord_t> sample_x;
//     coord_t sample_x1 = tile_width_x / 2;
//     coord_t sample_y1 = tile_width_y / 2;

//     for(coord_t i = sample_x1; i < width; i += tile_width_x)
//     {
//         sample_x.push_back((i - sample_x1) / tile_width_x);
//     }

//     for (std::size_t k = 0; k < channels; k++)
//     {
//         std::vector<histogram<source_channel_t>> prev_row, next_row;
//         coord_t prev = -1, next = 0;

//         for(std::ptrdiff_t i = 0; i < height, i += num_tiles_y)
//         {
//             if((i - sample_y1) / tile_width_y > next)
//             {
//                 prev = next;
//                 next = min(next + 1, (height - sample_y1) / tile_width_y);
//                 prev_row = next_row;
//                 for(std::ptrdiff_t j = sample_x1; j < width; j += tile_width_x)
//                 {
//                     if(prev == next)
//                         break;
//                     auto sub_chan_view = nth_channel_view(src_view, k);
//                     auto img_view = subimage_view(src_view, 
//                     max(0, j - sample_x1),
//                     max(0, next * tile_width_y),
//                     tile_width_x,
//                     tile_width_y);
//                     fill_histogram(img_view, next_row[(j - sample_x1) / tile_width_x]);
//                 }
//             }
//             std::vector<coord_t> r;
//             if((i - sample_y1) / tile_width_y == 0)
//             {
//                 r.push_back(prev);
//             }
//             else if(next == prev)
//             {
//                 r.push_back(next);
//             }
//             else
//             {
//                 r.push_back(prev);
//                 r.push_back(next);
//             }
//             for(std::ptrdiff_t j = 0; j < width; j += num_tiles_x)
//             {
//                 std::vector<coord_t> c;
//                 if ((j - sample_x1) / tile_width_x == 0)
//                 {
//                     c.push_back(0);
//                 }
//                 else ((j - sample_x1) / tile_width_x == (height - sample_y1) / tile_width_y)
//                 {
//                     c.push_back((height - sample_y1) / tile_width_y);
//                 }
//                 else if()
//                 {
//                     c.push_back((j - sample_x1) / tile_width_x);
//                     c.push_back((j - sample_x1) / tile_width_x + 1);
//                 }
//                 std::vector<point_t> sample_pts;
//                 for(auto x : c)
//                 {
//                     for(auto y : r)
//                     {
//                         sample_pts.push_back(point_t(x, y));
//                     }
//                 }
//                 if(sample_pts.size() == 1)
//                 {

//                 }
//                 else if(sample_pts.size() == 2)
//                 {

//                 }
//                 else if(sample_pts.size() == 4)
//                 {

//                 }
//             }
//         }
//     }
// }

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

    // std::cout<<"New image"<<std::endl;
    // for(std::size_t g=0;g<new_width;g++)
    // {
    //     for(std::size_t h=0;h<new_height;h++)
    //     {
    //         std::cout<<int(view(padded_img)(g, h))<<" ";
    //     }
    //     std::cout<<std::endl;
    // }

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
                // std::cout<<"NEXT"<<" "<<next<<"\n";
                for(std::ptrdiff_t j = sample_x1; j < new_width; j += tile_width_x)
                {
                    // std::cout<<int(std::max<coord_t>(std::min<coord_t>(tile_width_x + j - sample_x1, bottom_right_x) - (j - sample_x1), 0))<<" "<<j - sample_x1;
                    // std::cout<<int(std::max<coord_t>(std::min<coord_t>((next + 1) * tile_width_y, bottom_right_y) - next * tile_width_y, 0))<<" "<<j - sample_x1;
                    auto img_view = subimage_view(channel_view,
                    next * tile_width_y,
                    j - sample_x1,                    
                    std::max<int>(std::min<int>((next + 1) * tile_width_y, bottom_right_y) - next * tile_width_y, 0),
                    std::max<int>(std::min<int>(tile_width_x + j - sample_x1, bottom_right_x) - (j - sample_x1), 0));
                    // for(std::size_t g=0;g<img_view.width();g++)
                    // {
                    //     for(std::size_t h=0;h<img_view.height();h++)
                    //     {
                    //         std::cout<<int(channel_view(g, h))<<" ";
                    //     }
                    //     std::cout<<std::endl;
                    // }
                    fill_histogram(img_view, next_row[(j - sample_x1) / tile_width_x], 1, false, false);
                    // std::cout<<j<<" "<<"histogram: "<<" "<<next * tile_width_y<<std::endl;
                    // for(std::size_t g=0;g<img_view.width();g++)
                    // {
                    //     for(std::size_t h=0;h<img_view.height();h++)
                    //     {
                    //         std::cout<<int(img_view(g, h))<<" ";
                    //     }
                    //     std::cout<<std::endl;
                    // }
                    // for (auto it:next_row[(j - sample_x1) / tile_width_x])
                    // {
                    //     std::cout<<int(std::get<0>(it.first))<<" "<<it.second<<std::endl;
                    // }
                    // std::cout<<std::endl<<"Map: "<<std::endl;
                    next_map[(j - sample_x1) / tile_width_x] = histogram_equalization(next_row[(j - sample_x1) / tile_width_x]);
                    // for (auto it:next_map[(j - sample_x1) / tile_width_x])
                    // {
                    //     std::cout<<int(it.first)<<" "<<int(it.second)<<std::endl;
                    // }
                    // std::cout<<std::endl;
                }
                // if(i == 0)
                //     prev_row = next_row;
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

                // if((prev_col_mask || prev_row_mask || next_col_mask || next_row_mask) == false)
                //     std::cout<<i<<" "<<j<<std::endl;

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
                

                
                // Debugging
                // std::cout<<"\n ------------------------ \n";
                // std::cout<<"("<<i<<", "<<j<<std::endl;
                // std::cout<<"Pixel: "<<int(channel_view(i, j))<<std::endl;
                // std::cout<<"[["<<(prev_row_mask&prev_col_mask)<<", "<<(prev_row_mask&next_col_mask)<<"] "<<std::endl;
                // std::cout<<" ["<<(next_row_mask&prev_col_mask)<<", "<<(next_row_mask&next_col_mask)<<"]]"<<std::endl;
                // std::cout<<"Pixel: "<<int(channel_view(i, j))<<std::endl;
                // std::cout<<"[["<<(top_left_area)<<", "<<(top_right_area)<<"] "<<std::endl;
                // std::cout<<" ["<<(bottom_left_area)<<", "<<(bottom_right_area)<<"]]"<<std::endl;
                // std::cout<<"Pixel: "<<int(channel_view(i, j))<<std::endl;
                // std::cout<<"[["<<int(prev_map[(top_left.x - sample_x1) / tile_width_x][channel_view(i, j)])<<", "<<int(prev_map[(top_right.x - sample_x1) / tile_width_x][channel_view(i, j)])<<"] "<<std::endl;
                // std::cout<<" ["<<int(next_map[(bottom_left.x - sample_x1) / tile_width_x][channel_view(i, j)])<<", "<<int(next_map[(bottom_right.x - sample_x1) / tile_width_x][channel_view(i, j)])<<"]]"<<std::endl;
                // std::cout<<"Top left: "<<top_left.x<<", "<<top_left.y<<std::endl;
                // std::cout<<"Top right: "<<top_right.x<<", "<<top_right.y<<std::endl;
                // std::cout<<"Bottom left: "<<bottom_left.x<<", "<<bottom_left.y<<std::endl;
                // std::cout<<"Bottom right: "<<bottom_right.x<<", "<<bottom_right.y<<std::endl;
                // std::cout<<"0, 0 - "<<prev_row
                // std::cout<<(prev_row_mask & prev_col_mask) << (i - top_left.y) * (j - top_left.x)<<" "
                //             <<(prev_row_mask & next_col_mask) << (i - top_right.y) * (top_right.x - j)<<" "
                //             <<(next_row_mask & prev_col_mask) << (bottom_left.y - i) * (j - bottom_left.x)<<" "
                //             <<(next_row_mask & next_col_mask) << (bottom_right.y - i) * (bottom_right.x - j)<<std::endl;

                // if(denominator != 0)
                channel_view(i, j) = static_cast<source_channel_t>(numerator / denominator);
            // std::cout<<"new val ("<<i<<", "<<j<<") "<<int(channel_view(i, j))<<std::endl;
                
    //             for(std::size_t g=0;g<new_width;g++)
    // {
    //     for(std::size_t h=0;h<new_height;h++)
    //     {
    //         std::cout<<int(view(padded_img)(g, h))<<" ";
    //     }
    //     std::cout<<std::endl;
    // }

            }
        // std::cout<<"New image"<<std::endl;
    // for(std::size_t g=0;g<new_width;g++)
    // {
    //     for(std::size_t h=0;h<new_height;h++)
    //     {
    //         std::cout<<int(view(padded_img)(g, h))<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
        }
    }

    copy_pixels(subimage_view(view(padded_img), top_left_x, top_left_y, width, height), dst_view);

}

}}  //namespace boost::gil

#endif
