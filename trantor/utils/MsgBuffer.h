#pragma once

#include <trantor/utils/NonCopyable.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <assert.h>
#define BUFFER_DEF_LENGTH 2048
namespace trantor
{
    class MsgBuffer
    {
    public:
        MsgBuffer(size_t len=BUFFER_DEF_LENGTH);
        //peek
        const char *peek() const {return begin()+head_;}
        const uint8_t peekInt8() const {
            assert(readableBytes()>=1);
            return *(static_cast<const uint8_t*>((void *)peek()));}
        const uint16_t peekInt16() const;
        const uint32_t peekInt32() const;
        const uint64_t peekInt64() const;
        //read
        std::string read(uint64_t len);
        uint8_t readInt8();
        uint16_t readInt16();
        uint32_t readInt32();
        uint64_t readInt64();
        void swap(MsgBuffer &buf);
        const size_t readableBytes() const {return tail_-head_;}
        const size_t writableBytes() const {return buffer_.size()-tail_;}
//append
        void append(const MsgBuffer &buf);
        void append(const char *buf,size_t len);
        void append(const std::string &buf){
            ensureWritableBytes(buf.length());
            append(buf.c_str(),buf.length());}
        void appendInt8(const uint8_t b){append(static_cast<const char*>((void *)&b),1);}
        void appendInt16(const uint16_t s);
        void appendInt32(const uint32_t i);
        void appendInt64(const uint64_t l);
//add in front
        void addInFront(const char *buf,size_t len);

        void retrieveAll();
        void retrieve(size_t len);
        size_t readFd(int fd,int *retErrno);

    private:
        size_t head_;
        std::vector<char> buffer_;

        size_t tail_;
        const char *begin() const {return &buffer_[0];}
        char *begin() {return &buffer_[0];}
        void ensureWritableBytes(size_t len);
    };
}