#if defined _language_included
	#endinput
#endif 

#define _language_included	

#include <a_samp>


#if !defined MAX_LANGUAGES 
	#define MAX_LANGUAGES 		2
#endif


#if !defined MAX_LANGUAGE_NAME
	#define MAX_LANGUAGE_NAME 	32
#endif	


#if !defined MAX_LANGUAGE_TEXTS
	#define MAX_LANGUAGE_TEXTS 				32
#endif	


#if !defined MAX_LANGUAGE_TEXT_SIZE
	#define MAX_LANGUAGE_TEXT_SIZE 			64
#endif


#define Language_IsValid(%0)	(-1 < (%0) < s_LanguageCount)
#define ___isnull(%0) 			((%0[(%0[0])=='\1']) == '\0')


#if defined OnPlayerChangeLanguage
	forward OnPlayerChangeLanguage(playerid, newlang, oldlang);
#endif	


//
//	DATA
//



static s_LanguageCount;
static s_LangTexts[MAX_LANGUAGES][MAX_LANGUAGE_TEXTS][MAX_LANGUAGE_TEXT_SIZE];
static s_LangTag[MAX_LANGUAGE_TEXTS];
static s_LangTextIndex[MAX_LANGUAGES];

static s_PlayerLang[MAX_PLAYERS];


#define T(%0,%1) 				Language_GetText(%0,#%1)
#define T@(%0)					Language_GetText(GetPlayerLanguage(playerid),#%0)



//
//	FUNCTIONS
//


stock Language_Add()
{
	new current = s_LanguageCount;

	if(current + 1 >= MAX_LANGUAGES)
	{	
		printf("[Language]: Language_Add reached your limit of %d", MAX_LANGUAGES);	
		return -1;
	}

	++s_LanguageCount;

	return current;	
}



stock Language_AddTranslation(langid, const tag[], const string[], length = sizeof string)
{
	if(!Language_IsValid(langid) || ___isnull(string) || ___isnull(tag))
		return 0;

	if(length > MAX_LANGUAGE_TEXT_SIZE)
	{
		printf("[Language]: Language_Add reached caracters limit: %d", MAX_LANGUAGE_TEXT_SIZE);	
		return 0;		
	}

	new cur = s_LangTextIndex[langid]++;
	strcat(s_LangTexts[langid][cur], string, length);
	s_LangTag[cur] = bernstein(tag);
	return 1;
}



stock Language_GetText(langid, tag[])
{
	if(!Language_IsValid(langid))
		langid = 0;

	new string[MAX_LANGUAGE_TEXT_SIZE];

	if(Language_GetTextAsArray(langid, tag, string) == 0)
	{
		printf("[Language] Language_GetText unknown tag '%s'", tag);
	}	

	return string;
}



stock Language_GetTextAsArray(langid, string:tag[], output[])
{
	for(new x, value = bernstein(tag); x < s_LangTextIndex[langid]; x++)
	{
		if(s_LangTag[x] == value)
		{
			strcat(output, s_LangTexts[langid][x], MAX_LANGUAGE_TEXT_SIZE);
			return 1;
		}
	}

	return 0;
}


stock SetPlayerLanguage(playerid, language)
{
	if(!IsPlayerConnected(playerid))
		return 0;

	if(!Language_IsValid(language))
		return 0;

	if(s_PlayerLang[playerid] != language)
	{
		#if defined OnPlayerChangeLanguage
			OnPlayerChangeLanguage(playerid, language, s_PlayerLang[playerid]);
		#endif	

		s_PlayerLang[playerid] = language;
	}
	return 1;
}


stock GetPlayerLanguage(playerid)
{
	return IsPlayerConnected(playerid) ? 0 : s_PlayerLang[playerid];
}


//
//	CALLBACKS
//



public OnPlayerConnect(playerid)
{
	s_PlayerLang[playerid] = 0;

	#if defined LANG_OnPlayerConnect
		return LANG_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}



//
//	HOOKS
//



#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect LANG_OnPlayerConnect
#if defined LANG_OnPlayerConnect
	forward LANG_OnPlayerConnect(playerid);
#endif


#undef ___isnull