# spotify-qt
![spotify-qt with breeze dark](https://i.vgy.me/wF2EV5.png)

A Spotify client using Qt as a simpler, lighter alternative to the official client, inspired by [spotify-tui](https://github.com/Rigellute/spotify-tui).
Much like spotify-tui, you need an actual Spotify client running, for example [spotifyd](https://github.com/Spotifyd/spotifyd), which can be configured from within the app. Also like other clients, controlling music playback requires Spotify Premium.

Currently mostly stable. Most things should work, but don't be surprised when unexpected things happen, but please open an issue about it.
It's also currently only available through [AUR](https://aur.archlinux.org/packages/spotify-qt). Feel free to add it to any other package manager, but if you do, open a pull request or an issue so I can add it to a list.

Check out the [QML branch](https://github.com/kraxarn/spotify-qt/tree/qml) if you're interested in a Qt Quick UI.

## Supported platforms
Tested: Linux, Windows

Supported: macOS, *BSD, Haiku, any platform [officially supported](https://doc.qt.io/qt-5/supported-platforms.html) by Qt5

## Installing/building:
Make sure you have Qt (5.12+), CMake (3.5+) and various build tools like `make`, `g++` and `git`.
```
git clone https://github.com/kraxarn/spotify-qt.git
cd spotify-qt
mkdir build
cd build
cmake ..
make
./spotify-qt
```

## Known issues:
* Most network requests are done on the main thread.
* D-Bus media controller is buggy (see issue [#4](https://github.com/kraxarn/spotify-qt/issues/4)).
* "Made for you" is missing in library.