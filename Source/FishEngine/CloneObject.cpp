#include <FishEngine/Serialization/CloneArchive.hpp>
#include <FishEngine/CreateObject.hpp>

namespace FishEngine
{
//	template<class K, class V>
//	bool MapHasKey(const std::map<K, V>& dict, const K& key)
//	{
//		auto pos = dict.find(key);
//
//	};
//
	std::vector<Object*> CloneObjects(std::vector<Object*> objects, std::map<Object*, Object*>& memo)
	{
		// step 1: collect all objects
		CollectObjectsArchive archive;
		for (auto obj : objects)
			archive.Collect(obj);


		int size = archive.m_Objects.size();
		for (auto o : archive.m_Objects)
		{
			auto pos = memo.find(o);
			if (pos != memo.end())
				continue;

			int classID = o->GetClassID();
			if (classID == GameObject::ClassID)
			{
//				printf("Clone GameObject %s %d\n", o->GetName().c_str(), o->GetInstanceID());
			}
//			Object* cloned = nullptr;
//			if (classID != Prefab::ClassID)
//				cloned = CreateEmptyObjectByClassID(classID);
			Object* cloned = CreateEmptyObjectByClassID(classID);

			if (cloned == nullptr)
				memo[o] = o;	// mesh, material...
			else
			{
				memo[o] = cloned;
//				printf("Cloned[%s] %d <- %d\n", cloned->GetClassName(), cloned->GetInstanceID(), o->GetInstanceID());
			}
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
			if (cloned == nullptr) // prefab instance
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
			if (cloned == nullptr) // prefab instance
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