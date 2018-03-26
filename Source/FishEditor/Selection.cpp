#include <FishEditor/Selection.hpp>

using namespace FishEngine;
using namespace FishEditor;

boost::signals2::signal<void(void)> Selection::OnSelectionChanged;
Object* Selection::s_ActiveObject = nullptr;
