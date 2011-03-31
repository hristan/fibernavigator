#ifndef MYLISTCTRL_H_
#define MYLISTCTRL_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/listctrl.h"
#include "wx/treectrl.h"
#include "wx/imaglist.h"

class MyTreeCtrl: public wxTreeCtrl {
public:
	MyTreeCtrl(wxWindow *parent, const wxWindowID id, const wxPoint& pos,
			const wxSize& size, long style);

private:
	void OnChar(wxKeyEvent& event);
	void OnRightClick(wxMouseEvent& event);
	void OnToggleAndNot(wxCommandEvent& event);
	void OnDeleteBox(wxCommandEvent& event);
	int getSelectedType();

	DECLARE_EVENT_TABLE()
}	;

class MyListCtrl: public wxListCtrl {
public:
	MyListCtrl(wxWindow *parent, const wxWindowID id, const wxPoint& pos,
			const wxSize& size, long style);

	void OnLeftClick(wxMouseEvent& event);
	void OnRightClick(wxMouseEvent& event);
	int getColClicked();
	int getColActivated();
	void moveItemUp(long);
	void moveItemDown(long);

private:
	int m_col_clicked;
	int m_col_activated;
	void swap(long, long);

	DECLARE_EVENT_TABLE()
}	;

class MySlider: public wxSlider {
public:
	MySlider( wxWindow *parent, const wxWindowID id,
			  int value , int minValue, int maxValue,
			  const wxPoint& pos, const wxSize& size, long style ) :
		wxSlider (parent, id, value, minValue, maxValue, pos, size, style)
	{
		m_leftIsDown = false;
	};

	bool leftDown() {return m_leftIsDown;};
private:
	void OnMouseEvent(wxMouseEvent& event);

	bool m_leftIsDown;

	DECLARE_EVENT_TABLE()
};

#endif /*MYLISTCTRL_H_*/
