#include <FishEngine/Serialization/CloneArchive.hpp>
#include <FishEngine/CreateObject.hpp>

namespace FishEngine
{
	Object* CloneObject(Object* obj, std::map<Object*, Object*>& memo)
	{
		// step 1: collect all objects
		CollectObjectsArchive archive;
		archive.SerializeObject(obj);

//		std::map<Object*, Object*> memo;
		for (auto obj : archive.m_Objects)
		{
			int classID = obj->GetClassID();
			Object* cloned = CreateEmptyObjectByClassID(classID);
			if (cloned == nullptr)
				memo[obj] = obj;	// mesh, material...
			else
				memo[obj] = cloned;
		}

		CloneOutputArchive out;

		for (auto o : archive.m_Objects)
		{
			auto cloned = memo[o];
			if (cloned == o)		// mesh, material...
				continue;
			o->Serialize(out);
			CloneInputArchive in(out, memo);
			cloned->Deserialize(in);
			out.AssertEmpty();
		}

		auto cloned = memo[obj];
		return cloned;
	}
}