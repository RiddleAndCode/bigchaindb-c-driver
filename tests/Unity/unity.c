/* ========================================================================= // NOLINT
    Unity Project - A Test Framework for C // NOLINT
    Copyright (c) 2007-14 Mike Karlesky, Mark VanderVoord, Greg Williams // NOLINT
    [Released under MIT License. Please refer to license.txt for details] // NOLINT
============================================================================ */
// NOLINT
// NOLINT
#define UNITY_INCLUDE_SETUP_STUBS // NOLINT
#include "unity.h"                // NOLINT
#include <stddef.h>               // NOLINT
// NOLINT
/* If omitted from header, declare overrideable prototypes here so they're ready // NOLINT
 * for use */
// NOLINT
#ifdef UNITY_OMIT_OUTPUT_CHAR_HEADER_DECLARATION            // NOLINT
void UNITY_OUTPUT_CHAR(int);                                // NOLINT
#endif                                                      // NOLINT
// NOLINT
/* Helpful macros for us to use here in Assert functions */ // NOLINT
#define UNITY_FAIL_AND_BAIL          \
    {                                \
        Unity.CurrentTestFailed = 1; \
        TEST_ABORT();                \
    } // NOLINT
#define UNITY_IGNORE_AND_BAIL         \
    {                                 \
        Unity.CurrentTestIgnored = 1; \
        TEST_ABORT();                 \
    } // NOLINT
#define RETURN_IF_FAIL_OR_IGNORE                             \
    if (Unity.CurrentTestFailed || Unity.CurrentTestIgnored) \
    return                                                                     // NOLINT
// NOLINT
struct UNITY_STORAGE_T Unity;                                                  // NOLINT
// NOLINT
#ifdef UNITY_OUTPUT_COLOR                                                      // NOLINT
static const char UnityStrOk[] = "\033[42mOK\033[00m";                         // NOLINT
static const char UnityStrPass[] = "\033[42mPASS\033[00m";                     // NOLINT
static const char UnityStrFail[] = "\033[41mFAIL\033[00m";                     // NOLINT
static const char UnityStrIgnore[] = "\033[43mIGNORE\033[00m";                 // NOLINT
#else                                                                          // NOLINT
static const char UnityStrOk[] = "OK";         // NOLINT
static const char UnityStrPass[] = "PASS";     // NOLINT
static const char UnityStrFail[] = "FAIL";     // NOLINT
static const char UnityStrIgnore[] = "IGNORE"; // NOLINT
#endif                                                                         // NOLINT
static const char UnityStrNull[] = "NULL";                                     // NOLINT
static const char UnityStrSpacer[] = ". ";                                     // NOLINT
static const char UnityStrExpected[] = " Expected ";                           // NOLINT
static const char UnityStrWas[] = " Was ";                                     // NOLINT
static const char UnityStrGt[] = " to be greater than ";                       // NOLINT
static const char UnityStrLt[] = " to be less than ";                          // NOLINT
static const char UnityStrOrEqual[] = "or equal to ";                          // NOLINT
static const char UnityStrElement[] = " Element ";                             // NOLINT
static const char UnityStrByte[] = " Byte ";                                   // NOLINT
static const char UnityStrMemory[] = " Memory Mismatch.";                      // NOLINT
static const char UnityStrDelta[] = " Values Not Within Delta ";               // NOLINT
static const char UnityStrPointless[] =                                        // NOLINT
  " You Asked Me To Compare Nothing, Which Was Pointless.";                  // NOLINT
static const char UnityStrNullPointerForExpected[] =                           // NOLINT
  " Expected pointer to be NULL";                                            // NOLINT
static const char UnityStrNullPointerForActual[] = " Actual pointer was NULL"; // NOLINT
#ifndef UNITY_EXCLUDE_FLOAT                                                    // NOLINT
static const char UnityStrNot[] = "Not ";                                      // NOLINT
static const char UnityStrInf[] = "Infinity";                                  // NOLINT
static const char UnityStrNegInf[] = "Negative Infinity";                      // NOLINT
static const char UnityStrNaN[] = "NaN";                                       // NOLINT
static const char UnityStrDet[] = "Determinate";                               // NOLINT
static const char UnityStrInvalidFloatTrait[] = "Invalid Float Trait";         // NOLINT
#endif                                                                         // NOLINT
const char UnityStrErrFloat[] = "Unity Floating Point Disabled";               // NOLINT
const char UnityStrErrDouble[] = "Unity Double Precision Disabled";            // NOLINT
const char UnityStrErr64[] = "Unity 64-bit Support Disabled";                  // NOLINT
static const char UnityStrBreaker[] = "-----------------------";               // NOLINT
static const char UnityStrResultsTests[] = " Tests ";                          // NOLINT
static const char UnityStrResultsFailures[] = " Failures ";                    // NOLINT
static const char UnityStrResultsIgnored[] = " Ignored ";                      // NOLINT
static const char UnityStrDetail1Name[] = UNITY_DETAIL1_NAME " ";              // NOLINT
static const char UnityStrDetail2Name[] = " " UNITY_DETAIL2_NAME " ";          // NOLINT
// NOLINT
/*----------------------------------------------- // NOLINT
 * Pretty Printers & Test Result Output Handlers // NOLINT
 *-----------------------------------------------*/
// NOLINT
// NOLINT
void UnityPrint(const char *string) {                           // NOLINT
  // NOLINT
  const char *pch = string;                                   // NOLINT
  // NOLINT
  if (pch != NULL) {                                          // NOLINT
    // NOLINT
    while (*pch) {                                          // NOLINT
      // NOLINT
      /* printable characters plus CR & LF are printed */ // NOLINT
      if ((*pch <= 126) && (*pch >= 32)) {                // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR(*pch);                        // NOLINT
      }                                                   // NOLINT
      /* write escaped carriage returns */                // NOLINT
      else if (*pch == 13) {                              // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('\\');                        // NOLINT
        UNITY_OUTPUT_CHAR('r');                         // NOLINT
      }                                                   // NOLINT
      /* write escaped line feeds */                      // NOLINT
      else if (*pch == 10) {                              // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('\\');                        // NOLINT
        UNITY_OUTPUT_CHAR('n');                         // NOLINT
      }                                                   // NOLINT
#ifdef UNITY_OUTPUT_COLOR                                       // NOLINT
      /* print ANSI escape code */                        // NOLINT
      else if (*pch == 27 && *(pch + 1) == '[') {         // NOLINT
        // NOLINT
        while (*pch && *pch != 'm') {                   // NOLINT
          // NOLINT
          UNITY_OUTPUT_CHAR(*pch);                    // NOLINT
          pch++;                                      // NOLINT
        }                                               // NOLINT
        UNITY_OUTPUT_CHAR('m');                         // NOLINT
      }                                                   // NOLINT
#endif                                                          // NOLINT
      /* unprintable characters are shown as codes */     // NOLINT
      else {                                              // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('\\');                        // NOLINT
        UNITY_OUTPUT_CHAR('x');                         // NOLINT
        UnityPrintNumberHex((UNITY_UINT)*pch, 2);       // NOLINT
      }                                                   // NOLINT
      pch++;                                              // NOLINT
    }                                                       // NOLINT
  }                                                           // NOLINT
} // NOLINT
// NOLINT
void UnityPrintLen(const char *string, const UNITY_UINT32 length) { // NOLINT
  // NOLINT
  const char *pch = string;                                     // NOLINT
  // NOLINT
  if (pch != NULL) {                                            // NOLINT
    // NOLINT
    while (*pch && (UNITY_UINT32)(pch - string) < length) {   // NOLINT
      // NOLINT
      /* printable characters plus CR & LF are printed */   // NOLINT
      if ((*pch <= 126) && (*pch >= 32)) {                  // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR(*pch);                          // NOLINT
      }                                                     // NOLINT
      /* write escaped carriage returns */                  // NOLINT
      else if (*pch == 13) {                                // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('\\');                          // NOLINT
        UNITY_OUTPUT_CHAR('r');                           // NOLINT
      }                                                     // NOLINT
      /* write escaped line feeds */                        // NOLINT
      else if (*pch == 10) {                                // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('\\');                          // NOLINT
        UNITY_OUTPUT_CHAR('n');                           // NOLINT
      }                                                     // NOLINT
      /* unprintable characters are shown as codes */       // NOLINT
      else {                                                // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('\\');                          // NOLINT
        UNITY_OUTPUT_CHAR('x');                           // NOLINT
        UnityPrintNumberHex((UNITY_UINT)*pch, 2);         // NOLINT
      }                                                     // NOLINT
      pch++;                                                // NOLINT
    }                                                         // NOLINT
  }                                                             // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                          // NOLINT
void UnityPrintNumberByStyle(const UNITY_INT number,                         // NOLINT
                             const UNITY_DISPLAY_STYLE_T style) {            // NOLINT
  // NOLINT
  if ((style & UNITY_DISPLAY_RANGE_INT) == UNITY_DISPLAY_RANGE_INT) {      // NOLINT
    // NOLINT
    UnityPrintNumber(number);                                            // NOLINT
  }                                                                        // NOLINT
  else if ((style & UNITY_DISPLAY_RANGE_UINT) == UNITY_DISPLAY_RANGE_UINT) { // NOLINT
    // NOLINT
    UnityPrintNumberUnsigned((UNITY_UINT)number);                        // NOLINT
  }                                                                        // NOLINT
  else {                                                                   // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR('0');                                              // NOLINT
    UNITY_OUTPUT_CHAR('x');                                              // NOLINT
    UnityPrintNumberHex((UNITY_UINT)number, (char)((style & 0xF) * 2));  // NOLINT
  }                                                                        // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/     // NOLINT
void UnityPrintNumber(const UNITY_INT number_to_print) { // NOLINT
  // NOLINT
  UNITY_UINT number = (UNITY_UINT)number_to_print;    // NOLINT
  // NOLINT
  if (number_to_print < 0) {                          // NOLINT
    // NOLINT
    /* A negative number, including MIN negative */ // NOLINT
    UNITY_OUTPUT_CHAR('-');                         // NOLINT
    number = (UNITY_UINT)(-number_to_print);        // NOLINT
  }                                                   // NOLINT
  UnityPrintNumberUnsigned(number);                   // NOLINT
} // NOLINT
// NOLINT
/*----------------------------------------------- // NOLINT
 * basically do an itoa using as little ram as possible */
// NOLINT
void UnityPrintNumberUnsigned(const UNITY_UINT number) {          // NOLINT
  // NOLINT
  UNITY_UINT divisor = 1;                                       // NOLINT
  // NOLINT
  /* figure out initial divisor */                              // NOLINT
  while (number / divisor > 9) {                                // NOLINT
    // NOLINT
    divisor *= 10;                                            // NOLINT
  }                                                             // NOLINT
  // NOLINT
  /* now mod and print, then divide divisor */                  // NOLINT
  do {                                                          // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR((char)('0' + (number / divisor % 10))); // NOLINT
    divisor /= 10;                                            // NOLINT
  } while (divisor > 0);                                        // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                            // NOLINT
void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print) { // NOLINT
  // NOLINT
  char nibbles = nibbles_to_print;                                           // NOLINT
  if ((unsigned)nibbles > (2 * sizeof(number)))                              // NOLINT
    nibbles = 2 * sizeof(number);                                          // NOLINT
  // NOLINT
  while (nibbles > 0) {                                                      // NOLINT
    // NOLINT
    nibbles--;                                                             // NOLINT
    int nibble = (int)(number >> (nibbles * 4)) & 0x0F;                    // NOLINT
    if (nibble <= 9) {                                                     // NOLINT
      // NOLINT
      UNITY_OUTPUT_CHAR((char)('0' + nibble));                           // NOLINT
    }                                                                      // NOLINT
    else {                                                                 // NOLINT
      // NOLINT
      UNITY_OUTPUT_CHAR((char)('A' - 10 + nibble));                      // NOLINT
    }                                                                      // NOLINT
  }                                                                          // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                  // NOLINT
void UnityPrintMask(const UNITY_UINT mask, const UNITY_UINT number) { // NOLINT
  // NOLINT
  UNITY_UINT current_bit = (UNITY_UINT)1 << (UNITY_INT_WIDTH - 1); // NOLINT
  UNITY_INT32 i;                                                   // NOLINT
  // NOLINT
  for (i = 0; i < UNITY_INT_WIDTH; i++) {                          // NOLINT
    // NOLINT
    if (current_bit & mask) {                                    // NOLINT
      // NOLINT
      if (current_bit & number) {                              // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('1');                              // NOLINT
      }                                                        // NOLINT
      else {                                                   // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('0');                              // NOLINT
      }                                                        // NOLINT
    }                                                            // NOLINT
    else {                                                       // NOLINT
      // NOLINT
      UNITY_OUTPUT_CHAR('X');                                  // NOLINT
    }                                                            // NOLINT
    current_bit = current_bit >> 1;                              // NOLINT
  }                                                                // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/ // NOLINT
#ifndef UNITY_EXCLUDE_FLOAT_PRINT                   // NOLINT
/* This function prints a floating-point value in a format similar to // NOLINT
 * printf("%.6g").  It can work with either single- or double-precision, // NOLINT
 * but for simplicity, it prints only 6 significant digits in either case. // NOLINT
 * Printing more than 6 digits accurately is hard (at least in the single- // NOLINT
 * precision case) and isn't attempted here. */
// NOLINT
void UnityPrintFloat(const UNITY_DOUBLE input_number) {               // NOLINT
  // NOLINT
  UNITY_DOUBLE number = input_number;                               // NOLINT
  // NOLINT
  /* print minus sign (including for negative zero) */              // NOLINT
  if (number < 0.0f || (number == 0.0f && 1.0f / number < 0.0f)) {  // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR('-');                                       // NOLINT
    number = -number;                                             // NOLINT
  }                                                                 // NOLINT
  // NOLINT
  /* handle zero, NaN, and +/- infinity */                          // NOLINT
  if (number == 0.0f)                                               // NOLINT
    UnityPrint("0");                                              // NOLINT
  else if (isnan(number))                                           // NOLINT
    UnityPrint("nan");                                            // NOLINT
  else if (isinf(number))                                           // NOLINT
    UnityPrint("inf");                                            // NOLINT
  else {                                                            // NOLINT
    // NOLINT
    int exponent = 0;                                             // NOLINT
    int decimals, digits;                                         // NOLINT
    UNITY_INT32 n;                                                // NOLINT
    char buf[16];                                                 // NOLINT
    // NOLINT
    /* scale up or down by powers of 10 */                        // NOLINT
    while (number < 100000.0f / 1e6f) {                           // NOLINT
      // NOLINT
      number *= 1e6f;                                           // NOLINT
      exponent -= 6;                                            // NOLINT
    }                                                             // NOLINT
    while (number < 100000.0f) {                                  // NOLINT
      // NOLINT
      number *= 10.0f;                                          // NOLINT
      exponent--;                                               // NOLINT
    }                                                             // NOLINT
    while (number > 1000000.0f * 1e6f) {                          // NOLINT
      // NOLINT
      number /= 1e6f;                                           // NOLINT
      exponent += 6;                                            // NOLINT
    }                                                             // NOLINT
    while (number > 1000000.0f) {                                 // NOLINT
      // NOLINT
      number /= 10.0f;                                          // NOLINT
      exponent++;                                               // NOLINT
    }                                                             // NOLINT
    // NOLINT
    /* round to nearest integer */                                // NOLINT
    n = ((UNITY_INT32)(number + number) + 1) / 2;                 // NOLINT
    if (n > 999999) {                                             // NOLINT
      // NOLINT
      n = 100000;                                               // NOLINT
      exponent++;                                               // NOLINT
    }                                                             // NOLINT
    // NOLINT
    /* determine where to place decimal point */                  // NOLINT
    decimals = (exponent <= 0 && exponent >= -9) ? -exponent : 5; // NOLINT
    exponent += decimals;                                         // NOLINT
    // NOLINT
    /* truncate trailing zeroes after decimal point */            // NOLINT
    while (decimals > 0 && n % 10 == 0) {                         // NOLINT
      // NOLINT
      n /= 10;                                                  // NOLINT
      decimals--;                                               // NOLINT
    }                                                             // NOLINT
    // NOLINT
    /* build up buffer in reverse order */                        // NOLINT
    digits = 0;                                                   // NOLINT
    while (n != 0 || digits < decimals + 1) {                     // NOLINT
      // NOLINT
      buf[digits++] = (char)('0' + n % 10);                     // NOLINT
      n /= 10;                                                  // NOLINT
    }                                                             // NOLINT
    while (digits > 0) {                                          // NOLINT
      // NOLINT
      if (digits == decimals)                                   // NOLINT
        UNITY_OUTPUT_CHAR('.');                               // NOLINT
      UNITY_OUTPUT_CHAR(buf[--digits]);                         // NOLINT
    }                                                             // NOLINT
    // NOLINT
    /* print exponent if needed */                                // NOLINT
    if (exponent != 0) {                                          // NOLINT
      // NOLINT
      UNITY_OUTPUT_CHAR('e');                                   // NOLINT
      // NOLINT
      if (exponent < 0) {                                       // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('-');                               // NOLINT
        exponent = -exponent;                                 // NOLINT
      }                                                         // NOLINT
      else {                                                    // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR('+');                               // NOLINT
      }                                                         // NOLINT
      // NOLINT
      digits = 0;                                               // NOLINT
      while (exponent != 0 || digits < 2) {                     // NOLINT
        // NOLINT
        buf[digits++] = (char)('0' + exponent % 10);          // NOLINT
        exponent /= 10;                                       // NOLINT
      }                                                         // NOLINT
      while (digits > 0) {                                      // NOLINT
        // NOLINT
        UNITY_OUTPUT_CHAR(buf[--digits]);                     // NOLINT
      }                                                         // NOLINT
    }                                                             // NOLINT
  }                                                                 // NOLINT
} // NOLINT
#endif /* ! UNITY_EXCLUDE_FLOAT_PRINT */                      // NOLINT
// NOLINT
/*-----------------------------------------------*/           // NOLINT
static void UnityTestResultsBegin(const char *file,           // NOLINT
                                  const UNITY_LINE_TYPE line) { // NOLINT
  // NOLINT
  UnityPrint(file);                                         // NOLINT
  UNITY_OUTPUT_CHAR(':');                                   // NOLINT
  UnityPrintNumber((UNITY_INT)line);                        // NOLINT
  UNITY_OUTPUT_CHAR(':');                                   // NOLINT
  UnityPrint(Unity.CurrentTestName);                        // NOLINT
  UNITY_OUTPUT_CHAR(':');                                   // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/               // NOLINT
static void UnityTestResultsFailBegin(const UNITY_LINE_TYPE line) { // NOLINT
  // NOLINT
  UnityTestResultsBegin(Unity.TestFile, line);                  // NOLINT
  UnityPrint(UnityStrFail);                                     // NOLINT
  UNITY_OUTPUT_CHAR(':');                                       // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                         // NOLINT
void UnityConcludeTest(void) {                                              // NOLINT
  // NOLINT
  if (Unity.CurrentTestIgnored) {                                         // NOLINT
    // NOLINT
    Unity.TestIgnores++;                                                // NOLINT
  }                                                                       // NOLINT
  else if (!Unity.CurrentTestFailed) {                                    // NOLINT
    // NOLINT
    UnityTestResultsBegin(Unity.TestFile, Unity.CurrentTestLineNumber); // NOLINT
    UnityPrint(UnityStrPass);                                           // NOLINT
  }                                                                       // NOLINT
  else {                                                                  // NOLINT
    // NOLINT
    Unity.TestFailures++;                                               // NOLINT
  }                                                                       // NOLINT
  // NOLINT
  Unity.CurrentTestFailed = 0;                                            // NOLINT
  Unity.CurrentTestIgnored = 0;                                           // NOLINT
  UNITY_PRINT_EOL();                                                      // NOLINT
  UNITY_FLUSH_CALL();                                                     // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/ // NOLINT
static void UnityAddMsgIfSpecified(const char *msg) { // NOLINT
  // NOLINT
  if (msg) {                                      // NOLINT
    // NOLINT
    UnityPrint(UnityStrSpacer);                 // NOLINT
#ifndef UNITY_EXCLUDE_DETAILS                       // NOLINT
    if (Unity.CurrentDetail1) {                 // NOLINT
      // NOLINT
      UnityPrint(UnityStrDetail1Name);        // NOLINT
      UnityPrint(Unity.CurrentDetail1);       // NOLINT
      if (Unity.CurrentDetail2) {             // NOLINT
        // NOLINT
        UnityPrint(UnityStrDetail2Name);    // NOLINT
        UnityPrint(Unity.CurrentDetail2);   // NOLINT
      }                                       // NOLINT
      UnityPrint(UnityStrSpacer);             // NOLINT
    }                                           // NOLINT
#endif                                              // NOLINT
    UnityPrint(msg);                            // NOLINT
  }                                               // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                  // NOLINT
static void UnityPrintExpectedAndActualStrings(const char *expected, // NOLINT
    const char *actual) { // NOLINT
  // NOLINT
  UnityPrint(UnityStrExpected);                                    // NOLINT
  if (expected != NULL) {                                          // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                     // NOLINT
    UnityPrint(expected);                                        // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                     // NOLINT
  }                                                                // NOLINT
  else {                                                           // NOLINT
    // NOLINT
    UnityPrint(UnityStrNull);                                    // NOLINT
  }                                                                // NOLINT
  UnityPrint(UnityStrWas);                                         // NOLINT
  if (actual != NULL) {                                            // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                     // NOLINT
    UnityPrint(actual);                                          // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                     // NOLINT
  }                                                                // NOLINT
  else {                                                           // NOLINT
    // NOLINT
    UnityPrint(UnityStrNull);                                    // NOLINT
  }                                                                // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                          // NOLINT
static void UnityPrintExpectedAndActualStringsLen(const char *expected,      // NOLINT
    const char *actual,        // NOLINT
    const UNITY_UINT32 length) { // NOLINT
  // NOLINT
  UnityPrint(UnityStrExpected);                                            // NOLINT
  if (expected != NULL) {                                                  // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                             // NOLINT
    UnityPrintLen(expected, length);                                     // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                             // NOLINT
  }                                                                        // NOLINT
  else {                                                                   // NOLINT
    // NOLINT
    UnityPrint(UnityStrNull);                                            // NOLINT
  }                                                                        // NOLINT
  UnityPrint(UnityStrWas);                                                 // NOLINT
  if (actual != NULL) {                                                    // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                             // NOLINT
    UnityPrintLen(actual, length);                                       // NOLINT
    UNITY_OUTPUT_CHAR('\'');                                             // NOLINT
  }                                                                        // NOLINT
  else {                                                                   // NOLINT
    // NOLINT
    UnityPrint(UnityStrNull);                                            // NOLINT
  }                                                                        // NOLINT
} // NOLINT
// NOLINT
/*----------------------------------------------- // NOLINT
 * Assertion & Control Helpers // NOLINT
 *-----------------------------------------------*/
// NOLINT
// NOLINT
static int UnityIsOneArrayNull(UNITY_INTERNAL_PTR expected,      // NOLINT
                               UNITY_INTERNAL_PTR actual,        // NOLINT
                               const UNITY_LINE_TYPE lineNumber, // NOLINT
                               const char *msg) {                // NOLINT
  // NOLINT
  if (expected == actual)                                      // NOLINT
    return 0; /* Both are NULL or same pointer */            // NOLINT
  // NOLINT
  /* print and return true if just expected is NULL */         // NOLINT
  if (expected == NULL) {                                      // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                   // NOLINT
    UnityPrint(UnityStrNullPointerForExpected);              // NOLINT
    UnityAddMsgIfSpecified(msg);                             // NOLINT
    return 1;                                                // NOLINT
  }                                                            // NOLINT
  // NOLINT
  /* print and return true if just actual is NULL */           // NOLINT
  if (actual == NULL) {                                        // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                   // NOLINT
    UnityPrint(UnityStrNullPointerForActual);                // NOLINT
    UnityAddMsgIfSpecified(msg);                             // NOLINT
    return 1;                                                // NOLINT
  }                                                            // NOLINT
  // NOLINT
  return 0; /* return false if neither is NULL */              // NOLINT
} // NOLINT
// NOLINT
/*----------------------------------------------- // NOLINT
 * Assertion Functions // NOLINT
 *-----------------------------------------------*/
// NOLINT
// NOLINT
void UnityAssertBits(const UNITY_INT mask, const UNITY_INT expected, // NOLINT
                     const UNITY_INT actual, const char *msg,        // NOLINT
                     const UNITY_LINE_TYPE lineNumber) {             // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                        // NOLINT
  // NOLINT
  if ((mask & expected) != (mask & actual)) {                      // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                       // NOLINT
    UnityPrint(UnityStrExpected);                                // NOLINT
    UnityPrintMask((UNITY_UINT)mask, (UNITY_UINT)expected);      // NOLINT
    UnityPrint(UnityStrWas);                                     // NOLINT
    UnityPrintMask((UNITY_UINT)mask, (UNITY_UINT)actual);        // NOLINT
    UnityAddMsgIfSpecified(msg);                                 // NOLINT
    UNITY_FAIL_AND_BAIL;                                         // NOLINT
  }                                                                // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                            // NOLINT
void UnityAssertEqualNumber(const UNITY_INT expected, const UNITY_INT actual,  // NOLINT
                            const char *msg, const UNITY_LINE_TYPE lineNumber, // NOLINT
                            const UNITY_DISPLAY_STYLE_T style) {               // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                  // NOLINT
  // NOLINT
  if (expected != actual) {                                                  // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                                 // NOLINT
    UnityPrint(UnityStrExpected);                                          // NOLINT
    UnityPrintNumberByStyle(expected, style);                              // NOLINT
    UnityPrint(UnityStrWas);                                               // NOLINT
    UnityPrintNumberByStyle(actual, style);                                // NOLINT
    UnityAddMsgIfSpecified(msg);                                           // NOLINT
    UNITY_FAIL_AND_BAIL;                                                   // NOLINT
  }                                                                          // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                           // NOLINT
void UnityAssertGreaterOrLessOrEqualNumber(const UNITY_INT threshold,         // NOLINT
    const UNITY_INT actual,            // NOLINT
    const UNITY_COMPARISON_T compare,  // NOLINT
    const char *msg,                   // NOLINT
    const UNITY_LINE_TYPE lineNumber,  // NOLINT
    const UNITY_DISPLAY_STYLE_T style) { // NOLINT
  // NOLINT
  int failed = 0;                                                           // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                 // NOLINT
  // NOLINT
  if (threshold == actual && compare & UNITY_EQUAL_TO)                      // NOLINT
    return;                                                               // NOLINT
  if (threshold == actual)                                                  // NOLINT
    failed = 1;                                                           // NOLINT
  // NOLINT
  if ((style & UNITY_DISPLAY_RANGE_INT) == UNITY_DISPLAY_RANGE_INT) {       // NOLINT
    // NOLINT
    if (actual > threshold && compare & UNITY_SMALLER_THAN)               // NOLINT
      failed = 1;                                                       // NOLINT
    if (actual < threshold && compare & UNITY_GREATER_THAN)               // NOLINT
      failed = 1;                                                       // NOLINT
  }                                                                         // NOLINT
  else /* UINT or HEX */                                                    // NOLINT
  {
    // NOLINT
    if ((UNITY_UINT)actual > (UNITY_UINT)threshold &&                     // NOLINT
        compare & UNITY_SMALLER_THAN)                                     // NOLINT
      failed = 1;                                                       // NOLINT
    if ((UNITY_UINT)actual < (UNITY_UINT)threshold &&                     // NOLINT
        compare & UNITY_GREATER_THAN)                                     // NOLINT
      failed = 1;                                                       // NOLINT
  }                                                                         // NOLINT
  // NOLINT
  if (failed) {                                                             // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                                // NOLINT
    UnityPrint(UnityStrExpected);                                         // NOLINT
    UnityPrintNumberByStyle(actual, style);                               // NOLINT
    if (compare & UNITY_GREATER_THAN)                                     // NOLINT
      UnityPrint(UnityStrGt);                                           // NOLINT
    if (compare & UNITY_SMALLER_THAN)                                     // NOLINT
      UnityPrint(UnityStrLt);                                           // NOLINT
    if (compare & UNITY_EQUAL_TO)                                         // NOLINT
      UnityPrint(UnityStrOrEqual);                                      // NOLINT
    UnityPrintNumberByStyle(threshold, style);                            // NOLINT
    UnityAddMsgIfSpecified(msg);                                          // NOLINT
    UNITY_FAIL_AND_BAIL;                                                  // NOLINT
  }                                                                         // NOLINT
} // NOLINT
// NOLINT
#define UnityPrintPointlessAndBail()           \
    {                                          \
        UnityTestResultsFailBegin(lineNumber); \
        UnityPrint(UnityStrPointless);         \
        UnityAddMsgIfSpecified(msg);           \
        UNITY_FAIL_AND_BAIL;                   \
    }                                                                           // NOLINT
// NOLINT
/*-----------------------------------------------*/                             // NOLINT
void UnityAssertEqualIntArray(UNITY_INTERNAL_PTR expected,                      // NOLINT
                              UNITY_INTERNAL_PTR actual,                        // NOLINT
                              const UNITY_UINT32 num_elements, const char *msg, // NOLINT
                              const UNITY_LINE_TYPE lineNumber,                 // NOLINT
                              const UNITY_DISPLAY_STYLE_T style,                // NOLINT
                              const UNITY_FLAGS_T flags) {                      // NOLINT
  // NOLINT
  UNITY_UINT32 elements = num_elements;                                       // NOLINT
  unsigned int length = style & 0xF;                                          // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                   // NOLINT
  // NOLINT
  if (num_elements == 0) {                                                    // NOLINT
    // NOLINT
    UnityPrintPointlessAndBail();                                           // NOLINT
  }                                                                           // NOLINT
  // NOLINT
  if (expected == actual)                                                     // NOLINT
    return; /* Both are NULL or same pointer */                             // NOLINT
  if (UnityIsOneArrayNull(expected, actual, lineNumber, msg))                 // NOLINT
    UNITY_FAIL_AND_BAIL;                                                    // NOLINT
  // NOLINT
  while ((elements > 0) && elements--) {                                      // NOLINT
    // NOLINT
    UNITY_INT expect_val;                                                   // NOLINT
    UNITY_INT actual_val;                                                   // NOLINT
    switch (length) {                                                       // NOLINT
    // NOLINT
    case 1:                                                                 // NOLINT
      expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT8 *)expected;     // NOLINT
      actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT8 *)actual;       // NOLINT
      break;                                                              // NOLINT
    case 2:                                                                 // NOLINT
      expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT16 *)expected;    // NOLINT
      actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT16 *)actual;      // NOLINT
      break;                                                              // NOLINT
#ifdef UNITY_SUPPORT_64                                                         // NOLINT
    case 8:                                                                 // NOLINT
      expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT64 *)expected;    // NOLINT
      actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT64 *)actual;      // NOLINT
      break;                                                              // NOLINT
#endif                                                                          // NOLINT
    default: /* length 4 bytes */                                           // NOLINT
      expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT32 *)expected;    // NOLINT
      actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT32 *)actual;      // NOLINT
      length = 4;                                                         // NOLINT
      break;                                                              // NOLINT
    }                                                                       // NOLINT
    // NOLINT
    if (expect_val != actual_val) {                                         // NOLINT
      // NOLINT
      if (style & UNITY_DISPLAY_RANGE_UINT &&                             // NOLINT
          length <                                                        // NOLINT
          sizeof(expect_val)) {                                       // NOLINT
        /* For UINT, remove sign extension (padding // NOLINT
        1's) from signed // NOLINT
        type casts above */
        // NOLINT
        UNITY_INT mask = 1;                                             // NOLINT
        mask = (mask << 8 * length) - 1;                                // NOLINT
        expect_val &= mask;                                             // NOLINT
        actual_val &= mask;                                             // NOLINT
      }                                                                   // NOLINT
      UnityTestResultsFailBegin(lineNumber);                              // NOLINT
      UnityPrint(UnityStrElement);                                        // NOLINT
      UnityPrintNumberUnsigned(num_elements - elements - 1);              // NOLINT
      UnityPrint(UnityStrExpected);                                       // NOLINT
      UnityPrintNumberByStyle(expect_val, style);                         // NOLINT
      UnityPrint(UnityStrWas);                                            // NOLINT
      UnityPrintNumberByStyle(actual_val, style);                         // NOLINT
      UnityAddMsgIfSpecified(msg);                                        // NOLINT
      UNITY_FAIL_AND_BAIL;                                                // NOLINT
    }                                                                       // NOLINT
    if (flags == UNITY_ARRAY_TO_ARRAY) {                                    // NOLINT
      // NOLINT
      expected = (UNITY_INTERNAL_PTR)(length + (const char *)expected);   // NOLINT
    }                                                                       // NOLINT
    actual = (UNITY_INTERNAL_PTR)(length + (const char *)actual);           // NOLINT
  }                                                                           // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                         // NOLINT
#ifndef UNITY_EXCLUDE_FLOAT                                                 // NOLINT
/* Wrap this define in a function with variable types as float or double */ // NOLINT
#define UNITY_FLOAT_OR_DOUBLE_WITHIN(delta, expected, actual, diff) \
    if (isinf(expected) && isinf(actual) &&                         \
        (((expected) < 0) == ((actual) < 0)))                       \
        return 1;                                                   \
    if (UNITY_NAN_CHECK)                                            \
        return 1;                                                   \
    (diff) = (actual) - (expected);                                 \
    if ((diff) < 0)                                                 \
        (diff) = -(diff);                                           \
    if ((delta) < 0)                                                \
        (delta) = -(delta);                                         \
    return !(isnan(diff) || isinf(diff) || ((diff) > (delta)))                // NOLINT
/* This first part of this condition will catch any NaN or Infinite values */ // NOLINT
#ifndef UNITY_NAN_NOT_EQUAL_NAN                                               // NOLINT
#define UNITY_NAN_CHECK isnan(expected) && isnan(actual)                      // NOLINT
#else                                                                         // NOLINT
#define UNITY_NAN_CHECK 0                                                     // NOLINT
#endif                                                                        // NOLINT
// NOLINT
#ifndef UNITY_EXCLUDE_FLOAT_PRINT                                             // NOLINT
#define UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT(expected, actual) \
    {                                                           \
        UnityPrint(UnityStrExpected);                           \
        UnityPrintFloat(expected);                              \
        UnityPrint(UnityStrWas);                                \
        UnityPrintFloat(actual);                                \
    } // NOLINT
#else // NOLINT
#define UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT(expected, actual) \
    UnityPrint(UnityStrDelta)                                         // NOLINT
#endif /* UNITY_EXCLUDE_FLOAT_PRINT */                                // NOLINT
// NOLINT
static int UnityFloatsWithin(UNITY_FLOAT delta, UNITY_FLOAT expected, // NOLINT
                             UNITY_FLOAT actual) {                    // NOLINT
  // NOLINT
  UNITY_FLOAT diff;                                                 // NOLINT
  UNITY_FLOAT_OR_DOUBLE_WITHIN(delta, expected, actual, diff);      // NOLINT
} // NOLINT
// NOLINT
void UnityAssertEqualFloatArray(UNITY_PTR_ATTRIBUTE const UNITY_FLOAT *expected,     // NOLINT
                                UNITY_PTR_ATTRIBUTE const UNITY_FLOAT *actual,       // NOLINT
                                const UNITY_UINT32 num_elements,                     // NOLINT
                                const char *msg,                                     // NOLINT
                                const UNITY_LINE_TYPE lineNumber,                    // NOLINT
                                const UNITY_FLAGS_T flags) {                         // NOLINT
  // NOLINT
  UNITY_UINT32 elements = num_elements;                                            // NOLINT
  UNITY_PTR_ATTRIBUTE const UNITY_FLOAT *ptr_expected = expected;                  // NOLINT
  UNITY_PTR_ATTRIBUTE const UNITY_FLOAT *ptr_actual = actual;                      // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                        // NOLINT
  // NOLINT
  if (elements == 0) {                                                             // NOLINT
    // NOLINT
    UnityPrintPointlessAndBail();                                                // NOLINT
  }                                                                                // NOLINT
  // NOLINT
  if (expected == actual)                                                          // NOLINT
    return; /* Both are NULL or same pointer */                                  // NOLINT
  if (UnityIsOneArrayNull((UNITY_INTERNAL_PTR)expected,                            // NOLINT
                          (UNITY_INTERNAL_PTR)actual, lineNumber, msg))            // NOLINT
    UNITY_FAIL_AND_BAIL;                                                         // NOLINT
  // NOLINT
  while (elements--) {                                                             // NOLINT
    // NOLINT
    if (!UnityFloatsWithin(*ptr_expected * UNITY_FLOAT_PRECISION, *ptr_expected, // NOLINT
                           *ptr_actual)) {                                       // NOLINT
      // NOLINT
      UnityTestResultsFailBegin(lineNumber);                                   // NOLINT
      UnityPrint(UnityStrElement);                                             // NOLINT
      UnityPrintNumberUnsigned(num_elements - elements - 1);                   // NOLINT
      UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT((UNITY_DOUBLE)*ptr_expected,       // NOLINT
                                            (UNITY_DOUBLE)*ptr_actual);        // NOLINT
      UnityAddMsgIfSpecified(msg);                                             // NOLINT
      UNITY_FAIL_AND_BAIL;                                                     // NOLINT
    }                                                                            // NOLINT
    if (flags == UNITY_ARRAY_TO_ARRAY) {                                         // NOLINT
      // NOLINT
      ptr_expected++;                                                          // NOLINT
    }                                                                            // NOLINT
    ptr_actual++;                                                                // NOLINT
  }                                                                                // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                     // NOLINT
void UnityAssertFloatsWithin(const UNITY_FLOAT delta,                   // NOLINT
                             const UNITY_FLOAT expected,                // NOLINT
                             const UNITY_FLOAT actual, const char *msg, // NOLINT
                             const UNITY_LINE_TYPE lineNumber) {        // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                           // NOLINT
  // NOLINT
  if (!UnityFloatsWithin(delta, expected, actual)) {                  // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                          // NOLINT
    UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT((UNITY_DOUBLE)expected,   // NOLINT
                                          (UNITY_DOUBLE)actual);    // NOLINT
    UnityAddMsgIfSpecified(msg);                                    // NOLINT
    UNITY_FAIL_AND_BAIL;                                            // NOLINT
  }                                                                   // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                        // NOLINT
void UnityAssertFloatSpecial(const UNITY_FLOAT actual, const char *msg,    // NOLINT
                             const UNITY_LINE_TYPE lineNumber,             // NOLINT
                             const UNITY_FLOAT_TRAIT_T style) {            // NOLINT
  // NOLINT
  const char *trait_names[] = {UnityStrInf, UnityStrNegInf, UnityStrNaN, // NOLINT
                               UnityStrDet
                              };                             // NOLINT
  UNITY_INT should_be_trait = ((UNITY_INT)style & 1);                    // NOLINT
  UNITY_INT is_trait = !should_be_trait;                                 // NOLINT
  UNITY_INT trait_index = (UNITY_INT)(style >> 1);                       // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                              // NOLINT
  // NOLINT
  switch (style) {                                                       // NOLINT
  // NOLINT
  case UNITY_FLOAT_IS_INF:                                               // NOLINT
  case UNITY_FLOAT_IS_NOT_INF:                                           // NOLINT
    is_trait = isinf(actual) && (actual > 0);                          // NOLINT
    break;                                                             // NOLINT
  case UNITY_FLOAT_IS_NEG_INF:                                           // NOLINT
  case UNITY_FLOAT_IS_NOT_NEG_INF:                                       // NOLINT
    is_trait = isinf(actual) && (actual < 0);                          // NOLINT
    break;                                                             // NOLINT
  // NOLINT
  case UNITY_FLOAT_IS_NAN:                                               // NOLINT
  case UNITY_FLOAT_IS_NOT_NAN:                                           // NOLINT
    is_trait = isnan(actual) ? 1 : 0;                                  // NOLINT
    break;                                                             // NOLINT
  // NOLINT
  case UNITY_FLOAT_IS_DET:                                               /* A determinate number is non infinite and not NaN. // NOLINT
                            */
  // NOLINT
  case UNITY_FLOAT_IS_NOT_DET:                                           // NOLINT
    is_trait = !isinf(actual) && !isnan(actual);                       // NOLINT
    break;                                                             // NOLINT
  // NOLINT
  default:                                                               // NOLINT
    trait_index = 0;                                                   // NOLINT
    trait_names[0] = UnityStrInvalidFloatTrait;                        // NOLINT
    break;                                                             // NOLINT
  }                                                                      // NOLINT
  // NOLINT
  if (is_trait != should_be_trait) {                                     // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                             // NOLINT
    UnityPrint(UnityStrExpected);                                      // NOLINT
    if (!should_be_trait)                                              // NOLINT
      UnityPrint(UnityStrNot);                                       // NOLINT
    UnityPrint(trait_names[trait_index]);                              // NOLINT
    UnityPrint(UnityStrWas);                                           // NOLINT
#ifndef UNITY_EXCLUDE_FLOAT_PRINT                                          // NOLINT
    UnityPrintFloat((UNITY_DOUBLE)actual);                             // NOLINT
#else                                                                      // NOLINT
    if (should_be_trait)                  // NOLINT
      UnityPrint(UnityStrNot);          // NOLINT
    UnityPrint(trait_names[trait_index]); // NOLINT
#endif                                                                     // NOLINT
    UnityAddMsgIfSpecified(msg);                                       // NOLINT
    UNITY_FAIL_AND_BAIL;                                               // NOLINT
  }                                                                      // NOLINT
} // NOLINT
// NOLINT
#endif /* not UNITY_EXCLUDE_FLOAT */                                     // NOLINT
// NOLINT
/*-----------------------------------------------*/                      // NOLINT
#ifndef UNITY_EXCLUDE_DOUBLE                                             // NOLINT
static int UnityDoublesWithin(UNITY_DOUBLE delta, UNITY_DOUBLE expected, // NOLINT
                              UNITY_DOUBLE actual) {                     // NOLINT
  // NOLINT
  UNITY_DOUBLE diff;                                                   // NOLINT
  UNITY_FLOAT_OR_DOUBLE_WITHIN(delta, expected, actual, diff);         // NOLINT
} // NOLINT
// NOLINT
void UnityAssertEqualDoubleArray(                                              // NOLINT
  UNITY_PTR_ATTRIBUTE const UNITY_DOUBLE *expected,                          // NOLINT
  UNITY_PTR_ATTRIBUTE const UNITY_DOUBLE *actual,                            // NOLINT
  const UNITY_UINT32 num_elements, const char *msg,                          // NOLINT
  const UNITY_LINE_TYPE lineNumber, const UNITY_FLAGS_T flags) {             // NOLINT
  // NOLINT
  UNITY_UINT32 elements = num_elements;                                      // NOLINT
  UNITY_PTR_ATTRIBUTE const UNITY_DOUBLE *ptr_expected = expected;           // NOLINT
  UNITY_PTR_ATTRIBUTE const UNITY_DOUBLE *ptr_actual = actual;               // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                  // NOLINT
  // NOLINT
  if (elements == 0) {                                                       // NOLINT
    // NOLINT
    UnityPrintPointlessAndBail();                                          // NOLINT
  }                                                                          // NOLINT
  // NOLINT
  if (expected == actual)                                                    // NOLINT
    return; /* Both are NULL or same pointer */                            // NOLINT
  if (UnityIsOneArrayNull((UNITY_INTERNAL_PTR)expected,                      // NOLINT
                          (UNITY_INTERNAL_PTR)actual, lineNumber, msg))      // NOLINT
    UNITY_FAIL_AND_BAIL;                                                   // NOLINT
  // NOLINT
  while (elements--) {                                                       // NOLINT
    // NOLINT
    if (!UnityDoublesWithin(*ptr_expected * UNITY_DOUBLE_PRECISION,        // NOLINT
                            *ptr_expected, *ptr_actual)) {                 // NOLINT
      // NOLINT
      UnityTestResultsFailBegin(lineNumber);                             // NOLINT
      UnityPrint(UnityStrElement);                                       // NOLINT
      UnityPrintNumberUnsigned(num_elements - elements - 1);             // NOLINT
      UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT(*ptr_expected, *ptr_actual); // NOLINT
      UnityAddMsgIfSpecified(msg);                                       // NOLINT
      UNITY_FAIL_AND_BAIL;                                               // NOLINT
    }                                                                      // NOLINT
    if (flags == UNITY_ARRAY_TO_ARRAY) {                                   // NOLINT
      // NOLINT
      ptr_expected++;                                                    // NOLINT
    }                                                                      // NOLINT
    ptr_actual++;                                                          // NOLINT
  }                                                                          // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                       // NOLINT
void UnityAssertDoublesWithin(const UNITY_DOUBLE delta,                   // NOLINT
                              const UNITY_DOUBLE expected,                // NOLINT
                              const UNITY_DOUBLE actual, const char *msg, // NOLINT
                              const UNITY_LINE_TYPE lineNumber) {         // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                             // NOLINT
  // NOLINT
  if (!UnityDoublesWithin(delta, expected, actual)) {                   // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                            // NOLINT
    UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT(expected, actual);          // NOLINT
    UnityAddMsgIfSpecified(msg);                                      // NOLINT
    UNITY_FAIL_AND_BAIL;                                              // NOLINT
  }                                                                     // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                        // NOLINT
// NOLINT
void UnityAssertDoubleSpecial(const UNITY_DOUBLE actual, const char *msg,  // NOLINT
                              const UNITY_LINE_TYPE lineNumber,            // NOLINT
                              const UNITY_FLOAT_TRAIT_T style) {           // NOLINT
  // NOLINT
  const char *trait_names[] = {UnityStrInf, UnityStrNegInf, UnityStrNaN, // NOLINT
                               UnityStrDet
                              };                             // NOLINT
  UNITY_INT should_be_trait = ((UNITY_INT)style & 1);                    // NOLINT
  UNITY_INT is_trait = !should_be_trait;                                 // NOLINT
  UNITY_INT trait_index = (UNITY_INT)(style >> 1);                       // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                              // NOLINT
  // NOLINT
  switch (style) {                                                       // NOLINT
  // NOLINT
  case UNITY_FLOAT_IS_INF:                                               // NOLINT
  case UNITY_FLOAT_IS_NOT_INF:                                           // NOLINT
    is_trait = isinf(actual) && (actual > 0);                          // NOLINT
    break;                                                             // NOLINT
  case UNITY_FLOAT_IS_NEG_INF:                                           // NOLINT
  case UNITY_FLOAT_IS_NOT_NEG_INF:                                       // NOLINT
    is_trait = isinf(actual) && (actual < 0);                          // NOLINT
    break;                                                             // NOLINT
  // NOLINT
  case UNITY_FLOAT_IS_NAN:                                               // NOLINT
  case UNITY_FLOAT_IS_NOT_NAN:                                           // NOLINT
    is_trait = isnan(actual) ? 1 : 0;                                  // NOLINT
    break;                                                             // NOLINT
  // NOLINT
  case UNITY_FLOAT_IS_DET:                                               /* A determinate number is non infinite and not NaN. // NOLINT
                            */
  // NOLINT
  case UNITY_FLOAT_IS_NOT_DET:                                           // NOLINT
    is_trait = !isinf(actual) && !isnan(actual);                       // NOLINT
    break;                                                             // NOLINT
  // NOLINT
  default:                                                               // NOLINT
    trait_index = 0;                                                   // NOLINT
    trait_names[0] = UnityStrInvalidFloatTrait;                        // NOLINT
    break;                                                             // NOLINT
  }                                                                      // NOLINT
  // NOLINT
  if (is_trait != should_be_trait) {                                     // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                             // NOLINT
    UnityPrint(UnityStrExpected);                                      // NOLINT
    if (!should_be_trait)                                              // NOLINT
      UnityPrint(UnityStrNot);                                       // NOLINT
    UnityPrint(trait_names[trait_index]);                              // NOLINT
    UnityPrint(UnityStrWas);                                           // NOLINT
#ifndef UNITY_EXCLUDE_FLOAT_PRINT                                          // NOLINT
    UnityPrintFloat(actual);                                           // NOLINT
#else                                                                      // NOLINT
    if (should_be_trait)                  // NOLINT
      UnityPrint(UnityStrNot);          // NOLINT
    UnityPrint(trait_names[trait_index]); // NOLINT
#endif                                                                     // NOLINT
    UnityAddMsgIfSpecified(msg);                                       // NOLINT
    UNITY_FAIL_AND_BAIL;                                               // NOLINT
  }                                                                      // NOLINT
} // NOLINT
// NOLINT
#endif /* not UNITY_EXCLUDE_DOUBLE */                                           // NOLINT
// NOLINT
/*-----------------------------------------------*/                             // NOLINT
void UnityAssertNumbersWithin(const UNITY_UINT delta, const UNITY_INT expected, // NOLINT
                              const UNITY_INT actual, const char *msg,          // NOLINT
                              const UNITY_LINE_TYPE lineNumber,                 // NOLINT
                              const UNITY_DISPLAY_STYLE_T style) {              // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                   // NOLINT
  // NOLINT
  if ((style & UNITY_DISPLAY_RANGE_INT) == UNITY_DISPLAY_RANGE_INT) {         // NOLINT
    // NOLINT
    if (actual > expected)                                                  // NOLINT
      Unity.CurrentTestFailed =                                           // NOLINT
        (UNITY_UINT)((UNITY_UINT)(actual - expected) > delta);          // NOLINT
    else                                                                    // NOLINT
      Unity.CurrentTestFailed =                                           // NOLINT
        (UNITY_UINT)((UNITY_UINT)(expected - actual) > delta);          // NOLINT
  }                                                                           // NOLINT
  else {                                                                      // NOLINT
    // NOLINT
    if ((UNITY_UINT)actual > (UNITY_UINT)expected)                          // NOLINT
      Unity.CurrentTestFailed =                                           // NOLINT
        (UNITY_UINT)((UNITY_UINT)(actual - expected) > delta);          // NOLINT
    else                                                                    // NOLINT
      Unity.CurrentTestFailed =                                           // NOLINT
        (UNITY_UINT)((UNITY_UINT)(expected - actual) > delta);          // NOLINT
  }                                                                           // NOLINT
  // NOLINT
  if (Unity.CurrentTestFailed) {                                              // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                                  // NOLINT
    UnityPrint(UnityStrDelta);                                              // NOLINT
    UnityPrintNumberByStyle((UNITY_INT)delta, style);                       // NOLINT
    UnityPrint(UnityStrExpected);                                           // NOLINT
    UnityPrintNumberByStyle(expected, style);                               // NOLINT
    UnityPrint(UnityStrWas);                                                // NOLINT
    UnityPrintNumberByStyle(actual, style);                                 // NOLINT
    UnityAddMsgIfSpecified(msg);                                            // NOLINT
    UNITY_FAIL_AND_BAIL;                                                    // NOLINT
  }                                                                           // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                            // NOLINT
void UnityAssertEqualString(const char *expected, const char *actual,          // NOLINT
                            const char *msg, const UNITY_LINE_TYPE lineNumber) { // NOLINT
  // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                  // NOLINT
  // NOLINT
  /* if both pointers not null compare the strings */                        // NOLINT
  if (expected && actual) {                                                  // NOLINT
    // NOLINT
    for (UNITY_UINT32 i = 0; expected[i] || actual[i]; i++) {              // NOLINT
      // NOLINT
      if (expected[i] != actual[i]) {                                    // NOLINT
        // NOLINT
        Unity.CurrentTestFailed = 1;                                   // NOLINT
        break;                                                         // NOLINT
      }                                                                  // NOLINT
    }                                                                      // NOLINT
  }                                                                          // NOLINT
  else {                                                                     // NOLINT
    /* handle case of one pointers being null (if both null, test should // NOLINT
    pass) */
    // NOLINT
    if (expected != actual) {                                              // NOLINT
      // NOLINT
      Unity.CurrentTestFailed = 1;                                       // NOLINT
    }                                                                      // NOLINT
  }                                                                          // NOLINT
  // NOLINT
  if (Unity.CurrentTestFailed) {                                             // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                                 // NOLINT
    UnityPrintExpectedAndActualStrings(expected, actual);                  // NOLINT
    UnityAddMsgIfSpecified(msg);                                           // NOLINT
    UNITY_FAIL_AND_BAIL;                                                   // NOLINT
  }                                                                          // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                               // NOLINT
void UnityAssertEqualStringLen(const char *expected, const char *actual,          // NOLINT
                               const UNITY_UINT32 length, const char *msg,        // NOLINT
                               const UNITY_LINE_TYPE lineNumber) {                // NOLINT
  // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                     // NOLINT
  // NOLINT
  /* if both pointers not null compare the strings */                           // NOLINT
  if (expected && actual) {                                                     // NOLINT
    // NOLINT
    for (UNITY_UINT32 i = 0; (i < length) && (expected[i] || actual[i]); i++) { // NOLINT
      // NOLINT
      if (expected[i] != actual[i]) {                                       // NOLINT
        // NOLINT
        Unity.CurrentTestFailed = 1;                                      // NOLINT
        break;                                                            // NOLINT
      }                                                                     // NOLINT
    }                                                                         // NOLINT
  }                                                                             // NOLINT
  else {                                                                        // NOLINT
    /* handle case of one pointers being null (if both null, test should // NOLINT
    pass) */
    // NOLINT
    if (expected != actual) {                                                 // NOLINT
      // NOLINT
      Unity.CurrentTestFailed = 1;                                          // NOLINT
    }                                                                         // NOLINT
  }                                                                             // NOLINT
  // NOLINT
  if (Unity.CurrentTestFailed) {                                                // NOLINT
    // NOLINT
    UnityTestResultsFailBegin(lineNumber);                                    // NOLINT
    UnityPrintExpectedAndActualStringsLen(expected, actual, length);          // NOLINT
    UnityAddMsgIfSpecified(msg);                                              // NOLINT
    UNITY_FAIL_AND_BAIL;                                                      // NOLINT
  }                                                                             // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                       // NOLINT
void UnityAssertEqualStringArray(UNITY_INTERNAL_PTR expected,             // NOLINT
                                 const char **actual,                     // NOLINT
                                 const UNITY_UINT32 num_elements,         // NOLINT
                                 const char *msg,                         // NOLINT
                                 const UNITY_LINE_TYPE lineNumber,        // NOLINT
                                 const UNITY_FLAGS_T flags) {             // NOLINT
  // NOLINT
  UNITY_UINT32 i = 0;                                                   // NOLINT
  UNITY_UINT32 j = 0;                                                   // NOLINT
  const char *expd = NULL;                                              // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                             // NOLINT
  // NOLINT
  /* if no elements, it's an error */                                   // NOLINT
  if (num_elements == 0) {                                              // NOLINT
    // NOLINT
    UnityPrintPointlessAndBail();                                     // NOLINT
  }                                                                     // NOLINT
  // NOLINT
  if ((const void *)expected == (const void *)actual) {                 // NOLINT
    // NOLINT
    return; /* Both are NULL or same pointer */                       // NOLINT
  }                                                                     // NOLINT
  // NOLINT
  if (UnityIsOneArrayNull((UNITY_INTERNAL_PTR)expected,                 // NOLINT
                          (UNITY_INTERNAL_PTR)actual, lineNumber, msg)) { // NOLINT
    // NOLINT
    UNITY_FAIL_AND_BAIL;                                              // NOLINT
  }                                                                     // NOLINT
  // NOLINT
  if (flags != UNITY_ARRAY_TO_ARRAY) {                                  // NOLINT
    // NOLINT
    expd = (const char *)expected;                                    // NOLINT
  }                                                                     // NOLINT
  // NOLINT
  do {                                                                  // NOLINT
    // NOLINT
    const char *act = actual[j];                                      // NOLINT
    if (flags == UNITY_ARRAY_TO_ARRAY) {                              // NOLINT
      // NOLINT
      expd = ((const char *const *)expected)[j];                    // NOLINT
    }                                                                 // NOLINT
    // NOLINT
    /* if both pointers not null compare the strings */               // NOLINT
    if (expd && act) {                                                // NOLINT
      // NOLINT
      for (i = 0; expd[i] || act[i]; i++) {                         // NOLINT
        // NOLINT
        if (expd[i] != act[i]) {                                  // NOLINT
          // NOLINT
          Unity.CurrentTestFailed = 1;                          // NOLINT
          break;                                                // NOLINT
        }                                                         // NOLINT
      }                                                             // NOLINT
    }                                                                 // NOLINT
    else {                                                            // NOLINT
      /* handle case of one pointers being null (if both null, test // NOLINT
      should pass) */
      // NOLINT
      if (expd != act) {                                            // NOLINT
        // NOLINT
        Unity.CurrentTestFailed = 1;                              // NOLINT
      }                                                             // NOLINT
    }                                                                 // NOLINT
    // NOLINT
    if (Unity.CurrentTestFailed) {                                    // NOLINT
      // NOLINT
      UnityTestResultsFailBegin(lineNumber);                        // NOLINT
      if (num_elements > 1) {                                       // NOLINT
        // NOLINT
        UnityPrint(UnityStrElement);                              // NOLINT
        UnityPrintNumberUnsigned(j);                              // NOLINT
      }                                                             // NOLINT
      UnityPrintExpectedAndActualStrings(expd, act);                // NOLINT
      UnityAddMsgIfSpecified(msg);                                  // NOLINT
      UNITY_FAIL_AND_BAIL;                                          // NOLINT
    }                                                                 // NOLINT
  } while (++j < num_elements);                                         // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                           // NOLINT
void UnityAssertEqualMemory(UNITY_INTERNAL_PTR expected,                      // NOLINT
                            UNITY_INTERNAL_PTR actual,                        // NOLINT
                            const UNITY_UINT32 length,                        // NOLINT
                            const UNITY_UINT32 num_elements, const char *msg, // NOLINT
                            const UNITY_LINE_TYPE lineNumber,                 // NOLINT
                            const UNITY_FLAGS_T flags) {                      // NOLINT
  // NOLINT
  UNITY_PTR_ATTRIBUTE const unsigned char *ptr_exp =                        // NOLINT
    (UNITY_PTR_ATTRIBUTE const unsigned char *)expected;                  // NOLINT
  UNITY_PTR_ATTRIBUTE const unsigned char *ptr_act =                        // NOLINT
    (UNITY_PTR_ATTRIBUTE const unsigned char *)actual;                    // NOLINT
  UNITY_UINT32 elements = num_elements;                                     // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                                 // NOLINT
  // NOLINT
  if ((elements == 0) || (length == 0)) {                                   // NOLINT
    // NOLINT
    UnityPrintPointlessAndBail();                                         // NOLINT
  }                                                                         // NOLINT
  // NOLINT
  if (expected == actual)                                                   // NOLINT
    return; /* Both are NULL or same pointer */                           // NOLINT
  if (UnityIsOneArrayNull(expected, actual, lineNumber, msg))               // NOLINT
    UNITY_FAIL_AND_BAIL;                                                  // NOLINT
  // NOLINT
  while (elements--) {                                                      // NOLINT
    // NOLINT
    UNITY_UINT32 bytes = length;                                          // NOLINT
    while (bytes--) {                                                     // NOLINT
      // NOLINT
      if (*ptr_exp != *ptr_act) {                                       // NOLINT
        // NOLINT
        UnityTestResultsFailBegin(lineNumber);                        // NOLINT
        UnityPrint(UnityStrMemory);                                   // NOLINT
        if (num_elements > 1) {                                       // NOLINT
          // NOLINT
          UnityPrint(UnityStrElement);                              // NOLINT
          UnityPrintNumberUnsigned(num_elements - elements - 1);    // NOLINT
        }                                                             // NOLINT
        UnityPrint(UnityStrByte);                                     // NOLINT
        UnityPrintNumberUnsigned(length - bytes - 1);                 // NOLINT
        UnityPrint(UnityStrExpected);                                 // NOLINT
        UnityPrintNumberByStyle(*ptr_exp, UNITY_DISPLAY_STYLE_HEX8);  // NOLINT
        UnityPrint(UnityStrWas);                                      // NOLINT
        UnityPrintNumberByStyle(*ptr_act, UNITY_DISPLAY_STYLE_HEX8);  // NOLINT
        UnityAddMsgIfSpecified(msg);                                  // NOLINT
        UNITY_FAIL_AND_BAIL;                                          // NOLINT
      }                                                                 // NOLINT
      ptr_exp++;                                                        // NOLINT
      ptr_act++;                                                        // NOLINT
    }                                                                     // NOLINT
    if (flags == UNITY_ARRAY_TO_VAL) {                                    // NOLINT
      // NOLINT
      ptr_exp = (UNITY_PTR_ATTRIBUTE const unsigned char *)expected;    // NOLINT
    }                                                                     // NOLINT
  }                                                                         // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                           // NOLINT
// NOLINT
static union {                                                                // NOLINT
  UNITY_INT8 i8;                                                            // NOLINT
  UNITY_INT16 i16;                                                          // NOLINT
  UNITY_INT32 i32;                                                          // NOLINT
#ifdef UNITY_SUPPORT_64                                                       // NOLINT
  UNITY_INT64 i64;                                                          // NOLINT
#endif                                                                        // NOLINT
#ifndef UNITY_EXCLUDE_FLOAT                                                   // NOLINT
  float f;                                                                  // NOLINT
#endif                                                                        // NOLINT
#ifndef UNITY_EXCLUDE_DOUBLE                                                  // NOLINT
  double d;                                                                 // NOLINT
#endif                                                                        // NOLINT
} UnityQuickCompare;                                                          // NOLINT
// NOLINT
UNITY_INTERNAL_PTR UnityNumToPtr(const UNITY_INT num, const UNITY_UINT8 size) { // NOLINT
  // NOLINT
  switch (size) {                                                           // NOLINT
  // NOLINT
  case 1:                                                                   // NOLINT
    UnityQuickCompare.i8 = (UNITY_INT8)num;                               // NOLINT
    return (UNITY_INTERNAL_PTR)(&UnityQuickCompare.i8);                   // NOLINT
  // NOLINT
  case 2:                                                                   // NOLINT
    UnityQuickCompare.i16 = (UNITY_INT16)num;                             // NOLINT
    return (UNITY_INTERNAL_PTR)(&UnityQuickCompare.i16);                  // NOLINT
    // NOLINT
#ifdef UNITY_SUPPORT_64                                                       // NOLINT
  case 8:                                                                   // NOLINT
    UnityQuickCompare.i64 = (UNITY_INT64)num;                             // NOLINT
    return (UNITY_INTERNAL_PTR)(&UnityQuickCompare.i64);                  // NOLINT
#endif                                                                        // NOLINT
  default: /* 4 bytes */                                                    // NOLINT
    UnityQuickCompare.i32 = (UNITY_INT32)num;                             // NOLINT
    return (UNITY_INTERNAL_PTR)(&UnityQuickCompare.i32);                  // NOLINT
  }                                                                         // NOLINT
} // NOLINT
// NOLINT
#ifndef UNITY_EXCLUDE_FLOAT                            // NOLINT
UNITY_INTERNAL_PTR UnityFloatToPtr(const float num) {  // NOLINT
  // NOLINT
  UnityQuickCompare.f = num;                         // NOLINT
  return (UNITY_INTERNAL_PTR)(&UnityQuickCompare.f); // NOLINT
} // NOLINT
#endif                                                 // NOLINT
// NOLINT
#ifndef UNITY_EXCLUDE_DOUBLE                           // NOLINT
UNITY_INTERNAL_PTR UnityDoubleToPtr(const double num) { // NOLINT
  // NOLINT
  UnityQuickCompare.d = num;                         // NOLINT
  return (UNITY_INTERNAL_PTR)(&UnityQuickCompare.d); // NOLINT
} // NOLINT
#endif // NOLINT
// NOLINT
/*----------------------------------------------- // NOLINT
 * Control Functions // NOLINT
 *-----------------------------------------------*/
// NOLINT
// NOLINT
void UnityFail(const char *msg, const UNITY_LINE_TYPE line) { // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                               // NOLINT
  // NOLINT
  UnityTestResultsBegin(Unity.TestFile, line);            // NOLINT
  UnityPrint(UnityStrFail);                               // NOLINT
  if (msg != NULL) {                                      // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR(':');                             // NOLINT
    // NOLINT
#ifndef UNITY_EXCLUDE_DETAILS                               // NOLINT
    if (Unity.CurrentDetail1) {                         // NOLINT
      // NOLINT
      UnityPrint(UnityStrDetail1Name);                // NOLINT
      UnityPrint(Unity.CurrentDetail1);               // NOLINT
      if (Unity.CurrentDetail2) {                     // NOLINT
        // NOLINT
        UnityPrint(UnityStrDetail2Name);            // NOLINT
        UnityPrint(Unity.CurrentDetail2);           // NOLINT
      }                                               // NOLINT
      UnityPrint(UnityStrSpacer);                     // NOLINT
    }                                                   // NOLINT
#endif                                                      // NOLINT
    if (msg[0] != ' ') {                                // NOLINT
      // NOLINT
      UNITY_OUTPUT_CHAR(' ');                         // NOLINT
    }                                                   // NOLINT
    UnityPrint(msg);                                    // NOLINT
  }                                                       // NOLINT
  // NOLINT
  UNITY_FAIL_AND_BAIL;                                    // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/           // NOLINT
void UnityIgnore(const char *msg, const UNITY_LINE_TYPE line) { // NOLINT
  // NOLINT
  RETURN_IF_FAIL_OR_IGNORE;                                 // NOLINT
  // NOLINT
  UnityTestResultsBegin(Unity.TestFile, line);              // NOLINT
  UnityPrint(UnityStrIgnore);                               // NOLINT
  if (msg != NULL) {                                        // NOLINT
    // NOLINT
    UNITY_OUTPUT_CHAR(':');                               // NOLINT
    UNITY_OUTPUT_CHAR(' ');                               // NOLINT
    UnityPrint(msg);                                      // NOLINT
  }                                                         // NOLINT
  UNITY_IGNORE_AND_BAIL;                                    // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/                    // NOLINT
void UnityDefaultTestRun(UnityTestFunction Func, const char *FuncName, // NOLINT
                         const int FuncLineNum) {                      // NOLINT
  // NOLINT
  Unity.CurrentTestName = FuncName;                                  // NOLINT
  Unity.CurrentTestLineNumber = (UNITY_LINE_TYPE)FuncLineNum;        // NOLINT
  Unity.NumberOfTests++;                                             // NOLINT
  UNITY_CLR_DETAILS();                                               // NOLINT
  if (TEST_PROTECT()) {                                              // NOLINT
    // NOLINT
    setUp();                                                       // NOLINT
    Func();                                                        // NOLINT
  }                                                                  // NOLINT
  if (TEST_PROTECT()) {                                              // NOLINT
    // NOLINT
    tearDown();                                                    // NOLINT
  }                                                                  // NOLINT
  UnityConcludeTest();                                               // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/ // NOLINT
void UnityBegin(const char *filename) {             // NOLINT
  // NOLINT
  Unity.TestFile = filename;                      // NOLINT
  Unity.CurrentTestName = NULL;                   // NOLINT
  Unity.CurrentTestLineNumber = 0;                // NOLINT
  Unity.NumberOfTests = 0;                        // NOLINT
  Unity.TestFailures = 0;                         // NOLINT
  Unity.TestIgnores = 0;                          // NOLINT
  Unity.CurrentTestFailed = 0;                    // NOLINT
  Unity.CurrentTestIgnored = 0;                   // NOLINT
  // NOLINT
  UNITY_CLR_DETAILS();                            // NOLINT
  UNITY_OUTPUT_START();                           // NOLINT
} // NOLINT
// NOLINT
/*-----------------------------------------------*/     // NOLINT
int UnityEnd(void) {                                    // NOLINT
  // NOLINT
  UNITY_PRINT_EOL();                                  // NOLINT
  UnityPrint(UnityStrBreaker);                        // NOLINT
  UNITY_PRINT_EOL();                                  // NOLINT
  UnityPrintNumber((UNITY_INT)(Unity.NumberOfTests)); // NOLINT
  UnityPrint(UnityStrResultsTests);                   // NOLINT
  UnityPrintNumber((UNITY_INT)(Unity.TestFailures));  // NOLINT
  UnityPrint(UnityStrResultsFailures);                // NOLINT
  UnityPrintNumber((UNITY_INT)(Unity.TestIgnores));   // NOLINT
  UnityPrint(UnityStrResultsIgnored);                 // NOLINT
  UNITY_PRINT_EOL();                                  // NOLINT
  if (Unity.TestFailures == 0U) {                     // NOLINT
    // NOLINT
    UnityPrint(UnityStrOk);                         // NOLINT
  }                                                   // NOLINT
  else {                                              // NOLINT
    // NOLINT
    UnityPrint(UnityStrFail);                       // NOLINT
#ifdef UNITY_DIFFERENTIATE_FINAL_FAIL                   // NOLINT
    UNITY_OUTPUT_CHAR('E');                         // NOLINT
    UNITY_OUTPUT_CHAR('D');                         // NOLINT
#endif                                                  // NOLINT
  }                                                   // NOLINT
  UNITY_PRINT_EOL();                                  // NOLINT
  UNITY_FLUSH_CALL();                                 // NOLINT
  UNITY_OUTPUT_COMPLETE();                            // NOLINT
  return (int)(Unity.TestFailures);                   // NOLINT
} // NOLINT
// NOLINT
/*----------------------------------------------- // NOLINT
 * Command Line Argument Support // NOLINT
 *-----------------------------------------------*/
// NOLINT
#ifdef UNITY_USE_COMMAND_LINE_ARGS                                              // NOLINT
// NOLINT
char *UnityOptionIncludeNamed = NULL;                                           // NOLINT
char *UnityOptionExcludeNamed = NULL;                                           // NOLINT
int UnityVerbosity = 1;                                                         // NOLINT
// NOLINT
int UnityParseOptions(int argc, char **argv) {                                  // NOLINT
  // NOLINT
  UnityOptionIncludeNamed = NULL;                                             // NOLINT
  UnityOptionExcludeNamed = NULL;                                             // NOLINT
  // NOLINT
  for (int i = 1; i < argc; i++) {                                            // NOLINT
    // NOLINT
    if (argv[i][0] == '-') {                                                // NOLINT
      // NOLINT
      switch (argv[i][1]) {                                               // NOLINT
      // NOLINT
      case 'l': /* list tests */                                          // NOLINT
        return -1;                                                      // NOLINT
      case 'n': /* include tests with name including this string */       // NOLINT
      case 'f': /* an alias for -n */                                     // NOLINT
        if (argv[i][2] == '=')                                          // NOLINT
          UnityOptionIncludeNamed = &argv[i][3];                      // NOLINT
        else if (++i < argc)                                            // NOLINT
          UnityOptionIncludeNamed = argv[i];                          // NOLINT
        else {                                                          // NOLINT
          // NOLINT
          UnityPrint("ERROR: No Test String to Include Matches For"); // NOLINT
          UNITY_PRINT_EOL();                                          // NOLINT
          return 1;                                                   // NOLINT
        }                                                               // NOLINT
        break;                                                          // NOLINT
      case 'q': /* quiet */                                               // NOLINT
        UnityVerbosity = 0;                                             // NOLINT
        break;                                                          // NOLINT
      case 'v': /* verbose */                                             // NOLINT
        UnityVerbosity = 2;                                             // NOLINT
        break;                                                          // NOLINT
      case 'x': /* exclude tests with name including this string */       // NOLINT
        if (argv[i][2] == '=')                                          // NOLINT
          UnityOptionExcludeNamed = &argv[i][3];                      // NOLINT
        else if (++i < argc)                                            // NOLINT
          UnityOptionExcludeNamed = argv[i];                          // NOLINT
        else {                                                          // NOLINT
          // NOLINT
          UnityPrint("ERROR: No Test String to Exclude Matches For"); // NOLINT
          UNITY_PRINT_EOL();                                          // NOLINT
          return 1;                                                   // NOLINT
        }                                                               // NOLINT
        break;                                                          // NOLINT
      default:                                                            // NOLINT
        UnityPrint("ERROR: Unknown Option ");                           // NOLINT
        UNITY_OUTPUT_CHAR(argv[i][1]);                                  // NOLINT
        UNITY_PRINT_EOL();                                              // NOLINT
        return 1;                                                       // NOLINT
      }                                                                   // NOLINT
    }                                                                       // NOLINT
  }                                                                           // NOLINT
  // NOLINT
  return 0;                                                                   // NOLINT
} // NOLINT
// NOLINT
int IsStringInBiggerString(const char *longstring, const char *shortstring) {  // NOLINT
  // NOLINT
  const char *lptr = longstring;                                             // NOLINT
  const char *sptr = shortstring;                                            // NOLINT
  const char *lnext = lptr;                                                  // NOLINT
  // NOLINT
  if (*sptr == '*')                                                          // NOLINT
    return 1;                                                              // NOLINT
  // NOLINT
  while (*lptr) {                                                            // NOLINT
    // NOLINT
    lnext = lptr + 1;                                                      // NOLINT
    // NOLINT
    /* If they current bytes match, go on to the next bytes */             // NOLINT
    while (*lptr && *sptr && (*lptr == *sptr)) {                           // NOLINT
      // NOLINT
      lptr++;                                                            // NOLINT
      sptr++;                                                            // NOLINT
      // NOLINT
      /* We're done if we match the entire string or up to a wildcard */ // NOLINT
      if (*sptr == '*')                                                  // NOLINT
        return 1;                                                      // NOLINT
      if (*sptr == ',')                                                  // NOLINT
        return 1;                                                      // NOLINT
      if (*sptr == '"')                                                  // NOLINT
        return 1;                                                      // NOLINT
      if (*sptr == '\'')                                                 // NOLINT
        return 1;                                                      // NOLINT
      if (*sptr == ':')                                                  // NOLINT
        return 2;                                                      // NOLINT
      if (*sptr == 0)                                                    // NOLINT
        return 1;                                                      // NOLINT
    }                                                                      // NOLINT
    // NOLINT
    /* Otherwise we start in the long pointer 1 character further and try again // NOLINT
    */
    // NOLINT
    lptr = lnext;       // NOLINT
    sptr = shortstring; // NOLINT
  }                       // NOLINT
  return 0;               // NOLINT
} // NOLINT
// NOLINT
int UnityStringArgumentMatches(const char *str) {                                  // NOLINT
  // NOLINT
  const char *ptr1;                                                              // NOLINT
  const char *ptr2;                                                              // NOLINT
  // NOLINT
  /* Go through the options and get the substrings for matching one at a time */ // NOLINT
  ptr1 = str;                                                                    // NOLINT
  while (ptr1[0] != 0) {                                                         // NOLINT
    // NOLINT
    if ((ptr1[0] == '"') || (ptr1[0] == '\''))                                 // NOLINT
      ptr1++;                                                                // NOLINT
    // NOLINT
    /* look for the start of the next partial */                               // NOLINT
    ptr2 = ptr1;                                                               // NOLINT
    const char *ptrf = 0;                                                      // NOLINT
    do {                                                                       // NOLINT
      // NOLINT
      ptr2++;                                                                // NOLINT
      if ((ptr2[0] == ':') && (ptr2[1] != 0) && (ptr2[0] != '\'') &&         // NOLINT
          (ptr2[0] != '"') && (ptr2[0] != ','))                              // NOLINT
        ptrf = &ptr2[1];                                                   // NOLINT
    } while ((ptr2[0] != 0) && (ptr2[0] != '\'') && (ptr2[0] != '"') &&        // NOLINT
             (ptr2[0] != ','));                                                // NOLINT
    while ((ptr2[0] != 0) && ((ptr2[0] == ':') || (ptr2[0] == '\'') ||         // NOLINT
                              (ptr2[0] == '"') || (ptr2[0] == ',')))           // NOLINT
      ptr2++;                                                                // NOLINT
    // NOLINT
    /* done if complete filename match */                                      // NOLINT
    int retval = IsStringInBiggerString(Unity.TestFile, ptr1);                 // NOLINT
    if (retval == 1)                                                           // NOLINT
      return retval;                                                         // NOLINT
    // NOLINT
    /* done if testname match after filename partial match */                  // NOLINT
    if ((retval == 2) && (ptrf != 0)) {                                        // NOLINT
      // NOLINT
      if (IsStringInBiggerString(Unity.CurrentTestName, ptrf))               // NOLINT
        return 1;                                                          // NOLINT
    }                                                                          // NOLINT
    // NOLINT
    /* done if complete testname match */                                      // NOLINT
    if (IsStringInBiggerString(Unity.CurrentTestName, ptr1) == 1)              // NOLINT
      return 1;                                                              // NOLINT
    // NOLINT
    ptr1 = ptr2;                                                               // NOLINT
  }                                                                              // NOLINT
  // NOLINT
  /* we couldn't find a match for any substrings */                              // NOLINT
  return 0;                                                                      // NOLINT
} // NOLINT
// NOLINT
int UnityTestMatches(void) {                                          // NOLINT
  // NOLINT
  /* Check if this test name matches the included test pattern */   // NOLINT
  int retval;                                                       // NOLINT
  if (UnityOptionIncludeNamed) {                                    // NOLINT
    // NOLINT
    retval = UnityStringArgumentMatches(UnityOptionIncludeNamed); // NOLINT
  }                                                                 // NOLINT
  else                                                              // NOLINT
    retval = 1;                                                   // NOLINT
  // NOLINT
  /* Check if this test name matches the excluded test pattern */   // NOLINT
  if (UnityOptionExcludeNamed) {                                    // NOLINT
    // NOLINT
    if (UnityStringArgumentMatches(UnityOptionExcludeNamed))      // NOLINT
      retval = 0;                                               // NOLINT
  }                                                                 // NOLINT
  return retval;                                                    // NOLINT
} // NOLINT
// NOLINT
#endif /* UNITY_USE_COMMAND_LINE_ARGS */            // NOLINT
/*-----------------------------------------------*/ // NOLINT
