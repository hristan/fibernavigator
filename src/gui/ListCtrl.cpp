#include "ListCtrl.h"

#include "../Logger.h"
#include "../dataset/DatasetInfo.h"
#include "../dataset/FibersGroup.h"

#include <wx/imaglist.h>
#include <wx/string.h>

#define ID_LIST_CTRL2 292

BEGIN_EVENT_TABLE( ListCtrl, wxListCtrl )
    EVT_LEFT_DOWN( ListCtrl::onLeftClick )
    EVT_LIST_ITEM_ACTIVATED( ID_LIST_CTRL2, ListCtrl::onActivate )
END_EVENT_TABLE()


ListCtrl::ListCtrl( wxWindow *pParent, const wxPoint &point, const wxSize &size, const long style )
: wxListCtrl( pParent, ID_LIST_CTRL2, point, size, style ),
  m_column( 0 ),
  m_isFiberGroupPresent( false )
{
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::AssignImageList( wxImageList *imageList, int which )
{
    wxListCtrl::AssignImageList( imageList, which );
}

//////////////////////////////////////////////////////////////////////////

long ListCtrl::InsertColumn( long col, const wxListItem& info )
{
    return wxListCtrl::InsertColumn( col, info );
}

//////////////////////////////////////////////////////////////////////////
void ListCtrl::InsertItem( const DatasetInfo * const pDataset )
{
    if( NULL == pDataset )
    {
        return;
    }

    if( FIBERS == pDataset->getType() && !m_isFiberGroupPresent )
    {
        InsertItem( new FibersGroup(NULL) );
        m_isFiberGroupPresent = true;
    }

    // insert at zero is a well-known bug on OSX, so we append there...
    // http://trac.wxwidgets.org/ticket/4492
    // To have the same behavior on all platforms, we add to the end of the list
    long index = GetItemCount();

    wxListCtrl::InsertItem( index, pDataset->getShow() ? 0 : 1 );
    SetItemData( index, (long)pDataset );
    SetItemState( index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
    Update( index );
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::onActivate( wxListEvent& evt )
{
    Logger::getInstance()->print( _T( "Event triggered - ListCtrl::onActivate" ), LOGLEVEL_DEBUG );

    int index = evt.GetIndex();
    DatasetInfo *pDataset = (DatasetInfo *)GetItemData( index );

    switch( m_column )
    {        
    case 0:
        pDataset->toggleShow();
        Update( index );
        break;
    case 1:
        pDataset->toggleShowFS();
        Update( index );
        break;
    case 3:
        // TODO: Delete item
        DeleteItem( index );
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::onLeftClick( wxMouseEvent& evt )
{
    int posX = evt.GetPosition().x;
    int sizeX( 0 );
    for( int col( 0 ); col < GetColumnCount(); ++col)
    {
        sizeX += GetColumnWidth(col);
        if( posX <= sizeX )
        {
            m_column = col;
            break;
        }
    }
    evt.Skip();
}

//////////////////////////////////////////////////////////////////////////

bool ListCtrl::SetColumnWidth( int col, int width )
{
    return wxListCtrl::SetColumnWidth( col, width );
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::SetMaxSize( const wxSize &size )
{
    wxListCtrl::SetMaxSize( size );
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::SetMinSize( const wxSize &size )
{
    wxListCtrl::SetMinSize( size );
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::UpdateSelected()
{
    long index = GetNextItem( 0, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    Update( index );
}

//////////////////////////////////////////////////////////////////////////

void ListCtrl::Update( long index )
{
    DatasetInfo *pDataset = (DatasetInfo *)GetItemData( index );
    SetItem( index, 0, wxT( "" ), pDataset->getShow() ? 0 : 1 );
    SetItem( index, 1, pDataset->getName().BeforeFirst('.') + ( pDataset->getShowFS() ? wxT( "" ) : wxT( "*" ) ) );
    Logger::getInstance()->print(wxString::Format( wxT( "%s::Threshold: %f" ), pDataset->getName(), pDataset->getThreshold() * pDataset->getOldMax() ), LOGLEVEL_DEBUG );
    SetItem( index, 2, wxString::Format( 2 != m_column && pDataset->getType() < MESH && pDataset->getUseTex() ? wxT( "%.2f" ) : wxT( "(%.2f)" ), pDataset->getThreshold() * pDataset->getOldMax() ) );
    SetItem( index, 3, wxT( "" ), 2 );
}

//////////////////////////////////////////////////////////////////////////
