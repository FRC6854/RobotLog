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

#include <fstream>
#include <string>

#include "VikingInstructionExport.hpp"

VikingInstructionExport::VikingInstructionExport() : opened(false) {}

VikingInstructionExport::VikingInstructionExport(const char *filename) : ofs(filename) {
	opened = ofs.is_open();
}

VikingInstructionExport::VikingInstructionExport(const std::string& filename) : ofs(filename) {
	opened = ofs.is_open();
}

VikingInstructionExport::~VikingInstructionExport() {
	if (opened) {
		ofs.close();
	}
}

void VikingInstructionExport::open(const char *filename) {
	if (!opened) {
		ofs.open(filename);
		opened = ofs.is_open();
	}
}

void VikingInstructionExport::open(const std::string& filename) {
	if (!opened) {
		ofs.open(filename);
		opened = ofs.is_open();
	}
}

void VikingInstructionExport::close() {
	if (opened) {
		ofs.close();
		opened = false;
	}
}

void VikingInstructionExport::append(double x, double y) {
	if (opened) {
		ofs << pastBadge << " translate " << x << " " << y << '\n';
	}
}

const char *VikingInstructionExport::get_type_name() {
	return "Viking Instruction File";
}

const char *VikingInstructionExport::get_file_ext() {
	return "vkg";
}
