/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NIPSYSMESHEMITTER_H_
#define _NIPSYSMESHEMITTER_H_

#include "NiPSysEmitter.h"

// Include structures
#include "../Ref.h"
namespace Niflib {

// Forward define of referenced blocks
class NiTriBasedGeom;

//#include "../gen/obj_defines.h"

class NiPSysMeshEmitter;
typedef Ref<NiPSysMeshEmitter> NiPSysMeshEmitterRef;

/*!
 * NiPSysMeshEmitter - Unknown.
 */

class NiPSysMeshEmitter : public NI_P_SYS_MESH_EMITTER_PARENT {
public:
	NIFLIB_API NiPSysMeshEmitter();
	NIFLIB_API ~NiPSysMeshEmitter();
	//Run-Time Type Information
	NIFLIB_API static const Type & TypeConst() { return TYPE; }
private:
	static const Type TYPE;
public:
	NIFLIB_API virtual const Type & GetType() const;
	NIFLIB_HIDDEN virtual void Read( istream& in, list<unsigned int> & link_stack, unsigned int version, unsigned int user_version );
	NIFLIB_HIDDEN virtual void Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, unsigned int version, unsigned int user_version ) const;
	NIFLIB_API virtual string asString( bool verbose = false ) const;
	NIFLIB_HIDDEN virtual void FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, unsigned int version, unsigned int user_version );
	NIFLIB_HIDDEN virtual list<NiObjectRef> GetRefs() const;

protected:
	NI_P_SYS_MESH_EMITTER_MEMBERS
private:
	void InternalRead( istream& in, list<unsigned int> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, const map<NiObjectRef,unsigned int> & link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
