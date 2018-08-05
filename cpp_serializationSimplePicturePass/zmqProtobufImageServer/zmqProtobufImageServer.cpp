// zmq + protobuf
// server using zmq for protobuf message
//

#include <zmq.hpp>
#include <string>
#include <iostream>
#include "objectInfos.pb.h"
#include <opencv2/opencv.hpp>
int main()
{
	//  Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:6666");
	//socket.bind("ipc:///tmp/test");
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	while (true)
	{
		//  receive message
		zmq::message_t message;
		if (socket.recv(&message))
		{
			// decoode
			std::vector<uchar> imageData = std::vector<uchar>(message.size());
			memcpy(imageData.data(), message.data(), message.size());
			cv::Mat loaded_data = cv::imdecode(imageData, CV_LOAD_IMAGE_GRAYSCALE);
			//  show cv::mat
			{
				std::cout << "waiting for your key press on the image." << "\n";
				cv::imshow("load", loaded_data);
				cv::waitKey(0);
			}
		}
	
		//  send feedback
		{
			// random feedback
			proto::objectInfos stObjectInfos;
			{
				proto::objectInfos::objectInfo *p = stObjectInfos.add_infos();
				p->set_objclass(0);
				p->set_score(0);
				p->set_tlx(0);
				p->set_tly(0);
				p->set_brx(11);
				p->set_bry(32);
			}
			{
				proto::objectInfos::objectInfo *p = stObjectInfos.add_infos();
				p->set_objclass(1);
				p->set_score(2);
				p->set_tlx(3);
				p->set_tly(4);
				p->set_brx(67);
				p->set_bry(88);
			}

			// serialize
			std::string buf;
			stObjectInfos.SerializeToString(&buf);

			// send
			zmq::message_t message(buf.size());
			memcpy(message.data(), buf.c_str(), buf.size());
			socket.send(message);
		}
	}
	socket.close();
	return 0;
}