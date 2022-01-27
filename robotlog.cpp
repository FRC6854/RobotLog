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

#include <cassert>
#include <fstream>
#include <gtkmm.h>
#include <iostream>

#include "pathreader.hpp"
#include "robotlog.hpp"

// initialize the window
RobotLog::RobotLog() {
	// window properties
	set_default_size(1456, 684 + 100);
	set_resizable(false);
	set_title("RobotLog");
	// load background image
	Glib::RefPtr<Gdk::Pixbuf> tmp_pixbuf = Gdk::Pixbuf::create_from_file("res/2021-field.png");
	field_background_image = tmp_pixbuf->scale_simple(1456, 684, Gdk::InterpType::INTERP_BILINEAR);
	// get drawing area ready
	field_area.set_size_request(1456, 684);
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
	// csv filter for replay sensor file dialog
	csv_filter = Gtk::FileFilter::create();
	csv_filter->set_name("CSV File (Replay sensor file)");
	csv_filter->add_pattern("*.csv");
	csv_filter->add_mime_type("test/csv");
	// choose path file button
	choose_pathfile.set_width_chars(30);
	choose_pathfile.add_filter(csv_filter);
	choose_pathfile.signal_file_set().connect(sigc::mem_fun(*this, &RobotLog::choose_path_file));
	// about dialog
	std::vector<Glib::ustring> authors;
	authors.push_back("Jason Xu");
	authors.push_back("Omar El-Sawy");
	about_dialog.set_program_name("RobotLog");
	about_dialog.set_version("2022-Alpha");
	about_dialog.set_comments("A simple FRC path planing and replay tool");
	about_dialog.set_authors(authors);
	about_dialog.set_website_label("FRC 6854 - Viking Robotics");
	about_dialog.set_website("https://team6854.com");
	about_dialog.set_license_type(Gtk::LICENSE_GPL_3_0);
	about_dialog.set_copyright("Copyright (c) 2020-2022 FRC 6854 - Viking Robotics");
	// mode selection combobox
	combobox_mode_select.append("Planning Mode");
	combobox_mode_select.append("Log Viewing Mode");
	combobox_mode_select.signal_changed().connect(sigc::mem_fun(*this, &RobotLog::mode_changed));
	combobox_mode_select.set_active(0);
	// background selection combobox
	combobox_background_select.append("2018");
	combobox_background_select.append("2019");
	combobox_background_select.append("2020");
	combobox_background_select.append("2021");
	combobox_background_select.append("2022");
	combobox_background_select.signal_changed().connect(
		sigc::mem_fun(*this, &RobotLog::background_select_changed));
	combobox_background_select.set_active(4);
	// add all widget to window
	box.put(field_area, 0, 0);
	box.put(button_start, 10, 700);
	box.put(button_pause, 120, 700);
	box.put(button_reset, 200, 700);
	box.put(button_undo, 10, 700);
	box.put(button_export, 100, 700);
	box.put(button_gohome, 200, 700);
	box.put(button_deploy, 450, 700);
	box.put(combobox_startup, 300, 700);
	box.put(label_time, 450, 710);
	box.put(choose_pathfile, 700, 700);
	box.put(button_about, 10, 740);
	box.put(combobox_mode_select, 500, 740);
	box.put(combobox_background_select, 600, 700);
	add(box);
	show_all_children();
	// start with planning mode
	button_start.hide();
	button_pause.hide();
	button_reset.hide();
	label_time.hide();
	choose_pathfile.hide();
	// path startup point
	plan_path.push_back(PathPoint(60, 315, 0));
	// 20ms timeout timer
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &RobotLog::on_timeout), 20);
	// set field area click limit and displayment
	field_cursor_top_limit = 60;
	field_cursor_bottom_limit = 684 + 60;
	field_cursor_disp_x = -25;
	field_cursor_disp_y = -60;
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
		if (!plan_path.empty()) {
			plan_path[0].x = 60;
			plan_path[0].y = 315;
		}
		break;
	case 1:
		// blue 3
		log_start_x = 60;
		log_start_y = 505;
		log_start_hdg = 0;
		if (!plan_path.empty()) {
			plan_path[0].x = 60;
			plan_path[0].y = 505;
		}
		break;
	case 2:
		// red 1
		log_start_x = 1580;
		log_start_y = 315;
		log_start_hdg = 180;
		if (!plan_path.empty()) {
			plan_path[0].x = 1580;
			plan_path[0].y = 315;
		}
		break;
	case 3:
		// red 3
		log_start_x = 1580;
		log_start_y = 505;
		log_start_hdg = 180;
		if (!plan_path.empty()) {
			plan_path[0].x = 1580;
			plan_path[0].y = 505;
		}
		break;
	}
	if (!choose_pathfile.get_filename().empty()) {
		StevePathReader steve_path_reader;
		log_path
			= steve_path_reader.read_path(path_file_path, log_start_x, log_start_y, log_start_hdg);
	}
	field_area.queue_draw();
}

// drawing field area
bool RobotLog::field_area_ondraw(const ::Cairo::RefPtr<::Cairo::Context>& cr) {
	// draw background
	Gdk::Cairo::set_source_pixbuf(cr, field_background_image, 0, 0);
	cr->paint();
	// draw planning lines
	if (op_mode == OpMode::planning) {
		if (plan_path.size() > 1) {
			cr->set_source_rgb(0.8, 0.0, 0.0);
			cr->set_line_width(10);
			for (unsigned int i = 0; i < plan_path.size() - 1; i++) {
				cr->move_to(plan_path[i].x, plan_path[i].y);
				cr->line_to(plan_path[i + 1].x, plan_path[i + 1].y);
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
	for (unsigned int i = 0; i < log_path.size() - 1; i++) {
		if (i == playtime) {
			cr->stroke();
			cr->set_source_rgb(0.0, 0.8, 0.0);
		}
		cr->move_to(log_path[i].x, log_path[i].y);
		cr->line_to(log_path[i + 1].x, log_path[i + 1].y);
	}
	cr->stroke();
	// draw robot
	cr->set_source_rgb(0.0, 0.0, 0.5);
	cr->translate(log_path[playtime].x, log_path[playtime].y);
	cr->rotate_degrees(log_path[playtime].hdg);
	cr->rectangle(-35, -30, 70, 60);
	cr->fill();
	// only update playtime when playing, not pausing
	if (playing == true) {
		playtime++;
	}
	// reset after finished playing
	if (playtime == log_path.size()) {
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

void RobotLog::background_select_changed() {
	Glib::RefPtr<Gdk::Pixbuf> tmp_pixbuf;
	switch (combobox_background_select.get_active_row_number()) {
	case 0:
		tmp_pixbuf = Gdk::Pixbuf::create_from_file("res/2018-field.jpg");
		break;
	case 1:
		tmp_pixbuf = Gdk::Pixbuf::create_from_file("res/2019-field.jpg");
		break;
	case 2:
		tmp_pixbuf = Gdk::Pixbuf::create_from_file("res/2020-field.png");
		break;
	case 3:
		tmp_pixbuf = Gdk::Pixbuf::create_from_file("res/2021-field.png");
		break;
	case 4:
		tmp_pixbuf = Gdk::Pixbuf::create_from_file("res/2022-field.png");
		break;
	}
	field_background_image = tmp_pixbuf->scale_simple(1456, 684, Gdk::InterpType::INTERP_BILINEAR);
	field_area.queue_draw();
}
