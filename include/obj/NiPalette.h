/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NIPALETTE_H_
#define _NIPALETTE_H_

#include "NiObject.h"
namespace Niflib {

//#include "../gen/obj_defines.h"

class NiPalette;
typedef Ref<NiPalette> NiPaletteRef;

/*!
 * NiPalette - A color palette.
 */

class NiPalette : public NI_PALETTE_PARENT {
public:
	NIFLIB_API NiPalette();
	NIFLIB_API ~NiPalette();
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

	/*! Retrieves the palette data from this palette block.
	 * \return A vector containing the the colors stored in the palette.
	 * \sa NiPalette::SetPalette
	 */
	NIFLIB_API vector<Color4> GetPalette() const;

	/*! Sets the palette data for this palette block.
	 * \param new_pal A vector containing the the new colors to be stored in the palette.
	 * \sa NiPalette::GetPalette
	 */
	NIFLIB_API void SetPalette( const vector<Color4> & new_pal );
protected:
	NI_PALETTE_MEMBERS
private:
	void InternalRead( istream& in, list<unsigned int> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, const map<NiObjectRef,unsigned int> & link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
