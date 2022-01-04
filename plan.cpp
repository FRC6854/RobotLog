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

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "VikingInstructionExport.hpp"
#include "csvexport.hpp"
#include "pathexport.hpp"
#include "pathreader.hpp"
#include "robotlog.hpp"

bool RobotLog::field_clicked(GdkEventButton *button_event) {
	if ((button_event->y < field_cursor_top_limit)
		|| (button_event->y >= field_cursor_bottom_limit)) {
		return false;
	}
	if (op_mode == OpMode::logview) {
		return true;
	}
	plan_path.push_back(
		PathPoint(button_event->x + field_cursor_disp_x, button_event->y + field_cursor_disp_y, 0));
	field_area.queue_draw();
	return true;
}

void RobotLog::undo_button_clicked() {
	plan_path.pop_back();
}

void RobotLog::export_button_clicked() {
	auto choose_save
		= Gtk::FileChooserNative::create("Save path CSV file", Gtk::FILE_CHOOSER_ACTION_SAVE);

	CSVExport csv_export;
	VikingInstructionExport viking_export;
	std::vector<Glib::RefPtr<Gtk::FileFilter>> export_file_filter;
	export_file_filter.push_back(Gtk::FileFilter::create());
	export_file_filter[0]->set_name(std::string(csv_export.get_type_name()) + " (*."
									+ csv_export.get_file_ext() + ")");
	export_file_filter[0]->add_pattern(std::string("*.") + csv_export.get_file_ext());
	export_file_filter[0]->add_mime_type("text/csv");
	export_file_filter.push_back(Gtk::FileFilter::create());
	export_file_filter[1]->set_name(std::string(viking_export.get_type_name()) + " (*."
									+ viking_export.get_file_ext() + ")");
	export_file_filter[1]->add_pattern(std::string("*.") + viking_export.get_file_ext());
	for (const Glib::RefPtr<Gtk::FileFilter>& ff : export_file_filter) {
		choose_save->add_filter(ff);
	}

	int choose_save_result = choose_save->run();
	if (choose_save_result == Gtk::RESPONSE_ACCEPT) {
		std::string out_filename = choose_save->get_filename();
		if (choose_save->get_filter() == export_file_filter[0]) {
			csv_export.open(out_filename);
			export_path(csv_export);
			csv_export.close();
		} else if (choose_save->get_filter() == export_file_filter[1]) {
			viking_export.open(out_filename);
			export_path(viking_export);
			viking_export.close();
		}
	}
}

void RobotLog::gohome_button_clicked() {
	plan_path.push_back(PathPoint(log_start_x, log_start_y, 0));
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
	CSVExport csv_export(tmp_path_name);
	export_path(csv_export);
	csv_export.close();
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

void RobotLog::export_path(PathExport& exporter) {
	for (const PathPoint& pp : plan_path) {
		assert((log_start_hdg == 0) || (log_start_hdg == 180));
		if (log_start_hdg == 0) {
			exporter.append(pp.x - log_start_x, pp.y - log_start_y);
		} else if (log_start_hdg == 180) {
			exporter.append(log_start_x - pp.x, log_start_y - pp.y);
		}
	}
}
