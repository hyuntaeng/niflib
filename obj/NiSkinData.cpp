/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#include "obj/NiSkinData.h"
#include "gen/SkinData.h"
#include "gen/SkinWeight.h"
#include "obj/NiSkinPartition.h"

//Definition of TYPE constant
const Type NiSkinData::TYPE("NiSkinData", &NI_SKIN_DATA_PARENT::TYPE );

NiSkinData::NiSkinData() NI_SKIN_DATA_CONSTRUCT {}

NiSkinData::~NiSkinData() {}

void NiSkinData::Read( istream& in, list<uint> & link_stack, unsigned int version ) {
	NI_SKIN_DATA_READ
}

void NiSkinData::Write( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version ) const {
	NI_SKIN_DATA_WRITE
}

string NiSkinData::asString( bool verbose ) const {
	NI_SKIN_DATA_STRING
}

void NiSkinData::FixLinks( const vector<NiObjectRef> & objects, list<uint> & link_stack, unsigned int version ) {
	NI_SKIN_DATA_FIXLINKS
}

list<NiObjectRef> NiSkinData::GetRefs() const {
	NI_SKIN_DATA_GETREFS
}

const Type & NiSkinData::GetType() const {
	return TYPE;
};

