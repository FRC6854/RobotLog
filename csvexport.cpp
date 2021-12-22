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

#include "csvexport.hpp"

CSVExport::CSVExport() : opened(false) {}

CSVExport::CSVExport(const char *filename) : ofs(filename) {
	opened = ofs.is_open();
}

CSVExport::CSVExport(const std::string& filename) : ofs(filename) {
	opened = ofs.is_open();
}

CSVExport::~CSVExport() {
	if (opened) {
		ofs.close();
	}
}

void CSVExport::open(const char *filename) {
	if (!opened) {
		ofs.open(filename);
		opened = ofs.is_open();
	}
}

void CSVExport::open(const std::string& filename) {
	if (!opened) {
		ofs.open(filename);
		opened = ofs.is_open();
	}
}

void CSVExport::close() {
	if (opened) {
		ofs.close();
		opened = false;
	}
}

void CSVExport::append(double x, double y) {
	if (opened) {
		ofs << x << ',' << y << '\n';
	}
}

const char *CSVExport::get_type_name() {
	return "CSV file";
}

const char *CSVExport::get_file_ext() {
	return "csv";
}
