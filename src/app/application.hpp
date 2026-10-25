#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ui.hpp"
#include "asset.hpp"
#include "input.hpp"
#include "draw.hpp"
#include "time.hpp"
#include "camera.hpp"
#include "action_set.hpp"
#include "editor.hpp"
#include "shape.hpp"

#include "util/common.hpp"
#include "util/template.hpp"
#include "util/math_util.hpp"

namespace bolt
{

    class Application;

    struct Event_Timeout {
        s64 event = 0;
        bool active = false;
    };

    // userdata is included in the Application but maybe this is convenient
    typedef bool (*InitCallback)(void *userdata, Application* app);
    typedef bool (*EventCallback)(SDL_Event event, void *userdata, Application* app);
    typedef void (*InputCallback)(void *userdata, Application* app);
    typedef void (*DrawCallback)(void *userdata, Application* app);
    typedef void (*BeforeCleanupCallback)(void *userdata, Application* app);
    typedef void (*AfterCleanupCallback)(void *userdata, Application* app);

    typedef void (*UpdateFunction)(void *userdata, Application* app);
    typedef void (*FixedUpdateFunction)(void *userdata, Application* app);

    struct UpdateState
    {
        UpdateFunction update = nullptr;
        FixedUpdateFunction fixedUpdate = nullptr;
        s64 ticks = 0;
        double elapsed = 0;
        double timeScale = 0;
    	int updateRate = 0;

        double calculateTimeStep() { return 1.0 / updateRate; }
    };

    // these callbacks won't be called if they are null
    // user code can set them however it wants
    struct UserData {
    	// passed to every single user function
    	void *userdata = nullptr;

    	UpdateState* update_state = {};

    	// called once after SDL and everything is initialized
    	InitCallback init = nullptr;
    	// called with an event before application tries to process it itself
    	// use this to react to one time events
    	// if this returns true, the application will consider that event consumed and will not process it
    	EventCallback event = nullptr;
    	// called after all events are processed.
    	// you can read a snapshot of the input state as it is recorded
    	InputCallback input = nullptr;
    	// called with user data in draw()
    	DrawCallback draw = nullptr;
    	// called before SDL and everything else is quitted
    	BeforeCleanupCallback before_cleanup = nullptr;
    	// called after SDL and everything else is quitted
    	AfterCleanupCallback after_cleanup = nullptr;
    };

    struct InitConfiguration {
    	int window_width = 1440;
    	int window_height = 810;

        int target_fps = 60;

    	SDL_InitFlags flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
        bool init_ttf = true;
        bool init_mixer = true;
        bool init_net = false;

    	// Default Name
    	const char* name = nullptr;

        RenderInitConfig render = {};
        AssetInitConfig asset = {};
    };

    // call this to get default values
    InitConfiguration get_default_init_configuration();

    typedef u32 UiStateId;

    class Application {
    public:
        Window window = {};
        RenderContext render = {};
        AudioPlayer audio_player = {};
        InputState input = {};
        AssetCatalog catalog = {};

        EditorState editor = {};

        ActionSet actions = {};

        DArray<UiState> uiStates = {};
        UiStateId active_ui_state = 0;

        TimeInfo timeInfo = {};

        ShapeData shape_data = {};

        DArray<Event_Timeout> events = {};

        DArray<Camera> cameras = {};
        Camera active_camera = {};

        AssetId font = {};
        AssetId editor_font = {};

        int target_fps = 0;

        bool quit = false;
        bool doing_text_input = false;  // don't mess with this.

    	// fill this out
    	UserData user = {};

        bool initialize(InitConfiguration conf);

        void handle_events();
        void update();
        void draw();
        void wait_frame_time();

		void loop_step() {
            handle_events();
            update();
            draw();
            wait_frame_time();
        }

        void cleanup();

        double get_target_frame_time()
        {
            double time = 1.0 / (double)target_fps;
            return time;
        }

        s64 get_target_frame_time_ms()
        {
            return get_target_frame_time() * MILLISECONDS_PER_SECOND;
        }

        void set_camera(CameraId cam);

        void set_event_active(int event_index, double timeout_seconds);
        void set_event_deactive(int event_index);

        bool is_minimized() const;
        bool is_maximized() const;
        bool is_fullscreen() const;
        bolt::vec2 get_window_size() const;
    private:
        bool init_render(bool enableDebug);

    	bool load_assets();
        bool reload_assets(); // @todo not implemented

        UiState* get_active_ui();

    	void user_update();

        void timeout();
        void update_ui_state(bolt::vec2 window_size);
        void update_ui_pos();

        void draw_ui_state(UiState& state);

        bool on_mouse_down();
        void on_mouse_up(int button);
        void on_mouse_move();
        void mouse_move_ui(UiState& ui);

    	bool mouse_input_common();

        void update_keyboard_state();

        bool keyboard_input_down(KeyboardEvent keyboard);

        void text_input_start();
        void text_input_stop();
        void toggle_text_input();

        bool read_asset_catalog(String_Builder& path, AssetInitConfig asset_config);

        void render_rectangle_outline(bolt::Rectangle rect, bolt::Color color, bool center = true) const;
        void render_rectangle(bolt::Rectangle rect, bolt::Color color, bool center = true) const;

        Icon create_icon(AssetId image, bolt::Color background);

        void render_slider(bolt::Rectangle area, bolt::vec2 knob_scale, float value, bolt::Color slider_color, bolt::Color knob_color, const Text& text) const;
        void render_text_field(Text_Field& text_field) const;
        void render_text_editor(TextEditor& editor) const;
        void render_dropdown(const Drop_Down_List& list) const;
        void render_discrete_slider(const DiscreteSlider& slider) const;
        void render_panel(const Panel& panel) const;
        void render_value_panel(const UiState& ui, const ValuePanel& panel) const;
        void render_button_group(const ButtonGroup& group) const;
    };

    void get_base_path(String_Builder& builder);
    void get_pref_path(String_Builder& builder, const char *org, const char *app);
} // namespace

#endif // APPLICATION_HPP
