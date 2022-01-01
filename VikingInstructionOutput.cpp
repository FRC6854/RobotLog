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
#include <string>

#include "VikingInstructionOutput.hpp"

VikingInstructionOutput::VikingInstructionOutput() : opened(false) {}

VikingInstructionOutput::VikingInstructionOutput(const char *filename) : ofs(filename) {
	opened = ofs.is_open();
}

VikingInstructionOutput::VikingInstructionOutput(const std::string& filename) : ofs(filename) {
	opened = ofs.is_open();
}

VikingInstructionOutput::~VikingInstructionOutput() {
	if (opened) {
		ofs.close();
	}
}

void VikingInstructionOutput::open(const char *filename) {
	if (!opened) {
		ofs.open(filename);
		opened = ofs.is_open();
	}
}

void VikingInstructionOutput::open(const std::string& filename) {
	if (!opened) {
		ofs.open(filename);
		opened = ofs.is_open();
	}
}

void VikingInstructionOutput::close() {
	if (opened) {
		ofs.close();
		opened = false;
	}
}

void VikingInstructionOutput::append(double x, double y) {
	if (opened) {
		ofs << pastBadge << " translate " << x << " " << y << '\n';
	}
}

const char *VikingInstructionOutput::get_type_name() {
	return "VKG file";
}

const char *VikingInstructionOutput::get_file_ext() {
	return "vkg";
}
