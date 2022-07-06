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

#include <cstdlib>
#include <string>

#include "SFTPFileTransfer.hpp"

SFTPFileTransfer::SFTPFileTransfer(const std::string& hostname, const std::string& username)
	: target_hostname(hostname), target_username(username) {}

int SFTPFileTransfer::put(const std::string& local_path, const std::string& target_path) {
	return 0;
}

int SFTPFileTransfer::get(const std::string& local_path, const std::string& target_path) {
	return 0;
}
