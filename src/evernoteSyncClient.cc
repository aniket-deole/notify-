#include <dlfcn.h>
#include <iostream>
#include <string>
#include <cstring>
#include <libevernote/evernote.hh>
#include <evernoteSyncClient.hh>

#include <utils.hh>

std::string consumerKey 					= "analogx";
std::string consumerSecret 				= "953b9c5d5f64c09b";
std::string requestTokenUrl 			= "https://sandbox.evernote.com/oauth";
std::string requestTokenQueryArgs = "oauth_callback=sandbox.evernote.com";
std::string authorizeUrl 					= "https://sandbox.evernote.com/OAuth.action";
std::string accessTokenUrl 				= "https://sandbox.evernote.com/oauth";

void EvernoteSyncClient::thirdStageComplete (WebKitWebView* webView,
		WebKitWebFrame* webFrame, gpointer userData) {
		
	std::string tokenData = "";
	webkit_web_view_execute_script (webView,
			"document.title=document.documentElement.innerHTML;");
	tokenData.append (webkit_web_frame_get_title (webFrame));

	tokenData = replaceString (tokenData, "%3D", "=");
	tokenData = replaceString (tokenData, "%3A", ":");

	tokenData = tokenData.substr (tokenData.find ("oauth_token"));
	tokenData = tokenData.replace (tokenData.find ("<"), tokenData.length (), "");

	tokenData = replaceString (tokenData, "&amp;", "&");
	std::cout << "Third Loading:" << tokenData << std::endl;

	EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
    OAuthManager_generateAccessToken_t* OAuthManager_generateAccessToken_p = 
      (OAuthManager_generateAccessToken_t*) dlsym (esc->handle, "OAuthManager_generateAccessToken");

    std::string authToken = OAuthManager_generateAccessToken_p (esc->oAuthManager, tokenData);

    // load the symbols.
    createUserStore_t* createUserStore_p = (createUserStore_t*) dlsym(esc->handle, "createUserStore");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    }
    evernote::UserStore* userStore = createUserStore_p ("sandbox.evernote.com", 80, "/edam/user", authToken);

    UserStore_getNoteStoreUrl_t* UserStore_getNoteStoreUrl_p = (UserStore_getNoteStoreUrl_t*) dlsym (esc->handle,"UserStore_getNoteStoreUrl");

    createNoteStore_t* createNoteStore_p = (createNoteStore_t*) dlsym (esc->handle, "createNoteStore");
    evernote::NoteStore* noteStore = createNoteStore_p (UserStore_getNoteStoreUrl_p (userStore, authToken));

    // std::cout << "*. List all notebooks\n";
    // std::cout << "----------------------------------------------------------------\n";
    NoteStore_listNotebooks_t* NoteStore_listNotebooks_p = (NoteStore_listNotebooks_t*) dlsym (esc->handle, "NoteStore_listNotebooks");
    std::vector<evernote::Notebook*>* notebookList = NoteStore_listNotebooks_p (noteStore, authToken);

    for (int i = 0; i < notebookList->size (); i++) {
         std::cout << notebookList->at (i)->stack << ":" << notebookList->at (i)->name << std::endl;
    }

    // std::cout << "----------------------------------------------------------------\n";
    // std::cout << "----------------------------------------------------------------\n";

    NoteStore_createNoteFilter_t* NoteStore_createNoteFilter_p = (NoteStore_createNoteFilter_t*) dlsym (esc->handle, "NoteStore_createNoteFilter");
    evernote::NoteFilter* nf = NoteStore_createNoteFilter_p ();
    NoteStore_createNotesMetadataResultSpec_t* NoteStore_createNotesMetadataResultSpec_p = 
        (NoteStore_createNotesMetadataResultSpec_t*) dlsym (esc->handle, "NoteStore_createNotesMetadataResultSpec");
    evernote::NotesMetadataResultSpec* nmrs = NoteStore_createNotesMetadataResultSpec_p ();
    nmrs->includeTitle = true;

    NoteStore_findNotesMetadata_t* NoteStore_findNotesMetadata_p = (NoteStore_findNotesMetadata_t*) dlsym (esc->handle, "NoteStore_findNotesMetadata");

    evernote::NotesMetadataList* nml = NoteStore_findNotesMetadata_p (noteStore, authToken, nf, 0, 20, nmrs);
    // std::cout << nml->totalNotes << std::endl;
    for (int i = 0; i < nml->notes.size (); i++) {
        // std::cout << nml->notes[i]->title << std::endl;
        // // std::cout << noteStore->getNoteContent (authToken, nml->notes[i]->guid);
       NoteStore_getNote_t* NoteStore_getNote_p = (NoteStore_getNote_t*) dlsym (esc->handle, "NoteStore_getNote");

       evernote::Note* note = NoteStore_getNote_p (noteStore, authToken, nml->notes[i]->guid, true, true, false, false);
       
       Note_enmlToHtml_t* Note_enmlToHtml_p = (Note_enmlToHtml_t*) dlsym (esc->handle, "Note_enmlToHtml");
       Note_enmlToHtml_p (note);

       std::cout << note->contentEnml << std::endl;
       std::cout << "-----------------------------";
       std::cout << "\n" << note->contentHtml << std::endl;
       std::cout << "-----------------------------\n";
       std::cout << "-----------------------------\n";
        for (int j = 0; j < note->resources.size (); j++) {
//            std::ofstream myfile;
//                  myfile.open ("asd");
//              myfile << note->resources[j]->data->body;
//              myfile.close();
          std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(note->resources[j]->data->body.c_str ()), note->resources[j]->data->size);

         //std::cout << "encoded: " << encoded << std::endl;                
        }
    }

    // Create a Note.
    NoteStore_createNote_t* NoteStore_createNote_p = (NoteStore_createNote_t*) dlsym (esc->handle, "NoteStore_createNote");
    evernote::Note* note = NoteStore_createNote_p ();
    note->title = "First UpSync Note.i fromapplicatoin";
    note->content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE en-note SYSTEM \"http://xml.evernote.com/pub/enml2.dtd\"><en-note> HelloWorld </en-note>";

    NoteStore_createNote2_t* NoteStore_createNote2_p = (NoteStore_createNote2_t*) dlsym (esc->handle, "NoteStore_createNote2");

    NoteStore_createNote2_p (noteStore, authToken, note);


}

void EvernoteSyncClient::secondStageComplete (WebKitWebView *webView,
		WebKitWebFrame* webFrame, gpointer userData) {

	std::string uri = webkit_web_view_get_uri (webView);
	
	if (uri.find ("oauth_verifier") == -1) {
		return;
	} else {
		std::cout << uri << std::endl;
	}

	EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
	g_signal_handler_disconnect (webView, esc->signalHandlerId);

	esc->signalHandlerId = g_signal_connect (webView, "document-load-finished",
			G_CALLBACK (&EvernoteSyncClient::thirdStageComplete), userData);

	// Extract pin from uri
	// i.e. extract oauth_verifier from
	// https://sandbox.evernote.com/sandbox.evernote.com?oauth_token=analogx.14E45C5D180.
	// 73616E64626F782E657665726E6F74652E636F6D.1E46AD4B8C702FB88068A3C66E410E08
	// &oauth_verifier=C910C86D82CF819E3E5DAF38683024FA&sandbox_lnb=false
	
	std::string pin = "";
	pin = uri.substr (uri.find ("oauth_verifier"));
	pin = pin.substr (pin.find ("=") + 1);
	pin = pin.replace (pin.find ("&"), pin.length (), "");

	std::cout << "Pin:" << pin << std::endl;

  OAuthManager_generateFinalAccessTokenUrl_t* OAuthManager_generateFinalAccessTokenUrl_p =
    (OAuthManager_generateFinalAccessTokenUrl_t*) dlsym (esc->handle, 
				"OAuthManager_generateFinalAccessTokenUrl");
  std::string fat = OAuthManager_generateFinalAccessTokenUrl_p (esc->oAuthManager, pin);

	esc->ewvb->hide ();
	esc->app->remove ();
	esc->app->add (*esc->app->windowBody);
	esc->app->show_all ();

	webkit_web_view_load_uri (webView, fat.c_str ());
	std::cout << "Second loading:" << fat << std::endl;	
	return;
}

void EvernoteSyncClient::firstStageComplete (WebKitWebView  *webView,
               WebKitWebFrame *webFrame,
               gpointer        userData) {
	std::string tokenData = "";
	webkit_web_view_execute_script (webView,
			"document.title=document.documentElement.innerHTML;");
	tokenData.append (webkit_web_frame_get_title (webFrame));


	tokenData = tokenData.substr (tokenData.find ("oauth_token"));
	
	tokenData = tokenData.replace (tokenData.find ("<"), tokenData.length (), "");

	std::cout << tokenData << std::endl;

	EvernoteSyncClient* esc = (EvernoteSyncClient*) userData;
	
	OAuthManager_generateAuthorizationUrl_t* OAuthManager_generateAuthorizationUrl_p =
		(OAuthManager_generateAuthorizationUrl_t*) dlsym (esc->handle, "OAuthManager_generateAuthorizationUrl");

	tokenData = replaceString (tokenData, "&amp;", "&");
	
	std::string authUrl = OAuthManager_generateAuthorizationUrl_p (esc->oAuthManager, tokenData);

	g_signal_handler_disconnect (webView, esc->signalHandlerId);
	esc->signalHandlerId = g_signal_connect (webView, "document-load-finished",
			G_CALLBACK (&EvernoteSyncClient::secondStageComplete), userData);
	webkit_web_view_load_uri (webView, authUrl.c_str ());
	esc->ewvb->show ();
	return;
}


int EvernoteSyncClient::sync () {
    /* Check if library is loaded. Else ask the user to install it. */
    handle = dlopen ("libevernote.so", RTLD_LAZY);
    if (!handle) {
      const char* dlsym_error = dlerror();
      std::cout << "Please Install libevernote.so along with its dependencies.\nError: " << 
				dlsym_error << std::endl;
      return 1;
    }

		createOAuthManager_t* createOAuthManager_p = (createOAuthManager_t*) dlsym (handle, 
				"createOAuthManager");

		oAuthManager = createOAuthManager_p (consumerKey,
				consumerSecret, requestTokenUrl, requestTokenQueryArgs, authorizeUrl,
				accessTokenUrl);

		OAuthManager_generateRequestTokenUrl_t* OAuthManager_generateRequestTokenUrl_p =
			(OAuthManager_generateRequestTokenUrl_t*) dlsym (handle, "OAuthManager_generateRequestTokenUrl");

		std::string rqu = OAuthManager_generateRequestTokenUrl_p (oAuthManager);

		std::cout << rqu << std::endl;

		// Hide the normal ui and get permissions from evernote.
		app->remove ();
		ewvb = Gtk::manage (new EvernoteWebViewBox (false, 0, Gtk::PACK_SHRINK, 0, app));
		ewvb->hide ();
		app->add (*ewvb);		
		
		webkit_web_view_load_uri (ewvb->webview, rqu.c_str ());
		signalHandlerId = g_signal_connect (ewvb->webview, "document-load-finished", 
				G_CALLBACK (&EvernoteSyncClient::firstStageComplete), this);
    
    return 0;
}
