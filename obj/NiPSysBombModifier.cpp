/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#include "obj/NiPSysBombModifier.h"
#include "obj/NiNode.h"

//Definition of TYPE constant
const Type NiPSysBombModifier::TYPE("NiPSysBombModifier", &NI_P_SYS_BOMB_MODIFIER_PARENT::TYPE );

NiPSysBombModifier::NiPSysBombModifier() NI_P_SYS_BOMB_MODIFIER_CONSTRUCT {}

NiPSysBombModifier::~NiPSysBombModifier() {}

void NiPSysBombModifier::Read( istream& in, list<uint> & link_stack, unsigned int version ) {
	NI_P_SYS_BOMB_MODIFIER_READ
}

void NiPSysBombModifier::Write( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version ) const {
	NI_P_SYS_BOMB_MODIFIER_WRITE
}

string NiPSysBombModifier::asString( bool verbose ) const {
	NI_P_SYS_BOMB_MODIFIER_STRING
}

void NiPSysBombModifier::FixLinks( const vector<NiObjectRef> & objects, list<uint> & link_stack, unsigned int version ) {
	NI_P_SYS_BOMB_MODIFIER_FIXLINKS
}

list<NiObjectRef> NiPSysBombModifier::GetRefs() const {
	NI_P_SYS_BOMB_MODIFIER_GETREFS
}

const Type & NiPSysBombModifier::GetType() const {
	return TYPE;
};

