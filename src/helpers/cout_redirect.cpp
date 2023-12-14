#include <iostream>
#include <string>

#include "helpers/cout_redirect.hpp"

/**
 * @class cout_redirect
 * @brief
 *
 */
CoutRedirect::CoutRedirect(std::streambuf *new_buffer)
    : old(std::cout.rdbuf(new_buffer)) {}

CoutRedirect::~CoutRedirect() { std::cout.rdbuf(old); }
