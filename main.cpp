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

#include <cstddef>
#include <cstring>
#include <gtkmm.h>

#include "RobotLog.hpp"

const char *gdk_backends_need_cursor_fix[] = {"GdkX11Display", "GdkWin32Display"};

int main() {
	auto app = Gtk::Application::create();
	RobotLog robotlog;
	// check if GDK backend need cursor position fixup
	for (size_t i = 0;
		 i < (sizeof(gdk_backends_need_cursor_fix) / sizeof(gdk_backends_need_cursor_fix[0]));
		 i++) {
		if (strcmp(G_OBJECT_TYPE_NAME(robotlog.get_display()->gobj()),
				   gdk_backends_need_cursor_fix[i])
			== 0) {
			// set cursor displayment and bound
			robotlog.field_cursor_top_limit = 0;
			robotlog.field_cursor_bottom_limit = 684;
			robotlog.field_cursor_disp_x = 0;
			robotlog.field_cursor_disp_y = 0;
			break;
		}
	}
	app->run(robotlog);
}
