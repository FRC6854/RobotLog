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

#ifndef ROBOTLOG_HPP
#define ROBOTLOG_HPP

#include <gtkmm.h>

struct point {
	double x, y;
};

double deg2rad(double deg);

class RobotLog : public Gtk::Window {
public:
	RobotLog();
	void make_path(double robot_x, double robot_y, double hdg, std::string path_file_filename);

	std::string path_file_path;
	std::string robot_ip;
	bool path_ready = false;
	bool playing = false;
	int playtime = 0;
	int robot_data_size;
	double robot_path_x[10000];
	double robot_path_y[10000];
	double robot_path_hdg[10000];
	point path_point[100];
	int path_pointer = 0;
	double log_start_x, log_start_y, log_start_hdg;

protected:
	enum class OpMode { planning, logview };
	OpMode op_mode = OpMode::planning;

	// signals
	void start_button_clicked();
	void pause_button_clicked();
	void reset_button_clicked();
	void undo_button_clicked();
	void export_button_clicked();
	void gohome_button_clicked();
	void deploy_button_clicked();
	void startup_changed();
	bool on_timeout();
	bool field_area_ondraw(const ::Cairo::RefPtr<::Cairo::Context>& cr);
	void choose_path_file();
	void display_about_dialog();
	void mode_changed();
	void export_path(const std::string& filename);
	bool field_clicked(GdkEventButton* button_event);

	Glib::RefPtr<Gdk::Pixbuf> field_background_image;
	Glib::RefPtr<Gtk::FileFilter> csv_filter;
	// widgets
	Gtk::Fixed box;
	Gtk::DrawingArea field_area;
	Gtk::Button button_start;
	Gtk::Button button_pause;
	Gtk::Button button_reset;
	Gtk::Button button_undo;
	Gtk::Button button_export;
	Gtk::Button button_gohome;
	Gtk::Button button_deploy;
	Gtk::Button button_about;
	Gtk::ComboBoxText combobox_startup;
	Gtk::Label label_time;
	Gtk::FileChooserButton choose_pathfile;
	Gtk::AboutDialog about_dialog;
	Gtk::ComboBoxText combobox_mode_select;
};

#endif // ROBOTLOG_HPP
