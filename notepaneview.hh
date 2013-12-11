#ifndef _NOTEVIEW_HH_
#define _NOTEVIEW_HH_

#include <gtkmm.h>
#include <webkit/webkit.h>
#include "notedata.hh"

class NotePaneView : public Gtk::Box {
private:
	int a;
  WebKitWebView* webview;

  Gtk::ScrolledWindow* webviewWrapper;

public:
    NotePaneView (bool homogeneous, int spacing, Gtk::PackOptions options, int padding = 0);
    ~NotePaneView ();
};
#endif
