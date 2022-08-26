//
// cpp11/can_require_not_applicable_free.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "asio/require.hpp"
#include <cassert>

template <int>
struct prop
{
  static constexpr bool is_requirable = true;
};

template <int>
struct object
{
  template <int N>
  friend constexpr object<N> require(const object&, prop<N>)
  {
    return object<N>();
  }
};

int main()
{
  static_assert(!asio::can_require<object<1>, prop<2>>::value, "");
  static_assert(!asio::can_require<object<1>, prop<2>, prop<3>>::value, "");
  static_assert(!asio::can_require<object<1>, prop<2>, prop<3>, prop<4>>::value, "");
  static_assert(!asio::can_require<const object<1>, prop<2>>::value, "");
  static_assert(!asio::can_require<const object<1>, prop<2>, prop<3>>::value, "");
  static_assert(!asio::can_require<const object<1>, prop<2>, prop<3>, prop<4>>::value, "");
}
