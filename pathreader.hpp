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

#ifndef PATHREADER_HPP
#define PATHREADER_HPP

#include <numbers>
#include <string>
#include <vector>

struct PathPoint {
	double x, y, hdg;

	PathPoint(double _x, double _y, double _hdg) {
		x = _x;
		y = _y;
		hdg = _hdg;
	}
};

constexpr double deg2rad(double deg) {
	return deg * std::numbers::pi_v<double> / 180.0;
}

constexpr double rad2deg(double rad) {
	return rad / std::numbers::pi_v<double> * 180.0;
}

class PathReader {
public:
	virtual std::vector<PathPoint> read_path(const std::string& path_file_filename, double init_x,
											 double init_y, double init_hdg)
		= 0;
};

class StevePathReader : public PathReader {
public:
	std::vector<PathPoint> read_path(const std::string& path_file_filename, double init_x,
									 double init_y, double init_hdg) override;
};

#endif
