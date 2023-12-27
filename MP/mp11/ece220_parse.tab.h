/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
