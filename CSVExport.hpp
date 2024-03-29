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

#ifndef CSVEXPORT_HPP
#define CSVEXPORT_HPP

#include <fstream>
#include <string>

#include "PathExport.hpp"

class CSVExport : public PathExport {
private:
	std::ofstream ofs;
	bool opened;

public:
	CSVExport();
	CSVExport(const char *filename);
	CSVExport(const std::string& filename);
	~CSVExport();
	virtual void open(const char *filename) override;
	virtual void open(const std::string& filename) override;
	virtual void close() override;
	virtual void append(double x, double y) override;
	virtual const char *get_type_name() override;
	virtual const char *get_file_ext() override;
};

#endif
