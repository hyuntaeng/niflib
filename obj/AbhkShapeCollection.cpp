/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#include "AbhkShapeCollection.h"

//Definition of TYPE constant
const Type AbhkShapeCollection::TYPE("AbhkShapeCollection", &ABHK_SHAPE_COLLECTION_PARENT::TYPE );

AbhkShapeCollection::AbhkShapeCollection() ABHK_SHAPE_COLLECTION_CONSTRUCT {}

AbhkShapeCollection::~AbhkShapeCollection() {}

void AbhkShapeCollection::Read( istream& in, list<uint> link_stack, unsigned int version ) {
	ABHK_SHAPE_COLLECTION_READ
}

void AbhkShapeCollection::Write( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version ) const {
	ABHK_SHAPE_COLLECTION_WRITE
}

string AbhkShapeCollection::asString( bool verbose ) const {
	ABHK_SHAPE_COLLECTION_STRING
}

void AbhkShapeCollection::FixLinks( const vector<NiObjectRef> & objects, list<uint> link_stack, unsigned int version ) {
	ABHK_SHAPE_COLLECTION_FIXLINKS
}
