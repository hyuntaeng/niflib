/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _BHKTRANSFORMSHAPE_H_
#define _BHKTRANSFORMSHAPE_H_

#include "bhkEntity.h"

#include "gen/obj_defines.h"

/*
 * bhkTransformShape
 */

class bhkTransformShape;
typedef Ref<bhkTransformShape> bhkTransformShapeRef;

class bhkTransformShape : public BHK_TRANSFORM_SHAPE_PARENT {
public:
	bhkTransformShape();
	~bhkTransformShape();
	//Run-Time Type Information
	static const Type TYPE;
	virtual void Read( istream& in, list<uint> & link_stack, unsigned int version );
	virtual void Write( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version ) const;
	virtual string asString( bool verbose = false ) const;
	virtual void FixLinks( const vector<NiObjectRef> & objects, list<uint> & link_stack, unsigned int version );
	virtual list<NiObjectRef> GetRefs() const;
	virtual const Type & GetType() const;
protected:
	BHK_TRANSFORM_SHAPE_MEMBERS
};

#endif
