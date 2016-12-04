#include "ros/ros.h"

#include <iostream>
#include <math.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Twist.h"

int fd1;
int fd2;

char *buff, *buffer, *bufptr;

int wr, rd, wr2, rd2, nbytes, tries;

int main (int argc, char** argv)
{
	
	fd1 = open("/dev/rfcomm0", O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (fd1 == -1)
	{
		perror("open_port: Unable to open /dev/rfcomm0 -");
	}
	
	else 
	{
		fcntl(fd1, F_SETFL, 0);
		
		printf ("Port 1 has been successfully opend and %d is the file description, boi\n", fd1);
	}

	ros::init(argc, argv, "mySerial");
	ros::NodeHandle nh;	
	ros::Rate r(20);
	
	// Publisher for drone
	ros::Publisher accl_data_pub = nh.advertise<geometry_msgs::Vector3>("/accl_data", 1000);
	ros::Publisher bebop_commands = nh.advertise<geometry_msgs::Twist>("/bebop/cmd_vel", 1);
	
	while (ros::ok())
	{
		char* pEnd;
		wr = write(fd1,"a",1);
		usleep(100000);
		char tempbuff[512];
		rd = read(fd1,tempbuff,511);
		
		double temp = strtod(tempbuff, &pEnd);
		//printf("Bytes sent are %d \n", rd);
		//printf("Recieved: %lf\n",temp);
		//printf("test2 %f\n", tempbuff);
		
		wr2 = write(fd1,"b",1);
		usleep(100000);
		char tempbuff2[512];
		rd2 = read(fd1,tempbuff2,511);
		
		double temp2 = strtod(tempbuff2, &pEnd);
		//printf("Bytes sent are %d \n", rd);
		//printf("Recieved: %lf\n",temp2);
		//printf("test %f\n", tempbuff2);
		
		geometry_msgs::Vector3 accl_data_;
		geometry_msgs::Twist bebop_data_;
		
		accl_data_.x = temp; //save x data as x accl data from myRIO
		accl_data_.y = temp2; //save x data as y accl data from myRIO
		
		double x_data = temp;
		if (x_data > 0.25) {x_data = 0.25;}
		else if (x_data < -0.25) {x_data = -0.25;}
		else {x_data = x_data;}
		

		double y_data = temp2;
		if (y_data > 0.25) {y_data = 0.25;}
		else if (y_data < -0.25) {y_data = -0.25;}
		else {y_data = y_data;}
		
		bebop_data_.linear.x = x_data;
		bebop_data_.linear.y = -y_data;
		
		accl_data_pub.publish(accl_data_);
		bebop_commands.publish(bebop_data_);
		/*
		 * Real talk man, I'm trying to get out of here and be done 
		 * Dogg just make the data smooth and we cool
		 * Aight Imma try it out
		*/
		ros::spinOnce();
		r.sleep();
	}	
	
	close(fd1);
	std::cout << "P O R T S  C L O S E D" << std::endl;

	return 0;
}
