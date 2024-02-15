# Compatability with the following C libraries across linux distributions is
# assumed. Use this constant in the install command to exclude these libraries
# from the installation.
set(EXCLUDE_C_LIBS_REGEX
    ".*ld-linux.*\\.so.*" ".*libc\\.so.*" ".*libm\\.so.*" ".*libdbm\\.so.*"
    ".*libpthread\\.so.*"
    CACHE STRING "Regular expression to exclude C libraries from installation")
