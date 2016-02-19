
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
    , _minKey( std::numeric_limits< K >::min( ))
    , _maxKey( std::numeric_limits< K >::max( ))
{}

template< typename K, typename T >
FloatMap< K, T >& FloatMap< K, T >::operator = ( const FloatMap< K, T >& from )
{
    if( this != &from )
    {
        from._cond.lock();
        MapType copy = from._map;
        const size_t maxSize = from._maxSize;
        from._cond.unlock();

        _cond.lock();
        _map.swap( copy );
        _cond.signal();
        _cond.unlock();
    }
    return *this;
}

template< typename K, typename T >
size_t FloatMap< K, T >::waitSize( const size_t minSize ) const
{
    _cond.lock();
    while( _map.size() < minSize )
        _cond.wait();
    const size_t s = _map.size();
    _cond.unlock();
    return s;
}

template< typename K, typename T >
void FloatMap< K, T >::clear()
{
    _cond.lock();
    _map.clear();
    _cond.signal();
    _cond.unlock();
}

template< typename K, typename T >
bool FloatMap< K, T >::tryRemove( const K key, T& result )
{
    _cond.lock();
    if( _map.empty( ))
    {
        _cond.unlock();
        return false;
    }

    bool found = _tryRemove( key, result );

    _cond.signal();
    _cond.unlock();
    return found;
}

template< typename K, typename T >
void FloatMap< K, T >::tryRemove( const size_t num, const K key, std::vector< T >& result )
{
    _cond.lock();
    if( _map.empty( ))
    {
        _cond.unlock();
        return;
    }

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

    _cond.signal();
    _cond.unlock();
}

template< typename K, typename T >
void FloatMap< K, T >::insert( const K key, const T& value )
{
    _cond.lock();
    _map.insert( std::pair< K, T >( key, value ));
    _cond.signal();
    _cond.unlock();
}

template< typename K, typename T >
void FloatMap< K, T >::setKeyInterval( const K minKey, const K maxKey )
{
    _minKey = minKey;
    _maxKey = maxKey;
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
