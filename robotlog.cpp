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
#include <fstream>
#include <gtkmm.h>
#include <iostream>

using namespace std;

// degrees to radians
double deg2rad(double deg) {
	return deg * M_PI / 180.0;
}

// initialize the window
RobotLog::RobotLog() {
	// load background image
	field_background_image = Gdk::Pixbuf::create_from_file("res/frc-field-2019.jpg");
	// get drawing area ready
	field_area.set_size_request(1646, 823);
	RobotLog::signal_button_press_event().connect(sigc::mem_fun(*this, &RobotLog::field_clicked));
	field_area.signal_draw().connect(sigc::mem_fun(*this, &RobotLog::field_area_ondraw));
	// start button
	button_start.add_label("Start");
	button_start.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::start_button_clicked));
	// pause button
	button_pause.add_label("Pause");
	button_pause.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::pause_button_clicked));
	// reset button
	button_reset.set_label("Reset");
	button_reset.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::reset_button_clicked));
	// undo button
	button_undo.set_label("Undo");
	button_undo.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::undo_button_clicked));
	// export_button
	button_export.set_label("Export");
	button_export.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::export_button_clicked));
	// go home button
	button_gohome.set_label("Go Home");
	button_gohome.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::gohome_button_clicked));
	// path deploy button
	button_deploy.set_label("Deploy Path");
	button_deploy.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::deploy_button_clicked));
	// startup location selection combobox
	combobox_startup.append("Blue 1");
	combobox_startup.append("Blue 3");
	combobox_startup.append("Red 1");
	combobox_startup.append("Red 3");
	combobox_startup.signal_changed().connect(sigc::mem_fun(*this, &RobotLog::startup_changed));
	combobox_startup.set_active(0);
	// time label
	label_time.set_text("Time: 000.000");
	// about button
	button_about.set_label("About");
	button_about.signal_clicked().connect(sigc::mem_fun(*this, &RobotLog::display_about_dialog));
	// csv filter for file dialog
	csv_filter = Gtk::FileFilter::create();
	csv_filter->set_name("CSV File");
	csv_filter->add_pattern("*.csv");
	// choose path file button
	choose_pathfile.set_width_chars(30);
	choose_pathfile.add_filter(csv_filter);
	choose_pathfile.signal_file_set().connect(sigc::mem_fun(*this, &RobotLog::choose_path_file));
	// about dialog
	vector<Glib::ustring> authors;
	authors.push_back("Jason Xu");
	about_dialog.set_program_name("RobotLog");
	about_dialog.set_version("alpha");
	about_dialog.set_comments("A simple FRC path planing tool");
	about_dialog.set_authors(authors);
	about_dialog.set_website_label("FRC6854");
	about_dialog.set_website("https://team6854.com");
	about_dialog.set_license_type(Gtk::LICENSE_GPL_3_0);
	about_dialog.set_copyright("Copyright (c) 2019 FRC 6854 - Viking Robotics");
	// mode selection combobox
	combobox_mode_select.append("Planning Mode");
	combobox_mode_select.append("Log Viewing Mode");
	combobox_mode_select.signal_changed().connect(sigc::mem_fun(*this, &RobotLog::mode_changed));
	combobox_mode_select.set_active(0);
	// add all widget to window
	box.put(field_area, 0, 0);
	box.put(button_start, 10, 840);
	box.put(button_pause, 100, 840);
	box.put(button_reset, 200, 840);
	box.put(button_undo, 10, 840);
	box.put(button_export, 100, 840);
	box.put(button_gohome, 200, 840);
	box.put(button_deploy, 450, 840);
	box.put(combobox_startup, 300, 840);
	box.put(label_time, 450, 840);
	box.put(choose_pathfile, 600, 840);
	box.put(button_about, 10, 880);
	box.put(combobox_mode_select, 500, 880);
	add(box);
	show_all_children();
	// start with planning mode
	button_start.hide();
	button_pause.hide();
	button_reset.hide();
	label_time.hide();
	choose_pathfile.hide();
	// path startup point
	path_point[0].x = 60;
	path_point[0].y = 315;
	// 20ms timeout timer
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &RobotLog::on_timeout), 20);
}

// change startup location event
void RobotLog::startup_changed() {
	// update path cache array
	switch (combobox_startup.get_active_row_number()) {
	case 0:
		// blue 1
		log_start_x = 60;
		log_start_y = 315;
		log_start_hdg = 0;
		path_point[0].x = 60;
		path_point[0].y = 315;
		break;
	case 1:
		// blue 3
		log_start_x = 60;
		log_start_y = 505;
		log_start_hdg = 0;
		path_point[0].x = 60;
		path_point[0].y = 505;
		break;
	case 2:
		// red 1
		log_start_x = 1580;
		log_start_y = 315;
		log_start_hdg = 180;
		path_point[0].x = 1580;
		path_point[0].y = 315;
		break;
	case 3:
		// red 3
		log_start_x = 1580;
		log_start_y = 505;
		log_start_hdg = 180;
		path_point[0].x = 1580;
		path_point[0].y = 505;
		break;
	}
	if (!choose_pathfile.get_filename().empty()) {
		make_path(log_start_x, log_start_y, log_start_hdg, path_file_path);
	}
	field_area.queue_draw();
}

// drawing field area
bool RobotLog::field_area_ondraw(const ::Cairo::RefPtr<::Cairo::Context> &cr) {
	// draw background
	Gdk::Cairo::set_source_pixbuf(cr, field_background_image, 0, 0);
	cr->paint();
	// draw planning lines
	if (op_mode == OpMode::planning) {
		if (path_pointer > 0) {
			cr->set_source_rgb(0.8, 0.0, 0.0);
			cr->set_line_width(10);
			for (int i = 0; i < path_pointer; i++) {
				cr->move_to(path_point[i].x, path_point[i].y);
				cr->line_to(path_point[i + 1].x, path_point[i + 1].y);
				cr->stroke();
			}
		}
		return true;
	}
	// do not draw anything when path is not ready
	if (!path_ready) {
		return true;
	}
	// walked line inital line style
	cr->set_source_rgb(0.8, 0.0, 0.0);
	cr->set_line_width(10);
	// draw path
	for (int i = 0; i < robot_data_size - 1; i++) {
		if (i == playtime) {
			cr->stroke(); // draw passed path
			// change style of new path
			cr->set_source_rgb(0.0, 0.8, 0.0);
		}
		cr->move_to(robot_path_x[i], robot_path_y[i]);
		cr->line_to(robot_path_x[i + 1], robot_path_y[i + 1]);
	}
	cr->stroke();
	// draw robot
	cr->set_source_rgb(0.0, 0.0, 0.5);
	cr->translate(robot_path_x[playtime], robot_path_y[playtime]);
	cr->rotate_degrees(robot_path_hdg[playtime]);
	cr->rectangle(-35, -30, 70, 60);
	cr->fill();
	// only update playtime when playing, not pausing
	if (playing == true) {
		playtime++;
	}
	// reset after finished playing
	if (playtime == robot_data_size) {
		playing = false;
		playtime = 0;
	}
	return true;
}

void RobotLog::display_about_dialog() {
	about_dialog.show();
}

void RobotLog::mode_changed() {
	switch (combobox_mode_select.get_active_row_number()) {
	case 0:
		// planning mode
		op_mode = OpMode::planning;
		button_undo.show();
		button_export.show();
		button_gohome.show();
		button_deploy.show();
		button_start.hide();
		button_pause.hide();
		button_reset.hide();
		label_time.hide();
		choose_pathfile.hide();
		break;
	case 1:
		// log viewing mode
		op_mode = OpMode::logview;
		button_start.show();
		button_pause.show();
		button_reset.show();
		label_time.show();
		choose_pathfile.show();
		button_undo.hide();
		button_export.hide();
		button_gohome.hide();
		button_deploy.hide();
		break;
	}
}
