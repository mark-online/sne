// Unity3D serializing
#pragma once

#include "RpcContainers.h"
#include <sne/core/stream/IStream.h>
#include <sne/core/stream/OStream.h>
#include <type_traits>

namespace sne { namespace srpc {

/** @addtogroup Stream
* @{
*/

struct Vector2
{
    float x;
    float y;

    template <typename Stream>
    void serialize(Stream& stream) {
        stream & x & y;
    }
};

struct Vector3
{
    float x;
    float y;
    float z;

    Vector3() :
        x(0),
        y(0),
        z(0) {}

    template <typename Stream>
    void serialize(Stream& stream) {
        stream & x & y & z;
    }
};

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;

    Quaternion() :
        x(0),
        y(0),
        z(0),
        w(0) {}

    template <typename Stream>
    void serialize(Stream& stream) {
        stream & x & y & z & w;
    }
};


typedef sne::srpc::RVector<Vector3> Vector3s;

/** @} */ // addtogroup Stream

}} // namespace sne { namespace srpc
