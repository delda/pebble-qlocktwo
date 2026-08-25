# Word Clock for Pebble

A Pebble watchface inspired by QLOCKTWO. It displays the current time as words
in an English letter grid.

## How it works

The watchface highlights the words that describe the time, rounded down to the
nearest five minutes:

- `IT IS FIVE PAST TEN`
- `IT IS A QUARTER TO THREE`
- `IT IS HALF PAST SEVEN`

The four dots at the bottom of the display indicate the remaining minutes. For
example, at 10:07 the watchface shows `IT IS FIVE PAST TEN` and two dots.

The grid and time phrases are available in English, Italian, French, Spanish,
German, Dutch, and Portuguese.

## Customisation

The configuration screen in the Pebble/Rebble app lets you choose the
watchface colour: black, white, red, purple, azure, green, pink, or brown. The
choice is stored on the watch and retained across restarts.

## Supported platforms

- aplite — Pebble
- basalt — Pebble Time
- diorite — Pebble Time Steel
- emery — Pebble Time 2
- flint — Pebble 2

The round chalk and gabbro platforms are not supported.

## Development

Requires Pebble SDK 3 and the project's JavaScript dependencies:

```sh
npm install
pebble build
```

To try the watchface in the Emery emulator:

```sh
pebble install --emulator emery
```

Or to install it on a paired phone:

```sh
pebble install --phone <ip>
```

## Project layout

```
src/c/                 Watchface logic and grid rendering
src/pkjs/              PebbleKit JS / Clay configuration
resources/fonts/       Stencil font bundled with the app
resources/images/      Watchface icon
package.json           Metadata, platforms, resources, and message keys
wscript                Pebble build rules
```

For SDK documentation, see <https://developer.rebble.io>.
