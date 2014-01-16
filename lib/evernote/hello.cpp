#include <protocol/TBinaryProtocol.h>
#include <transport/THttpClient.h>
#include <transport/TSSLSocket.h>
#include <vector>

#include "UserStore.h"
#include "NoteStore.h"



int main () {
	std::string site = "sandbox.evernote.com";
	boost::shared_ptr<apache::thrift::transport::THttpClient> auth_http( new apache::thrift::transport::THttpClient("sandbox.evernote.com", 80, "/edam/user") );
  	auth_http->open();
	boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> userStoreProt( new apache::thrift::protocol::TBinaryProtocol(auth_http) );
  	evernote::edam::UserStoreClient userStore(userStoreProt, userStoreProt );

  	std::string  noteStoreUrl = "";
  	std::string authToken = "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0";
  	userStore.getNoteStoreUrl (noteStoreUrl, "S=s1:U=7558a:E=14aae5ecd73:C=14356ada175:P=1cd:A=en-devtoken:V=2:H=905a30846fdad07b83592ff73da7a7c0");
	auth_http->close();  	
  	std::cout << noteStoreUrl << std::endl;

  	noteStoreUrl = "/shard/s1/notestore";

	boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory> sslSocketFactory = boost::shared_ptr<apache::thrift::transport::TSSLSocketFactory>(new apache::thrift::transport::TSSLSocketFactory());;

 	boost::shared_ptr<apache::thrift::transport::TSocket> sslSocket = sslSocketFactory->createSocket("sandbox.evernote.com", 443);
    boost::shared_ptr<apache::thrift::transport::TBufferedTransport> bufferedTransport(new apache::thrift::transport::TBufferedTransport(sslSocket));
    boost::shared_ptr<apache::thrift::transport::THttpClient> userStoreHttpClient = boost::shared_ptr<apache::thrift::transport::THttpClient>(new apache::thrift::transport::THttpClient(bufferedTransport, "sandbox.evernote.com", noteStoreUrl));

    userStoreHttpClient->open();

	boost::shared_ptr<apache::thrift::protocol::TBinaryProtocol> noteStoreProt(new apache::thrift::protocol::TBinaryProtocol(userStoreHttpClient) );
  	evernote::edam::NoteStoreClient noteStore(noteStoreProt, noteStoreProt );

  	std::vector<evernote::edam::Notebook> notebooks;

	noteStore.listNotebooks(notebooks, authToken);
	userStoreHttpClient->close();
	
	userStoreHttpClient->close();

 	std::cout << notebooks.size () << std::endl;

    return 0;
}
