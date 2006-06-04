/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#include "obj/NiSkinInstance.h"
#include "gen/Bones.h"
#include "obj/NiNode.h"
#include "obj/NiSkinData.h"
#include "obj/NiSkinPartition.h"
#include "obj/NiNode.h"

//Definition of TYPE constant
const Type NiSkinInstance::TYPE("NiSkinInstance", &NI_SKIN_INSTANCE_PARENT::TYPE );

NiSkinInstance::NiSkinInstance() NI_SKIN_INSTANCE_CONSTRUCT {}

NiSkinInstance::~NiSkinInstance() {}

void NiSkinInstance::Read( istream& in, list<uint> & link_stack, unsigned int version ) {
	NI_SKIN_INSTANCE_READ
}

void NiSkinInstance::Write( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version ) const {
	NI_SKIN_INSTANCE_WRITE
}

string NiSkinInstance::asString( bool verbose ) const {
	NI_SKIN_INSTANCE_STRING
}

void NiSkinInstance::FixLinks( const vector<NiObjectRef> & objects, list<uint> & link_stack, unsigned int version ) {
	NI_SKIN_INSTANCE_FIXLINKS
}

list<NiObjectRef> NiSkinInstance::GetRefs() const {
	NI_SKIN_INSTANCE_GETREFS
}

const Type & NiSkinInstance::GetType() const {
	return TYPE;
};

NiNode * NiSkinInstance::SkeletonRoot() const { return NULL; }
