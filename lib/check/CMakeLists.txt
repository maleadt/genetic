# We'll build the POSIX version
ADD_DEFINITIONS(-D_POSIX_VERSION)

# Auxiliary libraries
ADD_LIBRARY(check_error check_error.h check_error.c)
ADD_LIBRARY(check_list check_list.h check_list.c)
ADD_LIBRARY(check_pack check_pack.h check_pack.c)
ADD_LIBRARY(check_print check_print.h check_print.c)
ADD_LIBRARY(check_str check_str.h check_str.c)

# Log library
ADD_LIBRARY(check_log check_log.h check_log.c)
TARGET_LINK_LIBRARIES(check_log check_error check_list check_print check_str)

# Suite runner
ADD_LIBRARY(check_run check_run.c)
TARGET_LINK_LIBRARIES(check_run check_log)

# Message handling
ADD_LIBRARY(check_msg check_msg.h check_msg.c)
TARGET_LINK_LIBRARIES(check_msg check_pack)

# Main library
ADD_LIBRARY(check check.h check.c)
TARGET_LINK_LIBRARIES(check check_error check_list check_msg)

