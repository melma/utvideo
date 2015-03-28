/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id$ */

#pragma once

int InitializeLogWriter(void);
int __declspec(dllexport) WriteLog(const char *p);
int UninitializeLogWriter(void);

#ifdef _WIN32

// 関数の場合はインポートする側も dllexport で動いてしまうが、変数の場合はそうはいかない。
#ifdef LOGWRITER_OWNER
extern __declspec(dllexport) HANDLE hLogPipe;
#else
extern __declspec(dllimport) HANDLE hLogPipe;
#endif

static inline bool IsLogWriterInitialized()
{
	return ::hLogPipe != INVALID_HANDLE_VALUE;
}

#ifdef _DEBUG

static inline bool IsLogWriterInitializedOrDebugBuild()
{
	return true;
}

#define LOGPRINTF(__fmt__, ...) \
	do \
	{ \
		char __LOGPRINTF_local_buf1__[256]; \
		char __LOGPRINTF_local_buf2__[256]; \
		sprintf(__LOGPRINTF_local_buf1__, __fmt__, __VA_ARGS__); \
		sprintf(__LOGPRINTF_local_buf2__, "<%s> %s", LOG_MODULE_NAME, __LOGPRINTF_local_buf1__); \
		if (IsLogWriterInitialized()) \
		{ \
			WriteLog(__LOGPRINTF_local_buf2__); \
		} \
		strcat(__LOGPRINTF_local_buf2__, "\n"); \
		OutputDebugStringA(__LOGPRINTF_local_buf2__); \
	} while (false)

#define DBGPRINTF(__fmt__, ...) \
	do \
	{ \
		char __LOGPRINTF_local_buf1__[256]; \
		char __LOGPRINTF_local_buf2__[256]; \
		sprintf(__LOGPRINTF_local_buf1__, __fmt__, __VA_ARGS__); \
		sprintf(__LOGPRINTF_local_buf2__, "<%s> %s", LOG_MODULE_NAME, __LOGPRINTF_local_buf1__); \
		strcat(__LOGPRINTF_local_buf2__, "\n"); \
		OutputDebugStringA(__LOGPRINTF_local_buf2__); \
	} while (false)

#else

static inline bool IsLogWriterInitializedOrDebugBuild()
{
	return IsLogWriterInitialized();
}

#define LOGPRINTF(__fmt__, ...) \
	do \
	{ \
		if (IsLogWriterInitialized()) \
		{ \
			char __LOGPRINTF_local_buf1__[256]; \
			char __LOGPRINTF_local_buf2__[256]; \
			sprintf(__LOGPRINTF_local_buf1__, __fmt__, __VA_ARGS__); \
			sprintf(__LOGPRINTF_local_buf2__, "<%s> %s", LOG_MODULE_NAME, __LOGPRINTF_local_buf1__); \
			WriteLog(__LOGPRINTF_local_buf2__); \
		} \
	} while (false)

#define DBGPRINTF(__fmt__, ...) \
	do \
	{ \
	} while (false)

#endif

#endif
