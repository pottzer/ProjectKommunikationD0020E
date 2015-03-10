/*
* Copyright 2013, Pierre St Juste
* Copyright 2014, Chris Ball
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "ListenerSocket.h"
#include "WebRtcConnectionManager.h"


void createConnectionString(WebRtcConnectionManager* manager)
{
	manager->InitConnection();
	manager->CreateOffer();
}


int main(int argc, char **argv) {

	ListenerSocket socketListener;
	socketListener.initSocketServer(2222);

	WebRtcConnectionManager manager;

	rtc::InitializeSSL(NULL);
	createConnectionString(&manager);
	while (1) {
		std::string input;
		std::cout << "Enter command:" << std::endl;
		std::getline(std::cin, input);


		if (input.compare("print") == 0) {
			std::string string_desc = manager.DescriptionToString(
					manager.local_description());
			std::cout << string_desc << std::endl;

			std::cout << "Enter reply:" << std::endl;
			std::getline(std::cin, string_desc);
			webrtc::SessionDescriptionInterface* desc =
					manager.StringToDescription(string_desc);
			manager.OnOfferReply(desc);

			while (1) {
				//Waits here until clients send data.
				std::string output = socketListener.listenPort();

				//Send data through webrtc
				std::cout << "message from socket recieved: " << output << std::endl;
				Json::FastWriter writer;
				Json::Value json;
				json["message"] = output;
				std::string message = writer.write(json);
				DataChannelInterface* data = manager.data_channel();
				if (data->state() == webrtc::DataChannelInterface::kOpen)
					data->Send(webrtc::DataBuffer(message));
				else
					std::cout << "Warning: data channel is not open."
							<< std::endl;
			}

		}
	}
	rtc::CleanupSSL();
	return 0;
} 
