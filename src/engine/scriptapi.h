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
void scriptexecute(std::string &cmd); // Mis-leading name, allows the script to execute a command.

#endif // SCRIPTAPI_H