#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/algorithm.hpp>
#include <boost/gil/image_processing/adaptive_histogram_equalization.hpp>
#include <boost/gil/image_processing/histogram_equalization_temp.hpp>

using namespace boost::gil;

int main()
{
    gray8_image_t img(8, 8), img2;
    for(std::size_t g=0;g<4;g++)
    {
        for(std::size_t h=0;h<4;h++)
        {
            view(img)(g, h) = g*4+h;
        }
    }
    for(std::size_t g=0;g<4;g++)
    {
        for(std::size_t h=0;h<4;h++)
        {
            view(img)(g, h+4) = g*4+h;
        }
    }
    for(std::size_t g=0;g<4;g++)
    {
        for(std::size_t h=0;h<4;h++)
        {
            view(img)(g+4, h) = g*4+h;
        }
    }
    for(std::size_t g=0;g<4;g++)
    {
        for(std::size_t h=0;h<4;h++)
        {
            view(img)(g+4, h+4) = g*4+h;
        }
    }
    read_image("original_barbara.png", img2, png_tag{});
    gray8_image_t img_out(img.dimensions()), img_out1(img.dimensions()), img_out2(img2.dimensions());

    boost::gil::non_overlapping_interpolated_clahe(view(img2), view(img_out2));
    boost::gil::histogram_equalization(view(img), view(img_out1));

    std::cout<<"output\n";
    for(std::size_t g=0;g<8;g++)
    {
        for(std::size_t h=0;h<8;h++)
        {
            std::cout<<int(view(img_out)(g, h))<<" ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"expected\n";
    for(std::size_t g=0;g<8;g++)
    {
        for(std::size_t h=0;h<8;h++)
        {
            std::cout<<int(view(img_out1)(g, h))<<" ";
        }
        std::cout<<std::endl;
    }

    // auto out = extend_boundary(view(img), 50, boundary_option::extend_zero);

    write_view("ahe_tiles_8x8.png", view(img_out2), png_tag{});

    // rgb8_image_t rgb_img;
    // read_image("test-histogram-equalization-rgb.png",rgb_img, png_tag{});
    // rgb8_image_t rgb_img_out(rgb_img.dimensions());

    // boost::gil::histogram_equalization(view(img),view(img_out));

    // write_view("histogram_gray_equalized_image2.png", view(img_out), png_tag{});

    // boost::gil::histogram_equalization(view(rgb_img),view(rgb_img_out));

    // write_view("me1.png", view(rgb_img_out), png_tag{});

    // // Change image to independent color space, cmyk by default
    // cmyk8_image_t converted_image(srcview.dimensions());
    // copy_and_convert_pixels(srcview, view(converted_image));

    return 0;
}