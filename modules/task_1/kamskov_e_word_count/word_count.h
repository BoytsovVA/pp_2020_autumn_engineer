// Copyright 2020 Kamskov Eugene

#ifndef MODULES_TASK_1_KAMSKOV_E_WORD_COUNT_WORD_COUNT_H_
#define MODULES_TASK_1_KAMSKOV_E_WORD_COUNT_WORD_COUNT_H_

#define EMPTY_STRING_ERROR -1

#include <string>

int ParCount(const std::string);
int SimpleCount(const std::string);
int SimpleCountDelta(const std::string);
/**
 * @brief Creates string with the specified count of random words.
 * @param wordCount Count of random words in a result string.
*/
std::string GenRandString(int);

#endif  //  MODULES_TASK_1_KAMSKOV_E_WORD_COUNT_WORD_COUNT_H_
