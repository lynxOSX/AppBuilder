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

`init` scaffolds a project: `App.toml`, a hello-world `Sources/main.c`, a
placeholder `Resources/AppIcon.png` (128×128 PNG, stdlib-only writer — swap
in real art), and a project `.gitignore`.

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
| `docs/bundle-format.md` | the `.app` format, Info.json keys, App.toml reference |

## Related

- `lynxOSX/rootFSBuilder` — creates the rootfs, including `/Applications`
- `lynxOSX/lynxOS` — the distro these bundles install into
