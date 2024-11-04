// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_SERIALIZATION_BASE_MACROS_H_
#define PARA_SERIALIZATION_BASE_MACROS_H_

// Macros to apply other macros over all elements in a list.

// Recursive expansion macros.
#define _PARA_EXPAND0(...) __VA_ARGS__
#define _PARA_EXPAND1(...) _PARA_EXPAND0(_PARA_EXPAND0(_PARA_EXPAND0(__VA_ARGS__)))
#define _PARA_EXPAND2(...) _PARA_EXPAND1(_PARA_EXPAND1(_PARA_EXPAND1(__VA_ARGS__)))
#define _PARA_EXPAND3(...) _PARA_EXPAND2(_PARA_EXPAND2(_PARA_EXPAND2(__VA_ARGS__)))
#define _PARA_EXPAND4(...) _PARA_EXPAND3(_PARA_EXPAND3(_PARA_EXPAND3(__VA_ARGS__)))
#define _PARA_EXPAND5(...) _PARA_EXPAND4(_PARA_EXPAND4(_PARA_EXPAND4(__VA_ARGS__)))
#define _PARA_EXPAND(...) _PARA_EXPAND5(_PARA_EXPAND5(_PARA_EXPAND5(__VA_ARGS__)))

// Required to workaround a bug in the VC++ preprocessor.
#define _PARA_INDIRECT_EXPAND(macro, args) macro args

// Defines a step separation for macro expansion.
#define _PARA_SEPARATOR

// Clears any remaining contents wrapped in parentheses.
#define _PARA_CLEAR(...)

// Introduces a first dummy argument and _PARA_CLEAR as second argument.
#define _PARA_CLEAR_IF_LAST() _, _PARA_CLEAR

// Returns the first argument of a list.
#define _PARA_FIRST_ARG(first, ...) first

#define _PARA_REST_ARG(_, ...) __VA_ARGS__

// Returns the second argument of a list.
#define _PARA_SECOND_ARG(_, second, ...) second

#define _PARA_CAT(a, ...) a##__VA_ARGS__

#define _PARA_IS_PROBE(...) _PARA_SECOND_ARG(__VA_ARGS__, 0)
#define _PARA_PROBE() ~, 1

#define _PARA_IS_PAREN(...) _PARA_IS_PROBE(_PARA_IS_PAREN_PROBE __VA_ARGS__)
#define _PARA_IS_PAREN_PROBE(...) _PARA_PROBE()

#define _PARA_NOT(x) _PARA_IS_PROBE(_PARA_CAT(_PARA_NOT_, x))
#define _PARA_NOT_0 _PARA_PROBE()

#define _PARA_BOOL(x) _PARA_NOT(_PARA_NOT(x))

#define _PARA_IF_ELSE(condition) __PARA_IF_ELSE(_PARA_BOOL(condition))
#define __PARA_IF_ELSE(condition) _PARA_CAT(_PARA_IF_, condition)

#define _PARA_IF_1(...) __VA_ARGS__ _PARA_IF_1_ELSE
#define _PARA_IF_0(...) _PARA_IF_0_ELSE

#define _PARA_IF_1_ELSE(...)
#define _PARA_IF_0_ELSE(...) __VA_ARGS__

#define _PARA_HAS_ARGS(...) _PARA_BOOL(_PARA_FIRST_ARG(_PARA_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _PARA_END_OF_ARGUMENTS_(...) _PARA_IS_PAREN(__VA_ARGS__)

#define _PARA_EMPTY()
#define _PARA_DEFER1(m) m _PARA_EMPTY()
#define _PARA_DEFER2(m) m _PARA_EMPTY _PARA_EMPTY()()
#define _PARA_DEFER3(m) m _PARA_EMPTY _PARA_EMPTY _PARA_EMPTY()()()
#define _PARA_DEFER4(m) m _PARA_EMPTY _PARA_EMPTY _PARA_EMPTY _PARA_EMPTY()()()()
#define _PARA_DEFER5(m) m _PARA_EMPTY _PARA_EMPTY _PARA_EMPTY _PARA_EMPTY _PARA_EMPTY()()()()()

#define _PARA_REMOVE_PARENS(...)               \
    _PARA_IF_ELSE(_PARA_IS_PAREN(__VA_ARGS__)) \
    (_PARA_STRIP_PARENS __VA_ARGS__)(__VA_ARGS__)

#define _PARA_STRIP_PARENS(...) __VA_ARGS__

#define PARA_MAP(...) _PARA_EXPAND(_PARA_MAP_FIRST(__VA_ARGS__))

#define _PARA_MAP_FIRST(m, ...)                \
    _PARA_IF_ELSE(_PARA_HAS_ARGS(__VA_ARGS__)) \
    (m(_PARA_REMOVE_PARENS(_PARA_FIRST_ARG(__VA_ARGS__))) _PARA_MAP_NEXT(m, _PARA_REST_ARG(__VA_ARGS__)))(/* done */)

#define _PARA_MAP_NEXT(m, ...)                              \
    _PARA_IF_ELSE(_PARA_HAS_ARGS(__VA_ARGS__))              \
    (, m(_PARA_REMOVE_PARENS(_PARA_FIRST_ARG(__VA_ARGS__))) \
           _PARA_DEFER3(__PARA_MAP_NEXT)()(m, _PARA_REST_ARG(__VA_ARGS__)))(/* done */)

#define __PARA_MAP_NEXT() _PARA_MAP_NEXT

#define PARA_MAP_ARGS(...) _PARA_EXPAND(_PARA_MAP_FIRST_ARGS(__VA_ARGS__))
#define _PARA_MAP_ARGS() PARA_MAP_ARGS

#define _PARA_MAP_FIRST_ARGS(m, args, ...)                                           \
    _PARA_IF_ELSE(_PARA_HAS_ARGS(__VA_ARGS__))                                       \
    (m(_PARA_REMOVE_PARENS(args), _PARA_REMOVE_PARENS(_PARA_FIRST_ARG(__VA_ARGS__))) \
         _PARA_MAP_NEXT_ARGS(m, args, _PARA_REST_ARG(__VA_ARGS__)))(/* done */)

#define _PARA_MAP_NEXT_ARGS(m, args, ...)                                              \
    _PARA_IF_ELSE(_PARA_HAS_ARGS(__VA_ARGS__))                                         \
    (, m(_PARA_REMOVE_PARENS(args), _PARA_REMOVE_PARENS(_PARA_FIRST_ARG(__VA_ARGS__))) \
           _PARA_DEFER3(__PARA_MAP_NEXT_ARGS)()(m, args, _PARA_REST_ARG(__VA_ARGS__)))(/* done */)

#define __PARA_MAP_NEXT_ARGS() _PARA_MAP_NEXT_ARGS

#endif  // PARA_SERIALIZATION_BASE_MACROS_H_
