# The lynxOS app bundle format

A lynxOS app is a directory, `<Name>.app`, laid out like a macOS bundle but
with a JSON manifest (lynxOS has no plist):

```
MyApp.app/
  Contents/
    Info.json          the manifest (compiled from App.toml by appbuilder)
    Resources/         read-only app files: icon, art, strings...
    lynxOS/            the executable lives here, as in macOS's MacOS/
      MyApp
```

`/Applications/<Name>.app` is how apps install (the skeleton creates
`/Applications` — see rootFSBuilder's `scripts/apply-skel.sh`).

## Preference panes (`[bundle] type = "prefPane"`)

A System Preferences pane is the same bundle layout with a different
package type: set `type = "prefPane"` in `[bundle]` and the project
builds `<Name>.prefPane` with `CFBundlePackageType = "BNDL"` (and the
icon also written to `NSPrefPaneIconFile`, the key System Preferences
reads). The "executable" is a **shared library** — the pane's compiled
view — placed in `Contents/lynxOS/` like an app's executable; it is
dlopen()ed by System Preferences, which looks for panes in
`/System/Library/PreferencePanes/`.

A pane exports (extern "C", like the menu-extras ABI):

| symbol | meaning |
|---|---|
| `lynxPrefPaneCreate()` | returns the pane's root `QWidget*` |
| `lynxPrefPaneDestroy(QWidget*)` | destroys it before the library is closed |

## Info.json

Written by `appbuilder build`, sorted and indented. Standard keys use the
macOS CoreFoundation names (the distro is a macOS-a-like; the window server
reads these):

| key | from App.toml | notes |
|---|---|---|
| `CFBundleName` | `[bundle] name` | short name, also the bundle dir name |
| `CFBundleDisplayName` | `[bundle] display_name` | defaults to `name` |
| `CFBundleIdentifier` | `[bundle] identifier` | reverse-DNS; key for settings/storage |
| `CFBundleShortVersionString` | `[bundle] version` | `x.y` or `x.y.z` |
| `CFBundleVersion` | `[bundle] build` | integer, ≥ 1; bump for every install over an old one |
| `CFBundleExecutable` | `[bundle] executable` | at `Contents/lynxOS/` |
| `CFBundleIconFile` | `[bundle] icon` | a file in `Resources/`, PNG |
| `CFBundlePackageType` | — | always `"APPL"` |
| `CFBundleInfoDictionaryVersion` | — | always `"6.0"` |

Any `[info]` table in `App.toml` is copied verbatim into the manifest, so an
app can carry its own keys (`windowSize`, `minimumSystem`, ...). Overriding a
standard key is allowed but warned about.

## App.toml (the project, at build time)

```toml
[bundle]
name = "MyApp"
display_name = "My App"
identifier = "com.example.myapp"
version = "0.1.0"
build = 1
executable = "MyApp"
icon = "AppIcon.png"

[build]
command = ["cc", "-O2", "-std=c11", "-o", "{out}", "Sources/main.c"]

[info]
windowSize = [640, 400]
```

`{out}` in the build command is the path of the executable to produce — the
compiler writes straight into the fresh bundle at
`<out>/<Name>.app/Contents/lynxOS/<Executable>`. The default command compiles
`Sources/main.c` with `cc`.

## What `appbuilder check` enforces

- bundle layout: `Contents/Info.json`, `Contents/lynxOS/`, `Contents/Resources/`
- manifest: parses as JSON, required keys present, identifier reverse-DNS
- executable: exists, executable bit set, ELF
- icon: present in `Resources/` and a PNG
- right after a build: the manifest must match `App.toml` exactly
