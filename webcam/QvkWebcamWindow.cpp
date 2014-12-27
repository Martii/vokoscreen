#include "QvkWebcamWindow.h" 

using namespace std;

QvkWebcamWindow::QvkWebcamWindow()
{
  vkSettings.readAll();
  setGeometry( vkSettings.getWebcamX(), vkSettings.getWebcamY(), vkSettings.getWebcamWidth(), vkSettings.getWebcamHeight() );

  setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
  setWindowTitle( "vokoscreen webcam");
  setToolTip( tr( "Right click for menu" ) );

  //if( QX11Info::isCompositingManagerRunning() ) // QT5 *************************** vorerst Ersatzlos gestrichen ******************************************************
    setAttribute( Qt::WA_TranslucentBackground, true );
  
  webcamLabel = new QLabel( this );
  webcamLabel->setGeometry( 0, 0, width(), height() );
  webcamLabel->setAlignment( Qt::AlignCenter );
  webcamLabel->setScaledContents( true );
  webcamLabel->show();
  
  action160x120 = new QAction( "160 x 120", this );
  action160x120->setCheckable( true );
  connect( action160x120, SIGNAL( triggered() ), this, SLOT( set160x120() ) );
   
  action320x240 = new QAction( "320 x 240", this );
  action320x240->setCheckable( true );
  connect( action320x240, SIGNAL( triggered() ), this, SLOT( set320x240() ) );

  action640x480 = new QAction( "640 x 480", this );
  action640x480->setCheckable( true );
  connect( action640x480, SIGNAL( triggered() ), this, SLOT( set640x480() ) );

  actionUserDefined = new QAction( tr( "User-defined" ), this );
  actionUserDefined->setCheckable( true );
  connect( actionUserDefined, SIGNAL( triggered() ), this, SLOT( setActionUserDefined() ) );

  actionBorder = new QAction( tr ( "Border" ), this );
  actionBorder->setCheckable( true );
  actionBorder->setChecked( true );  
  connect( actionBorder, SIGNAL( triggered() ), this, SLOT( setBorder() ) );

  actionVisibleOverFullscreen = new QAction( tr ( "Show over fullscreen" ), this );
  actionVisibleOverFullscreen->setCheckable( true );
  actionVisibleOverFullscreen->setChecked( false );
  connect( actionVisibleOverFullscreen, SIGNAL( triggered() ), this, SLOT( setVisibleOverFullscreen() ) );

  actionClose = new QAction( tr ( "Close" ), this );
  connect( actionClose, SIGNAL( triggered() ), this, SLOT( closeMenue() ) );
  
  if ( ( vkSettings.getWebcamWidth() == 160 ) and ( vkSettings.getWebcamHeight() == 120 ) )
  {
    set160x120();
  }
  else if ( ( vkSettings.getWebcamWidth() == 320 ) and ( vkSettings.getWebcamHeight() == 240 ) )
       {
         set320x240();
       }else if ( ( vkSettings.getWebcamWidth() == 640 ) and ( vkSettings.getWebcamHeight() == 480 ) )
             {
               set640x480();
             }else
              {
                setActionUserDefined();
              }
}


QvkWebcamWindow::~QvkWebcamWindow()
{
}


void QvkWebcamWindow::setValueBorder( bool value )
{
  border = value; 
}

bool QvkWebcamWindow::getValueBorder()
{
  return border; 
}

int QvkWebcamWindow::getValueX()
{
  return x();
}

int QvkWebcamWindow::getValueY()
{
  return y();
}

int QvkWebcamWindow::getValueWidth()
{
  return width();
}

int QvkWebcamWindow::getValueHeight()
{
  return height();
}

/**
 *  closeEvent wird ausgelößt wenn das webcamfenster geschloßen wird
 */
void QvkWebcamWindow::closeEvent( QCloseEvent * event )
{
  (void)event;
  emit closeWebcamWindow();
}

/**
 * Wird aufgerufen sobald die Kontexttaste betätigt wird
 */
void QvkWebcamWindow::contextMenuEvent( QContextMenuEvent *event )
{
     menu.addAction( action160x120 );
     menu.addAction( action320x240 );
     menu.addAction( action640x480 );
     menu.addAction( actionUserDefined );
     menu.addSeparator();
     menu.addAction( actionBorder );
     menu.addAction( actionVisibleOverFullscreen );
     menu.addSeparator();
     menu.addAction( actionClose );
     menu.exec( event->globalPos() );
 }

/**
 * Wird vom Popupmenue aufgerufen
 */
void QvkWebcamWindow::closeMenue()
{
  close();
}
 
 
void QvkWebcamWindow::set160x120()
{
  setGeometry( x(), y(), 160, 120 );
  action160x120->setChecked( true );
  action320x240->setChecked( false );
  action640x480->setChecked( false );
  actionUserDefined->setChecked( false );
  actionUserDefined->setVisible ( false );
}


void QvkWebcamWindow::set320x240()
{
  setGeometry( x(), y(), 320, 240 );
  action160x120->setChecked( false );
  action320x240->setChecked( true );
  action640x480->setChecked( false );
  actionUserDefined->setChecked( false );
  actionUserDefined->setVisible ( false );
}


void QvkWebcamWindow::set640x480()
{
  setGeometry( x(), y(), 640, 480 );
  action160x120->setChecked( false );
  action320x240->setChecked( false );
  action640x480->setChecked( true );
  actionUserDefined->setChecked( false );
  actionUserDefined->setVisible ( false );
}


void QvkWebcamWindow::setActionUserDefined()
{
  action160x120->setChecked( false );
  action320x240->setChecked( false );
  action640x480->setChecked( false );
  actionUserDefined->setChecked( true );
  actionUserDefined->setVisible ( true );
}


void QvkWebcamWindow::setBorder()
{
  if ( actionBorder->isChecked() )
  {
    actionVisibleOverFullscreen->setChecked( false );
    setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    show();
    setValueBorder( true );
  }
  else
  {
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    show();
    setValueBorder( false );
  }
}


void QvkWebcamWindow::setVisibleOverFullscreen()
{
  if ( actionVisibleOverFullscreen->isChecked() )
  {
    actionBorder->setChecked( false );
    setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint );
    activateWindow();
    show();
  }
  else
  {
    actionBorder->setChecked( true );
    setWindowFlags( Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );
    activateWindow();
    show();
  }
}


void QvkWebcamWindow::resizeEvent ( QResizeEvent * )
{
  webcamLabel->setGeometry( 0, 0, this->width(), this->height() );

  if ( ( width() == 160 ) and ( height() == 120 ) )
  { 
    return;
  } 
  else if ( ( width() == 320 ) and ( height() == 240 ) )
       { 
         return;
       }
       else if ( ( width() == 640 ) and ( height() == 480 ) )
            { 
              return;
            }
            else
            {
              setActionUserDefined();
            }
}
