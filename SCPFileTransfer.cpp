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

#include "SCPFileTransfer.hpp"

std::string SCPFileTransfer::remote_path(const std::string& path) {
	return target_username + "@" + target_hostname + ":" + path;
}

SCPFileTransfer::SCPFileTransfer(const std::string& hostname, const std::string& username,
								 unsigned int port)
	: target_hostname(hostname), target_username(username), port(port) {}

bool SCPFileTransfer::ssh_test() {
	return std::system(("ssh -T " + target_username + "@" + target_hostname).c_str());
}

int SCPFileTransfer::put(const std::string& local_path, const std::string& target_path) {
	return std::system(("scp -o ConnectTimeout=3 -P " + std::to_string(port) + " " + local_path
						+ " " + remote_path(target_path))
						   .c_str());
}

int SCPFileTransfer::get(const std::string& local_path, const std::string& target_path) {
	return std::system(("scp -o ConnectTimeout=3 -P " + std::to_string(port) + " "
						+ remote_path(target_path) + " " + local_path)
						   .c_str());
}
