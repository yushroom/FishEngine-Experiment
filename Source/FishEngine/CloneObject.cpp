#include <FishEngine/Serialization/CloneArchive.hpp>
#include <FishEngine/CreateObject.hpp>

namespace FishEngine
{
	Object* CloneObject(Object* obj, std::map<Object*, Object*>& memo)
	{
		// step 1: collect all objects
		CollectObjectsArchive archive;
		archive.SerializeObject(obj);

		for (auto o : archive.m_Objects)
		{
			int classID = o->GetClassID();
			Object* cloned = CreateEmptyObjectByClassID(classID);
			if (cloned == nullptr)
				memo[o] = o;	// mesh, material...
			else
				memo[o] = cloned;
		}

		CloneOutputArchive out;

		// clone GameObjects first
		for (auto o : archive.m_Objects)
		{
			if (o->GetClassID() != GameObject::ClassID)
				continue;
			auto cloned = memo[o];
			if (cloned == o)		// mesh, material...
				continue;
			o->Serialize(out);
			CloneInputArchive in(out, memo);
			cloned->Deserialize(in);
			out.AssertEmpty();	// make sure all serialized properties are deserialized
		}

		for (auto o : archive.m_Objects)
		{
			if (o->GetClassID() == GameObject::ClassID)
				continue;
			auto cloned = memo[o];
			if (cloned == o)		// mesh, material...
				continue;
			o->Serialize(out);
			CloneInputArchive in(out, memo);
			cloned->Deserialize(in);
			out.AssertEmpty();	// make sure all serialized properties are deserialized
		}

		auto cloned = memo[obj];
		return cloned;
	}
}