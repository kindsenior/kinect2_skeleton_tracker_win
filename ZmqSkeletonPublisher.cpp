#include "ZmqSkeletonPublisher.h"

struct POINT3D{
	double x;
	double y;
	double z;
};
typedef POINT3D point;

typedef struct COORDINATE{
	int id;
	time_t timeStamp;
	point p;
	char name[30];
}coordinate;

void PublishJointMassages(Joint* joints){
	static char jointNameList[JointType_Count][30] = {
		"torso",//SpineBase
		"chest",//SpineMid
		"neck",//Neck
		"head",//Head
		"left_shoulder",//ShoulderLeft
		"left_elbow",//ElbowLeft
		"left_hand",//WristLeft
		"left_hand1",//HandLeft
		"right_shoulder",//ShoulderRight
		"right_elbow",//ElbowRight
		"right_hand",//WristRight
		"right_hand1",//HandRight
		"left_hip",//HipLeft
		"left_knee",//KneeLeft
		"left_foot",//AnkleLeft
		"left_foot1",//FootLeft
		"right_hip",//HipRight
		"right_knee",//KneeRight
		"right_foot",//AnkleRight
		"right_foot1",//FootRight
		"chest2",//SpineShoulder
		"left_hand2",//HandTipLeft
		"left_hand3",//ThumbLeft
		"right_hand2",//HandTipRight
		"right_hand3"//ThumbRight
	};

	// 0MQÇÃèâä˙âª
	static void *context = zmq_ctx_new();
	static void *publisher = zmq_socket(context, ZMQ_PUB);
	static int dummyVal = zmq_connect(publisher, "tcp://192.168.97.146:5556");// clientÇ™connect


	coordinate coord1;
	coord1.id = 1;
	coord1.timeStamp = time(NULL);

	static int loopNum = 1;
	for (int i = 0; i < loopNum; ++i){
		//zmq_msg_t msg;
		//zmq_msg_send(&msg, "hoge", 0);


		for (int j = 0; j < JointType_Count; ++j)
		//for (int j = 0; j < 3; ++j)
		{
			if (joints[j].TrackingState == TrackingState_Inferred)
			{
				coord1.p.x = joints[j].Position.X;
				coord1.p.y = joints[j].Position.Y;
				coord1.p.z = joints[j].Position.Z;
			}
			else if (joints[j].TrackingState == TrackingState_Tracked)
			{
				coord1.p.x = joints[j].Position.X;
				coord1.p.y = joints[j].Position.Y;
				coord1.p.z = joints[j].Position.Z;
			}
			else{
				coord1.p.x = 0;
				coord1.p.y = 0;
				coord1.p.z = 0;
			}

			//strcpy(coord1.name, "/");
			//strcat(coord1.name, jointNameList[j]);
			strcpy(coord1.name, jointNameList[j]);
			strcat(coord1.name, "_0");

			// msg
			zmq_msg_t msg;
			zmq_msg_init_size(&msg, sizeof(coord1));
			memcpy(zmq_msg_data(&msg), &coord1, sizeof(coord1));
			if ( j == JointType_Count - 1 ){
			//if ( j == 3-1 ){
				zmq_msg_send(&msg, publisher, ZMQ_DONTWAIT);
			}
			else
			{
				zmq_msg_send(&msg, publisher, ZMQ_SNDMORE);
			}
		}

	}

}