#pragma once

#ifdef _WIN32

// A template can not be exported. Only an instantiation may be exported.
#   define SNE_DO_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT

// struct 'XXX' needs to have dll-interface to be used by clients of class 'XX'
#   pragma warning (disable: 4251)
//non dll-interface class 'XXX' used as base for dll-interface class 'XXX'
#   pragma warning (disable: 4275)
// conditional expression is constant
#   pragma warning ( disable: 4127 )
// copy constructor could not be generated
#   pragma warning ( disable: 4511 )
//assignment operator could not be generated
#   pragma warning ( disable: 4512 )
#   pragma warning (disable: 4231)
#   pragma warning (disable: 4706)

#ifndef STRICT
#   define STRICT
#endif // STRICT

#ifndef NOMINMAX
#   define NOMINMAX
#endif // NOMINMAX

#ifndef NOCRYPT
#   define NOCRYPT
#endif // NOCRYPT

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#endif // _WIN32
