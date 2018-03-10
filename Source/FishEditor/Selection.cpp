#include <FishEditor/Selection.hpp>

using namespace FishEngine;
using namespace FishEditor;

boost::signals2::signal<void(void)> Selection::selectionChanged;
Object* Selection::s_activeObject = nullptr;
