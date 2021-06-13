/*
Copyright (c) 2020-2021 FRC 6854 - Viking Robotics

This file is part of RobotLog.

RobotLog is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RobotLog is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RobotLog.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <fstream>
#include <iostream>

#include "../robotlog.hpp"

#define ANGLEDEEP 5.4
#define SPEEDFACTOR 8

void RobotLog::make_path(double robot_x, double robot_y, double hdg,
						 std::string path_file_filename) {
	double left_wheel, right_wheel;
	int len;
	int path_line = 0;

	std::ifstream path_file(path_file_filename);
	if (!path_file.is_open()) {
		std::cout << "path file open error\n";
		exit(2);
	}
	while (path_file >> left_wheel >> right_wheel) {
		hdg += (left_wheel - right_wheel) * ANGLEDEEP;
		len = (left_wheel + right_wheel) / 2 * SPEEDFACTOR;
		if (hdg < 0) {
			hdg += 360;
		}
		if (hdg > 360) {
			hdg -= 360;
		}
		if ((hdg >= 0) && (hdg < 90)) {
			robot_x += cos(deg2rad(hdg)) * len;
			robot_y += sin(deg2rad(hdg)) * len;
		} else if ((hdg >= 90) && (hdg < 180)) {
			robot_x -= cos(deg2rad(180 - hdg)) * len;
			robot_y += sin(deg2rad(180 - hdg)) * len;
		} else if ((hdg >= 180) && (hdg < 270)) {
			robot_x -= cos(deg2rad(hdg - 180)) * len;
			robot_y -= sin(deg2rad(hdg - 180)) * len;
		} else if ((hdg >= 270) && (hdg < 360)) {
			robot_x += cos(deg2rad(360 - hdg)) * len;
			robot_y -= sin(deg2rad(360 - hdg)) * len;
		}
		robot_path_x[path_line] = robot_x;
		robot_path_y[path_line] = robot_y;
		robot_path_hdg[path_line] = hdg;
		path_line++;
	}
	path_file.close();
	robot_data_size = path_line;
}
