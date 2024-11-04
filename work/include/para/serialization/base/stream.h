// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_STREAM_H_
#define PARA_SERIALIZATION_STREAM_H_

#include <iostream>
#include <sstream>

namespace para
{
namespace serialization
{

struct stringbuf : std::stringbuf
{
    const char* strbuffer() const
    {
        return pbase();
    }
};

struct ostringstream : stringbuf, std::ostream
{
    using char_type = std::ostream::char_type;

    ostringstream() : stringbuf(), std::ostream(static_cast<std::stringbuf*>(this))
    {}
};

struct membuf : std::streambuf
{
    membuf(char const* base, std::size_t size)
    {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }

    void reset(char const* base, std::size_t size)
    {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }

  protected:
    pos_type seekoff(off_type off, std::ios_base::seekdir dir,
                     std::ios_base::openmode which = std::ios_base::in) override
    {
        if (dir == std::ios_base::cur)
            gbump(off);
        else if (dir == std::ios_base::end)
            setg(eback(), egptr() + off, egptr());
        else if (dir == std::ios_base::beg)
            setg(eback(), eback() + off, egptr());

        return gptr() - eback();
    }
};

struct imemstream : virtual membuf, std::istream
{
    using char_type = typename std::istream::char_type;

    imemstream(char const* base, size_t size) : membuf(base, size), std::istream(static_cast<std::streambuf*>(this))
    {}
};

}  // namespace serialization
}  // namespace para

#endif