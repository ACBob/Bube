#ifndef SCRIPTAPI_H
#define SCRIPTAPI_H

#include "engine.h"

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

// Script Engine
extern asIScriptEngine *scriptengine;
// Context
extern asIScriptContext *scriptcontext;
// Script builder
extern CScriptBuilder builder;

bool scriptrunfile(const char *fp);
bool initscriptengine();

void destructscriptengine();

#endif // SCRIPTAPI_H