#pragma once

#include "Archive.hpp"

#include <experimental/>

namespace FishEngine
{
    	class CollectObjectArchive : public OutputArchive
	{
	public:
		void AddObject(Object* t) override
		{
			auto it = m_Objects.find(t);
			if (it == m_Objects.end())
			{
				m_Objects.insert(t);
				t->Serialize(*this);
			}
		}

	protected:
		std::set<Object*> m_Objects;
	};


    class CloneArchive
    {
    public:
        Object* Clone(Object* obj)
        {

        }
        CollectObjectArchive archive1;
    };
}
