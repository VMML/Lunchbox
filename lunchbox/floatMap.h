
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

#ifndef LUNCHBOX_FLOATMAP_H
#define LUNCHBOX_FLOATMAP_H

#include <lunchbox/condition.h>
#include <lunchbox/debug.h>

#include <algorithm>
#include <limits>
#include <cmath>
#include <map>
#include <string.h>

namespace lunchbox
{
    /**
     * A thread-safe map with a blocking read access.
     *
     * S is deprecated, and defines the initial maximum capacity of the
     * Map<K, T>. When the capacity is reached, pushing new values blocks until
     * items have been consumed.
     */
    template< typename K, typename T > class FloatMap
    {
    public:
        typedef T value_type;

        /** Construct a new map. @version 1.x */
        FloatMap();

        /** Construct a copy of a map. @version 1.0 */
        FloatMap( const FloatMap< K, T >& from )  { *this = from; }

        /** Destruct this Map. @version 1.x */
        ~FloatMap() {}

        /** Assign the values of another map. @version 1.x */
        FloatMap< K, T >& operator = ( const FloatMap< K, T >& from );

        /** @return true if the map is empty, false otherwise. @version 1.0 */
        bool isEmpty() const { return _map.empty(); }

        /** @return the number of items currently in the map. @version 1.0 */
        size_t getSize() const { return _map.size(); }

        /**
         * Wait for the size to be at least the number of given elements.
         *
         * @return the current size when the condition was fulfilled.
         * @version 1.x
         */
        size_t waitSize( const size_t minSize ) const;

        /** Reset (empty) the map. @version 1.x */
        void clear();

        /**
         * Try to retrieve most similar item from the map.
         *
         * @param result the value or unmodified.
         * @return true if an element was placed in result, false if the map
         *         is empty.
         * @version 1.x
         */
        bool tryRemove( const K key, T& result );

        /**
         * Try to retrieve a num most similar items from the map.
         *
         * Between zero and the given number of items are appended to the
         * vector.
         *
         * @param num the maximum number of items to retrieve
         * @param result the back value or unmodified.
         * @return true if an element was placed in result, false if the map
         *         is empty.
         * @version 1.x
         */
        void tryRemove( const size_t num, const K key, std::vector< T >& result );

        /** Insert a new element into the queue. @version 1.x */
        void insert( const K key, const T& value );

        void setMaxKey( const K key );

        void setMaxDistance( const K dist );
    
        void setMaxDistance( const K distLeft, const K distRight );

        /** @name STL compatibility. @version 1.x */
        //@{
        bool empty() const { return isEmpty(); }
        size_t size() { return getSize(); }
        //@}

    private:
        typedef std::map< K, T > MapType;

        bool _tryRemove( const K key, T& result );

        typename MapType::iterator _findTarget( const K key );

        MapType _map;
        K _maxDistLeft;
        K _maxDistRight;
        K _maxKey;
#ifdef LB_MT_FLOAT_MAP
        mutable Condition _cond;
#endif
    };

template< typename K, typename T >
typename FloatMap< K, T >::MapType::iterator FloatMap< K, T >::_findTarget( const K key )
{
    K currDist = std::numeric_limits< K >::max();
    K prevDist = std::numeric_limits< K >::max();
    typename MapType::iterator previous = _map.end();
    typename MapType::iterator current = _map.lower_bound( key );
    if( current != _map.begin( ))
    {
        previous = current;
        --previous;

        prevDist = std::abs( key - previous->first );
    }
    if( current != _map.end( ))
    {
        currDist = std::abs( key - current->first );
    }
    
    typename MapType::iterator target = _map.end();
    bool validPrev = ( prevDist <= _maxDistLeft && previous != _map.end( ));
    bool validCur = ( currDist <= _maxDistRight && current != _map.end( ));
    if( validPrev && (!validCur || prevDist < currDist ))
    {
        target = previous;
    }
    else if( validCur && (!validPrev || prevDist >= currDist ))
    {
        target = current;
    }
    else return _map.end();

    return target;
}

template< typename K, typename T >
bool FloatMap< K, T >::_tryRemove( const K key, T& result )
{
    typename MapType::iterator target = _findTarget( key );
    K pos = target->first;
    K dist = std::abs( key - pos );

    if( key > _maxKey / 2 )
    {
        typename MapType::iterator targetStart = _findTarget( 0 );
        K posStart = targetStart->first;
        K distStart = std::abs( _maxKey - key - posStart );
        if( distStart < dist )
        {
            target = targetStart;
        }
    }
    else
    {
        typename MapType::iterator targetEnd = _findTarget( _maxKey );
        K posEnd = targetEnd->first;
        K distEnd = std::abs( _maxKey - posEnd - key );
        if( distEnd < dist )
        {
            target = targetEnd;
        }
    }

    if( target != _map.end( ))
    {
        result = target->second;
        _map.erase( target );
        return true;
    }
    return false;
}
}

// template implementation
#ifdef LB_MT_FLOAT_MAP
#include "mtFloatMap.ipp"
#else
#include "floatMap.ipp"
#endif

#endif //LUNCHBOX_FLOATMAP_H
