#include "mainFrame.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/laywin.h"
#include "wx/filedlg.h"
#include "wx/statbmp.h"

#include "icons/eyes.xpm"
#include "icons/delete.xpm"

DECLARE_EVENT_TYPE(wxEVT_NAVGL_EVENT, -1)
   
BEGIN_EVENT_TABLE(MainFrame, wxMDIParentFrame)
    EVT_MENU(VIEWER_ABOUT, MainFrame::OnAbout)
    EVT_SIZE(MainFrame::OnSize)
    EVT_MENU(VIEWER_QUIT, MainFrame::OnQuit)
    EVT_MENU(VIEWER_NEW, MainFrame::OnNew)
    EVT_MENU(VIEWER_LOAD, MainFrame::OnLoad)
    EVT_MOUSE_EVENTS(MainFrame::OnMouseEvent)
    /* mouse click in one of the three navigation windows */
    EVT_COMMAND(ID_GL_NAV_X, wxEVT_NAVGL_EVENT, MainFrame::OnGLEvent)
    EVT_COMMAND(ID_GL_NAV_Y, wxEVT_NAVGL_EVENT, MainFrame::OnGLEvent)
	EVT_COMMAND(ID_GL_NAV_Z, wxEVT_NAVGL_EVENT, MainFrame::OnGLEvent)
	EVT_COMMAND(ID_GL_MAIN, wxEVT_NAVGL_EVENT, MainFrame::OnGLEvent)
	/* slize selection slider moved */
	EVT_SLIDER(ID_X_SLIDER, MainFrame::OnXSliderMoved)
	EVT_SLIDER(ID_Y_SLIDER, MainFrame::OnYSliderMoved)
	EVT_SLIDER(ID_Z_SLIDER, MainFrame::OnZSliderMoved)
	/* click on toolbar button to toggle one of the 3 panes in the
	 * main GL window */ 
	EVT_MENU(VIEWER_TOGGLEVIEW1, MainFrame::OnToggleView1)
	EVT_MENU(VIEWER_TOGGLEVIEW2, MainFrame::OnToggleView2)
	EVT_MENU(VIEWER_TOGGLEVIEW3, MainFrame::OnToggleView3)
	EVT_MENU(VIEWER_TOGGLE_SELBOX, MainFrame::OnToggleSelBox)
	/* click on reload shaders button */
	EVT_MENU(VIEWER_RELOAD_SHADER, MainFrame::OnReloadShaders)
	/* list ctrl events */
	EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, MainFrame::OnActivateListItem)
	EVT_LIST_ITEM_SELECTED(LIST_CTRL, MainFrame::OnSelectListItem)
	EVT_BUTTON(ID_BUTTON_UP, MainFrame::OnListItemUp)
	EVT_BUTTON(ID_BUTTON_DOWN, MainFrame::OnListItemDown)
	EVT_SLIDER(ID_T_SLIDER, MainFrame::OnTSliderMoved)
	/* tree ctrl events */
	EVT_TREE_SEL_CHANGED(TREE_CTRL, MainFrame::OnSelectTreeItem)
END_EVENT_TABLE()

// Define my frame constructor
MainFrame::MainFrame(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
    const long style):
  wxMDIParentFrame(parent, id, title, pos, size, style)
{
	NAV_SIZE = wxMin(255,size.y/4);
	NAV_GL_SIZE = NAV_SIZE - 4;
	
	// A window to the left of the client window
	wxSashLayoutWindow* win = new wxSashLayoutWindow(this, wxID_ANY, 
	  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE*4),
	  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(150 + NAV_SIZE, 1020));
    win->SetOrientation(wxLAYOUT_VERTICAL);
    win->SetAlignment(wxLAYOUT_LEFT);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_leftWindowHolder = win;
	
    // Window to hold the tree widget and nav windows
    win = new wxSashLayoutWindow(m_leftWindowHolder, wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
      		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(150 + NAV_SIZE, 3*NAV_SIZE + 60));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_leftWindowTop = win; 
    
    // Window to hold the list ctrl widget
    win = new wxSashLayoutWindow(m_leftWindowHolder, wxID_ANY, 
          		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
          		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(150 + NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_leftWindowBottom = win;
    
    win = new wxSashLayoutWindow(m_leftWindowBottom, wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
      		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(150 + NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_leftWindowBottom1 = win;
    
    win = new wxSashLayoutWindow(m_leftWindowBottom, wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
      		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(150 + NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(255, 255, 255));
    m_leftWindowBottom2 = win;
    
    wxButton *button = new wxButton(m_leftWindowBottom2, ID_BUTTON_UP, wxT("up"), wxPoint(0,2), wxSize(50,19));
    button->SetFont(wxFont(6, wxDEFAULT, wxNORMAL, wxNORMAL));
    button = new wxButton(m_leftWindowBottom2, ID_BUTTON_DOWN, wxT("down"), wxPoint(50,2), wxSize(50,19));
    button->SetFont(wxFont(6, wxDEFAULT, wxNORMAL, wxNORMAL));

    m_tSlider = new wxSlider(m_leftWindowBottom2, ID_T_SLIDER, 30, 0, 100, 
        		wxPoint(100,2), wxSize(150, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS);

    
    win = new wxSashLayoutWindow(m_leftWindowTop, wxID_ANY,
                               wxDefaultPosition, wxSize(150, 30),
                               wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
	win->SetDefaultSize(wxSize(150, 3*NAV_SIZE));
	win->SetOrientation(wxLAYOUT_VERTICAL);
	win->SetAlignment(wxLAYOUT_LEFT);
	win->SetBackgroundColour(wxColour(255, 255, 255));
	win->SetSashVisible(wxSASH_RIGHT, false);
	m_leftWindow = win;
	
	// navigation window with three sub windows for gl widgets 
    win = new wxSashLayoutWindow(m_leftWindowTop, wxID_ANY, 
  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE*4),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE*4));
    win->SetOrientation(wxLAYOUT_VERTICAL);
    win->SetAlignment(wxLAYOUT_LEFT);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_navWindow = win;

    // main window right side, holds the big gl widget
    win = new wxSashLayoutWindow(this, wxID_ANY, 
  		  wxDefaultPosition, wxSize(200, 30),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(765, 765));
    win->SetMinSize(wxSize(100,100));
    win->SetMaxSize(wxSize(2000,2000));
    win->SetOrientation(wxLAYOUT_VERTICAL);
    win->SetAlignment(wxLAYOUT_LEFT);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_rightWindowHolder = win;
    
    // main window right side, holds the big gl widget
    win = new wxSashLayoutWindow(m_rightWindowHolder, wxID_ANY, 
  		  wxDefaultPosition, wxSize(200, 30),
  		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(700, 700));
    win->SetMinSize(wxSize(100,100));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(180, 180, 180));
    m_rightWindow = win;
    
    // extra window right side, prevent scaling of the main gl widget
    win = new wxSashLayoutWindow(m_rightWindowHolder, wxID_ANY, 
  		  wxDefaultPosition, wxSize(765, 765),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(765, 1));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_BOTTOM);
    win->SetBackgroundColour(wxColour(255, 255, 255));
    m_extraRightWindow = win;

    win = new wxSashLayoutWindow(m_navWindow, wxID_ANY, 
  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
  		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_topNavWindow = win;
    
    win = new wxSashLayoutWindow(m_navWindow, wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, 20),
      		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        win->SetDefaultSize(wxSize(NAV_SIZE, 20));
        win->SetOrientation(wxLAYOUT_HORIZONTAL);
        win->SetAlignment(wxLAYOUT_TOP);
        win->SetBackgroundColour(wxColour(255, 255, 255));
    
    m_zSlider = new wxSlider(win, ID_Z_SLIDER, 50, 0, 100, 
        		wxPoint(0, 0), wxSize(NAV_SIZE, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS);
    
    win = new wxSashLayoutWindow(m_navWindow, wxID_ANY, 
  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
  		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_middleNavWindow = win;

    win = new wxSashLayoutWindow(m_navWindow, wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, 20),
      		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        win->SetDefaultSize(wxSize(NAV_SIZE, 20));
        win->SetOrientation(wxLAYOUT_HORIZONTAL);
        win->SetAlignment(wxLAYOUT_TOP);
        win->SetBackgroundColour(wxColour(255, 255, 255));
    
    m_ySlider = new wxSlider(win, ID_Y_SLIDER, 50, 0, 100, wxPoint(0,0), 
        		wxSize(NAV_SIZE, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS);
    
    win = new wxSashLayoutWindow(m_navWindow, wxID_ANY, 
     		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
     		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
       win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
       win->SetOrientation(wxLAYOUT_HORIZONTAL);
       win->SetAlignment(wxLAYOUT_TOP);
       win->SetBackgroundColour(wxColour(0, 0, 0));
    m_bottomNavWindow = win;
    
    win = new wxSashLayoutWindow(m_navWindow, wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, 20),
      		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
        win->SetDefaultSize(wxSize(NAV_SIZE, 20));
        win->SetOrientation(wxLAYOUT_HORIZONTAL);
        win->SetAlignment(wxLAYOUT_TOP);
        win->SetBackgroundColour(wxColour(255, 255, 255));
    
    m_xSlider = new wxSlider(win, ID_X_SLIDER, 50, 0, 100, wxPoint(0,0), 
            		wxSize(NAV_SIZE, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS);
    
    // extra window to avoid scaling of the bottom gl widget when resizing
    win = new wxSashLayoutWindow(m_navWindow,wxID_ANY, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
       		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
          //win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
          win->SetOrientation(wxLAYOUT_HORIZONTAL);
          win->SetAlignment(wxLAYOUT_TOP);
          win->SetBackgroundColour(wxColour(255, 255, 255));
    m_extraNavWindow = win;

    m_xSlider->SetMinSize(wxSize(1, -1));
    m_ySlider->SetMinSize(wxSize(1, -1));
    m_zSlider->SetMinSize(wxSize(1, -1));
    
    m_datasetListCtrl = new MyListCtrl(m_leftWindowBottom1, LIST_CTRL, wxDefaultPosition, 
    		wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);
    
    wxImageList* imageList = new wxImageList(16,16);
    imageList->Add(wxIcon(eyes_xpm));
    imageList->Add(wxIcon(delete_xpm));
    m_datasetListCtrl->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
    
    wxListItem itemCol;
    itemCol.SetText(wxT(""));
    m_datasetListCtrl->InsertColumn(0, itemCol);
        
    itemCol.SetText(wxT("Name"));
    itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
    m_datasetListCtrl->InsertColumn(1, itemCol);
    
    itemCol.SetText(wxT("Threshold"));
    itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
    m_datasetListCtrl->InsertColumn(2, itemCol);
    
    itemCol.SetText(wxT(""));
    m_datasetListCtrl->InsertColumn(3, itemCol);
        
    m_treeWidget = new wxTreeCtrl(m_leftWindow, TREE_CTRL, wxPoint(0, 0), 
    		wxDefaultSize, wxTR_HAS_BUTTONS|wxTR_SINGLE|wxTR_HIDE_ROOT|wxTR_HAS_BUTTONS);
    m_treeWidget->AssignImageList(imageList);
    
    m_tRootId = m_treeWidget->AddRoot(wxT("Root"));
    m_tPlanesId = m_treeWidget->AppendItem(m_tRootId, wxT("planes"));
	    m_tAxialId    = m_treeWidget->AppendItem(m_tPlanesId, wxT("axial"));
	    m_tCoronalId  = m_treeWidget->AppendItem(m_tPlanesId, wxT("coronal"));
	    m_tSagittalId = m_treeWidget->AppendItem(m_tPlanesId, wxT("sagittal"));
    m_tDatasetId = m_treeWidget->AppendItem(m_tRootId, wxT("datasets"));
    m_tMeshId = m_treeWidget->AppendItem(m_tRootId, wxT("meshes"));
    m_tFiberId = m_treeWidget->AppendItem(m_tRootId, wxT("fibers"));
    m_tPointId  = m_treeWidget->AppendItem(m_tRootId, wxT("points"));
    
    /*
     * Set OpenGL attributes 
     */
    GLboolean doubleBuffer = GL_TRUE;
    #ifdef __WXMSW__
    	int *gl_attrib = NULL;
	#else
    int gl_attrib[20] = { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
        WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
        WX_GL_DOUBLEBUFFER,
	#if defined(__WXMAC__) || defined(__WXCOCOA__)
        GL_NONE };
	#else
        None };
	#endif
	#endif
    if(!doubleBuffer)
    {
        printf("don't have double buffer, disabling\n");
	#ifdef __WXGTK__
        gl_attrib[9] = None;
	#endif
        doubleBuffer = GL_FALSE;
    }
    
    
    TheDataset::columns = 1;
    TheDataset::rows = 1;
    TheDataset::frames = 1;
    TheDataset::lastError = wxT("");
    m_scene = new TheScene();
    m_scene->setDataListCtrl(m_datasetListCtrl);
    
    m_mainGL = new MainCanvas(m_scene, mainView, m_rightWindow, ID_GL_MAIN, wxDefaultPosition,
        			wxDefaultSize, 0, _T("MainGLCanvas"), gl_attrib);
    m_gl0 = new MainCanvas(m_scene, axial, m_topNavWindow, ID_GL_NAV_X, wxDefaultPosition,
    	        wxDefaultSize, 0, _T("NavGLCanvasX"), gl_attrib);
    
    m_gl1 = new MainCanvas(m_scene, coronal, m_middleNavWindow, ID_GL_NAV_Y, wxDefaultPosition,
        	        wxDefaultSize, 0, _T("NavGLCanvasY"), gl_attrib);
    m_gl2 = new MainCanvas(m_scene, sagittal, m_bottomNavWindow, ID_GL_NAV_Z, wxDefaultPosition,
       	        wxDefaultSize, 0, _T("NavGLCanvasZ"), gl_attrib);
    
    m_scene->setMainGLContext(new wxGLContext(m_mainGL));
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
	load(true, wxT(""));
}

void MainFrame::load(bool showLoadDialog, wxString path)
{
	if (m_datasetListCtrl->GetItemCount() > 9)
	{
		wxMessageBox(wxT("ERROR\nCan't load any more files.\nDelete some first.\n"),  wxT(""), wxOK|wxICON_INFORMATION, NULL);
		m_statusBar->SetStatusText(wxT("ERROR"),1);
		m_statusBar->SetStatusText(TheDataset::lastError,2);
		return;
	}
	
	if (showLoadDialog)
	{
		wxString caption = wxT("Choose a file");
		wxString wildcard = wxT("Header files (*.hea)|*.hea|Mesh files (*.mesh)|*.mesh|Fibers VTK (*.fib)|*.fib|*.*|*.*");
		wxString defaultDir = wxEmptyString;
		wxString defaultFilename = wxEmptyString;
		wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxOPEN);
		if (dialog.ShowModal() == wxID_OK)
		{
			path = dialog.GetPath();
		}
	}

	DatasetInfo *info = TheDataset::load(path); 
	if ( info == NULL) 
	{
		wxMessageBox(wxT("ERROR\n") + TheDataset::lastError,  wxT(""), wxOK|wxICON_INFORMATION, NULL);
		m_statusBar->SetStatusText(wxT("ERROR"),1);
		m_statusBar->SetStatusText(TheDataset::lastError,2);
		return;
	}
	 
	int i = m_datasetListCtrl->GetItemCount();
	m_datasetListCtrl->InsertItem(i, wxT(""), 0);
	m_datasetListCtrl->SetItem(i, 1, info->getName());
	m_datasetListCtrl->SetItem(i, 2, wxT("0.10"));
	m_datasetListCtrl->SetItem(i, 3, wxT(""), 1);
	m_datasetListCtrl->SetItemData(i, (long)info);
	m_datasetListCtrl->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	
	m_statusBar->SetStatusText(wxT("Ready"),1);
	m_statusBar->SetStatusText(info->getName() + wxT(" loaded"),2);
		
	m_xSlider->SetMax(wxMax(2,TheDataset::columns-1));
	m_xSlider->SetValue(TheDataset::columns/2);
	m_ySlider->SetMax(wxMax(2,TheDataset::rows-1));
	m_ySlider->SetValue( TheDataset::rows/2);
	m_zSlider->SetMax(wxMax(2,TheDataset::frames-1));
	m_zSlider->SetValue( TheDataset::frames/2);
	m_tSlider->SetValue(10);
	
	m_scene->updateView(m_xSlider->GetValue(),m_ySlider->GetValue(),m_zSlider->GetValue());
		
	if (m_datasetListCtrl->GetItemCount() == 1)
	{
		m_scene->assignTextures();
		renewAllGLWidgets();
		updateTreeDims();
		updateTreeDS(i);
	}
	else
	{
		m_scene->addTexture();
		refreshAllGLWidgets();
		updateTreeDS(i);
	}
}

void MainFrame::updateTreeDims()
{
	m_treeWidget->DeleteChildren(m_tAxialId);
	m_treeWidget->DeleteChildren(m_tCoronalId);
	m_treeWidget->DeleteChildren(m_tSagittalId);
	m_treeWidget->AppendItem(m_tAxialId, wxString::Format(wxT("%d rows"), TheDataset::rows));
	m_treeWidget->AppendItem(m_tAxialId, wxString::Format(wxT("%d columns"), TheDataset::columns));
	m_treeWidget->AppendItem(m_tCoronalId, wxString::Format(wxT("%d columns"), TheDataset::columns));
	m_treeWidget->AppendItem(m_tCoronalId, wxString::Format(wxT("%d frames"), TheDataset::frames));
	m_treeWidget->AppendItem(m_tSagittalId, wxString::Format(wxT("%d rows"), TheDataset::rows));
	m_treeWidget->AppendItem(m_tSagittalId, wxString::Format(wxT("%d frames"), TheDataset::frames));
}

void MainFrame::updateTreeDS(int i)
{
	//m_treeWidget->DeleteChildren(m_tDatasetId);
	//m_treeWidget->DeleteChildren(m_tMeshId);
	//m_treeWidget->DeleteChildren(m_tFiberId);

	DatasetInfo* info = (DatasetInfo*)m_datasetListCtrl->GetItemData(i);
	switch (info->getType())
	{
	case Head_byte:
	case Head_short:
		info->m_treeId = m_treeWidget->AppendItem(m_tDatasetId, info->getName(), 
				-1, -1, new MyTreeItemData(info));
		break;
	case Overlay:
		info->m_treeId = m_treeWidget->AppendItem(m_tDatasetId, info->getName(),
				-1, -1, new MyTreeItemData(info));
		break;
	case RGB:
		info->m_treeId = m_treeWidget->AppendItem(m_tDatasetId, info->getName(),
				-1, -1, new MyTreeItemData(info));
		break;
	case Mesh_:
		info->m_treeId = m_treeWidget->AppendItem(m_tMeshId, info->getName(),
				-1, -1, new MyTreeItemData(info));
		break;
	case Curves_:
		info->m_treeId = m_treeWidget->AppendItem(m_tFiberId, info->getName(),
				-1, -1, new MyTreeItemData(info));
		break;
	}
	
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
      (void)wxMessageBox(_T("Viewer\nAuthor: Ralph Schurade (c) 2008"), _T("About Viewer"));
}

void MainFrame::OnGLEvent( wxCommandEvent &event )
{
	wxPoint pos, newpos;
	
	switch (event.GetInt())
	{
	case axial:
		pos = m_gl0->getMousePos();
		m_xSlider->SetValue((int)(((float)pos.x/NAV_GL_SIZE)*TheDataset::columns));
		m_ySlider->SetValue((int)(((float)pos.y/NAV_GL_SIZE)*TheDataset::rows));
		break;
	case coronal:
		pos = m_gl1->getMousePos();
		m_xSlider->SetValue((int)(((float)pos.x/NAV_GL_SIZE)*TheDataset::columns));
		m_zSlider->SetValue((int)(((float)pos.y/NAV_GL_SIZE)*TheDataset::frames));
		break;
	case sagittal:
		pos = m_gl2->getMousePos();
		m_ySlider->SetValue((int)(((float)pos.x/NAV_GL_SIZE)*TheDataset::rows));
		m_zSlider->SetValue((int)(((float)pos.y/NAV_GL_SIZE)*TheDataset::frames));
		break;
	case mainView:
		int delta = m_mainGL->getDelta();
		
		switch (m_mainGL->getPicked())
		{
		case axial:
			m_zSlider->SetValue(wxMin(wxMax(m_zSlider->GetValue() + delta, 0), m_zSlider->GetMax()));
			break;
		case coronal:
			m_ySlider->SetValue(wxMin(wxMax(m_ySlider->GetValue() + delta, 0), m_ySlider->GetMax()));
			break;
		case sagittal:
			m_xSlider->SetValue(wxMin(wxMax(m_xSlider->GetValue() + delta, 0), m_xSlider->GetMax()));
			break;
		}
	}
	m_scene->updateView(m_xSlider->GetValue(),m_ySlider->GetValue(),m_zSlider->GetValue());
	updateStatusBar();
	refreshAllGLWidgets();
}

void MainFrame::OnMouseEvent(wxMouseEvent& event)
{	
	this->Refresh();
}

void MainFrame::OnSize(wxSizeEvent& WXUNUSED(event))
{
	/* resize the navigation widgets */
	int height = this->GetClientSize().y;
	NAV_SIZE = wxMin(255, height/4);
	NAV_GL_SIZE = NAV_SIZE-4;
	
	m_leftWindowHolder->SetDefaultSize(wxSize(150 + NAV_SIZE, height));
	m_leftWindowTop->SetDefaultSize(wxSize(150 + NAV_SIZE, NAV_SIZE*3 + 65));
	m_leftWindowBottom->SetDefaultSize(wxSize(150 + NAV_SIZE, height - m_leftWindowTop->GetSize().y));
	m_leftWindowBottom1->SetDefaultSize(wxSize(150 + NAV_SIZE, m_leftWindowBottom->GetClientSize().y - 20));
	m_leftWindowBottom2->SetDefaultSize(wxSize(150 + NAV_SIZE, 20));
	m_navWindow->SetDefaultSize(wxSize(NAV_SIZE, height));
	m_topNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	m_middleNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	m_bottomNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	m_extraNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	
	/* resize sliders */
	m_xSlider->SetSize(wxSize(NAV_GL_SIZE, -1));
	m_ySlider->SetSize(wxSize(NAV_GL_SIZE, -1));
	m_zSlider->SetSize(wxSize(NAV_GL_SIZE, -1));

	/* resize list ctrl widget */
	m_datasetListCtrl->SetSize(0,0, m_leftWindowBottom->GetClientSize().x, m_leftWindowBottom->GetClientSize().y);
	m_datasetListCtrl->SetColumnWidth(0, 20);
	m_datasetListCtrl->SetColumnWidth(1, m_leftWindowBottom->GetClientSize().x - 140);
	m_datasetListCtrl->SetColumnWidth(2, 80);
	m_datasetListCtrl->SetColumnWidth(3, 20);
	
	/* resize main gl window */
	int mainSize = wxMin((this->GetClientSize().x - m_leftWindow->GetSize().x - m_navWindow->GetSize().x), 
			this->GetClientSize().y);
	m_rightWindowHolder->SetDefaultSize(wxSize(mainSize, mainSize));
	m_rightWindow->SetDefaultSize(wxSize(mainSize, mainSize));
	
	m_scene->updateView(m_xSlider->GetValue(),m_ySlider->GetValue(),m_zSlider->GetValue());

#if wxUSE_MDI_ARCHITECTURE
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
#endif // wxUSE_MDI_ARCHITECTURE

    GetClientWindow()->Update();
    this->Update();
}

void MainFrame::OnXSliderMoved(wxCommandEvent& event)
{
	 m_scene->updateView(m_xSlider->GetValue(),m_ySlider->GetValue(),m_zSlider->GetValue());
	 refreshAllGLWidgets();
}

void MainFrame::OnYSliderMoved(wxCommandEvent& event)
{
	m_scene->updateView(m_xSlider->GetValue(),m_ySlider->GetValue(),m_zSlider->GetValue());
	refreshAllGLWidgets();
}

void MainFrame::OnZSliderMoved(wxCommandEvent& event)
{
	m_scene->updateView(m_xSlider->GetValue(),m_ySlider->GetValue(),m_zSlider->GetValue());
	refreshAllGLWidgets();
}

void MainFrame::OnTSliderMoved(wxCommandEvent& event)
{
	float threshold = (float)m_tSlider->GetValue()/100.0;
		
	long item = m_datasetListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item == -1) return;
	m_datasetListCtrl->SetItem(item, 2, wxString::Format(wxT("%.2f"), threshold ));
	DatasetInfo *info = (DatasetInfo*) m_datasetListCtrl->GetItemData(item);
	info->setThreshold(threshold);
	refreshAllGLWidgets();
}

void MainFrame::refreshAllGLWidgets()
{
	m_gl0->render();
	m_gl1->render();
	m_gl2->render();
	m_mainGL->render();
	updateStatusBar();
}

void MainFrame::renewAllGLWidgets()
{
	m_mainGL->invalidate();
	m_gl0->invalidate();
	m_gl1->invalidate();
	m_gl2->invalidate();
	refreshAllGLWidgets();
	updateInfoString();
}

void MainFrame::updateInfoString()
{
	
}

void MainFrame::OnToggleView1(wxCommandEvent& event)
{
	if (!m_scene) return;
	m_scene->m_showAxial = !m_scene->m_showAxial;
	m_mainGL->render();
}

void MainFrame::OnToggleView2(wxCommandEvent& event)
{
	if (!m_scene) return;
	m_scene->m_showCoronal = !m_scene->m_showCoronal;
	m_mainGL->render();
}

void MainFrame::OnToggleView3(wxCommandEvent& event)
{
	if (!m_scene) return;
	m_scene->m_showSagittal = !m_scene->m_showSagittal;
	m_mainGL->render();
}

void MainFrame::OnToggleSelBox(wxCommandEvent& event)
{
	if (!m_scene) return;
	m_scene->m_showSelBox = !m_scene->m_showSelBox;
	m_mainGL->render();
}

void MainFrame::loadStandard()
{
	load(false, wxT("/home/ralph/bin/devel/workspace/diplom/data/t1_1mm.hea"));
	load(false, wxT("/home/ralph/bin/devel/workspace/diplom/data/overlay_swap.hea"));
	load(false, wxT("/home/ralph/bin/devel/workspace/diplom/data/rgb.hea"));
	//load(false, wxT("/home/ralph/bin/devel/workspace/diplom/data/s1_Rwhite.mesh"));
}

void MainFrame::OnReloadShaders(wxCommandEvent& event)
{
	renewAllGLWidgets();
}

void MainFrame::OnNew(wxCommandEvent& event)
{
	m_datasetListCtrl->DeleteAllItems();
	m_mainGL->invalidate();
	
	TheDataset::columns = 1;
    TheDataset::rows = 1;
    TheDataset::frames = 1;
    TheDataset::lastError = wxT("");

	delete m_scene;
	m_scene = new TheScene();
	
	m_scene->setDataListCtrl(m_datasetListCtrl);
	m_scene->setMainGLContext(new wxGLContext(m_mainGL));
	
	m_mainGL->setScene(m_scene);
	m_gl0->setScene(m_scene);
	m_gl1->setScene(m_scene);
	m_gl2->setScene(m_scene);
	updateInfoString();
	refreshAllGLWidgets();
}

void MainFrame::updateStatusBar()
{
	wxString sbString0 = wxT("");
	sbString0 = wxString::Format(wxT("Axial: %d Coronal: %d Sagittal: %d"),m_zSlider->GetValue(), m_ySlider->GetValue(), m_xSlider->GetValue()); 
	m_statusBar->SetStatusText(sbString0,0);
}

void MainFrame::OnActivateListItem(wxListEvent& event)
{
	int item = event.GetIndex();
	DatasetInfo *info = (DatasetInfo*) m_datasetListCtrl->GetItemData(item);
	int col = m_datasetListCtrl->getColClicked();
	switch (col)
	{
	case 0:
		if (info->toggleShow())
		{
			m_datasetListCtrl->SetItem(item, 0, wxT(""), 0);
		}
		else
		{
			m_datasetListCtrl->SetItem(item, 0, wxT(""), 1);
		}
		refreshAllGLWidgets();
		break;
	case 1:
		if (info->getType() >= Mesh_)
		{
			if (!info->toggleShowFS())
				m_datasetListCtrl->SetItem(item, 1, info->getName() + wxT("*"));
			else
				m_datasetListCtrl->SetItem(item, 1, info->getName());
		}
		break;
	case 2:
		if (info->getType() >= Mesh_)
		{
			if (!info->toggleUseTex())
				m_datasetListCtrl->SetItem(item, 2, wxT("(") + wxString::Format(wxT("%.2f"), info->getThreshold()) + wxT(")") );
			else
				m_datasetListCtrl->SetItem(item, 2, wxString::Format(wxT("%.2f"), info->getThreshold() ));
		}
		break;
	case 3:
		m_treeWidget->Delete(info->m_treeId);
		delete info;
		m_datasetListCtrl->DeleteItem(item);
		renewAllGLWidgets();
		break;
	default:
		break;
	}
	refreshAllGLWidgets();
}

void MainFrame::OnSelectListItem(wxListEvent& event)
{
	int item = event.GetIndex();
	if (item == -1) return;
	DatasetInfo *info = (DatasetInfo*) m_datasetListCtrl->GetItemData(item);
	m_tSlider->SetValue((int)(info->getThreshold()*100));
	m_treeWidget->SelectItem(info->m_treeId);
	m_treeWidget->EnsureVisible(info->m_treeId);
}

void MainFrame::OnListItemUp(wxCommandEvent& event)
{
	long item = m_datasetListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item == -1) return;
	m_datasetListCtrl->moveItemUp(item);
	m_datasetListCtrl->EnsureVisible(item);
	if (item > 0) m_scene->swapTextures(item, item -1);
	refreshAllGLWidgets();
}
    
void MainFrame::OnListItemDown(wxCommandEvent& event)
{
	long item = m_datasetListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item == -1) return;
	m_datasetListCtrl->moveItemDown(item);
	m_datasetListCtrl->EnsureVisible(item);
	if (item < m_datasetListCtrl->GetItemCount() - 1) m_scene->swapTextures(item, item + 1);
	refreshAllGLWidgets();
}

void MainFrame::OnSelectTreeItem(wxTreeEvent& event)
{
	wxTreeItemId treeid = m_treeWidget->GetSelection();
	MyTreeItemData *data = (MyTreeItemData*)m_treeWidget->GetItemData(treeid);
	if (!data) return;
	for (int i = 0 ; i < m_datasetListCtrl->GetItemCount(); ++i)
	{
		DatasetInfo *info = (DatasetInfo*) m_datasetListCtrl->GetItemData(i);
		if (info->m_treeId == treeid)
		{
			m_datasetListCtrl->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	}
}
