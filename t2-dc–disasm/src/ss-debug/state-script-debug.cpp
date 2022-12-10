/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "state-script-debug.h"
#include "../dc/state-script/state-script.h"
#include "../script/script-module.h"
#include "../script/script-manager-eval.h"
#include "../stringid/sidbase.h"


#include <stdio.h>

extern uintptr_t g_moduleBase;

void PrintVariable(SsDeclaration* pDecl)
{
	switch (pDecl->m_declTypeId)
	{
		case SID("boolean"):
		{
			uint32_t* pVal = reinterpret_cast<uint32_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var boolean '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal == 1 ? "#t" : "#f", ((uintptr_t)pVal-g_moduleBase));
			}
			else
			{
				printf("(var boolean '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("vector"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var vector '%s = %.2f %.2f %.2f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), pVal[0], pVal[1], pVal[2], ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var vector '%s = ??? ??? ???)\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("quat"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var quat '%s = %.4f %.4f %.4f %.4f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), pVal[0], pVal[1], pVal[2], pVal[3], ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var quat '%s = ??? ??? ??? ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("float"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var float '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var float '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("string"):
		{
			const char** pVal = reinterpret_cast<const char**>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var string '%s = \"%s\") //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var string '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("symbol"):
		{
			StringId64* pVal = reinterpret_cast<StringId64*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var symbol '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), StringIdToStringInternal(*pVal), ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var symbol '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			//printf("(var symbol '%s = %s)\n", StringIdToStringInternal(pDecl->m_declId), );
			break;
		}

		case SID("int32"):
		{
			int32_t* pVal = reinterpret_cast<int32_t*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var int32 '%s = %d) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var int32 '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("timer"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var timer '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var timer '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		case SID("point"):
		{
			float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
			if (pVal)
			{
				printf("(var point '%s = %.2f %.2f %.2f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), pVal[0], pVal[1], pVal[2], ((uintptr_t)pVal - g_moduleBase));
			}
			else
			{
				printf("(var point '%s = ??? ??? ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
			}
			break;
		}

		default:
		{
			printf("(var '%s '%s = ???\n", StringIdToStringInternal(pDecl->m_declTypeId), StringIdToStringInternal(pDecl->m_declId));
			puts("Tell icemesh");
			break;
		}
	}
}

void PrintTag(SsDeclaration* pDecl)
{
	switch (pDecl->m_declTypeId)
	{
	case SID("boolean"):
	{
		uint32_t* pVal = reinterpret_cast<uint32_t*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			printf("(tag boolean '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal == 1 ? "#t" : "#f", ((uintptr_t)pVal - g_moduleBase));
		}
		else
		{
			printf("(tag boolean '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}
		break;
	}

	case SID("string"):
	{
		char* pVal = reinterpret_cast<char*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			puts("IMPLEMENT ME SID(string) Tell icemesh");
			//printf("(tag string '%s = \"%s\")")
		}
		else
		{
			printf("(tag string '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}

		break;
	}

	case SID("float"):
	{
		float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			printf("(tag float '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
		}
		else
		{
			printf("(tag float '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}
		break;
	}

	case SID("identifier"):
	{
		StringId64* pVal = reinterpret_cast<StringId64*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			printf("(tag identifier '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), StringIdToStringInternal(*pVal), ((uintptr_t)pVal - g_moduleBase));
		}
		else
		{
			printf("(tag identifier '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}
		
		break;
	}

	case SID("symbol"):
	{
		StringId64* pVal = reinterpret_cast<StringId64*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			printf("(tag symbol '%s = %s) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), StringIdToStringInternal(*pVal), ((uintptr_t)pVal - g_moduleBase));
		}
		else
		{
			printf("(tag symbol '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}
		break;
	}

	case SID("int32"):
	{
		int32_t* pVal = reinterpret_cast<int32_t*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			printf("(tag int32 '%s = %d) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
		}
		else
		{
			printf("(tag int32 '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}
		break;
	}

	case SID("timer"):
	{
		float* pVal = reinterpret_cast<float*>(pDecl->m_pDeclValue);
		if (pVal)
		{
			printf("(tag timer '%s = %f) //value @ %08llX\n", StringIdToStringInternal(pDecl->m_declId), *pVal, ((uintptr_t)pVal - g_moduleBase));
		}
		else
		{
			printf("(tag timer '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declId));
		}
		break;
	}

	default:
	{
		printf("(tag %s '%s = ??? (nullptr))\n", StringIdToStringInternal(pDecl->m_declTypeId), StringIdToStringInternal(pDecl->m_declId));
		puts("Tell icemesh");
		break;
	}
	}
}

void DumpState(SsState* pState)
{
	printf("\t(state (\"%s\")\n", StringIdToStringInternal(pState->m_stateId));
	int64_t maxBlocks = pState->m_numSsOnBlocks;
	for (int32_t iBlocks = 0; iBlocks < maxBlocks; iBlocks++)
	{
		DumpBlocks(&pState->m_pSsOnBlocks[iBlocks]);
	}
	puts("\t)\n");
}

void DumpBlocks(SsOnBlock* pBlock)
{
	const char* blockType = NULL;
	switch (pBlock->m_blockType)
	{
	case 0:
	{
		blockType = "start";
		break;
	}

		case 1:
		{
			//blockType = pBlock->m_name;
			blockType = "end";
			break;
		}

		case 2:
		{
			//blockType = "event";
			//printf("\t\tevent name: %s\n", StringIdToStringInternal(pBlock->m_blockId));
			char buf[0x20];
			snprintf(buf, 0x20, "event '%s", StringIdToStringInternal(pBlock->m_blockEventId));
			blockType = buf;
			break;
		}

		case 3:
		{
			blockType = "update";
			break;
		}

		case 4:
		{
			blockType = "virtual";
			break;
		}

		default:
		{
			blockType = "???";
			break;
		}
	}
	printf("\t\t(on (%s)\n", blockType);
	for (int i = 0; i < pBlock->m_numTracks; i++)
	{
		PrintTrackGroup(&pBlock->m_pTrack[i]);
	}

	puts("\t\t)");
}

void PrintTrackGroup(SsTrack* pTrack)
{
	printf("\t\t\t(track (\"%s\")\n", StringIdToStringInternal(pTrack->m_trackId));
	int16_t tot = pTrack->m_totalLambdaCount;
	SsLambda* pTable = pTrack->m_pSsLambda;
	for (int16_t i = 0; i < tot; i++)
	{
		SsLambda* pdata = &pTable[i];
		ExecuteScriptCode(pdata->m_pScriptLambda);
	}
	puts("\t\t\t)");
}

void DumpScript(StateScript* pStateScript)
{
	printf("(location \"%s\":%d)\n", pStateScript->m_pDebugFileName, pStateScript->m_line);
	printf("\t:initial-state '%s  //value @ %08llX\n", StringIdToStringInternal(pStateScript->m_initialStateId), ((uintptr_t)&(pStateScript->m_initialStateId) - g_moduleBase));
	//dump eventual ss-options
	SsOptions* pOptions = pStateScript->m_pSsOptions;
	if (pOptions != NULL && pOptions->m_pSymbolArray != NULL)
	{
		SymbolArray* pArr = pOptions->m_pSymbolArray;
		printf("\t:options (new ss-options :wait-for-levels (make-symbol-array ");
		for (int32_t iSymbols = 0; iSymbols < pArr->m_numEntries; iSymbols++)
		{
			printf("'%s ", StringIdToStringInternal(pArr->m_pSymbols[iSymbols]));
		}
		puts("))");
	}
	//dump variables
	SsDeclarationList* pDeclTable = pStateScript->m_pSsDeclList;
	if (pDeclTable != NULL)
	{
		int32_t maxDecl = pDeclTable->m_numDeclarations;
		//printf("(declarations (%d)\n", maxDecl);
		printf("\t:declarations (declaration-list \n");
		for (int i = 0; i < maxDecl; i++)
		{
			SsDeclaration* pDecl = &pDeclTable->m_pDeclarations[i];
			if ((pDecl->m_isVar & 1) != 0)
			{
				printf("\t\t");
				PrintVariable(pDecl);
			}
			else
			{
				printf("\t\t");
				PrintTag(pDecl);
			}
		}
		puts("\t)");
	}
	//dump states
	puts("");
	int16_t maxStates = pStateScript->m_numStates;
	for (int16_t iStates = 0; iStates < maxStates; iStates++)
	{
		DumpState(&pStateScript->m_pSsStateTable[iStates]);
	}

}

void ExecuteScriptLambda(ScriptLambda* pLambda)
{
	if (pLambda)
	{
		ExecuteScriptCode(pLambda);
	}
}