#include "ZmqSkeletonPublisher.h"

struct POINT3D{
	double x;
	double y;
	double z;
};
typedef POINT3D point;

typedef struct ORIENT{
	double x;
	double y;
	double z;
	double w;
}orient;

typedef struct COORDINATE{
	int id;
	time_t timeStamp;
	point p;
	orient q;
	char name[30];
}coordinate;

typedef struct JOINTINFO{
	char jointName[30];
	int indexOffset;
}JointInfo;

void PublishJointMassages(Joint* joints, JointOrientation* jointOrientations){
	static JointInfo jointInfoList[JointType_Count] = {
		{ "torso", 0 },//SpineBase 0
		{ "chest", 0 },//SpineMid 1
		{ "neck" , 0 },//Neck 2
		{ "head" , 0 },//Head 3
		{ "right_shoulder", 1 },//ShoulderLeft 4
		{ "right_elbow", 1 },//ElbowLeft 5
		{ "right_hand", 1 },//WristLeft 6
		{ "right_hand1", 0 },//HandLeft 7
		{ "left_shoulder", 1 },//ShoulderRight 8
		{ "left_elbow", 1 },//ElbowRight 9
		{  "left_hand", 1 },//WristRight 10
		{ "left_hand1", 0 },//HandRight 11
		{ "right_hip", 1 },//HipLeft 12
		{ "right_knee", 1 },//KneeLeft 13
		{ "right_foot", 0 },//AnkleLeft 14
		{ "right_foot1", 0 },//FootLeft 15
		{ "left_hip", 1 },//HipRight 16
		{ "left_knee", 1 },//KneeRight 17
		{ "left_foot", 0 },//AnkleRight 18
		{ "left_foot1", 0 },//FootRight 19
		{ "chest2", 0 },//SpineShoulder 20
		{ "right_hand2", 0 },//HandTipLeft 21
		{ "right_hand3", 0 },//ThumbLeft 22
		{ "left_hand2", 0 },//HandTipRight 23
		{ "left_hand3", 0 }//ThumbRight 24
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

		for (int j = 0; j < JointType_Count; ++j)
		{
			if (joints[j].TrackingState != TrackingState_NotTracked)
			{
				coord1.p.x = joints[j].Position.X;
				coord1.p.y = joints[j].Position.Y;
				coord1.p.z = joints[j].Position.Z;
				Vector4 q = jointOrientations[ j+jointInfoList[j].indexOffset ].Orientation;
				if (q.w != 0){
					coord1.q.x = q.x;
					coord1.q.y = q.y;
					coord1.q.z = q.z;
					coord1.q.w = q.w;
				}else{
					coord1.q.x = 1;
					coord1.q.y = 0;
					coord1.q.z = 0;
					coord1.q.w = 1;
				}
			}
			else{
				coord1.p.x = 0;
				coord1.p.y = 0;
				coord1.p.z = 0;
				coord1.q.x = 1;
				coord1.q.y = 0;
				coord1.q.z = 0;
				coord1.q.w = 1;
			}

			strcpy(coord1.name, jointInfoList[j].jointName);
			strcat(coord1.name, "_0");

			// msg
			zmq_msg_t msg;
			zmq_msg_init_size(&msg, sizeof(coord1));
			memcpy(zmq_msg_data(&msg), &coord1, sizeof(coord1));
			if ( j == JointType_Count - 1 ){
				zmq_msg_send(&msg, publisher, ZMQ_DONTWAIT);
			}
			else
			{
				zmq_msg_send(&msg, publisher, ZMQ_SNDMORE);
			}
		}

	}

}