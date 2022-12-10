/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/
//this is a common header that every .bin file has.
#include "../stringid/stringid.h"

struct Entry;

struct DC_Header
{
	uint32_t				m_magic;				///< <c>0x00</c>: magic 0x44433030 -> DC00
	uint32_t				m_versionNumber;		///< <c>0x04</c>: always 0x1
	uint32_t				m_textSize;				///< <c>0x08</c>: size from 0x0C to (0xC+m_TextSize)
	uint32_t				unk;					///< <c>0x0C</c>: always 0 
	uint32_t				unk1;					///< <c>0x10</c>: always 1
	int32_t					m_numEntries;			///< <c>0x14</c>: num of entries
	Entry*					m_pStartOfData;			///< <c>0x18</c>: ptr to the start of data/state script(s) 
};

struct Entry
{
	StringId64				m_scriptId;				///< <c>0x00</c>: StringId64 of the script name
	StringId64				m_typeId;				///< <c>0x08</c>: StringId64 of the script type eg SID("state-script")
	void*					m_entryPtr;				///< <c>0x10</c>: ptr to the scriptType cast this to the IdGroup || StateScript etc..
};