// Compiles with C++14 due to Boost.Histogram -> https://github.com/boostorg/histogram/blob/develop/README.md

#include <boost/gil.hpp>
#include <boost/gil/extension/histogram/boost_histogram.hpp>
#include <boost/gil/extension/histogram/histogram_operations.hpp>
#include <boost/gil/image_processing/histogram_equalization.hpp>

#include <boost/histogram.hpp>
#include <boost/core/lightweight_test.hpp>

#include <vector>
#include <map>

using namespace boost::gil;
using namespace boost::histogram;

int main()
{	
	gray8_image_t img(1,1,gray8_pixel_t(1));
	
	// check calls
	std::vector<int> v;
	std::vector<std::vector<int> > vv;
	
	std::map<int,int> m;
	std::map<int,std::map<int,int> > mm;
	
	//Boost.Histogram
	auto h = make_histogram(axis::regular<>(2,1,2,"x"));
	
	imgHist(view(img),v,false);  // Works
	imgHist(view(img),vv,false);  // Works due to overload
	imgHist(view(img),m,false);  // Works
	// imgHist(view(img),mm,false);  // Fails due to no overload present
	imgHist(view(img),h,false);  // Works due to overload

	
	gray8_image_t dst_img;

	// check histogram equalization call
	// Not the best look to the function call, will try to standardise upon a particular container type based on performance
	histogram_equalization<>()(view(img),const_view(dst_img));  

	histogram_equalization<std::map<int, int>>()(view(img),const_view(dst_img));

	return boost::report_errors();
}