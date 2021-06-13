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

#include "robotlog.hpp"

void RobotLog::start_button_clicked() {
	playing = true;
	button_start.set_label("Start");
}

void RobotLog::pause_button_clicked() {
	playing = false;
	button_start.set_label("Continue");
}

void RobotLog::reset_button_clicked() {
	playing = false;
	playtime = 0;
	button_start.set_label("Start");
}

// 20ms timer timeout event
bool RobotLog::on_timeout() {
	// update time label
	label_time.set_text("Time: " + std::to_string(playtime / 50) + ":" +
						std::to_string(playtime % 50 * 20));
	field_area.queue_draw(); // drawing
	return true;
}

// a path file is selected
void RobotLog::choose_path_file() {
	path_file_path = choose_pathfile.get_filename();
	startup_changed();
	path_ready = true;
}
