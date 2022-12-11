/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
#include "script-manager-eval.h"
#include "../dc/state-script/state-script.h"
#include "script-module.h"
#include "../stringId/sidbase.h"

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <intrin.h> //bittest
#include <math.h> //fmodf


typedef uint32_t ScriptInstruction;

extern DC::StackFrame* g_stackFrame;
extern DC::StackFrame* g_unkArea;

extern uintptr_t g_moduleBase;

int g_globalIdx = 0;

DC::Variant ExecuteScriptCode(ScriptLambda* pLambda)
{
	memset(g_stackFrame, 0x0, sizeof(DC::StackFrame) * 2);
	int64_t idx = 0;
	int64_t tmpIdx = 0;
	int numberOfMovBeforeCall = 0;
	uint8_t* pOpcode = pLambda->m_pOpcode;
	DC::Variant* pSymbolTbl = reinterpret_cast<DC::Variant*>(pLambda->m_pSymbols);
	//printf("\t\t\t\tBlock_%d\n", g_globalIdx);
	puts("\t\t\t\t(");
	while (1)
	{
		int8_t opcode = pOpcode[sizeof(ScriptInstruction) * idx];
		uint8_t dstReg = pOpcode[sizeof(ScriptInstruction) * idx + 0x1];
		uint8_t operand1 = pOpcode[sizeof(ScriptInstruction) * idx + 0x2];
		uint8_t operand2 = pOpcode[sizeof(ScriptInstruction) * idx + 0x3];
		tmpIdx = idx + 0x1;
		printf("\t\t\t\t\tCODE: 0x%.08llX %02X %02X %02X %02X ", (reinterpret_cast<uintptr_t>(&pOpcode[sizeof(ScriptInstruction) * idx]) - g_moduleBase), opcode, dstReg, operand1, operand2);
		switch (opcode)
		{
			case DC::kScriptCodeReturn:
			{
				DC::Variant ret = g_stackFrame->m_registers[operand1];
				printf("DC::kScriptCodeReturn: 0x%.16llX\n", ret.m_U64);
				puts("\t\t\t\t)");
				return ret;
			}
			case DC::kScriptCodeIAdd:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 + g_stackFrame->m_registers[operand2].m_I64;
				idx = tmpIdx;
				printf("DC::kScriptCodeIAdd: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				break;
			}

			case DC::kScriptCodeISubtract:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 - g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kScriptCodeISubtract: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeIMultiply:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 * g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kScriptCodeIMultiply: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeIDivide:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 / g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kScriptCodeIMultiply: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFAdd:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 + g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kScriptCodeFAdd: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFSubtract:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 - g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kScriptCodeFSubtract: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFMultiply:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 * g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kScriptCodeFMultiply: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kScriptCodeFDivide:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 / g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kScriptCodeFDivide: S%d, S%d, S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticInteger:
				//			case DC::kLoadStaticPointer:
			{
				DC::Variant val = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64];
				//g_stackFrame->m_registers[dstReg] = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64];
				g_stackFrame->m_registers[dstReg] = val;
				printf("DC::kLoadStaticInteger: R%d, [pSymbolTbl, R%d] -> 0x%016llX\n", dstReg, operand1, g_stackFrame->m_registers[dstReg].m_I64);
				//printf("Registers:  R%d -> 0x%.16llX\n\t\t\t\t\t\t\tR%d -> 0x%.16llX\n", dstReg, g_stackFrame->m_registers[dstReg].m_I64, operand1, g_stackFrame->m_registers[operand1].m_U32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticFloat:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64].m_U32;
				printf("DC::kLoadStaticFloat: S%d, [pSymbolTbl, S%d]\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticPointer:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[g_stackFrame->m_registers[operand1].m_I64].m_U64;
				printf("DC::kLoadStaticPointer: R%d, [pSymbolTbl, R%d]\n", dstReg, operand1);
				//printf("Registers:  R%d -> 0x%.16llX\n\t\t\t\t\t\t\tR%d -> 0x%.16llX\n", dstReg, g_stackFrame->m_registers[dstReg].m_I64, operand1, g_stackFrame->m_registers[operand1].m_U32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU16Immediate:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = (operand1 | (operand2 << 0x8));
				printf("DC::kLoadU16Immediate: R%d, %d | (%d << 0x8)\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU32:
			{
			//	g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_U32;
				printf("DC::kLoadU32: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadFloat:
			{
			//	g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_I64;
				g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_U32;
				printf("DC::kLoadFloat: S%d, [R%d]\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadPointer:
			case DC::kLoadI64:
			case DC::kLoadU64:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64;
				printf("DC::kLoadPointer/kLoadI64/kLoadU64: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreInteger:
			case DC::kStoreU64:
			{
				uint64_t data = g_stackFrame->m_registers[operand2].m_U64;
				g_stackFrame->m_registers[operand1].m_U64 = data;
				g_stackFrame->m_registers[dstReg].m_U64 = data;
				printf("DC::kStoreInteger/kStoreU64: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreFloat:
			{
				uint32_t data = g_stackFrame->m_registers[operand2].m_U32;
				g_stackFrame->m_registers[operand1].m_U64 = data;
				g_stackFrame->m_registers[dstReg].m_I32 = data;
				printf("DC::kStoreFloat: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStorePointer:
			{
				uint64_t data = g_stackFrame->m_registers[operand2].m_U64;
				g_stackFrame->m_registers[operand1].m_U64 = data;
				g_stackFrame->m_registers[dstReg].m_U64 = data;
				printf("DC::kStorePointer: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kLookupInteger:
			case DC::kLookupFloat:
			case DC::kLookupPointer:
			{
				//printf("DC::kLookupInteger/kLookupFloat: R%d R%d -> 0x%016llX\n", opcode, dstReg, operand1, operand2, dstReg, operand1, pSymbolTbl[operand1].m_U32);
				printf("DC::kLookupInteger/kLookupFloat: R%d R%d -> hashid: 0x%016llX(%s)\n", dstReg, operand1, pSymbolTbl[operand1].m_U64, StringIdToStringInternal(pSymbolTbl[operand1].m_U64));
				//	g_stackFrame->m_registers[dstReg].m_U64 = 0;
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I64;
				idx = tmpIdx;
				break;
			}
			/*
			case DC::kLookupPointer:
			{
				printf("DC::kLookupPointer: R%d R%d -> 0x%08X -> %s\n", opcode, dstReg, operand1, operand2, dstReg, operand1, pSymbolTbl[operand1].m_U32, StringIdToStringInternal(pSymbolTbl[operand1].m_U32) );
			//	g_stackFrame->m_registers[dstReg].m_U64 = 0;
				g_stackFrame->m_registers[dstReg].m_U32 = pSymbolTbl[operand1].m_U32;
				idx = tmpIdx;
				break;
			}
			*/
			case DC::kMoveInteger:
			case DC::kMovePointer:
			case DC::kMove:
			{
				//g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64;
				g_stackFrame->m_registers[dstReg] = g_stackFrame->m_registers[operand1];
				//mov     rax, [rbp+r15*8+s+208h]
				//mov     [rbp+rsi*8+s+208h], rax
				//g_unkArea->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64;
				//g_unkArea->m_registers[dstReg] = g_stackFrame->m_registers[operand1];
				printf("DC::kMoveInteger/kMovePointer/kMove: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kMoveFloat:
			{
				g_stackFrame->m_registers[dstReg].m_I32 = g_stackFrame->m_registers[operand1].m_I32;
				printf("DC::kMoveInteger/kMovePointer/kMove: S%d, S%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kCastInteger:
			{
				g_stackFrame->m_registers[dstReg].m_I32 = (int32_t)g_stackFrame->m_registers[dstReg].m_F32;
				printf("DC::kCastInteger: R%d, S%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kCastFloat:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = (float)g_stackFrame->m_registers[dstReg].m_F64;
				printf("DC::kCastFloat: S%d, S%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kCall:
			{
				ScriptLambda* pValue = reinterpret_cast<ScriptLambda*>(g_stackFrame->m_registers[operand1].m_U64);
				if (pValue)
				{
					//g_stackFrame->m_registers[dstReg] = ExecuteScriptCode(pValue);
					printf("DC::kCall: '%s(", StringIdToStringInternal(g_stackFrame->m_registers[operand1].m_U64));
					int8_t argc = pOpcode[sizeof(ScriptInstruction) * idx + 0x3];
					PrintCallArgs(g_stackFrame, argc);
					puts(")");
				}
				else
				{
					puts("DC::kCall: Unable to call defun");
				}
		//		DebugDumpRegisters(g_stackFrame);
		//		DebugDumpRegisters(g_unkArea);
				idx = tmpIdx;
				numberOfMovBeforeCall = 0;
				break;
			}

			case DC::kCallFf:
			{
				uint64_t data = g_stackFrame->m_registers[operand1].m_U64;
				printf("DC::kCallFf: '%s(", StringIdToStringInternal(g_stackFrame->m_registers[operand1].m_U64));
				
				int8_t argc = pOpcode[sizeof(ScriptInstruction) * idx + 0x3];
				PrintCallArgs(g_stackFrame, argc);
				puts(")");
			//	DebugDumpRegisters(g_stackFrame);
			//	DebugDumpRegisters(g_unkArea);
				numberOfMovBeforeCall = 0;
				idx = tmpIdx;
				break;
			}

			case DC::kIeq:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 == g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kIeq: R%d == R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIgt:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 > g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kIgt: R%d > R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIgte:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 >= g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kIgte: R%d >= R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIlt:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 < g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kIlt: R%d < R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIlte:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 <= g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kIlte: R%d <= R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kFeq:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 == g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kFeq: S%d == S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFgt:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 > g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kFgt: S%d > S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFgte:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 >= g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kFgte: S%d >= S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFlt:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 < g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kFlt: S%d < S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFlte:
			{
				int32_t res = g_stackFrame->m_registers[operand1].m_F32 <= g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kFlte: S%d <= S%d\n", operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I32 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kImod:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 % g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kImod: R%d mod R%d\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kFmod:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = fmodf(g_stackFrame->m_registers[operand1].m_F32, g_stackFrame->m_registers[operand2].m_F32);
				printf("DC::kFmod: fmodf(S%d S%d)\n", operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIabs:
			{
				int64_t res = -g_stackFrame->m_registers[operand1].m_I64;
				if (res < 1)
				{
					res = g_stackFrame->m_registers[operand1].m_I64;
				}
				printf("DC::kIabs: abs(R%d)\n", operand1);
				g_stackFrame->m_registers[dstReg].m_I64 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kFabs:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = fabsf(g_stackFrame->m_registers[operand1].m_F32);
				printf("DC::kFabs: S%d fabsf(S%d)\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kBranch:
			{
				//idx = dstReg | (uint32_t)(operand2 << 0x8);
				//printf("DC::kBranch: B loc_%lld\n", dstReg | (uint32_t)(operand2 << 0x8));
				printf("DC::kBranch: B loc_%d\n", dstReg | (uint32_t)(operand2 << 0x8));
				idx = tmpIdx;
				break;
			}

			case DC::kBranchIf:
			{
				if (g_stackFrame->m_registers[operand1].m_U64)
				{
					//tmpIdx = dstReg | (uint32_t)operand2 << 0x8;
					//printf("Taken DC::kBranchIf: B loc_%lld\n", dstReg | (uint32_t)operand2 << 0x8);
					printf("DC::kBranchIf: B loc_%d\n", dstReg | (uint32_t)operand2 << 0x8);
				}
				else
				{
					//printf("Not Taken DC::kBranchIf: B loc_%lld\n", tmpIdx);
					printf("DC::kBranchIf: B loc_%lld\n", tmpIdx);
				}
				idx = tmpIdx;
				break;
			}

			case DC::kBranchIfNot:
			{
				if (g_stackFrame->m_registers[operand1].m_U64)
				{
					//tmpIdx = dstReg | (uint32_t)operand2 << 0x8;
					//printf("Taken DC::kBranchIfNot: B loc_%lld\n", tmpIdx);
					printf("DC::kBranchIfNot: B loc_%d\n", dstReg | (uint32_t)operand2 << 0x8);
				}
				else
				{
					printf("DC::kBranchIfNot: B loc_%lld\n", tmpIdx);
				}
				idx = tmpIdx;
				break;
			}

			case DC::kNot:
			{
				//	I8 res = -(g_stackFrame->m_registers[operand1].m_I64 == 0);
				g_stackFrame->m_registers[dstReg].m_I64 = -(g_stackFrame->m_registers[operand1].m_I64 == 0);;
				printf("DC::kNot: R%d R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseAnd:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64 & g_stackFrame->m_registers[operand2].m_U64;
				printf("DC::kBitwiseAnd: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseNot:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = ~g_stackFrame->m_registers[operand1].m_U64;
				printf("DC::kBitwiseAnd: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseIor:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64 | g_stackFrame->m_registers[operand2].m_U64;
				printf("DC::kBitwiseIor: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseXor:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = g_stackFrame->m_registers[operand1].m_U64 ^ g_stackFrame->m_registers[operand2].m_U64;
				printf("DC::kBitwiseXor: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kBitwiseNor:
			{
				g_stackFrame->m_registers[dstReg].m_U64 = ~(g_stackFrame->m_registers[operand1].m_U64 | g_stackFrame->m_registers[operand2].m_U64);
				printf("DC::kBitwiseNor: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kAnd: //this is probably the implementation of '&&'
			{
				uint8_t res = 0;
				if (g_stackFrame->m_registers[operand1].m_U64)
				{
					if (g_stackFrame->m_registers[operand2].m_U64)
					{
						res = 1;
					}
				}
				printf("DC::kAnd: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				g_stackFrame->m_registers[dstReg].m_I64 = res;
				idx = tmpIdx;
				break;
			}

			case DC::kOr:  //implementation of '||'
			{
				uint8_t res = 1;
				if (!g_stackFrame->m_registers[operand1].m_U64)
				{
					res = g_stackFrame->m_registers[operand2].m_U64 != 0;
				}
				g_stackFrame->m_registers[dstReg].m_I64 = res;
				printf("DC::kOr: R%d, R%d, R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kInegate:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = -g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kInegate: R%d, R%d\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kFnegate:
			{
				g_stackFrame->m_registers[dstReg].m_U32 = g_stackFrame->m_registers[operand1].m_U32 ^ 0x80000000;
				printf("DC::kFnegate: R%d, R%d ^ 0x80000000\n", dstReg, operand1);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadParamCount:
			{
				//TODO: this is partially correct
				//the parameters gets passed in input to the function
				g_stackFrame->m_registers[dstReg].m_U32 = 65LL;
				printf("DC::kLoadParamCount: R%d, #%X\n", dstReg, 65);
				idx = tmpIdx;
				break;
			}

			case DC::kIaddImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = operand2 + g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kIaddImm: R%d, R%d, %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIsubtractImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 - operand2;
				printf("DC::kIsubtractImm: R%d, R%d - %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kImultiplyImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = operand2 * g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kImultiplyImm: R%d, R%d * %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kIdivideImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 / operand2;
				printf("DC::kIdivideImm: R%d, R%d / %d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticI32Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I32;
				printf("DC::kLoadStaticI32Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_I32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticFloatImm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U32;
				printf("DC::kLoadStaticFloatImm: S%d, %f\n", dstReg, pSymbolTbl[operand1].m_F32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticPointerImm:
			case DC::kLoadStaticI64Imm:
			case DC::kLoadStaticU64Imm:
			{
				g_stackFrame->m_registers[dstReg] = pSymbolTbl[operand1];
				printf("DC::kLoadStaticPointerImm/kLoadStaticI64Imm/kLoadStaticU64Imm: R%d, %.16llX(%s)\n", dstReg, pSymbolTbl[operand1].m_U64, StringIdToStringInternal(pSymbolTbl[operand1].m_U64) );
				idx = tmpIdx;
				break;
			}

			case DC::kIash:
			{
				int64_t hash = g_stackFrame->m_registers[operand1].m_I64 >> -(char)g_stackFrame->m_registers[operand2].m_I8;
				if (g_stackFrame->m_registers[operand2].m_I64 >= 0)
				{
					hash = g_stackFrame->m_registers[operand1].m_I64 << g_stackFrame->m_registers[operand2].m_I8;
				}
				g_stackFrame->m_registers[dstReg].m_I64 = hash;
				printf("DC::kIash: R%d, %.16llX\n", dstReg, hash);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticU32Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U32;
				printf("DC::kLoadStaticU32Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_U32);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticI8Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I8;
				printf("DC::kLoadStaticI8Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_I8);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticU8Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U8;
				printf("DC::kLoadStaticU8Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_U8);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticI16Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_I16;
				printf("DC::kLoadStaticI16Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_I16);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadStaticU16Imm:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = pSymbolTbl[operand1].m_U16;
				printf("DC::kLoadStaticU16Imm: R%d, %08X\n", dstReg, pSymbolTbl[operand1].m_U16);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadI8:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kLoadI8: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU8:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kLoadU8: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadI16:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kLoadI16: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadU16:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kLoadU16: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kLoadI32:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64;
				printf("DC::kLoadI32: R%d, %016llX\n", dstReg, pSymbolTbl[operand1].m_I64);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI8:
			{
				int8_t val = g_stackFrame->m_registers[operand2].m_I8;
				g_stackFrame->m_registers[operand1].m_I8 = val;
				g_stackFrame->m_registers[dstReg].m_I8 = val;
				printf("DC::kStoreI8: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreU8:
			{
				uint8_t val = g_stackFrame->m_registers[operand2].m_U8;
				g_stackFrame->m_registers[operand1].m_U8 = val;
				g_stackFrame->m_registers[dstReg].m_U8 = val;
				printf("DC::kStoreU8: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI16:
			{
				int16_t val = g_stackFrame->m_registers[operand2].m_I16;
				g_stackFrame->m_registers[operand1].m_I16 = val;
				g_stackFrame->m_registers[dstReg].m_I16 = val;
				printf("DC::kStoreI16: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreU16:
			{
				uint16_t val = g_stackFrame->m_registers[operand2].m_U16;
				g_stackFrame->m_registers[operand1].m_U16 = val;
				g_stackFrame->m_registers[dstReg].m_U16 = val;
				printf("DC::kStoreU16: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI32:
			{
				int32_t val = g_stackFrame->m_registers[operand2].m_I32;
				g_stackFrame->m_registers[operand1].m_I32 = val;
				g_stackFrame->m_registers[dstReg].m_I32 = val;
				printf("DC::kStoreI32: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreU32:
			{
				uint32_t val = g_stackFrame->m_registers[operand2].m_U32;
				g_stackFrame->m_registers[operand1].m_U32 = val;
				g_stackFrame->m_registers[dstReg].m_U32 = val;
				printf("DC::kStoreU32: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreI64:
			{
				int64_t val = g_stackFrame->m_registers[operand2].m_I64;
				g_stackFrame->m_registers[operand1].m_I64 = val;
				g_stackFrame->m_registers[dstReg].m_I64 = val;
				printf("DC::kStoreI64: R%d!, {R%d, R%d}\n", operand2, operand1, dstReg);
				idx = tmpIdx;
				break;
			}

			case DC::kNotIeq:
			{
				g_stackFrame->m_registers[dstReg].m_I64 = g_stackFrame->m_registers[operand1].m_I64 != g_stackFrame->m_registers[operand2].m_I64;
				printf("DC::kStoreI64: R%d, R%d != R%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kNotFeq:
			{
				g_stackFrame->m_registers[dstReg].m_F32 = g_stackFrame->m_registers[operand1].m_F32 != g_stackFrame->m_registers[operand2].m_F32;
				printf("DC::kStoreI64: S%d, S%d != S%d\n", dstReg, operand1, operand2);
				idx = tmpIdx;
				break;
			}

			case DC::kStoreArray:
			{
				//TODO: implement me
				printf("DC::kStoreArray: Tell icemesh\n");
			//	__debugbreak();
				idx = tmpIdx;
				break;
			}

			case DC::kAssertPointer:
			{
				printf("DC::kAssertPointer:\n");
			//	__debugbreak();
				idx = tmpIdx;
				break;
			}

			default:
			{
				printf("RunScript: Unknown opcode '0x%X' encountered!\n", opcode);
				DC::Variant ret;
				ret.m_I8 = 0;
				return ret;
			}
		} //end switch
	} // end while
}

void DebugDumpRegisters(DC::StackFrame* pFrame)
{
	if (pFrame)
	{
		printf("R%d : [0x%.16llX]\n", 49, pFrame->m_registers[49].m_U64);
		printf("R%d : [0x%.16llX]\n", 50, pFrame->m_registers[50].m_U64);
		printf("R%d : [0x%.16llX]\n", 51, pFrame->m_registers[51].m_U64);
		printf("R%d : [0x%.16llX]\n", 52, pFrame->m_registers[52].m_U64);
		/*
		for (int i = 0; i < kMaxRegisters; i++)
		{
			if (pFrame->m_registers[i].m_U64 != 0)
			{
				printf("R%d : [0x%.16llX]\n", i, pFrame->m_registers[i].m_U64);
			}
		}
		*/
	}
}

void PrintCallArgs(DC::StackFrame* pFrame, int count)
{
	if (count > 0)
	{
		/*
		int regIdx = 49;
		printf("'%s ", StringIdToStringInternal(pFrame->m_registers[regIdx].m_U64));
		regIdx = 51;
		for (int i = 1; i < count; i++)
		{
			printf("'%s ", StringIdToStringInternal(pFrame->m_registers[regIdx].m_U64));
			regIdx++;
		}
		*/
	//	printf("'%s(", StringIdToStringInternal(pFrame->m_registers[0].m_U64));
		
		int regIdx = 49;
		for (int i = 0; i < count; i++)
		{
			printf("'%s ", StringIdToStringInternal(pFrame->m_registers[regIdx].m_U64));
			regIdx++;
		}
	}
}