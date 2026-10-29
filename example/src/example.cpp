#include <app/application.hpp>
#include <app/animation.hpp>

#include <random>
#include <time.h>

using namespace bolt;

// @todo a more well organized example
// this is more a test program for the developer

#define NUM_RANDOM 8

struct State
{
	float number = 0;
	DArray<MeshReference> references = {};
	MeshId mesh = {};
	Texture texture = {};
	DrawGroupId group_solid = {};
	DrawGroupId group_tex = {};
	DrawGroupId group_lines = {};
	DrawGroupId group_text = {};

	Text text = {};

	SpriteAnimation animation = {};

	GraphicsPipelineId graphics = {};

	vec2 position[NUM_RANDOM];
};

bool initialize(void *userdata, Application *app)
{
	srand(time(0));

	State* state = (State*) userdata;
	app->render.clear_color = Colorf(0.5, 0.6, 0.8);
	//app->render.light_clear_color = Colorf(0.6, 0.4, 0.3);

	for (int i = 0; i < NUM_RANDOM; i++)
	{
		int range = 600;
		float x = rand() % range;
		float y = rand() % range;

		x = x - range / 2;
		y = y - range / 2;

		state->position[i] = vec2(x, y);
	}

	AssetId texId = get_asset(String("TestImg"), app->catalog);
	AssetId fontId = get_asset(String("TestFont"), app->catalog);

	if (!(texId.is_valid() && fontId.is_valid()))
	{
		return false;
	}

	app->font = fontId;
	Text text = create_text(app->render, String("Hello"), app->catalog.get_font(app->font), bolt::Color(0x33, 0x44, 0x33));

	Texture texture = app->catalog.get_image(texId);

	ASSERT(texture.is_valid());

	bolt::DrawGroupId id = app->render.make_draw_group(app->render.graphics_instance_texture, texture, 1024);
	bolt::DrawGroupId id_solid = app->render.make_draw_group(app->render.graphics_instance, TEXTURE_INVALID, 1024);
	bolt::DrawGroupId id_lines = app->render.make_draw_group(app->render.graphics_lines, TEXTURE_INVALID, 1024);
	bolt::DrawGroupId id_text = app->render.make_draw_group(app->render.graphics_instance_texture, text.texture, 16);
	if (!(id.is_valid() && id_solid.is_valid() && id_lines.is_valid() && id_text.is_valid()))
	{
		return false;
	}

	TransferData triangle, quad;

	float scale = 100;

	// define persistent custom geometry

	Vertex vertices[3] = {
		{ 1,     0,     0, 0, 1, 0, 0, 1},
		{ -0.5,  0.866, 0, 0, 1, 0, 0, 1},
		{ -0.5, -0.866, 0, 0, 1, 0, 0, 1},
	};
	u16 indices[3] = {
		0, 1, 2
	};

	for (int i = 0; i < 3; i++) {
		vertices[i].x *= scale;
		vertices[i].y *= scale;
	}

	Vertex qv[4] = {
		{ 0,     0, 0, 1, 1, 1, 1, 1 },
		{ 0.5,   0, 1, 1, 1, 1, 1, 1 },
		{ 0.5, 0.5, 1, 0, 1, 1, 1, 1 },
		{ 0,   0.5, 0, 0, 1, 1, 1, 1 },
	};
	u16 qi[6] = {
		0, 1, 2,
		0, 2, 3
	};

	for (int i = 0; i < 4; i++) {
		qv[i].x *= scale;
		qv[i].y *= scale;
	}

	MeshData meshData[2] = {};

	meshData[0].vertices.add_array(vertices, 3);
	meshData[0].indices.add_array(indices, 3);

	meshData[1].vertices.add_array(qv, 4);
	meshData[1].indices.add_array(qi, 6);

	TransferData memory = add_to_transfer_buffer(app->render, app->render.instance_transfer_buffer, meshData, 2);

	int vertex_buffer = app->render.allocate_gpu_buffer(GPUBufferVertex, 1024, false);
	int index_buffer = app->render.allocate_gpu_buffer(GPUBufferIndex, 1024, false);

	if (vertex_buffer == -1 || index_buffer == -1)
	{
		return false;
	}

	app->render.set_vertex_buffer(vertex_buffer);
	app->render.set_index_buffer(index_buffer);

	if (!app->render.get_command_buffer())
	{
		return false;
	}

	if (!app->render.start_copy_pass())
	{
		log_error("Couldn't start copy pass");
		app->render.submit_command_buffer();
		return false;
	}

	MeshId mesh = {};
	{
		float scale = 200;

		// define persistent custom geometry

		// this will be multiplied by the projection matrix so it should be defined in unit coordinates like the builtin shapes
		VertexInstance vertices[3] = {
			{ 1,     0,     1, 0.5},
			{ -0.5,  0.866, 0, 1},
			{ -0.5, -0.866, 0, 0},
		};
		u16 indices[3] = {
			0, 1, 2
		};

		MeshInstanceData mesh_data = {};
		mesh_data.vertices.add_array(vertices, 3);
		mesh_data.indices.add_array(indices, 3);

		mesh = app->render.make_mesh(mesh_data);
	}

	GPUTexture& gpu_tex = app->render.get_texture(texture);
	float anim_frame_size = 128;
	auto atlas = TextureAtlas(texture, anim_frame_size, anim_frame_size, gpu_tex.width / anim_frame_size, gpu_tex.height / anim_frame_size);
	SpriteAnimation anim = make_sprite_animation(
							atlas,
							4,
							0.2,
							AnimationFlags::AnimationLoop
							);

	PointLight light = {
		100, 100, 0,
		100, 1,
		bolt::colorToHex(Colorf(1,0,0,1))
	};
	PointLight light2 = {
		-100, -100, 0,
		256, 1,
		bolt::colorToHex(Colorf(1,1,0,1))
	};

	bolt::add_point_light(app->render, light);
	bolt::add_point_light(app->render, light2);

	app->render.get_draw_group(id_solid).mesh = mesh;

	app->render.ambient_light = {
		0.8,0.8,0.8,1
	};

	state->references = upload_mesh_data(app->render, memory);
	state->mesh = mesh;
	state->texture = texture;
	state->animation = anim;
	state->group_tex = id;
	state->group_solid = id_solid;
	state->group_lines = id_lines;
	state->group_text = id_text;
	state->text = text;

	app->render.end_copy_pass();
	app->render.submit_command_buffer();

	app->active_camera = init_camera();
	app->render.camera = &app->active_camera;

	return true;
}

void draw(void *userdata, Application *app)
{
	State* state = (State*) userdata;

	MeshDraw draw = {};
	draw.mesh = state->references.get(0);

	bolt::queue_draw_mesh(app->render, draw);

	draw.mesh = state->references.get(1);
	draw.texture = state->texture;
	bolt::queue_draw_mesh(app->render, draw);

	vec2 offset = vec2(0, 0);
	vec2 scale = vec2(1,1);

	vec2 qscale = vec2(100, 100);

	InstanceData q = {};
	q.sourceOffset = pack_unorm16x2(offset);
	q.sourceScale = pack_unorm16x2(scale);

	q.x = 100;
	q.y = 100;
	q.z = 0;
	q.rotation = 0;
	q.scale = bolt::pack_scale(qscale);
	q.color = 0xFFFFFFFF;

	auto p = q;
	p.x += 10;
	p.y += 10;
	p.color = 0xff0000ff;
	p.z = 1;

	bolt::queue_draw_group(app->render, p, state->group_tex);
	bolt::queue_draw_group(app->render, q, state->group_tex);

	q.y += 200;
	bolt::queue_draw_group(app->render, q, state->group_tex);
	q.x += 200;
	q.y += 150;
	bolt::queue_draw_group(app->render, q, state->group_tex);
	q.x += 200;
	q.color = bolt::colorToHex(Colorf(1,0,0));
	q.rotation = CONSTANT_PI * (4.0f / 3.0f);
	bolt::queue_draw_group(app->render, q, state->group_tex);
	q.color = 0xffffffff;

	/*
	for (int i = 0; i < NUM_RANDOM; i++)
	{
		q.x = state->position[i].x;
		q.y = state->position[i].y;
		bolt::queue_draw_group(app->render, q, state->group_tex);
	}
	*/

	bolt::InstanceData solid = bolt::InstanceData {
		0,
		100,
		0,
		0,
		pack_scale(bolt::vec2(128, 128)),
		bolt::colorToHex(bolt::Colorf(0.4, 0.4, 0.4)),
	};
	bolt::InstanceData solid2 = bolt::InstanceData{
		300, 300, 0, 0,
		pack_scale(bolt::vec2(256, 256)),
		bolt::colorToHex(bolt::Colorf(0.8, 0.4, 0.6))
	};
	bolt::queue_draw_group(app->render, solid, state->group_solid);
	bolt::queue_draw_group(app->render, solid2, state->group_solid);

	bolt::InstanceData frame = state->animation.get_frame(vec3(-100, 100, 0.2), 0, vec2(100, 100));
	bolt::queue_draw_group(app->render, frame, state->group_tex);

	// lines
	bolt::InstanceData line = bolt::InstanceData {
		0, 0, 0, 0, bolt::pack_scale(bolt::vec2(200, 200)), bolt::colorToHex(bolt::Colorf(0.5, 0.5, 0.5))
	};
	bolt::queue_draw_group(app->render, line, state->group_lines);

	bolt::InstanceData text = {
		-200, 0, 0.5, 0, bolt::pack_scale(bolt::vec2(128, 128)), bolt::colorToHex(bolt::Colorf(1,1,1)), {}, bolt::pack_unorm16x2(bolt::vec2(1,1))
	};
	bolt::queue_draw_group(app->render, text, state->group_text);
}

bool handleEvent(SDL_Event event, void *userdata, Application* app)
{
	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		switch (event.key.scancode)
		{
			case SDL_SCANCODE_ESCAPE:
			{
				app->quit = true;
				return true;
			}
			case SDL_SCANCODE_E:
			{
				app->active_camera.zoom -= 0.1;
				return true;
			}
			case SDL_SCANCODE_Q:
			{
				app->active_camera.zoom += 0.1;
				return true;
			}
		}
	}
	else if (event.type == SDL_EVENT_MOUSE_WHEEL)
	{
		float zoom = event.wheel.y;
		if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
		{
			zoom = -zoom;
		}

		app->active_camera.zoom += zoom * 0.1f;

		app->active_camera.zoom = bolt::clamp(0.1, 10, app->active_camera.zoom);
	}

	return false;
}

void handleInput(void* userdata, Application* app)
{
	vec2 mouse_pos = app->input.mouse.pos;

	if ((vec2(100, 100) - mouse_pos).magnitude() < 100)
	{
		// app->render.clear_color.r += 0.01;
		// app->render.clear_color.r = fmodf(app->render.clear_color.r, 0.8f);

		app->active_camera.position.x += 0.8 * app->active_camera.zoom;
		app->active_camera.zoom += 0.02;
		if (app->active_camera.zoom > 2)
		{
			app->active_camera.zoom = 1;
		}
	}
}

void updateFunc(void *userdata, Application *app)
{
	State *state = (State*) userdata;

	float dt = app->timeInfo.deltaTimeSeconds;
	state->number = dt;

	state->animation.step(dt);
}

void fixedUpdate(void *userdata, Application *app)
{
	State *state = (State*) userdata;

	float dt = app->user.update_state->calculateTimeStep() * app->user.update_state->timeScale;

	state->number = dt;
}

int main()
{
	bolt::Application app;

	State state = {};

	UpdateState update = {};

	update.fixedUpdate = fixedUpdate;
	update.update = updateFunc;

	app.user.userdata = &state;
	app.user.init = initialize;
	app.user.draw = draw;
	app.user.event = handleEvent;
	app.user.input = handleInput;
	app.user.update_state = &update;

	InitConfiguration conf = bolt::get_default_init_configuration();
	conf.render.gpuDebug = true;
	conf.render.doLights = true;

	if (!app.initialize(conf))
	{
		return 1;
	}

	while (!app.quit)
	{
		app.loop_step();
	}

	app.cleanup();

	return 0;
}
