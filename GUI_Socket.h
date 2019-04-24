#ifndef GTKMM_EX_GUI_H
#define GTKMM_EX_GUI_H

#include <gtkmm.h>

class Gui_Ex : public Gtk::Window
{
public:
    Gui_Ex();
    virtual ~Gui_Ex();

protected:
    // Signal handlers:
    // new button
    void on_connect();
  void on_spin();

  //void on_spinbutton_digits_changed();

    //child widgets
  //Gtk::Frame m_Frame_Init, m_Frame_Control;
  Glib::RefPtr<Gtk::Adjustment> m_adjustment;
  Gtk::Label m_label;
  Gtk::Grid m_grid;

  Gtk::SpinButton m_spin;
    Gtk::Button m_button2, m_button3; //m_connect;



};

#endif // GTKMM_EX_GUI_H
