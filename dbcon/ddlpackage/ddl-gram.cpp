/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         ddlparse
#define yylex           ddllex
#define yyerror         ddlerror
#define yylval          ddllval
#define yychar          ddlchar
#define yydebug         ddldebug
#define yynerrs         ddlnerrs

/* Copy the first part of user declarations.  */


#include "sqlparser.h"

#ifdef _MSC_VER
#include "ddl-gram-win.h"
#else
#include "ddl-gram.h"
#endif

#define scanner x->scanner

using namespace std;
using namespace ddlpackage;	

int ddllex(YYSTYPE* ddllval, void* yyscanner);
void ddlerror(struct pass_to_bison* x, char const *s);
char* copy_string(const char *str);



# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "ddl-gram-temp.hpp".  */
#ifndef YY_DDL_DDL_GRAM_TEMP_HPP_INCLUDED
# define YY_DDL_DDL_GRAM_TEMP_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ddldebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ACTION = 258,
     ADD = 259,
     ALTER = 260,
     AUTO_INCREMENT = 261,
     BIGINT = 262,
     BIT = 263,
     ERYDB_BLOB = 264,
     CASCADE = 265,
     ERYDB_CHAR = 266,
     CHARACTER = 267,
     CHECK = 268,
     CLOB = 269,
     COLUMN = 270,
     COLUMNS = 271,
     COMMENT = 272,
     CONSTRAINT = 273,
     CONSTRAINTS = 274,
     CREATE = 275,
     CURRENT_USER = 276,
     DATETIME = 277,
     DEC = 278,
     DECIMAL = 279,
     DEFAULT = 280,
     DEFERRABLE = 281,
     DEFERRED = 282,
     ERYDB_DELETE = 283,
     DROP = 284,
     ENGINE = 285,
     FOREIGN = 286,
     FULL = 287,
     IMMEDIATE = 288,
     INDEX = 289,
     INITIALLY = 290,
     ERYDB_INT = 291,
     INTEGER = 292,
     KEY = 293,
     MATCH = 294,
     MAX_ROWS = 295,
     MIN_ROWS = 296,
     MODIFY = 297,
     NO = 298,
     NOT = 299,
     NULL_TOK = 300,
     NUMBER = 301,
     NUMERIC = 302,
     ON = 303,
     PARTIAL = 304,
     PRECISION = 305,
     PRIMARY = 306,
     REFERENCES = 307,
     RENAME = 308,
     RESTRICT = 309,
     SET = 310,
     SMALLINT = 311,
     TABLE = 312,
     TIME = 313,
     TINYINT = 314,
     TO = 315,
     UNIQUE = 316,
     UNSIGNED = 317,
     UPDATE = 318,
     USER = 319,
     SESSION_USER = 320,
     SYSTEM_USER = 321,
     VARCHAR = 322,
     VARBINARY = 323,
     VARYING = 324,
     WITH = 325,
     ZONE = 326,
     DOUBLE = 327,
     ERYDB_FLOAT = 328,
     REAL = 329,
     CHARSET = 330,
     ERYDB_IF = 331,
     EXISTS = 332,
     CHANGE = 333,
     TRUNCATE = 334,
     IDENT = 335,
     FCONST = 336,
     SCONST = 337,
     CP_SEARCH_CONDITION_TEXT = 338,
     ICONST = 339,
     DATE = 340
   };
#endif
/* Tokens.  */
#define ACTION 258
#define ADD 259
#define ALTER 260
#define AUTO_INCREMENT 261
#define BIGINT 262
#define BIT 263
#define ERYDB_BLOB 264
#define CASCADE 265
#define ERYDB_CHAR 266
#define CHARACTER 267
#define CHECK 268
#define CLOB 269
#define COLUMN 270
#define COLUMNS 271
#define COMMENT 272
#define CONSTRAINT 273
#define CONSTRAINTS 274
#define CREATE 275
#define CURRENT_USER 276
#define DATETIME 277
#define DEC 278
#define DECIMAL 279
#define DEFAULT 280
#define DEFERRABLE 281
#define DEFERRED 282
#define ERYDB_DELETE 283
#define DROP 284
#define ENGINE 285
#define FOREIGN 286
#define FULL 287
#define IMMEDIATE 288
#define INDEX 289
#define INITIALLY 290
#define ERYDB_INT 291
#define INTEGER 292
#define KEY 293
#define MATCH 294
#define MAX_ROWS 295
#define MIN_ROWS 296
#define MODIFY 297
#define NO 298
#define NOT 299
#define NULL_TOK 300
#define NUMBER 301
#define NUMERIC 302
#define ON 303
#define PARTIAL 304
#define PRECISION 305
#define PRIMARY 306
#define REFERENCES 307
#define RENAME 308
#define RESTRICT 309
#define SET 310
#define SMALLINT 311
#define TABLE 312
#define TIME 313
#define TINYINT 314
#define TO 315
#define UNIQUE 316
#define UNSIGNED 317
#define UPDATE 318
#define USER 319
#define SESSION_USER 320
#define SYSTEM_USER 321
#define VARCHAR 322
#define VARBINARY 323
#define VARYING 324
#define WITH 325
#define ZONE 326
#define DOUBLE 327
#define ERYDB_FLOAT 328
#define REAL 329
#define CHARSET 330
#define ERYDB_IF 331
#define EXISTS 332
#define CHANGE 333
#define TRUNCATE 334
#define IDENT 335
#define FCONST 336
#define SCONST 337
#define CP_SEARCH_CONDITION_TEXT 338
#define ICONST 339
#define DATE 340



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


  ddlpackage::AlterTableStatement *alterTableStmt;
  ddlpackage::AlterTableAction *ata;
  ddlpackage::AlterTableActionList *ataList;
  ddlpackage::DDL_CONSTRAINT_ATTRIBUTES cattr;
  std::pair<std::string, std::string> *tableOption;
  const char *columnOption;
  ddlpackage::ColumnConstraintDef *columnConstraintDef;
  ddlpackage::ColumnNameList *columnNameList;
  ddlpackage::ColumnType* columnType;
  ddlpackage::ConstraintAttributes *constraintAttributes;
  ddlpackage::ColumnConstraintList *constraintList;
  ddlpackage::DDL_CONSTRAINTS constraintType;
  double dval;
  bool flag;
  int ival;
  ddlpackage::QualifiedName *qualifiedName;
  ddlpackage::SchemaObject *schemaObject;
  ddlpackage::SqlStatement *sqlStmt;
  ddlpackage::SqlStatementList *sqlStmtList;
  const char *str;
  ddlpackage::TableConstraintDef *tableConstraint;
  ddlpackage::TableElementList *tableElementList;
  ddlpackage::TableOptionMap *tableOptionMap;
  ddlpackage::ColumnDefaultValue *colDefault;
  ddlpackage::DDL_MATCH_TYPE matchType;
  ddlpackage::DDL_REFERENTIAL_ACTION refActionCode;
  ddlpackage::ReferentialAction *refAction;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int ddlparse (void *YYPARSE_PARAM);
#else
int ddlparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int ddlparse (struct ddlpackage::pass_to_bison * x);
#else
int ddlparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_DDL_DDL_GRAM_TEMP_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */






#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  23
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   403

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  93
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  78
/* YYNRULES -- Number of rules.  */
#define YYNRULES  235
/* YYNRULES -- Number of states.  */
#define YYNSTATES  403

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   340

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    92,
      87,    88,     2,     2,    89,     2,    91,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    86,
       2,    90,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    11,    13,    15,    17,    19,
      21,    23,    24,    29,    36,    39,    40,    44,    54,    65,
      67,    68,    77,    85,    89,    90,    94,    97,    99,   103,
     105,   107,   112,   116,   118,   119,   121,   123,   125,   130,
     132,   136,   139,   141,   154,   156,   157,   160,   162,   164,
     166,   167,   170,   173,   175,   176,   178,   179,   183,   187,
     189,   192,   195,   198,   200,   202,   205,   206,   208,   212,
     216,   220,   224,   227,   231,   236,   242,   247,   249,   253,
     256,   258,   260,   262,   264,   266,   268,   270,   272,   274,
     278,   281,   285,   290,   295,   301,   307,   314,   320,   327,
     334,   342,   348,   355,   362,   370,   377,   385,   392,   400,
     408,   417,   425,   434,   439,   442,   445,   449,   451,   455,
     457,   460,   464,   469,   475,   477,   479,   481,   484,   488,
     493,   499,   504,   509,   515,   522,   528,   535,   541,   542,
     544,   547,   550,   553,   556,   559,   562,   564,   566,   568,
     570,   572,   574,   576,   579,   582,   587,   589,   590,   593,
     596,   598,   599,   601,   604,   607,   610,   612,   615,   617,
     619,   624,   628,   630,   632,   637,   642,   648,   654,   659,
     664,   666,   668,   671,   675,   678,   682,   685,   689,   692,
     695,   698,   701,   704,   708,   712,   716,   720,   724,   728,
     734,   735,   739,   740,   743,   747,   751,   756,   759,   763,
     766,   770,   774,   780,   781,   783,   785,   787,   789,   791,
     795,   799,   800,   804,   805,   809,   814,   820,   825,   831,
     833,   835,   836,   842,   848,   850
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      94,     0,    -1,    95,    -1,    95,    86,    96,    -1,    96,
      -1,   126,    -1,   102,    -1,    99,    -1,    97,    -1,   100,
      -1,   104,    -1,    -1,    29,    57,    98,   136,    -1,    29,
      57,    98,   136,    10,    19,    -1,    76,    77,    -1,    -1,
      29,    34,   136,    -1,    20,    34,   136,    48,   136,    87,
     111,    88,   101,    -1,    20,    61,    34,   136,    48,   136,
      87,   111,    88,   101,    -1,   123,    -1,    -1,    20,    57,
     103,   135,    87,   105,    88,   123,    -1,    20,    57,   103,
     135,    87,   105,    88,    -1,    76,    44,    77,    -1,    -1,
      79,    57,   136,    -1,    79,   136,    -1,   106,    -1,   105,
      89,   106,    -1,   141,    -1,   107,    -1,    18,   108,   109,
     147,    -1,   108,   109,   147,    -1,   139,    -1,    -1,   110,
      -1,   113,    -1,   153,    -1,   112,    87,   111,    88,    -1,
     138,    -1,   111,    89,   138,    -1,    51,    38,    -1,    61,
      -1,    31,    38,    87,   111,    88,    52,   135,    87,   111,
      88,   114,   116,    -1,   115,    -1,    -1,    39,   115,    -1,
      32,    -1,    49,    -1,   117,    -1,    -1,   120,   118,    -1,
     121,   119,    -1,   121,    -1,    -1,   120,    -1,    -1,    48,
      63,   122,    -1,    48,    28,   122,    -1,    10,    -1,    55,
      45,    -1,    55,    25,    -1,    43,     3,    -1,    54,    -1,
     125,    -1,   123,   125,    -1,    -1,    90,    -1,    30,   124,
      80,    -1,    40,   124,    84,    -1,    41,   124,    84,    -1,
      17,   124,   154,    -1,    17,   154,    -1,     6,   124,    84,
      -1,    25,    75,   124,    80,    -1,    25,    11,    55,   124,
      80,    -1,     5,    57,   135,   127,    -1,   128,    -1,   127,
      89,   128,    -1,   127,   128,    -1,   137,    -1,   167,    -1,
     169,    -1,   133,    -1,   132,    -1,   134,    -1,   130,    -1,
     131,    -1,   129,    -1,    17,   124,   154,    -1,    17,   154,
      -1,    42,   138,   144,    -1,    42,    15,   138,   144,    -1,
      78,   138,   138,   144,    -1,    78,   138,   138,   144,   140,
      -1,    78,    15,   138,   138,   144,    -1,    78,    15,   138,
     138,   144,   140,    -1,    78,   138,   138,   144,   145,    -1,
      78,    15,   138,   138,   144,   145,    -1,    78,   138,   138,
     144,   145,   140,    -1,    78,    15,   138,   138,   144,   145,
     140,    -1,    78,   138,   138,   144,   143,    -1,    78,    15,
     138,   138,   144,   143,    -1,    78,   138,   138,   144,   143,
     140,    -1,    78,    15,   138,   138,   144,   143,   140,    -1,
      78,   138,   138,   144,   145,   143,    -1,    78,    15,   138,
     138,   144,   145,   143,    -1,    78,   138,   138,   144,   143,
     145,    -1,    78,    15,   138,   138,   144,   143,   145,    -1,
      78,   138,   138,   144,   145,   143,   140,    -1,    78,    15,
     138,   138,   144,   145,   143,   140,    -1,    78,   138,   138,
     144,   143,   145,   140,    -1,    78,    15,   138,   138,   144,
     143,   145,   140,    -1,    29,    18,   139,   168,    -1,     4,
     107,    -1,    53,   135,    -1,    53,    60,   135,    -1,   136,
      -1,    80,    91,    80,    -1,    80,    -1,     4,   141,    -1,
       4,    15,   141,    -1,     4,    87,   105,    88,    -1,     4,
      15,    87,   105,    88,    -1,    85,    -1,    80,    -1,    80,
      -1,    17,   154,    -1,   138,   144,   142,    -1,   138,   144,
     142,   145,    -1,   138,   144,   142,   143,   145,    -1,   138,
     144,   142,   143,    -1,   138,   144,   142,   140,    -1,   138,
     144,   142,   145,   140,    -1,   138,   144,   142,   143,   145,
     140,    -1,   138,   144,   142,   145,   143,    -1,   138,   144,
     142,   145,   143,   140,    -1,   138,   144,   142,   143,   140,
      -1,    -1,    45,    -1,    25,   163,    -1,    25,    45,    -1,
      25,    64,    -1,    25,    21,    -1,    25,    65,    -1,    25,
      66,    -1,   155,    -1,   156,    -1,   157,    -1,   164,    -1,
       9,    -1,    14,    -1,   146,    -1,   145,   146,    -1,   152,
     147,    -1,    18,   139,   152,   147,    -1,   148,    -1,    -1,
     151,   149,    -1,    26,   151,    -1,   150,    -1,    -1,    26,
      -1,    35,    27,    -1,    35,    33,    -1,    44,    45,    -1,
      61,    -1,    51,    38,    -1,     6,    -1,   153,    -1,    13,
      87,    83,    88,    -1,    92,    82,    92,    -1,    12,    -1,
      11,    -1,    12,    87,    84,    88,    -1,    11,    87,    84,
      88,    -1,    12,    69,    87,    84,    88,    -1,    11,    69,
      87,    84,    88,    -1,    67,    87,    84,    88,    -1,    68,
      87,    84,    88,    -1,   158,    -1,   161,    -1,    47,   159,
      -1,    47,   159,    62,    -1,    24,   159,    -1,    24,   159,
      62,    -1,    46,   159,    -1,    46,   159,    62,    -1,    37,
     160,    -1,    36,   160,    -1,    56,   160,    -1,    59,   160,
      -1,     7,   160,    -1,    37,   160,    62,    -1,    36,   160,
      62,    -1,    56,   160,    62,    -1,    59,   160,    62,    -1,
       7,   160,    62,    -1,    87,    84,    88,    -1,    87,    84,
      89,    84,    88,    -1,    -1,    87,    84,    88,    -1,    -1,
      72,   162,    -1,    72,   162,    62,    -1,    72,    50,   162,
      -1,    72,    50,   162,    62,    -1,    74,   162,    -1,    74,
     162,    62,    -1,    73,   162,    -1,    73,   162,    62,    -1,
      87,    84,    88,    -1,    87,    84,    89,    84,    88,    -1,
      -1,    84,    -1,   154,    -1,    81,    -1,    22,    -1,    85,
      -1,    58,   165,   166,    -1,    87,    84,    88,    -1,    -1,
      70,    58,    71,    -1,    -1,    29,   138,   168,    -1,    29,
      15,   138,   168,    -1,    29,    15,    87,   111,    88,    -1,
      29,    87,   111,    88,    -1,    29,    16,    87,   111,    88,
      -1,    10,    -1,    54,    -1,    -1,     5,   170,   138,    55,
     143,    -1,     5,   170,   138,    29,    25,    -1,    15,    -1,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   206,   206,   211,   221,   236,   237,   238,   239,   240,
     241,   242,   246,   247,   254,   255,   259,   265,   270,   278,
     279,   283,   288,   295,   296,   300,   301,   305,   311,   319,
     320,   324,   330,   338,   339,   343,   344,   345,   349,   359,
     364,   372,   373,   377,   384,   385,   389,   393,   394,   398,
     399,   403,   409,   418,   419,   423,   424,   428,   432,   436,
     437,   438,   439,   440,   444,   450,   459,   459,   463,   465,
     467,   469,   471,   473,   478,   480,   484,   491,   503,   508,
     516,   517,   518,   519,   520,   521,   522,   523,   524,   528,
     531,   538,   541,   547,   549,   551,   553,   555,   557,   559,
     561,   563,   565,   567,   569,   571,   573,   575,   577,   579,
     581,   583,   585,   590,   597,   601,   602,   606,   610,   611,
     623,   624,   625,   626,   630,   631,   635,   639,   642,   646,
     650,   654,   658,   662,   666,   670,   674,   678,   684,   687,
     691,   695,   696,   697,   698,   699,   703,   704,   705,   706,
     707,   712,   721,   726,   734,   745,   760,   761,   765,   770,
     784,   785,   789,   800,   801,   805,   806,   807,   808,   809,
     813,   817,   821,   826,   831,   836,   841,   847,   853,   861,
     869,   870,   874,   880,   886,   892,   898,   904,   910,   915,
     920,   925,   930,   935,   940,   945,   950,   955,   963,   964,
     965,   969,   970,   974,   979,   984,   989,   994,   999,  1004,
    1009,  1017,  1019,  1020,  1024,  1025,  1026,  1030,  1036,  1042,
    1051,  1052,  1056,  1057,  1061,  1062,  1063,  1064,  1065,  1069,
    1070,  1071,  1075,  1076,  1080,  1081
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ACTION", "ADD", "ALTER",
  "AUTO_INCREMENT", "BIGINT", "BIT", "ERYDB_BLOB", "CASCADE", "ERYDB_CHAR",
  "CHARACTER", "CHECK", "CLOB", "COLUMN", "COLUMNS", "COMMENT",
  "CONSTRAINT", "CONSTRAINTS", "CREATE", "CURRENT_USER", "DATETIME", "DEC",
  "DECIMAL", "DEFAULT", "DEFERRABLE", "DEFERRED", "ERYDB_DELETE", "DROP",
  "ENGINE", "FOREIGN", "FULL", "IMMEDIATE", "INDEX", "INITIALLY",
  "ERYDB_INT", "INTEGER", "KEY", "MATCH", "MAX_ROWS", "MIN_ROWS", "MODIFY",
  "NO", "NOT", "NULL_TOK", "NUMBER", "NUMERIC", "ON", "PARTIAL",
  "PRECISION", "PRIMARY", "REFERENCES", "RENAME", "RESTRICT", "SET",
  "SMALLINT", "TABLE", "TIME", "TINYINT", "TO", "UNIQUE", "UNSIGNED",
  "UPDATE", "USER", "SESSION_USER", "SYSTEM_USER", "VARCHAR", "VARBINARY",
  "VARYING", "WITH", "ZONE", "DOUBLE", "ERYDB_FLOAT", "REAL", "CHARSET",
  "ERYDB_IF", "EXISTS", "CHANGE", "TRUNCATE", "IDENT", "FCONST", "SCONST",
  "CP_SEARCH_CONDITION_TEXT", "ICONST", "DATE", "';'", "'('", "')'", "','",
  "'='", "'.'", "'\\''", "$accept", "stmtblock", "stmtmulti", "stmt",
  "drop_table_statement", "opt_if_exists", "drop_index_statement",
  "create_index_statement", "opt_table_options", "create_table_statement",
  "opt_if_not_exists", "trunc_table_statement", "table_element_list",
  "table_element", "table_constraint_def", "opt_constraint_name",
  "table_constraint", "unique_constraint_def", "column_name_list",
  "unique_specifier", "referential_constraint_def", "opt_match_type",
  "match_type", "opt_referential_triggered_action",
  "referential_triggered_action", "opt_delete_rule", "opt_update_rule",
  "update_rule", "delete_rule", "referential_action", "table_options",
  "opt_equal", "table_option", "alter_table_statement",
  "alter_table_actions", "alter_table_action", "alter_table_comment",
  "modify_column", "rename_column", "drop_table_constraint_def",
  "add_table_constraint_def", "ata_rename_table", "table_name",
  "qualified_name", "ata_add_column", "column_name", "constraint_name",
  "column_option", "column_def", "opt_null_tok", "default_clause",
  "data_type", "column_qualifier_list", "column_constraint_def",
  "opt_constraint_attributes", "constraint_attributes",
  "opt_deferrability_clause", "deferrability_clause",
  "constraint_check_time", "column_constraint", "check_constraint_def",
  "string_literal", "character_string_type", "binary_string_type",
  "numeric_type", "exact_numeric_type", "opt_precision_scale",
  "opt_display_width", "approximate_numeric_type",
  "opt_display_precision_scale_null", "literal", "datetime_type",
  "opt_time_precision", "opt_with_time_zone", "drop_column_def",
  "drop_behavior", "alter_column_def", "opt_column", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,    59,    40,    41,    44,
      61,    46,    39
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    93,    94,    95,    95,    96,    96,    96,    96,    96,
      96,    96,    97,    97,    98,    98,    99,   100,   100,   101,
     101,   102,   102,   103,   103,   104,   104,   105,   105,   106,
     106,   107,   107,   108,   108,   109,   109,   109,   110,   111,
     111,   112,   112,   113,   114,   114,   115,   115,   115,   116,
     116,   117,   117,   118,   118,   119,   119,   120,   121,   122,
     122,   122,   122,   122,   123,   123,   124,   124,   125,   125,
     125,   125,   125,   125,   125,   125,   126,   127,   127,   127,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   129,
     129,   130,   130,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   132,   133,   134,   134,   135,   136,   136,
     137,   137,   137,   137,   138,   138,   139,   140,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   142,   142,
     143,   143,   143,   143,   143,   143,   144,   144,   144,   144,
     144,   144,   145,   145,   146,   146,   147,   147,   148,   148,
     149,   149,   150,   151,   151,   152,   152,   152,   152,   152,
     153,   154,   155,   155,   155,   155,   155,   155,   155,   156,
     157,   157,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   159,   159,
     159,   160,   160,   161,   161,   161,   161,   161,   161,   161,
     161,   162,   162,   162,   163,   163,   163,   164,   164,   164,
     165,   165,   166,   166,   167,   167,   167,   167,   167,   168,
     168,   168,   169,   169,   170,   170
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     6,     2,     0,     3,     9,    10,     1,
       0,     8,     7,     3,     0,     3,     2,     1,     3,     1,
       1,     4,     3,     1,     0,     1,     1,     1,     4,     1,
       3,     2,     1,    12,     1,     0,     2,     1,     1,     1,
       0,     2,     2,     1,     0,     1,     0,     3,     3,     1,
       2,     2,     2,     1,     1,     2,     0,     1,     3,     3,
       3,     3,     2,     3,     4,     5,     4,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     3,     4,     4,     5,     5,     6,     5,     6,     6,
       7,     5,     6,     6,     7,     6,     7,     6,     7,     7,
       8,     7,     8,     4,     2,     2,     3,     1,     3,     1,
       2,     3,     4,     5,     1,     1,     1,     2,     3,     4,
       5,     4,     4,     5,     6,     5,     6,     5,     0,     1,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     4,     1,     0,     2,     2,
       1,     0,     1,     2,     2,     2,     1,     2,     1,     1,
       4,     3,     1,     1,     4,     4,     5,     5,     4,     4,
       1,     1,     2,     3,     2,     3,     2,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     5,
       0,     3,     0,     2,     3,     3,     4,     2,     3,     2,
       3,     3,     5,     0,     1,     1,     1,     1,     1,     3,
       3,     0,     3,     0,     3,     4,     5,     4,     5,     1,
       1,     0,     5,     5,     1,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      11,     0,     0,     0,     0,     0,     2,     4,     8,     7,
       9,     6,    10,     5,     0,     0,    24,     0,     0,    15,
       0,   119,    26,     1,    11,     0,   117,     0,     0,     0,
       0,    16,     0,     0,    25,     0,     3,    34,   235,     0,
       0,     0,     0,     0,    76,    77,    88,    86,    87,    84,
      83,    85,    80,    81,    82,     0,     0,     0,     0,    14,
      12,   118,     0,    34,   125,   124,    34,   114,     0,     0,
      33,   120,   234,     0,    67,     0,     0,    90,     0,     0,
       0,   125,     0,   231,     0,     0,     0,   115,     0,     0,
       0,    79,     0,    23,    34,     0,     0,    34,   121,   126,
       0,     0,    27,    30,    29,     0,     0,     0,    42,   157,
      35,     0,    36,    37,   202,   150,   173,   172,   151,   217,
     200,   202,   202,   200,   200,   202,   221,   202,     0,     0,
     213,   213,   213,   218,   138,   146,   147,   148,   180,   181,
     149,     0,     0,    89,     0,   231,     0,   231,     0,    39,
     229,   230,   224,     0,    91,   116,     0,     0,    78,     0,
       0,     0,    13,     0,   157,   122,    34,     0,     0,    41,
       0,     0,    32,   156,   161,     0,     0,   192,     0,     0,
       0,     0,     0,   184,   189,   188,   186,   182,   190,     0,
     223,   191,     0,     0,   213,     0,   203,   209,   207,   139,
     128,     0,     0,   171,     0,   225,     0,   113,   227,     0,
      92,     0,    93,     0,    22,     0,   123,    31,    28,     0,
       0,   159,   163,   164,   162,   158,   160,     0,     0,   197,
       0,     0,     0,     0,     0,   185,   194,   193,   187,   183,
     195,     0,     0,   219,   196,     0,     0,   205,     0,   204,
     210,   208,   168,     0,     0,     0,     0,     0,   166,   132,
     131,   129,   152,   157,   169,   233,   232,   226,   228,    40,
      95,    94,   101,    97,    20,    66,     0,     0,    66,    66,
      66,    21,    64,     0,   170,     0,    38,   201,     0,   175,
       0,   174,   198,     0,   220,     0,   178,   179,   206,   211,
       0,   127,     0,   143,   141,   142,   144,   145,   216,   214,
     215,   140,   165,   167,   137,   130,   133,   135,   153,   154,
      96,   102,    98,   103,   107,    99,   105,    17,    19,     0,
       0,    72,     0,    66,     0,     0,     0,    65,    20,     0,
     177,   176,     0,   222,     0,   157,   134,   136,   104,   108,
     100,   106,   111,   109,    73,    71,    66,     0,    68,    69,
      70,    18,     0,   199,   212,   155,   112,   110,     0,    74,
       0,    75,     0,     0,    45,    47,     0,    48,    50,    44,
      46,     0,    43,    49,    54,    56,     0,     0,     0,    51,
      53,     0,    52,    55,    59,     0,    63,     0,    58,    57,
      62,    61,    60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    33,     9,    10,   327,    11,
      29,    12,   101,   102,   103,    68,   109,   110,   148,   111,
     112,   378,   379,   382,   383,   389,   392,   384,   385,   398,
     328,    76,   282,    13,    44,    45,    46,    47,    48,    49,
      50,    51,    25,    26,    52,   149,    70,   259,   104,   200,
     260,   134,   261,   262,   172,   173,   225,   226,   174,   263,
     264,    77,   135,   136,   137,   138,   183,   177,   139,   196,
     311,   140,   190,   243,    53,   152,    54,    73
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -274
static const yytype_int16 yypact[] =
{
       7,   -33,    75,    -3,   110,    39,   -12,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,    49,    49,    11,   122,    49,    66,
      49,    79,  -274,  -274,     7,   101,  -274,   156,   174,    49,
      49,  -274,   152,    49,  -274,   161,  -274,    14,   231,    76,
      10,    46,   114,    77,    99,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,    49,   171,   169,   215,  -274,
     275,  -274,   134,   217,    94,  -274,    32,  -274,   108,   139,
    -274,  -274,  -274,     4,  -274,   216,   207,  -274,   160,   213,
     217,  -274,     4,   148,     4,   139,    49,  -274,     4,     4,
     101,  -274,   214,  -274,    32,    49,   283,    32,  -274,  -274,
     108,   103,  -274,  -274,  -274,   218,   265,   266,  -274,   170,
    -274,   219,  -274,  -274,   220,  -274,   -46,    95,  -274,  -274,
     221,   220,   220,   221,   221,   220,   222,   220,   223,   224,
     -39,   226,   226,  -274,   267,  -274,  -274,  -274,  -274,  -274,
    -274,     9,   228,  -274,     4,   148,     4,   148,   162,  -274,
    -274,  -274,  -274,   139,  -274,  -274,     4,   139,  -274,     4,
     177,   229,  -274,   184,   170,  -274,    32,   232,   230,  -274,
     279,   166,  -274,  -274,   292,     4,   235,   259,   236,   238,
     237,   241,   242,   268,   269,   270,   272,   273,   274,   243,
     258,   276,   245,   253,   226,   255,   278,   280,   281,  -274,
     209,   316,   319,  -274,   186,  -274,   189,  -274,  -274,     4,
    -274,   139,   209,   191,   227,     4,  -274,  -274,  -274,   257,
       4,  -274,  -274,  -274,  -274,  -274,  -274,   193,   260,  -274,
     262,   261,   263,   264,   195,  -274,  -274,  -274,  -274,  -274,
    -274,   271,   293,  -274,  -274,   277,   282,   288,   199,  -274,
    -274,  -274,  -274,   207,   217,   144,   308,   317,  -274,  -274,
     225,   209,  -274,   170,  -274,  -274,  -274,  -274,  -274,  -274,
     209,  -274,   225,   209,   227,   284,    76,     5,   284,   284,
     284,   227,  -274,   201,  -274,   203,  -274,  -274,   285,  -274,
     287,  -274,  -274,   294,  -274,   286,  -274,  -274,  -274,  -274,
     295,  -274,   210,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,   225,  -274,   337,  -274,  -274,
    -274,   225,   209,  -274,   225,  -274,   337,  -274,   227,   296,
     207,  -274,   301,   284,   289,   297,   298,  -274,   227,   306,
    -274,  -274,   299,  -274,   300,   170,  -274,  -274,  -274,   225,
    -274,   337,  -274,  -274,  -274,  -274,   284,   291,  -274,  -274,
    -274,  -274,    49,  -274,  -274,  -274,  -274,  -274,   303,  -274,
     290,  -274,     4,   205,   200,  -274,   200,  -274,   312,  -274,
    -274,   -14,  -274,  -274,   313,   314,   117,   117,   335,  -274,
    -274,   304,  -274,  -274,  -274,   361,  -274,   158,  -274,  -274,
    -274,  -274,  -274
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -274,  -274,  -274,   342,  -274,  -274,  -274,  -274,    30,  -274,
    -274,  -274,   123,   206,   339,   321,   302,  -274,  -142,  -274,
    -274,  -274,    13,  -274,  -274,  -274,  -274,     0,     2,     3,
     178,  -233,  -273,  -274,  -274,   -38,  -274,  -274,  -274,  -274,
    -274,  -274,   -29,    78,  -274,   -22,   -73,  -202,    -2,  -274,
    -182,   -76,  -207,  -239,  -161,  -274,  -274,  -274,   233,    89,
     -31,   -75,  -274,  -274,  -274,  -274,   172,   137,  -274,     6,
    -274,  -274,  -274,  -274,  -274,    90,  -274,  -274
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -127
static const yytype_int16 yytable[] =
{
      57,   143,   204,   217,   206,   273,    91,   147,   337,   154,
     271,   194,     1,    87,   386,    69,   332,   213,    83,    85,
     266,    89,   318,   178,    14,    78,    79,     2,    80,    62,
     272,    18,    63,   227,   318,    71,     3,   113,   201,    23,
      69,   179,   329,   330,    69,   334,   335,   336,   195,   387,
      63,   141,   158,   315,    19,   337,   145,   155,   314,   316,
      98,    84,   153,   322,   202,   324,   156,   157,   320,   113,
     323,   325,    69,   283,    24,    69,   318,   210,   285,   317,
     333,   212,    22,   318,    81,   318,     4,    28,   321,    65,
      81,   326,    88,    27,    64,    65,    31,    82,    34,    65,
     357,    66,   319,    37,    38,    37,    38,  -126,    58,    15,
     318,    60,    64,   346,   349,   347,    39,    65,    39,   348,
     350,   105,   352,   368,   353,  -126,    81,   394,    40,    21,
      40,    65,    16,    92,   211,   270,    17,   197,   198,   106,
     351,    41,    32,    41,    69,  -126,   114,   366,   115,   367,
     116,   117,    42,   118,    42,  -126,    30,    81,   150,   107,
     395,   119,    65,   120,   180,   303,    74,    20,    75,   108,
      35,   396,   397,   161,    86,   121,   122,    43,   301,    43,
     310,   302,   181,   401,   365,   123,   124,   269,    90,   304,
      21,   165,   166,   222,    21,   125,   170,   126,   127,   223,
     247,   331,   151,   402,    55,   171,   128,   129,   305,   306,
     307,   130,   131,   132,    81,   252,   252,   160,    56,    65,
     163,    97,   105,   105,   133,   308,   253,   254,   309,    59,
     373,   252,   375,   275,   255,   205,    75,   207,   105,   376,
      81,    61,   253,   254,   276,    65,    72,   144,    93,   377,
     208,   209,   277,   256,   256,   355,    94,   278,   184,   185,
     257,   257,   188,    95,   191,   214,   166,   279,   280,   256,
     258,   258,   216,   166,   267,   209,   257,   268,   209,   274,
     209,   286,   209,   292,   293,    96,   258,   299,   300,   338,
     209,   339,   209,   374,   209,   186,   187,    99,   142,    75,
     146,   159,   162,   168,   169,   167,   175,   176,   182,   189,
     192,   193,   199,   195,   171,   219,   215,   220,   224,   228,
     203,   229,   231,   230,   232,   233,   234,   241,   242,   245,
     235,   236,   237,   370,   238,   239,   240,   246,   244,   248,
     249,   265,   250,   251,   255,   284,   288,   290,   287,   289,
     298,   295,   291,   312,   253,   313,   356,   343,   362,   294,
     381,   388,   391,   386,   400,   296,    36,   387,   361,   358,
     297,   369,   218,   340,    74,   341,    67,   372,   342,   344,
     354,   359,   360,   371,   100,   393,   390,   363,   364,   380,
     399,   345,   281,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   164,   221
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-274)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      29,    76,   144,   164,   146,   212,    44,    80,   281,    85,
     212,    50,     5,    42,    28,    37,    11,   159,    40,    41,
     202,    43,   261,    69,    57,    15,    16,    20,    18,    15,
     212,    34,    18,   175,   273,    37,    29,    68,    29,     0,
      62,    87,   275,   276,    66,   278,   279,   280,    87,    63,
      18,    73,    90,   260,    57,   328,    78,    86,   260,   261,
      62,    15,    84,   270,    55,   272,    88,    89,   270,   100,
     272,   273,    94,   215,    86,    97,   315,   153,   220,   261,
      75,   157,     4,   322,    80,   324,    79,    76,   270,    85,
      80,   273,    15,    15,    80,    85,    18,    87,    20,    85,
     333,    87,   263,     4,     5,     4,     5,    13,    30,    34,
     349,    33,    80,   315,   321,   317,    17,    85,    17,   321,
     322,    13,   324,   356,   326,    31,    80,    10,    29,    80,
      29,    85,    57,    55,   156,   211,    61,   131,   132,    31,
     322,    42,    76,    42,   166,    51,     7,   349,     9,   351,
      11,    12,    53,    14,    53,    61,    34,    80,    10,    51,
      43,    22,    85,    24,    69,    21,    90,    57,    92,    61,
      91,    54,    55,    95,    60,    36,    37,    78,   253,    78,
     255,   254,    87,    25,   345,    46,    47,   209,    89,    45,
      80,    88,    89,    27,    80,    56,    26,    58,    59,    33,
     194,   276,    54,    45,    48,    35,    67,    68,    64,    65,
      66,    72,    73,    74,    80,     6,     6,    94,    44,    85,
      97,    87,    13,    13,    85,    81,    17,    18,    84,    77,
     372,     6,    32,     6,    25,   145,    92,   147,    13,    39,
      80,    80,    17,    18,    17,    85,    15,    87,    77,    49,
      88,    89,    25,    44,    44,   330,    87,    30,   121,   122,
      51,    51,   125,    48,   127,    88,    89,    40,    41,    44,
      61,    61,    88,    89,    88,    89,    51,    88,    89,    88,
      89,    88,    89,    88,    89,    10,    61,    88,    89,    88,
      89,    88,    89,    88,    89,   123,   124,    80,    82,    92,
      87,    87,    19,    38,    38,    87,    87,    87,    87,    87,
      87,    87,    45,    87,    35,    83,    87,    87,    26,    84,
      92,    62,    84,    87,    87,    84,    84,    84,    70,    84,
      62,    62,    62,   362,    62,    62,    62,    84,    62,    84,
      62,    25,    62,    62,    25,    88,    84,    84,    88,    88,
      62,    58,    88,    45,    17,    38,    55,    71,    52,    88,
      48,    48,    48,    28,     3,    88,    24,    63,   338,    80,
      88,    80,   166,    88,    90,    88,    37,    87,    84,    84,
      84,    84,    84,    80,    63,   385,   384,    88,    88,   376,
     387,   302,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   170
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    20,    29,    79,    94,    95,    96,    97,    99,
     100,   102,   104,   126,    57,    34,    57,    61,    34,    57,
      57,    80,   136,     0,    86,   135,   136,   136,    76,   103,
      34,   136,    76,    98,   136,    91,    96,     4,     5,    17,
      29,    42,    53,    78,   127,   128,   129,   130,   131,   132,
     133,   134,   137,   167,   169,    48,    44,   135,   136,    77,
     136,    80,    15,    18,    80,    85,    87,   107,   108,   138,
     139,   141,    15,   170,    90,    92,   124,   154,    15,    16,
      18,    80,    87,   138,    15,   138,    60,   135,    15,   138,
      89,   128,   136,    77,    87,    48,    10,    87,   141,    80,
     108,   105,   106,   107,   141,    13,    31,    51,    61,   109,
     110,   112,   113,   153,     7,     9,    11,    12,    14,    22,
      24,    36,    37,    46,    47,    56,    58,    59,    67,    68,
      72,    73,    74,    85,   144,   155,   156,   157,   158,   161,
     164,   138,    82,   154,    87,   138,    87,   139,   111,   138,
      10,    54,   168,   138,   144,   135,   138,   138,   128,    87,
     105,   136,    19,   105,   109,    88,    89,    87,    38,    38,
      26,    35,   147,   148,   151,    87,    87,   160,    69,    87,
      69,    87,    87,   159,   160,   160,   159,   159,   160,    87,
     165,   160,    87,    87,    50,    87,   162,   162,   162,    45,
     142,    29,    55,    92,   111,   168,   111,   168,    88,    89,
     144,   138,   144,   111,    88,    87,    88,   147,   106,    83,
      87,   151,    27,    33,    26,   149,   150,   111,    84,    62,
      87,    84,    87,    84,    84,    62,    62,    62,    62,    62,
      62,    84,    70,   166,    62,    84,    84,   162,    84,    62,
      62,    62,     6,    17,    18,    25,    44,    51,    61,   140,
     143,   145,   146,   152,   153,    25,   143,    88,    88,   138,
     144,   140,   143,   145,    88,     6,    17,    25,    30,    40,
      41,   123,   125,   111,    88,   111,    88,    88,    84,    88,
      84,    88,    88,    89,    88,    58,    88,    88,    62,    88,
      89,   154,   139,    21,    45,    64,    65,    66,    81,    84,
     154,   163,    45,    38,   140,   145,   140,   143,   146,   147,
     140,   143,   145,   140,   145,   140,   143,   101,   123,   124,
     124,   154,    11,    75,   124,   124,   124,   125,    88,    88,
      88,    88,    84,    71,    84,   152,   140,   140,   140,   145,
     140,   143,   140,   140,    84,   154,    55,   124,    80,    84,
      84,   101,    52,    88,    88,   147,   140,   140,   124,    80,
     135,    80,    87,   111,    88,    32,    39,    49,   114,   115,
     115,    48,   116,   117,   120,   121,    28,    63,    48,   118,
     121,    48,   119,   120,    10,    43,    54,    55,   122,   122,
       3,    25,    45
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (x, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, x); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct ddlpackage::pass_to_bison * x)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, x)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    struct ddlpackage::pass_to_bison * x;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (x);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct ddlpackage::pass_to_bison * x)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, x)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    struct ddlpackage::pass_to_bison * x;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, x);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, struct ddlpackage::pass_to_bison * x)
#else
static void
yy_reduce_print (yyvsp, yyrule, x)
    YYSTYPE *yyvsp;
    int yyrule;
    struct ddlpackage::pass_to_bison * x;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , x);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, x); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct ddlpackage::pass_to_bison * x)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, x)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    struct ddlpackage::pass_to_bison * x;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (x);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (struct ddlpackage::pass_to_bison * x)
#else
int
yyparse (x)
    struct ddlpackage::pass_to_bison * x;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = YYLEX;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    { x->fParseTree = (yyvsp[(1) - (1)].sqlStmtList); }
    break;

  case 3:

    {
		if ((yyvsp[(3) - (3)].sqlStmt) != NULL) {
			(yyvsp[(1) - (3)].sqlStmtList)->push_back((yyvsp[(3) - (3)].sqlStmt));
			(yyval.sqlStmtList) = (yyvsp[(1) - (3)].sqlStmtList);
		}
		else {
			(yyval.sqlStmtList) = (yyvsp[(1) - (3)].sqlStmtList);
		}
	}
    break;

  case 4:

    { 
		if ((yyvsp[(1) - (1)].sqlStmt) != NULL)
		{
			(yyval.sqlStmtList) = x->fParseTree;
			(yyval.sqlStmtList)->push_back((yyvsp[(1) - (1)].sqlStmt));
		}
		else
		{
			(yyval.sqlStmtList) = NULL;
		}
	}
    break;

  case 11:

    { (yyval.sqlStmt) = NULL; }
    break;

  case 12:

    {(yyval.sqlStmt) = new DropTableStatement((yyvsp[(4) - (4)].qualifiedName), false);}
    break;

  case 13:

    {
		{(yyval.sqlStmt) = new DropTableStatement((yyvsp[(4) - (6)].qualifiedName), true);}
	}
    break;

  case 14:

    {(yyval.str) = NULL;}
    break;

  case 15:

    {(yyval.str) = NULL;}
    break;

  case 16:

    {(yyval.sqlStmt) = new DropIndexStatement((yyvsp[(3) - (3)].qualifiedName));}
    break;

  case 17:

    {
		(yyval.sqlStmt) = new CreateIndexStatement((yyvsp[(3) - (9)].qualifiedName), (yyvsp[(5) - (9)].qualifiedName), (yyvsp[(7) - (9)].columnNameList), false);
		delete (yyvsp[(9) - (9)].tableOptionMap);
	}
    break;

  case 18:

    {
		(yyval.sqlStmt) = new CreateIndexStatement((yyvsp[(4) - (10)].qualifiedName), (yyvsp[(6) - (10)].qualifiedName), (yyvsp[(8) - (10)].columnNameList), true);
		delete (yyvsp[(10) - (10)].tableOptionMap);
	}
    break;

  case 20:

    {(yyval.tableOptionMap) = NULL;}
    break;

  case 21:

    {
		(yyval.sqlStmt) = new CreateTableStatement(new TableDef((yyvsp[(4) - (8)].qualifiedName), (yyvsp[(6) - (8)].tableElementList), (yyvsp[(8) - (8)].tableOptionMap)));
	}
    break;

  case 22:

    {
		(yyval.sqlStmt) = new CreateTableStatement(new TableDef((yyvsp[(4) - (7)].qualifiedName), (yyvsp[(6) - (7)].tableElementList), NULL));
	}
    break;

  case 23:

    {(yyval.str) = NULL;}
    break;

  case 24:

    {(yyval.str) = NULL;}
    break;

  case 25:

    {(yyval.sqlStmt) = new TruncTableStatement((yyvsp[(3) - (3)].qualifiedName));}
    break;

  case 26:

    { {(yyval.sqlStmt) = new TruncTableStatement((yyvsp[(2) - (2)].qualifiedName));} }
    break;

  case 27:

    {
		(yyval.tableElementList) = new TableElementList();
		(yyval.tableElementList)->push_back((yyvsp[(1) - (1)].schemaObject));
	}
    break;

  case 28:

    {
		(yyval.tableElementList) = (yyvsp[(1) - (3)].tableElementList);
		(yyval.tableElementList)->push_back((yyvsp[(3) - (3)].schemaObject));
	}
    break;

  case 31:

    {
		(yyval.schemaObject) = (yyvsp[(3) - (4)].schemaObject);
		(yyvsp[(3) - (4)].schemaObject)->fName = (yyvsp[(2) - (4)].str);
	}
    break;

  case 32:

    {
		(yyval.schemaObject) = (yyvsp[(2) - (3)].schemaObject);
		(yyvsp[(2) - (3)].schemaObject)->fName = (yyvsp[(1) - (3)].str);
	}
    break;

  case 33:

    {(yyval.str) = (yyvsp[(1) - (1)].str);}
    break;

  case 34:

    {(yyval.str) = "noname";}
    break;

  case 37:

    {(yyval.schemaObject) = new TableCheckConstraintDef((yyvsp[(1) - (1)].str));}
    break;

  case 38:

    {
		if ((yyvsp[(1) - (4)].constraintType) == DDL_UNIQUE)
		    (yyval.schemaObject) = new TableUniqueConstraintDef((yyvsp[(3) - (4)].columnNameList));
        else if ((yyvsp[(1) - (4)].constraintType) == DDL_PRIMARY_KEY)
            (yyval.schemaObject) = new TablePrimaryKeyConstraintDef((yyvsp[(3) - (4)].columnNameList));
	}
    break;

  case 39:

    {
		(yyval.columnNameList) = new vector<string>;
		(yyval.columnNameList)->push_back((yyvsp[(1) - (1)].str));
	}
    break;

  case 40:

    {
		(yyval.columnNameList) = (yyvsp[(1) - (3)].columnNameList);
		(yyval.columnNameList)->push_back((yyvsp[(3) - (3)].str));
	}
    break;

  case 41:

    {(yyval.constraintType) = DDL_PRIMARY_KEY;}
    break;

  case 42:

    {(yyval.constraintType) = DDL_UNIQUE;}
    break;

  case 43:

    {
		(yyval.schemaObject) = new TableReferencesConstraintDef((yyvsp[(4) - (12)].columnNameList), (yyvsp[(7) - (12)].qualifiedName), (yyvsp[(9) - (12)].columnNameList), (yyvsp[(11) - (12)].matchType), (yyvsp[(12) - (12)].refAction));
	}
    break;

  case 45:

    {(yyval.matchType) = DDL_FULL;}
    break;

  case 46:

    {(yyval.matchType) = (yyvsp[(2) - (2)].matchType);}
    break;

  case 47:

    {(yyval.matchType) = DDL_FULL;}
    break;

  case 48:

    {(yyval.matchType) = DDL_PARTIAL;}
    break;

  case 50:

    {(yyval.refAction) = NULL;}
    break;

  case 51:

    {
		(yyval.refAction) = new ReferentialAction();
		(yyval.refAction)->fOnUpdate = (yyvsp[(1) - (2)].refActionCode);
		(yyval.refAction)->fOnDelete = (yyvsp[(2) - (2)].refActionCode);
	}
    break;

  case 52:

    {
		(yyval.refAction) = new ReferentialAction();
		(yyval.refAction)->fOnUpdate = (yyvsp[(2) - (2)].refActionCode);
		(yyval.refAction)->fOnDelete = (yyvsp[(1) - (2)].refActionCode);
	}
    break;

  case 54:

    {(yyval.refActionCode) = DDL_NO_ACTION;}
    break;

  case 56:

    {(yyval.refActionCode) = DDL_NO_ACTION;}
    break;

  case 57:

    {(yyval.refActionCode) = (yyvsp[(3) - (3)].refActionCode);}
    break;

  case 58:

    {(yyval.refActionCode) = (yyvsp[(3) - (3)].refActionCode);}
    break;

  case 59:

    {(yyval.refActionCode) = DDL_CASCADE;}
    break;

  case 60:

    {(yyval.refActionCode) = DDL_SET_NULL;}
    break;

  case 61:

    {(yyval.refActionCode) = DDL_SET_DEFAULT;}
    break;

  case 62:

    {(yyval.refActionCode) = DDL_NO_ACTION;}
    break;

  case 63:

    {(yyval.refActionCode) = DDL_RESTRICT;}
    break;

  case 64:

    {
		(yyval.tableOptionMap) = new TableOptionMap();
		(*(yyval.tableOptionMap))[(yyvsp[(1) - (1)].tableOption)->first] = (yyvsp[(1) - (1)].tableOption)->second;
		delete (yyvsp[(1) - (1)].tableOption);
	}
    break;

  case 65:

    {
		(yyval.tableOptionMap) = (yyvsp[(1) - (2)].tableOptionMap);
		(*(yyval.tableOptionMap))[(yyvsp[(2) - (2)].tableOption)->first] = (yyvsp[(2) - (2)].tableOption)->second;
		delete (yyvsp[(2) - (2)].tableOption);
	}
    break;

  case 66:

    {}
    break;

  case 67:

    {}
    break;

  case 68:

    {(yyval.tableOption) = new pair<string,string>("engine", (yyvsp[(3) - (3)].str));}
    break;

  case 69:

    {(yyval.tableOption) = new pair<string,string>("max_rows", (yyvsp[(3) - (3)].str));}
    break;

  case 70:

    {(yyval.tableOption) = new pair<string,string>("min_rows", (yyvsp[(3) - (3)].str));}
    break;

  case 71:

    {(yyval.tableOption) = new pair<string,string>("comment", (yyvsp[(3) - (3)].str));}
    break;

  case 72:

    {(yyval.tableOption) = new pair<string,string>("comment", (yyvsp[(2) - (2)].str));}
    break;

  case 73:

    {
       (yyval.tableOption) = new pair<string,string>("auto_increment", (yyvsp[(3) - (3)].str));
    }
    break;

  case 74:

    {(yyval.tableOption) = new pair<string,string>("default charset", (yyvsp[(4) - (4)].str));}
    break;

  case 75:

    {(yyval.tableOption) = new pair<string,string>("default charset", (yyvsp[(5) - (5)].str));}
    break;

  case 76:

    {
		(yyval.sqlStmt) = new AlterTableStatement((yyvsp[(3) - (4)].qualifiedName), (yyvsp[(4) - (4)].ataList));
	}
    break;

  case 77:

    {
		if ((yyvsp[(1) - (1)].ata) != NULL) {
			(yyval.ataList) = new AlterTableActionList();
			(yyval.ataList)->push_back((yyvsp[(1) - (1)].ata));
		}
		else {
			/* An alter_table_statement requires at least one action.
			   So, this shouldn't happen. */
			(yyval.ataList) = NULL;
		}		
	}
    break;

  case 78:

    {
		(yyval.ataList) = (yyvsp[(1) - (3)].ataList);
		(yyval.ataList)->push_back((yyvsp[(3) - (3)].ata));
	}
    break;

  case 79:

    {
		(yyval.ataList) = (yyvsp[(1) - (2)].ataList);
		(yyval.ataList)->push_back((yyvsp[(2) - (2)].ata));
	}
    break;

  case 89:

    {(yyval.ata) = new AtaTableComment((yyvsp[(3) - (3)].str));}
    break;

  case 90:

    {(yyval.ata) = new AtaTableComment((yyvsp[(2) - (2)].str));}
    break;

  case 91:

    {(yyval.ata) = new AtaModifyColumnType((yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].columnType));}
    break;

  case 92:

    {(yyval.ata) = new AtaModifyColumnType((yyvsp[(3) - (4)].str),(yyvsp[(4) - (4)].columnType));}
    break;

  case 93:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].columnType), NULL);}
    break;

  case 94:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (5)].str), (yyvsp[(3) - (5)].str), (yyvsp[(4) - (5)].columnType), (yyvsp[(5) - (5)].columnOption));}
    break;

  case 95:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (5)].str), (yyvsp[(4) - (5)].str), (yyvsp[(5) - (5)].columnType), NULL);}
    break;

  case 96:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].str), (yyvsp[(5) - (6)].columnType), (yyvsp[(6) - (6)].columnOption));}
    break;

  case 97:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (5)].str), (yyvsp[(3) - (5)].str), (yyvsp[(4) - (5)].columnType), (yyvsp[(5) - (5)].constraintList), NULL);}
    break;

  case 98:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].str), (yyvsp[(5) - (6)].columnType), (yyvsp[(6) - (6)].constraintList), NULL);}
    break;

  case 99:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].columnType), (yyvsp[(5) - (6)].constraintList), NULL, (yyvsp[(6) - (6)].columnOption));}
    break;

  case 100:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (7)].str), (yyvsp[(4) - (7)].str), (yyvsp[(5) - (7)].columnType), (yyvsp[(6) - (7)].constraintList), NULL, (yyvsp[(7) - (7)].columnOption));}
    break;

  case 101:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (5)].str), (yyvsp[(3) - (5)].str), (yyvsp[(4) - (5)].columnType), NULL, (yyvsp[(5) - (5)].colDefault));}
    break;

  case 102:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].str), (yyvsp[(5) - (6)].columnType), NULL, (yyvsp[(6) - (6)].colDefault));}
    break;

  case 103:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].columnType), NULL, (yyvsp[(5) - (6)].colDefault), (yyvsp[(6) - (6)].columnOption));}
    break;

  case 104:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (7)].str), (yyvsp[(4) - (7)].str), (yyvsp[(5) - (7)].columnType), NULL, (yyvsp[(6) - (7)].colDefault), (yyvsp[(7) - (7)].columnOption));}
    break;

  case 105:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].columnType), (yyvsp[(5) - (6)].constraintList), (yyvsp[(6) - (6)].colDefault));}
    break;

  case 106:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (7)].str), (yyvsp[(4) - (7)].str), (yyvsp[(5) - (7)].columnType), (yyvsp[(6) - (7)].constraintList), (yyvsp[(7) - (7)].colDefault));}
    break;

  case 107:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (6)].str), (yyvsp[(3) - (6)].str), (yyvsp[(4) - (6)].columnType), (yyvsp[(6) - (6)].constraintList), (yyvsp[(5) - (6)].colDefault));}
    break;

  case 108:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (7)].str), (yyvsp[(4) - (7)].str), (yyvsp[(5) - (7)].columnType), (yyvsp[(7) - (7)].constraintList), (yyvsp[(6) - (7)].colDefault));}
    break;

  case 109:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (7)].str), (yyvsp[(3) - (7)].str), (yyvsp[(4) - (7)].columnType), (yyvsp[(5) - (7)].constraintList), (yyvsp[(6) - (7)].colDefault), (yyvsp[(7) - (7)].columnOption));}
    break;

  case 110:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (8)].str), (yyvsp[(4) - (8)].str), (yyvsp[(5) - (8)].columnType), (yyvsp[(6) - (8)].constraintList), (yyvsp[(7) - (8)].colDefault), (yyvsp[(8) - (8)].columnOption));}
    break;

  case 111:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(2) - (7)].str), (yyvsp[(3) - (7)].str), (yyvsp[(4) - (7)].columnType), (yyvsp[(6) - (7)].constraintList), (yyvsp[(5) - (7)].colDefault), (yyvsp[(7) - (7)].columnOption));}
    break;

  case 112:

    {(yyval.ata) = new AtaRenameColumn((yyvsp[(3) - (8)].str), (yyvsp[(4) - (8)].str), (yyvsp[(5) - (8)].columnType), (yyvsp[(7) - (8)].constraintList), (yyvsp[(6) - (8)].colDefault), (yyvsp[(8) - (8)].columnOption));}
    break;

  case 113:

    {
		(yyval.ata) = new AtaDropTableConstraint((yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].refActionCode));
	}
    break;

  case 114:

    {(yyval.ata) = new AtaAddTableConstraint(dynamic_cast<TableConstraintDef *>((yyvsp[(2) - (2)].schemaObject)));}
    break;

  case 115:

    {(yyval.ata) = new AtaRenameTable((yyvsp[(2) - (2)].qualifiedName));}
    break;

  case 116:

    {(yyval.ata) = new AtaRenameTable((yyvsp[(3) - (3)].qualifiedName));}
    break;

  case 118:

    {(yyval.qualifiedName) = new QualifiedName((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str));}
    break;

  case 119:

    {
				if (x->fDBSchema.size())
					(yyval.qualifiedName) = new QualifiedName((char*)x->fDBSchema.c_str(), (yyvsp[(1) - (1)].str));
				else
				    (yyval.qualifiedName) = new QualifiedName((yyvsp[(1) - (1)].str));   
			}
    break;

  case 120:

    {(yyval.ata) = new AtaAddColumn(dynamic_cast<ColumnDef*>((yyvsp[(2) - (2)].schemaObject)));}
    break;

  case 121:

    {(yyval.ata) = new AtaAddColumn(dynamic_cast<ColumnDef*>((yyvsp[(3) - (3)].schemaObject)));}
    break;

  case 122:

    {(yyval.ata) = new AtaAddColumns((yyvsp[(3) - (4)].tableElementList));}
    break;

  case 123:

    {(yyval.ata) = new AtaAddColumns((yyvsp[(4) - (5)].tableElementList));}
    break;

  case 127:

    {(yyval.columnOption) = (yyvsp[(2) - (2)].str);}
    break;

  case 128:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (3)].str), (yyvsp[(2) - (3)].columnType), NULL, NULL );
	}
    break;

  case 129:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (4)].str), (yyvsp[(2) - (4)].columnType), (yyvsp[(4) - (4)].constraintList), NULL);
	}
    break;

  case 130:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (5)].str), (yyvsp[(2) - (5)].columnType), (yyvsp[(5) - (5)].constraintList), (yyvsp[(4) - (5)].colDefault));
	}
    break;

  case 131:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (4)].str), (yyvsp[(2) - (4)].columnType), NULL, (yyvsp[(4) - (4)].colDefault), NULL);
	}
    break;

  case 132:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (4)].str), (yyvsp[(2) - (4)].columnType), NULL, NULL, (yyvsp[(4) - (4)].columnOption) );
	}
    break;

  case 133:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (5)].str), (yyvsp[(2) - (5)].columnType), (yyvsp[(4) - (5)].constraintList), NULL, (yyvsp[(5) - (5)].columnOption));
	}
    break;

  case 134:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (6)].str), (yyvsp[(2) - (6)].columnType), (yyvsp[(5) - (6)].constraintList), (yyvsp[(4) - (6)].colDefault), (yyvsp[(6) - (6)].columnOption));
	}
    break;

  case 135:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (5)].str), (yyvsp[(2) - (5)].columnType), (yyvsp[(4) - (5)].constraintList), (yyvsp[(5) - (5)].colDefault));
	}
    break;

  case 136:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (6)].str), (yyvsp[(2) - (6)].columnType), (yyvsp[(4) - (6)].constraintList), (yyvsp[(5) - (6)].colDefault), (yyvsp[(6) - (6)].columnOption));
	}
    break;

  case 137:

    {
		(yyval.schemaObject) = new ColumnDef((yyvsp[(1) - (5)].str), (yyvsp[(2) - (5)].columnType), NULL, (yyvsp[(4) - (5)].colDefault), (yyvsp[(5) - (5)].columnOption));
	}
    break;

  case 140:

    {
		(yyval.colDefault) = new ColumnDefaultValue((yyvsp[(2) - (2)].str));
	}
    break;

  case 141:

    {(yyval.colDefault) = new ColumnDefaultValue(NULL);}
    break;

  case 142:

    {(yyval.colDefault) = new ColumnDefaultValue("$USER");}
    break;

  case 143:

    {(yyval.colDefault) = new ColumnDefaultValue("$CURRENT_USER");}
    break;

  case 144:

    {(yyval.colDefault) = new ColumnDefaultValue("$SESSION_USER");}
    break;

  case 145:

    {(yyval.colDefault) = new ColumnDefaultValue("$SYSTEM_USER");}
    break;

  case 150:

    {
		(yyval.columnType) = new ColumnType(DDL_BLOB);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_BLOB];
	}
    break;

  case 151:

    {
		(yyval.columnType) = new ColumnType(DDL_CLOB);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_CLOB];
	}
    break;

  case 152:

    {
		(yyval.constraintList) = new ColumnConstraintList();
		(yyval.constraintList)->push_back((yyvsp[(1) - (1)].columnConstraintDef));
	}
    break;

  case 153:

    {
		(yyval.constraintList) = (yyvsp[(1) - (2)].constraintList);
		(yyval.constraintList)->push_back((yyvsp[(2) - (2)].columnConstraintDef));
	}
    break;

  case 154:

    {
		(yyval.columnConstraintDef) = (yyvsp[(1) - (2)].columnConstraintDef);

		if((yyvsp[(2) - (2)].constraintAttributes) != NULL)
		{
			(yyvsp[(1) - (2)].columnConstraintDef)->fDeferrable = (yyvsp[(2) - (2)].constraintAttributes)->fDeferrable;
			(yyvsp[(1) - (2)].columnConstraintDef)->fCheckTime = (yyvsp[(2) - (2)].constraintAttributes)->fCheckTime;
		}

	}
    break;

  case 155:

    {
		(yyval.columnConstraintDef) = (yyvsp[(3) - (4)].columnConstraintDef);
		(yyvsp[(3) - (4)].columnConstraintDef)->fName = (yyvsp[(2) - (4)].str);

		if((yyvsp[(4) - (4)].constraintAttributes) != NULL)
		{
			(yyvsp[(3) - (4)].columnConstraintDef)->fDeferrable = (yyvsp[(4) - (4)].constraintAttributes)->fDeferrable;
			(yyvsp[(3) - (4)].columnConstraintDef)->fCheckTime = (yyvsp[(4) - (4)].constraintAttributes)->fCheckTime;
		}
		
	}
    break;

  case 156:

    {(yyval.constraintAttributes) = (yyvsp[(1) - (1)].constraintAttributes);}
    break;

  case 157:

    {(yyval.constraintAttributes) = NULL;}
    break;

  case 158:

    {
		(yyval.constraintAttributes) = new ConstraintAttributes((yyvsp[(1) - (2)].cattr), ((yyvsp[(2) - (2)].cattr) != 0));
	}
    break;

  case 159:

    {
		(yyval.constraintAttributes) = new ConstraintAttributes((yyvsp[(2) - (2)].cattr), true);
	}
    break;

  case 161:

    {(yyval.cattr) = DDL_NON_DEFERRABLE;}
    break;

  case 162:

    {(yyval.cattr) = DDL_DEFERRABLE;}
    break;

  case 163:

    {(yyval.cattr) = DDL_INITIALLY_DEFERRED;}
    break;

  case 164:

    {(yyval.cattr) = DDL_INITIALLY_IMMEDIATE;}
    break;

  case 165:

    {(yyval.columnConstraintDef) = new ColumnConstraintDef(DDL_NOT_NULL);}
    break;

  case 166:

    {(yyval.columnConstraintDef) = new ColumnConstraintDef(DDL_UNIQUE);}
    break;

  case 167:

    {(yyval.columnConstraintDef) = new ColumnConstraintDef(DDL_PRIMARY_KEY);}
    break;

  case 168:

    {(yyval.columnConstraintDef) = new ColumnConstraintDef(DDL_AUTO_INCREMENT);}
    break;

  case 169:

    {(yyval.columnConstraintDef) = new ColumnConstraintDef((yyvsp[(1) - (1)].str));}
    break;

  case 170:

    {(yyval.str) = (yyvsp[(3) - (4)].str);}
    break;

  case 171:

    {(yyval.str) = (yyvsp[(2) - (3)].str);}
    break;

  case 172:

    {
		(yyval.columnType) = new ColumnType(DDL_CHAR);
		(yyval.columnType)->fLength = 1;
	}
    break;

  case 173:

    {
		(yyval.columnType) = new ColumnType(DDL_CHAR);
		(yyval.columnType)->fLength = 1;
	}
    break;

  case 174:

    {
		(yyval.columnType) = new ColumnType(DDL_CHAR);
		(yyval.columnType)->fLength = atoi((yyvsp[(3) - (4)].str));
	}
    break;

  case 175:

    {
		(yyval.columnType) = new ColumnType(DDL_CHAR);
		(yyval.columnType)->fLength = atoi((yyvsp[(3) - (4)].str));
	}
    break;

  case 176:

    {
		(yyval.columnType) = new ColumnType(DDL_VARCHAR);
		(yyval.columnType)->fLength = atoi((yyvsp[(4) - (5)].str));
	}
    break;

  case 177:

    {
		(yyval.columnType) = new ColumnType(DDL_VARCHAR);
		(yyval.columnType)->fLength = atoi((yyvsp[(4) - (5)].str));
	}
    break;

  case 178:

    {
		(yyval.columnType) = new ColumnType(DDL_VARCHAR);
		(yyval.columnType)->fLength = atoi((yyvsp[(3) - (4)].str));
	}
    break;

  case 179:

    {
		(yyval.columnType) = new ColumnType(DDL_VARBINARY);
		(yyval.columnType)->fLength = atoi((yyvsp[(3) - (4)].str));
	}
    break;

  case 182:

    {
		(yyvsp[(2) - (2)].columnType)->fType = DDL_NUMERIC;
		(yyvsp[(2) - (2)].columnType)->fLength = DDLDatatypeLength[DDL_NUMERIC];
		(yyval.columnType) = (yyvsp[(2) - (2)].columnType);
	}
    break;

  case 183:

    {
		(yyvsp[(2) - (3)].columnType)->fType = DDL_UNSIGNED_NUMERIC;
		(yyvsp[(2) - (3)].columnType)->fLength = DDLDatatypeLength[DDL_UNSIGNED_NUMERIC];
		(yyval.columnType) = (yyvsp[(2) - (3)].columnType);
	}
    break;

  case 184:

    {
		(yyvsp[(2) - (2)].columnType)->fType = DDL_DECIMAL;
/*	   	$2->fLength = DDLDatatypeLength[DDL_DECIMAL]; */
		(yyval.columnType) = (yyvsp[(2) - (2)].columnType);
	}
    break;

  case 185:

    {
		(yyvsp[(2) - (3)].columnType)->fType = DDL_UNSIGNED_DECIMAL;
/*	   	$3->fLength = DDLDatatypeLength[DDL_DECIMAL]; */
		(yyval.columnType) = (yyvsp[(2) - (3)].columnType);
	}
    break;

  case 186:

    {
		(yyvsp[(2) - (2)].columnType)->fType = DDL_DECIMAL;
		(yyvsp[(2) - (2)].columnType)->fLength = DDLDatatypeLength[DDL_DECIMAL];
		(yyval.columnType) = (yyvsp[(2) - (2)].columnType);
	}
    break;

  case 187:

    {
		(yyvsp[(2) - (3)].columnType)->fType = DDL_UNSIGNED_DECIMAL;
		(yyvsp[(2) - (3)].columnType)->fLength = DDLDatatypeLength[DDL_UNSIGNED_DECIMAL];
		(yyval.columnType) = (yyvsp[(2) - (3)].columnType);
	}
    break;

  case 188:

    {
		(yyval.columnType) = new ColumnType(DDL_INT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_INT];
	}
    break;

  case 189:

    {
		(yyval.columnType) = new ColumnType(DDL_INT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_INT];
	}
    break;

  case 190:

    {
		(yyval.columnType) = new ColumnType(DDL_SMALLINT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_SMALLINT];
	}
    break;

  case 191:

    {
		(yyval.columnType) = new ColumnType(DDL_TINYINT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_TINYINT];
	}
    break;

  case 192:

    {
		(yyval.columnType) = new ColumnType(DDL_BIGINT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_BIGINT];
	}
    break;

  case 193:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_INT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_INT];
	}
    break;

  case 194:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_INT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_INT];
	}
    break;

  case 195:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_SMALLINT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_SMALLINT];
	}
    break;

  case 196:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_TINYINT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_TINYINT];
	}
    break;

  case 197:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_BIGINT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_BIGINT];
	}
    break;

  case 198:

    {(yyval.columnType) = new ColumnType(atoi((yyvsp[(2) - (3)].str)), 0);}
    break;

  case 199:

    {(yyval.columnType) = new ColumnType(atoi((yyvsp[(2) - (5)].str)), atoi((yyvsp[(4) - (5)].str)));}
    break;

  case 200:

    {(yyval.columnType) = new ColumnType(10,0);}
    break;

  case 201:

    {(yyval.str) = NULL;}
    break;

  case 202:

    {(yyval.str) = NULL;}
    break;

  case 203:

    {
		(yyval.columnType) = new ColumnType(DDL_DOUBLE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DOUBLE];
	}
    break;

  case 204:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_DOUBLE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DOUBLE];
	}
    break;

  case 205:

    {
		(yyval.columnType) = new ColumnType(DDL_DOUBLE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DOUBLE];
	}
    break;

  case 206:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_DOUBLE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DOUBLE];
	}
    break;

  case 207:

    {
		(yyval.columnType) = new ColumnType(DDL_DOUBLE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DOUBLE];
	}
    break;

  case 208:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_DOUBLE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DOUBLE];
	}
    break;

  case 209:

    {
		(yyval.columnType) = new ColumnType(DDL_FLOAT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_FLOAT];
	}
    break;

  case 210:

    {
		(yyval.columnType) = new ColumnType(DDL_UNSIGNED_FLOAT);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_FLOAT];
	}
    break;

  case 211:

    {(yyval.str) = NULL;}
    break;

  case 212:

    {(yyval.str) = NULL;}
    break;

  case 213:

    {(yyval.str) = NULL;}
    break;

  case 217:

    {
		(yyval.columnType) = new ColumnType(DDL_DATETIME);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DATETIME];
	}
    break;

  case 218:

    {
		(yyval.columnType) = new ColumnType(DDL_DATE);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DATE];
	}
    break;

  case 219:

    {
		(yyval.columnType) = new ColumnType(DDL_DATETIME);
		(yyval.columnType)->fLength = DDLDatatypeLength[DDL_DATETIME];
		(yyval.columnType)->fPrecision = (yyvsp[(2) - (3)].ival);
		(yyval.columnType)->fWithTimezone = (yyvsp[(3) - (3)].flag);
	}
    break;

  case 220:

    {(yyval.ival) = atoi((yyvsp[(2) - (3)].str));}
    break;

  case 221:

    {(yyval.ival) = -1;}
    break;

  case 222:

    {(yyval.flag) = true;}
    break;

  case 223:

    {(yyval.flag) = false;}
    break;

  case 224:

    {(yyval.ata) = new AtaDropColumn((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].refActionCode));}
    break;

  case 225:

    {(yyval.ata) = new AtaDropColumn((yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].refActionCode));}
    break;

  case 226:

    {(yyval.ata) = new AtaDropColumns((yyvsp[(4) - (5)].columnNameList));}
    break;

  case 227:

    {(yyval.ata) = new AtaDropColumns((yyvsp[(3) - (4)].columnNameList));}
    break;

  case 228:

    {(yyval.ata) = new AtaDropColumns((yyvsp[(4) - (5)].columnNameList));}
    break;

  case 229:

    {(yyval.refActionCode) = DDL_CASCADE;}
    break;

  case 230:

    {(yyval.refActionCode) = DDL_RESTRICT;}
    break;

  case 231:

    {(yyval.refActionCode) = DDL_NO_ACTION;}
    break;

  case 232:

    {(yyval.ata) = new AtaSetColumnDefault((yyvsp[(3) - (5)].str), (yyvsp[(5) - (5)].colDefault));}
    break;

  case 233:

    {(yyval.ata) = new AtaDropColumnDefault((yyvsp[(3) - (5)].str));}
    break;



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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (x, YY_("syntax error"));
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (x, yymsgp);
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
		      yytoken, &yylval, x);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
		  yystos[yystate], yyvsp, x);
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
  yyerror (x, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, x);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, x);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}






