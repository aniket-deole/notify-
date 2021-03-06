/***
Copyright (C) 2013 Aniket Deole <aniket.deole@gmail.com>
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 2.1, as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranties of
MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef _MAINTOOLBAR_HH_
#define _MAINTOOLBAR_HH_

#include <gtkmm/toolbar.h>
#include "notify.hh"

class Notify;

class MainToolbar : public Gtk::HeaderBar {
private:
    int a;

    bool searchEntryActive;
    bool collapsedHeaderBar;
    bool syncStarted;
		

public:
    Notify* app;
    bool syncComplete;
    bool progressBarStarted;
    MainToolbar ();
    ~MainToolbar ();
    void setApp (Notify* a) { app = a; }
    void exitNotify ();
    void newNote ();
    void newNotebook ();
    void searchCallback ();
    void searchEntryClicked ();
    void toggleHeaderBarCallback ();
    void syncButtonCallback ();
    void syncButtonSandboxCallback ();
    void syncButtonConsolidatedCallback (int);
    bool on_timeout ();
    static void* asynchronousSync (void* data, int);

    bool connectedToEvernote;
    static std::string headerBarSubStatus;
    
		bool evernoteConnectionInProgress;
		Gtk::Entry* searchEntry;
    Gtk::Button* newNoteButton;
    Gtk::Button* newNotebookButton;
    Gtk::Button* syncButton;
    Gtk::ProgressBar* progressBar;
    Gtk::Button* collapseHeaderBar;

		std::string tempAuthToken;
		static int checkAuthTokenCallback (void* esc, int argc, char** argv, char** azColName);

	
};

#endif
