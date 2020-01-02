/*
Copyright (c) 2020 FRC 6854 - Viking Robotics

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

#include "robotlog.hpp"
#include <gtkmm.h>

int main() {
	auto app = Gtk::Application::create();
	RobotLog robotlog;
	robotlog.set_default_size(1646, 823 + 100);
	robotlog.set_resizable(false);
	robotlog.set_title("RobotLog");
	app->run(robotlog);
}
