#!/usr/bin/env bash

# Capture the requested language/theme/time combinations from every supported
# emulator and assemble one endlessly looping GIF per platform.
# Pass platform names as arguments to capture only a subset, for example:
#   scripts/capture_language_theme_demo.sh emery flint
set -euo pipefail

readonly PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
readonly APP_UUID="541c1176-8eb5-4105-aa83-1cd0214166ee"
readonly FRAME_DELAY_SECONDS=3
readonly TIME_SETTLE_SECONDS=2

readonly -a DEFAULT_PLATFORMS=(aplite basalt diorite emery flint)
readonly -a TIMES=(00:00)
readonly -a COLORS=(black)
readonly -a LANGUAGES=(en)

emulator_logs_pid=""
emulator_logs_file=""

if ! command -v ffmpeg >/dev/null 2>&1; then
  echo "ffmpeg is required to create the GIF." >&2
  exit 1
fi

cd "$PROJECT_DIR"

platforms=("${DEFAULT_PLATFORMS[@]}")
if (( $# > 0 )); then
  platforms=("$@")
fi

# The Pebble SDK build requires this Node version. A login shell provides nvm.
bash -lic 'nvm use v24.14.0 && pebble build'

capture_screenshot() {
  local platform="$1"
  local frame="$2"
  local attempt

  for attempt in 1 2 3; do
    if pebble screenshot --emulator "$platform" --no-open "$frame"; then
      return 0
    fi
    sleep 1
  done

  echo "Unable to capture $frame after three attempts." >&2
  return 1
}

stop_emulator() {
  if [[ -n "$emulator_logs_pid" ]]; then
    kill "$emulator_logs_pid" 2>/dev/null || true
    wait "$emulator_logs_pid" 2>/dev/null || true
    emulator_logs_pid=""
  fi

  if [[ -n "$emulator_logs_file" ]]; then
    rm -f "$emulator_logs_file"
    emulator_logs_file=""
  fi
}

start_emulator() {
  local platform="$1"
  local attempt

  # The Pebble CLI uses global emulator state. A QEMU left by a previous run
  # can make individual --emulator commands attach to different stale bridge
  # processes, so `emu-set-time` appears to succeed but screenshots keep the
  # old clock. Start each platform from one known, clean instance.
  pebble kill --force >/dev/null 2>&1 || true

  # Commands using --emulator create a QEMU instance only for the duration of
  # that command. Keep `pebble logs` alive so the install, time changes, app
  # messages, and screenshots all target one persistent watch.
  emulator_logs_file="$(mktemp)"
  pebble logs -vv --emulator "$platform" >"$emulator_logs_file" 2>&1 &
  emulator_logs_pid=$!

  # Do not use `pebble ping` as a probe: it puts an overlay on the watch face.
  # More importantly, waiting for the actual boot messages ensures that later
  # commands attach to this QEMU instead of spawning another one at host time.
  for attempt in {1..30}; do
    if { rg -q 'Firmware booted\.' "$emulator_logs_file" &&
         rg -q 'pypkjs:Ready\.' "$emulator_logs_file"; } ||
       { rg -q 'QEMU is already running\.' "$emulator_logs_file" &&
         rg -q 'pypkjs is already running\.' "$emulator_logs_file"; }; then
      return 0
    fi

    if ! kill -0 "$emulator_logs_pid" 2>/dev/null; then
      break
    fi
    sleep 1
  done

  cat "$emulator_logs_file" >&2
  echo "Unable to start a ready $platform emulator." >&2
  return 1
}

install_watchface() {
  local platform="$1"
  local attempt

  for attempt in 1 2 3; do
    if pebble install --emulator "$platform" build/pebble-qlocktwo.pbw; then
      return 0
    fi
    sleep 1
  done

  echo "Unable to install the watchface on $platform after three attempts." >&2
  return 1
}

show_watchface() {
  local platform="$1"

  # A previous diagnostic `pebble ping` can leave its overlay open in the
  # emulator's persistent state. Back returns to the installed watch face and
  # is a no-op when it is already visible.
  pebble emu-button --emulator "$platform" click back
}

set_emulator_time() {
  local platform="$1"
  local time="$2"
  local attempt

  for attempt in 1 2 3; do
    if pebble emu-set-time --emulator "$platform" --utc "${time}:00"; then
      return 0
    fi
    sleep 1
  done

  echo "Unable to set $time on $platform after three attempts." >&2
  return 1
}

set_watchface_options() {
  local platform="$1"
  local language="$2"
  local color="$3"
  local attempt

  for attempt in 1 2 3; do
    if pebble send-app-message --emulator "$platform" --app-uuid "$APP_UUID" \
      --string "10000=$language" &&
      # `pebble send-app-message` accepts multiple values syntactically, but
      # the emulator CLI only delivers the first one. Send each setting in
      # its own AppMessage so language and palette both reach the watchface.
      pebble send-app-message --emulator "$platform" --app-uuid "$APP_UUID" \
        --string "10001=$color"; then
      return 0
    fi
    sleep 1
  done

  echo "Unable to configure $platform after three attempts." >&2
  return 1
}

capture_platform() {
  local platform="$1"
  local output_dir="$PROJECT_DIR/resources/images/screenshots/$platform/language-theme-demo"
  local output_gif="$output_dir/language-theme-demo.gif"
  local index time color language frame

  mkdir -p "$output_dir"
  rm -f "$output_dir"/frame-*.png "$output_gif"

  start_emulator "$platform"
  install_watchface "$platform"
  show_watchface "$platform"

  for index in "${!TIMES[@]}"; do
    time="${TIMES[$index]}"
    color="${COLORS[$index]}"
    language="${LANGUAGES[$index]}"
    frame="$output_dir/frame-$(printf '%02d' "$index")-${time//:/-}-${color}-${language}.png"

    set_watchface_options "$platform" "$language" "$color"
    # Sending an AppMessage can restart the emulator-side bridge and restore
    # its host clock. Set the requested time last, immediately before capture.
    set_emulator_time "$platform" "$time"
    # Time changes are delivered to the watchface by its tick timer. Wait for
    # a complete tick after setting the emulator clock before capturing.
    sleep "$TIME_SETTLE_SECONDS"
    capture_screenshot "$platform" "$frame"
  done

  ffmpeg -y -loglevel error -framerate "1/$FRAME_DELAY_SECONDS" \
    -pattern_type glob -i "$output_dir/frame-*.png" -loop 0 "$output_gif"
  stop_emulator
  echo "Created $output_gif"
}

trap stop_emulator EXIT

for platform in "${platforms[@]}"; do
  capture_platform "$platform"
done
