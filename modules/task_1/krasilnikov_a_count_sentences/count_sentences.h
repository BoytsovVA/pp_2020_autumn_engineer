// Copyright 2020 Krasilnikov Alexey

#ifndef MODULES_TASK_1_KRASILNIKOV_A_COUNT_SENTENCES_COUNT_SENTENCES_H_
#define MODULES_TASK_1_KRASILNIKOV_A_COUNT_SENTENCES_COUNT_SENTENCES_H_

#include <string>

std::string getRandomString(const size_t size);
uint32_t getCountSentencesSequential(const std::string& str);
uint32_t getCountSentencesParallel(const std::string& str, const size_t size_str);


#endif  //  MODULES_TASK_1_KRASILNIKOV_A_COUNT_SENTENCES_COUNT_SENTENCES_H_
