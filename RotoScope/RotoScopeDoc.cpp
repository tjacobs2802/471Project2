// RotoScopeDoc.cpp : implementation of the CRotoScopeDoc class
//

#include "pch.h"
#include "RotoScope.h"
#include <fstream>

#include "RotoScopeDoc.h"
#include "xmlhelp.h"
#include "CMorph.h"
#include "CChromakey.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// Simple inline function to range bound a double and cast to a short.
inline short ShortRange(double a) {return a > 32767 ? 32767 : (a < -32768. ? -32768 : short(a));}


// CRotoScopeDoc

IMPLEMENT_DYNCREATE(CRotoScopeDoc, CDocument)

BEGIN_MESSAGE_MAP(CRotoScopeDoc, CDocument)
    ON_COMMAND(ID_MOVIES_OPENSOURCEMOVIE, &CRotoScopeDoc::OnMoviesOpensourcemovie)
    ON_COMMAND(ID_MOVIES_OPENOUTPUTMOVIE, &CRotoScopeDoc::OnMoviesOpenoutputmovie)
    ON_COMMAND(ID_FRAMES_CREATEONEFRAME, &CRotoScopeDoc::OnFramesCreateoneframe)
    ON_COMMAND(ID_FRAMES_WRITEONEFRAME, &CRotoScopeDoc::OnFramesWriteoneframe)
    ON_UPDATE_COMMAND_UI(ID_FRAMES_WRITEONEFRAME, &CRotoScopeDoc::OnUpdateFramesWriteoneframe)
    ON_COMMAND(ID_MOVIES_CLOSESOURCEMOVIE, &CRotoScopeDoc::OnMoviesClosesourcemovie)
    ON_UPDATE_COMMAND_UI(ID_MOVIES_CLOSESOURCEMOVIE, &CRotoScopeDoc::OnUpdateMoviesClosesourcemovie)
    ON_COMMAND(ID_MOVIES_CLOSEOUTPUTMOVIE, &CRotoScopeDoc::OnMoviesCloseoutputmovie)
    ON_UPDATE_COMMAND_UI(ID_MOVIES_CLOSEOUTPUTMOVIE, &CRotoScopeDoc::OnUpdateMoviesCloseoutputmovie)
    ON_COMMAND(ID_FRAMES_WRITETHENCREATEONEFRAME, &CRotoScopeDoc::OnFramesWritethencreateoneframe)
    ON_UPDATE_COMMAND_UI(ID_FRAMES_WRITETHENCREATEONEFRAME, &CRotoScopeDoc::OnUpdateFramesWritethencreateoneframe)
    ON_COMMAND(ID_FRAMES_WRITETHENCREATEONESECOND, &CRotoScopeDoc::OnFramesWritethencreateonesecond)
    ON_COMMAND(ID_FRAMES_WRITETHENCREATEREMAINING, &CRotoScopeDoc::OnFramesWritethencreateremaining)
    ON_COMMAND(ID_MOVIES_OPENBACKGROUNDAUDIO, &CRotoScopeDoc::OnMoviesOpenbackgroundaudio)
    ON_COMMAND(ID_MOVIES_CLOSEBACKGROUNDAUDIO, &CRotoScopeDoc::OnMoviesClosebackgroundaudio)
    ON_UPDATE_COMMAND_UI(ID_MOVIES_CLOSEBACKGROUNDAUDIO, &CRotoScopeDoc::OnUpdateMoviesClosebackgroundaudio)
	ON_COMMAND(ID_EDIT_CLEARFRAME, &CRotoScopeDoc::OnEditClearframe)
	ON_COMMAND(ID_EDIT_DRAWLINE, &CRotoScopeDoc::OnEditDrawline)
	ON_COMMAND(ID_EDIT_PLACEBIRD, &CRotoScopeDoc::OnEditPlacebird)
	ON_COMMAND(ID_EDIT_ROTATEIMAGE, &CRotoScopeDoc::OnEditRotateimage)
	ON_COMMAND(ID_EDIT_SETVARIABLES, &CRotoScopeDoc::OnEditSetvariables)
	ON_COMMAND(ID_MOUSEMODE_PEN, &CRotoScopeDoc::OnMousemodePen)
	ON_COMMAND(ID_MOUSEMODE_LINE, &CRotoScopeDoc::OnMousemodeLine)
	ON_COMMAND(ID_MOUSEMODE_BIRD, &CRotoScopeDoc::OnMousemodeBird)
	ON_COMMAND(ID_EDIT_UNDO32793, &CRotoScopeDoc::OnEditUndo32793)
	ON_COMMAND(ID_MOUSEMODE_AIDAN, &CRotoScopeDoc::OnMousemodeAidan)
	ON_COMMAND(ID_MOUSEMODE_MARIO, &CRotoScopeDoc::OnMousemodeMario)
	ON_COMMAND(ID_EDIT_PLACEMARIO, &CRotoScopeDoc::OnEditPlacemario)
	ON_COMMAND(ID_MOUSEMODE_JULIA, &CRotoScopeDoc::OnMousemodeJulia)
	ON_COMMAND(ID_MOUSEMODE_TREVOR, &CRotoScopeDoc::OnMousemodeTrevor)
	ON_COMMAND(ID_MOUSEMODE_APPLYWAVEWARP, &CRotoScopeDoc::OnMousemodeApplywavewarp)
	ON_COMMAND(ID_EDIT_PLACEAIDAN, &CRotoScopeDoc::OnEditPlaceaidan)
	ON_COMMAND(ID_MOUSEMODE_GREG, &CRotoScopeDoc::OnMousemodeGreg)
	ON_COMMAND(ID_EDIT_MORPH, &CRotoScopeDoc::OnEditMorph)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MORPH, &CRotoScopeDoc::OnUpdateEditMorph)
	ON_COMMAND(ID_MOUSEMODE_RECOLOR, &CRotoScopeDoc::OnMousemodeRecolor)
	ON_UPDATE_COMMAND_UI(ID_MOUSEMODE_APPLYWAVEWARP, &CRotoScopeDoc::OnUpdateMousemodeApplywavewarp)
	ON_COMMAND(ID_EDIT_GREENSCREEN, &CRotoScopeDoc::OnEditGreenscreen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GREENSCREEN, &CRotoScopeDoc::OnUpdateEditGreenscreen)
	ON_COMMAND(ID_MOUSEMODE_RECOLOR, &CRotoScopeDoc::OnMousemodeRecolor)
	ON_UPDATE_COMMAND_UI(ID_MOUSEMODE_APPLYWAVEWARP, &CRotoScopeDoc::OnUpdateMousemodeApplywavewarp)
END_MESSAGE_MAP()


//! Constructor for the document class.  
CRotoScopeDoc::CRotoScopeDoc()
{
	m_waveEnabled = false;
	m_morphEnabled = false;
	m_greenScreenEnabled = false;
    ::CoInitialize(NULL);

    // Set the image size to an initial default value and black.
	m_image.SetSize(640, 480);
    m_image.Fill(0, 0, 0);
	m_movieframe = 0;
	m_b = 0;
	m_g = 0;
	m_r = 255;
	m_width = 1;
	m_dot_count = 0;
	m_bird.LoadFile(L"birdp.png");
	m_mario.LoadFile(L"mario.png");
	m_aidan.LoadFile(L"aidan.png");
	m_aidanArm.LoadFile(L"aidanArm.png");
	m_julia.LoadFile(L"julia.png");
	m_fireworks.LoadFile(L"fireworks.png");
	m_trevor.LoadFile(L"trevor.png");
	m_greg.LoadFile(L"greg.png");

	m_moviemake.SetProfileName(L"profile720p.prx");

	//OnEditSetvariables();
}

//! Destructor
CRotoScopeDoc::~CRotoScopeDoc()
{
    ::CoUninitialize();
}


//! Function that is called when a new document is created
//! \returns true if successful
BOOL CRotoScopeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



// CRotoScopeDoc diagnostics

#ifdef _DEBUG
void CRotoScopeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRotoScopeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Source movie management
//
///////////////////////////////////////////////////////////////////////////////////////////////////


//
// Name :         CRotoScopeDoc::OnMoviesOpensourcemovie()
// Description :  Open a video input source.
//

void CRotoScopeDoc::OnMoviesOpensourcemovie()
{
	static TCHAR BASED_CODE szFilter[] = TEXT("Video Files (*.avi;*.wmv;*.asf)|*.avi; *.wmv; *.asf|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(".avi"), NULL, 0, szFilter, NULL);
	if(dlg.DoModal() != IDOK)
        return;

    if(!m_moviesource.Open(dlg.GetPathName()))
        return;
}

void CRotoScopeDoc::OnMoviesClosesourcemovie() { m_moviesource.Close(); }
void CRotoScopeDoc::OnUpdateMoviesClosesourcemovie(CCmdUI *pCmdUI) { pCmdUI->Enable(m_moviesource.IsOpen()); }


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Background music management
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Name :        CRotoScopeDoc::OnMoviesOpenbackgroundaudio()
// Description : Opens an audio file we can use as a background music source.
//

void CRotoScopeDoc::OnMoviesOpenbackgroundaudio()
{
	static TCHAR BASED_CODE szFilter[] = TEXT("Audio Files (*.wav;*.mp3)|*.wav; *.mp3|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(".wav"), NULL, 0, szFilter, NULL);
	if(dlg.DoModal() != IDOK)
        return;

    if(!m_backaudio.Open(dlg.GetPathName()))
        return;
}

void CRotoScopeDoc::OnMoviesClosebackgroundaudio() { m_backaudio.Close(); }
void CRotoScopeDoc::OnUpdateMoviesClosebackgroundaudio(CCmdUI *pCmdUI) { pCmdUI->Enable(m_backaudio.IsOpen()); }


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Output movie management
//
///////////////////////////////////////////////////////////////////////////////////////////////////


//
// Name :        CRotoScopeDoc::OnMoviesOpenoutputmovie()
// Description : Open an output movie.
//

void CRotoScopeDoc::OnMoviesOpenoutputmovie()
{
	m_movieframe = 0;
	static TCHAR BASED_CODE szFilter[] = TEXT("ASF Files (*.asf)|*.asf|All Files (*.*)|*.*||");

	CFileDialog dlg(FALSE, TEXT(".asf"), NULL, 0, szFilter, NULL);
	if(dlg.DoModal() != IDOK)
        return;

    if(!m_moviemake.Open(dlg.GetPathName()))
        return;

    // Make CImage match the size of the output movie.
    m_image.SetSize(m_moviemake.GetWidth(), m_moviemake.GetHeight());

    // Make the audio buffer match the storage requirement for one video frame
    m_audio.clear();
    m_audio.resize( size_t(m_moviemake.GetSampleRate() / m_moviemake.GetFPS()) * m_moviemake.GetNumChannels() );

    UpdateAllViews(NULL);
}


void CRotoScopeDoc::OnMoviesCloseoutputmovie() { m_moviemake.Close(); }
void CRotoScopeDoc::OnUpdateMoviesCloseoutputmovie(CCmdUI *pCmdUI) { pCmdUI->Enable(m_moviemake.IsOpen()); }

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Creating and writing video frames.
//
///////////////////////////////////////////////////////////////////////////////////////////////////


//
// Name :         CRotoScopeDoc::OnFramesCreateoneframe()
// Description :  Menu handler for Frame/Create One Frame menu option.
//                This will call the function that creates one frame.
//

void CRotoScopeDoc::OnFramesCreateoneframe()
{
    CreateOneFrame();
    UpdateAllViews(NULL);
}


//
// Name :        CRotoScopeDoc::CreateOneFrame()
// Description : This function creates a frame for display.
//               This is a demonstration of how we can read frames and audio 
//               and create an output frame.
//

void CRotoScopeDoc::CreateOneFrame()
{
	//
	// Clear our frame first
	//

	m_image.Fill(0, 0, 0);

	//
	// Read any image from source video?
	//

	if (m_moviesource.HasVideo())
	{
		// Important:  Don't read directly into m_image.  Our source may be a 
		// different size!  I'm reading into a temporary image, then copying
		// the data over.

		CGrImage image;
		if (m_moviesource.ReadImage(m_initial))
		{
			DrawImage();
		}
	}

	//
	// Read any audio from the source video?  Note that we read and write the 
	// audio associated with one frame of video.
	//

	std::vector<short> audio;
	if (m_moviesource.HasAudio() && m_moviesource.ReadAudio(audio))
	{
		// The problem is that the input audio may not be in the same format
		// as the output audio.  For example, we may have a different number of 
		// audio frames for a given video frame.  Also, the channels may be
		// different.  I'll assume my output is stereo here, since I created a
		// stereo profile, but the input may be mono.

		if (m_moviesource.GetNumChannels() == 2)
		{
			// Easiest, both are the same.
			// What's the ratio of playback?
			double playrate = double(audio.size()) / double(m_audio.size());
			for (unsigned f = 0; f < m_audio.size() / 2; f++)
			{
				int srcframe = int(playrate * f);
				m_audio[f * 2] = audio[srcframe * 2];
				m_audio[f * 2 + 1] = audio[srcframe * 2 + 1];
			}
		}
		else
		{
			// Mono into stereo
			double playrate = double(2. * audio.size()) / double(m_audio.size());
			for (unsigned f = 0; f < m_audio.size() / 2; f++)
			{
				int srcframe = int(playrate * f);
				m_audio[f * 2] = audio[srcframe];
				m_audio[f * 2 + 1] = audio[srcframe];
			}
		}

	}
	else
	{
		// If there is no audio to read, set to silence.
		for (unsigned int i = 0; i < m_audio.size(); i++)
			m_audio[i] = 0;
	}

	//
	// Is there any background audio to mix in?
	//

	if (m_backaudio.IsOpen())
	{
		for (std::vector<short>::iterator i = m_audio.begin(); i != m_audio.end(); )
		{
			short audio[2];
			m_backaudio.ReadStereoFrame(audio);

			*i = ShortRange(*i + audio[0] * 0.3);
			i++;
			*i = ShortRange(*i + audio[1] * 0.3);
			i++;
		}
	}

	if (m_waveEnabled) {
		ApplyWaveEffect();
	}

	int frameNumber = m_movieframe;
	int initialX = 400;
	int initialY = 0;
	int speed = 2;

	int newX = initialX; 
	int newY = initialY + frameNumber * speed; 

	newX = (std::min)(m_image.GetWidth() - 1, (std::max)(0, newX));
	newY = (std::min)(m_image.GetHeight() - 1, (std::max)(0, newY));

	DrawFireworks(m_image, newX, newY);

	OnFramesWriteoneframe();

	UpdateAllViews(NULL);
}

//
// Most of the following are various menu options for video processing designed to make
// the user interface easier.
//

void CRotoScopeDoc::OnFramesWriteoneframe()
{
    m_moviemake.WriteImage(m_image);
    m_moviemake.WriteAudio(m_audio);
	m_movieframe++;
}

void CRotoScopeDoc::OnUpdateFramesWriteoneframe(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_moviemake.IsOpen());
}


void CRotoScopeDoc::OnFramesWritethencreateoneframe()
{
    OnFramesWriteoneframe();
    OnFramesCreateoneframe();
}

void CRotoScopeDoc::OnUpdateFramesWritethencreateoneframe(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_moviemake.IsOpen());
}

void CRotoScopeDoc::OnFramesWritethencreateonesecond()
{
    if(m_moviesource.IsOpen() && !m_moviesource.HasAudio())
        return;

    // Do the creation operation for one entire second
    for(int i=0;  i<int(m_moviemake.GetFPS() + 0.5);  i++)
    {
        OnFramesWriteoneframe();
        OnFramesCreateoneframe();
        if(m_moviesource.IsOpen() && !m_moviesource.HasVideo())
            break;

        MessagePump();
    }
}

//
// Name :        CRotoScopeDoc::OnFramesWritethencreateremaining()
// Description : This is an example of a loop that will consume all remaining 
//               source video.
//

void CRotoScopeDoc::OnFramesWritethencreateremaining()
{
    // This only makes sense if there's an input video
    if(!m_moviesource.IsOpen() || !m_moviesource.HasAudio())
        return;

    // Do the creation operation for one entire second
    for(int i=0;  ;  i++)
    {
        OnFramesWriteoneframe();
        OnFramesCreateoneframe();
        if(!m_moviesource.HasVideo())
            break;

        MessagePump();
    }

    AfxMessageBox(TEXT("All Done"));
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// The mouse-based user interface.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//! This function is called by the view class when the mouse is
//! pressed or moved on the screen.
/*! It sets the associated pixel in the image to red right now. You'll 
 * likely make it do something different
 * \param p_x X location in image
 * \param p_y Y location in image */
void CRotoScopeDoc::Mouse(int p_x, int p_y)
{
	// We need to convert screen locations to image locations
	int x = p_x;                            // No problem there.
	int y = m_image.GetHeight() - p_y - 1;     // Just invert it.

	if (m_mode == 0)
	{

		// Ensure there is an entry for every frame up till this one...
		std::list<CPoint> empty;
		while ((int)m_draw.size() < m_movieframe + 1)
			m_draw.push_back(empty);

		// Add the mouse point to the list for the frame
		m_draw[m_movieframe].push_back(CPoint(x, y));

		DrawImage();
	}

	else if (m_mode == 1)
	{
		if (m_dot_count == 0) 
		{
			m_dot_count++;
			m_x1 = x;
			m_y1 = y;
		}
		else
		{
			m_dot_count = 0;
			m_x2 = x;
			m_y2 = y;

			DrawLine(m_image, m_x1, m_y1, m_x2, m_y2, m_width, m_b, m_g, m_r);
			UpdateAllViews(NULL);
		}
	}

	else if (m_mode == 2)
	{
		DrawBird(m_image, x, y);
		UpdateAllViews(NULL);
	}

	else if (m_mode == 3)
	{
		DrawAidan(m_image, x, y);
		UpdateAllViews(NULL);
	}

	else if (m_mode == 4)
	{
		DrawMario(m_image, x, y);
		UpdateAllViews(NULL);
	}

	else if (m_mode == 5) {
		DrawJulia(m_image, x, y);
		UpdateAllViews(NULL);
	}

	else if (m_mode == 6) {
		DrawTrevor(m_image, x, y);
		UpdateAllViews(NULL);
	}

	else if (m_mode == 7) {
		DrawGreg(m_image, x, y);
		UpdateAllViews(NULL);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Utility functions...
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Name :        CRotoScopeDoc::MessagePump(void)
// Description : This function is a Windows message pump. It allows message processing to 
//               proceed while your program is in some loop.  This is so the menus and such
//               will continue to work.  Be very careful.  Usually you pop up a modal 
//               dialog box while this is going on to prevent accessing anything other than
//               a cancel button.  I'm not doing that, so be careful about what you hit.
//

void CRotoScopeDoc::MessagePump(void)
{
    // Allow any messages to be processed
    MSG msg;
    while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
    {
            AfxGetThread()->PumpMessage();
    }
}



BOOL CRotoScopeDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	//
	// Create an XML document
	//

	// CComPtr<> is a "smart" pointer. It takes care of reference counting on COM objects for
	// us so they know when to delete themselves.
	CComPtr<IXMLDOMDocument>  xmlDoc;
	HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&xmlDoc);
	if (hr != S_OK)
	{
		AfxMessageBox(L"Unable to create XML document");
		return false;
	}

	// This is a pointer we'll use for nodes in our XML
	CComPtr<IXMLDOMNode> node;

	// Create a processing instruction targeted for xml.
	// This creates the line:  <?xml version='1.0'>
	CComPtr<IXMLDOMProcessingInstruction> pi;
	xmlDoc->createProcessingInstruction(L"xml", L"version='1.0'", &pi);
	xmlDoc->appendChild(pi, &node);
	pi.Release();
	node.Release();

	// Create the root element
	CComPtr<IXMLDOMElement> pe;
	xmlDoc->createElement(L"movie", &pe);

	if (m_moviemake.IsOpen())
	{
		pe->setAttribute(L"output", CComVariant(m_moviemake.GetFilename()));
	}

	if (m_moviesource.IsOpen())
	{
		pe->setAttribute(L"source", CComVariant(m_moviesource.GetFilename()));
	}

	xmlDoc->appendChild(pe, &node);
	pe.Release();

	SaveMovieData(xmlDoc, node);

	node.Release();

	hr = xmlDoc->save(CComVariant(lpszPathName));
	xmlDoc.Release();


	return TRUE;
}

void CRotoScopeDoc::SaveMovieData(IXMLDOMDocument *xmlDoc, IXMLDOMNode *inNode)
{
	for (int frame = 0; frame < (int)m_draw.size(); frame++)
	{
		// Create an XML node for the frame
		CComPtr<IXMLDOMNode> node;
		CComPtr<IXMLDOMElement> pe;
		xmlDoc->createElement(L"frame", &pe);

		// Add an attribute for the frame number
		pe->setAttribute(L"num", CComVariant(frame));

		// Append the node to the node we are nested inside.
		inNode->appendChild(pe, &node);
		pe.Release();

		//
		// Now save the point data for the frame
		//

		for (std::list<CPoint>::iterator p = m_draw[frame].begin(); p != m_draw[frame].end(); p++)
		{
			// Create an XML node for the point
			xmlDoc->createElement(L"point", &pe);

			// Add attributes for the point
			pe->setAttribute(L"x", CComVariant(p->x));
			pe->setAttribute(L"y", CComVariant(p->y));

			// Append the node to the node we are nested inside.
			CComPtr<IXMLDOMNode> nodePoint;
			node->appendChild(pe, &nodePoint);
			pe.Release();
			nodePoint.Release();
		}

		// When done, release <frame> the node
		node.Release();
	}
}

BOOL CRotoScopeDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// Clear any previous frame information
	m_draw.clear();

	//
	// Create an XML document
	//

	CComPtr<IXMLDOMDocument>  pXMLDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument, (void**)&pXMLDoc));
	if (!succeeded)
	{
		AfxMessageBox(L"Failed to create an XML document to use");
		return FALSE;
	}

	// Open the XML document
	VARIANT_BOOL ok;
	succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(lpszPathName), &ok));
	if (!succeeded || ok == VARIANT_FALSE)
	{
		AfxMessageBox(L"Failed to open XML score file");
		return FALSE;
	}

	//
	// Traverse the XML document in memory!!!!
	// Top level tag is <score>
	//

	CComPtr<IXMLDOMNode> node;
	pXMLDoc->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);

		if (nodeName == L"movie")
		{
			XmlLoadMovie(node);
		}
	}


	return TRUE;
}

void CRotoScopeDoc::XmlLoadMovie(IXMLDOMNode *xml)
{
	// Handle the children of a <movie> tag
	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);

		if (nodeName == L"frame")
		{
			XmlLoadFrame(node);
		}
	}
}

void CRotoScopeDoc::XmlLoadFrame(IXMLDOMNode *xml)
{
	// When this function is called we have a new <frame> tag.
	// We assume we don't skip any tag numbers.
	// Push on an empty frame
	list<CPoint> empty;
	m_draw.push_back(empty);

	// Traverse the children of the <frame> tag
	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);

		// Handle finding a nested <point> tag
		if (nodeName == L"point")
		{
			CPoint point;

			// Get a list of all attribute nodes and the
			// length of that list
			CComPtr<IXMLDOMNamedNodeMap> attributes;
			node->get_attributes(&attributes);
			long len;
			attributes->get_length(&len);

			// Loop over the list of attributes
			for (int i = 0; i<len; i++)
			{
				// Get attribute i
				CComPtr<IXMLDOMNode> attrib;
				attributes->get_item(i, &attrib);

				// Get the name of the attribute
				CComBSTR name;
				attrib->get_nodeName(&name);

				// Get the value of the attribute.  A CComVariant is a variable
				// that can have any type. It loads the attribute value as a
				// string (UNICODE), but we can then change it to an integer 
				// (VT_I4) or double (VT_R8) using the ChangeType function 
				// and then read its integer or double value from a member variable.
				CComVariant value;
				attrib->get_nodeValue(&value);

				if (name == "x")
				{
					value.ChangeType(VT_I4);
					point.x = value.intVal;
				}
				else if (name == "y")
				{
					value.ChangeType(VT_I4);
					point.y = value.intVal;
				}
			}

			// When we've pulled the x,y values from the
			// tag, push it onto the end of our list of 
			// points.
			m_draw.back().push_back(point);
		}
	}
}


void CRotoScopeDoc::OnEditClearframe()
{
	if (m_movieframe >= 0 && m_movieframe < int(m_draw.size()))
	{
		m_draw[m_movieframe].clear();
		m_image = m_initial;
	}

	while (!m_images.empty())
	{
		m_image = m_images.top();
		m_images.pop();
	}

	DrawImage();
	UpdateAllViews(NULL);
}




void CRotoScopeDoc::DrawImage()
{
	CGrImage tempImage;
	tempImage = m_initial;
	if (m_greenScreenEnabled)
	{	
		CChromakey chromakey(0.0, 95, 1.1);
		CGrImage background;

		background.SetSameSize(m_initial);
		background.Fill(225, 0, 0);

		tempImage = chromakey.Apply(m_initial, background);
		
	}
	// Write image from tempImage into the current image
	for (int r = 0; r < m_image.GetHeight() && r < tempImage.GetHeight(); r++)
	{
		for (int c = 0; c < m_image.GetWidth() && c < tempImage.GetWidth(); c++)
		{
			m_image[r][c * 3] = tempImage[r][c * 3];
			m_image[r][c * 3 + 1] = tempImage[r][c * 3 + 1];
			m_image[r][c * 3 + 2] = tempImage[r][c * 3 + 2];
		}
	}

	// Write any saved drawings into the frame
	if (m_movieframe < (int)m_draw.size())
	{
		for (std::list<CPoint>::iterator i = m_draw[m_movieframe].begin();
			i != m_draw[m_movieframe].end(); i++)
		{
			for (int w = 0; w < m_width; w++)
			{
				m_image.Set(i->x + w, i->y, m_r, m_g, m_b);
			}
		}
	}

	// Add Mario sprite (or other specific drawings)
	/*
	for (int r = 0; r < m_mario.GetHeight(); r++)
	{
		for (int c = 0; c < m_mario.GetWidth(); c++)
		{
			if (m_mario[r][c * 4 + 3] >= 192) // Alpha channel check
			{
				m_image[r][c * 3] = m_mario[r][c * 4];
				m_image[r][c * 3 + 1] = m_mario[r][c * 4 + 1];
				m_image[r][c * 3 + 2] = m_mario[r][c * 4 + 2];
			}
		}
	}
	*/

	UpdateAllViews(NULL);
}

void CRotoScopeDoc::OnEditDrawline()
{
	if (m_dlg.DoModal() == IDOK)
	{
		m_x1 = m_dlg.m_x1;
		m_y1 = m_dlg.m_y1;
		m_x2 = m_dlg.m_x2;
		m_y2 = m_dlg.m_y2;
		m_b = m_dlg.m_b;
		m_g = m_dlg.m_g;
		m_r = m_dlg.m_r;
		m_width = m_dlg.m_width;

		DrawLine(m_image,m_x1, m_y1, m_x2, m_y2, m_width, m_b, m_g, m_r);
		UpdateAllViews(NULL);
	}
}


void CRotoScopeDoc::OnEditPlacebird()
{
	if (m_dlg.DoModal() == IDOK)
	{
		m_x1 = m_dlg.m_x1;
		m_y1 = m_dlg.m_y1;

		DrawBird(m_image, m_x1, m_y1);
		UpdateAllViews(NULL);
	}
}


void CRotoScopeDoc::OnEditPlacemario()
{
	// TODO: Add your command handler code here
	if (m_dlg.DoModal() == IDOK)
	{
		m_x1 = m_dlg.m_x1;
		m_y1 = m_dlg.m_y1;

		DrawMario(m_image, m_x1, m_y1);
		UpdateAllViews(NULL);
	}
}

void CRotoScopeDoc::OnEditPlaceaidan()
{
	if (m_armDlg.DoModal() == IDOK)
	{
		int x = m_armDlg.m_x;          // Shared pivot X-coordinate (center of the body)
		int y = m_armDlg.m_y;          // Shared pivot Y-coordinate (center of the body)
		double angle = m_armDlg.m_angle; // Rotation angle for the arm

		// Adjust these offsets to place the arm at the shoulder
		int offsetX = 37;  // Example X offset for shoulder
		int offsetY = 67;  // Example Y offset for shoulder

		// Clear the canvas
		m_image.Fill(0, 0, 0);

		// Draw the body at the pivot point
		DrawAidan(m_image, x, y);

		// Create the rotated arm
		CGrImage rotatedArm;
		RotateArm(angle, rotatedArm);

		// Overlay the rotated arm onto the canvas
		for (int r = 0; r < rotatedArm.GetHeight(); ++r)
		{
			for (int c = 0; c < rotatedArm.GetWidth(); ++c)
			{
				int armX = x + offsetX + c - rotatedArm.GetWidth() / 2; // Adjusted for shoulder
				int armY = y + offsetY + r - rotatedArm.GetHeight() / 2; // Adjusted for shoulder

				if (armY < m_image.GetHeight() && armX < m_image.GetWidth() && armY >= 0 && armX >= 0)
				{
					if (rotatedArm[r][c * 4 + 3] >= 192) // Non-transparent pixels
					{
						m_image[armY][armX * 3] = rotatedArm[r][c * 4];       // Red
						m_image[armY][armX * 3 + 1] = rotatedArm[r][c * 4 + 1]; // Green
						m_image[armY][armX * 3 + 2] = rotatedArm[r][c * 4 + 2]; // Blue
					}
				}
			}
		}

		// Refresh the view
		UpdateAllViews(NULL);
	}
}

void CRotoScopeDoc::RotateArm(double angle, CGrImage& rotatedArm)
{
	int rows = m_aidanArm.GetHeight();
	int cols = m_aidanArm.GetWidth();

	// Adjust these to the shoulder coordinates relative to the arm image
	int shoulderX = 37;   // X offset for shoulder within the arm image
	int shoulderY = 67;   // Y offset for shoulder within the arm image

	double radians = angle * 3.14159 / 180.0;  // Convert angle to radians

	// Initialize the rotated image with transparent background
	rotatedArm.SetSize(cols, rows, 4); // Ensure same size and alpha channel
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			rotatedArm[r][c * 4] = 0;       // Red
			rotatedArm[r][c * 4 + 1] = 0;   // Green
			rotatedArm[r][c * 4 + 2] = 0;   // Blue
			rotatedArm[r][c * 4 + 3] = 0;   // Alpha
		}
	}

	// Perform the rotation around the shoulder pivot
	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			int dx = c - shoulderX;
			int dy = r - shoulderY;

			// Apply rotation around the shoulder pivot
			int newX = static_cast<int>(cols / 2 + dx * cos(radians) - dy * sin(radians));
			int newY = static_cast<int>(rows / 2 + dx * sin(radians) + dy * cos(radians));

			// Copy pixel if within bounds
			if (newX >= 0 && newX < cols && newY >= 0 && newY < rows)
			{
				rotatedArm[newY][newX * 4] = m_aidanArm[r][c * 4];       // Red
				rotatedArm[newY][newX * 4 + 1] = m_aidanArm[r][c * 4 + 1]; // Green
				rotatedArm[newY][newX * 4 + 2] = m_aidanArm[r][c * 4 + 2]; // Blue
				rotatedArm[newY][newX * 4 + 3] = m_aidanArm[r][c * 4 + 3]; // Alpha
			}
		}
	}
}






void CRotoScopeDoc::DrawLine(CGrImage &image, int x1, int y1, int x2, int y2, int width, int b, int g, int r)
{
	std::list<CPoint> empty;
	while ((int)m_draw.size() < m_movieframe + 1)
		m_draw.push_back(empty);

	if (abs(x2 - x1) > abs(y2 - y1))
	{
		// Step in the X direction...
		if (x1 > x2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		if (x1 == x2)
		{
			for (int w = 0; w < width; w++)
			{
				//image.Set(x1 + w, y1, r, g, b);
				m_draw[m_movieframe].push_back(CPoint(x1 + w, y1));
			}
		}
		else
		{
			for (int x = x1; x <= x2; x++)
			{
				for (int w = 0; w < width; w++)
				{
					//image.Set(x, y1 + w + (x - x1) * (y2 - y1) / (x2 - x1), r, g, b);
					m_draw[m_movieframe].push_back(CPoint(x, y1 + w + (x - x1) * (y2 - y1) / (x2 - x1)));
				}
			}
		}
	}
	else
	{
		// Step in the Y direction...
		if (y1 > y2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		if (y1 == y2)
		{
			for (int w = 0; w < width; w++)
			{
				//image.Set(x1 + w, y1, r, g, b);
				m_draw[m_movieframe].push_back(CPoint(x1 + w, y1));
			}
		}
		else
		{
			for (int y = y1; y <= y2; y++)
			{
				for (int w = 0; w < width; w++)
				{
					//image.Set(x1 + w + (y - y1) * (x2 - x1) / (y2 - y1), y, r, g, b);
					m_draw[m_movieframe].push_back(CPoint(x1 + w + (y - y1) * (x2 - x1) / (y2 - y1), y));
				}
			}
		}
	}

	DrawImage();
}

//inputs: the reference image to roatate, angle theta to rotate in degrees
//converts input points to output points in the roatated image and fills in any holes created by the transformation
//a positive theta will create a clockwise rotation, a negative theta will create a counter clockwise rotation
void CRotoScopeDoc::RotateImage(CGrImage &image, int theta)
{
	m_images.push(m_image);

	int r0, c0; //center point
	int r1, c1; //output points in new image
	int rows, cols; //rows and cols of the input image
	rows = image.GetHeight();
	cols = image.GetWidth();
	CGrImage newImage; //image to transfer points to
	newImage.SetSameSize(image, 4); //alpha map for finding holes in new image

	float rads = (theta * 3.14159265) / 180.0; //convert degrees to radians

											   //find the center point
	r0 = rows / 2;
	c0 = cols / 2;

	//loop through input image
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			//calculate new points in rotated image from input image
			r1 = (int)(r0 + ((r - r0) * cos(rads)) - ((c - c0) * sin(rads)));
			c1 = (int)(c0 + ((r - r0) * sin(rads)) + ((c - c0) * cos(rads)));

			//put new points into the new image if they are not outside the bounds of the new image
			if (r1 > 0 && r1 < newImage.GetHeight() && c1 > 0 && c1 < newImage.GetWidth())
			{
				newImage[r1][c1 * 4] = image[r][c * 3];
				newImage[r1][c1 * 4 + 1] = image[r][c * 3 + 1];
				newImage[r1][c1 * 4 + 2] = image[r][c * 3 + 2];
				newImage[r1][c1 * 4 + 3] = 0; //mark point as not a hole
			}
		}
	}

	//look for holes in new image and fill them with the neighbor color
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			//a hole so add neighbor color
			if (newImage[r][c * 4 + 3] > 192)
			{
				m_image[r][(c)* 3] = newImage[r][(c + 1) * 4];
				m_image[r][(c)* 3 + 1] = newImage[r][(c + 1) * 4 + 1];
				m_image[r][(c)* 3 + 2] = newImage[r][(c + 1) * 4 + 2];
			}
			//not a hole add existing color
			else
			{
				m_image[r][(c)* 3] = newImage[r][(c)* 4];
				m_image[r][(c)* 3 + 1] = newImage[r][(c)* 4 + 1];
				m_image[r][(c)* 3 + 2] = newImage[r][(c)* 4 + 2];
			}
		}
	}
}

void CRotoScopeDoc::DrawBird(CGrImage &image, int x1, int y1)
{
	//allow undo of placing
	m_images.push(m_image);
	for (int r = 0; r<m_bird.GetHeight(); r++)
	{
		for (int c = 0; c<m_bird.GetWidth(); c++)
		{
			//make sure point is inside image
			if (r + y1 < m_image.GetHeight() && c + x1 < m_image.GetWidth())
			{
				if (m_bird[r][c * 4 + 3] >= 192)
				{
					m_image[r + y1][(c + x1) * 3] = m_bird[r][c * 4];
					m_image[r + y1][(c + x1) * 3 + 1] = m_bird[r][c * 4 + 1];
					m_image[r + y1][(c + x1) * 3 + 2] = m_bird[r][c * 4 + 2];
				}
			}
		}
	}
}

void CRotoScopeDoc::DrawFireworks(CGrImage& image, int x1, int y1)
{
	//allow undo of placing
	m_images.push(m_image);
	for (int r = 0; r < m_fireworks.GetHeight(); r++)
	{
		for (int c = 0; c < m_fireworks.GetWidth(); c++)
		{
			//make sure point is inside image
			if (r + y1 < m_image.GetHeight() && c + x1 < m_image.GetWidth())
			{
				if (m_fireworks[r][c * 4 + 3] >= 192)
				{
					m_image[r + y1][(c + x1) * 3] = m_fireworks[r][c * 4];
					m_image[r + y1][(c + x1) * 3 + 1] = m_fireworks[r][c * 4 + 1];
					m_image[r + y1][(c + x1) * 3 + 2] = m_fireworks[r][c * 4 + 2];
				}
			}
		}
	}
}

void CRotoScopeDoc::DrawAidan(CGrImage& image, int bodyX, int bodyY)
{
	// Draw the body (yourself)
	for (int r = 0; r < m_aidan.GetHeight(); ++r)
	{
		for (int c = 0; c < m_aidan.GetWidth(); ++c)
		{
			// Ensure we stay within the canvas bounds
			if (r + bodyY < image.GetHeight() && c + bodyX < image.GetWidth())
			{
				if (m_aidan[r][c * 4 + 3] >= 192) // Alpha check for non-transparency
				{
					image[r + bodyY][(c + bodyX) * 3] = m_aidan[r][c * 4];       // Red
					image[r + bodyY][(c + bodyX) * 3 + 1] = m_aidan[r][c * 4 + 1]; // Green
					image[r + bodyY][(c + bodyX) * 3 + 2] = m_aidan[r][c * 4 + 2]; // Blue
				}
			}
		}
	}

	//// Draw the arm (overlay directly on the body at its pre-aligned position)
	//for (int r = 0; r < m_aidanArm.GetHeight(); ++r)
	//{
	//	for (int c = 0; c < m_aidanArm.GetWidth(); ++c)
	//	{
	//		// Calculate the arm's position relative to the canvas
	//		int armX = c + bodyX;
	//		int armY = r + bodyY;

	//		// Ensure we stay within the canvas bounds
	//		if (armY < image.GetHeight() && armX < image.GetWidth())
	//		{
	//			if (m_aidanArm[r][c * 4 + 3] >= 192) // Alpha check for non-transparency
	//			{
	//				image[armY][armX * 3] = m_aidanArm[r][c * 4];       // Red
	//				image[armY][armX * 3 + 1] = m_aidanArm[r][c * 4 + 1]; // Green
	//				image[armY][armX * 3 + 2] = m_aidanArm[r][c * 4 + 2]; // Blue
	//			}
	//		}
	//	}
	//}
}






void CRotoScopeDoc::DrawJulia(CGrImage& image, int x1, int y1)
{
	//allow undo of placing
	m_images.push(m_image);
	for (int r = 0; r < m_julia.GetHeight(); r++)
	{
		for (int c = 0; c < m_julia.GetWidth(); c++)
		{
			//make sure point is inside image
			if (r + y1 < m_image.GetHeight() && c + x1 < m_image.GetWidth())
			{
				if (m_julia[r][c * 4 + 3] >= 192)
				{
					m_image[r + y1][(c + x1) * 3] = m_julia[r][c * 4];
					m_image[r + y1][(c + x1) * 3 + 1] = m_julia[r][c * 4 + 1];
					m_image[r + y1][(c + x1) * 3 + 2] = m_julia[r][c * 4 + 2];
				}
			}
		}
	}
}

void CRotoScopeDoc::DrawMario(CGrImage& image, int x1, int y1)
{
	// Allow undo of placing
	m_images.push(m_image);

	CGrImage* imageToUse;

	if (m_morphEnabled) {
		// Morphing is enabled: create a morphed image
		CMorph morph;
		static double alpha = 0.5; // Adjust alpha as needed
		imageToUse = new CGrImage(morph.MorphImages(m_aidan, m_mario, alpha));
	}
	else {
		// Use the standard Mario image
		imageToUse = &m_mario;
	}

	// Overlay the selected image onto m_image
	for (int r = 0; r < imageToUse->GetHeight(); r++) {
		for (int c = 0; c < imageToUse->GetWidth(); c++) {
			// Ensure the pixel is within bounds of the main image
			if (r + y1 < m_image.GetHeight() && c + x1 < m_image.GetWidth()) {
				if ((*imageToUse)[r][c * 4 + 3] >= 192) {
					m_image[r + y1][(c + x1) * 3] = (*imageToUse)[r][c * 4];
					m_image[r + y1][(c + x1) * 3 + 1] = (*imageToUse)[r][c * 4 + 1];
					m_image[r + y1][(c + x1) * 3 + 2] = (*imageToUse)[r][c * 4 + 2];
				}
			}
		}
	}

	// If we allocated a new image, delete it to avoid memory leaks
	if (m_morphEnabled) {
		delete imageToUse;
	}

	// Update the view to reflect changes
	UpdateAllViews(NULL);
}
void CRotoScopeDoc::DrawTrevor(CGrImage& image, int x1, int y1)
{
	//allow undo of placing
	m_images.push(m_image);
	for (int r = 0; r < m_trevor.GetHeight(); r++)
	{
		for (int c = 0; c < m_trevor.GetWidth(); c++)
		{
			//make sure point is inside image
			if (r + y1 < m_image.GetHeight() && c + x1 < m_image.GetWidth())
			{
				if (m_trevor[r][c * 4 + 3] >= 192)
				{
					m_image[r + y1][(c + x1) * 3] = m_trevor[r][c * 4];
					m_image[r + y1][(c + x1) * 3 + 1] = m_trevor[r][c * 4 + 1];
					m_image[r + y1][(c + x1) * 3 + 2] = m_trevor[r][c * 4 + 2];
				}
			}
		}
	}
}

void CRotoScopeDoc::DrawGreg(CGrImage& image, int x1, int y1)
{
	//allow undo of placing
	m_images.push(m_image);
	for (int r = 0; r < m_greg.GetHeight(); r++)
	{
		for (int c = 0; c < m_greg.GetWidth(); c++)
		{
			//make sure point is inside image
			if (r + y1 < m_image.GetHeight() && c + x1 < m_image.GetWidth())
			{
				if (m_greg[r][c * 4 + 3] >= 192)
				{
					m_image[r + y1][(c + x1) * 3] = m_greg[r][c * 4];
					m_image[r + y1][(c + x1) * 3 + 1] = m_greg[r][c * 4 + 1];
					m_image[r + y1][(c + x1) * 3 + 2] = m_greg[r][c * 4 + 2];
				}
			}
		}
	}
}

void CRotoScopeDoc::OnEditRotateimage()
{
	if (m_dlg.DoModal() == IDOK)
	{
		m_theta = m_dlg.m_x1;

		RotateImage(m_image, m_theta);
		UpdateAllViews(NULL);
	}
}


void CRotoScopeDoc::OnEditSetvariables()
{
	if (m_dlg.DoModal() == IDOK)
	{
		m_x1 = m_dlg.m_x1;
		m_y1 = m_dlg.m_y1;
		m_x2 = m_dlg.m_x2;
		m_y2 = m_dlg.m_y2;
		m_b = m_dlg.m_b;
		m_g = m_dlg.m_g;
		m_r = m_dlg.m_r;
		m_width = m_dlg.m_width;

		UpdateAllViews(NULL);
	}
}


void CRotoScopeDoc::OnMousemodePen()
{
	m_mode = 0;
}


void CRotoScopeDoc::OnMousemodeLine()
{
	m_mode = 1;
}


void CRotoScopeDoc::OnMousemodeBird()
{
	m_mode = 2;
}

void CRotoScopeDoc::OnMousemodeAidan()
{
	m_mode = 3;
}

void CRotoScopeDoc::OnMousemodeMario()
{
	m_mode = 4;
}

void CRotoScopeDoc::OnMousemodeJulia()
{
	m_mode = 5;
}

void CRotoScopeDoc::OnMousemodeTrevor()
{
	m_mode = 6;
}


void CRotoScopeDoc::OnEditUndo32793()
{
	if (!m_images.empty())
	{
		m_image = m_images.top();
		m_images.pop();
		UpdateAllViews(NULL);
	}
}

void CRotoScopeDoc::OnMousemodeGreg()
{
	m_mode = 7;
}


void CRotoScopeDoc::OnEditMorph()
{
	if (m_morphEnabled) {
		m_morphEnabled = false;
	}
	else {
		m_morphEnabled = true;
	}
}

void CRotoScopeDoc::OnUpdateEditMorph(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_morphEnabled);
}


void CRotoScopeDoc::OnEditGreenscreen()
{
	if (m_greenScreenEnabled) {
		m_greenScreenEnabled = false;
	}
	else {
		m_greenScreenEnabled = true;
	}
}


void CRotoScopeDoc::OnUpdateEditGreenscreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_greenScreenEnabled);
}

void CRotoScopeDoc::RecolorRedToBlue(CGrImage& inputImage, CGrImage& outputImage)
{
	int redThresholdLow = 100;
	int redThresholdHigh = 255;
	int greenThresholdMax = 100;
	int blueThresholdMax = 100;

	outputImage.SetSameSize(inputImage);
	for (int y = 0; y < inputImage.GetHeight(); ++y)
	{
		for (int x = 0; x < inputImage.GetWidth(); ++x)
		{
			int r = inputImage[y][x * 3];
			int g = inputImage[y][x * 3 + 1];
			int b = inputImage[y][x * 3 + 2];

			if (r >= redThresholdLow && r <= redThresholdHigh && g <= greenThresholdMax && b <= blueThresholdMax)
			{
				int newR = (std::max)(0, r - 50);
				int newB = (std::min)(255, r + 50);
				outputImage.Set(x, y, newR, g, newB);
			}
			else
			{
				outputImage.Set(x, y, r, g, b);
			}
		}
	}
}



void CRotoScopeDoc::OnMousemodeRecolor()
{
	CGrImage originalImage = m_image;
	CGrImage recoloredImage;

	RecolorRedToBlue(originalImage, recoloredImage);

	m_image = recoloredImage;

	UpdateAllViews(NULL);
}

void CRotoScopeDoc::ApplyWaveEffect()
{
	if (!m_waveEnabled) {
		return; // If the flag is false, don't apply the wave effect
	}
	CGrImage tempImage;
	tempImage.SetSameSize(m_image);

	double waveAmplitude = 4.0;
	double waveFrequency = 0.1;

	for (int y = 0; y < m_image.GetHeight(); y++)
	{
		for (int x = 0; x < m_image.GetWidth(); x++)
		{
			int newX = x + waveAmplitude * sin(y * waveFrequency);
			int newY = y + waveAmplitude * cos(x * waveFrequency);

			newX = max(0, min(m_image.GetWidth() - 1, newX));
			newY = max(0, min(m_image.GetHeight() - 1, newY));

			tempImage[y][x * 3] = m_image[newY][newX * 3];
			tempImage[y][x * 3 + 1] = m_image[newY][newX * 3 + 1];
			tempImage[y][x * 3 + 2] = m_image[newY][newX * 3 + 2];
		}
	}

	m_image = tempImage;
	UpdateAllViews(NULL);
}


void CRotoScopeDoc::OnMousemodeApplywavewarp()
{
	if (m_waveEnabled) {
		m_waveEnabled = false;
		m_image = m_image;
		UpdateAllViews(NULL);
	}
	else {
		m_waveEnabled = true;
		m_tempImage = m_image;
		ApplyWaveEffect();
	}
}

void CRotoScopeDoc::OnUpdateMousemodeApplywavewarp(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_waveEnabled);
}
