# AppBuilder

Builds lynxOS app bundles from source. An app is a macOS-style bundle with a
JSON manifest — lynxOS has no plist (see `docs/bundle-format.md`):

```
MyApp.app/
  Contents/Info.json        compiled from App.toml
  Contents/Resources/       icon, art, ...
  Contents/lynxOS/MyApp     the executable
```

Authoring happens in a plain project directory with an `App.toml`; the build
compiles `Sources/` (cc by default, command overridable) straight into the
bundle and writes the manifest.

## Usage

```sh
appbuilder init MyApp [--id com.example.myapp] [--name "My App"]
appbuilder build            # -> out/MyApp.app
appbuilder check out/MyApp.app
```

`init` scaffolds a GUI project on [AppKit](https://github.com/lynxOSX/AppKit)
(the 10.6-SDK-style framework on Qt 6): `App.toml`, `Sources/main.cpp` (a
window, a File menu with "About <App>", an NSAlert demo), the vendored
`Sources/AppKit.h` / `Sources/AppKit.cpp` so `build` works with nothing
installed, a placeholder `Resources/AppIcon.png` (128×128 PNG, stdlib-only
writer — swap in real art), and a project `.gitignore`. The build needs Qt 6
Widgets + `pkg-config`; `-fPIC` is required when linking against Qt 6.

`build` compiles with the `[build] command` from `App.toml` (`{out}` is the
executable to produce), then assembles the bundle, writes `Contents/Info.json`
and runs the same validation as `check` before declaring success.

`check` works on both projects (`App.toml`, icon present, command sane) and
built bundles (layout, manifest, ELF executable, icon). `clean` removes
`out/`.

python3 stdlib only; needs 3.11+ (`tomllib`).

## Building the tool itself

`appbuilder` is a single executable script — copy or symlink it onto PATH:

```sh
ln -s "$PWD/appbuilder" ~/.local/bin/appbuilder
```

## Repository layout

| | |
|---|---|
| `appbuilder` | the tool (init/build/check/clean) |
| `templates/gui/` | the GUI scaffold (AppKit vendored: AppKit.h/cpp, main.cpp, App.toml) |
| `docs/bundle-format.md` | the `.app` format, Info.json keys, App.toml reference |

## Related

- `lynxOSX/AppKit` — the framework vendored into GUI scaffolds (canonical copy)
- `lynxOSX/rootFSBuilder` — creates the rootfs, including `/Applications`
- `lynxOSX/lynxOS` — the distro these bundles install into
