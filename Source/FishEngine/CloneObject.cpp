#include <FishEngine/Serialization/CloneArchive.hpp>
#include <FishEngine/CreateObject.hpp>

namespace FishEngine
{
	std::vector<Object*> CloneObjects(std::vector<Object*> objects, std::map<Object*, Object*>& memo)
	{
		// step 1: collect all objects
		CollectObjectsArchive archive;
		for (auto obj : objects)
			archive.Collect(obj);

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

//		auto cloned = memo[obj];
		std::vector<Object*> result;
		result.reserve(objects.size());
		for (auto obj : objects)
			result.push_back(memo[obj]);
		return result;
	}

	Object* CloneObject(Object* obj, std::map<Object*, Object*>& memo)
	{
		std::vector<Object*> objects = {obj};
		auto result = CloneObjects(objects, memo);
		return result[0];
	}
}