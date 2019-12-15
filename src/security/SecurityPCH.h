#pragma once

#include <sne/Common.h>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4100 4244 4702)
#endif
#include <cryptopp/aes.h>
#include <cryptopp/tea.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4100 4189 4244 4267 4702 4706)
#endif
#include <cryptopp/rsa.h>
#include <cryptopp/randpool.h>
#include <cryptopp/filters.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

// Boost
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4819)
#endif
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
