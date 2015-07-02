
/* Copyright (c) 2010-2013, Stefan Eilemann <eile@equalizergraphics.com>
 *                    2012, Daniel Nachbaur <danielnachbaur@gmail.com>
 *                    2015, David Steiner <steiner@ifi.uzh.ch>
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

#include <pthread.h>
#include <test.h>
#include <lunchbox/clock.h>
#include <lunchbox/compiler.h>
#include <lunchbox/floatMap.h>
#include <lunchbox/thread.h>
#include <iostream>

#define NOPS 100000
#define NTHREADS 4

lunchbox::FloatMap< float, uint64_t > map;
// lunchbox::FloatMap< float, uint64_t >::Group group( NTHREADS + 1 );

#ifdef LB_GCC_4_6_OR_LATER
#  pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
// class ReadThread : public lunchbox::Thread
// {
// public:
//     virtual ~ReadThread() {}
//     virtual void run() { run_(); }
// 
//     static void run_()
//     {
//         uint64_t item = 0xffffffffffffffffull;
// #ifndef NDEBUG
//         uint64_t last = 0;
// #endif
//         while( map.popBarrier( item, group ))
//         {
// #ifndef NDEBUG
//             TESTINFO( last < item, last << " >= " << item );
//             last = item;
// #endif
//         }
//         TEST( map.isEmpty( ));
//     }
// };

void fillMap()
{
    map.clear();
    map.insert(.04f, 0x1);
    map.insert(.05f, 0x2);
    map.insert(.09f, 0x3);
    map.insert(.10f, 0x4);
    map.insert(.11f, 0x5);
    map.insert(.15f, 0x6);
    map.insert(.16f, 0x7);
}

void fillMap2()
{
    map.clear();
    map.insert(.125f, 0x1);
    map.insert(.375f, 0x2);
    map.insert(.625f, 0x3);
    map.insert(.875f, 0x4);
}

int main( int argc, char **argv )
{
    fillMap();
    float key = .1f;
    uint64_t result;
    for( int i = 0; i < 3; ++i )
    {
        TEST( map.tryRemove( key, result ));
        std::cout << "map[" << key << "]: " << result << std::endl;
    }

    fillMap();
    std::vector< uint64_t > resultVec;
    map.tryRemove( 1, .5f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == 1 )

    resultVec.clear();
    const int nItems = 6;
    map.tryRemove( nItems, .07f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == nItems )

    fillMap();
    resultVec.clear();
    float maxDist = .02f;
    map.setMaxDistance(maxDist);
    std::cout << "maxDist: " << maxDist << std::endl;
    map.tryRemove( nItems, .095f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == 3 )

    fillMap();
    resultVec.clear();
    float maxDistL = .03f;
    float maxDistR = .07f;
    map.setMaxDistance(maxDistL, maxDistR);
    std::cout << "maxDist: " << maxDistL << ", " << maxDistR << std::endl;
    map.tryRemove( nItems, .095f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == 5 )

    fillMap();
    resultVec.clear();
    maxDistL = .07f;
    maxDistR = .03f;
    map.setMaxDistance(maxDistL, maxDistR);
    std::cout << "maxDist: " << maxDistL << ", " << maxDistR << std::endl;
    map.tryRemove( nItems, .11f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == 5 )

    fillMap2();
    resultVec.clear();
    maxDist = .25f;
    map.setMaxDistance(maxDist);
    std::cout << "maxDist: " << maxDist << std::endl;
    map.setMaxKey( 1.f );
    map.tryRemove( nItems, .125f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == 3 )

    fillMap2();
    resultVec.clear();
    maxDist = .25f;
    map.setMaxDistance(maxDist);
    std::cout << "maxDist: " << maxDist << std::endl;
    map.setMaxKey( 1.f );
    map.tryRemove( nItems, .8f, resultVec );
    for( size_t i = 0; i < resultVec.size(); ++i )
    {
        std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
    }
    TEST( resultVec.size() == 3 )

//     resultVec.clear();
//     map.tryRemove( nItems, .07f, resultVec );
//     for( int i = 0; i < resultVec.size(); ++i )
//     {
//         std::cout << "vec[" << i << "]: " << resultVec[ i ] << std::endl;
//     }
//     TEST( resultVec.size() == nItems )

//     ReadThread reader[ NTHREADS ];
//     for( size_t i = 0; i < NTHREADS; ++i )
//         TEST( reader[i].start( ));
// 
//     lunchbox::Clock clock;
// //     for( size_t i = 1 ; i < NOPS; ++i )
// //         map.push( i );
//     const float time = clock.getTimef();
// 
//     ReadThread::run_();
// 
//     for( size_t i = 0; i < NTHREADS; ++i )
//         TEST( reader[i].join( ));

//     std::cout << NOPS/time << " writes/ms" << std::endl;
    return EXIT_SUCCESS;
}

