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

#include <cstdio>

#include "PipeStream.hpp"

PipeStream::PipeStream(std::string command) : opened(false) {
	pipe_stream = popen(command.c_str(), "w");
	if (pipe_stream) {
		opened = true;
	}
}

PipeStream::~PipeStream() {
	close();
}

bool PipeStream::is_open() {
	return opened;
}

void PipeStream::close() {
	if (opened) {
		pclose(pipe_stream);
		opened = false;
	}
}

void PipeStream::write(std::string s) {
	if (opened) {
		fputs(s.c_str(), pipe_stream);
	}
}
