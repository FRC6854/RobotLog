/*
Copyright (c) 2020-2022 FRC 6854 - Viking Robotics

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

#include <cmath>
#include <fstream>
#include <iostream>

#include "../pathreader.hpp"

#define ANGLEDEEP 5.4
#define SPEEDFACTOR 8

std::vector<PathPoint> StevePathReader::read_path(const std::string& path_file_filename,
												  double init_x, double init_y, double init_hdg) {
	double left_wheel, right_wheel;
	int len;

	double robot_x = init_x;
	double robot_y = init_y;
	double hdg = init_hdg;

	std::vector<PathPoint> path;

	std::ifstream path_file(path_file_filename);
	if (!path_file.is_open()) {
		std::cout << "path file open error\n";
		exit(2);
	}
	char comma;
	while (path_file >> left_wheel >> comma >> right_wheel) {
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
		path.push_back(PathPoint(robot_x, robot_y, hdg));
	}
	path_file.close();
	return path;
}
