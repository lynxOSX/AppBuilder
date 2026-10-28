/* {{DISPLAY_NAME}} — a lynxOS app (AppKit + Qt 6).
   AppKit is vendored in Sources/ (see the AppKit repo); this scaffold
   shows the 10.6-SDK style: a window, the File menu with "About
   {{DISPLAY_NAME}}", and an NSAlert. */
#include "AppKit.h"

#include <QLabel>
#include <QPushButton>
#include <cstdlib>

int main()
{
    NSApplication *app = NSApplication::sharedApplication();

    NSWindow *win = NSWindow::windowWithTitle("{{DISPLAY_NAME}}", 420, 260);
    auto *label = new QLabel("Hello from a lynxOS app", win->contentView());
    label->setAlignment(Qt::AlignCenter);
    win->contentLayout()->addWidget(label);

    auto *button = new QPushButton("Show Alert", win->contentView());
    win->contentLayout()->addWidget(button);

    /* a small utility window: the compact 14 px titlebar, like macOS X's
       small captions on panels and Finders dialogs */
    auto *small = new QPushButton("Show Small Window", win->contentView());
    win->contentLayout()->addWidget(small);

    QObject::connect(small, &QPushButton::clicked, [] {
        NSWindow *pan = NSWindow::windowWithTitle("Small Window", 220, 140);
        NSSetWindowCompactTitlebar(pan->qtWindow(), true);
        auto *lbl = new QLabel("A small utility window", pan->contentView());
        lbl->setAlignment(Qt::AlignCenter);
        pan->contentLayout()->addWidget(lbl);
        pan->makeKeyAndOrderFront();
    });

    QObject::connect(button, &QPushButton::clicked, [app] {
        NSAlert *alert = NSAlert::alertWithMessageText(
            "{{DISPLAY_NAME}}", NSInformationalAlertStyle,
            "OK", QString(), QString(),
            "An NSAlert, with the app's bundle icon.");
        alert->runModal();
    });

    /* the File menu, macOS style */
    NSMenu *file = new NSMenu("File");
    file->addItem(new NSMenuItem("About {{DISPLAY_NAME}}",
        [app] { app->orderFrontStandardAboutPanel(); }));
    file->addSeparator();
    file->addItem(new NSMenuItem("Quit {{DISPLAY_NAME}}",
        [app] { exit(0); }, "Ctrl+Q"));
    win->setMainMenu(file);

    win->makeKeyAndOrderFront();
    app->run();
    return 0;
}
