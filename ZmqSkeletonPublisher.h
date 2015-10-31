#pragma once

#include "stdafx.h"

#include <time.h>

#include <zmq.h>


void PublishJointMassages(Joint* joints, JointOrientation* jointOrientations);
