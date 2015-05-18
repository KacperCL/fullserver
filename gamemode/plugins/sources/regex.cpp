/*----------------------------------------------------------------------------------------------------*-

Автор:
	Fro он же Fro1sha
	
Описание:
	Regular Expression
 
Права: 
    Copyright © 2009-2011 TBG
	
	1)	Запрещается любое коммерческое использование плагина.
	2)	Данный плагин поставляется по принципу "as is"("как есть"). 
		Никаких гарантий не прилагается и не предусматривается. 
		Вы используете плагин на свой страх и риск. 
		Авторы не будут отвечать ни за какие потери или искажения данных, 
		любую упущенную выгоду в процессе использования или неправильного использования плагина.
	3)  Вы НЕ имеете права где-либо размещать, а также модифицировать плагин или любую его часть, 
		предварительно не получив согласия Авторов.
	4)  Установка и использование данного плагина означает, что Вы ознакомились и понимаете 
		положения настоящего лицензионного соглашения и согласны с ними. 

                            ______              __________             
                            ___  /______  __    ___  ____/____________ 
                            __  __ \_  / / /    __  /_   __  ___/  __ \
                            _  /_/ /  /_/ /     _  __/   _  /   / /_/ /
                            /_.___/_\__, /      /_/      /_/    \____/ 
                                   /____/                              
                                                                              
																				
           _______________       _________                                              
           ___  __/__  __ )      __  ____/_____ _______ ________________ ___________  __
           __  /  __  __  |_______  / __ _  __ `/_  __ `__ \  _ \_  ___/ __  ___/  / / /
           _  /   _  /_/ /_/_____/ /_/ / / /_/ /_  / / / / /  __/(__  )___  /   / /_/ / 
           /_/    /_____/        \____/  \__,_/ /_/ /_/ /_/\___//____/_(_)_/    \__,_/  
                                                                                        

	http://tb-games.ru/
	
-*----------------------------------------------------------------------------------------------------*/



#define PLUGIN_VERSION							"0.2.1"

// -------------------------------------------------------------------------------------*-

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#include <malloc.h>
#include <string.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <boost/regex.hpp>

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"


// -------------------------------------------------------------------------------------*-

typedef void (* logprintf_t)(char *, ...);

// -------------------------------------------------------------------------------------*-

#define CHECK_PARAMS(m, n) \
	if (params[0] != (m * 4)) \
	{ \
		logprintf("Erorr native function %s: Expecting %d parameter(s), but found %d", n, m, params[0] / 4); \
		return 0; \
	}

// -------------------------------------------------------------------------------------*-

logprintf_t
	logprintf;

extern void *
	pAMXFunctions;

// -------------------------------------------------------------------------------------*-

struct 
	RegularExpression
{
	RegularExpression()
	{
		references = 0;
	}

	AMX 
		* amx
	;

	int 
		references
	;

	boost::regex expression;
};

// -------------------------------------------------------------------------------------*-

typedef boost::intrusive_ptr<RegularExpression> RegEx;

boost::unordered_map<int, RegEx> regEx;

// -------------------------------------------------------------------------------------*-

namespace boost
{
	void intrusive_ptr_add_ref(RegularExpression * regex)
	{
		++regex->references;		
	}

	void intrusive_ptr_release(RegularExpression * regex)
	{
		if ( !(--regex->references) )
		{
			delete regex;
		}
	}
}
// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT unsigned int PLUGIN_CALL
	Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT bool PLUGIN_CALL
	Load(void ** ppData)
{		
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	logprintf("\n\r");
	logprintf("______________________________________\n\r");
	logprintf(" Regular Expression Plugin v"PLUGIN_VERSION" loaded");
	logprintf("______________________________________\n\r");
	logprintf(" By: Fro (c) Copyright <TBG> 2009-2011");
	logprintf("______________________________________\n\r");
	return true;
}

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT void PLUGIN_CALL
	Unload()
{
	logprintf("\n\r");
	logprintf("______________________________________\n\r");
	logprintf(" Regular Expression Plugin v"PLUGIN_VERSION" unloaded");
	logprintf("______________________________________\n\r");
}

// -------------------------------------------------------------------------------------*-

// native RegEx:regex_build(const expression[]);
static cell AMX_NATIVE_CALL
	n_regex_build(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "regex_build");

	char 
		* expression
	;	

	amx_StrParam(amx, params[1], expression);	

	if ( expression )
	{
		int
			expID = 1
		;

		for ( boost::unordered_map<int, RegEx>::const_iterator e = regEx.begin(); e != regEx.end(); ++e )
		{
			if ( e->first != expID )
			{
				break;
			}
			++expID;
		}

		RegEx 
			exp(new RegularExpression)
		;

		exp->amx = amx;
		exp->expression = expression;

		regEx.insert(std::make_pair(expID, exp));
		return (cell)expID;
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_delete(RegEx:expID);
static cell AMX_NATIVE_CALL
	n_regex_delete(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "regex_delete");

	boost::unordered_map<int, RegEx>::iterator e = regEx.find((int)params[1]);
	if ( e != regEx.end() )
	{
		regEx.quick_erase(e);
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_isvalid(RegEx:expID);
static cell AMX_NATIVE_CALL
	n_regex_isvalid(AMX * amx, cell * params)
{
	CHECK_PARAMS(1, "regex_isvalid");

	boost::unordered_map<int, RegEx>::iterator e = regEx.find((int)params[1]);
	if ( e != regEx.end() )
	{
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_delete_all();
static cell AMX_NATIVE_CALL
	n_regex_delete_all(AMX * amx, cell * params)
{
	if ( !regEx.empty() )
	{
		regEx.clear();
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_match(const string[], const expression[]);
static cell AMX_NATIVE_CALL
	n_regex_match(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "regex_match");

	char 
		* string,
		* expression
	;	

	amx_StrParam(amx, params[1], string);
	amx_StrParam(amx, params[2], expression);	

	if ( string && expression )
	{
		return (cell)boost::regex_match(string, boost::regex(expression));
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_search(const string[], const expression[]);
static cell AMX_NATIVE_CALL
	n_regex_search(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "regex_search");

	char 
		* string,
		* expression
	;	

	amx_StrParam(amx, params[1], string);
	amx_StrParam(amx, params[2], expression);	

	if ( string && expression )
	{	
		return (cell)boost::regex_search(string, boost::regex(expression));
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// regex_replace(const string[], const expression[], const to[], dest[], size = sizeof dest);
static cell AMX_NATIVE_CALL
	n_regex_replace(AMX * amx, cell * params)
{
	CHECK_PARAMS(5, "regex_replace");

	char 
		* string,		
		* expression,
		* to
	;	

	amx_StrParam(amx, params[1], string);	
	amx_StrParam(amx, params[2], expression);	
	amx_StrParam(amx, params[3], to);

	if ( string && expression )
	{
		std::string
			text
		;

		text.append(string);

		text = boost::regex_replace(text, boost::regex(expression), to);

		cell 
			* buf
		;
		int
			size = text.length()
		;

		if ( (int)params[5] < size ) size = (int)params[5];
		
		amx_GetAddr(amx, params[4], &buf);
		amx_SetString(buf, text.c_str(), 0, 0, size + 1);
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_match_exid(const string[], RegEx:expID);
static cell AMX_NATIVE_CALL
	n_regex_match_exid(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "regex_match_exid");

	boost::unordered_map<int, RegEx>::iterator e = regEx.find((int)params[2]);
	if ( e != regEx.end() )
	{
		char 
			* string
		;	

		amx_StrParam(amx, params[1], string);

		if ( string )
		{
			return (cell)boost::regex_match(string, e->second->expression);
		}
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// native regex_search_exid(const string[], RegEx:expID);
static cell AMX_NATIVE_CALL
	n_regex_search_exid(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "regex_search_exid");

	boost::unordered_map<int, RegEx>::iterator e = regEx.find((int)params[2]);
	if ( e != regEx.end() )
	{
		char 
			* string
		;	

		amx_StrParam(amx, params[1], string);

		if ( string )
		{	
			return (cell)boost::regex_search(string, e->second->expression);
		}
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

// regex_replace_exid(const string[], RegEx:expID, const to[], dest[], size = sizeof dest);
static cell AMX_NATIVE_CALL
	n_regex_replace_exid(AMX * amx, cell * params)
{
	CHECK_PARAMS(5, "regex_replace_exid");

	boost::unordered_map<int, RegEx>::iterator e = regEx.find((int)params[2]);
	if ( e != regEx.end() )
	{
		char 
			* string,		
			* to
		;	

		amx_StrParam(amx, params[1], string);	
		amx_StrParam(amx, params[3], to);

		if ( string )
		{
			std::string
				text
			;

			text.append(string);

			text = boost::regex_replace(text, e->second->expression, to);

			cell 
				* buf
			;
			int
				size = text.length()
			;

			if ( (int)params[5] < size ) size = (int)params[5];
			
			amx_GetAddr(amx, params[4], &buf);
			amx_SetString(buf, text.c_str(), 0, 0, size + 1);
			return 1;
		}
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

AMX_NATIVE_INFO
	regex_natives[] =
{
	{ "regex_build", n_regex_build },
	{ "regex_delete", n_regex_delete },
	{ "regex_isvalid", n_regex_isvalid },
	{ "regex_delete_all", n_regex_delete_all },

	{ "regex_match", n_regex_match },
	{ "regex_search", n_regex_search },
	{ "regex_replace", n_regex_replace },

	{ "regex_match_exid", n_regex_match_exid },
	{ "regex_search_exid", n_regex_search_exid },
	{ "regex_replace_exid", n_regex_replace_exid },

	{ 0, 0 }
};

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT int PLUGIN_CALL
	AmxLoad(AMX * amx)
{
	return amx_Register(amx, regex_natives, -1);
}

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT int PLUGIN_CALL
	AmxUnload(AMX * amx)
{
	boost::unordered_map<int, RegEx>::const_iterator e = regEx.begin();
	while ( e != regEx.end() )
	{
		if ( e->second->amx == amx )
		{
			regEx.quick_erase(e);
		}
		else
		{
			++e;
		}
	}
	return AMX_ERR_NONE;
}

// -------------------------------------------------------------------------------------*-