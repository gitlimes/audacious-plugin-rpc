# audacious-plugin-rpc
A Discord Rich Presence plugin for the Audacious music player!

<img src="https://github.com/gitlimes/audacious-plugin-rpc/assets/49426949/9ef50cfb-89bd-424c-b8db-22d46690a3b5" alt="A screenshot of the Discord activity UI (Playing a game). On the left, the Audacious logo, with a play icon in the bottom right. On the right, text spanning across three rows reads: 'Audacious', 'K.K. Stroll - Shiho Fujii', 'Animal Crossing: New Horizons Original Soundtrack 2'" title="A screenshot of the Discord activity UI (Playing a game). On the left, the Audacious logo, with a play icon in the bottom right. On the right, text spanning across three rows reads: 'Audacious', 'K.K. Stroll - Shiho Fujii', 'Animal Crossing: New Horizons Original Soundtrack 2'">

# Usage
1. Download the current release from the [releases page](https://github.com/darktohka/audacious-plugin-rpc/releases).
2. Extract `libaudacious-plugin-rpc.so` into one of the following folders, depending on your installation:
    - `/usr/lib/audacious/General/`,
    - `/usr/lib64/audacious/General/`,
    - `/usr/lib/x86_64-linux-gnu/audacious/General/`
    - `...`
4. Open Audacious, go to Settings > Plugins and enable the `Discord RPC` plugin.

# Compilation
1. Clone the repository.
2. Compile and install the plugin:
```
mkdir build
cd build
cmake ..
make install
```
