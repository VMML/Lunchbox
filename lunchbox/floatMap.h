
/* Copyright (c) 2015-2016, David Steiner <steiner@ifi.uzh.ch>
 *               2016,      Enrique G. Paredes <egparedes@ifi.uzh.ch>
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

        void setKeyInterval( const K minKey, const K maxKey );

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
        K _minKey;
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
    typename MapType::iterator current = _map.lower_bound( key );
    typename MapType::iterator previous = current;
    if( current != _map.begin( ))
    {
        --previous;
        prevDist = key - previous->first;
    }
    else // wrap around
    {
        // Get forward iterator to last valid element
        previous = --(_map.rbegin().base());
        prevDist = ( key - _minKey ) + ( _maxKey - previous->first );
    }

    if( current != _map.end( ))
    {
        currDist = current->first - key;
    }
    else // wrap around
    {
        current = _map.begin();
        currDist = ( _maxKey - key ) + ( current->first - _minKey );
    }

    typename MapType::iterator target = _map.end();
    bool validPrev = ( prevDist <= _maxDistLeft );
    bool validCur = ( currDist <= _maxDistRight );
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
