//
// Copyright (c) 2013-2016 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BEAST_HTTP_STRING_BODY_HPP
#define BEAST_HTTP_STRING_BODY_HPP

#include <beast/http/body_type.hpp>
#include <beast/core/detail/ignore_unused.hpp>
#include <boost/asio/buffer.hpp>
#include <memory>
#include <string>

namespace beast {
namespace http {

/** A Body represented by a std::string.

    Meets the requirements of @b `Body`.
*/
struct string_body
{
    /// The type of the `message::body` member
    using value_type = std::string;

#if GENERATING_DOCS
private:
#endif

    class reader
    {
        value_type& s_;

    public:
        template<bool isRequest, class Headers>
        explicit
        reader(message<isRequest,
                string_body, Headers>& m) noexcept
            : s_(m.body)
        {
        }

        void
        write(void const* data,
            std::size_t size, error_code&) noexcept
        {
            auto const n = s_.size();
            s_.resize(n + size);
            std::memcpy(&s_[n], data, size);
        }
    };

    class writer
    {
        value_type const& body_;

    public:
        writer(writer const&) = delete;
        writer& operator=(writer const&) = delete;

        template<bool isRequest, class Headers>
        explicit
        writer(message<
                isRequest, string_body, Headers> const& msg)
            : body_(msg.body)
        {
        }

        void
        init(error_code& ec)
        {
            beast::detail::ignore_unused(ec);
        }

        std::uint64_t
        content_length() const
        {
            return body_.size();
        }

        template<class Write>
        boost::tribool
        operator()(resume_context&&, error_code&, Write&& write)
        {
            write(boost::asio::buffer(body_));
            return true;
        }
    };
};

} // http
} // beast

#endif
