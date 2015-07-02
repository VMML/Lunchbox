
/* Copyright (c) 2015, David Steiner <steiner@ifi.uzh.ch>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

namespace lunchbox
{

template< typename K, typename T >
FloatMap< K, T >::FloatMap()
    : _maxDistLeft( std::numeric_limits< K >::max( ))
    , _maxDistRight( std::numeric_limits< K >::max( ))
    , _maxKey( std::numeric_limits< K >::max( ))
{}

template< typename K, typename T >
FloatMap< K, T >& FloatMap< K, T >::operator = ( const FloatMap< K, T >& from )
{
    if( this != &from )
    {
        MapType copy = from._map;
        _map.swap( copy );
    }
    return *this;
}

template< typename K, typename T >
size_t FloatMap< K, T >::waitSize( const size_t minSize ) const
{
    while( _map.size() < minSize );
    const size_t s = _map.size();
    return s;
}

template< typename K, typename T >
void FloatMap< K, T >::clear()
{
    _map.clear();
}

template< typename K, typename T >
bool FloatMap< K, T >::tryRemove( const K key, T& result )
{
    if( _map.empty( )) return false; 
    bool found = _tryRemove( key, result );

    return found;
}

template< typename K, typename T >
void FloatMap< K, T >::tryRemove( const size_t num, const K key, std::vector< T >& result )
{
    if( _map.empty( )) return;

    size_t nRemoved = 0;
    while( nRemoved < num )
    {
        T value;
        if( _tryRemove( key, value ))
        {
            result.push_back( value );
            ++nRemoved;
        }
        else break;
    }
}

template< typename K, typename T >
void FloatMap< K, T >::setMaxKey( const K key )
{
    _maxKey = key;
}

template< typename K, typename T >
void FloatMap< K, T >::insert( const K key, const T& value )
{
    _map.insert( std::pair< K, T >( key, value ));
}

template< typename K, typename T >
void FloatMap< K, T >::setMaxDistance( const K dist )
{
    _maxDistLeft = dist;
    _maxDistRight = dist;
}

template< typename K, typename T >
void FloatMap< K, T >::setMaxDistance( const K distLeft, const K distRight )
{
    _maxDistLeft = distLeft;
    _maxDistRight = distRight;
}

}
