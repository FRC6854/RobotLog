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
#include <iostream>

#include "robotlog.hpp"

bool RobotLog::field_clicked(GdkEventButton* button_event) {
	if ((button_event->y < 60) || (button_event->y > 823 + 60)) {
		return false;
	}
	if (op_mode == OpMode::logview) {
		return true;
	}
	path_pointer++;
	path_point[path_pointer].x = button_event->x - 30;
	path_point[path_pointer].y = button_event->y - 60;
	field_area.queue_draw();
	return true;
}

void RobotLog::undo_button_clicked() {
	path_pointer--;
	if (path_pointer < 0) {
		path_pointer = 0;
	}
}

void RobotLog::export_button_clicked() {
	auto choose_save =
		Gtk::FileChooserNative::create("Save path CSV file", Gtk::FILE_CHOOSER_ACTION_SAVE);
	choose_save->add_filter(csv_filter);
	int choose_save_result = choose_save->run();
	std::string out_filename;
	switch (choose_save_result) {
	case Gtk::RESPONSE_ACCEPT:
		out_filename = choose_save->get_filename();
		break;
	default:
		return;
	}
	export_path(out_filename);
}

void RobotLog::gohome_button_clicked() {
	path_pointer++;
	path_point[path_pointer].x = log_start_x;
	path_point[path_pointer].y = log_start_y;
	field_area.queue_draw();
}

void RobotLog::deploy_button_clicked() {
	if (robot_ip == "") {
		Gtk::MessageDialog ip_dialog("Enter Robot IP address");
		Gtk::Entry ip_box;
		ip_dialog.get_vbox()->pack_start(ip_box, Gtk::PACK_SHRINK);
		ip_dialog.show_all();
		ip_dialog.run();
		robot_ip = ip_box.get_text();
	}
	Gtk::MessageDialog filename_dialog("Enter path file name");
	Gtk::Entry filename_box;
	filename_dialog.get_vbox()->pack_start(filename_box, Gtk::PACK_SHRINK);
	filename_dialog.show_all();
	filename_dialog.run();
	std::string robot_path_name = filename_box.get_text();
	std::string tmp_path_name = Glib::get_tmp_dir() + "/" + robot_path_name;
	export_path(tmp_path_name);
	int scp_status = system(
		("scp -o ConnectTimeout=5 " + tmp_path_name + " lvuser@" + robot_ip + ":" + robot_path_name)
			.c_str());
	if (scp_status == 0) {
		Gtk::MessageDialog success_dialog("Deploy successful to ~/" + robot_path_name);
		success_dialog.run();
	} else {
		Gtk::MessageDialog fail_dialog("Deploy failed, scp returned " + std::to_string(scp_status));
		fail_dialog.run();
	}
	std::remove(tmp_path_name.c_str());
}

void RobotLog::export_path(const std::string& filename) {
	std::ofstream outcsv(filename);
	for (int i = 0; i <= path_pointer; i++) {
		if (log_start_hdg == 0) {
			outcsv << path_point[i].x - log_start_x << "," << path_point[i].y - log_start_y << "\n";
		} else if (log_start_hdg == 180) {
			outcsv << log_start_x - path_point[i].x << "," << log_start_y - path_point[i].y << "\n";
		}
	}
	outcsv.close();
}
