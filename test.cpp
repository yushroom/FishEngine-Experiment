#include <FishEngine/FishEngine.hpp>

#include <boost/python.hpp>
#include <iostream>
#include <FishEngine/GameApp.hpp>

using namespace boost::python;
// namespace bp = boost::python;

#include <FishEditor/FishEditorInternal.hpp>

#include <cstdlib>  // setenv

class TestApp : public FishEngine::GameApp
{
public:
	virtual void Init() override
	{
	}
	
	virtual void Update() override
	{
	}
};


int main(int argc, char** argv)
{
    try {
		FishEditor::Init();
		TestApp app;
		app.SetWindowSize(600, 400);
		app.Run();
    } catch (error_already_set& e) {
        std::cerr << ">>> Error! Uncaught exception:\n";
        PyErr_PrintEx(0);
        return 1;
    }
    return 0;
}
