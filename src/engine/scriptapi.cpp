// The api for da angelscript!

#include "scriptapi.h"

// Script Engine
asIScriptEngine *scriptengine;
// Context
asIScriptContext *scriptcontext;
// Script builder
CScriptBuilder builder;

// message callback
void scriptmsgcallback(const asSMessageInfo *msg, void *param) {
	int type = CON_ERROR;
	if (msg->type == asMSGTYPE_WARNING)
		type = CON_WARN;
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = CON_INFO;

	conoutf(type, "%s (%d, %d) : %s\n", msg->section, msg->row, msg->col, msg->message);
}
// wrapper around conoutf
void scriptlogfunc(const char* msg) {
	conoutf(msg);
}

// Initialize the angelscript engine.
bool initscriptengine() {
	scriptengine = asCreateScriptEngine();

	int r = scriptengine->SetMessageCallback(asFUNCTION(scriptmsgcallback), 0, asCALL_CDECL);
	if (r < 0)
		return false;

	// Use the script type from the angelscript addon
	// TODO: Might we be better making it ourselves?
	RegisterStdString(scriptengine);

	// Register C functions
	r = scriptengine->RegisterGlobalFunction("void log(const char *in)", asFUNCTION(scriptlogfunc), asCALL_CDECL);
	if (r < 0)
		return false;
	
	// Run builtin
	scriptrunfile("script/builtin.as");

	return true;
}

void destructscriptengine() {
	if (scriptengine)
		scriptengine->ShutDownAndRelease();
	if (scriptcontext)
		scriptcontext->Release();
}

bool scriptrunfile(const char *fp) {
	// Make the builder
	int r = builder.StartNewModule(scriptengine, fp);
	if (r < 0) {
		fatal("Could not initialize new module! FATAL!");
		return false;
	}

	r = builder.AddSectionFromFile(fp);
	if (r < 0) {
		return false;
	}

	r = builder.BuildModule();
	if (r < 0) {
		return false;
	}

	// here we go
	asIScriptModule *mod = scriptengine->GetModule(fp);
	asIScriptFunction *entry = mod->GetFunctionByDecl("void main()");
	if (entry == 0) {
		conoutf(CON_ERROR, "The script %s is missing an entry point (void main()).", fp);
		return false;
	}

	// Create our context, prepare it, and then execute
	scriptcontext = scriptengine->CreateContext();
	scriptcontext->Prepare(entry);
	r = scriptcontext->Execute();
	if( r != asEXECUTION_FINISHED )
	{
		// The execution didn't complete as expected. Determine what happened.
		if( r == asEXECUTION_EXCEPTION )
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			// printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
			conoutf(CON_ERROR, "Angel Script exception \"%s\"", scriptcontext->GetExceptionString());
			scriptcontext->Release();
			return false;
		}
	}
	scriptcontext->Release();

	return true;
}