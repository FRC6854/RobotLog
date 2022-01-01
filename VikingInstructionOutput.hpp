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

#ifndef VIKINGINSTRUCTIONOUTPUT_HPP
#define VIKINGINSTRUCTIONOUTPUT_HPP

#include <fstream>
#include <string>

#include "pathexport.hpp"

class VikingInstructionOutput : public PathExport {
private:
	std::ofstream ofs;
	bool opened;

public:
	const int pastBadge = 0;
	char new_x, new_y;
	VikingInstructionOutput();
	VikingInstructionOutput(const char *filename);
	VikingInstructionOutput(const std::string& filename);
	~VikingInstructionOutput();
	virtual void open(const char *filename) override;
	virtual void open(const std::string& filename) override;
	virtual void close() override;
	virtual void append(double x, double y) override;
	virtual const char *get_type_name() override;
	virtual const char *get_file_ext() override;
};

#endif
