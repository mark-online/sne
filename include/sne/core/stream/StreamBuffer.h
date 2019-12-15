#pragma once

#include <sne/Common.h>

namespace sne { namespace core {

/** @addtogroup Stream
* 스트림 IO
* @{
*/

/**
 * @class StreamBuffer
 *
 * std::queue와 동작 방식이 유사하다
 * - 에러가 발생할 경우 StreamingException을 던져야(throw) 한다
 */
class StreamBuffer
{
public:
    typedef uint8_t Item;
public:
    virtual ~StreamBuffer() {}

    /**
     * 아이템을 버퍼 끝에 추가한다
     * @warning 정책에 따라 메모리 처리를 적절히 해주어야 한다
     * @exception StreamingException 실패할 경우
     */
    virtual void push(Item item) = 0;

    /**
     * 버퍼 앞에서 아이템 하나를 삭제한다
     * @pre ! empty()
     */
    virtual void pop() = 0;

    /**
     * 버퍼로 부터 바이트 스트림을 복사한다
     * @param buffer 원본 버퍼
     * @param bufferSize buffer 바이트 수
     * @exception StreamingException 실패할 경우
     */
    virtual void copyFrom(const Item* buffer, size_t bufferSize) = 0;

    /**
    * 버퍼로 바이트 스트림을 복사한다
    * @param buffer 대상 버퍼
    * @param bufferSize buffer 바이트 수
    * @pre size() >= bufferSize
    * @exception StreamingException 실패할 경우
    */
    virtual void copyTo(Item* buffer, size_t bufferSize) = 0;

    /// 항목을 초기화한다
    virtual void reset() = 0;

    /**
     * 버퍼 제일 앞쪽 데이터를 리턴한다
     * @pre ! empty()
     */
    virtual Item front() const = 0;

    /**
     * 버퍼 제일 앞쪽 데이터의 레퍼런스를 리턴한다
     * @pre ! empty()
     */
    virtual Item& front() = 0;

    /**
     * 버퍼 제일 뒤쪽 데이터를 리턴한다
     * @pre ! empty()
     */
    virtual Item back() const = 0;

    /**
     * 버퍼 제일 뒤쪽 데이터의 레퍼런스를 리턴한다
     * @pre ! empty()
     */
    virtual Item& back() = 0;

    /// 버퍼의 크기를 얻는다
    virtual size_t size() const = 0;

    /// 버퍼가 비어 있는가?
    virtual bool empty() const = 0;

    /// 버퍼에 남은 빈 공간 크기(capacity - size())
    virtual size_t space() const = 0;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core
