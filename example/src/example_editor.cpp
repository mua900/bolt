#include <app/application.hpp>

struct State
{

};

bool initialize(void *user, bolt::Application *app)
{
    return true;
}

void draw(void *user, bolt::Application *app)
{
}

bool handleEvent(SDL_Event event, void* user, bolt::Application *app)
{
	return false;
}

void handleInput(void *user, bolt::Application *app)
{

}

void updateFunc(void *user, bolt::Application *app)
{

}

void fixedUpdate(void *user, bolt::Application *app)
{

}

int main()
{
	bolt::Application app;

	State state = {};

	bolt::UpdateState update = {};

	update.fixedUpdate = fixedUpdate;
	update.update = updateFunc;

	app.user.userdata = &state;
	app.user.init = initialize;
	app.user.draw = draw;
	app.user.event = handleEvent;
	app.user.input = handleInput;
	app.user.update_state = &update;

	bolt::InitConfiguration conf = bolt::get_default_init_configuration();
	conf.name = "Editor Example";

	if (!app.initialize(conf))
	{
		return 1;
	}

	while (!app.quit)
	{
		app.handle_events();
		app.update();
		app.draw();
	}

	app.cleanup();

	return 0;
}
