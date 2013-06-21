##
##
## Build the library
##
##

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := libsqlite.so
LOCAL_MODULE := sqlite
include $(PREBUILT_SHARED_LIBRARY)

# NOTE the following flags,
#   SQLITE_TEMP_STORE=3 causes all TEMP files to go into RAM. and thats the behavior we want
#   SQLITE_ENABLE_FTS3   enables usage of FTS3 - NOT FTS1 or 2.
#   SQLITE_DEFAULT_AUTOVACUUM=1  causes the databases to be subject to auto-vacuum
common_sqlite_flags := \
	-DNDEBUG=1 \
	-DHAVE_USLEEP=1 \
	-DSQLITE_HAVE_ISNAN \
	-DSQLITE_DEFAULT_JOURNAL_SIZE_LIMIT=1048576 \
	-DSQLITE_THREADSAFE=2 \
	-DSQLITE_TEMP_STORE=3 \
	-DSQLITE_POWERSAFE_OVERWRITE=1 \
	-DSQLITE_DEFAULT_FILE_FORMAT=4 \
	-DSQLITE_DEFAULT_AUTOVACUUM=1 \
	-DSQLITE_ENABLE_MEMORY_MANAGEMENT=1 \
	-DSQLITE_ENABLE_FTS3 \
	-DSQLITE_ENABLE_FTS3_BACKWARDS \
	-DSQLITE_ENABLE_FTS4 \
	-DSQLITE_OMIT_BUILTIN_TEST \
	-DSQLITE_OMIT_COMPILEOPTION_DIAGS \
	-DSQLITE_OMIT_LOAD_EXTENSION \
	-DSQLITE_DEFAULT_FILE_PERMISSIONS=0600


# the device library
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    main.c \
	elf_hook.c

LOCAL_CFLAGS += $(common_sqlite_flags) -Dfdatasync=fdatasync

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_MODULE := test

LOCAL_SHARED_LIBRARIES := libdl liblog libsqlite \
            libicuuc \
            libicui18n \
            libutils

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog -ldl
 
include $(BUILD_SHARED_LIBRARY)

