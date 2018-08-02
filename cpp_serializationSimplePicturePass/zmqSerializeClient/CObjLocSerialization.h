#pragma once


//more details see
//2. http://uscilab.github.io/cereal/serialization_functions.html

#include  "ObjLoc.h"

namespace cereal 
{
	template<class Archive>
	void save(Archive& ar, const ObjLoc& stObjLoc)
	{
		ar(
			stObjLoc.objClass,
			stObjLoc.score,
			stObjLoc.tlx,
			stObjLoc.tly,
			stObjLoc.brx,
			stObjLoc.bry);
	}

	template<class Archive>
	void load(Archive& ar, ObjLoc& stObjLoc)
	{
		ar(
			stObjLoc.objClass,
			stObjLoc.score,
			stObjLoc.tlx,
			stObjLoc.tly,
			stObjLoc.brx,
			stObjLoc.bry);
	}

}//end of namespace cereal

