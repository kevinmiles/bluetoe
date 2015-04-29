#ifndef BLUETOE_CLIENT_CHARACTERISTIC_CONFIGURATION_HPP
#define BLUETOE_CLIENT_CHARACTERISTIC_CONFIGURATION_HPP

#include <cassert>
#include <cstdint>

namespace bluetoe {
namespace details {

    class client_characteristic_configuration
    {
    public:
        client_characteristic_configuration()
            : data_( nullptr )
            , size_( 0 )
        {
        }

        // if there are no bugs, data will be null when there is no attribute access function, that will call flags()
        explicit client_characteristic_configuration( std::uint8_t* data, std::size_t s )
            : data_( data )
            , size_( s )
        {
        }

        std::uint16_t flags( std::size_t index ) const
        {
            assert( data_ );
            assert( index < size_ );

            return ( data_[ index / 4 ] >> shift( index ) ) & 0x3;
        }

        void flags( std::size_t index, std::uint16_t new_flags )
        {
            assert( data_ );
            assert( index < size_ );

            // do not assert on new_flags as they might come from a client
            data_[ index / 4 ] = ( data_[ index / 4 ] & ~mask( index ) ) | ( ( new_flags & 0x03 ) << shift( index ) );
        }

        static constexpr std::size_t bits_per_config = 2;

    private:
        static std::size_t shift( std::size_t index )
        {
            return ( index % 4 ) * bits_per_config;
        }

        static std::uint8_t mask( std::size_t index )
        {
            return 0x03 << shift( index );
        }

        std::uint8_t*   data_;

        // this member is purly for debugging and can be removed
        std::size_t     size_;
    };

    template < std::size_t Size >
    class client_characteristic_configurations
    {
    public:
        client_characteristic_configurations()
        {
            std::fill( std::begin( configs_ ), std::end( configs_ ), 0 );
        }

        client_characteristic_configuration client_configurations()
        {
            return client_characteristic_configuration( &configs_[ 0 ], Size );
        };

    private:
        std::uint8_t configs_[ ( Size * client_characteristic_configuration::bits_per_config + 7 ) / 8 ];
    };

    template <>
    class client_characteristic_configurations< 0 >
    {
    public:
        client_characteristic_configuration client_configurations()
        {
            return client_characteristic_configuration();
        }
    };

}
}

#endif