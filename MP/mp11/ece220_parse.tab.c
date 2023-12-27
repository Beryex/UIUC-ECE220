/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         ece220parse
#define yylex           ece220lex
#define yyerror         ece220error
#define yydebug         ece220debug
#define yynerrs         ece220nerrs
#define yylval          ece220lval
#define yychar          ece220char

/* First part of user prologue.  */
#line 37 "ece220_parse.y"

    #include <stdio.h>
    #include <stdlib.h>

    #include "ece220_lex.h"
    #include "ece220_parse.h"
    #include "ece220_symtab.h"
    #define YYSTYPE ast220_t*

    extern int ece220lex ();

    void parse_program ();

    static void var_create (ast220_t* name, ast220_t* a_len);
    static void start_main ();
    static ast220_t* new_ast_node ();
    static ast220_t* new_ast_num (int32_t value);
    static ast220_t* new_ast_str (char* str);
    static ast220_t* new_ast_func (ast220_builtin_func_t id);
    static ast220_t* new_ast_op (ast220_type_t type, ast220_t* left, 
    				 ast220_t* right);

    static int32_t reading_globals;
    static int32_t global_offset;
    static int32_t local_offset;
    static int32_t has_main;

    static ast220_t* ece220_main;

#line 107 "ece220_parse.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_ECE220_ECE220_PARSE_TAB_H_INCLUDED
# define YY_ECE220_ECE220_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ece220debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    C220_KEY_IDENT = 258,
    C220_KEY_INT = 259,
    C220_KEY_MAIN = 260,
    C220_KEY_INT_TYPE = 261,
    C220_KEY_STRING = 262,
    C220_KEY_ELSE = 263,
    C220_KEY_FOR = 264,
    C220_KEY_IF = 265,
    C220_KEY_RETURN = 266,
    C220_KEY_PRINTF = 267,
    C220_KEY_RAND = 268,
    C220_KEY_SCANF = 269,
    C220_KEY_SRAND = 270,
    C220_KEY_INCR = 271,
    C220_KEY_DECR = 272,
    C220_KEY_UNARY_NEG = 273,
    C220_KEY_DEBUG_MARKER = 274,
    C220_KEY_AND = 275,
    C220_KEY_OR = 276,
    C220_KEY_EQ = 277,
    C220_KEY_NE = 278,
    C220_KEY_LE = 279,
    C220_KEY_GE = 280,
    C220_KEY_POST_ID = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE ece220lval;

int ece220parse (void);

#endif /* !YY_ECE220_ECE220_PARSE_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   324

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  130

#define YYUNDEFTOK  2
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,     2,     2,    33,    34,     2,
      43,    44,    31,    29,    40,    30,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
      25,    20,    28,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,    42,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    21,    22,    23,    24,    26,
      27,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    88,    88,    89,    90,    93,    94,    97,    99,   101,
     102,   105,   107,   109,   110,   113,   115,   121,   123,   124,
     138,   139,   142,   143,   150,   159,   164,   170,   171,   174,
     176,   177,   184,   190,   193,   196,   199,   202,   203,   208,
     209,   212,   215,   218,   221,   224,   227,   230,   233,   236,
     239,   242,   245,   248,   251,   254,   255,   258,   275,   276,
     279,   281,   288,   289,   290,   291,   294,   295,   298,   299,
     305,   306,   309
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "C220_KEY_IDENT", "C220_KEY_INT",
  "C220_KEY_MAIN", "C220_KEY_INT_TYPE", "C220_KEY_STRING", "C220_KEY_ELSE",
  "C220_KEY_FOR", "C220_KEY_IF", "C220_KEY_RETURN", "C220_KEY_PRINTF",
  "C220_KEY_RAND", "C220_KEY_SCANF", "C220_KEY_SRAND", "C220_KEY_INCR",
  "C220_KEY_DECR", "C220_KEY_UNARY_NEG", "C220_KEY_DEBUG_MARKER", "'='",
  "C220_KEY_AND", "C220_KEY_OR", "C220_KEY_EQ", "C220_KEY_NE", "'<'",
  "C220_KEY_LE", "C220_KEY_GE", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'&'", "'!'", "C220_KEY_POST_ID", "'['", "']'", "';'", "','", "'{'",
  "'}'", "'('", "')'", "$accept", "program", "declarations", "one_decl",
  "type", "var_list", "variable", "ident", "array_sizes", "number", "main",
  "main_open", "statements", "stmt", "simple_stmt", "maybe_else",
  "compound_stmt", "maybe_expr", "expr", "var_ref", "array_index", "lval",
  "fn_call", "fn_name", "maybe_args", "arg_list", "str_or_expr", "string", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      61,   275,   276,   277,   278,    60,   279,   280,    62,    43,
      45,    42,    47,    37,    38,    33,   281,    91,    93,    59,
      44,   123,   125,    40,    41
};
# endif

#define YYPACT_NINF (-114)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-61)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -114,     6,  -114,  -114,  -114,    33,  -114,   -34,  -114,   -19,
     -35,  -114,    17,  -114,    -5,  -114,    59,    75,  -114,    77,
    -114,  -114,  -114,    48,  -114,    59,    61,  -114,    44,    45,
     153,  -114,  -114,  -114,  -114,    59,    59,    47,   153,    59,
     153,  -114,  -114,   153,    55,  -114,  -114,  -114,  -114,    54,
     280,     5,    41,  -114,    51,   153,   153,   224,  -114,  -114,
    -114,    75,  -114,  -114,  -114,    96,   176,   153,  -114,  -114,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,  -114,  -114,   153,   129,    58,   200,  -114,
      57,  -114,  -114,   262,   291,   291,     2,     2,   -13,   -13,
     -13,   -13,    35,    35,  -114,  -114,  -114,   280,  -114,   280,
      70,    76,  -114,  -114,   153,    78,  -114,  -114,  -114,   129,
     243,    90,  -114,   153,    78,  -114,    73,  -114,    78,  -114
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     8,     3,     0,     4,     0,    12,     0,
       0,     9,    13,     5,     0,     7,     0,     0,    11,    18,
      17,    10,    15,     0,     6,     0,    30,    14,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,     0,
       0,    18,    16,     0,    58,    55,    19,    20,    21,     0,
      31,    37,     0,    39,     0,    30,     0,     0,    60,    35,
      36,     0,    40,    38,    41,    30,     0,     0,    57,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,    34,     0,    66,     0,     0,    25,
       0,    29,    56,     0,    53,    54,    50,    47,    48,    49,
      51,    52,    42,    43,    44,    45,    46,    32,    72,    70,
       0,    67,    68,    71,     0,     0,    26,    59,    61,     0,
       0,    27,    69,    30,     0,    23,     0,    28,     0,    24
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -114,  -114,  -114,    99,   101,  -114,   105,    -2,  -114,    -8,
    -114,  -114,    81,  -114,  -114,  -114,  -113,   -54,   -30,    24,
    -114,    46,  -114,  -114,  -114,  -114,     4,  -114
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    19,     4,     5,    10,    11,    44,    18,    45,
       6,     7,    26,    46,    47,   125,    48,    49,    50,    51,
      68,    52,    53,    54,   110,   111,   112,   113
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    87,   121,    12,    15,    16,     2,    13,    62,    23,
      64,   127,     3,    66,    12,   129,    78,    79,    80,    81,
      82,   -60,   -60,    12,    14,   -60,    88,    74,    75,    76,
      77,    78,    79,    80,    81,    82,     8,    93,     9,    20,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,    90,    17,   107,   109,    83,    84,    58,
      58,    85,     8,    58,     8,    22,    80,    81,    82,   126,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    22,
      37,    59,    60,     3,   120,    63,    27,    55,    56,   109,
      61,    38,    67,    69,    86,    39,    40,   114,   124,     8,
      22,   116,    41,    42,    43,    28,    29,    30,    31,    32,
      33,    34,    35,    36,   118,    37,   119,   128,    24,    41,
      25,    21,    65,   122,     0,     0,    38,     0,     0,     0,
      39,    40,     8,    22,     0,     0,   108,    41,    91,    43,
       0,    31,    32,    33,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     8,    22,     0,    38,
       0,     0,     0,    39,    40,    31,    32,    33,    34,    35,
      36,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,     0,     0,     0,    39,    40,     0,
       0,     0,     0,     0,     0,     0,    43,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,     0,     0,
       0,     0,     0,    89,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,     0,     0,
       0,     0,   123,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,     0,     0,     0,     0,
     117,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82
};

static const yytype_int16 yycheck[] =
{
      30,    55,   115,     5,    39,    40,     0,    41,    38,    17,
      40,   124,     6,    43,    16,   128,    29,    30,    31,    32,
      33,    16,    17,    25,    43,    20,    56,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     3,    67,     5,    44,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    61,    37,    85,    86,    16,    17,    35,
      36,    20,     3,    39,     3,     4,    31,    32,    33,   123,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     4,
      19,    35,    36,     6,   114,    39,    38,    43,    43,   119,
      43,    30,    37,    39,    43,    34,    35,    39,     8,     3,
       4,    44,    41,    42,    43,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    44,    19,    40,    44,    19,    41,
      19,    16,    41,   119,    -1,    -1,    30,    -1,    -1,    -1,
      34,    35,     3,     4,    -1,    -1,     7,    41,    42,    43,
      -1,    12,    13,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,    -1,    30,
      -1,    -1,    -1,    34,    35,    12,    13,    14,    15,    16,
      17,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    39,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    -1,    -1,
      -1,    -1,    39,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
      38,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,     0,     6,    48,    49,    55,    56,     3,     5,
      50,    51,    52,    41,    43,    39,    40,    37,    53,    47,
      44,    51,     4,    54,    48,    49,    57,    38,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    19,    30,    34,
      35,    41,    42,    43,    52,    54,    58,    59,    61,    62,
      63,    64,    66,    67,    68,    43,    43,    63,    64,    66,
      66,    43,    63,    66,    63,    57,    63,    37,    65,    39,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    16,    17,    20,    43,    62,    63,    39,
      54,    42,    44,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,     7,    63,
      69,    70,    71,    72,    39,    44,    44,    38,    44,    40,
      63,    61,    71,    39,     8,    60,    62,    61,    44,    61
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    46,    46,    47,    47,    48,    49,    50,
      50,    51,    52,    53,    53,    54,    55,    56,    57,    57,
      58,    58,    59,    59,    59,    59,    59,    60,    60,    61,
      62,    62,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    64,    65,    65,
      66,    67,    68,    68,    68,    68,    69,    69,    70,    70,
      71,    71,    72
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     2,     3,     1,     1,
       3,     2,     1,     0,     3,     1,     5,     4,     0,     2,
       1,     1,     2,     6,     9,     3,     4,     0,     2,     3,
       0,     1,     3,     2,     2,     2,     2,     1,     2,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     2,     0,     3,
       1,     4,     1,     1,     1,     1,     0,     1,     1,     3,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4:
#line 90 "ece220_parse.y"
                             {ece220_main = yyvsp[0];}
#line 1486 "ece220_parse.tab.c"
    break;

  case 11:
#line 105 "ece220_parse.y"
                                  { var_create (yyvsp[-1], yyvsp[0]); yyval = NULL;}
#line 1492 "ece220_parse.tab.c"
    break;

  case 12:
#line 107 "ece220_parse.y"
                               {yyval = new_ast_str (ece220token.name);}
#line 1498 "ece220_parse.tab.c"
    break;

  case 13:
#line 109 "ece220_parse.y"
                            {yyval = new_ast_num (0);}
#line 1504 "ece220_parse.tab.c"
    break;

  case 14:
#line 110 "ece220_parse.y"
                               {yyval = yyvsp[-1];}
#line 1510 "ece220_parse.tab.c"
    break;

  case 15:
#line 113 "ece220_parse.y"
                             { yyval = new_ast_num (ece220token.ival); }
#line 1516 "ece220_parse.tab.c"
    break;

  case 16:
#line 115 "ece220_parse.y"
                                                          {
		    reading_globals = 0;
		    yyval = yyvsp[-1];
		}
#line 1525 "ece220_parse.tab.c"
    break;

  case 17:
#line 121 "ece220_parse.y"
                                           {start_main ();}
#line 1531 "ece220_parse.tab.c"
    break;

  case 18:
#line 123 "ece220_parse.y"
                            {yyval = NULL;}
#line 1537 "ece220_parse.tab.c"
    break;

  case 19:
#line 124 "ece220_parse.y"
                                {
		    /* slow for long code, but simple... */
		    if (NULL == yyvsp[-1]) {
		        yyval = yyvsp[0];
		    } else {
			ast220_t* find;

			for (find = yyvsp[-1]; NULL != find->next; find = find->next);
			find->next = yyvsp[0];
			yyval = yyvsp[-1];
		    }
		}
#line 1554 "ece220_parse.tab.c"
    break;

  case 20:
#line 138 "ece220_parse.y"
                            {yyval = yyvsp[0];}
#line 1560 "ece220_parse.tab.c"
    break;

  case 21:
#line 139 "ece220_parse.y"
                              {yyval = yyvsp[0];}
#line 1566 "ece220_parse.tab.c"
    break;

  case 22:
#line 142 "ece220_parse.y"
                               {yyval = yyvsp[-1];}
#line 1572 "ece220_parse.tab.c"
    break;

  case 23:
#line 143 "ece220_parse.y"
                                                                  {
	            yyval = new_ast_node ();
		    yyval->type = AST220_IF_STMT;
		    yyval->test = yyvsp[-3];
		    yyval->left = yyvsp[-1];
		    yyval->right = yyvsp[0];
		}
#line 1584 "ece220_parse.tab.c"
    break;

  case 24:
#line 151 "ece220_parse.y"
                                                          { 
	            yyval = new_ast_node ();
		    yyval->type = AST220_FOR_STMT;
		    yyval->test = yyvsp[-4];
		    yyval->left = yyvsp[-6];
		    yyval->middle = yyvsp[0];
		    yyval->right = yyvsp[-2];
		}
#line 1597 "ece220_parse.tab.c"
    break;

  case 25:
#line 159 "ece220_parse.y"
                                         {
	            yyval = new_ast_node ();
		    yyval->type = AST220_RETURN_STMT;
		    yyval->left = yyvsp[-1];
	        }
#line 1607 "ece220_parse.tab.c"
    break;

  case 26:
#line 164 "ece220_parse.y"
                                                     {
	            yyval = yyvsp[-1];
		    yyval->type = AST220_DEBUG_MARKER;
	        }
#line 1616 "ece220_parse.tab.c"
    break;

  case 27:
#line 170 "ece220_parse.y"
                            {yyval = NULL;}
#line 1622 "ece220_parse.tab.c"
    break;

  case 28:
#line 171 "ece220_parse.y"
                                            {yyval = yyvsp[0];}
#line 1628 "ece220_parse.tab.c"
    break;

  case 29:
#line 174 "ece220_parse.y"
                                   {yyval = yyvsp[-1];}
#line 1634 "ece220_parse.tab.c"
    break;

  case 30:
#line 176 "ece220_parse.y"
                            {yyval = NULL;}
#line 1640 "ece220_parse.tab.c"
    break;

  case 31:
#line 177 "ece220_parse.y"
                     {
	      	    yyval = new_ast_node (); 
		    yyval->type = AST220_POP_STACK; 
		    yyval->left = yyvsp[0];
		}
#line 1650 "ece220_parse.tab.c"
    break;

  case 32:
#line 184 "ece220_parse.y"
                              {
	            yyval = new_ast_node ();
		    yyval->type = AST220_OP_ASSIGN;
		    yyval->left = yyvsp[-2];
		    yyval->right = yyvsp[0];
		}
#line 1661 "ece220_parse.tab.c"
    break;

  case 33:
#line 190 "ece220_parse.y"
                                                          {
	            yyval = new_ast_op (AST220_OP_POST_INCR, yyvsp[-1], NULL);
	        }
#line 1669 "ece220_parse.tab.c"
    break;

  case 34:
#line 193 "ece220_parse.y"
                                                          {
	            yyval = new_ast_op (AST220_OP_POST_DECR, yyvsp[-1], NULL);
	        }
#line 1677 "ece220_parse.tab.c"
    break;

  case 35:
#line 196 "ece220_parse.y"
                                   {
	            yyval = new_ast_op (AST220_OP_PRE_INCR, yyvsp[0], NULL);
	        }
#line 1685 "ece220_parse.tab.c"
    break;

  case 36:
#line 199 "ece220_parse.y"
                                   {
	            yyval = new_ast_op (AST220_OP_PRE_DECR, yyvsp[0], NULL);
	        }
#line 1693 "ece220_parse.tab.c"
    break;

  case 37:
#line 202 "ece220_parse.y"
                        {yyval = yyvsp[0];}
#line 1699 "ece220_parse.tab.c"
    break;

  case 38:
#line 203 "ece220_parse.y"
                         {
	            yyval = new_ast_node ();
		    yyval->type = AST220_GET_ADDRESS;
		    yyval->left = yyvsp[0];
		}
#line 1709 "ece220_parse.tab.c"
    break;

  case 39:
#line 208 "ece220_parse.y"
                        {yyval = yyvsp[0];}
#line 1715 "ece220_parse.tab.c"
    break;

  case 40:
#line 209 "ece220_parse.y"
                                                  {
	            yyval = new_ast_op (AST220_OP_NEGATE, yyvsp[0], NULL);
	        }
#line 1723 "ece220_parse.tab.c"
    break;

  case 41:
#line 212 "ece220_parse.y"
                         {
	            yyval = new_ast_op (AST220_OP_LOG_NOT, yyvsp[0], NULL);
	        }
#line 1731 "ece220_parse.tab.c"
    break;

  case 42:
#line 215 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_OP_ADD, yyvsp[-2], yyvsp[0]);
	        }
#line 1739 "ece220_parse.tab.c"
    break;

  case 43:
#line 218 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_OP_SUB, yyvsp[-2], yyvsp[0]);
	        }
#line 1747 "ece220_parse.tab.c"
    break;

  case 44:
#line 221 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_OP_MULT, yyvsp[-2], yyvsp[0]);
	        }
#line 1755 "ece220_parse.tab.c"
    break;

  case 45:
#line 224 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_OP_DIV, yyvsp[-2], yyvsp[0]);
	        }
#line 1763 "ece220_parse.tab.c"
    break;

  case 46:
#line 227 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_OP_MOD, yyvsp[-2], yyvsp[0]);
	        }
#line 1771 "ece220_parse.tab.c"
    break;

  case 47:
#line 230 "ece220_parse.y"
                                      {
	            yyval = new_ast_op (AST220_CMP_NE, yyvsp[-2], yyvsp[0]);
	        }
#line 1779 "ece220_parse.tab.c"
    break;

  case 48:
#line 233 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_CMP_LESS, yyvsp[-2], yyvsp[0]);
	        }
#line 1787 "ece220_parse.tab.c"
    break;

  case 49:
#line 236 "ece220_parse.y"
                                      {
	            yyval = new_ast_op (AST220_CMP_LE, yyvsp[-2], yyvsp[0]);
	        }
#line 1795 "ece220_parse.tab.c"
    break;

  case 50:
#line 239 "ece220_parse.y"
                                      {
	            yyval = new_ast_op (AST220_CMP_EQ, yyvsp[-2], yyvsp[0]);
	        }
#line 1803 "ece220_parse.tab.c"
    break;

  case 51:
#line 242 "ece220_parse.y"
                                      {
	            yyval = new_ast_op (AST220_CMP_GE, yyvsp[-2], yyvsp[0]);
	        }
#line 1811 "ece220_parse.tab.c"
    break;

  case 52:
#line 245 "ece220_parse.y"
                              {
	            yyval = new_ast_op (AST220_CMP_GREATER, yyvsp[-2], yyvsp[0]);
	        }
#line 1819 "ece220_parse.tab.c"
    break;

  case 53:
#line 248 "ece220_parse.y"
                                       {
	            yyval = new_ast_op (AST220_OP_LOG_AND, yyvsp[-2], yyvsp[0]);
	        }
#line 1827 "ece220_parse.tab.c"
    break;

  case 54:
#line 251 "ece220_parse.y"
                                      {
	            yyval = new_ast_op (AST220_OP_LOG_OR, yyvsp[-2], yyvsp[0]);
	        }
#line 1835 "ece220_parse.tab.c"
    break;

  case 55:
#line 254 "ece220_parse.y"
                       {yyval = yyvsp[0]; yyval->type = AST220_PUSH_INT;}
#line 1841 "ece220_parse.tab.c"
    break;

  case 56:
#line 255 "ece220_parse.y"
                             {yyval = yyvsp[-1];}
#line 1847 "ece220_parse.tab.c"
    break;

  case 57:
#line 258 "ece220_parse.y"
                                  {
		    symtab_entry_t* entry = symtab_lookup (yyvsp[-1]->name);

		    if (NULL == entry ||
			(0 == entry->array_len && NULL != yyvsp[0]) ||
			(0 != entry->array_len && NULL == yyvsp[0])) {
			ece220errorf ("invalid variable access");
			/* should discard AST nodes here... */
			yyval = NULL;
		    } else {
			yyval = yyvsp[-1]; 
			yyval->type = AST220_VARIABLE;
			yyval->left = yyvsp[0];
		    }
		}
#line 1867 "ece220_parse.tab.c"
    break;

  case 58:
#line 275 "ece220_parse.y"
                            {yyval = NULL;}
#line 1873 "ece220_parse.tab.c"
    break;

  case 59:
#line 276 "ece220_parse.y"
                             {yyval = yyvsp[-1];}
#line 1879 "ece220_parse.tab.c"
    break;

  case 60:
#line 279 "ece220_parse.y"
                        {yyval = yyvsp[0];}
#line 1885 "ece220_parse.tab.c"
    break;

  case 61:
#line 281 "ece220_parse.y"
                                           {
		    yyval = yyvsp[-3]; 
		    yyval->type = AST220_FUNC_CALL;
		    yyval->left = yyvsp[-1];
		}
#line 1895 "ece220_parse.tab.c"
    break;

  case 62:
#line 288 "ece220_parse.y"
                                {yyval = new_ast_func (AST220_PRINTF);}
#line 1901 "ece220_parse.tab.c"
    break;

  case 63:
#line 289 "ece220_parse.y"
                                {yyval = new_ast_func (AST220_RAND);}
#line 1907 "ece220_parse.tab.c"
    break;

  case 64:
#line 290 "ece220_parse.y"
                                {yyval = new_ast_func (AST220_SCANF);}
#line 1913 "ece220_parse.tab.c"
    break;

  case 65:
#line 291 "ece220_parse.y"
                                {yyval = new_ast_func (AST220_SRAND);}
#line 1919 "ece220_parse.tab.c"
    break;

  case 66:
#line 294 "ece220_parse.y"
                            {yyval = NULL;}
#line 1925 "ece220_parse.tab.c"
    break;

  case 67:
#line 295 "ece220_parse.y"
                         {yyval = yyvsp[0];}
#line 1931 "ece220_parse.tab.c"
    break;

  case 68:
#line 298 "ece220_parse.y"
                            {yyval = yyvsp[0];}
#line 1937 "ece220_parse.tab.c"
    break;

  case 69:
#line 299 "ece220_parse.y"
                                         {
	            yyvsp[0]->next = yyvsp[-2];
		    yyval = yyvsp[0];
		}
#line 1946 "ece220_parse.tab.c"
    break;

  case 70:
#line 305 "ece220_parse.y"
                     {yyval = yyvsp[0];}
#line 1952 "ece220_parse.tab.c"
    break;

  case 71:
#line 306 "ece220_parse.y"
                       {yyval = yyvsp[0]; yyval->type = AST220_PUSH_STR;}
#line 1958 "ece220_parse.tab.c"
    break;

  case 72:
#line 309 "ece220_parse.y"
                                {yyval = new_ast_str (ece220token.name);}
#line 1964 "ece220_parse.tab.c"
    break;


#line 1968 "ece220_parse.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 311 "ece220_parse.y"


ast220_t*
ece220_parse_program ()
{
    ece220_reset_lexer ();
    ece220_main = NULL;
    reading_globals = 1;
    global_offset = 0;
    local_offset = 0;
    has_main = 0;
    ece220parse ();
    return ece220_main;
}

static void
var_create (ast220_t* name, ast220_t* a_len)
{
    int32_t var_size;
    symtab_entry_t* entry;

    entry = symtab_create (name->name);
    if (NULL == entry) {
	ece220errorf ("can't create variable: %s", name->name);
    } else {
	entry->array_len = a_len->value;
	entry->is_global = reading_globals;
	var_size = (a_len->value == 0 ? 1 : a_len->value);
	if (reading_globals) {
	    entry->offset = global_offset;
	    global_offset += var_size;
	} else {
	    entry->offset = local_offset - (var_size - 1);
	    local_offset -= var_size;
	}
    }
    free (name->name);
    free (name);
    free (a_len);
}

static void 
start_main ()
{
    reading_globals = 0;
    if (has_main) {
        ece220errorf ("multiple main functions");
    } else {
        has_main = 1;
    }
}

static ast220_t* 
new_ast_node ()
{
    ast220_t* node;

    /* not good practice to terminate in the middle of the program... */
    if (NULL == (node = malloc (sizeof (ast220_t)))) {
        perror ("panic");
	exit (3);
    }
    return node;
}

static ast220_t* 
new_ast_num (int32_t value)
{
    ast220_t* node;

    node = new_ast_node ();
    node->value = value;
    return node;
}

static ast220_t* 
new_ast_str (char* str)
{
    ast220_t* node;

    node = new_ast_node ();
    node->name = str;
    return node;
}

static ast220_t* 
new_ast_func (ast220_builtin_func_t id)
{
    ast220_t* node;

    node = new_ast_node ();
    node->fnum = id;
    return node;
}

static ast220_t* 
new_ast_op (ast220_type_t type, ast220_t* left, ast220_t* right)
{
    ast220_t* node;

    node = new_ast_node ();
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

int32_t
ece220_local_var_space ()
{
    return -local_offset;
}

int32_t
ece220_global_var_space ()
{
    return global_offset;
}

