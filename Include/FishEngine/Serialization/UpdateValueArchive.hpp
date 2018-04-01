#pragma once

#include <FishEngine/Serialization/Archive.hpp>
#include <set>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace FishEngine
{
	class UpdateValueArchive : public InputArchive
	{
	public:

		UpdateValueArchive() = default;

		// It does not matter if we do not know the changed value is an Object or not,
		// since we can get type info from propertyPath
		// like:
		//	- target: {fileID: 100008, guid: 217b612561ad045a8ac7a16923956985, type: 3}
		//	  propertyPath: m_Name
		//	  value:
		//	  objectReference: {fileID: 0}
		//	- target: {fileID: 3300008, guid: 217b612561ad045a8ac7a16923956985, type: 3}
		//	  propertyPath: m_Mesh
		//	  value:
		//	  objectReference: {fileID: 0}
		void UpdateValue(Object* target, const std::string& propertyPath, const std::string& value, Object* obj)
		{
			m_PropertyPath = propertyPath;
			m_Value = value;
			m_ObjectValue = obj;
			target->Deserialize(*this);
		}

		void UpdateValue(Object* target, const std::string& propertyPath, const std::string& value)
		{
			UpdateValue(target, propertyPath, value, nullptr);
		}

		void UpdateValue(Object* target, const std::string& propertyPath, Object* obj)
		{
			UpdateValue(target, propertyPath, "", obj);
		}


	protected:
		void Deserialize(short & t) override 				{ t = static_cast<short>(std::stoi(m_Value)); }
		void Deserialize(unsigned short & t) override 		{ t = static_cast<unsigned short>(std::stoi(m_Value)); }
		void Deserialize(int & t) override 					{ t = std::stoi(m_Value); }
		void Deserialize(unsigned int & t) override 		{ t = static_cast<unsigned int>(std::stoul(m_Value)); }
		void Deserialize(long & t) override 				{ t = std::stol(m_Value); }
		void Deserialize(unsigned long & t) override 		{ t = std::stoul(m_Value); }
		void Deserialize(long long & t) override 			{ t = std::stoll(m_Value); }
		void Deserialize(unsigned long long & t) override 	{ t = std::stoull(m_Value); }
		void Deserialize(float & t) override 				{ t = std::stof(m_Value); }
		void Deserialize(double & t) override 				{ t = std::stod(m_Value); }
		void Deserialize(bool & t) override 				{ t = (std::stoi(m_Value) == 1); }
		void Deserialize(std::string & t) override 			{ t = m_Value; }

		Object* DeserializeObject() override
		{
//			assert(m_ValueIsObject);
			return m_ObjectValue;
		}

		// Map
		bool MapKey(const char* name) override
		{
			// do not Skip next value if path matches
			m_CurrentPath.push_back(name);
			std::string path = boost::algorithm::join(m_CurrentPath, ".");
			if (path.size() <= m_PropertyPath.size())
			{
				if (boost::starts_with(m_PropertyPath, path))
				{
					return true;
				}
			}
			return false;
		}

		void AfterValue() override
		{
			m_CurrentPath.pop_back();
		}

		// Sequence
		int BeginSequence() override { return 0; }
//		virtual void BeginSequenceItem() override {}
//		virtual void AfterSequenceItem() override {}
//		virtual void EndSequence() override {}

	protected:
		std::string m_PropertyPath;
//		bool m_ValueIsObject = false;
		std::string m_Value;
		Object* m_ObjectValue = nullptr;

		std::vector<std::string> m_CurrentPath;
	};
}