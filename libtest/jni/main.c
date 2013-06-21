#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <android/log.h>
#include <sqlite/sqlite3.h>
#include <mydb.h>
#include <elf_hook.h>
#include <locale.h>

#define TAG "hook"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

void _init(char* args) {
	LOGD("lib loaded");
}

int utf16_to_char(const char *in_char, size_t in_size, char *out_char, size_t out_size) {
	char *tmp;
	size_t in_len;
	size_t out_len;
	int i;
	if (in_char == NULL) return;
	if (in_size > 2*out_size+2) return;
	for (i = 0; i < out_size; i++) {
		out_char[i] = in_char[2*i];
	}
	out_char[out_size] = '\0';
	return 0;
}

int my_open_db(const char* fileName, sqlite3 **ppDb, int sqliteFlags, const char* zVfs) {
	LOGD("sqlite3_open_v2 hooked! db path: %s", fileName);
	sqlite3_open_v2(fileName, ppDb, sqliteFlags, NULL);
}

int my_prepare (
		sqlite3 *db,
		const void *zSql,       /* SQL statement, UTF-16 encoded */
		int nByte,              /* Maximum length of zSql in bytes. */
		sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
		const void **pzTail     /* OUT: Pointer to unused portion of zSql */
		) {
	size_t out_size = 2*nByte+2;
	wchar_t *sql = (wchar_t *)malloc(out_size * sizeof(char));
	utf16_to_char(zSql, nByte, sql, out_size);
	LOGD("prepare SQL: %ls", sql);
	LOGD("DB num: %d", db->nDb);
	const char* filename = sqlite3_db_filename(db, "main");
	LOGD("DB file: %s", filename);

	return sqlite3_prepare16_v2(db, zSql, nByte, ppStmt, pzTail);
}

void hooked_my_print(const char* text) {
	LOGD("hooked!");
}

void hooked_log(const char* level, const char* tag, const char* msg) {
	LOGD("hooked!");
}

void so_entry(char* p) {
	// char* sym = "my_print";
	char* sym = "sqlite3_prepare16_v2";
	// char* sym = "sqlite3_open_v2";
	// char* module_path = "/system/lib/libmylib.so";
	char* module_path = "/system/lib/libandroid_runtime.so";
	// char* module_path = "/data/myservice";
	void *handle = dlopen(module_path, RTLD_LAZY);
	void *base = NULL;
	if (get_module_base_address(module_path, handle, &base)) {
		LOGE("[+] Failed to get module base address");
		return;
	}
	void *orig_func;
	orig_func = elf_hook(module_path, base, sym, my_prepare);

	if (orig_func == 0) {
		LOGE("[+] hook %s failed", sym);
		return;
	}
	LOGD("original func 0x%x", orig_func);

	orig_func = elf_hook(module_path, base, "sqlite3_open_v2", my_open_db);
	if (orig_func == 0) {
		LOGE("[+] hook %s failed", sym);
		return;
	}
	LOGD("original func 0x%x", orig_func);
}
