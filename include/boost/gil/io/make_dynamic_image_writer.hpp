//
// Copyright 2012 Christian Henning
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_IO_MAKE_DYNAMIC_IMAGE_WRITER_HPP
#define BOOST_GIL_IO_MAKE_DYNAMIC_IMAGE_WRITER_HPP

#include <boost/gil/io/get_writer.hpp>

#include <boost/utility/enable_if.hpp>

namespace boost { namespace gil {

template< typename String
        , typename FormatTag
        >
inline
typename get_dynamic_image_writer< String
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( const String&                        file_name
                         , const image_write_info< FormatTag >& info
                         , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                                        , is_format_tag< FormatTag >
                                                        >
                                             >::type* /* ptr */ = nullptr
                         )
{
    typename get_write_device< String
                    , FormatTag
                    >::type device( detail::convert_to_native_string( file_name )
                                  , typename detail::file_stream_device< FormatTag >::write_tag()
                                  );

    return typename get_dynamic_image_writer< String
                                            , FormatTag
                                            >::type( device
                                                   , info
                                                   );
}

template< typename FormatTag >
inline
typename get_dynamic_image_writer< std::wstring
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( const std::wstring&                  file_name
                         , const image_write_info< FormatTag >& info
                         )
{
    const char* str = detail::convert_to_native_string( file_name );

    typename get_write_device< std::wstring
                    , FormatTag
                    >::type device( str
                                  , typename detail::file_stream_device< FormatTag >::write_tag()
                                  );

    delete[] str;

    return typename get_dynamic_image_writer< std::wstring
                                            , FormatTag
                                            >::type( device
                                                   , info
                                                   );
}

#ifdef BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
template< typename FormatTag >
inline
typename get_dynamic_image_writer< std::wstring
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( const filesystem::path&              path
                         , const image_write_info< FormatTag >& info
                         )
{
    return make_dynamic_image_writer( path.wstring()
                                    , info
                                    );
}
#endif // BOOST_GIL_IO_ADD_FS_PATH_SUPPORT


template< typename Device
        , typename FormatTag
        >
inline
typename get_dynamic_image_writer< Device
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( Device&                              file
                         , const image_write_info< FormatTag >& info
                         , typename enable_if< mpl::and_< typename detail::is_adaptable_output_device< FormatTag, Device >::type
                                                        , is_format_tag< FormatTag >
                                                        >
                                             >::type* /* ptr */ = 0
                         )
{
    typename get_write_device< Device
                    , FormatTag
                    >::type device( file );

    return typename get_dynamic_image_writer< Device
                                            , FormatTag
                                            >::type( device
                                                   , info
                                                   );
}


// no image_write_info

template< typename String
        , typename FormatTag
        >
inline
typename get_dynamic_image_writer< String
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( const String&    file_name
                         , const FormatTag&
                         , typename enable_if< mpl::and_< detail::is_supported_path_spec< String >
                                                        , is_format_tag< FormatTag >
                                                        >
                                             >::type* /* ptr */ = nullptr
                         )
{
    return make_dynamic_image_writer( file_name
                                    , image_write_info< FormatTag >()
                                    );
}

template< typename FormatTag >
inline
typename get_dynamic_image_writer< std::wstring
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( const std::wstring& file_name
                         , const FormatTag&
                         )
{
    return make_dynamic_image_writer( file_name
                                    , image_write_info< FormatTag >()
                                    );
}

#ifdef BOOST_GIL_IO_ADD_FS_PATH_SUPPORT
template< typename FormatTag >
inline
typename get_dynamic_image_writer< std::wstring
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( const filesystem::path& path
                         , const FormatTag&
                         )
{
    return make_dynamic_image_writer( path.wstring()
                                    , image_write_info< FormatTag >()
                                    );
}
#endif // BOOST_GIL_IO_ADD_FS_PATH_SUPPORT


template< typename Device
        , typename FormatTag
        >
inline
typename get_dynamic_image_writer< Device
                                 , FormatTag
                                 >::type
make_dynamic_image_writer( Device&          file
                         , const FormatTag&
                         , typename enable_if< mpl::and_< typename detail::is_adaptable_output_device< FormatTag, Device >::type
                                                        , is_format_tag< FormatTag >
                                                        >
                                             >::type* /* ptr */ = 0
                         )
{
    return make_dynamic_image_writer( file
                                    , image_write_info< FormatTag >()
                                    );
}

} // namespace gil
} // namespace boost

#endif
