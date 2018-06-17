#include <FishEngine/Serialization/CloneArchive.hpp>
#include <FishEngine/CreateObject.hpp>

#include <FishEngine/Asset.hpp>

namespace FishEngine
{
	std::vector<Object*> CloneObjects(std::vector<Object*> objects, std::map<Object*, Object*>& memo)
	{
		// step 1: collect all objects
		CollectObjectsArchive archive;
		for (auto obj : objects)
			archive.Collect(obj);

		// step 2: create empty objects
		for (auto o : archive.m_Objects)
		{
			auto pos = memo.find(o);
			if (pos != memo.end())
				continue;

			int classID = o->GetClassID();
			Object* cloned = nullptr;
			if (classID == Script::ClassID)
			{
				auto script = o->As<Script>();
				cloned = script->Clone();
			}
			else if (IsAsset(o) || classID == 1001)
			{
				cloned = o;		// do not clone asset(mesh, material...)
			}
			else
			{
				cloned = CreateEmptyObjectByClassID(classID);
			}
			
			assert(cloned != nullptr);
			memo[o] = cloned;
		}

		CloneOutputArchive out;

		// step 3: clone GameObjects first
		for (auto o : archive.m_Objects)
		{
			if (!o->Is<GameObject>())
				continue;
			auto cloned = memo[o];
			if (cloned == o)		// mesh, material...
				continue;
			if (cloned == nullptr) // prefab instance
				continue;
			o->Serialize(out);
			CloneInputArchive in(out, memo);
			cloned->Deserialize(in);
			out.AssertEmpty();	// make sure all serialized properties are deserialized
		}

		// step 4: copy values to cloned object
		for (auto o : archive.m_Objects)
		{
			if (o->Is<GameObject>())
				continue;
			auto cloned = memo[o];
			if (cloned == o)
				continue;	// mesh, material, prefab...
			if (o->Is<Script>())
				continue;
			if (cloned == nullptr) // prefab instance
				continue;
			o->Serialize(out);
			CloneInputArchive in(out, memo);
			cloned->Deserialize(in);
			out.AssertEmpty();	// make sure all serialized properties are deserialized
		}

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
