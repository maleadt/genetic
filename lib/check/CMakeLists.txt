# We'll build the POSIX version
ADD_DEFINITIONS(-D_POSIX_VERSION)

# Auxiliary libraries
ADD_LIBRARY(check_error error.h error.c)
ADD_LIBRARY(check_list list.h list.c)
ADD_LIBRARY(check_pack pack.h pack.c)
ADD_LIBRARY(check_print print.h print.c)
ADD_LIBRARY(check_str str.h str.c)

# Log library
ADD_LIBRARY(check_log log.h log.c)
TARGET_LINK_LIBRARIES(check_log check_error check_list check_print check_str)

# Suite runner
ADD_LIBRARY(check_run run.c)
TARGET_LINK_LIBRARIES(check_run check_log)

# Message handling
ADD_LIBRARY(check_msg msg.h msg.c)
TARGET_LINK_LIBRARIES(check_msg check_pack)

# Main library
ADD_LIBRARY(check check.h check.c)
TARGET_LINK_LIBRARIES(check check_error check_list check_msg)

