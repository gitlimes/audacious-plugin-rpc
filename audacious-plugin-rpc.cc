#include <iostream>
#include <string.h>

#include <libaudcore/drct.h>
#include <libaudcore/i18n.h>
#include <libaudcore/plugin.h>
#include <libaudcore/hook.h>
#include <libaudcore/audstrings.h>
#include <libaudcore/tuple.h>
#include <libaudcore/preferences.h>
#include <libaudcore/runtime.h>

#include <discord_rpc.h>

#define EXPORT __attribute__((visibility("default")))
#define APPLICATION_ID "1236402067044831273"

static const char *SETTING_EXTRA_TEXT = "extra_text";

class RPCPlugin : public GeneralPlugin {

public:
    static const char about[];
    static const PreferencesWidget widgets[];
    static const PluginPreferences prefs;

    static constexpr PluginInfo info = {
        N_("Discord RPC"),
        "audacious-plugin-rpc",
        about,
        &prefs
    };

    constexpr RPCPlugin() : GeneralPlugin (info, false) {}

    bool init();
    void cleanup();
};

EXPORT RPCPlugin aud_plugin_instance;

DiscordEventHandlers handlers;
DiscordRichPresence presence;
std::string fullTitle;
std::string playingStatus;
std::string stateText;

void init_discord() {
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

void update_presence() {
    Discord_UpdatePresence(&presence);
}

void init_presence() {
    memset(&presence, 0, sizeof(presence));
    presence.state = "Initialized";
    presence.details = "Waiting...";
    presence.largeImageKey = "logo";
    presence.smallImageKey = "stop";
    update_presence();
}

void cleanup_discord() {
    Discord_ClearPresence();
    Discord_Shutdown();
}

void title_changed() {
    if (!aud_drct_get_ready()) {
        return;
    }

    if (aud_drct_get_playing()) {
        bool paused = aud_drct_get_paused();
        Tuple tuple = aud_drct_get_tuple();
        String artist = tuple.get_str(Tuple::Artist);
        std::string title(tuple.get_str(Tuple::Title));
        String album = tuple.get_str(Tuple::Album);

        if (artist) {
            fullTitle = (title + " - " + std::string(artist)).substr(0, 127);
        } else {
            fullTitle = title.substr(0, 127);
        }

        playingStatus = paused ? "Paused" : "Listening";

        if (album) {
            stateText = std::string(album).substr(0, 127);
        } else {
            stateText = playingStatus;
        }

        presence.details = fullTitle.c_str();
        presence.smallImageKey = paused ? "pause" : "play";
    } else {
        stateText = "Stopped";
        presence.state = "Stopped";
        presence.smallImageKey = "stop";
    }

    std::string extraText(aud_get_str("audacious-plugin-rpc", SETTING_EXTRA_TEXT));
    stateText = (stateText + " " + extraText).substr(0, 127);

    presence.state = stateText.c_str();
    update_presence();
}

void update_title_presence(void*, void*) {
    title_changed();
}

void open_github() {
   system("xdg-open https://github.com/gitlimes/audacious-plugin-rpc");
}

bool RPCPlugin::init() {
    init_discord();
    init_presence();
    hook_associate("playback ready", update_title_presence, nullptr);
    hook_associate("playback end", update_title_presence, nullptr);
    hook_associate("playback stop", update_title_presence, nullptr);
    hook_associate("playback pause", update_title_presence, nullptr);
    hook_associate("playback unpause", update_title_presence, nullptr);
    hook_associate("title change", update_title_presence, nullptr);
    return true;
}

void RPCPlugin::cleanup() {
    hook_dissociate("playback ready", update_title_presence);
    hook_dissociate("playback end", update_title_presence);
    hook_dissociate("playback stop", update_title_presence);
    hook_dissociate("playback pause", update_title_presence);
    hook_dissociate("playback unpause", update_title_presence);
    hook_dissociate("title change", update_title_presence);
    cleanup_discord();
}

const char RPCPlugin::about[] = N_("Discord RPC music status plugin\n\nWritten by: Derzsi Daniel <daniel@tohka.us>");

const PreferencesWidget RPCPlugin::widgets[] =
{
  WidgetEntry(
      N_("Extra status text:"),
      WidgetString("audacious-plugin-rpc", SETTING_EXTRA_TEXT, title_changed)
  ),
  WidgetButton(
      N_("Fork on GitHub"),
      {open_github}
  )
};

const PluginPreferences RPCPlugin::prefs = {{ widgets }};
