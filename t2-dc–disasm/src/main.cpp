/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "dc/dc.h"
#include "script/script-module.h"
#include "stringid/stringid-cache.h"
#include "script/script-manager-eval.h"
#include "stringid/sidbase.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

StringIdCache* g_pStringIdCache = nullptr;
uintptr_t g_moduleBase = 0;
DC::StackFrame* g_stackFrame = nullptr;
//DC::StackFrame* g_unkArea = nullptr;


int main(int argc, const char* aArgv[])
{
	puts("-------------[ t2 script disassembler by icemesh ]--------------\n");
	if (argc > 1)
	{
		Module* pModule = new Module(aArgv[1]);
		if (pModule)
		{
			if (pModule->Login())
			{
				void* pFile = pModule->GetLoadedFilePtr();
				g_moduleBase = reinterpret_cast<uintptr_t>(pFile);
				DC_Header* pHdr = reinterpret_cast<DC_Header*>(pFile);
				g_pStringIdCache = new StringIdCache();
				g_stackFrame = (DC::StackFrame*)malloc(sizeof(DC::StackFrame) * 2);
				assert(g_stackFrame != NULL);
				g_pStringIdCache->GetDebugInfo(pHdr);
				InitSidbase();
				int32_t numEntries = pHdr->m_numEntries;
				Entry* pTable = pHdr->m_pStartOfData;
				for (int32_t i = 0; i < numEntries; i++)
				{
					pModule->DumpEntry(&pTable[i]);
				}
				ShutdownSidbase();
				g_pStringIdCache->PrintCacheEntries();
				delete g_pStringIdCache;
				free(g_stackFrame);
			}
			delete pModule;
			return 0;
		}
		else
		{
			puts("Failed to allocate module class");
			return -1;
		}
	}
	else
	{
		//print usage
		puts("Usage: .\\t2-dc-disasm ss-xxxxx.bin");
		return 0;
	}
}
