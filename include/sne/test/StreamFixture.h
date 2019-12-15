#pragma once

#include "DummyStreamBuffer.h"
#include <sne/core/stream/detail/SneStreamFactory.h>
#include <sne/core/stream/IStream.h>
#include <sne/core/stream/OStream.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4512)
#endif
#include <gtest/gtest.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace sne { namespace test {

/**
* @class StreamFixture
*
*/
class StreamFixture : public testing::Test
{
protected:
    static const bool shouldUseUtf8ForString = true;
    static const size_t bytesForRpcChar =
        shouldUseUtf8ForString ? 1 : sizeof(wchar_t);

protected:
    virtual void SetUp() override {
        core::SneStreamFactory streamFactory(shouldUseUtf8ForString,
            shouldUseBitPacking());
        ostream_.reset(streamFactory.createOStream(buffer_).release());
        istream_.reset(streamFactory.createIStream(buffer_).release());
    }

private:
    virtual bool shouldUseBitPacking() const = 0;

protected:
    DummyStreamBuffer buffer_;
    std::unique_ptr<core::OStream> ostream_;
    std::unique_ptr<core::IStream> istream_;
};


/**
* @class BitStreamFixture
*
*/
class BitStreamFixture : public StreamFixture
{
private:
    virtual bool shouldUseBitPacking() const override {
        return true;
    }
};


/**
* @class ByteStreamFixture
*
*/
class ByteStreamFixture : public StreamFixture
{
private:
    virtual bool shouldUseBitPacking() const override {
        return false;
    }
};

}} // namespace sne { namespace test {
