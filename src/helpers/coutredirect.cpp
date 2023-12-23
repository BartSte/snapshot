#include <helpers/coutredirect.hpp>

#include <iostream>
#include <string>

/**
 * @class coutredirect
 * @brief
 *
 */
CoutRedirect::CoutRedirect(std::streambuf *new_buffer)
    : old(std::cout.rdbuf(new_buffer)) {}

CoutRedirect::~CoutRedirect() { std::cout.rdbuf(old); }
