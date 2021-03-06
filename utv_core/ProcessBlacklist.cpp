/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id$ */

#include "stdafx.h"
#include "ProcessBlacklist.h"
#include "EnvFlag.h"

#if defined(_WIN32)
#define PATHSEP '\\'
#define PATHENVSEPSTR ";"
#elif defined(__APPLE__) || defined(__unix__)
#define PATHSEP '/'
#define PATHENVSEPSTR ":"
#endif

#if defined(_WIN32)
#define safe_strtok strtok_s
#define pathcmp _stricmp
#define pathncmp _strnicmp
#elif defined(__APPLE__) || defined(__unix__)
#define safe_strtok strtok_r
#define pathcmp strcmp
#define pathncmp strncmp
#endif

static char szProcessFullName[256];
static const char* pszProcessBaseName = NULL;
static std::once_flag once;

static bool MatchProcessName(char* pszBlacklist)
{
	char* context = NULL;
	for (auto p = safe_strtok(pszBlacklist, PATHENVSEPSTR, &context); p != NULL; p = safe_strtok(NULL, PATHENVSEPSTR, &context))
	{
		const char* sep = strrchr(p, PATHSEP);
		size_t len = strlen(p);
		if (sep == NULL)
		{
			// パス区切り文字を含まない -> basename と比較
			if (pathcmp(p, pszProcessBaseName) == 0)
				return true;
		}
		else if (sep == p + len - 1)
		{
			// パス区切り文字が最後にある -> prefix として比較
			if (pathncmp(p, szProcessFullName, len) == 0)
				return true;
		}
		else
		{
			// その他 -> fullname と比較
			if (pathcmp(p, szProcessFullName) == 0)
				return true;
		}
	}
	return false;
}

DLLEXPORT bool IsInterfaceDisabled(const char* pszInterface, const char* pszRole)
{
	std::call_once(once, []() {
#if defined(_WIN32)
		GetModuleFileName(NULL, szProcessFullName, sizeof(szProcessFullName));
		pszProcessBaseName = strrchr(szProcessFullName, PATHSEP);
		if (pszProcessBaseName != NULL)
			pszProcessBaseName++;
		else
			pszProcessBaseName = szProcessFullName;
#endif
	});

	char envname[256];

	snprintf(envname, sizeof(envname), "UTVIDEO_DISABLE_%s_%s", pszInterface, pszRole);
	envname[sizeof(envname) - 1] = '\0';
	for (auto p = envname; *p; ++p)
		*p = toupper((unsigned char)*p);

	if (GetEnvFlagBool(envname))
		return true;

	snprintf(envname, sizeof(envname), "UTVIDEO_PROCESSBLACKLIST_%s_%s", pszInterface, pszRole);
	envname[sizeof(envname)-1] = '\0';
	for (auto p = envname; *p; ++p)
		*p = toupper((unsigned char)*p);

	char* envvar;
#if defined(_WIN32)
	char buf;
	DWORD cch = GetEnvironmentVariable(envname, &buf, 0);
	if (cch != 0)
	{
		envvar = (char*)malloc(cch);
		GetEnvironmentVariable(envname, envvar, cch);
#elif defined(__APPLE__) || defined(__unix__)
	envvar = getenv(envname);
	if (envvar != NULL)
	{
		envvar = strdup(envvar);
#endif
		auto ret = MatchProcessName(envvar);
		free(envvar);
		if (ret)
			return ret;
	}

	return false;
}
