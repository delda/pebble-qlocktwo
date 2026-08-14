# pebble-qlocktwo

A minimal Pebble watchface written in C using the Pebble SDK. It shows only
the current hour and minute, centred on the display, and follows the watch's
12/24-hour preference.

## Building & running

```sh
pebble build                          # build for all targetPlatforms
pebble install --emulator emery       # install on the emery emulator
pebble install --phone <ip>           # install to a paired phone
```

## Target platforms

`targetPlatforms` in `package.json` controls which watches you build for. The
round platforms, **chalk** (Pebble Time Round) and **gabbro** (Pebble Round 2),
are excluded. The remaining targets are emery (Pebble Time 2), flint (Pebble 2
Duo), and the original aplite, basalt, and diorite platforms.

## Project layout

```
src/c/           C source for the watchapp
src/pkjs/        PebbleKit JS (phone-side) source, if any
worker_src/c/    Background worker source, if any
resources/       Images, fonts, and other bundled resources
package.json     Project metadata (UUID, platforms, resources, message keys)
wscript          Build rules — usually no need to edit
```

This project is configured as a watchface (`pebble.watchapp.watchface` is
`true` in `package.json`).

## Documentation

Full SDK docs, tutorials, and API reference: <https://developer.repebble.com>
