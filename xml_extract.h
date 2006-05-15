/* --------------------------------------------------------------------------
 * xml_extract.h: C++ header file for raw reading, writing, and printing
 *                NetImmerse and Gamebryo files (.nif & .kf & .kfa)
 * --------------------------------------------------------------------------
 * ***** BEGIN LICENSE BLOCK *****
 *
 * Copyright (c) 2005, NIF File Format Library and Tools
 * All rights reserved.
 * 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *    * Neither the name of the NIF File Format Library and Tools
 *      project nor the names of its contributors may be used to endorse
 *      or promote products derived from this software without specific
 *      prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END LICENCE BLOCK *****
 * --------------------------------------------------------------------------
 */

#ifndef _XML_EXTRACT_H_
#define _XML_EXTRACT_H_

#include "NIF_IO.h"
#include "nif_objects.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

//  This is a list of bone influences.  It points to blocks higher in the
// hierarchy so ints are used to represent the indices.
struct Bones {
  // Block indicies of the bones.
  vector<NiNode* > bones;
};

//  An array of bytes.
struct ByteArray {
  // Unknown.
  uint unknownInt;
  // The bytes which make up the array
  vector<byte > data;
};

//  An integer value that may or may not be used.
struct CondInt {
  // Non-Zero if the following Integer appears.  Otherwise, the integer
  // does not appear.
  bool isUsed;
  // An unknown integer.
  uint unknownInt;
};

//  An array of (untyped) keys.
template <class T >
struct KeyArray {
  // The keys.
  vector<Key<T > > keys;
};

//  List of block indices.
template <class T >
struct LinkGroup {
  // The list of block indices.
  vector<Ref<T > > indices;
};

//  The NIF file footer.
struct Footer {
  // List of root blocks. If there is a camera, for 1st person view, then
  // this block is referred to as well in this list, even if it is not a
  // root block (usually we want the camera to be attached to the Bip Head
  // node).
  vector< Ref<NiAVObject > > roots;
};

//  The distance range where a specific level of detail applies.
struct LODRange {
  // Begining of range.
  float near;
  // End of Range.
  float far;
};

//  Group of vertex indices of vertices that match.
struct MatchGroup {
  // The vertex indices.
  vector<ushort > vertexIndices;
};

//  Description of a MipMap within a NiPixelData block.
struct MipMap {
  // Width of the mipmap image.
  uint width;
  // Height of the mipmap image.
  uint height;
  // Offset into the pixel data array where this mipmap starts.
  uint offset;
};

//  A link group conditioned on a boolean value.
struct ModifierGroup {
  // Determines whether or not the link group is present.
  bool hasModifiers;
  // The list of particle modifiers.
  //vector< Ref<AParticleModifier > > modifiers;
};

//  Linear key type (!!! for NifSkope optimizer only, use key, keyrot, or
// keyvec for regular use).
template <class T >
struct ns_keylin {
  // Key time.
  float time;
  // The key value.
  T value;
};

//  Array of keys, not interpolated (!!! for NifSkope only, use keyarray
// for regular use).
template <class T >
struct ns_keyarray {
  // The keys.
  vector<ns_keylin<T > > keys;
};

//  Key with tangents (!!! for NifSkope only, use keyvec instead for
// regular purposes).
template <class T >
struct ns_keytan {
  // The key time.
  float time;
  // The key value.
  T value;
  // Forward tangent.
  T forward;
  // Backward tangent.
  T backward;
};

//  A quaternion as it appears in the havok blocks.
struct QuaternionXYZW {
  // The x-coordinate.
  float x;
  // The y-coordinate.
  float y;
  // The z-coordinate.
  float z;
  // The w-coordinate.
  float w;
};

//  Another string format, for short strings.  Specific to Bethesda-
// specific header tags.
struct ShortString {
  // The string itself, null terminated (the null terminator is taken into
  // account in the length byte).
  vector<byte > value;
};

//  Reference to shape and skin instance.
struct SkinShape {
  // The shape.
  //Ref<NiTriShape > shape;
  // Skinning instance for the shape?
  //Ref<NiSkinInstance > skinInstance;
};

//  Unknown.
struct SkinShapeGroup {
  // First link is a NiTriShape block. Second link is a NiSkinInstance
  // block.
  vector<SkinShape > linkPairs;
};

//  A weighted vertex.
struct SkinWeight {
  // The vertex index, in the mesh.
  ushort index;
  // The vertex weight - between 0.0 and 1.0
  float weight;
};

//  Used in NiDefaultAVObjectPalette.
struct AVObject {
  // Object name.
  string name;
  // Object reference.
  NiAVObject * object;
};

//  In a .kf file, this links to a controllable block, via its name (or
// for version 10.2.0.0 and up, a link and offset to a NiStringPalette
// that contains the name), and a sequence of interpolators that apply to
// this controllable block, via links.
struct ControllerLink {
  // Name of a controllable block in another NIF file.
  string name;
  // Link to an interpolator.
  //Ref<AInterpolator > interpolator;
  // Unknown link. Usually -1.
  Ref<NiObject > unknownLink1;
  // Unknown.
  Ref<NiObject > unknownLink2;
  // Unknown.
  ushort unknownShort0;
  // Idle animations tend to have low values for this, and blocks that have
  // high values tend to correspond with the important parts of the
  // animation. WARNING: BREAKS CIV4 NIF FILES! Only observed in Oblivion
  // NIF files so far.
  byte priority_;
  // Refers to the NiStringPalette which contains the name of the
  // controlled block.
  //Ref<NiStringPalette > stringPalette;
  // The name of the animated node.
  string nodeName;
  // Offset in the string palette where the name of the controlled node
  // (NiNode, NiTriShape, ...) starts.
  ushort nodeNameOffset;
  // Unknown, always 0.
  ushort unknownShort1;
  // Name of the property (NiMaterialProperty, ...), if this controller
  // controls a property.
  string propertyType;
  // Offset in the string palette where the property (NiMaterialProperty,
  // ...) starts, if this controller controls a property. Otherwise, -1.
  ushort propertyTypeOffset;
  // Unknown, usually 0, but sometimes also 0xFFFF.
  ushort unknownShort2;
  // Probably the block type name of the controller in the NIF file that is
  // child of the controlled block.
  string controllerType;
  // Apparently the offset in the string palette of some type of controller
  // related to Interpolator (for example, a 'NiTransformInterpolator' will
  // have here a 'NiTransformController', etc.). Sometimes the type of
  // controller that links to the interpolator. Probably it refers to the
  // controller in the NIF file that is child of the controlled block, via
  // its type name.
  ushort controllerTypeOffset;
  // Unknown, always 0.
  ushort unknownShort3;
  // Some variable string (such as 'SELF_ILLUM', '0-0-TT_TRANSLATE_U',
  // 'tongue_out', etc.).
  string variable1;
  // Offset in the string palette where some variable string starts (such
  // as 'SELF_ILLUM', '0-0-TT_TRANSLATE_U', 'tongue_out', etc.). Usually,
  // -1.
  ushort variableOffset1;
  // Unknown, usually 0, but sometimes 0xFFFF.
  ushort unknownShort4;
  // Another variable string, apparently used for particle system
  // controllers.
  string variable2;
  // Offset in the string palette where some variable string starts (so far
  // only 'EmitterActive' and 'BirthRate' have been observed in official
  // files, used for particle system controllers). Usually, -1.
  ushort variableOffset2;
  // Unknown, usually 0, but sometimes 0xFFFF.
  ushort unknownShort5;
};

//  The NIF file header.
struct Header {
  // 'NetImmerse File Format x.x.x.x' (versions <= 10.0.1.2) or 'Gamebryo
  // File Format x.x.x.x' (versions >= 10.1.0.0), with x.x.x.x the version
  // written out. Ends with a newline character (0x0A).
  HeaderString headerString;
  // The NIF version, in hexadecimal notation: 0x04000002, 0x0401000C,
  // 0x04020002, 0x04020100, 0x04020200, 0x0A000100, 0x0A010000,
  // 0x0A020000, 0x14000004, ...
  uint version;
  // Determines the endian-ness of the data.  1 = little endian (default) 0
  // = big endian
  byte endianType;
  // An extra version number, for companies that decide to modify the file
  // format.
  uint userVersion;
  // Unknown.
  uint unknownInt1;
  // Unknown.
  uint unknownInt3;
  // Could be the name of the creator of the NIF file?
  ShortString creator_;
  // Unknown. Can be something like 'TriStrip Process Script'.
  ShortString exportType_;
  // Unknown. Possibly the selected option of the export script. Can be
  // something like 'Default Export Script'.
  ShortString exportScript_;
  // List of all block types used in this NIF file.
  vector<string > blockTypes;
  // Maps file blocks on their corresponding type: first file block is of
  // type block_types[block_type_index[0]], the second of
  // block_types[block_type_index[1]], etc.
  vector<ushort > blockTypeIndex;
  // Unknown.
  uint unknownInt2;
  Header() : version(0x04000002), endianType(1) {};
};

//  Describes a shader.
struct Shader {
  // Do we have a shader?
  bool hasShader;
  // The shader name.
  string shaderName;
  // Unknown link, usually -1.
  Ref<NiObject > unknownLink;
};

//  A list of \\0 terminated strings.
struct StringPalette {
  // A bunch of 0x00 seperated strings.
  string palette;
  // Length of the palette string is repeated here.
  uint length;
};

//  A list of node targets.
struct TargetGroup {
  // The list of block indices.
  vector<NiAVObject*> indices;
};

//  Tension, bias, continuity.
struct TBC {
  // Tension.
  float t;
  // Bias.
  float b;
  // Continuity.
  float c;
};

//  A TBC key (!!! for NifSkope only, use keyvec for regular purposes).
template <class T >
struct ns_keytbc {
  // The key time.
  float time;
  // The key value.
  T value;
  // Tension, bias, continuity.
  TBC tbc;
};

//  Array of interpolable keys (!!! for NifSkope only, use keyvecarray
// for regular use).
template <class T >
struct ns_keyvecarray {
  // Number of keys.
  uint numKeys;
  // The key type (1, 2, or 3).
  uint keyType;
  // Linearly interpolated keys.
  vector<ns_keylin<T > > keys;
};

//  Rotation subkey (!!! Nifskope only).
struct ns_keyrotsub {
  // Time.
  float time;
  // The sub keys, one for every axis.
  vector<ns_keyvecarray<float > > subKeys;
};

//  Array of rotation keys (!!! for NifSkope only, use keyrotarray for
// regular use).
template <class T >
struct ns_keyrotarray {
  // Number of rotation keys.
  uint numKeys;
  // The key type (1, 2, 3, or 4).
  uint keyType;
  // Linear keys.
  vector<ns_keylin<T > > keys;
  // Special rotation keys (3 float arrays, one for each axis).
  vector<ns_keyrotsub > keysSub;
};

//  Array of interpolable keys (!!! for NifSkope only, use keyvecarraytyp
// for regular use)
template <class T >
struct ns_keyvecarraytyp {
  // The key type (1, 2, 3)
  uint keyType;
  // Linearly interpolated keys.
  vector<ns_keylin<T > > keys;
};

//  Texture description.
struct TexDesc {
  // NiSourceTexture block index.
  //Ref<NiSourceTexture > source;
  // 0=clamp S clamp T, 1=clamp S wrap T, 2=wrap S clamp T, 3=wrap S wrap T
  TexClampMode clampMode;
  // 0=nearest, 1=bilinear, 2=trilinear, 3=..., 4=..., 5=...
  TexFilterMode filterMode;
  // Texture set? Usually 0.
  uint textureSet;
  // 0?
  ushort ps2L;
  // 0xFFB5?
  ushort ps2K;
  // Unknown, 0 or 0x0101?
  ushort unknown1;
  // Determines whether or not the texture's coordinates are transformed.
  bool hasTextureTransform;
  // The amount to translate the texture coordinates in each direction?
  TexCoord translation;
  // The number of times the texture is tiled in each direction?
  TexCoord tiling;
  // 2D Rotation of texture image around third W axis after U and V.
  float wRotation;
  // The texture transform type?  Doesn't seem to do anything.
  uint transformType_;
  // The offset from the origin?
  TexCoord centerOffset;
  TexDesc() : clampMode(WRAP_S_WRAP_T), filterMode(FILTER_TRILERP), textureSet(0), ps2L(0), ps2K(0xFFB5), hasTextureTransform(false), wRotation(0.0f), transformType_(0) {};
};

//  A texture that happens to be a bump map.  Contains extra data.
struct BumpMap {
  // Determines whether this bumpmap contains any information. If Non-Zero
  // the following data is present, otherwise it is not.
  bool isUsed;
  // The bump map texture description.
  TexDesc texture;
  // ?
  float bumpMapLumaScale;
  // ?
  float bumpMapLumaOffset;
  // ?
  Matrix22 matrix;
};

//  A texture source.
struct TexSource {
  // Is the texture external?
  byte useExternal;
  // The external texture file name.  Note: all original morrowind nifs use
  // name.ext only for addressing the textures, but most mods use something
  // like textures/[subdir/]name.ext. This is due to a feature in Morrowind
  // resource manager: it loads name.ext, textures/name.ext and
  // textures/subdir/name.ext but NOT subdir/name.ext.
  string fileName;
  // Unknown.
  Ref<NiObject > unknownLink;
  // Unknown.
  byte unknownByte;
  // The original source filename of the image embedded by the referred
  // NiPixelData block.
  string originalFileName_;
  // Pixel data block index.
  //Ref<NiPixelData > pixelData;
};

//  A texture that is not a bumpmap.
struct Texture {
  // Determines whether this texture contains any information. If Non-Zero
  // the following data is present, otherwise it is not.
  bool isUsed;
  // The texture description.
  TexDesc textureData;
};

//  An extended version of texture.
struct Texture2 {
  // Is it used?
  bool isUsed;
  // The texture data.
  TexDesc textureData;
  // Unknown.
  uint unknownInt;
};

//  Group of extra textures.
struct ExtraTextureGroup {
  // The textures.
  vector<Texture2 > textures;
};

//  Skinning data for a submesh, optimized for hardware skinning. Part of
// NiSkinPartition.
struct SkinPartition {
  // Number of strips in this submesh (zero if not stripped).
  ushort numStrips;
  // List of bones.
  vector<ushort > bones;
  // Do we have a vertex map?
  bool hasVertexMap;
  // Maps the weight/influence lists in this submesh to the vertices in the
  // shape being skinned.
  vector<ushort > vertexMap;
  // Do we have vertex weights?
  bool hasVertexWeights;
  // The vertex weights.
  vector<vector<float > > vertexWeights;
  // Do we have strip data?
  bool hasStrips;
  // The strips.
  vector<vector<ushort > > strips;
  // The triangles.
  vector<Triangle > triangles;
  // Do we have bone indices?
  bool hasBoneIndices;
  // Bone indices, they index into 'Bones'.
  vector<vector<byte > > boneIndices;
};

//  Unknown.
struct unk292bytes {
  // Unknown.
  vector<vector<byte > > unknown292Bytes;
};

//  Bounding box.
struct BoundingBox {
  // This value determines whether this bounding box contains information.
  // If it is non-zero, the following information is present. At version
  // 4.0.0.1 and above, it becomes a byte instead of an int.
  bool isUsed;
  // Usually 1.
  uint unknownInt;
  // Translation vector.
  Vector3 translation;
  // Rotation matrix.
  Matrix33 rotation;
  // Radius, per direction.
  Vector3 radius;
};

//  Describes a furniture position?
struct FurniturePosition {
  // Unknown. Position?
  Vector3 unknownVector;
  // Unknown.
  ushort unknownShort;
  // This might refer to a furnituremarkerxx.nif file.
  byte positionRef1_;
  // This might also refer to a furnituremarkerxx.nif file.
  byte positionRef2_;
};

//  A triangle with extra data used for physics.
struct hkTriangle {
  // The triangle.
  Triangle triangle;
  // Another short, doesn't look like a vertex index.
  ushort unknownShort;
  // This appears to be a normalized vector, so probably it is a normal or
  // a tangent vector or something like that.
  Vector3 normal;
};

//  Info about level of detail ranges.
struct LODInfo {
  // Type of LOD info (0=regular, 1=info is in a NiRangeLODData block).
  uint lodType;
  // ?
  Vector3 lodCenter;
  // The ranges of distance that each level of detail applies in.
  vector<LODRange > lodLevels;
  // Zero?
  ushort unknownShort;
  // Refers to NiRangeLODData block.
  //Ref<NiRangeLODData > rangeData;
};

//  particle array entry
struct Particle {
  // Particle velocity
  Vector3 velocity;
  // Unknown
  Vector3 unknownVector;
  // The particle's age.
  float lifetime;
  // Maximum age of the particle.
  float lifespan;
  // Timestamp of the last update.
  float timestamp;
  // Unknown short (=0)
  ushort unknownShort;
  // Particle/vertex index matches array index
  ushort vertexId;
};

//  Data for several particles.
struct ParticleGroup {
  // Number of valid entries in the following array. (number of active
  // particles at the time the system was saved)
  ushort numValid;
  // Individual particle modifiers?
  vector<Particle > particles;
};

//  Skinning data component.
struct SkinData {
  // Rotation offset of the skin from this bone in bind position.
  Matrix33 rotation;
  // Translation offset of the skin from this bone in bind position.
  Vector3 translation;
  // Scale offset of the skin from this bone in bind position. (Assumption
  // - this is always 1.0 so far)
  float scale;
  // This has been verified not to be a normalized quaternion.  They may or
  // may not be related to each other so their specification as an array of
  // 4 floats may be misleading.
  vector<float > unknown4Floats;
  // The vertex weights.
  vector<SkinWeight > vertexWeights;
};

//  An array of keys. This one always has a Key Type.
template <class T >
struct TypedVectorKeyArray {
  // The key type.
  KeyType keyType;
  // The keys.
  vector<Key<T > > keys;
};

//  Geometry morphing data component.
struct Morph {
  // Name of the frame.
  string frameName;
  // The morphing keyframes.
  TypedVectorKeyArray<float > frames;
  // Unknown.
  uint unknownInt;
  // Morph vectors.
  vector<Vector3 > vectors;
};

//  Array of vector keys (anything that can be interpolated, except
// rotations).
template <class T >
struct VectorKeyArray {
  // Number of keys in the array.
  uint numKeys;
  // The key type.
  KeyType keyType;
  // The keys.
  vector<Key<T > > keys;
};

//  Rotation key array.
template <class T >
struct RotationKeyArray {
  // Number of keys.
  uint numKeys;
  // Key type (1, 2, 3, or 4).
  KeyType keyType;
  // The rotation keys.
  vector<Key<T > > keys;
};

#define NI_OBJECT_MEMBERS \

#define NI_OBJECT_PARENTS ABlock \

#define NI_OBJECT_CONSTRUCT \

#define NI_OBJECT_READ \

#define NI_OBJECT_WRITE \

#define NI_OBJECT_STRING \
stringstream out; \
return out.str(); \

#define NI_OBJECT_FIXLINKS \

#define A_INTERPOLATOR_MEMBERS \

#define A_INTERPOLATOR_PARENTS NiObject \

#define A_INTERPOLATOR_CONSTRUCT \

#define A_INTERPOLATOR_READ \
NiObject::Read( in, link_stack, version ); \

#define A_INTERPOLATOR_WRITE \
NiObject::Write( out, link_map, version ); \

#define A_INTERPOLATOR_STRING \
stringstream out; \
out << NiObject::asString(); \
return out.str(); \

#define A_INTERPOLATOR_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \

#define A_BLEND_INTERPOLATOR_MEMBERS \
ushort unknownShort; \
uint unknownInt; \

#define A_BLEND_INTERPOLATOR_PARENTS AInterpolator \

#define A_BLEND_INTERPOLATOR_CONSTRUCT \

#define A_BLEND_INTERPOLATOR_READ \
AInterpolator::Read( in, link_stack, version ); \
NifStream( unknownShort, in, version ); \
NifStream( unknownInt, in, version ); \

#define A_BLEND_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( unknownShort, out, version ); \
NifStream( unknownInt, out, version ); \

#define A_BLEND_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Unknown Int:  " << unknownInt << endl; \
return out.str(); \

#define A_BLEND_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \

#define A_B_SPLINE_COMP_INTERPOLATOR_MEMBERS \
float startTime; \
float stopTime; \

#define A_B_SPLINE_COMP_INTERPOLATOR_PARENTS AInterpolator \

#define A_B_SPLINE_COMP_INTERPOLATOR_CONSTRUCT \

#define A_B_SPLINE_COMP_INTERPOLATOR_READ \
AInterpolator::Read( in, link_stack, version ); \
NifStream( startTime, in, version ); \
NifStream( stopTime, in, version ); \

#define A_B_SPLINE_COMP_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( startTime, out, version ); \
NifStream( stopTime, out, version ); \

#define A_B_SPLINE_COMP_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Start Time:  " << startTime << endl; \
out << "Stop Time:  " << stopTime << endl; \
return out.str(); \

#define A_B_SPLINE_COMP_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \

#define A_KEYED_DATA_MEMBERS \

#define A_KEYED_DATA_PARENTS NiObject \

#define A_KEYED_DATA_CONSTRUCT \

#define A_KEYED_DATA_READ \
NiObject::Read( in, link_stack, version ); \

#define A_KEYED_DATA_WRITE \
NiObject::Write( out, link_map, version ); \

#define A_KEYED_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
return out.str(); \

#define A_KEYED_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \

#define A_KEYFRAME_DATA_MEMBERS \
RotationKeyArray<Quaternion > rotations; \
VectorKeyArray<Vector3 > translations; \
VectorKeyArray<float > scales; \

#define A_KEYFRAME_DATA_PARENTS NiObject \

#define A_KEYFRAME_DATA_CONSTRUCT \

#define A_KEYFRAME_DATA_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( rotations.numKeys, in, version ); \
if ( rotations.numKeys != 0 ) { \
  NifStream( rotations.keyType, in, version ); \
}; \
rotations.keys.resize(rotations.numKeys); \
for (uint i0 = 0; i0 < rotations.numKeys; i0++) { \
  NifStream( rotations.keys[i0], in, version ); \
}; \
NifStream( translations.numKeys, in, version ); \
if ( translations.numKeys != 0 ) { \
  NifStream( translations.keyType, in, version ); \
}; \
translations.keys.resize(translations.numKeys); \
for (uint i0 = 0; i0 < translations.numKeys; i0++) { \
  NifStream( translations.keys[i0], in, version ); \
}; \
NifStream( scales.numKeys, in, version ); \
if ( scales.numKeys != 0 ) { \
  NifStream( scales.keyType, in, version ); \
}; \
scales.keys.resize(scales.numKeys); \
for (uint i0 = 0; i0 < scales.numKeys; i0++) { \
  NifStream( scales.keys[i0], in, version ); \
}; \

#define A_KEYFRAME_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( rotations.numKeys, out, version ); \
if ( rotations.numKeys != 0 ) { \
  NifStream( rotations.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < rotations.numKeys; i0++) { \
  NifStream( rotations.keys[i0], out, version ); \
}; \
NifStream( translations.numKeys, out, version ); \
if ( translations.numKeys != 0 ) { \
  NifStream( translations.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < translations.numKeys; i0++) { \
  NifStream( translations.keys[i0], out, version ); \
}; \
NifStream( scales.numKeys, out, version ); \
if ( scales.numKeys != 0 ) { \
  NifStream( scales.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < scales.numKeys; i0++) { \
  NifStream( scales.keys[i0], out, version ); \
}; \

#define A_KEYFRAME_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Num Keys:  " << rotations.numKeys << endl; \
if ( rotations.numKeys != 0 ) { \
  out << "  Key Type:  " << rotations.keyType << endl; \
}; \
for (uint i0 = 0; i0 < rotations.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << rotations.keys[i0] << endl; \
}; \
out << "Num Keys:  " << translations.numKeys << endl; \
if ( translations.numKeys != 0 ) { \
  out << "  Key Type:  " << translations.keyType << endl; \
}; \
for (uint i0 = 0; i0 < translations.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << translations.keys[i0] << endl; \
}; \
out << "Num Keys:  " << scales.numKeys << endl; \
if ( scales.numKeys != 0 ) { \
  out << "  Key Type:  " << scales.keyType << endl; \
}; \
for (uint i0 = 0; i0 < scales.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << scales.keys[i0] << endl; \
}; \
return out.str(); \

#define A_KEYFRAME_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( rotations.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < rotations.numKeys; i0++) { \
}; \
if ( translations.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < translations.numKeys; i0++) { \
}; \
if ( scales.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < scales.numKeys; i0++) { \
}; \

#define A_PARTICLE_MODIFIER_MEMBERS \
Ref<AParticleModifier > nextModifier; \
uint previousModifier; \

#define A_PARTICLE_MODIFIER_PARENTS NiObject \

#define A_PARTICLE_MODIFIER_CONSTRUCT \

#define A_PARTICLE_MODIFIER_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( previousModifier, in, version ); \

#define A_PARTICLE_MODIFIER_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( link_map[nextModifier], out, version ); \
NifStream( previousModifier, out, version ); \

#define A_PARTICLE_MODIFIER_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Next Modifier:  " << nextModifier << endl; \
out << "Previous Modifier:  " << previousModifier << endl; \
return out.str(); \

#define A_PARTICLE_MODIFIER_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
nextModifier = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define A_P_SYS_MODIFIER_MEMBERS \
string name; \
uint order; \
NiParticleSystem * target; \
bool active; \

#define A_P_SYS_MODIFIER_PARENTS NiObject \

#define A_P_SYS_MODIFIER_CONSTRUCT \

#define A_P_SYS_MODIFIER_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( name, in, version ); \
NifStream( order, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( active, in, version ); \

#define A_P_SYS_MODIFIER_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( name, out, version ); \
NifStream( order, out, version ); \
NifStream( link_map[target], out, version ); \
NifStream( active, out, version ); \

#define A_P_SYS_MODIFIER_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Order:  " << order << endl; \
out << "Target:  " << target << endl; \
out << "Active:  " << active << endl; \
return out.str(); \

#define A_P_SYS_MODIFIER_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
target = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define A_P_SYS_EMITTER_MEMBERS \
float speed; \
float speedVariation; \
float declination; \
float declinationVariation; \
float planarAngle; \
float planarAngleVariation; \
Color4 initialColor; \
float initialRadius; \
float radiusVariation; \
float lifeSpan; \
float lifeSpanVariation; \

#define A_P_SYS_EMITTER_PARENTS APSysModifier \

#define A_P_SYS_EMITTER_CONSTRUCT \

#define A_P_SYS_EMITTER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( speed, in, version ); \
NifStream( speedVariation, in, version ); \
NifStream( declination, in, version ); \
NifStream( declinationVariation, in, version ); \
NifStream( planarAngle, in, version ); \
NifStream( planarAngleVariation, in, version ); \
NifStream( initialColor, in, version ); \
NifStream( initialRadius, in, version ); \
NifStream( radiusVariation, in, version ); \
NifStream( lifeSpan, in, version ); \
NifStream( lifeSpanVariation, in, version ); \

#define A_P_SYS_EMITTER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( speed, out, version ); \
NifStream( speedVariation, out, version ); \
NifStream( declination, out, version ); \
NifStream( declinationVariation, out, version ); \
NifStream( planarAngle, out, version ); \
NifStream( planarAngleVariation, out, version ); \
NifStream( initialColor, out, version ); \
NifStream( initialRadius, out, version ); \
NifStream( radiusVariation, out, version ); \
NifStream( lifeSpan, out, version ); \
NifStream( lifeSpanVariation, out, version ); \

#define A_P_SYS_EMITTER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Speed:  " << speed << endl; \
out << "Speed Variation:  " << speedVariation << endl; \
out << "Declination:  " << declination << endl; \
out << "Declination Variation:  " << declinationVariation << endl; \
out << "Planar Angle:  " << planarAngle << endl; \
out << "Planar Angle Variation:  " << planarAngleVariation << endl; \
out << "Initial Color:  " << initialColor << endl; \
out << "Initial Radius:  " << initialRadius << endl; \
out << "Radius Variation:  " << radiusVariation << endl; \
out << "Life Span:  " << lifeSpan << endl; \
out << "Life Span Variation:  " << lifeSpanVariation << endl; \
return out.str(); \

#define A_P_SYS_EMITTER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define A_P_SYS_VOLUME_EMITTER_MEMBERS \
uint emitterObject; \

#define A_P_SYS_VOLUME_EMITTER_PARENTS APSysEmitter \

#define A_P_SYS_VOLUME_EMITTER_CONSTRUCT \

#define A_P_SYS_VOLUME_EMITTER_READ \
APSysEmitter::Read( in, link_stack, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( emitterObject, in, version ); \
}; \

#define A_P_SYS_VOLUME_EMITTER_WRITE \
APSysEmitter::Write( out, link_map, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( emitterObject, out, version ); \
}; \

#define A_P_SYS_VOLUME_EMITTER_STRING \
stringstream out; \
out << APSysEmitter::asString(); \
out << "Emitter Object:  " << emitterObject << endl; \
return out.str(); \

#define A_P_SYS_VOLUME_EMITTER_FIXLINKS \
APSysEmitter::FixLinks( objects, link_stack, version ); \
if ( version >= 0x14000004 ) { \
}; \

#define BHK_REF_OBJECT_MEMBERS \

#define BHK_REF_OBJECT_PARENTS NiObject \

#define BHK_REF_OBJECT_CONSTRUCT \

#define BHK_REF_OBJECT_READ \
NiObject::Read( in, link_stack, version ); \

#define BHK_REF_OBJECT_WRITE \
NiObject::Write( out, link_map, version ); \

#define BHK_REF_OBJECT_STRING \
stringstream out; \
out << NiObject::asString(); \
return out.str(); \

#define BHK_REF_OBJECT_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \

#define BHK_SERIALIZABLE_MEMBERS \

#define BHK_SERIALIZABLE_PARENTS bhkRefObject \

#define BHK_SERIALIZABLE_CONSTRUCT \

#define BHK_SERIALIZABLE_READ \
bhkRefObject::Read( in, link_stack, version ); \

#define BHK_SERIALIZABLE_WRITE \
bhkRefObject::Write( out, link_map, version ); \

#define BHK_SERIALIZABLE_STRING \
stringstream out; \
out << bhkRefObject::asString(); \
return out.str(); \

#define BHK_SERIALIZABLE_FIXLINKS \
bhkRefObject::FixLinks( objects, link_stack, version ); \

#define ABHK_CONSTRAINT_MEMBERS \
vector<bhkRigidBody > > bodies; \
uint priority; \

#define ABHK_CONSTRAINT_PARENTS bhkSerializable \

#define ABHK_CONSTRAINT_CONSTRUCT \
 : priority(1) \

#define ABHK_CONSTRAINT_READ \
bhkSerializable::Read( in, link_stack, version ); \
uint numBodies; \
NifStream( numBodies, in, version ); \
bodies.resize(numBodies); \
for (uint i0 = 0; i0 < numBodies; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( priority, in, version ); \

#define ABHK_CONSTRAINT_WRITE \
bhkSerializable::Write( out, link_map, version ); \
uint numBodies; \
numBodies = uint(numBodies.size()); \
NifStream( numBodies, out, version ); \
for (uint i0 = 0; i0 < numBodies; i0++) { \
  NifStream( link_map[bodies[i0]], out, version ); \
}; \
NifStream( priority, out, version ); \

#define ABHK_CONSTRAINT_STRING \
stringstream out; \
out << bhkSerializable::asString(); \
out << "Num Bodies:  " << numBodies << endl; \
for (uint i0 = 0; i0 < numBodies; i0++) { \
  out << "  Bodies[" << i0 << "]:  " << bodies[i0] << endl; \
}; \
out << "Priority:  " << priority << endl; \
return out.str(); \

#define ABHK_CONSTRAINT_FIXLINKS \
bhkSerializable::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numBodies; i0++) { \
  bodies[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define ABHK_RAGDOLL_CONSTRAINT_MEMBERS \
Float4 pivotA; \
Float4 planeA; \
Float4 twistA; \
Float4 pivotB; \
Float4 planeB; \
Float4 twistB; \
float coneMinAngle; \
float planeMinAngle; \
float planeMaxAngle; \
float twistMinAngle; \
float twistMaxAngle; \
float maxFriction; \

#define ABHK_RAGDOLL_CONSTRAINT_PARENTS AbhkConstraint \

#define ABHK_RAGDOLL_CONSTRAINT_CONSTRUCT \

#define ABHK_RAGDOLL_CONSTRAINT_READ \
AbhkConstraint::Read( in, link_stack, version ); \
NifStream( pivotA, in, version ); \
NifStream( planeA, in, version ); \
NifStream( twistA, in, version ); \
NifStream( pivotB, in, version ); \
NifStream( planeB, in, version ); \
NifStream( twistB, in, version ); \
NifStream( coneMinAngle, in, version ); \
NifStream( planeMinAngle, in, version ); \
NifStream( planeMaxAngle, in, version ); \
NifStream( twistMinAngle, in, version ); \
NifStream( twistMaxAngle, in, version ); \
NifStream( maxFriction, in, version ); \

#define ABHK_RAGDOLL_CONSTRAINT_WRITE \
AbhkConstraint::Write( out, link_map, version ); \
NifStream( pivotA, out, version ); \
NifStream( planeA, out, version ); \
NifStream( twistA, out, version ); \
NifStream( pivotB, out, version ); \
NifStream( planeB, out, version ); \
NifStream( twistB, out, version ); \
NifStream( coneMinAngle, out, version ); \
NifStream( planeMinAngle, out, version ); \
NifStream( planeMaxAngle, out, version ); \
NifStream( twistMinAngle, out, version ); \
NifStream( twistMaxAngle, out, version ); \
NifStream( maxFriction, out, version ); \

#define ABHK_RAGDOLL_CONSTRAINT_STRING \
stringstream out; \
out << AbhkConstraint::asString(); \
out << "Pivot A:  " << pivotA << endl; \
out << "Plane A:  " << planeA << endl; \
out << "Twist A:  " << twistA << endl; \
out << "Pivot B:  " << pivotB << endl; \
out << "Plane B:  " << planeB << endl; \
out << "Twist B:  " << twistB << endl; \
out << "Cone Min Angle:  " << coneMinAngle << endl; \
out << "Plane Min Angle:  " << planeMinAngle << endl; \
out << "Plane Max Angle:  " << planeMaxAngle << endl; \
out << "Twist Min Angle:  " << twistMinAngle << endl; \
out << "Twist Max Angle:  " << twistMaxAngle << endl; \
out << "Max Friction:  " << maxFriction << endl; \
return out.str(); \

#define ABHK_RAGDOLL_CONSTRAINT_FIXLINKS \
AbhkConstraint::FixLinks( objects, link_stack, version ); \

#define BHK_SHAPE_MEMBERS \

#define BHK_SHAPE_PARENTS bhkSerializable \

#define BHK_SHAPE_CONSTRUCT \

#define BHK_SHAPE_READ \
bhkSerializable::Read( in, link_stack, version ); \

#define BHK_SHAPE_WRITE \
bhkSerializable::Write( out, link_map, version ); \

#define BHK_SHAPE_STRING \
stringstream out; \
out << bhkSerializable::asString(); \
return out.str(); \

#define BHK_SHAPE_FIXLINKS \
bhkSerializable::FixLinks( objects, link_stack, version ); \

#define ABHK_SHAPE_COLLECTION_MEMBERS \

#define ABHK_SHAPE_COLLECTION_PARENTS bhkShape \

#define ABHK_SHAPE_COLLECTION_CONSTRUCT \

#define ABHK_SHAPE_COLLECTION_READ \
bhkShape::Read( in, link_stack, version ); \

#define ABHK_SHAPE_COLLECTION_WRITE \
bhkShape::Write( out, link_map, version ); \

#define ABHK_SHAPE_COLLECTION_STRING \
stringstream out; \
out << bhkShape::asString(); \
return out.str(); \

#define ABHK_SHAPE_COLLECTION_FIXLINKS \
bhkShape::FixLinks( objects, link_stack, version ); \

#define BHK_SPHERE_REP_SHAPE_MEMBERS \
uint material; \

#define BHK_SPHERE_REP_SHAPE_PARENTS bhkShape \

#define BHK_SPHERE_REP_SHAPE_CONSTRUCT \

#define BHK_SPHERE_REP_SHAPE_READ \
bhkShape::Read( in, link_stack, version ); \
NifStream( material, in, version ); \

#define BHK_SPHERE_REP_SHAPE_WRITE \
bhkShape::Write( out, link_map, version ); \
NifStream( material, out, version ); \

#define BHK_SPHERE_REP_SHAPE_STRING \
stringstream out; \
out << bhkShape::asString(); \
out << "Material:  " << material << endl; \
return out.str(); \

#define BHK_SPHERE_REP_SHAPE_FIXLINKS \
bhkShape::FixLinks( objects, link_stack, version ); \

#define BHK_CONVEX_SHAPE_MEMBERS \

#define BHK_CONVEX_SHAPE_PARENTS bhkSphereRepShape \

#define BHK_CONVEX_SHAPE_CONSTRUCT \

#define BHK_CONVEX_SHAPE_READ \
bhkSphereRepShape::Read( in, link_stack, version ); \

#define BHK_CONVEX_SHAPE_WRITE \
bhkSphereRepShape::Write( out, link_map, version ); \

#define BHK_CONVEX_SHAPE_STRING \
stringstream out; \
out << bhkSphereRepShape::asString(); \
return out.str(); \

#define BHK_CONVEX_SHAPE_FIXLINKS \
bhkSphereRepShape::FixLinks( objects, link_stack, version ); \

#define BHK_WORLD_OBJECT_MEMBERS \

#define BHK_WORLD_OBJECT_PARENTS bhkSerializable \

#define BHK_WORLD_OBJECT_CONSTRUCT \

#define BHK_WORLD_OBJECT_READ \
bhkSerializable::Read( in, link_stack, version ); \

#define BHK_WORLD_OBJECT_WRITE \
bhkSerializable::Write( out, link_map, version ); \

#define BHK_WORLD_OBJECT_STRING \
stringstream out; \
out << bhkSerializable::asString(); \
return out.str(); \

#define BHK_WORLD_OBJECT_FIXLINKS \
bhkSerializable::FixLinks( objects, link_stack, version ); \

#define BHK_ENTITY_MEMBERS \
Ref<bhkSphereRepShape > shape; \
uint layer; \

#define BHK_ENTITY_PARENTS bhkWorldObject \

#define BHK_ENTITY_CONSTRUCT \

#define BHK_ENTITY_READ \
uint block_num; \
bhkWorldObject::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( layer, in, version ); \

#define BHK_ENTITY_WRITE \
bhkWorldObject::Write( out, link_map, version ); \
NifStream( link_map[shape], out, version ); \
NifStream( layer, out, version ); \

#define BHK_ENTITY_STRING \
stringstream out; \
out << bhkWorldObject::asString(); \
out << "Shape:  " << shape << endl; \
out << "Layer:  " << layer << endl; \
return out.str(); \

#define BHK_ENTITY_FIXLINKS \
bhkWorldObject::FixLinks( objects, link_stack, version ); \
shape = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define ABHK_RIGID_BODY_MEMBERS \
vector<float > unknownFloats1; \
vector<ushort > unknownShorts1; \
uint layerCopy_; \
vector<ushort > unknownShorts2; \
Vector3 translation; \
float unknownFloat00; \
QuaternionXYZW rotation; \
Vector3 linearVelocity; \
float unknownFloat01; \
Vector3 angularVelocity; \
float unknownFloat02; \
vector<float > transform_; \
Vector3 center; \
float unknownFloat03; \
float mass; \
float linearDamping; \
float angularDamping; \
float friction; \
float restitution; \
float maxLinearVelocity; \
float maxAngularVelocity; \
float penDepth; \
byte motionSystem_; \
byte unknownByte1; \
byte unknownByte2; \
byte qualityType; \
uint unknownInt6; \
uint unknownInt7; \
uint unknownInt8; \
vector< Ref<AbhkConstraint > constraints; \

#define ABHK_RIGID_BODY_PARENTS bhkEntity \

#define ABHK_RIGID_BODY_CONSTRUCT \
 : maxAngularVelocity(31.415926535) \

#define ABHK_RIGID_BODY_READ \
uint block_num; \
bhkEntity::Read( in, link_stack, version ); \
unknownFloats1.resize(5); \
for (uint i0 = 0; i0 < 5; i0++) { \
  NifStream( unknownFloats1[i0], in, version ); \
}; \
unknownShorts1.resize(4); \
for (uint i0 = 0; i0 < 4; i0++) { \
  NifStream( unknownShorts1[i0], in, version ); \
}; \
NifStream( layerCopy_, in, version ); \
unknownShorts2.resize(6); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownShorts2[i0], in, version ); \
}; \
NifStream( translation, in, version ); \
NifStream( unknownFloat00, in, version ); \
NifStream( rotation.x, in, version ); \
NifStream( rotation.y, in, version ); \
NifStream( rotation.z, in, version ); \
NifStream( rotation.w, in, version ); \
NifStream( linearVelocity, in, version ); \
NifStream( unknownFloat01, in, version ); \
NifStream( angularVelocity, in, version ); \
NifStream( unknownFloat02, in, version ); \
transform_.resize(12); \
for (uint i0 = 0; i0 < 12; i0++) { \
  NifStream( transform_[i0], in, version ); \
}; \
NifStream( center, in, version ); \
NifStream( unknownFloat03, in, version ); \
NifStream( mass, in, version ); \
NifStream( linearDamping, in, version ); \
NifStream( angularDamping, in, version ); \
NifStream( friction, in, version ); \
NifStream( restitution, in, version ); \
NifStream( maxLinearVelocity, in, version ); \
NifStream( maxAngularVelocity, in, version ); \
NifStream( penDepth, in, version ); \
NifStream( motionSystem_, in, version ); \
NifStream( unknownByte1, in, version ); \
NifStream( unknownByte2, in, version ); \
NifStream( qualityType, in, version ); \
NifStream( unknownInt6, in, version ); \
NifStream( unknownInt7, in, version ); \
NifStream( unknownInt8, in, version ); \
uint constraints_numIndices; \
NifStream( constraints_numIndices, in, version ); \
constraints.indices.resize(constraints_numIndices); \
for (uint i0 = 0; i0 < constraints_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( unknownInt6, in, version ); \

#define ABHK_RIGID_BODY_WRITE \
bhkEntity::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 5; i0++) { \
  NifStream( unknownFloats1[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 4; i0++) { \
  NifStream( unknownShorts1[i0], out, version ); \
}; \
NifStream( layerCopy_, out, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownShorts2[i0], out, version ); \
}; \
NifStream( translation, out, version ); \
NifStream( unknownFloat00, out, version ); \
NifStream( rotation.x, out, version ); \
NifStream( rotation.y, out, version ); \
NifStream( rotation.z, out, version ); \
NifStream( rotation.w, out, version ); \
NifStream( linearVelocity, out, version ); \
NifStream( unknownFloat01, out, version ); \
NifStream( angularVelocity, out, version ); \
NifStream( unknownFloat02, out, version ); \
for (uint i0 = 0; i0 < 12; i0++) { \
  NifStream( transform_[i0], out, version ); \
}; \
NifStream( center, out, version ); \
NifStream( unknownFloat03, out, version ); \
NifStream( mass, out, version ); \
NifStream( linearDamping, out, version ); \
NifStream( angularDamping, out, version ); \
NifStream( friction, out, version ); \
NifStream( restitution, out, version ); \
NifStream( maxLinearVelocity, out, version ); \
NifStream( maxAngularVelocity, out, version ); \
NifStream( penDepth, out, version ); \
NifStream( motionSystem_, out, version ); \
NifStream( unknownByte1, out, version ); \
NifStream( unknownByte2, out, version ); \
NifStream( qualityType, out, version ); \
NifStream( unknownInt6, out, version ); \
NifStream( unknownInt7, out, version ); \
NifStream( unknownInt8, out, version ); \
uint constraints_numIndices; \
constraints_numIndices = uint(constraints.numIndices.size()); \
NifStream( constraints_numIndices, out, version ); \
for (uint i0 = 0; i0 < constraints_numIndices; i0++) { \
  NifStream( link_map[constraints.indices[i0]], out, version ); \
}; \
NifStream( unknownInt6, out, version ); \

#define ABHK_RIGID_BODY_STRING \
stringstream out; \
out << bhkEntity::asString(); \
for (uint i0 = 0; i0 < 5; i0++) { \
  out << "  Unknown Floats 1[" << i0 << "]:  " << unknownFloats1[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 4; i0++) { \
  out << "  Unknown Shorts 1[" << i0 << "]:  " << unknownShorts1[i0] << endl; \
}; \
out << "Layer Copy?:  " << layerCopy_ << endl; \
for (uint i0 = 0; i0 < 6; i0++) { \
  out << "  Unknown Shorts 2[" << i0 << "]:  " << unknownShorts2[i0] << endl; \
}; \
out << "Translation:  " << translation << endl; \
out << "Unknown Float 00:  " << unknownFloat00 << endl; \
out << "x:  " << rotation.x << endl; \
out << "y:  " << rotation.y << endl; \
out << "z:  " << rotation.z << endl; \
out << "w:  " << rotation.w << endl; \
out << "Linear Velocity:  " << linearVelocity << endl; \
out << "Unknown Float 01:  " << unknownFloat01 << endl; \
out << "Angular Velocity:  " << angularVelocity << endl; \
out << "Unknown Float 02:  " << unknownFloat02 << endl; \
for (uint i0 = 0; i0 < 12; i0++) { \
  out << "  Transform?[" << i0 << "]:  " << transform_[i0] << endl; \
}; \
out << "Center:  " << center << endl; \
out << "Unknown Float 03:  " << unknownFloat03 << endl; \
out << "Mass:  " << mass << endl; \
out << "Linear Damping:  " << linearDamping << endl; \
out << "Angular Damping:  " << angularDamping << endl; \
out << "Friction:  " << friction << endl; \
out << "Restitution:  " << restitution << endl; \
out << "Max Linear Velocity:  " << maxLinearVelocity << endl; \
out << "Max Angular Velocity:  " << maxAngularVelocity << endl; \
out << "Pen Depth:  " << penDepth << endl; \
out << "Motion System?:  " << motionSystem_ << endl; \
out << "Unknown Byte 1:  " << unknownByte1 << endl; \
out << "Unknown Byte 2:  " << unknownByte2 << endl; \
out << "Quality Type:  " << qualityType << endl; \
out << "Unknown Int 6:  " << unknownInt6 << endl; \
out << "Unknown Int 7:  " << unknownInt7 << endl; \
out << "Unknown Int 8:  " << unknownInt8 << endl; \
out << "Num Indices:  " << constraints_numIndices << endl; \
for (uint i0 = 0; i0 < constraints_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << constraints.indices[i0] << endl; \
}; \
out << "Unknown Int 6:  " << unknownInt6 << endl; \
return out.str(); \

#define ABHK_RIGID_BODY_FIXLINKS \
bhkEntity::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 5; i0++) { \
}; \
for (uint i0 = 0; i0 < 4; i0++) { \
}; \
for (uint i0 = 0; i0 < 6; i0++) { \
}; \
for (uint i0 = 0; i0 < 12; i0++) { \
}; \
for (uint i0 = 0; i0 < constraints_numIndices; i0++) { \
  constraints.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define ABHK_TRANSFORM_SHAPE_MEMBERS \
float unknownFloat1; \
float unknownFloat2; \
float unknownFloat3; \
Matrix44 transform; \

#define ABHK_TRANSFORM_SHAPE_PARENTS bhkEntity \

#define ABHK_TRANSFORM_SHAPE_CONSTRUCT \

#define ABHK_TRANSFORM_SHAPE_READ \
bhkEntity::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \
NifStream( unknownFloat3, in, version ); \
NifStream( transform, in, version ); \

#define ABHK_TRANSFORM_SHAPE_WRITE \
bhkEntity::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \
NifStream( unknownFloat3, out, version ); \
NifStream( transform, out, version ); \

#define ABHK_TRANSFORM_SHAPE_STRING \
stringstream out; \
out << bhkEntity::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Float 3:  " << unknownFloat3 << endl; \
out << "Transform:  " << transform << endl; \
return out.str(); \

#define ABHK_TRANSFORM_SHAPE_FIXLINKS \
bhkEntity::FixLinks( objects, link_stack, version ); \

#define NI_COLLISION_OBJECT_MEMBERS \
ushort unknownShort; \
Ref<NiObject > body; \

#define NI_COLLISION_OBJECT_PARENTS NiObject \

#define NI_COLLISION_OBJECT_CONSTRUCT \

#define NI_COLLISION_OBJECT_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
NiAVObject * parent; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknownShort, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_COLLISION_OBJECT_WRITE \
NiObject::Write( out, link_map, version ); \
NiAVObject * parent; \

#define NI_COLLISION_OBJECT_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Parent:  " << parent << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Body:  " << body << endl; \
return out.str(); \

#define NI_COLLISION_OBJECT_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
parent = blocks[link_stack.front()]; \
link_stack.pop_front(); \
body = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_EXTRA_DATA_MEMBERS \
string name; \
Ref<NiExtraData > nextExtraData; \

#define NI_EXTRA_DATA_PARENTS NiObject \

#define NI_EXTRA_DATA_CONSTRUCT \

#define NI_EXTRA_DATA_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
if ( version >= 0x0A000100 ) { \
  NifStream( name, in, version ); \
}; \
if ( version <= 0x04020200 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_EXTRA_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
if ( version >= 0x0A000100 ) { \
  NifStream( name, out, version ); \
}; \
if ( version <= 0x04020200 ) { \
  NifStream( link_map[nextExtraData], out, version ); \
}; \

#define NI_EXTRA_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Next Extra Data:  " << nextExtraData << endl; \
return out.str(); \

#define NI_EXTRA_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A000100 ) { \
}; \
if ( version <= 0x04020200 ) { \
  nextExtraData = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_OBJECT_N_E_T_MEMBERS \
string name; \
Ref<NiExtraData > extraData; \
vector< Ref<NiExtraData > extraDataList; \
Ref<NiTimeController > controller; \

#define NI_OBJECT_N_E_T_PARENTS NiObject \

#define NI_OBJECT_N_E_T_CONSTRUCT \

#define NI_OBJECT_N_E_T_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
NifStream( name, in, version ); \
if ( version <= 0x04020200 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
if ( version >= 0x0A000100 ) { \
  uint extraDataList_numIndices; \
  NifStream( extraDataList_numIndices, in, version ); \
  extraDataList.indices.resize(extraDataList_numIndices); \
  for (uint i1 = 0; i1 < extraDataList_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_OBJECT_N_E_T_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( name, out, version ); \
if ( version <= 0x04020200 ) { \
  NifStream( link_map[extraData], out, version ); \
}; \
if ( version >= 0x0A000100 ) { \
  uint extraDataList_numIndices; \
  extraDataList_numIndices = uint(extraDataList.numIndices.size()); \
  NifStream( extraDataList_numIndices, out, version ); \
  for (uint i1 = 0; i1 < extraDataList_numIndices; i1++) { \
    NifStream( link_map[extraDataList.indices[i1]], out, version ); \
  }; \
}; \
NifStream( link_map[controller], out, version ); \

#define NI_OBJECT_N_E_T_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Extra Data:  " << extraData << endl; \
out << "Num Indices:  " << extraDataList_numIndices << endl; \
for (uint i0 = 0; i0 < extraDataList_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << extraDataList.indices[i0] << endl; \
}; \
out << "Controller:  " << controller << endl; \
return out.str(); \

#define NI_OBJECT_N_E_T_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version <= 0x04020200 ) { \
  extraData = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( version >= 0x0A000100 ) { \
  for (uint i1 = 0; i1 < extraDataList_numIndices; i1++) { \
    extraDataList.indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \
controller = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_A_V_OBJECT_MEMBERS \
Flags flags; \
Vector3 translation; \
Matrix33 rotation; \
float scale; \
Vector3 velocity; \
vector< Ref<NiProperty > properties; \
BoundingBox boundingBox; \
Ref<NiCollisionData > collisionData; \

#define NI_A_V_OBJECT_PARENTS NiObjectNET \

#define NI_A_V_OBJECT_CONSTRUCT \
 : scale(1.0) \

#define NI_A_V_OBJECT_READ \
uint block_num; \
NiObjectNET::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \
NifStream( translation, in, version ); \
NifStream( rotation, in, version ); \
NifStream( scale, in, version ); \
if ( version <= 0x04020200 ) { \
  NifStream( velocity, in, version ); \
}; \
uint properties_numIndices; \
NifStream( properties_numIndices, in, version ); \
properties.indices.resize(properties_numIndices); \
for (uint i0 = 0; i0 < properties_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
if ( version <= 0x04020200 ) { \
  NifStream( boundingBox.isUsed, in, version ); \
  if ( boundingBox.isUsed != 0 ) { \
    NifStream( boundingBox.unknownInt, in, version ); \
    NifStream( boundingBox.translation, in, version ); \
    NifStream( boundingBox.rotation, in, version ); \
    NifStream( boundingBox.radius, in, version ); \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_A_V_OBJECT_WRITE \
NiObjectNET::Write( out, link_map, version ); \
NifStream( flags, out, version ); \
NifStream( translation, out, version ); \
NifStream( rotation, out, version ); \
NifStream( scale, out, version ); \
if ( version <= 0x04020200 ) { \
  NifStream( velocity, out, version ); \
}; \
uint properties_numIndices; \
properties_numIndices = uint(properties.numIndices.size()); \
NifStream( properties_numIndices, out, version ); \
for (uint i0 = 0; i0 < properties_numIndices; i0++) { \
  NifStream( link_map[properties.indices[i0]], out, version ); \
}; \
if ( version <= 0x04020200 ) { \
  NifStream( boundingBox.isUsed, out, version ); \
  if ( boundingBox.isUsed != 0 ) { \
    NifStream( boundingBox.unknownInt, out, version ); \
    NifStream( boundingBox.translation, out, version ); \
    NifStream( boundingBox.rotation, out, version ); \
    NifStream( boundingBox.radius, out, version ); \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  NifStream( link_map[collisionData], out, version ); \
}; \

#define NI_A_V_OBJECT_STRING \
stringstream out; \
out << NiObjectNET::asString(); \
out << "Flags:  " << flags << endl; \
out << "Translation:  " << translation << endl; \
out << "Rotation:  " << rotation << endl; \
out << "Scale:  " << scale << endl; \
out << "Velocity:  " << velocity << endl; \
out << "Num Indices:  " << properties_numIndices << endl; \
for (uint i0 = 0; i0 < properties_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << properties.indices[i0] << endl; \
}; \
out << "Is Used:  " << boundingBox.isUsed << endl; \
if ( boundingBox.isUsed != 0 ) { \
  out << "  Unknown Int:  " << boundingBox.unknownInt << endl; \
  out << "  Translation:  " << boundingBox.translation << endl; \
  out << "  Rotation:  " << boundingBox.rotation << endl; \
  out << "  Radius:  " << boundingBox.radius << endl; \
}; \
out << "Collision Data:  " << collisionData << endl; \
return out.str(); \

#define NI_A_V_OBJECT_FIXLINKS \
NiObjectNET::FixLinks( objects, link_stack, version ); \
if ( version <= 0x04020200 ) { \
}; \
for (uint i0 = 0; i0 < properties_numIndices; i0++) { \
  properties.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( version <= 0x04020200 ) { \
  if ( boundingBox.isUsed != 0 ) { \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  collisionData = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_DYNAMIC_EFFECT_MEMBERS \
CondInt affectedNodeList_; \
bool switchState; \
vector< Ref<NiAVObject > affectedNodes; \

#define NI_DYNAMIC_EFFECT_PARENTS NiAVObject \

#define NI_DYNAMIC_EFFECT_CONSTRUCT \

#define NI_DYNAMIC_EFFECT_READ \
uint block_num; \
NiAVObject::Read( in, link_stack, version ); \
if ( version <= 0x04000002 ) { \
  NifStream( affectedNodeList_.isUsed, in, version ); \
  if ( affectedNodeList_.isUsed != 0 ) { \
    NifStream( affectedNodeList_.unknownInt, in, version ); \
  }; \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( switchState, in, version ); \
}; \
if ( version >= 0x0A010000 ) { \
  uint affectedNodes_numIndices; \
  NifStream( affectedNodes_numIndices, in, version ); \
  affectedNodes.indices.resize(affectedNodes_numIndices); \
  for (uint i1 = 0; i1 < affectedNodes_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \

#define NI_DYNAMIC_EFFECT_WRITE \
NiAVObject::Write( out, link_map, version ); \
if ( version <= 0x04000002 ) { \
  NifStream( affectedNodeList_.isUsed, out, version ); \
  if ( affectedNodeList_.isUsed != 0 ) { \
    NifStream( affectedNodeList_.unknownInt, out, version ); \
  }; \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( switchState, out, version ); \
}; \
if ( version >= 0x0A010000 ) { \
  uint affectedNodes_numIndices; \
  affectedNodes_numIndices = uint(affectedNodes.numIndices.size()); \
  NifStream( affectedNodes_numIndices, out, version ); \
  for (uint i1 = 0; i1 < affectedNodes_numIndices; i1++) { \
    NifStream( link_map[affectedNodes.indices[i1]], out, version ); \
  }; \
}; \

#define NI_DYNAMIC_EFFECT_STRING \
stringstream out; \
out << NiAVObject::asString(); \
out << "Is Used:  " << affectedNodeList_.isUsed << endl; \
if ( affectedNodeList_.isUsed != 0 ) { \
  out << "  Unknown Int:  " << affectedNodeList_.unknownInt << endl; \
}; \
out << "Switch State:  " << switchState << endl; \
out << "Num Indices:  " << affectedNodes_numIndices << endl; \
for (uint i0 = 0; i0 < affectedNodes_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << affectedNodes.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_DYNAMIC_EFFECT_FIXLINKS \
NiAVObject::FixLinks( objects, link_stack, version ); \
if ( version <= 0x04000002 ) { \
  if ( affectedNodeList_.isUsed != 0 ) { \
  }; \
}; \
if ( version >= 0x0A020000 ) { \
}; \
if ( version >= 0x0A010000 ) { \
  for (uint i1 = 0; i1 < affectedNodes_numIndices; i1++) { \
    affectedNodes.indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \

#define NI_LIGHT_MEMBERS \
float dimmer; \
Color3 ambientColor; \
Color3 diffuseColor; \
Color3 specularColor; \

#define NI_LIGHT_PARENTS NiDynamicEffect \

#define NI_LIGHT_CONSTRUCT \

#define NI_LIGHT_READ \
NiDynamicEffect::Read( in, link_stack, version ); \
NifStream( dimmer, in, version ); \
NifStream( ambientColor, in, version ); \
NifStream( diffuseColor, in, version ); \
NifStream( specularColor, in, version ); \

#define NI_LIGHT_WRITE \
NiDynamicEffect::Write( out, link_map, version ); \
NifStream( dimmer, out, version ); \
NifStream( ambientColor, out, version ); \
NifStream( diffuseColor, out, version ); \
NifStream( specularColor, out, version ); \

#define NI_LIGHT_STRING \
stringstream out; \
out << NiDynamicEffect::asString(); \
out << "Dimmer:  " << dimmer << endl; \
out << "Ambient Color:  " << ambientColor << endl; \
out << "Diffuse Color:  " << diffuseColor << endl; \
out << "Specular Color:  " << specularColor << endl; \
return out.str(); \

#define NI_LIGHT_FIXLINKS \
NiDynamicEffect::FixLinks( objects, link_stack, version ); \

#define A_POINT_LIGHT_MEMBERS \
float constantAttenuation; \
float linearAttenuation; \
float quadraticAttenuation; \

#define A_POINT_LIGHT_PARENTS NiLight \

#define A_POINT_LIGHT_CONSTRUCT \

#define A_POINT_LIGHT_READ \
NiLight::Read( in, link_stack, version ); \
NifStream( constantAttenuation, in, version ); \
NifStream( linearAttenuation, in, version ); \
NifStream( quadraticAttenuation, in, version ); \

#define A_POINT_LIGHT_WRITE \
NiLight::Write( out, link_map, version ); \
NifStream( constantAttenuation, out, version ); \
NifStream( linearAttenuation, out, version ); \
NifStream( quadraticAttenuation, out, version ); \

#define A_POINT_LIGHT_STRING \
stringstream out; \
out << NiLight::asString(); \
out << "Constant Attenuation:  " << constantAttenuation << endl; \
out << "Linear Attenuation:  " << linearAttenuation << endl; \
out << "Quadratic Attenuation:  " << quadraticAttenuation << endl; \
return out.str(); \

#define A_POINT_LIGHT_FIXLINKS \
NiLight::FixLinks( objects, link_stack, version ); \

#define NI_PROPERTY_MEMBERS \

#define NI_PROPERTY_PARENTS NiObjectNET \

#define NI_PROPERTY_CONSTRUCT \

#define NI_PROPERTY_READ \
NiObjectNET::Read( in, link_stack, version ); \

#define NI_PROPERTY_WRITE \
NiObjectNET::Write( out, link_map, version ); \

#define NI_PROPERTY_STRING \
stringstream out; \
out << NiObjectNET::asString(); \
return out.str(); \

#define NI_PROPERTY_FIXLINKS \
NiObjectNET::FixLinks( objects, link_stack, version ); \

#define NI_TIME_CONTROLLER_MEMBERS \
Ref<NiTimeController > nextController; \
Flags flags; \
float frequency; \
float phase; \
float startTime; \
float stopTime; \
NiAVObject * target; \

#define NI_TIME_CONTROLLER_PARENTS NiObject \

#define NI_TIME_CONTROLLER_CONSTRUCT \

#define NI_TIME_CONTROLLER_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( flags, in, version ); \
NifStream( frequency, in, version ); \
NifStream( phase, in, version ); \
NifStream( startTime, in, version ); \
NifStream( stopTime, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_TIME_CONTROLLER_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( link_map[nextController], out, version ); \
NifStream( flags, out, version ); \
NifStream( frequency, out, version ); \
NifStream( phase, out, version ); \
NifStream( startTime, out, version ); \
NifStream( stopTime, out, version ); \
NifStream( link_map[target], out, version ); \

#define NI_TIME_CONTROLLER_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Next Controller:  " << nextController << endl; \
out << "Flags:  " << flags << endl; \
out << "Frequency:  " << frequency << endl; \
out << "Phase:  " << phase << endl; \
out << "Start Time:  " << startTime << endl; \
out << "Stop Time:  " << stopTime << endl; \
out << "Target:  " << target << endl; \
return out.str(); \

#define NI_TIME_CONTROLLER_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
nextController = blocks[link_stack.front()]; \
link_stack.pop_front(); \
target = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define A_BONE_L_O_D_CONTROLLER_MEMBERS \
uint unknownInt1; \
uint unknownInt2; \
vector<vector< Ref<NiAVObject > > nodeGroups; \

#define A_BONE_L_O_D_CONTROLLER_PARENTS NiTimeController \

#define A_BONE_L_O_D_CONTROLLER_CONSTRUCT \

#define A_BONE_L_O_D_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
uint numNodeGroups; \
NifStream( unknownInt1, in, version ); \
NifStream( numNodeGroups, in, version ); \
NifStream( unknownInt2, in, version ); \
nodeGroups.resize(numNodeGroups); \
for (uint i0 = 0; i0 < numNodeGroups; i0++) { \
  uint nodeGroups_numIndices; \
  NifStream( nodeGroups_numIndices, in, version ); \
  nodeGroups[i0].indices.resize(nodeGroups_numIndices); \
  for (uint i1 = 0; i1 < nodeGroups_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \

#define A_BONE_L_O_D_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
uint numNodeGroups; \
numNodeGroups = uint(numNodeGroups.size()); \
NifStream( unknownInt1, out, version ); \
NifStream( numNodeGroups, out, version ); \
NifStream( unknownInt2, out, version ); \
for (uint i0 = 0; i0 < numNodeGroups; i0++) { \
  uint nodeGroups_numIndices; \
  nodeGroups_numIndices = uint(nodeGroups[i0].numIndices.size()); \
  NifStream( nodeGroups_numIndices, out, version ); \
  for (uint i1 = 0; i1 < nodeGroups_numIndices; i1++) { \
    NifStream( link_map[nodeGroups[i0].indices[i1]], out, version ); \
  }; \
}; \

#define A_BONE_L_O_D_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
out << "Num Node Groups:  " << numNodeGroups << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
for (uint i0 = 0; i0 < numNodeGroups; i0++) { \
  out << "  Num Indices:  " << nodeGroups_numIndices << endl; \
  for (uint i1 = 0; i1 < nodeGroups_numIndices; i1++) { \
    out << "    Indices[" << i1 << "]:  " << nodeGroups[i0].indices[i1] << endl; \
  }; \
}; \
return out.str(); \

#define A_BONE_L_O_D_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numNodeGroups; i0++) { \
  for (uint i1 = 0; i1 < nodeGroups_numIndices; i1++) { \
    nodeGroups[i0].indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \

#define A_KEYFRAME_CONTROLLER_MEMBERS \
Ref<NiKeyframeData > data; \

#define A_KEYFRAME_CONTROLLER_PARENTS NiTimeController \

#define A_KEYFRAME_CONTROLLER_CONSTRUCT \

#define A_KEYFRAME_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define A_KEYFRAME_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \

#define A_KEYFRAME_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Data:  " << data << endl; \
return out.str(); \

#define A_KEYFRAME_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define A_PARTICLE_SYSTEM_CONTROLLER_MEMBERS \
float speed; \
float speedRandom; \
float verticalDirection; \
float verticalAngle; \
float horizontalDirection; \
float horizontalAngle; \
float unknownFloat5; \
float unknownFloat6; \
float unknownFloat7; \
float unknownFloat8; \
float unknownFloat9; \
float unknownFloat10; \
float unknownFloat11; \
float size; \
float emitStartTime; \
float emitStopTime; \
byte unknownByte; \
float emitRate; \
float lifetime; \
float lifetimeRandom; \
ushort emitFlags; \
Vector3 startRandom; \
Ref<NiObject > emitter; \
ushort unknownShort2_; \
float unknownFloat13_; \
uint unknownInt1_; \
uint unknownInt2_; \
ushort unknownShort3_; \
ParticleGroup particles; \
Ref<NiObject > unknownLink; \
Ref<AParticleModifier > particleExtra; \
Ref<NiObject > unknownLink2; \
byte trailer; \

#define A_PARTICLE_SYSTEM_CONTROLLER_PARENTS NiTimeController \

#define A_PARTICLE_SYSTEM_CONTROLLER_CONSTRUCT \

#define A_PARTICLE_SYSTEM_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
NifStream( speed, in, version ); \
NifStream( speedRandom, in, version ); \
NifStream( verticalDirection, in, version ); \
NifStream( verticalAngle, in, version ); \
NifStream( horizontalDirection, in, version ); \
NifStream( horizontalAngle, in, version ); \
NifStream( unknownFloat5, in, version ); \
NifStream( unknownFloat6, in, version ); \
NifStream( unknownFloat7, in, version ); \
NifStream( unknownFloat8, in, version ); \
NifStream( unknownFloat9, in, version ); \
NifStream( unknownFloat10, in, version ); \
NifStream( unknownFloat11, in, version ); \
NifStream( size, in, version ); \
NifStream( emitStartTime, in, version ); \
NifStream( emitStopTime, in, version ); \
NifStream( unknownByte, in, version ); \
NifStream( emitRate, in, version ); \
NifStream( lifetime, in, version ); \
NifStream( lifetimeRandom, in, version ); \
NifStream( emitFlags, in, version ); \
NifStream( startRandom, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknownShort2_, in, version ); \
NifStream( unknownFloat13_, in, version ); \
NifStream( unknownInt1_, in, version ); \
NifStream( unknownInt2_, in, version ); \
NifStream( unknownShort3_, in, version ); \
ushort particles_numParticles; \
NifStream( particles_numParticles, in, version ); \
NifStream( particles.numValid, in, version ); \
particles.particles.resize(particles_numParticles); \
for (uint i0 = 0; i0 < particles_numParticles; i0++) { \
  NifStream( particles.particles[i0].velocity, in, version ); \
  NifStream( particles.particles[i0].unknownVector, in, version ); \
  NifStream( particles.particles[i0].lifetime, in, version ); \
  NifStream( particles.particles[i0].lifespan, in, version ); \
  NifStream( particles.particles[i0].timestamp, in, version ); \
  NifStream( particles.particles[i0].unknownShort, in, version ); \
  NifStream( particles.particles[i0].vertexId, in, version ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( trailer, in, version ); \

#define A_PARTICLE_SYSTEM_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( speed, out, version ); \
NifStream( speedRandom, out, version ); \
NifStream( verticalDirection, out, version ); \
NifStream( verticalAngle, out, version ); \
NifStream( horizontalDirection, out, version ); \
NifStream( horizontalAngle, out, version ); \
NifStream( unknownFloat5, out, version ); \
NifStream( unknownFloat6, out, version ); \
NifStream( unknownFloat7, out, version ); \
NifStream( unknownFloat8, out, version ); \
NifStream( unknownFloat9, out, version ); \
NifStream( unknownFloat10, out, version ); \
NifStream( unknownFloat11, out, version ); \
NifStream( size, out, version ); \
NifStream( emitStartTime, out, version ); \
NifStream( emitStopTime, out, version ); \
NifStream( unknownByte, out, version ); \
NifStream( emitRate, out, version ); \
NifStream( lifetime, out, version ); \
NifStream( lifetimeRandom, out, version ); \
NifStream( emitFlags, out, version ); \
NifStream( startRandom, out, version ); \
NifStream( link_map[emitter], out, version ); \
NifStream( unknownShort2_, out, version ); \
NifStream( unknownFloat13_, out, version ); \
NifStream( unknownInt1_, out, version ); \
NifStream( unknownInt2_, out, version ); \
NifStream( unknownShort3_, out, version ); \
ushort particles_numParticles; \
particles_numParticles = ushort(particles.numParticles.size()); \
NifStream( particles_numParticles, out, version ); \
NifStream( particles.numValid, out, version ); \
for (uint i0 = 0; i0 < particles_numParticles; i0++) { \
  NifStream( particles.particles[i0].velocity, out, version ); \
  NifStream( particles.particles[i0].unknownVector, out, version ); \
  NifStream( particles.particles[i0].lifetime, out, version ); \
  NifStream( particles.particles[i0].lifespan, out, version ); \
  NifStream( particles.particles[i0].timestamp, out, version ); \
  NifStream( particles.particles[i0].unknownShort, out, version ); \
  NifStream( particles.particles[i0].vertexId, out, version ); \
}; \
NifStream( link_map[unknownLink], out, version ); \
NifStream( link_map[particleExtra], out, version ); \
NifStream( link_map[unknownLink2], out, version ); \
NifStream( trailer, out, version ); \

#define A_PARTICLE_SYSTEM_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Speed:  " << speed << endl; \
out << "Speed Random:  " << speedRandom << endl; \
out << "Vertical Direction:  " << verticalDirection << endl; \
out << "Vertical Angle:  " << verticalAngle << endl; \
out << "Horizontal Direction:  " << horizontalDirection << endl; \
out << "Horizontal Angle:  " << horizontalAngle << endl; \
out << "Unknown Float 5:  " << unknownFloat5 << endl; \
out << "Unknown Float 6:  " << unknownFloat6 << endl; \
out << "Unknown Float 7:  " << unknownFloat7 << endl; \
out << "Unknown Float 8:  " << unknownFloat8 << endl; \
out << "Unknown Float 9:  " << unknownFloat9 << endl; \
out << "Unknown Float 10:  " << unknownFloat10 << endl; \
out << "Unknown Float 11:  " << unknownFloat11 << endl; \
out << "Size:  " << size << endl; \
out << "Emit Start Time:  " << emitStartTime << endl; \
out << "Emit Stop Time:  " << emitStopTime << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Emit Rate:  " << emitRate << endl; \
out << "Lifetime:  " << lifetime << endl; \
out << "Lifetime Random:  " << lifetimeRandom << endl; \
out << "Emit Flags:  " << emitFlags << endl; \
out << "Start Random:  " << startRandom << endl; \
out << "Emitter:  " << emitter << endl; \
out << "Unknown Short 2?:  " << unknownShort2_ << endl; \
out << "Unknown Float 13?:  " << unknownFloat13_ << endl; \
out << "Unknown Int 1?:  " << unknownInt1_ << endl; \
out << "Unknown Int 2?:  " << unknownInt2_ << endl; \
out << "Unknown Short 3?:  " << unknownShort3_ << endl; \
out << "Num Particles:  " << particles_numParticles << endl; \
out << "Num Valid:  " << particles.numValid << endl; \
for (uint i0 = 0; i0 < particles_numParticles; i0++) { \
  out << "  Velocity:  " << particles.particles[i0].velocity << endl; \
  out << "  Unknown Vector:  " << particles.particles[i0].unknownVector << endl; \
  out << "  Lifetime:  " << particles.particles[i0].lifetime << endl; \
  out << "  Lifespan:  " << particles.particles[i0].lifespan << endl; \
  out << "  Timestamp:  " << particles.particles[i0].timestamp << endl; \
  out << "  Unknown Short:  " << particles.particles[i0].unknownShort << endl; \
  out << "  Vertex ID:  " << particles.particles[i0].vertexId << endl; \
}; \
out << "Unknown Link:  " << unknownLink << endl; \
out << "Particle Extra:  " << particleExtra << endl; \
out << "Unknown Link 2:  " << unknownLink2 << endl; \
out << "Trailer:  " << trailer << endl; \
return out.str(); \

#define A_PARTICLE_SYSTEM_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
emitter = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < particles_numParticles; i0++) { \
}; \
unknownLink = blocks[link_stack.front()]; \
link_stack.pop_front(); \
particleExtra = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink2 = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_MEMBERS \
Ref<AInterpolator > interpolator; \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_PARENTS NiTimeController \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_CONSTRUCT \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
if ( version >= 0x0A020000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
if ( version >= 0x0A020000 ) { \
  NifStream( link_map[interpolator], out, version ); \
}; \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Interpolator:  " << interpolator << endl; \
return out.str(); \

#define A_SINGLE_INTERPOLATOR_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A020000 ) { \
  interpolator = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define A_P_SYS_CTLR_MEMBERS \
string modifierName; \

#define A_P_SYS_CTLR_PARENTS ASingleInterpolatorController \

#define A_P_SYS_CTLR_CONSTRUCT \

#define A_P_SYS_CTLR_READ \
ASingleInterpolatorController::Read( in, link_stack, version ); \
NifStream( modifierName, in, version ); \

#define A_P_SYS_CTLR_WRITE \
ASingleInterpolatorController::Write( out, link_map, version ); \
NifStream( modifierName, out, version ); \

#define A_P_SYS_CTLR_STRING \
stringstream out; \
out << ASingleInterpolatorController::asString(); \
out << "Modifier Name:  " << modifierName << endl; \
return out.str(); \

#define A_P_SYS_CTLR_FIXLINKS \
ASingleInterpolatorController::FixLinks( objects, link_stack, version ); \

#define NI_TRI_BASED_GEOM_MEMBERS \
Ref<TriBasedGeomData > data; \
Ref<NiSkinInstance > skinInstance; \
Shader shader; \

#define NI_TRI_BASED_GEOM_PARENTS NiAVObject \

#define NI_TRI_BASED_GEOM_CONSTRUCT \

#define NI_TRI_BASED_GEOM_READ \
uint block_num; \
NiAVObject::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
if ( version >= 0x0A000100 ) { \
  NifStream( shader.hasShader, in, version ); \
  if ( shader.hasShader != 0 ) { \
    NifStream( shader.shaderName, in, version ); \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \

#define NI_TRI_BASED_GEOM_WRITE \
NiAVObject::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \
NifStream( link_map[skinInstance], out, version ); \
if ( version >= 0x0A000100 ) { \
  NifStream( shader.hasShader, out, version ); \
  if ( shader.hasShader != 0 ) { \
    NifStream( shader.shaderName, out, version ); \
    NifStream( link_map[shader.unknownLink], out, version ); \
  }; \
}; \

#define NI_TRI_BASED_GEOM_STRING \
stringstream out; \
out << NiAVObject::asString(); \
out << "Data:  " << data << endl; \
out << "Skin Instance:  " << skinInstance << endl; \
out << "Has Shader:  " << shader.hasShader << endl; \
if ( shader.hasShader != 0 ) { \
  out << "  Shader Name:  " << shader.shaderName << endl; \
  out << "  Unknown Link:  " << shader.unknownLink << endl; \
}; \
return out.str(); \

#define NI_TRI_BASED_GEOM_FIXLINKS \
NiAVObject::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \
skinInstance = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( version >= 0x0A000100 ) { \
  if ( shader.hasShader != 0 ) { \
    shader.unknownLink = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \

#define TRI_BASED_GEOM_DATA_MEMBERS \
string name; \
ushort unknownShort1; \
bool hasVertices; \
vector<Vector3 > vertices; \
byte numUvSets2; \
byte unknownByte; \
bool hasNormals; \
vector<Vector3 > normals; \
vector<vector<Vector3 > > unknownVectors1; \
vector<vector<Vector3 > > unknownVectors2; \
bool hasVertexColors; \
vector<Color4 > vertexColors; \
bool hasUv; \
vector<vector<TexCoord > > uvSets; \
vector<vector<TexCoord > > uvSets2; \
ushort unknownShort2; \
Ref<NiObject > unknownLink; \

#define TRI_BASED_GEOM_DATA_PARENTS NiObject \

#define TRI_BASED_GEOM_DATA_CONSTRUCT \

#define TRI_BASED_GEOM_DATA_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
ushort numVertices; \
Vector3 center; \
float radius; \
ushort numUvSets; \
if ( version >= 0x0A020000 ) { \
  NifStream( name, in, version ); \
}; \
NifStream( numVertices, in, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknownShort1, in, version ); \
}; \
NifStream( hasVertices, in, version ); \
if ( hasVertices != 0 ) { \
  vertices.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( vertices[i1], in, version ); \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  NifStream( numUvSets2, in, version ); \
  NifStream( unknownByte, in, version ); \
}; \
NifStream( hasNormals, in, version ); \
if ( hasNormals != 0 ) { \
  normals.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( normals[i1], in, version ); \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( unknownByte & 16 ) { \
    unknownVectors1.resize(hasNormals); \
    for (uint i2 = 0; i2 < hasNormals; i2++) \
      unknownVectors1[i2].resize(numVertices); \
    for (uint i2 = 0; i2 < hasNormals; i2++) { \
      for (uint i3 = 0; i3 < numVertices; i3++) { \
        NifStream( unknownVectors1[i2][i3], in, version ); \
      }; \
    }; \
    unknownVectors2.resize(hasNormals); \
    for (uint i2 = 0; i2 < hasNormals; i2++) \
      unknownVectors2[i2].resize(numVertices); \
    for (uint i2 = 0; i2 < hasNormals; i2++) { \
      for (uint i3 = 0; i3 < numVertices; i3++) { \
        NifStream( unknownVectors2[i2][i3], in, version ); \
      }; \
    }; \
  }; \
}; \
NifStream( center, in, version ); \
NifStream( radius, in, version ); \
NifStream( hasVertexColors, in, version ); \
if ( hasVertexColors != 0 ) { \
  vertexColors.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( vertexColors[i1], in, version ); \
  }; \
}; \
if ( version <= 0x04020200 ) { \
  NifStream( numUvSets, in, version ); \
}; \
if ( version <= 0x04000002 ) { \
  NifStream( hasUv, in, version ); \
}; \
if ( version <= 0x04020200 ) { \
  uvSets.resize(numUvSets); \
  for (uint i1 = 0; i1 < numUvSets; i1++) \
    uvSets[i1].resize(numVertices); \
  for (uint i1 = 0; i1 < numUvSets; i1++) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      NifStream( uvSets[i1][i2], in, version ); \
    }; \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  uvSets2.resize(numUvSets2 & 63); \
  for (uint i1 = 0; i1 < numUvSets2 & 63; i1++) \
    uvSets2[i1].resize(numVertices); \
  for (uint i1 = 0; i1 < numUvSets2 & 63; i1++) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      NifStream( uvSets2[i1][i2], in, version ); \
    }; \
  }; \
  NifStream( unknownShort2, in, version ); \
}; \
if ( version >= 0x14000004 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define TRI_BASED_GEOM_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
ushort numVertices; \
numVertices = ushort(numVertices.size()); \
Vector3 center; \

#define TRI_BASED_GEOM_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Num Vertices:  " << numVertices << endl; \
out << "Unknown Short 1:  " << unknownShort1 << endl; \
out << "Has Vertices:  " << hasVertices << endl; \
if ( hasVertices != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Vertices[" << i1 << "]:  " << vertices[i1] << endl; \
  }; \
}; \
out << "Num UV Sets 2:  " << numUvSets2 << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Has Normals:  " << hasNormals << endl; \
if ( hasNormals != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Normals[" << i1 << "]:  " << normals[i1] << endl; \
  }; \
}; \
if ( unknownByte & 16 ) { \
  for (uint i1 = 0; i1 < hasNormals; i1++) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      out << "      Unknown Vectors 1[" << i1 << "][" << i2 << "]:  " << unknownVectors1[i1][i2] << endl; \
    }; \
  }; \
  for (uint i1 = 0; i1 < hasNormals; i1++) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      out << "      Unknown Vectors 2[" << i1 << "][" << i2 << "]:  " << unknownVectors2[i1][i2] << endl; \
    }; \
  }; \
}; \
out << "Center:  " << center << endl; \
out << "Radius:  " << radius << endl; \
out << "Has Vertex Colors:  " << hasVertexColors << endl; \
if ( hasVertexColors != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Vertex Colors[" << i1 << "]:  " << vertexColors[i1] << endl; \
  }; \
}; \
out << "Num UV Sets:  " << numUvSets << endl; \
out << "Has UV:  " << hasUv << endl; \
for (uint i0 = 0; i0 < numUvSets; i0++) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    UV Sets[" << i0 << "][" << i1 << "]:  " << uvSets[i0][i1] << endl; \
  }; \
}; \
for (uint i0 = 0; i0 < numUvSets2 & 63; i0++) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    UV Sets 2[" << i0 << "][" << i1 << "]:  " << uvSets2[i0][i1] << endl; \
  }; \
}; \
out << "Unknown Short 2:  " << unknownShort2 << endl; \
out << "Unknown Link:  " << unknownLink << endl; \
return out.str(); \

#define TRI_BASED_GEOM_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A020000 ) { \
}; \
if ( version >= 0x0A010000 ) { \
}; \
if ( hasVertices != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
}; \
if ( hasNormals != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( unknownByte & 16 ) { \
    for (uint i2 = 0; i2 < hasNormals; i2++) { \
      for (uint i3 = 0; i3 < numVertices; i3++) { \
      }; \
    }; \
    for (uint i2 = 0; i2 < hasNormals; i2++) { \
      for (uint i3 = 0; i3 < numVertices; i3++) { \
      }; \
    }; \
  }; \
}; \
if ( hasVertexColors != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \
if ( version <= 0x04020200 ) { \
}; \
if ( version <= 0x04000002 ) { \
}; \
if ( version <= 0x04020200 ) { \
  for (uint i1 = 0; i1 < numUvSets; i1++) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
    }; \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  for (uint i1 = 0; i1 < numUvSets2 & 63; i1++) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  unknownLink = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define A_PARTICLES_DATA_MEMBERS \
ushort numParticles; \
float size; \
ushort numActive; \
ushort unknownShort; \
bool hasSizes; \
vector<float > sizes; \

#define A_PARTICLES_DATA_PARENTS TriBasedGeomData \

#define A_PARTICLES_DATA_CONSTRUCT \

#define A_PARTICLES_DATA_READ \
TriBasedGeomData::Read( in, link_stack, version ); \
if ( version <= 0x04000002 ) { \
  NifStream( numParticles, in, version ); \
}; \
if ( version <= 0x0A000100 ) { \
  NifStream( size, in, version ); \
}; \
if ( version <= 0x04000002 ) { \
  NifStream( numActive, in, version ); \
}; \
if ( ( version >= 0x0401000C ) && ( version <= 0x0A000100 ) ) { \
  NifStream( unknownShort, in, version ); \
}; \
NifStream( hasSizes, in, version ); \
if ( hasSizes != 0 ) { \
  sizes.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( sizes[i1], in, version ); \
  }; \
}; \

#define A_PARTICLES_DATA_WRITE \
TriBasedGeomData::Write( out, link_map, version ); \
if ( version <= 0x04000002 ) { \
  NifStream( numParticles, out, version ); \
}; \
if ( version <= 0x0A000100 ) { \
  NifStream( size, out, version ); \
}; \
if ( version <= 0x04000002 ) { \
  NifStream( numActive, out, version ); \
}; \
if ( ( version >= 0x0401000C ) && ( version <= 0x0A000100 ) ) { \
  NifStream( unknownShort, out, version ); \
}; \
NifStream( hasSizes, out, version ); \
if ( hasSizes != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( sizes[i1], out, version ); \
  }; \
}; \

#define A_PARTICLES_DATA_STRING \
stringstream out; \
out << TriBasedGeomData::asString(); \
out << "Num Particles:  " << numParticles << endl; \
out << "Size:  " << size << endl; \
out << "Num Active:  " << numActive << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Has Sizes:  " << hasSizes << endl; \
if ( hasSizes != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Sizes[" << i1 << "]:  " << sizes[i1] << endl; \
  }; \
}; \
return out.str(); \

#define A_PARTICLES_DATA_FIXLINKS \
TriBasedGeomData::FixLinks( objects, link_stack, version ); \
if ( version <= 0x04000002 ) { \
}; \
if ( version <= 0x0A000100 ) { \
}; \
if ( version <= 0x04000002 ) { \
}; \
if ( ( version >= 0x0401000C ) && ( version <= 0x0A000100 ) ) { \
}; \
if ( hasSizes != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \

#define A_P_SYS_DATA_MEMBERS \
bool hasUnknownFloats1; \
vector<float > unknownFloats1; \
ushort unknownShort3; \
bool hasUnknownFloats2; \
vector<float > unknownFloats2; \
byte unknownByte1; \

#define A_P_SYS_DATA_PARENTS TriBasedGeomData \

#define A_P_SYS_DATA_CONSTRUCT \

#define A_P_SYS_DATA_READ \
TriBasedGeomData::Read( in, link_stack, version ); \
NifStream( hasUnknownFloats1, in, version ); \
if ( hasUnknownFloats1 != 0 ) { \
  unknownFloats1.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( unknownFloats1[i1], in, version ); \
  }; \
}; \
NifStream( unknownShort3, in, version ); \
NifStream( hasUnknownFloats2, in, version ); \
if ( hasUnknownFloats2 != 0 ) { \
  unknownFloats2.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( unknownFloats2[i1], in, version ); \
  }; \
}; \
NifStream( unknownByte1, in, version ); \

#define A_P_SYS_DATA_WRITE \
TriBasedGeomData::Write( out, link_map, version ); \
NifStream( hasUnknownFloats1, out, version ); \
if ( hasUnknownFloats1 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( unknownFloats1[i1], out, version ); \
  }; \
}; \
NifStream( unknownShort3, out, version ); \
NifStream( hasUnknownFloats2, out, version ); \
if ( hasUnknownFloats2 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( unknownFloats2[i1], out, version ); \
  }; \
}; \
NifStream( unknownByte1, out, version ); \

#define A_P_SYS_DATA_STRING \
stringstream out; \
out << TriBasedGeomData::asString(); \
out << "Has Unknown Floats 1:  " << hasUnknownFloats1 << endl; \
if ( hasUnknownFloats1 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Unknown Floats 1[" << i1 << "]:  " << unknownFloats1[i1] << endl; \
  }; \
}; \
out << "Unknown Short 3:  " << unknownShort3 << endl; \
out << "Has Unknown Floats 2:  " << hasUnknownFloats2 << endl; \
if ( hasUnknownFloats2 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Unknown Floats 2[" << i1 << "]:  " << unknownFloats2[i1] << endl; \
  }; \
}; \
out << "Unknown Byte 1:  " << unknownByte1 << endl; \
return out.str(); \

#define A_P_SYS_DATA_FIXLINKS \
TriBasedGeomData::FixLinks( objects, link_stack, version ); \
if ( hasUnknownFloats1 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \
if ( hasUnknownFloats2 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \

#define A_ROTATING_PARTICLES_DATA_MEMBERS \
ushort numActive; \
bool hasUnknownFloats; \
vector<float > unknownFloats; \
bool hasRotations; \
vector<Quaternion > rotations; \

#define A_ROTATING_PARTICLES_DATA_PARENTS AParticlesData \

#define A_ROTATING_PARTICLES_DATA_CONSTRUCT \

#define A_ROTATING_PARTICLES_DATA_READ \
AParticlesData::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( numActive, in, version ); \
  NifStream( hasUnknownFloats, in, version ); \
  if ( hasUnknownFloats != 0 ) { \
    unknownFloats.resize(numVertices); \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      NifStream( unknownFloats[i2], in, version ); \
    }; \
  }; \
}; \
NifStream( hasRotations, in, version ); \
if ( hasRotations != 0 ) { \
  rotations.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( rotations[i1], in, version ); \
  }; \
}; \

#define A_ROTATING_PARTICLES_DATA_WRITE \
AParticlesData::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( numActive, out, version ); \
  NifStream( hasUnknownFloats, out, version ); \
  if ( hasUnknownFloats != 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      NifStream( unknownFloats[i2], out, version ); \
    }; \
  }; \
}; \
NifStream( hasRotations, out, version ); \
if ( hasRotations != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    NifStream( rotations[i1], out, version ); \
  }; \
}; \

#define A_ROTATING_PARTICLES_DATA_STRING \
stringstream out; \
out << AParticlesData::asString(); \
out << "Num Active:  " << numActive << endl; \
out << "Has Unknown Floats:  " << hasUnknownFloats << endl; \
if ( hasUnknownFloats != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Unknown Floats[" << i1 << "]:  " << unknownFloats[i1] << endl; \
  }; \
}; \
out << "Has Rotations:  " << hasRotations << endl; \
if ( hasRotations != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    out << "    Rotations[" << i1 << "]:  " << rotations[i1] << endl; \
  }; \
}; \
return out.str(); \

#define A_ROTATING_PARTICLES_DATA_FIXLINKS \
AParticlesData::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  if ( hasUnknownFloats != 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
    }; \
  }; \
}; \
if ( hasRotations != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
  }; \
}; \

#define BHK_BLEND_COLLISION_OBJECT_MEMBERS \
float unknownFloat1; \
float unknownFloat2; \

#define BHK_BLEND_COLLISION_OBJECT_PARENTS NiCollisionObject \

#define BHK_BLEND_COLLISION_OBJECT_CONSTRUCT \

#define BHK_BLEND_COLLISION_OBJECT_READ \
NiCollisionObject::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \

#define BHK_BLEND_COLLISION_OBJECT_WRITE \
NiCollisionObject::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \

#define BHK_BLEND_COLLISION_OBJECT_STRING \
stringstream out; \
out << NiCollisionObject::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
return out.str(); \

#define BHK_BLEND_COLLISION_OBJECT_FIXLINKS \
NiCollisionObject::FixLinks( objects, link_stack, version ); \

#define BHK_BLEND_CONTROLLER_MEMBERS \
uint unknownInt; \

#define BHK_BLEND_CONTROLLER_PARENTS NiTimeController \

#define BHK_BLEND_CONTROLLER_CONSTRUCT \

#define BHK_BLEND_CONTROLLER_READ \
NiTimeController::Read( in, link_stack, version ); \
NifStream( unknownInt, in, version ); \

#define BHK_BLEND_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( unknownInt, out, version ); \

#define BHK_BLEND_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Int:  " << unknownInt << endl; \
return out.str(); \

#define BHK_BLEND_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \

#define BHK_BOX_SHAPE_MEMBERS \
float unknownFloat1; \
ushort unknownShort1; \
ushort unknownShort2; \
ushort unknownShort3; \
ushort unknownShort4; \
Vector3 unknownVector; \
float unknownFloat2; \

#define BHK_BOX_SHAPE_PARENTS bhkConvexShape \

#define BHK_BOX_SHAPE_CONSTRUCT \

#define BHK_BOX_SHAPE_READ \
bhkConvexShape::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownShort1, in, version ); \
NifStream( unknownShort2, in, version ); \
NifStream( unknownShort3, in, version ); \
NifStream( unknownShort4, in, version ); \
NifStream( unknownVector, in, version ); \
NifStream( unknownFloat2, in, version ); \

#define BHK_BOX_SHAPE_WRITE \
bhkConvexShape::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownShort1, out, version ); \
NifStream( unknownShort2, out, version ); \
NifStream( unknownShort3, out, version ); \
NifStream( unknownShort4, out, version ); \
NifStream( unknownVector, out, version ); \
NifStream( unknownFloat2, out, version ); \

#define BHK_BOX_SHAPE_STRING \
stringstream out; \
out << bhkConvexShape::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Short 1:  " << unknownShort1 << endl; \
out << "Unknown Short 2:  " << unknownShort2 << endl; \
out << "Unknown Short 3:  " << unknownShort3 << endl; \
out << "Unknown Short 4:  " << unknownShort4 << endl; \
out << "Unknown Vector:  " << unknownVector << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
return out.str(); \

#define BHK_BOX_SHAPE_FIXLINKS \
bhkConvexShape::FixLinks( objects, link_stack, version ); \

#define BHK_CAPSULE_SHAPE_MEMBERS \
float radius; \
ushort unknownShort1; \
ushort unknownShort2; \
ushort unknownShort3; \
ushort unknownShort4; \
Vector3 unknownVector1; \
float radius1; \
Vector3 unknownVector2; \
float radius2; \

#define BHK_CAPSULE_SHAPE_PARENTS bhkConvexShape \

#define BHK_CAPSULE_SHAPE_CONSTRUCT \

#define BHK_CAPSULE_SHAPE_READ \
bhkConvexShape::Read( in, link_stack, version ); \
NifStream( radius, in, version ); \
NifStream( unknownShort1, in, version ); \
NifStream( unknownShort2, in, version ); \
NifStream( unknownShort3, in, version ); \
NifStream( unknownShort4, in, version ); \
NifStream( unknownVector1, in, version ); \
NifStream( radius1, in, version ); \
NifStream( unknownVector2, in, version ); \
NifStream( radius2, in, version ); \

#define BHK_CAPSULE_SHAPE_WRITE \
bhkConvexShape::Write( out, link_map, version ); \
NifStream( radius, out, version ); \
NifStream( unknownShort1, out, version ); \
NifStream( unknownShort2, out, version ); \
NifStream( unknownShort3, out, version ); \
NifStream( unknownShort4, out, version ); \
NifStream( unknownVector1, out, version ); \
NifStream( radius1, out, version ); \
NifStream( unknownVector2, out, version ); \
NifStream( radius2, out, version ); \

#define BHK_CAPSULE_SHAPE_STRING \
stringstream out; \
out << bhkConvexShape::asString(); \
out << "Radius:  " << radius << endl; \
out << "Unknown Short 1:  " << unknownShort1 << endl; \
out << "Unknown Short 2:  " << unknownShort2 << endl; \
out << "Unknown Short 3:  " << unknownShort3 << endl; \
out << "Unknown Short 4:  " << unknownShort4 << endl; \
out << "Unknown Vector 1:  " << unknownVector1 << endl; \
out << "Radius 1:  " << radius1 << endl; \
out << "Unknown Vector 2:  " << unknownVector2 << endl; \
out << "Radius 2:  " << radius2 << endl; \
return out.str(); \

#define BHK_CAPSULE_SHAPE_FIXLINKS \
bhkConvexShape::FixLinks( objects, link_stack, version ); \

#define BHK_COLLISION_OBJECT_MEMBERS \

#define BHK_COLLISION_OBJECT_PARENTS NiCollisionObject \

#define BHK_COLLISION_OBJECT_CONSTRUCT \

#define BHK_COLLISION_OBJECT_READ \
NiCollisionObject::Read( in, link_stack, version ); \

#define BHK_COLLISION_OBJECT_WRITE \
NiCollisionObject::Write( out, link_map, version ); \

#define BHK_COLLISION_OBJECT_STRING \
stringstream out; \
out << NiCollisionObject::asString(); \
return out.str(); \

#define BHK_COLLISION_OBJECT_FIXLINKS \
NiCollisionObject::FixLinks( objects, link_stack, version ); \

#define BHK_CONVEX_TRANSFORM_SHAPE_MEMBERS \

#define BHK_CONVEX_TRANSFORM_SHAPE_PARENTS AbhkTransformShape \

#define BHK_CONVEX_TRANSFORM_SHAPE_CONSTRUCT \

#define BHK_CONVEX_TRANSFORM_SHAPE_READ \
AbhkTransformShape::Read( in, link_stack, version ); \

#define BHK_CONVEX_TRANSFORM_SHAPE_WRITE \
AbhkTransformShape::Write( out, link_map, version ); \

#define BHK_CONVEX_TRANSFORM_SHAPE_STRING \
stringstream out; \
out << AbhkTransformShape::asString(); \
return out.str(); \

#define BHK_CONVEX_TRANSFORM_SHAPE_FIXLINKS \
AbhkTransformShape::FixLinks( objects, link_stack, version ); \

#define BHK_CONVEX_VERTICES_SHAPE_MEMBERS \
vector<float > unknownFloats1; \
vector<Float4 > unknownVectors1; \
vector<Float4 > unknownVectors2; \

#define BHK_CONVEX_VERTICES_SHAPE_PARENTS bhkSphereRepShape \

#define BHK_CONVEX_VERTICES_SHAPE_CONSTRUCT \

#define BHK_CONVEX_VERTICES_SHAPE_READ \
bhkSphereRepShape::Read( in, link_stack, version ); \
uint num1; \
uint num2; \
unknownFloats1.resize(7); \
for (uint i0 = 0; i0 < 7; i0++) { \
  NifStream( unknownFloats1[i0], in, version ); \
}; \
NifStream( num1, in, version ); \
unknownVectors1.resize(num1); \
for (uint i0 = 0; i0 < num1; i0++) { \
  NifStream( unknownVectors1[i0], in, version ); \
}; \
NifStream( num2, in, version ); \
unknownVectors2.resize(num2); \
for (uint i0 = 0; i0 < num2; i0++) { \
  NifStream( unknownVectors2[i0], in, version ); \
}; \

#define BHK_CONVEX_VERTICES_SHAPE_WRITE \
bhkSphereRepShape::Write( out, link_map, version ); \
uint num1; \
num1 = uint(num1.size()); \
uint num2; \
num2 = uint(num2.size()); \
for (uint i0 = 0; i0 < 7; i0++) { \
  NifStream( unknownFloats1[i0], out, version ); \
}; \
NifStream( num1, out, version ); \
for (uint i0 = 0; i0 < num1; i0++) { \
  NifStream( unknownVectors1[i0], out, version ); \
}; \
NifStream( num2, out, version ); \
for (uint i0 = 0; i0 < num2; i0++) { \
  NifStream( unknownVectors2[i0], out, version ); \
}; \

#define BHK_CONVEX_VERTICES_SHAPE_STRING \
stringstream out; \
out << bhkSphereRepShape::asString(); \
for (uint i0 = 0; i0 < 7; i0++) { \
  out << "  Unknown Floats 1[" << i0 << "]:  " << unknownFloats1[i0] << endl; \
}; \
out << "Num 1:  " << num1 << endl; \
for (uint i0 = 0; i0 < num1; i0++) { \
  out << "  Unknown Vectors 1[" << i0 << "]:  " << unknownVectors1[i0] << endl; \
}; \
out << "Num 2:  " << num2 << endl; \
for (uint i0 = 0; i0 < num2; i0++) { \
  out << "  Unknown Vectors 2[" << i0 << "]:  " << unknownVectors2[i0] << endl; \
}; \
return out.str(); \

#define BHK_CONVEX_VERTICES_SHAPE_FIXLINKS \
bhkSphereRepShape::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 7; i0++) { \
}; \
for (uint i0 = 0; i0 < num1; i0++) { \
}; \
for (uint i0 = 0; i0 < num2; i0++) { \
}; \

#define BHK_HINGE_CONSTRAINT_MEMBERS \
vector<vector<float > > unknownFloats; \

#define BHK_HINGE_CONSTRAINT_PARENTS AbhkConstraint \

#define BHK_HINGE_CONSTRAINT_CONSTRUCT \

#define BHK_HINGE_CONSTRAINT_READ \
AbhkConstraint::Read( in, link_stack, version ); \
unknownFloats.resize(5); \
for (uint i0 = 0; i0 < 5; i0++) \
  unknownFloats[i0].resize(4); \
for (uint i0 = 0; i0 < 5; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], in, version ); \
  }; \
}; \

#define BHK_HINGE_CONSTRAINT_WRITE \
AbhkConstraint::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 5; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], out, version ); \
  }; \
}; \

#define BHK_HINGE_CONSTRAINT_STRING \
stringstream out; \
out << AbhkConstraint::asString(); \
for (uint i0 = 0; i0 < 5; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown Floats[" << i0 << "][" << i1 << "]:  " << unknownFloats[i0][i1] << endl; \
  }; \
}; \
return out.str(); \

#define BHK_HINGE_CONSTRAINT_FIXLINKS \
AbhkConstraint::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 5; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
}; \

#define BHK_LIMITED_HINGE_CONSTRAINT_MEMBERS \
uint unknownInt; \

#define BHK_LIMITED_HINGE_CONSTRAINT_PARENTS AbhkRagdollConstraint \

#define BHK_LIMITED_HINGE_CONSTRAINT_CONSTRUCT \

#define BHK_LIMITED_HINGE_CONSTRAINT_READ \
AbhkRagdollConstraint::Read( in, link_stack, version ); \
NifStream( unknownInt, in, version ); \

#define BHK_LIMITED_HINGE_CONSTRAINT_WRITE \
AbhkRagdollConstraint::Write( out, link_map, version ); \
NifStream( unknownInt, out, version ); \

#define BHK_LIMITED_HINGE_CONSTRAINT_STRING \
stringstream out; \
out << AbhkRagdollConstraint::asString(); \
out << "Unknown Int:  " << unknownInt << endl; \
return out.str(); \

#define BHK_LIMITED_HINGE_CONSTRAINT_FIXLINKS \
AbhkRagdollConstraint::FixLinks( objects, link_stack, version ); \

#define BHK_LIST_SHAPE_MEMBERS \
vector< Ref<bhkSphereRepShape > subShapes; \
uint material; \
vector<float > unknownFloats; \
vector<uint > unknownInts; \

#define BHK_LIST_SHAPE_PARENTS AbhkShapeCollection \

#define BHK_LIST_SHAPE_CONSTRUCT \

#define BHK_LIST_SHAPE_READ \
uint block_num; \
AbhkShapeCollection::Read( in, link_stack, version ); \
uint numUnknownInts; \
uint subShapes_numIndices; \
NifStream( subShapes_numIndices, in, version ); \
subShapes.indices.resize(subShapes_numIndices); \
for (uint i0 = 0; i0 < subShapes_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( material, in, version ); \
unknownFloats.resize(6); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \
NifStream( numUnknownInts, in, version ); \
unknownInts.resize(numUnknownInts); \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  NifStream( unknownInts[i0], in, version ); \
}; \

#define BHK_LIST_SHAPE_WRITE \
AbhkShapeCollection::Write( out, link_map, version ); \
uint numUnknownInts; \
numUnknownInts = uint(numUnknownInts.size()); \
uint subShapes_numIndices; \
subShapes_numIndices = uint(subShapes.numIndices.size()); \
NifStream( subShapes_numIndices, out, version ); \
for (uint i0 = 0; i0 < subShapes_numIndices; i0++) { \
  NifStream( link_map[subShapes.indices[i0]], out, version ); \
}; \
NifStream( material, out, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \
NifStream( numUnknownInts, out, version ); \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  NifStream( unknownInts[i0], out, version ); \
}; \

#define BHK_LIST_SHAPE_STRING \
stringstream out; \
out << AbhkShapeCollection::asString(); \
out << "Num Indices:  " << subShapes_numIndices << endl; \
for (uint i0 = 0; i0 < subShapes_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << subShapes.indices[i0] << endl; \
}; \
out << "Material:  " << material << endl; \
for (uint i0 = 0; i0 < 6; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
out << "Num Unknown Ints:  " << numUnknownInts << endl; \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  out << "  Unknown Ints[" << i0 << "]:  " << unknownInts[i0] << endl; \
}; \
return out.str(); \

#define BHK_LIST_SHAPE_FIXLINKS \
AbhkShapeCollection::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < subShapes_numIndices; i0++) { \
  subShapes.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
for (uint i0 = 0; i0 < 6; i0++) { \
}; \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
}; \

#define BHK_MALLEABLE_CONSTRAINT_MEMBERS \
uint type; \
uint unknownInt2; \
Ref<NiObject > unknownLink1; \
Ref<NiObject > unknownLink2; \
uint unknownInt3; \
vector<float > unknownFloats1; \
vector<vector<float > > unknownFloats; \
float unknownFloat1; \
float unknownFloat2; \

#define BHK_MALLEABLE_CONSTRAINT_PARENTS AbhkConstraint \

#define BHK_MALLEABLE_CONSTRAINT_CONSTRUCT \

#define BHK_MALLEABLE_CONSTRAINT_READ \
uint block_num; \
AbhkConstraint::Read( in, link_stack, version ); \
NifStream( type, in, version ); \
NifStream( unknownInt2, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknownInt3, in, version ); \
unknownFloats1.resize(3); \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats1[i0], in, version ); \
}; \
unknownFloats.resize(7); \
for (uint i0 = 0; i0 < 7; i0++) \
  unknownFloats[i0].resize(4); \
for (uint i0 = 0; i0 < 7; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], in, version ); \
  }; \
}; \
NifStream( unknownFloat1, in, version ); \
if ( type == 2 ) { \
  NifStream( unknownFloat2, in, version ); \
}; \

#define BHK_MALLEABLE_CONSTRAINT_WRITE \
AbhkConstraint::Write( out, link_map, version ); \
NifStream( type, out, version ); \
NifStream( unknownInt2, out, version ); \
NifStream( link_map[unknownLink1], out, version ); \
NifStream( link_map[unknownLink2], out, version ); \
NifStream( unknownInt3, out, version ); \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats1[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 7; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], out, version ); \
  }; \
}; \
NifStream( unknownFloat1, out, version ); \
if ( type == 2 ) { \
  NifStream( unknownFloat2, out, version ); \
}; \

#define BHK_MALLEABLE_CONSTRAINT_STRING \
stringstream out; \
out << AbhkConstraint::asString(); \
out << "Type:  " << type << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
out << "Unknown Link 1:  " << unknownLink1 << endl; \
out << "Unknown Link 2:  " << unknownLink2 << endl; \
out << "Unknown Int 3:  " << unknownInt3 << endl; \
for (uint i0 = 0; i0 < 3; i0++) { \
  out << "  Unknown Floats 1[" << i0 << "]:  " << unknownFloats1[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 7; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown Floats[" << i0 << "][" << i1 << "]:  " << unknownFloats[i0][i1] << endl; \
  }; \
}; \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
if ( type == 2 ) { \
  out << "  Unknown Float 2:  " << unknownFloat2 << endl; \
}; \
return out.str(); \

#define BHK_MALLEABLE_CONSTRAINT_FIXLINKS \
AbhkConstraint::FixLinks( objects, link_stack, version ); \
unknownLink1 = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink2 = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < 3; i0++) { \
}; \
for (uint i0 = 0; i0 < 7; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
}; \
if ( type == 2 ) { \
}; \

#define BHK_MOPP_BV_TREE_SHAPE_MEMBERS \
vector<byte > unknownBytes1; \
float unknownFloat; \
vector<byte > unknownBytes2; \
vector<float > unknownFloats; \

#define BHK_MOPP_BV_TREE_SHAPE_PARENTS bhkEntity \

#define BHK_MOPP_BV_TREE_SHAPE_CONSTRUCT \

#define BHK_MOPP_BV_TREE_SHAPE_READ \
bhkEntity::Read( in, link_stack, version ); \
uint numUnknownBytes2; \
unknownBytes1.resize(8); \
for (uint i0 = 0; i0 < 8; i0++) { \
  NifStream( unknownBytes1[i0], in, version ); \
}; \
NifStream( unknownFloat, in, version ); \
NifStream( numUnknownBytes2, in, version ); \
unknownBytes2.resize(numUnknownBytes2); \
for (uint i0 = 0; i0 < numUnknownBytes2; i0++) { \
  NifStream( unknownBytes2[i0], in, version ); \
}; \
unknownFloats.resize(4); \
for (uint i0 = 0; i0 < 4; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \

#define BHK_MOPP_BV_TREE_SHAPE_WRITE \
bhkEntity::Write( out, link_map, version ); \
uint numUnknownBytes2; \
numUnknownBytes2 = uint(numUnknownBytes2.size()); \
for (uint i0 = 0; i0 < 8; i0++) { \
  NifStream( unknownBytes1[i0], out, version ); \
}; \
NifStream( unknownFloat, out, version ); \
NifStream( numUnknownBytes2, out, version ); \
for (uint i0 = 0; i0 < numUnknownBytes2; i0++) { \
  NifStream( unknownBytes2[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 4; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \

#define BHK_MOPP_BV_TREE_SHAPE_STRING \
stringstream out; \
out << bhkEntity::asString(); \
for (uint i0 = 0; i0 < 8; i0++) { \
  out << "  Unknown Bytes 1[" << i0 << "]:  " << unknownBytes1[i0] << endl; \
}; \
out << "Unknown Float:  " << unknownFloat << endl; \
out << "Num Unknown Bytes 2:  " << numUnknownBytes2 << endl; \
for (uint i0 = 0; i0 < numUnknownBytes2; i0++) { \
  out << "  Unknown Bytes 2[" << i0 << "]:  " << unknownBytes2[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 4; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
return out.str(); \

#define BHK_MOPP_BV_TREE_SHAPE_FIXLINKS \
bhkEntity::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 8; i0++) { \
}; \
for (uint i0 = 0; i0 < numUnknownBytes2; i0++) { \
}; \
for (uint i0 = 0; i0 < 4; i0++) { \
}; \

#define BHK_MULTI_SPHERE_SHAPE_MEMBERS \
float unknownFloat1; \
float unknownFloat2; \
float unknownFloat3; \
uint unknownInt2; \
vector<float > unknownFloats; \

#define BHK_MULTI_SPHERE_SHAPE_PARENTS bhkSphereRepShape \

#define BHK_MULTI_SPHERE_SHAPE_CONSTRUCT \

#define BHK_MULTI_SPHERE_SHAPE_READ \
bhkSphereRepShape::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \
NifStream( unknownFloat3, in, version ); \
NifStream( unknownInt2, in, version ); \
unknownFloats.resize(8); \
for (uint i0 = 0; i0 < 8; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \

#define BHK_MULTI_SPHERE_SHAPE_WRITE \
bhkSphereRepShape::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \
NifStream( unknownFloat3, out, version ); \
NifStream( unknownInt2, out, version ); \
for (uint i0 = 0; i0 < 8; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \

#define BHK_MULTI_SPHERE_SHAPE_STRING \
stringstream out; \
out << bhkSphereRepShape::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Float 3:  " << unknownFloat3 << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
for (uint i0 = 0; i0 < 8; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
return out.str(); \

#define BHK_MULTI_SPHERE_SHAPE_FIXLINKS \
bhkSphereRepShape::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 8; i0++) { \
}; \

#define BHK_NI_TRI_STRIPS_SHAPE_MEMBERS \
vector<float > unknownFloats1; \
vector<uint > unknownInts1; \
vector<float > unknownFloats2; \
uint unknownInt2; \
vector< Ref<NiTriStripsData > strips; \
vector<uint > unknownInts3; \

#define BHK_NI_TRI_STRIPS_SHAPE_PARENTS bhkSphereRepShape \

#define BHK_NI_TRI_STRIPS_SHAPE_CONSTRUCT \

#define BHK_NI_TRI_STRIPS_SHAPE_READ \
uint block_num; \
bhkSphereRepShape::Read( in, link_stack, version ); \
uint numUnknownInts3; \
unknownFloats1.resize(2); \
for (uint i0 = 0; i0 < 2; i0++) { \
  NifStream( unknownFloats1[i0], in, version ); \
}; \
unknownInts1.resize(5); \
for (uint i0 = 0; i0 < 5; i0++) { \
  NifStream( unknownInts1[i0], in, version ); \
}; \
unknownFloats2.resize(3); \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats2[i0], in, version ); \
}; \
NifStream( unknownInt2, in, version ); \
uint strips_numIndices; \
NifStream( strips_numIndices, in, version ); \
strips.indices.resize(strips_numIndices); \
for (uint i0 = 0; i0 < strips_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( numUnknownInts3, in, version ); \
unknownInts3.resize(numUnknownInts3); \
for (uint i0 = 0; i0 < numUnknownInts3; i0++) { \
  NifStream( unknownInts3[i0], in, version ); \
}; \

#define BHK_NI_TRI_STRIPS_SHAPE_WRITE \
bhkSphereRepShape::Write( out, link_map, version ); \
uint numUnknownInts3; \
numUnknownInts3 = uint(numUnknownInts3.size()); \
for (uint i0 = 0; i0 < 2; i0++) { \
  NifStream( unknownFloats1[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 5; i0++) { \
  NifStream( unknownInts1[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats2[i0], out, version ); \
}; \
NifStream( unknownInt2, out, version ); \
uint strips_numIndices; \
strips_numIndices = uint(strips.numIndices.size()); \
NifStream( strips_numIndices, out, version ); \
for (uint i0 = 0; i0 < strips_numIndices; i0++) { \
  NifStream( link_map[strips.indices[i0]], out, version ); \
}; \
NifStream( numUnknownInts3, out, version ); \
for (uint i0 = 0; i0 < numUnknownInts3; i0++) { \
  NifStream( unknownInts3[i0], out, version ); \
}; \

#define BHK_NI_TRI_STRIPS_SHAPE_STRING \
stringstream out; \
out << bhkSphereRepShape::asString(); \
for (uint i0 = 0; i0 < 2; i0++) { \
  out << "  Unknown Floats 1[" << i0 << "]:  " << unknownFloats1[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 5; i0++) { \
  out << "  Unknown Ints 1[" << i0 << "]:  " << unknownInts1[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  out << "  Unknown Floats 2[" << i0 << "]:  " << unknownFloats2[i0] << endl; \
}; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
out << "Num Indices:  " << strips_numIndices << endl; \
for (uint i0 = 0; i0 < strips_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << strips.indices[i0] << endl; \
}; \
out << "Num Unknown Ints 3:  " << numUnknownInts3 << endl; \
for (uint i0 = 0; i0 < numUnknownInts3; i0++) { \
  out << "  Unknown Ints 3[" << i0 << "]:  " << unknownInts3[i0] << endl; \
}; \
return out.str(); \

#define BHK_NI_TRI_STRIPS_SHAPE_FIXLINKS \
bhkSphereRepShape::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 2; i0++) { \
}; \
for (uint i0 = 0; i0 < 5; i0++) { \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
}; \
for (uint i0 = 0; i0 < strips_numIndices; i0++) { \
  strips.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
for (uint i0 = 0; i0 < numUnknownInts3; i0++) { \
}; \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_MEMBERS \
vector<vector<uint > > unknownInts; \
vector<float > unknownFloats; \
Ref<NiTriStripsData > data; \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_PARENTS AbhkShapeCollection \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_CONSTRUCT \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_READ \
uint block_num; \
AbhkShapeCollection::Read( in, link_stack, version ); \
ushort numUnknownInts; \
NifStream( numUnknownInts, in, version ); \
unknownInts.resize(numUnknownInts); \
for (uint i0 = 0; i0 < numUnknownInts; i0++) \
  unknownInts[i0].resize(3); \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  for (uint i1 = 0; i1 < 3; i1++) { \
    NifStream( unknownInts[i0][i1], in, version ); \
  }; \
}; \
unknownFloats.resize(13); \
for (uint i0 = 0; i0 < 13; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_WRITE \
AbhkShapeCollection::Write( out, link_map, version ); \
ushort numUnknownInts; \
numUnknownInts = ushort(numUnknownInts.size()); \
NifStream( numUnknownInts, out, version ); \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  for (uint i1 = 0; i1 < 3; i1++) { \
    NifStream( unknownInts[i0][i1], out, version ); \
  }; \
}; \
for (uint i0 = 0; i0 < 13; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \
NifStream( link_map[data], out, version ); \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_STRING \
stringstream out; \
out << AbhkShapeCollection::asString(); \
out << "Num Unknown Ints:  " << numUnknownInts << endl; \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  for (uint i1 = 0; i1 < 3; i1++) { \
    out << "    Unknown Ints[" << i0 << "][" << i1 << "]:  " << unknownInts[i0][i1] << endl; \
  }; \
}; \
for (uint i0 = 0; i0 < 13; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
out << "Data:  " << data << endl; \
return out.str(); \

#define BHK_PACKED_NI_TRI_STRIPS_SHAPE_FIXLINKS \
AbhkShapeCollection::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  for (uint i1 = 0; i1 < 3; i1++) { \
  }; \
}; \
for (uint i0 = 0; i0 < 13; i0++) { \
}; \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define BHK_PRISMATIC_CONSTRAINT_MEMBERS \
vector<vector<float > > unknownFloats; \
vector<float > unknownFloats2; \

#define BHK_PRISMATIC_CONSTRAINT_PARENTS AbhkConstraint \

#define BHK_PRISMATIC_CONSTRAINT_CONSTRUCT \

#define BHK_PRISMATIC_CONSTRAINT_READ \
AbhkConstraint::Read( in, link_stack, version ); \
unknownFloats.resize(8); \
for (uint i0 = 0; i0 < 8; i0++) \
  unknownFloats[i0].resize(4); \
for (uint i0 = 0; i0 < 8; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], in, version ); \
  }; \
}; \
unknownFloats2.resize(3); \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats2[i0], in, version ); \
}; \

#define BHK_PRISMATIC_CONSTRAINT_WRITE \
AbhkConstraint::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 8; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], out, version ); \
  }; \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats2[i0], out, version ); \
}; \

#define BHK_PRISMATIC_CONSTRAINT_STRING \
stringstream out; \
out << AbhkConstraint::asString(); \
for (uint i0 = 0; i0 < 8; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown Floats[" << i0 << "][" << i1 << "]:  " << unknownFloats[i0][i1] << endl; \
  }; \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  out << "  Unknown Floats 2[" << i0 << "]:  " << unknownFloats2[i0] << endl; \
}; \
return out.str(); \

#define BHK_PRISMATIC_CONSTRAINT_FIXLINKS \
AbhkConstraint::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 8; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
}; \

#define BHK_RAGDOLL_CONSTRAINT_MEMBERS \

#define BHK_RAGDOLL_CONSTRAINT_PARENTS AbhkRagdollConstraint \

#define BHK_RAGDOLL_CONSTRAINT_CONSTRUCT \

#define BHK_RAGDOLL_CONSTRAINT_READ \
AbhkRagdollConstraint::Read( in, link_stack, version ); \

#define BHK_RAGDOLL_CONSTRAINT_WRITE \
AbhkRagdollConstraint::Write( out, link_map, version ); \

#define BHK_RAGDOLL_CONSTRAINT_STRING \
stringstream out; \
out << AbhkRagdollConstraint::asString(); \
return out.str(); \

#define BHK_RAGDOLL_CONSTRAINT_FIXLINKS \
AbhkRagdollConstraint::FixLinks( objects, link_stack, version ); \

#define BHK_RIGID_BODY_MEMBERS \

#define BHK_RIGID_BODY_PARENTS AbhkRigidBody \

#define BHK_RIGID_BODY_CONSTRUCT \

#define BHK_RIGID_BODY_READ \
AbhkRigidBody::Read( in, link_stack, version ); \

#define BHK_RIGID_BODY_WRITE \
AbhkRigidBody::Write( out, link_map, version ); \

#define BHK_RIGID_BODY_STRING \
stringstream out; \
out << AbhkRigidBody::asString(); \
return out.str(); \

#define BHK_RIGID_BODY_FIXLINKS \
AbhkRigidBody::FixLinks( objects, link_stack, version ); \

#define BHK_RIGID_BODY_T_MEMBERS \

#define BHK_RIGID_BODY_T_PARENTS AbhkRigidBody \

#define BHK_RIGID_BODY_T_CONSTRUCT \

#define BHK_RIGID_BODY_T_READ \
AbhkRigidBody::Read( in, link_stack, version ); \

#define BHK_RIGID_BODY_T_WRITE \
AbhkRigidBody::Write( out, link_map, version ); \

#define BHK_RIGID_BODY_T_STRING \
stringstream out; \
out << AbhkRigidBody::asString(); \
return out.str(); \

#define BHK_RIGID_BODY_T_FIXLINKS \
AbhkRigidBody::FixLinks( objects, link_stack, version ); \

#define BHK_SIMPLE_SHAPE_PHANTOM_MEMBERS \
vector<float > unkownFloats; \
vector<vector<float > > unknownFloats2; \
float unknownFloat; \

#define BHK_SIMPLE_SHAPE_PHANTOM_PARENTS bhkEntity \

#define BHK_SIMPLE_SHAPE_PHANTOM_CONSTRUCT \

#define BHK_SIMPLE_SHAPE_PHANTOM_READ \
bhkEntity::Read( in, link_stack, version ); \
unkownFloats.resize(7); \
for (uint i0 = 0; i0 < 7; i0++) { \
  NifStream( unkownFloats[i0], in, version ); \
}; \
unknownFloats2.resize(3); \
for (uint i0 = 0; i0 < 3; i0++) \
  unknownFloats2[i0].resize(5); \
for (uint i0 = 0; i0 < 3; i0++) { \
  for (uint i1 = 0; i1 < 5; i1++) { \
    NifStream( unknownFloats2[i0][i1], in, version ); \
  }; \
}; \
NifStream( unknownFloat, in, version ); \

#define BHK_SIMPLE_SHAPE_PHANTOM_WRITE \
bhkEntity::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 7; i0++) { \
  NifStream( unkownFloats[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  for (uint i1 = 0; i1 < 5; i1++) { \
    NifStream( unknownFloats2[i0][i1], out, version ); \
  }; \
}; \
NifStream( unknownFloat, out, version ); \

#define BHK_SIMPLE_SHAPE_PHANTOM_STRING \
stringstream out; \
out << bhkEntity::asString(); \
for (uint i0 = 0; i0 < 7; i0++) { \
  out << "  Unkown Floats[" << i0 << "]:  " << unkownFloats[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  for (uint i1 = 0; i1 < 5; i1++) { \
    out << "    Unknown Floats 2[" << i0 << "][" << i1 << "]:  " << unknownFloats2[i0][i1] << endl; \
  }; \
}; \
out << "Unknown Float:  " << unknownFloat << endl; \
return out.str(); \

#define BHK_SIMPLE_SHAPE_PHANTOM_FIXLINKS \
bhkEntity::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 7; i0++) { \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  for (uint i1 = 0; i1 < 5; i1++) { \
  }; \
}; \

#define BHK_S_P_COLLISION_OBJECT_MEMBERS \

#define BHK_S_P_COLLISION_OBJECT_PARENTS NiCollisionObject \

#define BHK_S_P_COLLISION_OBJECT_CONSTRUCT \

#define BHK_S_P_COLLISION_OBJECT_READ \
NiCollisionObject::Read( in, link_stack, version ); \

#define BHK_S_P_COLLISION_OBJECT_WRITE \
NiCollisionObject::Write( out, link_map, version ); \

#define BHK_S_P_COLLISION_OBJECT_STRING \
stringstream out; \
out << NiCollisionObject::asString(); \
return out.str(); \

#define BHK_S_P_COLLISION_OBJECT_FIXLINKS \
NiCollisionObject::FixLinks( objects, link_stack, version ); \

#define BHK_SPHERE_SHAPE_MEMBERS \
float radius; \

#define BHK_SPHERE_SHAPE_PARENTS bhkConvexShape \

#define BHK_SPHERE_SHAPE_CONSTRUCT \

#define BHK_SPHERE_SHAPE_READ \
bhkConvexShape::Read( in, link_stack, version ); \
NifStream( radius, in, version ); \

#define BHK_SPHERE_SHAPE_WRITE \
bhkConvexShape::Write( out, link_map, version ); \
NifStream( radius, out, version ); \

#define BHK_SPHERE_SHAPE_STRING \
stringstream out; \
out << bhkConvexShape::asString(); \
out << "Radius:  " << radius << endl; \
return out.str(); \

#define BHK_SPHERE_SHAPE_FIXLINKS \
bhkConvexShape::FixLinks( objects, link_stack, version ); \

#define BHK_STIFF_SPRING_CONSTRAINT_MEMBERS \
vector<vector<float > > unknownFloats; \
float unknownFloat; \

#define BHK_STIFF_SPRING_CONSTRAINT_PARENTS AbhkConstraint \

#define BHK_STIFF_SPRING_CONSTRAINT_CONSTRUCT \

#define BHK_STIFF_SPRING_CONSTRAINT_READ \
AbhkConstraint::Read( in, link_stack, version ); \
unknownFloats.resize(2); \
for (uint i0 = 0; i0 < 2; i0++) \
  unknownFloats[i0].resize(4); \
for (uint i0 = 0; i0 < 2; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], in, version ); \
  }; \
}; \
NifStream( unknownFloat, in, version ); \

#define BHK_STIFF_SPRING_CONSTRAINT_WRITE \
AbhkConstraint::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 2; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknownFloats[i0][i1], out, version ); \
  }; \
}; \
NifStream( unknownFloat, out, version ); \

#define BHK_STIFF_SPRING_CONSTRAINT_STRING \
stringstream out; \
out << AbhkConstraint::asString(); \
for (uint i0 = 0; i0 < 2; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown Floats[" << i0 << "][" << i1 << "]:  " << unknownFloats[i0][i1] << endl; \
  }; \
}; \
out << "Unknown Float:  " << unknownFloat << endl; \
return out.str(); \

#define BHK_STIFF_SPRING_CONSTRAINT_FIXLINKS \
AbhkConstraint::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 2; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
}; \

#define BHK_TRANSFORM_SHAPE_MEMBERS \

#define BHK_TRANSFORM_SHAPE_PARENTS AbhkTransformShape \

#define BHK_TRANSFORM_SHAPE_CONSTRUCT \

#define BHK_TRANSFORM_SHAPE_READ \
AbhkTransformShape::Read( in, link_stack, version ); \

#define BHK_TRANSFORM_SHAPE_WRITE \
AbhkTransformShape::Write( out, link_map, version ); \

#define BHK_TRANSFORM_SHAPE_STRING \
stringstream out; \
out << AbhkTransformShape::asString(); \
return out.str(); \

#define BHK_TRANSFORM_SHAPE_FIXLINKS \
AbhkTransformShape::FixLinks( objects, link_stack, version ); \

#define B_S_BOUND_MEMBERS \
string name; \
vector<float > unknownFloats; \

#define B_S_BOUND_PARENTS NiObject \

#define B_S_BOUND_CONSTRUCT \

#define B_S_BOUND_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( name, in, version ); \
unknownFloats.resize(6); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \

#define B_S_BOUND_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( name, out, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \

#define B_S_BOUND_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
for (uint i0 = 0; i0 < 6; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
return out.str(); \

#define B_S_BOUND_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
}; \

#define B_S_FURNITURE_MARKER_MEMBERS \
string name; \
vector<FurniturePosition > positions; \

#define B_S_FURNITURE_MARKER_PARENTS NiObject \

#define B_S_FURNITURE_MARKER_CONSTRUCT \

#define B_S_FURNITURE_MARKER_READ \
NiObject::Read( in, link_stack, version ); \
uint numPositions; \
if ( version <= 0x14000005 ) { \
  NifStream( name, in, version ); \
}; \
NifStream( numPositions, in, version ); \
positions.resize(numPositions); \
for (uint i0 = 0; i0 < numPositions; i0++) { \
  NifStream( positions[i0].unknownVector, in, version ); \
  NifStream( positions[i0].unknownShort, in, version ); \
  NifStream( positions[i0].positionRef1_, in, version ); \
  NifStream( positions[i0].positionRef2_, in, version ); \
}; \

#define B_S_FURNITURE_MARKER_WRITE \
NiObject::Write( out, link_map, version ); \
uint numPositions; \
numPositions = uint(numPositions.size()); \
if ( version <= 0x14000005 ) { \
  NifStream( name, out, version ); \
}; \
NifStream( numPositions, out, version ); \
for (uint i0 = 0; i0 < numPositions; i0++) { \
  NifStream( positions[i0].unknownVector, out, version ); \
  NifStream( positions[i0].unknownShort, out, version ); \
  NifStream( positions[i0].positionRef1_, out, version ); \
  NifStream( positions[i0].positionRef2_, out, version ); \
}; \

#define B_S_FURNITURE_MARKER_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Num Positions:  " << numPositions << endl; \
for (uint i0 = 0; i0 < numPositions; i0++) { \
  out << "  Unknown Vector:  " << positions[i0].unknownVector << endl; \
  out << "  Unknown Short:  " << positions[i0].unknownShort << endl; \
  out << "  Position Ref 1?:  " << positions[i0].positionRef1_ << endl; \
  out << "  Position Ref 2?:  " << positions[i0].positionRef2_ << endl; \
}; \
return out.str(); \

#define B_S_FURNITURE_MARKER_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version <= 0x14000005 ) { \
}; \
for (uint i0 = 0; i0 < numPositions; i0++) { \
}; \

#define B_S_KEYFRAME_CONTROLLER_MEMBERS \
Ref<NiKeyframeData > data2; \

#define B_S_KEYFRAME_CONTROLLER_PARENTS AKeyframeController \

#define B_S_KEYFRAME_CONTROLLER_CONSTRUCT \

#define B_S_KEYFRAME_CONTROLLER_READ \
uint block_num; \
AKeyframeController::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define B_S_KEYFRAME_CONTROLLER_WRITE \
AKeyframeController::Write( out, link_map, version ); \
NifStream( link_map[data2], out, version ); \

#define B_S_KEYFRAME_CONTROLLER_STRING \
stringstream out; \
out << AKeyframeController::asString(); \
out << "Data 2:  " << data2 << endl; \
return out.str(); \

#define B_S_KEYFRAME_CONTROLLER_FIXLINKS \
AKeyframeController::FixLinks( objects, link_stack, version ); \
data2 = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define B_S_PARENT_VELOCITY_MODIFIER_MEMBERS \
float unknownFloat; \

#define B_S_PARENT_VELOCITY_MODIFIER_PARENTS APSysModifier \

#define B_S_PARENT_VELOCITY_MODIFIER_CONSTRUCT \

#define B_S_PARENT_VELOCITY_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( unknownFloat, in, version ); \

#define B_S_PARENT_VELOCITY_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( unknownFloat, out, version ); \

#define B_S_PARENT_VELOCITY_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Unknown Float:  " << unknownFloat << endl; \
return out.str(); \

#define B_S_PARENT_VELOCITY_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define B_S_P_SYS_ARRAY_EMITTER_MEMBERS \

#define B_S_P_SYS_ARRAY_EMITTER_PARENTS APSysVolumeEmitter \

#define B_S_P_SYS_ARRAY_EMITTER_CONSTRUCT \

#define B_S_P_SYS_ARRAY_EMITTER_READ \
APSysVolumeEmitter::Read( in, link_stack, version ); \

#define B_S_P_SYS_ARRAY_EMITTER_WRITE \
APSysVolumeEmitter::Write( out, link_map, version ); \

#define B_S_P_SYS_ARRAY_EMITTER_STRING \
stringstream out; \
out << APSysVolumeEmitter::asString(); \
return out.str(); \

#define B_S_P_SYS_ARRAY_EMITTER_FIXLINKS \
APSysVolumeEmitter::FixLinks( objects, link_stack, version ); \

#define B_S_X_FLAGS_MEMBERS \
string name; \
uint flags; \

#define B_S_X_FLAGS_PARENTS NiObject \

#define B_S_X_FLAGS_CONSTRUCT \
 : name("BSX") \

#define B_S_X_FLAGS_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( name, in, version ); \
NifStream( flags, in, version ); \

#define B_S_X_FLAGS_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( name, out, version ); \
NifStream( flags, out, version ); \

#define B_S_X_FLAGS_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Flags:  " << flags << endl; \
return out.str(); \

#define B_S_X_FLAGS_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \

#define HK_PACKED_NI_TRI_STRIPS_DATA_MEMBERS \
vector<hkTriangle > triangles; \
vector<Vector3 > vertices; \

#define HK_PACKED_NI_TRI_STRIPS_DATA_PARENTS AbhkShapeCollection \

#define HK_PACKED_NI_TRI_STRIPS_DATA_CONSTRUCT \

#define HK_PACKED_NI_TRI_STRIPS_DATA_READ \
AbhkShapeCollection::Read( in, link_stack, version ); \
uint numTriangles; \
uint numVertices; \
NifStream( numTriangles, in, version ); \
triangles.resize(numTriangles); \
for (uint i0 = 0; i0 < numTriangles; i0++) { \
  NifStream( triangles[i0].triangle, in, version ); \
  NifStream( triangles[i0].unknownShort, in, version ); \
  NifStream( triangles[i0].normal, in, version ); \
}; \
NifStream( numVertices, in, version ); \
vertices.resize(numVertices); \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  NifStream( vertices[i0], in, version ); \
}; \

#define HK_PACKED_NI_TRI_STRIPS_DATA_WRITE \
AbhkShapeCollection::Write( out, link_map, version ); \
uint numTriangles; \
numTriangles = uint(numTriangles.size()); \
uint numVertices; \
numVertices = uint(numVertices.size()); \
NifStream( numTriangles, out, version ); \
for (uint i0 = 0; i0 < numTriangles; i0++) { \
  NifStream( triangles[i0].triangle, out, version ); \
  NifStream( triangles[i0].unknownShort, out, version ); \
  NifStream( triangles[i0].normal, out, version ); \
}; \
NifStream( numVertices, out, version ); \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  NifStream( vertices[i0], out, version ); \
}; \

#define HK_PACKED_NI_TRI_STRIPS_DATA_STRING \
stringstream out; \
out << AbhkShapeCollection::asString(); \
out << "Num Triangles:  " << numTriangles << endl; \
for (uint i0 = 0; i0 < numTriangles; i0++) { \
  out << "  Triangle:  " << triangles[i0].triangle << endl; \
  out << "  Unknown Short:  " << triangles[i0].unknownShort << endl; \
  out << "  Normal:  " << triangles[i0].normal << endl; \
}; \
out << "Num Vertices:  " << numVertices << endl; \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  out << "  Vertices[" << i0 << "]:  " << vertices[i0] << endl; \
}; \
return out.str(); \

#define HK_PACKED_NI_TRI_STRIPS_DATA_FIXLINKS \
AbhkShapeCollection::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numTriangles; i0++) { \
}; \
for (uint i0 = 0; i0 < numVertices; i0++) { \
}; \

#define NI_ALPHA_CONTROLLER_MEMBERS \
Ref<NiFloatData > data; \

#define NI_ALPHA_CONTROLLER_PARENTS ASingleInterpolatorController \

#define NI_ALPHA_CONTROLLER_CONSTRUCT \

#define NI_ALPHA_CONTROLLER_READ \
uint block_num; \
ASingleInterpolatorController::Read( in, link_stack, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_ALPHA_CONTROLLER_WRITE \
ASingleInterpolatorController::Write( out, link_map, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( link_map[data], out, version ); \
}; \

#define NI_ALPHA_CONTROLLER_STRING \
stringstream out; \
out << ASingleInterpolatorController::asString(); \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_ALPHA_CONTROLLER_FIXLINKS \
ASingleInterpolatorController::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A010000 ) { \
  data = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_ALPHA_PROPERTY_MEMBERS \
Flags flags; \
byte threshold; \

#define NI_ALPHA_PROPERTY_PARENTS NiProperty \

#define NI_ALPHA_PROPERTY_CONSTRUCT \
 : flags(237) \

#define NI_ALPHA_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \
NifStream( threshold, in, version ); \

#define NI_ALPHA_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \
NifStream( threshold, out, version ); \

#define NI_ALPHA_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Threshold:  " << threshold << endl; \
return out.str(); \

#define NI_ALPHA_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_AMBIENT_LIGHT_MEMBERS \

#define NI_AMBIENT_LIGHT_PARENTS NiLight \

#define NI_AMBIENT_LIGHT_CONSTRUCT \

#define NI_AMBIENT_LIGHT_READ \
NiLight::Read( in, link_stack, version ); \

#define NI_AMBIENT_LIGHT_WRITE \
NiLight::Write( out, link_map, version ); \

#define NI_AMBIENT_LIGHT_STRING \
stringstream out; \
out << NiLight::asString(); \
return out.str(); \

#define NI_AMBIENT_LIGHT_FIXLINKS \
NiLight::FixLinks( objects, link_stack, version ); \

#define NI_AUTO_NORMAL_PARTICLES_DATA_MEMBERS \

#define NI_AUTO_NORMAL_PARTICLES_DATA_PARENTS AParticlesData \

#define NI_AUTO_NORMAL_PARTICLES_DATA_CONSTRUCT \

#define NI_AUTO_NORMAL_PARTICLES_DATA_READ \
AParticlesData::Read( in, link_stack, version ); \

#define NI_AUTO_NORMAL_PARTICLES_DATA_WRITE \
AParticlesData::Write( out, link_map, version ); \

#define NI_AUTO_NORMAL_PARTICLES_DATA_STRING \
stringstream out; \
out << AParticlesData::asString(); \
return out.str(); \

#define NI_AUTO_NORMAL_PARTICLES_DATA_FIXLINKS \
AParticlesData::FixLinks( objects, link_stack, version ); \

#define NI_BINARY_EXTRA_DATA_MEMBERS \
ByteArray binaryData; \

#define NI_BINARY_EXTRA_DATA_PARENTS NiExtraData \

#define NI_BINARY_EXTRA_DATA_CONSTRUCT \

#define NI_BINARY_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
uint binaryData_size; \
NifStream( binaryData_size, in, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( binaryData.unknownInt, in, version ); \
}; \
binaryData.data.resize(binaryData_size); \
for (uint i0 = 0; i0 < binaryData_size; i0++) { \
  NifStream( binaryData.data[i0], in, version ); \
}; \

#define NI_BINARY_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
uint binaryData_size; \
binaryData_size = uint(binaryData.size.size()); \
NifStream( binaryData_size, out, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( binaryData.unknownInt, out, version ); \
}; \
for (uint i0 = 0; i0 < binaryData_size; i0++) { \
  NifStream( binaryData.data[i0], out, version ); \
}; \

#define NI_BINARY_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Size:  " << binaryData_size << endl; \
out << "Unknown Int:  " << binaryData.unknownInt << endl; \
for (uint i0 = 0; i0 < binaryData_size; i0++) { \
  out << "  Data[" << i0 << "]:  " << binaryData.data[i0] << endl; \
}; \
return out.str(); \

#define NI_BINARY_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
if ( version >= 0x14000004 ) { \
}; \
for (uint i0 = 0; i0 < binaryData_size; i0++) { \
}; \

#define NI_BLEND_BOOL_INTERPOLATOR_MEMBERS \
byte bool; \

#define NI_BLEND_BOOL_INTERPOLATOR_PARENTS ABlendInterpolator \

#define NI_BLEND_BOOL_INTERPOLATOR_CONSTRUCT \

#define NI_BLEND_BOOL_INTERPOLATOR_READ \
ABlendInterpolator::Read( in, link_stack, version ); \
NifStream( bool, in, version ); \

#define NI_BLEND_BOOL_INTERPOLATOR_WRITE \
ABlendInterpolator::Write( out, link_map, version ); \
NifStream( bool, out, version ); \

#define NI_BLEND_BOOL_INTERPOLATOR_STRING \
stringstream out; \
out << ABlendInterpolator::asString(); \
out << "Bool:  " << bool << endl; \
return out.str(); \

#define NI_BLEND_BOOL_INTERPOLATOR_FIXLINKS \
ABlendInterpolator::FixLinks( objects, link_stack, version ); \

#define NI_BLEND_FLOAT_INTERPOLATOR_MEMBERS \
float float; \

#define NI_BLEND_FLOAT_INTERPOLATOR_PARENTS ABlendInterpolator \

#define NI_BLEND_FLOAT_INTERPOLATOR_CONSTRUCT \

#define NI_BLEND_FLOAT_INTERPOLATOR_READ \
ABlendInterpolator::Read( in, link_stack, version ); \
NifStream( float, in, version ); \

#define NI_BLEND_FLOAT_INTERPOLATOR_WRITE \
ABlendInterpolator::Write( out, link_map, version ); \
NifStream( float, out, version ); \

#define NI_BLEND_FLOAT_INTERPOLATOR_STRING \
stringstream out; \
out << ABlendInterpolator::asString(); \
out << "Float:  " << float << endl; \
return out.str(); \

#define NI_BLEND_FLOAT_INTERPOLATOR_FIXLINKS \
ABlendInterpolator::FixLinks( objects, link_stack, version ); \

#define NI_BLEND_POINT3_INTERPOLATOR_MEMBERS \
Vector3 point; \

#define NI_BLEND_POINT3_INTERPOLATOR_PARENTS ABlendInterpolator \

#define NI_BLEND_POINT3_INTERPOLATOR_CONSTRUCT \

#define NI_BLEND_POINT3_INTERPOLATOR_READ \
ABlendInterpolator::Read( in, link_stack, version ); \
NifStream( point, in, version ); \

#define NI_BLEND_POINT3_INTERPOLATOR_WRITE \
ABlendInterpolator::Write( out, link_map, version ); \
NifStream( point, out, version ); \

#define NI_BLEND_POINT3_INTERPOLATOR_STRING \
stringstream out; \
out << ABlendInterpolator::asString(); \
out << "Point:  " << point << endl; \
return out.str(); \

#define NI_BLEND_POINT3_INTERPOLATOR_FIXLINKS \
ABlendInterpolator::FixLinks( objects, link_stack, version ); \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_MEMBERS \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_PARENTS ABlendInterpolator \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_CONSTRUCT \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_READ \
ABlendInterpolator::Read( in, link_stack, version ); \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_WRITE \
ABlendInterpolator::Write( out, link_map, version ); \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_STRING \
stringstream out; \
out << ABlendInterpolator::asString(); \
return out.str(); \

#define NI_BLEND_TRANSFORM_INTERPOLATOR_FIXLINKS \
ABlendInterpolator::FixLinks( objects, link_stack, version ); \

#define NI_BONE_L_O_D_CONTROLLER_MEMBERS \
vector<SkinShapeGroup > shapeGroups1; \
vector< Ref<NiTriShape > shapeGroups2; \

#define NI_BONE_L_O_D_CONTROLLER_PARENTS ABoneLODController \

#define NI_BONE_L_O_D_CONTROLLER_CONSTRUCT \

#define NI_BONE_L_O_D_CONTROLLER_READ \
uint block_num; \
ABoneLODController::Read( in, link_stack, version ); \
uint numShapeGroups; \
NifStream( numShapeGroups, in, version ); \
shapeGroups1.resize(numShapeGroups); \
for (uint i0 = 0; i0 < numShapeGroups; i0++) { \
  uint shapeGroups1_numLinkPairs; \
  NifStream( shapeGroups1_numLinkPairs, in, version ); \
  shapeGroups1[i0].linkPairs.resize(shapeGroups1_numLinkPairs); \
  for (uint i1 = 0; i1 < shapeGroups1_numLinkPairs; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \
uint shapeGroups2_numIndices; \
NifStream( shapeGroups2_numIndices, in, version ); \
shapeGroups2.indices.resize(shapeGroups2_numIndices); \
for (uint i0 = 0; i0 < shapeGroups2_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_BONE_L_O_D_CONTROLLER_WRITE \
ABoneLODController::Write( out, link_map, version ); \
uint numShapeGroups; \
numShapeGroups = uint(numShapeGroups.size()); \
NifStream( numShapeGroups, out, version ); \
for (uint i0 = 0; i0 < numShapeGroups; i0++) { \
  uint shapeGroups1_numLinkPairs; \
  shapeGroups1_numLinkPairs = uint(shapeGroups1[i0].numLinkPairs.size()); \
  NifStream( shapeGroups1_numLinkPairs, out, version ); \
  for (uint i1 = 0; i1 < shapeGroups1_numLinkPairs; i1++) { \
    NifStream( link_map[shapeGroups1[i0].linkPairs[i1].shape], out, version ); \
    NifStream( link_map[shapeGroups1[i0].linkPairs[i1].skinInstance], out, version ); \
  }; \
}; \
uint shapeGroups2_numIndices; \
shapeGroups2_numIndices = uint(shapeGroups2.numIndices.size()); \
NifStream( shapeGroups2_numIndices, out, version ); \
for (uint i0 = 0; i0 < shapeGroups2_numIndices; i0++) { \
  NifStream( link_map[shapeGroups2.indices[i0]], out, version ); \
}; \

#define NI_BONE_L_O_D_CONTROLLER_STRING \
stringstream out; \
out << ABoneLODController::asString(); \
out << "Num Shape Groups:  " << numShapeGroups << endl; \
for (uint i0 = 0; i0 < numShapeGroups; i0++) { \
  out << "  Num Link Pairs:  " << shapeGroups1_numLinkPairs << endl; \
  for (uint i1 = 0; i1 < shapeGroups1_numLinkPairs; i1++) { \
    out << "    Shape:  " << shapeGroups1[i0].linkPairs[i1].shape << endl; \
    out << "    Skin Instance:  " << shapeGroups1[i0].linkPairs[i1].skinInstance << endl; \
  }; \
}; \
out << "Num Indices:  " << shapeGroups2_numIndices << endl; \
for (uint i0 = 0; i0 < shapeGroups2_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << shapeGroups2.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_BONE_L_O_D_CONTROLLER_FIXLINKS \
ABoneLODController::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numShapeGroups; i0++) { \
  for (uint i1 = 0; i1 < shapeGroups1_numLinkPairs; i1++) { \
    shapeGroups1[i0].linkPairs[i1].shape = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
    shapeGroups1[i0].linkPairs[i1].skinInstance = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \
for (uint i0 = 0; i0 < shapeGroups2_numIndices; i0++) { \
  shapeGroups2.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_BOOL_DATA_MEMBERS \
VectorKeyArray<byte > data; \

#define NI_BOOL_DATA_PARENTS AKeyedData \

#define NI_BOOL_DATA_CONSTRUCT \

#define NI_BOOL_DATA_READ \
AKeyedData::Read( in, link_stack, version ); \
NifStream( data.numKeys, in, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, in, version ); \
}; \
data.keys.resize(data.numKeys); \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], in, version ); \
}; \

#define NI_BOOL_DATA_WRITE \
AKeyedData::Write( out, link_map, version ); \
NifStream( data.numKeys, out, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], out, version ); \
}; \

#define NI_BOOL_DATA_STRING \
stringstream out; \
out << AKeyedData::asString(); \
out << "Num Keys:  " << data.numKeys << endl; \
if ( data.numKeys != 0 ) { \
  out << "  Key Type:  " << data.keyType << endl; \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << data.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_BOOL_DATA_FIXLINKS \
AKeyedData::FixLinks( objects, link_stack, version ); \
if ( data.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
}; \

#define NI_BOOLEAN_EXTRA_DATA_MEMBERS \
byte booleanData; \

#define NI_BOOLEAN_EXTRA_DATA_PARENTS NiExtraData \

#define NI_BOOLEAN_EXTRA_DATA_CONSTRUCT \

#define NI_BOOLEAN_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
NifStream( booleanData, in, version ); \

#define NI_BOOLEAN_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
NifStream( booleanData, out, version ); \

#define NI_BOOLEAN_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Boolean Data:  " << booleanData << endl; \
return out.str(); \

#define NI_BOOLEAN_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \

#define NI_BOOL_INTERPOLATOR_MEMBERS \
bool boolValue; \
Ref<NiBoolData > data; \

#define NI_BOOL_INTERPOLATOR_PARENTS AInterpolator \

#define NI_BOOL_INTERPOLATOR_CONSTRUCT \

#define NI_BOOL_INTERPOLATOR_READ \
uint block_num; \
AInterpolator::Read( in, link_stack, version ); \
NifStream( boolValue, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_BOOL_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( boolValue, out, version ); \
NifStream( link_map[data], out, version ); \

#define NI_BOOL_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Bool Value:  " << boolValue << endl; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_BOOL_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_BOOL_TIMELINE_INTERPOLATOR_MEMBERS \
byte bool; \
Ref<NiBoolData > data; \

#define NI_BOOL_TIMELINE_INTERPOLATOR_PARENTS AInterpolator \

#define NI_BOOL_TIMELINE_INTERPOLATOR_CONSTRUCT \

#define NI_BOOL_TIMELINE_INTERPOLATOR_READ \
uint block_num; \
AInterpolator::Read( in, link_stack, version ); \
NifStream( bool, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_BOOL_TIMELINE_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( bool, out, version ); \
NifStream( link_map[data], out, version ); \

#define NI_BOOL_TIMELINE_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Bool:  " << bool << endl; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_BOOL_TIMELINE_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_B_S_BONE_L_O_D_CONTROLLER_MEMBERS \

#define NI_B_S_BONE_L_O_D_CONTROLLER_PARENTS ABoneLODController \

#define NI_B_S_BONE_L_O_D_CONTROLLER_CONSTRUCT \

#define NI_B_S_BONE_L_O_D_CONTROLLER_READ \
ABoneLODController::Read( in, link_stack, version ); \

#define NI_B_S_BONE_L_O_D_CONTROLLER_WRITE \
ABoneLODController::Write( out, link_map, version ); \

#define NI_B_S_BONE_L_O_D_CONTROLLER_STRING \
stringstream out; \
out << ABoneLODController::asString(); \
return out.str(); \

#define NI_B_S_BONE_L_O_D_CONTROLLER_FIXLINKS \
ABoneLODController::FixLinks( objects, link_stack, version ); \

#define NI_B_S_P_ARRAY_CONTROLLER_MEMBERS \

#define NI_B_S_P_ARRAY_CONTROLLER_PARENTS AParticleSystemController \

#define NI_B_S_P_ARRAY_CONTROLLER_CONSTRUCT \

#define NI_B_S_P_ARRAY_CONTROLLER_READ \
AParticleSystemController::Read( in, link_stack, version ); \

#define NI_B_S_P_ARRAY_CONTROLLER_WRITE \
AParticleSystemController::Write( out, link_map, version ); \

#define NI_B_S_P_ARRAY_CONTROLLER_STRING \
stringstream out; \
out << AParticleSystemController::asString(); \
return out.str(); \

#define NI_B_S_P_ARRAY_CONTROLLER_FIXLINKS \
AParticleSystemController::FixLinks( objects, link_stack, version ); \

#define NI_B_SPLINE_BASIS_DATA_MEMBERS \
uint unknownInt; \

#define NI_B_SPLINE_BASIS_DATA_PARENTS NiObject \

#define NI_B_SPLINE_BASIS_DATA_CONSTRUCT \

#define NI_B_SPLINE_BASIS_DATA_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( unknownInt, in, version ); \

#define NI_B_SPLINE_BASIS_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( unknownInt, out, version ); \

#define NI_B_SPLINE_BASIS_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Unknown Int:  " << unknownInt << endl; \
return out.str(); \

#define NI_B_SPLINE_BASIS_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_MEMBERS \
vector<float > unknownFloats; \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_PARENTS ABSplineCompInterpolator \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_CONSTRUCT \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_READ \
ABSplineCompInterpolator::Read( in, link_stack, version ); \
unknownFloats.resize(6); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_WRITE \
ABSplineCompInterpolator::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_STRING \
stringstream out; \
out << ABSplineCompInterpolator::asString(); \
for (uint i0 = 0; i0 < 6; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
return out.str(); \

#define NI_B_SPLINE_COMP_FLOAT_INTERPOLATOR_FIXLINKS \
ABSplineCompInterpolator::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
}; \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_MEMBERS \
Ref<NiBSplineData > data; \
Ref<NiObject > unknownLink; \
vector<float > unknownFloats; \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_PARENTS ABSplineCompInterpolator \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_CONSTRUCT \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_READ \
uint block_num; \
ABSplineCompInterpolator::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
unknownFloats.resize(6); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_WRITE \
ABSplineCompInterpolator::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \
NifStream( link_map[unknownLink], out, version ); \
for (uint i0 = 0; i0 < 6; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_STRING \
stringstream out; \
out << ABSplineCompInterpolator::asString(); \
out << "Data:  " << data << endl; \
out << "Unknown Link:  " << unknownLink << endl; \
for (uint i0 = 0; i0 < 6; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
return out.str(); \

#define NI_B_SPLINE_COMP_POINT3_INTERPOLATOR_FIXLINKS \
ABSplineCompInterpolator::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < 6; i0++) { \
}; \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_MEMBERS \
Ref<NiBSplineData > data; \
Ref<NiBSplineBasisData > basisData; \
vector<float > unknown4; \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_PARENTS ABSplineCompInterpolator \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_CONSTRUCT \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_READ \
uint block_num; \
ABSplineCompInterpolator::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
unknown4.resize(17); \
for (uint i0 = 0; i0 < 17; i0++) { \
  NifStream( unknown4[i0], in, version ); \
}; \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_WRITE \
ABSplineCompInterpolator::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \
NifStream( link_map[basisData], out, version ); \
for (uint i0 = 0; i0 < 17; i0++) { \
  NifStream( unknown4[i0], out, version ); \
}; \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_STRING \
stringstream out; \
out << ABSplineCompInterpolator::asString(); \
out << "Data:  " << data << endl; \
out << "Basis Data:  " << basisData << endl; \
for (uint i0 = 0; i0 < 17; i0++) { \
  out << "  Unknown4[" << i0 << "]:  " << unknown4[i0] << endl; \
}; \
return out.str(); \

#define NI_B_SPLINE_COMP_TRANSFORM_INTERPOLATOR_FIXLINKS \
ABSplineCompInterpolator::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \
basisData = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < 17; i0++) { \
}; \

#define NI_B_SPLINE_DATA_MEMBERS \
uint unknownInt; \
vector<vector<byte > > unknownData; \

#define NI_B_SPLINE_DATA_PARENTS NiObject \

#define NI_B_SPLINE_DATA_CONSTRUCT \

#define NI_B_SPLINE_DATA_READ \
NiObject::Read( in, link_stack, version ); \
uint count; \
NifStream( unknownInt, in, version ); \
NifStream( count, in, version ); \
unknownData.resize(count); \
for (uint i0 = 0; i0 < count; i0++) \
  unknownData[i0].resize(2); \
for (uint i0 = 0; i0 < count; i0++) { \
  for (uint i1 = 0; i1 < 2; i1++) { \
    NifStream( unknownData[i0][i1], in, version ); \
  }; \
}; \

#define NI_B_SPLINE_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
uint count; \
count = uint(count.size()); \
NifStream( unknownInt, out, version ); \
NifStream( count, out, version ); \
for (uint i0 = 0; i0 < count; i0++) { \
  for (uint i1 = 0; i1 < 2; i1++) { \
    NifStream( unknownData[i0][i1], out, version ); \
  }; \
}; \

#define NI_B_SPLINE_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Unknown Int:  " << unknownInt << endl; \
out << "Count:  " << count << endl; \
for (uint i0 = 0; i0 < count; i0++) { \
  for (uint i1 = 0; i1 < 2; i1++) { \
    out << "    Unknown Data[" << i0 << "][" << i1 << "]:  " << unknownData[i0][i1] << endl; \
  }; \
}; \
return out.str(); \

#define NI_B_SPLINE_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < count; i0++) { \
  for (uint i1 = 0; i1 < 2; i1++) { \
  }; \
}; \

#define NI_CAMERA_MEMBERS \
ushort unknownShort; \
float frustumLeft; \
float frustumRight; \
float frustumTop; \
float frustumBottom; \
float frustumNear; \
float frustumFar; \
bool useOrthographicProjection; \
float viewportLeft; \
float viewportRight; \
float viewportTop; \
float viewportBottom; \
float lodAdjust; \
Ref<NiObject > unknownLink_; \
uint unknownInt; \
uint unknownInt2; \

#define NI_CAMERA_PARENTS NiAVObject \

#define NI_CAMERA_CONSTRUCT \

#define NI_CAMERA_READ \
uint block_num; \
NiAVObject::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknownShort, in, version ); \
}; \
NifStream( frustumLeft, in, version ); \
NifStream( frustumRight, in, version ); \
NifStream( frustumTop, in, version ); \
NifStream( frustumBottom, in, version ); \
NifStream( frustumNear, in, version ); \
NifStream( frustumFar, in, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( useOrthographicProjection, in, version ); \
}; \
NifStream( viewportLeft, in, version ); \
NifStream( viewportRight, in, version ); \
NifStream( viewportTop, in, version ); \
NifStream( viewportBottom, in, version ); \
NifStream( lodAdjust, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknownInt, in, version ); \
if ( version >= 0x04020100 ) { \
  NifStream( unknownInt2, in, version ); \
}; \

#define NI_CAMERA_WRITE \
NiAVObject::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknownShort, out, version ); \
}; \
NifStream( frustumLeft, out, version ); \
NifStream( frustumRight, out, version ); \
NifStream( frustumTop, out, version ); \
NifStream( frustumBottom, out, version ); \
NifStream( frustumNear, out, version ); \
NifStream( frustumFar, out, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( useOrthographicProjection, out, version ); \
}; \
NifStream( viewportLeft, out, version ); \
NifStream( viewportRight, out, version ); \
NifStream( viewportTop, out, version ); \
NifStream( viewportBottom, out, version ); \
NifStream( lodAdjust, out, version ); \
NifStream( link_map[unknownLink_], out, version ); \
NifStream( unknownInt, out, version ); \
if ( version >= 0x04020100 ) { \
  NifStream( unknownInt2, out, version ); \
}; \

#define NI_CAMERA_STRING \
stringstream out; \
out << NiAVObject::asString(); \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Frustum Left:  " << frustumLeft << endl; \
out << "Frustum Right:  " << frustumRight << endl; \
out << "Frustum Top:  " << frustumTop << endl; \
out << "Frustum Bottom:  " << frustumBottom << endl; \
out << "Frustum Near:  " << frustumNear << endl; \
out << "Frustum Far:  " << frustumFar << endl; \
out << "Use Orthographic Projection:  " << useOrthographicProjection << endl; \
out << "Viewport Left:  " << viewportLeft << endl; \
out << "Viewport Right:  " << viewportRight << endl; \
out << "Viewport Top:  " << viewportTop << endl; \
out << "Viewport Bottom:  " << viewportBottom << endl; \
out << "LOD Adjust:  " << lodAdjust << endl; \
out << "Unknown Link?:  " << unknownLink_ << endl; \
out << "Unknown Int:  " << unknownInt << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
return out.str(); \

#define NI_CAMERA_FIXLINKS \
NiAVObject::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
}; \
if ( version >= 0x0A010000 ) { \
}; \
unknownLink_ = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( version >= 0x04020100 ) { \
}; \

#define NI_COLLISION_DATA_MEMBERS \
NiNode * targetNode; \
uint unknown2; \
byte unknown3; \
uint collisionType; \
uint unknown5; \
Vector3 unknown7; \
vector<float > unknown6; \
vector<float > unknown8; \

#define NI_COLLISION_DATA_PARENTS NiObject \

#define NI_COLLISION_DATA_CONSTRUCT \

#define NI_COLLISION_DATA_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknown2, in, version ); \
NifStream( unknown3, in, version ); \
NifStream( collisionType, in, version ); \
if ( collisionType == 0 ) { \
  NifStream( unknown5, in, version ); \
  NifStream( unknown7, in, version ); \
}; \
if ( collisionType == 2 ) { \
  unknown6.resize(8); \
  for (uint i1 = 0; i1 < 8; i1++) { \
    NifStream( unknown6[i1], in, version ); \
  }; \
}; \
if ( collisionType == 1 ) { \
  unknown8.resize(15); \
  for (uint i1 = 0; i1 < 15; i1++) { \
    NifStream( unknown8[i1], in, version ); \
  }; \
}; \

#define NI_COLLISION_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( link_map[targetNode], out, version ); \
NifStream( unknown2, out, version ); \
NifStream( unknown3, out, version ); \
NifStream( collisionType, out, version ); \
if ( collisionType == 0 ) { \
  NifStream( unknown5, out, version ); \
  NifStream( unknown7, out, version ); \
}; \
if ( collisionType == 2 ) { \
  for (uint i1 = 0; i1 < 8; i1++) { \
    NifStream( unknown6[i1], out, version ); \
  }; \
}; \
if ( collisionType == 1 ) { \
  for (uint i1 = 0; i1 < 15; i1++) { \
    NifStream( unknown8[i1], out, version ); \
  }; \
}; \

#define NI_COLLISION_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Target Node:  " << targetNode << endl; \
out << "Unknown2:  " << unknown2 << endl; \
out << "Unknown3:  " << unknown3 << endl; \
out << "Collision Type:  " << collisionType << endl; \
if ( collisionType == 0 ) { \
  out << "  Unknown5:  " << unknown5 << endl; \
  out << "  Unknown7:  " << unknown7 << endl; \
}; \
if ( collisionType == 2 ) { \
  for (uint i1 = 0; i1 < 8; i1++) { \
    out << "    Unknown6[" << i1 << "]:  " << unknown6[i1] << endl; \
  }; \
}; \
if ( collisionType == 1 ) { \
  for (uint i1 = 0; i1 < 15; i1++) { \
    out << "    Unknown8[" << i1 << "]:  " << unknown8[i1] << endl; \
  }; \
}; \
return out.str(); \

#define NI_COLLISION_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
targetNode = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( collisionType == 0 ) { \
}; \
if ( collisionType == 2 ) { \
  for (uint i1 = 0; i1 < 8; i1++) { \
  }; \
}; \
if ( collisionType == 1 ) { \
  for (uint i1 = 0; i1 < 15; i1++) { \
  }; \
}; \

#define NI_COLOR_DATA_MEMBERS \
VectorKeyArray<Color4 > data; \

#define NI_COLOR_DATA_PARENTS AKeyedData \

#define NI_COLOR_DATA_CONSTRUCT \

#define NI_COLOR_DATA_READ \
AKeyedData::Read( in, link_stack, version ); \
NifStream( data.numKeys, in, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, in, version ); \
}; \
data.keys.resize(data.numKeys); \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], in, version ); \
}; \

#define NI_COLOR_DATA_WRITE \
AKeyedData::Write( out, link_map, version ); \
NifStream( data.numKeys, out, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], out, version ); \
}; \

#define NI_COLOR_DATA_STRING \
stringstream out; \
out << AKeyedData::asString(); \
out << "Num Keys:  " << data.numKeys << endl; \
if ( data.numKeys != 0 ) { \
  out << "  Key Type:  " << data.keyType << endl; \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << data.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_COLOR_DATA_FIXLINKS \
AKeyedData::FixLinks( objects, link_stack, version ); \
if ( data.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
}; \

#define NI_COLOR_EXTRA_DATA_MEMBERS \
Color4 data; \

#define NI_COLOR_EXTRA_DATA_PARENTS NiExtraData \

#define NI_COLOR_EXTRA_DATA_CONSTRUCT \

#define NI_COLOR_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
NifStream( data, in, version ); \

#define NI_COLOR_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
NifStream( data, out, version ); \

#define NI_COLOR_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_COLOR_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \

#define NI_CONTROLLER_MANAGER_MEMBERS \
bool cumulative; \
vector< Ref<NiControllerSequence > controllerSequences; \
Ref<NiDefaultAVObjectPalette > objectPalette; \

#define NI_CONTROLLER_MANAGER_PARENTS NiTimeController \

#define NI_CONTROLLER_MANAGER_CONSTRUCT \

#define NI_CONTROLLER_MANAGER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
NifStream( cumulative, in, version ); \
uint controllerSequences_numIndices; \
NifStream( controllerSequences_numIndices, in, version ); \
controllerSequences.indices.resize(controllerSequences_numIndices); \
for (uint i0 = 0; i0 < controllerSequences_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_CONTROLLER_MANAGER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( cumulative, out, version ); \
uint controllerSequences_numIndices; \
controllerSequences_numIndices = uint(controllerSequences.numIndices.size()); \
NifStream( controllerSequences_numIndices, out, version ); \
for (uint i0 = 0; i0 < controllerSequences_numIndices; i0++) { \
  NifStream( link_map[controllerSequences.indices[i0]], out, version ); \
}; \
NifStream( link_map[objectPalette], out, version ); \

#define NI_CONTROLLER_MANAGER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Cumulative:  " << cumulative << endl; \
out << "Num Indices:  " << controllerSequences_numIndices << endl; \
for (uint i0 = 0; i0 < controllerSequences_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << controllerSequences.indices[i0] << endl; \
}; \
out << "Object Palette:  " << objectPalette << endl; \
return out.str(); \

#define NI_CONTROLLER_MANAGER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < controllerSequences_numIndices; i0++) { \
  controllerSequences.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
objectPalette = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_CONTROLLER_SEQUENCE_MEMBERS \
string name; \
ControllerLink textKeys; \
uint unknownInt1; \
vector<ControllerLink > controlledBlocks; \
float weight; \
Ref<NiTextKeyExtraData > textKeys2; \
uint cycleType; \
uint unknownInt0; \
float frequency; \
float startTime; \
float stopTime; \
float unknownFloat2; \
byte unknownByte; \
string unknownString; \
Ref<NiStringPalette > stringPalette; \

#define NI_CONTROLLER_SEQUENCE_PARENTS NiObject \

#define NI_CONTROLLER_SEQUENCE_CONSTRUCT \
 : weight(1.0) \

#define NI_CONTROLLER_SEQUENCE_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
uint numControlledBlocks; \
NiControllerManager * manager; \
NifStream( name, in, version ); \
if ( version <= 0x0A010000 ) { \
  if ( version <= 0x0A010000 ) { \
    NifStream( textKeys.name, in, version ); \
  }; \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  if ( version >= 0x0A01006A ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
    NifStream( textKeys.unknownShort0, in, version ); \
  }; \
  if ( version >= 0x0A01006A ) { \
    NifStream( textKeys.priority_, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( textKeys.nodeName, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( textKeys.nodeNameOffset, in, version ); \
    NifStream( textKeys.unknownShort1, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( textKeys.propertyType, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( textKeys.propertyTypeOffset, in, version ); \
    NifStream( textKeys.unknownShort2, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( textKeys.controllerType, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( textKeys.controllerTypeOffset, in, version ); \
    NifStream( textKeys.unknownShort3, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( textKeys.variable1, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( textKeys.variableOffset1, in, version ); \
    NifStream( textKeys.unknownShort4, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( textKeys.variable2, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( textKeys.variableOffset2, in, version ); \
    NifStream( textKeys.unknownShort5, in, version ); \
  }; \
}; \
NifStream( numControlledBlocks, in, version ); \
if ( version >= 0x0A01006A ) { \
  NifStream( unknownInt1, in, version ); \
}; \
controlledBlocks.resize(numControlledBlocks); \
for (uint i0 = 0; i0 < numControlledBlocks; i0++) { \
  if ( version <= 0x0A010000 ) { \
    NifStream( controlledBlocks[i0].name, in, version ); \
  }; \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  if ( version >= 0x0A01006A ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
    NifStream( controlledBlocks[i0].unknownShort0, in, version ); \
  }; \
  if ( version >= 0x0A01006A ) { \
    NifStream( controlledBlocks[i0].priority_, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( controlledBlocks[i0].nodeName, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( controlledBlocks[i0].nodeNameOffset, in, version ); \
    NifStream( controlledBlocks[i0].unknownShort1, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( controlledBlocks[i0].propertyType, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( controlledBlocks[i0].propertyTypeOffset, in, version ); \
    NifStream( controlledBlocks[i0].unknownShort2, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( controlledBlocks[i0].controllerType, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( controlledBlocks[i0].controllerTypeOffset, in, version ); \
    NifStream( controlledBlocks[i0].unknownShort3, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( controlledBlocks[i0].variable1, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( controlledBlocks[i0].variableOffset1, in, version ); \
    NifStream( controlledBlocks[i0].unknownShort4, in, version ); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( controlledBlocks[i0].variable2, in, version ); \
  }; \
  if ( version >= 0x0A020000 ) { \
    NifStream( controlledBlocks[i0].variableOffset2, in, version ); \
    NifStream( controlledBlocks[i0].unknownShort5, in, version ); \
  }; \
}; \
if ( version >= 0x0A01006A ) { \
  NifStream( weight, in, version ); \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( cycleType, in, version ); \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  NifStream( unknownInt0, in, version ); \
}; \
if ( version >= 0x0A01006A ) { \
  NifStream( frequency, in, version ); \
  NifStream( startTime, in, version ); \
  NifStream( stopTime, in, version ); \
}; \
if ( ( version >= 0x0A020000 ) && ( version <= 0x0A020000 ) ) { \
  NifStream( unknownFloat2, in, version ); \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  NifStream( unknownByte, in, version ); \
}; \
if ( version >= 0x0A01006A ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( unknownString, in, version ); \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_CONTROLLER_SEQUENCE_WRITE \
NiObject::Write( out, link_map, version ); \
uint numControlledBlocks; \
numControlledBlocks = uint(numControlledBlocks.size()); \
NiControllerManager * manager; \

#define NI_CONTROLLER_SEQUENCE_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Name:  " << name << endl; \
out << "Name:  " << textKeys.name << endl; \
out << "Interpolator:  " << textKeys.interpolator << endl; \
out << "Unknown Link 1:  " << textKeys.unknownLink1 << endl; \
out << "Unknown Link 2:  " << textKeys.unknownLink2 << endl; \
out << "Unknown Short 0:  " << textKeys.unknownShort0 << endl; \
out << "Priority?:  " << textKeys.priority_ << endl; \
out << "String Palette:  " << textKeys.stringPalette << endl; \
out << "Node Name:  " << textKeys.nodeName << endl; \
out << "Node Name Offset:  " << textKeys.nodeNameOffset << endl; \
out << "Unknown Short 1:  " << textKeys.unknownShort1 << endl; \
out << "Property Type:  " << textKeys.propertyType << endl; \
out << "Property Type Offset:  " << textKeys.propertyTypeOffset << endl; \
out << "Unknown Short 2:  " << textKeys.unknownShort2 << endl; \
out << "Controller Type:  " << textKeys.controllerType << endl; \
out << "Controller Type Offset:  " << textKeys.controllerTypeOffset << endl; \
out << "Unknown Short 3:  " << textKeys.unknownShort3 << endl; \
out << "Variable 1:  " << textKeys.variable1 << endl; \
out << "Variable Offset 1:  " << textKeys.variableOffset1 << endl; \
out << "Unknown Short 4:  " << textKeys.unknownShort4 << endl; \
out << "Variable 2:  " << textKeys.variable2 << endl; \
out << "Variable Offset 2:  " << textKeys.variableOffset2 << endl; \
out << "Unknown Short 5:  " << textKeys.unknownShort5 << endl; \
out << "Num Controlled Blocks:  " << numControlledBlocks << endl; \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
for (uint i0 = 0; i0 < numControlledBlocks; i0++) { \
  out << "  Name:  " << controlledBlocks[i0].name << endl; \
  out << "  Interpolator:  " << controlledBlocks[i0].interpolator << endl; \
  out << "  Unknown Link 1:  " << controlledBlocks[i0].unknownLink1 << endl; \
  out << "  Unknown Link 2:  " << controlledBlocks[i0].unknownLink2 << endl; \
  out << "  Unknown Short 0:  " << controlledBlocks[i0].unknownShort0 << endl; \
  out << "  Priority?:  " << controlledBlocks[i0].priority_ << endl; \
  out << "  String Palette:  " << controlledBlocks[i0].stringPalette << endl; \
  out << "  Node Name:  " << controlledBlocks[i0].nodeName << endl; \
  out << "  Node Name Offset:  " << controlledBlocks[i0].nodeNameOffset << endl; \
  out << "  Unknown Short 1:  " << controlledBlocks[i0].unknownShort1 << endl; \
  out << "  Property Type:  " << controlledBlocks[i0].propertyType << endl; \
  out << "  Property Type Offset:  " << controlledBlocks[i0].propertyTypeOffset << endl; \
  out << "  Unknown Short 2:  " << controlledBlocks[i0].unknownShort2 << endl; \
  out << "  Controller Type:  " << controlledBlocks[i0].controllerType << endl; \
  out << "  Controller Type Offset:  " << controlledBlocks[i0].controllerTypeOffset << endl; \
  out << "  Unknown Short 3:  " << controlledBlocks[i0].unknownShort3 << endl; \
  out << "  Variable 1:  " << controlledBlocks[i0].variable1 << endl; \
  out << "  Variable Offset 1:  " << controlledBlocks[i0].variableOffset1 << endl; \
  out << "  Unknown Short 4:  " << controlledBlocks[i0].unknownShort4 << endl; \
  out << "  Variable 2:  " << controlledBlocks[i0].variable2 << endl; \
  out << "  Variable Offset 2:  " << controlledBlocks[i0].variableOffset2 << endl; \
  out << "  Unknown Short 5:  " << controlledBlocks[i0].unknownShort5 << endl; \
}; \
out << "Weight:  " << weight << endl; \
out << "Text Keys 2:  " << textKeys2 << endl; \
out << "Cycle Type:  " << cycleType << endl; \
out << "Unknown Int 0:  " << unknownInt0 << endl; \
out << "Frequency:  " << frequency << endl; \
out << "Start Time:  " << startTime << endl; \
out << "Stop Time:  " << stopTime << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Manager:  " << manager << endl; \
out << "Unknown String:  " << unknownString << endl; \
out << "String Palette:  " << stringPalette << endl; \
return out.str(); \

#define NI_CONTROLLER_SEQUENCE_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A010000 ) { \
  if ( version <= 0x0A010000 ) { \
  }; \
  textKeys.interpolator = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version >= 0x0A01006A ) { \
    textKeys.unknownLink1 = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    textKeys.unknownLink2 = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
  if ( version >= 0x0A01006A ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
    textKeys.stringPalette = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
}; \
if ( version >= 0x0A01006A ) { \
}; \
for (uint i0 = 0; i0 < numControlledBlocks; i0++) { \
  if ( version <= 0x0A010000 ) { \
  }; \
  controlledBlocks[i0].interpolator = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version >= 0x0A01006A ) { \
    controlledBlocks[i0].unknownLink1 = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    controlledBlocks[i0].unknownLink2 = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
  if ( version >= 0x0A01006A ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
    controlledBlocks[i0].stringPalette = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  if ( version >= 0x0A020000 ) { \
  }; \
}; \
if ( version >= 0x0A01006A ) { \
  textKeys2 = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
}; \
if ( version >= 0x0A01006A ) { \
}; \
if ( ( version >= 0x0A020000 ) && ( version <= 0x0A020000 ) ) { \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
}; \
if ( version >= 0x0A01006A ) { \
  manager = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( version >= 0x0A020000 ) { \
  stringPalette = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_MEMBERS \
uint unknownInt; \
vector<AVObject > objects; \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_PARENTS NiObject \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_CONSTRUCT \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_READ \
NiObject::Read( in, link_stack, version ); \
uint numObjects; \
NifStream( unknownInt, in, version ); \
NifStream( numObjects, in, version ); \
objects.resize(numObjects); \
for (uint i0 = 0; i0 < numObjects; i0++) { \
  NifStream( objects[i0].name, in, version ); \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_WRITE \
NiObject::Write( out, link_map, version ); \
uint numObjects; \
numObjects = uint(numObjects.size()); \
NifStream( unknownInt, out, version ); \
NifStream( numObjects, out, version ); \
for (uint i0 = 0; i0 < numObjects; i0++) { \
  NifStream( objects[i0].name, out, version ); \
  NifStream( link_map[objects[i0].object], out, version ); \
}; \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Unknown Int:  " << unknownInt << endl; \
out << "Num Objects:  " << numObjects << endl; \
for (uint i0 = 0; i0 < numObjects; i0++) { \
  out << "  Name:  " << objects[i0].name << endl; \
  out << "  Object:  " << objects[i0].object << endl; \
}; \
return out.str(); \

#define NI_DEFAULT_A_V_OBJECT_PALETTE_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numObjects; i0++) { \
  objects[i0].object = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_DIRECTIONAL_LIGHT_MEMBERS \

#define NI_DIRECTIONAL_LIGHT_PARENTS NiLight \

#define NI_DIRECTIONAL_LIGHT_CONSTRUCT \

#define NI_DIRECTIONAL_LIGHT_READ \
NiLight::Read( in, link_stack, version ); \

#define NI_DIRECTIONAL_LIGHT_WRITE \
NiLight::Write( out, link_map, version ); \

#define NI_DIRECTIONAL_LIGHT_STRING \
stringstream out; \
out << NiLight::asString(); \
return out.str(); \

#define NI_DIRECTIONAL_LIGHT_FIXLINKS \
NiLight::FixLinks( objects, link_stack, version ); \

#define NI_DITHER_PROPERTY_MEMBERS \
Flags flags; \

#define NI_DITHER_PROPERTY_PARENTS NiProperty \

#define NI_DITHER_PROPERTY_CONSTRUCT \

#define NI_DITHER_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \

#define NI_DITHER_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \

#define NI_DITHER_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
return out.str(); \

#define NI_DITHER_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_FLIP_CONTROLLER_MEMBERS \
uint textureSlot; \
uint unknownInt2; \
float delta; \
vector< Ref<NiSourceTexture > sources; \

#define NI_FLIP_CONTROLLER_PARENTS ASingleInterpolatorController \

#define NI_FLIP_CONTROLLER_CONSTRUCT \

#define NI_FLIP_CONTROLLER_READ \
uint block_num; \
ASingleInterpolatorController::Read( in, link_stack, version ); \
NifStream( textureSlot, in, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( unknownInt2, in, version ); \
  NifStream( delta, in, version ); \
}; \
uint sources_numIndices; \
NifStream( sources_numIndices, in, version ); \
sources.indices.resize(sources_numIndices); \
for (uint i0 = 0; i0 < sources_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_FLIP_CONTROLLER_WRITE \
ASingleInterpolatorController::Write( out, link_map, version ); \
NifStream( textureSlot, out, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( unknownInt2, out, version ); \
  NifStream( delta, out, version ); \
}; \
uint sources_numIndices; \
sources_numIndices = uint(sources.numIndices.size()); \
NifStream( sources_numIndices, out, version ); \
for (uint i0 = 0; i0 < sources_numIndices; i0++) { \
  NifStream( link_map[sources.indices[i0]], out, version ); \
}; \

#define NI_FLIP_CONTROLLER_STRING \
stringstream out; \
out << ASingleInterpolatorController::asString(); \
out << "Texture Slot:  " << textureSlot << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
out << "Delta:  " << delta << endl; \
out << "Num Indices:  " << sources_numIndices << endl; \
for (uint i0 = 0; i0 < sources_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << sources.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_FLIP_CONTROLLER_FIXLINKS \
ASingleInterpolatorController::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A010000 ) { \
}; \
for (uint i0 = 0; i0 < sources_numIndices; i0++) { \
  sources.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_FLOAT_DATA_MEMBERS \
VectorKeyArray<float > data; \

#define NI_FLOAT_DATA_PARENTS AKeyedData \

#define NI_FLOAT_DATA_CONSTRUCT \

#define NI_FLOAT_DATA_READ \
AKeyedData::Read( in, link_stack, version ); \
NifStream( data.numKeys, in, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, in, version ); \
}; \
data.keys.resize(data.numKeys); \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], in, version ); \
}; \

#define NI_FLOAT_DATA_WRITE \
AKeyedData::Write( out, link_map, version ); \
NifStream( data.numKeys, out, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], out, version ); \
}; \

#define NI_FLOAT_DATA_STRING \
stringstream out; \
out << AKeyedData::asString(); \
out << "Num Keys:  " << data.numKeys << endl; \
if ( data.numKeys != 0 ) { \
  out << "  Key Type:  " << data.keyType << endl; \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << data.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_FLOAT_DATA_FIXLINKS \
AKeyedData::FixLinks( objects, link_stack, version ); \
if ( data.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
}; \

#define NI_FLOAT_EXTRA_DATA_MEMBERS \
float floatData; \

#define NI_FLOAT_EXTRA_DATA_PARENTS NiExtraData \

#define NI_FLOAT_EXTRA_DATA_CONSTRUCT \

#define NI_FLOAT_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
NifStream( floatData, in, version ); \

#define NI_FLOAT_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
NifStream( floatData, out, version ); \

#define NI_FLOAT_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Float Data:  " << floatData << endl; \
return out.str(); \

#define NI_FLOAT_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_MEMBERS \
Ref<NiObject > unknownLink; \
string unknownString; \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_PARENTS NiTimeController \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_CONSTRUCT \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( unknownString, in, version ); \
}; \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( link_map[unknownLink], out, version ); \
  NifStream( unknownString, out, version ); \
}; \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Link:  " << unknownLink << endl; \
out << "Unknown String:  " << unknownString << endl; \
return out.str(); \

#define NI_FLOAT_EXTRA_DATA_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( version >= 0x14000004 ) { \
  unknownLink = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_FLOAT_INTERPOLATOR_MEMBERS \
float floatValue; \
Ref<NiFloatData > data; \

#define NI_FLOAT_INTERPOLATOR_PARENTS AInterpolator \

#define NI_FLOAT_INTERPOLATOR_CONSTRUCT \

#define NI_FLOAT_INTERPOLATOR_READ \
uint block_num; \
AInterpolator::Read( in, link_stack, version ); \
NifStream( floatValue, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_FLOAT_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( floatValue, out, version ); \
NifStream( link_map[data], out, version ); \

#define NI_FLOAT_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Float Value:  " << floatValue << endl; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_FLOAT_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_FLOATS_EXTRA_DATA_MEMBERS \
vector<float > data; \

#define NI_FLOATS_EXTRA_DATA_PARENTS NiExtraData \

#define NI_FLOATS_EXTRA_DATA_CONSTRUCT \

#define NI_FLOATS_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
uint numFloats; \
NifStream( numFloats, in, version ); \
data.resize(numFloats); \
for (uint i0 = 0; i0 < numFloats; i0++) { \
  NifStream( data[i0], in, version ); \
}; \

#define NI_FLOATS_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
uint numFloats; \
numFloats = uint(numFloats.size()); \
NifStream( numFloats, out, version ); \
for (uint i0 = 0; i0 < numFloats; i0++) { \
  NifStream( data[i0], out, version ); \
}; \

#define NI_FLOATS_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Num Floats:  " << numFloats << endl; \
for (uint i0 = 0; i0 < numFloats; i0++) { \
  out << "  Data[" << i0 << "]:  " << data[i0] << endl; \
}; \
return out.str(); \

#define NI_FLOATS_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numFloats; i0++) { \
}; \

#define NI_FOG_PROPERTY_MEMBERS \
Flags flags; \
float fogDepth; \
Color3 fogColor; \

#define NI_FOG_PROPERTY_PARENTS NiProperty \

#define NI_FOG_PROPERTY_CONSTRUCT \

#define NI_FOG_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \
NifStream( fogDepth, in, version ); \
NifStream( fogColor, in, version ); \

#define NI_FOG_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \
NifStream( fogDepth, out, version ); \
NifStream( fogColor, out, version ); \

#define NI_FOG_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Fog Depth:  " << fogDepth << endl; \
out << "Fog Color:  " << fogColor << endl; \
return out.str(); \

#define NI_FOG_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_GEOM_MORPHER_CONTROLLER_MEMBERS \
ushort unknown; \
byte unknown2; \
Ref<NiMorphData > data; \
byte unknownByte; \
LinkGroup interpolators; \
vector<uint > unknownInts; \

#define NI_GEOM_MORPHER_CONTROLLER_PARENTS NiTimeController \

#define NI_GEOM_MORPHER_CONTROLLER_CONSTRUCT \

#define NI_GEOM_MORPHER_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
uint numUnknownInts; \
if ( version >= 0x0A010000 ) { \
  NifStream( unknown, in, version ); \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  NifStream( unknown2, in, version ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknownByte, in, version ); \
if ( version >= 0x0A01006A ) { \
  uint interpolators_numIndices; \
  NifStream( interpolators_numIndices, in, version ); \
  interpolators.indices.resize(interpolators_numIndices); \
  for (uint i1 = 0; i1 < interpolators_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( numUnknownInts, in, version ); \
  unknownInts.resize(numUnknownInts); \
  for (uint i1 = 0; i1 < numUnknownInts; i1++) { \
    NifStream( unknownInts[i1], in, version ); \
  }; \
}; \

#define NI_GEOM_MORPHER_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
uint numUnknownInts; \
numUnknownInts = uint(numUnknownInts.size()); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknown, out, version ); \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  NifStream( unknown2, out, version ); \
}; \
NifStream( link_map[data], out, version ); \
NifStream( unknownByte, out, version ); \
if ( version >= 0x0A01006A ) { \
  uint interpolators_numIndices; \
  interpolators_numIndices = uint(interpolators.numIndices.size()); \
  NifStream( interpolators_numIndices, out, version ); \
  for (uint i1 = 0; i1 < interpolators_numIndices; i1++) { \
    NifStream( link_map[interpolators.indices[i1]], out, version ); \
  }; \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( numUnknownInts, out, version ); \
  for (uint i1 = 0; i1 < numUnknownInts; i1++) { \
    NifStream( unknownInts[i1], out, version ); \
  }; \
}; \

#define NI_GEOM_MORPHER_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown:  " << unknown << endl; \
out << "Unknown 2:  " << unknown2 << endl; \
out << "Data:  " << data << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Num Indices:  " << interpolators_numIndices << endl; \
for (uint i0 = 0; i0 < interpolators_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << interpolators.indices[i0] << endl; \
}; \
out << "Num Unknown Ints:  " << numUnknownInts << endl; \
for (uint i0 = 0; i0 < numUnknownInts; i0++) { \
  out << "  Unknown Ints[" << i0 << "]:  " << unknownInts[i0] << endl; \
}; \
return out.str(); \

#define NI_GEOM_MORPHER_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
}; \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
}; \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( version >= 0x0A01006A ) { \
  for (uint i1 = 0; i1 < interpolators_numIndices; i1++) { \
    interpolators.indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \
if ( version >= 0x0A020000 ) { \
  for (uint i1 = 0; i1 < numUnknownInts; i1++) { \
  }; \
}; \

#define NI_GRAVITY_MEMBERS \
float unknownFloat1; \
float force; \
uint type; \
Vector3 position; \
Vector3 direction; \

#define NI_GRAVITY_PARENTS AParticleModifier \

#define NI_GRAVITY_CONSTRUCT \

#define NI_GRAVITY_READ \
AParticleModifier::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( force, in, version ); \
NifStream( type, in, version ); \
NifStream( position, in, version ); \
NifStream( direction, in, version ); \

#define NI_GRAVITY_WRITE \
AParticleModifier::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( force, out, version ); \
NifStream( type, out, version ); \
NifStream( position, out, version ); \
NifStream( direction, out, version ); \

#define NI_GRAVITY_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Force:  " << force << endl; \
out << "Type:  " << type << endl; \
out << "Position:  " << position << endl; \
out << "Direction:  " << direction << endl; \
return out.str(); \

#define NI_GRAVITY_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \

#define NI_INTEGER_EXTRA_DATA_MEMBERS \
uint integerData; \

#define NI_INTEGER_EXTRA_DATA_PARENTS NiExtraData \

#define NI_INTEGER_EXTRA_DATA_CONSTRUCT \

#define NI_INTEGER_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
NifStream( integerData, in, version ); \

#define NI_INTEGER_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
NifStream( integerData, out, version ); \

#define NI_INTEGER_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Integer Data:  " << integerData << endl; \
return out.str(); \

#define NI_INTEGER_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \

#define NI_INTEGERS_EXTRA_DATA_MEMBERS \
vector<uint > data; \

#define NI_INTEGERS_EXTRA_DATA_PARENTS NiExtraData \

#define NI_INTEGERS_EXTRA_DATA_CONSTRUCT \

#define NI_INTEGERS_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
uint numIntegers; \
NifStream( numIntegers, in, version ); \
data.resize(numIntegers); \
for (uint i0 = 0; i0 < numIntegers; i0++) { \
  NifStream( data[i0], in, version ); \
}; \

#define NI_INTEGERS_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
uint numIntegers; \
numIntegers = uint(numIntegers.size()); \
NifStream( numIntegers, out, version ); \
for (uint i0 = 0; i0 < numIntegers; i0++) { \
  NifStream( data[i0], out, version ); \
}; \

#define NI_INTEGERS_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Num Integers:  " << numIntegers << endl; \
for (uint i0 = 0; i0 < numIntegers; i0++) { \
  out << "  Data[" << i0 << "]:  " << data[i0] << endl; \
}; \
return out.str(); \

#define NI_INTEGERS_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numIntegers; i0++) { \
}; \

#define NI_KEYFRAME_CONTROLLER_MEMBERS \

#define NI_KEYFRAME_CONTROLLER_PARENTS AKeyframeController \

#define NI_KEYFRAME_CONTROLLER_CONSTRUCT \

#define NI_KEYFRAME_CONTROLLER_READ \
AKeyframeController::Read( in, link_stack, version ); \

#define NI_KEYFRAME_CONTROLLER_WRITE \
AKeyframeController::Write( out, link_map, version ); \

#define NI_KEYFRAME_CONTROLLER_STRING \
stringstream out; \
out << AKeyframeController::asString(); \
return out.str(); \

#define NI_KEYFRAME_CONTROLLER_FIXLINKS \
AKeyframeController::FixLinks( objects, link_stack, version ); \

#define NI_KEYFRAME_DATA_MEMBERS \

#define NI_KEYFRAME_DATA_PARENTS AKeyframeData \

#define NI_KEYFRAME_DATA_CONSTRUCT \

#define NI_KEYFRAME_DATA_READ \
AKeyframeData::Read( in, link_stack, version ); \

#define NI_KEYFRAME_DATA_WRITE \
AKeyframeData::Write( out, link_map, version ); \

#define NI_KEYFRAME_DATA_STRING \
stringstream out; \
out << AKeyframeData::asString(); \
return out.str(); \

#define NI_KEYFRAME_DATA_FIXLINKS \
AKeyframeData::FixLinks( objects, link_stack, version ); \

#define NI_LIGHT_COLOR_CONTROLLER_MEMBERS \
ushort unknownShort; \
Ref<NiPosData > data; \
Ref<NiPoint3Interpolator > interpolator; \

#define NI_LIGHT_COLOR_CONTROLLER_PARENTS NiTimeController \

#define NI_LIGHT_COLOR_CONTROLLER_CONSTRUCT \

#define NI_LIGHT_COLOR_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A010000 ) ) { \
  NifStream( unknownShort, in, version ); \
}; \
if ( version <= 0x0A010000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( unknownShort, in, version ); \
}; \

#define NI_LIGHT_COLOR_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A010000 ) ) { \
  NifStream( unknownShort, out, version ); \
}; \
if ( version <= 0x0A010000 ) { \
  NifStream( link_map[data], out, version ); \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( link_map[interpolator], out, version ); \
  NifStream( unknownShort, out, version ); \
}; \

#define NI_LIGHT_COLOR_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Data:  " << data << endl; \
out << "Interpolator:  " << interpolator << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
return out.str(); \

#define NI_LIGHT_COLOR_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A010000 ) ) { \
}; \
if ( version <= 0x0A010000 ) { \
  data = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( version >= 0x0A020000 ) { \
  interpolator = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_LIGHT_DIMMER_CONTROLLER_MEMBERS \
Ref<AInterpolator > unknownLink; \

#define NI_LIGHT_DIMMER_CONTROLLER_PARENTS NiTimeController \

#define NI_LIGHT_DIMMER_CONTROLLER_CONSTRUCT \

#define NI_LIGHT_DIMMER_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_LIGHT_DIMMER_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( link_map[unknownLink], out, version ); \

#define NI_LIGHT_DIMMER_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Link:  " << unknownLink << endl; \
return out.str(); \

#define NI_LIGHT_DIMMER_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
unknownLink = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_LOOK_AT_CONTROLLER_MEMBERS \
ushort unknown1; \
Ref<NiNode > lookAtNode; \

#define NI_LOOK_AT_CONTROLLER_PARENTS NiTimeController \

#define NI_LOOK_AT_CONTROLLER_CONSTRUCT \

#define NI_LOOK_AT_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknown1, in, version ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_LOOK_AT_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknown1, out, version ); \
}; \
NifStream( link_map[lookAtNode], out, version ); \

#define NI_LOOK_AT_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown1:  " << unknown1 << endl; \
out << "Look At Node:  " << lookAtNode << endl; \
return out.str(); \

#define NI_LOOK_AT_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
}; \
lookAtNode = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_LOOK_AT_INTERPOLATOR_MEMBERS \
ushort unknownShort; \
Ref<NiNode > lookAt; \
float unknownFloat; \
Vector3 translation; \
Quaternion rotation; \
float scale; \
Ref<NiPoint3Interpolator > unknownLink1; \
Ref<NiFloatInterpolator > unknownLink2; \
Ref<NiFloatInterpolator > unknownLink3; \

#define NI_LOOK_AT_INTERPOLATOR_PARENTS AInterpolator \

#define NI_LOOK_AT_INTERPOLATOR_CONSTRUCT \

#define NI_LOOK_AT_INTERPOLATOR_READ \
uint block_num; \
AInterpolator::Read( in, link_stack, version ); \
NifStream( unknownShort, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( unknownFloat, in, version ); \
NifStream( translation, in, version ); \
NifStream( rotation, in, version ); \
NifStream( scale, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_LOOK_AT_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( unknownShort, out, version ); \
NifStream( link_map[lookAt], out, version ); \
NifStream( unknownFloat, out, version ); \
NifStream( translation, out, version ); \
NifStream( rotation, out, version ); \
NifStream( scale, out, version ); \
NifStream( link_map[unknownLink1], out, version ); \
NifStream( link_map[unknownLink2], out, version ); \
NifStream( link_map[unknownLink3], out, version ); \

#define NI_LOOK_AT_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Look At:  " << lookAt << endl; \
out << "Unknown Float:  " << unknownFloat << endl; \
out << "Translation:  " << translation << endl; \
out << "Rotation:  " << rotation << endl; \
out << "Scale:  " << scale << endl; \
out << "Unknown Link 1:  " << unknownLink1 << endl; \
out << "Unknown Link 2:  " << unknownLink2 << endl; \
out << "Unknown Link 3:  " << unknownLink3 << endl; \
return out.str(); \

#define NI_LOOK_AT_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \
lookAt = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink1 = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink2 = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink3 = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_MATERIAL_COLOR_CONTROLLER_MEMBERS \
ushort unknown; \
Ref<NiColorData > data; \
ushort unknownShort; \

#define NI_MATERIAL_COLOR_CONTROLLER_PARENTS NiTimeController \

#define NI_MATERIAL_COLOR_CONTROLLER_CONSTRUCT \

#define NI_MATERIAL_COLOR_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A010000 ) ) { \
  NifStream( unknown, in, version ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
if ( version >= 0x0A020000 ) { \
  NifStream( unknownShort, in, version ); \
}; \

#define NI_MATERIAL_COLOR_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A010000 ) ) { \
  NifStream( unknown, out, version ); \
}; \
NifStream( link_map[data], out, version ); \
if ( version >= 0x0A020000 ) { \
  NifStream( unknownShort, out, version ); \
}; \

#define NI_MATERIAL_COLOR_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown:  " << unknown << endl; \
out << "Data:  " << data << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
return out.str(); \

#define NI_MATERIAL_COLOR_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A010000 ) ) { \
}; \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( version >= 0x0A020000 ) { \
}; \

#define NI_MATERIAL_PROPERTY_MEMBERS \
Flags flags; \
Color3 ambientColor; \
Color3 diffuseColor; \
Color3 specularColor; \
Color3 emissiveColor; \
float glossiness; \
float alpha; \

#define NI_MATERIAL_PROPERTY_PARENTS NiProperty \

#define NI_MATERIAL_PROPERTY_CONSTRUCT \

#define NI_MATERIAL_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
if ( version <= 0x0A000102 ) { \
  NifStream( flags, in, version ); \
}; \
NifStream( ambientColor, in, version ); \
NifStream( diffuseColor, in, version ); \
NifStream( specularColor, in, version ); \
NifStream( emissiveColor, in, version ); \
NifStream( glossiness, in, version ); \
NifStream( alpha, in, version ); \

#define NI_MATERIAL_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
if ( version <= 0x0A000102 ) { \
  NifStream( flags, out, version ); \
}; \
NifStream( ambientColor, out, version ); \
NifStream( diffuseColor, out, version ); \
NifStream( specularColor, out, version ); \
NifStream( emissiveColor, out, version ); \
NifStream( glossiness, out, version ); \
NifStream( alpha, out, version ); \

#define NI_MATERIAL_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Ambient Color:  " << ambientColor << endl; \
out << "Diffuse Color:  " << diffuseColor << endl; \
out << "Specular Color:  " << specularColor << endl; \
out << "Emissive Color:  " << emissiveColor << endl; \
out << "Glossiness:  " << glossiness << endl; \
out << "Alpha:  " << alpha << endl; \
return out.str(); \

#define NI_MATERIAL_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A000102 ) { \
}; \

#define NI_MESH_P_SYS_DATA_MEMBERS \
byte unknownByte11; \
vector<vector<float > > unknownFloats3; \
vector<vector<float > > unknownFloats4; \
vector<vector<float > > unknownFloats5; \
uint unknownInt1; \
Ref<AParticleModifier > modifier; \
byte unknownByte2; \
LinkGroup unknownLinkGroup; \
ushort unknownShort4; \
uint unknownInt2; \
byte unknownByte12; \
uint unknownInt3; \
uint unknownInt4; \
Ref<NiNode > unknownLink2; \

#define NI_MESH_P_SYS_DATA_PARENTS APSysData \

#define NI_MESH_P_SYS_DATA_CONSTRUCT \

#define NI_MESH_P_SYS_DATA_READ \
uint block_num; \
APSysData::Read( in, link_stack, version ); \
if ( version >= 0x14000005 ) { \
  NifStream( unknownByte11, in, version ); \
}; \
if ( version <= 0x14000004 ) { \
  unknownFloats3.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) \
    unknownFloats3[i1].resize(4); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 4; i2++) { \
      NifStream( unknownFloats3[i1][i2], in, version ); \
    }; \
  }; \
  unknownFloats4.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) \
    unknownFloats4[i1].resize(10); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 10; i2++) { \
      NifStream( unknownFloats4[i1][i2], in, version ); \
    }; \
  }; \
}; \
if ( version >= 0x14000005 ) { \
  unknownFloats5.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) \
    unknownFloats5[i1].resize(12); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 12; i2++) { \
      NifStream( unknownFloats5[i1][i2], in, version ); \
    }; \
  }; \
}; \
NifStream( unknownInt1, in, version ); \
if ( version <= 0x14000004 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
if ( ( version >= 0x0A020000 ) && ( version <= 0x14000004 ) ) { \
  NifStream( unknownByte2, in, version ); \
  uint unknownLinkGroup_numIndices; \
  NifStream( unknownLinkGroup_numIndices, in, version ); \
  unknownLinkGroup.indices.resize(unknownLinkGroup_numIndices); \
  for (uint i1 = 0; i1 < unknownLinkGroup_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \
if ( version >= 0x14000005 ) { \
  NifStream( unknownShort4, in, version ); \
  NifStream( unknownInt2, in, version ); \
  NifStream( unknownByte12, in, version ); \
  NifStream( unknownInt3, in, version ); \
  NifStream( unknownInt4, in, version ); \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_MESH_P_SYS_DATA_WRITE \
APSysData::Write( out, link_map, version ); \
if ( version >= 0x14000005 ) { \
  NifStream( unknownByte11, out, version ); \
}; \
if ( version <= 0x14000004 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 4; i2++) { \
      NifStream( unknownFloats3[i1][i2], out, version ); \
    }; \
  }; \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 10; i2++) { \
      NifStream( unknownFloats4[i1][i2], out, version ); \
    }; \
  }; \
}; \
if ( version >= 0x14000005 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 12; i2++) { \
      NifStream( unknownFloats5[i1][i2], out, version ); \
    }; \
  }; \
}; \
NifStream( unknownInt1, out, version ); \
if ( version <= 0x14000004 ) { \
  NifStream( link_map[modifier], out, version ); \
}; \
if ( ( version >= 0x0A020000 ) && ( version <= 0x14000004 ) ) { \
  NifStream( unknownByte2, out, version ); \
  uint unknownLinkGroup_numIndices; \
  unknownLinkGroup_numIndices = uint(unknownLinkGroup.numIndices.size()); \
  NifStream( unknownLinkGroup_numIndices, out, version ); \
  for (uint i1 = 0; i1 < unknownLinkGroup_numIndices; i1++) { \
    NifStream( link_map[unknownLinkGroup.indices[i1]], out, version ); \
  }; \
}; \
if ( version >= 0x14000005 ) { \
  NifStream( unknownShort4, out, version ); \
  NifStream( unknownInt2, out, version ); \
  NifStream( unknownByte12, out, version ); \
  NifStream( unknownInt3, out, version ); \
  NifStream( unknownInt4, out, version ); \
}; \
if ( version >= 0x0A020000 ) { \
  NifStream( link_map[unknownLink2], out, version ); \
}; \

#define NI_MESH_P_SYS_DATA_STRING \
stringstream out; \
out << APSysData::asString(); \
out << "Unknown Byte 11:  " << unknownByte11 << endl; \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown Floats 3[" << i0 << "][" << i1 << "]:  " << unknownFloats3[i0][i1] << endl; \
  }; \
}; \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  for (uint i1 = 0; i1 < 10; i1++) { \
    out << "    Unknown Floats 4[" << i0 << "][" << i1 << "]:  " << unknownFloats4[i0][i1] << endl; \
  }; \
}; \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  for (uint i1 = 0; i1 < 12; i1++) { \
    out << "    Unknown Floats 5[" << i0 << "][" << i1 << "]:  " << unknownFloats5[i0][i1] << endl; \
  }; \
}; \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
out << "Modifier:  " << modifier << endl; \
out << "Unknown Byte 2:  " << unknownByte2 << endl; \
out << "Num Indices:  " << unknownLinkGroup_numIndices << endl; \
for (uint i0 = 0; i0 < unknownLinkGroup_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << unknownLinkGroup.indices[i0] << endl; \
}; \
out << "Unknown Short 4:  " << unknownShort4 << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
out << "Unknown Byte 12:  " << unknownByte12 << endl; \
out << "Unknown Int 3:  " << unknownInt3 << endl; \
out << "Unknown Int 4:  " << unknownInt4 << endl; \
out << "Unknown Link 2:  " << unknownLink2 << endl; \
return out.str(); \

#define NI_MESH_P_SYS_DATA_FIXLINKS \
APSysData::FixLinks( objects, link_stack, version ); \
if ( version >= 0x14000005 ) { \
}; \
if ( version <= 0x14000004 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 4; i2++) { \
    }; \
  }; \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 10; i2++) { \
    }; \
  }; \
}; \
if ( version >= 0x14000005 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 12; i2++) { \
    }; \
  }; \
}; \
if ( version <= 0x14000004 ) { \
  modifier = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( ( version >= 0x0A020000 ) && ( version <= 0x14000004 ) ) { \
  for (uint i1 = 0; i1 < unknownLinkGroup_numIndices; i1++) { \
    unknownLinkGroup.indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \
if ( version >= 0x14000005 ) { \
}; \
if ( version >= 0x0A020000 ) { \
  unknownLink2 = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_MORPH_DATA_MEMBERS \
uint numVertices; \
byte unknownByte; \
vector<Morph > morphs; \

#define NI_MORPH_DATA_PARENTS NiObject \

#define NI_MORPH_DATA_CONSTRUCT \

#define NI_MORPH_DATA_READ \
NiObject::Read( in, link_stack, version ); \
uint numMorphs; \
NifStream( numMorphs, in, version ); \
NifStream( numVertices, in, version ); \
NifStream( unknownByte, in, version ); \
morphs.resize(numMorphs); \
for (uint i0 = 0; i0 < numMorphs; i0++) { \
  if ( version >= 0x0A01006A ) { \
    NifStream( morphs[i0].frameName, in, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    uint morphs_frames_numKeys; \
    NifStream( morphs_frames_numKeys, in, version ); \
    NifStream( morphs[i0].frames.keyType, in, version ); \
    morphs[i0].frames.keys.resize(morphs_frames_numKeys); \
    for (uint i2 = 0; i2 < morphs_frames_numKeys; i2++) { \
      NifStream( morphs[i0].frames.keys[i2], in, version ); \
    }; \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( morphs[i0].unknownInt, in, version ); \
  }; \
  morphs[i0].vectors.resize(attr_arg); \
  for (uint i1 = 0; i1 < attr_arg; i1++) { \
    NifStream( morphs[i0].vectors[i1], in, version ); \
  }; \
}; \

#define NI_MORPH_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
uint numMorphs; \
numMorphs = uint(numMorphs.size()); \
NifStream( numMorphs, out, version ); \
NifStream( numVertices, out, version ); \
NifStream( unknownByte, out, version ); \
for (uint i0 = 0; i0 < numMorphs; i0++) { \
  if ( version >= 0x0A01006A ) { \
    NifStream( morphs[i0].frameName, out, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    uint morphs_frames_numKeys; \
    morphs_frames_numKeys = uint(morphs[i0].frames.numKeys.size()); \
    NifStream( morphs_frames_numKeys, out, version ); \
    NifStream( morphs[i0].frames.keyType, out, version ); \
    for (uint i2 = 0; i2 < morphs_frames_numKeys; i2++) { \
      NifStream( morphs[i0].frames.keys[i2], out, version ); \
    }; \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
    NifStream( morphs[i0].unknownInt, out, version ); \
  }; \
  for (uint i1 = 0; i1 < attr_arg; i1++) { \
    NifStream( morphs[i0].vectors[i1], out, version ); \
  }; \
}; \

#define NI_MORPH_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Num Morphs:  " << numMorphs << endl; \
out << "Num Vertices:  " << numVertices << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
for (uint i0 = 0; i0 < numMorphs; i0++) { \
  out << "  Frame Name:  " << morphs[i0].frameName << endl; \
  out << "  Num Keys:  " << morphs_frames_numKeys << endl; \
  out << "  Key Type:  " << morphs[i0].frames.keyType << endl; \
  for (uint i1 = 0; i1 < morphs_frames_numKeys; i1++) { \
    out << "    Keys[" << i1 << "]:  " << morphs[i0].frames.keys[i1] << endl; \
  }; \
  out << "  Unknown Int:  " << morphs[i0].unknownInt << endl; \
  for (uint i1 = 0; i1 < attr_arg; i1++) { \
    out << "    Vectors[" << i1 << "]:  " << morphs[i0].vectors[i1] << endl; \
  }; \
}; \
return out.str(); \

#define NI_MORPH_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numMorphs; i0++) { \
  if ( version >= 0x0A01006A ) { \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < morphs_frames_numKeys; i2++) { \
    }; \
  }; \
  if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  }; \
  for (uint i1 = 0; i1 < attr_arg; i1++) { \
  }; \
}; \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_MEMBERS \
TargetGroup targets; \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_PARENTS NiTimeController \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_CONSTRUCT \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_READ \
NiTimeController::Read( in, link_stack, version ); \
ushort targets_numIndices; \
NifStream( targets_numIndices, in, version ); \
targets.indices.resize(targets_numIndices); \
for (uint i0 = 0; i0 < targets_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
ushort targets_numIndices; \
targets_numIndices = ushort(targets.numIndices.size()); \
NifStream( targets_numIndices, out, version ); \
for (uint i0 = 0; i0 < targets_numIndices; i0++) { \
  NifStream( link_map[targets.indices[i0]], out, version ); \
}; \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Num Indices:  " << targets_numIndices << endl; \
for (uint i0 = 0; i0 < targets_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << targets.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_MULTI_TARGET_TRANSFORM_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < targets_numIndices; i0++) { \
  targets.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_NODE_MEMBERS \
vector< Ref<NiAVObject > children; \
vector< Ref<NiDynamicEffect > effects; \

#define NI_NODE_PARENTS NiAVObject \

#define NI_NODE_CONSTRUCT \

#define NI_NODE_READ \
uint block_num; \
NiAVObject::Read( in, link_stack, version ); \
uint children_numIndices; \
NifStream( children_numIndices, in, version ); \
children.indices.resize(children_numIndices); \
for (uint i0 = 0; i0 < children_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
uint effects_numIndices; \
NifStream( effects_numIndices, in, version ); \
effects.indices.resize(effects_numIndices); \
for (uint i0 = 0; i0 < effects_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_NODE_WRITE \
NiAVObject::Write( out, link_map, version ); \
uint children_numIndices; \
children_numIndices = uint(children.numIndices.size()); \
NifStream( children_numIndices, out, version ); \
for (uint i0 = 0; i0 < children_numIndices; i0++) { \
  NifStream( link_map[children.indices[i0]], out, version ); \
}; \
uint effects_numIndices; \
effects_numIndices = uint(effects.numIndices.size()); \
NifStream( effects_numIndices, out, version ); \
for (uint i0 = 0; i0 < effects_numIndices; i0++) { \
  NifStream( link_map[effects.indices[i0]], out, version ); \
}; \

#define NI_NODE_STRING \
stringstream out; \
out << NiAVObject::asString(); \
out << "Num Indices:  " << children_numIndices << endl; \
for (uint i0 = 0; i0 < children_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << children.indices[i0] << endl; \
}; \
out << "Num Indices:  " << effects_numIndices << endl; \
for (uint i0 = 0; i0 < effects_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << effects.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_NODE_FIXLINKS \
NiAVObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < children_numIndices; i0++) { \
  children.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
for (uint i0 = 0; i0 < effects_numIndices; i0++) { \
  effects.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define A_FX_MEMBERS \
byte unknown1; \
unk292bytes unknown2; \

#define A_FX_PARENTS NiNode \

#define A_FX_CONSTRUCT \

#define A_FX_READ \
NiNode::Read( in, link_stack, version ); \
NifStream( unknown1, in, version ); \
unknown2.unknown292Bytes.resize(73); \
for (uint i0 = 0; i0 < 73; i0++) \
  unknown2.unknown292Bytes[i0].resize(4); \
for (uint i0 = 0; i0 < 73; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknown2.unknown292Bytes[i0][i1], in, version ); \
  }; \
}; \

#define A_FX_WRITE \
NiNode::Write( out, link_map, version ); \
NifStream( unknown1, out, version ); \
for (uint i0 = 0; i0 < 73; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( unknown2.unknown292Bytes[i0][i1], out, version ); \
  }; \
}; \

#define A_FX_STRING \
stringstream out; \
out << NiNode::asString(); \
out << "Unknown1:  " << unknown1 << endl; \
for (uint i0 = 0; i0 < 73; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown 292 Bytes[" << i0 << "][" << i1 << "]:  " << unknown2.unknown292Bytes[i0][i1] << endl; \
  }; \
}; \
return out.str(); \

#define A_FX_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 73; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
}; \

#define AVOID_NODE_MEMBERS \

#define AVOID_NODE_PARENTS NiNode \

#define AVOID_NODE_CONSTRUCT \

#define AVOID_NODE_READ \
NiNode::Read( in, link_stack, version ); \

#define AVOID_NODE_WRITE \
NiNode::Write( out, link_map, version ); \

#define AVOID_NODE_STRING \
stringstream out; \
out << NiNode::asString(); \
return out.str(); \

#define AVOID_NODE_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \

#define FX_BUTTON_MEMBERS \

#define FX_BUTTON_PARENTS AFx \

#define FX_BUTTON_CONSTRUCT \

#define FX_BUTTON_READ \
AFx::Read( in, link_stack, version ); \

#define FX_BUTTON_WRITE \
AFx::Write( out, link_map, version ); \

#define FX_BUTTON_STRING \
stringstream out; \
out << AFx::asString(); \
return out.str(); \

#define FX_BUTTON_FIXLINKS \
AFx::FixLinks( objects, link_stack, version ); \

#define FX_RADIO_BUTTON_MEMBERS \
uint unknownInt1; \
uint unknownInt2; \
uint unknownInt3; \
vector< Ref<NiObject > unknownLinks; \

#define FX_RADIO_BUTTON_PARENTS AFx \

#define FX_RADIO_BUTTON_CONSTRUCT \

#define FX_RADIO_BUTTON_READ \
uint block_num; \
AFx::Read( in, link_stack, version ); \
NifStream( unknownInt1, in, version ); \
NifStream( unknownInt2, in, version ); \
NifStream( unknownInt3, in, version ); \
uint unknownLinks_numIndices; \
NifStream( unknownLinks_numIndices, in, version ); \
unknownLinks.indices.resize(unknownLinks_numIndices); \
for (uint i0 = 0; i0 < unknownLinks_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define FX_RADIO_BUTTON_WRITE \
AFx::Write( out, link_map, version ); \
NifStream( unknownInt1, out, version ); \
NifStream( unknownInt2, out, version ); \
NifStream( unknownInt3, out, version ); \
uint unknownLinks_numIndices; \
unknownLinks_numIndices = uint(unknownLinks.numIndices.size()); \
NifStream( unknownLinks_numIndices, out, version ); \
for (uint i0 = 0; i0 < unknownLinks_numIndices; i0++) { \
  NifStream( link_map[unknownLinks.indices[i0]], out, version ); \
}; \

#define FX_RADIO_BUTTON_STRING \
stringstream out; \
out << AFx::asString(); \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
out << "Unknown Int  2:  " << unknownInt2 << endl; \
out << "Unknown Int 3:  " << unknownInt3 << endl; \
out << "Num Indices:  " << unknownLinks_numIndices << endl; \
for (uint i0 = 0; i0 < unknownLinks_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << unknownLinks.indices[i0] << endl; \
}; \
return out.str(); \

#define FX_RADIO_BUTTON_FIXLINKS \
AFx::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < unknownLinks_numIndices; i0++) { \
  unknownLinks.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define FX_WIDGET_MEMBERS \

#define FX_WIDGET_PARENTS AFx \

#define FX_WIDGET_CONSTRUCT \

#define FX_WIDGET_READ \
AFx::Read( in, link_stack, version ); \

#define FX_WIDGET_WRITE \
AFx::Write( out, link_map, version ); \

#define FX_WIDGET_STRING \
stringstream out; \
out << AFx::asString(); \
return out.str(); \

#define FX_WIDGET_FIXLINKS \
AFx::FixLinks( objects, link_stack, version ); \

#define NI_BILLBOARD_NODE_MEMBERS \
ushort billboardMode; \

#define NI_BILLBOARD_NODE_PARENTS NiNode \

#define NI_BILLBOARD_NODE_CONSTRUCT \

#define NI_BILLBOARD_NODE_READ \
NiNode::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( billboardMode, in, version ); \
}; \

#define NI_BILLBOARD_NODE_WRITE \
NiNode::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( billboardMode, out, version ); \
}; \

#define NI_BILLBOARD_NODE_STRING \
stringstream out; \
out << NiNode::asString(); \
out << "Billboard Mode:  " << billboardMode << endl; \
return out.str(); \

#define NI_BILLBOARD_NODE_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
}; \

#define NI_B_S_ANIMATION_NODE_MEMBERS \

#define NI_B_S_ANIMATION_NODE_PARENTS NiNode \

#define NI_B_S_ANIMATION_NODE_CONSTRUCT \

#define NI_B_S_ANIMATION_NODE_READ \
NiNode::Read( in, link_stack, version ); \

#define NI_B_S_ANIMATION_NODE_WRITE \
NiNode::Write( out, link_map, version ); \

#define NI_B_S_ANIMATION_NODE_STRING \
stringstream out; \
out << NiNode::asString(); \
return out.str(); \

#define NI_B_S_ANIMATION_NODE_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \

#define NI_B_S_PARTICLE_NODE_MEMBERS \

#define NI_B_S_PARTICLE_NODE_PARENTS NiNode \

#define NI_B_S_PARTICLE_NODE_CONSTRUCT \

#define NI_B_S_PARTICLE_NODE_READ \
NiNode::Read( in, link_stack, version ); \

#define NI_B_S_PARTICLE_NODE_WRITE \
NiNode::Write( out, link_map, version ); \

#define NI_B_S_PARTICLE_NODE_STRING \
stringstream out; \
out << NiNode::asString(); \
return out.str(); \

#define NI_B_S_PARTICLE_NODE_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \

#define NI_L_O_D_NODE_MEMBERS \
LODInfo lodInfo; \

#define NI_L_O_D_NODE_PARENTS NiNode \

#define NI_L_O_D_NODE_CONSTRUCT \

#define NI_L_O_D_NODE_READ \
uint block_num; \
NiNode::Read( in, link_stack, version ); \
uint lodInfo_numLodLevels; \
NifStream( lodInfo.lodType, in, version ); \
if ( lodInfo.lodType == 0 ) { \
  NifStream( lodInfo.lodCenter, in, version ); \
  NifStream( lodInfo_numLodLevels, in, version ); \
  lodInfo.lodLevels.resize(lodInfo_numLodLevels); \
  for (uint i1 = 0; i1 < lodInfo_numLodLevels; i1++) { \
    NifStream( lodInfo.lodLevels[i1].near, in, version ); \
    NifStream( lodInfo.lodLevels[i1].far, in, version ); \
  }; \
}; \
if ( lodInfo.lodType == 1 ) { \
  NifStream( lodInfo.unknownShort, in, version ); \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_L_O_D_NODE_WRITE \
NiNode::Write( out, link_map, version ); \
uint lodInfo_numLodLevels; \
lodInfo_numLodLevels = uint(lodInfo.numLodLevels.size()); \
NifStream( lodInfo.lodType, out, version ); \
if ( lodInfo.lodType == 0 ) { \
  NifStream( lodInfo.lodCenter, out, version ); \
  NifStream( lodInfo_numLodLevels, out, version ); \
  for (uint i1 = 0; i1 < lodInfo_numLodLevels; i1++) { \
    NifStream( lodInfo.lodLevels[i1].near, out, version ); \
    NifStream( lodInfo.lodLevels[i1].far, out, version ); \
  }; \
}; \
if ( lodInfo.lodType == 1 ) { \
  NifStream( lodInfo.unknownShort, out, version ); \
  NifStream( link_map[lodInfo.rangeData], out, version ); \
}; \

#define NI_L_O_D_NODE_STRING \
stringstream out; \
out << NiNode::asString(); \
out << "LOD Type:  " << lodInfo.lodType << endl; \
if ( lodInfo.lodType == 0 ) { \
  out << "  LOD Center:  " << lodInfo.lodCenter << endl; \
  out << "  Num LOD Levels:  " << lodInfo_numLodLevels << endl; \
  for (uint i1 = 0; i1 < lodInfo_numLodLevels; i1++) { \
    out << "    Near:  " << lodInfo.lodLevels[i1].near << endl; \
    out << "    Far:  " << lodInfo.lodLevels[i1].far << endl; \
  }; \
}; \
if ( lodInfo.lodType == 1 ) { \
  out << "  Unknown Short:  " << lodInfo.unknownShort << endl; \
  out << "  Range Data:  " << lodInfo.rangeData << endl; \
}; \
return out.str(); \

#define NI_L_O_D_NODE_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \
if ( lodInfo.lodType == 0 ) { \
  for (uint i1 = 0; i1 < lodInfo_numLodLevels; i1++) { \
  }; \
}; \
if ( lodInfo.lodType == 1 ) { \
  lodInfo.rangeData = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_PALETTE_MEMBERS \
byte unknownByte; \
uint numEntries_; \
vector<vector<byte > > palette; \

#define NI_PALETTE_PARENTS NiObject \

#define NI_PALETTE_CONSTRUCT \

#define NI_PALETTE_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( unknownByte, in, version ); \
NifStream( numEntries_, in, version ); \
palette.resize(256); \
for (uint i0 = 0; i0 < 256; i0++) \
  palette[i0].resize(4); \
for (uint i0 = 0; i0 < 256; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( palette[i0][i1], in, version ); \
  }; \
}; \

#define NI_PALETTE_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( unknownByte, out, version ); \
NifStream( numEntries_, out, version ); \
for (uint i0 = 0; i0 < 256; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( palette[i0][i1], out, version ); \
  }; \
}; \

#define NI_PALETTE_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Num Entries?:  " << numEntries_ << endl; \
for (uint i0 = 0; i0 < 256; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Palette[" << i0 << "][" << i1 << "]:  " << palette[i0][i1] << endl; \
  }; \
}; \
return out.str(); \

#define NI_PALETTE_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 256; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
}; \

#define NI_PARTICLE_BOMB_MEMBERS \
float unknownFloat1; \
float unknownFloat2; \
float unknownFloat3; \
float unknownFloat4; \
uint unknownInt1; \
uint unknownInt2; \
float unknownFloat5; \
float unknownFloat6; \
float unknownFloat7; \
float unknownFloat8; \
float unknownFloat9; \
float unknownFloat10; \

#define NI_PARTICLE_BOMB_PARENTS AParticleModifier \

#define NI_PARTICLE_BOMB_CONSTRUCT \

#define NI_PARTICLE_BOMB_READ \
AParticleModifier::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \
NifStream( unknownFloat3, in, version ); \
NifStream( unknownFloat4, in, version ); \
NifStream( unknownInt1, in, version ); \
NifStream( unknownInt2, in, version ); \
NifStream( unknownFloat5, in, version ); \
NifStream( unknownFloat6, in, version ); \
NifStream( unknownFloat7, in, version ); \
NifStream( unknownFloat8, in, version ); \
NifStream( unknownFloat9, in, version ); \
NifStream( unknownFloat10, in, version ); \

#define NI_PARTICLE_BOMB_WRITE \
AParticleModifier::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \
NifStream( unknownFloat3, out, version ); \
NifStream( unknownFloat4, out, version ); \
NifStream( unknownInt1, out, version ); \
NifStream( unknownInt2, out, version ); \
NifStream( unknownFloat5, out, version ); \
NifStream( unknownFloat6, out, version ); \
NifStream( unknownFloat7, out, version ); \
NifStream( unknownFloat8, out, version ); \
NifStream( unknownFloat9, out, version ); \
NifStream( unknownFloat10, out, version ); \

#define NI_PARTICLE_BOMB_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Float 3:  " << unknownFloat3 << endl; \
out << "Unknown Float 4:  " << unknownFloat4 << endl; \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
out << "Unknown Float 5:  " << unknownFloat5 << endl; \
out << "Unknown Float 6:  " << unknownFloat6 << endl; \
out << "Unknown Float 7:  " << unknownFloat7 << endl; \
out << "Unknown Float 8:  " << unknownFloat8 << endl; \
out << "Unknown Float 9:  " << unknownFloat9 << endl; \
out << "Unknown Float 10:  " << unknownFloat10 << endl; \
return out.str(); \

#define NI_PARTICLE_BOMB_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \

#define NI_PARTICLE_COLOR_MODIFIER_MEMBERS \
Ref<NiColorData > colorData; \

#define NI_PARTICLE_COLOR_MODIFIER_PARENTS AParticleModifier \

#define NI_PARTICLE_COLOR_MODIFIER_CONSTRUCT \

#define NI_PARTICLE_COLOR_MODIFIER_READ \
uint block_num; \
AParticleModifier::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_PARTICLE_COLOR_MODIFIER_WRITE \
AParticleModifier::Write( out, link_map, version ); \
NifStream( link_map[colorData], out, version ); \

#define NI_PARTICLE_COLOR_MODIFIER_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Color Data:  " << colorData << endl; \
return out.str(); \

#define NI_PARTICLE_COLOR_MODIFIER_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \
colorData = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_PARTICLE_GROW_FADE_MEMBERS \
float grow; \
float fade; \

#define NI_PARTICLE_GROW_FADE_PARENTS AParticleModifier \

#define NI_PARTICLE_GROW_FADE_CONSTRUCT \

#define NI_PARTICLE_GROW_FADE_READ \
AParticleModifier::Read( in, link_stack, version ); \
NifStream( grow, in, version ); \
NifStream( fade, in, version ); \

#define NI_PARTICLE_GROW_FADE_WRITE \
AParticleModifier::Write( out, link_map, version ); \
NifStream( grow, out, version ); \
NifStream( fade, out, version ); \

#define NI_PARTICLE_GROW_FADE_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Grow:  " << grow << endl; \
out << "Fade:  " << fade << endl; \
return out.str(); \

#define NI_PARTICLE_GROW_FADE_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \

#define NI_PARTICLE_MESHES_DATA_MEMBERS \
Ref<NiTriBasedGeom > unknownLink2; \

#define NI_PARTICLE_MESHES_DATA_PARENTS ARotatingParticlesData \

#define NI_PARTICLE_MESHES_DATA_CONSTRUCT \

#define NI_PARTICLE_MESHES_DATA_READ \
uint block_num; \
ARotatingParticlesData::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_PARTICLE_MESHES_DATA_WRITE \
ARotatingParticlesData::Write( out, link_map, version ); \
NifStream( link_map[unknownLink2], out, version ); \

#define NI_PARTICLE_MESHES_DATA_STRING \
stringstream out; \
out << ARotatingParticlesData::asString(); \
out << "Unknown Link 2:  " << unknownLink2 << endl; \
return out.str(); \

#define NI_PARTICLE_MESHES_DATA_FIXLINKS \
ARotatingParticlesData::FixLinks( objects, link_stack, version ); \
unknownLink2 = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_PARTICLE_MESH_MODIFIER_MEMBERS \
LinkGroup particleMeshes; \

#define NI_PARTICLE_MESH_MODIFIER_PARENTS AParticleModifier \

#define NI_PARTICLE_MESH_MODIFIER_CONSTRUCT \

#define NI_PARTICLE_MESH_MODIFIER_READ \
uint block_num; \
AParticleModifier::Read( in, link_stack, version ); \
uint particleMeshes_numIndices; \
NifStream( particleMeshes_numIndices, in, version ); \
particleMeshes.indices.resize(particleMeshes_numIndices); \
for (uint i0 = 0; i0 < particleMeshes_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_PARTICLE_MESH_MODIFIER_WRITE \
AParticleModifier::Write( out, link_map, version ); \
uint particleMeshes_numIndices; \
particleMeshes_numIndices = uint(particleMeshes.numIndices.size()); \
NifStream( particleMeshes_numIndices, out, version ); \
for (uint i0 = 0; i0 < particleMeshes_numIndices; i0++) { \
  NifStream( link_map[particleMeshes.indices[i0]], out, version ); \
}; \

#define NI_PARTICLE_MESH_MODIFIER_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Num Indices:  " << particleMeshes_numIndices << endl; \
for (uint i0 = 0; i0 < particleMeshes_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << particleMeshes.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_PARTICLE_MESH_MODIFIER_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < particleMeshes_numIndices; i0++) { \
  particleMeshes.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_PARTICLE_ROTATION_MEMBERS \
byte unknownByte; \
float unknownFloat1; \
float unknownFloat2; \
float unknownFloat3; \
float unknownFloat4; \

#define NI_PARTICLE_ROTATION_PARENTS AParticleModifier \

#define NI_PARTICLE_ROTATION_CONSTRUCT \

#define NI_PARTICLE_ROTATION_READ \
AParticleModifier::Read( in, link_stack, version ); \
NifStream( unknownByte, in, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \
NifStream( unknownFloat3, in, version ); \
NifStream( unknownFloat4, in, version ); \

#define NI_PARTICLE_ROTATION_WRITE \
AParticleModifier::Write( out, link_map, version ); \
NifStream( unknownByte, out, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \
NifStream( unknownFloat3, out, version ); \
NifStream( unknownFloat4, out, version ); \

#define NI_PARTICLE_ROTATION_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Float 3:  " << unknownFloat3 << endl; \
out << "Unknown Float 4:  " << unknownFloat4 << endl; \
return out.str(); \

#define NI_PARTICLE_ROTATION_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \

#define NI_PARTICLES_MEMBERS \

#define NI_PARTICLES_PARENTS NiTriBasedGeom \

#define NI_PARTICLES_CONSTRUCT \

#define NI_PARTICLES_READ \
NiTriBasedGeom::Read( in, link_stack, version ); \

#define NI_PARTICLES_WRITE \
NiTriBasedGeom::Write( out, link_map, version ); \

#define NI_PARTICLES_STRING \
stringstream out; \
out << NiTriBasedGeom::asString(); \
return out.str(); \

#define NI_PARTICLES_FIXLINKS \
NiTriBasedGeom::FixLinks( objects, link_stack, version ); \

#define NI_AUTO_NORMAL_PARTICLES_MEMBERS \

#define NI_AUTO_NORMAL_PARTICLES_PARENTS NiParticles \

#define NI_AUTO_NORMAL_PARTICLES_CONSTRUCT \

#define NI_AUTO_NORMAL_PARTICLES_READ \
NiParticles::Read( in, link_stack, version ); \

#define NI_AUTO_NORMAL_PARTICLES_WRITE \
NiParticles::Write( out, link_map, version ); \

#define NI_AUTO_NORMAL_PARTICLES_STRING \
stringstream out; \
out << NiParticles::asString(); \
return out.str(); \

#define NI_AUTO_NORMAL_PARTICLES_FIXLINKS \
NiParticles::FixLinks( objects, link_stack, version ); \

#define NI_MESH_PARTICLE_SYSTEM_MEMBERS \
ModifierGroup modifiers; \

#define NI_MESH_PARTICLE_SYSTEM_PARENTS NiParticles \

#define NI_MESH_PARTICLE_SYSTEM_CONSTRUCT \

#define NI_MESH_PARTICLE_SYSTEM_READ \
uint block_num; \
NiParticles::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( modifiers.hasModifiers, in, version ); \
  uint modifiers_modifiers_numIndices; \
  NifStream( modifiers_modifiers_numIndices, in, version ); \
  modifiers.modifiers.indices.resize(modifiers_modifiers_numIndices); \
  for (uint i1 = 0; i1 < modifiers_modifiers_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \

#define NI_MESH_PARTICLE_SYSTEM_WRITE \
NiParticles::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( modifiers.hasModifiers, out, version ); \
  uint modifiers_modifiers_numIndices; \
  modifiers_modifiers_numIndices = uint(modifiers.modifiers.numIndices.size()); \
  NifStream( modifiers_modifiers_numIndices, out, version ); \
  for (uint i1 = 0; i1 < modifiers_modifiers_numIndices; i1++) { \
    NifStream( link_map[modifiers.modifiers.indices[i1]], out, version ); \
  }; \
}; \

#define NI_MESH_PARTICLE_SYSTEM_STRING \
stringstream out; \
out << NiParticles::asString(); \
out << "Has Modifiers:  " << modifiers.hasModifiers << endl; \
out << "Num Indices:  " << modifiers_modifiers_numIndices << endl; \
for (uint i0 = 0; i0 < modifiers_modifiers_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << modifiers.modifiers.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_MESH_PARTICLE_SYSTEM_FIXLINKS \
NiParticles::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  for (uint i1 = 0; i1 < modifiers_modifiers_numIndices; i1++) { \
    modifiers.modifiers.indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \

#define NI_PARTICLE_MESHES_MEMBERS \

#define NI_PARTICLE_MESHES_PARENTS NiParticles \

#define NI_PARTICLE_MESHES_CONSTRUCT \

#define NI_PARTICLE_MESHES_READ \
NiParticles::Read( in, link_stack, version ); \

#define NI_PARTICLE_MESHES_WRITE \
NiParticles::Write( out, link_map, version ); \

#define NI_PARTICLE_MESHES_STRING \
stringstream out; \
out << NiParticles::asString(); \
return out.str(); \

#define NI_PARTICLE_MESHES_FIXLINKS \
NiParticles::FixLinks( objects, link_stack, version ); \

#define NI_PARTICLES_DATA_MEMBERS \

#define NI_PARTICLES_DATA_PARENTS ARotatingParticlesData \

#define NI_PARTICLES_DATA_CONSTRUCT \

#define NI_PARTICLES_DATA_READ \
ARotatingParticlesData::Read( in, link_stack, version ); \

#define NI_PARTICLES_DATA_WRITE \
ARotatingParticlesData::Write( out, link_map, version ); \

#define NI_PARTICLES_DATA_STRING \
stringstream out; \
out << ARotatingParticlesData::asString(); \
return out.str(); \

#define NI_PARTICLES_DATA_FIXLINKS \
ARotatingParticlesData::FixLinks( objects, link_stack, version ); \

#define NI_PARTICLE_SYSTEM_MEMBERS \
ModifierGroup modifiers; \

#define NI_PARTICLE_SYSTEM_PARENTS NiParticles \

#define NI_PARTICLE_SYSTEM_CONSTRUCT \

#define NI_PARTICLE_SYSTEM_READ \
uint block_num; \
NiParticles::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( modifiers.hasModifiers, in, version ); \
  uint modifiers_modifiers_numIndices; \
  NifStream( modifiers_modifiers_numIndices, in, version ); \
  modifiers.modifiers.indices.resize(modifiers_modifiers_numIndices); \
  for (uint i1 = 0; i1 < modifiers_modifiers_numIndices; i1++) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \

#define NI_PARTICLE_SYSTEM_WRITE \
NiParticles::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( modifiers.hasModifiers, out, version ); \
  uint modifiers_modifiers_numIndices; \
  modifiers_modifiers_numIndices = uint(modifiers.modifiers.numIndices.size()); \
  NifStream( modifiers_modifiers_numIndices, out, version ); \
  for (uint i1 = 0; i1 < modifiers_modifiers_numIndices; i1++) { \
    NifStream( link_map[modifiers.modifiers.indices[i1]], out, version ); \
  }; \
}; \

#define NI_PARTICLE_SYSTEM_STRING \
stringstream out; \
out << NiParticles::asString(); \
out << "Has Modifiers:  " << modifiers.hasModifiers << endl; \
out << "Num Indices:  " << modifiers_modifiers_numIndices << endl; \
for (uint i0 = 0; i0 < modifiers_modifiers_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << modifiers.modifiers.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_PARTICLE_SYSTEM_FIXLINKS \
NiParticles::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  for (uint i1 = 0; i1 < modifiers_modifiers_numIndices; i1++) { \
    modifiers.modifiers.indices[i1] = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \

#define NI_PARTICLE_SYSTEM_CONTROLLER_MEMBERS \

#define NI_PARTICLE_SYSTEM_CONTROLLER_PARENTS AParticleSystemController \

#define NI_PARTICLE_SYSTEM_CONTROLLER_CONSTRUCT \

#define NI_PARTICLE_SYSTEM_CONTROLLER_READ \
AParticleSystemController::Read( in, link_stack, version ); \

#define NI_PARTICLE_SYSTEM_CONTROLLER_WRITE \
AParticleSystemController::Write( out, link_map, version ); \

#define NI_PARTICLE_SYSTEM_CONTROLLER_STRING \
stringstream out; \
out << AParticleSystemController::asString(); \
return out.str(); \

#define NI_PARTICLE_SYSTEM_CONTROLLER_FIXLINKS \
AParticleSystemController::FixLinks( objects, link_stack, version ); \

#define NI_PATH_CONTROLLER_MEMBERS \
ushort unknownShort2; \
uint unknownInt1; \
uint unknownInt2; \
uint unknownInt3; \
ushort unknownShort; \
Ref<NiPosData > posData; \
Ref<NiFloatData > floatData; \

#define NI_PATH_CONTROLLER_PARENTS NiTimeController \

#define NI_PATH_CONTROLLER_CONSTRUCT \

#define NI_PATH_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknownShort2, in, version ); \
}; \
NifStream( unknownInt1, in, version ); \
NifStream( unknownInt2, in, version ); \
NifStream( unknownInt3, in, version ); \
NifStream( unknownShort, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_PATH_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( unknownShort2, out, version ); \
}; \
NifStream( unknownInt1, out, version ); \
NifStream( unknownInt2, out, version ); \
NifStream( unknownInt3, out, version ); \
NifStream( unknownShort, out, version ); \
NifStream( link_map[posData], out, version ); \
NifStream( link_map[floatData], out, version ); \

#define NI_PATH_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Short 2:  " << unknownShort2 << endl; \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
out << "Unknown Int 2:  " << unknownInt2 << endl; \
out << "Unknown Int 3:  " << unknownInt3 << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Pos Data:  " << posData << endl; \
out << "Float Data:  " << floatData << endl; \
return out.str(); \

#define NI_PATH_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
}; \
posData = blocks[link_stack.front()]; \
link_stack.pop_front(); \
floatData = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_PATH_INTERPOLATOR_MEMBERS \
float unknownFloat1; \
float unknownFloat2; \
ushort unknownShort2; \
Ref<NiPosData > posData; \
Ref<NiFloatData > floatData; \

#define NI_PATH_INTERPOLATOR_PARENTS ABlendInterpolator \

#define NI_PATH_INTERPOLATOR_CONSTRUCT \

#define NI_PATH_INTERPOLATOR_READ \
uint block_num; \
ABlendInterpolator::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \
NifStream( unknownShort2, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_PATH_INTERPOLATOR_WRITE \
ABlendInterpolator::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \
NifStream( unknownShort2, out, version ); \
NifStream( link_map[posData], out, version ); \
NifStream( link_map[floatData], out, version ); \

#define NI_PATH_INTERPOLATOR_STRING \
stringstream out; \
out << ABlendInterpolator::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Short 2:  " << unknownShort2 << endl; \
out << "Pos Data:  " << posData << endl; \
out << "Float Data:  " << floatData << endl; \
return out.str(); \

#define NI_PATH_INTERPOLATOR_FIXLINKS \
ABlendInterpolator::FixLinks( objects, link_stack, version ); \
posData = blocks[link_stack.front()]; \
link_stack.pop_front(); \
floatData = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_PIXEL_DATA_MEMBERS \
PixelFormat pixelFormat; \
uint redMask; \
uint greenMask; \
uint blueMask; \
uint alphaMask; \
uint bitsPerPixel; \
vector<byte > unknown8Bytes; \
uint unknownInt; \
vector<byte > unknown54Bytes; \
Ref<NiPalette > palette; \
uint bytesPerPixel; \
vector<MipMap > mipmaps; \
ByteArray pixelData; \

#define NI_PIXEL_DATA_PARENTS NiObject \

#define NI_PIXEL_DATA_CONSTRUCT \

#define NI_PIXEL_DATA_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
uint numMipmaps; \
NifStream( pixelFormat, in, version ); \
if ( version <= 0x0A020000 ) { \
  NifStream( redMask, in, version ); \
  NifStream( greenMask, in, version ); \
  NifStream( blueMask, in, version ); \
  NifStream( alphaMask, in, version ); \
  NifStream( bitsPerPixel, in, version ); \
  unknown8Bytes.resize(8); \
  for (uint i1 = 0; i1 < 8; i1++) { \
    NifStream( unknown8Bytes[i1], in, version ); \
  }; \
}; \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A020000 ) ) { \
  NifStream( unknownInt, in, version ); \
}; \
if ( version >= 0x14000004 ) { \
  unknown54Bytes.resize(54); \
  for (uint i1 = 0; i1 < 54; i1++) { \
    NifStream( unknown54Bytes[i1], in, version ); \
  }; \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( numMipmaps, in, version ); \
NifStream( bytesPerPixel, in, version ); \
mipmaps.resize(numMipmaps); \
for (uint i0 = 0; i0 < numMipmaps; i0++) { \
  NifStream( mipmaps[i0].width, in, version ); \
  NifStream( mipmaps[i0].height, in, version ); \
  NifStream( mipmaps[i0].offset, in, version ); \
}; \
uint pixelData_size; \
NifStream( pixelData_size, in, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( pixelData.unknownInt, in, version ); \
}; \
pixelData.data.resize(pixelData_size); \
for (uint i0 = 0; i0 < pixelData_size; i0++) { \
  NifStream( pixelData.data[i0], in, version ); \
}; \

#define NI_PIXEL_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
uint numMipmaps; \
numMipmaps = uint(numMipmaps.size()); \
NifStream( pixelFormat, out, version ); \
if ( version <= 0x0A020000 ) { \
  NifStream( redMask, out, version ); \
  NifStream( greenMask, out, version ); \
  NifStream( blueMask, out, version ); \
  NifStream( alphaMask, out, version ); \
  NifStream( bitsPerPixel, out, version ); \
  for (uint i1 = 0; i1 < 8; i1++) { \
    NifStream( unknown8Bytes[i1], out, version ); \
  }; \
}; \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A020000 ) ) { \
  NifStream( unknownInt, out, version ); \
}; \
if ( version >= 0x14000004 ) { \
  for (uint i1 = 0; i1 < 54; i1++) { \
    NifStream( unknown54Bytes[i1], out, version ); \
  }; \
}; \
NifStream( link_map[palette], out, version ); \
NifStream( numMipmaps, out, version ); \
NifStream( bytesPerPixel, out, version ); \
for (uint i0 = 0; i0 < numMipmaps; i0++) { \
  NifStream( mipmaps[i0].width, out, version ); \
  NifStream( mipmaps[i0].height, out, version ); \
  NifStream( mipmaps[i0].offset, out, version ); \
}; \
uint pixelData_size; \
pixelData_size = uint(pixelData.size.size()); \
NifStream( pixelData_size, out, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( pixelData.unknownInt, out, version ); \
}; \
for (uint i0 = 0; i0 < pixelData_size; i0++) { \
  NifStream( pixelData.data[i0], out, version ); \
}; \

#define NI_PIXEL_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Pixel Format:  " << pixelFormat << endl; \
out << "Red Mask:  " << redMask << endl; \
out << "Green Mask:  " << greenMask << endl; \
out << "Blue Mask:  " << blueMask << endl; \
out << "Alpha Mask:  " << alphaMask << endl; \
out << "Bits Per Pixel:  " << bitsPerPixel << endl; \
for (uint i0 = 0; i0 < 8; i0++) { \
  out << "  Unknown 8 Bytes[" << i0 << "]:  " << unknown8Bytes[i0] << endl; \
}; \
out << "Unknown Int:  " << unknownInt << endl; \
for (uint i0 = 0; i0 < 54; i0++) { \
  out << "  Unknown 54 Bytes[" << i0 << "]:  " << unknown54Bytes[i0] << endl; \
}; \
out << "Palette:  " << palette << endl; \
out << "Num Mipmaps:  " << numMipmaps << endl; \
out << "Bytes Per Pixel:  " << bytesPerPixel << endl; \
for (uint i0 = 0; i0 < numMipmaps; i0++) { \
  out << "  Width:  " << mipmaps[i0].width << endl; \
  out << "  Height:  " << mipmaps[i0].height << endl; \
  out << "  Offset:  " << mipmaps[i0].offset << endl; \
}; \
out << "Size:  " << pixelData_size << endl; \
out << "Unknown Int:  " << pixelData.unknownInt << endl; \
for (uint i0 = 0; i0 < pixelData_size; i0++) { \
  out << "  Data[" << i0 << "]:  " << pixelData.data[i0] << endl; \
}; \
return out.str(); \

#define NI_PIXEL_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A020000 ) { \
  for (uint i1 = 0; i1 < 8; i1++) { \
  }; \
}; \
if ( ( version >= 0x0A010000 ) && ( version <= 0x0A020000 ) ) { \
}; \
if ( version >= 0x14000004 ) { \
  for (uint i1 = 0; i1 < 54; i1++) { \
  }; \
}; \
palette = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < numMipmaps; i0++) { \
}; \
if ( version >= 0x14000004 ) { \
}; \
for (uint i0 = 0; i0 < pixelData_size; i0++) { \
}; \

#define NI_PLANAR_COLLIDER_MEMBERS \
ushort unknownShort; \
float unknownFloat1; \
float unknownFloat2; \
ushort unknownShort2; \
float unknownFloat3; \
float unknownFloat4; \
float unknownFloat5; \
float unknownFloat6; \
float unknownFloat7; \
float unknownFloat8; \
float unknownFloat9; \
float unknownFloat10; \
float unknownFloat11; \
float unknownFloat12; \
float unknownFloat13; \
float unknownFloat14; \
float unknownFloat15; \
float unknownFloat16; \

#define NI_PLANAR_COLLIDER_PARENTS AParticleModifier \

#define NI_PLANAR_COLLIDER_CONSTRUCT \

#define NI_PLANAR_COLLIDER_READ \
AParticleModifier::Read( in, link_stack, version ); \
if ( version >= 0x0A000100 ) { \
  NifStream( unknownShort, in, version ); \
}; \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownFloat2, in, version ); \
if ( ( version >= 0x04020200 ) && ( version <= 0x04020200 ) ) { \
  NifStream( unknownShort2, in, version ); \
}; \
NifStream( unknownFloat3, in, version ); \
NifStream( unknownFloat4, in, version ); \
NifStream( unknownFloat5, in, version ); \
NifStream( unknownFloat6, in, version ); \
NifStream( unknownFloat7, in, version ); \
NifStream( unknownFloat8, in, version ); \
NifStream( unknownFloat9, in, version ); \
NifStream( unknownFloat10, in, version ); \
NifStream( unknownFloat11, in, version ); \
NifStream( unknownFloat12, in, version ); \
NifStream( unknownFloat13, in, version ); \
NifStream( unknownFloat14, in, version ); \
NifStream( unknownFloat15, in, version ); \
NifStream( unknownFloat16, in, version ); \

#define NI_PLANAR_COLLIDER_WRITE \
AParticleModifier::Write( out, link_map, version ); \
if ( version >= 0x0A000100 ) { \
  NifStream( unknownShort, out, version ); \
}; \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownFloat2, out, version ); \
if ( ( version >= 0x04020200 ) && ( version <= 0x04020200 ) ) { \
  NifStream( unknownShort2, out, version ); \
}; \
NifStream( unknownFloat3, out, version ); \
NifStream( unknownFloat4, out, version ); \
NifStream( unknownFloat5, out, version ); \
NifStream( unknownFloat6, out, version ); \
NifStream( unknownFloat7, out, version ); \
NifStream( unknownFloat8, out, version ); \
NifStream( unknownFloat9, out, version ); \
NifStream( unknownFloat10, out, version ); \
NifStream( unknownFloat11, out, version ); \
NifStream( unknownFloat12, out, version ); \
NifStream( unknownFloat13, out, version ); \
NifStream( unknownFloat14, out, version ); \
NifStream( unknownFloat15, out, version ); \
NifStream( unknownFloat16, out, version ); \

#define NI_PLANAR_COLLIDER_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Short 2:  " << unknownShort2 << endl; \
out << "Unknown Float 3:  " << unknownFloat3 << endl; \
out << "Unknown Float 4:  " << unknownFloat4 << endl; \
out << "Unknown Float 5:  " << unknownFloat5 << endl; \
out << "Unknown Float 6:  " << unknownFloat6 << endl; \
out << "Unknown Float 7:  " << unknownFloat7 << endl; \
out << "Unknown Float 8:  " << unknownFloat8 << endl; \
out << "Unknown Float 9:  " << unknownFloat9 << endl; \
out << "Unknown Float 10:  " << unknownFloat10 << endl; \
out << "Unknown Float 11:  " << unknownFloat11 << endl; \
out << "Unknown Float 12:  " << unknownFloat12 << endl; \
out << "Unknown Float 13:  " << unknownFloat13 << endl; \
out << "Unknown Float 14:  " << unknownFloat14 << endl; \
out << "Unknown Float 15:  " << unknownFloat15 << endl; \
out << "Unknown Float 16:  " << unknownFloat16 << endl; \
return out.str(); \

#define NI_PLANAR_COLLIDER_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A000100 ) { \
}; \
if ( ( version >= 0x04020200 ) && ( version <= 0x04020200 ) ) { \
}; \

#define NI_POINT3_INTERPOLATOR_MEMBERS \
Vector3 point3Value; \
Ref<NiPosData > data; \

#define NI_POINT3_INTERPOLATOR_PARENTS AInterpolator \

#define NI_POINT3_INTERPOLATOR_CONSTRUCT \

#define NI_POINT3_INTERPOLATOR_READ \
uint block_num; \
AInterpolator::Read( in, link_stack, version ); \
NifStream( point3Value, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_POINT3_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( point3Value, out, version ); \
NifStream( link_map[data], out, version ); \

#define NI_POINT3_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Point 3 Value:  " << point3Value << endl; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_POINT3_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_POINT_LIGHT_MEMBERS \

#define NI_POINT_LIGHT_PARENTS APointLight \

#define NI_POINT_LIGHT_CONSTRUCT \

#define NI_POINT_LIGHT_READ \
APointLight::Read( in, link_stack, version ); \

#define NI_POINT_LIGHT_WRITE \
APointLight::Write( out, link_map, version ); \

#define NI_POINT_LIGHT_STRING \
stringstream out; \
out << APointLight::asString(); \
return out.str(); \

#define NI_POINT_LIGHT_FIXLINKS \
APointLight::FixLinks( objects, link_stack, version ); \

#define NI_POS_DATA_MEMBERS \
VectorKeyArray<Vector3 > data; \

#define NI_POS_DATA_PARENTS AKeyedData \

#define NI_POS_DATA_CONSTRUCT \

#define NI_POS_DATA_READ \
AKeyedData::Read( in, link_stack, version ); \
NifStream( data.numKeys, in, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, in, version ); \
}; \
data.keys.resize(data.numKeys); \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], in, version ); \
}; \

#define NI_POS_DATA_WRITE \
AKeyedData::Write( out, link_map, version ); \
NifStream( data.numKeys, out, version ); \
if ( data.numKeys != 0 ) { \
  NifStream( data.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  NifStream( data.keys[i0], out, version ); \
}; \

#define NI_POS_DATA_STRING \
stringstream out; \
out << AKeyedData::asString(); \
out << "Num Keys:  " << data.numKeys << endl; \
if ( data.numKeys != 0 ) { \
  out << "  Key Type:  " << data.keyType << endl; \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << data.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_POS_DATA_FIXLINKS \
AKeyedData::FixLinks( objects, link_stack, version ); \
if ( data.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < data.numKeys; i0++) { \
}; \

#define NI_P_SYS_AGE_DEATH_MODIFIER_MEMBERS \
bool spawnOnDeath; \
Ref<NiPSysSpawnModifier > spawnModifier; \

#define NI_P_SYS_AGE_DEATH_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_AGE_DEATH_MODIFIER_CONSTRUCT \

#define NI_P_SYS_AGE_DEATH_MODIFIER_READ \
uint block_num; \
APSysModifier::Read( in, link_stack, version ); \
NifStream( spawnOnDeath, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_P_SYS_AGE_DEATH_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( spawnOnDeath, out, version ); \
NifStream( link_map[spawnModifier], out, version ); \

#define NI_P_SYS_AGE_DEATH_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Spawn on Death:  " << spawnOnDeath << endl; \
out << "Spawn Modifier:  " << spawnModifier << endl; \
return out.str(); \

#define NI_P_SYS_AGE_DEATH_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
spawnModifier = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_P_SYS_BOMB_MODIFIER_MEMBERS \
NiNode * unknownLink; \
vector<uint > unknownInts1; \
vector<float > unknownFloats; \
vector<uint > unknownInts2; \

#define NI_P_SYS_BOMB_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_BOMB_MODIFIER_CONSTRUCT \

#define NI_P_SYS_BOMB_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
unknownInts1.resize(2); \
for (uint i0 = 0; i0 < 2; i0++) { \
  NifStream( unknownInts1[i0], in, version ); \
}; \
unknownFloats.resize(3); \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \
unknownInts2.resize(2); \
for (uint i0 = 0; i0 < 2; i0++) { \
  NifStream( unknownInts2[i0], in, version ); \
}; \

#define NI_P_SYS_BOMB_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( link_map[unknownLink], out, version ); \
for (uint i0 = 0; i0 < 2; i0++) { \
  NifStream( unknownInts1[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \
for (uint i0 = 0; i0 < 2; i0++) { \
  NifStream( unknownInts2[i0], out, version ); \
}; \

#define NI_P_SYS_BOMB_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Unknown Link:  " << unknownLink << endl; \
for (uint i0 = 0; i0 < 2; i0++) { \
  out << "  Unknown Ints 1[" << i0 << "]:  " << unknownInts1[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
for (uint i0 = 0; i0 < 2; i0++) { \
  out << "  Unknown Ints 2[" << i0 << "]:  " << unknownInts2[i0] << endl; \
}; \
return out.str(); \

#define NI_P_SYS_BOMB_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
unknownLink = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < 2; i0++) { \
}; \
for (uint i0 = 0; i0 < 3; i0++) { \
}; \
for (uint i0 = 0; i0 < 2; i0++) { \
}; \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_MEMBERS \
ushort updateSkip; \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_CONSTRUCT \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( updateSkip, in, version ); \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( updateSkip, out, version ); \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Update Skip:  " << updateSkip << endl; \
return out.str(); \

#define NI_P_SYS_BOUND_UPDATE_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_BOX_EMITTER_MEMBERS \
float width; \
float height; \
float depth; \

#define NI_P_SYS_BOX_EMITTER_PARENTS APSysVolumeEmitter \

#define NI_P_SYS_BOX_EMITTER_CONSTRUCT \

#define NI_P_SYS_BOX_EMITTER_READ \
APSysVolumeEmitter::Read( in, link_stack, version ); \
NifStream( width, in, version ); \
NifStream( height, in, version ); \
NifStream( depth, in, version ); \

#define NI_P_SYS_BOX_EMITTER_WRITE \
APSysVolumeEmitter::Write( out, link_map, version ); \
NifStream( width, out, version ); \
NifStream( height, out, version ); \
NifStream( depth, out, version ); \

#define NI_P_SYS_BOX_EMITTER_STRING \
stringstream out; \
out << APSysVolumeEmitter::asString(); \
out << "Width:  " << width << endl; \
out << "Height:  " << height << endl; \
out << "Depth:  " << depth << endl; \
return out.str(); \

#define NI_P_SYS_BOX_EMITTER_FIXLINKS \
APSysVolumeEmitter::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_COLLIDER_MANAGER_MEMBERS \
Ref<NiPSysPlanarCollider > collider; \

#define NI_P_SYS_COLLIDER_MANAGER_PARENTS APSysModifier \

#define NI_P_SYS_COLLIDER_MANAGER_CONSTRUCT \

#define NI_P_SYS_COLLIDER_MANAGER_READ \
uint block_num; \
APSysModifier::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_P_SYS_COLLIDER_MANAGER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( link_map[collider], out, version ); \

#define NI_P_SYS_COLLIDER_MANAGER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Collider:  " << collider << endl; \
return out.str(); \

#define NI_P_SYS_COLLIDER_MANAGER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
collider = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_P_SYS_COLOR_MODIFIER_MEMBERS \
Ref<NiColorData > data; \

#define NI_P_SYS_COLOR_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_COLOR_MODIFIER_CONSTRUCT \

#define NI_P_SYS_COLOR_MODIFIER_READ \
uint block_num; \
APSysModifier::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_P_SYS_COLOR_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \

#define NI_P_SYS_COLOR_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_P_SYS_COLOR_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_P_SYS_CYLINDER_EMITTER_MEMBERS \
float radius; \
float height; \

#define NI_P_SYS_CYLINDER_EMITTER_PARENTS APSysVolumeEmitter \

#define NI_P_SYS_CYLINDER_EMITTER_CONSTRUCT \

#define NI_P_SYS_CYLINDER_EMITTER_READ \
APSysVolumeEmitter::Read( in, link_stack, version ); \
NifStream( radius, in, version ); \
NifStream( height, in, version ); \

#define NI_P_SYS_CYLINDER_EMITTER_WRITE \
APSysVolumeEmitter::Write( out, link_map, version ); \
NifStream( radius, out, version ); \
NifStream( height, out, version ); \

#define NI_P_SYS_CYLINDER_EMITTER_STRING \
stringstream out; \
out << APSysVolumeEmitter::asString(); \
out << "Radius:  " << radius << endl; \
out << "Height:  " << height << endl; \
return out.str(); \

#define NI_P_SYS_CYLINDER_EMITTER_FIXLINKS \
APSysVolumeEmitter::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_DATA_MEMBERS \
vector<vector<float > > unknownFloats4; \
bool unknownBool1; \
vector<vector<byte > > unknownBytes; \
vector<vector<byte > > unknownBytesAlt; \
byte unknownByte3; \
bool unknownBool2; \
vector<vector<byte > > unknownBytes2; \
uint unknownInt1; \

#define NI_P_SYS_DATA_PARENTS APSysData \

#define NI_P_SYS_DATA_CONSTRUCT \

#define NI_P_SYS_DATA_READ \
APSysData::Read( in, link_stack, version ); \
if ( version <= 0x0A020000 ) { \
  unknownFloats4.resize(numVertices); \
  for (uint i1 = 0; i1 < numVertices; i1++) \
    unknownFloats4[i1].resize(10); \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 10; i2++) { \
      NifStream( unknownFloats4[i1][i2], in, version ); \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  NifStream( unknownBool1, in, version ); \
  if ( unknownBool1 != 0 ) { \
    unknownBytes.resize(numVertices); \
    for (uint i2 = 0; i2 < numVertices; i2++) \
      unknownBytes[i2].resize(32); \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 32; i3++) { \
        NifStream( unknownBytes[i2][i3], in, version ); \
      }; \
    }; \
  }; \
  if ( unknownBool1 == 0 ) { \
    unknownBytesAlt.resize(numVertices); \
    for (uint i2 = 0; i2 < numVertices; i2++) \
      unknownBytesAlt[i2].resize(28); \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 28; i3++) { \
        NifStream( unknownBytesAlt[i2][i3], in, version ); \
      }; \
    }; \
  }; \
  NifStream( unknownByte3, in, version ); \
  NifStream( unknownBool2, in, version ); \
  if ( unknownBool2 != 0 ) { \
    unknownBytes2.resize(numVertices); \
    for (uint i2 = 0; i2 < numVertices; i2++) \
      unknownBytes2[i2].resize(4); \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 4; i3++) { \
        NifStream( unknownBytes2[i2][i3], in, version ); \
      }; \
    }; \
  }; \
}; \
NifStream( unknownInt1, in, version ); \

#define NI_P_SYS_DATA_WRITE \
APSysData::Write( out, link_map, version ); \
if ( version <= 0x0A020000 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 10; i2++) { \
      NifStream( unknownFloats4[i1][i2], out, version ); \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  NifStream( unknownBool1, out, version ); \
  if ( unknownBool1 != 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 32; i3++) { \
        NifStream( unknownBytes[i2][i3], out, version ); \
      }; \
    }; \
  }; \
  if ( unknownBool1 == 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 28; i3++) { \
        NifStream( unknownBytesAlt[i2][i3], out, version ); \
      }; \
    }; \
  }; \
  NifStream( unknownByte3, out, version ); \
  NifStream( unknownBool2, out, version ); \
  if ( unknownBool2 != 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 4; i3++) { \
        NifStream( unknownBytes2[i2][i3], out, version ); \
      }; \
    }; \
  }; \
}; \
NifStream( unknownInt1, out, version ); \

#define NI_P_SYS_DATA_STRING \
stringstream out; \
out << APSysData::asString(); \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  for (uint i1 = 0; i1 < 10; i1++) { \
    out << "    Unknown Floats 4[" << i0 << "][" << i1 << "]:  " << unknownFloats4[i0][i1] << endl; \
  }; \
}; \
out << "Unknown Bool 1:  " << unknownBool1 << endl; \
if ( unknownBool1 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 32; i2++) { \
      out << "      Unknown Bytes[" << i1 << "][" << i2 << "]:  " << unknownBytes[i1][i2] << endl; \
    }; \
  }; \
}; \
if ( unknownBool1 == 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 28; i2++) { \
      out << "      Unknown Bytes Alt[" << i1 << "][" << i2 << "]:  " << unknownBytesAlt[i1][i2] << endl; \
    }; \
  }; \
}; \
out << "Unknown Byte 3:  " << unknownByte3 << endl; \
out << "Unknown Bool 2:  " << unknownBool2 << endl; \
if ( unknownBool2 != 0 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 4; i2++) { \
      out << "      Unknown Bytes 2[" << i1 << "][" << i2 << "]:  " << unknownBytes2[i1][i2] << endl; \
    }; \
  }; \
}; \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
return out.str(); \

#define NI_P_SYS_DATA_FIXLINKS \
APSysData::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A020000 ) { \
  for (uint i1 = 0; i1 < numVertices; i1++) { \
    for (uint i2 = 0; i2 < 10; i2++) { \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  if ( unknownBool1 != 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 32; i3++) { \
      }; \
    }; \
  }; \
  if ( unknownBool1 == 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 28; i3++) { \
      }; \
    }; \
  }; \
  if ( unknownBool2 != 0 ) { \
    for (uint i2 = 0; i2 < numVertices; i2++) { \
      for (uint i3 = 0; i3 < 4; i3++) { \
      }; \
    }; \
  }; \
}; \

#define NI_P_SYS_DRAG_MODIFIER_MEMBERS \
uint parent; \
Vector3 dragAxis; \
float percentage; \
float range; \
float rangeFalloff; \

#define NI_P_SYS_DRAG_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_DRAG_MODIFIER_CONSTRUCT \

#define NI_P_SYS_DRAG_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( parent, in, version ); \
NifStream( dragAxis, in, version ); \
NifStream( percentage, in, version ); \
NifStream( range, in, version ); \
NifStream( rangeFalloff, in, version ); \

#define NI_P_SYS_DRAG_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( parent, out, version ); \
NifStream( dragAxis, out, version ); \
NifStream( percentage, out, version ); \
NifStream( range, out, version ); \
NifStream( rangeFalloff, out, version ); \

#define NI_P_SYS_DRAG_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Parent:  " << parent << endl; \
out << "Drag Axis:  " << dragAxis << endl; \
out << "Percentage:  " << percentage << endl; \
out << "Range:  " << range << endl; \
out << "Range Falloff:  " << rangeFalloff << endl; \
return out.str(); \

#define NI_P_SYS_DRAG_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_EMITTER_CTLR_MEMBERS \
Ref<NiPSysData > data; \

#define NI_P_SYS_EMITTER_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_EMITTER_CTLR_CONSTRUCT \

#define NI_P_SYS_EMITTER_CTLR_READ \
uint block_num; \
APSysCtlr::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_P_SYS_EMITTER_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \

#define NI_P_SYS_EMITTER_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_P_SYS_EMITTER_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_P_SYS_EMITTER_CTLR_DATA_MEMBERS \
VectorKeyArray<float > floatKeys_; \
KeyArray<byte > visibilityKeys_; \

#define NI_P_SYS_EMITTER_CTLR_DATA_PARENTS NiObject \

#define NI_P_SYS_EMITTER_CTLR_DATA_CONSTRUCT \

#define NI_P_SYS_EMITTER_CTLR_DATA_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( floatKeys_.numKeys, in, version ); \
if ( floatKeys_.numKeys != 0 ) { \
  NifStream( floatKeys_.keyType, in, version ); \
}; \
floatKeys_.keys.resize(floatKeys_.numKeys); \
for (uint i0 = 0; i0 < floatKeys_.numKeys; i0++) { \
  NifStream( floatKeys_.keys[i0], in, version ); \
}; \
uint visibilityKeys__numKeys; \
NifStream( visibilityKeys__numKeys, in, version ); \
visibilityKeys_.keys.resize(visibilityKeys__numKeys); \
for (uint i0 = 0; i0 < visibilityKeys__numKeys; i0++) { \
  NifStream( visibilityKeys_.keys[i0], in, version ); \
}; \

#define NI_P_SYS_EMITTER_CTLR_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( floatKeys_.numKeys, out, version ); \
if ( floatKeys_.numKeys != 0 ) { \
  NifStream( floatKeys_.keyType, out, version ); \
}; \
for (uint i0 = 0; i0 < floatKeys_.numKeys; i0++) { \
  NifStream( floatKeys_.keys[i0], out, version ); \
}; \
uint visibilityKeys__numKeys; \
visibilityKeys__numKeys = uint(visibilityKeys_.numKeys.size()); \
NifStream( visibilityKeys__numKeys, out, version ); \
for (uint i0 = 0; i0 < visibilityKeys__numKeys; i0++) { \
  NifStream( visibilityKeys_.keys[i0], out, version ); \
}; \

#define NI_P_SYS_EMITTER_CTLR_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Num Keys:  " << floatKeys_.numKeys << endl; \
if ( floatKeys_.numKeys != 0 ) { \
  out << "  Key Type:  " << floatKeys_.keyType << endl; \
}; \
for (uint i0 = 0; i0 < floatKeys_.numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << floatKeys_.keys[i0] << endl; \
}; \
out << "Num Keys:  " << visibilityKeys__numKeys << endl; \
for (uint i0 = 0; i0 < visibilityKeys__numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << visibilityKeys_.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_P_SYS_EMITTER_CTLR_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( floatKeys_.numKeys != 0 ) { \
}; \
for (uint i0 = 0; i0 < floatKeys_.numKeys; i0++) { \
}; \
for (uint i0 = 0; i0 < visibilityKeys__numKeys; i0++) { \
}; \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_MEMBERS \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_CONSTRUCT \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_EMITTER_DECLINATION_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_MEMBERS \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_CONSTRUCT \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_EMITTER_DECLINATION_VAR_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_MEMBERS \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_CONSTRUCT \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_EMITTER_INITIAL_RADIUS_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_MEMBERS \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_CONSTRUCT \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_EMITTER_LIFE_SPAN_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_EMITTER_SPEED_CTLR_MEMBERS \

#define NI_P_SYS_EMITTER_SPEED_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_EMITTER_SPEED_CTLR_CONSTRUCT \

#define NI_P_SYS_EMITTER_SPEED_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_EMITTER_SPEED_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_EMITTER_SPEED_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_EMITTER_SPEED_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_GRAVITY_MODIFIER_MEMBERS \
NiNode * gravityObject; \
Vector3 gravityAxis; \
float decay; \
float strength; \
uint forceType; \
float turbulence; \
float turbulenceScale; \

#define NI_P_SYS_GRAVITY_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_GRAVITY_MODIFIER_CONSTRUCT \
 : turbulenceScale(1.0) \

#define NI_P_SYS_GRAVITY_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( gravityAxis, in, version ); \
NifStream( decay, in, version ); \
NifStream( strength, in, version ); \
NifStream( forceType, in, version ); \
NifStream( turbulence, in, version ); \
NifStream( turbulenceScale, in, version ); \

#define NI_P_SYS_GRAVITY_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( link_map[gravityObject], out, version ); \
NifStream( gravityAxis, out, version ); \
NifStream( decay, out, version ); \
NifStream( strength, out, version ); \
NifStream( forceType, out, version ); \
NifStream( turbulence, out, version ); \
NifStream( turbulenceScale, out, version ); \

#define NI_P_SYS_GRAVITY_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Gravity Object:  " << gravityObject << endl; \
out << "Gravity Axis:  " << gravityAxis << endl; \
out << "Decay:  " << decay << endl; \
out << "Strength:  " << strength << endl; \
out << "Force Type:  " << forceType << endl; \
out << "Turbulence:  " << turbulence << endl; \
out << "Turbulence Scale:  " << turbulenceScale << endl; \
return out.str(); \

#define NI_P_SYS_GRAVITY_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
gravityObject = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_MEMBERS \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_CONSTRUCT \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_GRAVITY_STRENGTH_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_GROW_FADE_MODIFIER_MEMBERS \
float growTime; \
ushort growGeneration; \
float fadeTime; \
ushort fadeGeneration; \

#define NI_P_SYS_GROW_FADE_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_GROW_FADE_MODIFIER_CONSTRUCT \

#define NI_P_SYS_GROW_FADE_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( growTime, in, version ); \
NifStream( growGeneration, in, version ); \
NifStream( fadeTime, in, version ); \
NifStream( fadeGeneration, in, version ); \

#define NI_P_SYS_GROW_FADE_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( growTime, out, version ); \
NifStream( growGeneration, out, version ); \
NifStream( fadeTime, out, version ); \
NifStream( fadeGeneration, out, version ); \

#define NI_P_SYS_GROW_FADE_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Grow Time:  " << growTime << endl; \
out << "Grow Generation:  " << growGeneration << endl; \
out << "Fade Time:  " << fadeTime << endl; \
out << "Fade Generation:  " << fadeGeneration << endl; \
return out.str(); \

#define NI_P_SYS_GROW_FADE_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_MESH_EMITTER_MEMBERS \
LinkGroup emitterMeshes; \
uint initialVelocityType; \
uint emissionType; \
Vector3 emissionAxis; \

#define NI_P_SYS_MESH_EMITTER_PARENTS APSysEmitter \

#define NI_P_SYS_MESH_EMITTER_CONSTRUCT \

#define NI_P_SYS_MESH_EMITTER_READ \
uint block_num; \
APSysEmitter::Read( in, link_stack, version ); \
uint emitterMeshes_numIndices; \
NifStream( emitterMeshes_numIndices, in, version ); \
emitterMeshes.indices.resize(emitterMeshes_numIndices); \
for (uint i0 = 0; i0 < emitterMeshes_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( initialVelocityType, in, version ); \
NifStream( emissionType, in, version ); \
NifStream( emissionAxis, in, version ); \

#define NI_P_SYS_MESH_EMITTER_WRITE \
APSysEmitter::Write( out, link_map, version ); \
uint emitterMeshes_numIndices; \
emitterMeshes_numIndices = uint(emitterMeshes.numIndices.size()); \
NifStream( emitterMeshes_numIndices, out, version ); \
for (uint i0 = 0; i0 < emitterMeshes_numIndices; i0++) { \
  NifStream( link_map[emitterMeshes.indices[i0]], out, version ); \
}; \
NifStream( initialVelocityType, out, version ); \
NifStream( emissionType, out, version ); \
NifStream( emissionAxis, out, version ); \

#define NI_P_SYS_MESH_EMITTER_STRING \
stringstream out; \
out << APSysEmitter::asString(); \
out << "Num Indices:  " << emitterMeshes_numIndices << endl; \
for (uint i0 = 0; i0 < emitterMeshes_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << emitterMeshes.indices[i0] << endl; \
}; \
out << "Initial Velocity Type:  " << initialVelocityType << endl; \
out << "Emission Type:  " << emissionType << endl; \
out << "Emission Axis:  " << emissionAxis << endl; \
return out.str(); \

#define NI_P_SYS_MESH_EMITTER_FIXLINKS \
APSysEmitter::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < emitterMeshes_numIndices; i0++) { \
  emitterMeshes.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_MEMBERS \
LinkGroup meshes; \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_CONSTRUCT \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_READ \
uint block_num; \
APSysModifier::Read( in, link_stack, version ); \
uint meshes_numIndices; \
NifStream( meshes_numIndices, in, version ); \
meshes.indices.resize(meshes_numIndices); \
for (uint i0 = 0; i0 < meshes_numIndices; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
uint meshes_numIndices; \
meshes_numIndices = uint(meshes.numIndices.size()); \
NifStream( meshes_numIndices, out, version ); \
for (uint i0 = 0; i0 < meshes_numIndices; i0++) { \
  NifStream( link_map[meshes.indices[i0]], out, version ); \
}; \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Num Indices:  " << meshes_numIndices << endl; \
for (uint i0 = 0; i0 < meshes_numIndices; i0++) { \
  out << "  Indices[" << i0 << "]:  " << meshes.indices[i0] << endl; \
}; \
return out.str(); \

#define NI_P_SYS_MESH_UPDATE_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < meshes_numIndices; i0++) { \
  meshes.indices[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_MEMBERS \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_PARENTS APSysCtlr \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_CONSTRUCT \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_READ \
APSysCtlr::Read( in, link_stack, version ); \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_WRITE \
APSysCtlr::Write( out, link_map, version ); \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_STRING \
stringstream out; \
out << APSysCtlr::asString(); \
return out.str(); \

#define NI_P_SYS_MODIFIER_ACTIVE_CTLR_FIXLINKS \
APSysCtlr::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_PLANAR_COLLIDER_MEMBERS \
float bounce; \
bool spawnOnCollide; \
bool dieOnCollide; \
Ref<NiPSysSpawnModifier > spawnModifier; \
NiObject * parent; \
Ref<NiObject > unknownLink_; \
Ref<NiNode > colliderObject; \
float width; \
float height; \
Vector3 xAxis; \
Vector3 yAxis; \

#define NI_P_SYS_PLANAR_COLLIDER_PARENTS NiObject \

#define NI_P_SYS_PLANAR_COLLIDER_CONSTRUCT \

#define NI_P_SYS_PLANAR_COLLIDER_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
NifStream( bounce, in, version ); \
NifStream( spawnOnCollide, in, version ); \
NifStream( dieOnCollide, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( width, in, version ); \
NifStream( height, in, version ); \
NifStream( xAxis, in, version ); \
NifStream( yAxis, in, version ); \

#define NI_P_SYS_PLANAR_COLLIDER_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( bounce, out, version ); \
NifStream( spawnOnCollide, out, version ); \
NifStream( dieOnCollide, out, version ); \
NifStream( link_map[spawnModifier], out, version ); \
NifStream( link_map[parent], out, version ); \
NifStream( link_map[unknownLink_], out, version ); \
NifStream( link_map[colliderObject], out, version ); \
NifStream( width, out, version ); \
NifStream( height, out, version ); \
NifStream( xAxis, out, version ); \
NifStream( yAxis, out, version ); \

#define NI_P_SYS_PLANAR_COLLIDER_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Bounce:  " << bounce << endl; \
out << "Spawn on Collide:  " << spawnOnCollide << endl; \
out << "Die on Collide:  " << dieOnCollide << endl; \
out << "Spawn Modifier:  " << spawnModifier << endl; \
out << "Parent:  " << parent << endl; \
out << "Unknown Link?:  " << unknownLink_ << endl; \
out << "Collider Object:  " << colliderObject << endl; \
out << "Width:  " << width << endl; \
out << "Height:  " << height << endl; \
out << "X Axis:  " << xAxis << endl; \
out << "Y Axis:  " << yAxis << endl; \
return out.str(); \

#define NI_P_SYS_PLANAR_COLLIDER_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
spawnModifier = blocks[link_stack.front()]; \
link_stack.pop_front(); \
parent = blocks[link_stack.front()]; \
link_stack.pop_front(); \
unknownLink_ = blocks[link_stack.front()]; \
link_stack.pop_front(); \
colliderObject = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_P_SYS_POSITION_MODIFIER_MEMBERS \

#define NI_P_SYS_POSITION_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_POSITION_MODIFIER_CONSTRUCT \

#define NI_P_SYS_POSITION_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \

#define NI_P_SYS_POSITION_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \

#define NI_P_SYS_POSITION_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
return out.str(); \

#define NI_P_SYS_POSITION_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_MEMBERS \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_PARENTS NiTimeController \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_CONSTRUCT \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_READ \
NiTimeController::Read( in, link_stack, version ); \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_WRITE \
NiTimeController::Write( out, link_map, version ); \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_STRING \
stringstream out; \
out << NiTimeController::asString(); \
return out.str(); \

#define NI_P_SYS_RESET_ON_LOOP_CTLR_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_ROTATION_MODIFIER_MEMBERS \
float initialRotationSpeed; \
float initialRotationSpeedVariation; \
float initialRotationAngle; \
float initialRotationAngleVariation; \
bool randomRotSpeedSign; \
bool randomInitialAxis; \
Vector3 initialAxis; \

#define NI_P_SYS_ROTATION_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_ROTATION_MODIFIER_CONSTRUCT \

#define NI_P_SYS_ROTATION_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( initialRotationSpeed, in, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( initialRotationSpeedVariation, in, version ); \
  NifStream( initialRotationAngle, in, version ); \
  NifStream( initialRotationAngleVariation, in, version ); \
  NifStream( randomRotSpeedSign, in, version ); \
}; \
NifStream( randomInitialAxis, in, version ); \
NifStream( initialAxis, in, version ); \

#define NI_P_SYS_ROTATION_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( initialRotationSpeed, out, version ); \
if ( version >= 0x14000004 ) { \
  NifStream( initialRotationSpeedVariation, out, version ); \
  NifStream( initialRotationAngle, out, version ); \
  NifStream( initialRotationAngleVariation, out, version ); \
  NifStream( randomRotSpeedSign, out, version ); \
}; \
NifStream( randomInitialAxis, out, version ); \
NifStream( initialAxis, out, version ); \

#define NI_P_SYS_ROTATION_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Initial Rotation Speed:  " << initialRotationSpeed << endl; \
out << "Initial Rotation Speed Variation:  " << initialRotationSpeedVariation << endl; \
out << "Initial Rotation Angle:  " << initialRotationAngle << endl; \
out << "Initial Rotation Angle Variation:  " << initialRotationAngleVariation << endl; \
out << "Random Rot Speed Sign:  " << randomRotSpeedSign << endl; \
out << "Random Initial Axis:  " << randomInitialAxis << endl; \
out << "Initial Axis:  " << initialAxis << endl; \
return out.str(); \

#define NI_P_SYS_ROTATION_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \
if ( version >= 0x14000004 ) { \
}; \

#define NI_P_SYS_SPAWN_MODIFIER_MEMBERS \
ushort numSpawnGenerations; \
float percentageSpawned; \
ushort minNumToSpawn; \
ushort maxNumToSpawn; \
float spawnSpeedChaos; \
float spawnDirChaos; \
float lifeSpan; \
float lifeSpanVariation; \

#define NI_P_SYS_SPAWN_MODIFIER_PARENTS APSysModifier \

#define NI_P_SYS_SPAWN_MODIFIER_CONSTRUCT \

#define NI_P_SYS_SPAWN_MODIFIER_READ \
APSysModifier::Read( in, link_stack, version ); \
NifStream( numSpawnGenerations, in, version ); \
NifStream( percentageSpawned, in, version ); \
NifStream( minNumToSpawn, in, version ); \
NifStream( maxNumToSpawn, in, version ); \
NifStream( spawnSpeedChaos, in, version ); \
NifStream( spawnDirChaos, in, version ); \
NifStream( lifeSpan, in, version ); \
NifStream( lifeSpanVariation, in, version ); \

#define NI_P_SYS_SPAWN_MODIFIER_WRITE \
APSysModifier::Write( out, link_map, version ); \
NifStream( numSpawnGenerations, out, version ); \
NifStream( percentageSpawned, out, version ); \
NifStream( minNumToSpawn, out, version ); \
NifStream( maxNumToSpawn, out, version ); \
NifStream( spawnSpeedChaos, out, version ); \
NifStream( spawnDirChaos, out, version ); \
NifStream( lifeSpan, out, version ); \
NifStream( lifeSpanVariation, out, version ); \

#define NI_P_SYS_SPAWN_MODIFIER_STRING \
stringstream out; \
out << APSysModifier::asString(); \
out << "Num Spawn Generations:  " << numSpawnGenerations << endl; \
out << "Percentage Spawned:  " << percentageSpawned << endl; \
out << "Min Num to Spawn:  " << minNumToSpawn << endl; \
out << "Max Num to Spawn:  " << maxNumToSpawn << endl; \
out << "Spawn Speed Chaos:  " << spawnSpeedChaos << endl; \
out << "Spawn Dir Chaos:  " << spawnDirChaos << endl; \
out << "Life Span:  " << lifeSpan << endl; \
out << "Life Span Variation:  " << lifeSpanVariation << endl; \
return out.str(); \

#define NI_P_SYS_SPAWN_MODIFIER_FIXLINKS \
APSysModifier::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_SPHERE_EMITTER_MEMBERS \
float radius; \

#define NI_P_SYS_SPHERE_EMITTER_PARENTS APSysVolumeEmitter \

#define NI_P_SYS_SPHERE_EMITTER_CONSTRUCT \

#define NI_P_SYS_SPHERE_EMITTER_READ \
APSysVolumeEmitter::Read( in, link_stack, version ); \
NifStream( radius, in, version ); \

#define NI_P_SYS_SPHERE_EMITTER_WRITE \
APSysVolumeEmitter::Write( out, link_map, version ); \
NifStream( radius, out, version ); \

#define NI_P_SYS_SPHERE_EMITTER_STRING \
stringstream out; \
out << APSysVolumeEmitter::asString(); \
out << "Radius:  " << radius << endl; \
return out.str(); \

#define NI_P_SYS_SPHERE_EMITTER_FIXLINKS \
APSysVolumeEmitter::FixLinks( objects, link_stack, version ); \

#define NI_P_SYS_UPDATE_CTLR_MEMBERS \

#define NI_P_SYS_UPDATE_CTLR_PARENTS NiTimeController \

#define NI_P_SYS_UPDATE_CTLR_CONSTRUCT \

#define NI_P_SYS_UPDATE_CTLR_READ \
NiTimeController::Read( in, link_stack, version ); \

#define NI_P_SYS_UPDATE_CTLR_WRITE \
NiTimeController::Write( out, link_map, version ); \

#define NI_P_SYS_UPDATE_CTLR_STRING \
stringstream out; \
out << NiTimeController::asString(); \
return out.str(); \

#define NI_P_SYS_UPDATE_CTLR_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \

#define NI_RANGE_L_O_D_DATA_MEMBERS \
Vector3 lodCenter; \
vector<LODRange > lodLevels; \

#define NI_RANGE_L_O_D_DATA_PARENTS NiObject \

#define NI_RANGE_L_O_D_DATA_CONSTRUCT \

#define NI_RANGE_L_O_D_DATA_READ \
NiObject::Read( in, link_stack, version ); \
uint numLodLevels; \
NifStream( lodCenter, in, version ); \
NifStream( numLodLevels, in, version ); \
lodLevels.resize(numLodLevels); \
for (uint i0 = 0; i0 < numLodLevels; i0++) { \
  NifStream( lodLevels[i0].near, in, version ); \
  NifStream( lodLevels[i0].far, in, version ); \
}; \

#define NI_RANGE_L_O_D_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
uint numLodLevels; \
numLodLevels = uint(numLodLevels.size()); \
NifStream( lodCenter, out, version ); \
NifStream( numLodLevels, out, version ); \
for (uint i0 = 0; i0 < numLodLevels; i0++) { \
  NifStream( lodLevels[i0].near, out, version ); \
  NifStream( lodLevels[i0].far, out, version ); \
}; \

#define NI_RANGE_L_O_D_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "LOD Center:  " << lodCenter << endl; \
out << "Num LOD Levels:  " << numLodLevels << endl; \
for (uint i0 = 0; i0 < numLodLevels; i0++) { \
  out << "  Near:  " << lodLevels[i0].near << endl; \
  out << "  Far:  " << lodLevels[i0].far << endl; \
}; \
return out.str(); \

#define NI_RANGE_L_O_D_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numLodLevels; i0++) { \
}; \

#define NI_ROTATING_PARTICLES_MEMBERS \

#define NI_ROTATING_PARTICLES_PARENTS NiParticles \

#define NI_ROTATING_PARTICLES_CONSTRUCT \

#define NI_ROTATING_PARTICLES_READ \
NiParticles::Read( in, link_stack, version ); \

#define NI_ROTATING_PARTICLES_WRITE \
NiParticles::Write( out, link_map, version ); \

#define NI_ROTATING_PARTICLES_STRING \
stringstream out; \
out << NiParticles::asString(); \
return out.str(); \

#define NI_ROTATING_PARTICLES_FIXLINKS \
NiParticles::FixLinks( objects, link_stack, version ); \

#define NI_ROTATING_PARTICLES_DATA_MEMBERS \

#define NI_ROTATING_PARTICLES_DATA_PARENTS ARotatingParticlesData \

#define NI_ROTATING_PARTICLES_DATA_CONSTRUCT \

#define NI_ROTATING_PARTICLES_DATA_READ \
ARotatingParticlesData::Read( in, link_stack, version ); \

#define NI_ROTATING_PARTICLES_DATA_WRITE \
ARotatingParticlesData::Write( out, link_map, version ); \

#define NI_ROTATING_PARTICLES_DATA_STRING \
stringstream out; \
out << ARotatingParticlesData::asString(); \
return out.str(); \

#define NI_ROTATING_PARTICLES_DATA_FIXLINKS \
ARotatingParticlesData::FixLinks( objects, link_stack, version ); \

#define NI_SCREEN_L_O_D_DATA_MEMBERS \
vector<float > unknownFloats; \
vector<float > unknownFloats2; \

#define NI_SCREEN_L_O_D_DATA_PARENTS NiObject \

#define NI_SCREEN_L_O_D_DATA_CONSTRUCT \

#define NI_SCREEN_L_O_D_DATA_READ \
NiObject::Read( in, link_stack, version ); \
uint unknownCount; \
unknownFloats.resize(8); \
for (uint i0 = 0; i0 < 8; i0++) { \
  NifStream( unknownFloats[i0], in, version ); \
}; \
NifStream( unknownCount, in, version ); \
unknownFloats2.resize(unknownCount); \
for (uint i0 = 0; i0 < unknownCount; i0++) { \
  NifStream( unknownFloats2[i0], in, version ); \
}; \

#define NI_SCREEN_L_O_D_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
uint unknownCount; \
unknownCount = uint(unknownCount.size()); \
for (uint i0 = 0; i0 < 8; i0++) { \
  NifStream( unknownFloats[i0], out, version ); \
}; \
NifStream( unknownCount, out, version ); \
for (uint i0 = 0; i0 < unknownCount; i0++) { \
  NifStream( unknownFloats2[i0], out, version ); \
}; \

#define NI_SCREEN_L_O_D_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
for (uint i0 = 0; i0 < 8; i0++) { \
  out << "  Unknown Floats[" << i0 << "]:  " << unknownFloats[i0] << endl; \
}; \
out << "Unknown Count:  " << unknownCount << endl; \
for (uint i0 = 0; i0 < unknownCount; i0++) { \
  out << "  Unknown Floats 2[" << i0 << "]:  " << unknownFloats2[i0] << endl; \
}; \
return out.str(); \

#define NI_SCREEN_L_O_D_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 8; i0++) { \
}; \
for (uint i0 = 0; i0 < unknownCount; i0++) { \
}; \

#define NI_SEQUENCE_STREAM_HELPER_MEMBERS \

#define NI_SEQUENCE_STREAM_HELPER_PARENTS NiObjectNET \

#define NI_SEQUENCE_STREAM_HELPER_CONSTRUCT \

#define NI_SEQUENCE_STREAM_HELPER_READ \
NiObjectNET::Read( in, link_stack, version ); \

#define NI_SEQUENCE_STREAM_HELPER_WRITE \
NiObjectNET::Write( out, link_map, version ); \

#define NI_SEQUENCE_STREAM_HELPER_STRING \
stringstream out; \
out << NiObjectNET::asString(); \
return out.str(); \

#define NI_SEQUENCE_STREAM_HELPER_FIXLINKS \
NiObjectNET::FixLinks( objects, link_stack, version ); \

#define NI_SHADE_PROPERTY_MEMBERS \
Flags flags; \

#define NI_SHADE_PROPERTY_PARENTS NiProperty \

#define NI_SHADE_PROPERTY_CONSTRUCT \

#define NI_SHADE_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \

#define NI_SHADE_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \

#define NI_SHADE_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
return out.str(); \

#define NI_SHADE_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_SKIN_DATA_MEMBERS \
Matrix33 rotation; \
Vector3 translation; \
float scale; \
Ref<NiSkinPartition > skinPartition; \
byte unknownByte; \
vector<SkinData > boneList; \

#define NI_SKIN_DATA_PARENTS NiObject \

#define NI_SKIN_DATA_CONSTRUCT \

#define NI_SKIN_DATA_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
uint numBones; \
NifStream( rotation, in, version ); \
NifStream( translation, in, version ); \
NifStream( scale, in, version ); \
NifStream( numBones, in, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
if ( version >= 0x04020100 ) { \
  NifStream( unknownByte, in, version ); \
}; \
boneList.resize(numBones); \
for (uint i0 = 0; i0 < numBones; i0++) { \
  ushort boneList_numVertices; \
  NifStream( boneList[i0].rotation, in, version ); \
  NifStream( boneList[i0].translation, in, version ); \
  NifStream( boneList[i0].scale, in, version ); \
  boneList[i0].unknown4Floats.resize(4); \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( boneList[i0].unknown4Floats[i1], in, version ); \
  }; \
  NifStream( boneList_numVertices, in, version ); \
  boneList[i0].vertexWeights.resize(boneList_numVertices); \
  for (uint i1 = 0; i1 < boneList_numVertices; i1++) { \
    NifStream( boneList[i0].vertexWeights[i1].index, in, version ); \
    NifStream( boneList[i0].vertexWeights[i1].weight, in, version ); \
  }; \
}; \

#define NI_SKIN_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
uint numBones; \
numBones = uint(numBones.size()); \
NifStream( rotation, out, version ); \
NifStream( translation, out, version ); \
NifStream( scale, out, version ); \
NifStream( numBones, out, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( link_map[skinPartition], out, version ); \
}; \
if ( version >= 0x04020100 ) { \
  NifStream( unknownByte, out, version ); \
}; \
for (uint i0 = 0; i0 < numBones; i0++) { \
  ushort boneList_numVertices; \
  boneList_numVertices = ushort(boneList[i0].numVertices.size()); \
  NifStream( boneList[i0].rotation, out, version ); \
  NifStream( boneList[i0].translation, out, version ); \
  NifStream( boneList[i0].scale, out, version ); \
  for (uint i1 = 0; i1 < 4; i1++) { \
    NifStream( boneList[i0].unknown4Floats[i1], out, version ); \
  }; \
  NifStream( boneList_numVertices, out, version ); \
  for (uint i1 = 0; i1 < boneList_numVertices; i1++) { \
    NifStream( boneList[i0].vertexWeights[i1].index, out, version ); \
    NifStream( boneList[i0].vertexWeights[i1].weight, out, version ); \
  }; \
}; \

#define NI_SKIN_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Rotation:  " << rotation << endl; \
out << "Translation:  " << translation << endl; \
out << "Scale:  " << scale << endl; \
out << "Num Bones:  " << numBones << endl; \
out << "Skin Partition:  " << skinPartition << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
for (uint i0 = 0; i0 < numBones; i0++) { \
  out << "  Rotation:  " << boneList[i0].rotation << endl; \
  out << "  Translation:  " << boneList[i0].translation << endl; \
  out << "  Scale:  " << boneList[i0].scale << endl; \
  for (uint i1 = 0; i1 < 4; i1++) { \
    out << "    Unknown 4 Floats[" << i1 << "]:  " << boneList[i0].unknown4Floats[i1] << endl; \
  }; \
  out << "  Num Vertices:  " << boneList_numVertices << endl; \
  for (uint i1 = 0; i1 < boneList_numVertices; i1++) { \
    out << "    Index:  " << boneList[i0].vertexWeights[i1].index << endl; \
    out << "    Weight:  " << boneList[i0].vertexWeights[i1].weight << endl; \
  }; \
}; \
return out.str(); \

#define NI_SKIN_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A010000 ) { \
  skinPartition = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( version >= 0x04020100 ) { \
}; \
for (uint i0 = 0; i0 < numBones; i0++) { \
  for (uint i1 = 0; i1 < 4; i1++) { \
  }; \
  for (uint i1 = 0; i1 < boneList_numVertices; i1++) { \
  }; \
}; \

#define NI_SKIN_INSTANCE_MEMBERS \
Ref<NiSkinData > data; \
Ref<NiSkinPartition > skinPartition; \
Bones bones; \

#define NI_SKIN_INSTANCE_PARENTS NiObject \

#define NI_SKIN_INSTANCE_CONSTRUCT \

#define NI_SKIN_INSTANCE_READ \
uint block_num; \
NiObject::Read( in, link_stack, version ); \
NiNode * skeletonRoot; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
if ( version >= 0x0A020000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
uint bones_numBones; \
NifStream( bones_numBones, in, version ); \
bones.bones.resize(bones_numBones); \
for (uint i0 = 0; i0 < bones_numBones; i0++) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_SKIN_INSTANCE_WRITE \
NiObject::Write( out, link_map, version ); \
NiNode * skeletonRoot; \

#define NI_SKIN_INSTANCE_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Data:  " << data << endl; \
out << "Skin Partition:  " << skinPartition << endl; \
out << "Skeleton Root:  " << skeletonRoot << endl; \
out << "Num Bones:  " << bones_numBones << endl; \
for (uint i0 = 0; i0 < bones_numBones; i0++) { \
  out << "  Bones[" << i0 << "]:  " << bones.bones[i0] << endl; \
}; \
return out.str(); \

#define NI_SKIN_INSTANCE_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( version >= 0x0A020000 ) { \
  skinPartition = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
skeletonRoot = blocks[link_stack.front()]; \
link_stack.pop_front(); \
for (uint i0 = 0; i0 < bones_numBones; i0++) { \
  bones.bones[i0] = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_SKIN_PARTITION_MEMBERS \
vector<SkinPartition > skinPartitionBlocks; \

#define NI_SKIN_PARTITION_PARENTS NiObject \

#define NI_SKIN_PARTITION_CONSTRUCT \

#define NI_SKIN_PARTITION_READ \
NiObject::Read( in, link_stack, version ); \
uint numSkinPartitionBlocks; \
NifStream( numSkinPartitionBlocks, in, version ); \
skinPartitionBlocks.resize(numSkinPartitionBlocks); \
for (uint i0 = 0; i0 < numSkinPartitionBlocks; i0++) { \
  ushort skinPartitionBlocks_numVertices; \
  ushort skinPartitionBlocks_numTriangles; \
  ushort skinPartitionBlocks_numBones; \
  ushort skinPartitionBlocks_numWeightsPerVertex; \
  vector<ushort > skinPartitionBlocks_stripLengths; \
  NifStream( skinPartitionBlocks_numVertices, in, version ); \
  NifStream( skinPartitionBlocks_numTriangles, in, version ); \
  NifStream( skinPartitionBlocks_numBones, in, version ); \
  NifStream( skinPartitionBlocks[i0].numStrips, in, version ); \
  NifStream( skinPartitionBlocks_numWeightsPerVertex, in, version ); \
  skinPartitionBlocks[i0].bones.resize(skinPartitionBlocks_numBones); \
  for (uint i1 = 0; i1 < skinPartitionBlocks_numBones; i1++) { \
    NifStream( skinPartitionBlocks[i0].bones[i1], in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( skinPartitionBlocks[i0].hasVertexMap, in, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    skinPartitionBlocks[i0].vertexMap.resize(skinPartitionBlocks_numVertices); \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      NifStream( skinPartitionBlocks[i0].vertexMap[i2], in, version ); \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasVertexMap != 0 ) { \
      skinPartitionBlocks[i0].vertexMap.resize(skinPartitionBlocks_numVertices); \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) { \
        NifStream( skinPartitionBlocks[i0].vertexMap[i3], in, version ); \
      }; \
    }; \
    NifStream( skinPartitionBlocks[i0].hasVertexWeights, in, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    skinPartitionBlocks[i0].vertexWeights.resize(skinPartitionBlocks_numVertices); \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) \
      skinPartitionBlocks[i0].vertexWeights[i2].resize(skinPartitionBlocks_numWeightsPerVertex); \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
        NifStream( skinPartitionBlocks[i0].vertexWeights[i2][i3], in, version ); \
      }; \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasVertexWeights != 0 ) { \
      skinPartitionBlocks[i0].vertexWeights.resize(skinPartitionBlocks_numVertices); \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) \
        skinPartitionBlocks[i0].vertexWeights[i3].resize(skinPartitionBlocks_numWeightsPerVertex); \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) { \
        for (uint i4 = 0; i4 < skinPartitionBlocks_numWeightsPerVertex; i4++) { \
          NifStream( skinPartitionBlocks[i0].vertexWeights[i3][i4], in, version ); \
        }; \
      }; \
    }; \
  }; \
  skinPartitionBlocks_stripLengths.resize(skinPartitionBlocks[i0].numStrips); \
  for (uint i1 = 0; i1 < skinPartitionBlocks[i0].numStrips; i1++) { \
    NifStream( skinPartitionBlocks_stripLengths[i1], in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( skinPartitionBlocks[i0].hasStrips, in, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    skinPartitionBlocks[i0].strips.resize(skinPartitionBlocks[i0].numStrips); \
    for (uint i2 = 0; i2 < skinPartitionBlocks[i0].numStrips; i2++) \
      skinPartitionBlocks[i0].strips[i2].resize(skinPartitionBlocks_stripLengths); \
    for (uint i2 = 0; i2 < skinPartitionBlocks[i0].numStrips; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_stripLengths; i3++) { \
        NifStream( skinPartitionBlocks[i0].strips[i2][i3], in, version ); \
      }; \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasStrips != 0 ) { \
      skinPartitionBlocks[i0].strips.resize(skinPartitionBlocks[i0].numStrips); \
      for (uint i3 = 0; i3 < skinPartitionBlocks[i0].numStrips; i3++) \
        skinPartitionBlocks[i0].strips[i3].resize(skinPartitionBlocks_stripLengths); \
      for (uint i3 = 0; i3 < skinPartitionBlocks[i0].numStrips; i3++) { \
        for (uint i4 = 0; i4 < skinPartitionBlocks_stripLengths; i4++) { \
          NifStream( skinPartitionBlocks[i0].strips[i3][i4], in, version ); \
        }; \
      }; \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].numStrips == 0 ) { \
    skinPartitionBlocks[i0].triangles.resize(skinPartitionBlocks_numTriangles); \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numTriangles; i2++) { \
      NifStream( skinPartitionBlocks[i0].triangles[i2], in, version ); \
    }; \
  }; \
  NifStream( skinPartitionBlocks[i0].hasBoneIndices, in, version ); \
  if ( skinPartitionBlocks[i0].hasBoneIndices != 0 ) { \
    skinPartitionBlocks[i0].boneIndices.resize(skinPartitionBlocks_numVertices); \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) \
      skinPartitionBlocks[i0].boneIndices[i2].resize(skinPartitionBlocks_numWeightsPerVertex); \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
        NifStream( skinPartitionBlocks[i0].boneIndices[i2][i3], in, version ); \
      }; \
    }; \
  }; \
}; \

#define NI_SKIN_PARTITION_WRITE \
NiObject::Write( out, link_map, version ); \
uint numSkinPartitionBlocks; \
numSkinPartitionBlocks = uint(numSkinPartitionBlocks.size()); \
NifStream( numSkinPartitionBlocks, out, version ); \
for (uint i0 = 0; i0 < numSkinPartitionBlocks; i0++) { \
  ushort skinPartitionBlocks_numVertices; \
  skinPartitionBlocks_numVertices = ushort(skinPartitionBlocks[i0].numVertices.size()); \
  ushort skinPartitionBlocks_numTriangles; \
  skinPartitionBlocks_numTriangles = ushort(skinPartitionBlocks[i0].numTriangles.size()); \
  ushort skinPartitionBlocks_numBones; \
  skinPartitionBlocks_numBones = ushort(skinPartitionBlocks[i0].numBones.size()); \
  ushort skinPartitionBlocks_numWeightsPerVertex; \
  skinPartitionBlocks_numWeightsPerVertex = ushort(skinPartitionBlocks[i0].numWeightsPerVertex.size()); \
  vector<ushort > skinPartitionBlocks_stripLengths; \
  skinPartitionBlocks_stripLengths.resize(skinPartitionBlocks[i0].stripLengths.size()); \
  for (uint i1 = 0; i < skinPartitionBlocks[i0].stripLengths.size(); i++) \
    skinPartitionBlocks_stripLengths[i1] = ushort(skinPartitionBlocks[i0].stripLengths[i1].size()); \
  NifStream( skinPartitionBlocks_numVertices, out, version ); \
  NifStream( skinPartitionBlocks_numTriangles, out, version ); \
  NifStream( skinPartitionBlocks_numBones, out, version ); \
  NifStream( skinPartitionBlocks[i0].numStrips, out, version ); \
  NifStream( skinPartitionBlocks_numWeightsPerVertex, out, version ); \
  for (uint i1 = 0; i1 < skinPartitionBlocks_numBones; i1++) { \
    NifStream( skinPartitionBlocks[i0].bones[i1], out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( skinPartitionBlocks[i0].hasVertexMap, out, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      NifStream( skinPartitionBlocks[i0].vertexMap[i2], out, version ); \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasVertexMap != 0 ) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) { \
        NifStream( skinPartitionBlocks[i0].vertexMap[i3], out, version ); \
      }; \
    }; \
    NifStream( skinPartitionBlocks[i0].hasVertexWeights, out, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
        NifStream( skinPartitionBlocks[i0].vertexWeights[i2][i3], out, version ); \
      }; \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasVertexWeights != 0 ) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) { \
        for (uint i4 = 0; i4 < skinPartitionBlocks_numWeightsPerVertex; i4++) { \
          NifStream( skinPartitionBlocks[i0].vertexWeights[i3][i4], out, version ); \
        }; \
      }; \
    }; \
  }; \
  for (uint i1 = 0; i1 < skinPartitionBlocks[i0].numStrips; i1++) { \
    NifStream( skinPartitionBlocks_stripLengths[i1], out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( skinPartitionBlocks[i0].hasStrips, out, version ); \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks[i0].numStrips; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_stripLengths; i3++) { \
        NifStream( skinPartitionBlocks[i0].strips[i2][i3], out, version ); \
      }; \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasStrips != 0 ) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks[i0].numStrips; i3++) { \
        for (uint i4 = 0; i4 < skinPartitionBlocks_stripLengths; i4++) { \
          NifStream( skinPartitionBlocks[i0].strips[i3][i4], out, version ); \
        }; \
      }; \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].numStrips == 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numTriangles; i2++) { \
      NifStream( skinPartitionBlocks[i0].triangles[i2], out, version ); \
    }; \
  }; \
  NifStream( skinPartitionBlocks[i0].hasBoneIndices, out, version ); \
  if ( skinPartitionBlocks[i0].hasBoneIndices != 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
        NifStream( skinPartitionBlocks[i0].boneIndices[i2][i3], out, version ); \
      }; \
    }; \
  }; \
}; \

#define NI_SKIN_PARTITION_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Num Skin Partition Blocks:  " << numSkinPartitionBlocks << endl; \
for (uint i0 = 0; i0 < numSkinPartitionBlocks; i0++) { \
  out << "  Num Vertices:  " << skinPartitionBlocks_numVertices << endl; \
  out << "  Num Triangles:  " << skinPartitionBlocks_numTriangles << endl; \
  out << "  Num Bones:  " << skinPartitionBlocks_numBones << endl; \
  out << "  Num Strips:  " << skinPartitionBlocks[i0].numStrips << endl; \
  out << "  Num Weights Per Vertex:  " << skinPartitionBlocks_numWeightsPerVertex << endl; \
  for (uint i1 = 0; i1 < skinPartitionBlocks_numBones; i1++) { \
    out << "    Bones[" << i1 << "]:  " << skinPartitionBlocks[i0].bones[i1] << endl; \
  }; \
  out << "  Has Vertex Map:  " << skinPartitionBlocks[i0].hasVertexMap << endl; \
  for (uint i1 = 0; i1 < skinPartitionBlocks_numVertices; i1++) { \
    out << "    Vertex Map[" << i1 << "]:  " << skinPartitionBlocks[i0].vertexMap[i1] << endl; \
  }; \
  if ( skinPartitionBlocks[i0].hasVertexMap != 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      out << "      Vertex Map[" << i2 << "]:  " << skinPartitionBlocks[i0].vertexMap[i2] << endl; \
    }; \
  }; \
  out << "  Has Vertex Weights:  " << skinPartitionBlocks[i0].hasVertexWeights << endl; \
  for (uint i1 = 0; i1 < skinPartitionBlocks_numVertices; i1++) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numWeightsPerVertex; i2++) { \
      out << "      Vertex Weights[" << i1 << "][" << i2 << "]:  " << skinPartitionBlocks[i0].vertexWeights[i1][i2] << endl; \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].hasVertexWeights != 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
        out << "        Vertex Weights[" << i2 << "][" << i3 << "]:  " << skinPartitionBlocks[i0].vertexWeights[i2][i3] << endl; \
      }; \
    }; \
  }; \
  for (uint i1 = 0; i1 < skinPartitionBlocks[i0].numStrips; i1++) { \
    out << "    Strip Lengths[" << i1 << "]:  " << skinPartitionBlocks_stripLengths[i1] << endl; \
  }; \
  out << "  Has Strips:  " << skinPartitionBlocks[i0].hasStrips << endl; \
  for (uint i1 = 0; i1 < skinPartitionBlocks[i0].numStrips; i1++) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_stripLengths; i2++) { \
      out << "      Strips[" << i1 << "][" << i2 << "]:  " << skinPartitionBlocks[i0].strips[i1][i2] << endl; \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].hasStrips != 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks[i0].numStrips; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_stripLengths; i3++) { \
        out << "        Strips[" << i2 << "][" << i3 << "]:  " << skinPartitionBlocks[i0].strips[i2][i3] << endl; \
      }; \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].numStrips == 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numTriangles; i2++) { \
      out << "      Triangles[" << i2 << "]:  " << skinPartitionBlocks[i0].triangles[i2] << endl; \
    }; \
  }; \
  out << "  Has Bone Indices:  " << skinPartitionBlocks[i0].hasBoneIndices << endl; \
  if ( skinPartitionBlocks[i0].hasBoneIndices != 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
        out << "        Bone Indices[" << i2 << "][" << i3 << "]:  " << skinPartitionBlocks[i0].boneIndices[i2][i3] << endl; \
      }; \
    }; \
  }; \
}; \
return out.str(); \

#define NI_SKIN_PARTITION_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numSkinPartitionBlocks; i0++) { \
  for (uint i1 = 0; i1 < skinPartitionBlocks_numBones; i1++) { \
  }; \
  if ( version >= 0x0A010000 ) { \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasVertexMap != 0 ) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) { \
      }; \
    }; \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
      }; \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasVertexWeights != 0 ) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numVertices; i3++) { \
        for (uint i4 = 0; i4 < skinPartitionBlocks_numWeightsPerVertex; i4++) { \
        }; \
      }; \
    }; \
  }; \
  for (uint i1 = 0; i1 < skinPartitionBlocks[i0].numStrips; i1++) { \
  }; \
  if ( version >= 0x0A010000 ) { \
  }; \
  if ( version <= 0x0A000102 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks[i0].numStrips; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_stripLengths; i3++) { \
      }; \
    }; \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( skinPartitionBlocks[i0].hasStrips != 0 ) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks[i0].numStrips; i3++) { \
        for (uint i4 = 0; i4 < skinPartitionBlocks_stripLengths; i4++) { \
        }; \
      }; \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].numStrips == 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numTriangles; i2++) { \
    }; \
  }; \
  if ( skinPartitionBlocks[i0].hasBoneIndices != 0 ) { \
    for (uint i2 = 0; i2 < skinPartitionBlocks_numVertices; i2++) { \
      for (uint i3 = 0; i3 < skinPartitionBlocks_numWeightsPerVertex; i3++) { \
      }; \
    }; \
  }; \
}; \

#define NI_SOURCE_TEXTURE_MEMBERS \
TexSource textureSource; \
PixelLayout pixelLayout; \
MipMapFormat useMipmaps; \
AlphaFormat alphaFormat; \
byte unknownByte; \
byte unknownByte2; \

#define NI_SOURCE_TEXTURE_PARENTS NiObjectNET \

#define NI_SOURCE_TEXTURE_CONSTRUCT \

#define NI_SOURCE_TEXTURE_READ \
uint block_num; \
NiObjectNET::Read( in, link_stack, version ); \
NifStream( textureSource.useExternal, in, version ); \
if ( textureSource.useExternal == 1 ) { \
  NifStream( textureSource.fileName, in, version ); \
}; \
if ( version >= 0x0A010000 ) { \
  if ( textureSource.useExternal == 1 ) { \
    NifStream( block_num, in, version ); \
    link_stack.push_back( block_num ); \
  }; \
}; \
if ( version <= 0x0A000100 ) { \
  if ( textureSource.useExternal == 0 ) { \
    NifStream( textureSource.unknownByte, in, version ); \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( textureSource.useExternal == 0 ) { \
    NifStream( textureSource.originalFileName_, in, version ); \
  }; \
}; \
if ( textureSource.useExternal == 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \
NifStream( pixelLayout, in, version ); \
NifStream( useMipmaps, in, version ); \
NifStream( alphaFormat, in, version ); \
NifStream( unknownByte, in, version ); \
if ( version >= 0x0A01006A ) { \
  NifStream( unknownByte2, in, version ); \
}; \

#define NI_SOURCE_TEXTURE_WRITE \
NiObjectNET::Write( out, link_map, version ); \
NifStream( textureSource.useExternal, out, version ); \
if ( textureSource.useExternal == 1 ) { \
  NifStream( textureSource.fileName, out, version ); \
}; \
if ( version >= 0x0A010000 ) { \
  if ( textureSource.useExternal == 1 ) { \
    NifStream( link_map[textureSource.unknownLink], out, version ); \
  }; \
}; \
if ( version <= 0x0A000100 ) { \
  if ( textureSource.useExternal == 0 ) { \
    NifStream( textureSource.unknownByte, out, version ); \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( textureSource.useExternal == 0 ) { \
    NifStream( textureSource.originalFileName_, out, version ); \
  }; \
}; \
if ( textureSource.useExternal == 0 ) { \
  NifStream( link_map[textureSource.pixelData], out, version ); \
}; \
NifStream( pixelLayout, out, version ); \
NifStream( useMipmaps, out, version ); \
NifStream( alphaFormat, out, version ); \
NifStream( unknownByte, out, version ); \
if ( version >= 0x0A01006A ) { \
  NifStream( unknownByte2, out, version ); \
}; \

#define NI_SOURCE_TEXTURE_STRING \
stringstream out; \
out << NiObjectNET::asString(); \
out << "Use External:  " << textureSource.useExternal << endl; \
if ( textureSource.useExternal == 1 ) { \
  out << "  File Name:  " << textureSource.fileName << endl; \
  out << "  Unknown Link:  " << textureSource.unknownLink << endl; \
}; \
if ( textureSource.useExternal == 0 ) { \
  out << "  Unknown Byte:  " << textureSource.unknownByte << endl; \
  out << "  Original File Name?:  " << textureSource.originalFileName_ << endl; \
  out << "  Pixel Data:  " << textureSource.pixelData << endl; \
}; \
out << "Pixel Layout:  " << pixelLayout << endl; \
out << "Use Mipmaps:  " << useMipmaps << endl; \
out << "Alpha Format:  " << alphaFormat << endl; \
out << "Unknown Byte:  " << unknownByte << endl; \
out << "Unknown Byte 2:  " << unknownByte2 << endl; \
return out.str(); \

#define NI_SOURCE_TEXTURE_FIXLINKS \
NiObjectNET::FixLinks( objects, link_stack, version ); \
if ( textureSource.useExternal == 1 ) { \
}; \
if ( version >= 0x0A010000 ) { \
  if ( textureSource.useExternal == 1 ) { \
    textureSource.unknownLink = blocks[link_stack.front()]; \
    link_stack.pop_front(); \
  }; \
}; \
if ( version <= 0x0A000100 ) { \
  if ( textureSource.useExternal == 0 ) { \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( textureSource.useExternal == 0 ) { \
  }; \
}; \
if ( textureSource.useExternal == 0 ) { \
  textureSource.pixelData = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \
if ( version >= 0x0A01006A ) { \
}; \

#define NI_SPECULAR_PROPERTY_MEMBERS \
Flags flags; \

#define NI_SPECULAR_PROPERTY_PARENTS NiProperty \

#define NI_SPECULAR_PROPERTY_CONSTRUCT \

#define NI_SPECULAR_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \

#define NI_SPECULAR_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \

#define NI_SPECULAR_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
return out.str(); \

#define NI_SPECULAR_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_SPHERICAL_COLLIDER_MEMBERS \
float unknownFloat1; \
ushort unknownShort; \
float unknownFloat2; \
float unknownFloat3; \
float unknownFloat4; \
float unknownFloat5; \

#define NI_SPHERICAL_COLLIDER_PARENTS AParticleModifier \

#define NI_SPHERICAL_COLLIDER_CONSTRUCT \

#define NI_SPHERICAL_COLLIDER_READ \
AParticleModifier::Read( in, link_stack, version ); \
NifStream( unknownFloat1, in, version ); \
NifStream( unknownShort, in, version ); \
NifStream( unknownFloat2, in, version ); \
NifStream( unknownFloat3, in, version ); \
NifStream( unknownFloat4, in, version ); \
NifStream( unknownFloat5, in, version ); \

#define NI_SPHERICAL_COLLIDER_WRITE \
AParticleModifier::Write( out, link_map, version ); \
NifStream( unknownFloat1, out, version ); \
NifStream( unknownShort, out, version ); \
NifStream( unknownFloat2, out, version ); \
NifStream( unknownFloat3, out, version ); \
NifStream( unknownFloat4, out, version ); \
NifStream( unknownFloat5, out, version ); \

#define NI_SPHERICAL_COLLIDER_STRING \
stringstream out; \
out << AParticleModifier::asString(); \
out << "Unknown Float 1:  " << unknownFloat1 << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Unknown Float 2:  " << unknownFloat2 << endl; \
out << "Unknown Float 3:  " << unknownFloat3 << endl; \
out << "Unknown Float 4:  " << unknownFloat4 << endl; \
out << "Unknown Float 5:  " << unknownFloat5 << endl; \
return out.str(); \

#define NI_SPHERICAL_COLLIDER_FIXLINKS \
AParticleModifier::FixLinks( objects, link_stack, version ); \

#define NI_SPOT_LIGHT_MEMBERS \
float cutoffAngle; \
float exponent; \

#define NI_SPOT_LIGHT_PARENTS APointLight \

#define NI_SPOT_LIGHT_CONSTRUCT \

#define NI_SPOT_LIGHT_READ \
APointLight::Read( in, link_stack, version ); \
NifStream( cutoffAngle, in, version ); \
NifStream( exponent, in, version ); \

#define NI_SPOT_LIGHT_WRITE \
APointLight::Write( out, link_map, version ); \
NifStream( cutoffAngle, out, version ); \
NifStream( exponent, out, version ); \

#define NI_SPOT_LIGHT_STRING \
stringstream out; \
out << APointLight::asString(); \
out << "Cutoff Angle:  " << cutoffAngle << endl; \
out << "Exponent:  " << exponent << endl; \
return out.str(); \

#define NI_SPOT_LIGHT_FIXLINKS \
APointLight::FixLinks( objects, link_stack, version ); \

#define NI_STENCIL_PROPERTY_MEMBERS \
Flags flags; \
bool stencilEnabled; \
uint stencilFunction; \
uint stencilRef; \
uint stencilMask; \
uint failAction; \
uint zFailAction; \
uint passAction; \
uint drawMode; \

#define NI_STENCIL_PROPERTY_PARENTS NiProperty \

#define NI_STENCIL_PROPERTY_CONSTRUCT \

#define NI_STENCIL_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
if ( version <= 0x0A000102 ) { \
  NifStream( flags, in, version ); \
}; \
NifStream( stencilEnabled, in, version ); \
NifStream( stencilFunction, in, version ); \
NifStream( stencilRef, in, version ); \
NifStream( stencilMask, in, version ); \
NifStream( failAction, in, version ); \
NifStream( zFailAction, in, version ); \
NifStream( passAction, in, version ); \
NifStream( drawMode, in, version ); \

#define NI_STENCIL_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
if ( version <= 0x0A000102 ) { \
  NifStream( flags, out, version ); \
}; \
NifStream( stencilEnabled, out, version ); \
NifStream( stencilFunction, out, version ); \
NifStream( stencilRef, out, version ); \
NifStream( stencilMask, out, version ); \
NifStream( failAction, out, version ); \
NifStream( zFailAction, out, version ); \
NifStream( passAction, out, version ); \
NifStream( drawMode, out, version ); \

#define NI_STENCIL_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Stencil Enabled:  " << stencilEnabled << endl; \
out << "Stencil Function:  " << stencilFunction << endl; \
out << "Stencil Ref:  " << stencilRef << endl; \
out << "Stencil Mask:  " << stencilMask << endl; \
out << "Fail Action:  " << failAction << endl; \
out << "Z Fail Action:  " << zFailAction << endl; \
out << "Pass Action:  " << passAction << endl; \
out << "Draw Mode:  " << drawMode << endl; \
return out.str(); \

#define NI_STENCIL_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A000102 ) { \
}; \

#define NI_STRING_EXTRA_DATA_MEMBERS \
string stringData; \

#define NI_STRING_EXTRA_DATA_PARENTS NiExtraData \

#define NI_STRING_EXTRA_DATA_CONSTRUCT \

#define NI_STRING_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
uint bytesRemaining; \
if ( version <= 0x04020200 ) { \
  NifStream( bytesRemaining, in, version ); \
}; \
NifStream( stringData, in, version ); \

#define NI_STRING_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
uint bytesRemaining; \

#define NI_STRING_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Bytes Remaining:  " << bytesRemaining << endl; \
out << "String Data:  " << stringData << endl; \
return out.str(); \

#define NI_STRING_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
if ( version <= 0x04020200 ) { \
}; \

#define NI_STRING_PALETTE_MEMBERS \
StringPalette palette; \

#define NI_STRING_PALETTE_PARENTS NiObject \

#define NI_STRING_PALETTE_CONSTRUCT \

#define NI_STRING_PALETTE_READ \
NiObject::Read( in, link_stack, version ); \
NifStream( palette.palette, in, version ); \
NifStream( palette.length, in, version ); \

#define NI_STRING_PALETTE_WRITE \
NiObject::Write( out, link_map, version ); \
NifStream( palette.palette, out, version ); \
NifStream( palette.length, out, version ); \

#define NI_STRING_PALETTE_STRING \
stringstream out; \
out << NiObject::asString(); \
out << "Palette:  " << palette.palette << endl; \
out << "Length:  " << palette.length << endl; \
return out.str(); \

#define NI_STRING_PALETTE_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \

#define NI_STRINGS_EXTRA_DATA_MEMBERS \
vector<string > data; \

#define NI_STRINGS_EXTRA_DATA_PARENTS NiExtraData \

#define NI_STRINGS_EXTRA_DATA_CONSTRUCT \

#define NI_STRINGS_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
uint numStrings; \
NifStream( numStrings, in, version ); \
data.resize(numStrings); \
for (uint i0 = 0; i0 < numStrings; i0++) { \
  NifStream( data[i0], in, version ); \
}; \

#define NI_STRINGS_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
uint numStrings; \
numStrings = uint(numStrings.size()); \
NifStream( numStrings, out, version ); \
for (uint i0 = 0; i0 < numStrings; i0++) { \
  NifStream( data[i0], out, version ); \
}; \

#define NI_STRINGS_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Num Strings:  " << numStrings << endl; \
for (uint i0 = 0; i0 < numStrings; i0++) { \
  out << "  Data[" << i0 << "]:  " << data[i0] << endl; \
}; \
return out.str(); \

#define NI_STRINGS_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numStrings; i0++) { \
}; \

#define NI_TEXT_KEY_EXTRA_DATA_MEMBERS \
uint unknownInt1; \
KeyArray<string > textKeys; \

#define NI_TEXT_KEY_EXTRA_DATA_PARENTS NiExtraData \

#define NI_TEXT_KEY_EXTRA_DATA_CONSTRUCT \

#define NI_TEXT_KEY_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
if ( version <= 0x04020200 ) { \
  NifStream( unknownInt1, in, version ); \
}; \
uint textKeys_numKeys; \
NifStream( textKeys_numKeys, in, version ); \
textKeys.keys.resize(textKeys_numKeys); \
for (uint i0 = 0; i0 < textKeys_numKeys; i0++) { \
  NifStream( textKeys.keys[i0], in, version ); \
}; \

#define NI_TEXT_KEY_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
if ( version <= 0x04020200 ) { \
  NifStream( unknownInt1, out, version ); \
}; \
uint textKeys_numKeys; \
textKeys_numKeys = uint(textKeys.numKeys.size()); \
NifStream( textKeys_numKeys, out, version ); \
for (uint i0 = 0; i0 < textKeys_numKeys; i0++) { \
  NifStream( textKeys.keys[i0], out, version ); \
}; \

#define NI_TEXT_KEY_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Unknown Int 1:  " << unknownInt1 << endl; \
out << "Num Keys:  " << textKeys_numKeys << endl; \
for (uint i0 = 0; i0 < textKeys_numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << textKeys.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_TEXT_KEY_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
if ( version <= 0x04020200 ) { \
}; \
for (uint i0 = 0; i0 < textKeys_numKeys; i0++) { \
}; \

#define NI_TEXTURE_EFFECT_MEMBERS \
Matrix33 modelProjectionMatrix; \
Vector3 modelProjectionTransform; \
uint textureFiltering; \
uint textureClamping; \
uint textureType; \
uint coordinateGenerationType; \
Ref<NiSourceTexture > sourceTexture; \
byte clippingPlane; \
Vector3 unknownVector; \
float unknownFloat; \
ushort ps2L; \
ushort ps2K; \
ushort unknownShort; \

#define NI_TEXTURE_EFFECT_PARENTS NiDynamicEffect \

#define NI_TEXTURE_EFFECT_CONSTRUCT \

#define NI_TEXTURE_EFFECT_READ \
uint block_num; \
NiDynamicEffect::Read( in, link_stack, version ); \
NifStream( modelProjectionMatrix, in, version ); \
NifStream( modelProjectionTransform, in, version ); \
NifStream( textureFiltering, in, version ); \
NifStream( textureClamping, in, version ); \
NifStream( textureType, in, version ); \
NifStream( coordinateGenerationType, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \
NifStream( clippingPlane, in, version ); \
NifStream( unknownVector, in, version ); \
NifStream( unknownFloat, in, version ); \
if ( version <= 0x0A020000 ) { \
  NifStream( ps2L, in, version ); \
  NifStream( ps2K, in, version ); \
}; \
if ( version <= 0x0401000C ) { \
  NifStream( unknownShort, in, version ); \
}; \

#define NI_TEXTURE_EFFECT_WRITE \
NiDynamicEffect::Write( out, link_map, version ); \
NifStream( modelProjectionMatrix, out, version ); \
NifStream( modelProjectionTransform, out, version ); \
NifStream( textureFiltering, out, version ); \
NifStream( textureClamping, out, version ); \
NifStream( textureType, out, version ); \
NifStream( coordinateGenerationType, out, version ); \
NifStream( link_map[sourceTexture], out, version ); \
NifStream( clippingPlane, out, version ); \
NifStream( unknownVector, out, version ); \
NifStream( unknownFloat, out, version ); \
if ( version <= 0x0A020000 ) { \
  NifStream( ps2L, out, version ); \
  NifStream( ps2K, out, version ); \
}; \
if ( version <= 0x0401000C ) { \
  NifStream( unknownShort, out, version ); \
}; \

#define NI_TEXTURE_EFFECT_STRING \
stringstream out; \
out << NiDynamicEffect::asString(); \
out << "Model Projection Matrix:  " << modelProjectionMatrix << endl; \
out << "Model Projection Transform:  " << modelProjectionTransform << endl; \
out << "Texture Filtering:  " << textureFiltering << endl; \
out << "Texture Clamping:  " << textureClamping << endl; \
out << "Texture Type:  " << textureType << endl; \
out << "Coordinate Generation Type:  " << coordinateGenerationType << endl; \
out << "Source Texture:  " << sourceTexture << endl; \
out << "Clipping Plane:  " << clippingPlane << endl; \
out << "Unknown Vector:  " << unknownVector << endl; \
out << "Unknown Float:  " << unknownFloat << endl; \
out << "PS2 L:  " << ps2L << endl; \
out << "PS2 K:  " << ps2K << endl; \
out << "Unknown Short:  " << unknownShort << endl; \
return out.str(); \

#define NI_TEXTURE_EFFECT_FIXLINKS \
NiDynamicEffect::FixLinks( objects, link_stack, version ); \
sourceTexture = blocks[link_stack.front()]; \
link_stack.pop_front(); \
if ( version <= 0x0A020000 ) { \
}; \
if ( version <= 0x0401000C ) { \
}; \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_MEMBERS \
byte unknown2; \
uint textureSlot; \
uint operation; \
Ref<NiFloatData > data; \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_PARENTS ASingleInterpolatorController \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_CONSTRUCT \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_READ \
uint block_num; \
ASingleInterpolatorController::Read( in, link_stack, version ); \
NifStream( unknown2, in, version ); \
NifStream( textureSlot, in, version ); \
NifStream( operation, in, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
}; \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_WRITE \
ASingleInterpolatorController::Write( out, link_map, version ); \
NifStream( unknown2, out, version ); \
NifStream( textureSlot, out, version ); \
NifStream( operation, out, version ); \
if ( version <= 0x0A010000 ) { \
  NifStream( link_map[data], out, version ); \
}; \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_STRING \
stringstream out; \
out << ASingleInterpolatorController::asString(); \
out << "Unknown2:  " << unknown2 << endl; \
out << "Texture Slot:  " << textureSlot << endl; \
out << "Operation:  " << operation << endl; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_TEXTURE_TRANSFORM_CONTROLLER_FIXLINKS \
ASingleInterpolatorController::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A010000 ) { \
  data = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
}; \

#define NI_TEXTURING_PROPERTY_MEMBERS \
Flags flags; \
ApplyMode applyMode; \
uint textureCount; \
Texture baseTexture; \
Texture darkTexture; \
Texture detailTexture; \
Texture glossTexture; \
Texture glowTexture; \
BumpMap bumpMapTexture; \
Texture decal0Texture; \
Texture decalTexture1; \
ExtraTextureGroup shaderTextures; \

#define NI_TEXTURING_PROPERTY_PARENTS NiProperty \

#define NI_TEXTURING_PROPERTY_CONSTRUCT \
 : applyMode(2), textureCount(7) \

#define NI_TEXTURING_PROPERTY_READ \
uint block_num; \
NiProperty::Read( in, link_stack, version ); \
if ( version <= 0x0A000102 ) { \
  NifStream( flags, in, version ); \
}; \
NifStream( applyMode, in, version ); \
NifStream( textureCount, in, version ); \
NifStream( baseTexture.isUsed, in, version ); \
if ( baseTexture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( baseTexture.textureData.clampMode, in, version ); \
  NifStream( baseTexture.textureData.filterMode, in, version ); \
  NifStream( baseTexture.textureData.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( baseTexture.textureData.ps2L, in, version ); \
    NifStream( baseTexture.textureData.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( baseTexture.textureData.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( baseTexture.textureData.hasTextureTransform, in, version ); \
    if ( baseTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( baseTexture.textureData.translation, in, version ); \
      NifStream( baseTexture.textureData.tiling, in, version ); \
      NifStream( baseTexture.textureData.wRotation, in, version ); \
      NifStream( baseTexture.textureData.transformType_, in, version ); \
      NifStream( baseTexture.textureData.centerOffset, in, version ); \
    }; \
  }; \
}; \
NifStream( darkTexture.isUsed, in, version ); \
if ( darkTexture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( darkTexture.textureData.clampMode, in, version ); \
  NifStream( darkTexture.textureData.filterMode, in, version ); \
  NifStream( darkTexture.textureData.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( darkTexture.textureData.ps2L, in, version ); \
    NifStream( darkTexture.textureData.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( darkTexture.textureData.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( darkTexture.textureData.hasTextureTransform, in, version ); \
    if ( darkTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( darkTexture.textureData.translation, in, version ); \
      NifStream( darkTexture.textureData.tiling, in, version ); \
      NifStream( darkTexture.textureData.wRotation, in, version ); \
      NifStream( darkTexture.textureData.transformType_, in, version ); \
      NifStream( darkTexture.textureData.centerOffset, in, version ); \
    }; \
  }; \
}; \
NifStream( detailTexture.isUsed, in, version ); \
if ( detailTexture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( detailTexture.textureData.clampMode, in, version ); \
  NifStream( detailTexture.textureData.filterMode, in, version ); \
  NifStream( detailTexture.textureData.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( detailTexture.textureData.ps2L, in, version ); \
    NifStream( detailTexture.textureData.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( detailTexture.textureData.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( detailTexture.textureData.hasTextureTransform, in, version ); \
    if ( detailTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( detailTexture.textureData.translation, in, version ); \
      NifStream( detailTexture.textureData.tiling, in, version ); \
      NifStream( detailTexture.textureData.wRotation, in, version ); \
      NifStream( detailTexture.textureData.transformType_, in, version ); \
      NifStream( detailTexture.textureData.centerOffset, in, version ); \
    }; \
  }; \
}; \
NifStream( glossTexture.isUsed, in, version ); \
if ( glossTexture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( glossTexture.textureData.clampMode, in, version ); \
  NifStream( glossTexture.textureData.filterMode, in, version ); \
  NifStream( glossTexture.textureData.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( glossTexture.textureData.ps2L, in, version ); \
    NifStream( glossTexture.textureData.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( glossTexture.textureData.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( glossTexture.textureData.hasTextureTransform, in, version ); \
    if ( glossTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( glossTexture.textureData.translation, in, version ); \
      NifStream( glossTexture.textureData.tiling, in, version ); \
      NifStream( glossTexture.textureData.wRotation, in, version ); \
      NifStream( glossTexture.textureData.transformType_, in, version ); \
      NifStream( glossTexture.textureData.centerOffset, in, version ); \
    }; \
  }; \
}; \
NifStream( glowTexture.isUsed, in, version ); \
if ( glowTexture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( glowTexture.textureData.clampMode, in, version ); \
  NifStream( glowTexture.textureData.filterMode, in, version ); \
  NifStream( glowTexture.textureData.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( glowTexture.textureData.ps2L, in, version ); \
    NifStream( glowTexture.textureData.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( glowTexture.textureData.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( glowTexture.textureData.hasTextureTransform, in, version ); \
    if ( glowTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( glowTexture.textureData.translation, in, version ); \
      NifStream( glowTexture.textureData.tiling, in, version ); \
      NifStream( glowTexture.textureData.wRotation, in, version ); \
      NifStream( glowTexture.textureData.transformType_, in, version ); \
      NifStream( glowTexture.textureData.centerOffset, in, version ); \
    }; \
  }; \
}; \
NifStream( bumpMapTexture.isUsed, in, version ); \
if ( bumpMapTexture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( bumpMapTexture.texture.clampMode, in, version ); \
  NifStream( bumpMapTexture.texture.filterMode, in, version ); \
  NifStream( bumpMapTexture.texture.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( bumpMapTexture.texture.ps2L, in, version ); \
    NifStream( bumpMapTexture.texture.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( bumpMapTexture.texture.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( bumpMapTexture.texture.hasTextureTransform, in, version ); \
    if ( bumpMapTexture.texture.hasTextureTransform != 0 ) { \
      NifStream( bumpMapTexture.texture.translation, in, version ); \
      NifStream( bumpMapTexture.texture.tiling, in, version ); \
      NifStream( bumpMapTexture.texture.wRotation, in, version ); \
      NifStream( bumpMapTexture.texture.transformType_, in, version ); \
      NifStream( bumpMapTexture.texture.centerOffset, in, version ); \
    }; \
  }; \
  NifStream( bumpMapTexture.bumpMapLumaScale, in, version ); \
  NifStream( bumpMapTexture.bumpMapLumaOffset, in, version ); \
  NifStream( bumpMapTexture.matrix, in, version ); \
}; \
NifStream( decal0Texture.isUsed, in, version ); \
if ( decal0Texture.isUsed != 0 ) { \
  NifStream( block_num, in, version ); \
  link_stack.push_back( block_num ); \
  NifStream( decal0Texture.textureData.clampMode, in, version ); \
  NifStream( decal0Texture.textureData.filterMode, in, version ); \
  NifStream( decal0Texture.textureData.textureSet, in, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( decal0Texture.textureData.ps2L, in, version ); \
    NifStream( decal0Texture.textureData.ps2K, in, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( decal0Texture.textureData.unknown1, in, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( decal0Texture.textureData.hasTextureTransform, in, version ); \
    if ( decal0Texture.textureData.hasTextureTransform != 0 ) { \
      NifStream( decal0Texture.textureData.translation, in, version ); \
      NifStream( decal0Texture.textureData.tiling, in, version ); \
      NifStream( decal0Texture.textureData.wRotation, in, version ); \
      NifStream( decal0Texture.textureData.transformType_, in, version ); \
      NifStream( decal0Texture.textureData.centerOffset, in, version ); \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  if ( textureCount == 8 ) { \
    NifStream( decalTexture1.isUsed, in, version ); \
    if ( decalTexture1.isUsed != 0 ) { \
      NifStream( block_num, in, version ); \
      link_stack.push_back( block_num ); \
      NifStream( decalTexture1.textureData.clampMode, in, version ); \
      NifStream( decalTexture1.textureData.filterMode, in, version ); \
      NifStream( decalTexture1.textureData.textureSet, in, version ); \
      if ( version <= 0x0A020000 ) { \
        NifStream( decalTexture1.textureData.ps2L, in, version ); \
        NifStream( decalTexture1.textureData.ps2K, in, version ); \
      }; \
      if ( version <= 0x0401000C ) { \
        NifStream( decalTexture1.textureData.unknown1, in, version ); \
      }; \
      if ( version >= 0x0A010000 ) { \
        NifStream( decalTexture1.textureData.hasTextureTransform, in, version ); \
        if ( decalTexture1.textureData.hasTextureTransform != 0 ) { \
          NifStream( decalTexture1.textureData.translation, in, version ); \
          NifStream( decalTexture1.textureData.tiling, in, version ); \
          NifStream( decalTexture1.textureData.wRotation, in, version ); \
          NifStream( decalTexture1.textureData.transformType_, in, version ); \
          NifStream( decalTexture1.textureData.centerOffset, in, version ); \
        }; \
      }; \
    }; \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  uint shaderTextures_numTextures; \
  NifStream( shaderTextures_numTextures, in, version ); \
  shaderTextures.textures.resize(shaderTextures_numTextures); \
  for (uint i1 = 0; i1 < shaderTextures_numTextures; i1++) { \
    NifStream( shaderTextures.textures[i1].isUsed, in, version ); \
    if ( shaderTextures.textures[i1].isUsed != 0 ) { \
      NifStream( block_num, in, version ); \
      link_stack.push_back( block_num ); \
      NifStream( shaderTextures.textures[i1].textureData.clampMode, in, version ); \
      NifStream( shaderTextures.textures[i1].textureData.filterMode, in, version ); \
      NifStream( shaderTextures.textures[i1].textureData.textureSet, in, version ); \
      if ( version <= 0x0A020000 ) { \
        NifStream( shaderTextures.textures[i1].textureData.ps2L, in, version ); \
        NifStream( shaderTextures.textures[i1].textureData.ps2K, in, version ); \
      }; \
      if ( version <= 0x0401000C ) { \
        NifStream( shaderTextures.textures[i1].textureData.unknown1, in, version ); \
      }; \
      if ( version >= 0x0A010000 ) { \
        NifStream( shaderTextures.textures[i1].textureData.hasTextureTransform, in, version ); \
        if ( shaderTextures.textures[i1].textureData.hasTextureTransform != 0 ) { \
          NifStream( shaderTextures.textures[i1].textureData.translation, in, version ); \
          NifStream( shaderTextures.textures[i1].textureData.tiling, in, version ); \
          NifStream( shaderTextures.textures[i1].textureData.wRotation, in, version ); \
          NifStream( shaderTextures.textures[i1].textureData.transformType_, in, version ); \
          NifStream( shaderTextures.textures[i1].textureData.centerOffset, in, version ); \
        }; \
      }; \
      NifStream( shaderTextures.textures[i1].unknownInt, in, version ); \
    }; \
  }; \
}; \

#define NI_TEXTURING_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
if ( version <= 0x0A000102 ) { \
  NifStream( flags, out, version ); \
}; \
NifStream( applyMode, out, version ); \
NifStream( textureCount, out, version ); \
NifStream( baseTexture.isUsed, out, version ); \
if ( baseTexture.isUsed != 0 ) { \
  NifStream( link_map[baseTexture.textureData.source], out, version ); \
  NifStream( baseTexture.textureData.clampMode, out, version ); \
  NifStream( baseTexture.textureData.filterMode, out, version ); \
  NifStream( baseTexture.textureData.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( baseTexture.textureData.ps2L, out, version ); \
    NifStream( baseTexture.textureData.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( baseTexture.textureData.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( baseTexture.textureData.hasTextureTransform, out, version ); \
    if ( baseTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( baseTexture.textureData.translation, out, version ); \
      NifStream( baseTexture.textureData.tiling, out, version ); \
      NifStream( baseTexture.textureData.wRotation, out, version ); \
      NifStream( baseTexture.textureData.transformType_, out, version ); \
      NifStream( baseTexture.textureData.centerOffset, out, version ); \
    }; \
  }; \
}; \
NifStream( darkTexture.isUsed, out, version ); \
if ( darkTexture.isUsed != 0 ) { \
  NifStream( link_map[darkTexture.textureData.source], out, version ); \
  NifStream( darkTexture.textureData.clampMode, out, version ); \
  NifStream( darkTexture.textureData.filterMode, out, version ); \
  NifStream( darkTexture.textureData.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( darkTexture.textureData.ps2L, out, version ); \
    NifStream( darkTexture.textureData.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( darkTexture.textureData.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( darkTexture.textureData.hasTextureTransform, out, version ); \
    if ( darkTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( darkTexture.textureData.translation, out, version ); \
      NifStream( darkTexture.textureData.tiling, out, version ); \
      NifStream( darkTexture.textureData.wRotation, out, version ); \
      NifStream( darkTexture.textureData.transformType_, out, version ); \
      NifStream( darkTexture.textureData.centerOffset, out, version ); \
    }; \
  }; \
}; \
NifStream( detailTexture.isUsed, out, version ); \
if ( detailTexture.isUsed != 0 ) { \
  NifStream( link_map[detailTexture.textureData.source], out, version ); \
  NifStream( detailTexture.textureData.clampMode, out, version ); \
  NifStream( detailTexture.textureData.filterMode, out, version ); \
  NifStream( detailTexture.textureData.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( detailTexture.textureData.ps2L, out, version ); \
    NifStream( detailTexture.textureData.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( detailTexture.textureData.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( detailTexture.textureData.hasTextureTransform, out, version ); \
    if ( detailTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( detailTexture.textureData.translation, out, version ); \
      NifStream( detailTexture.textureData.tiling, out, version ); \
      NifStream( detailTexture.textureData.wRotation, out, version ); \
      NifStream( detailTexture.textureData.transformType_, out, version ); \
      NifStream( detailTexture.textureData.centerOffset, out, version ); \
    }; \
  }; \
}; \
NifStream( glossTexture.isUsed, out, version ); \
if ( glossTexture.isUsed != 0 ) { \
  NifStream( link_map[glossTexture.textureData.source], out, version ); \
  NifStream( glossTexture.textureData.clampMode, out, version ); \
  NifStream( glossTexture.textureData.filterMode, out, version ); \
  NifStream( glossTexture.textureData.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( glossTexture.textureData.ps2L, out, version ); \
    NifStream( glossTexture.textureData.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( glossTexture.textureData.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( glossTexture.textureData.hasTextureTransform, out, version ); \
    if ( glossTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( glossTexture.textureData.translation, out, version ); \
      NifStream( glossTexture.textureData.tiling, out, version ); \
      NifStream( glossTexture.textureData.wRotation, out, version ); \
      NifStream( glossTexture.textureData.transformType_, out, version ); \
      NifStream( glossTexture.textureData.centerOffset, out, version ); \
    }; \
  }; \
}; \
NifStream( glowTexture.isUsed, out, version ); \
if ( glowTexture.isUsed != 0 ) { \
  NifStream( link_map[glowTexture.textureData.source], out, version ); \
  NifStream( glowTexture.textureData.clampMode, out, version ); \
  NifStream( glowTexture.textureData.filterMode, out, version ); \
  NifStream( glowTexture.textureData.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( glowTexture.textureData.ps2L, out, version ); \
    NifStream( glowTexture.textureData.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( glowTexture.textureData.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( glowTexture.textureData.hasTextureTransform, out, version ); \
    if ( glowTexture.textureData.hasTextureTransform != 0 ) { \
      NifStream( glowTexture.textureData.translation, out, version ); \
      NifStream( glowTexture.textureData.tiling, out, version ); \
      NifStream( glowTexture.textureData.wRotation, out, version ); \
      NifStream( glowTexture.textureData.transformType_, out, version ); \
      NifStream( glowTexture.textureData.centerOffset, out, version ); \
    }; \
  }; \
}; \
NifStream( bumpMapTexture.isUsed, out, version ); \
if ( bumpMapTexture.isUsed != 0 ) { \
  NifStream( link_map[bumpMapTexture.texture.source], out, version ); \
  NifStream( bumpMapTexture.texture.clampMode, out, version ); \
  NifStream( bumpMapTexture.texture.filterMode, out, version ); \
  NifStream( bumpMapTexture.texture.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( bumpMapTexture.texture.ps2L, out, version ); \
    NifStream( bumpMapTexture.texture.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( bumpMapTexture.texture.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( bumpMapTexture.texture.hasTextureTransform, out, version ); \
    if ( bumpMapTexture.texture.hasTextureTransform != 0 ) { \
      NifStream( bumpMapTexture.texture.translation, out, version ); \
      NifStream( bumpMapTexture.texture.tiling, out, version ); \
      NifStream( bumpMapTexture.texture.wRotation, out, version ); \
      NifStream( bumpMapTexture.texture.transformType_, out, version ); \
      NifStream( bumpMapTexture.texture.centerOffset, out, version ); \
    }; \
  }; \
  NifStream( bumpMapTexture.bumpMapLumaScale, out, version ); \
  NifStream( bumpMapTexture.bumpMapLumaOffset, out, version ); \
  NifStream( bumpMapTexture.matrix, out, version ); \
}; \
NifStream( decal0Texture.isUsed, out, version ); \
if ( decal0Texture.isUsed != 0 ) { \
  NifStream( link_map[decal0Texture.textureData.source], out, version ); \
  NifStream( decal0Texture.textureData.clampMode, out, version ); \
  NifStream( decal0Texture.textureData.filterMode, out, version ); \
  NifStream( decal0Texture.textureData.textureSet, out, version ); \
  if ( version <= 0x0A020000 ) { \
    NifStream( decal0Texture.textureData.ps2L, out, version ); \
    NifStream( decal0Texture.textureData.ps2K, out, version ); \
  }; \
  if ( version <= 0x0401000C ) { \
    NifStream( decal0Texture.textureData.unknown1, out, version ); \
  }; \
  if ( version >= 0x0A010000 ) { \
    NifStream( decal0Texture.textureData.hasTextureTransform, out, version ); \
    if ( decal0Texture.textureData.hasTextureTransform != 0 ) { \
      NifStream( decal0Texture.textureData.translation, out, version ); \
      NifStream( decal0Texture.textureData.tiling, out, version ); \
      NifStream( decal0Texture.textureData.wRotation, out, version ); \
      NifStream( decal0Texture.textureData.transformType_, out, version ); \
      NifStream( decal0Texture.textureData.centerOffset, out, version ); \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  if ( textureCount == 8 ) { \
    NifStream( decalTexture1.isUsed, out, version ); \
    if ( decalTexture1.isUsed != 0 ) { \
      NifStream( link_map[decalTexture1.textureData.source], out, version ); \
      NifStream( decalTexture1.textureData.clampMode, out, version ); \
      NifStream( decalTexture1.textureData.filterMode, out, version ); \
      NifStream( decalTexture1.textureData.textureSet, out, version ); \
      if ( version <= 0x0A020000 ) { \
        NifStream( decalTexture1.textureData.ps2L, out, version ); \
        NifStream( decalTexture1.textureData.ps2K, out, version ); \
      }; \
      if ( version <= 0x0401000C ) { \
        NifStream( decalTexture1.textureData.unknown1, out, version ); \
      }; \
      if ( version >= 0x0A010000 ) { \
        NifStream( decalTexture1.textureData.hasTextureTransform, out, version ); \
        if ( decalTexture1.textureData.hasTextureTransform != 0 ) { \
          NifStream( decalTexture1.textureData.translation, out, version ); \
          NifStream( decalTexture1.textureData.tiling, out, version ); \
          NifStream( decalTexture1.textureData.wRotation, out, version ); \
          NifStream( decalTexture1.textureData.transformType_, out, version ); \
          NifStream( decalTexture1.textureData.centerOffset, out, version ); \
        }; \
      }; \
    }; \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  uint shaderTextures_numTextures; \
  shaderTextures_numTextures = uint(shaderTextures.numTextures.size()); \
  NifStream( shaderTextures_numTextures, out, version ); \
  for (uint i1 = 0; i1 < shaderTextures_numTextures; i1++) { \
    NifStream( shaderTextures.textures[i1].isUsed, out, version ); \
    if ( shaderTextures.textures[i1].isUsed != 0 ) { \
      NifStream( link_map[shaderTextures.textures[i1].textureData.source], out, version ); \
      NifStream( shaderTextures.textures[i1].textureData.clampMode, out, version ); \
      NifStream( shaderTextures.textures[i1].textureData.filterMode, out, version ); \
      NifStream( shaderTextures.textures[i1].textureData.textureSet, out, version ); \
      if ( version <= 0x0A020000 ) { \
        NifStream( shaderTextures.textures[i1].textureData.ps2L, out, version ); \
        NifStream( shaderTextures.textures[i1].textureData.ps2K, out, version ); \
      }; \
      if ( version <= 0x0401000C ) { \
        NifStream( shaderTextures.textures[i1].textureData.unknown1, out, version ); \
      }; \
      if ( version >= 0x0A010000 ) { \
        NifStream( shaderTextures.textures[i1].textureData.hasTextureTransform, out, version ); \
        if ( shaderTextures.textures[i1].textureData.hasTextureTransform != 0 ) { \
          NifStream( shaderTextures.textures[i1].textureData.translation, out, version ); \
          NifStream( shaderTextures.textures[i1].textureData.tiling, out, version ); \
          NifStream( shaderTextures.textures[i1].textureData.wRotation, out, version ); \
          NifStream( shaderTextures.textures[i1].textureData.transformType_, out, version ); \
          NifStream( shaderTextures.textures[i1].textureData.centerOffset, out, version ); \
        }; \
      }; \
      NifStream( shaderTextures.textures[i1].unknownInt, out, version ); \
    }; \
  }; \
}; \

#define NI_TEXTURING_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Apply Mode:  " << applyMode << endl; \
out << "Texture Count:  " << textureCount << endl; \
out << "Is Used:  " << baseTexture.isUsed << endl; \
if ( baseTexture.isUsed != 0 ) { \
  out << "  Source:  " << baseTexture.textureData.source << endl; \
  out << "  Clamp Mode:  " << baseTexture.textureData.clampMode << endl; \
  out << "  Filter Mode:  " << baseTexture.textureData.filterMode << endl; \
  out << "  Texture Set:  " << baseTexture.textureData.textureSet << endl; \
  out << "  PS2 L:  " << baseTexture.textureData.ps2L << endl; \
  out << "  PS2 K:  " << baseTexture.textureData.ps2K << endl; \
  out << "  Unknown1:  " << baseTexture.textureData.unknown1 << endl; \
  out << "  Has Texture Transform:  " << baseTexture.textureData.hasTextureTransform << endl; \
  if ( baseTexture.textureData.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << baseTexture.textureData.translation << endl; \
    out << "    Tiling:  " << baseTexture.textureData.tiling << endl; \
    out << "    W Rotation:  " << baseTexture.textureData.wRotation << endl; \
    out << "    Transform Type?:  " << baseTexture.textureData.transformType_ << endl; \
    out << "    Center Offset:  " << baseTexture.textureData.centerOffset << endl; \
  }; \
}; \
out << "Is Used:  " << darkTexture.isUsed << endl; \
if ( darkTexture.isUsed != 0 ) { \
  out << "  Source:  " << darkTexture.textureData.source << endl; \
  out << "  Clamp Mode:  " << darkTexture.textureData.clampMode << endl; \
  out << "  Filter Mode:  " << darkTexture.textureData.filterMode << endl; \
  out << "  Texture Set:  " << darkTexture.textureData.textureSet << endl; \
  out << "  PS2 L:  " << darkTexture.textureData.ps2L << endl; \
  out << "  PS2 K:  " << darkTexture.textureData.ps2K << endl; \
  out << "  Unknown1:  " << darkTexture.textureData.unknown1 << endl; \
  out << "  Has Texture Transform:  " << darkTexture.textureData.hasTextureTransform << endl; \
  if ( darkTexture.textureData.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << darkTexture.textureData.translation << endl; \
    out << "    Tiling:  " << darkTexture.textureData.tiling << endl; \
    out << "    W Rotation:  " << darkTexture.textureData.wRotation << endl; \
    out << "    Transform Type?:  " << darkTexture.textureData.transformType_ << endl; \
    out << "    Center Offset:  " << darkTexture.textureData.centerOffset << endl; \
  }; \
}; \
out << "Is Used:  " << detailTexture.isUsed << endl; \
if ( detailTexture.isUsed != 0 ) { \
  out << "  Source:  " << detailTexture.textureData.source << endl; \
  out << "  Clamp Mode:  " << detailTexture.textureData.clampMode << endl; \
  out << "  Filter Mode:  " << detailTexture.textureData.filterMode << endl; \
  out << "  Texture Set:  " << detailTexture.textureData.textureSet << endl; \
  out << "  PS2 L:  " << detailTexture.textureData.ps2L << endl; \
  out << "  PS2 K:  " << detailTexture.textureData.ps2K << endl; \
  out << "  Unknown1:  " << detailTexture.textureData.unknown1 << endl; \
  out << "  Has Texture Transform:  " << detailTexture.textureData.hasTextureTransform << endl; \
  if ( detailTexture.textureData.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << detailTexture.textureData.translation << endl; \
    out << "    Tiling:  " << detailTexture.textureData.tiling << endl; \
    out << "    W Rotation:  " << detailTexture.textureData.wRotation << endl; \
    out << "    Transform Type?:  " << detailTexture.textureData.transformType_ << endl; \
    out << "    Center Offset:  " << detailTexture.textureData.centerOffset << endl; \
  }; \
}; \
out << "Is Used:  " << glossTexture.isUsed << endl; \
if ( glossTexture.isUsed != 0 ) { \
  out << "  Source:  " << glossTexture.textureData.source << endl; \
  out << "  Clamp Mode:  " << glossTexture.textureData.clampMode << endl; \
  out << "  Filter Mode:  " << glossTexture.textureData.filterMode << endl; \
  out << "  Texture Set:  " << glossTexture.textureData.textureSet << endl; \
  out << "  PS2 L:  " << glossTexture.textureData.ps2L << endl; \
  out << "  PS2 K:  " << glossTexture.textureData.ps2K << endl; \
  out << "  Unknown1:  " << glossTexture.textureData.unknown1 << endl; \
  out << "  Has Texture Transform:  " << glossTexture.textureData.hasTextureTransform << endl; \
  if ( glossTexture.textureData.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << glossTexture.textureData.translation << endl; \
    out << "    Tiling:  " << glossTexture.textureData.tiling << endl; \
    out << "    W Rotation:  " << glossTexture.textureData.wRotation << endl; \
    out << "    Transform Type?:  " << glossTexture.textureData.transformType_ << endl; \
    out << "    Center Offset:  " << glossTexture.textureData.centerOffset << endl; \
  }; \
}; \
out << "Is Used:  " << glowTexture.isUsed << endl; \
if ( glowTexture.isUsed != 0 ) { \
  out << "  Source:  " << glowTexture.textureData.source << endl; \
  out << "  Clamp Mode:  " << glowTexture.textureData.clampMode << endl; \
  out << "  Filter Mode:  " << glowTexture.textureData.filterMode << endl; \
  out << "  Texture Set:  " << glowTexture.textureData.textureSet << endl; \
  out << "  PS2 L:  " << glowTexture.textureData.ps2L << endl; \
  out << "  PS2 K:  " << glowTexture.textureData.ps2K << endl; \
  out << "  Unknown1:  " << glowTexture.textureData.unknown1 << endl; \
  out << "  Has Texture Transform:  " << glowTexture.textureData.hasTextureTransform << endl; \
  if ( glowTexture.textureData.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << glowTexture.textureData.translation << endl; \
    out << "    Tiling:  " << glowTexture.textureData.tiling << endl; \
    out << "    W Rotation:  " << glowTexture.textureData.wRotation << endl; \
    out << "    Transform Type?:  " << glowTexture.textureData.transformType_ << endl; \
    out << "    Center Offset:  " << glowTexture.textureData.centerOffset << endl; \
  }; \
}; \
out << "Is Used:  " << bumpMapTexture.isUsed << endl; \
if ( bumpMapTexture.isUsed != 0 ) { \
  out << "  Source:  " << bumpMapTexture.texture.source << endl; \
  out << "  Clamp Mode:  " << bumpMapTexture.texture.clampMode << endl; \
  out << "  Filter Mode:  " << bumpMapTexture.texture.filterMode << endl; \
  out << "  Texture Set:  " << bumpMapTexture.texture.textureSet << endl; \
  out << "  PS2 L:  " << bumpMapTexture.texture.ps2L << endl; \
  out << "  PS2 K:  " << bumpMapTexture.texture.ps2K << endl; \
  out << "  Unknown1:  " << bumpMapTexture.texture.unknown1 << endl; \
  out << "  Has Texture Transform:  " << bumpMapTexture.texture.hasTextureTransform << endl; \
  if ( bumpMapTexture.texture.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << bumpMapTexture.texture.translation << endl; \
    out << "    Tiling:  " << bumpMapTexture.texture.tiling << endl; \
    out << "    W Rotation:  " << bumpMapTexture.texture.wRotation << endl; \
    out << "    Transform Type?:  " << bumpMapTexture.texture.transformType_ << endl; \
    out << "    Center Offset:  " << bumpMapTexture.texture.centerOffset << endl; \
  }; \
  out << "  Bump Map Luma Scale:  " << bumpMapTexture.bumpMapLumaScale << endl; \
  out << "  Bump Map Luma Offset:  " << bumpMapTexture.bumpMapLumaOffset << endl; \
  out << "  Matrix:  " << bumpMapTexture.matrix << endl; \
}; \
out << "Is Used:  " << decal0Texture.isUsed << endl; \
if ( decal0Texture.isUsed != 0 ) { \
  out << "  Source:  " << decal0Texture.textureData.source << endl; \
  out << "  Clamp Mode:  " << decal0Texture.textureData.clampMode << endl; \
  out << "  Filter Mode:  " << decal0Texture.textureData.filterMode << endl; \
  out << "  Texture Set:  " << decal0Texture.textureData.textureSet << endl; \
  out << "  PS2 L:  " << decal0Texture.textureData.ps2L << endl; \
  out << "  PS2 K:  " << decal0Texture.textureData.ps2K << endl; \
  out << "  Unknown1:  " << decal0Texture.textureData.unknown1 << endl; \
  out << "  Has Texture Transform:  " << decal0Texture.textureData.hasTextureTransform << endl; \
  if ( decal0Texture.textureData.hasTextureTransform != 0 ) { \
    out << "    Translation:  " << decal0Texture.textureData.translation << endl; \
    out << "    Tiling:  " << decal0Texture.textureData.tiling << endl; \
    out << "    W Rotation:  " << decal0Texture.textureData.wRotation << endl; \
    out << "    Transform Type?:  " << decal0Texture.textureData.transformType_ << endl; \
    out << "    Center Offset:  " << decal0Texture.textureData.centerOffset << endl; \
  }; \
}; \
if ( textureCount == 8 ) { \
  out << "  Is Used:  " << decalTexture1.isUsed << endl; \
  if ( decalTexture1.isUsed != 0 ) { \
    out << "    Source:  " << decalTexture1.textureData.source << endl; \
    out << "    Clamp Mode:  " << decalTexture1.textureData.clampMode << endl; \
    out << "    Filter Mode:  " << decalTexture1.textureData.filterMode << endl; \
    out << "    Texture Set:  " << decalTexture1.textureData.textureSet << endl; \
    out << "    PS2 L:  " << decalTexture1.textureData.ps2L << endl; \
    out << "    PS2 K:  " << decalTexture1.textureData.ps2K << endl; \
    out << "    Unknown1:  " << decalTexture1.textureData.unknown1 << endl; \
    out << "    Has Texture Transform:  " << decalTexture1.textureData.hasTextureTransform << endl; \
    if ( decalTexture1.textureData.hasTextureTransform != 0 ) { \
      out << "      Translation:  " << decalTexture1.textureData.translation << endl; \
      out << "      Tiling:  " << decalTexture1.textureData.tiling << endl; \
      out << "      W Rotation:  " << decalTexture1.textureData.wRotation << endl; \
      out << "      Transform Type?:  " << decalTexture1.textureData.transformType_ << endl; \
      out << "      Center Offset:  " << decalTexture1.textureData.centerOffset << endl; \
    }; \
  }; \
}; \
out << "Num Textures:  " << shaderTextures_numTextures << endl; \
for (uint i0 = 0; i0 < shaderTextures_numTextures; i0++) { \
  out << "  Is Used:  " << shaderTextures.textures[i0].isUsed << endl; \
  if ( shaderTextures.textures[i0].isUsed != 0 ) { \
    out << "    Source:  " << shaderTextures.textures[i0].textureData.source << endl; \
    out << "    Clamp Mode:  " << shaderTextures.textures[i0].textureData.clampMode << endl; \
    out << "    Filter Mode:  " << shaderTextures.textures[i0].textureData.filterMode << endl; \
    out << "    Texture Set:  " << shaderTextures.textures[i0].textureData.textureSet << endl; \
    out << "    PS2 L:  " << shaderTextures.textures[i0].textureData.ps2L << endl; \
    out << "    PS2 K:  " << shaderTextures.textures[i0].textureData.ps2K << endl; \
    out << "    Unknown1:  " << shaderTextures.textures[i0].textureData.unknown1 << endl; \
    out << "    Has Texture Transform:  " << shaderTextures.textures[i0].textureData.hasTextureTransform << endl; \
    if ( shaderTextures.textures[i0].textureData.hasTextureTransform != 0 ) { \
      out << "      Translation:  " << shaderTextures.textures[i0].textureData.translation << endl; \
      out << "      Tiling:  " << shaderTextures.textures[i0].textureData.tiling << endl; \
      out << "      W Rotation:  " << shaderTextures.textures[i0].textureData.wRotation << endl; \
      out << "      Transform Type?:  " << shaderTextures.textures[i0].textureData.transformType_ << endl; \
      out << "      Center Offset:  " << shaderTextures.textures[i0].textureData.centerOffset << endl; \
    }; \
    out << "    Unknown Int:  " << shaderTextures.textures[i0].unknownInt << endl; \
  }; \
}; \
return out.str(); \

#define NI_TEXTURING_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \
if ( version <= 0x0A000102 ) { \
}; \
if ( baseTexture.isUsed != 0 ) { \
  baseTexture.textureData.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( baseTexture.textureData.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( darkTexture.isUsed != 0 ) { \
  darkTexture.textureData.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( darkTexture.textureData.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( detailTexture.isUsed != 0 ) { \
  detailTexture.textureData.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( detailTexture.textureData.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( glossTexture.isUsed != 0 ) { \
  glossTexture.textureData.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( glossTexture.textureData.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( glowTexture.isUsed != 0 ) { \
  glowTexture.textureData.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( glowTexture.textureData.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( bumpMapTexture.isUsed != 0 ) { \
  bumpMapTexture.texture.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( bumpMapTexture.texture.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( decal0Texture.isUsed != 0 ) { \
  decal0Texture.textureData.source = blocks[link_stack.front()]; \
  link_stack.pop_front(); \
  if ( version <= 0x0A020000 ) { \
  }; \
  if ( version <= 0x0401000C ) { \
  }; \
  if ( version >= 0x0A010000 ) { \
    if ( decal0Texture.textureData.hasTextureTransform != 0 ) { \
    }; \
  }; \
}; \
if ( version >= 0x14000004 ) { \
  if ( textureCount == 8 ) { \
    if ( decalTexture1.isUsed != 0 ) { \
      decalTexture1.textureData.source = blocks[link_stack.front()]; \
      link_stack.pop_front(); \
      if ( version <= 0x0A020000 ) { \
      }; \
      if ( version <= 0x0401000C ) { \
      }; \
      if ( version >= 0x0A010000 ) { \
        if ( decalTexture1.textureData.hasTextureTransform != 0 ) { \
        }; \
      }; \
    }; \
  }; \
}; \
if ( version >= 0x0A000100 ) { \
  for (uint i1 = 0; i1 < shaderTextures_numTextures; i1++) { \
    if ( shaderTextures.textures[i1].isUsed != 0 ) { \
      shaderTextures.textures[i1].textureData.source = blocks[link_stack.front()]; \
      link_stack.pop_front(); \
      if ( version <= 0x0A020000 ) { \
      }; \
      if ( version <= 0x0401000C ) { \
      }; \
      if ( version >= 0x0A010000 ) { \
        if ( shaderTextures.textures[i1].textureData.hasTextureTransform != 0 ) { \
        }; \
      }; \
    }; \
  }; \
}; \

#define NI_TRANSFORM_CONTROLLER_MEMBERS \

#define NI_TRANSFORM_CONTROLLER_PARENTS ASingleInterpolatorController \

#define NI_TRANSFORM_CONTROLLER_CONSTRUCT \

#define NI_TRANSFORM_CONTROLLER_READ \
ASingleInterpolatorController::Read( in, link_stack, version ); \

#define NI_TRANSFORM_CONTROLLER_WRITE \
ASingleInterpolatorController::Write( out, link_map, version ); \

#define NI_TRANSFORM_CONTROLLER_STRING \
stringstream out; \
out << ASingleInterpolatorController::asString(); \
return out.str(); \

#define NI_TRANSFORM_CONTROLLER_FIXLINKS \
ASingleInterpolatorController::FixLinks( objects, link_stack, version ); \

#define NI_TRANSFORM_DATA_MEMBERS \

#define NI_TRANSFORM_DATA_PARENTS AKeyframeData \

#define NI_TRANSFORM_DATA_CONSTRUCT \

#define NI_TRANSFORM_DATA_READ \
AKeyframeData::Read( in, link_stack, version ); \

#define NI_TRANSFORM_DATA_WRITE \
AKeyframeData::Write( out, link_map, version ); \

#define NI_TRANSFORM_DATA_STRING \
stringstream out; \
out << AKeyframeData::asString(); \
return out.str(); \

#define NI_TRANSFORM_DATA_FIXLINKS \
AKeyframeData::FixLinks( objects, link_stack, version ); \

#define NI_TRANSFORM_INTERPOLATOR_MEMBERS \
Vector3 translation; \
Quaternion rotation; \
float scale; \
vector<byte > unknownBytes; \
Ref<NiTransformData > data; \

#define NI_TRANSFORM_INTERPOLATOR_PARENTS AInterpolator \

#define NI_TRANSFORM_INTERPOLATOR_CONSTRUCT \

#define NI_TRANSFORM_INTERPOLATOR_READ \
uint block_num; \
AInterpolator::Read( in, link_stack, version ); \
NifStream( translation, in, version ); \
NifStream( rotation, in, version ); \
NifStream( scale, in, version ); \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  unknownBytes.resize(3); \
  for (uint i1 = 0; i1 < 3; i1++) { \
    NifStream( unknownBytes[i1], in, version ); \
  }; \
}; \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_TRANSFORM_INTERPOLATOR_WRITE \
AInterpolator::Write( out, link_map, version ); \
NifStream( translation, out, version ); \
NifStream( rotation, out, version ); \
NifStream( scale, out, version ); \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  for (uint i1 = 0; i1 < 3; i1++) { \
    NifStream( unknownBytes[i1], out, version ); \
  }; \
}; \
NifStream( link_map[data], out, version ); \

#define NI_TRANSFORM_INTERPOLATOR_STRING \
stringstream out; \
out << AInterpolator::asString(); \
out << "Translation:  " << translation << endl; \
out << "Rotation:  " << rotation << endl; \
out << "Scale:  " << scale << endl; \
for (uint i0 = 0; i0 < 3; i0++) { \
  out << "  Unknown Bytes[" << i0 << "]:  " << unknownBytes[i0] << endl; \
}; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_TRANSFORM_INTERPOLATOR_FIXLINKS \
AInterpolator::FixLinks( objects, link_stack, version ); \
if ( ( version >= 0x0A01006A ) && ( version <= 0x0A01006A ) ) { \
  for (uint i1 = 0; i1 < 3; i1++) { \
  }; \
}; \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_TRI_SHAPE_MEMBERS \

#define NI_TRI_SHAPE_PARENTS NiTriBasedGeom \

#define NI_TRI_SHAPE_CONSTRUCT \

#define NI_TRI_SHAPE_READ \
NiTriBasedGeom::Read( in, link_stack, version ); \

#define NI_TRI_SHAPE_WRITE \
NiTriBasedGeom::Write( out, link_map, version ); \

#define NI_TRI_SHAPE_STRING \
stringstream out; \
out << NiTriBasedGeom::asString(); \
return out.str(); \

#define NI_TRI_SHAPE_FIXLINKS \
NiTriBasedGeom::FixLinks( objects, link_stack, version ); \

#define NI_TRI_SHAPE_DATA_MEMBERS \
uint numTrianglePoints; \
bool hasTriangles; \
vector<Triangle > triangles; \
vector<MatchGroup > matchGroups; \

#define NI_TRI_SHAPE_DATA_PARENTS TriBasedGeomData \

#define NI_TRI_SHAPE_DATA_CONSTRUCT \

#define NI_TRI_SHAPE_DATA_READ \
TriBasedGeomData::Read( in, link_stack, version ); \
ushort numTriangles; \
ushort numMatchGroups; \
NifStream( numTriangles, in, version ); \
NifStream( numTrianglePoints, in, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( hasTriangles, in, version ); \
}; \
if ( version <= 0x0A000102 ) { \
  triangles.resize(numTriangles); \
  for (uint i1 = 0; i1 < numTriangles; i1++) { \
    NifStream( triangles[i1], in, version ); \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( hasTriangles != 0 ) { \
    triangles.resize(numTriangles); \
    for (uint i2 = 0; i2 < numTriangles; i2++) { \
      NifStream( triangles[i2], in, version ); \
    }; \
  }; \
}; \
NifStream( numMatchGroups, in, version ); \
matchGroups.resize(numMatchGroups); \
for (uint i0 = 0; i0 < numMatchGroups; i0++) { \
  ushort matchGroups_numVertices; \
  NifStream( matchGroups_numVertices, in, version ); \
  matchGroups[i0].vertexIndices.resize(matchGroups_numVertices); \
  for (uint i1 = 0; i1 < matchGroups_numVertices; i1++) { \
    NifStream( matchGroups[i0].vertexIndices[i1], in, version ); \
  }; \
}; \

#define NI_TRI_SHAPE_DATA_WRITE \
TriBasedGeomData::Write( out, link_map, version ); \
ushort numTriangles; \
numTriangles = ushort(numTriangles.size()); \
ushort numMatchGroups; \
numMatchGroups = ushort(numMatchGroups.size()); \
NifStream( numTriangles, out, version ); \
NifStream( numTrianglePoints, out, version ); \
if ( version >= 0x0A010000 ) { \
  NifStream( hasTriangles, out, version ); \
}; \
if ( version <= 0x0A000102 ) { \
  for (uint i1 = 0; i1 < numTriangles; i1++) { \
    NifStream( triangles[i1], out, version ); \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( hasTriangles != 0 ) { \
    for (uint i2 = 0; i2 < numTriangles; i2++) { \
      NifStream( triangles[i2], out, version ); \
    }; \
  }; \
}; \
NifStream( numMatchGroups, out, version ); \
for (uint i0 = 0; i0 < numMatchGroups; i0++) { \
  ushort matchGroups_numVertices; \
  matchGroups_numVertices = ushort(matchGroups[i0].numVertices.size()); \
  NifStream( matchGroups_numVertices, out, version ); \
  for (uint i1 = 0; i1 < matchGroups_numVertices; i1++) { \
    NifStream( matchGroups[i0].vertexIndices[i1], out, version ); \
  }; \
}; \

#define NI_TRI_SHAPE_DATA_STRING \
stringstream out; \
out << TriBasedGeomData::asString(); \
out << "Num Triangles:  " << numTriangles << endl; \
out << "Num Triangle Points:  " << numTrianglePoints << endl; \
out << "Has Triangles:  " << hasTriangles << endl; \
for (uint i0 = 0; i0 < numTriangles; i0++) { \
  out << "  Triangles[" << i0 << "]:  " << triangles[i0] << endl; \
}; \
if ( hasTriangles != 0 ) { \
  for (uint i1 = 0; i1 < numTriangles; i1++) { \
    out << "    Triangles[" << i1 << "]:  " << triangles[i1] << endl; \
  }; \
}; \
out << "Num Match Groups:  " << numMatchGroups << endl; \
for (uint i0 = 0; i0 < numMatchGroups; i0++) { \
  out << "  Num Vertices:  " << matchGroups_numVertices << endl; \
  for (uint i1 = 0; i1 < matchGroups_numVertices; i1++) { \
    out << "    Vertex Indices[" << i1 << "]:  " << matchGroups[i0].vertexIndices[i1] << endl; \
  }; \
}; \
return out.str(); \

#define NI_TRI_SHAPE_DATA_FIXLINKS \
TriBasedGeomData::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0A010000 ) { \
}; \
if ( version <= 0x0A000102 ) { \
  for (uint i1 = 0; i1 < numTriangles; i1++) { \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( hasTriangles != 0 ) { \
    for (uint i2 = 0; i2 < numTriangles; i2++) { \
    }; \
  }; \
}; \
for (uint i0 = 0; i0 < numMatchGroups; i0++) { \
  for (uint i1 = 0; i1 < matchGroups_numVertices; i1++) { \
  }; \
}; \

#define NI_TRI_STRIPS_MEMBERS \

#define NI_TRI_STRIPS_PARENTS NiTriBasedGeom \

#define NI_TRI_STRIPS_CONSTRUCT \

#define NI_TRI_STRIPS_READ \
NiTriBasedGeom::Read( in, link_stack, version ); \

#define NI_TRI_STRIPS_WRITE \
NiTriBasedGeom::Write( out, link_map, version ); \

#define NI_TRI_STRIPS_STRING \
stringstream out; \
out << NiTriBasedGeom::asString(); \
return out.str(); \

#define NI_TRI_STRIPS_FIXLINKS \
NiTriBasedGeom::FixLinks( objects, link_stack, version ); \

#define NI_TRI_STRIPS_DATA_MEMBERS \
ushort numTriangles; \
bool hasPoints; \
vector<vector<ushort > > points; \

#define NI_TRI_STRIPS_DATA_PARENTS TriBasedGeomData \

#define NI_TRI_STRIPS_DATA_CONSTRUCT \

#define NI_TRI_STRIPS_DATA_READ \
TriBasedGeomData::Read( in, link_stack, version ); \
ushort numStrips; \
vector<ushort > stripLengths; \
NifStream( numTriangles, in, version ); \
NifStream( numStrips, in, version ); \
stripLengths.resize(numStrips); \
for (uint i0 = 0; i0 < numStrips; i0++) { \
  NifStream( stripLengths[i0], in, version ); \
}; \
if ( version >= 0x0A010000 ) { \
  NifStream( hasPoints, in, version ); \
}; \
if ( version <= 0x0A000102 ) { \
  points.resize(numStrips); \
  for (uint i1 = 0; i1 < numStrips; i1++) \
    points[i1].resize(stripLengths); \
  for (uint i1 = 0; i1 < numStrips; i1++) { \
    for (uint i2 = 0; i2 < stripLengths; i2++) { \
      NifStream( points[i1][i2], in, version ); \
    }; \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( hasPoints != 0 ) { \
    points.resize(numStrips); \
    for (uint i2 = 0; i2 < numStrips; i2++) \
      points[i2].resize(stripLengths); \
    for (uint i2 = 0; i2 < numStrips; i2++) { \
      for (uint i3 = 0; i3 < stripLengths; i3++) { \
        NifStream( points[i2][i3], in, version ); \
      }; \
    }; \
  }; \
}; \

#define NI_TRI_STRIPS_DATA_WRITE \
TriBasedGeomData::Write( out, link_map, version ); \
ushort numStrips; \
numStrips = ushort(numStrips.size()); \
vector<ushort > stripLengths; \
stripLengths.resize(stripLengths.size()); \
for (uint i0 = 0; i < stripLengths.size(); i++) \
  stripLengths[i0] = ushort(stripLengths[i0].size()); \
NifStream( numTriangles, out, version ); \
NifStream( numStrips, out, version ); \
for (uint i0 = 0; i0 < numStrips; i0++) { \
  NifStream( stripLengths[i0], out, version ); \
}; \
if ( version >= 0x0A010000 ) { \
  NifStream( hasPoints, out, version ); \
}; \
if ( version <= 0x0A000102 ) { \
  for (uint i1 = 0; i1 < numStrips; i1++) { \
    for (uint i2 = 0; i2 < stripLengths; i2++) { \
      NifStream( points[i1][i2], out, version ); \
    }; \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( hasPoints != 0 ) { \
    for (uint i2 = 0; i2 < numStrips; i2++) { \
      for (uint i3 = 0; i3 < stripLengths; i3++) { \
        NifStream( points[i2][i3], out, version ); \
      }; \
    }; \
  }; \
}; \

#define NI_TRI_STRIPS_DATA_STRING \
stringstream out; \
out << TriBasedGeomData::asString(); \
out << "Num Triangles:  " << numTriangles << endl; \
out << "Num Strips:  " << numStrips << endl; \
for (uint i0 = 0; i0 < numStrips; i0++) { \
  out << "  Strip Lengths[" << i0 << "]:  " << stripLengths[i0] << endl; \
}; \
out << "Has Points:  " << hasPoints << endl; \
for (uint i0 = 0; i0 < numStrips; i0++) { \
  for (uint i1 = 0; i1 < stripLengths; i1++) { \
    out << "    Points[" << i0 << "][" << i1 << "]:  " << points[i0][i1] << endl; \
  }; \
}; \
if ( hasPoints != 0 ) { \
  for (uint i1 = 0; i1 < numStrips; i1++) { \
    for (uint i2 = 0; i2 < stripLengths; i2++) { \
      out << "      Points[" << i1 << "][" << i2 << "]:  " << points[i1][i2] << endl; \
    }; \
  }; \
}; \
return out.str(); \

#define NI_TRI_STRIPS_DATA_FIXLINKS \
TriBasedGeomData::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numStrips; i0++) { \
}; \
if ( version >= 0x0A010000 ) { \
}; \
if ( version <= 0x0A000102 ) { \
  for (uint i1 = 0; i1 < numStrips; i1++) { \
    for (uint i2 = 0; i2 < stripLengths; i2++) { \
    }; \
  }; \
}; \
if ( version >= 0x0A010000 ) { \
  if ( hasPoints != 0 ) { \
    for (uint i2 = 0; i2 < numStrips; i2++) { \
      for (uint i3 = 0; i3 < stripLengths; i3++) { \
      }; \
    }; \
  }; \
}; \

#define NI_U_V_CONTROLLER_MEMBERS \
ushort unknownShort; \
Ref<NiUVData > data; \

#define NI_U_V_CONTROLLER_PARENTS NiTimeController \

#define NI_U_V_CONTROLLER_CONSTRUCT \

#define NI_U_V_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
NifStream( unknownShort, in, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_U_V_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( unknownShort, out, version ); \
NifStream( link_map[data], out, version ); \

#define NI_U_V_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Unknown Short:  " << unknownShort << endl; \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_U_V_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_U_V_DATA_MEMBERS \
vector<VectorKeyArray<float > > uvGroups; \

#define NI_U_V_DATA_PARENTS NiObject \

#define NI_U_V_DATA_CONSTRUCT \

#define NI_U_V_DATA_READ \
NiObject::Read( in, link_stack, version ); \
uvGroups.resize(4); \
for (uint i0 = 0; i0 < 4; i0++) { \
  NifStream( uvGroups[i0].numKeys, in, version ); \
  if ( uvGroups[i0].numKeys != 0 ) { \
    NifStream( uvGroups[i0].keyType, in, version ); \
  }; \
  uvGroups[i0].keys.resize(uvGroups[i0].numKeys); \
  for (uint i1 = 0; i1 < uvGroups[i0].numKeys; i1++) { \
    NifStream( uvGroups[i0].keys[i1], in, version ); \
  }; \
}; \

#define NI_U_V_DATA_WRITE \
NiObject::Write( out, link_map, version ); \
for (uint i0 = 0; i0 < 4; i0++) { \
  NifStream( uvGroups[i0].numKeys, out, version ); \
  if ( uvGroups[i0].numKeys != 0 ) { \
    NifStream( uvGroups[i0].keyType, out, version ); \
  }; \
  for (uint i1 = 0; i1 < uvGroups[i0].numKeys; i1++) { \
    NifStream( uvGroups[i0].keys[i1], out, version ); \
  }; \
}; \

#define NI_U_V_DATA_STRING \
stringstream out; \
out << NiObject::asString(); \
for (uint i0 = 0; i0 < 4; i0++) { \
  out << "  Num Keys:  " << uvGroups[i0].numKeys << endl; \
  if ( uvGroups[i0].numKeys != 0 ) { \
    out << "    Key Type:  " << uvGroups[i0].keyType << endl; \
  }; \
  for (uint i1 = 0; i1 < uvGroups[i0].numKeys; i1++) { \
    out << "    Keys[" << i1 << "]:  " << uvGroups[i0].keys[i1] << endl; \
  }; \
}; \
return out.str(); \

#define NI_U_V_DATA_FIXLINKS \
NiObject::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < 4; i0++) { \
  if ( uvGroups[i0].numKeys != 0 ) { \
  }; \
  for (uint i1 = 0; i1 < uvGroups[i0].numKeys; i1++) { \
  }; \
}; \

#define NI_VECTOR_EXTRA_DATA_MEMBERS \
Vector3 vectorData; \
float unknownFloat; \

#define NI_VECTOR_EXTRA_DATA_PARENTS NiExtraData \

#define NI_VECTOR_EXTRA_DATA_CONSTRUCT \

#define NI_VECTOR_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
NifStream( vectorData, in, version ); \
NifStream( unknownFloat, in, version ); \

#define NI_VECTOR_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
NifStream( vectorData, out, version ); \
NifStream( unknownFloat, out, version ); \

#define NI_VECTOR_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Vector Data:  " << vectorData << endl; \
out << "Unknown Float:  " << unknownFloat << endl; \
return out.str(); \

#define NI_VECTOR_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \

#define NI_VERTEX_COLOR_PROPERTY_MEMBERS \
Flags flags; \
VertMode vertexMode; \
LightMode lightingMode; \

#define NI_VERTEX_COLOR_PROPERTY_PARENTS NiProperty \

#define NI_VERTEX_COLOR_PROPERTY_CONSTRUCT \

#define NI_VERTEX_COLOR_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \
NifStream( vertexMode, in, version ); \
NifStream( lightingMode, in, version ); \

#define NI_VERTEX_COLOR_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \
NifStream( vertexMode, out, version ); \
NifStream( lightingMode, out, version ); \

#define NI_VERTEX_COLOR_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Vertex Mode:  " << vertexMode << endl; \
out << "Lighting Mode:  " << lightingMode << endl; \
return out.str(); \

#define NI_VERTEX_COLOR_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_VERT_WEIGHTS_EXTRA_DATA_MEMBERS \
uint numBytes; \
vector<float > weight; \

#define NI_VERT_WEIGHTS_EXTRA_DATA_PARENTS NiExtraData \

#define NI_VERT_WEIGHTS_EXTRA_DATA_CONSTRUCT \

#define NI_VERT_WEIGHTS_EXTRA_DATA_READ \
NiExtraData::Read( in, link_stack, version ); \
ushort numVertices; \
NifStream( numBytes, in, version ); \
NifStream( numVertices, in, version ); \
weight.resize(numVertices); \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  NifStream( weight[i0], in, version ); \
}; \

#define NI_VERT_WEIGHTS_EXTRA_DATA_WRITE \
NiExtraData::Write( out, link_map, version ); \
ushort numVertices; \
numVertices = ushort(numVertices.size()); \
NifStream( numBytes, out, version ); \
NifStream( numVertices, out, version ); \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  NifStream( weight[i0], out, version ); \
}; \

#define NI_VERT_WEIGHTS_EXTRA_DATA_STRING \
stringstream out; \
out << NiExtraData::asString(); \
out << "Num Bytes:  " << numBytes << endl; \
out << "Num Vertices:  " << numVertices << endl; \
for (uint i0 = 0; i0 < numVertices; i0++) { \
  out << "  Weight[" << i0 << "]:  " << weight[i0] << endl; \
}; \
return out.str(); \

#define NI_VERT_WEIGHTS_EXTRA_DATA_FIXLINKS \
NiExtraData::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < numVertices; i0++) { \
}; \

#define NI_VIS_CONTROLLER_MEMBERS \
Ref<NiVisData > data; \

#define NI_VIS_CONTROLLER_PARENTS NiTimeController \

#define NI_VIS_CONTROLLER_CONSTRUCT \

#define NI_VIS_CONTROLLER_READ \
uint block_num; \
NiTimeController::Read( in, link_stack, version ); \
NifStream( block_num, in, version ); \
link_stack.push_back( block_num ); \

#define NI_VIS_CONTROLLER_WRITE \
NiTimeController::Write( out, link_map, version ); \
NifStream( link_map[data], out, version ); \

#define NI_VIS_CONTROLLER_STRING \
stringstream out; \
out << NiTimeController::asString(); \
out << "Data:  " << data << endl; \
return out.str(); \

#define NI_VIS_CONTROLLER_FIXLINKS \
NiTimeController::FixLinks( objects, link_stack, version ); \
data = blocks[link_stack.front()]; \
link_stack.pop_front(); \

#define NI_VIS_DATA_MEMBERS \
KeyArray<byte > data; \

#define NI_VIS_DATA_PARENTS AKeyedData \

#define NI_VIS_DATA_CONSTRUCT \

#define NI_VIS_DATA_READ \
AKeyedData::Read( in, link_stack, version ); \
uint data_numKeys; \
NifStream( data_numKeys, in, version ); \
data.keys.resize(data_numKeys); \
for (uint i0 = 0; i0 < data_numKeys; i0++) { \
  NifStream( data.keys[i0], in, version ); \
}; \

#define NI_VIS_DATA_WRITE \
AKeyedData::Write( out, link_map, version ); \
uint data_numKeys; \
data_numKeys = uint(data.numKeys.size()); \
NifStream( data_numKeys, out, version ); \
for (uint i0 = 0; i0 < data_numKeys; i0++) { \
  NifStream( data.keys[i0], out, version ); \
}; \

#define NI_VIS_DATA_STRING \
stringstream out; \
out << AKeyedData::asString(); \
out << "Num Keys:  " << data_numKeys << endl; \
for (uint i0 = 0; i0 < data_numKeys; i0++) { \
  out << "  Keys[" << i0 << "]:  " << data.keys[i0] << endl; \
}; \
return out.str(); \

#define NI_VIS_DATA_FIXLINKS \
AKeyedData::FixLinks( objects, link_stack, version ); \
for (uint i0 = 0; i0 < data_numKeys; i0++) { \
}; \

#define NI_WIREFRAME_PROPERTY_MEMBERS \
Flags flags; \

#define NI_WIREFRAME_PROPERTY_PARENTS NiProperty \

#define NI_WIREFRAME_PROPERTY_CONSTRUCT \

#define NI_WIREFRAME_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \

#define NI_WIREFRAME_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \

#define NI_WIREFRAME_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
return out.str(); \

#define NI_WIREFRAME_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \

#define NI_Z_BUFFER_PROPERTY_MEMBERS \
Flags flags; \
uint function; \

#define NI_Z_BUFFER_PROPERTY_PARENTS NiProperty \

#define NI_Z_BUFFER_PROPERTY_CONSTRUCT \
 : flags(3), function(3) \

#define NI_Z_BUFFER_PROPERTY_READ \
NiProperty::Read( in, link_stack, version ); \
NifStream( flags, in, version ); \
if ( version >= 0x0401000C ) { \
  NifStream( function, in, version ); \
}; \

#define NI_Z_BUFFER_PROPERTY_WRITE \
NiProperty::Write( out, link_map, version ); \
NifStream( flags, out, version ); \
if ( version >= 0x0401000C ) { \
  NifStream( function, out, version ); \
}; \

#define NI_Z_BUFFER_PROPERTY_STRING \
stringstream out; \
out << NiProperty::asString(); \
out << "Flags:  " << flags << endl; \
out << "Function:  " << function << endl; \
return out.str(); \

#define NI_Z_BUFFER_PROPERTY_FIXLINKS \
NiProperty::FixLinks( objects, link_stack, version ); \
if ( version >= 0x0401000C ) { \
}; \

#define ROOT_COLLISION_NODE_MEMBERS \

#define ROOT_COLLISION_NODE_PARENTS NiNode \

#define ROOT_COLLISION_NODE_CONSTRUCT \

#define ROOT_COLLISION_NODE_READ \
NiNode::Read( in, link_stack, version ); \

#define ROOT_COLLISION_NODE_WRITE \
NiNode::Write( out, link_map, version ); \

#define ROOT_COLLISION_NODE_STRING \
stringstream out; \
out << NiNode::asString(); \
return out.str(); \

#define ROOT_COLLISION_NODE_FIXLINKS \
NiNode::FixLinks( objects, link_stack, version ); \

#endif